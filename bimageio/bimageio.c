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
#include "butil.h"

int image_close(image_stream_t *istream)
{
	if (! istream)
	{
		return -1;
	}
	if (! istream->stream)
	{
		return 0;
	}
	istream->stream->close(istream->stream);
	istream->stream = NULL;
	free(istream);
	return 0;
}

int image_open_file(
				const char *file,
				image_open_intent_t intent,
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
	istream = (image_stream_t *)malloc(sizeof(image_stream_t));
	*pistream = istream;
	if (! istream)
	{
		butil_log(0, "Can't alloc image stream\n");
		return -1;
	}
	memset(istream, 0, sizeof(image_stream_t));
	istream->file_format = format;
	istream->intent = intent;

	switch (istream->file_format)
	{
	case butil_mime_png:
		istream->open  = image_open_png;
		istream->read  = image_read_png;
		istream->write = image_write_png;
		istream->close = image_close_png;
		break;
	default:
		butil_log(1, "File format %d not supported\n",
									(int)istream->file_format);
		free(istream);
		return -1;
	}
	if (istream->intent == IMAGE_READ)
	{
		istream->stream = iostream_create_reader_from_file(file);
	}
	else
	{
		istream->stream = iostream_create_writer_from_file(file);
	}
	if (! istream->stream)
	{
		butil_log(1, "Can't open %s\n", file);
		free(istream);
		return -1;
	}
	return istream->open(istream, intent);
}

