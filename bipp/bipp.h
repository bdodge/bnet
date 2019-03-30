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
#ifndef BIPP_H
#define BIPP_H 1

#include "bippconfig.h"
#include "bippjobs.h"
#include "bippreq.h"
#include "bhttpconfig.h"
#include "bhttp.h"
#include "bstreamio.h"
#include "butil.h"

#if 0       // define to include memory tracing debug
#define BMEM_TRACE_ALLOCS 1
#include "bmem.h"
#define malloc bmem_alloc
#define free bmem_free
#endif

/// Context for an IPP entity (a printer / print-server)
//
typedef struct tag_ipp_server
{
    /// path and port we serve on
    char                path[IPP_MAX_TEXT];
    uint16_t            port;
    uint16_t            secure_port;
    uint16_t            web_port;

    /// serving uri, for convenience
    char                print_uri[IPP_MAX_TEXT];
    char                secure_print_uri[IPP_MAX_TEXT];
    char                web_uri[IPP_MAX_TEXT];

    /// ipp scheme handle
    butil_url_scheme_t   scheme;

    /// http server context
    http_server_t       server;

    /// next job id
    uint32_t            job_id;

    /// print stream path
    char                stream_path[IPP_MAX_TEXT];

    /// last time jobs were checked
    time_t              idle_time;

    /// pool of job contexts
    ipp_job_t           job_pool[IPP_MAX_JOBS];
    ipp_job_t          *jobs_free;
    ipp_job_t          *jobs_active;
    ipp_job_t          *jobs_completed;

    /// pool of ipp request contexts
    ipp_request_t       req_pool[IPP_MAX_REQUESTS];
    ipp_request_t      *req_free;
}
ipp_server_t;

const char *ipp_state_string(ipp_req_state_t state);
#if 1
#define ipp_set_error(a, b) ipp_set_error_dbg(__FILE__, __LINE__, a, b)
int ipp_set_error_dbg (const char* fname, int line, ipp_request_t *req, int16_t ecode);
#else
int ipp_set_error     (ipp_request_t *req, int16_t ecode);
#endif
int ipp_request        (ipp_request_t *req);
int ipp_process        (ipp_request_t *req);

#endif

