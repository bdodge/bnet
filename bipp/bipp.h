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
#include "bhttpconfig.h"
#include "bhttp.h"
#include "bstreamio.h"
#include "butil.h"
#include "bippreq.h"

/// Context for an IPP entity (a printer / print-server)
//
typedef struct tag_ipp_server
{
    /// pool of ipp request contexts
    ipp_request_t req_pool[IPP_MAX_REQUESTS];
    ipp_request_t *req_free;
}
ipp_server_t;

int ipp_request(ipp_server_t *ipp, ipp_request_t *req);
int ipp_process(ipp_server_t *ipp, ipp_request_t *req);

#endif

