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
#include "bhttp.h"

#if HTTP_SUPPORT_WEBSOCKET

// first byte
#define WSBIT_FIN   0x80
#define WSBIT_OPC   0x0F

// second byte
#define WSBIT_MASK  0x80
#define WSBIT_PAYL  0x7F

#define WSOPCODE_CONT   0x00
#define WSOPCODE_TEXT   0x01
#define WSOPCODE_DATA   0x02
#define WSOPCODE_CLOSE  0x08
#define WSOPCODE_PING   0x09
#define WSOPCODE_PONG   0x0A

static const char *ws_state_name(http_ws_state_t state)
{
    switch (state)
    {
    case wssFrame:          return "Frame";
    case wssHeaderByte1:    return "Header";
    case wssPayloadLen1:    return "2-byte Payload Len";
    case wssPayloadLen2:    return "8-byte Payload Len";
    case wssMaskingKey:     return "Masking Key";
    case wssPayload:        return "Payload";
    default:                return "???";
    }
}

static uint8_t ws_next_client_byte(http_client_t *client)
{
    uint8_t byte = client->in.data[client->in.tail];

    client->in.count--;
    client->in.tail++;
    if (client->in.tail >= client->in.size)
    {
        client->in.tail = 0;
    }
    return byte;
}

static int ws_stream_inject_input(http_client_t *client)
{
    ws_stream_ctx_t *wsx;
    uint64_t chunk, room;
    uint32_t count;
    uint8_t byte;
    int i;
    int result;

    if (! client || ! client->ws_stream || ! client->ws_stream->priv)
    {
        return -1;
    }
    wsx = (ws_stream_ctx_t *)client->ws_stream->priv;

    if (wsx->in_state != wsx->prev_in_state)
    {
        http_log(5, "WS state %s\n", ws_state_name(wsx->in_state));
    }
    wsx->prev_in_state = wsx->in_state;

    if (client->out.count > 0)
    {
        // write output
        result = http_send_out_data(client, client->state, client->state);
        if (result)
        {
            return result;
        }
    }
    if (client->in.count == 0)
    {
        // nothing to do
        return 0;
    }
    switch (wsx->in_state)
    {
    case wssFrame:
        wsx->opcode = ws_next_client_byte(client);
        wsx->in_state = wssHeaderByte1;
        break;

    case wssHeaderByte1:
        byte = ws_next_client_byte(client);
        wsx->in_masked = (byte & WSBIT_MASK) ? true : false;
        wsx->payload_length = byte & WSBIT_PAYL;
        if (wsx->payload_length == 126)
        {
            wsx->in_state = wssPayloadLen1;
        }
        else if (wsx->payload_length == 127)
        {
            wsx->in_state = wssPayloadLen2;
        }
        else
        {
            wsx->in_state = wssMaskingKey;
        }
        break;

    case wssPayloadLen1:
        if (client->in.count < 2)
        {
            return 0;
        }
        byte = ws_next_client_byte(client);
        wsx->payload_length = (uint64_t)byte;
        wsx->payload_length <<= 8;
        byte = ws_next_client_byte(client);
        wsx->payload_length |= (uint64_t)byte;
        wsx->in_state = wssMaskingKey;
        break;

    case wssPayloadLen2:
        if (client->in.count < 8)
        {
            return 0;
        }
        byte = ws_next_client_byte(client);
        wsx->payload_length = (uint64_t)byte;
        wsx->payload_length <<= 8;
        byte = ws_next_client_byte(client);
        wsx->payload_length |= (uint64_t)byte;
        wsx->payload_length <<= 8;
        byte = ws_next_client_byte(client);
        wsx->payload_length |= (uint64_t)byte;
        wsx->payload_length <<= 8;
        byte = ws_next_client_byte(client);
        wsx->payload_length |= (uint64_t)byte;
        wsx->payload_length <<= 8;
        byte = ws_next_client_byte(client);
        wsx->payload_length |= (uint64_t)byte;
        wsx->payload_length <<= 8;
        byte = ws_next_client_byte(client);
        wsx->payload_length |= (uint64_t)byte;
        wsx->payload_length <<= 8;
        byte = ws_next_client_byte(client);
        wsx->payload_length |= (uint64_t)byte;
        wsx->payload_length <<= 8;
        byte = ws_next_client_byte(client);
        wsx->payload_length |= (uint64_t)byte;
        wsx->in_state = wssMaskingKey;
        break;

    case wssMaskingKey:
        if (wsx->in_masked)
        {
            if (client->in.count < 4)
            {
                return 0;
            }
            wsx->in_mask_key[0] = ws_next_client_byte(client);
            wsx->in_mask_key[1] = ws_next_client_byte(client);
            wsx->in_mask_key[2] = ws_next_client_byte(client);
            wsx->in_mask_key[3] = ws_next_client_byte(client);
        }
        http_log(5, "Frame code=%02X fin=%c len=%llu\n",
                wsx->opcode & WSBIT_OPC,
                (wsx->opcode & WSBIT_FIN) ? 'Y' : 'N',
                wsx->payload_length);

        switch (wsx->opcode & WSBIT_OPC)
        {
        case WSOPCODE_CONT:
        case WSOPCODE_TEXT:
        case WSOPCODE_DATA:
            wsx->in_state = wssPayload;
            break;
        case WSOPCODE_CLOSE:
            http_log(4, "WebSocket Close request\n");
            client->ws_stream->close(client->ws_stream);
            client->ws_stream = NULL;
            client->stream = NULL;
            client->state = httpDone;
            break;
        case WSOPCODE_PING:
        case WSOPCODE_PONG:
        default:
            HTTP_ERROR("Bad opcode");
            return -1;
        }
        break;

    case wssPayload:
        // rotate input buffer to insure contiguity
        iostream_normalize_ring(&client->in, NULL);

        // rotate ws stream buffer to insure contiguity
        iostream_normalize_ring(&wsx->in, NULL);

        // read payload data
        chunk = wsx->payload_length;
        if (chunk > (uint64_t)client->in.count)
        {
            chunk = (uint64_t)client->in.count;
        }
        // into our stream buffer
        room = (uint64_t)(wsx->in.size - wsx->in.count);
        if (room < chunk)
        {
            chunk = room; // chunk is now 32 bit safe
        }
        count = (uint32_t)chunk;

        if (wsx->in_masked)
        {
            // unmask bytes
            for (i = 0; i < count; i++)
            {
                wsx->in.data[wsx->in.head + i] =
                    client->in.data[client->in.tail + i] ^ wsx->in_mask_key[i & 0x3];
            }
        }
        else
        {
            for (i = 0; i < count; i++)
            {
                wsx->in.data[wsx->in.head + i] = client->in.data[client->in.tail + i];
            }
        }
        wsx->in.count += count;
        wsx->in.head += count;
        if (wsx->in.head >= wsx->in.size)
        {
            wsx->in.head = 0;
        }
        client->in.count -= count;
        client->in.tail += count;
        if (client->in.tail >= client->in.size)
        {
            client->in.tail = 0;
        }
        if (1) // debug
        {
            if (wsx->in.head < wsx->in.size - 1)
                wsx->in.data[wsx->in.head] = '\n';
            if (wsx->in.head < wsx->in.size)
                wsx->in.data[wsx->in.head + 1] = 0;

            if (0) // more debug
            {
                http_log(5, "%s", wsx->in.data + wsx->in.tail);

                wsx->in.tail = wsx->in.head;
                wsx->in.count = 0;
            }
        }
        wsx->payload_length -= (uint64_t)count;
        if (wsx->payload_length == 0)
        {
            wsx->in_state = wssFrame;
        }
        break;

    default:
        HTTP_ERROR("Bad State");
        return -1;
    }
    return 0;
}

static int ws_stream_read(iostream_t *stream, uint8_t *buffer, int count)
{
    ws_stream_ctx_t *wsx;
    int chunk;

    if (! stream || ! stream->priv || ! buffer)
    {
        return -1;
    }
    wsx = (ws_stream_ctx_t *)stream->priv;

    if (wsx->in.count == 0)
    {
        return 0;
    }
    if (wsx->in.tail < wsx->in.head)
    {
        chunk = wsx->in.head - wsx->in.tail;
    }
    else
    {
        chunk = wsx->in.size - wsx->in.tail;
    }
    if (chunk > count)
    {
        chunk = count;
    }
    if (chunk > 0)
    {
        memcpy(buffer, wsx->in.data + wsx->in.tail, chunk);
        wsx->in.count -= chunk;
        wsx->in.tail += chunk;
        if (wsx->in.tail >= wsx->in.count)
        {
            wsx->in.tail = 0;
        }
    }
    return chunk;
}

static int ws_stream_write(iostream_t *stream, uint8_t *buffer, int count)
{
    ws_stream_ctx_t *wsx;
    int room;

    if (! stream || ! stream->priv || ! buffer)
    {
        return -1;
    }
    wsx = (ws_stream_ctx_t *)stream->priv;

    if (wsx->in.count < wsx->in.size)
    {
        if (wsx->in.head < wsx->in.tail)
        {
            room = wsx->in.tail - wsx->in.head;
        }
        else
        {
            room = wsx->in.size - wsx->in.head;
        }
    }
    else
    {
        room = 0;
    }
    if (room < count)
    {
        count = room;
    }
    if (count > 0)
    {
        memcpy(wsx->in.data + wsx->in.head, buffer, count);
        wsx->in.count += count;
        wsx->in.head += count;
        if (wsx->in.head >= wsx->in.size)
        {
            wsx->in.head = 0;
        }
    }
    return count;
}

static int ws_stream_poll(iostream_t *stream, polldir_t dir, int to_secs, int to_usecs)
{
    ws_stream_ctx_t *wsx;
    int result;

    if (! stream || ! stream->priv)
    {
        return -1;
    }
    wsx = (ws_stream_ctx_t *)stream->priv;

    if (dir == readable)
    {
        return (wsx->in.count > 0) ? 1 : 0;
    }
    else
    {
        return 1;
    }
}

static int ws_stream_close(iostream_t *stream)
{
    ws_stream_ctx_t *wsx;

    if (! stream || ! stream->priv)
    {
        return -1;
    }
    wsx = (ws_stream_ctx_t *)stream->priv;

    if (wsx->stream)
    {
        // if we wrapped it, close it
        //wsx->stream->close(wsx->stream);
        wsx->stream = NULL;
    }
    if (wsx->in.data)
    {
        free(wsx->in.data);
        wsx->in.data = NULL;
    }
    free(wsx);
    free(stream);
    return 0;
}

static iostream_t *iostream_websocket_create_from_stream(iostream_t *instream)
{
    iostream_t *stream;
    ws_stream_ctx_t *wsx;

    stream = (iostream_t *)malloc(sizeof(iostream_t));
    if (! stream)
    {
        HTTP_ERROR("No mem for stream");
        return NULL;
    }
    wsx = (ws_stream_ctx_t *)malloc(sizeof(ws_stream_ctx_t));
    if (! wsx)
    {
        HTTP_ERROR("No mem for stream ctx");
        free(stream);
        return NULL;
    }
    wsx->in.data = (uint8_t *)malloc(HTTP_IO_SIZE);
    if (! wsx->in.data)
    {
        HTTP_ERROR("No mem for stream in");
        free(stream);
        free(wsx);
        return NULL;
    }
    wsx->in.tail = 0;
    wsx->in.head = 0;
    wsx->in.count = 0;
    wsx->in.size = HTTP_IO_SIZE;

    wsx->stream     = instream;
    wsx->in_state   = wssFrame;
    wsx->out_state  = wssFrame;
    wsx->prev_in_state  = wssPayload;
    wsx->prev_out_state = wssPayload;

    // defaults. owner can override in request callback
    wsx->out_fmt  = WSOPCODE_DATA;
    wsx->out_masked = false;

    stream->read  = ws_stream_read;
    stream->write = ws_stream_write;
    stream->poll  = ws_stream_poll;
    stream->close = ws_stream_close;
    stream->priv = (void*)wsx;

    return stream;
}

int http_websocket_set_format(struct http_client *client, http_ws_format_t fmt, bool ismasked, char *mask)
{
    ws_stream_ctx_t *wsx;

    if (! client || ! client->ws_stream || ! client->ws_stream->priv)
    {
        return -1;
    }
    wsx = (ws_stream_ctx_t *)client->ws_stream->priv;
    wsx->out_fmt = (fmt == wsdfText) ? WSOPCODE_TEXT : WSOPCODE_DATA;
    wsx->out_masked = ismasked;
    if (ismasked)
    {
        memcpy(wsx->out_mask_key, mask, 4);
    }
    return 0;
}

int http_websocket_check_key(char *theirkey, char *ourkey)
{
    if (! theirkey || ! ourkey)
    {
        return -1;
    }
    // todo - validate
    return 0;
}

int http_websocket_create_key(char *keybuf, size_t bufbytes)
{
    uint8_t b[8];
    int i;

    if (! keybuf || bufbytes < HTTP_MAX_WEBSOCKET_KEY)
    {
        return -1;
    }
    for (i = 0; i < 8; i++)
    {
        b[i] = i + 1;
    }
    butil_base64_encode(keybuf, bufbytes, b, i, false, false);
    return 0;
}

int http_websocket_slice(struct http_client *client)
{
    ws_stream_ctx_t *wsx;
    uint8_t *pbackdata;
    uint8_t byte;
    size_t taken;
    size_t avail;
    int result;

    if (! client || ! client->ws_stream || ! client->ws_stream->priv)
    {
        return -1;
    }
    wsx = (ws_stream_ctx_t *)client->ws_stream->priv;

    // pull in any stream data on input
    //
    result = http_client_input(client, 0, 10000);
    if (result < 0)
    {
        http_log(3, "in stream ends websocket\n");
        client->ws_stream->close(client->ws_stream);
        client->ws_stream = NULL;

        client->state = httpDone;
        return 0;
    }
    // decode the protocol and buffer any payload
    //
    result = ws_stream_inject_input(client);

    // present any input data on stream to client resource
    //
    iostream_normalize_ring(&wsx->in, NULL);
    avail = wsx->in.count;

    if (! client->resource || ! client->resource->callback)
    {
        // no resource, drop data
        //
        wsx->in.count = 0;
        return 0;
    }
    if (wsx->in.count > 0)
    {
        taken = wsx->in.count;

        result = client->resource->callback(client,
                client->resource, httpDownloadData, &wsx->in.data, &taken);
        if (result < 0)
        {
            HTTP_ERROR("Callback cancels");
            return -1;
        }
        if (taken > 0)
        {
            wsx->in.count -= taken;
            wsx->in.tail += taken;
            if (wsx->in.tail >= wsx->in.size)
            {
                wsx->in.tail = 0;
            }
        }
    }
    // get any output data from client resource and put back in stream
    //
    iostream_normalize_ring(&client->out, NULL);
    avail = client->out.size - client->out.count;
    pbackdata = NULL;

    if (avail > 16) // need room for protocol bytes
    {
        result = client->resource->callback(client,
                client->resource, httpUploadData, &pbackdata, &avail);
        if (result)
        {
            HTTP_ERROR("Callback cancels");
            return -1;
        }
        if (avail > 0)
        {
            byte = WSBIT_FIN;
            if (wsx->out_fmt & WSOPCODE_TEXT)
            {
                byte |= WSOPCODE_TEXT;
            }
            else
            {
                byte |= WSOPCODE_DATA;
            }
            client->out.data[client->out.head] = byte;
            client->out.head++;
            client->out.count++;

            if (avail < 126)
            {
                // one byte length
                byte = avail;
                if (wsx->out_masked)
                {
                    byte |= WSBIT_MASK;
                }
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
            }
            else if (avail < 65535)
            {
                // two byte length
                byte = 126;
                if (wsx->out_masked)
                {
                    byte |= WSBIT_MASK;
                }
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                byte = ((avail >> 8) & 0xFF);
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                byte = (avail & 0xFF);
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
            }
            else
            {
                // eight byte length
                byte = 127;
                if (wsx->out_masked)
                {
                    byte |= WSBIT_MASK;
                }
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
#if (0)
                byte = ((avail >> 56) & 0xFF);
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                byte = ((avail >> 48) & 0xFF);
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                byte = ((avail >> 40) & 0xFF);
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                byte = ((avail >> 32) & 0xFF);
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
#else
                byte = 0;
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
#endif
                byte = ((avail >> 24) & 0xFF);
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                byte = ((avail >> 16) & 0xFF);
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                byte = ((avail >> 8) & 0xFF);
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
                byte = (avail & 0xFF);
                client->out.data[client->out.head] = byte;
                client->out.head++;
                client->out.count++;
            }
            if (wsx->out_masked)
            {
                size_t i, j;

                client->out.data[client->out.head] = wsx->out_mask_key[0];
                client->out.head++;
                client->out.count++;
                client->out.data[client->out.head] = wsx->out_mask_key[1];
                client->out.head++;
                client->out.count++;
                client->out.data[client->out.head] = wsx->out_mask_key[2];
                client->out.head++;
                client->out.count++;
                client->out.data[client->out.head] = wsx->out_mask_key[3];
                client->out.head++;
                client->out.count++;
                j = client->out.head;
                for (i = 0; i < avail; i++, j++)
                {
                    client->out.data[j] = pbackdata[i] ^ wsx->out_mask_key[i & 0x3];
                }
            }
            else
            {
                memcpy(client->out.data + client->out.head, pbackdata, avail);
            }
            client->out.count += avail;
            client->out.head += avail;
            if (client->out.head >= client->out.size)
            {
                client->out.head = 0;
            }
        }
    }
    return result;
}

int http_websocket_create_stream(http_client_t *client)
{
    // allocate a websocket iostream.  NOTE! this does NOT wrap
    // the underlying iostream.  the underlying http infrastructure
    // will still deal directly with the iostream object and only the
    // websocket slice will deal with this ws stream
    //
    client->ws_stream = iostream_websocket_create_from_stream(client->stream);
    if (! client->ws_stream)
    {
        HTTP_ERROR("No stream");
        return -1;
    }
    return 0;
}

int http_websocket_upgrade_reply(http_client_t *client)
{
    int result = 0;
    uint8_t *key_buffer, *sha_buffer;
    int key_len, key_room, sha_room;

    http_log(5, "WS Upgrade stage 1\n");

    // first-state in upgrade
    //
    if (! client->ws_key[0])
    {
        HTTP_ERROR("No Sec-WebSocket-Key");
        return http_error_reply(client, 400, "No Key", true);
    }
    // begin the upgrade reply
    //
    result = http_begin_reply(client, 101, "Switching Protocols");
    result |= http_append_reply(client, "Upgrade: websocket");
    result |= http_append_reply(client, "Connection: Upgrade");
    if (client->ws_proto[0])
    {
        result |= http_append_reply(client, "Sec-WebSocket-Protocol: %s", client->ws_proto);
    }
    // leave output head after reply key header
    result |= http_append_reply(client, "Sec-WebSocket-Accept: ");
    if (result)
    {
        return result;
    }
    // back up over the crlf
    client->out.head -= 2;
    client->out.count -= 2;

    // allocate a buffer area in the output buffer space for formatting
    //
    key_buffer = client->out.data + client->out.head + 64;
    key_room   = 128;

    sha_buffer = key_buffer + key_room;
    sha_room   = 64;

    if ((sha_room + key_room) >= (client->out.size - client->out.count))
    {
        HTTP_ERROR("No room for keys");
        return -1;
    }
    // copy key to buffer area
    strncpy((char*)key_buffer, client->ws_key, key_room);
    key_len = strlen((char *)key_buffer);

    // append uuid to key (key remains base64 encoded)
    strncpy((char *)key_buffer + key_len, HTTP_WEBSOCKET_KEY_UUID, key_room - key_len);
    key_len = strlen((char *)key_buffer);
    if (key_len >= key_room)
    {
        HTTP_ERROR("No room for keys");
        return -1;
    }
    // get SHA1 hash of that string into the next buffer
    //
    result = iostream_sha1_hash(sha_buffer, key_buffer, key_len);

    // base64 encode that 20 byte hash directly into the current output position
    result = butil_base64_encode((char *)client->out.data + client->out.head, 64, sha_buffer, 20, false, false);
    if (result <= 0)
    {
        HTTP_ERROR("No room for encoded key");
        return -1;
    }
    client->out.count += result;
    client->out.head += result;

    // finish off header, and add a blank line
    result = http_append_reply(client, "");
    result |= http_append_reply(client, "");
    if (result)
    {
        return result;
    }
    return http_send_out_data(client, httpSendReply, httpWebSocketUpgrade);
}

#endif


