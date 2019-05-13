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
#include "bimgjpeg.h"
#include <setjmp.h>
#include <jpeglib.h>
#include <jinclude.h>
#include <jerror.h>

typedef struct tag_jpeg_info
{
    struct jpeg_error_mgr           jpeg_error_man; /* keep this first! */
    jmp_buf                         jpeg_error_buf;
    struct jpeg_decompress_struct   cinfo;
    struct jpeg_source_mgr          jpeg_src_man;
    uint8_t                        *iobuf;
    int                             iobuf_size;
    int                             bytes_read;
}
jpeg_info_t;

/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */
static void jpg_stream_init_source(j_decompress_ptr cinfo)
{
    jpeg_info_t *info = (jpeg_info_t *)cinfo->client_data;

    /* We reset the empty-input-file flag for each image,
    * but we don't clear the input buffer.
    * This is correct behavior for reading a series of images from one source.
    */
    info->bytes_read = 0;
}

/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In typical applications, this should read fresh data into the buffer
 * (ignoring the current state of next_input_byte & bytes_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been reloaded.  It is not necessary to
 * fill the buffer entirely, only to obtain at least one more byte.
 *
 * There is no such thing as an EOF return.  If the end of the file has been
 * reached, the routine has a choice of ERREXIT() or inserting fake data into
 * the buffer.  In most cases, generating a warning message and inserting a
 * fake EOI marker is the best course of action --- this will allow the
 * decompressor to output however much of the image is there.  However,
 * the resulting error message is misleading if the real problem is an empty
 * input file, so we handle that case specially.
 *
 * In applications that need to be able to suspend compression due to input
 * not being available yet, a FALSE return indicates that no more data can be
 * obtained right now, but more may be forthcoming later.  In this situation,
 * the decompressor will return to its caller (with an indication of the
 * number of scanlines it has read, if any).  The application should resume
 * decompression after it has loaded more data into the input buffer.  Note
 * that there are substantial restrictions on the use of suspension --- see
 * the documentation.
 *
 * When suspending, the decompressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_input_byte & bytes_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point must be rescanned after resumption, so move it to
 * the front of the buffer rather than discarding it.
 */

boolean jpg_stream_fill_input_buffer (j_decompress_ptr cinfo)
{
    image_stream_t *istream;
    jpeg_info_t *info;
    size_t nbytes;
    int result;

    istream = (image_stream_t *)cinfo->client_data;
    if (! istream || ! istream->priv)
    {
        return FALSE;
    }
    info = (jpeg_info_t *)istream->priv;

    // wait for data, infile could be a network stream
    //
    result = istream->stream->poll(istream->stream, readable, 5, 0);
    if (result < 0)
    {
        // stream ends
        //
        butil_log(4, "Jpeg data stream error\n");
        return FALSE;
    }
    else if (result == 0)
    {
        // no data
        //
        butil_log(4, "Jpeg data underflow\n");
        return FALSE;
    }
    nbytes = istream->stream->read(istream->stream, info->iobuf, info->iobuf_size);

    if ((int)nbytes <= 0)
    {
        if (info->bytes_read == 0)
        {
            ERREXIT(cinfo, JERR_INPUT_EMPTY);
        }
        WARNMS(cinfo, JWRN_JPEG_EOF);
        /* Insert a fake EOI marker */
        info->iobuf[0] = (JOCTET) 0xFF;
        info->iobuf[1] = (JOCTET) JPEG_EOI;
        nbytes = 2;
    }
    info->jpeg_src_man.next_input_byte = info->iobuf;
    info->jpeg_src_man.bytes_in_buffer = nbytes;
    info->bytes_read += nbytes;

    return TRUE;
}

/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */
void jpg_stream_skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
    image_stream_t *istream;
    jpeg_info_t *info;
    int result;

    istream = (image_stream_t *)cinfo->client_data;
    if (! istream || ! istream->priv)
    {
        return;
    }
    info = (jpeg_info_t *)istream->priv;

    if (num_bytes > 0)
    {
        while (num_bytes > (long)info->jpeg_src_man.bytes_in_buffer)
        {
            num_bytes -= (long)info->jpeg_src_man.bytes_in_buffer;
            result = (int)jpg_stream_fill_input_buffer(cinfo);

            /* note we assume that fill_input_buffer will never return FALSE,
            * so suspension need not be handled.
            */
        }
        info->jpeg_src_man.next_input_byte += (size_t)num_bytes;
        info->jpeg_src_man.bytes_in_buffer -= (size_t)num_bytes;
    }
}

/*
 * An additional method that can be provided by data source modules is the
 * resync_to_restart method for error recovery in the presence of RST markers.
 * For the moment, this source module just uses the default resync method
 * provided by the JPEG library.  That method assumes that no backtracking
 * is possible.
 */

static void jpg_stream_term_source(j_decompress_ptr cinfo)
{
}

static void jpg_stream_fatal(j_common_ptr cinfo)
{
    jpeg_info_t *info = (jpeg_info_t *)cinfo->err;
    longjmp(info->jpeg_error_buf, 1);
}

int image_open_jpeg_reader(image_stream_t *istream)
{
    jpeg_info_t *info;
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
    info = (jpeg_info_t *)malloc(sizeof(jpeg_info_t));
    if (! info)
    {
        BERROR("Can't alloc info\n");
        return -1;
    }
    info->iobuf_size = JPEG_IO_SIZE;
    info->iobuf = (uint8_t *)malloc(info->iobuf_size);
    if (! info)
    {
        BERROR("Can't alloc iobuf\n");
        free(info);
        return -1;
    }
    info->cinfo.err = jpeg_std_error(&info->jpeg_error_man);
    info->cinfo.err->error_exit = jpg_stream_fatal;
    if (setjmp(info->jpeg_error_buf))
    {
        butil_log(0, "JPEG lib failure\n");
        jpeg_abort_decompress(&info->cinfo);
        return -1;
    }
    jpeg_create_decompress(&info->cinfo);

    // set our image stream object into client info
    istream->priv = info;
    info->cinfo.client_data = istream;

    // setup r/w functions
    info->jpeg_src_man.init_source        = jpg_stream_init_source;
    info->jpeg_src_man.fill_input_buffer  = jpg_stream_fill_input_buffer;
    info->jpeg_src_man.skip_input_data    = jpg_stream_skip_input_data;
    info->jpeg_src_man.resync_to_restart  = jpeg_resync_to_restart;
    info->jpeg_src_man.term_source        = jpg_stream_term_source;
    info->jpeg_src_man.bytes_in_buffer = 0;
    info->jpeg_src_man.next_input_byte = NULL;

    info->cinfo.src = &info->jpeg_src_man;

    result = jpeg_read_header(&info->cinfo, TRUE);
    if (result)
    {
    }
    info->cinfo.out_color_space = JCS_RGB;
    info->cinfo.scale_num   = 1;
    info->cinfo.scale_denom = 1;

    result = jpeg_start_decompress(&info->cinfo);
    if (result)
    {
    }
    img->width = info->cinfo.output_width;
    img->height = info->cinfo.output_height;
    img->depth = 8;
    img->components = info->cinfo.output_components;
    img->stride = (info->cinfo.output_width) * (info->cinfo.output_components);
    img->format = IMAGE_RGB;

    istream->cur_line = 0;
    return 0;
}

int image_open_jpeg_writer(image_stream_t *istream)
{
    jpeg_info_t *info;
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
    info = (jpeg_info_t *)malloc(sizeof(jpeg_info_t));
    if (! info)
    {
        BERROR("Can't alloc info\n");
        return -1;
    }
    return 0;
}

int image_open_jpeg(image_stream_t *istream, image_open_intent_t intent)
{
    if (! istream)
    {
        return -1;
    }
    if (intent == IMAGE_READ)
    {
        return image_open_jpeg_reader(istream);
    }
    else
    {
        return image_open_jpeg_writer(istream);
    }
}

int image_read_jpeg(image_stream_t *istream, uint8_t *line, size_t nline)
{
    jpeg_info_t *info;

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
    info = (jpeg_info_t *)istream->priv;

    if (setjmp(info->jpeg_error_buf))
    {
        butil_log(0, "JPEG lib error (read)\n");
        jpeg_abort_decompress(&info->cinfo);
        return -1;
    }
    istream->cur_line += jpeg_read_scanlines(&info->cinfo, &line, 1);
    return 0;
}

int image_write_jpeg(image_stream_t *istream, uint8_t *line, size_t nline)
{
    jpeg_info_t *info;

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
    info = (jpeg_info_t *)istream->priv;
    return 0;
}

int image_close_jpeg(image_stream_t *istream)
{
    if (! istream)
    {
        return -1;
    }
    if (istream->priv)
    {
        jpeg_info_t *info = (jpeg_info_t *)istream->priv;

        if (istream->intent == IMAGE_READ)
        {
        }
        else
        {
        }
        free(istream->priv);
        istream->priv = NULL;
    }
    return image_close(istream);
}

