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

static butil_url_scheme_t s_ipp_scheme;
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
    int old_count;
    int meth;

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

        // begin the request processing. if there is something
        // wrong with the req url itself, the req can be aborted now
        //
        result = ipp_request(ipp, req);
        if (result)
        {
            // httpDone cb will clean us up
            return result;
        }
        break;

    case httpDownloadData:

        butil_log(5, "IPP download cb (body->): %zu bytes\n", count ? *count : 0);

        // remember the state of in buffer
        //
        old_count = client->in.count;

        // since the whole request might not fit in a single io buffer the
        // req is processed incrementally each chance we get here
        //
        result = ipp_process(ipp, req);

        // update count as how many bytes processing consumed
        *count = old_count - client->in.count;
        break;

    case httpDownloadDone:

        butil_log(5, "IPP download done cb\n");

        result = ipp_process(ipp, req);
        if (result)
        {
            // any errors that don't format a reply go to done state, else
            // assume the request handler sent a specific reply already
            //
            if (client->state != httpSendReply)
            {
                client->state = httpDone;
            }
            else
            {
                result = 0;
            }
        }
        else
        {
            if (client->state != httpSendReply)
            {
                // sip wants to continue, so move to usermethod state
                // which will call our method handler function
                //
                client->state = httpUserMethod;
            }
            result = 0;
        }
        break;

    case httpUploadData:

        butil_log(5, "IPP upload cb (<-body)\n");
        *count = 0;
        *data = NULL;
        break;

    case httpComplete:

        butil_log(5, "IPP done cb\n");
        if (req)
        {
            result = ipp_req_destroy(ipp, req);
            client->ctxpriv = NULL;
        }
        break;

    default:

        return -1;
    }
    return 0;
}

int ipp_server(const char *program, uint16_t port, bool isTLS)
{
    http_server_t server;
    http_resource_t *resources = NULL;
    http_credentials_t creds;

    int result;
    int i;

    butil_set_log_level(5);

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
    // register a custom scheme
    result = butil_register_scheme("ipp", &s_ipp_scheme);
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
    // handle all HTTP scheme urls in callback
    //
    result = http_add_func_resource(&resources, schemeHTTP,  "*",
                NULL, ipp_resource_callback, &s_ipp_server);
    if (result)
    {
        BERROR("can't make resource");
        return result;
    }
    // set use-tls for certain ports
    //
    result = http_server_init(&server, resources, port, httpTCP, isTLS);
    if (result)
    {
        BERROR("can't start server");
        return result;
    }
    result = http_serve(&server);
    if (result)
    {
        butil_log(2, "server on port %u ends\n", server.port);
    }
    http_server_cleanup(&server);
    return result;
}
