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

int ipp_write_bytes(ioring_t *out, const uint8_t *bytes, uint16_t len)
{
    if (! out || ! bytes)
    {
        return -1;
    }
    if (out->count > (out->size - len))
    {
        return 1;
    }
    memcpy(out->data + out->head, bytes, len);
    out->head += len;
    out->count += len;

    return 0;
}

int ipp_write_length_and_bytes(ioring_t *out, const uint8_t *bytes, uint16_t len)
{
    int result;

    if (! out || ! bytes)
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
        memcpy(out->data + out->head, bytes, len);
        out->head += len;
        out->count += len;
    }
    return result;
}

int ipp_write_text(ioring_t *out, const char *text, uint16_t len)
{
    return ipp_write_length_and_bytes(out, (const uint8_t *)text, len);
}

int ipp_write_attribute_text(ioring_t *out, const char *text)
{
    int result;
    size_t len;

    if (! out || ! text)
    {
        return -1;
    }
    len = strlen(text);
    if (len > IPP_MAX_LENGTH)
    {
        return -2;
    }
    return ipp_write_text(out, text, len);
}

int ipp_write_attribute_name(ioring_t *out, int8_t tag, const char *text)
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
    return ipp_write_attribute_text(out, text);
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

int ipp_grouping_for_io_group(
                                ipp_io_groups_t group,
                                ipp_attr_grouping_code_t *grouping
                              )
{
    switch (group)
    {
    case IPP_OPER_ATTRS:
        *grouping = IPP_GROUPING_OPERATION;
        break;
    case IPP_JOB_ATTRS:
        *grouping = IPP_GROUPING_JOB_STATUS;
        break;
    case IPP_PRT_ATTRS:
        *grouping = IPP_GROUPING_PRINTER_STATUS;
        break;
    default:
        return -1;
    }
    return 0;
}

int ipp_get_attr_template(
                               ipp_request_t *req,
                               ipp_io_groups_t group,
                               const char *name,
                                ipp_attr_t **pattr
                        )
{
    ipp_attr_grouping_code_t grouping;
    ipp_attr_t *attr;
    int result;

    // get group table from io group
    result = ipp_grouping_for_io_group(group, &grouping);
    if (result)
    {
        butil_log(1, "No grouping table for group %u\n", group);
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return result;
    }
    // lookup name in group table
    result = ipp_get_group_attr_by_name(name, grouping, &attr);
    if (result)
    {
        butil_log(1, "No attribute %s in grouping %u\n", name, grouping);
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return result;
    }
    // duplicate the attribute template in table
    result = ipp_dupe_attr(attr, pattr);
    if (result)
    {
        butil_log(0, "Alloc attr failed\n");
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return result;
    }
    return result;
}

int ipp_set_req_out_bytes_attr(
                                ipp_request_t *req,
                                ipp_io_groups_t group,
                                const char *name,
                                const uint8_t *value,
                                size_t value_len
                                )
{
    ipp_attr_grouping_code_t grouping;
    ipp_attr_t *attr;
    int result;

    if (! name || ! value)
    {
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return -1;
    }
    // generate a attribute to use based on template in group table
    result = ipp_get_attr_template(req, group, name, &attr);
    if (result)
    {
        // that call would have set error code and dbg print
        return result;
    }
    // set that attribute's value with the string value
    result = ipp_set_attr_value(attr, value, value_len);
    if (result)
    {
        butil_log(0, "Set attr %s with %s failed\n", name, value);
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return result;
    }
    // add that value to the request's group
    result = ipp_add_req_out_attribute(req, group, attr);
    if (result)
    {
        butil_log(0, "Add attr %s into response failed\n", name);
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return result;
    }
    return result;
}

int ipp_set_req_out_string_attr(
                                ipp_request_t *req,
                                ipp_io_groups_t group,
                                const char *name,
                                const char *value
                                )
{
    int result;
    size_t len;

    if (! req || ! name || ! value)
    {
        return -1;
    }
    len = strlen(value);
    result = ipp_set_req_out_bytes_attr(
                                        req,
                                        group,
                                        name,
                                        (uint8_t*)value,
                                        len
                                        );
    return result;
}

int ipp_set_req_out_int32_attr(
                                ipp_request_t *req,
                                ipp_io_groups_t group,
                                const char *name,
                                const int32_t value
                                )
{
    int32_t nvalue;
    int result;

    if (! req || ! name || ! value)
    {
        return -1;
    }
    // note attr ints in network byte order
    nvalue = htonl(value);

    result = ipp_set_req_out_bytes_attr(
                                        req,
                                        group,
                                        name,
                                        (uint8_t*)&nvalue,
                                        sizeof(int32_t)
                                        );
    return result;
}

int ipp_set_req_out_tag(
                                ipp_request_t *req,
                                ipp_io_groups_t group,
                                ipp_tag_t       tag
                                )
{
    ipp_attr_t *attr;
    int result;

    attr = ipp_create_attr(IPP_RECDEX_TAG, NULL, 0);
    if (! attr)
    {
        BERROR("Create attr");
        return -1;
    }
    attr->alloc_len = 2;
    attr->value = (uint8_t*)malloc(attr->alloc_len);
    if (! attr->value)
    {
        BERROR("alloc tag val");
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        ipp_destroy_attr(attr);
        return -1;
    }
    attr->value[0] = tag;
    attr->value_len = 1;

    result = ipp_add_req_out_attribute(req, group, attr);
    if (result)
    {
        butil_log(0, "Add tag %u into response failed\n", tag);
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return result;
    }
    return result;
}

int ipp_add_req_out_group(
                                ipp_request_t *req,
                                ipp_attr_grouping_code_t grouping
                                )
{
    ipp_attr_t *attr;
    ipp_attr_t *nattr;
    int result;

    if (! req)
    {
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return -1;
    }
    result = ipp_get_attr_for_grouping(grouping, &attr);
    if (result)
    {
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return result;
    }
    while (attr)
    {
        if (attr->value)
        {
            result = ipp_dupe_attr(attr, &nattr);
            if (result)
            {
                ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
                return result;
            }
            result = ipp_add_req_out_attribute(req, IPP_PRT_ATTRS, nattr);
            if (result)
            {
                return result;
            }
        }
        attr = attr->next;
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

    req->job = NULL;

    req->bytes_needed = 0;
    req->download_complete = false;

    req->in.count = 0;
    req->in.size = 0;
    req->in.head = 0;
    req->in.tail = 0;
    req->in.data = NULL;

    req->out.count = 0;
    req->out.size = 0;
    req->out.head = 0;
    req->out.tail = 0;
    req->out.data = NULL;

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
    ipp_attr_t *attr;
    ipp_attr_t *nattr;
    ipp_io_groups_t gdex;

    if (! ipp || ! req)
    {
        return -1;
    }
    // free any attribute lists
    for (gdex = IPP_OPER_ATTRS; gdex < IPP_MAX_IO_ATTRS; gdex++)
    {
        ipp_destroy_attrlist(req->in_attrs[gdex]);
        ipp_destroy_attrlist(req->out_attrs[gdex]);
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

