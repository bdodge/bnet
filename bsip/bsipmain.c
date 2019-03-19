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

int sip_resource_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    butil_log(5, "Resource CB: %d\n", cbtype);

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
#if 0
    switch (cbtype)
    {
    case httpRequest:

        switch (client->method)
        {
        case httpUser0:
            return 0;
        default:
            return -1;
        }
        break;

    case httpUploadData:

        break;

    case httpDownloadData:

        break;

    case httpComplete:

        break;

    default:

        return -1;
    }
    return 0;
#endif
}

int sip_method_callback(
                        http_client_t *client,
                        http_method_callback_type_t type,
                        const http_method_t method,
                        const char *data,
                        void *priv
                        )
{
    butil_log(5, "Method cb %d  %s: %s\n", type,
            http_method_name(method), data ? data : "<nil>");
    return 0;
}

static butil_url_scheme_t s_sip_scheme;

static struct tag_sip_method_registry
{
    const char *name;
    http_method_t method;
}
s_method_registry[] =
{
    { "INVITE",     httpUnsupported },
    { "ACK",        httpUnsupported },
    { "PRACK",      httpUnsupported },
    { "CANCEL",     httpUnsupported },
    { "UPDATE",     httpUnsupported },
    { "INFO",       httpUnsupported },
    { "SUBSCRIBE",  httpUnsupported },
    { "NOTIFY",     httpUnsupported },
    { "REFER",      httpUnsupported },
    { "MESSAGE",    httpUnsupported },
    { "REGISTER",   httpUnsupported },
    { "BYE",        httpUnsupported },
};

#define SIP_NUM_METHODS (sizeof(s_method_registry)/sizeof(struct tag_sip_method_registry))

int main(int argc, char **argv)
{
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

    port = 8080;
    result = 0;

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

    // add canned index.html
    //
    char *cannedindex = "<h2>hello world</h2>";

    result = http_add_canned_resource(&resources, schemeHTTP,  "/canned.html",
                NULL, butil_mime_html, (uint8_t*)cannedindex, strlen(cannedindex));
    if (result)
    {
        BERROR("can't make resource");
        return result;
    }
    butil_url_scheme_t myscheme;

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
                                    NULL,
                                    &s_method_registry[i].method
                                    );
        if (result)
        {
            BERROR("can't register method");
            return result;
        }
    }
    // add a resource for the scheme
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
    // set use-tls for certain ports
    //
    result = http_server_init(&server, resources, port, httpTCP, (port == 443 || port == 4443));
    if (result)
    {
        BERROR("can't start server");
        return result;
    }
    result = http_serve(&server);
    if (result)
    {
        butil_log(2, "server on port %u ends\n", server.port);
    }
    http_server_cleanup(&server);
    return result;
}

