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
#ifndef HTTPCONFIG_H
#define HTTPCONFIG_H 1

///
// Maximium sizes of various URL components.  these are
// internal size limits, not absolute limits.  URLs have
// no actual limit in length beyond de-facto limits imposed
// by various internet components, about 2k bytes for some
// browsers.  These can be adjusted as needed for size
//
#ifndef HTTP_MAX_SCHEME
#define HTTP_MAX_SCHEME     (8)
#endif
#ifndef HTTP_MAX_HOSTNAME
#define HTTP_MAX_HOSTNAME   (64)
#endif
#ifndef HTTP_MAX_PORTSPEC
#define HTTP_MAX_PORTSPEC   (5)
#endif
#ifndef HTTP_MAX_PATH
#define HTTP_MAX_PATH       (256)
#endif

#define HTTP_MAX_URL        (   \
                            HTTP_MAX_SCHEME +       \
                            HTTP_MAX_HOSTNAME +     \
                            HTTP_MAX_PATH +         \
                            HTTP_MAX_PORTSPEC +     \
                            4   )

/// Longest user method string
//
#ifndef HTTP_MAX_METHOD_NAME
#define HTTP_MAX_METHOD_NAME  (16)
#endif
/// Longest basic-authentication user/password string accepted
//
#ifndef HTTP_MAX_AUTH_STRING
#define HTTP_MAX_AUTH_STRING  (64)
#endif

/// How many clients, per server, can be connected
//
#ifndef HTTP_MAX_CLIENT_CONNECTIONS
#define HTTP_MAX_CLIENT_CONNECTIONS (8)
#endif

/// Max line in HTTP header
//
#ifndef HTTP_MAX_LINE
#define HTTP_MAX_LINE   HTTP_MAX_URL
#endif

/// Size of IO buffer (need at least one packet for udp)
//
#ifndef HTTP_IO_SIZE
#define HTTP_IO_SIZE    (1436*2)
#endif

/// Always assure these many bytes can fit in the IO buffer
//  (e.g. TLS block size)
//
#ifndef HTTP_IO_MIN_ROOM
#define HTTP_IO_MIN_ROOM    (32)
#endif

/// Timeout for I/O (seconds)
//
#ifndef HTTP_LONG_TIMEOUT
#define HTTP_LONG_TIMEOUT   (50)
#endif

/// Timeout for I/O (seconds)
//
#ifndef HTTP_KEEPALIVE_TIMEOUT
#define HTTP_KEEPALIVE_TIMEOUT (5)
#endif

/// Is HTTP on UDP supported (needed for SIP, for example)
//
#ifndef HTTP_SUPPORT_UDP
#define HTTP_SUPPORT_UDP (0)
#endif

/// Is TLS supported
//
#ifndef HTTP_SUPPORT_TLS
#define HTTP_SUPPORT_TLS (1)
#endif

/// Is BASIC authentication supported
//
#ifndef HTTP_SUPPORT_AUTH_BASIC
#define HTTP_SUPPORT_AUTH_BASIC (1)
#endif

/// Is DIGEST authentication supported (needs TLS)
//
#ifndef HTTP_SUPPORT_AUTH_DIGEST
#define HTTP_SUPPORT_AUTH_DIGEST (1)
#endif

/// Is multipart form-data supported
//
#ifndef HTTP_SUPPORT_MULTIPART
#define HTTP_SUPPORT_MULTIPART (1)
#endif

/// Support a larger set of mime types (more strings space)
//
#ifndef HTTP_SUPPORT_EXTENDED_MIME_TYPES
#define HTTP_SUPPORT_EXTENDED_MIME_TYPES (1)
#endif

/// Is byte-range supported
//
#ifndef HTTP_SUPPORT_RANGES
#define HTTP_SUPPORT_RANGES (1)
#endif

/// Is DAV supported
//
#ifndef HTTP_SUPPORT_WEBDAV
#define HTTP_SUPPORT_WEBDAV (1)
#endif

/// Is WebSockets supported (needs TLS)
//
#ifndef HTTP_SUPPORT_WEBSOCKET
#define HTTP_SUPPORT_WEBSOCKET  HTTP_SUPPORT_TLS
#endif

/// Maximum depth of directory traversal in DAV PROPFIND
/// Most DAV clients mine one level at a time so it
/// can work with just 1 here in most cases.  Each http
/// client will get a stack of find states this deep
/// and a find state is system dependent in size
//
#ifndef HTTP_MAX_WEBDAV_DEPTH
#define HTTP_MAX_WEBDAV_DEPTH   (16)
#endif

/// permission mask used to create directories (webdav mkcol)
//
#ifndef HTTP_WEBDAV_PERMISSION_MASK
#define HTTP_WEBDAV_PERMISSION_MASK (0775)
#endif

/// Force HTTP 1.0 semantics
//
#ifndef HTTP_FORCE_1_0
#define HTTP_FORCE_1_0 (0)
#endif

/// Path character for local file paths
//
#ifndef HTTP_LOCAL_PATH_CHAR
#define HTTP_LOCAL_PATH_CHAR    '/'
#endif

/// Default file mime type
//
#ifndef HTTP_DEFAULT_MIME
#define HTTP_DEFAULT_MIME       "text/html; charset=utf-8"
                                /*"application/octet-stream"*/
#endif

#endif
