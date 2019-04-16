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
#include "bhttpgz.h"
#include "bnetheaders.h"
#include "bhttpconfig.h"
#include "bhttp.h"
#include "butil.h"
#include "bstreamio.h"

#if HTTP_SUPPORT_COMPRESSION

#include "zlib.h"

int http_open_compression(http_client_t *client)
{
	int level;
	int result;

	if (! client || client->compress < httpCanCompress)
	{
		return -1;
	}
	// re-init compression buffer
	//
	client->comprio.head = 0;
	client->comprio.tail = 0;
	client->comprio.count = 0;

    client->zstrm.zalloc = Z_NULL;
    client->zstrm.zfree = Z_NULL;
    client->zstrm.opaque = Z_NULL;

	result = deflateInit2(&client->zstrm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, (15+16), 8, Z_DEFAULT_STRATEGY);
	if (result != Z_OK)
    {
		butil_log(3, "Can't init zlib for compressing body: %d\n", Z_ERRNO);
        client->compress = httpDontCompress;
    }
    else
    {
        client->compress = httpWillCompress;
    }
	return 0;
}

int http_compress_stream(http_client_t *client, iostream_t *stream, uint8_t *outbuf, size_t *iocount)
{
	int room;
	int more;
	int flush;
	int used;
	int made;
	int tot_used;
	int tot_made;
	int result;

	if (! client || ! stream || ! outbuf || ! iocount)
	{
		return -1;
	}
	http_log(6, "Compress data state: %d\n", client->compress);

	if (client->compress == httpDidCompress)
	{
		*iocount = 0;
		return 0;
	}
	// align compression input on left of ring
	iostream_normalize_ring(&client->comprio, NULL);

	tot_used = 0;
	tot_made = 0;

	if (client->compress == httpWillCompress)
	{
		// extract name-only of file
		int pathlen;
		const char *basepath;
		const char *pp;

		for (basepath = pp = client->path; pp;)
		{
			pp = strchr(basepath, '/');
			if (pp)
			{
				pp++;
				basepath = pp;
			}
		}
		if (! basepath)
		{
			basepath = client->path;
		}
		pathlen = strlen(basepath) + 1;

		// TODO - set filename in header?

		client->compress = httpCompressing;
	}
	flush = Z_FINISH;

	if (client->compress == httpCompressing)
	{
		room = client->comprio.size - client->comprio.count;		// room to read into
		if (room > 0)
		{
			more = stream->read(stream, client->comprio.data + client->comprio.head, room);
			if (more > 0)
			{
				client->comprio.head += more;
				client->comprio.count += more;
			}
			else if (more < 0)
			{
				return -1;
			}
			else if (client->comprio.count == 0)
			{
				// assume end of stream for 0 and no fodder
				http_log(5, "compr finish, %d avail\n", client->comprio.count);
			}
		}
		if (client->comprio.count)
		{
			flush = Z_NO_FLUSH;
		}
		room = *iocount;
		client->zstrm.avail_out = room;					// room to compress into

		while (client->zstrm.avail_out > 0)
		{
			client->zstrm.next_out  = outbuf + tot_made;	// where to compress into

			client->zstrm.avail_in = client->comprio.count;
			client->zstrm.next_in  = client->comprio.data + client->comprio.tail;

			result = deflate(&client->zstrm, flush);
			if (result == Z_STREAM_ERROR)
			{
				*iocount = 0;
				http_log(2, "Error %d compression\n", Z_ERRNO);
				return -1;
			}
			used = client->comprio.count - client->zstrm.avail_in; // how many taken

			client->comprio.tail += used;
			client->comprio.count -= used;
			tot_used += used;

			made = room - client->zstrm.avail_out; // how many produced
			room -= made;
			tot_made += made;

			if (made == 0)
			{
				break;
			}
			http_log(6, "Compressed %d from %d\n", made, used);
		}
		if (flush == Z_FINISH)
		{
			client->compress = httpDidCompress;
		}
	}
	*iocount = tot_made;
	return 0;
}

int http_close_compression(http_client_t *client)
{
	int result;

	if (! client || client->compress < httpWillCompress)
	{
		return -1;
	}
	deflateEnd(&client->zstrm);
	return 0;
}

int http_open_decompression(http_client_t *client)
{
	int level;
	int result;

	if (! client || client->compress < httpCanCompress)
	{
		return -1;
	}
	// re-init decompression buffer
	//
	client->comprio.head = 0;
	client->comprio.tail = 0;
	client->comprio.count = 0;

    client->zstrm.zalloc = Z_NULL;
    client->zstrm.zfree = Z_NULL;
    client->zstrm.opaque = Z_NULL;

	result = inflateInit2(&client->zstrm, (15+16));
	if (result != Z_OK)
    {
		butil_log(3, "Can't init zlib for decompressing body: %d\n", Z_ERRNO);
        client->compress = httpDontCompress;
    }
    else
    {
        client->compress = httpWillCompress;
    }
	return 0;
}

int http_decompress_stream(http_client_t *client, iostream_t *stream, uint8_t *inbuf, size_t *iocount)
{
	int room;
	int more;
	int flush;
	int used;
	int made;
	int tot_made;
	int tot_used;
	int result;
	int prev_count;
	int i;
	uint32_t uval;

	if (! client || ! stream || ! inbuf || ! iocount)
	{
		return -1;
	}
	http_log(6, "Compress data state: %d\n", client->compress);

	if (client->compress == httpDidCompress)
	{
		*iocount = 0;
		return 0;
	}
	tot_made = 0;
	tot_used = 0;

	// align decompression output on left of ring
	iostream_normalize_ring(&client->comprio, NULL);

	if (client->compress == httpWillCompress)
	{
		client->compress = httpCompressing;
	}
	flush = Z_NO_FLUSH;

	room = client->comprio.size - client->comprio.count;	// how much space in output
	client->zstrm.avail_out = room;

	more = *iocount;										// how much data to decompress
	if (client->compress == httpCompressing)
	{
		client->zstrm.avail_in = more;

		while (client->zstrm.avail_in > 0)
		{
			prev_count = client->comprio.count;

			client->zstrm.next_in  = inbuf + tot_used;	// where to decompress from
			client->zstrm.next_out  = client->comprio.data + client->comprio.head;

			result = inflate(&client->zstrm, flush);
			if (result == Z_STREAM_ERROR)
			{
				*iocount = 0;
				http_log(2, "Error %d compression\n", Z_ERRNO);
				return -1;
			}
			switch (result)
			{
            case Z_NEED_DICT:
                result = Z_DATA_ERROR;
				break;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                break;
			default:
				result = 0;
				break;
            }
 			if (result)
			{
				*iocount = 0;
				http_log(2, "Error %d compression\n", result);
				return -1;
			}
			made = room - client->zstrm.avail_out; // how many made
			room -= made;
			tot_made += made;

			client->comprio.head += made;
			client->comprio.count += made;

			used =  more - client->zstrm.avail_in; // how many used
			more -= used;
			tot_used += used;

			if (made == 0)
			{
				break;
			}
			http_log(6, "Decompressed %d from %d\n", made, used);
		}
		while (client->comprio.count > 0)
		{
			// write stream with decompressed results
			//
			result = stream->write(stream, client->comprio.data + client->comprio.tail, client->comprio.count);
			if (result < 0)
			{
				return result;
			}
			client->comprio.tail += result;
			client->comprio.count -= result;
		}
	}
	*iocount = tot_used;
	return 0;
}

int http_close_decompression(http_client_t *client)
{
	int result;

	if (! client || client->compress < httpWillCompress)
	{
		return -1;
	}
	inflateEnd(&client->zstrm);
	return 0;
}

#endif

