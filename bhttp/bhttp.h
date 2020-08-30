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
#ifndef BHTTP_H
#define BHTTP_H 1

#include "bnetheaders.h"
#include "bhttpconfig.h"

#include "butil.h"
#include "bstreamio.h"

#if HTTP_SUPPORT_WEBDAV
#include "bhttpdav.h"
#endif
#if HTTP_SUPPORT_WEBSOCKET
#include "bhttpws.h"
#endif
#if HTTP_SUPPORT_COMPRESSION
#include "zlib.h"
#endif

#define HTTP_ERROR BERROR

#define HTTP_SUPPORT_AUTH \
    (HTTP_SUPPORT_AUTH_BASIC | HTTP_SUPPORT_AUTH_DIGEST)

typedef enum
{
    httpTCP,
    httpUDP
}
http_transport_t;

typedef enum
{
    httpUnsupported,
    httpOptions,
    httpGet,
    httpHead,
    httpPost,
    httpPut,
    httpDelete,
    // dav
    httpTrace,
    httpConnect,
    httpPropFind,
    httpPropPatch,
    httpMkCol,
    httpCopy,
    httpMove,
    httpLock,
    httpUnlock,
    // user methods
    httpUser0,
    httpUser1,
    httpUser2,
    httpUser3,
    httpUser4,
    httpUser5,
    httpUser6,
    httpUser7,
    httpUser8,
    httpUser9,
    httpUser10,
    httpUser11,
    httpUser12,
    httpUser13,
    httpUser14,
    httpUser15
}
http_method_t;

#define HTTP_NUM_USER_METHODS (((int)httpUser15 - (int)httpUser0) + 1)
#define HTTP_FIRST_USER_METHOD ((int)httpUser0)

typedef enum
{
    httpAuthNone,
    httpAuthBasic,
    httpAuthBearer,
    httpAuthDigest,
    httpAuthMutual,
    httpAuthOAuth
}
http_auth_type_t;

#include "bhttputil.h"
#include "bhttpres.h"

typedef enum
{
    httpNone,
    httpLength,
    httpChunked,
}
http_transfer_t;

typedef enum
{
    httpDontCompress,
    httpCanCompress,
    httpWillCompress,
    httpCompressing,
    httpDidCompress
}
http_zip_state_t;

typedef enum
{
    httpServeInit,
    httpClientInit,
    httpReadline,
    httpReadRequest,
    httpReadReply,
    httpSendRequest,
    httpHeaders,
    httpMultipartHeaders,
    httpTLSsocketUpgrade,
    httpWebSocketUpgrade,
    httpHandleReadRequest,
    httpHandleSendRequest,
    httpReadChunkCount,
    httpBodyDownload,
    httpBodyUpload,
    httpSendReply,
    httpPropFindEnumerate,
    httpUserMethod,
    httpKeepAlive,
    httpDone
}
http_state_t;

typedef struct http_client
{
    struct http_client *next;
    uint32_t            id;
    bool                secure;
    http_transport_t    transport;
    iostream_t         *stream;
    socket_t            socket;
    bool                isclient;
    http_resource_t    *resources;
    http_state_t        state;
    http_state_t        prev_state;
    http_state_t        next_state;
    time_t              long_timeout;
    time_t              last_in_time;
    time_t              last_out_time;
    uint8_t             line[HTTP_MAX_LINE];
    int                 line_count;
    bool                reported_line_error;
    http_method_t       method;
    uint8_t             vmaj;
    uint8_t             vmin;
    uint16_t            response;
    bool                keepalive;
    bool                use100;
    butil_url_scheme_t  scheme;
    char                host[HTTP_MAX_HOSTNAME];
    uint16_t            port;
    char                path[HTTP_MAX_LINE];
    char                location[HTTP_MAX_LINE];
    uint32_t            redirects;
    #if HTTP_SUPPORT_MULTIPART
    char                boundary[HTTP_MAX_LINE];
    size_t              boundary_length;
    #endif
    #if HTTP_SUPPORT_AUTH
    bool                is_authorized;
    http_auth_type_t    auth_type;
    char                auth_creds[HTTP_MAX_AUTH_STRING];
    #endif
    #if HTTP_SUPPORT_WEBDAV
    char                dav_findpath[HTTP_MAX_PATH * 2];
    webdav_findstate_t  dav_find_state[HTTP_MAX_WEBDAV_DEPTH];
    char                dav_token[HTTP_MAX_LINE];
    bool                dav_no_root;
    int                 dav_depth_in;
    int                 dav_depth_at;
    #endif
    #if HTTP_SUPPORT_TLS
    bool                tls_upgrade;
    #endif
    #if HTTP_SUPPORT_COMPRESSION
    ioring_t            comprio;
    http_zip_state_t    compress;
    z_stream            zstrm;
    #endif
    #if HTTP_SUPPORT_WEBSOCKET
    bool                ws_upgrade;
    uint32_t            ws_version;
    uint32_t            ws_extensions;
    char                ws_key[HTTP_MAX_WEBSOCKET_KEY];
    char                ws_proto[HTTP_MAX_WEBSOCKET_PROTOCOL];
    iostream_t         *ws_stream;
    #endif
    http_resource_t    *resource;
    bool                resource_open;
    void               *ctxpriv;
    bool                expect100;
    http_transfer_t     in_transfer_type;
    size_t              in_content_length;
    mime_content_type_t in_content_type;
    size_t              in_transferred;
    http_transfer_t     out_transfer_type;
    size_t              out_content_length;
    mime_content_type_t out_content_type;
    size_t              out_gotten;
    size_t              out_transferred;
    #if HTTP_SUPPORT_UDP
    char                out_host[HTTP_MAX_HOSTNAME];
    uint16_t            out_port;
    #endif
    size_t              start_byte;
    size_t              end_byte;
    time_t              ifmodifiedsince;
    time_t              modifiedwhen;
    ioring_t            out;
    ioring_t            in;
}
http_client_t;

typedef struct http_server
{
    struct http_server  *next;
    http_resource_t     *resources;
    http_client_t       *clients;
    uint16_t             port;
    bool                 secure;
    bool                 aborted;
    http_transport_t     transport;
    uint32_t             connections;
    uint32_t             max_connections;
    socket_t             socket;
}
http_server_t;

typedef int (*http_idle_callback_t)(void *priv);

int http_send_data(http_client_t *client, const uint8_t *data, int count);
int http_send_out_data(http_client_t *client, http_state_t send_state, http_state_t next_state);

int http_begin_reply(http_client_t *client, uint16_t code, const char *message);
int http_begin_request(http_client_t *client);

#define http_append_request http_append_reply
int http_append_reply(http_client_t *client, const char *content, ...);
#define http_append_connection_to_request http_append_connection_to_reply
int http_append_connection_to_reply(http_client_t *client, bool forceclose);
#define http_quick_reply http_error_reply
int http_error_reply(http_client_t *client, uint16_t code, const char *msg, bool forceclose);

http_client_t *http_client_create(http_resource_t *resources, bool isclient);
void http_client_reinit(http_client_t *client, bool newstream);
void http_client_free(http_client_t *client);

int http_client_input(http_client_t *client, int to_secs, int to_usecs);
int http_client_slice(http_client_t *client);

int http_client_blocked(http_client_t *client);
int http_wait_for_client_event(http_client_t *client_list, int to_secs, int to_usecs);
int http_client_request(
                        http_client_t *client,
                        http_method_t method,
                        const char *url,
                        http_transport_t transport,
                        bool use100,
                        const char *localpath,
                        http_resource_t *resource
                       );

socket_t http_create_server_socket(http_transport_t transport, uint16_t port, uint32_t max_connections);

int http_server_init(
                        http_server_t *server,
                        http_resource_t *resources,
                        uint16_t port,
                        http_transport_t transport,
                        uint32_t max_connections,
                        bool secure
                      );
void http_server_cleanup(http_server_t *server);
int http_server_abort(http_server_t *server);

int http_wait_for_server_event(http_server_t *servers);
int http_server_slice(http_server_t *server, int to_secs, int to_usecs);
int http_serve(http_server_t *servers, http_idle_callback_t on_idle, void *priv);

#endif

