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
#include "bimageio.h"

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
            image_stream_t *istream;
            image_stream_t *ostream;
            image_t outimg;
            mime_content_type_t type;
            uint32_t y;

            static uint8_t pixels[4096 * 4];

            type = butil_content_type_for_file(*argv);
            if (strstr(*argv, ".pwg"))
                type = 0x1234;
            result = image_open_file_reader(*argv, type, &istream);
            if (result)
            {
                fprintf(stderr, "Can't open %s\n", *argv);
                return -1;
            }
            outimg.width  = istream->img->width;
            outimg.height = istream->img->height;
            outimg.depth  = istream->img->depth;
            outimg.stride = istream->img->stride;
            outimg.format = istream->img->format;

            result = image_open_file_writer("out.png", butil_mime_png, &outimg, &ostream);
            if (result)
            {
                fprintf(stderr, "Can't open %s\n", "out.png");
                return -1;
            }
            for (y = 0; y < istream->img->height; y++)
            {
                result = istream->read(istream, pixels, sizeof(pixels));
                if (result)
                {
                    fprintf(stderr, "Can't read line %u\n", y);
                    return -1;
                }
                result = ostream->write(ostream, pixels, ostream->img->stride);
                if (result)
                {
                    fprintf(stderr, "Can't write line %u\n", y);
                    return -1;
                }
            }
            istream->close(istream);
            ostream->close(ostream);
            argv++;
            argc--;
        }
    }
    return result;
}

