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
#ifndef BUIGDI_H
#define BUIGDI_H 1

struct tag_gdi_driver;

typedef struct tag_gdi_image_t
{
    int w;                  // width
    int h;                  // height
    int d;                  // depth
    int s;                  // stride
    int f;                  // format
    uint8_t *bits;          // image data
}
gdi_image_t;

typedef void (*fill_rect_f_t)   (struct tag_gdi_driver *driver, int x, int y, int w, int h, COLORREF fill);
typedef void (*text_out_f_t)    (struct tag_gdi_driver *driver, const char *text, int x, int y, COLORREF frg);
typedef void (*image_out_f_t)   (struct tag_gdi_driver *driver, const gdi_image_t *img, int x, int y, int w, int h);

typedef struct tag_gdi_driver
{
    int w;                      // graphic width in pixels
    int h;                      // graphic height in pixels
    int d;                      // graphic depth in pixels
    int s;                      // stride: bytes across one line

    COLORREF bkg;               // text background

    fill_rect_f_t fill_rect;    // fill rectangle function
    text_out_f_t  text_out;     // text output function
    image_out_f_t image_out;    // image output function

    bool    fb_is_alloced;      // non-0 if frame buffer is allocated
    uint8_t *fb;                // frame buffer
}
gdi_driver_t;

gdi_driver_t *gdi_create_bitmap_driver_24bpp(int w, int h);
gdi_driver_t *gdi_create_bitmap_driver_1bpp(int w, int h);

void gdi_driver_destroy(gdi_driver_t *gc);
void gdi_fill_rect(gdi_driver_t *gc, int x, int y, int w, int h, COLORREF frg);
void gdi_text_out(gdi_driver_t *gc, const char *text, int x, int y, COLORREF frg);

#endif

