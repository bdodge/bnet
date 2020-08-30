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

int usage(const char *prgname)
{
    printf("Usage %s [-l loglevel][-p port][-t] <root path>\n"
           "   -l   level   console log verbosity 0-10\n"
           "   -p   port    port to serve on\n"
           "   -t           use TLS\n\n",
           prgname);
    return -1;
}

int error(const char *prgname, const char *msg)
{
    fprintf(stderr, "%s Error: %s\n", prgname, msg);
    return -1;
}

int main(int argc, char **argv)
{
    uint16_t port;
    char root[HTTP_MAX_PATH];
    const char *progname, *arg;
    int loglevel = 5;
    bool secure;
    int result;

#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        return error(progname, "WSAStartup failed");
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    http_set_log_level(loglevel);

#if HTTP_SUPPORT_TLS
    result = iostream_tls_prolog();
    if (result)
    {
        return error(progname, "Can't init TLS");
    }
#endif

    progname = *argv++;
    argc--;

    loglevel = 3;

    snprintf(root, sizeof(root), ".%c", HTTP_LOCAL_PATH_CHAR);
    port = 8080;
    secure = false;
    result = 0;

    while (argc > 0 && ! result)
    {
        arg = *argv++;
        argc--;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 't':
                secure = true;
                break;
            case 'p':
                if (argc > 0)
                {
                    port = strtoul(*argv, NULL, 0);
                    argc--;
                    argv++;
                }
                else
                {
                    return usage(progname);
                }
                break;
            case 'l':
                if (argc > 0)
                {
                    loglevel = strtoul(*argv, NULL, 0);
                    argc--;
                    argv++;
                }
                else
                {
                    return usage(progname);
                }
                break;
            default:
                return usage(progname);
            }
        }
        else
        {
            strncpy(root, arg, sizeof(root) - 1);
            root[sizeof(root) - 1] = '\0';
        }
    }
    http_set_log_level(loglevel);
    
    http_server_t server;
    http_resource_t *resource, *resources = NULL;
    http_credentials_t dav_creds;

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
    result = http_add_dav_resource(&resources, "/", root, &dav_creds);
    if (result)
    {
        return error(progname, "Can't make resource");
    }
    butil_log(2, "Serving DAV at %s on %sport %u\n", root, secure ? "secure " : "", port);
    
    result = http_server_init(
                                &server,
                                resources,
                                port,
                                httpTCP,
                                HTTP_MAX_CLIENT_CONNECTIONS,
                                secure
                            );
    if (result)
    {
        return error(progname, "Can't start server");
    }
    result = http_serve(&server, NULL, NULL);
    if (result)
    {
        butil_log(2, "Server on port %u ends\n", server.port);
    }
    http_server_cleanup(&server);
    return result;
}

