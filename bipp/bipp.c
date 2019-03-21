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
    http_client_t *client;
    int result;

    if (! ipp || ! req || ! req->client)
    {
        return -1;
    }
    client = req->client;

    // start http reply in client outbuffer
    result  = http_begin_reply(client, 200, "OK");
    result |= http_append_reply(client, "Content-Type: application/ipp");
    result |= http_append_reply(client, "Transfer-Encoding: chunked");
    result |= http_append_connection_to_reply(client, false);
    result |= http_append_reply(client, "");

    // reserve 6 bytes for reply chunk count at top of reply
    req->chunk_pos = client->out.head;
    ipp_write_uint32(req, 0);
    ipp_write_uint8(req, '\r');
    ipp_write_uint8(req, '\n');
    result = 0;
    return result;
}

int ipp_dispatch(ipp_server_t *ipp, ipp_request_t *req)
{
    if (! ipp || ! req)
    {
        return -1;
    }
    // assume we don't support this
    req->last_error = IPP_STATUS_ERROR_OPERATION_NOT_SUPPORTED;

    switch (req->opid)
    {
    case IPP_OP_PRINT_JOB:
    case IPP_OP_PRINT_URI:
    case IPP_OP_VALIDATE_JOB:
    case IPP_OP_CREATE_JOB:
    case IPP_OP_SEND_DOCUMENT:
    case IPP_OP_SEND_URI:
    case IPP_OP_CANCEL_JOB:
    case IPP_OP_GET_JOB_ATTRIBUTES:
    case IPP_OP_GET_JOBS:
        break;
    case IPP_OP_GET_PRINTER_ATTRIBUTES:
        break;
    case IPP_OP_HOLD_JOB:
    case IPP_OP_RELEASE_JOB:
    case IPP_OP_RESTART_JOB:
    case IPP_OP_PAUSE_PRINTER:
    case IPP_OP_RESUME_PRINTER:
    case IPP_OP_PURGE_JOBS:
    case IPP_OP_SET_PRINTER_ATTRIBUTES:
    case IPP_OP_SET_JOB_ATTRIBUTES:
    case IPP_OP_GET_PRINTER_SUPPORTED_VALUES:
    case IPP_OP_CREATE_PRINTER_SUBSCRIPTIONS:
    case IPP_OP_CREATE_JOB_SUBSCRIPTIONS:
    case IPP_OP_GET_SUBSCRIPTION_ATTRIBUTES:
    case IPP_OP_GET_SUBSCRIPTIONS:
    case IPP_OP_RENEW_SUBSCRIPTION:
    case IPP_OP_CANCEL_SUBSCRIPTION:
    case IPP_OP_GET_NOTIFICATIONS:
    case IPP_OP_SEND_NOTIFICATIONS:
    case IPP_OP_GET_RESOURCE_ATTRIBUTES:
    case IPP_OP_GET_RESOURCE_DATA:
    case IPP_OP_GET_RESOURCES:
    case IPP_OP_GET_PRINT_SUPPORT_FILES:
    case IPP_OP_ENABLE_PRINTER:
    case IPP_OP_DISABLE_PRINTER:
    case IPP_OP_PAUSE_PRINTER_AFTER_CURRENT_JOB:
    case IPP_OP_HOLD_NEW_JOBS:
    case IPP_OP_RELEASE_HELD_NEW_JOBS:
    case IPP_OP_DEACTIVATE_PRINTER:
    case IPP_OP_ACTIVATE_PRINTER:
    case IPP_OP_RESTART_PRINTER:
    case IPP_OP_SHUTDOWN_PRINTER:
    case IPP_OP_STARTUP_PRINTER:
    case IPP_OP_REPROCESS_JOB:
    case IPP_OP_CANCEL_CURRENT_JOB:
    case IPP_OP_SUSPEND_CURRENT_JOB:
    case IPP_OP_RESUME_JOB:
    case IPP_OP_PROMOTE_JOB:
    case IPP_OP_SCHEDULE_JOB_AFTER:
    case IPP_OP_CANCEL_DOCUMENT:
    case IPP_OP_GET_DOCUMENT_ATTRIBUTES:
    case IPP_OP_GET_DOCUMENTS:
    case IPP_OP_DELETE_DOCUMENT:
    case IPP_OP_SET_DOCUMENT_ATTRIBUTES:
    case IPP_OP_CANCEL_JOBS:
    case IPP_OP_CANCEL_MY_JOBS:
    case IPP_OP_RESUBMIT_JOB:
    case IPP_OP_CLOSE_JOB:
    case IPP_OP_IDENTIFY_PRINTER:
    case IPP_OP_VALIDATE_DOCUMENT:
    case IPP_OP_ADD_DOCUMENT_IMAGES:
    case IPP_OP_ACKNOWLEDGE_DOCUMENT:
    case IPP_OP_ACKNOWLEDGE_IDENTIFY_PRINTER:
    case IPP_OP_ACKNOWLEDGE_JOB:
    case IPP_OP_FETCH_DOCUMENT:
    case IPP_OP_FETCH_JOB:
    case IPP_OP_GET_OUTPUT_DEVICE_ATTRIBUTES:
    case IPP_OP_UPDATE_ACTIVE_JOBS:
    case IPP_OP_DEREGISTER_OUTPUT_DEVICE:
    case IPP_OP_UPDATE_DOCUMENT_STATUS:
    case IPP_OP_UPDATE_JOB_STATUS:
    case IPP_OP_UPDATE_OUTPUT_DEVICE_ATTRIBUTES:
    case IPP_OP_GET_NEXT_DOCUMENT_DATA:
    case IPP_OP_ALLOCATE_PRINTER_RESOURCES:
    case IPP_OP_CREATE_PRINTER:
    case IPP_OP_DEALLOCATE_PRINTER_RESOURCES:
    case IPP_OP_DELETE_PRINTER:
    case IPP_OP_GET_PRINTERS:
    case IPP_OP_SHUTDOWN_ONE_PRINTER:
    case IPP_OP_STARTUP_ONE_PRINTER:
    case IPP_OP_CANCEL_RESOURCE:
    case IPP_OP_CREATE_RESOURCE:
    case IPP_OP_INSTALL_RESOURCE:
    case IPP_OP_SEND_RESOURCE_DATA:
    case IPP_OP_SET_RESOURCE_ATTRIBUTES:
    case IPP_OP_CREATE_RESOURCE_SUBSCRIPTIONS:
    case IPP_OP_CREATE_SYSTEM_SUBSCRIPTIONS:
    case IPP_OP_DISABLE_ALL_PRINTERS:
    case IPP_OP_ENABLE_ALL_PRINTERS:
    case IPP_OP_GET_SYSTEM_ATTRIBUTES:
    case IPP_OP_GET_SYSTEM_SUPPORTED_VALUES:
    case IPP_OP_PAUSE_ALL_PRINTERS:
    case IPP_OP_PAUSE_ALL_PRINTERS_AFTER_CURRENT_JOB:
    case IPP_OP_REGISTER_OUTPUT_DEVICE:
    case IPP_OP_RESTART_SYSTEM:
    case IPP_OP_RESUME_ALL_PRINTERS:
    case IPP_OP_SET_SYSTEM_ATTRIBUTES:
    case IPP_OP_SHUTDOWN_ALL_PRINTERS:
    case IPP_OP_STARTUP_ALL_PRINTERS:
    default:
        return 1;
    }
    return 0;
}

int ipp_reply(ipp_server_t *ipp, ipp_request_t *req)
{
    int old_count;
    int bytes_made;
    int result;

    if (! req || ! req->client)
    {
        return -1;
    }
    old_count = req->client->out.count;

    if (req->last_error == IPP_STATUS_OK)
    {
        req->last_error = IPP_STATUS_ERROR_INTERNAL;
    }
    // add reply header
    result  = ipp_write_int8(req, req->vmaj);
    result |= ipp_write_int8(req, req->vmin);
    result |= ipp_write_int16(req, req->last_error);
    result |= ipp_write_int32(req, req->reqid);

    // and reply charset/language
    result |= ipp_write_int8(req, IPP_TAG_OPERATION);
    result |= ipp_write_named_attribute(req, IPP_TAG_CHARSET, "attributes-charset");
    result |= ipp_write_text_attribute(req, "utf-8");
    result |= ipp_write_named_attribute(req, IPP_TAG_LANGUAGE, "attributes-natural-language");
    result |= ipp_write_text_attribute(req, "en");
    result |= ipp_write_int8(req, IPP_TAG_END);

    bytes_made = req->client->out.count - old_count;

    // append a 0 chunk
    result = ipp_write_chunk_count(req, 0);

    // back-annotate the chunk count
    result = ipp_update_chunk_count(req, bytes_made);
    if (result)
    {
        // not sure what to do here, really can't happen
        BERROR("hdr write");
        return result;
    }
    // send the reply
    result = http_send_out_data(req->client, httpSendReply, httpKeepAlive);
    if (result)
    {
        return result;
    }
    return 0;
}

int ipp_process(ipp_server_t *ipp, ipp_request_t *req)
{
    static ipp_req_state_t prevstate = reqReadInput;
    static size_t prevtop;

    http_client_t *client;
    int8_t tag;
    int result;

    client = req->client;

    if (req->state[req->top] != prevstate || req->top != prevtop)
    {
        butil_log(5, "ipp state %d [%d]\n",
                req->state[req->top], req->top);
    }
    switch (req->state[req->top])
    {
    case reqHeader: // read in the IPP request header

        // wait for enough data in to procede
        result = ipp_wait_for_bytes(req, 8);
        if (result)
        {
            return result;
        }
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

        req->state[req->top] = reqAttributes;
        break;

    case reqAttributes: // incrementally parse attrubute groups

        // According to RFC 8010 3.1.1, there are 0 or more groups of
        // attributes, so having no more input is here not a failure case here
        // even though all the IPP test suites insist that a request
        // has a charset and natural language operation attributes
        //
        result = ipp_read_int8(req, &tag);
        if (result < 0)
        {
            return result;
        }
        if (result > 0)
        {
            // there are no attributes, dispatch this. this code will NOT
            // be excersized by normal IPP testing tools as they always
            // insert operation attributes so just go dispatch it
            //
            result = 0;
            req->state[req->top] = reqDispatch;
        }
        switch (tag)
        {
        case IPP_TAG_OPERATION:
        case IPP_TAG_JOB:
        case IPP_TAG_PRINTER:
        case IPP_TAG_DOCUMENT:
        case IPP_TAG_SYSTEM:
        default:
            req->last_error = IPP_STATUS_ERROR_BAD_REQUEST;
            req->state[req->top] = reqReply;
        }
        break;

    case reqOperationAttributes:
    case reqJobAttributes:
    /*
    case reqPrinterAttributes:
    case reqDocumentAttributes:
    case reqSystemAttributes:
    */
    case reqValidation: // got all the attributes we're getting, so validate

        req->last_error = IPP_STATUS_OK;

        do //TRY
        {
            // validate the request. mostly this is to pass conformance testing
            // since the protocol itself is pretty simple and would just work
            // without all this insistance on ordering/presence
            //
            // RFC 8011 section 4.1.1 - no 0 request id
            if (req->reqid == 0)
            {
                req->last_error = IPP_STATUS_ERROR_BAD_REQUEST;
                break;
            }
            // RFC 8011 section 4.1.4 - charset+natural language are first two
            // attributes.
        }
        while (0); // catch

        req->state[req->top] = reqDispatch;
        break;

    case reqDispatch:

        // dispatch the operation -----------------------------------
        //
        result = ipp_dispatch(ipp, req);
        if (! result)
        {
            req->state[req->top] = reqReply;
        }
        break;

    case reqReply:

        result = ipp_reply(ipp, req);
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

