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

struct tag_ipp_server;

#define IPP_REQ_MAX_STACK 16

typedef enum
{
    reqHeader,
    reqReadInput,
    reqWriteOutput
}
ipp_req_state_t;

/// Context for a single IPP request/response
//
typedef struct tag_ipp_request
{
    ipp_req_state_t state[IPP_REQ_MAX_STACK];
    size_t top;
    size_t bytes_needed;
    struct tag_ipp_request *next;
    http_client_t *client;

    int16_t  last_error;

    // IPP request header. Yes spec says they are signed
    //
    int8_t   vmaj, vmin;
    int16_t  opid;
    int16_t  status;
    int32_t  reqid;
}
ipp_request_t;

int ipp_read_uint8      (ipp_request_t *req, uint8_t *val);
int ipp_read_uint16     (ipp_request_t *req, uint16_t *val);
int ipp_read_uint32     (ipp_request_t *req, uint32_t *val);

int ipp_read_int8       (ipp_request_t *req, int8_t *val);
int ipp_read_int16      (ipp_request_t *req, int16_t *val);
int ipp_read_int32      (ipp_request_t *req, int32_t *val);

int ipp_write_uint8     (ipp_request_t *req, uint8_t val);
int ipp_write_uint16    (ipp_request_t *req, uint16_t val);
int ipp_write_uint32    (ipp_request_t *req, uint32_t val);

int ipp_write_int8      (ipp_request_t *req, int8_t val);
int ipp_write_int16     (ipp_request_t *req, int16_t val);
int ipp_write_int32     (ipp_request_t *req, int32_t val);

ipp_request_t *ipp_req_create   (struct tag_ipp_server *ipp, http_client_t *client);
int ipp_req_destroy             (struct tag_ipp_server *ipp, ipp_request_t *req);
int ipp_req_init                (struct tag_ipp_server *ipp);

#endif

