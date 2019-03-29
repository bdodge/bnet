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

int ipp_op_validate_job(ipp_request_t *req)
{
    return 0;
}

int ipp_op_create_job(ipp_request_t *req, ipp_job_t **pjob)
{
    ipp_job_t *job;
    ipp_attr_t *attr;
    char mimestr[IPP_MAX_TEXT];
    char *jobstatereasons;
    int result;

    if (! req || ! req->ipp)
    {
        return -1;
    }
    if (pjob)
    {
        *pjob = NULL;
    }
    // get job document format
    //
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "document-format", &attr);
    if (result)
    {
        butil_log(5, "No document-format in job attributes\n");
        if (pjob)
        {
            // coming from print-job, need a doc type
            ipp_set_error(req, IPP_STATUS_ERROR_DOCUMENT_FORMAT_NOT_SUPPORTED);
            return result;
        }
        // wait for send-document to set doc type
        result = 0;
    }
    else
    {
        // check mime type against supported values
        //
        butil_log(5, "Create-Job: document-format is %s\n", ((char*)attr->value + 2));

        //ipp_set_error(req, IPP_STATUS_ERROR_DOCUMENT_FORMAT_NOT_SUPPORTED);
    }
    job = NULL;

    do  // try
    {
        result = ipp_create_job(req->ipp, req, &job);
        if (result || ! job)
        {
            return -1;
        }
        result = ipp_start_job(req->ipp, job);
        if (result)
        {
            butil_log(1, "Can't activate job %u\n", job->id);
            break;
        }
        // copy operation attributes list to job (easier than taking only whats needed
        //
        result = ipp_dupe_attr_list(req->in_attrs[IPP_OPER_ATTRS], &job->job_oper_attr);
        if (result)
        {
            break;
        }
        // add jobs current status attributes list to reply
        //
        result = ipp_dupe_attr_list(job->job_stat_attr, &req->out_attrs[IPP_JOB_ATTRS]);
        if (result)
        {
            break;
        }
        // set job name and other things culled from operations/req
        //
        result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "job-name", &attr);
        if (! result)
        {
            result = ipp_set_attr_attr_value("job-name", job->job_desc_attr, attr);
            if (result)
            {
                break;
            }
        }
        result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "requesting-user-name", &attr);
        if (! result)
        {
            result = ipp_set_attr_attr_value("job-originating-user-name", job->job_stat_attr, attr);
            if (result)
            {
                break;
            }
        }
        result = ipp_set_attr_string_value("job-printer-uri", job->job_stat_attr, 1, req->ipp->uri);
        if (result)
        {
            break;
        }
    }
    while (0);  // catch

    if (result)
    {
        if (job)
        {
            result = ipp_destroy_job(req->ipp, job);
        }
        return -1;
    }
    if (pjob)
    {
        // attach job to request for handing data to job
        //
        req->job = job;
        *pjob = job;
    }
    return 0;
}

static int ipp_req_job_attrs(ipp_request_t *req, ipp_attr_t *reqattrs, ipp_job_t *job)
{
    char uri[IPP_MAX_TEXT];
    int result;

    if (! req || ! job)
    {
        return -1;
    }
    if (! reqattrs)
    {
        // put job id into response
        //
        result = ipp_set_req_out_int32_attr(req, IPP_JOB_ATTRS, "job-id", job->id);
        if (result)
        {
            return result;
        }
        // put job uri into response
        //
        snprintf(uri, sizeof(uri), "%s/job/%d", req->ipp->uri, job->id);

        result = ipp_set_req_out_string_attr(req, IPP_JOB_ATTRS, "job-uri", uri);
        if (result)
        {
            return result;
        }
    }
    else
    {
        ipp_attr_t *attr;
        ipp_attr_t *nattr;
        char text[IPP_MAX_TEXT];
        ipp_attr_iter_t *iterator;
        bool retall = false;

        // for each reqattr, lookup attr and add to response
        //
        result = ipp_open_attr_value(reqattrs, &iterator);
        if (result)
        {
            return result;
        }
        do
        {
            result = ipp_get_next_attr_string_value(
                            reqattrs, iterator, text, sizeof(text));
            if (result > 0)
            {
                // end of values
                result = 0;
                break;
            }
            if (result)
            {
                break;
            }
            // if "all" is found, enumerate all
            //
            if (! strcmp(text, "all"))
            {
                retall = true;
                break;
            }
            // find the requested attribute
            //
            result = ipp_get_attr_by_name(text, job->job_stat_attr, &attr);
            if (result)
            {
                result = ipp_get_attr_by_name(text, job->job_desc_attr, &attr);
            }
            if (result)
            {
                butil_log(1, "Can't find =%s= in job status/description\n", text);
                // todo - add to unsupported?
                ipp_set_error(req, IPP_STATUS_ERROR_ATTRIBUTES_OR_VALUES);
                return result;
            }
            result = ipp_dupe_attr(attr, &nattr);
            if (result)
            {
                return result;
            }
            result = ipp_add_req_out_attribute(req, IPP_JOB_ATTRS, nattr);
            if (result)
            {
                return result;
            }
        }
        while (! result);

        ipp_close_attr_value(iterator);

        if (! retall || result)
        {
            return result;
        }
        // return all attributes in job description
        //
        attr = job->job_stat_attr;
        while (attr)
        {
            if (attr->value)
            {
                result = ipp_dupe_attr(attr, &nattr);
                if (result)
                {
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
        attr = job->job_desc_attr;
        while (attr)
        {
            if (attr->value)
            {
                result = ipp_dupe_attr(attr, &nattr);
                if (result)
                {
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
    }
    return result;
}

static int ipp_op_get_jobs(ipp_request_t *req)
{
    ipp_attr_t *reqattr;
    ipp_attr_t *attr;
    ipp_job_t  *job;
    size_t      njobs;
    enum { opgjAll, opgjUser, opgjActive, opgjCompleted } which_jobs;
    char text[IPP_MAX_TEXT];
    char user[IPP_MAX_TEXT];
    int32_t     limit;
    int         myjobs;
    int         result;

    which_jobs = opgjAll;
    njobs = 0;
    limit = 0;
    myjobs = false;
    user[0] = '\0';

    // check for which-jobs
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "which-jobs", &reqattr);
    if (! result)
    {
        result = ipp_get_only_attr_string_value(reqattr, text, sizeof(text));
        if (! strcmp(text, "completed"))
        {
            which_jobs = opgjCompleted;
        }
        else if (! strcmp(text, "not-completed"))
        {
            which_jobs = opgjActive;
        }
        else
        {
            butil_log(2, "Not sure what which-jobs=%s means\n", text);
        }
    }
    // check for user
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "requesting-user-name", &reqattr);
    if (! result)
    {
        result = ipp_get_only_attr_string_value(reqattr, user, sizeof(user));
        if (result)
        {
            return result;
        }
    }
    // check for limit
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "limit", &reqattr);
    if (! result)
    {
        result = ipp_get_only_attr_int32_value(reqattr, &limit);
        if (result)
        {
            return result;
        }
    }
    // check for my-jobs
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "my-jobs", &reqattr);
    if (! result)
    {
        result = ipp_get_only_attr_bool_value(reqattr, &myjobs);
        if (result)
        {
            return result;
        }
    }
    // finally, check for requested-attributes, and leave in reqattr
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "requested-attributes", &reqattr);
    if (result)
    {
        reqattr = NULL;
    }
    butil_log(5, "Get-Jobs limit=%d myjobs=%u user=%s reqattr=%p which=%u\n",
                    limit, myjobs, user, reqattr, which_jobs);

    // iterate over active
    //
    if (which_jobs == opgjAll || which_jobs == opgjActive)
    {
        result = ipp_get_active_jobs(req->ipp, &job);
        if (result)
        {
            job = NULL;
        }
        while (job)
        {
            // if reached limit, end
            //
            if (limit > 0 && njobs >= limit)
            {
                break;
            }
            // match job against requesting user if my-job set
            //
            if (myjobs && user[0])
            {
                result = ipp_get_attr_by_name("requesting-user-name", job->job_oper_attr, &attr);
                if (! result)
                {
                    result = ipp_get_only_attr_string_value(attr, text, sizeof(text));
                    if (! result)
                    {
                        if (strcmp(user, text))
                        {
                            butil_log(5, "exclude job for user %s, only want %s\n", text, user);
                            job = job->next;
                            continue;
                        }
                    }
                }
            }
            // if this isn't first job, add new start tag into response
            //
            if (njobs > 0)
            {
                result = ipp_set_req_out_tag(req, IPP_JOB_ATTRS, IPP_TAG_JOB);
                if (result)
                {
                    return result;
                }
            }
            result = ipp_req_job_attrs(req, reqattr, job);
            if (result)
            {
                ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
                return result;
            }
            njobs++;
            job = job->next;
        }
    }
    // iterate over completed
    if (which_jobs == opgjAll || which_jobs == opgjCompleted)
    {
        result = ipp_get_active_jobs(req->ipp, &job);
        if (result)
        {
            job = NULL;
        }
        while (job)
        {
            // if reached limit, end
            //
            if (limit > 0 && njobs >= limit)
            {
                break;
            }
            // match job against requesting user if my-job set
            //
            if (myjobs && user[0])
            {
                result = ipp_get_attr_by_name("requesting-user-name", job->job_oper_attr, &attr);
                if (! result)
                {
                    result = ipp_get_only_attr_string_value(attr, text, sizeof(text));
                    if (! result)
                    {
                        if (strcmp(user, text))
                        {
                            butil_log(5, "exclude job for user %s, only want %s\n", text, user);
                            job = job->next;
                            continue;
                        }
                    }
                }
            }
            if (njobs > 0)
            {
                result = ipp_set_req_out_tag(req, IPP_JOB_ATTRS, IPP_TAG_JOB);
                if (result)
                {
                    ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
                    return result;
                }
            }
            result = ipp_req_job_attrs(req, reqattr, job);
            if (result)
            {
                ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
                return result;
            }
            njobs++;
            job = job->next;
        }
    }
    return result;
}

static int ipp_op_get_job_attributes(ipp_request_t *req)
{
    ipp_attr_t *reqattr;
    ipp_attr_t *allattr;
    ipp_attr_t *attr;
    ipp_job_t  *job;
    int32_t     jobid;
    int         result;

    if (! req || ! req->ipp)
    {
        return -1;
    }
    // get job id
    //
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "job-id", &attr);
    if (result)
    {
        butil_log(5, "No job-id in oper attributes\n");
        ipp_set_error(req, IPP_STATUS_ERROR_ATTRIBUTES_OR_VALUES);
        return result;
    }
    result = ipp_get_only_attr_int32_value(attr, &jobid);
    if (result)
    {
        return result;
    }
    // look for job id in listings
    //
    result = ipp_get_job_by_id(req->ipp, jobid, &job);
    if (result || ! job)
    {
        butil_log(5, "No job id %d found\n", jobid);
        ipp_set_error(req, result ?
                    IPP_STATUS_ERROR_ATTRIBUTES_OR_VALUES :
                    IPP_STATUS_ERROR_NOT_POSSIBLE);
        return -1;
    }
    // see if there are requested return attributes list
    //
    allattr = NULL;
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "requested-attributes", &reqattr);
    if (result)
    {
        butil_log(5, "No requested attributes, returning all\n");

        // duplicate this attr from oper group
        result = ipp_get_group_attr_by_name("requested-attributes", IPP_GROUPING_OPERATION, &attr);
        if (! result)
        {
            result = ipp_dupe_attr(attr, &allattr);
            if (! result)
            {
                result = ipp_set_attr_string_value("requested-attributes", allattr, 1, "all");
                reqattr = allattr;
            }
        }
    }
    // dump all the job attributes into the response
    //
    result = ipp_req_job_attrs(req, reqattr, job);

    if (allattr)
    {
        ipp_destroy_attr(allattr);
    }
    return result;
}

int ipp_op_cancel_job(ipp_request_t *req)
{
    ipp_job_t *job;
    ipp_attr_t *attr;
    int32_t jobid;
    int result;

    if (! req || ! req->ipp)
    {
        return -1;
    }
    // get job id
    //
    result = ipp_get_req_in_attribute(req, IPP_OPER_ATTRS, "job-id", &attr);
    if (result)
    {
        butil_log(5, "No job-id in oper attributes\n");
        ipp_set_error(req, IPP_STATUS_ERROR_ATTRIBUTES_OR_VALUES);
        return result;
    }
    result = ipp_get_only_attr_int32_value(attr, &jobid);
    if (result)
    {
        return result;
    }
    // look for job id in listings
    //
    result = ipp_get_job_by_id(req->ipp, jobid, &job);
    if (result)
    {
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
    result = ipp_cancel_job(req->ipp, job);
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
        return ipp_op_validate_job(req);

    case IPP_OP_GET_JOBS:
        return ipp_op_get_jobs(req);

    case IPP_OP_GET_JOB_ATTRIBUTES:
        return ipp_op_get_job_attributes(req);

    case IPP_OP_CREATE_JOB:
        return ipp_op_create_job(req, NULL);

    case IPP_OP_CANCEL_JOB:
        return ipp_op_cancel_job(req);

    case IPP_OP_SEND_DOCUMENT:
    case IPP_OP_SEND_URI:
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


