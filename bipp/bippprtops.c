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
#include "bippjobops.h"
#include "bipp.h"
#include "bippproto.h"
#include "butil.h"

static int ipp_op_get_printer_attributes(ipp_request_t *req)
{
    ipp_attr_iter_t *reqiterator;
    ipp_attr_t *reqattrs;
    ipp_attr_t *attr;
    ipp_attr_t *nattr;
    bool retall;
    int result;

    // see if there are requested return attributes list
    //
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "requested-attributes", &reqattrs);
    if (result)
    {
        butil_log(5, "No requested attributes, returning all\n");
        reqattrs = NULL;
        result = 0;
    }
    retall = false;

    if (reqattrs)
    {
        char reqname[IPP_MAX_TEXT];
        uint8_t *value;
        size_t   value_len;

        // iterate over requested attributes and return, if we can,
        // any that are set in printer description or status
        //
        result = ipp_open_attr_value(reqattrs, &reqiterator);
        if (result)
        {
            ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
            return result;
        }
        do
        {
            result = ipp_get_next_attr_string_value(
                                            reqattrs,
                                            reqiterator,
                                            reqname,
                                            sizeof(reqname)
                                            );
            if (result > 0)
            {
                // end of values
                result = 0;
                break;
            }
            if (result < 0)
            {
                // error getting them
                break;
            }
            // escape if "all" is in there
            //
            if (! strcmp(reqname, "all"))
            {
                retall = true;
                break;
            }
            // look up the requested name in the usual places
            //
            result = ipp_get_group_attr_by_name(reqname, IPP_GROUPING_PRINTER_DESCRIPTION, &attr);
            if (result)
            {
                result = ipp_get_group_attr_by_name(reqname, IPP_GROUPING_PRINTER_STATUS, &attr);
            }
            if (result)
            {
                butil_log(1, "Can't find %s in printer status/description\n", reqname);
                ipp_set_error(req, IPP_STATUS_ERROR_ATTRIBUTES_OR_VALUES);
                return result;
            }
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
        }
        while (! result);

        result = ipp_close_attr_value(reqiterator);
        if (result)
        {
            ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
            return result;
        }
        if (! retall)
        {
            return 0;
        }
    }
    // iterate over printer description group adding each set attribute
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
    return result;
}

static int ipp_op_print_job(ipp_request_t *req)
{
    ipp_job_t *job;
    ipp_attr_t *attr;
    ipp_attr_t *nattr;
    char mimestr[IPP_MAX_TEXT];
    char *jobstatereasons;
    int result;

    if (! req || ! req->ipp)
    {
        return -1;
    }
    result = ipp_op_create_job(req, &job);
    if (result)
    {
        return result;
    }
    result = ipp_complete_job(req->ipp, job);
    if (result)
    {
        butil_log(1, "Can't complete job %u\n", job->id);
        result = ipp_destroy_job(req->ipp, job);
        return -1;
    }
    return result;
}

int ipp_op_send_document(ipp_request_t *req)
{
    ipp_attr_t *attr;
    ipp_job_t  *job;
    int32_t     jobid;
    int         islast;
    int         result;

    if (! req || ! req->ipp)
    {
        return -1;
    }
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "job-id", &attr);
    if (result)
    {
        butil_log(5, "No job-id in oper attributes\n");
        ipp_set_error(req, IPP_STATUS_ERROR_BAD_REQUEST);
        return result;
    }
    result = ipp_get_only_attr_int32_value(attr, &jobid);
    if (result)
    {
        return result;
    }
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "last-document", &attr);
    if (result)
    {
        butil_log(5, "No last-document in oper attributes\n");
        ipp_set_error(req, IPP_STATUS_ERROR_BAD_REQUEST);
        return result;
    }
    result = ipp_get_only_attr_bool_value(attr, &islast);
    if (result)
    {
        return result;
    }
    // look for job id in listings
    //
    result = ipp_get_job_by_id(req->ipp, jobid, &job);
    if (result)
    {
        butil_log(5, "No job id %d found\n", jobid);
        job = NULL;
        result = 0;
    }
    // if cancelled, or completed, can't do it again
    //
    if (
            ! job
        ||  job->state == IPP_JSTATE_COMPLETED
        ||  job->state == IPP_JSTATE_CANCELED
        ||  job->state == IPP_JSTATE_ABORTED
    )
    {
        ipp_set_error(req, IPP_STATUS_ERROR_NOT_POSSIBLE);
        return result;
    }
    // add jobs current status attributes list to reply
    //
    result = ipp_dupe_attr_list(job->job_stat_attr, &req->out_attrs[IPP_JOB_ATTRS]);
    if (result)
    {
        return result;
    }
    return 0;
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
        return ipp_op_get_printer_attributes(req);

    case IPP_OP_PRINT_JOB:
        return ipp_op_print_job(req);

    case IPP_OP_SEND_DOCUMENT:
        return ipp_op_send_document(req);

    case IPP_OP_PRINT_URI:
    case IPP_OP_SEND_URI:
    default:
        ipp_set_error(req, IPP_STATUS_ERROR_OPERATION_NOT_SUPPORTED);
        result = -1;
    }
    return result;
}

