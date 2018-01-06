#ifndef HTTPCONFIG_H
#define HTTPCONFIG_H 1

///
// Maximium sizes of various URL components.  these are
// internal size limits, not absolute limits.  URLs have
// no actual limit in length beyond de-facto limits imposed
// by various internet components, about 2k bytes for some
// browsers.  These can be adjusted as needed for size
//
#define HTTP_MAX_SCHEME     8
#define HTTP_MAX_HOSTNAME   64
#define HTTP_MAX_PORTSPEC   5
#define HTTP_MAX_PATH       203
#define HTTP_MAX_URL        (   \
                            HTTP_MAX_SCHEME +       \
                            HTTP_MAX_HOSTNAME +     \
                            HTTP_MAX_PATH +         \
                            HTTP_MAX_PORTSPEC +     \
                            4   )

/// Longest basic-authentication user/password string accepted
//
#define HTTP_MAX_AUTH_STRING 64

/// How many clients, per server, can be connected
//
#define HTTP_MAX_CLIENT_CONNECTIONS 8

/// Max line in HTTP header
//
#define HTTP_MAX_LINE   256

/// Size of IO buffer (need at least one packet for udp)
//
#define HTTP_IO_SIZE    (1024*2)

/// Always assure these many bytes can fit in the IO buffer
//  (e.g. TLS block size)
//
#define HTTP_IO_MIN_ROOM    32

/// Timeout for I/O (seconds)
//
#define HTTP_LONG_TIMEOUT   50

/// Timeout for I/O (seconds)
//
#define HTTP_KEEPALIVE_TIMEOUT 5

/// Is HTTP on UDP supported (needed for SIP, for example)
//
#define HTTP_SUPPORT_UDP 1

/// Is TLS supported
//
#define HTTP_SUPPORT_TLS 1

/// Is BASIC authentication supported
//
#define HTTP_SUPPORT_AUTH_BASIC 1

/// Is DIGEST authentication supported (needs TLS)
//
#define HTTP_SUPPORT_AUTH_DIGEST 1

/// Is multipart form-data supported
//
#define HTTP_SUPPORT_MULTIPART 1

/// Support a larger set of mime types (more strings space)
//
#define HTTP_SUPPORT_EXTENDED_MIME_TYPES 1

/// Is byte-range supported
//
#define HTTP_SUPPORT_RANGES 1

/// Is DAV supported
//
#define HTTP_SUPPORT_WEBDAV 1

/// Is SIP supported
//
#define HTTP_SUPPORT_SIP 1

/// Is WebSockets supported (needs TLS)
//
#define HTTP_SUPPORT_WEBSOCKET  1

/// Maximum depth of directory traversal in DAV PROPFIND
/// Most DAV clients mine one level at a time so it
/// can work with just 1 here in most cases.  Each http
/// client will get a stack of find states this deep
/// and a find state is system dependent in size
//
#define HTTP_MAX_WEBDAV_DEPTH   16

/// permission mask used to create directories (webdav mkcol)
//
#define HTTP_WEBDAV_PERMISSION_MASK 0775

#define HTTP_FORCE_1_0 0

/// Path character for local file paths
//
#define HTTP_LOCAL_PATH_CHAR    '/'

/// Default file mime type
//
#define HTTP_DEFAULT_MIME       "text/html; charset=utf-8"
                                /*"application/octet-stream"*/

#endif
