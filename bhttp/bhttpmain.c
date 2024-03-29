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
#include "bhttp.h"

#if HTTP_SUPPORT_WEBSOCKET
int echo_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    size_t bytes;
    static uint8_t echobuf[1024];
    static size_t echobytes;
    int result;

    switch (cbtype)
    {
    case httpRequestHeader:
        http_log(5, "HDR: %s\n", (data && *data) ? (char*)*data : "<nil>");
        break;

    case httpRequest:

        http_log(5, "ECB: %d\n", cbtype);
        switch (client->method)
        {
        case httpGet:
            echobytes = 0;
            break;

        default:
            HTTP_ERROR("Can't do method on websocket");
            return -1;
        }
        break;

    case httpUploadData:
        // write data back
        bytes = *count;
        if (bytes > echobytes)
        {
            bytes = echobytes;
        }
        if (*data)
        {
            if (bytes > 0)
            {
                memcpy(*data, echobuf, bytes);
            }
        }
        else
        {
            *data = echobuf;
        }
        *count = bytes;
        echobytes = 0;
        break;;

    case httpDownloadData:
        http_log(5, "ECB: %d\n", cbtype);
        http_log(4, "%s",*data);
        // set format to text, or wsdfBinary, and set mask perhaps
        result = http_websocket_set_format(client, wsdfText, false, "mask");
        if (result)
        {
            return -1;
        }
        bytes = *count;
        if (bytes > sizeof(echobuf))
        {
            bytes = sizeof(echobuf);
        }
        memcpy(echobuf, *data, bytes);
        echobytes = bytes;
        *count = bytes;
        break;

    case httpDownloadDone:
        break;

    case httpComplete:
        break;

    default:
        return -1;
    }
    return 0;
}

int echo_client_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    size_t bytes;
    static char *hellob = "Hello Websockets";
    static bool sentping = false;
    size_t echobytes;
    int result;

    switch (cbtype)
    {
    case httpRequestHeader:
        http_log(5, "HDR: %s\n", (data && *data) ? (char*)*data : "<nil>");
        break;

    case httpRequest:

        http_log(5, "ECB: %d\n", cbtype);
        break;

    case httpUploadData:
        // write data to remote server
        if (!sentping)
        {
            // set format to text, or wsdfBinary, and set mask perhaps
            result = http_websocket_set_format(client, wsdfText, true, "mask");
            if (result)
            {
                return -1;
            }
            bytes = *count;
            echobytes = strlen(hellob);
            if (bytes > echobytes)
            {
                bytes = echobytes;
            }
            if (*data)
            {
                if (bytes > 0)
                {
                    memcpy(*data, hellob, bytes);
                }
            }
            else
            {
                *data = (uint8_t*)hellob;
            }
            *count = bytes;
            sentping = true;
        }
        else
        {
            *count = 0;
        }
        break;

    case httpDownloadData:
        // getting data from remote server
        http_log(5, "ECB: %d\n", cbtype);
        http_log(4, "%s",*data);
        bytes = *count;
        *count = bytes;
        break;

    case httpDownloadDone:
        break;

    case httpComplete:
        break;

    default:
        return -1;
    }
    return 0;
}

#endif

int cgi_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    int result;

    result = 0;

    http_log(5, "CGI Call % to %s\n",
                    http_method_name(client->method), client->path);
    switch (cbtype)
    {
    case httpRequest:

        switch (client->method)
        {
        case httpPost:
            client->ctxpriv = (void*)client->path;
            client->out_content_length = strlen(client->path);
            break;

        default:
            HTTP_ERROR("Can't do method on cgi, post only");
            return -1;
        }
        break;

    case httpDownloadData:
        http_log(4, "Download %s", *data);
        break;

    case httpDownloadDone:
        break;

    case httpUploadData:
        if (client->ctxpriv)
        {
            strcpy((char*)*data, (char*)client->ctxpriv);
            client->ctxpriv = NULL;
            *count = strlen((char*)*data);
        }
        else
        {
            *count = 0;
        }
        break;;

    case httpComplete:
        break;

    default:
        return -1;
    }
    return result;
}

int on_evil_scheme(
                    http_client_t *client,
                    const http_method_t method,
                    const char *data,
                    void *priv
                )
{
    http_log(5, "Method cb %s: %s\n",
           http_method_name(method), data ? data : "<nil>");
    return 0;
}

int evil_canned_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
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

int main(int argc, char **argv)
{
    bool isserver;
    uint16_t port;
    char url[HTTP_MAX_LINE];
    const char *program, *arg;
    http_method_t method;
    int loglevel = 5;
    bool websock;
    int result;

#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        HTTP_ERROR("WSAStartup failed");
        return -1;
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    http_set_log_level(loglevel);

#if HTTP_SUPPORT_TLS
    result = iostream_tls_prolog();
    if (result)
    {
        HTTP_ERROR("Can't init TLS");
        return -1;
    }
#endif

    program = *argv++;
    argc--;

    isserver = true;
    loglevel = 3;

    port = 8080;
    url[0] = '\0';
    method = httpGet;
    websock = false;
    result = 0;

    while (argc > 0 && ! result)
    {
        arg = *argv++;
        argc--;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 'X':
                isserver = false;
                if (argc > 0)
                {
                    result = http_method_from_name(*argv, &method);
                    if (result)
                    {
                        http_log(0, "Bad method: %s\n", *argv);
                    }
                    argc--;
                    argv++;
                }
                else
                {
                    http_log(0, "Use: -X [method]");
                }
                break;
            case 'w':
                websock = true;
                isserver = false;
                break;
            case 'u':
            case 't':
                isserver = false;
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
                    http_log(0, "Use: -p [port]");
                }
                break;
            case 'l':
                if (argc > 0)
                {
                    loglevel = strtoul(*argv, NULL, 0);
                    argv++;
                    argc--;
                }
                else
                {
                    http_log(0, "Use: -l [level]");
                }
                break;
            default:
                http_log(0, "Bad Switch: %s\n", arg);
                break;
            }
        }
        else
        {
            strncpy(url, arg, sizeof(url) - 1);
            url[sizeof(url) - 1] = '\0';
        }
    }
    http_set_log_level(loglevel);

    if (isserver)
    {
        http_server_t server;
        http_resource_t *resource, *resources = NULL;
        http_credentials_t fs_creds;
        http_credentials_t dav_creds;

        // add canned index.html
        //
        char *cannedindex = "<h2>hello world</h2>";

        result = http_add_canned_resource(&resources, schemeHTTP,  "/canned.html",
                    NULL, butil_mime_html, (uint8_t*)cannedindex, strlen(cannedindex));
        if (result)
        {
            HTTP_ERROR("can't make resource");
            return result;
        }
#if HTTP_SUPPORT_WEBSOCKET
        // add a function server resource at echo for websockets
        //
        result = http_add_func_resource(&resources, schemeWS, "/echo", NULL, echo_callback, NULL);
        if (result)
        {
            HTTP_ERROR("Can't make resource");
            return result;
        }
#endif
        // add generic file server resource at doc root
        //
#if 1
        fs_creds.user = "";
        fs_creds.pass = "";
        fs_creds.type = httpAuthNone;
#else
        fs_creds.user = "aaa";
        fs_creds.pass = "bbb";
        fs_creds.type = httpAuthBasic;
#endif
        result = http_add_file_resource(&resources, schemeHTTP, "*", "./media", &fs_creds);
        if (result)
        {
            HTTP_ERROR("Can't make resource");
            return result;
        }
        // add cgi url
        result = http_add_func_resource(&resources, schemeHTTP, "*.cgi", &fs_creds, cgi_callback, NULL);
        if (result)
        {
            HTTP_ERROR("Can't make resource");
            return result;
        }
#if HTTP_SUPPORT_WEBDAV
        // add dav root at doc root
        //
#if 1
        dav_creds.user = "";
        dav_creds.pass = "";
        dav_creds.type = httpAuthNone;
#else
        dav_creds.user = "aaa";
        dav_creds.pass = "bbb";
        dav_creds.type = httpAuthBasic;
#endif
        result = http_add_dav_resource(&resources, "/dav", "./media/d1", &dav_creds);
        if (result)
        {
            HTTP_ERROR("Can't make resource");
            return result;
        }
#endif
        {
        butil_url_scheme_t myscheme;

        // register a custom scheme
        result = butil_register_scheme("sip", &myscheme);
        if (result)
        {
            HTTP_ERROR("can't register scheme");
        }
        // and add a few methods for that scheme
        result = http_register_method("FROBULATE", on_evil_scheme, NULL, NULL);
        if (result)
        {
            HTTP_ERROR("can't register method");
        }
        result = http_register_method("DEFROBULATE", on_evil_scheme, NULL, NULL);
        if (result)
        {
            HTTP_ERROR("can't register method");
        }
        // add a resource for the scheme
        char *cannedfrob = "<h2>hello frobulated world</h2>";

        result = http_add_func_resource(&resources, myscheme,  "/index.html",
                    NULL, evil_canned_callback, NULL);
        if (result)
        {
            HTTP_ERROR("can't make resource");
            return result;
        }
        resource = http_find_resource(resources, myscheme, "/index.html", httpGet);
        if (resource)
        {
            resource->resource.canned_data.content = (const uint8_t*)cannedfrob;
            resource->resource.canned_data.count = strlen(cannedfrob);
            resource->resource.canned_data.content_type = butil_mime_html;
        }
        }
        // set use-tls for certain ports
        //
        result = http_server_init(
                                    &server,
                                    resources,
                                    port,
                                    httpTCP,
                                    HTTP_MAX_CLIENT_CONNECTIONS,
                                    (port == 443 || port == 4443));
        if (result)
        {
            HTTP_ERROR("can't start server");
            return result;
        }
        result = http_serve(&server, NULL, NULL);
        if (result)
        {
            http_log(2, "server on port %u ends\n", server.port);
        }
        http_server_cleanup(&server);
    }
    else
    {
        http_resource_t *resource, *resources = NULL;
        uint16_t port;
        http_client_t *client;
        int to_secs, to_usecs;
        int redirects;
        bool busy;

        http_log(3, "%s %s\n", http_method_name(method), url);

        client = http_client_create(NULL, true);
        if (! client)
        {
            HTTP_ERROR("No memory for client");
            return -1;
        }
        client->keepalive = false;
        redirects = 0;

#if HTTP_SUPPORT_WEBSOCKET
        if (websock)
        {
            // set clients resource function
            //
            result = http_add_func_resource(&resources, schemeWS, "/", NULL, echo_client_callback, NULL);
            http_client_set_websocket_key(client, NULL, NULL);
        }
#endif
        do
        {
            result = http_client_request(
                                        client,
                                        method,
                                        url,
                                        httpTCP,
                                        false,
                                        websock ? NULL : "local.html",
                                        resources
                                        );

            while (! result)
            {
                result = http_client_slice(client);
                if (result)
                {
                    HTTP_ERROR("Client error");
                    break;
                }
                if (client->state == httpDone)
                {
                    http_log(3, "http client complete\n");
                    break;
                }
                to_secs = busy ? 0 : 0;
                to_usecs = busy ? 0 : 10000;
                result = http_wait_for_client_event(client, to_secs, to_usecs);
                if (result < 0)
                {
                    break;
                }
                result = 0;
            }
        }
        while (! result && client->response >= 300 && client->response < 400);

        http_log(1, "client ends\n");
        http_client_free(client);
    }
    return result;
}

