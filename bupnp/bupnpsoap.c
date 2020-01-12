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

typedef struct soap_context
{
    uint16_t ecode;
    char     soap_header[UPNP_MAX_URL];
    ioring_t soap;
    uint8_t  soap_buffer[UPNP_MAX_SOAP];
}
soap_context_t;

int upnp_get_arg_value(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        upnp_val_t **pval
                      )
{
    upnp_action_t *action;
    upnp_arglist_t *arg;

    if (! service || ! action_name || ! arg_name || ! pval)
    {
        return -1;
    }
    *pval = NULL;

    action = upnp_action_from_name(service, action_name);
    if (! action)
    {
        return -1;
    }

    arg = upnp_arg_from_name(action, arg_name);
    if (! arg)
    {
        return -1;
    }

    if (! arg->isset)
    {
        butil_log(4, "Argument %s not set in call %s\n",
                    arg_name, action_name);
        return -1;
    }

    *pval = &arg->val;
    return 0;
}

int upnp_get_arg_value_as_string(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        char *buf,
                        size_t nbuf,
                        const char **value,
                        size_t *slen
                      )
{
    upnp_val_t *val;
    int result;

    result = upnp_get_arg_value(service, action_name, arg_name, &val);
    if (result)
    {
        return result;
    }

    return upnp_get_val_value_as_string(val, buf, nbuf, value, slen);
}

int upnp_get_arg_value_as_int(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        int *pval
                      )
{
    upnp_val_t *val;
    int result;

    if (! pval)
    {
        return -1;
    }
    *pval = 0;

    result = upnp_get_arg_value(service, action_name, arg_name, &val);
    if (result)
    {
        return result;
    }

    return upnp_get_val_value_as_int(val, pval);
}

int upnp_set_arg_value_from_string(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        const char *sval
                      )
{
    upnp_action_t *action;
    upnp_arglist_t *arg;

    if (! service || ! action_name || ! arg_name)
    {
        return -1;
    }

    action = upnp_action_from_name(service, action_name);
    if (! action)
    {
        return -1;
    }

    arg = upnp_arg_from_name(action, arg_name);
    if (! arg)
    {
        return -1;
    }

    arg->isset = true;

    return upnp_set_val_value_from_string(&arg->val, sval);
}

int upnp_set_arg_value_from_int(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        int ival
                      )
{
    upnp_action_t *action;
    upnp_arglist_t *arg;

    if (! service || ! action_name || ! arg_name)
    {
        return -1;
    }

    action = upnp_action_from_name(service, action_name);
    if (! action)
    {
        return -1;
    }

    arg = upnp_arg_from_name(action, arg_name);
    if (! arg)
    {
        return -1;
    }

    arg->isset = true;

    return upnp_set_val_value_from_int(&arg->val, ival);
}

int upnp_dispatch_soap(upnp_server_t *server, upnp_service_t *service, ioring_t *soap, const char *action_name)
{
    bxml_parser_t body_parser;
    bxml_parser_t *pxpb;
    bxml_parser_t action_parser;
    bxml_parser_t *pxpa;
    upnp_action_t *action;
    upnp_arglist_t *arg;
    char argval[256];
    size_t made;
    const char *ptag;

    int result;

    if (! server || ! service || ! action_name)
    {
        return -1;
    }
    butil_log(5, "Action %s on service %s\n", action_name, service->usn);

    pxpb = bxml_parser_create(&body_parser, (char*)soap->data);
    if (! pxpb)
    {
        butil_log(1, "Can't make soap xml parser\n");
        return -2;
    }

    // locate the element "Envelope.Body.<action>" where action is the SOAP action
    //
    result = bxml_find_element(
                                pxpb,
                                "Envelope.Body", '.',
                                0,
                                &ptag
                            );
    if (! result)
    {
        result = bxml_find_next_element(pxpb, action_name, '.', 0, &ptag);
    }

    if (result)
    {
        bxml_parser_destroy(pxpb);
        return result;
    }

    // lookup the action in the service
    //
    action = upnp_action_from_name(service, action_name);

    if (! action)
    {
        butil_log(3, "No action %s in service %s\n", action_name, service->usn);
        bxml_parser_destroy(pxpb);
        return 0; //?;
    }

    // xml sub-parser on the action parameters
    //
    pxpa = bxml_parser_create(&action_parser, ptag);

    for (arg = action->args; arg; arg = arg->next)
    {
        arg->isset = false;

        if (! arg->inOUT)
        {
            bool is_string_type;

            // for each action argument, extract arg from action element
            // for non-string type.  string types just reference the
            // positon in the soap-buffer where the value starts
            //
            if (! arg->var)
            {
                butil_log(3, "Arg %s has no var to give type info, assume string\n", arg->name);
                arg->val.type = upnp_dt_string;
                is_string_type = true;
            }
            else
            {
                // inherit argument value type from relatedstatevar type
                //
                is_string_type = upnp_type_is_string_type(arg->val.type);
            }

            if (! is_string_type || ! arg->istype)
            {
                result = bxml_find_and_copy_element(
                                                pxpa,
                                                arg->name, '.',
                                                0,
                                                argval,
                                                sizeof(argval),
                                                false,
                                                false
                                                );
                upnp_set_val_value_from_string(&arg->val, argval);

                // if the argument has a related state variable that is NOT
                // just a type-def (A_ARG_), then also set the state variable
                // (can this ever happen?) Note: large string arguments are
                // not possible here, perhaps allocate argval for length?
                //
                if (arg->var && ! arg->istype)
                {
                    butil_log(4, "Setting statevar %s from arg\n", arg->var->name);

                    result = upnp_set_var_value_from_string(arg->var, argval);
                }
            }
            else
            {
                // just reference the string in the incoming soap
                // to avoid large allocs or buffering
                //
                result = bxml_find_element(
                                                pxpa,
                                                arg->name, '.',
                                                0,
                                                &ptag
                                         );
                if (! result)
                {
                    result = bxml_parse_value(
                                                pxpa,
                                                ptag,
                                                NULL, 0, NULL,
                                                (const char **)&arg->val.value.sval,
                                                &arg->val.slen
                                            );
                    if (result)
                    {
                        arg->val.value.sval = NULL;
                        arg->val.alloclen = 0;
                        arg->val.slen = 0;
                    }
                }
            }
            if (! result)
            {
                if (butil_get_log_level() >= 5)
                {
                    if (is_string_type)
                    {
                        int len = arg->val.slen;

                        if (len > sizeof(argval) - 4)
                        {
                            len = sizeof(argval) - 4;
                            strcpy(argval + sizeof(argval) - 4, "...");
                        }
                        memcpy(argval, arg->val.value.sval, len);
                        argval[sizeof(argval) - 1] = '\0';

                        butil_log(5, "Arg %s value=%s=\n", arg->name, argval);
                    }
                    else
                    {
                        butil_log(5, "Arg %s value=%s=\n", arg->name, argval);
                    }
                }
                arg->isset = true;
            }
            else
            {
                butil_log(4, "Argument %s missing in action %s\n", arg->name, action_name);
            }
        }
    }

    bxml_parser_destroy(pxpa);
    bxml_parser_destroy(pxpb);

    // if the service has a callback, do that
    //
    result = service->callback(server, service, action_name);

    // format reply xml
    //
    iostream_reset_ring(soap);

    bxml_format_header(soap->data, soap->size, &made, "1.0", "utf-8");
    soap->head += made;
    soap->count += made;

    result = bxml_format_element(
                        soap->data + soap->head,
                        soap->size - soap->count,
                        &made,
                        false,
                        "s:Envelope",
                        NULL,
                        2,
                        "s:encodingStyle",
                        "http://schemas.xmlsoap.org/soap/encoding/",
                        "xmlns:s",
                        "http://schemas.xmlsoap.org/soap/envelope/"
                      );
    soap->head += made;
    soap->count += made;

    result = bxml_format_element(
                        soap->data + soap->head,
                        soap->size - soap->count,
                        &made,
                        false,
                        "s:Body",
                        NULL,
                        0
                      );
    soap->head += made;
    soap->count += made;

    snprintf(argval, sizeof(argval), "u:%sResponse", action_name);
    result = bxml_format_element(
                        soap->data + soap->head,
                        soap->size - soap->count,
                        &made,
                        false,
                        argval,
                        NULL,
                        1,
                        "xmlns:u", "urn:schemas-upnp-org:service:ContentDirectory:1"
                      );
    soap->head += made;
    soap->count += made;

    // for each set out argument, add it and its value to the body
    //
    for (arg = action->args; arg; arg = arg->next)
    {
        if (arg->inOUT && arg->isset)
        {
            size_t slen;

            // for arguments related to state vars, get state var value
            // else get arg value
            //
            if (arg->istype)
            {
                result = upnp_get_val_value_as_string(&arg->val, argval, sizeof(argval), &ptag, &slen);
            }
            else
            {
                result = upnp_get_var_value_as_string(arg->var, argval, sizeof(argval), &ptag, &slen);
            }
            if (! result)
            {
                result = bxml_format_element(
                                    soap->data + soap->head,
                                    soap->size - soap->count,
                                    &made,
                                    false,
                                    arg->name,
                                    ptag,
                                    0
                                  );
                soap->head += made;
                soap->count += made;
            }
        }
    }

    snprintf(argval, sizeof(argval), "u:%sResponse", action_name);
    result = bxml_format_endtag(
                        soap->data + soap->head,
                        soap->size - soap->count,
                        &made,
                        argval
                      );
    soap->head += made;
    soap->count += made;

    result = bxml_format_endtag(
                        soap->data + soap->head,
                        soap->size - soap->count,
                        &made,
                        "s:Body"
                      );
    soap->head += made;
    soap->count += made;

    result = bxml_format_endtag(
                        soap->data + soap->head,
                        soap->size - soap->count,
                        &made,
                        "s:Envelope"
                      );
    soap->head += made;
    soap->count += made;

    //butil_log(3, "REPLY=\n%s\n=\n", soap->data);

    return result;
}

int upnp_handle_control_url(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                        )
{
    soap_context_t *context;
    upnp_server_t *server;
    upnp_device_t *device;
    upnp_service_t *service;
    char *header;
    char *value;
    char  usn[UPNP_MAX_URL];
    char *pusn;
    char *pact;
    char *psrc;
    int len;
    int moved;
    int result;

    server = (upnp_server_t *)resource->priv;
    if (! server)
    {
        return -1;
    }

    context = (soap_context_t *)client->ctxpriv;
    if (! context)
    {
        context = (soap_context_t *)malloc(sizeof(soap_context_t));
        if (! context)
        {
            UPNP_ERROR("Cant alloc soap context");
            return -1;
        }

        context->ecode = 200;
        context->soap.size = UPNP_MAX_SOAP;
        context->soap.data = context->soap_buffer;
        iostream_reset_ring(&context->soap);

        context->soap_header[0] = '\0';
        client->ctxpriv = context;
    }

    result = 0;

    butil_log(5, "ControlCB %d %s to %s\n", cbtype, http_method_name(client->method), client->path);

    switch (cbtype)
    {
    case httpRequest:

        client->keepalive = false;

        if (client->method == httpPost)
        {
            butil_log(4, "POST %s %s\n", client->path, context->soap_header);
            iostream_reset_ring(&context->soap);
            return 0;
        }
        else
        {
            butil_log(1, "Can't do method \"%s\" on UPnP\n",
                   http_method_name(client->method));
            return -1;
        }
        break;

    case httpRequestHeader:

        header = (char*)*data;
        value = header;
        if (! header)
        {
            break;
        }

        while (*value != ' ' && *value != '\t' && *value)
        {
            value++;
        }

        while (*value == ' ' || *value == '\t')
        {
            value++;
        }

        if (! http_ncasecmp(header, "soapaction:"))
        {
            // save action header,
            strncpy(context->soap_header, value, sizeof(context->soap_header) - 1);
            context->soap_header[sizeof(context->soap_header) - 1] = '\0';
            break;
        }
        break;

    case httpDownloadData:

        // copy data from http client to our ring buffer
        //
        {
            size_t ndata = *count;
            size_t room  = context->soap.size - context->soap.count - 1;
            uint8_t *input = (data != NULL) ? *data : NULL;

            if (input && ndata)
            {
                if (ndata > room)
                {
                    butil_log(2, "Truncating soap\n");
                    ndata = room;
                }
                memcpy(context->soap.data + context->soap.head, input, ndata);
                context->soap.head += ndata;
                context->soap.count += ndata;
                context->soap.data[context->soap.head] = '\0';
            }
            butil_log(6, "Got %u soap, for %u tot\n", ndata, context->soap.count);
        }
        break;

    case httpDownloadDone:

        butil_log(4, "ControlCB %d: SOAP %s body of %u bytes\n", client->id, context->soap_header, context->soap.count);

        // figure out which service this control is intended for by comparing usn's
        //
        pusn = context->soap_header;

        // incoming usn is between " and #, so extract it
        if (*pusn == '\"')
        {
            pusn++;
        }

        for (psrc = pusn; *psrc && *psrc != '#';)
        {
            psrc++;
        }

        if (*psrc == '#')
        {
            *psrc++ = '\0';
        }
        else
        {
            butil_log(3, "No action for SOAP\n");
            break;
        }

        // action is between # and " so extract that
        //
        pact = psrc;
        while (*psrc && *psrc != '\"')
        {
            psrc++;
        }
        *psrc = '\0';

        service = NULL;
        for (device = server->root_device; device && ! service; device = device->next)
        {
            for (service = device->services; service; service = service->next)
            {
                snprintf(usn, sizeof(usn), "urn:schemas-upnp-org:service:%s:", service->upnp_service_type);

                // compare to search target
                //
                butil_log(3, "Compare =%s= to our =%s=\n", pusn, usn);

                if (! http_ncasecmp(pusn, usn))
                {
                    // partially matched a service, so check our version is >= the req ver
                    //
                    uint32_t theirver = strtoul(pusn + strlen(usn), NULL, 0);

                    if (service->upnp_service_version >= theirver)
                    {
                        butil_log(3, "Control target matches %s\n", usn);
                        break;
                    }
                }
            }
        }
        if (! service)
        {
            butil_log(4, "No service \"%s\" on any device\n", usn);
            break;
        }
        context->soap_header[0] = '\0';

        // extract parameters and call the service's callback with the action
        // if there's a reply, set the content length and upload it next
        //
        result = upnp_dispatch_soap(server, service, &context->soap, pact);

        butil_log(3, "ControlCB %d: Processed %s with result %d, reply of %u bytes\n",
                    client->id, context->soap_header, result, context->soap.count);

        // if there is anything in the soap buffer, assume its a reply
        //
        if (! result)
        {
            if (context->soap.count > 0)
            {
                context->soap.tail = 0;
                client->out_content_length = context->soap.count;
                client->out_content_type   = butil_mime_xml;
            }
            else
            {
                client->out_content_length = 0;
            }
            // do the reply headers ourselves here, as they're a bit different from
            // the http defaults
            //
            result = http_begin_reply(client, 200, "OK");
            result |= http_append_reply(client, "Content-Type: %s", butil_mime_string_for_content_type(client->out_content_type));
            result |= http_append_reply(client, "Content-Length: %d", client->out_content_length);
            result |= http_append_reply(client, "Connection: close");
            result |= http_append_reply(client, "Ext: ");
            result |= http_append_reply(client, "Server: bnet/1.0 UPnP/1.0 bnet/1.0");
            result |= http_append_reply(client, "");
            client->state = httpBodyUpload; // this bypasses http system building its own reply
        }
        break;

    case httpUploadData:

        moved = *count; // what client has space for
        if (moved > context->soap.count)
        {
            moved = context->soap.count;
        }

        butil_log(5, "Move %d (can take %d) at off %d\n", moved, *count, context->soap.tail);
        memcpy(*data, context->soap.data + context->soap.tail, moved);
        context->soap.tail += moved;
        context->soap.count -= moved;
        *count = moved;
        break;

    case httpComplete:
        if (context)
        {
            free(context);
        }
        client->ctxpriv = NULL;
        break;

    default:
        return -1;
    }
    return result;
}

int upnp_handle_event_url(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    upnp_server_t *server;
    upnp_device_t *device;
    upnp_service_t *service;
    upnp_subscription_t *sub;
    char *header;
    char *value;
    uint16_t ecode;
    int result;

    server = (upnp_server_t *)resource->priv;
    if (! server)
    {
        return -1;
    }

    sub = (upnp_subscription_t *)client->ctxpriv;
    if (! sub)
    {
        sub = server->subs_free;
        if (! sub)
        {
            UPNP_ERROR("Cant alloc subscription");
            // TODO - format error reply
            return -1;
        }

        // alloc a subscription record from pool to use as http data context
        //
        server->subs_free = sub->next;

        sub->next = NULL;
        sub->expiry = 0;
        sub->loc[0] = '\0';
        sub->sid[0] = '\0';
        sub->seq = 0;

        client->ctxpriv = sub;
    }

    result = 0;

    butil_log(5, "EventCB %d %s to %s\n", cbtype, http_method_name(client->method), client->path);

    switch (cbtype)
    {
    case httpRequest:

        client->keepalive = false;
        client->out_content_length = 0;

        break;

    case httpRequestHeader:

        header = (char*)*data;
        value = header;
        if (! header)
        {
            break;
        }

        while (*value != ' ' && *value != '\t' && *value)
        {
            value++;
        }

        while (*value == ' ' || *value == '\t')
        {
            value++;
        }

        if (! http_ncasecmp(header, "callback"))
        {
            if (value[0] == '<')
            {
                int vlen;

                value++;
                vlen = strlen(value);
                if (vlen > 1)
                {
                    if (value[vlen - 1] == '>')
                    {
                        value[vlen - 1] = '\0';
                    }
                }
            }
            strncpy(sub->loc, value, sizeof(sub->loc) - 1);
            sub->loc[sizeof(sub->loc) - 1] = '\0';
            break;
        }
        else if (! http_ncasecmp(header, "sid"))
        {
            if (! http_ncasecmp(value, "uuid:"))
            {
                value += 5;
            }
            strncpy(sub->sid, value, sizeof(sub->sid) - 1);
            sub->sid[sizeof(sub->sid) - 1] = '\0';
            break;
        }
        else if (! http_ncasecmp(header, "timeout"))
        {
            if (! http_ncasecmp(value, "second-"))
            {
                value += strlen("second-");
            }
            sub->expiry = time(NULL) + strtoul(value, NULL, 0);
            break;
        }
        break;

    case httpDownloadData:
        break;

    case httpDownloadDone:

        service = NULL;
        for (device = server->root_device; device && ! service; device = device->next)
        {
            for (service = device->services; service; service = service->next)
            {
                butil_log(6, "Compare =%s= to our =%s=\n", client->path, service->event_url);

                if (! strcmp(client->path, service->event_url))
                {
                    break;
                }
            }
        }
        if (! service)
        {
            butil_log(4, "No service with url \"%s\" on any device\n", client->path);
            ecode = 404;
            result = -1;
        }
        else
        {
            sub->service = service;

            if (client->method == s_method_SUBSCRIBE)
            {
                butil_log(4, "SUBSCRIBE %s\n", client->path);
                result = upnp_subscribe(server, sub, &ecode);
            }
            else if (client->method == s_method_UNSUBSCRIBE)
            {
                butil_log(4, "UNSUBSCRIBE %s\n", client->path);
                result = upnp_unsubscribe(server, sub, &ecode);
            }
            else
            {
                butil_log(1, "Can't do method \"%s\" on UPnP\n",
                       http_method_name(client->method));
                ecode = 401;
                result = -1;
            }
        }
        if (! result)
        {
            // do the reply headers ourselves here, as they're a bit different from
            // the http defaults
            //
            result = http_begin_reply(client, 200, "OK");
            result |= http_append_reply(client, "Content-Type: %s", butil_mime_string_for_content_type(butil_mime_xml));
            result |= http_append_reply(client, "Content-Length: %d", 0);
            result |= http_append_reply(client, "Connection: close");
            if (client->method == s_method_SUBSCRIBE)
            {
                result |= http_append_reply(client, "SID: uuid:%s", sub->sid);
                result |= http_append_reply(client, "Timeout: Second-%u", sub->expiry - time(NULL));
            }
            result |= http_append_reply(client, "Server: bnet/1.0 UPnP/1.0 bnet/1.0");
            result |= http_append_reply(client, "Ext: ");
            result |= http_append_reply(client, "");
            client->state = httpBodyUpload; // this bypasses http system building its own reply
        }
        else
        {
        }
        break;

    case httpUploadData:
        *count = 0;
        break;

    case httpComplete:
        if (sub)
        {
            sub->next = server->subs_free;
            server->subs_free = sub;
        }
        break;

    default:
        return -1;
    }
    return result;
}

