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
#ifndef BIPPJOBS_H
#define BIPPJOBS_H 1

#include "bnetheaders.h"
#include "bstreamio.h"
#include "bippattr.h"

struct tag_ipp_server;
struct tag_ipp_request;

typedef struct tag_job_context
{
    iostream_t             *print_stream;    ///< data output stream
    ipp_attr_t             *job_oper_attr;   ///< Operation attributes (at job create time)
    ipp_attr_t             *job_stat_attr;   ///< job status attributes
    ipp_attr_t             *job_desc_attr;   ///< job description attributes
    uint32_t                id;              ///< job id
    time_t                  last_byte_time;  ///< time the last byte arrived
    time_t                  max_idle_time;   ///< how long job can go idle (no data)
    ipp_jstate_t            state;           ///< current job state
    void                   *priv;            ///< for processor to attach
    struct tag_job_context *next;
}
ipp_job_t;

int ipp_sink_job_data       (struct tag_ipp_request *req);
int ipp_job_check_timeout   (struct tag_ipp_server *ipp);
int ipp_get_active_jobs     (struct tag_ipp_server *ipp, ipp_job_t **pjobs);
int ipp_get_completed_jobs  (struct tag_ipp_server *ipp, ipp_job_t **pjobs);
int ipp_start_job           (struct tag_ipp_server *ipp, ipp_job_t *job);
int ipp_abort_job           (struct tag_ipp_server *ipp, ipp_job_t *job);
int ipp_cancel_job          (struct tag_ipp_server *ipp, ipp_job_t *job);
int ipp_complete_job        (struct tag_ipp_server *ipp, ipp_job_t *job);
int ipp_get_job_by_id       (struct tag_ipp_server *ipp, int32_t jobid, ipp_job_t **pjob);
int ipp_create_job          (struct tag_ipp_server *ipp, struct tag_ipp_request *req, ipp_job_t **pjob);
int ipp_destroy_job         (struct tag_ipp_server *ipp, ipp_job_t *job);
int ipp_job_init            (struct tag_ipp_server *ipp);

#endif

