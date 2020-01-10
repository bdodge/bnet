/*
 * Copyright 2019 Brian Dodge
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "bupnp.h"
#include "bxml.h"

int upnp_handle_notify(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    upnp_subscription_t *sub;
    int result;

    sub = (upnp_subscription_t *)resource->priv;
    if (! sub)
    {
        return -1;
    }

    result = 0;

    butil_log(5, "UPnP Notifier %d %s to %s\n", cbtype, http_method_name(client->method), client->path);

    switch (cbtype)
    {
    case httpRequest:
        result |= http_append_request(client, "NT: upnp:event");
        result |= http_append_request(client, "NTS: upnp:propchange");
        result |= http_append_request(client, "SID: uuid:%s", sub->sid);
        result |= http_append_request(client, "SEQ: %u", sub->seq++);
        result |= http_append_request(client, "Cache-Control: no-cache");
        break;

    case httpComplete:
        if (resource->resource.canned_data.content != NULL)
        {
            free((char*)resource->resource.canned_data.content);
            resource->resource.canned_data.content = NULL;
        }
        break;

    default:
        break;
    }

    return http_canned_callback(client, resource, cbtype, data, count);
}

int upnp_subscribe(
                        upnp_server_t       *server,
                        upnp_subscription_t *sub_desc,
                        uint16_t            *ecode
                        )
{
    upnp_subscription_t *sub;
    time_t now;
    int result;

    if (! server || ! sub_desc)
    {
        return -1;
    }

    time(&now);
    *ecode = 500;

    // find subscription in database if there is an id assoctiated
    //
    if (sub_desc->sid[0] != '\0')
    {
        for (sub = server->subscriptions; sub; sub = sub->next)
        {
            if (! strcmp(sub->sid, sub_desc->sid))
            {
                break;
            }
        }

        if (! sub)
        {
            butil_log(1, "No subscription %s in database\n", sub_desc->sid);
            *ecode = 404;
            return -1;
        }

        // update current subscription with new timeout
        //
        sub->expiry = sub_desc->expiry;

        butil_log(3, "Updated subscription from %s expiring +%u seconds\n",
                            sub->loc, sub->expiry - now);
        *ecode = 200;
    }
    else
    {
        // create a new subscription based on this description
        //
        sub = server->subs_free;
        if (! sub)
        {
            UPNP_ERROR("Cant alloc subscription");
            return -1;
        }

        // alloc a subscription record from pool to use as http data context
        //
        server->subs_free = sub->next;

        // SID is uuid with lower bytes the subscription ptr (but not used as such!)
        //
        sub->service = sub_desc->service;
        uuid_format(sub->service->usn, sub->sid, sizeof(sub->sid));
        snprintf(sub->sid + strlen(sub->sid) - 12, 15, "beef%06x", (uint32_t)(uintptr_t)sub);

        // return sid in the descriptor
        strncpy(sub_desc->sid, sub->sid, sizeof(sub_desc->sid) - 1);

        sub->next = server->subscriptions;
        server->subscriptions = sub;
        strncpy(sub->loc, sub_desc->loc, sizeof(sub->loc) - 1);
        sub->expiry = sub_desc->expiry;
        sub->seq = 0;
        sub->dirty = true;

        // mark all state variables dirty so they are included in the first
        // event notification triggered by sub->dirty
        //
        upnp_mark_var_dirty(sub->service, NULL, true);

        butil_log(3, "Created subscription from %s expiring +%u seconds =%s=\n",
                            sub->loc, sub->expiry - now, sub->sid);

        *ecode = 200;
    }
    return 0;
}

int upnp_unsubscribe(
                        upnp_server_t       *server,
                        upnp_subscription_t *sub_desc,
                        uint16_t            *ecode
                        )
{
    upnp_subscription_t *sub;
    upnp_subscription_t *prevsub;

    prevsub = NULL;

    for (sub = server->subscriptions; sub; sub = sub->next)
    {
        if (! strcmp(sub->sid, sub_desc->sid))
        {
            break;
        }

        prevsub = sub;
    }

    if (sub)
    {
        if (sub == server->subscriptions)
        {
            server->subscriptions = sub->next;
        }
        else if (prevsub)
        {
            prevsub->next = sub->next;
        }
        else
        {
            butil_log(0, "sublist corrupt\n");
        }

        // note that the sub stays intact here in case it is actively being
        // used to notify while being unsubscribed to.. it just goes to the
        // free list to be reused later
        //
        sub->next = server->subs_free;
        server->subs_free = sub;
    }

    return 0;
}

int upnp_notify_subscribers(
                        struct upnp_server  *server,
                        upnp_service_t      *service
                        )
{
    upnp_subscription_t *sub;
    http_resource_t *resource;
    upnp_var_t *var;
    ioring_t reply;
    size_t made;
    int result;

    if (! server || ! service)
    {
        return -1;
    }

    if (! server->event_http_client)
    {
        return 0;
    }

    if (server->event_http_client->state != httpDone)
    {
        return 0;
    }

    for (sub = server->subscriptions; sub; sub = sub->next)
    {
        if (sub->service == service)
        {
            break;
        }
    }

    reply.size = UPNP_MAX_SOAP;
    reply.data = (uint8_t *)malloc(reply.size);
    if (! reply.data)
    {
        return -1;
    }
    // format reply xml
    //
    iostream_reset_ring(&reply);
#if 0
    bxml_format_header(reply.data, reply.size, &made, "1.0", "utf-8");
    reply.head += made;
    reply.count += made;
#endif
    result = bxml_format_element(
                        reply.data + reply.head,
                        reply.size - reply.count,
                        &made,
                        false,
                        "e:propertyset",
                        NULL,
                        1,
                        "xmlns:e", "urn:schemas-upnp-org:event-1-0"
                      );
    reply.head += made;
    reply.count += made;

    // for each dirty state var in service, add value property
    //
    for (var = service->state_vars; var; var = var->next)
    {
        if (upnp_get_var_dirty(var))
        {
            char varvalue[256];

            result = upnp_get_val_value_as_string(&var->val, varvalue, sizeof(varvalue), NULL, NULL);

            if (! result)
            {
                result = bxml_format_element(
                                    reply.data + reply.head,
                                    reply.size - reply.count,
                                    &made,
                                    false,
                                    "e:property",
                                    NULL,
                                    0
                                  );
                reply.head += made;
                reply.count += made;

                result = bxml_format_element(
                                    reply.data + reply.head,
                                    reply.size - reply.count,
                                    &made,
                                    false,
                                    var->name,
                                    var->val.type == upnp_dt_string ? "" : "0",
                                    0
                                  );
                reply.head += made;
                reply.count += made;

                result = bxml_format_endtag(
                                    reply.data + reply.head,
                                    reply.size - reply.count,
                                    &made,
                                    "e:property"
                                  );
                reply.head += made;
                reply.count += made;

            }
        }
    }

    result = bxml_format_endtag(
                        reply.data + reply.head,
                        reply.size - reply.count,
                        &made,
                        "e:propertyset"
                      );
    reply.head += made;
    reply.count += made;

    butil_log(3, "NOTIFY=\n%s\n=\n", reply.data);

    // setup the event resource to pull reply buffer
    //
    resource = server->event_resources;
    resource->priv = sub;

    resource->resource.canned_data.content = reply.data;
    resource->resource.canned_data.count   = reply.count;
    resource->resource.canned_data.content_type = butil_mime_xml;

    result = http_client_request(
                        server->event_http_client,
                        s_method_NOTIFY,
                        sub->loc,
                        httpTCP,
                        false,
                        NULL,
                        server->event_resources
                       );
    return result;
}

