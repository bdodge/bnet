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
#include "bsip.h"


static butil_url_scheme_t s_sip_scheme;

static struct tag_sip_method_registry
{
    const char *name;
    http_method_t httpmethod;
    sip_method_t  sipmethod;
}
s_method_registry[] =
{
    { "INVITE",     httpUnsupported, sipInvite },
    { "ACK",        httpUnsupported, sipAck },
    { "PRACK",      httpUnsupported, sipPrack },
    { "CANCEL",     httpUnsupported, sipCancel },
    { "UPDATE",     httpUnsupported, sipUpdate },
    { "INFO",       httpUnsupported, sipInfo },
    { "SUBSCRIBE",  httpUnsupported, sipSubscribe },
    { "NOTIFY",     httpUnsupported, sipNotify },
    { "REFER",      httpUnsupported, sipRefer },
    { "MESSAGE",    httpUnsupported, sipMessage },
    { "REGISTER",   httpUnsupported, sipRegister },
    { "BYE",        httpUnsupported, sipBye },
};

#define SIP_NUM_METHODS (sizeof(s_method_registry)/sizeof(struct tag_sip_method_registry))

int sip_canned_resource_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    butil_log(5, "Canned Resource CB: %d\n", cbtype);

    http_method_t method;
    int result;

    method = client->method;
    if (method == httpUser0)
    {
        client->method = httpGet;
    }
    else if (method == httpUser1)
    {
        client->method = httpPut;
    }
    result = http_canned_callback(
                                client,
                                resource,
                                cbtype,
                                data,
                                count
                                );
    client->method = method;
    return result;
}

int sip_resource_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    sip_server_context_t *sip;
    int result;
    int meth;

    butil_log(5, "SIP Resource CB: %d\n", cbtype);

    if (! resource)
    {
        BERROR("no resource");
        return -1;
    }
    sip = (sip_server_context_t *)resource->priv;
    if (! sip)
    {
        BERROR("no server context in resource");
        return -1;
    }
    switch (cbtype)
    {
    case httpRequest:

        butil_log(5, "sip resource cb (request)  %s: %s\n",
                http_method_name(client->method), data ? (char*)data : "<nil>");

        for (meth = 0; meth < SIP_NUM_METHODS; meth++)
        {
            if (client->method == s_method_registry[meth].httpmethod)
            {
                break;
            }
        }
        if (meth >= SIP_NUM_METHODS)
        {
            BERROR("No such method\n");
            // todo 405, bad req
            return 0;
        }
        sip->method = s_method_registry[meth].sipmethod;

        butil_log(5, "SIP Request %s\n",
            http_method_name(s_method_registry[meth].httpmethod));

        if (! client->resource)
        {
            BERROR("No SIP resource");
            result = http_error_reply(client, 405, "Bad Request", false);
            if (result)
            {
                return -1;
            }
            return 0;
        }
        // hold of passing on request until body is uploaded
        break;

    case httpDownloadData:

        butil_log(5, "sip download cb (request)  %s: %zu bytes\n",
                http_method_name(client->method), count ? *count : 0);

        if (data && *data && count && *count)
        {
            uint8_t *pd = (uint8_t*)*data;
            size_t nd = *count;

            while (nd-- > 0 && sip->sdpin.count < (sip->sdpin.size - 1))
            {
                sip->sdpin.data[sip->sdpin.head++] = *pd++;
                if (sip->sdpin.head >= sip->sdpin.size)
                {
                    sip->sdpin.head = 0;
                }
                sip->sdpin.count++;
            }
         //   *count = nd;
        }
        sip->sdpin.data[sip->sdpin.head] = '\0';
        break;

    case httpDownloadDone:

        butil_log(5, "sip download done cb   %s\n",
                http_method_name(client->method));

        result = sip_request(client, sip);
        if (result)
        {
            // any errors that don't format a reply go to done state, else
            // assume the request handler sent a specific reply already
            //
            if (client->state != httpSendReply)
            {
                client->state = httpDone;
            }
            result = 0;
        }
        else
        {
            if (client->state != httpSendReply)
            {
                // sip wants to continue, so move to usermethod state
                // which will call our method handler function
                //
                client->state = httpUserMethod;
            }
            result = 0;
        }
        break;

    case httpUploadData:

        butil_log(5, "sip upload cb (request)  %s\n",
                http_method_name(client->method));
        *count = 0;
        *data = NULL;
        break;

    case httpComplete:

        break;

    default:

        return -1;
    }
    return 0;
}

int sip_method_callback(
                        http_client_t *client,
                        http_method_callback_type_t type,
                        const http_method_t method,
                        const char *data,
                        void *priv
                        )
{
    sip_server_context_t *sip;
    int result;
    int meth;

    sip = (sip_server_context_t *)priv;

    if (! sip)
    {
        return 1;
    }
    if (type == httpMethodHeader)
    {
        butil_log(5, "Method cb (header)  %s: %s\n",
                http_method_name(method), data ? data : "<nil>");
        result = sip_process_header(sip, data);
        return result;
    }
    if (type == httpMethodRequest)
    {
        result = sip_slice(client, sip);

        if (client->out.count > 0)
        {
            iostream_normalize_ring(&client->out, NULL);

            // if slice generated data, go to reply state until
            // it is sent, then come back to usermethod
            //
            if (client->out.count > 0)
            {
                return http_send_out_data(client, httpSendReply, httpUserMethod);
            }
        }
        else if (result)
        {
            client->state = httpDone;
        }
        else
        {
            // if any input pending for client, go back to read request state
            // which may (or may not) get back to usermethod state
            //
            result = http_client_input(client, 0, 0);
            if (result == 0 && client->in.count > 0)
            {
                client->next_state  = httpReadRequest;
                client->state       = httpReadline;
                client->line_count  = 0;
            }
        }
    }
    return 0;
}

int sip_phone(int argc, char **argv)
{
    sip_server_context_t sip_server;
    uint16_t port;
    const char *program, *arg;
    http_method_t method;
    int i;
    int result;

#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0)
    {
        BERROR("WSAStartup failed");
        return -1;
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    http_set_log_level(5);

#if HTTP_SUPPORT_TLS
    result = iostream_tls_prolog();
    if (result)
    {
        BERROR("Can't init TLS");
        return -1;
    }
#endif

    program = *argv++;
    argc--;

    port = 5050;
    result = 0;

    memset(&sip_server, 0, sizeof(sip_server));

    while (argc > 0 && ! result)
    {
        arg = *argv++;
        argc--;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 'u':
            case 't':
                argc--;
                argv--;
                break;
            case 'p':
                if (argc > 0)
                {
                    port = strtoul(*argv, NULL, 0);
                    argv++;
                    argc--;
                }
                else
                {
                    butil_log(0, "Use: -p [port]");
                }
                break;
            default:
                butil_log(0, "Bad Switch: %s\n", arg);
                break;
            }
        }
        else
        {
            butil_log(5, "Ignore parm %s\n", arg);
        }
    }
    http_server_t server;
    http_resource_t *resource, *resources = NULL;
    http_credentials_t creds;

    butil_url_scheme_t myscheme;

    result = sip_init_context(&sip_server);
    if (result)
    {
        BERROR("Can't init");
        return result;
    }
    // register a custom scheme
    result = butil_register_scheme("sip", &s_sip_scheme);
    if (result)
    {
        BERROR("can't register scheme");
    }
    // register our methods
    for (i = 0; i < SIP_NUM_METHODS; i++)
    {
        result = http_register_method(
                                    s_method_registry[i].name,
                                    sip_method_callback,
                                    &sip_server,
                                    &s_method_registry[i].httpmethod
                                    );
        if (result)
        {
            BERROR("can't register method");
            return result;
        }
    }
    // add canned index.html at http for testing
    char *cannedindex = "<h2>hello world</h2>";

    result = http_add_canned_resource(&resources, schemeHTTP,  "/index.html",
                NULL, butil_mime_html, (uint8_t*)cannedindex, strlen(cannedindex));
    if (result)
    {
        BERROR("can't make resource");
        return result;
    }
    // add a resource for the scheme as well
    char *cannedfrob = "<h2>hello frobulated world</h2>";

    result = http_add_func_resource(&resources, s_sip_scheme,  "/index.html",
                NULL, sip_resource_callback, NULL);
    if (result)
    {
        BERROR("can't make resource");
        return result;
    }
    resource = http_find_resource(resources, s_sip_scheme, "/index.html", httpGet);
    if (resource)
    {
        resource->resource.canned_data.content = (const uint8_t*)cannedfrob;
        resource->resource.canned_data.count = strlen(cannedfrob);
        resource->resource.canned_data.content_type = butil_mime_html;
    }
    // handle all sip scheme urls in callback
    //
    result = http_add_func_resource(&resources, s_sip_scheme,  "*",
                NULL, sip_resource_callback, &sip_server);
    if (result)
    {
        BERROR("can't make resource");
        return result;
    }
    // set use-tls for certain ports
    //
    result = http_server_init(&server, resources, port, httpUDP, (port == 5061));
    if (result)
    {
        BERROR("can't start server");
        return result;
    }
    result = http_serve(&server, NULL, NULL);
    if (result)
    {
        butil_log(2, "server on port %u ends\n", server.port);
    }
    http_server_cleanup(&server);
    return result;
}

