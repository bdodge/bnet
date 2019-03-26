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
#include "bippattr.h"
#include "bipplib.h"

static uint16_t s_ipp_port = 6310;

static int usage (const char *program)
{
    fprintf(stderr, "Use: %s [-lpu]\n", program);
    fprintf(stderr, "     -u    Run unit tests\n");
    fprintf(stderr, "     -l    Set debug log level (default 1: errors/warnings only)\n");
    fprintf(stderr, "     -p    Use port number (default %u)\n", s_ipp_port);
    return 1;
}

int main(int argc, char **argv)
{
    const char *program;
    int result;
    uint32_t uval;
    uint16_t port;
    int loglevel;
    bool s_unit_test;
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

    loglevel = 5;
    butil_set_log_level(loglevel);

    port = s_ipp_port;
    s_unit_test = false;
    port = 6310;

    while (argc > 0 && ! result)
    {
        arg = *argv;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 'l':
            case 'p':
            {
                if (arg[2] == '\0')
                {
                    if (argc > 0)
                    {
                        argc--;
                        argv++;
                        uval = strtoul(*argv, NULL, 0);
                    }
                    else
                    {
                        return usage(program);
                    }
                }
                else
                {
                    uval = strtoul((arg + 2), NULL, 0);
                }
                if (arg[1] == 'l')
                {
                    loglevel = uval;
                }
                else if (arg[1] == 'p')
                {
                    port = uval;
                }
                else
                {
                    return usage(program);
                }
                break;
            }
            case 'u':
                s_unit_test = true;
                break;
            default:
                fprintf(stderr, "Bad Switch: %s\n", arg);
                break;
            }
            argc--;
            argv++;
        }
        else
        {
            return usage(program);
        }
    }
    butil_set_log_level(loglevel);

    if (s_unit_test)
    {
        // unit test
        result = test_find_attr_rec();
        result |= test_set_get_string_attr();
        return result;
    }
    else
    {
        return ipp_server(program, port, false);
    }
}

