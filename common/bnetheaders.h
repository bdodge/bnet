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

// format strings for int64 types
#if defined(__IA64__) || defined(_WIN64) || defined(_M_IA64) || \
        defined(_M_X64) || defined(__x86_64__) || defined(X86_64) || defined(AMD64)
    // 64 bit CPU, ptrs and longs are 8 bytes, ints are 4
#include <inttypes.h>
#define LIFS    PRId64
#define LUFS    PRIu64
#else
#define LIFS    "ld"
#define LUFS    "lu"
#endif

#define BERROR(m) \
    fprintf(stderr, "%s %d: " m "\n", __FUNCTION__, __LINE__)

#endif
