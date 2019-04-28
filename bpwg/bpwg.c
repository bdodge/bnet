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
#ifndef BPWG_H
#define BPWG_H 1

#include "bpwg.h"
// include this "private" api header to keep it up to date
#include "bpwgunits.h"

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
    hdr->xres = htonl(hdr->xres);
    hdr->yres = htonl(hdr->yres);
    hdr->PageSize_X = htonl(hdr->PageSize_X);
    hdr->PageSize_Y = htonl(hdr->PageSize_Y);
    hdr->BitsPerColor = htonl(hdr->BitsPerColor);
    hdr->BitsPerPixel = htonl(hdr->BitsPerPixel);
    hdr->BytesPerLine = htonl(hdr->BytesPerLine);
    return 0;
}

int pwg_decode_line(pwg_context_t *pwg, uint8_t *data, int *ndata, bool *done)
{
    int avail;
    int took;

    butil_log(8, "state %u l=%u.%u p=%u %u\n",
            pwg->line_state, pwg->lineno, pwg->line_repeat,
            pwg->pixelno, pwg->color_repeat);

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
        pwg->pixelno += pwg->color_repeat;
        if (pwg->pixelno >= pwg->pixels)
        {
            if (pwg->pixelno != pwg->pixels)
            {
                butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
            }
            *done = true;
            pwg->lineno+= pwg->line_repeat;
            pwg->line_state = pwgLineLine;
            pwg->bytes_left = 1;
            break;
        }
        pwg->line_state = pwgLineRun;
        pwg->bytes_left = 1;
        break;
    case pwgLineSoloColor:
        avail = *ndata;
        took  = 0;
        while (avail >= 3 && pwg->color_repeat > 0)
        {
            took += 3;
            pwg->color_repeat--;
            pwg->pixelno++;
            if (pwg->pixelno >= pwg->pixels)
            {
                if (pwg->pixelno != pwg->pixels)
                {
                    butil_log(2, "Corrupt? line len is %u\n", pwg->pixelno);
                }
                if (pwg->color_repeat != 0)
                {
                    butil_log(2, "Corrupt? more colors: %u\n", pwg->color_repeat);
                }
                *done = true;
                pwg->lineno+= pwg->line_repeat;
                pwg->line_state = pwgLineLine;
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
    }
    return 0;
}

int pwg_slice(pwg_context_t *pwg, uint8_t *data, int *ndata)
{
    int chunk;
    bool done;
    int result;

    if (! pwg)
    {
        return -1;
    }
    switch (pwg->state)
    {
    case pwgSync:
        if (*ndata < 4)
        {
            return 0;
            *ndata = 0;
        }
        if (strcmp((char*)data, PWG_SYNC_STRING))
        {
            *ndata = 1;
        }
        *ndata = 4;
        pwg->pageno = 0;
        pwg->state = pwgHeader;
        pwg->bytes_left = sizeof(pwg_header_t);
        pwg->bytes_gotten = 0;
        break;
    case pwgHeader:
        chunk = *ndata;
        if (chunk > pwg->bytes_left)
        {
            chunk = pwg->bytes_left;
        }
        memcpy((char*)&pwg->hdr + pwg->bytes_gotten, data, chunk);
        pwg->bytes_left -= chunk;
        pwg->bytes_gotten += chunk;
        *ndata = chunk;
        if (pwg->bytes_left == 0)
        {
            // byte swap all the uint32s in hdr
            pwg_byteswap_header(&pwg->hdr);
            pwg->state = pwgPage;
        }
        break;
    case pwgPage:
        pwg_dump_header(&pwg->hdr);
        pwg->pages = pwg->hdr.TotalPageCount;
        pwg->rows = pwg->hdr.PageSize_Y * pwg->hdr.yres / 72;
        pwg->pixels = pwg->hdr.PageSize_X * pwg->hdr.xres / 72;
        pwg->pageno++;
        pwg->lineno = 0;
        pwg->pixels = pwg->hdr.BytesPerLine * 8 / pwg->hdr.BitsPerPixel;
        pwg->state = pwgLine;
        pwg->bytes_left = 1;
        pwg->bytes_gotten = 0;
        *ndata = 0;
        break;
    case pwgLine:
        result = pwg_decode_line(pwg, data, ndata, &done);
        if (! done)
        {
            break;
        }
        butil_log(6, "line %zu of %u\n", pwg->lineno, pwg->rows);
        if(pwg->lineno >= pwg->rows)
        {
            butil_log(5, "End of page %u\n", pwg->pageno);
            pwg->state = pwgHeader;
            pwg->bytes_left = sizeof(pwg_header_t);
            pwg->bytes_gotten = 0;
            break;
        }
        pwg->bytes_left = 1;
        pwg->bytes_gotten = 0;
        break;
    }
    return 0;
}

int pwg_init_context(pwg_context_t *pwg)
{
    if (! pwg)
    {
        return -1;
    }
    memset(pwg, 0, sizeof(pwg_context_t));
    return 0;
}

#endif


