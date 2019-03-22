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

#include "bipperror.h"

struct tag_ipp_server;

// How deep the parsing state stack can go
//
#define IPP_REQ_MAX_STACK 16

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
    reqReply,
    reqReadInput,
    reqWriteOutput,
    reqDone
}
ipp_req_state_t;

/// Context for a single IPP request/response
//
typedef struct tag_ipp_request
{
    struct tag_ipp_request *next;
    struct tag_ipp_server  *ipp;
    ipp_req_state_t state[IPP_REQ_MAX_STACK];
    size_t          top;

    ioring_t        in;
    size_t          bytes_needed;
    int             chunk_pos;
    http_client_t  *client;

    bool            download_complete;
    int             op_attr_count;
    int             job_attr_count;

    int16_t         last_error;

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
    uint16_t        attr_value_len;
    uint16_t        attr_bytes_remain;
    union
    {
        int8_t      i8v;
        uint8_t     u8v;
        int16_t     i16v;
        uint16_t    u16v;
        int32_t     i32v;
        uint32_t    u32v;
        char        tv[IPP_MAX_TEXT];
    }
                    attr_value;
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

int ipp_write_text              (ioring_t *out, const char *text, uint16_t len);
int ipp_write_text_attribute    (ioring_t *out, const char *text);
int ipp_write_named_attribute   (ioring_t *out, int8_t tag, const char *text);

int ipp_write_chunk_count       (ioring_t *out, int chunk);
int ipp_update_chunk_count      (ioring_t *out, int chunkpos, int chunksize);

ipp_request_t *ipp_req_create   (struct tag_ipp_server *ipp, http_client_t *client);
int ipp_req_destroy             (struct tag_ipp_server *ipp, ipp_request_t *req);
int ipp_req_init                (struct tag_ipp_server *ipp);

#endif

