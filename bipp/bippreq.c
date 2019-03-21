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
#include "bipp.h"

int ipp_read_uint8(ipp_request_t *req, uint8_t *val)
{
	http_client_t *client;

	if (! req || ! val)
	{
		return -1;
	}
	client = req->client;
	if (! client)
	{
		return -1;
	}
	if (client->in.count > 0)
	{
		*val = client->in.data[client->in.tail];
		client->in.count--;
		client->in.tail++;
		if (client->in.tail >= client->in.size)
		{
			client->in.tail = 0;
		}
		return 0;
	}
	return 1;
}

int ipp_read_uint16(ipp_request_t *req, uint16_t *val)
{
	http_client_t *client;
	uint16_t uval;

	if (! req || ! val)
	{
		return -1;
	}
	client = req->client;
	if (! client)
	{
		return -1;
	}
	if (client->in.count > 1)
	{
		uval = client->in.data[client->in.tail];
		client->in.count--;
		client->in.tail++;
		if (client->in.tail >= client->in.size)
		{
			client->in.tail = 0;
		}
		uval <<= 8;
		uval |= client->in.data[client->in.tail];
		client->in.count--;
		client->in.tail++;
		if (client->in.tail >= client->in.size)
		{
			client->in.tail = 0;
		}
		*val = uval;
		return 0;
	}
	return 1;
}

int ipp_read_uint32(ipp_request_t *req, uint32_t *val)
{
	http_client_t *client;
	uint32_t uval;

	if (! req || ! val)
	{
		return -1;
	}
	client = req->client;
	if (! client)
	{
		return -1;
	}
	if (client->in.count > 3)
	{
		uval = client->in.data[client->in.tail];
		client->in.count--;
		client->in.tail++;
		if (client->in.tail >= client->in.size)
		{
			client->in.tail = 0;
		}
		uval <<= 8;
		uval |= client->in.data[client->in.tail];
		client->in.count--;
		client->in.tail++;
		if (client->in.tail >= client->in.size)
		{
			client->in.tail = 0;
		}
		uval <<= 8;
		uval |= client->in.data[client->in.tail];
		client->in.count--;
		client->in.tail++;
		if (client->in.tail >= client->in.size)
		{
			client->in.tail = 0;
		}
		uval <<= 8;
		uval |= client->in.data[client->in.tail];
		client->in.count--;
		client->in.tail++;
		if (client->in.tail >= client->in.size)
		{
			client->in.tail = 0;
		}
		*val = uval;
		return 0;
	}
	return 1;
}

int ipp_read_int8(ipp_request_t *req, int8_t *val)
{
	return ipp_read_uint8(req, (uint8_t*)val);
}

int ipp_read_int16(ipp_request_t *req, int16_t *val)
{
	return ipp_read_uint16(req, (uint16_t*)val);
}

int ipp_read_int32(ipp_request_t *req, int32_t *val)
{
	return ipp_read_uint32(req, (uint32_t*)val);
}

int ipp_write_uint8(ipp_request_t *req, uint8_t val)
{
	http_client_t *client;

	if (! req || ! req->client)
	{
		return -1;
	}
	client = req->client;

	if (client->out.count < client->out.size)
	{
		client->out.data[client->out.head] = val;
		client->out.count++;
		client->out.head++;
		if (client->out.head >= client->out.size)
		{
			client->out.head = 0;
		}
		return 0;
	}
	return 1;
}

int ipp_write_uint16(ipp_request_t *req, uint16_t val)
{
	http_client_t *client;
	uint16_t uval;

	if (! req || ! req->client)
	{
		return -1;
	}
	client = req->client;

	if (client->out.count < (client->out.size - 1))
	{
		client->out.data[client->out.head] = (uint8_t)((val >> 8) & 0xFF);
		client->out.count++;
		client->out.head++;
		if (client->out.head >= client->out.size)
		{
			client->out.head = 0;
		}
		client->out.data[client->out.head] = (uint8_t)(val & 0xFF);
		client->out.count++;
		client->out.head++;
		if (client->out.head >= client->out.size)
		{
			client->out.head = 0;
		}
		return 0;
	}
	return 1;
}

int ipp_write_uint32(ipp_request_t *req, uint32_t val)
{
	http_client_t *client;
	uint32_t uval;

	if (! req || ! req->client)
	{
		return -1;
	}
	client = req->client;

	if (client->out.count < (client->out.size - 3))
	{
		client->out.data[client->out.head] = (uint8_t)((val >> 24) & 0xFF);
		client->out.count++;
		client->out.head++;
		if (client->out.head >= client->out.size)
		{
			client->out.head = 0;
		}
		client->out.data[client->out.head] = (uint8_t)((val >> 16) & 0xFF);
		client->out.count++;
		client->out.head++;
		if (client->out.head >= client->out.size)
		{
			client->out.head = 0;
		}
		client->out.data[client->out.head] = (uint8_t)((val >> 8) & 0xFF);
		client->out.count++;
		client->out.head++;
		if (client->out.head >= client->out.size)
		{
			client->out.head = 0;
		}
		client->out.data[client->out.head] = (uint8_t)(val & 0xFF);
		client->out.count++;
		client->out.head++;
		if (client->out.head >= client->out.size)
		{
			client->out.head = 0;
		}
		return 0;
	}
	return 1;
}

int ipp_write_int8(ipp_request_t *req, int8_t val)
{
	return ipp_write_uint8(req, (uint8_t)val);
}

int ipp_write_int16(ipp_request_t *req, int16_t val)
{
	return ipp_write_uint16(req, (uint16_t)val);
}

int ipp_write_int32(ipp_request_t *req, int32_t val)
{
	return ipp_write_uint32(req, (uint32_t)val);
}

int ipp_write_text(ipp_request_t *req, const char *text, uint16_t len)
{
	http_client_t *client;
	int result;

	if (! req || ! req->client)
	{
		return -1;
	}
	client = req->client;

	if (client->out.count > (client->out.size - len - 2))
	{
		return 1;
	}
	client->out.data[client->out.head] = (uint8_t)((len >> 8) & 0xFF);
	client->out.count++;
	client->out.head++;
	if (client->out.head >= client->out.size)
	{
		client->out.head = 0;
	}
	client->out.data[client->out.head] = (uint8_t)(len & 0xFF);
	client->out.count++;
	client->out.head++;
	if (client->out.head >= client->out.size)
	{
		client->out.head = 0;
	}
	while (len-- > 0)
	{
		client->out.data[client->out.head] = *text++;
		client->out.count++;
		client->out.head++;
		if (client->out.head >= client->out.size)
		{
			client->out.head = 0;
		}
	}
	return 0;
}

int ipp_write_text_attribute(ipp_request_t *req, const char *text)
{
	int result;
	size_t len;

	if (! req || ! text)
	{
		return -1;
	}
	len = strlen(text);
	if (len > 65535)
	{
		return -2;
	}
	return ipp_write_text(req, text, len);
}

int ipp_write_named_attribute(ipp_request_t *req, int8_t tag, const char *text)
{
	int result;

	if (! req || ! text)
	{
		return -1;
	}
	result = ipp_write_int8(req, tag);
	if (result)
	{
		return result;
	}
	return ipp_write_text_attribute(req, text);
}

int ipp_write_chunk_count(ipp_request_t *req, int chunk)
{
	http_client_t *client;
	char chunk_str[8];
	int result;

	if (! req || ! req->client)
	{
		return -1;
	}
	snprintf(chunk_str, sizeof(chunk_str), "%04X", chunk);

	result  = ipp_write_uint8(req, chunk_str[0]);
	result |= ipp_write_uint8(req, chunk_str[1]);
	result |= ipp_write_uint8(req, chunk_str[2]);
	result |= ipp_write_uint8(req, chunk_str[3]);
    result |= ipp_write_uint8(req, '\r');
    result |= ipp_write_uint8(req, '\n');
	if (chunk == 0)
	{
	    result |= ipp_write_uint8(req, '\r');
	    result |= ipp_write_uint8(req, '\n');
	}
	return result;
}

int ipp_update_chunk_count(ipp_request_t *req, int chunksize)
{
	http_client_t *client;
	char chunk_str[8];
	int cur_head;
	int result;

	if (! req || ! req->client)
	{
		return -1;
	}
	client = req->client;

	// save current position in out buffer
	cur_head = client->out.head;

	// seek to place reserved chunk count was placed
	client->out.head = req->chunk_pos;

	// write the chunk count there
	result = ipp_write_chunk_count(req, chunksize);

	// restore position
	client->out.head = cur_head;
	return result;
}

ipp_request_t *ipp_req_create(ipp_server_t *ipp, http_client_t *client)
{
	ipp_request_t *req;

	if (! ipp || ! ipp->req_free)
	{
		return NULL;
	}
	req = ipp->req_free;
	ipp->req_free = req->next;
	req->client = client;
	req->top = 0;
	req->bytes_needed = 0;
	req->state[0] = reqHeader;
	return req;
}

int ipp_req_destroy(ipp_server_t *ipp, ipp_request_t *req)
{
	if (! ipp || ! req)
	{
		return -1;
	}
	req->next = ipp->req_free;
	ipp->req_free = req;
	return 0;
}

int ipp_req_init(ipp_server_t *ipp)
{
	size_t r;

	if (! ipp)
	{
		return -1;
	}
	for (r = 0; r < IPP_MAX_REQUESTS - 1; r++)
	{
		ipp->req_pool[r].client = NULL;
		ipp->req_pool[r].next = &ipp->req_pool[r + 1];
	}
	ipp->req_pool[r].next = NULL;
	ipp->req_free = &ipp->req_pool[0];
	return 0;
}

