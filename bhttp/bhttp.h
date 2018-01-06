#ifndef HTTP_H
#define HTTP_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#ifdef Windows
#include <windows.h>
#include <winsock.h>
#else
#include <errno.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <signal.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

#ifdef _WIN32
typedef SOCKET socket_t;
#define close_socket closesocket
#define ioctl_socket ioctlsocket
#else
typedef int socket_t;
#define INVALID_SOCKET -1
#define close_socket close
#define ioctl_socket ioctl
#endif

#include "bhttpconfig.h"
#include "bhttpio.h"
#include "bhttptls.h"
#include "bhttpdav.h"
#include "bhttpws.h"
#include "bhttpsip.h"


#define HTTP_ERROR(m)  \
    fprintf(stderr, "%s %d: " m "\n", __FUNCTION__, __LINE__)

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
    httpHTTP,
    httpSIP,
    httpHTTPS,
    httpSIPS,
}
http_scheme_t;

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
    // sip
    httpInvite,
    httpAck,
    httpPrack,
    httpCancel,
    httpUpdate,
    httpInfo,
    httpSubscribe,
    httpNotify,
    httpRefer,
    httpMessage,
    httpRegister,
    httpBye,
}
http_method_t;

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

typedef enum
{
    htmtbin,
    htmtcss,
    htmtgif,
    htmthtml,
    htmtico,
    htmtjpeg,
    htmtjar,
    htmtjs,
    htmtjson,
    htmttext,
    htmttiff,
    htmtxml,
#if HTTP_SUPPORT_MULTIPART
    htmtmulti,
#endif
#if HTTP_SUPPORT_EXTENDED_MIME_TYPES
    htmtaac,
    htmtabw,
    htmtarc,
    htmtavi,
    htmtazw,
    htmtbz,
    htmtbz2,
    htmtcsh,
    htmtcsv,
    htmtdoc,
    htmteot,
    htmtepub,
    htmtics,
    htmtmidi,
    htmtmpeg,
    htmtmpkg,
    htmtodp,
    htmtods,
    htmtodt,
    htmtoga,
    htmtogv,
    htmtogx,
    htmtotf,
    htmtpng,
    htmtpdf,
    htmtppt,
    htmtrar,
    htmtrtf,
    htmtsh,
    htmtsvg,
    htmtswf,
    htmttar,
    htmtts,
    htmtttf,
    htmtvsd,
    htmtwav,
    htmtweba,
    htmtwebm,
    htmtwebp,
    htmtwoff,
    htmtwoff2,
    htmtxhtml,
    htmtxls,
    htmtxlsx,
    htmtxul,
    htmtzip,
    htmt3gp,
    htmt3g2,
    htmt7z,
#endif
}
http_content_type_t;

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
    httpServeInit,
    httpClientInit,
    httpReadline,
    httpReadRequest,
    httpReadReply,
    httpSendRequest,
    httpHeaders,
    httpMultipartHeaders,
    httpWebSocketUpgrade,
    httpHandleReadRequest,
    httpHandleSendRequest,
    httpReadChunkCount,
    httpBodyDownload,
    httpBodyUpload,
    httpSendReply,
    httpPropFindEnumerate,
    httpSipSlice,
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
    http_scheme_t       scheme;
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
    #if HTTP_SUPPORT_WEBSOCKET
    bool                ws_upgrade;
    uint32_t            ws_version;
    uint32_t            ws_extensions;
    char                ws_key[HTTP_MAX_WEBSOCKET_KEY];
    char                ws_proto[HTTP_MAX_WEBSOCKET_PROTOCOL];
    iostream_t         *ws_stream;
    #endif
    #if HTTP_SUPPORT_SIP
    char                sip_via[HTTP_MAX_SIP_VIA];
    char                sip_from[HTTP_MAX_SIP_ADDR];
    char                sip_to[HTTP_MAX_SIP_ADDR];
    char                sip_contact[HTTP_MAX_SIP_ADDR];
    char                sip_callid[HTTP_MAX_SIP_ADDR];
    int                 sip_depth;
    uint32_t            sip_cseq;
    #endif
    http_resource_t    *resource;
    bool                resource_open;
    void               *ctxpriv;
    bool                expect100;
    http_transfer_t     in_transfer_type;
    size_t              in_content_length;
    http_content_type_t in_content_type;
    size_t              in_transferred;
    http_transfer_t     out_transfer_type;
    size_t              out_content_length;
    http_content_type_t out_content_type;
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
    struct http_server *next;
    http_resource_t    *resources;
    http_client_t      *clients;
    uint16_t            port;
    bool                secure;
    http_transport_t    transport;
    uint32_t            connections;
    socket_t            socket;
}
http_server_t;

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

int http_server_init(
                        http_server_t *server,
                        http_resource_t *resources,
                        uint16_t port,
                        http_transport_t transport,
                        bool secure
                      );
void http_server_cleanup(http_server_t *server);

int http_server_slice(http_server_t *server, int to_secs, int to_usecs);
int http_serve(http_server_t *servers);

#endif

