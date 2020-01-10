/*
 * Copyright 2019 Brian Dodge
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    upnp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef BUPNP_H
#define BUPNP_H 1

#include "bnetheaders.h"

#include "butil.h"
#include "bstreamio.h"
#include "bhttpconfig.h"
#include "bhttp.h"

#define UPNP_MAX_CONCURRENT_CLIENTS (5)
#define UPNP_MAX_SOAP               (65536 * 2)
#define UPNP_MAX_SUBSCRIPTIONS      (16)

#define UPNP_MAX_URL HTTP_MAX_URL

#include "bupnpvar.h"
#include "bupnpservice.h"
#include "bupnpevent.h"
#include "bupnpsoap.h"

#define UPNP_ERROR BERROR

#define UPNP_HTTP_PORT          8080
#define UPNP_PORT               1900
#define UPNP_EVENT_PORT         51123

#define UPNP_MCAST_IPV4ADDR     "239.255.255.250"

#define UPNP_MAX_QUEUED_REPLIES (16)

typedef enum
{
    upnpServeInit,
    upnpClientInit,
    upnpServe,
    upnpDone
}
upnp_state_t;

typedef struct upnp_replyq
{
    char                reply_to_host[UPNP_MAX_URL];
    uint16_t            reply_to_port;
    time_t              when;
    char                st[UPNP_MAX_URL];
    char                usn[UPNP_MAX_URL];
    time_t              rate;
    struct upnp_replyq *next;
}
upnp_replyq_t;

typedef struct upnp_client
{
    struct upnp_client *next;
    uint32_t            id;
    iostream_t         *stream;
    socket_t            socket;
    upnp_state_t        state;
    upnp_state_t        prev_state;
    upnp_state_t        next_state;
    ioring_t            out;
    ioring_t            in;
}
upnp_client_t;

typedef struct upnp_server
{
    upnp_state_t        state;
    upnp_state_t        prev_state;
    upnp_state_t        next_state;
    uint16_t            port;
    bool                aborted;
    http_resource_t    *upnp_resources;     ///< Resources for UPnP HTTP server
    http_server_t       upnp_http_server;   ///< HTTP UDP server for UPnP
    http_resource_t    *http_resources;     ///< Resources for Document HTTP server
    http_server_t       doc_http_server;    ///< HTTP TCP server for serving clients
    http_client_t      *event_http_client;  ///< HTTP TCP Client for event notification
    http_resource_t    *event_resources;    ///< Resources for notifier client

    upnp_device_t      *root_device;

    upnp_replyq_t       q_pool[UPNP_MAX_QUEUED_REPLIES];
    upnp_replyq_t      *q_free;
    upnp_replyq_t      *replyq;

    upnp_subscription_t subs_pool[UPNP_MAX_SUBSCRIPTIONS];
    upnp_subscription_t *subs_free;
    upnp_subscription_t *subscriptions;

    char                search_header[UPNP_MAX_URL];
    char                man_header[UPNP_MAX_URL];
    uint32_t            mx_header;

    char                soap_header[UPNP_MAX_URL];
}
upnp_server_t;

extern http_method_t s_method_SUBSCRIBE;
extern http_method_t s_method_UNSUBSCRIBE;
extern http_method_t s_method_NOTIFY;

typedef int (*upnp_idle_callback_t)(void *priv);

int             upnp_server_init(
                        upnp_server_t    *server,
                        const size_t      max_request,
                        const uint16_t    port,
                        const uuid_t      root_udn,
                        const char       *description_url,
                        const char       *device_name,
                        const uint32_t    device_version,
                        const uint32_t    advertising_rate
                      );
void            upnp_server_cleanup(upnp_server_t *server);
int             upnp_server_abort(upnp_server_t *server);

int             upnp_server_slice(upnp_server_t *server, int to_secs, int to_usecs);
int             upnp_serve(upnp_server_t *servers, upnp_idle_callback_t on_idle, void *priv);

int             upnp_add_file_url(
                     upnp_server_t  *server,
                     const char     *path,
                     const mime_content_type_t type,
                     const uint8_t  *payload,
                     size_t          paylen
                );

int             upnp_add_text_url(
                     upnp_server_t    *server,
                     const char       *path,
                     const mime_content_type_t type,
                     const char       *value
                );

int             upnp_add_func_url(
                     upnp_server_t    *server,
                     const char       *path,
                     http_resource_function_t func,
                     void             *priv
                );

#endif

