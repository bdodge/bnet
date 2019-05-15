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
#ifndef BIMAGEIO_H
#define BIMAGEIO_H 1

#include "bnetheaders.h"
#include "bstreamio.h"
#include "butil.h"

#define _IMAGE_GRAY          (0x01)
#define _IMAGE_RGB       (0x02)
#define _IMAGE_CMYK          (0x04)
#define _IMAGE_SWAPPED   (0x40)
#define _IMAGE_ALPHA     (0x80)

typedef enum
{
 IMAGE_GRAY  = (_IMAGE_GRAY),
 IMAGE_RGB   = (_IMAGE_RGB),
 IMAGE_BGR   = (_IMAGE_RGB | _IMAGE_SWAPPED),
 IMAGE_RGBA  = (_IMAGE_RGB | _IMAGE_ALPHA),
 IMAGE_ABGR  = (_IMAGE_RGB | _IMAGE_ALPHA | _IMAGE_SWAPPED),
 IMAGE_CMYK  = (_IMAGE_CMYK),
}
image_color_format_t;

typedef struct tag_image
{
 uint32_t                width;      ///< image width, pixels
 uint32_t                height;     ///< image height, pixels
 uint32_t                depth;      ///< image depth, bits per pixel
 uint32_t                components; ///< color components per pixel
 image_color_format_t    format;     ///< color format
 uint32_t                stride;     ///< bytes per scanline
 uint32_t                xres;       ///< x resolution to format image
 uint32_t                yres;       ///< y resolution to format image
}
image_t;

typedef enum
{
 IMAGE_READ,
 IMAGE_WRITE
}
image_open_intent_t;

struct tag_image_stream;

typedef int (*image_open_t)      (
                             struct tag_image_stream *istream,
                             image_open_intent_t intent
                             );
typedef int (*image_read_line_t)(
                             struct tag_image_stream *istream,
                             uint8_t *line,
                             size_t nline
                             );
typedef int (*image_write_line_t)(
                             struct tag_image_stream *istream,
                             uint8_t *line,
                             size_t nline
                             );
typedef int (*image_close_t) (
                             struct tag_image_stream *istream
                             );

typedef struct tag_image_stream
{
 image_t            *img;
 iostream_t         *stream;
 image_open_intent_t intent;
 mime_content_type_t file_format;
 bool                own_img;
 void               *priv;
 uint32_t            cur_line;
 image_open_t        open;
 image_read_line_t   read;
 image_write_line_t  write;
 image_close_t       close;
}
image_stream_t;

int image_close            		(image_stream_t *istream);

int image_open_file_reader      (
                             	const char           *inputfile,
                             	mime_content_type_t   inputfileformat,
                             	uint32_t              outdepth,
                               	image_color_format_t  outformat,
                             	image_stream_t      **pistream
                             	);

int image_open_file_writer      (
                             	const char           *outfile,
                             	mime_content_type_t   outfileformat,
                             	image_t              *prototypeimg,
                             	image_stream_t      **pistream
                             	);

#endif

