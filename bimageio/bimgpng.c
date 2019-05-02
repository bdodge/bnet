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
#include "bimgpng.h"
#include <png.h>

typedef struct tag_png_info
{
    png_structp png_ptr;
    png_infop   info_ptr;
    int         passes;
}
png_info_t;

int image_open_png_reader(image_stream_t *istream)
{
    png_info_t *info;
    image_t *img;
    int color_type, interlace_type;

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
    info = (png_info_t *)malloc(sizeof(png_info_t));
    if (! info)
    {
        BERROR("Can't alloc info\n");
        return -1;
    }
    istream->priv = info;

    info->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (info->png_ptr == NULL)
    {
        free(info);
        return -1;
    }
    info->info_ptr = png_create_info_struct(info->png_ptr);
    if (info->info_ptr == NULL)
    {
        free(info);
        png_destroy_read_struct(&info->png_ptr, NULL, NULL);
        return -1;
    }
    if (setjmp(png_jmpbuf(info->png_ptr)))
    {
        png_destroy_read_struct(&info->png_ptr, &info->info_ptr, NULL);
        // If we get here, we had a problem reading the file.
        butil_log(0, "PNG parsing error\n");
        return -1;
    }
    // haven't read any png bytes yet
    png_set_sig_bytes(info->png_ptr, 0);

    // read file header
    png_read_info(info->png_ptr, info->info_ptr);

    png_get_IHDR(
                    info->png_ptr,
                    info->info_ptr,
                    &img->width,
                    &img->height,
                    (int*)&img->depth,
                    &color_type,
                    &interlace_type,
                    NULL,
                    NULL
                );

#if 0
    // convert 16 to 8 bit depth
#ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
    png_set_scale_16(png_ptr);
#else
    png_set_strip_16(png_ptr);
#endif
#endif
#if 0
    // drop alpha without combining
    png_set_strip_alpha(png_ptr);
#endif
#if 0
    // Extract multiple pixels with bit depths of 1, 2 or 4 from a single
    // byte into separate bytes (useful for paletted and grayscale images).
    png_set_packing(png_ptr);
#endif
#if 0
    // Change the order of packed pixels to least significant bit first
    // (not useful if you are using png_set_packing).
    png_set_packswap(png_ptr);
#endif
    // Expand paletted colors into true RGB triplets.
    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(png_ptr);
    }
    // Expand grayscale images to the full 8 bits from 1, 2 or 4 bits/pixel.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    // Expand paletted or RGB images with transparency to full alpha channels
    // so the data will be available as RGBA quartets.
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) != 0)
    {
        png_set_tRNS_to_alpha(png_ptr);
    }
    // Set the background color to draw transparent and alpha images over.
    // It is possible to set the red, green and blue components directly
    // for paletted images, instead of supplying a palette index.  Note that,
    // even if the PNG file supplies a background, you are not required to
    // use it - you should use the (solid) application background if it has one.
    //
#if 0
    png_color_16 my_background, *image_background;

    if (png_get_bKGD(png_ptr, info_ptr, &image_background) != 0)
    {
        png_set_background(png_ptr, image_background,
                                PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
    }
    else
    {
        png_set_background(png_ptr, &my_background,
                                PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
    }
#endif
    screen_gamma = PNG_DEFAULT_sRGB; /* A good guess for a PC monitor in a dimly lit room */
//    screen_gamma = PNG_GAMMA_MAC_18 or 1.0; /* Good guesses for Mac systems */

    // Tell libpng to handle the gamma conversion for you.  The final call
    // is a good guess for PC generated images, but it should be configurable
    // by the user at run time.  Gamma correction support in your application
    // is strongly recommended.
    //
#if 0
    int intent;

    if (png_get_sRGB(png_ptr, info_ptr, &intent) != 0)
    {
        png_set_gamma(png_ptr, screen_gamma, PNG_DEFAULT_sRGB);
    }
    else
    {
        double image_gamma;

        if (png_get_gAMA(png_ptr, info_ptr, &image_gamma) != 0)
        {
            png_set_gamma(png_ptr, screen_gamma, image_gamma);
        }
        else
        {
            png_set_gamma(png_ptr, screen_gamma, 0.45455);
        }
    }
#endif
#if 0
#ifdef PNG_READ_QUANTIZE_SUPPORTED
    // Quantize RGB files down to 8-bit palette, or reduce palettes
    // to the number of colors available on your screen.
    //
    if ((color_type & PNG_COLOR_MASK_COLOR) != 0)
    {
        int num_palette;
        png_colorp palette;

        // This reduces the image to the application-supplied palette.
        if (0 /* We have our own palette */)
        {
            // An array of colors to which the image should be quantized.
            png_color std_color_cube[MAX_SCREEN_COLORS];

            png_set_quantize(png_ptr, std_color_cube, MAX_SCREEN_COLORS,
                    MAX_SCREEN_COLORS, NULL, 0);
        }
        // This reduces the image to the palette supplied in the file.
        else if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette) != 0)
        {
            png_uint_16p histogram = NULL;
            png_get_hIST(png_ptr, info_ptr, &histogram);
            png_set_quantize(png_ptr, palette, num_palette,
                    max_screen_colors, histogram, 0);
        }
    }
#endif /* READ_QUANTIZE */
#endif

    // Invert monochrome files to have 0 as white and 1 as black.
    png_set_invert_mono(png_ptr);

#if 0
    // If you want to shift the pixel values from the range [0,255] or
    // [0,65535] to the original [0,7] or [0,31], or whatever range the
    // colors were originally in:
    //
    if (png_get_valid(info->png_ptr, info->info_ptr, PNG_INFO_sBIT) != 0)
    {
        png_color_8p sig_bit_p;
        png_get_sBIT(info->png_ptr, info->info_ptr, &sig_bit_p);
        png_set_shift(info->png_ptr, sig_bit_p);
    }
#endif
#if 0
    // Flip the RGB pixels to BGR (or RGBA to BGRA).
    if ((color_type & PNG_COLOR_MASK_COLOR) != 0)
    {
        png_set_bgr(info->png_ptr);
    }
#endif
#if 0
    // Swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR).
    png_set_swap_alpha(info->png_ptr);
#endif
#if 0
    // Swap bytes of 16-bit files to least significant byte first.
    png_set_swap(info->png_ptr);
#endif
#if 0
    // Add filler (or alpha) byte (before/after each RGB triplet).
    png_set_filler(info->png_ptr, 0xffff, PNG_FILLER_AFTER);
#endif
#ifdef PNG_READ_INTERLACING_SUPPORTED
    info->number_passes = png_set_interlace_handling(info->png_ptr);
#else /* !READ_INTERLACING */
    info->number_passes = 1;
#endif /* READ_INTERLACING */

    // Optional call to gamma correct and add the background to the palette
    // and update info structure.  REQUIRED if you are expecting libpng to
    // update the palette for you (i.e. you selected such a transform above).
    //
    png_read_update_info(info->png_ptr, info->info_ptr);

    /* Allocate the memory to hold the image using the fields of info_ptr. */
    png_bytep row_pointers[height];

    for (row = 0; row < height; row++)
    {
        row_pointers[row] = NULL; /* Clear the pointer array */
    }
    for (row = 0; row < height; row++)
    {
        row_pointers[row] =
            png_malloc(info->png_ptr,
                png_get_rowbytes(info->png_ptr, info->info_ptr));
    }
    // The other way to read images - deal with interlacing
    for (info->pass = 0; info->pass < info->number_passes; info->pass++)
    {
        for (y = 0; y < img->height; y++)
        {
            png_read_rows(info->png_ptr, &row_pointers[y], NULL, 1);
        }
    }
    // Read rest of file, and get additional chunks in info_ptr
    png_read_end(info->png_ptr, info->info_ptr);

    return 0;
}

int image_open_png_writer(image_stream_t *istream)
{
    return 0;
}

int image_open_png(image_stream_t *istream, image_open_intent_t intent)
{
    if (! istream)
    {
        return -1;
    }
    if (intent == IMAGE_READ)
    {
        return image_open_png_reader(istream);
    }
    else
    {
        return image_open_png_writer(istream);
    }
}

int image_read_png(image_stream_t *istream, uint8_t *line, size_t nline)
{
    return 0;
}

int image_write_png(image_stream_t *istream, uint8_t *line, size_t nline)
{
    return 0;
}

int image_close_png(image_stream_t *istream)
{
    if (! istream)
    {
        return -1;
    }
    if (istream->priv)
    {
        png_info_t *info = (png_info_t *)istream->priv;
        png_destroy_read_struct(&info->png_ptr, &info->info_ptr, NULL);
        free(istream->priv);
        istream->priv = NULL;
    }
    return image_close(istream);
}

