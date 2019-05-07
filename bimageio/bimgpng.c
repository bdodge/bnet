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
    int         number_passes;
}
png_info_t;

static int image_format_for_png_type(int type, image_color_format_t *format)
{
    switch (type)
    {
    case PNG_COLOR_TYPE_GRAY:
        *format = IMAGE_GRAY;
        break;
    case PNG_COLOR_TYPE_RGB:
        *format = IMAGE_RGB;
        break;
    case PNG_COLOR_TYPE_RGBA:
        *format = IMAGE_RGBA;
        break;
    default:
        return -1;
    }
    return 0;
}

static int image_png_type_for_format(image_color_format_t format)
{
    switch (format)
    {
    case IMAGE_GRAY:
        return PNG_COLOR_TYPE_GRAY;
    case IMAGE_RGB:
        return PNG_COLOR_TYPE_RGB;
    case IMAGE_BGR:
        return PNG_COLOR_TYPE_RGB;
    case IMAGE_RGBA:
        return PNG_COLOR_TYPE_RGBA;
    case IMAGE_ABGR:
        return PNG_COLOR_TYPE_RGBA;
    case IMAGE_CMYK:
        return PNG_COLOR_TYPE_RGBA;
    }
}

static void image_read_png_bytes(png_structp png_ptr, png_bytep data, png_size_t nbytes)
{
    image_stream_t *istream;
    int nread;
    int result;

    istream = (image_stream_t *)png_get_io_ptr(png_ptr);
    if (! istream)
    {
        png_error(png_ptr, "No stream");
        return;
    }
    if (! istream->stream)
    {
        png_error(png_ptr, "No stream*");
        return;
    }
    result = istream->stream->poll(istream->stream, readable, 1, 0);
    if (result <= 0)
    {
        png_error(png_ptr, "Stream not ready");
    }
    nread = istream->stream->read(istream->stream, data, (int)nbytes);
    if (nread < 0)
    {
        png_error(png_ptr, "Stream short read");
    }
}

static void image_write_png_bytes(png_structp png_ptr, png_bytep data, png_size_t nbytes)
{
    image_stream_t *istream;
    int nwrote;
    int result;

    istream = (image_stream_t *)png_get_io_ptr(png_ptr);
    if (! istream)
    {
        png_error(png_ptr, "No stream");
        return;
    }
    if (! istream->stream)
    {
        png_error(png_ptr, "No stream*");
        return;
    }
    result = istream->stream->poll(istream->stream, writeable, 1, 0);
    if (result <= 0)
    {
        png_error(png_ptr, "Stream not ready");
    }
    nwrote = istream->stream->write(istream->stream, data, (int)nbytes);
    if (nwrote < 0)
    {
        png_error(png_ptr, "Stream short write");
    }
}

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
    info->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (info->png_ptr == NULL)
    {
        free(info);
        return -1;
    }
    info->info_ptr = png_create_info_struct(info->png_ptr);
    if (info->info_ptr == NULL)
    {
        png_destroy_read_struct(&info->png_ptr, NULL, NULL);
        free(info);
        return -1;
    }
    // set png reader function
    png_set_read_fn(info->png_ptr, (void *)istream, image_read_png_bytes);

    istream->priv = info;

    if (setjmp(png_jmpbuf(info->png_ptr)))
    {
        png_destroy_read_struct(&info->png_ptr, &info->info_ptr, NULL);
        // If we get here, we had a problem reading the file.
        info->png_ptr = NULL;
        info->info_ptr = NULL;
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

    img->stride = png_get_rowbytes(info->png_ptr, info->info_ptr);
    if (image_format_for_png_type(color_type, &img->format))
    {
        png_error(info->png_ptr, "No format for type");
    }
    istream->cur_line = 0;
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
        png_set_palette_to_rgb(info->png_ptr);
    }
    // Expand grayscale images to the full 8 bits from 1, 2 or 4 bits/pixel.
    if (color_type == PNG_COLOR_TYPE_GRAY && img->depth < 8)
    {
        png_set_expand_gray_1_2_4_to_8(info->png_ptr);
    }
    // Expand paletted or RGB images with transparency to full alpha channels
    // so the data will be available as RGBA quartets.
    if (png_get_valid(info->png_ptr, info->info_ptr, PNG_INFO_tRNS) != 0)
    {
        png_set_tRNS_to_alpha(info->png_ptr);
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
//    screen_gamma = PNG_DEFAULT_sRGB; /* A good guess for a PC monitor in a dimly lit room */
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
    png_set_invert_mono(info->png_ptr);

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

    return 0;
}

int image_open_png_writer(image_stream_t *istream)
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
    info->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                    NULL, NULL, NULL);
    if (info->png_ptr == NULL)
    {
        free(info);
        return -1;
    }
    info->info_ptr = png_create_info_struct(info->png_ptr);
    if (info->info_ptr == NULL)
    {
        png_destroy_write_struct(&info->png_ptr,  NULL);
        free(info);
        return -1;
    }
    istream->priv = info;

    if (setjmp(png_jmpbuf(info->png_ptr)))
    {
        // If we get here, we had a problem writing the image
        png_destroy_write_struct(&info->png_ptr, &info->info_ptr);
        istream->priv = NULL;
        return -1;
    }
    png_set_write_fn(info->png_ptr, (void *)istream, image_write_png_bytes, NULL);

    // Set the image information here.  Width and height are up to 2^31,
    // bit_depth is one of 1, 2, 4, 8 or 16, but valid values also depend on
    // the color_type selected.  color_type is one of PNG_COLOR_TYPE_GRAY,
    // PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
    // or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
    // PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
    // currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE.
    // REQUIRED.
    //
    png_set_IHDR(
                    info->png_ptr,
                    info->info_ptr,
                    img->width,
                    img->height,
                    img->depth,
                    image_png_type_for_format(img->format),
                    PNG_INTERLACE_NONE,
                    PNG_COMPRESSION_TYPE_BASE,
                    PNG_FILTER_TYPE_BASE
                );
#if 0
   palette = (png_colorp)png_malloc(png_ptr,
       PNG_MAX_PALETTE_LENGTH * (sizeof (png_color)));
   png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);

   png_color_8 sig_bit;

   /* If we are dealing with a grayscale image then */
   sig_bit.gray = true_bit_depth;

   /* Otherwise, if we are dealing with a color image then */
   sig_bit.red = true_red_bit_depth;
   sig_bit.green = true_green_bit_depth;
   sig_bit.blue = true_blue_bit_depth;

   /* If the image has an alpha channel then */
   sig_bit.alpha = true_alpha_bit_depth;

   png_set_sBIT(png_ptr, info_ptr, &sig_bit);

   /* Optional gamma chunk is strongly suggested if you have any guess
    * as to the correct gamma of the image.
    */
   png_set_gAMA(png_ptr, info_ptr, gamma);

   /* Optionally write comments into the image. */
   {
      png_text text_ptr[3];

      char key0[] = "Title";
      char text0[] = "Mona Lisa";
      text_ptr[0].key = key0;
      text_ptr[0].text = text0;
      text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
      text_ptr[0].itxt_length = 0;
      text_ptr[0].lang = NULL;
      text_ptr[0].lang_key = NULL;

      char key1[] = "Author";
      char text1[] = "Leonardo DaVinci";
      text_ptr[1].key = key1;
      text_ptr[1].text = text1;
      text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
      text_ptr[1].itxt_length = 0;
      text_ptr[1].lang = NULL;
      text_ptr[1].lang_key = NULL;

      char key2[] = "Description";
      char text2[] = "<long text>";
      text_ptr[2].key = key2;
      text_ptr[2].text = text2;
      text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
      text_ptr[2].itxt_length = 0;
      text_ptr[2].lang = NULL;
      text_ptr[2].lang_key = NULL;

      png_set_text(write_ptr, write_info_ptr, text_ptr, 3);
   }

   /* Other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs. */

   /* Note that if sRGB is present, the gAMA and cHRM chunks must be ignored
    * on read and, if your application chooses to write them, they must
    * be written in accordance with the sRGB profile.
    */
#endif

   png_write_info(info->png_ptr, info->info_ptr);

   /* If you want, you can write the info in two steps, in case you need to
    * write your private chunk ahead of PLTE:
    *
    *   png_write_info_before_PLTE(write_ptr, write_info_ptr);
    *   write_my_chunk();
    *   png_write_info(png_ptr, info_ptr);
    *
    * However, given the level of known- and unknown-chunk support in 1.2.0
    * and up, this should no longer be necessary.
    */

   /* Once we write out the header, the compression type on the text
    * chunk gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
    * PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
    * at the end.
    */

   /* Set up the transformations you want.  Note that these are
    * all optional.  Only call them if you want them.
    */

    // Invert monochrome pixels.
    png_set_invert_mono(info->png_ptr);

#if 0
    /* Shift the pixels up to a legal bit depth and fill in
    * as appropriate to correctly scale the image.
    */
    png_set_shift(png_ptr, &sig_bit);

    /* Pack pixels into bytes. */
    png_set_packing(png_ptr);

    /* Swap location of alpha bytes from ARGB to RGBA. */
    png_set_swap_alpha(png_ptr);

    /* Get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
    * RGB (4 channels -> 3 channels).  The second parameter is not used.
    */
    png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);

    /* Flip BGR pixels to RGB. */
    png_set_bgr(png_ptr);

    /* Swap bytes of 16-bit files to most significant byte first. */
    png_set_swap(png_ptr);

    /* Swap bits of 1-bit, 2-bit, 4-bit packed pixel formats. */
    png_set_packswap(png_ptr);

    /* Turn on interlace handling if you are not using png_write_image(). */
    if (interlacing != 0)
    {
        number_passes = png_set_interlace_handling(png_ptr);
    }
    else
    {
        number_passes = 1;
    }
#endif

    info->number_passes = 1;

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
    png_info_t *info;
    png_bytep rowp;
    int pass;

    if (! istream || ! istream->priv)
    {
        return -1;
    }
    if (istream->cur_line >= istream->img->height)
    {
        return -1;
    }
    info = (png_info_t *)istream->priv;
    rowp = (png_bytep)line;

    if (! info->png_ptr)
    {
        return -1;
    }
    if (nline < istream->img->stride)
    {
        return -1;
    }
    for (pass = 0; pass < info->number_passes; pass++)
    {
        if (! istream->priv)
        {
            // longjmp cleared it
            return -1;
        }
        png_read_rows(info->png_ptr, &rowp, NULL, 1);
    }
    return 0;
}

int image_write_png(image_stream_t *istream, uint8_t *line, size_t nline)
{
    png_info_t *info;
    png_bytep rowp;
    int pass;

    if (! istream || ! istream->priv)
    {
        return -1;
    }
    if (istream->cur_line >= istream->img->height)
    {
        return -1;
    }
    info = (png_info_t *)istream->priv;
    rowp = (png_bytep)line;

    if (! info->png_ptr)
    {
        return -1;
    }
    if (nline < istream->img->stride)
    {
        return -1;
    }
    for (pass = 0; pass < info->number_passes; pass++)
    {
        if (! istream->priv)
        {
            // longjmp cleared it
            return -1;
        }
        png_write_rows(info->png_ptr, &rowp, 1);
    }
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

        if (istream->intent == IMAGE_READ)
        {
            png_read_end(info->png_ptr, info->info_ptr);
            png_destroy_read_struct(&info->png_ptr, &info->info_ptr, NULL);
        }
        else
        {
            png_write_end(info->png_ptr, info->info_ptr);
            png_destroy_write_struct(&info->png_ptr, &info->info_ptr);
        }
        free(istream->priv);
        istream->priv = NULL;
    }
    return image_close(istream);
}

