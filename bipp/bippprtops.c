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
        // fetch printer-attributes group and add to return
        result = ipp_get_attr_by_name("printer-uri-supported", IPP_GROUPING_PRINTER_STATUS, &attr);
        if (result)
        {
            ipp_set_error(req, IPP_STATUS_ERROR_INTERNAL);
            break;
        }
        result = ipp_add_req_out_attribute(req, IPP_PRT_ATTRS, attr);
        break;
    case IPP_OP_PRINT_JOB:
    case IPP_OP_PRINT_URI:
    case IPP_OP_VALIDATE_JOB:
    case IPP_OP_CREATE_JOB:
    case IPP_OP_SEND_DOCUMENT:
    case IPP_OP_SEND_URI:
    case IPP_OP_CANCEL_JOB:
    case IPP_OP_GET_JOB_ATTRIBUTES:
    case IPP_OP_GET_JOBS:
    case IPP_OP_HOLD_JOB:
    case IPP_OP_RELEASE_JOB:
    case IPP_OP_RESTART_JOB:
    case IPP_OP_PAUSE_PRINTER:
    case IPP_OP_RESUME_PRINTER:
    case IPP_OP_PURGE_JOBS:
    case IPP_OP_SET_PRINTER_ATTRIBUTES:
    case IPP_OP_SET_JOB_ATTRIBUTES:
    case IPP_OP_GET_PRINTER_SUPPORTED_VALUES:
    case IPP_OP_CREATE_PRINTER_SUBSCRIPTIONS:
    case IPP_OP_CREATE_JOB_SUBSCRIPTIONS:
    case IPP_OP_GET_SUBSCRIPTION_ATTRIBUTES:
    case IPP_OP_GET_SUBSCRIPTIONS:
    case IPP_OP_RENEW_SUBSCRIPTION:
    case IPP_OP_CANCEL_SUBSCRIPTION:
    case IPP_OP_GET_NOTIFICATIONS:
    case IPP_OP_SEND_NOTIFICATIONS:
    case IPP_OP_GET_RESOURCE_ATTRIBUTES:
    case IPP_OP_GET_RESOURCE_DATA:
    case IPP_OP_GET_RESOURCES:
    case IPP_OP_GET_PRINT_SUPPORT_FILES:
    case IPP_OP_ENABLE_PRINTER:
    case IPP_OP_DISABLE_PRINTER:
    case IPP_OP_PAUSE_PRINTER_AFTER_CURRENT_JOB:
    case IPP_OP_HOLD_NEW_JOBS:
    case IPP_OP_RELEASE_HELD_NEW_JOBS:
    case IPP_OP_DEACTIVATE_PRINTER:
    case IPP_OP_ACTIVATE_PRINTER:
    case IPP_OP_RESTART_PRINTER:
    case IPP_OP_SHUTDOWN_PRINTER:
    case IPP_OP_STARTUP_PRINTER:
    case IPP_OP_REPROCESS_JOB:
    case IPP_OP_CANCEL_CURRENT_JOB:
    case IPP_OP_SUSPEND_CURRENT_JOB:
    case IPP_OP_RESUME_JOB:
    case IPP_OP_PROMOTE_JOB:
    case IPP_OP_SCHEDULE_JOB_AFTER:
    case IPP_OP_CANCEL_DOCUMENT:
    case IPP_OP_GET_DOCUMENT_ATTRIBUTES:
    case IPP_OP_GET_DOCUMENTS:
    case IPP_OP_DELETE_DOCUMENT:
    case IPP_OP_SET_DOCUMENT_ATTRIBUTES:
    case IPP_OP_CANCEL_JOBS:
    case IPP_OP_CANCEL_MY_JOBS:
    case IPP_OP_RESUBMIT_JOB:
    case IPP_OP_CLOSE_JOB:
    case IPP_OP_IDENTIFY_PRINTER:
    case IPP_OP_VALIDATE_DOCUMENT:
    case IPP_OP_ADD_DOCUMENT_IMAGES:
    case IPP_OP_ACKNOWLEDGE_DOCUMENT:
    case IPP_OP_ACKNOWLEDGE_IDENTIFY_PRINTER:
    case IPP_OP_ACKNOWLEDGE_JOB:
    case IPP_OP_FETCH_DOCUMENT:
    case IPP_OP_FETCH_JOB:
    case IPP_OP_GET_OUTPUT_DEVICE_ATTRIBUTES:
    case IPP_OP_UPDATE_ACTIVE_JOBS:
    case IPP_OP_DEREGISTER_OUTPUT_DEVICE:
    case IPP_OP_UPDATE_DOCUMENT_STATUS:
    case IPP_OP_UPDATE_JOB_STATUS:
    case IPP_OP_UPDATE_OUTPUT_DEVICE_ATTRIBUTES:
    case IPP_OP_GET_NEXT_DOCUMENT_DATA:
    case IPP_OP_ALLOCATE_PRINTER_RESOURCES:
    case IPP_OP_CREATE_PRINTER:
    case IPP_OP_DEALLOCATE_PRINTER_RESOURCES:
    case IPP_OP_DELETE_PRINTER:
    case IPP_OP_GET_PRINTERS:
    case IPP_OP_SHUTDOWN_ONE_PRINTER:
    case IPP_OP_STARTUP_ONE_PRINTER:
    case IPP_OP_CANCEL_RESOURCE:
    case IPP_OP_CREATE_RESOURCE:
    case IPP_OP_INSTALL_RESOURCE:
    case IPP_OP_SEND_RESOURCE_DATA:
    case IPP_OP_SET_RESOURCE_ATTRIBUTES:
    case IPP_OP_CREATE_RESOURCE_SUBSCRIPTIONS:
    case IPP_OP_CREATE_SYSTEM_SUBSCRIPTIONS:
    case IPP_OP_DISABLE_ALL_PRINTERS:
    case IPP_OP_ENABLE_ALL_PRINTERS:
    case IPP_OP_GET_SYSTEM_ATTRIBUTES:
    case IPP_OP_GET_SYSTEM_SUPPORTED_VALUES:
    case IPP_OP_PAUSE_ALL_PRINTERS:
    case IPP_OP_PAUSE_ALL_PRINTERS_AFTER_CURRENT_JOB:
    case IPP_OP_REGISTER_OUTPUT_DEVICE:
    case IPP_OP_RESTART_SYSTEM:
    case IPP_OP_RESUME_ALL_PRINTERS:
    case IPP_OP_SET_SYSTEM_ATTRIBUTES:
    case IPP_OP_SHUTDOWN_ALL_PRINTERS:
    case IPP_OP_STARTUP_ALL_PRINTERS:
    default:
        ipp_set_error(req, IPP_STATUS_ERROR_OPERATION_NOT_SUPPORTED);
        result = -1;
    }
    return result;
}


