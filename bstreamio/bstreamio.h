#ifndef HTTPIO_H
#define HTTPIO_H 1

typedef struct ioring
{
    int     head, tail, count, size;
    uint8_t *data;
}
ioring_t;

struct iostream;

typedef enum { readable, writeable } polldir_t;

typedef int (*ioreadf)  (struct iostream *, uint8_t *, int);
typedef int (*iowritef) (struct iostream *, uint8_t *, int);
typedef int (*iopollf)  (
                            struct iostream *,
                            polldir_t pollfor,
                            int to_secs,
                            int to_usecs
                        );
typedef int (*ioclosef) (struct iostream *);

typedef struct iostream
{
    ioreadf     read;
    iowritef    write;
    iopollf     poll;
    ioclosef    close;
    void       *priv;
}
iostream_t;

void iostream_normalize_ring(ioring_t *ring, uint8_t *temp);

iostream_t *iostream_alloc(void);
void iostream_free(iostream_t *stream);
int iostream_posix_poll(iostream_t *stream, polldir_t pollfor, int to_secs, int to_usecs);
iostream_t *iostream_create_reader_from_file(const char *filename);
iostream_t *iostream_create_writer_from_file(const char *filename);

int iostream_socket_sendto(iostream_t *stream, uint8_t *buf, int len, const char *host, uint16_t port);

socket_t iostream_create_tcp_socket(void);
socket_t iostream_create_udp_socket(void);

iostream_t *iostream_create_from_socket(socket_t socket);
iostream_t *iostream_create_from_tcp_connection(
                                            const char *hostname,
                                            uint16_t port
                                              );

int iostream_sha1_hash(uint8_t *result, uint8_t *source, size_t bytes);

iostream_t *iostream_tls_create_from_iostream(iostream_t *stream, bool isclient);

int iostream_tls_prolog(void);
int iostream_tls_epilog(void);

#endif

