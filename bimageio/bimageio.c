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
#include "bimgpng.h"
#include "bimgpwg.h"
#include "bimgjpeg.h"
#include "butil.h"

int image_close(image_stream_t *istream)
{
	if (! istream)
	{
		return -1;
	}
	if (istream->stream)
	{
		istream->stream->close(istream->stream);
		istream->stream = NULL;
	}
	if (istream->img && istream->own_img)
	{
		free(istream->img);
		istream->img = NULL;
	}
	free(istream);
	return 0;
}

static int image_set_format(image_stream_t *istream, mime_content_type_t format)
{
	if (! istream)
	{
		return -1;
	}
	istream->file_format = format;

	switch (istream->file_format)
	{
	case butil_mime_png:
		istream->open  = image_open_png;
		istream->read  = image_read_png;
		istream->write = image_write_png;
		istream->close = image_close_png;
		break;
	case butil_mime_jpeg:
		istream->open  = image_open_jpeg;
		istream->read  = image_read_jpeg;
		istream->write = image_write_jpeg;
		istream->close = image_close_jpeg;
		break;
	case 0x1234:
		istream->open  = image_open_pwg;
		istream->read  = image_read_pwg;
		istream->write = image_write_pwg;
		istream->close = image_close_pwg;
		break;
	default:
		butil_log(1, "File format %d not supported\n",
									(int)istream->file_format);
		return -1;
	}
	return 0;
}

int image_open_file_reader(
				const char *file,
				mime_content_type_t format,
				image_stream_t **pistream
				)
{
	image_stream_t *istream;
	int result;

	if (! file || ! pistream)
	{
		return -1;
	}
	*pistream = NULL;

	istream = (image_stream_t *)malloc(sizeof(image_stream_t));
	if (! istream)
	{
		butil_log(0, "Can't alloc image stream\n");
		return -1;
	}
	memset(istream, 0, sizeof(image_stream_t));
	istream->intent = IMAGE_READ;

	result = image_set_format(istream, format);
	if (result)
	{
		free(istream);
		return -1;
	}
	istream->img = (image_t *)malloc(sizeof(image_t));
	if (! istream->img)
	{
		butil_log(0, "Can't alloc image\n");
		free(istream);
		return -1;
	}
	istream->own_img = true;

	*pistream = istream;

	istream->stream = iostream_create_reader_from_file(file);
	if (! istream->stream)
	{
		butil_log(1, "Can't open %s for read\n", file);
		free(istream);
		return -1;
	}
	return istream->open(istream, istream->intent);
}

int image_open_file_writer(
				const char *file,
				mime_content_type_t format,
				image_t *img,
				image_stream_t **pistream
				)
{
	image_stream_t *istream;
	int result;

	if (! file || ! pistream)
	{
		return -1;
	}
	*pistream = NULL;

	istream = (image_stream_t *)malloc(sizeof(image_stream_t));
	if (! istream)
	{
		butil_log(0, "Can't alloc image stream\n");
		return -1;
	}
	memset(istream, 0, sizeof(image_stream_t));
	istream->file_format = format;
	istream->intent = IMAGE_WRITE;

	if (! img)
	{
		istream->img = (image_t *)malloc(sizeof(image_t));
		if (! istream->img)
		{
			butil_log(0, "Can't alloc image\n");
			free(istream);
			return -1;
		}
		istream->own_img = true;
	}
	else
	{
		istream->img = img;
		istream->own_img = false;
	}
	*pistream = istream;

	result = image_set_format(istream, format);
	if (result)
	{
		free(istream);
		return -1;
	}
	istream->stream = iostream_create_writer_from_file(file);
	if (! istream->stream)
	{
		butil_log(1, "Can't open %s for write\n", file);
		free(istream);
		return -1;
	}
	return istream->open(istream, istream->intent);
}

