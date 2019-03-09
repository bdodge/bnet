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
#include "bsnmpio.h"
#include "bsnmpber.h"
#include "bsnmp.h"
#include "butil.h"
#include "bnetheaders.h"

int bsnmp_stream_init(bsnmp_server_t *server, uint16_t version, uint16_t port)
{
    socket_t sock;

    if (! server)
    {
        return -1;
    }
    server->port = port;
    server->version = version;

    server->in.size = SNMP_IO_SIZE;
    server->in.data = (uint8_t *)malloc(server->in.size);
    if (! server->in.data)
    {
        SNMP_ERROR("Can't alloc in data");
        return -1;
    }
    server->in.head = 0;
    server->in.tail = 0;
    server->in.count = 0;

    server->out.size = SNMP_IO_SIZE;
    server->out.data = (uint8_t *)malloc(server->out.size);
    if (! server->out.data)
    {
        SNMP_ERROR("Can't alloc out data");
        free(server->in.data);
        server->in.data = NULL;
        return -1;
    }
    server->out.head = 0;
    server->out.tail = 0;
    server->out.count = 0;

    sock = iostream_create_udp_socket();
    if (sock != INVALID_SOCKET)
    {
        int result;

        result = iostream_bind_socket(sock, port);
        if (! result)
        {
            server->stream = iostream_create_from_socket(sock);
            if (server->stream)
            {
                return 0;
            }
            SNMP_ERROR("Can't create stream");
        }
        else
        {
            SNMP_ERROR("Can't bind socket");
        }
        close_socket(sock);
    }
    else
    {
        server->stream = NULL;
    }
    return 0;
}

int bsnmp_next_byte(bsnmp_server_t *server, uint8_t *b)
{
    uint8_t rv;

    if (server->in.count == 0)
    {
        server->last_error = SNMP_ErrWrongLength;
        return -1;
    }
    server->in.count--;
    *b = server->in.data[server->in.tail];
    server->in.tail++;
    if (server->in.tail >= server->in.size)
    {
        server->in.tail = 0;
    }
    return 0;
}

int bsnmp_out_byte(bsnmp_server_t *server, const uint8_t b)
{
    if (server->out.count == server->out.size)
    {
        server->last_error = SNMP_ErrWrongLength;
        return -1;
    }
    server->out.count++;
    server->out.data[server->out.head] = b;
    server->out.head++;
    if (server->out.head >= server->out.size)
    {
        server->out.tail = 0;
    }
    return 0;
}

int bsnmp_stream_save_in_state(bsnmp_server_t *server, ioring_t *state)
{
    if (! server || ! server->stream)
    {
        return -1;
    }
    memcpy(state, &server->in, sizeof(ioring_t));
    return 0;
}

int bsnmp_stream_restore_in_state(bsnmp_server_t *server, ioring_t *state)
{
    if (! server || ! server->stream)
    {
        return -1;
    }
    memcpy(&server->in, state, sizeof(ioring_t));
    return 0;
}

int bsnmp_stream_save_out_state(bsnmp_server_t *server, ioring_t *state)
{
    if (! server || ! server->stream)
    {
        return -1;
    }
    memcpy(state, &server->out, sizeof(ioring_t));
    return 0;
}

int bsnmp_stream_restore_out_state(bsnmp_server_t *server, ioring_t *state)
{
    if (! server || ! server->stream)
    {
        return -1;
    }
    memcpy(&server->out, state, sizeof(ioring_t));
    return 0;
}

int bsnmp_stream_annotate_length(bsnmp_server_t *server, ioring_t *frompos, uint32_t bytes_reserved)
{
    uint32_t bytes_made;
    uint32_t lenbytes;
    int result;

    // frompos is the location where the length should be encoded
    bytes_made = server->out.head - frompos->head;
    bytes_made -= bytes_reserved;

    // move back to reserved spot and encode actual length there
    result = bsnmp_stream_restore_out_state(server, frompos);
    if (result)
    {
        return result;
    }
    result = bsnmp_ber_from_length(server, bytes_made, &lenbytes);
    if (result)
    {
        return result;
    }
    if (bytes_reserved > lenbytes)
    {
        memmove(&server->out.data[frompos->head + lenbytes],
                &server->out.data[frompos->head + bytes_reserved],
                bytes_made);
    }
    // move end of out to proper spot
    if (server->out.head + bytes_made > server->out.size)
    {
        SNMP_ERROR("Outring overflow");
        return -1;
    }
    // restore outpos where it was, minus any extra bytes
    server->out.head+= bytes_made;
    server->out.count+= bytes_made;
    return 0;
}

int bsnmp_stream_read(bsnmp_server_t *server, int tos, int tous)
{
    int isreadable;
    int room;
    int count;

    if (! server || ! server->stream)
    {
        return -1;
    }
    if (server->in.count >= server->in.size)
    {
        return 0;
    }
    isreadable = server->stream->poll(server->stream, readable, tos, tous);
    if (isreadable > 0)
    {
        if (server->in.tail > server->in.head)
        {
            room = server->in.tail - server->in.head;
        }
        else
        {
            room = server->in.size - server->in.head;
        }
        #if 1
        socket_t fd = (socket_t)(intptr_t)server->stream->priv;
        socklen_t srclen = sizeof(server->srcaddr);
        count = recvfrom(fd, server->in.data + server->in.head, room, 0, (struct sockaddr *)&server->srcaddr, &srclen);
        #else
        count = server->stream->read(server->stream, server->in.data + server->in.head, room);
        #endif
        if (count <= 0)
        {
            SNMP_ERROR("Read failure");
            return count;
        }
        server->in.head += count;
        if (server->in.head >= server->in.size)
        {
            server->in.head = 0;
        }
        server->in.count += count;

        butil_log(7, "Read %u on snmp\n", count);
    }
    else if (isreadable < 0)
    {
        return isreadable;
    }
    return 0;
}

int bsnmp_stream_write(bsnmp_server_t *server, int tos, int tous)
{
    int iswriteable;
    int chunk;
    int count;

    if (! server || ! server->stream)
    {
        return -1;
    }
    if (server->out.count == 0)
    {
        return 0;
    }
    iswriteable = server->stream->poll(server->stream, writeable, tos, tous);
    if (iswriteable > 0)
    {
        if (server->out.tail >= server->out.head)
        {
            chunk = server->out.size - server->out.tail;
        }
        else
        {
            chunk = server->out.head - server->out.tail;
        }
        #if 1
        socket_t fd = (socket_t)(intptr_t)server->stream->priv;
        int dstlen = sizeof(server->srcaddr);
        count = sendto(fd, server->out.data + server->out.tail, chunk, 0, (struct sockaddr *)&server->srcaddr, dstlen);
        #else
        count = server->stream->write(server->stream, server->out.data + server->out.tail, chunk);
        #endif
        if (count <= 0)
        {
            SNMP_ERROR("Write failure");
            return count;
        }
        server->out.tail += count;
        if (server->out.tail >= server->out.size)
        {
            server->out.tail = 0;
        }
        server->out.count -= count;

        butil_log(1, "Wrote %u on snmp\n", count);
    }
    else if (iswriteable < 0)
    {
        return iswriteable;
    }
    return 0;
}


