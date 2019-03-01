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
#include "bsnmp.h"
#include "bsnmpber.h"
#include "bsnmputils.h"
#include "bsnmpvar.h"
#include "bsnmpio.h"
#include "butil.h"

static int bsnmp_format_varbind_reply(bsnmp_server_t *server, bsnmp_var_t *var)
{
    ioring_t varbind_dex;
    int result;

    result = bsnmp_ber_from_typecode(server, asnSTRUCT);
    if (result)
    {
        return result;
    }
    // remember position of struct len (total len of varbind)
    bsnmp_stream_save_out_state(server, &varbind_dex);

    // reserve 5 bytes for length, will back annotate when it is known
    result = bsnmp_ber_reserve_length(server, 5);
    if (result)
    {
        return result;
    }
    result = bsnmp_ber_from_oid(server, &var->oid);
    if (result)
    {
        return result;
    }
    // get value of var from ber
    switch (var->type)
    {
    case SNMP_NULL:
        break;
    case SNMP_INTEGER:
        result = bsnmp_ber_from_int32(server, var->type, var->val.iVal);
        break;
    case SNMP_BOOLEAN:
    case SNMP_UNSIGNED:
    case SNMP_UNSIGNED32:
    case SNMP_IPADDRESS:
    case SNMP_COUNTER:
    case SNMP_TIMETICKS:
        result = bsnmp_ber_from_uint32(server, var->type, var->val.uVal);
        break;
    case SNMP_COUNTER64:
    case SNMP_UNSIGNED64:
        result = bsnmp_ber_from_uint64(server, var->type, var->val.ullVal);
        break;
    case SNMP_INTEGER64:
        result = bsnmp_ber_from_uint64(server, var->type, var->val.llVal);
        break;
    case SNMP_OCTET_STRING:
        result = bsnmp_ber_from_string(server, var->val.sVal, var->len);
        break;
    case SNMP_OBJECT_ID:
        result = bsnmp_ber_from_oid(server, var->val.oVal);
        break;
    case SNMP_PRINTABLE_STR:
    case SNMP_UTC_TIME:
    case SNMP_OPAQUE:
    case SNMP_NSAPADDRESS:
    case SNMP_SEQUENCE:
    case SNMP_SETOF:
    case SNMP_BIT_STRING:
    case SNMP_FLOAT:
    case SNMP_DOUBLE:
    case SNMP_FLOAT32:
    case SNMP_FLOAT64:
    case SNMP_FLOAT128:
        butil_log(0, "VB: type not supported: %u\n", var->type);
        return -1;

    default:
        butil_log(0, "VB: not a type: %u\n", var->type);
        return -1;
    }
    if (result)
    {
        return result;
    }
    // back annotate length at varbind_dex
    //
    result = bsnmp_stream_annotate_length(server, &varbind_dex, 5);
    return result;
}

static int bsnmp_format_reply(bsnmp_server_t *server, bsnmp_request_t *req)
{
    bsnmp_var_t *var;

    ioring_t msglen_pos;
    ioring_t pdulen_pos;
    ioring_t varlist_dex;
    ioring_t msgdata_dex;

    int result;

    // reset output stream buffer
    //
    iostream_reset_ring(&server->out);

    // reply looks just like request, a struct containing pdu
    //
    result = bsnmp_ber_from_typecode(server, asnSTRUCT);
    if (result)
    {
        return result;
    }
    // remember io position of struct len (total len of reply)
    bsnmp_stream_save_out_state(server, &msglen_pos);

    // reserve 5 bytes for length, will back annotate when it is known
    // and move the rest of the response left if shorter than 5 bytes
    result = bsnmp_ber_reserve_length(server, 5);
    if (result)
    {
        return result;
    }
    result = bsnmp_ber_from_int32(server, SNMP_INTEGER, req->version);
    if (result)
    {
        return result;
    }
    // for version 3, get the message global data
    //
    if (req->version == 3)
    {
        butil_log(0, "not supported yet");
        return -1;
    }
    else
    {
        result = bsnmp_ber_from_string(server, (const char*)req->community, strlen(req->community));
        if (result)
        {
            return result;
        }
    }
    result = bsnmp_ber_from_typecode(server, (bsnmp_type_t)SNMP_RESPONSE);
    if (result)
    {
        return result;
    }
    // save location of PDU to back-annotate length
    bsnmp_stream_save_out_state(server, &pdulen_pos);

    // reserve 5 bytes for length
    result = bsnmp_ber_reserve_length(server, 5);
    if (result)
    {
        return result;
    }
    result = bsnmp_ber_from_int32(server, SNMP_INTEGER, req->id);
    if (result)
    {
        return result;
    }
    result = bsnmp_ber_from_int32(server, SNMP_INTEGER, req->errmsg);
    if (result)
    {
        return result;
    }
    result = bsnmp_ber_from_int32(server, SNMP_INTEGER, req->errdex);
    if (result)
    {
        return result;
    }
    result = bsnmp_ber_from_typecode(server, asnSTRUCT);
    if (result)
    {
        return result;
    }
    // remember position of struct len (total len of varbinds)
    bsnmp_stream_save_out_state(server, &varlist_dex);

    // reserve 5 bytes for length, will back annotate when it is known
    result = bsnmp_ber_reserve_length(server, 5);
    if (result)
    {
        return result;
    }
    for (var = req->values; var; var = var->next)
    {
        result = bsnmp_format_varbind_reply(server, var);
        if (result)
        {
            return result;
        }
    }
    // back annotate varbind list len
    result = bsnmp_stream_annotate_length(server, &varlist_dex, 5);
    if (result)
    {
        return result;
    }
    // back annotate PDU len
    result = bsnmp_stream_annotate_length(server, &pdulen_pos, 5);
    if (result)
    {
        return result;
    }
    // back annotate message len

    // back annotate total length
    result = bsnmp_stream_annotate_length(server, &msglen_pos, 5);
    if (result)
    {
        return result;
    }
    return 0;
}

static int bsnmp_process_varbind(bsnmp_server_t *server, bsnmp_request_t *req, bsnmp_oid_t *oid)
{
    bsnmp_var_t *var;
    bsnmp_type_t type;
    bval_t       val;
    ioring_t svin;
    int result;

    var = bsnmp_var_create();
    if (! var)
    {
        return -1;
    }
    bsnmp_var_init(var, oid, SNMP_NULL);

    // peak at var type code in stream to decode it properly by saving stream state
    //
    bsnmp_stream_save_in_state(server, &svin);
    result = bsnmp_typecode_from_ber(server, &var->type);
    bsnmp_stream_restore_in_state(server, &svin);
    if (result)
    {
        bsnmp_var_destroy(var);
        return result;
    }
    // get value of var from ber
    switch (var->type)
    {
    case SNMP_NULL:
        result = bsnmp_null_from_ber(server, &var->len);
        break;
    case SNMP_INTEGER:
        result = bsnmp_int32_from_ber(server, &var->val.iVal);
        break;
    case SNMP_BOOLEAN:
    case SNMP_UNSIGNED:
    case SNMP_UNSIGNED32:
    case SNMP_IPADDRESS:
    case SNMP_COUNTER:
    case SNMP_TIMETICKS:
        result = bsnmp_uint32_from_ber(server, &var->val.uVal);
        break;
    case SNMP_COUNTER64:
    case SNMP_UNSIGNED64:
        result = bsnmp_uint64_from_ber(server, &var->val.ullVal);
        break;
    case SNMP_INTEGER64:
        result = bsnmp_uint64_from_ber(server, &var->val.llVal);
        break;
    case SNMP_OCTET_STRING:
        bsnmp_stream_save_in_state(server, &svin);
        result = bsnmp_typelen_from_ber(server, &var->type, &var->len);
        bsnmp_stream_restore_in_state(server, &svin);
        if (result)
        {
            break;
        }
        // alloc len needed
        var->alloc_len = var->len + 2;
        var->val.sVal = malloc(var->alloc_len);
        if (! var->val.sVal)
        {
            butil_log(0, "VB: Can't alloc\n");
            break;
        }
        result = bsnmp_string_from_ber(server, var->val.sVal, var->alloc_len, &var->len);
        if (result)
        {
            free(var->val.sVal);
            var->val.sVal = NULL;
            break;
        }
        break;
    case SNMP_OBJECT_ID:
        bsnmp_stream_save_in_state(server, &svin);
        result = bsnmp_typelen_from_ber(server, &var->type, &var->len);
        bsnmp_stream_restore_in_state(server, &svin);
        if (result)
        {
            break;
        }
        // alloc len needed
        var->alloc_len = var->len + 2;
        var->val.oVal = (bsnmp_oid_t *)malloc(sizeof(bsnmp_oid_t *));
        if (! var->val.oVal)
        {
            butil_log(0, "VB: Can't alloc\n");
            break;
        }
        result = bsnmp_oid_from_ber(server, var->val.oVal);
        if (result)
        {
            free(var->val.oVal);
            var->val.oVal = NULL;
            break;
        }
        break;
    case SNMP_PRINTABLE_STR:
    case SNMP_UTC_TIME:
    case SNMP_OPAQUE:
    case SNMP_NSAPADDRESS:
    case SNMP_SEQUENCE:
    case SNMP_SETOF:
    case SNMP_BIT_STRING:
    case SNMP_FLOAT:
    case SNMP_DOUBLE:
    case SNMP_FLOAT32:
    case SNMP_FLOAT64:
    case SNMP_FLOAT128:
        butil_log(0, "VB: type not supported: %u\n", var->type);
        result = -1;
        break;

    default:
        butil_log(0, "VB: not a type: %u\n", var->type);
        result = -1;
        break;
    }
    if (result)
    {
        bsnmp_var_destroy(var);
        return result;
    }
    // callback to process var
    //
    if (server->callback)
    {
        result = server->callback(server, req, var);
        if (result)
        {
            butil_log(1, "VB: Callback cancels\n");
            bsnmp_var_destroy(var);
            return result;
        }
    }
    // success, append var to results list
    req->values = bsnmp_varlist_appendvar(req->values, var);
    return 0;
}

static int bsnmp_process_req_varbinds(bsnmp_server_t *server, bsnmp_request_t *req)
{
    bsnmp_var_t var;
    bsnmp_oid_t oid;
    uint32_t varbinds_len;
    uint32_t vbblen;
    uint32_t a_varbind_len;
    int result;

    result = bsnmp_struct_from_ber(server, &varbinds_len);
    if (result)
    {
        return result;
    }
    while (varbinds_len > 0)
    {
        result = bsnmp_struct_from_ber(server, &a_varbind_len);
        if (result)
        {
            return result;
        }
        // take out this varbind struct len from list len
        varbinds_len -= a_varbind_len;

        // take out struct tag byte from list len
        varbinds_len -= 1;

        // take out bytes used to encode struct len from varbinds len
        if (a_varbind_len < 128)
        {
            varbinds_len -= 1;
        }
        else
        {
            uint32_t bytes = a_varbind_len;

            for (bytes = a_varbind_len; bytes; bytes >>= 8)
            {
                varbinds_len--;
            }
        }
        if (a_varbind_len == 0)
        {
            butil_log(0, "VB: No vblen\n");
            return -1;
        }
        result = bsnmp_oid_from_ber(server, &oid);
        if (result)
        {
            return result;
        }
        result = bsnmp_process_varbind(server, req, &oid);
        if (result)
        {
            return result;
        }
    }
    return result;
}

static int bsnmp_process_req_header(bsnmp_server_t *server, bsnmp_request_t *req)
{
    int result;
    uint8_t b;
    uint32_t berlen;
    uint32_t uval;
    int32_t  ival;
    uint32_t stringlen;

    // Expect a struct(len) { pdu }
    //
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        butil_log(0, "BER: No Data\n");
        return result;
    }
    if (b != asnSTRUCT)
    {
        butil_log(0, "BER: Not a Struct\n");
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    result = bsnmp_length_from_ber(server, &berlen);
    if (result)
    {
        butil_log(0, "BER: No Len\n");
        return result;
    }
    // Check that the data in store matches length
    if (berlen != server->in.count)
    {
        butil_log(0, "BER: Size wrong");
        server->last_error = SNMP_ErrWrongLength;
        return -1;
    }
    result = bsnmp_int32_from_ber(server, &req->version);
    if (result)
    {
        butil_log(0, "BER: No Version\n");
        return result;
    }
    butil_log(5, "SNMP Msg Len=%ub version=%d\n", berlen, req->version);
    switch (req->version)
    {
    case SNMP_V1:
        break;
    case SNMP_V2C:
        result = bsnmp_string_from_ber(server, req->community, sizeof req->community, &stringlen);
        if (result)
        {
            butil_log(0, "BER: No Community");
            return result;
        }
        break;
    case SNMP_V3:
        butil_log(0, "not supported yet");
        return -1;
    default:
        butil_log(0, "Unsupported version %d\n", req->version);
        server->last_error = SNMP_ErrGenErr;
        return -1;
    }
    butil_log(5, "  - ver:%s community:%s\n", bsnmp_version_string(req->version), req->community);
    return 0;    
}

static int bsnmp_process_req_pdu(bsnmp_server_t *server, bsnmp_request_t *req)
{
    int result;

    // PDU request type
    //
    result = bsnmp_request_from_ber(server, &req->code, &req->len);
    if (result)
    {
        butil_log(0, "BER: No request\n");
        return result;
    }
    switch(req->code)
    {
    case SNMP_GET:
    case SNMP_GETNEXT:
    case SNMP_RESPONSE:
    case SNMP_SET:
    case SNMP_TRAP:
        break;
    case SNMP_GETBULK:
    case SNMP_INFORM:
    case SNMP_NOTIFY:
        if (server->version == SNMP_V1)
        {
            server->last_error = SNMP_ErrGenErr;
            return -1;
        }
        break;
    case SNMP_REPORT:
        if (server->version != SNMP_V3)
        {
            server->last_error = SNMP_ErrGenErr;
            return -1;
        }
        break;
    default:
        butil_log(0, "BER: invalid req\n");
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    if (req->len == 0)
    {
        // legal?
        return 0;
    }
    // reinsure the length is proper
    //
    if (req->len != server->in.count)
    {
        butil_log(0, "PDU: Req Len Bad %u needed vs %u left\n", req->len, server->in.count);
        server->last_error = SNMP_ErrWrongLength;
        return -1;
    }
    // Request ID
    //
    result = bsnmp_int32_from_ber(server, &req->id);
    if (result)
    {
        butil_log(0, "PDU: No Id\n");
        return result;
    }
    if (req->code != SNMP_GETBULK)
    {
        int32_t ival;
        
        result = bsnmp_int32_from_ber(server, &ival);
        if (result)
        {
            butil_log(0, "PDU: No err msg\n");
            return result;
        }
        req->errmsg = ival;

        result = bsnmp_int32_from_ber(server, &ival);
        if (result)
        {
            butil_log(0, "PDU: No err index\n");
            return result;
        }
        req->errdex = ival;
    }
    else
    {
        result = bsnmp_int32_from_ber(server, &req->non_repeaters);
        if (result)
        {
            butil_log(0, "PDU: No non repeaters\n");
            return result;
        }
        result = bsnmp_int32_from_ber(server, &req->max_repeating);
        if (result)
        {
            butil_log(0, "PDU: No max repeating\n");
            return result;
        }
    }
    return 0;
}

static int bsnmp_process_request(bsnmp_server_t *server, bsnmp_request_t *req)
{
    int result;
    uint32_t uval;

    result = 0;

    req->values = NULL;
    req->errmsg = SNMP_ErrNoError;
    req->errdex = 0;
    server->last_error = SNMP_OK;

    if (server->in.count == 0)
    {
        return 0;
    }
    result = bsnmp_process_req_header(server, req);
    if (result)
    {
        return result;
    }
    result = bsnmp_process_req_pdu(server, req);
    if (result)
    {
        return result;
    }    
    result = bsnmp_process_req_varbinds(server, req);
    if (result)
    {
        return result;
    }    
    return result;
}

int bsnmp_serve(bsnmp_server_t *server)
{
    int result;

    if (! server || ! server->stream)
    {
        return 1;
    }
    do
    {
        if (server->out.count)
        {
            // flush pending output
            //
            result = bsnmp_stream_write(server, 1, 0);
        }
        else
        {
            // poll for next request, setting time
            // to wait based on timers
            //
            result = bsnmp_stream_read(server, 1, 0);
            if (! result)
            {
                bsnmp_request_t req;

                // initialize the request context
                memset(&req, 0, sizeof(req));

                // parse the request
                result = bsnmp_process_request(server, &req);

                // consume remaining input regardless
                server->in.count = 0;
                server->in.head = 0;
                server->in.tail = 0;

                // if request has a reply, format that
                switch (req.code)
                {
                case SNMP_GET:
                case SNMP_SET:
                case SNMP_GETNEXT:
                case SNMP_GETBULK:
                    if (result)
                    {
                        // something failed in processing request or reply
                        // so make sure the responce has an errmsg and
                        // no varbinds. if errmsg set, use it, if not
                        // use last_error in the context, else set generr
                        //
                        if (req.errmsg == SNMP_ErrNoError)
                        {
                            if (server->last_error != SNMP_ErrNoError)
                            {
                                req.errmsg = server->last_error;
                            }
                            if (req.errmsg == SNMP_ErrNoError)
                            {
                                req.errmsg = SNMP_ErrGenErr;
                            }
                        }
                        // remove any vars in response
                        bsnmp_varlist_cleanup(req.values);
                        req.values = NULL;
                    }
                    result = bsnmp_format_reply(server, &req);
                    break;
                default:
                    break;
                }
                // remove any vars in response
                bsnmp_varlist_cleanup(req.values);
                req.values = NULL;
            }
        }
    }
    while (! result);

    return result;
}

int bsnmp_server_init(
                        bsnmp_server_t *server,
                        uint16_t version,
                        uint16_t port,
                        bsnmp_callback_t callback
                     )
{
    int result;

    server->callback = callback;
    result = bsnmp_stream_init(server, version, port);
    if (result)
    {
        return result;
    }
    return 0;
}

int bsnmp_server_cleanup(bsnmp_server_t *server)
{
    if (! server)
    {
        return 1;
    }
    if (server->stream)
    {
        server->stream->close(server->stream);
        server->stream = NULL;
    }
    if (server->in.data)
    {
        free(server->in.data);
        server->in.data = NULL;
    }
    if (server->out.data)
    {
        free(server->out.data);
        server->out.data = NULL;
    }
    return 0;
}

