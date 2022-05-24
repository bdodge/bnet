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
#ifndef BNETHEADERS_H
#define BNETHEADERS_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#ifdef Windows
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#define HTTP_SUPPORT_POSIX (1)
#include <windows.h>
#include <winsock.h>
#elif defined(EMBEDDED_ATCMD)
#define HTTP_SUPPORT_POSIX (0)
#include <errno.h>
#include <strings.h>
#include <net/net_ip.h>
#include "attcp.h"
#ifdef CONFIG_MBEDTLS
#define HTTP_SUPPORT_TLS (1)
#else
#define HTTP_SUPPORT_TLS (0)
#define HTTP_SUPPORT_WEBSOCKET (1)
#endif
//typedef uint32_t time_t;
#else
#include <unistd.h>
#include <sys/types.h>
#define HTTP_SUPPORT_POSIX (1)
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <signal.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

#if (HTTP_SUPPORT_POSIX)
#ifdef _WIN32
typedef SOCKET socket_t;
#define close_socket closesocket
#define ioctl_socket ioctlsocket
#else
typedef int socket_t;
#define INVALID_SOCKET -1
#define close_socket close
#define ioctl_socket ioctl
#define http_time(p) time(p)
#endif
#elif defined(EMBEDDED_ATCMD)
typedef attcp_socket_t socket_t;
#define close_socket(s) ATTCPclose(s)
#define http_time(p) ATTCPwallTime(p)
#else
#endif

typedef struct tag_ipv4addr { uint32_t addr;    } bipv4addr_t;
typedef struct tag_ipv6addr { uint16_t addr[8]; } bipv6addr_t;

// format strings for int64 types
#if defined(__IA64__) || defined(_WIN64) || defined(_M_IA64) || \
        defined(_M_X64) || defined(__x86_64__) || defined(X86_64) || defined(AMD64)
    // 64 bit CPU, ptrs and longs are 8 bytes, ints are 4
#include <inttypes.h>
#define LIFS    PRId64
#define LUFS    PRIu64
#else
#define LIFS    "lld"
#define LUFS    "llu"
#endif

#define BERROR(m) \
    fprintf(stderr, "%s %d: " m "\n", __FUNCTION__, __LINE__)

#define UUID_LENGTH (16)

typedef uint8_t buuid_t[UUID_LENGTH];

#define uuid_clear(a) memset((a), 0, UUID_LENGTH)
#define uuid_compare(a, b) memcmp((a), (b), UUID_LENGTH)
#define uuid_copy(dst, src) memcpy((dst), (src), UUID_LENGTH)

#define uuid_format(a, dst, ndst) \
 snprintf(dst, ndst, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",  \
   a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],a[10],a[11],a[12],a[13],a[14],a[15])

#endif
