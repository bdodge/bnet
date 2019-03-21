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
#include "bipp.h"
#include "bipplib.h"

int main(int argc, char **argv)
{
    const char *program;
    int result;
    uint16_t port;
    char *arg;

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
    program = *argv++;
    argc--;

    port = 6310;

    while (argc > 0 && ! result)
    {
        arg = *argv++;
        argc--;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 'p':
                if (argc > 0)
                {
                    port = strtoul(*argv, NULL, 0);
                    argv++;
                    argc--;
                }
                else
                {
                    fprintf(stderr, "Use: -p [port]");
                }
                break;
            default:
                fprintf(stderr, "Bad Switch: %s\n", arg);
                break;
            }
        }
        else
        {
            printf("Ignore parm %s\n", arg);
        }
    }
    return ipp_server(program, port, false);
}

