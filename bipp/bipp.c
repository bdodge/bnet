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
#include "bipperror.h"
#include "butil.h"

static int ipp_push_state(ipp_request_t *req, ipp_req_state_t tostate)
{
    if (req->top >= IPP_REQ_MAX_STACK)
    {
        BERROR("req stack depth");
        return -1;
    }
    req->state[++req->top] = tostate;
    return 0;
}

static int ipp_pop_state(ipp_request_t *req)
{
    if (req->top == 0)
    {
        BERROR("req stack bottom");
        return -1;
    }
    req->top--;
    return 0;
}

static int ipp_wait_for_bytes(ipp_request_t *req, size_t bytes)
{
    if (req->client->in.count >= bytes)
    {
        return 0;
    }
    req->bytes_needed = bytes;
    return ipp_push_state(req, reqReadInput);
}

int ipp_request(ipp_server_t *ipp, ipp_request_t *req)
{
    return 0;
}

int ipp_dispatch(ipp_server_t *ipp, ipp_request_t *req)
{
    if (! ipp || ! req)
    {
        return -1;
    }
    switch (req->opid)
    {
    default:
        req->last_error = IPP_STATUS_ERROR_OPERATION_NOT_SUPPORTED;
        return 1;
    }
    return 0;
}

int ipp_process(ipp_server_t *ipp, ipp_request_t *req)
{
    static ipp_req_state_t prevstate = reqReadInput;
    static size_t prevtop;

    http_client_t *client;
    int result;

    client = req->client;

    if (req->state[req->top] != prevstate || req->top != prevtop)
    {
        butil_log(5, "ipp state %d [%d]\n",
                req->state[req->top], req->top);
    }
    switch (req->state[req->top])
    {
    case reqHeader:
        // wait for enough data in to procede
        result = ipp_wait_for_bytes(req, 8);
        if (result)
        {
            return result;
        }
        {
            result  = ipp_read_int8(req, &req->vmaj);
            result |= ipp_read_int8(req, &req->vmin);
            result |= ipp_read_int16(req, &req->opid);
            result |= ipp_read_int32(req, &req->reqid);
            if (result)
            {
                BERROR("hdr read");
                return result;
            }
            butil_log(5, "IPP %d.%d op=%04X reqid=%d\n", req->vmaj, req->vmin, req->opid, req->reqid);

            // assume no errors
            //
            if (req->reqid == 0)
            {
                req->last_error = IPP_STATUS_ERROR_BAD_REQUEST;
            }
            else
            {
                req->last_error = IPP_STATUS_OK;

                // dispatch the operation
                //
                result = ipp_dispatch(ipp, req);
            }
            if (result || req->last_error != IPP_STATUS_OK)
            {
                if (req->last_error == IPP_STATUS_OK)
                {
                    req->last_error = IPP_STATUS_ERROR_INTERNAL;
                }
                // start http reply in client outbuffer
                result  = http_begin_reply(client, 200, "OK");
                result |= http_append_reply(client, "Content-Type: application/ipp");
                result |= http_append_reply(client, "Content-Length: 8");
                result |= http_append_connection_to_reply(client, false);
                result |= http_append_reply(client, "");

                // add reply header (only)
                result  = ipp_write_int8(req, req->vmaj);
                result |= ipp_write_int8(req, req->vmin);
                result |= ipp_write_int16(req, req->last_error);
                result |= ipp_write_int32(req, req->reqid);
                /*
                if (result)
                {
                    // not sure what to do here, really can't happen
                    BERROR("hdr write");
                    return result;
                }
                */
                // send the reply
                result = http_send_out_data(client, httpSendReply, httpKeepAlive);
                if (result)
                {
                    return result;
                }
            }
        }
        break;

    case reqReadInput:
        if (client->in.count >= req->bytes_needed)
        {
            result = ipp_pop_state(req);
            if (result)
            {
                return result;
            }
        }
        break;

    default:
        break;
    }
    return 0;
}

