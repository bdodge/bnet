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
#include "bippjobops.h"
#include "bipp.h"
#include "bippproto.h"
#include "butil.h"

static int ipp_get_job_attributes(ipp_request_t *req)
{
    ipp_attr_iter_t *reqiterator;
    ipp_attr_t *reqattrs;
    ipp_attr_t *attr;
    ipp_attr_t *nattr;
    int result;

    int32_t jobid = 0x1234beef;
    int32_t jobstate;
    char *jobstatereasons;

    // see if there are requested return attributes list
    //
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "requested-attributes", &reqattrs);
    if (result)
    {
        butil_log(5, "No requested attributes, returning all\n");
        reqattrs = NULL;
        result = 0;
    }
#if 0
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
            result = ipp_get_next_attr_value(
                                            reqattrs,
                                            reqiterator,
                                            &value,
                                            &value_len
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
            // format value into proper c string
            //
            if (value_len >= IPP_MAX_TEXT)
            {
                ipp_set_error(req, IPP_STATUS_ERROR_ATTRIBUTES_OR_VALUES);
                return -1;
            }
            memcpy(reqname, value, value_len);
            reqname[value_len] = '\0';

            // look up the requested name in the usual places
            //
            result = ipp_get_attr_by_name(reqname, IPP_GROUPING_PRINTER_DESCRIPTION, &attr);
            if (result)
            {
                result = ipp_get_attr_by_name(reqname, IPP_GROUPING_PRINTER_STATUS, &attr);
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
        }
        return result;
    }
#endif
#if 0
    // iterate over job description group adding each set attribute
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
    // iterate over job status group adding each set attribute
    // into the response
    //
    result = ipp_get_attr_for_grouping(IPP_GROUPING_JOB_STATUS, &attr);
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
            result = ipp_add_req_out_attribute(req, IPP_JOB_ATTRS, nattr);
            if (result)
            {
                return result;
            }
        }
        attr = attr->next;
    }
#endif
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

int ipp_job_op_dispatch(ipp_request_t *req)
{
    ipp_attr_t *attr;
    int result;

    if (! req)
    {
        return -1;
    }
    switch (req->opid)
    {
    case IPP_OP_VALIDATE_JOB:
        return 0;

    case IPP_OP_GET_JOBS:
        return 0;

    case IPP_OP_GET_JOB_ATTRIBUTES:
        return ipp_get_job_attributes(req);

    case IPP_OP_CREATE_JOB:
    case IPP_OP_SEND_DOCUMENT:
    case IPP_OP_SEND_URI:
    case IPP_OP_CANCEL_JOB:
    case IPP_OP_HOLD_JOB:
    case IPP_OP_RELEASE_JOB:
    case IPP_OP_RESTART_JOB:
    case IPP_OP_PURGE_JOBS:
    case IPP_OP_SET_JOB_ATTRIBUTES:
    case IPP_OP_CREATE_JOB_SUBSCRIPTIONS:
    case IPP_OP_HOLD_NEW_JOBS:
    case IPP_OP_RELEASE_HELD_NEW_JOBS:
    case IPP_OP_REPROCESS_JOB:
    case IPP_OP_CANCEL_CURRENT_JOB:
    case IPP_OP_SUSPEND_CURRENT_JOB:
    case IPP_OP_RESUME_JOB:
    case IPP_OP_PROMOTE_JOB:
    case IPP_OP_SCHEDULE_JOB_AFTER:
    case IPP_OP_CANCEL_JOBS:
    case IPP_OP_CANCEL_MY_JOBS:
    case IPP_OP_RESUBMIT_JOB:
    case IPP_OP_CLOSE_JOB:
    case IPP_OP_ACKNOWLEDGE_JOB:
    case IPP_OP_FETCH_JOB:
    case IPP_OP_UPDATE_ACTIVE_JOBS:
    case IPP_OP_UPDATE_JOB_STATUS:
    default:
        ipp_set_error(req, IPP_STATUS_ERROR_OPERATION_NOT_SUPPORTED);
        return 1;
    }
    return 0;
}


