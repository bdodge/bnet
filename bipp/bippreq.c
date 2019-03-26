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

int ipp_read_uint8(ioring_t *in, uint8_t *val)
{
	http_client_t *client;

	if (! in || ! val)
	{
		return -1;
	}
	if (in->count > 0)
	{
		*val = in->data[in->tail];
		in->count--;
		in->tail++;
		return 0;
	}
	return 1;
}

int ipp_read_uint16(ioring_t *in, uint16_t *val)
{
	http_client_t *client;
	uint16_t uval;

	if (! in || ! val)
	{
		return -1;
	}
	if (in->count > 1)
	{
		uval = in->data[in->tail];
		in->count--;
		in->tail++;
		uval <<= 8;
		uval |= in->data[in->tail];
		in->count--;
		in->tail++;
		*val = uval;
		return 0;
	}
	return 1;
}

int ipp_read_uint32(ioring_t *in, uint32_t *val)
{
	http_client_t *client;
	uint32_t uval;

	if (! in || ! val)
	{
		return -1;
	}
	if (in->count > 3)
	{
		uval = in->data[in->tail];
		in->count--;
		in->tail++;

		uval <<= 8;
		uval |= in->data[in->tail];
		in->count--;
		in->tail++;

		uval <<= 8;
		uval |= in->data[in->tail];
		in->count--;
		in->tail++;

		uval <<= 8;
		uval |= in->data[in->tail];
		in->count--;
		in->tail++;

		*val = uval;
		return 0;
	}
	return 1;
}

int ipp_read_int8(ioring_t *in, int8_t *val)
{
	return ipp_read_uint8(in, (uint8_t*)val);
}

int ipp_read_int16(ioring_t *in, int16_t *val)
{
	return ipp_read_uint16(in, (uint16_t*)val);
}

int ipp_read_int32(ioring_t *in, int32_t *val)
{
	return ipp_read_uint32(in, (uint32_t*)val);
}

int ipp_read_text(ioring_t *in, char *text, uint16_t len)
{
	if (! in || ! text)
	{
		return -1;
	}
	if (in->count < len)
	{
		return 1;
	}
	while (len-- > 0)
	{
		*text++ = in->data[in->tail];
		in->count--;
		in->tail++;
	}
	return 0;
}

int ipp_write_uint8(ioring_t *out, uint8_t val)
{
	if (! out)
	{
		return -1;
	}
	if (out->count < out->size)
	{
		out->data[out->head] = val;
		out->count++;
		out->head++;
		return 0;
	}
	return 1;
}

int ipp_write_uint16(ioring_t *out, uint16_t val)
{
	uint16_t uval;

	if (! out)
	{
		return -1;
	}
	if (out->count < (out->size - 1))
	{
		out->data[out->head] = (uint8_t)((val >> 8) & 0xFF);
		out->count++;
		out->head++;
		out->data[out->head] = (uint8_t)(val & 0xFF);
		out->count++;
		out->head++;
		return 0;
	}
	return 1;
}

int ipp_write_uint32(ioring_t *out, uint32_t val)
{
	uint32_t uval;

	if (! out)
	{
		return -1;
	}
	if (out->count < (out->size - 3))
	{
		out->data[out->head] = (uint8_t)((val >> 24) & 0xFF);
		out->count++;
		out->head++;
		out->data[out->head] = (uint8_t)((val >> 16) & 0xFF);
		out->count++;
		out->head++;
		out->data[out->head] = (uint8_t)((val >> 8) & 0xFF);
		out->count++;
		out->head++;
		out->data[out->head] = (uint8_t)(val & 0xFF);
		out->count++;
		out->head++;
		return 0;
	}
	return 1;
}

int ipp_write_int8(ioring_t *out, int8_t val)
{
	return ipp_write_uint8(out, (uint8_t)val);
}

int ipp_write_int16(ioring_t *out, int16_t val)
{
	return ipp_write_uint16(out, (uint16_t)val);
}

int ipp_write_int32(ioring_t *out, int32_t val)
{
	return ipp_write_uint32(out, (uint32_t)val);
}

int ipp_write_text(ioring_t *out, const char *text, uint16_t len)
{
	int result;

	if (! out || ! text)
	{
		return -1;
	}
	if (out->count > (out->size - len - 2))
	{
		return 1;
	}
	result = ipp_write_uint16(out, len);
	if (! result && (len > 0))
	{
		memcpy(out->data + out->head, text, len);
		out->head += len;
		out->count -= len;
	}
	return result;
}

int ipp_write_text_attribute(ioring_t *out, const char *text)
{
	int result;
	size_t len;

	if (! out || ! text)
	{
		return -1;
	}
	len = strlen(text);
	if (len > 65535)
	{
		return -2;
	}
	return ipp_write_text(out, text, len);
}

int ipp_write_named_attribute(ioring_t *out, int8_t tag, const char *text)
{
	int result;

	if (! out || ! text)
	{
		return -1;
	}
	result = ipp_write_int8(out, tag);
	if (result)
	{
		return result;
	}
	return ipp_write_text_attribute(out, text);
}

int ipp_write_chunk_count(ioring_t *out, int chunk)
{
	char chunk_str[8];
	int result;

	if (! out)
	{
		return -1;
	}
	snprintf(chunk_str, sizeof(chunk_str), "%04X", chunk);

	result  = ipp_write_uint8(out, chunk_str[0]);
	result |= ipp_write_uint8(out, chunk_str[1]);
	result |= ipp_write_uint8(out, chunk_str[2]);
	result |= ipp_write_uint8(out, chunk_str[3]);
    result |= ipp_write_uint8(out, '\r');
    result |= ipp_write_uint8(out, '\n');
	if (chunk == 0)
	{
	    result |= ipp_write_uint8(out, '\r');
	    result |= ipp_write_uint8(out, '\n');
	}
	return result;
}

int ipp_update_chunk_count(ioring_t *out, int chunkpos, int chunksize)
{
	char chunk_str[8];
	int cur_head;
	int cur_count;
	int result;

	if (! out)
	{
		return -1;
	}
	// save current position and count in out buffer
	cur_head  = out->head;
	cur_count = out->count;

	// seek to place reserved chunk count was placed
	out->head = chunkpos;

	// write the chunk count there
	result = ipp_write_chunk_count(out, chunksize);

	// restore position and count (had already reserved it)
	out->head  = cur_head;
	out->count = cur_count;
	return result;
}

int ipp_get_req_in_attribute(ipp_request_t *req, ipp_io_groups_t group, const char *name, ipp_attr_t **pattr)
{
	ipp_attr_t *attr;

	if (! req || ! name || ! pattr || group >= IPP_MAX_IO_ATTRS)
	{
		return -1;
	}
	*pattr = NULL;
	for (attr = req->in_attrs[group]; attr; attr = attr->next)
	{
		if (! strcmp(name, ipp_name_of_attr(attr)))
		{
			*pattr = attr;
			return 0;
		}
	}
	return -1;
}

int ipp_add_req_out_attribute(ipp_request_t *req, ipp_io_groups_t group, ipp_attr_t *pattr)
{
	ipp_attr_t *attr;

	if (! req || ! pattr || group >= IPP_MAX_IO_ATTRS)
	{
		return -1;
	}
	if (! req->out_attrs[group])
	{
		req->out_attrs[group] = pattr;
	}
	else
	{
		for (attr = req->out_attrs[group]; attr->next;)
		{
			attr = attr->next;
		}
		attr->next = pattr;
	}
	return 0;
}

ipp_request_t *ipp_req_create(ipp_server_t *ipp, http_client_t *client)
{
	ipp_request_t *req;
	size_t attrdex;

	if (! ipp || ! ipp->req_free)
	{
		return NULL;
	}
	req = ipp->req_free;
	ipp->req_free = req->next;

	req->ipp = ipp;
	req->client = client;

	req->last_error = IPP_STATUS_OK;

	req->bytes_needed = 0;
	req->download_complete = false;

	req->in.count = 0;
	req->in.size = 0;
	req->in.head = 0;
	req->in.tail = 0;

	req->op_attr_count = 0;
	req->job_attr_count = 0;

	for (attrdex = 0; attrdex < IPP_MAX_IO_ATTRS; attrdex++)
	{
		req->in_attrs[attrdex] = NULL;
		req->out_attrs[attrdex] = NULL;
	}
	req->cur_in_group = IPP_OPER_ATTRS;
	req->cur_out_group = IPP_OPER_ATTRS;
	req->cur_in_attr = NULL;
	req->cur_out_attr = NULL;

	req->top = 0;
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

