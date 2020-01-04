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
#include "bimgpwg.h"
#include "bimageio.h"

typedef enum
{
    pwgSync,
    pwgHeader,
    pwgPage,
    pwgLine,
}
pwg_state_t;

typedef enum
{
    pwgLineLine,
    pwgLineRun,
    pwgLineRepeatColor,
    pwgLineSoloColor,
    pwgLineDone,
}
pwg_line_state_t;

struct tag_pwg_context;

typedef int (*pwg_line_decoder_t)(
                                    struct tag_pwg_context *pwg,
                                    uint8_t *data,
                                    int *ndata,
                                    bool *done
                                );

typedef struct tag_pwg_context
{
    pwg_state_t         state;
    pwg_line_state_t    line_state;
    pwg_header_t        hdr;
    pwg_line_decoder_t  decoder;
    int                 pages;
    int                 rows;
    int                 pixels;
    int                 pageno;
    int                 lineno;
    int                 pixelno;
    int                 bytes_left;
    int                 bytes_gotten;
    int                 line_repeat;
    int                 color_repeat;
    int                 out_depth;
    int                 out_components;
    bool                at_eof;
    uint8_t             iobuf[PWG_IO_SIZE];
    ioring_t            io;
    uint8_t            *line;
    uint8_t            *pp;
}
pwg_context_t;

static int pwg_dump_header(pwg_header_t *hdr)
{
    int dlev = 5;

    butil_log(dlev, "PWG Header: %u pages\n", hdr->TotalPageCount);
    butil_log(dlev, "XRES=%u YRES=%u\n", hdr->xres, hdr->yres);
    butil_log(dlev, "W=%u H=%u\n", hdr->PageSize_X, hdr->PageSize_Y);
    butil_log(dlev, "BitsPC=%u BipsPP=%u\n", hdr->BitsPerColor, hdr->BitsPerPixel);
    butil_log(dlev, "BytesPerLine=%u\n", hdr->BytesPerLine);
    return 0;
}

static int pwg_byteswap_header(pwg_header_t *hdr)
{
    hdr->TotalPageCount = htonl(hdr->TotalPageCount);
    hdr->Width = htonl(hdr->Width);
    hdr->Height = htonl(hdr->Height);
    hdr->xres = htonl(hdr->xres);
    hdr->yres = htonl(hdr->yres);
    hdr->PageSize_X = htonl(hdr->PageSize_X);
    hdr->PageSize_Y = htonl(hdr->PageSize_Y);
    hdr->BitsPerColor = htonl(hdr->BitsPerColor);
    hdr->BitsPerPixel = htonl(hdr->BitsPerPixel);
    hdr->BytesPerLine = htonl(hdr->BytesPerLine);
    return 0;
}

int pwg_decode_1x1(pwg_context_t *pwg, uint8_t *data, int *ndata, bool *done)
{
    int avail;
    int took;
    int i;

    *done = false;
    if (*ndata < pwg->bytes_left)
    {
        // wait for bytes neede
        *ndata = 0;
        return 0;
    }
    switch (pwg->line_state)
    {
    case pwgLineLine:
        pwg->pixelno = 0;
        pwg->line_repeat = 1 + (uint8_t)*data;
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        *ndata = 1;
        break;
    case pwgLineRun:
        pwg->color_repeat = (uint8_t)*data;
        if (pwg->color_repeat > 127)
        {
            pwg->color_repeat = 257 - pwg->color_repeat;
            pwg->line_state = pwgLineSoloColor;
        }
        else
        {
            pwg->color_repeat += 1;
            pwg->line_state = pwgLineRepeatColor;
        }
        pwg->bytes_left = 1;
        *ndata = 1;
        break;
    case pwgLineRepeatColor:
        *ndata = 1;
        for (i = 0; i < pwg->color_repeat; i++)
        {
            *pwg->pp++ = data[0];
        }
        pwg->pixelno += 8 * pwg->color_repeat;
        if (pwg->pixelno >= pwg->pixels)
        {
            if (pwg->pixelno != ((pwg->pixels + 7) & ~7))
            {
                butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                return -1;
            }
            *done = true;
            pwg->lineno+= pwg->line_repeat;
            pwg->line_state = pwgLineDone;
            pwg->bytes_left = 1;
            break;
        }
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        break;
    case pwgLineSoloColor:
        avail = *ndata;
        took  = 0;
        while (took < avail && pwg->color_repeat > 0)
        {
            *pwg->pp++ = data[took];
            took++;
            pwg->color_repeat--;
            pwg->pixelno+= 8;
            if (pwg->pixelno >= pwg->pixels)
            {
                if (pwg->pixelno != ((pwg->pixels + 7) & ~7))
                {
                    butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                    return -1;
                }
                if (pwg->color_repeat != 0)
                {
                    butil_log(2, "Corrupt? more colors: %u\n", pwg->color_repeat);
                    return -1;
                }
                *done = true;
                pwg->lineno+= pwg->line_repeat;
                pwg->line_state = pwgLineDone;
                pwg->bytes_left = 1;
                break;
            }
            if (pwg->color_repeat == 0)
            {
                pwg->line_state = pwgLineRun;
                pwg->bytes_left = 1;
                break;
            }
            pwg->bytes_left = 1;
        }
        *ndata = took;
        break;
    case pwgLineDone:
        pwg->line_state = pwgLineLine;
        *ndata = 0;
        break;
    }
    return 0;
}

int pwg_decode_1x1x1(pwg_context_t *pwg, uint8_t *data, int *ndata, bool *done)
{
    int avail;
    int took;
    int i;

    *done = false;
    if (*ndata < pwg->bytes_left)
    {
        // wait for bytes neede
        *ndata = 0;
        return 0;
    }
    switch (pwg->line_state)
    {
    case pwgLineLine:
        pwg->pixelno = 0;
        pwg->line_repeat = 1 + (uint8_t)*data;
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        *ndata = 1;
        break;
    case pwgLineRun:
        pwg->color_repeat = (uint8_t)*data;
        if (pwg->color_repeat > 127)
        {
            pwg->color_repeat = 257 - pwg->color_repeat;
            pwg->line_state = pwgLineSoloColor;
        }
        else
        {
            pwg->color_repeat += 1;
            pwg->line_state = pwgLineRepeatColor;
        }
        pwg->bytes_left = 1;
        *ndata = 1;
        break;
    case pwgLineRepeatColor:
        *ndata = 1;
        for (i = 0; i < pwg->color_repeat; i++)
        {
            *pwg->pp++ = (data[0] & 0x80) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x40) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x20) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x10) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x8) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x4) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x2) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x1) ? 0x00 : 0xFF;
        }
        pwg->pixelno += 8 * pwg->color_repeat;
        if (pwg->pixelno >= pwg->pixels)
        {
            if (pwg->pixelno != ((pwg->pixels + 7) & ~7))
            {
                butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                return -1;
            }
            *done = true;
            pwg->lineno+= pwg->line_repeat;
            pwg->line_state = pwgLineDone;
            pwg->bytes_left = 1;
            break;
        }
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        break;
    case pwgLineSoloColor:
        avail = *ndata;
        took  = 0;
        while (took < avail && pwg->color_repeat > 0)
        {
            *pwg->pp++ = (data[took] & 0x80) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x40) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x20) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x10) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x8) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x4) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x2) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x1) ? 0x00 : 0xFF;
            took++;
            pwg->color_repeat--;
            pwg->pixelno+= 8;
            if (pwg->pixelno >= pwg->pixels)
            {
                if (pwg->pixelno != ((pwg->pixels + 7) & ~7))
                {
                    butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                    return -1;
                }
                if (pwg->color_repeat != 0)
                {
                    butil_log(2, "Corrupt? more colors: %u\n", pwg->color_repeat);
                    return -1;
                }
                *done = true;
                pwg->lineno+= pwg->line_repeat;
                pwg->line_state = pwgLineDone;
                pwg->bytes_left = 1;
                break;
            }
            if (pwg->color_repeat == 0)
            {
                pwg->line_state = pwgLineRun;
                pwg->bytes_left = 1;
                break;
            }
            pwg->bytes_left = 1;
        }
        *ndata = took;
        break;
    case pwgLineDone:
        pwg->line_state = pwgLineLine;
        *ndata = 0;
        break;
    }
    return 0;
}

int pwg_decode_1x1x3(pwg_context_t *pwg, uint8_t *data, int *ndata, bool *done)
{
    int avail;
    int took;
    int i;

    *done = false;
    if (*ndata < pwg->bytes_left)
    {
        // wait for bytes needed
        *ndata = 0;
        return 0;
    }
    switch (pwg->line_state)
    {
    case pwgLineLine:
        pwg->pixelno = 0;
        pwg->line_repeat = 1 + (uint8_t)*data;
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        *ndata = 1;
        break;
    case pwgLineRun:
        pwg->color_repeat = (uint8_t)*data;
        if (pwg->color_repeat > 127)
        {
            pwg->color_repeat = 257 - pwg->color_repeat;
            pwg->line_state = pwgLineSoloColor;
        }
        else
        {
            pwg->color_repeat += 1;
            pwg->line_state = pwgLineRepeatColor;
        }
        pwg->bytes_left = 1;
        *ndata = 1;
        break;
    case pwgLineRepeatColor:
        *ndata = 1;
        for (i = 0; i < pwg->color_repeat; i++)
        {
            *pwg->pp++ = (data[0] & 0x80) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x80) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x80) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x40) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x40) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x40) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x20) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x20) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x20) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x10) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x10) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x10) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x8) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x8) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x8) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x4) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x4) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x4) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x2) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x2) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x2) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x1) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x1) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[0] & 0x1) ? 0x00 : 0xFF;
        }
        pwg->pixelno += 8 * pwg->color_repeat;
        if (pwg->pixelno >= pwg->pixels)
        {
            if (pwg->pixelno != ((pwg->pixels + 7) & ~7))
            {
                butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                return -1;
            }
            *done = true;
            pwg->lineno+= pwg->line_repeat;
            pwg->line_state = pwgLineDone;
            pwg->bytes_left = 1;
            break;
        }
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        break;
    case pwgLineSoloColor:
        avail = *ndata;
        took  = 0;
        while (took < avail && pwg->color_repeat > 0)
        {
            *pwg->pp++ = (data[took] & 0x80) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x80) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x80) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x40) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x40) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x40) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x20) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x20) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x20) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x10) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x10) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x10) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x8) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x8) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x8) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x4) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x4) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x4) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x2) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x2) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x2) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x1) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x1) ? 0x00 : 0xFF;
            *pwg->pp++ = (data[took] & 0x1) ? 0x00 : 0xFF;

            took++;
            pwg->color_repeat--;
            pwg->pixelno+= 8;
            if (pwg->pixelno >= pwg->pixels)
            {
                if (pwg->pixelno != ((pwg->pixels + 7) & ~7))
                {
                    butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                    return -1;
                }
                if (pwg->color_repeat != 0)
                {
                    butil_log(2, "Corrupt? more colors: %u\n", pwg->color_repeat);
                    return -1;
                }
                *done = true;
                pwg->lineno+= pwg->line_repeat;
                pwg->line_state = pwgLineDone;
                pwg->bytes_left = 1;
                break;
            }
            if (pwg->color_repeat == 0)
            {
                pwg->line_state = pwgLineRun;
                pwg->bytes_left = 1;
                break;
            }
            pwg->bytes_left = 1;
        }
        *ndata = took;
        break;
    case pwgLineDone:
        pwg->line_state = pwgLineLine;
        *ndata = 0;
        break;
    }
    return 0;
}

int pwg_decode_1x8(pwg_context_t *pwg, uint8_t *data, int *ndata, bool *done)
{
    int avail;
    int took;
    int i;

    *done = false;
    if (*ndata < pwg->bytes_left)
    {
        // wait for bytes neede
        *ndata = 0;
        return 0;
    }
    switch (pwg->line_state)
    {
    case pwgLineLine:
        pwg->pixelno = 0;
        pwg->line_repeat = 1 + (uint8_t)*data;
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        *ndata = 1;
        break;
    case pwgLineRun:
        pwg->color_repeat = (uint8_t)*data;
        if (pwg->color_repeat > 127)
        {
            pwg->color_repeat = 257 - pwg->color_repeat;
            pwg->line_state = pwgLineSoloColor;
        }
        else
        {
            pwg->color_repeat += 1;
            pwg->line_state = pwgLineRepeatColor;
        }
        pwg->bytes_left = 3;
        *ndata = 1;
        break;
    case pwgLineRepeatColor:
        *ndata = 1;
        for (i = 0; i < pwg->color_repeat; i++)
        {
            *pwg->pp++ = data[0];
        }
        pwg->pixelno += pwg->color_repeat;
        if (pwg->pixelno >= pwg->pixels)
        {
            if (pwg->pixelno != pwg->pixels)
            {
                butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                return -1;
            }
            *done = true;
            pwg->lineno+= pwg->line_repeat;
            pwg->line_state = pwgLineDone;
            pwg->bytes_left = 1;
            break;
        }
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        break;
    case pwgLineSoloColor:
        avail = *ndata;
        took  = 0;
        while (took < avail && pwg->color_repeat > 0)
        {
            *pwg->pp++ = data[0];
            took += 1;
            pwg->color_repeat--;
            pwg->pixelno++;
            if (pwg->pixelno >= pwg->pixels)
            {
                if (pwg->pixelno != pwg->pixels)
                {
                    butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                    return -1;
                }
                if (pwg->color_repeat != 0)
                {
                    butil_log(2, "Corrupt? more colors: %u\n", pwg->color_repeat);
                    return -1;
                }
                *done = true;
                pwg->lineno+= pwg->line_repeat;
                pwg->line_state = pwgLineDone;
                pwg->bytes_left = 1;
                break;
            }
            if (pwg->color_repeat == 0)
            {
                pwg->line_state = pwgLineRun;
                pwg->bytes_left = 1;
                break;
            }
            pwg->bytes_left = 3;
        }
        *ndata = took;
        break;
    case pwgLineDone:
        pwg->line_state = pwgLineLine;
        *ndata = 0;
        break;
    }
    return 0;
}

int pwg_decode_1x8x3(pwg_context_t *pwg, uint8_t *data, int *ndata, bool *done)
{
    int avail;
    int took;
    int i;

    *done = false;
    if (*ndata < pwg->bytes_left)
    {
        // wait for bytes neede
        *ndata = 0;
        return 0;
    }
    switch (pwg->line_state)
    {
    case pwgLineLine:
        pwg->pixelno = 0;
        pwg->line_repeat = 1 + (uint8_t)*data;
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        *ndata = 1;
        break;
    case pwgLineRun:
        pwg->color_repeat = (uint8_t)*data;
        if (pwg->color_repeat > 127)
        {
            pwg->color_repeat = 257 - pwg->color_repeat;
            pwg->line_state = pwgLineSoloColor;
        }
        else
        {
            pwg->color_repeat += 1;
            pwg->line_state = pwgLineRepeatColor;
        }
        pwg->bytes_left = 1;
        *ndata = 1;
        break;
    case pwgLineRepeatColor:
        *ndata = 1;
        for (i = 0; i < pwg->color_repeat; i++)
        {
            *pwg->pp++ = data[0];
            *pwg->pp++ = data[0];
            *pwg->pp++ = data[0];
        }
        pwg->pixelno += pwg->color_repeat;
        if (pwg->pixelno >= pwg->pixels)
        {
            if (pwg->pixelno != pwg->pixels)
            {
                butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                return -1;
            }
            *done = true;
            pwg->lineno+= pwg->line_repeat;
            pwg->line_state = pwgLineDone;
            pwg->bytes_left = 1;
            break;
        }
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        break;
    case pwgLineSoloColor:
        avail = *ndata;
        took  = 0;
        while (took < avail && pwg->color_repeat > 0)
        {
            *pwg->pp++ = data[took];
            *pwg->pp++ = data[took];
            *pwg->pp++ = data[took];
            took += 1;
            pwg->color_repeat--;
            pwg->pixelno++;
            if (pwg->pixelno >= pwg->pixels)
            {
                if (pwg->pixelno != pwg->pixels)
                {
                    butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                    return -1;
                }
                if (pwg->color_repeat != 0)
                {
                    butil_log(2, "Corrupt? more colors: %u\n", pwg->color_repeat);
                    return -1;
                }
                *done = true;
                pwg->lineno+= pwg->line_repeat;
                pwg->line_state = pwgLineDone;
                pwg->bytes_left = 1;
                break;
            }
            if (pwg->color_repeat == 0)
            {
                pwg->line_state = pwgLineRun;
                pwg->bytes_left = 1;
                break;
            }
            pwg->bytes_left = 3;
        }
        *ndata = took;
        break;
    case pwgLineDone:
        pwg->line_state = pwgLineLine;
        *ndata = 0;
        break;
    }
    return 0;
}

int pwg_decode_3x8(pwg_context_t *pwg, uint8_t *data, int *ndata, bool *done)
{
    int avail;
    int took;
    int i;

    *done = false;
    if (*ndata < pwg->bytes_left)
    {
        // wait for bytes neede
        *ndata = 0;
        return 0;
    }
    switch (pwg->line_state)
    {
    case pwgLineLine:
        pwg->pixelno = 0;
        pwg->line_repeat = 1 + (uint8_t)*data;
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        *ndata = 1;
        break;
    case pwgLineRun:
        pwg->color_repeat = (uint8_t)*data;
        if (pwg->color_repeat > 127)
        {
            pwg->color_repeat = 257 - pwg->color_repeat;
            pwg->line_state = pwgLineSoloColor;
        }
        else
        {
            pwg->color_repeat += 1;
            pwg->line_state = pwgLineRepeatColor;
        }
        pwg->bytes_left = 3;
        *ndata = 1;
        break;
    case pwgLineRepeatColor:
        *ndata = 3;
        if (pwg->line && pwg->pp)
        {
            for (i = 0; i < pwg->color_repeat; i++)
            {
                *pwg->pp++ = data[0];
                *pwg->pp++ = data[1];
                *pwg->pp++ = data[2];
            }
        }
        pwg->pixelno += pwg->color_repeat;
        if (pwg->pixelno >= pwg->pixels)
        {
            if (pwg->pixelno != pwg->pixels)
            {
                butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                return -1;
            }
            *done = true;
            pwg->lineno+= pwg->line_repeat;
            pwg->line_state = pwgLineDone;
            pwg->bytes_left = 1;
            break;
        }
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        break;
    case pwgLineSoloColor:
        avail = *ndata;
        took  = 0;
        while (((avail - took) > 2) && pwg->color_repeat > 0)
        {
            took += 3;
            if (pwg->line && pwg->pp)
            {
                for (i = 0; i < pwg->color_repeat; i++)
                {
                    *pwg->pp++ = data[0];
                    *pwg->pp++ = data[1];
                    *pwg->pp++ = data[2];
                }
            }
            pwg->color_repeat--;
            pwg->pixelno++;
            if (pwg->pixelno >= pwg->pixels)
            {
                if (pwg->pixelno != pwg->pixels)
                {
                    butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                    return -1;
                }
                if (pwg->color_repeat != 0)
                {
                    butil_log(2, "Corrupt? more colors: %u\n", pwg->color_repeat);
                    return -1;
                }
                *done = true;
                pwg->lineno+= pwg->line_repeat;
                pwg->line_state = pwgLineDone;
                pwg->bytes_left = 1;
                break;
            }
            if (pwg->color_repeat == 0)
            {
                pwg->line_state = pwgLineRun;
                pwg->bytes_left = 1;
                break;
            }
            pwg->bytes_left = 3;
        }
        *ndata = took;
        break;
    case pwgLineDone:
        pwg->line_state = pwgLineLine;
        *ndata = 0;
        break;
    }
    return 0;
}

static int pwg_process_input(pwg_context_t *pwg, iostream_t *stream)
{
    int room;
    int nread;
    int result;

    room = pwg->io.size - pwg->io.count;
    if (room > (pwg->io.size / 2))
    {
        result = stream->poll(stream, readable, 0, 50000);
        if (result < 0)
        {
            return result;
        }
        if (result > 0)
        {
            iostream_normalize_ring(&pwg->io, NULL);
            room = pwg->io.size - pwg->io.head;

            nread = stream->read(stream, pwg->io.data + pwg->io.head, room);
            if (nread < 0)
            {
                butil_log(1, "Can't read file\n");
                result = -1;
                return result;
            }
            if (nread == 0)
            {
                pwg->at_eof = true;
                result = 0;
                if (pwg->io.count == 0)
                {
                    butil_log(1, "End of file\n");
                    return 0;
                }
            }
            pwg->io.count += nread;
            pwg->io.head += nread;
            if (pwg->io.head >= pwg->io.size)
            {
                pwg->io.head = 0;
            }
        }
    }
    return 0;
}

int pwg_slice(pwg_context_t *pwg, iostream_t *stream)
{
    uint8_t *data;
    int ndata;
    int ntook;
    int chunk;
    bool done;
    int result;

    if (! pwg)
    {
        return -1;
    }
    ntook = 0;
    ndata = pwg->io.count;
    data  = pwg->io.data + pwg->io.tail;

    if (pwg->io.count < pwg->bytes_left && pwg->at_eof)
    {
        butil_log(0, "EOF before end of page\n");
        return -1;
    }
    if (pwg->io.count < pwg->bytes_left)
    {
        result = pwg_process_input(pwg, stream);
        if (result)
        {
            return result;
        }
    }
    switch (pwg->state)
    {
    case pwgSync:
        if (strncmp((char*)data, PWG_SYNC_STRING, 4))
        {
            // ignore bytes until sync word
            ntook = 1;
            break;
        }
        ntook = 4;
        pwg->pageno = 0;
        pwg->state = pwgHeader;
        pwg->bytes_left = sizeof(pwg_header_t);
        pwg->bytes_gotten = 0;
        break;
    case pwgHeader:
        chunk = ndata;
        if (chunk > pwg->bytes_left)
        {
            chunk = pwg->bytes_left;
        }
        memcpy((char*)&pwg->hdr + pwg->bytes_gotten, data, chunk);
        pwg->bytes_left -= chunk;
        pwg->bytes_gotten += chunk;
        ntook = chunk;
        if (pwg->bytes_left == 0)
        {
            // byte swap all the uint32s in hdr
            pwg_byteswap_header(&pwg->hdr);
            pwg->state = pwgPage;
            if (pwg->pageno == 0)
            {
                pwg->pages = pwg->hdr.TotalPageCount;
            }
        }
        break;
    case pwgPage:
        butil_log(5, "Page %d of %d\n", pwg->pageno + 1, pwg->pages);
        pwg_dump_header(&pwg->hdr);
        pwg->rows = pwg->hdr.PageSize_Y * pwg->hdr.yres / 72;
        pwg->pixels = pwg->hdr.PageSize_X * pwg->hdr.xres / 72;

        if (pwg->out_depth == 0)
        {
            pwg->out_depth = pwg->hdr.BitsPerPixel;
            pwg->out_components =  pwg->hdr.BitsPerPixel / pwg->hdr.BitsPerColor;
        }
        switch (pwg->hdr.BitsPerColor)
        {
        case 1:
            switch (pwg->hdr.BitsPerPixel)
            {
            case 1:
                if (pwg->out_depth == 8)
                {
                    switch (pwg->out_components)
                    {
                    case 1:
                        pwg->decoder = pwg_decode_1x1x1;
                        break;
                    case 3:
                        pwg->decoder = pwg_decode_1x1x3;
                        break;
                    default:
                        butil_log(0, "Unhandled format\n");
                        return -1;
                    }
                }
                else
                {
                    pwg->decoder = pwg_decode_1x1;
                }
                break;
            default:
                butil_log(0, "Unhandled format\n");
                return -1;
            }
            break;
        case 8:
            switch (pwg->hdr.BitsPerPixel)
            {
            case 8:
                if (pwg->out_components == 3)
                {
                    pwg->decoder = pwg_decode_1x8x3;
                }
                else
                {
                    pwg->decoder = pwg_decode_1x8;
                }
                break;
            case 24:
                pwg->decoder = pwg_decode_3x8;
                break;
            default:
                butil_log(0, "Unhandled format\n");
                return -1;
            }
            break;
        default:
            butil_log(0, "Unhandled format\n");
            return -1;
        }
        pwg->pageno++;
        pwg->lineno = 0;
        pwg->pixels = pwg->hdr.BytesPerLine * 8 / pwg->hdr.BitsPerPixel;
        pwg->state = pwgLine;
        pwg->bytes_left = 1;
        pwg->bytes_gotten = 0;
        break;
    case pwgLine:
        butil_log(8, "state %u l=%u.%u p=%u %u\n",
                pwg->line_state, pwg->lineno, pwg->line_repeat,
                pwg->pixelno, pwg->color_repeat);

        result = pwg->decoder(pwg, data, &ndata, &done);
        if (result)
        {
            return result;
        }
        ntook = ndata;
        if (! done)
        {
            break;
        }
        butil_log(6, "line %zu of %u\n", pwg->lineno, pwg->rows);
        if(pwg->lineno >= pwg->rows)
        {
            butil_log(5, "End of Page %u\n", pwg->pageno);
            if (pwg->pageno >= pwg->pages)
            {
                butil_log(5, "End of Document\n");
                pwg->state = pwgSync;
                break;
            }
            pwg->state = pwgHeader;
            pwg->bytes_left = sizeof(pwg_header_t);
            pwg->bytes_gotten = 0;
            break;
        }
        pwg->bytes_left = 1;
        pwg->bytes_gotten = 0;
        break;
    }
    if (ntook > 0)
    {
        pwg->io.count -= ntook;
        pwg->io.tail += ntook;
        if (pwg->io.tail >= pwg->io.size)
        {
            pwg->io.tail = 0;
        }
    }
    return 0;
}

static int pwg_read_header(iostream_t *stream, pwg_context_t *pwg)
{
    int result;

    if (! stream || ! pwg)
    {
        return -1;
    }
    while (pwg->state != pwgPage)
    {
        result = pwg_slice(pwg, stream);
        if (result)
        {
            butil_log(1, "PWG processing failed\n");
            return result;
        }
    }
    return 0;
}

static int pwg_read_line(iostream_t *stream, pwg_context_t *pwg, uint8_t *line)
{
    int result;

    if (! stream || ! pwg)
    {
        return -1;
    }
    pwg->line = line;
    pwg->pp = line;

    if (pwg->line_state == pwgLineDone)
    {
        if (pwg->line_repeat > 1)
        {
            pwg->line_repeat--;
            return 0;
        }
    }
    do
    {
        result = pwg_slice(pwg, stream);
        if (result)
        {
            butil_log(1, "PWG processing failed\n");
            return result;
        }
        if (pwg->state == pwgPage)
        {
            butil_log(1, "PWG went past end\n");
            return -1;
        }
    }
    while (pwg->line_state != pwgLineDone);
    return 0;
}

int pwg_init_context(pwg_context_t *pwg)
{
    if (! pwg)
    {
        return -1;
    }
    memset(pwg, 0, sizeof(pwg_context_t));
    pwg->io.data = pwg->iobuf;
    pwg->io.size = sizeof(pwg->iobuf);
    pwg->bytes_left = 4;
    pwg->state = pwgSync;
    return 0;
}

int image_open_pwg_reader(image_stream_t *istream)
{
    pwg_context_t *info;
    image_t *img;
    int result;

    if (! istream)
    {
        return -1;
    }
    img = istream->img;
    if (! img)
    {
        butil_log(0, "No image\n");
        return -1;
    }
    info = (pwg_context_t *)malloc(sizeof(pwg_context_t));
    if (! info)
    {
        BERROR("Can't alloc info\n");
        return -1;
    }
    pwg_init_context(info);

    if (img->depth > 0)
    {
        // want our own output depth
        //
        info->out_depth = img->depth;
        info->out_components = (img->format & _IMAGE_RGB) ? 3 : 1;
    }
    else
    {
        info->out_depth = 0;
        info->out_components = 0;
    }
    // set our image stream object into client info
    istream->priv = info;

    result = pwg_read_header(istream->stream, info);
    if (result)
    {
        butil_log(0, "Can't read header\n");
        free(info);
        return -1;
    }
    img->width = info->hdr.Width;
    img->height = info->hdr.Height;
    img->depth = info->out_depth;
    img->components = info->out_components;
    img->stride = (info->out_depth * info->out_components * img->width + 7) / 8; //info->hdr.BytesPerLine;
    img->format = (img->components > 1) ? IMAGE_RGB : IMAGE_GRAY;

    istream->cur_line = 0;
    return 0;
}

int image_open_pwg_writer(image_stream_t *istream)
{
    pwg_context_t *info;
    image_t *img;

    if (! istream)
    {
        return -1;
    }
    img = istream->img;
    if (! img)
    {
        butil_log(0, "No image\n");
        return -1;
    }
    info = (pwg_context_t *)malloc(sizeof(pwg_context_t));
    if (! info)
    {
        BERROR("Can't alloc info\n");
        return -1;
    }
    pwg_init_context(info);
    return 0;
}

int image_open_pwg(image_stream_t *istream, image_open_intent_t intent)
{
    if (! istream)
    {
        return -1;
    }
    if (intent == IMAGE_READ)
    {
        return image_open_pwg_reader(istream);
    }
    else
    {
        return image_open_pwg_writer(istream);
    }
}

int image_read_pwg(image_stream_t *istream, uint8_t *line, size_t nline)
{
    pwg_context_t *info;
    int result;

    if (! istream || ! istream->priv)
    {
        return -1;
    }
    if (istream->intent != IMAGE_READ)
    {
        return -1;
    }
    if (istream->cur_line >= istream->img->height)
    {
        return -1;
    }
    if (nline < istream->img->stride)
    {
        return -1;
    }
    info = (pwg_context_t *)istream->priv;

    result = pwg_read_line(istream->stream, info, line);

    return result;
}

int image_write_pwg(image_stream_t *istream, uint8_t *line, size_t nline)
{
    pwg_context_t *info;

    if (! istream || ! istream->priv)
    {
        return -1;
    }
    if (istream->intent != IMAGE_WRITE)
    {
        return -1;
    }
    if (istream->cur_line >= istream->img->height)
    {
        return -1;
    }
    if (nline < istream->img->stride)
    {
        return -1;
    }
    info = (pwg_context_t *)istream->priv;
    return 0;
}

int image_close_pwg(image_stream_t *istream)
{
    if (! istream)
    {
        return -1;
    }
    if (istream->priv)
    {
        free(istream->priv);
        istream->priv = NULL;
    }
    return image_close(istream);
}

