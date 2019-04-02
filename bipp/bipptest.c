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

static uint16_t s_ipp_open_port = 6310;
static uint16_t s_ipp_secure_port = 6311;

static int usage (const char *program)
{
    fprintf(stderr, "Use: %s [-lpu]\n", program);
    fprintf(stderr, "     -u    Run unit tests\n");
    fprintf(stderr, "     -l    Set debug log level (default 1: errors/warnings only)\n");
    fprintf(stderr, "     -p    Use open port number (default %u), 0 to disable open port\n", s_ipp_open_port);
    fprintf(stderr, "     -s    Use TLS port number (default %u), 0 to disable TLS port\n", s_ipp_secure_port);
    return 1;
}

int main(int argc, char **argv)
{
    const char *program;
    int result;
    uint32_t uval;
    uint16_t open_port;
    uint16_t secure_port;
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

    open_port = s_ipp_open_port;
    secure_port = s_ipp_secure_port;
    s_unit_test = false;

    while (argc > 0 && ! result)
    {
        arg = *argv;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 'l':
            case 'p':
            case 's':
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
                    open_port = uval;
                }
                else if (arg[1] == 's')
                {
                    secure_port = uval;
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
        result |= test_find_xref_rec();
        return result;
    }
    else
    {
#ifdef BMEM_H
        static uint8_t g_spool1[0x100000];
        static uint8_t g_spool2[0x100000];
        static uint8_t g_mpool[0x100000];
        static uint8_t g_lpool[0x100000];

        void *pool;
        size_t pool_size, pool_chunk;
        size_t size;
        int result;

        result = bmem_init();
        if (result)
        {
            BERROR("Can't init pool\n");
            return result;
        }
        pool_size = sizeof(g_spool1);
        pool_chunk = 32;
        pool = g_spool1;
        result = bmem_add_pool(pool, pool_size, pool_chunk);
        if (result)
        {
            BERROR("Can't add small pool 1\n");
            return result;
        }
        pool_size = sizeof(g_spool2);
        pool_chunk = 64;
        pool = g_spool2;
        result = bmem_add_pool(pool, pool_size, pool_chunk);
        if (result)
        {
            BERROR("Can't add small pool 2\n");
            return result;
        }
        pool_size = sizeof(g_mpool);
        pool_chunk = 512;
        pool = g_mpool;
        result = bmem_add_pool(pool, pool_size, pool_chunk);
        if (result)
        {
            BERROR("Can't add medium pool\n");
            return result;
        }
        pool_size = sizeof(g_lpool);
        pool_chunk = 16384;
        pool = g_lpool;
        result = bmem_add_pool(pool, pool_size, pool_chunk);
        if (result)
        {
            BERROR("Can't add large pool\n");
            return result;
        }
#endif
        result = ipp_server(program, open_port, secure_port, "./printout");

        #ifdef BMEM_H
        bmem_stats(5);
        bmem_deinit();
        #endif
        return result;
    }
}

