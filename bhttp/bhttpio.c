#include "bhttp.h"

iostream_t *iostream_alloc(void)
{
    return (iostream_t *)malloc(sizeof(iostream_t));
}

void iostream_free(iostream_t *stream)
{
    free(stream);
}

static int iostream_file_read(iostream_t *stream, uint8_t *buf, int len)
{
    int fileno;

    if (! stream)
    {
        HTTP_ERROR("Bad stream");
        return -1;
    }
    fileno = (int)(intptr_t)stream->priv;
    if (fileno < 0)
    {
        HTTP_ERROR("Bad file");
        return -1;
    }
    return read(fileno, buf, len);
}

void iostream_normalize_ring(ioring_t *ring, uint8_t *temp)
{
    uint8_t mbuf[4];

    if (! ring)
    {
        return;
    }
    if (ring->tail == 0)
    {
        return;
    }
    if (ring->count == 0)
    {
        ring->tail = ring->head = 0;
        return;
    }
    if (ring->tail < ring->head)
    {
        memmove(ring->data, ring->data + ring->tail, ring->count);
        ring->tail = 0;
        ring->head = ring->count;
        return;
    }
    if (temp)
    {
        memcpy(temp, ring->data, ring->head);
        memmove(ring->data, ring->data + ring->tail, ring->size - ring->tail);
        memcpy(ring->data + ring->size - ring->tail, temp, ring->head);
        ring->tail = 0;
        ring->head = ring->count;
        return;
    }
    while (ring->tail >= 4)
    {
        mbuf[0] = ring->data[0];
        mbuf[1] = ring->data[1];
        mbuf[2] = ring->data[2];
        mbuf[3] = ring->data[3];
        memmove(ring->data, ring->data + 4, ring->size - 4);
        ring->data[ring->size - 4] = mbuf[0];
        ring->data[ring->size - 3] = mbuf[1];
        ring->data[ring->size - 2] = mbuf[2];
        ring->data[ring->size - 1] = mbuf[3];
        ring->tail -= 4;
        ring->head -= 4;
        if (ring->head < 0)
        {
            ring->head += ring->size;
        }
    }
    while (ring->tail > 0)
    {
        mbuf[0] = ring->data[0];
        memmove(ring->data, ring->data + 1, ring->size - 1);
        ring->data[ring->size - 1] = mbuf[0];
        ring->tail--;
        ring->head--;
        if (ring->head < 0)
        {
            ring->head += ring->size;
        }
    }
}

static int iostream_file_write(iostream_t *stream, uint8_t *buf, int len)
{
    int fileno;

    if (! stream)
    {
        HTTP_ERROR("Bad stream");
        return -1;
    }
    fileno = (int)(intptr_t)stream->priv;
    if (fileno < 0)
    {
        HTTP_ERROR("Bad file");
        return -1;
    }
    return write(fileno, buf, len);
}

int iostream_posix_poll(iostream_t *stream, polldir_t pollfor, int to_secs, int to_usecs)
{
    fd_set fds;
    int nfds;
    struct timeval timeout;
    int fileno;
    int result;

    if (! stream)
    {
        HTTP_ERROR("Bad stream");
        return -1;
    }
    fileno = (int)(intptr_t)stream->priv;
    if (fileno < 0)
    {
        HTTP_ERROR("Bad file");
        return -1;
    }
    FD_ZERO(&fds);
    FD_SET(fileno, &fds);
    nfds = fileno + 1;
    timeout.tv_sec = to_secs;
    timeout.tv_usec = to_usecs;

    result = select(
                    nfds,
                    (pollfor == readable) ? &fds : NULL,
                    (pollfor == writeable) ? &fds : NULL,
                    NULL,
                    (to_secs >= 0 && to_usecs >= 0) ? &timeout : NULL
                   );
    return result;
}

static int iostream_file_close(iostream_t *stream)
{
    int fileno;

    if (! stream)
    {
        HTTP_ERROR("Bad stream");
        return -1;
    }
    fileno = (int)(intptr_t)stream->priv;
    if (fileno < 0)
    {
        HTTP_ERROR("Bad file");
        return -1;
    }
    stream->priv = (void*)(intptr_t)-1;
    iostream_free(stream);
    close(fileno);
}

static int iostream_socket_read(iostream_t *stream, uint8_t *buf, int len)
{
    socket_t socket;

    if (! stream)
    {
        HTTP_ERROR("Bad stream");
        return -1;
    }
    socket = (socket_t)(intptr_t)stream->priv;
    if (socket < 0)
    {
        HTTP_ERROR("Bad socket");
        return -1;
    }
    return recv(socket, buf, len, 0);
}

static int iostream_socket_write(iostream_t *stream, uint8_t *buf, int len)
{
    socket_t socket;

    if (! stream)
    {
        HTTP_ERROR("Bad stream");
        return -1;
    }
    socket = (socket_t)(intptr_t)stream->priv;
    if (socket < 0)
    {
        HTTP_ERROR("Bad socket");
        return -1;
    }
    return send(socket, buf, len, 0);
}

static int iostream_socket_close(iostream_t *stream)
{
    socket_t socket;

    if (! stream)
    {
        HTTP_ERROR("Bad stream");
        return -1;
    }
    socket = (socket_t)(intptr_t)stream->priv;
    if (socket == INVALID_SOCKET)
    {
        HTTP_ERROR("Bad socket");
        return -1;
    }
    stream->priv = (void*)(intptr_t)INVALID_SOCKET;
    iostream_free(stream);
    close_socket(socket);
}

iostream_t *iostream_create_reader_from_file(const char *filename)
{
    iostream_t *stream;
    int fileno;

    fileno = open(filename, O_RDONLY
            #ifdef Windows
                          | O_BINARY
            #endif
                  ,0666
    );
    if (fileno < 0)
    {
        HTTP_ERROR("Can't open file");
        return NULL;
    }
    stream = iostream_alloc();
    if (! stream)
    {
        HTTP_ERROR("Can't alloc stream");
        return NULL;
    }
    stream->read    = iostream_file_read;
    stream->write   = iostream_file_write;
    stream->poll    = iostream_posix_poll;
    stream->close   = iostream_file_close;
    stream->priv    = (void*)(intptr_t)fileno;
    return stream;
}

iostream_t *iostream_create_writer_from_file(const char *filename)
{
    iostream_t *stream;
    int fileno;

    fileno = open(filename, O_WRONLY | O_CREAT | O_TRUNC
            #ifdef Windows
                          | O_BINARY
            #endif
            , 0644
    );
    if (fileno < 0)
    {
        HTTP_ERROR("Can't open file");
        return NULL;
    }
    stream = iostream_alloc();
    if (! stream)
    {
        HTTP_ERROR("Can't alloc stream");
        return NULL;
    }
    stream->read    = iostream_file_read;
    stream->write   = iostream_file_write;
    stream->poll    = iostream_posix_poll;
    stream->close   = iostream_file_close;
    stream->priv    = (void*)(intptr_t)fileno;
    return stream;
}

iostream_t *iostream_create_from_socket(socket_t socket)
{
    iostream_t *stream;

    stream = iostream_alloc();
    if (! stream)
    {
        HTTP_ERROR("Can't alloc stream");
        return NULL;
    }
    stream->read    = iostream_socket_read;
    stream->write   = iostream_socket_write;
    stream->poll    = iostream_posix_poll;
    stream->close   = iostream_socket_close;
    stream->priv    = (void*)(intptr_t)socket;
    return stream;
}

int iostream_socket_sendto(iostream_t *stream, uint8_t *buf, int len, const char *host, uint16_t port)
{
    socket_t socket;
    struct hostent *server;
    struct sockaddr_in server_addr;
    size_t addr_len;
    size_t i;
    bool isname;

    if (! stream)
    {
        HTTP_ERROR("Bad stream");
        return -1;
    }
    socket = (socket_t)(intptr_t)stream->priv;
    if (socket < 0)
    {
        HTTP_ERROR("Bad socket");
        return -1;
    }
    // if host is an IP address, use directly
    for (i = 0, isname = false; i < strlen(host); i++)
    {
        if ((host[i] < '0' || host[i] > '9') && host[i] != '.')
        {
            isname = true;
            break;
        }
    }
    addr_len = sizeof(server_addr);
    memset(&server_addr, 0, addr_len);

    if (! isname)
    {
        server = gethostbyname(host);
        if (server == NULL)
        {
            HTTP_ERROR("Can't find host");
            return -1;
        }
        memcpy((char *)&server_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    }
    else
    {
        if (! inet_aton(host, &server_addr.sin_addr))
        {
            HTTP_ERROR("Invalid address");
            return -1;
        }
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    return sendto(socket, buf, len, 0, (struct sockaddr *)&server_addr, addr_len);
}

iostream_t *iostream_create_from_tcp_connection(
                                                const char *hostname,
                                                uint16_t port
                                                )
{
}


