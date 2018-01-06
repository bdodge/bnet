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

#define BERROR(m) \
    fprintf(stderr, "%s %d: " m "\n", __FUNCTION__, __LINE__)

#endif
