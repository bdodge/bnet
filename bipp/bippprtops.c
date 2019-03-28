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
#include "bippprtops.h"
#include "bipp.h"
#include "bippproto.h"
#include "butil.h"

static int ipp_get_printer_attributes(ipp_request_t *req)
{
    ipp_attr_t *attr;
    ipp_attr_t *nattr;
    int result;

#if 1
    // iterate over printer dezcription group adding each set attribute
    // into the response
    //
    result = ipp_get_attr_for_grouping(IPP_GROUPING_PRINTER_DESCRIPTION, &attr);
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
    // iterate over printer status group adding each set attribute
    // into the response
    //
    result = ipp_get_attr_for_grouping(IPP_GROUPING_PRINTER_STATUS, &attr);
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
#else
    // fetch printer-attributes group and add to return
    result = ipp_get_attr_by_name("printer-uri-supported", IPP_GROUPING_PRINTER_STATUS, &attr);
    if (result)
    {
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return result;
    }
    result = ipp_dupe_attr(attr, &nattr);
    if (result)
    {
        ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
        return result;
    }
    result = ipp_add_req_out_attribute(req, IPP_PRT_ATTRS, nattr);
#endif
    return result;
}

static int ipp_print_job(ipp_request_t *req)
{
    ipp_attr_t *attr;
    ipp_attr_t *nattr;
    char uri[IPP_MAX_TEXT];
    char mimestr[IPP_MAX_TEXT];
    int32_t jobid = 0x1234beef;
    int32_t jobstate;
    char *jobstatereasons;
    int result;

    // get job document format
    //
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "document-format", &attr);
    if (result)
    {
        butil_log(5, "No document-format in job attributes\n");
        ipp_set_error(req, IPP_STATUS_ERROR_DOCUMENT_FORMAT_NOT_SUPPORTED);
        return result;
    }
    // check mime type against supported values
    //
    butil_log(5, "document-format is %s\n", (char*)attr->value);

    //ipp_set_error(req, IPP_STATUS_ERROR_DOCUMENT_FORMAT_NOT_SUPPORTED);

    // put job uri into response
    //
    snprintf(uri, sizeof(uri), "%s/job/%d", req->ipp->uri, jobid);

    result = ipp_set_req_out_string_attr(req, IPP_JOB_ATTRS, "job-uri", uri);
    if (result)
    {
        return result;
    }
    // put job id into response
    //
    result = ipp_set_req_out_int32_attr(req, IPP_JOB_ATTRS, "job-id", jobid);

    // put job-state into response
    //
    jobstate = IPP_JSTATE_COMPLETED;
    result = ipp_set_req_out_int32_attr(req, IPP_JOB_ATTRS, "job-state", jobstate);

    // put job-state-reasons into response
    //
    jobstatereasons = "job-completed-successfully";
    result = ipp_set_req_out_string_attr(req, IPP_JOB_ATTRS, "job-state-reasons", jobstatereasons);
    if (result)
    {
        return result;
    }
    return result;
}

int ipp_printer_op_dispatch(ipp_request_t *req)
{
    ipp_attr_t *attr;
    int result;

    if (! req)
    {
        return -1;
    }
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "printer-uri", &attr);
    if (result)
    {
        ipp_set_error(req, IPP_STATUS_ERROR_BAD_REQUEST);
        return result;
    }
    switch (req->opid)
    {
    case IPP_OP_GET_PRINTER_ATTRIBUTES:
        return ipp_get_printer_attributes(req);
        break;

    case IPP_OP_PRINT_JOB:
        return ipp_print_job(req);
        break;

    case IPP_OP_PRINT_URI:
    case IPP_OP_VALIDATE_JOB:
    case IPP_OP_CREATE_JOB:
    case IPP_OP_SEND_DOCUMENT:
    case IPP_OP_SEND_URI:
    default:
        ipp_set_error(req, IPP_STATUS_ERROR_OPERATION_NOT_SUPPORTED);
        result = -1;
    }
    return result;
}

