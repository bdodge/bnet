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
#include "bipp.h"
#include "bipplib.h"

// debug log level for ipp http operations
//
#define IPPLHT (6)

static ipp_server_t s_ipp_server;

int ipp_canned_resource_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    butil_log(IPPLHT, "Canned Resource CB: %d\n", cbtype);

    http_method_t method;
    int result;

    method = client->method;
    if (method == httpUser0)
    {
        client->method = httpGet;
    }
    else if (method == httpUser1)
    {
        client->method = httpPut;
    }
    result = http_canned_callback(
                                client,
                                resource,
                                cbtype,
                                data,
                                count
                                );
    client->method = method;
    return result;
}

int ipp_resource_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    ipp_server_t *ipp;
    ipp_request_t *req;
    int result;

    butil_log(7, "IPP Resource CB: %d\n", cbtype);

    if (! resource)
    {
        BERROR("no resource");
        return -1;
    }
    // the ipp server context is attached to the resource
    // when the resource was created
    //
    ipp = (ipp_server_t *)resource->priv;
    if (! ipp)
    {
        BERROR("no server context in resource");
        return -1;
    }
    // the request callback should have attached a req
    // context onto the http client
    //
    if (cbtype != httpRequest)
    {
        req = (ipp_request_t *)client->ctxpriv;
        if (! req)
        {
            BERROR("no client request context");
            return -1;
        }
    }
    switch (cbtype)
    {
    case httpRequest:

        butil_log(IPPLHT, "IPP resource cb (request)  %s: %s\n",
                http_method_name(client->method), data ? (char*)data : "<nil>");

        // grab an ipp request context for use during this request
        //
        req = ipp_req_create(ipp, client);
        if (! req)
        {
            BERROR("can't make request");
            return -1;
        }
        // attach it to the http client for access
        //
        client->ctxpriv = req;

        // ensure the output buffer is empty and aligned
        //
        if (client->out.count != 0)
        {
            BERROR("remnant output being dropped");
            client->out.count = 0;
        }
        iostream_normalize_ring(&client->out, NULL);

        // begin the request processing. if there is something
        // wrong with the req url itself, the req can be aborted now
        //
        result = ipp_request(req);

        if (result)
        {
            // httpDone cb will clean us up
            return result;
        }
        // any valid HTTP request will have inserted a 200 OK return
        // header in the output buffer. IPP reply data is sent in
        // chunks to avoid needing to back annotate counts
        //
        break;

    case httpDownloadData:

        butil_log(IPPLHT, "IPP download cb (body->): %zu bytes\n", count ? *count : 0);

        // since the whole request might not fit in a single io buffer the
        // req is processed incrementally each chance we get here
        //
        // copy the input ptrs to req's ioring. Note that this is ALWAYS a
        // single contiguous buffer. the http server insures its input is
        // normalized (left aligned and contiguous) before calling here. It
        // ends up being more efficient to do a memmove in the ioring each
        // request than to check for ring wrap at every byte
        //
        req->in.data = *data;
        req->in.count = *count;
        req->in.tail = 0;
        req->in.head = req->in.count;
        req->in.size = req->in.count;

        result = ipp_process(req);

        // see how many bytes we took from data, to update client
        //
        *count = (*count - req->in.count);

        break;

    case httpDownloadDone:

        butil_log(IPPLHT, "IPP download done cb in state %s\n",
                ipp_state_string(req->state[req->top]));

        // indicate no more body data
        req->download_complete = true;

        // ipp parsing might need multiple calls to get finished and
        // dispatch the command, and get to reply state. once it is
        // in the reply state, we move to httpBodyUpload state to
        // drive response building and sending where http client takes
        // care of chunking, etc.  we just give it a big content length
        //
        result = 0;

        while (! result && req->state[req->top] < reqReply)
        {
            result = ipp_process(req);
            if (result)
            {
                // errors in processing will still move to reply state so
                // ignore them here
                //
                result = 0;
            }
        }
        if (req->state[req->top] == reqReply)
        {
            // build the reply in the outbuf inside upload data
            //
            client->state = httpBodyUpload;
            client->out_content_length = 0x100000;
            client->out_transfer_type = httpChunked;
        }
        else
        {
            // something went very wrong?
            //
            butil_log(0, "Ending in cb, not expected\n");
            client->state = httpDone; //??
            client->out_content_length = 0x100000;
        }
        break;

    case httpUploadData:

        butil_log(IPPLHT, "IPP upload cb (<-body)\n");

        // setup request's output buffer as spot in client's out buffer
        //
        // this is really a window into the http client's output buffer
        // but we know that that buffer is contiguous at this point
        //
        // *data is the buffer area (contiguous)
        // *count is the size of the buffer area
        //
        req->out.data  = *data;
        req->out.count = 0;
        req->out.tail = 0;
        req->out.head = 0;
        req->out.size = *count;

        // process the current request, until it goes to done state
        //
        result = ipp_process(req);
        if (result)
        {
            // end uploading ?  can't get here really
            //
            butil_log(0, "Catastrophy ends IPP Upload\n");
            client->state = httpDone;
        }
        else
        {
            if (req->state[req->top] == reqDone)
            {
                // set the content length to amount of data we made
                // so the http client moves on to complete after
                // sending the data out
                //
                butil_log(IPPLHT, "IPP Done, ending upload of body\n");
                client->out_content_length = req->out.count;
            }
            else
            {
                // ipp still chugging, force a large out len
                // to keep http calling us back here
                //
                if (req->out.count)
                {
                    butil_log(IPPLHT, "IPP made %d body bytes to upload\n", req->out.count);
                }
                client->out_content_length = 0x100000;
            }
        }
        // return any data generated by IPP
        //
        *count = req->out.count;
        *data = req->out.data;
        break;

    case httpComplete:

        butil_log(IPPLHT, "IPP done cb\n");
        if (req)
        {
            result = ipp_req_destroy(ipp, req);
            client->ctxpriv = NULL;
        }
#ifdef BMEM_H
        #if 1
        bmem_stats(5);
        #else
        http_server_abort(&ipp->server);
        #endif
#endif
        break;

    default:

        return -1;
    }
    return 0;
}

int ipp_set_static_environment(ipp_server_t *ipp)
{
    ipp_attr_t *attr;
    char text[IPP_MAX_TEXT];
    int result;

    result = ipp_set_group_attr_string_value(
                                                "charset-configured",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "utf-8"
                                                );

    result |= ipp_set_group_attr_string_value(
                                                "charset-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "utf-8"
                                                );

    result |= ipp_set_group_attr_string_value(
                                                "compression-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "none"
                                                );

    result |= ipp_set_group_attr_int32_value(
                                                "copies-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                1
                                                );

    result |= ipp_set_group_attr_range_value(
                                                "copies-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                1, 99
                                                );

    result |= ipp_set_group_attr_string_value(
                                                "document-format-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "text/plain"
                                                );
#if 1
    result |= ipp_get_attr_for_grouping(IPP_GROUPING_PRINTER_DESCRIPTION, &attr);

    if (! result)
    {
        result = ipp_get_attr_by_name("document-format-supported", attr, &attr);
    }
    if (! result)
    {
        const char *pdlstr;

#if IPP_SUPPORT_PDF
        pdlstr = "application/pdf";
        result |= ipp_add_attr_value(attr, (uint8_t*)pdlstr, strlen(pdlstr));
#endif
#if IPP_SUPPORT_PLAINTEXT
        pdlstr = "text/plain";
        result |= ipp_add_attr_value(attr, (uint8_t*)pdlstr, strlen(pdlstr));
#endif
#if IPP_SUPPORT_JPEG
        pdlstr = "image/jpeg";
        result |= ipp_add_attr_value(attr, (uint8_t*)pdlstr, strlen(pdlstr));
#endif
    }
#else
    result |= ipp_set_group_attr_string_value(
                                                "document-format-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "text/plain"
                                                );
#endif
    result |= ipp_set_group_attr_string_value(
                                                "generated-natural-language-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "en"
                                                );

    snprintf(text, sizeof(text), "%d.%d", IPP_MAJOR_VERSION_MAX, IPP_MINOR_VERSION_MAX);

    result |= ipp_set_group_attr_string_value(
                                                "ipp-versions-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                2,
                                                "1.1",
                                                text
                                                );

    result |= ipp_set_group_attr_string_value(
                                                "pdl-override-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "attempted"
                                                );

    result |= ipp_set_group_attr_string_value(
                                                "natural-language-configured",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "en"
                                                );

    result |= ipp_set_group_attr_int32_value(
                                                "operations-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                    #if  (IPP_MAJOR_VERSION_MAX<2)
                                                8,
                    #else
                                                53,
                    #endif
                                                IPP_OP_PRINT_JOB,
                                                IPP_OP_VALIDATE_JOB,
                                                IPP_OP_CREATE_JOB,
                                                IPP_OP_CANCEL_JOB,
                                                IPP_OP_GET_JOB_ATTRIBUTES,
                                                IPP_OP_SEND_DOCUMENT,
                                                IPP_OP_GET_JOBS,
                                                IPP_OP_GET_PRINTER_ATTRIBUTES
                    #if  (IPP_MAJOR_VERSION_MAX>=2)
                                                    ,
                                                IPP_OP_HOLD_JOB,
                                                IPP_OP_RELEASE_JOB,
                                                IPP_OP_RESTART_JOB,
                                                IPP_OP_PAUSE_PRINTER,
                                                IPP_OP_RESUME_PRINTER,
                                                IPP_OP_PURGE_JOBS,
                                                IPP_OP_SET_PRINTER_ATTRIBUTES,
                                                IPP_OP_SET_JOB_ATTRIBUTES,
                                                IPP_OP_GET_PRINTER_SUPPORTED_VALUES,
                                                IPP_OP_CREATE_PRINTER_SUBSCRIPTIONS,
                                                IPP_OP_CREATE_JOB_SUBSCRIPTIONS,
                                                IPP_OP_GET_SUBSCRIPTION_ATTRIBUTES,
                                                IPP_OP_GET_SUBSCRIPTIONS,
                                                IPP_OP_RENEW_SUBSCRIPTION,
                                                IPP_OP_CANCEL_SUBSCRIPTION,
                                                IPP_OP_GET_NOTIFICATIONS,
                                                IPP_OP_SEND_NOTIFICATIONS,
                                                IPP_OP_GET_RESOURCE_ATTRIBUTES,
                                                IPP_OP_GET_RESOURCE_DATA,
                                                IPP_OP_GET_RESOURCES,
                                                IPP_OP_GET_PRINT_SUPPORT_FILES,
                                                IPP_OP_ENABLE_PRINTER,
                                                IPP_OP_DISABLE_PRINTER,
                                                IPP_OP_PAUSE_PRINTER_AFTER_CURRENT_JOB,
                                                IPP_OP_HOLD_NEW_JOBS,
                                                IPP_OP_RELEASE_HELD_NEW_JOBS,
                                                IPP_OP_RESTART_PRINTER,
                                                IPP_OP_SHUTDOWN_PRINTER,
                                                IPP_OP_STARTUP_PRINTER,
                                                IPP_OP_REPROCESS_JOB,
                                                IPP_OP_CANCEL_CURRENT_JOB,
                                                IPP_OP_SUSPEND_CURRENT_JOB,
                                                IPP_OP_RESUME_JOB,
                                                IPP_OP_PROMOTE_JOB,
                                                IPP_OP_SCHEDULE_JOB_AFTER,
                                                IPP_OP_CANCEL_DOCUMENT,
                                                IPP_OP_GET_DOCUMENT_ATTRIBUTES,
                                                IPP_OP_GET_DOCUMENTS,
                                                IPP_OP_DELETE_DOCUMENT,
                                                IPP_OP_SET_DOCUMENT_ATTRIBUTES,
                                                IPP_OP_CANCEL_JOBS,
                                                IPP_OP_CANCEL_MY_JOBS,
                                                IPP_OP_RESUBMIT_JOB,
                                                IPP_OP_CLOSE_JOB,
                                                IPP_OP_IDENTIFY_PRINTER
                    #endif

                                             );

    result |= ipp_set_group_attr_string_value(
                                                "printer-name",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "bnetipp"
                                                );

    result |= ipp_set_group_attr_string_value(
                                                "uri-authentication-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "none"
                                                    /*
                                                "requesting-user-name",
                                                "basic",
                                                "digest"
                                                    */
                                                );

    result |= ipp_set_group_attr_string_value(
                                                "uri-security-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "none"
                                                    /*
                                                "none",
                                                "s",
                                                "gest"
                                                    */
                                                );
#if  (IPP_MAJOR_VERSION_MAX>=2)
    result |= ipp_set_group_attr_string_value(
                                                "uri-security-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1,
                                                "none"
                                                );
    result |= ipp_set_group_attr_bool_value(
                                                "color-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, true
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "finishings-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, IPP_FINISHINGS_NONE
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "finishings-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                2, IPP_FINISHINGS_NONE, IPP_FINISHINGS_STAPLE
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "ippget-event-life",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, 230
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "job-creation-attributes-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "job-hold-until-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "no-hold"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "job-hold-until-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "no-hold"
                                            );
    result |= ipp_set_group_attr_bool_value(
                                                "job-ids-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, true
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "job-priority-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, 5
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "job-priority-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, 5
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "job-settable-attributes-supported",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "job-sheets-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "job-sheets-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
#if 0
    result |= ipp_set_group_attr_string_value(
                                                "media-col-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "media-col-ready",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
#endif
    result |= ipp_set_group_attr_string_value(
                                                "media-col-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "media-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "media-ready",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "media-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_bool_value(
                                                "multiple-document-jobs-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, true
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "multiple-operation-time-out",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, IPP_MAX_JOB_IDLE_SECONDS
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "notify-events-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "notify-events-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "notify-lease-duration-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, 10
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "notify-lease-duration-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, 10
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "notify-max-events-supported",
                                                IPP_GROUPING_SUBSCRIPTION_TEMPLATE,
                                                1, 16
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "notify-pull-method-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "number-up-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, 1
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "number-up-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                3, 1, 2, 3
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "orientation-requested-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, IPP_ORIENT_PORTRAIT
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "orientation-requested-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                2, IPP_ORIENT_PORTRAIT, IPP_ORIENT_LANDSCAPE
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "output-bin-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "output-bin-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "overrides-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_bool_value(
                                                "page-ranges-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, true
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "pages-per-minute",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1, 300
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "pages-per-minute-color",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1, 200
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "print-quality-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, IPP_QUALITY_NORMAL
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "print-quality-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                2, IPP_QUALITY_NORMAL, IPP_QUALITY_HIGH
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "printer-alert",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "printer-alert-description",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "printer-device-id",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "printer-info",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "printer-location",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "printer-make-and-model",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "printer-message-from-operator",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "printer-more-info",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1, ipp->web_uri
                                            );
    result |= ipp_set_group_attr_resolution_value(
                                                "printer-resolution-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, 600, 600, IPP_RES_PER_INCH
                                            );
    result |= ipp_set_group_attr_resolution_value(
                                                "printer-resolution-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                2,
                                                600, 600, IPP_RES_PER_INCH,
                                                1200, 1200, IPP_RES_PER_INCH
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "printer-settable-attributes-supported",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1, "none"
                                            );
    result |= ipp_set_group_attr_int32_value(
                                                "printer-state-change-time",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1, 0
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "sides-default",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "one-sided"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "sides-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "one-sided"
                                            );
    result |= ipp_set_group_attr_string_value(
                                                "which-jobs-supported",
                                                IPP_GROUPING_PRINTER_DESCRIPTION,
                                                1, "none"
                                            );
#endif
    return result;
}

int ipp_set_environment(ipp_server_t *ipp)
{
    char hostname[HTTP_MAX_PATH];
    int result;

    result = gethostname(hostname, sizeof(hostname));
    if (result)
    {
        return result;
    }
    // for uri of printers ipp url
    //
    result = butil_paste_url(ipp->print_uri, sizeof(ipp->print_uri), ipp->scheme, hostname, ipp->port, ipp->path);
    if (result)
    {
        return result;
    }
    result = butil_paste_url(ipp->web_uri, sizeof(ipp->web_uri), schemeHTTP, hostname, ipp->web_port, "/");
    if (result)
    {
        return result;
    }
    result = ipp_set_group_attr_string_value(
                                                "printer-uri-supported",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                IPP_SUPPORT_TLS ? 2 : 1,
                                                ipp->print_uri,
                                                ipp->secure_print_uri
                                                );

    result |= ipp_set_group_attr_bool_value(
                                                "printer-is-accepting-jobs",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1,
                                                1
                                                );

    result |= ipp_set_group_attr_int32_value(
                                                "printer-state",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1,
                                                3
                                                );

    result |= ipp_set_group_attr_string_value(
                                                "printer-state-reasons",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1,
                                                "none"
                                                );

    result |= ipp_set_group_attr_int32_value(
                                                "printer-up-time",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1,
                                                1000
                                                );

    result |= ipp_set_group_attr_int32_value(
                                                "queued-job-count",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1,
                                                0
                                                );
#if  (IPP_MAJOR_VERSION_MAX>=2)
    result |= ipp_set_group_attr_string_value(
                                                "printer-more-info",
                                                IPP_GROUPING_PRINTER_STATUS,
                                                1, ipp->web_uri
                                            );
#endif
    return result;
}

int ipp_on_idle(void *priv)
{
    ipp_server_t *ipp;
    time_t now;

    ipp = (ipp_server_t *)priv;
    if (! ipp)
    {
        return -1;
    }
    // every second, check on job expiry
    //
    time(&now);
    if (now > ipp->idle_time)
    {
        ipp->idle_time = now;
        ipp_job_check_timeout(ipp);
    }
    return 0;
}

int ipp_server(const char *program, uint16_t port, bool isTLS, const char *print_path)
{
    ipp_server_t *ipp;
    http_resource_t *resources = NULL;
    http_credentials_t creds;

    int result;
    int i;

#if HTTP_SUPPORT_TLS
    result = iostream_tls_prolog();
    if (result)
    {
        BERROR("Can't init TLS");
        return -1;
    }
#endif
    ipp = &s_ipp_server;

    result = ipp_req_init(ipp);
    if (result)
    {
        BERROR("can't init reqs");
    }
    result = ipp_job_init(ipp);
    if (result)
    {
        BERROR("can't init jobs");
    }
    result = ipp_attr_init();
    if (result)
    {
        BERROR("can't init attrs");
    }
    // register a custom scheme
    result = butil_register_scheme("ipp", &ipp->scheme);
    if (result)
    {
        BERROR("can't register scheme");
    }
    // add canned index.html at http for testing
    char *cannedindex = "<h2>hello world</h2>";

    result = http_add_canned_resource(&resources, schemeHTTP,  "/index.html",
                NULL, butil_mime_html, (uint8_t*)cannedindex, strlen(cannedindex));
    if (result)
    {
        BERROR("can't make resource");
        return result;
    }
    // setup the printer environment
    //
    strncpy(ipp->stream_path, print_path, sizeof(ipp->stream_path) - 1);
    ipp->stream_path[sizeof(ipp->stream_path) - 1] = '\0';

    ipp->port = port;
    ipp->secure_port = 443;
    ipp->web_port = 8080;

    strncpy(ipp->path, "/ipp", sizeof(ipp->path) - 1);
    ipp->path[sizeof(ipp->path) - 1] = '\0';

    result = ipp_set_static_environment(ipp);
    if (result)
    {
        return result;
    }
    result = ipp_set_environment(ipp);
    if (result)
    {
        return result;
    }

    // handle all HTTP scheme urls in callback
    //
    result = http_add_func_resource(&resources, schemeHTTP,  ipp->path,
                    NULL, ipp_resource_callback, ipp);
    if (result)
    {
        BERROR("can't make resource");
        return result;
    }
    // set use-tls for certain ports
    //
    result = http_server_init(&ipp->server, resources, ipp->port, httpTCP, isTLS);
    if (result)
    {
        BERROR("can't start server");
        return result;
    }
    // set an idle-loop callback to do time-keeping jobs
    //

    result = http_serve(&ipp->server, ipp_on_idle, (void*)ipp);
    if (result)
    {
        butil_log(2, "server on port %u ends\n", ipp->server.port);
    }
    http_server_cleanup(&ipp->server);
    return result;
}

