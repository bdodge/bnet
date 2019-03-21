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
        // wait for enough data
        result = ipp_wait_for_bytes(req, 8);
        if (result)
        {
            return result;
        }
        {
            int16_t op;

            result = ipp_read_int16(req, &op);

            butil_log(5, "%d op=%04X\n", result, op);
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

