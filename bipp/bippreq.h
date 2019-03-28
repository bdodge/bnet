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
#ifndef BIPPREQ_H
#define BIPPREQ_H 1

#include "bippproto.h"
#include "bippattr.h"
#include "bhttp.h"

struct tag_ipp_server;

// How deep the parsing state stack can go
//
#define IPP_REQ_MAX_STACK 4

typedef enum
{
    reqHeader,
    reqAttributes,
    reqOperationAttributes,
    reqJobAttributes,
    reqPrinterAttributes,
    reqAttributeTag,
    reqAttributeNameLength,
    reqAttributeNameText,
    reqAttributeValueLength,
    reqAttributeValue,
    reqValidation,
    reqDispatch,
    reqPrintData,
    reqReply,
    reqReplyOneAttribute,
    reqReplyAttributeValue,
    reqReplyOperationAttributes,
    reqReplyJobAttributes,
    reqReplyPrinterAttributes,
    reqReadInput,
    reqWriteOutput,
    reqDone
}
ipp_req_state_t;

typedef enum
{
    IPP_OPER_ATTRS,
    IPP_PRT_ATTRS,
    IPP_JOB_ATTRS,
    IPP_MAX_IO_ATTRS
}
ipp_io_groups_t;

/// Context for a single IPP request/response
//
typedef struct tag_ipp_request
{
    struct tag_ipp_request *next;
    struct tag_ipp_server  *ipp;
    ipp_req_state_t state[IPP_REQ_MAX_STACK];
    size_t          top;

    ioring_t        in;
    ioring_t        out;
    size_t          bytes_needed;
    int             chunk_pos;
    http_client_t  *client;

    bool            download_complete;
    int             op_attr_count;
    int             job_attr_count;

    int16_t         last_error;

    // attribute groups, input
    //
    ipp_attr_t     *in_attrs[IPP_MAX_IO_ATTRS];
    ipp_attr_t     *out_attrs[IPP_MAX_IO_ATTRS];
    ipp_io_groups_t cur_in_group;
    ipp_io_groups_t cur_out_group;
    ipp_attr_t     *cur_in_attr;
    ipp_attr_t     *cur_out_attr;

    // IPP request header. Yes spec says they are signed
    //
    int8_t          vmaj, vmin;
    int16_t         opid;
    int16_t         status;
    int32_t         reqid;

    // current attribute parsing
    //
    int8_t          attr_tag;
    uint16_t        attr_name_len;
    char            attr_name[IPP_MAX_TEXT];
    uint8_t         attr_value[IPP_MAX_LENGTH];
    uint16_t        attr_value_len;
    uint16_t        attr_bytes_read;

    // for incrementally sending attr value
    uint8_t        *attr_out_value;
}
ipp_request_t;

int ipp_read_uint8      (ioring_t *in, uint8_t *val);
int ipp_read_uint16     (ioring_t *in, uint16_t *val);
int ipp_read_uint32     (ioring_t *in, uint32_t *val);

int ipp_read_int8       (ioring_t *in, int8_t *val);
int ipp_read_int16      (ioring_t *in, int16_t *val);
int ipp_read_int32      (ioring_t *in, int32_t *val);

int ipp_read_text       (ioring_t *in, char *text, uint16_t len);

int ipp_write_uint8     (ioring_t *in, uint8_t val);
int ipp_write_uint16    (ioring_t *in, uint16_t val);
int ipp_write_uint32    (ioring_t *in, uint32_t val);

int ipp_write_int8      (ioring_t *in, int8_t val);
int ipp_write_int16     (ioring_t *in, int16_t val);
int ipp_write_int32     (ioring_t *in, int32_t val);

int ipp_write_bytes             (ioring_t *out, const uint8_t *bytes, uint16_t len);
int ipp_write_length_and_bytes  (ioring_t *out, const uint8_t *bytes, uint16_t len);
int ipp_write_text              (ioring_t *out, const char *text, uint16_t len);
int ipp_write_attribute_text    (ioring_t *out, const char *text);
int ipp_write_attribute_name    (ioring_t *out, int8_t tag, const char *text);

int ipp_get_req_in_attribute    (
                                ipp_request_t  *req,
                                ipp_io_groups_t group,
                                const char     *name,
                                ipp_attr_t    **pattr
                                );
int ipp_add_req_out_attribute   (
                                ipp_request_t  *req,
                                ipp_io_groups_t group,
                                ipp_attr_t     *pattr
                                );

int ipp_set_req_out_bytes_attr(
                                ipp_request_t *req,
                                ipp_io_groups_t group,
                                const char *name,
                                const uint8_t *value,
                                size_t value_len
                                );

int ipp_set_req_out_string_attr(
                                ipp_request_t *req,
                                ipp_io_groups_t group,
                                const char *name,
                                const char *value
                                );

int ipp_set_req_out_int32_attr(
                                ipp_request_t *req,
                                ipp_io_groups_t group,
                                const char *name,
                                const int32_t value
                                );

int ipp_set_req_out_tag(
                                ipp_request_t *req,
                                ipp_io_groups_t group,
                                ipp_tag_t       tag
                                );

ipp_request_t *ipp_req_create   (struct tag_ipp_server *ipp, http_client_t *client);
int ipp_req_destroy             (struct tag_ipp_server *ipp, ipp_request_t *req);
int ipp_req_init                (struct tag_ipp_server *ipp);

#endif

