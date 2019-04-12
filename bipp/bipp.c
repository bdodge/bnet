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
#include "bippproto.h"
#include "bippprtops.h"
#include "bippjobops.h"
#include "butil.h"

// log level for attribute parsing
#define IPPLAP 6

const char *ipp_state_string(ipp_req_state_t state)
{
    switch (state)
    {
    case reqReadInput:              return "ReadInput";
    case reqWriteOutput:            return "WriteOutput";
    case reqHeader:                 return "Header";
    case reqAttributes:             return "Attributes";
    case reqOperationAttributes:    return "OperationAttributes";
    case reqJobAttributes:          return "JobAttributes";
    case reqPrinterAttributes:      return "PrinterAttributes";
    case reqAttributeTag:           return "AttributeTag";
    case reqAttributeNameLength:    return "AttributeNameLength";
    case reqAttributeNameText:      return "AttributeNameText";
    case reqAttributeValueLength:   return "AttributeValueLength";
    case reqAttributeValue:         return "AttributeValue";
    case reqValidation:             return "Validation";
    case reqDispatch:               return "Dispatch";
    case reqPrintData:              return "PrintData";
    case reqReply:                  return "Reply";
    case reqReplyOneAttribute:      return "ReplyOneAttribute";
    case reqReplyAttributeValue:    return "ReplyAttributeValue";
    case reqReplyOperationAttributes:return "ReplyOperationAttributes";
    case reqReplyJobAttributes:     return "ReplyJobAttributes";
    case reqReplyPrinterAttributes: return "ReplyPrinterAttributes";
    case reqDone:                   return "Done";
    default:                        return "????";
    }
}

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

static int ipp_move_state(ipp_request_t *req, ipp_req_state_t tostate)
{
    req->state[req->top] = tostate;
    return 0;
}

static int ipp_wait_for_bytes(ipp_request_t *req, size_t bytes)
{
    if (req->in.count >= bytes)
    {
        return 0;
    }
    req->bytes_needed = bytes;
    return ipp_push_state(req, reqReadInput);
}

#if 1
#define ipp_set_error(a, b) ipp_set_error_dbg(__FILE__, __LINE__, a, b)
int ipp_set_error_dbg(const char* fname, int line, ipp_request_t *req, int16_t ecode)
{
    butil_log(((ecode == IPP_STATUS_OK) ? 7 : 1),
            "***** %s:%d set error %u\n", fname, line, ecode);
    if (req)
    {
        req->last_error = ecode;
    }
    return 0;
}
#else
int ipp_set_error(ipp_request_t *req, int16_t ecode)
{
    butil_log(1, "***** Set error %u\n", ecode);
    if (req)
    {
        req->last_error = ecode;
    }
    return 0;
}
#endif

static int ipp_parse_value(ipp_request_t *req)
{
    ipp_attr_t *attr;
    size_t recdex;
    int result;
    uint8_t vb;

    // incrementally reading value bytes
    //
    while (req->attr_bytes_read < req->attr_value_len)
    {
        result = ipp_read_uint8(&req->in, &vb);
        if (result < 0)
        {
            return result;
        }
        if (result > 0)
        {
            if (req->download_complete)
            {
                butil_log(0, "End of data in attr value\n");
                return -1;
            }
            // need bytes to move on, but do it 1 at a time
            //
            return ipp_wait_for_bytes(req, 1);
        }
        req->attr_value[req->attr_bytes_read++] = vb;
    }
    // got all value bytes, create and add the attribute to the group
    //
    // first, if this is a secondary value (cur_attr not null)
    //
    if (req->cur_attr)
    {
        result = ipp_add_attr_value(req->cur_attr, req->attr_value, req->attr_value_len);
        if (result)
        {
            butil_log(1, "Bad secondary value for %s\n", req->attr_name);
            ipp_set_error(req, IPP_STATUS_ERROR_ATTRIBUTES_OR_VALUES);
            return result;
        }
    }
    else
    {
        // make sure it's actually an attr
        //
        result = ipp_find_attr_rec(req->attr_name, &recdex, NULL);
        if (result)
        {
            butil_log(1, "No such attribute: %s\n", req->attr_name);
            ipp_set_error(req, IPP_STATUS_ERROR_ATTRIBUTES_OR_VALUES);
            return result;
        }
        attr = ipp_create_attr(recdex, req->attr_value, req->attr_value_len);
        if (! attr)
        {
            butil_log(0, "Can't alloc attr\n");
            ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
            return -1;
        }
        attr->next = NULL;

        // if there is a collection active, add this as a member to it
        //
        if (req->col_top >= 0)
        {
            result = ipp_add_member_attrs_to_attr(req->cur_collection[req->col_top], attr);
            if (result)
            {
                butil_log(0, "Can't add member %s\n", req->attr_name);
                ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
                return result;
            }
        }
        else
        {
            // add it to the right in group
            //
            if (req->in_attrs[req->cur_in_group] == NULL)
            {
                req->in_attrs[req->cur_in_group] = attr;
            }
            else
            {
                req->cur_in_attr->next = attr;
            }
            req->cur_in_attr = attr;
        }
        // if the attribute begins a collection, then set the
        // current collection (stack it on top) so that
        // subsequent values are added as members to the collection
        //
        // note that only the top level collection is added to
        // the req->in_attrs list
        //
        if (req->attr_tag == IPP_TAG_BEGIN_COLLECTION)
        {
            if (req->col_top >= IPP_MAX_NESTED_COLLECTIONS)
            {
                butil_log(0, "Collections nested too deep\n");
                ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
                return -1;
            }
            req->col_top++;
            req->cur_collection[req->col_top] = attr;
        }
        // set the currently building attribute so secondary
        // values can be added if its an array type
        // this gets reset the next non-0 name length parsed
        //
        req->cur_attr = attr;
    }
    // and then go back to wherever we got here from
    //
    return ipp_pop_state(req);
}

static int ipp_parse_value_length(ipp_request_t *req)
{
    int result;

    result = ipp_read_uint16(&req->in, &req->attr_value_len);
    if (result < 0)
    {
        return result;
    }
    if (result > 0)
    {
        if (req->download_complete)
        {
            butil_log(0, "End of data in attr value length\n");
            return -1;
        }
        // need two bytes to move on
        //
        return ipp_wait_for_bytes(req, 2);
    }
    if (req->attr_value_len > IPP_MAX_LENGTH)
    {
        butil_log(0, "Length %u too long\n", req->attr_value_len);
        ipp_set_error(req, IPP_STATUS_ERROR_BAD_REQUEST);
        return -1;
    }
    req->attr_bytes_read = 0;
    return ipp_move_state(req, reqAttributeValue);
}

static int ipp_parse_name_text(ipp_request_t *req)
{
    int result;

    result = ipp_read_text(&req->in, req->attr_name, req->attr_name_len);
    if (result < 0)
    {
        return 0;
    }
    if (result > 0)
    {
        if (req->download_complete)
        {
            butil_log(0, "End of data in attr name\n");
            return -1;
        }
        // need len bytes to move on
        //
        return ipp_wait_for_bytes(req, req->attr_name_len);
    }
    req->attr_name[req->attr_name_len] = '\0';
    return ipp_move_state(req, reqAttributeValueLength);
}

static int ipp_parse_name_length(ipp_request_t *req)
{
    int result;

    result = ipp_read_uint16(&req->in, &req->attr_name_len);
    if (result < 0)
    {
        return result;
    }
    if (result > 0)
    {
        if (req->download_complete)
        {
            butil_log(0, "End of data in attr name length\n");
            return -1;
        }
        // need two bytes to move on
        //
        return ipp_wait_for_bytes(req, 2);
    }
    if (req->attr_name_len >= IPP_MAX_TEXT)
    {
        butil_log(0, "Name Length %u too long\n", req->attr_name_len);
        ipp_set_error(req, IPP_STATUS_ERROR_BAD_REQUEST);
        return -1;
    }
    if (req->attr_name_len == 0)
    {
        // there is more value for the last attribute, so got to valuelen state
        //
        return ipp_move_state(req, reqAttributeValueLength);
    }
    req->cur_attr = NULL;

    return ipp_move_state(req, reqAttributeNameText);
}

static int ipp_parse_attribute(ipp_request_t *req)
{
    int result;

    // read first byte. if end-tag, pop state, it no data, wait for it
    //
    result = ipp_read_int8(&req->in, &req->attr_tag);
    if (result < 0)
    {
        return result;
    }
    if (result > 0)
    {
        if (req->download_complete)
        {
            butil_log(0, "End of data in attr group, %s\n", ipp_state_string(req->state[req->top]));
            return -1;
        }
        // need a byte to move on
        //
        return ipp_wait_for_bytes(req, 1);
    }
    if (req->attr_tag == IPP_TAG_END)
    {
        butil_log(IPPLAP, "End of attr in group, %s\n", ipp_state_string(req->state[req->top]));
        result = ipp_pop_state(req);
        if (result)
        {
            return result;
        }
        result = ipp_pop_state(req);
        if (result)
        {
            return result;
        }
        if (req->state[req->top] != reqAttributes)
        {
            BERROR("expected reqAttributes state");
            return -1;
        }
        butil_log(5, "IPP Attributes parsed, validate and dispatch\n");
        return ipp_move_state(req, reqValidation);
    }
    if (req->attr_tag == IPP_TAG_JOB)
    {
        req->cur_in_group = IPP_JOB_ATTRS;
        butil_log(IPPLAP, "Move to JOB attr from group, %s\n", ipp_state_string(req->state[req->top]));
        return ipp_move_state(req, reqJobAttributes);
    }
    if (req->attr_tag == IPP_TAG_PRINTER)
    {
        req->cur_in_group = IPP_PRT_ATTRS;
        butil_log(IPPLAP, "Move to PRT attr from group, %s\n", ipp_state_string(req->state[req->top]));
        return ipp_move_state(req, reqPrinterAttributes);
    }
    return ipp_move_state(req, reqAttributeNameLength);
}

int ipp_request(ipp_request_t *req)
{
    http_client_t *client;
    int result;

    if (! req || ! req->client)
    {
        return -1;
    }
    client = req->client;

    // start http reply in client outbuffer
    result  = http_begin_reply(client, 200, "OK");
    result |= http_append_reply(client, "Content-Type: application/ipp");
    result |= http_append_reply(client, "Transfer-Encoding: chunked");
    result |= http_append_connection_to_reply(client, false);
    // don't add a blank line, the chunk count will do that
    result = 0;
    return result;
}

int ipp_dispatch(ipp_request_t *req)
{
    ipp_attr_t *attr;
    int result;

    if (! req)
    {
        return -1;
    }
    switch (req->opid)
    {
    case IPP_OP_GET_PRINTER_ATTRIBUTES:
    case IPP_OP_PRINT_JOB:
    case IPP_OP_SEND_DOCUMENT:
        result = ipp_printer_op_dispatch(req);
        break;

    case IPP_OP_VALIDATE_JOB:
    case IPP_OP_CREATE_JOB:
    case IPP_OP_CANCEL_JOB:
    case IPP_OP_GET_JOB_ATTRIBUTES:
    case IPP_OP_GET_JOBS:
    case IPP_OP_HOLD_JOB:
    case IPP_OP_RELEASE_JOB:
    case IPP_OP_RESTART_JOB:
    case IPP_OP_PURGE_JOBS:
    case IPP_OP_SET_JOB_ATTRIBUTES:
        result = ipp_job_op_dispatch(req);
        break;

    case IPP_OP_PRINT_URI:
    case IPP_OP_SEND_URI:
    case IPP_OP_PAUSE_PRINTER:
    case IPP_OP_RESUME_PRINTER:
    case IPP_OP_SET_PRINTER_ATTRIBUTES:
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
        ipp_set_error(req, IPP_STATUS_ERROR_OPERATION_NOT_SUPPORTED);
        result = -1;
    }
    return result;
}

int ipp_send_tag(ipp_request_t *req)
{
    ipp_attr_t *attr;
    int result;

    // get current attribute sending
    //
    attr = req->cur_out_attr;
    if (! attr)
    {
        return ipp_pop_state(req);
    }
    if (req->out.count == req->out.size)
    {
        butil_log(IPPLAP, "Flush output to add tag\n");
        result = ipp_push_state(req, reqWriteOutput);
    }
    else
    {
        result = ipp_write_bytes(&req->out, attr->value, attr->value_len);
        if (! result)
        {
            result = ipp_pop_state(req);
        }
        else
        {
            ipp_pop_state(req);
        }
        // move to next out attr to send
        //
        req->cur_out_attr = req->cur_out_attr->next;
    }
    return result;
}

int ipp_send_attribute(ipp_request_t *req)
{
    ipp_attr_rec_t *attrec;
    ipp_attr_t *attr;
    int room;
    int result;
    ipp_tag_t tag;
    bool isarray;
    size_t len;
    uint16_t namelen;
    uint16_t vallen;

    // get current attribute sending
    //
    attr = req->cur_out_attr;
    if (! attr)
    {
        return ipp_pop_state(req);
    }
    // if its a special (tag) send it
    //
    if (attr->recdex == IPP_RECDEX_TAG)
    {
        return ipp_send_tag(req);
    }
    // if there's no value, there's no value sending it
    //
    if (! attr->value || ! attr->value_len)
    {
        // move to next out attr to send
        //
        result = ipp_pop_state(req);
        req->cur_out_attr = req->cur_out_attr->next;
        return result;
    }
    // get name for attr
    //
    result = ipp_get_attr_rec(attr, &attrec);
    if (result || ! attrec)
    {
        return -1;
    }
    // make sure there is room in the output buffer for the attribute's name/value
    //
    namelen = strlen(attrec->name);
    vallen  = attr->value_len;
    tag = attr->value[0];

    room = req->out.size - req->out.count;

    len = (size_t)namelen + (size_t)vallen + 5;

    if (len >= (HTTP_IO_SIZE - 8))
    {
        // the value is really big, send name first, then value
        // we expect name at least to fit, but values can be huge
        // so they are split into their own chunks if needed
        //
        if ((namelen + 5) > room)
        {
            // flush and come back exactly to here, room will be bigger
            //
            butil_log(IPPLAP, "Flush output to add attribute name\n");
            result = ipp_push_state(req, reqWriteOutput);
        }
        else
        {
            // setup to incrementally send value bytes
            //
            req->attr_out_value = attr->value + 1;
            req->attr_value_len = attr->value_len - 1;

            // write out name and come back to write value
            //
            result = ipp_write_attribute_name(&req->out, tag, attrec->name);

            // for collection tags, insert a 0 value length
            if (tag == IPP_TAG_BEGIN_COLLECTION)
            {
                result = ipp_write_int16(&req->out, 0);

                // TODO - each value is actually an attibute, so send them
                // individually as member-values and when complete, terminate
                // this collection
            }
            result |= ipp_move_state(req, reqReplyAttributeValue);
            if (! result)
            {
                result = ipp_push_state(req, reqWriteOutput);
            }
        }
    }
    else
    {
        if (len > room)
        {
            butil_log(IPPLAP, "Flush output to add attribute\n");
            result = ipp_push_state(req, reqWriteOutput);
        }
        else
        {
            result = ipp_write_attribute_name(&req->out, tag, attrec->name);

            // for collection tags, insert a 0 value length
            if (tag == IPP_TAG_BEGIN_COLLECTION)
            {
                result |= ipp_write_int16(&req->out, 0);

                // TODO - each value is actually an attibute, so send them
                // individually as member-values and when complete, terminate
                // this collection
            }
            result |= ipp_write_bytes(&req->out, attr->value + 1, attr->value_len - 1);

            if (! result)
            {
                result = ipp_pop_state(req);
            }
            else
            {
                ipp_pop_state(req);
            }
            // move to next out attr to send
            //
            req->cur_out_attr = req->cur_out_attr->next;
        }
    }
    return result;
}

int ipp_send_attribute_value(ipp_request_t *req)
{
    ipp_attr_t *attr;
    int room;
    int result;
    size_t len;

    // get current attribute sending
    //
    attr = req->cur_out_attr;
    if (! attr)
    {
        return ipp_pop_state(req);
    }
    room = req->out.size - req->out.count;

    if (room < 64)
    {
        butil_log(IPPLAP, "Flush output to add (more) attribute value\n");
        result = ipp_push_state(req, reqWriteOutput);
    }
    else
    {
        if (room > req->attr_value_len)
        {
            room = req->attr_value_len;
        }
        result = ipp_write_bytes(&req->out, req->attr_out_value, room);
        if (! result)
        {
            req->attr_out_value += room;
            req->attr_value_len -= room;

            if (req->attr_value_len == 0)
            {
                butil_log(IPPLAP, "Finished sending attribute value\n");
                req->attr_out_value = NULL;
                result = ipp_pop_state(req);

                // move to next out attr to send
                //
                req->cur_out_attr = req->cur_out_attr->next;
            }
        }
        else
        {
            ipp_pop_state(req);
        }
    }
    return result;
}

int ipp_reply(ipp_request_t *req)
{
    http_client_t *client;
    int result;

    if (! req || ! req->client)
    {
        return -1;
    }
    // add reply header
    result  = ipp_write_int8(&req->out, req->vmaj);
    result |= ipp_write_int8(&req->out, req->vmin);
    result |= ipp_write_int16(&req->out, req->last_error);
    result |= ipp_write_int32(&req->out, req->reqid);

    // and reply charset/language, always
    result |= ipp_write_int8(&req->out, IPP_TAG_OPERATION);
    result |= ipp_write_attribute_name(&req->out, IPP_TAG_CHARSET, "attributes-charset");
    result |= ipp_write_attribute_text(&req->out, "utf-8");
    result |= ipp_write_attribute_name(&req->out, IPP_TAG_LANGUAGE, "attributes-natural-language");
    result |= ipp_write_attribute_text(&req->out, "en");

    // if returning an error, ignore response attr groups and flush response
    //
    if (req->last_error != IPP_STATUS_OK)
    {
        // finish operations group and go to done
        //
        result |= ipp_write_int8(&req->out, IPP_TAG_END);
        if (result)
        {
            // means output buffer too small? can't happen in?
            BERROR("hdr write");
            return result;
        }
        result = ipp_move_state(req, reqDone);
    }
    else
    {
        // setup to incrementally upload response attributes
        //
        req->cur_out_group = IPP_OPER_ATTRS;
        req->cur_out_attr = req->out_attrs[IPP_OPER_ATTRS];

        // send the reply but come back to upload the response, starting
        // with operation attributes
        //
        result = ipp_move_state(req, reqReplyOperationAttributes);
    }
    return result;
}

int ipp_process(ipp_request_t *req)
{
    static ipp_req_state_t prevstate = reqReadInput;
    static size_t prevtop;

    http_client_t *client;
    int8_t tag;
    int result;

    client = req->client;

    if (req->state[req->top] != prevstate || req->top != prevtop)
    {
        butil_log(IPPLAP, "ipp state %s [%d]\n",
                ipp_state_string(req->state[req->top]), req->top);
    }
    result = 0;

    switch (req->state[req->top])
    {
    case reqHeader: // read in the IPP request header

        // wait for enough data in to procede
        //
        if (req->in.count < 8)
        {
            return ipp_wait_for_bytes(req, 8);
        }
        result  = ipp_read_int8(&req->in, &req->vmaj);
        result |= ipp_read_int8(&req->in, &req->vmin);
        result |= ipp_read_int16(&req->in, &req->opid);
        result |= ipp_read_int32(&req->in, &req->reqid);
        if (result)
        {
            BERROR("hdr read");
            break;
        }
        butil_log(IPPLAP, "IPP (%d) %d.%d op=%04X reqid=%d\n",
                   result, req->vmaj, req->vmin, req->opid, req->reqid);

        result = ipp_move_state(req, reqAttributes);
        break;

    case reqAttributes: // incrementally parse attribute groups

        // According to RFC 8010 3.1.1, there are 0 or more groups of
        // attributes, so having no more input here is not a failure case
        // even though all the IPP test suites insist that a request
        // has a charset and natural language operation attributes
        //
        req->cur_attr = NULL;
        req->col_top = -1;

        result = ipp_read_int8(&req->in, &tag);
        if (result < 0)
        {
            BERROR("read attr");
            break;
        }
        if (result > 0)
        {
            if (req->download_complete)
            {
                // no more body data, and none coming

                // there are no attributes, dispatch this. this code will NOT
                // be exercised by normal IPP testing tools as they always
                // insert operation attributes, so just go dispatch it
                //
                butil_log(5, "IPP Attributes blank, validate and dispatch\n");

                result = ipp_move_state(req, reqValidation);
                break;
            }
            else
            {
                // still downloading request, wait for more data
                //
                result = ipp_wait_for_bytes(req, 1);
                break;
            }
        }
        switch (tag)
        {
        case IPP_TAG_OPERATION:
            req->cur_in_group = IPP_OPER_ATTRS;
            result = ipp_push_state(req, reqOperationAttributes);
            break;

        default:
            ipp_set_error(req, IPP_STATUS_ERROR_BAD_REQUEST);
            result = ipp_move_state(req, reqReply);
            break;
        }
        break;

    case reqOperationAttributes:
    case reqJobAttributes:
    case reqPrinterAttributes:

        // start off attribute parsing
        result = ipp_push_state(req, reqAttributeTag);
        break;

    case reqAttributeTag:

        // fetch an attribute, or end-tag (which pops state)
        result = ipp_parse_attribute(req);
        if (! result)
        {
            // if got an end tag or other, that would have popped the state
            // back to the group state, so pop it again to get to
            // Attribute state, then move
            // now, in which case, pop once more to get out of group
            //
            if (req->state[req->top] < reqAttributeTag)
            {
                butil_log(IPPLAP, "End of Group, %s\n", ipp_state_string(req->state[req->top]));
                result = ipp_pop_state(req);
            }
        }
        break;

    case reqAttributeNameLength:

        result = ipp_parse_name_length(req);
        break;

    case reqAttributeNameText:

        result = ipp_parse_name_text(req);
        break;

    case reqAttributeValueLength:

        result = ipp_parse_value_length(req);
        break;

    case reqAttributeValue:

        result = ipp_parse_value(req);
        break;

    case reqValidation: // got all the attributes we're getting, so validate

        if (req->last_error != IPP_STATUS_OK)
        {
            butil_log(IPPLAP, "Skipping validation due to pre-existing condition\n");
            result = ipp_move_state(req, reqReply);
            break;
        }
        ipp_set_error(req, IPP_STATUS_OK);

        do // try
        {
            ipp_attr_t *attr;
            size_t recdex;

            // validate the request. mostly this is to pass conformance testing
            // since the protocol itself is pretty simple and would just work
            // ok without all this insistance on ordering/presence
            //
            // RFC 8011 section 4.1.1 - no 0 request id
            if (req->reqid == 0)
            {
                butil_log(2, "Fail: Request ID is 0\n");
                ipp_set_error(req, IPP_STATUS_ERROR_BAD_REQUEST);
                result = -1;
                break;
            }
            // RFC 8011 section 4.1.4 - charset+natural language are first two and present
            result = -1;
            do  // try
            {
                attr = req->in_attrs[IPP_OPER_ATTRS];

                if (! attr)
                {
                    butil_log(1, "Fail: No operation attributes\n");
                    break;
                }
                if (ipp_find_attr_rec("attributes-charset", &recdex, NULL))
                {
                    butil_log(1, "Fail: internal: attribute table missing charset\n");
                    break;
                }
                if (attr->recdex != recdex)
                {
                    butil_log(1, "Fail: First operation attribute not charset, got %s\n",
                                    ipp_name_of_attr(attr));
                    break;
                }
                if (! attr->next)
                {
                    butil_log(1, "Fail: No natural-language attribute\n");
                    break;
                }
                attr = attr->next;

                if (ipp_find_attr_rec("attributes-natural-language", &recdex, NULL))
                {
                    butil_log(1, "Fail: internal: attribute table missing natlang\n");
                    break;
                }
                if (attr->recdex != recdex)
                {
                    butil_log(1, "Fail: Second operation attribute not natural-language, got %s\n",
                                    ipp_name_of_attr(attr));
                    break;
                }
                result = 0;
            }
            while (0); // catch

            if (result)
            {
                ipp_set_error(req, IPP_STATUS_ERROR_BAD_REQUEST);
                break;
            }
            // RFC 8011 section 4.1.8 - Bad Version
            if (
                    (req->vmaj < IPP_MAJOR_VERSION_MIN)
                |   (req->vmaj > IPP_MAJOR_VERSION_MAX)
                |   (req->vmaj == IPP_MAJOR_VERSION_MAX && req->vmin > IPP_MINOR_VERSION_MAX)
                |   (req->vmaj == IPP_MAJOR_VERSION_MIN && req->vmin < IPP_MINOR_VERSION_MIN)
            )
            {
                ipp_set_error(req, IPP_STATUS_ERROR_VERSION_NOT_SUPPORTED);
                result = -1;
                break;
            }

        }
        while (0); // catch

        if (result)
        {
            break;
        }
        // validated OK, go dispatch
        //
        butil_log(IPPLAP, "Pass: Valid Request ID %u\n", req->reqid);

        result = ipp_move_state(req, reqDispatch);
        break;

    case reqDispatch:

        // dispatch the operation -----------------------------------
        //
        result = ipp_dispatch(req);
        if (! result)
        {
            // if there is print data for this request, wait
            // till download complete before replying
            //
            if (req->download_complete)
            {
                result = ipp_move_state(req, reqReply);
            }
            else
            {
                result = ipp_move_state(req, reqPrintData);
            }
            break;
        }
        break;

    case reqPrintData:

        if (req->download_complete)
        {
            butil_log(5, "End of print data\n");
            if (req->job)
            {
                result = ipp_sink_job_data(req);
            }
            result = ipp_move_state(req, reqReply);
            break;
        }
        if (req->job)
        {
            result = ipp_sink_job_data(req);
        }
        else
        {
            butil_log(IPPLAP, "Absorb %d bytes print data\n", req->in.count);
            req->in.tail += req->in.count;
            req->in.count = 0;
        }
        break;

    case reqReply:

        if (req->download_complete)
        {
            butil_log(5, "End of request data, reply now\n");
            result = ipp_reply(req);
        }
        else
        {
            butil_log(IPPLAP, "Flush %d bytes request data, reply ready\n", req->in.count);
            req->in.count = 0;
        }
        break;

    case reqReplyOneAttribute:

        if (! req->cur_out_attr)
        {
            result = ipp_pop_state(req);
        }
        result = ipp_send_attribute(req);
        break;

    case reqReplyAttributeValue:

        if (! req->attr_out_value || ! req->attr_value_len)
        {
            result = ipp_pop_state(req);
        }
        result = ipp_send_attribute_value(req);
        break;

    case reqReplyOperationAttributes:

        if (req->cur_out_attr)
        {
            ipp_push_state(req, reqReplyOneAttribute);
        }
        else
        {
            req->cur_out_group = IPP_JOB_ATTRS;
            req->cur_out_attr = req->out_attrs[IPP_JOB_ATTRS];
            if (req->cur_out_attr)
            {
                // there are job attrs in response, send them
                result = ipp_write_int8(&req->out, IPP_TAG_JOB);
            }
            // but go to jobattr state in all cases, to move thing along
            result = ipp_move_state(req, reqReplyJobAttributes);
        }
        break;

    case reqReplyJobAttributes:

        if (req->cur_out_attr)
        {
            ipp_push_state(req, reqReplyOneAttribute);
        }
        else
        {
            // end job group, amove to printer group
            req->cur_out_group = IPP_PRT_ATTRS;
            req->cur_out_attr = req->out_attrs[IPP_PRT_ATTRS];
            if (req->cur_out_attr)
            {
                // there are prt attrs in response, send them
                result = ipp_write_int8(&req->out, IPP_TAG_PRINTER);
            }
            // but go to
            result = ipp_move_state(req, reqReplyPrinterAttributes);
        }
        break;

    case reqReplyPrinterAttributes:

        if (req->cur_out_attr)
        {
            ipp_push_state(req, reqReplyOneAttribute);
        }
        else
        {
            // end printer group, and move to done
            result = ipp_write_int8(&req->out, IPP_TAG_END);
            result = ipp_move_state(req, reqDone);
        }
        break;

    case reqDone:

        /*
        if (req->job)
        {
            ipp_complete_job(req->ipp, req->job);
            req->job = NULL;
        }
        */
        result = 0;
        break;

    case reqReadInput:

        if (req->in.count >= req->bytes_needed)
        {
            butil_log(0, "have %d of %d needed\n", req->in.count, req->bytes_needed);
            result = ipp_pop_state(req);
            if (result)
            {
                break;
            }
        }
        break;

    case reqWriteOutput:

        if (client->out.count == 0 && req->out.count == 0)
        {
            // nothing queued to send, go back to whatever we were doing
            //
            result = ipp_pop_state(req);
            break;
        }
        // flush any buffered output to make room, buffer will already have
        // chunk counts etc properly in it already
        //
        result = http_send_out_data(client, httpBodyUpload, httpBodyUpload);
        break;

    default:
        break;
    }
    if (result)
    {
        // http server calling us doesn't care about the result code really, it just
        // cancels the request handing. if we find errors we will report them back
        // in an IPP reply, so just go to that state and make sure last_error is set
        //
        req->top = 0; // unwind state
        if (req->state[req->top] < reqReply)
        {
            if (req->last_error == IPP_STATUS_OK)
            {
                ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
            }
            result = ipp_move_state(req, reqReply);
        }
        else
        {
            result = ipp_move_state(req, reqDone);
        }
        result = 0;
    }
    return result;
}

