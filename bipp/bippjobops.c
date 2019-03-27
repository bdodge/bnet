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

    case IPP_OP_CREATE_JOB:
    case IPP_OP_SEND_DOCUMENT:
    case IPP_OP_SEND_URI:
    case IPP_OP_CANCEL_JOB:
    case IPP_OP_GET_JOB_ATTRIBUTES:
    case IPP_OP_GET_JOBS:
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


