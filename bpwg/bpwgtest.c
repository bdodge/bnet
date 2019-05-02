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
#include "bpwg.h"
#include "bpwgunits.h"

int pwg_rip_file(const char *fname, int iosize)
{
    pwg_context_t spwg;
    iostream_t *fs;
    ioring_t io;
    int room;
    int nread;
    int navail;
    int ntook;
    int result;

    result = pwg_init_context(&spwg);
    if (result)
    {
        butil_log(0, "Can't init context\n");
        return result;
    }
    fs = iostream_create_reader_from_file(fname);
    if (! fs)
    {
        butil_log(0, "Can't open %s\n", fname);
        return -1;
    }
    io.head = 0;
    io.tail = 0;
    io.count = 0;
    io.size = iosize;
    io.data = (uint8_t*)malloc(iosize);
    if (! io.data)
    {
        butil_log(0, "Can't alloc io\n");
        return -1;
    }
    do
    {
        room = io.size - io.count;
        if (room > (io.size / 2))
        {
            result = fs->poll(fs, readable, 0, 50000);
            if (result < 0)
            {
                break;
            }
            if (result > 0)
            {
                iostream_normalize_ring(&io, NULL);
                room = io.size - io.head;

                nread = fs->read(fs, io.data + io.head, room);
                if (nread < 0)
                {
                    butil_log(1, "Can't read file\n");
                    result = -1;
                    break;
                }
                if (nread == 0)
                {
                    butil_log(1, "End of file\n");
                    result = 0;
                    if (io.count == 0)
                    break;
                }
                io.count += nread;
                io.head += nread;
                if (io.head >= io.size)
                {
                    io.head = 0;
                }
            }
        }
        while (io.count > 0)
        {
            if (io.head > io.tail)
            {
                navail = io.head - io.tail;
            }
            else
            {
                navail = io.size - io.tail;
            }
            result = pwg_slice(&spwg, io.data + io.tail, &navail);
            if (result)
            {
                break;
            }
            ntook = navail;

            //butil_log(5, "slice took %zu of %zu\n", navail, nread - ntook);

            if (ntook == 0)
            {
                break;
            }
            io.count -= ntook;
            io.tail += ntook;
            if (io.tail >= io.size)
            {
                io.tail = 0;
            }
        }
        if (result)
        {
            break;
        }
    }
    while (! result);

    free(io.data);
    fs->close(fs);
    return result;
}

static int usage (const char *program)
{
    fprintf(stderr, "Use: %s [-l<N>] <filename>\n", program);
    fprintf(stderr, "     -l    Set debug log level to N (default 1: errors/warnings only)\n");
    return 1;
}

int main(int argc, char **argv)
{
    char *arg;
    char *program;
    char *testfile;
    uint32_t uval;
    int loglevel;
    int result;

#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        BERROR("WSAStartup failed");
        return -1;
    }
#else
    signal(SIGPIPE, SIG_IGN);
    result = 0;
#endif
    loglevel = 5;
    butil_set_log_level(loglevel);

    program = *argv++;
    argc--;

    if (sizeof(pwg_header_t) != 1796)
    {
        pwg_header_t hdr;

        butil_log(0, "Header size wrong: %u\n", sizeof(pwg_header_t));
        butil_log(1, " off CutMedia=%u\n", (char*)&hdr.CutMedia - (char*)&hdr);
        butil_log(1, " off InsertSheet=%u\n", (char*)&hdr.InsertSheet - (char*)&hdr);
        butil_log(1, " off Orientation=%u\n", (char*)&hdr.Orientation - (char*)&hdr);
    }
    while (argc > 0 && ! result)
    {
        arg = *argv;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 'l':
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
                    butil_set_log_level(loglevel);
                }
                else
                {
                    return usage(program);
                }
                break;
            }
            default:
                fprintf(stderr, "Bad Switch: %s\n", arg);
                break;
            }
            argc--;
            argv++;
        }
        else
        {
            int iosize;

            testfile = *argv++;
            argc--;

            for (result = 0, iosize = 10; iosize < 2000000; iosize += 12345)
            {
                result = pwg_rip_file(testfile, iosize);
            }
        }
    }
    return result;
}

