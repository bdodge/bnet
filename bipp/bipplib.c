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

static ipp_server_t s_ipp_server;

int ipp_canned_resource_callback(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    butil_log(5, "Canned Resource CB: %d\n", cbtype);

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

        butil_log(5, "IPP resource cb (request)  %s: %s\n",
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

        butil_log(5, "IPP download cb (body->): %zu bytes\n", count ? *count : 0);

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

        // if the request has already got to reply state (because
        // of errors for example) then move to upload state
        //
        if (req->state[req->top] >= reqReply)
        {
            client->state = httpBodyUpload;
            client->out_content_length = 0x100000;
            client->out_transfer_type = httpChunked;
        }
        break;

    case httpDownloadDone:

        butil_log(5, "IPP download done cb in state %s\n",
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

        butil_log(5, "IPP upload cb (<-body)\n");

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
                butil_log(5, "IPP Done, ending upload of body\n");
                client->out_content_length = req->out.count;
            }
            else
            {
                // ipp still chugging, force a large out len
                // to keep http calling us back here
                //
                if (req->out.count)
                {
                    butil_log(5, "IPP made %d body bytes to upload\n", req->out.count);
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

        butil_log(5, "IPP done cb\n");
        if (req)
        {
            result = ipp_req_destroy(ipp, req);
            client->ctxpriv = NULL;
        }
#ifdef BMEM_H
        http_server_abort(&ipp->server);
#endif
        break;

    default:

        return -1;
    }
    return 0;
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
    result = butil_paste_url(ipp->uri, sizeof(ipp->uri), ipp->scheme, hostname, ipp->port, ipp->path);
    if (result)
    {
        return result;
    }
    // set printer-uri in printer status group
    //
    result = ipp_set_attr_string_value_by_name("printer-uri-supported", IPP_GROUPING_PRINTER_STATUS, ipp->uri);
    return result;
}

int ipp_server(const char *program, uint16_t port, bool isTLS)
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
    result = ipp_req_init(&s_ipp_server);
    if (result)
    {
        BERROR("can't init reqs");
    }
    ipp = &s_ipp_server;

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
    ipp->port = port;
    strncpy(ipp->path, "/ipp", sizeof(ipp->path) - 1);
    ipp->path[sizeof(ipp->path) - 1] = '\0';

    result = ipp_set_environment(ipp);

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
    result = http_serve(&ipp->server);
    if (result)
    {
        butil_log(2, "server on port %u ends\n", ipp->server.port);
    }
    http_server_cleanup(&ipp->server);
    return result;
}

