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

static http_method_t s_method_MSEARCH;
http_method_t s_method_NOTIFY;
http_method_t s_method_SUBSCRIBE;
http_method_t s_method_UNSUBSCRIBE;

// remove proper delays in serch replies to make response faster
//
#define UPNP_NO_JITTERS 1

time_t upnp_rate_fudge(time_t rate)
{
    time_t rn;

    if (rate < 8)
    {
        rate = 8;
    }
    rate /= 2;

    rn = rand();

    // randomize advertising rate a bit (up to 1/4 less)
    //
    return rate - ((rn * (rate / 4)) / RAND_MAX);
}

time_t upnp_mx_fudge(time_t mx)
{
    // randomize mx to between 0 and mx
    //
    return mx - (((rand() * mx) + (RAND_MAX / 2)) / RAND_MAX);
}

static int upnp_queue_reply(
                            upnp_server_t *server,
                            time_t         when,
                            const char    *to_host,
                            const uint16_t to_port,
                            const char    *st,
                            const char    *usn,
                            time_t         rate
                            )
{
    upnp_replyq_t *reply;

    reply = server->q_free;
    if (! reply)
    {
        return -1;
    }

    server->q_free = reply->next;

#ifndef UPNP_NO_JITTERS
    time(&reply->when);
    reply->when += when;
#else
    reply->when = 0;
#endif
    strncpy(reply->reply_to_host, to_host, sizeof(reply->reply_to_host));
    reply->reply_to_port = to_port;
    strncpy(reply->st, st, sizeof(reply->st));
    strncpy(reply->usn, usn, sizeof(reply->usn));
    reply->rate = rate;
    reply->next = NULL;

    butil_log(4, "Q reply to %s %d from now\n", to_host, when);

    if (! server->replyq)
    {
        server->replyq = reply;
    }
    else
    {
        upnp_replyq_t *pq;

        for (pq = server->replyq; pq->next;)
        {
            pq = pq->next;
        }
        pq->next = reply;
    }
    return 0;
}

static int upnp_free_reply(upnp_server_t *server, upnp_replyq_t *reply)
{
    if (! server || ! reply)
    {
        return -1;
    }

    reply->next = server->q_free;
    server->q_free = reply;

    return 0;
}

static int upnp_reply_st_usn(
                            upnp_server_t   *server,
                            const char      *to_host,
                            const uint16_t   to_port,
                            const char      *st,
                            const char      *usn,
                            const uint32_t   rate
                            )
{
    upnp_device_t *device;
    http_client_t *phttp;
    char hostname[UPNP_MAX_URL];
    char location[UPNP_MAX_URL];
    bipv4addr_t myipv4addr;
    bipv6addr_t myipv6addr;
    int iface_index;
    int result;

    if (! server || ! server->upnp_http_server.clients)
    {
        return -1;
    }

    if (! server->upnp_http_server.clients->stream)
    {
        return -2;
    }

    device = server->root_device;
    if (! device)
    {
        return 0;
    }

    result = butil_get_host_info(
                                hostname,
                                sizeof(hostname),
                                &myipv4addr,
                                &myipv6addr,
                                &iface_index
                                );
    if (result)
    {
        butil_log(1, "Can't get interface data\n");
        return result;
    }

    if (myipv4addr.addr != 0)
    {
        butil_str_for_ipv4(myipv4addr.addr, hostname, sizeof(hostname));
    }
    else
    {
        butil_str_for_ipv6(&myipv6addr, hostname, sizeof(hostname));
    }

    result = butil_paste_url(
                                location,
                                sizeof(location),
                                schemeHTTP,
                                hostname,
                                server->port,
                                device->desc_url
                            );
    if (result)
    {
        butil_log(1, "Can't paste url\n");
        return result;
    }

    // use the upnp http server's permanent client to send replies
    // for convenience, since it has the infrastructure and the socket/stream
    //
    phttp = server->upnp_http_server.clients;

    strncpy(phttp->out_host, to_host, sizeof(phttp->out_host));
    phttp->out_port = to_port;

    http_begin_reply(phttp, 200, "OK");
    http_append_reply(phttp, "ST: %s", st);
    http_append_reply(phttp, "Location: %s", location);
    http_append_reply(phttp, "USN: %s", usn);
    http_append_reply(phttp, "Cache-Control: max-age=%d", rate);
    http_append_reply(phttp, "Server: net/1.0 UPnP/1.0 bnet/1.0");
    http_append_reply(phttp, "Content-Length: 0");
    http_append_reply(phttp, "Ext:");
    http_append_reply(phttp, "");

    result = http_send_out_data(phttp, phttp->state, phttp->state);
    if (result)
    {
        butil_log(1, "Send reply failed\n");

        // this isn't fatal as it is udp after all
        result = 0;
    }
    return result;
}

int upnp_respond_to_search(upnp_server_t *server, http_client_t *client)
{
    char usn[UPNP_MAX_URL];
    char uuid_string[64];
    upnp_device_t *device;
    upnp_service_t *service;
    int result;
    bool match_all  = false;
    bool match_root = false;
    bool match_uuid = false;
    bool match_urn  = false;
    bool match;

    butil_log(4, "M-SEARCH %s\n", client->path);

    // if the search target matches one of our devices or services, reply
    //
    if (! strcmp(server->man_header, "\"ssdp:discover\""))
    {
        if (! http_ncasecmp(server->search_header, "ssdp:all"))
        {
            match_all = true;
        }
        else if (! http_ncasecmp(server->search_header, "upnp:rootdevice"))
        {
            match_root = true;
        }
        else if (! http_ncasecmp(server->search_header, "uuid:"))
        {
            match_uuid = true;
        }
        else if (! http_ncasecmp(server->search_header, "urn:"))
        {
            match_urn = true;
        }
        else
        {
            butil_log(4, "Ignore search targer \"%s\"\n", server->search_header);
            return 0;
        }
    }
    else
    {
        butil_log(3, "Ignore Man hdr \"%s\"\n", server->man_header);
        return 0;
    }

    for (device = server->root_device; device; device = device->next)
    {
        snprintf(uuid_string, sizeof(uuid_string), "uuid:");
        uuid_format(device->udn, uuid_string + 5, sizeof(uuid_string) - 5);

        match = false;

        if (match_root || match_all)
        {
            match = match_all || (device == server->root_device);

            if (match)
            {
                snprintf(usn, sizeof(usn), "%s::upnp:rootdevice", uuid_string);

                result = upnp_queue_reply(
                                        server,
                                        upnp_mx_fudge(server->mx_header),
                                        client->out_host,
                                        client->out_port,
                                        server->search_header,
                                        usn,
                                        server->root_device->adv_rate
                                        );
            }
        }
        if (match_uuid || match_all)
        {
            match = (match_all || (! http_ncasecmp(server->search_header, uuid_string)));

            if (match)
            {
                result = upnp_queue_reply(
                                        server,
                                        upnp_mx_fudge(server->mx_header),
                                        client->out_host,
                                        client->out_port,
                                        server->search_header,
                                        uuid_string,
                                        server->root_device->adv_rate
                                        );
            }
        }
        if (match_urn || match_all)
        {
            // format urn of device, sans version
            //
            snprintf(usn, sizeof(usn), "urn:schemas-upnp-org:device:%s:", device->upnp_device_type);

            if (! match_all)
            {
                // compare to search target
                //
                butil_log(3, "Compare =%s= to our =%s=\n", server->search_header, usn);

                if (! http_ncasecmp(server->search_header, usn))
                {
                    // partially matched a device, so check our version is >= the
                    // searching for version
                    //
                    uint32_t theirver = strtoul(server->search_header + strlen(usn), NULL, 0);

                    if (device->upnp_device_version >= theirver)
                    {
                        match = true;

                        butil_log(4, "Search matched our device %p ver:%d for req ver:%d\n",
                                                        device, device->upnp_device_version, theirver);
                    }
                }
            }
            else
            {
                match = true;
            }

            if (match)
            {
                snprintf(usn, sizeof(usn), "%s::urn:schemas-upnp-org:device:%s:%d",
                                uuid_string, device->upnp_device_type, device->upnp_device_version);

                result = upnp_queue_reply(
                                        server,
                                        upnp_mx_fudge(server->mx_header),
                                        client->out_host,
                                        client->out_port,
                                        server->search_header,
                                        usn,
                                        server->root_device->adv_rate
                                        );
            }
        }

        for (service = device->services; service; service = service->next)
        {
            match = false;

            if (match_urn || match_all)
            {
                // format urn of service, sans version
                //
                snprintf(usn, sizeof(usn), "urn:schemas-upnp-org:service:%s:", service->upnp_service_type);

                if (! match_all)
                {
                    // compare to search target
                    //
                    butil_log(5, "Compare =%s= to our =%s=\n", server->search_header, usn);

                    if (! http_ncasecmp(server->search_header, usn))
                    {
                        // partially matched a device, so check our version is >= the
                        // searching for version
                        //
                        uint32_t theirver = strtoul(server->search_header + strlen(usn), NULL, 0);

                        if (service->upnp_service_version >= theirver)
                        {
                            match = true;

                            butil_log(4, "Search matched our service %p ver:%d for req ver:%d\n",
                                                            service, service->upnp_service_version, theirver);
                        }
                    }
                }
                else
                {
                    match = true;
                }

                if (match)
                {
                    snprintf(usn, sizeof(usn), "%s::urn:schemas-upnp-org:service:%s:%d",
                                    uuid_string, service->upnp_service_type, service->upnp_service_version);

                    result = upnp_queue_reply(
                                            server,
                                            upnp_mx_fudge(server->mx_header),
                                            client->out_host,
                                            client->out_port,
                                            server->search_header,
                                            usn,
                                            server->root_device->adv_rate
                                            );
                }
            }
        }
    }

    server->search_header[0] = '\0';
    server->man_header[0] = '\0';

    return 0;
}

static int upnp_handle_udp_url(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    upnp_server_t *server;
    char *header;
    char *value;
    int result;

    server = (upnp_server_t *)resource->priv;
    if (! server)
    {
        return -1;
    }

    result = 0;

    butil_log(5, "UPnP %d %s to %s\n", cbtype, http_method_name(client->method), client->path);

    switch (cbtype)
    {
    case httpRequest:

        if (client->method == s_method_MSEARCH)
        {
            result = upnp_respond_to_search(server, client);
            return result;
        }
        else if (client->method == s_method_NOTIFY)
        {
            butil_log(4, "NOTIFY %s\n", client->path);
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

        while (*value != ' ' && *value != '\t' && *value != ':' && *value)
        {
            value++;
        }

        while (*value == ' ' || *value == '\t' || *value == ':')
        {
            value++;
        }

        if (! http_ncasecmp((char*)*data, "st:"))
        {
            strncpy(server->search_header, value, sizeof(server->search_header));
            break;
        }

        if (! http_ncasecmp((char*)*data, "man:"))
        {
            strncpy(server->man_header, value, sizeof(server->man_header));
            break;
        }

        if (! http_ncasecmp((char*)*data, "mx:"))
        {
            server->mx_header = strtoul(value, NULL, 10);
            break;
        }
        break;

    case httpDownloadData:
        break;

    case httpDownloadDone:
        break;

    case httpUploadData:
        *count = 0;
        break;

    case httpComplete:
        break;

    default:
        return -1;
    }
    return result;
}

static int upnp_notify_nt_usn(
                            upnp_server_t   *server,
                            const upnp_service_state_t state,
                            const char      *path,
                            const char      *nt,
                            const char      *usn,
                            const char      *location,
                            const uint32_t  rate
                            )
{
    upnp_device_t *device;
    http_client_t *phttp;
    int result;

    if (! server || ! server->upnp_http_server.clients)
    {
        return -1;
    }

    if (! server->upnp_http_server.clients->stream)
    {
        return -2;
    }

    // use the upnp http server's permanent client to sent notifications
    // for convenience, since it has the infrastructure and the socket/stream
    //
    phttp = server->upnp_http_server.clients;

    phttp->method   = s_method_NOTIFY;
    phttp->vmaj     = 1;
    phttp->vmin     = 1;
    phttp->scheme   = schemeHTTP;

    // notify target path
    //
    strncpy(phttp->path, path, sizeof(phttp->path));

    // set the destination as multicast address:port
    //
    strncpy(phttp->out_host, UPNP_MCAST_IPV4ADDR, sizeof(phttp->host));
    phttp->out_port = UPNP_PORT;

    http_begin_request(phttp);
    http_append_request(phttp, "HOST:239.255.255.250:1900");
    http_append_request(phttp, "NT:%s", nt);
    http_append_request(phttp, "NTS:ssdp:%s", (state == upnpServiceOnline) ? "alive" : "byebye");
    http_append_request(phttp, "Location:%s", location);
    http_append_request(phttp, "USN:%s", usn);
    http_append_request(phttp, "Cache-Control:max-age=%d", rate);
    http_append_request(phttp, "Server:bnet/1.0 UPnP/1.0 bnet/1.0");
    http_append_request(phttp, "Content-Length:0");
    http_append_request(phttp, "");

    result = http_send_out_data(phttp, phttp->state, phttp->state);

    return result;
}

static int upnp_notify(upnp_server_t *server)
{
    upnp_device_t *device;
    upnp_service_t *service;
    http_client_t *phttp;
    char uuid_string[64];
    char usn[UPNP_MAX_URL];
    char urn[UPNP_MAX_URL];
    char hostname[UPNP_MAX_URL];
    char location[UPNP_MAX_URL];
    bipv4addr_t myipv4addr;
    bipv6addr_t myipv6addr;
    int iface_index;
    int result;

    if (! server || ! server->upnp_http_server.clients)
    {
        return -1;
    }

    if (! server->upnp_http_server.clients->stream)
    {
        return -2;
    }

    device = server->root_device;
    if (! device)
    {
        return 0;
    }

    result = butil_get_host_info(
                                hostname,
                                sizeof(hostname),
                                &myipv4addr,
                                &myipv6addr,
                                &iface_index
                                );
    if (result)
    {
        butil_log(1, "Can't get interface data\n");
        return result;
    }

    if (myipv4addr.addr != 0)
    {
        butil_str_for_ipv4(myipv4addr.addr, hostname, sizeof(hostname));
    }
    else
    {
        butil_str_for_ipv6(&myipv6addr, hostname, sizeof(hostname));
    }

    result = butil_paste_url(
                                location,
                                sizeof(location),
                                schemeHTTP,
                                hostname,
                                server->port,
                                device->desc_url
                            );
    if (result)
    {
        butil_log(1, "Can't paste url\n");
        return result;
    }

    // format uuid string as "uuid:<device uuid>"
    //
    snprintf(uuid_string, sizeof(uuid_string), "uuid:");
    uuid_format(device->udn, uuid_string + 5, sizeof(uuid_string) - 5);

    // for each root device, three notifications, the root, and then regular device notifications
    //
    // 1)   NT=upnp:rootdevice   USN=uuid:device-UUID::upnp:rootdevice
    //
    snprintf(usn, sizeof(usn), "%s::upnp:rootdevice", uuid_string);

    result = upnp_notify_nt_usn(
                                server,
                                upnpServiceOnline,
                                "*",
                                "upnp:rootdevice",
                                usn,
                                location,
                                device->adv_rate
                               );

    if (result)
    {
        butil_log(1, "notify 1 failed");
        return result;
    }
    while (device)
    {
        result = butil_paste_url(
                                    location,
                                    sizeof(location),
                                    schemeHTTP,
                                    hostname,
                                    server->port,
                                    device->desc_url
                                );
        if (result)
        {
            butil_log(1, "Can't paste url\n");
            return result;
        }

        // 2)   NT=uuid:device-UUID  USN=uuid:device-UUID
        //
        result = upnp_notify_nt_usn(
                                    server,
                                    upnpServiceOnline,
                                    "*",
                                    uuid_string,
                                    uuid_string,
                                    location,
                                    device->adv_rate
                                   );
        if (result)
        {
            butil_log(1, "notify 2 failed");
            return result;
        }

        // 3)   NT=urn:<schema>:<device-type>:<device-vers>
        //      USN=uuid:device-UUID::urn:<schema>:device:<device-type>:<device-version>
        //
        snprintf(urn, sizeof(urn), "urn:schemas-upnp-org:device:%s:%d",
                        device->upnp_device_type, device->upnp_device_version);
        snprintf(usn, sizeof(usn), "%s::%s", uuid_string, urn);

        result = upnp_notify_nt_usn(
                                    server,
                                    upnpServiceOnline,
                                    "*",
                                    urn,
                                    usn,
                                    location,
                                    device->adv_rate
                                   );
        if (result)
        {
            butil_log(1, "notify 3 failed");
            return result;
        }

        // for each service in each device, notify of service
        //
        for (service = device->services; service; service = service->next)
        {
            result = butil_paste_url(
                                        location,
                                        sizeof(location),
                                        schemeHTTP,
                                        hostname,
                                        server->port,
                                        service->scpd_url
                                    );
            if (result)
            {
                butil_log(1, "Can't paste scpd url\n");
                return result;
            }

            // 1)   NT=urn:<schema>:<service-type>:<service-vers>
            //      USN=uuid:service-UUID::urn:<schema>:service:<service-type>:<service-version>
            //
            snprintf(urn, sizeof(urn), "urn:schemas-upnp-org:service:%s:%d",
                            service->upnp_service_type, service->upnp_service_version);
            snprintf(usn, sizeof(usn), "%s::%s", uuid_string, urn);

            result = upnp_notify_nt_usn(
                                        server,
                                        upnpServiceOnline,
                                        "*",
                                        urn,
                                        usn,
                                        location,
                                        device->adv_rate
                                       );
            if (result)
            {
                butil_log(1, "notify service failed");
                return result;
            }

        }

        device = device->next;
    }

    return 0;
}

int upnp_send_pending_replies(upnp_server_t *server, time_t now)
{
    upnp_replyq_t *reply;
    upnp_replyq_t *prevreply;
    int result;
    bool culled;

    result = 0;

    for (reply = server->replyq; reply && ! result; reply = reply->next)
    {
        if (reply->when <= now)
        {
            butil_log(4, "S reply to %s\n", reply->reply_to_host);

            // send the reply now
            result = upnp_reply_st_usn(
                            server,
                            reply->reply_to_host,
                            reply->reply_to_port,
                            reply->st,
                            reply->usn,
                            reply->rate
                            );

            // mark as sent for later culling
            reply->when = 0;
        }
    }

    // cull the sent items
    //
    do
    {
        culled = false;
        prevreply = NULL;

        for (reply = server->replyq; reply; reply = reply->next)
        {
            if (reply->when == 0)
            {
                if (reply == server->replyq)
                {
                    server->replyq = reply->next;
                }
                else if (prevreply /* cant be null but check anyway */)
                {
                    prevreply->next = reply->next;
                }
                upnp_free_reply(server, reply);
                culled = true;
                break;
            }
            prevreply = reply;
        }
    }
    while (culled);

    return result;
}


int upnp_add_file_url(
                     upnp_server_t  *server,
                     const char     *path,
                     const mime_content_type_t type,
                     const uint8_t  *payload,
                     size_t          paylen
                )
{
    int result;

    if (! server || ! path || ! payload || ! paylen)
    {
        return -1;
    }

    result = http_add_canned_resource(
                        &server->http_resources,
                        schemeHTTP,
                        path,
                        NULL,
                        type,
                        payload,
                        paylen
                        );
    if (! result)
    {
        server->doc_http_server.resources = server->http_resources;
    }

    return result;
}

int upnp_add_text_url(
                     upnp_server_t    *server,
                     const char       *path,
                     const mime_content_type_t type,
                     const char       *value
                )
{
    if (! value)
    {
        return -1;
    }

    return upnp_add_file_url(server, path, type, (uint8_t*)value, strlen(value));
}

int upnp_add_func_url(
                     upnp_server_t    *server,
                     const char       *path,
                     http_resource_function_t func,
                     void             *priv
                )
{
    int result;

    if (! server || ! path || ! func)
    {
        return -1;
    }

    result = http_add_func_resource(
                        &server->http_resources,
                        schemeHTTP,
                        path,
                        NULL,
                        func,
                        priv
                        );
    if (! result)
    {
        server->doc_http_server.resources = server->http_resources;
    }

    return result;
}

static const char *upnp_state_name(upnp_state_t state)
{
    switch (state)
    {
    case upnpServeInit:         return "Init(server)";
    case upnpClientInit:        return "Init(client)";
    case upnpServe:             return "Serve";
    case upnpDone:              return "Done";
    default:                    return "- bad state -";
    }
}

int upnp_server_slice(upnp_server_t *server, int to_secs, int to_usecs)
{
    time_t now;
    int result = 0;

    time(&now);

    if (server->prev_state != server->state)
    {
        butil_log(6, "server: %s\n", upnp_state_name(server->state));
    }
    server->prev_state = server->state;

    switch (server->state)
    {
    case upnpServeInit:
        server->state = upnpServe;
        break;

    case upnpClientInit:
        break;

    case upnpServe:

        if (server->upnp_http_server.clients->state == httpServeInit)
        {
            // send any queued replies pending if the upnp http client is idle
            //
            upnp_send_pending_replies(server, now);

            // see if it is time to notify the world of our wonderous capabilities
            //
            if (server->root_device && server->root_device->next_adv < now)
            {
                server->root_device->next_adv = now + upnp_rate_fudge(server->root_device->adv_rate);
                upnp_notify(server);
            }
        }
        // wait for something to happen on a server, or
        // one of the server's clients
        //
        http_wait_for_server_event(&server->upnp_http_server);

        // process events
        //
        result  = http_server_slice(&server->doc_http_server, 0, 2000);
        result |= http_server_slice(&server->upnp_http_server, 0, 2000);

        if (server->event_http_client && server->event_http_client->state != httpDone)
        {
            // there is an active notification being sent out, do that
            //
            result = http_client_slice(server->event_http_client);
        }
        else
        {
            // for each subscription marked dirty, notify the subscriber
            //
            if (server->subscriptions)
            {
                upnp_subscription_t *sub;
                upnp_device_t *device;
                upnp_service_t *service;

                for (sub = server->subscriptions; sub; sub = sub->next)
                {
                    if (sub->dirty)
                    {
                        sub->dirty = false;

                        break; // only do one notification at a a time
                    }
                }

                if (sub)
                {
                    upnp_notify_subscribers(server, sub->service);

                    // if this is the only/last dirty subscription to this
                    // service, mark the service's state vars clean;
                    //
                    service = sub->service;

                    for (sub = server->subscriptions; sub; sub = sub->next)
                    {
                        if (sub->dirty && sub->service == service)
                        {
                            break;
                        }
                    }

                    if (! sub)
                    {
                        // no more dirty subs to this service so clean vars
                        //
                        upnp_mark_var_dirty(service, NULL, false);
                    }
                }
            }
        }
        break;

    case upnpDone:
        break;

    default:
        UPNP_ERROR("Bad state");
        return -1;
    }
    return result;
}

int upnp_server_init(
                        upnp_server_t   *server,
                        const size_t     max_request,
                        const uint16_t   port,
                        const uuid_t     root_udn,
                        const char      *description_url,
                        const char      *device_name,
                        const uint32_t   device_version,
                        const uint32_t   advertising_rate
                      )
{
    struct ip_mreq mreq;
    socket_t sock;
    butil_url_scheme_t scheme;
    int iparm;
    int result;

    if (! server)
    {
        UPNP_ERROR("No context to set up");
        return -1;
    }
    server->state = upnpServeInit;
    server->port = port;
    server->aborted = false;
    server->upnp_resources = NULL;
    server->http_resources = NULL;

    server->root_device = NULL;

    // init reply queue pool
    //
    for (iparm = 0; iparm < UPNP_MAX_QUEUED_REPLIES - 1; iparm++)
    {
        server->q_pool[iparm].next = &server->q_pool[iparm + 1];
    }
    server->q_pool[iparm].next = NULL;
    server->q_free = &server->q_pool[0];
    server->replyq = NULL;

    // init subscription pool
    //
    for (iparm = 0; iparm < UPNP_MAX_SUBSCRIPTIONS - 1; iparm++)
    {
        server->subs_pool[iparm].next = &server->subs_pool[iparm + 1];
    }
    server->subs_pool[iparm].next = NULL;
    server->subs_free = &server->subs_pool[0];
    server->subscriptions = NULL;

    result = 0;

    do // try
    {
        // create the single root device
        //
        result = upnp_add_device(
                                server,
                                root_udn,
                                description_url,
                                device_name,
                                device_version,
                                advertising_rate
                                );
        if (result)
        {
            UPNP_ERROR("Can't add root device");
            break;
        }

        // Setup HTTP Document Server
        //
        result = http_server_init(&server->doc_http_server,
                        server->http_resources, port, httpTCP, UPNP_MAX_CONCURRENT_CLIENTS, false);
        if (result)
        {
            UPNP_ERROR("Can't make http doc server");
            break;
        }

        // get the HTTP Scheme
        //
        result = butil_scheme_from_name("HTTP", &scheme);
        if (result)
        {
            UPNP_ERROR("can't get HTTP scheme");
            break;
        }

        // and add the UPnP methods for the HTTP scheme
        //
        result = http_register_method("M-SEARCH", NULL, NULL, &s_method_MSEARCH);
        if (result)
        {
            UPNP_ERROR("can't register m-search method");
            break;
        }

        result = http_register_method("NOTIFY", NULL, NULL, &s_method_NOTIFY);
        if (result)
        {
            UPNP_ERROR("can't register notify method");
            break;
        }

        result = http_register_method("SUBSCRIBE", NULL, NULL, &s_method_SUBSCRIBE);
        if (result)
        {
            UPNP_ERROR("can't register subscribe method");
            return result;
        }

        result = http_register_method("UNSUBSCRIBE", NULL, NULL, &s_method_UNSUBSCRIBE);
        if (result)
        {
            UPNP_ERROR("can't register unsubscribe method");
            return result;
        }

        // and add any url going to the upnp server as handled here
        //
        result = http_add_func_resource(&server->upnp_resources, schemeHTTP, "*", NULL, upnp_handle_udp_url, server);
        if (result)
        {
            UPNP_ERROR("can't make upnp resource");
            break;
        }

        // Setup HTTP UPnP server (UDP port 1900)
        //
        result = http_server_init(&server->upnp_http_server, server->upnp_resources, UPNP_PORT, httpUDP, 1, false);
        if (result)
        {
            UPNP_ERROR("Can't make http doc server");
            break;
        }

        // serving UDP means having one permanent client in the server which subsumes the server's socket
        // so get that client's socket to setup more
        //
        if (! server->upnp_http_server.clients || server->upnp_http_server.clients->socket == INVALID_SOCKET)
        {
            UPNP_ERROR("No UPD server socket");
            result = -1;
            break;
        }
        sock = server->upnp_http_server.clients->socket;

        // setup the server's socket in the UPnP multicast group
        //
        memset(&mreq, 0, sizeof(mreq));

        mreq.imr_multiaddr.s_addr = inet_addr(UPNP_MCAST_IPV4ADDR);
        mreq.imr_interface.s_addr = INADDR_ANY;

        result = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
        if (result < 0)
        {
            butil_log(0, "Can't join mcast group: %d\n", errno);
            break;
        }
        // and enable multicast on the interface
        //
        result = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF,
                (char *)&mreq.imr_interface.s_addr, sizeof(mreq.imr_interface.s_addr));
        if (result < 0)
        {
            butil_log(0, "Can't enable  mcast: %d\n", errno);
            break;
        }
        #if 1
        // disable loopback for the socket, to avoid read loops.
        //
        iparm = 0;
        result = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&iparm, sizeof(iparm));
        if (result < 0)
        {
            butil_log(0, "Can't disable loopback: %d\n", errno);
            break;
        }
        #endif
        // packet TTL is 4 per spec
        //
        iparm = 4;
        result = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &iparm, sizeof(iparm));
        if (result)
        {
            butil_log(0, "Can't set socket TTL\n");
            break;
        }

        // link servers together
        //
        server->upnp_http_server.next = &server->doc_http_server;
        server->doc_http_server.next = NULL;

        // create an http client for eventing
        //
        server->event_http_client = http_client_create(NULL, true);
        if (! server->event_http_client)
        {
            butil_log(0, "Can't make event http client\n");
            result = -1;
            break;
        }

        result = http_add_func_resource(&server->event_resources, schemeHTTP, "*", NULL, upnp_handle_notify, server);
        if (result)
        {
            UPNP_ERROR("can't make upnp resource");
            break;
        }
    }
    while (0); // catch

    if (result)
    {
        upnp_server_cleanup(server);
    }
    return result;
}

void upnp_server_cleanup(upnp_server_t *server)
{
    upnp_client_t *client;

    if (! server)
    {
        UPNP_ERROR("no server");
        return;
    }
    http_server_cleanup(&server->doc_http_server);
    http_server_cleanup(&server->upnp_http_server);

    if (server->event_http_client)
    {
        http_client_free(server->event_http_client);
        server->event_http_client = NULL;
    }
}

int upnp_server_abort(upnp_server_t *server)
{
    if (server)
    {
        server->aborted = true;
    }
    return 0;
}

int upnp_serve(upnp_server_t *server, upnp_idle_callback_t on_idle, void *priv)
{
    int result;

    do
    {
        result = upnp_server_slice(server, 0, 0);
        if (result < 0)
        {
            // remove server?
        }
        if (on_idle)
        {
            result = on_idle(priv);
            if (result)
            {
                butil_log(1, "Idle callback stops serving\n");
            }
        }
    }
    while (! result);

    butil_log(4, "UPNP Server ends\n");
    return result;
}

