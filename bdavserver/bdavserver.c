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
#include "bxml.h"

int usage(const char *prgname)
{
    printf("Usage %s [-l loglevel][-p port][-c cred][-t] <root path>\n"
           "   -l   level   console log verbosity 0-10\n"
           "   -p   port    port to serve on\n"
           "   -c   cred    XML file containing credentials such as:\n"
           "                   <credentials><user>user</user><password>password</password></credentials>\n"
           "   -t           use TLS\n\n",
           prgname);
    return -1;
}

int error(const char *prgname, const char *msg)
{
    fprintf(stderr, "%s Error: %s\n", prgname, msg);
    return -1;
}

char username[64];
char password[64];

int main(int argc, char **argv)
{
    uint16_t port;
    char root[HTTP_MAX_PATH];
    char credfile[HTTP_MAX_PATH];
    const char *progname, *arg;
    int loglevel = 5;
    bool secure;
    int argdex;
    int result;

#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0)
    {
        return error(progname, "WSAStartup failed");
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    butil_set_log_level(loglevel);

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

    credfile[0] = '\0';
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
            argdex = 1;
            
            while (arg[argdex] != '\0')
            {
                switch (arg[argdex++])
                {
                case 't':
                    secure = true;
                    break;
                case 'p':
                    if (arg[argdex] >= '0' && arg[argdex] <= '9')
                    {
                        port = strtoul(arg + argdex, NULL, 0);
                        while (arg[argdex] != '\0')
                        {
                            argdex++;
                        }
                    }
                    else if (argc > 0)
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
                    if (arg[argdex] >= '0' && arg[argdex] <= '9')
                    {
                        loglevel = strtoul(arg + argdex, NULL, 0);
                        while (arg[argdex] != '\0')
                        {
                            argdex++;
                        }
                    }
                    else if (argc > 0)
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
                case 'c':
                    if (argc > 0)
                    {
                        strncpy(credfile, *argv, sizeof(credfile) - 1);
                        root[sizeof(credfile) - 1] = '\0';
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
    username[0] = '\0';
    password[0] = '\0';
    dav_creds.user = username;
    dav_creds.pass = password;
    dav_creds.type = httpAuthNone;

    if (credfile[0])
    {
        char filebuffer[128];
        int cf;
        
        cf = open(credfile, O_RDONLY);
        if (cf < 0)
        {
            return error(progname, "Can't open credential file\n");
        }
        result = read(cf, filebuffer, sizeof(filebuffer) - 1);

        close(cf);
        
        if (result < 0)
        {
            return error(progname, "Can't read credential file\n");
        }
        filebuffer[result] = '\0';
        
        result = bxml_find_and_copy_nth_element(
                                filebuffer,
                                "credentials$user",
                                '$',
                                0,
                                username,
                                sizeof(username),
                                false,
                                false
                                );
        if (result)
        {
            return error(progname, "No user element in credentials file\n");
        }
        result |= bxml_find_and_copy_nth_element(
                                filebuffer,
                                "credentials$password",
                                '$',
                                0,
                                password,
                                sizeof(password),
                                false,
                                false
                                );
        if (result)
        {
            return error(progname, "No password element in credentials file\n");
        }
        dav_creds.type = httpAuthBasic;
    }
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

