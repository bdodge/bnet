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
#include "bippjobs.h"
#include "bipp.h"
#include "bippproto.h"
#include "butil.h"

int ipp_get_active_jobs(ipp_server_t *ipp, ipp_job_t **pjobs)
{
    if (! ipp || ! pjobs)
    {
        return -1;
    }
    *pjobs = ipp->jobs_active;
    return 0;
}

int ipp_get_completed_jobs(ipp_server_t *ipp, ipp_job_t **pjobs)
{
    if (! ipp || ! pjobs)
    {
        return -1;
    }
    *pjobs = ipp->jobs_completed;
    return 0;
}

int ipp_start_job(ipp_server_t *ipp, ipp_job_t *job)
{
    ipp_job_t *aj;
    char uri[IPP_MAX_TEXT];
    int result;

    if (! ipp || ! job)
    {
        return -1;
    }
    // duplicate job status and description groups
    // into job attributes lists
    //
    result  = ipp_dupe_grouping(IPP_GROUPING_JOB_STATUS, &job->job_stat_attr);
    result |= ipp_dupe_grouping(IPP_GROUPING_JOB_DESCRIPTION, &job->job_desc_attr);
    if (result)
    {
        return result;
    }
    // set initial value of non-static content
    //
    result = ipp_set_attr_int32_value("job-id", job->job_stat_attr, 1, job->id);
    if (result)
    {
        return result;
    }
    snprintf(uri, sizeof(uri), "%s/job/%d", ipp->uri, job->id);
    result = ipp_set_attr_string_value("job-uri", job->job_stat_attr, 1, uri);
    if (result)
    {
        return result;
    }
    if (! ipp->jobs_active)
    {
        ipp->jobs_active = job;
    }
    else
    {
        for (aj = ipp->jobs_active; aj->next;)
        {
            aj = aj->next;
        }
        aj->next = job;
    }
    job->next = NULL;
    job->state = IPP_JSTATE_PROCESSING;
    return 0;
}

int ipp_cancel_job(ipp_server_t *ipp, ipp_job_t *job)
{
    int result;

    result = ipp_complete_job(ipp, job);
    job->state = IPP_JSTATE_CANCELED;
    return result;
}

int ipp_complete_job(ipp_server_t *ipp, ipp_job_t *job)
{
    ipp_job_t *aj;

    if (! ipp || ! job)
    {
        return -1;
    }
    if (! ipp->jobs_completed)
    {
        ipp->jobs_completed = job;
    }
    else
    {
        for (aj = ipp->jobs_completed; aj->next;)
        {
            aj = aj->next;
        }
        aj->next = job;
    }
    job->next = NULL;
    job->state = IPP_JSTATE_COMPLETED;
    return 0;
}

int ipp_create_job(ipp_server_t *ipp, ipp_request_t *req, ipp_job_t **pjob)
{
    ipp_job_t *job;

    if (! req || ! req->ipp)
    {
        return -1;
    }
    job = NULL;

    if (ipp->jobs_free)
    {
        job = ipp->jobs_free;
        ipp->jobs_free = ipp->jobs_free->next;
    }
    if (! job)
    {
        // no free jobs, cull from completed list end
        //
        if (! ipp->jobs_completed)
        {
            BERROR("Alloc job");
            return -1;
        }
        if (ipp->jobs_completed->next)
        {
            ipp_job_t *prevjob;

            prevjob = ipp->jobs_completed;
            job = prevjob->next;
            while (job->next)
            {
                prevjob = job;
                job = job->next;
            }
            prevjob->next = NULL;
        }
        else
        {
            job = ipp->jobs_completed;
            ipp->jobs_completed = NULL;
        }
    }
    if (! job)
    {
        BERROR("Alloc-pool job");
        return -1;
    }
    job->id = ipp->job_id++;
    if (ipp->job_id > 0x7fffffff)
    {
        ipp->job_id = 1;
    }
    job->job_stat_attr = NULL;
    job->job_desc_attr = NULL;
    *pjob = job;
    return 0;
}

int ipp_destroy_job(ipp_server_t *ipp, ipp_job_t *job)
{
    if (! ipp || ! job)
    {
        return -1;
    }
    job->next = ipp->jobs_free;
    ipp->jobs_free = job;
    return 0;
}

int ipp_job_init(ipp_server_t *ipp)
{
    size_t jdex;

    ipp->job_id = 1;

    for (jdex = 0; jdex < (IPP_MAX_JOBS - 1); jdex++)
    {
        ipp->job_pool[jdex].next = &ipp->job_pool[jdex + 1];
        ipp->job_pool[jdex].job_stat_attr = NULL;
        ipp->job_pool[jdex].job_desc_attr = NULL;
    }
    ipp->job_pool[jdex].next = NULL;
    ipp->job_pool[jdex].job_stat_attr = NULL;
    ipp->job_pool[jdex].job_desc_attr = NULL;

    ipp->jobs_free = ipp->job_pool;
    ipp->jobs_active = NULL;
    ipp->jobs_completed = NULL;

    return 0;
}

