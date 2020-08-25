/*
 * Copyright 2020 Brian Dodge
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
#include "bui.h"

static void gdi_fill_rect_1bpp(gdi_driver_t *gc, int x, int y, int w, int h, COLORREF frg)
{
    int xx;
    int yy;
    uint8_t *pline;
    uint8_t pixel;
    uint8_t mask;

    if (x < 0)
    {
        w += x;
        x = 0;
    }
    if (y < 0)
    {
        h += y;
        y = 0;
    }
    if (x + w > gc->w)
    {
        w = gc->w - x;
    }
    if (y + h > gc->h)
    {
        h = gc->h - y;
    }
    if (w < 0 || h < 0)
    {
        return;
    }
    for (yy = y; yy < (y + h); yy++)
    {
        pline = &gc->fb[yy * gc->s + (x >> 3)];
        pixel = *pline;
        mask = 0x80 >> (x & 0x7);

        for (xx = x; xx < (x + w); xx++)
        {
            if (frg)
            {
                pixel |= mask;
            }
            else
            {
                pixel &= ~mask;
            }
            mask >>= 1;
            if (! mask)
            {
                *pline++ = pixel;
                mask = 0x80;
            }
        }
        if (mask != 0x80)
        {
            *pline = pixel;
        }
    }
}

gdi_driver_t *gdi_create_bitmap_driver_1bpp(int w, int h)
{
    gdi_driver_t *gc;

    gc = (gdi_driver_t *)malloc(sizeof(gdi_driver_t));
    if (! gc)
    {
        return NULL;
    }
    gc->w = w;
    gc->h = h;
    gc->d = 1;
    gc->s = (gc->w + 7) / 8;

    gc->fb = (uint8_t *)malloc(gc->h * gc->s);
    if (! gc->fb)
    {
        free(gc);
        return NULL;
    }

    gc->fb_is_alloced = true;
    memset(gc->fb, 0, gc->h * gc->s);

    gc->fill_rect = gdi_fill_rect_1bpp;
    gc->text_out  = NULL;
    gc->image_out = NULL;

    return gc;
}

static void gdi_fill_rect_24bpp(gdi_driver_t *gc, int x, int y, int w, int h, COLORREF frg)
{
    int xx;
    int yy;
    uint8_t *pline;
    uint8_t pixel;
    uint8_t mask;

    if (x < 0)
    {
        w += x;
        x = 0;
    }
    if (y < 0)
    {
        h += y;
        y = 0;
    }
    if (x + w > gc->w)
    {
        w = gc->w - x;
    }
    if (y + h > gc->h)
    {
        h = gc->h - y;
    }
    if (w < 0 || h < 0)
    {
        return;
    }
    pline = &gc->fb[(y * gc->s) + (x * (gc->d / 8))];

    for (yy = y; yy < (y + h); yy++)
    {
        uint8_t *pout = pline;

        for (xx = 0; xx < w; xx++)
        {
            *pout++ = (uint8_t)(frg);
            *pout++ = (uint8_t)(frg >> 8);
            *pout++ = (uint8_t)(frg >> 16);
        }
        pline += gc->s;
    }
}

gdi_driver_t *gdi_create_bitmap_driver_24bpp(int w, int h)
{
    gdi_driver_t *gc;

    gc = (gdi_driver_t *)malloc(sizeof(gdi_driver_t));
    if (! gc)
    {
        return NULL;
    }
    gc->w = w;
    gc->h = h;
    gc->d = 24;
    gc->s = 3 * ((gc->w + 3) & ~3);

    gc->fb = (uint8_t *)malloc(gc->h * gc->s);
    if (! gc->fb)
    {
        free(gc);
        return NULL;
    }

    gc->fb_is_alloced = true;
    memset(gc->fb, 0xff, gc->h * gc->s);

    gc->fill_rect = gdi_fill_rect_24bpp;
    gc->text_out  = NULL;
    gc->image_out = NULL;

    return gc;
}

void gdi_driver_destroy(gdi_driver_t *gc)
{
    if (gc)
    {
        if (gc->fb && gc->fb_is_alloced)
        {
            free(gc->fb);
        }
        free(gc);
    }
}

void gdi_fill_rect(gdi_driver_t *gc, int x, int y, int w, int h, COLORREF frg)
{
    if (! gc)
    {
        return;
    }
    if (! gc->fill_rect)
    {
        return;
    }
    gc->fill_rect(gc, x, y, w, h, frg);
}

void gdi_text_out(gdi_driver_t *gc, const char *text, int x, int y, COLORREF frg)
{
    if (! gc)
    {
        return;
    }
    if (! gc->text_out)
    {
        return;
    }
    gc->text_out(gc, text, x, y, frg);
}

