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

static const char *http_state_name(http_state_t state)
{
    switch (state)
    {
    case httpServeInit:         return "Init(server)";
    case httpClientInit:        return "Init(client)";
    case httpReadline:          return "Readline";
    case httpReadRequest:       return "Request (read)";
    case httpReadReply:         return "Reply (read)";
    case httpSendRequest:       return "Request (send)";
    case httpHeaders:           return "Headers";
    case httpMultipartHeaders:  return "Headers (multipart)";
    case httpTLSsocketUpgrade:  return "TLS Upgrade";
    case httpWebSocketUpgrade:  return "WebSocket Upgrade";
    case httpHandleReadRequest: return "Handle Request (read)";
    case httpHandleSendRequest: return "Handle Request (send)";
    case httpReadChunkCount:    return "Chunk Count (read)";
    case httpBodyDownload:      return "Body (read)";
    case httpBodyUpload:        return "Body (send)";
    case httpSendReply:         return "Reply (send)";
    case httpPropFindEnumerate: return "Enumerate (find)";
    case httpUserMethod:        return "User Method";
    case httpKeepAlive:         return "KeepAlive";
    case httpDone:              return "Done";
    default:                    return "- bad state -";
    }
}

http_client_t *http_client_create(http_resource_t *resources, bool isclient)
{
    http_client_t *client;
    static int s_ids = 0;

    client = (http_client_t *)malloc(sizeof(http_client_t));
    if (! client)
    {
        HTTP_ERROR("Can't alloc http");
        return NULL;
    }
    memset(client, 0, sizeof(http_client_t));

    client->isclient = isclient;
    client->resources = resources;
    client->resource = NULL;
    client->resource_open = false;

    if (! isclient)
    {
        // this is a context to serve a client
        client->state = httpServeInit;
    }
    else
    {
        // this is a client context, don't init until stream is set
        client->state = httpDone;
    }
    client->prev_state = httpDone;
    client->long_timeout = HTTP_LONG_TIMEOUT;
    client->in.size = HTTP_IO_SIZE;
    client->in.data = (uint8_t *)malloc(client->in.size);
    if (! client->in.data)
    {
        HTTP_ERROR("Can't alloc in data");
        free(client);
        return NULL;
    }

    client->out.size = HTTP_IO_SIZE;
    client->out.data = (uint8_t *)malloc(client->out.size);
    if (! client->out.data)
    {
        HTTP_ERROR("Can't alloc out data");
        free(client->in.data);
        free(client);
        return NULL;
    }

    http_client_reinit(client, true);

    client->transport = httpTCP;
    client->keepalive = false;
    client->use100    = false;
    client->expect100 = false;
    client->redirects = 0;
    #if HTTP_SUPPORT_TLS
    client->tls_upgrade = false;
    #endif
    #if HTTP_SUPPORT_COMPRESSION
    client->comprio.size = HTTP_IO_SIZE;
    client->comprio.data = (uint8_t *)malloc(client->comprio.size);
    if (! client->comprio.data)
    {
        HTTP_ERROR("Can't alloc compression buffer data");
        free(client);
        return NULL;
    }
    client->compress = httpDontCompress;
    #endif
    #if HTTP_SUPPORT_WEBSOCKET
    client->ws_upgrade = false;
    client->ws_key[0] = '\0';
    client->ws_proto[0] = '\0';
    #endif
    client->id = s_ids++;

    return client;
}

void http_client_reinit(http_client_t *client, bool newstream)
{
    // init context
    client->response = 0;
    client->long_timeout = HTTP_LONG_TIMEOUT;
    client->last_in_time = 0;
    client->last_out_time = 0;
    client->in_transfer_type = httpNone;
    client->in_content_length = 0;
    client->in_content_type = butil_mime_bin;
    client->in_transferred = 0;
    client->ifmodifiedsince = 0;
    client->modifiedwhen = 0;
    client->out_transfer_type = httpNone;
    client->out_content_length = 0;
    client->out_content_type = butil_mime_bin;
    client->out_gotten = 0;
    client->out_transferred = 0;
    client->start_byte = 0;
    client->end_byte = 0;
    client->location[0] = '\0';

    if (client->resource && client->resource_open && client->resource->callback)
    {
        client->resource->callback(
                                    client,
                                    client->resource,
                                    httpComplete,
                                    NULL,
                                    NULL
                                  );
    }
    client->resource = NULL;
    client->resource_open = false;

    #if HTTP_SUPPORT_COMPRESSION
    if (client->compress >= httpWillCompress)
    {
        deflateEnd(&client->zstrm);
    }
    client->compress = httpDontCompress;
    #endif
    #if HTTP_SUPPORT_MULTIPART
    client->boundary[0] = '\0';
    client->boundary_length = 0;
    #endif
    #if HTTP_SUPPORT_AUTH
    client->is_authorized = true;
    client->auth_type = httpAuthNone;
    client->auth_creds[0] = '\0';
    #endif
    #if HTTP_SUPPORT_WEBDAV
    client->dav_token[0] = '\0';
    client->dav_depth_in = 0;
    client->dav_depth_at = -1;
    client->dav_no_root = false;
    http_webdav_findstate_init(client);
    #endif
    #if HTTP_SUPPORT_WEBSOCKET
    client->ws_extensions = 0;
    if (client->ws_stream)
    {
        client->ws_stream->close(client->ws_stream);
        client->ws_stream = NULL;
    }
    #endif

    if (newstream)
    {
        if (client->stream)
        {
            client->stream->close(client->stream);
            client->stream = NULL;
        }
        client->scheme = schemeHTTP;
        client->host[0] = '\0';
        client->port = 0;
        client->path[0] = '\0';

        client->in.tail = 0;
        client->in.head = 0;
        client->in.count = 0;
        client->out.tail = 0;
        client->out.head = 0;
        client->out.count = 0;
    }
}

void http_client_free(http_client_t *client)
{
    if (client)
    {
        if (client->resource && client->resource_open && client->resource->callback)
        {
            client->resource->callback(
                                        client,
                                        client->resource,
                                        httpComplete,
                                        NULL,
                                        NULL
                                      );
        }
        client->resource = NULL;
        client->resource_open = false;

        if (client->stream)
        {
            client->stream->close(client->stream);
            client->stream = NULL;
        }
        #if HTTP_SUPPORT_WEBSOCKET
        if (client->ws_stream)
        {
            client->ws_stream->close(client->ws_stream);
            client->ws_stream = NULL;
        }
        #endif
        #if HTTP_SUPPORT_COMPRESSION
        if (client->compress >= httpWillCompress)
        {
            deflateEnd(&client->zstrm);
        }
        if (client->comprio.data)
        {
            free(client->comprio.data);
        }
        #endif
        if (client->in.data)
        {
            free(client->in.data);
        }
        if (client->out.data)
        {
            free(client->out.data);
        }
        free(client);
    }
}

int http_client_input(http_client_t *client, int to_secs, int to_usecs)
{
    time_t now;
    int result;
    int room;
    int count;
    int len;

    if (! client || ! client->stream)
    {
        return -1;
    }
    count = client->in.count;

    if (client->in.count < client->in.size)
    {
        if (client->in.count == 0)
        {
            // to avoid later renormalizing
            client->in.head = 0;
            client->in.tail = 0;
        }
        if (client->in.head < client->in.tail)
        {
            room = client->in.tail - client->in.head;
        }
        else
        {
            room = client->in.size - client->in.head;
        }
        // if there is a bunch of data in the buffer, don't wait for more
        //
        if (room < (client->in.size / 2))
        {
            to_secs  = 0;
            to_usecs = 0;
        }
        // don't read if less than N bytes room available.  this is to
        // ensure TLS read block size can alway fit
        //
        if (room >= HTTP_IO_MIN_ROOM)
        {
            result = client->stream->poll(client->stream, readable, to_secs, to_usecs);
            if (result < 0)
            {
                return result;
            }
            if (result > 0)
            {
                if (client->transport == httpTCP)
                {
                   len = client->stream->read(client->stream,
                               client->in.data + client->in.head, room);
                }
                else
                {
                    #if HTTP_SUPPORT_UDP
                    len = iostream_socket_recvfrom(client->stream,
                                client->in.data + client->in.head, room,
                                client->out_host, &client->out_port);
                    #else
                    return -1;
                    #endif
                }
                if (len <= 0)
                {
                    if (client->in.count)
                    {
                        return 0;
                    }
                    // 0 read after ok poll means connection closed
                    http_log(5, "end of connection %u\n", client->id);
                    return -1;
                }
                client->in.head += len;
                if (client->in.head >= client->in.size)
                {
                    client->in.head = 0;
                }
                client->in.count += len;
                http_log(7, "read %d on id %u\n", len, client->id);
            }
        }
    }
    time(&now);

    if ((client->in.count == count) && (client->state == client->prev_state))
    {
        // check long timeout
        if (client->last_in_time == 0)
        {
            client->last_in_time = now;
        }
        if ((now - client->last_in_time) > client->long_timeout)
        {
            // for udp transports in an init state, never timeout, since the
            // client is permanent as long a the server is alive
            //
            if (client->transport == httpUDP)
            {
                butil_log(4, "cl:%u UDP timeout in state %s\n",
                        client->id, http_state_name(client->state));
                client->last_in_time = now;
                return 0;
            }
            http_log(3, "cl:%u read timeout\n", client->id);
            return -1;
        }
    }
    else
    {
        // reset long timeout
        client->last_in_time = now;
    }
    return 0;
}

int http_send_data(http_client_t *client, const uint8_t *data, int count)
{
    int wc;
    time_t now;

    if (! client || ! client->stream)
    {
        HTTP_ERROR("Bad client");
        return -1;
    }
    if (client->transport == httpTCP)
    {
        wc = client->stream->poll(client->stream, writeable, 0, 1000);
        if (wc < 0)
        {
            HTTP_ERROR("closed on send");
            return -1;
        }
        time(&now);

        if (wc == 0)
        {
            // check long timeout
            if (client->last_out_time == 0)
            {
                client->last_out_time = now;
            }
            if ((now - client->last_out_time) > client->long_timeout)
            {
                http_log(3, "client %u write timeout\n", client->id);
                return -1;
            }
            return 0;
        }
        else
        {
            client->last_out_time = now;
        }
        wc = client->stream->write(client->stream, (uint8_t*)data, count);
        if (wc <= 0)
        {
            HTTP_ERROR("closed on send");
            return -1;
        }
    }
    else
    {
        #if HTTP_SUPPORT_UDP
        wc = iostream_socket_sendto(client->stream, (uint8_t *)data, count,
                client->out_host, client->out_port);
        #else
        return -1;
        #endif
    }
    return wc;
}

int http_send_out_data(http_client_t *client, http_state_t send_state, http_state_t next_state)
{
    int count;
    int len;

    client->state = send_state;
    client->next_state = next_state;

    if (client->out.count == 0)
    {
        client->last_out_time = 0;
        return 0;
    }
    if (client->out.tail < client->out.head)
    {
        count = client->out.head - client->out.tail;
    }
    else
    {
        count = client->out.size - client->out.tail;
    }
    len = http_send_data(client, client->out.data + client->out.tail, count);
    if (len < 0)
    {
        return -1;
    }
    client->out.count -= len;
    client->out.tail += len;
    if (client->out.tail >= client->out.size)
    {
        client->out.tail = 0;
    }
    return 0;
}

int http_begin_reply(http_client_t *client, uint16_t code, const char *message)
{
    int len;

    if (! client || ! client->stream)
    {
        HTTP_ERROR("Bad client");
        return -1;
    }
    // note the out buffer here is always used as tail == 0
    // and there is never wrapping in reply buffer
    //
    client->out.count = 0;
    client->out.head = 0;
    client->out.tail = 0;

    len = snprintf(
            (char*)client->out.data + client->out.head,
            client->out.size - client->out.count,
            "%s/%u.%u %3u %s\r\n",
            http_scheme_base_name(client->scheme),
            client->vmaj,
            HTTP_FORCE_1_0 ? 0 : client->vmin,
            code, message);
    if (len >= (client->out.size - client->out.count))
    {
        HTTP_ERROR("Reply buffer overflow");
        return 1;
    }
    client->out.count += len;
    client->out.head += len;
    return 0;
}

int http_begin_request(http_client_t *client)
{
    int len;

    client->out.count = 0;
    client->out.head = 0;
    client->out.tail = 0;

    len = snprintf(
            (char*)client->out.data + client->out.head,
            client->out.size - client->out.count,
            "%s %s %s/%u.%u\r\n",
            http_method_name(client->method),
            client->path,
            http_scheme_base_name(client->scheme),
            client->vmaj,
            HTTP_FORCE_1_0 ? 0 : client->vmin
            );
    if (len >= (client->out.size - client->out.count))
    {
        HTTP_ERROR("Request buffer overflow");
        return 1;
    }
    client->out.count += len;
    client->out.head += len;
    return 0;
}

#define http_append_request http_append_reply
int http_append_reply(http_client_t *client, const char *content, ...)
{
    va_list args;
    int len;

    va_start(args, content);

    len = vsnprintf(
            (char*)client->out.data + client->out.head,
            client->out.size - client->out.count - 2,
            content,
            args
            );
    va_end(args);

    if (len >= (client->out.size - client->out.count - 2))
    {
        HTTP_ERROR("Reply buffer overflow");
        return 1;
    }
    client->out.data[client->out.head + len] = '\r';
    client->out.data[client->out.head + len + 1] = '\n';
    len += 2;
    client->out.count += len;
    client->out.head += len;
    return 0;
}

int http_append_connection_to_reply(http_client_t *client, bool forceclose)
{
    if (forceclose || ! client->keepalive)
    {
        client->keepalive = false;
        return http_append_reply(client, "Connection: Close");
    }
    else
    {
        return http_append_reply(client, "Connection: Keep-Alive");
    }
}

int http_error_reply(http_client_t *client, uint16_t code, const char *msg, bool forceclose)
{
    int result;

    result = http_begin_reply(client, code, msg ? msg : "");
    result |= http_append_connection_to_reply(client, forceclose);
    result |= http_append_reply(client, "Content-Length: 0");
    result |= http_append_reply(client, "");
    //result |= http_append_reply(client, "");
    result |= http_send_out_data(client, httpSendReply, httpKeepAlive);
    return result;
}

int http_client_authorized(http_client_t *client, http_resource_t *resource)
{
    if (! client || ! resource)
    {
        return -1;
    }
#if HTTP_SUPPORT_AUTH
    client->is_authorized = true;

    // check resource authorization
    //
    if (client->resource->credentials.type != httpAuthNone)
    {
        int result = http_check_credentials(
                                    client->auth_type,
                                    client->auth_creds,
                                    &client->resource->credentials
                                    );
        if (result)
        {
            client->is_authorized = false;
        }
    }
#endif
    return 0;
}

int http_client_blocked(http_client_t *client)
{
    switch (client->state)
    {
    case httpServeInit:
    case httpReadline:
        if (! client->in.count)
        {
            // want to poll for readability
            return 1;
        }
        break;

    case httpBodyDownload:
        if (! client->in.count && client->in_content_length)
        {
            // want to poll for readability
            return 1;
        }
        break;

    case httpClientInit:
        return 1;

    case httpBodyUpload:
    case httpSendReply:
        if (client->out.count)
        {
            // want to poll for writeability
            return 1;
        }
        break;

    default:
        break;
    }
    // can make progress on this client
    return 0;
}

static char *http_skip_white(char *p)
{
    while (*(p) && ((*(p) == ' ') || (*(p) == '\t')))
    {
        p++;
    }
    return p;
}

static char *http_skip_nonwhite(char *p)
{
    while (*(p) && (*(p) != ' ') && (*(p) != '\t'))
    {
        p++;
    }
    return p;
}

static int http_process_header(http_client_t *client, char *header)
{
    char *value;

    http_log(5, "hdr %s\n", header);

    header = http_skip_white(header);
    value = http_skip_nonwhite(header);
    value = http_skip_white(value);

    if (! http_ncasecmp(header, "content-length:"))
    {
        client->in_content_length = strtoul(value, NULL, 10);
        client->in_transfer_type = httpLength;
    }
    else if (! http_ncasecmp(header, "transfer-encoding:"))
    {
        if (! http_ncasecmp(value, "chunked"))
        {
            client->in_transfer_type = httpChunked;
        }
    }
    else if (! http_ncasecmp(header, "connection:"))
    {
        if (! http_ncasecmp(value, "keep-alive"))
        {
            client->keepalive = true;
        }
        else if (! http_ncasecmp(value, "close"))
        {
            client->keepalive = false;
        }
    }
    else if (! http_ncasecmp(header, "if-modified-since:"))
    {
        client->ifmodifiedsince = butil_rfc2616_date_to_time(value);
    }
    else if (! http_ncasecmp(header, "content-type:"))
    {
        client->in_content_type = butil_content_type_for_mime_string(value);
        if (client->in_content_type == butil_mime_multi)
        {
            #if HTTP_SUPPORT_MULTIPART
            // get boundary
            value = strstr(value, "boundary=");
            if (value)
            {
                value += 9;
                value = http_skip_white(value);
            }
            if (! value || ! *value)
            {
                HTTP_ERROR("No boundary");
                return -1;
            }
            // reserve bytes in boundary buffer for CRLF--<boundary>
            if (strlen(value) >= (sizeof(client->boundary) - 5))
            {
                HTTP_ERROR("Boundary len");
                return -1;
            }
            http_log(4, "mp boundary=%s\n", value);

            // include the delimiter in the boundary so compares are easier
            strcpy(client->boundary, "--");

            // save boundary text
            strcpy(client->boundary + 2, value);

            // save length to avoid constant recalculation
            client->boundary_length = strlen(client->boundary);

            if (client->boundary_length == 0)
            {
                HTTP_ERROR("Boundary short");
                return -1;
            }
            #else
            HTTP_ERROR("Multipart not supported");
            return -1;
            #endif
        }
    }
    else if (! http_ncasecmp(header, "location:"))
    {
        if (strlen(value) >= sizeof(client->location))
        {
            HTTP_ERROR("Location len");
            return -1;
        }
        strcpy(client->location, value);
    }
    else if (! http_ncasecmp(header, "host:"))
    {
        if (strlen(value) >= sizeof(client->host))
        {
            HTTP_ERROR("Host len");
            return -1;
        }
        strcpy(client->host, value);
    }
    else if (! http_ncasecmp(header, "expect:"))
    {
        client->expect100 = !strcasecmp(value, "100-continue") ? true : false;
    }
    else if (! http_ncasecmp(header, "range:"))
    {
        // support bytes, but nothing else
        if (! http_ncasecmp(value, "bytes="))
        {
            value += 6;
            value = http_skip_white(value);
            client->start_byte = strtoul(value, &value, 10);
            if (*value == '-')
            {
                if (value[1] >= '0' && value[1] <= '9')
                {
                    client->end_byte = strtoul(value + 1, NULL, 10);
                }
                else
                {
                    client->end_byte = 0;
                }
            }
            else
            {
                client->end_byte = 0;
            }
            http_log(4, "RANGE: %u-%u\n", client->start_byte, client->end_byte);
        }
        else
        {
            HTTP_ERROR("Unsupported range type");
            return -1;
        }
    }
    #if HTTP_SUPPORT_AUTH
    else if (! http_ncasecmp(header, "authorization:"))
    {
        if (http_auth_string_to_type(value, &client->auth_type))
        {
            HTTP_ERROR("Unsupported Auth type");
            return -1;
        }
        value = http_skip_nonwhite(value);
        value = http_skip_white(value);
        if (*value)
        {
            switch (client->auth_type)
            {
            case httpAuthBasic:
                if (butil_base64_decode((uint8_t*)client->auth_creds, sizeof(client->auth_creds), value) < 0)
                {
                    HTTP_ERROR("Authorization encoding");
                    return -1;
                }
                break;
            default:
                if (strlen(value) >= sizeof(client->auth_creds))
                {
                    HTTP_ERROR("Authorization len");
                    return -1;
                }
                strcpy(client->auth_creds, value);
                break;
            }
        }
        else
        {
            HTTP_ERROR("No Credentials");
            return -1;
        }
    }
    #endif
    else if (! http_ncasecmp(header, "accept-encoding:"))
    {
    #if HTTP_SUPPORT_COMPRESSION
         value = strstr(value, "gzip");
         if (value)
         {
             client->compress = httpCanCompress;
         }
    #endif
    }
    else if (! http_ncasecmp(header, "content-encoding:"))
    {
    #if HTTP_SUPPORT_COMPRESSION
         value = strstr(value, "gzip");
         if (value)
         {
             client->compress = httpCanCompress;
         }
         else
         {
             HTTP_ERROR("Unsupported content-encoding");
             return -1;
         }
    #endif
    }
    #if HTTP_SUPPORT_WEBDAV
    else if (! http_ncasecmp(header, "lock-token:"))
    {
        size_t tokenlen;
        
        if (strlen(value) >= sizeof(client->dav_token))
        {
            HTTP_ERROR("DAV Token len");
            return -1;
        }
        // remove the <> around the token if there
        if (*value == '<')
        {
            value++;
        }
        strcpy(client->dav_token, value);
        tokenlen = strlen(client->dav_token);
        if (tokenlen > 0)
        {
            if (client->dav_token[tokenlen - 1] == '>')
            {
                client->dav_token[tokenlen - 1] = '\0';
            }
        }    
    }
    else if (! http_ncasecmp(header, "destination:"))
    {
        // use the find-state path to hold this since not propfind
        if (strlen(value) >= sizeof(client->dav_findpath))
        {
            HTTP_ERROR("Destination len");
            return -1;
        }
        #if 1 // assume move on server
        butil_parse_url(value, NULL, NULL, 0, NULL, client->dav_findpath, sizeof(client->dav_findpath));
        #else
        strcpy(client->dav_findpath, value);
        #endif
    }
    #endif
    else if (! http_ncasecmp(header, "upgrade:"))
    {
        #if HTTP_SUPPORT_WEBSOCKET
        if (! http_ncasecmp(value, "websocket"))
        {
            if (! client->isclient)
            {
                http_log(5, "WebSocket upgrade requested\n");
            }
            client->ws_upgrade = true;
        }
        #endif
        #if HTTP_SUPPORT_WEBSOCKET
        if (! http_ncasecmp(value, "tls"))
        {
            if (! client->isclient)
            {
                http_log(5, "TLS upgrade requested\n");
            }
            client->tls_upgrade = true;
        }
        #endif
    }
    #if HTTP_SUPPORT_WEBSOCKET
    else if (! http_ncasecmp(header, "sec-websocket-version:"))
    {
        client->ws_version = strtoul(value, NULL, 10);
    }
    else if (! http_ncasecmp(header, "sec-websocket-extensions:"))
    {
    }
    else if (! http_ncasecmp(header, "sec-websocket-key:"))
    {
        if (strlen(value) >= sizeof(client->ws_key))
        {
            HTTP_ERROR("Websocket Key len");
            return -1;
        }
        strcpy(client->ws_key, value);
    }
    else if (! http_ncasecmp(header, "sec-websocket-accept:"))
    {
        if (http_websocket_check_key(client->ws_key, value))
        {
            HTTP_ERROR("Websocket Accept Key");
            client->ws_upgrade = false;
            return -1;
        }
    }
    else if (! http_ncasecmp(header, "sec-websocket-protocol:"))
    {
        if (strlen(value) >= sizeof(client->ws_proto))
        {
            HTTP_ERROR("Websocket protocol len");
            return -1;
        }
        strcpy(client->ws_proto, value);
    }
    #endif
    #if HTTP_SUPPORT_WEBDAV
    else if (! http_ncasecmp(header, "depth:"))
    {
        if (client->scheme == schemeHTTP || client->scheme == schemeHTTPS)
        {
           client->dav_depth_in = (int)strtol(value, &value, 10);
           client->dav_no_root = false;
        }
        if (value[0])
        {
            if (value[0] != ',')
            {
                if (! http_ncasecmp(value, "infinity"))
                {
                    if (client->scheme == schemeHTTP || client->scheme == schemeHTTPS)
                    {
                        client->dav_depth_in = HTTP_MAX_WEBDAV_DEPTH - 1;
                    }
                    value += 8;
                }
                else
                {
                    HTTP_ERROR("Bad depth header");
                }
            }
            if (value[0] == ',')
            {
                value = http_skip_white(value);
                if (! http_ncasecmp(value, "noroot"))
                {
                    client->dav_no_root = true;
                }
            }
        }
    }
    #endif
    else
    {
        int result;

        // if the client has a resource, and callback, tell it about the header
        //
        if (client->resource && client->resource->callback)
        {
            result = client->resource->callback(
                                            client,
                                            client->resource,
                                            httpRequestHeader,
                                            (uint8_t**)&header,
                                            NULL
                                            );
            if (result)
            {
                // note we intentionally ignore error returns in header
                // processing callbacks, the main request callback should
                // handle any errors then
                //
                http_log(3, "Header callback error (ignored)\n");
                result = 0;
            }
        }
        else
        {
            http_log(5, "ignore header %s\n", header);
        }
    }
    return 0;
}

#if HTTP_SUPPORT_MULTIPART
static int http_process_multipart_header(http_client_t *client, char *header)
{
    char *value;

    http_log(7, "mphdr %s\n", header);

    header = http_skip_white(header);
    value = http_skip_nonwhite(header);
    value = http_skip_white(value);

    if (! http_ncasecmp(header, "content-disposition:"))
    {
        // revert method to post until a filename is found
        //
        client->method = httpPost;

        // look for "filename=" or "filename*="
        char *newpath = strstr(value, "filename");

        if (newpath)
        {
            while (*newpath && *newpath != '=')
            {
                newpath++;
            }
            while (*newpath == '=' || *newpath == '\"')
            {
                newpath++;
            }
            if (*newpath)
            {
                value = newpath;
                while (*newpath && *newpath != '\"')
                {
                    newpath++;
                }
                *newpath = '\0';

                // got a filename.  the method can be PUT not POST
                //
                client->method = httpPut;
            }
        }
        // new path is the filename component, or the whole header value here
        //
        strncpy(client->path, value, sizeof(client->path) - 2);
        client->path[sizeof(client->path) - 1] = '\0';
    }
    else if (! http_ncasecmp(header, "content-type:"))
    {
        client->in_content_type = butil_content_type_for_mime_string(value);
        if (client->in_content_type == butil_mime_multi)
        {
            // this is legal, but we don't handle it
            HTTP_ERROR("Multipart recursion");
            return -1;
        }
    }
    else
    {
        http_log(5, "ignore mp header %s\n", header);
    }
    return 0;
}
#endif

static void http_get_line(http_client_t *client, http_state_t next_state)
{
    client->next_state  = next_state;
    client->state       = httpReadline;
    client->line_count  = 0;
    client->reported_line_error = false;
}

static int http_slice_fatal(http_client_t *client, int result)
{
    // clean up client, including closing stream if opened
    http_client_reinit(client, true);
    return result;
}

int http_client_slice(http_client_t *client)
{
    char *pline;
    int result;
    int i;
    int incount;
    size_t bodyCount;

    if (client->prev_state != client->state)
    {
        http_log(6, "cl:%u: %s %s%s   in:%u  out:%u\n",
            client->id, http_state_name(client->state),
            client->state == httpReadline ? "->" : "",
            client->state == httpReadline ? http_state_name(client->next_state) : "",
            client->in.count, client->out.count);
    }
    client->prev_state = client->state;

    switch (client->state)
    {
    case httpServeInit:
        // wait for data on input
        result = http_client_input(client, 0, 0);
        if (result)
        {
            return http_slice_fatal(client, result);
        }
        if (client->in.count > 0)
        {
            // input is starting
            http_client_reinit(client, false);

    #if HTTP_SUPPORT_TLS
            client->tls_upgrade = false;
    #endif
    #if HTTP_SUPPORT_WEBSOCKET
            // insist client re-ask for upgrade
            client->ws_upgrade = false;
            client->ws_key[0] = '\0';
            client->ws_proto[0] = '\0';
    #endif
            // move to get request
            http_get_line(client, httpReadRequest);
        }
        break;

    case httpClientInit:
        // wait till socket to server connects
        if (! client->stream)
        {
            HTTP_ERROR("No stream");
            return http_slice_fatal(client, -1);
        }
        // poll for writeable means connected (assume caller is waiting
        // for client event)
        //
        result = client->stream->poll(client->stream, writeable, 0, 2000);
        if (result < 0)
        {
            HTTP_ERROR("stream");
            return http_slice_fatal(client, -1);
        }
        if (result == 0)
        {
            // no data, remain in init state
            break;
        }
        http_log(5, "cl:%u Connected\n", client->id);
        {
            http_resource_t *svres = client->resource;

            // reset client for request, but save resource so
            // reinit doesn't clear it out
            client->resource = NULL;
            http_client_reinit(client, false);
            client->resource = svres;
        }
    #if HTTP_SUPPORT_TLS
        // upgrade to TLS if indicated (scheme already checkd for support)
        //
        if (
                client->scheme == schemeHTTPS
    #if HTTP_SUPPORT_WEBSOCKET
            ||  client->scheme == schemeWSS
    #endif
        )
        {
            iostream_t *stream;

            client->secure = true;

            stream = iostream_tls_create_from_iostream(client->stream, true);
            if (! stream)
            {
                HTTP_ERROR("TLS upgrade failed");
                client->stream = NULL;
                return http_slice_fatal(client, -1);
            }
            client->stream = stream;
        }
    #endif
        // setup to send the request
        //
        result = http_begin_request(client);
        result |= http_append_request(client, "Host: %s:%u", client->host, client->port);
        client->expect100 = false;
        if (client->use100)
        {
            client->expect100 = true;
            result |= http_append_request(client, "Expect: 100-continue");
        }
        // get content-type, etc. from resource if present (post/put)
        //
        if (client->resource && client->resource->callback)
        {
            // tell resource the request is starting
            //
            // NOTE: this callback can change client state, setup xfer state, etc.
            //
            result = client->resource->callback(
                                        client,
                                        client->resource,
                                        httpRequest,
                                        NULL,
                                        NULL
                                        );
            if (result)
            {
                HTTP_ERROR("Send request callback cancels");
                return http_slice_fatal(client, -1);
            }
            client->resource_open = true;

            // if client changed state on purpose, assume they don't want our headers
            //
            if (client->state != httpClientInit)
            {
                break;
            }
        }
        #if HTTP_SUPPORT_WEBSOCKET
        if (client->ws_upgrade)
        {
            result |= http_websocket_create_key(client->ws_key, sizeof(client->ws_key));
            result |= http_append_request(client, "Upgrade: websocket");
            result |= http_append_request(client, "Connection: Upgrade");
            result |= http_append_request(client, "Sec-WebSocket-Key: %s", client->ws_key);
            result |= http_append_request(client, "Sec-WebSocket-Protocol: %s", client->ws_proto);
            result |= http_append_request(client, "Sec-WebSocket-Version: %u", client->ws_version);
        }
        else
        {
        #endif
        result |= http_append_connection_to_request(client, false);

        if (client->out_content_length)
        {
            if (client->out_transfer_type != httpChunked)
            {
                result |= http_append_request(client, "Content-Length: %d",
                            client->out_content_length);
            }
            else
            {
                result |= http_append_request(client, "Transfer-Encoding: chunked");
            }
            if (client->out_content_type == butil_mime_multi)
            {
                result |= http_append_request(client, "Content-Type: %s;boundary=%s",
                            butil_mime_string_for_content_type(client->out_content_type),
                            client->boundary);
            }
            else
            {
                result |= http_append_request(client, "Content-Type: %s",
                            butil_mime_string_for_content_type(client->out_content_type));
            }
        }
        else
        {
            result |= http_append_request(client, "Content-Length: 0");
        }
        result |= http_append_request(client, "Accept: */*");
        #if HTTP_SUPPORT_COMPRESSION
        result |= http_append_request(client, "Accept-Encoding: gzip");
        #endif
        #if HTTP_SUPPORT_WEBSOCKET
        }
        #endif
        if (
                    (client->out_transfer_type != httpChunked)
            ||      ! (client->resource && client->out_content_length)
        )
        {
            // first chunk output will insert empty line so only do this for non-chunked
            // (or for chunked xfer where there won't be any chunks)
            //
            result |= http_append_request(client, "");
        }
        if (result)
        {
            return http_slice_fatal(client, result);
        }
        // if we expect 100-continue, go to send-request state and
        // then get the reply, else go directly to body upload state
        // to allow combining of request/headers with body to save
        // writes to the remote server
        //
        if (client->use100 || client->ws_upgrade)
        {
            client->state = httpSendRequest;
        }
        else
        {
            client->state = httpBodyUpload;
        }
        break;

    case httpReadline:
        result = http_client_input(client, 0, 0);
        if (result)
        {
            return http_slice_fatal(client, result);
        }
        while (client->in.count > 0)
        {
            uint8_t next_in = client->in.data[client->in.tail++];

            client->in.count--;
            if (client->in.tail >= client->in.size)
            {
                client->in.tail = 0;
            }
            if (
                    (next_in == '\n')
                 && (client->line_count > 0)
                 && (client->line[client->line_count - 1] == '\r')
            )
            {
                // note line termination not included in result line
                client->line[client->line_count - 1] = '\0';
                client->state = client->next_state;
                client->reported_line_error = false;
                break;
            }
            client->line[client->line_count++] = next_in;
            if (client->line_count >= HTTP_MAX_LINE - 1)
            {
                // policy is to truncate lines that are too long
                // and let them through since they can often be ignored
                //
                if (! client->reported_line_error)
                {
                    HTTP_ERROR("Line too long, truncating");
                    client->reported_line_error = true;
                }
                client->line_count--;
            }
        }
        break;

    case httpReadRequest:
        // assume 1.1
        client->vmaj = 1;
        client->vmin = 1;

        // extract request
        pline = (char*)client->line;
        pline = http_skip_white(pline);
        if (http_method_from_name(pline, &client->method))
        {
            // still parse body to keep keepalive going
            HTTP_ERROR("No such method");
        }
        // extract path
        pline = http_skip_nonwhite(pline);
        pline = http_skip_white(pline);
        for (i = 0; (i < (HTTP_MAX_PATH - 1)) && *pline; i++)
        {
            if (*pline == ' ' || *pline == '\t')
            {
                break;
            }
            client->path[i] = (char)*pline++;
        }
        client->path[i] = 0;
        pline = http_skip_white(pline);

        // extract scheme and version
        //
        for (i = 0; pline[i]; i++)
        {
            if (pline[i] == '/')
            {
                break;
            }
        }
        incount = pline[i];
        pline[i] = '\0';
        client->scheme = schemeHTTP;
        result = butil_scheme_from_name(pline, &client->scheme);
        pline[i] = incount;
        if (result)
        {
            HTTP_ERROR("No scheme");
        }
        pline += i;
        if (*pline)
        {
            pline++;
        }
        client->vmaj = (uint8_t)strtoul(pline, &pline, 10);
        if (*pline)
        {
            pline++;
            client->vmin = (uint8_t)strtoul(pline, &pline, 10);
        }
        http_log(5, "cl:%u %s %s %s/%u.%u\n", client->id,
                http_method_name(client->method),
                client->path,
                http_scheme_base_name(client->scheme),
                client->vmaj, client->vmin);

        // per RFC7230 - http 1.1 defaults to persistent connection
        if (client->vmaj == 1)
        {
            if (client->vmin == 1)
            {
                client->keepalive = true;
            }
            else
            {
                client->keepalive = false;
            }
            // consider closing on bad versions.
        }
        client->resource = NULL;
        client->resource_open = false;
        client->ctxpriv  = NULL;

        // lookup the resource for requests that need a resource. this just
        // sets the client resource member. errors are flagged after any
        // headers are read in
        //
        if (!client->isclient && client->method != httpOptions && client->method != httpUnsupported)
        {
            client->resource = http_find_resource(
                                                client->resources,
                                                client->ws_upgrade ?
                                                    (client->secure ? schemeWSS : schemeWS) :
                                                    http_scheme_base(client->scheme),
                                                client->path,
                                                client->method
                                                );
            if (! client->resource)
            {
                http_log(5, "No resource %s for method %s\n", client->path, http_method_name(client->method));
                // note we read the headers anyway, because this helps flow, and debug
            }
        }
        http_get_line(client, httpHeaders);
        break;

    case httpReadReply:
        // we're a client, getting server reply
        pline = (char*)client->line;
        pline = http_skip_white(pline);

        // extract version
        // extract scheme and version
        //
        for (i = 0; pline[i]; i++)
        {
            if (pline[i] == '/')
            {
                break;
            }
        }
        {
        char ic;
        butil_url_scheme_t scheme;
        uint8_t vmaj, vmin;

        ic = pline[i];
        pline[i] = '\0';
        scheme = schemeHTTP;

        result = butil_scheme_from_name(pline, &scheme);
        pline[i] = ic;
        if (result)
        {
            HTTP_ERROR("No scheme");
            result = 0; // ignored
        }
        pline += i;
        if (*pline)
        {
            pline++;
        }
        vmaj = (uint8_t)strtoul(pline, &pline, 10);
        if (*pline)
        {
            pline++;
            vmin = (uint8_t)strtoul(pline, &pline, 10);
        }
        }
        pline = http_skip_nonwhite(pline);
        pline = http_skip_white(pline);

        client->response = strtoul(pline, &pline, 10);

        pline = http_skip_white(pline);
        http_log(4, "reply %3d %s\n", client->response, pline);

        if (client->expect100)
        {
            if (client->response != 100)
            {
                http_log(1, "Expected 100, got %d\n", client->response);
                client->state = httpDone;
                break;
            }
            // sent the request by itself, now send body, which gets back here after
            client->expect100 = false;
            client->state = httpBodyUpload;
        }
        else
        {
            // get reply headers
            http_get_line(client, httpHeaders);
        }
        break;

    case httpHeaders:
        if (client->line[0] == '\0')
        {
            // blank line ends headers
            //
            if (client->isclient)
            {
                if (client->response >= 400)
                {
                    http_log(1, "Error %3d  %s\n", client->response, pline);
                    client->state = httpDone;
                    break;
                }
                if (client->response >= 300 && client->response < 400)
                {
                    butil_url_scheme_t newscheme;
                    char newhost[HTTP_MAX_HOSTNAME];
                    uint16_t newport;
                    char newpath[HTTP_MAX_PATH];
                    char newurl[HTTP_MAX_URL];

                    // redirect, check for loops
                    //
                    if (client->redirects++ > 4)
                    {
                        HTTP_ERROR("Too many redirects");
                        return http_slice_fatal(client, -1);
                    }
                    http_log(3, "Redirect %d to %s\n", client->redirects, client->location);

                    // for absolute redirects, copy over path, else concatenate
                    //
                    if (client->location[0] == '/')
                    {
                        result = butil_paste_url(newurl, HTTP_MAX_URL,
                                client->scheme, client->host, client->port,
                                client->location + 1);
                        result |= butil_parse_url(client->location, &newscheme,
                                newhost, HTTP_MAX_HOSTNAME, &newport,
                                newpath, HTTP_MAX_PATH);
                    }
                    else
                    {
                        result = butil_parse_url(client->location, &newscheme,
                                newhost, HTTP_MAX_HOSTNAME, &newport,
                                newpath, HTTP_MAX_PATH);
                        result += butil_paste_url(newurl, HTTP_MAX_URL,
                                newscheme, newhost, newport, newpath);
                    }
                    if (result)
                    {
                        HTTP_ERROR("Bad redirect url");
                        return http_slice_fatal(client, -1);
                    }
                    // close down client connection if new location isn't same host:port
                    // or connection isn't kept alive
                    //
                    if (client->stream)
                    {
                        client->stream->close(client->stream);
                        client->stream = NULL;
                    }
                    // reinit stream state on client
                    //
                    http_client_reinit(client, true);

                    result = butil_parse_url(newurl, &client->scheme,
                            client->host, sizeof(client->host), &client->port,
                            client->path, sizeof(client->path));
                    if (result)
                    {
                        HTTP_ERROR("Bad url");
                        return http_slice_fatal(client, -1);
                    }
                    // connect to new server (this resets state, etc.)
                    //
                    client->stream = iostream_create_from_tcp_connection(client->host, client->port);
                    if (! client->stream)
                    {
                        HTTP_ERROR("Can't make stream");
                        return http_slice_fatal(client, -1);
                    }
                    // go wait for client socket to connect
                    client->state = httpClientInit;
                    break;
                }
                else /* < 300 response, a-ok */
                {
                    // already sent request and body, get response body
                    client->state = httpHandleSendRequest;
                }
            }
            else
            {
                // get body
                client->state = httpHandleReadRequest;
            }
            break;
        }
        result = http_process_header(client, (char*)client->line);
        if (result < 0)
        {
            result = http_error_reply(client, 500, "Header error", true);
            if (result)
            {
                return http_slice_fatal(client, result);
            }
            break;
        }
        if (result > 0)
        {
            result = http_error_reply(client, 400, "Header error", true);
            if (result)
            {
                return http_slice_fatal(client, result);
            }
            break;
        }
        result = 0;
        http_get_line(client, httpHeaders);
        break;

    case httpMultipartHeaders:
        // multipart headers count against content length
        i = strlen((char*)client->line) + 2;
        if (i <= client->in_content_length)
        {
            client->in_content_length -= i;
        }
        else
        {
            HTTP_ERROR("Bad content length");
            client->in_content_length = 0;
        }
        if (client->line[0] == '\0')
        {
            client->state = httpHandleReadRequest;
            break;
        }
        result = http_process_multipart_header(client, (char*)client->line);
        if (result < 0)
        {
            result =http_error_reply(client, 500, "Multipart Header error", true);
            if (result)
            {
                return http_slice_fatal(client, result);
            }
            break;
        }
        if (result > 0)
        {
            result = http_error_reply(client, 400, "Multipart Header error", true);
            if (result)
            {
                return http_slice_fatal(client, result);
            }
            break;
        }
        result = 0;
        http_get_line(client, httpMultipartHeaders);
        break;

    case httpHandleReadRequest:
        // server read a request
        if (client->expect100)
        {
            // if client is expecting 100, answer that now
            //
            client->expect100 = false;
            result  = http_begin_reply(client, 100, "Continue");
            result |= http_append_reply(client, "");
            result |= http_send_out_data(client, httpHandleReadRequest, httpHandleReadRequest);
            if (result)
            {
                return http_slice_fatal(client, result);
            }
            break;
        }
        if (client->method != httpOptions && client->method != httpUnsupported)
        {
            if (! client->resource || ! client->resource->callback)
            {
                result = http_error_reply(client, 404, "Not Found", false);
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
                // set next state to body to discard it, making sure there's no resource
                // so keep-alive is maintained properly
                //
                client->resource = NULL;
                client->resource_open = false;
                client->next_state = httpBodyDownload;
            }
#if HTTP_SUPPORT_AUTH
            // check credentials. if not-authorized, continue to download body but
            // make sure there is no resource to affect, and recheck auth then
            //
            result = http_client_authorized(client, client->resource);
            if (result)
            {
                return http_slice_fatal(client, result);
            }
            if (! client->is_authorized)
            {
                http_log(5, "cl:%u not authorized to %s\n", client->id, client->path);

                // set state to body to discard it so keep-alive is maintained properly
                //
                client->state = httpBodyDownload;

                // form resource-specific portion of reply, the rest is done after
                // body is downloaded
                //
                result = http_begin_reply(client, 401, "Unauthorized");
                result |= http_append_reply(client, "WWW-Authenticate: %s realm=\"%s\"",
                        http_auth_type_to_string(client->resource->credentials.type),
                        client->resource->urlbase);

                // ensure there is no resource to access for the rest of time
                //
                if (client->resource_open)
                {
                    // can't happen, just here to ensure it can't
                    HTTP_ERROR("Server had open resource");
                    return http_slice_fatal(client, -1);
                }
                client->resource = NULL;
                client->resource_open = false;
                break;
            }
#endif
            // tell resource that we are starting a request
            //
            // NOTE: this callback has already gotten any headers ahead of this call
            // NOTE: this callback can change the client state, set a reply, and setup xfer state
            // NOTE: this callback can change state of compress and open a compression context
            //  the only time a Content-Encoding: gzip is sent is if this callback changes
            //  compress from httpCanCompress to httpWillCompress, so all callbacks have control
            //  over whether to compress data or not
            //
            result = client->resource->callback(
                                        client,
                                        client->resource,
                                        httpRequest,
                                        NULL,
                                        NULL
                                        );
            if (result)
            {
                HTTP_ERROR("Request callback cancels");
                client->resource = NULL;
                client->resource_open = false;
                // return an error and force-close connection to discard data
                result = http_error_reply(client, 500, "Resource failure", true);
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
            }
            else
            {
                client->resource_open = true;
            }
        }
        else
        {
            // other methods are handled after body is downloaded in buffer
            result = 0;
        }
        // setup next state to read body or read chunk count unless callback changed our state
        //
        if (client->state == httpHandleReadRequest)
        {
            if (client->in_transfer_type == httpChunked)
            {
                http_get_line(client, httpReadChunkCount);
            }
            else
            {
                client->state = httpBodyDownload;
            }
        }
        break;

#if HTTP_SUPPORT_WEBSOCKET
    case httpWebSocketUpgrade:
        if (! client->ws_stream)
        {
            result = http_websocket_create_stream(client);
            if (result)
            {
                return http_slice_fatal(client, result);
            }
            if (! client->isclient)
            {
                // we're serving a client, format and send a reply
                //
                result = http_websocket_upgrade_reply(client);
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
            }
        }
        else
        {
            result = http_websocket_slice(client);
            if (result)
            {
                return http_slice_fatal(client, result);
            }
        }
        break;
#endif
#if HTTP_SUPPORT_TLS
    case httpTLSsocketUpgrade:

        if (client->tls_upgrade)
        {
            // stage one, reply on regular stream
            //
            client->tls_upgrade = false;

            result = http_begin_reply(client, 101, "Switching Protocols");
            result |= http_append_reply(client, "Upgrade: TLS/1.2, HTTP/1.1");
            result |= http_append_reply(client, "Connection: Upgrade");
            result |= http_append_reply(client, "Content-Length: 0");
            result |= http_append_reply(client, "");

            return http_send_out_data(client, httpSendReply, httpTLSsocketUpgrade);
        }
        else
        {
            iostream_t *stream;

            client->in.count = 0;
            client->in.head = 0;
            client->in.tail = 0;

            client->out.count = 0;
            client->out.head = 0;
            client->out.tail = 0;

            // wrap io stream in TLS stream
            //
            stream = iostream_tls_create_from_iostream(client->stream, false);
            if (! stream)
            {
                HTTP_ERROR("TLS upgrade failed");
                client->stream = NULL;
                return http_slice_fatal(client, -1);
            }
            client->stream = stream;
            client->state = httpBodyDownload;
        }
        break;
#endif
    case httpHandleSendRequest:
        // client is sending req to server
        if (client->resource && client->resource->callback)
        {
            if (client->method == httpGet)
            {
                // put local filename into client path first (and move it out of base)
                //
                strncpy(client->path, client->resource->urlbase, sizeof(client->path) - 1);
                client->path[sizeof(client->path) - 1] = '\0';
                client->resource->urlbase = "";

                // tell resource the request is starting for GET
                //
                // NOTE: this callback can change client state, setup xfer state, etc.
                //
                result = client->resource->callback(
                                            client,
                                            client->resource,
                                            httpRequest,
                                            NULL,
                                            NULL
                                            );
                if (result)
                {
                    HTTP_ERROR("Send request callback cancels");
                    client->resource = NULL;
                    client->resource_open = false;
                    result = http_error_reply(client, 500, "Resource failed", false);
                    if (result)
                    {
                        return http_slice_fatal(client, result);
                    }
                }
                else
                {
                    client->resource_open = true;
                }
            }
        }
        // setup next state to read body or read chunk count if resource callback didn't change state
        //
        if (client->state == httpHandleSendRequest)
        {
            if (client->in_transfer_type == httpChunked)
            {
                http_get_line(client, httpReadChunkCount);
            }
            else
            {
                client->state = httpBodyDownload;
            }
        }
        break;

    case httpReadChunkCount:
        result = http_client_input(client, 0, 0);
        if (result)
        {
            return http_slice_fatal(client, result);
        }
        // here, line could be CRLFnnnnn, or just nnnnn (first chunk) and if
        // the first case, the first time in here will have a 0 length
        // and the next cases willhave a length of 1, or a CR as first line char
        //
        if (client->line_count == 0 || ! client->line[0])
        {
            http_get_line(client, httpReadChunkCount);
            break;
        }
        client->in_content_length = (size_t)strtoul((char*)client->line, NULL, 16);
        http_log(5, "cl:%u: next chunk %u\n", client->id, client->in_content_length);
        if (client->in_content_length == 0)
        {
            // reading 0 means last chunk, switch to length xfer to stop
            client->in_transfer_type = httpLength;

            // read the blank line after the 0, then go back to downloading
            client->next_state = httpBodyDownload;
            http_get_line(client, httpBodyDownload);
            break;
        }
        client->state = httpBodyDownload;
        break;

    case httpBodyDownload:
        if (client->in_transfer_type == httpNone)
        {
            // If NO content-length specifying method was in the headers
            // then assume its "till connection is over" except in those
            // cases where http requires a length (put/post)
            //
            #if HTTP_SUPPORT_WEBSOCKET
            if (client->ws_upgrade)
            {
                // client requested a websocket upgrade, so now that we read the body
                // of the http request if any, reply to the upgrade and start a websocket
                //
                client->state = httpWebSocketUpgrade;
                break;
            }
            #endif
            #if HTTP_SUPPORT_TLS
            if (client->tls_upgrade)
            {
                // client requested a TLS upgrade, so now that we read the body
                // of the http request if any, reply to the upgrade and start TLS
                //
                client->state = httpTLSsocketUpgrade;
                break;
            }
            #endif
            if (! client->isclient)
            {
                // we're a server, insist that posts and puts to us have a length
                //
                if (client->method == httpPut || client->method == httpPost)
                {
                    butil_log(2, "No content length in request\n");
                    result = http_error_reply(client, 411, "Length Required", false);
                    if (result)
                    {
                        return http_slice_fatal(client, result);
                    }
                }
                else
                {
                    // assume there's no entity body with the request
                    //
                    client->in_transfer_type = httpLength;
                }
            }
            else
            {
                // wer're a client, and this is the response to our request, which
                // doesn't need a content-length if connection will close, but
                // we DO expect data so read till close
                //
                // keep kicking the can down the road till connection closes by assuming
                // there's more content than we could read in one go so we keep reading
                // (and find the connection closing)
                //
                client->in_content_length = client->in.size + 1;
            }
        }
        if (client->in_content_length == 0)
        {
            if (client->in_transfer_type == httpChunked)
            {
                // time to get another chunk
                http_get_line(client, httpReadChunkCount);
                break;
            }
            if (client->isclient)
            {
                // got all the data back from remote, all over now
                http_log(5, "cl:%u got body of client request\n", client->id);
                client->state = httpKeepAlive;
                break;
            }
            #if HTTP_SUPPORT_AUTH
            if (client->method != httpOptions && client->method != httpUnsupported)
            {
                // recheck authorization now. if not authorized we'll reply here and
                // go directly to keepalive state, otherwize continue on
                //
                if (! client->is_authorized)
                {
                    if (client->auth_type != httpAuthNone && client->auth_creds[0])
                    {
                        // credentials were supplied, so really we just say NO
                        //
                        result = http_error_reply(client, 403, "Forbidden", false);
                    }
                    else
                    {
                        // inform client of auth needed, reply was formed already before
                        // the resource was nulled (see ReadRequest handling) so just
                        // finish the response and go
                        //
                        result  = http_append_connection_to_reply(client, false);
                        result |= http_append_reply(client, "Content-Length: 0");
                        result |= http_append_reply(client, "");
                        result |= http_send_out_data(client, httpSendReply, httpKeepAlive);
                    }
                    break;
                }
            }
            #endif
            #if HTTP_SUPPORT_WEBSOCKET
            if (client->ws_upgrade)
            {
                // client requested a websocket upgrade, so now that we read the body
                // of the http request if any, reply to the upgrade and start a websocket
                //
                client->state = httpWebSocketUpgrade;
                break;
            }
            #endif
            #if HTTP_SUPPORT_TLS
            if (client->tls_upgrade)
            {
                // client requested a TLS upgrade, so now that we read the body
                // of the http request if any, reply to the upgrade and start TLS
                //
                client->state = httpTLSsocketUpgrade;
                break;
            }
            #endif
            if (client->resource && client->resource->callback)
            {
                // tell resource the body download is over
                //
                // NOTE: this callback can change client state, setup xfer state, etc.
                //
                result = client->resource->callback(
                                            client,
                                            client->resource,
                                            httpDownloadDone,
                                            NULL,
                                            NULL
                                            );
                if (result)
                {
                    HTTP_ERROR("Request body done callback cancels");
                    client->resource = NULL;
                    client->resource_open = false;
                    result = http_error_reply(client, 500, "Resource failed", false);
                    if (result)
                    {
                        return http_slice_fatal(client, result);
                    }
                }
                if (client->state != httpBodyDownload)
                {
                    // callback changed state, don't do anything more here
                    break;
                }
            }
            switch (client->method)
            {
            case httpGet:
            case httpPut:
            case httpPost:
                #if HTTP_SUPPORT_RANGES
                if (client->start_byte > 0 || client->end_byte > 0)
                {
                    result  = http_begin_reply(client, 206, "Partial Content");
                    result |= http_append_reply(client, "Content-Range: bytes %d-%d/%d",
                                    client->start_byte, client->end_byte, client->out_content_length);
                    if (client->end_byte == 0)
                    {
                        client->end_byte = client->out_content_length - 1;
                    }
                    client->out_content_length = client->end_byte - client->start_byte + 1;
                }
                else
                #endif
                {
                    result = http_begin_reply(client, 200, "OK");
                }
                if (client->resource && client->out_content_length)
                {
                    result |= http_append_reply(client, "Content-Type: %s",
                                    butil_mime_string_for_content_type(client->out_content_type));

                    if (client->out_transfer_type != httpChunked)
                    {
                        result |= http_append_reply(client, "Content-Length: %d",
                                    client->out_content_length);
                    }
                    else
                    {
                        result |= http_append_reply(client, "Transfer-Encoding: chunked");
                    }
                    #if HTTP_SUPPORT_COMPRESSION
                    if (client->compress >= httpWillCompress)
                    {
                        result |= http_append_reply(client, "Content-Encoding: gzip");
                    }
                    #endif
                }
                else
                {
                    result |= http_append_reply(client, "Content-Length: 0");

                    // disable chunked to avoid any counts being added in upload stage
                    // client->out_transfer_type == httpNone;
                }
                if (client->method == httpGet || client->method == httpHead)
                {
                    #if HTTP_SUPPORT_RANGES
                    result |= http_append_reply(client, "Accept-Ranges: bytes");
                    #endif
                }
                result |= http_append_connection_to_reply(client, false);
                if (client->modifiedwhen > 0)
                {
                    char modbuf[32];

                    result |= http_append_reply(client, "Last-Modified: %s",
                                butil_time_to_rfc2616_date(client->modifiedwhen, modbuf, sizeof(modbuf)));
                }
                if (
                            (client->out_transfer_type != httpChunked)
                    ||      ! (client->resource && client->out_content_length)
                )
                {
                    // first chunk output will insert empty line so only do this for non-chunked
                    // (of for chunked xfer where there won't be any chunks)
                    //
                    result |= http_append_reply(client, "");
                }
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
                client->state = httpBodyUpload;
                break;

            #if HTTP_SUPPORT_WEBDAV
            case httpDelete:
            case httpPropFind:
            case httpPropPatch:
            case httpMkCol:
            case httpCopy:
            case httpMove:
            case httpLock:
            case httpUnlock:
                if (! client->resource)
                {
                    HTTP_ERROR("No DAV resource");
                    result = http_error_reply(client, 405, "Bad Request", false);
                    if (result)
                    {
                        return http_slice_fatal(client, result);
                    }
                    return 0;
                }
                result = http_webdav_request(client);
                if (result)
                {
                    // any errors that don't format a reply go to done state, else
                    // assume the request handler sent a specific reply already
                    //
                    if (client->state != httpSendReply)
                    {
                        client->state = httpDone;
                    }
                    result = 0;
                }
                break;
            #else
            case httpDelete:
                result = http_error_reply(client, 403, "Not Permitted", false);
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
                break;
            #endif

            case httpOptions:
                result = http_begin_reply(client, 200, "OK");
            #if HTTP_SUPPORT_WEBDAV
                result |= http_append_reply(client, "Allow: GET,HEAD,PUT,POST,DELETE,OPTIONS,PROPFIND,PROPPATCH,MKCOL,COPY,MOVE,LOCK,UNLOCK");
                result |= http_append_reply(client, "DAV: 1, 2");
            #else
                result |= http_append_reply(client, "Allow: GET,HEAD,PUT,POST,DELETE,OPTIONS");
            #endif
                result |= http_append_reply(client, "Content-Length: 0");
                result |= http_append_connection_to_reply(client, false);
                result |= http_append_reply(client, "");
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
                result = http_send_out_data(client, httpSendReply, httpKeepAlive);
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
                break;

            case httpUnsupported:
                http_log(1, "Unsupported request\n");
                result = http_error_reply(client, 405, "Method not Allowed", false);
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
                break;

            case httpUser0: case httpUser1: case httpUser2: case httpUser3:
            case httpUser4: case httpUser5: case httpUser6: case httpUser7:
            case httpUser8: case httpUser9: case httpUser10: case httpUser11:
            case httpUser12: case httpUser13: case httpUser14: case httpUser15:
                // resource callback should have handled state if needed
                // so if getting here means nothing done, so move on
                client->state = httpBodyUpload;
                break;

            default:
                http_log(1, "Bad request: %s\n", http_method_name(client->method));
                result = http_error_reply(client, 405, "Bad Request", false);
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
                break;
            }
            // body download complete, all set
            //
            break;
        }
        // continue getting body data if we expect more
        //
        result = http_client_input(client, 0, 0);
        if (result)
        {
            // no more data, if expecting more, that
            if (client->in_transfer_type == httpNone)
            {
                client->in_transfer_type = httpLength;
                client->in_content_length = 0;
                return 0;
            }
            return http_slice_fatal(client, result);
        }
        // rotate the input buffer so it is one contiguous buffer. this
        // helps alot for things like writing files in block size chunks,
        // and it is a noop for any data in the middle of a body which
        // will normally have tail == 0 each block
        //
        iostream_normalize_ring(&client->in, NULL);

        incount = client->in.count;

        #if HTTP_SUPPORT_MULTIPART
        // if multipart, look for any sign of a boundary in the input buffer
        // and limit the input parsing to the data before the possible boundary
        // check for full boundary if have 2 more chars than needed so we
        // can check for crlf or -- after boundary in one go
        //
        if (
                (client->boundary_length > 0)
            &&  (client->boundary[0] != '\1')
            &&  (client->in.data[0] == client->boundary[0])
            &&  (client->in.count >= (client->boundary_length + 2))
            &&  ! strncmp((char*)client->in.data, client->boundary, client->boundary_length)
        )
        {
            // this is a boundary. complete previous xfer state and parse headers
            //
            http_log(4, "at boundary %02X\n", client->boundary[0]);
            if (client->resource && client->resource_open && client->resource->callback)
            {
                result = client->resource->callback(
                                            client,
                                            client->resource,
                                            httpComplete,
                                            NULL,
                                            NULL
                                            );
                if (result < 0)
                {
                    HTTP_ERROR("Callback cancels");
                    client->resource = NULL;
                    client->resource_open = false;
                    return http_slice_fatal(client, result);
                }
                result = 0;
            }
            client->resource = NULL;
            client->resource_open = false;

            // absorb the boundary
            //
            client->in_transferred += client->boundary_length;
            client->in.count -= client->boundary_length;
            client->in.tail += client->boundary_length;

            // take the used data out of the content length for counted xfer
            //
            client->in_content_length -= client->boundary_length;

            // if this is the first boundary, add in a prefix CRFL to the boundary
            // text to ease matching subsequent boundaries. room is checked earlier
            //
            // note that the first char in the boundary is used also as boundary
            // state to discard preamble
            //
            if (client->boundary[0] != '\r')
            {
                memmove(client->boundary + 2, client->boundary, client->boundary_length + 1);
                client->boundary[0] = '\r';
                client->boundary[1] = '\n';
                client->boundary_length += 2;
            }
            // if the bytes after the boundary are "--" this is the end of the boundary
            // so move to postamble phase to discard remaining data.  there has to always
            // be two bytes after a boundary so we wait for them above before checking
            //
            if (
                    (client->in.data[client->in.tail] == '-')
                &&  (client->in.data[client->in.tail + 1] == '-')
            )
            {
                client->in.tail += 2;
                client->in.count -= 2;
                client->in_content_length -= 2;

                http_log(5, "-- ends boundary\n");

                // clear boundary to indicate postamble phase (discard remaining data)
                client->boundary[0] = '\1';
            }
            else if (
                    (client->in.data[client->in.tail] == '\r')
                &&  (client->in.data[client->in.tail + 1] == '\n')
            )
            {
                // absorb any CRLF after a proper boundary
                client->in.tail += 2;
                client->in.count -= 2;
                client->in_content_length -= 2;
            }
            // go parse the next part's content disposition if more data
            //
            if (client->in_content_length > 0 && client->boundary[0] != '\1')
            {
                http_get_line(client, httpMultipartHeaders);
            }
            break;
        }
        else if (
                (client->boundary_length > 0)
            &&  (client->boundary[0] != '\1')
        )
        {
            // if a boundary is in the buffer, or the buffer ends with a partial boundary,
            // limit input buffer size to bytes up-to the boundary. (exclude last CRLF
            // of boundary because last one doesn't have it)
            //
            int boundex, index;
            bool isboundary;

            for (
                    index = 0, isboundary = false;
                    index < client->in.count && ! isboundary;
            )
            {
                for (
                        boundex = 0, i = index, isboundary = true;
                        boundex < client->boundary_length && i < client->in.count;
                        boundex++, i++
                )
                {
                    if (client->in.data[i] != client->boundary[boundex])
                    {
                        // this isn't a boundary, restart checking at next byte
                        //
                        isboundary = false;
                        index = i + 1;
                        break;
                    }
                }
            }
            if (isboundary)
            {
                // got to end of input with a partial match, or found a complete boundary,
                // so limit parsing to begining of boundary in input
                //
                http_log(4, "partial boundary %02X reduce to %u\n", client->boundary[0], index);
                incount = index;

                // if the first boundary hasn't been parsed (preamble phase) discard
                // this data (this is never seen in practice)
                //
                if (client->boundary[0] == '-')
                {
                    http_log(4, "discard %u preamble\n", index);
                    i = incount;
                    client->in.count -= i;
                    client->in.tail += i;
                    if (client->in_content_length >= i)
                    {
                        client->in_content_length -= i;
                    }
                    else
                    {
                        client->in_content_length = 0;
                    }
                    incount = 0;
                }
            }
        }
        else if (
                (client->boundary_length > 0)
            &&  (client->boundary[0] == '\1')
        )
        {
            // discard data past last boundary termination
            //
            i = client->in_content_length;
            if (i > incount)
            {
                i = incount;
            }
            http_log(4, "discard %u postamble\n", i);
            client->in.count -= i;
            client->in.tail += i;
            client->in_content_length -= i;
        }
        #endif

        // give body data to callback, buffer is contiguous here, limit
        // length to remaining content_length or amount available
        //
        bodyCount = client->in_content_length;
        if (bodyCount > incount)
        {
            bodyCount = incount;
        }
        if (client->resource && client->resource->callback)
        {
            result = client->resource->callback(
                                                client,
                                                client->resource,
                                                httpDownloadData,
                                                &client->in.data,
                                                &bodyCount
                                                );
            if (result)
            {
                HTTP_ERROR("Callback cancels");
                return http_slice_fatal(client, result);
            }
        }
        else
        {
            http_log(5, "Discard %d body of %d content remaining\n", bodyCount, client->in_content_length);
        }
        // take amount of data absorbed by callback away from input
        // (which is all avail data if no callback supplied)
        //
        client->in_transferred += bodyCount;
        client->in.count -= bodyCount;
        client->in.tail += bodyCount;
        if (client->in.tail >= client->in.size)
        {
            client->in.tail = 0;
        }
        // take the used data out of the content length for counted xfer
        //
        if (client->in_content_length >= bodyCount)
        {
            client->in_content_length -= bodyCount;
        }
        else
        {
            client->in_content_length = 0;
        }
        break;

    case httpBodyUpload:
        if (client->resource && client->resource->callback)
        {
            uint8_t *content;
            size_t   count, room;

            // rotate output buffer to ensure max contiguous data area
            // to hand to upload callback. normally this is already aligned
            // but callback code might want to combine reply with data in one
            // chunk for performance/convenience, or combine incrementally
            // generated output for the same reasons, so out might have data
            // in it already. In any case, this is more performant for m
            // callback code vs. it having to check for head wrap, etc.
            //
            iostream_normalize_ring(&client->out, NULL);

            // point to out buffer the caller can use to fill data into
            //
            content = client->out.data + client->out.head;
            room    = client->out.size - client->out.count;

            // if running out of room flush the output buffer and come back here
            //
            if (
                    (room < (client->out.size / 4))
                ||  (room < 9)
            )
            {
                // running out of output room, flush it
                //
                result = http_send_out_data(client, httpBodyUpload, httpBodyUpload);
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
                return 0;
            }
            count = client->out_content_length;
            if (count == 0)
            {
                // there is no more data to send
                //
                if (client->out_transfer_type == httpChunked)
                {
                    if(client->out_transferred > 0)
                    {
                        // append a 0 count to terminate chunked, but only if there were
                        // actual bytes and a previous non-0 count
                        //
                        result = http_append_reply(client, "\r\n0\r\n");
                        if (result)
                        {
                            return http_slice_fatal(client, result);
                        }
                    }
                }
                http_log(4, "cl:%u: end of upload of %s\n", client->id, client->path);

                if (client->isclient)
                {
                    // client - flush any request-and-data and move to download state
                    result = http_send_out_data(client, httpSendRequest, httpReadReply);
                }
                else
                {
                    // serving client - flush reply-and-data and move to done state
                    result = http_send_out_data(client, httpSendReply, httpKeepAlive);
                }
                if (result)
                {
                    return http_slice_fatal(client, result);
                }
                return 0;
            }
            if (client->out_transfer_type == httpChunked)
            {
                // leave room for chunk count (room is > 8 here!)
                //
                content += 8;
                room -= 8;
            }
        #if 0 // not needed really, we check after that we didn't get too much
            // limit data we want to remaining output content for normal xfers
            //
            if (room > count)
            {
                room = count;
            }
        #endif
            #if HTTP_SUPPORT_RANGES
            // further limit request to bytes before byte range start
            //
            if (client->start_byte > 0)
            {
                if (client->start_byte > client->out_gotten)
                {
                    count = client->start_byte - client->out_gotten;
                    if (room > count)
                    {
                        room = count;
                    }
                }
            }
            #endif
            if (room > 0)
            {
                // have room for data, call callback to fill out buf
                //
                count = room;
                result = client->resource->callback(
                                                    client,
                                                    client->resource,
                                                    httpUploadData,
                                                    &content,
                                                    &count
                                                    );
                if (result)
                {
                    HTTP_ERROR("Upload callback cancels");
                    result = http_error_reply(client, 500, "Reply internal", false);
                    if (result)
                    {
                        return http_slice_fatal(client, result);
                    }
                    return 0;
                }
                client->out_gotten += count;

                #if HTTP_SUPPORT_RANGES
                if (client->start_byte > 0)
                {
                    if (client->start_byte >= client->out_gotten)
                    {
                        // haven't read past the start byte yet
                        count = 0;
                    }
                    else if (client->end_byte != 0)
                    {
                        if (client->out_gotten > client->end_byte)
                        {
                            room = client->out_gotten - (client->end_byte + 1);
                            if (room >= count)
                            {
                                // past the end byte, shouldn't get here since
                                // out content length should be set properly
                                //
                                HTTP_ERROR("Bad range handling");
                                count = 0;
                            }
                            else
                            {
                                // limit bytes uploaded to pieces in range
                                count -= room;
                            }
                        }
                    }
                }
                #endif

                bodyCount = count;
                if (count > 0 && client->out_transfer_type == httpChunked)
                {
                    // back annotate chunk count
                    snprintf((char *)content - 8, 8, "\r\n%04X\r", (uint32_t)bodyCount);
                    content[-1] = '\n';
                    bodyCount += 8;
                }
            }
            else
            {
                count = 0;
                bodyCount = 0;
            }
            if (bodyCount > 0)
            {
                // got data from callback. if it used our reply buffer for
                // the data, and there is still good room left in the reply buffer
                // keep going, else write the buffer out and come back for more
                // after that is done.  if the callback user didn't use our
                // supplied buffer, have to send reply now, then data, then
                // back to here
                //
                if (
                        (content < client->out.data)
                     || (content > (client->out.data + client->out.size))
                )
                {
                    HTTP_ERROR("Not implemented");
                    return http_slice_fatal(client, -1);
                }
                client->out.count += bodyCount;
                client->out.head += bodyCount;
                if (client->out.head >= client->out.size)
                {
                    client->out.head = 0;
                }
                // remove bytes uploaded from content_length
                //
                if (count > client->out_content_length)
                {
                    HTTP_ERROR("Xfer more than len");
                    count = client->out_content_length;
                }
                client->out_transferred += count;
                client->out_content_length -= count;
            }
        }
        else if (client->out.count)
        {
            // no resource, but pre-loaded data (reply, request, etc.) in buffer
            //
            if (client->isclient)
            {
                // client - flush any request-and-data and move to download state
                http_log(4, "cl:%u request with no body\n", client->id);
                result = http_send_out_data(client, httpSendRequest, httpReadReply);
            }
            else
            {
                // serving client - flush reply-and-data and move to done/keep-alive state
                http_log(4, "cl:%u reply with no body\n", client->id);
                result = http_send_out_data(client, httpSendReply, httpKeepAlive);
            }
            if (result)
            {
                return http_slice_fatal(client, result);
            }
        }
        break;

    case httpSendReply:
    case httpSendRequest:
        if (client->out.count > 0)
        {
            result = http_send_out_data(client, client->state, client->next_state);
            if (result)
            {
                return http_slice_fatal(client, result);
            }
        }
        else
        {
            if (client->state == httpSendRequest)
            {
                // request (and maybe the body) have been sent, get reply
                http_get_line(client, httpReadReply);
            }
            else
            {
                // reply sent, do whats next
                client->state = client->next_state;
            }
        }
        break;

    case httpPropFindEnumerate:
    #if HTTP_SUPPORT_WEBDAV
        result = http_webdav_find_slice(client);
        if (result)
        {
            // any errors that don't format a reply go to done state
            //
            if (client->state == httpPropFindEnumerate)
            {
                client->state = httpDone;
            }
            result = 0;
        }
    #endif
        break;

    case httpUserMethod:
        // call user callback as long as it wants us to
        result = http_make_user_method_callback(
                                    client,
                                    client->method,
                                    NULL
                                    );
        if (result)
        {
            HTTP_ERROR("user method callback cancels");
            client->resource = NULL;
            client->resource_open = false;
            return http_slice_fatal(client, result);
        }
        break;

    case httpKeepAlive:
        // if keep-alive, restart this, and set initial input timeout
        // as the keep-alive timeout
        //
        if (client->keepalive && ! client->isclient)
        {
            if (client->resource && client->resource->callback && client->resource_open)
            {
                result = client->resource->callback(
                                                client,
                                                client->resource,
                                                httpComplete,
                                                NULL,
                                                NULL
                                              );
                if (result < 0)
                {
                    HTTP_ERROR("callback cancels");
                    client->resource = NULL;
                    client->resource_open = false;
                    return http_slice_fatal(client, result);
                }
                result = 0;
            }
            client->resource = NULL;
            client->resource_open = false;

            client->long_timeout = HTTP_KEEPALIVE_TIMEOUT;
            client->state = httpServeInit;
        }
        else
        {
            if (client->transport == httpUDP)
            {
                client->state = httpServeInit;
            }
            else
            {
                client->state = httpDone;
            }
        }
        break;

    case httpDone:
        if (client->resource && client->resource_open && client->resource->callback)
        {
            result = client->resource->callback(
                                            client,
                                            client->resource,
                                            httpComplete,
                                            NULL,
                                            NULL
                                          );
            if (result < 0)
            {
                HTTP_ERROR("callback cancels");
                client->resource = NULL;
                client->resource_open = false;
                return http_slice_fatal(client, result);
            }
            result = 0;
        }
        client->resource = NULL;
        client->resource_open = false;
        break;

    default:
        HTTP_ERROR("Bad state");
        return http_slice_fatal(client, -1);
        break;

    }
    return 0;
}

int http_wait_for_client_event(http_client_t *client_list, int to_secs, int to_usecs)
{
    http_client_t *client;
    int result;

    result = 0;

    for (client = client_list; client; client = client->next)
    {
        if (! http_client_blocked(client))
        {
            return 1;
        }
    }
    for (client = client_list; client; client = client->next)
    {
        if (client->stream && http_client_blocked(client))
        {
            // if there is any data client wants to send, or waiting for a server
            // to respond to a connection then poll for writeability
            // else poll for readability if blocked on input data
            //
            if (client->out.count || client->state == httpClientInit)
            {
                result = client->stream->poll(client->stream, writeable, to_secs, to_usecs);
            }
            else
            {
                result = client->stream->poll(client->stream, readable, to_secs, to_usecs);
            }
            if (result)
            {
                // stop when any client indicates data-ready
                http_log(7, "cl:%u poll ok\n", client->id);
                break;
            }
        }
    }
    return result;
}

int http_client_request(
                        http_client_t *client,
                        http_method_t method,
                        const char *url,
                        http_transport_t transport,
                        bool use100,
                        const char *localpath,
                        http_resource_t *resource
                       )
{
    http_resource_t *resources;
    char scheme[HTTP_MAX_SCHEME];
    int result;

    // assume 1.1
    client->vmaj = 1;
    client->vmin = 1;

    result = butil_parse_url(url, &client->scheme,
            client->host, sizeof(client->host), &client->port,
            client->path, sizeof(client->path));
    if (result)
    {
        HTTP_ERROR("Bad url");
        return result;
    }
#if ! HTTP_SUPPORT_TLS
    if (
            client->scheme == schemeHTTPS
#if HTTP_SUPPORT_WEBSOCKET
        ||  client->scheme == schemeWSS
#endif
    )
    {
        HTTP_ERROR("TLS not supported");
        return -1;
    }
#endif
    if (localpath)
    {
        resources = NULL;
        result = http_add_file_resource(&resources, schemeHTTP, localpath, "./", NULL);
        if (result)
        {
            HTTP_ERROR("Can't add resource");
            return result;
        }
    }
    else
    {
        resources = resource;
    }
    client->resource = resources;
    client->method = method;
    client->transport = transport;

    if (use100)
    {
        client->use100 = true;
    }
    if (transport == httpTCP)
    {
        client->stream = iostream_create_from_tcp_connection(client->host, client->port);
    }
    else
    {
        socket_t socket;

        socket = iostream_create_udp_socket();
        if (socket != INVALID_SOCKET)
        {
            client->stream = iostream_create_from_socket(socket);
        }
        else
        {
            client->stream = NULL;
        }
    }
    if (! client->stream)
    {
        HTTP_ERROR("Can't make stream");
        http_resource_free(resources);
        return -1;
    }
    // go wait for client socket to connect
    client->state = httpClientInit;
    return 0;
}

socket_t http_create_server_socket(http_transport_t transport, uint16_t port, uint32_t max_connections)
{
    struct sockaddr_in serv_addr;
    socket_t sock;
    int result;

    sock = socket(AF_INET, (transport == httpTCP) ? SOCK_STREAM : SOCK_DGRAM, 0);
    if (sock < 0)
    {
        HTTP_ERROR("Can't create socket");
        return INVALID_SOCKET;
    }
    result = iostream_bind_socket(sock, port);
    if (result)
    {
        HTTP_ERROR("Can't bind server socket");
        close_socket(sock);
        return INVALID_SOCKET;
    }
    if (transport == httpTCP)
    {
        // Note: this is mostly useless on Linux which maintains
        // two separate queues for connections giving clients false hopes
        // if you see streams of failed requests consider upping the count
        //
        result = iostream_listen_socket(sock, max_connections);
        if (result < 0)
        {
            HTTP_ERROR("Can't listen on server socket");
            close_socket(sock);
            return INVALID_SOCKET;
        }
    }
    return sock;
}

static socket_t http_get_client_connection(socket_t sock)
{
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    socket_t clientsock;

    clilen = sizeof(cli_addr);
    clientsock = accept(sock, (struct sockaddr *)&cli_addr, &clilen);

    if (clientsock != INVALID_SOCKET)
    {
        #ifdef Windows
        unsigned long nonblock;
        #else
        uint32_t nonblock;
        #endif
        nonblock = 1;
        if (ioctl_socket(clientsock, FIONBIO, &nonblock) < 0)
        {
            HTTP_ERROR("can't make nonblocking");
            close_socket(clientsock);
            return INVALID_SOCKET;
        }
    }
    return clientsock;
}

static socket_t http_pending_client_connection(socket_t sock, int to_secs, int to_usecs)
{
    fd_set fds;
    int nfds;
    struct timeval timeout;
    int result;

    FD_ZERO(&fds);
    FD_SET((int)sock, &fds);
    nfds = (int)sock + 1;
    timeout.tv_sec = to_secs;
    timeout.tv_usec = to_usecs;
    result = select(
                    nfds,
                    &fds,
                    NULL,
                    NULL,
                    (to_secs >= 0 && to_usecs >= 0) ? &timeout : NULL
                   );
    http_log(8, "serv sel %08X\n", result);
    return result;
}

#if defined(OSX) || defined(Linux)
static void http_server_signal()
{
    return;
}
#endif

int http_server_init(
                        http_server_t *server,
                        http_resource_t *resources,
                        uint16_t port,
                        http_transport_t transport,
                        uint32_t max_connections,
                        bool secure
                      )
{
    if (! server)
    {
        HTTP_ERROR("No context to set up");
        return -1;
    }
    server->port = port;
    server->secure = secure;
    server->transport = transport;
    server->next = NULL;
    server->resources = resources;
    server->clients = NULL;
    server->connections = 0;
    server->max_connections = max_connections;
    server->aborted = false;

    // create server socket
    //
    server->socket = http_create_server_socket(transport, port, max_connections);
    if (server->socket == INVALID_SOCKET)
    {
        HTTP_ERROR("Can't make server socket");
        return -1;
    }
    if (transport == httpTCP)
    {
        // tcp, listen on port for connections
        //
        http_log(1, "listening TCP %son %u\n",
                server->secure ? "secure " : "", server->port);
        #if defined(OSX) || defined(Linux)
        signal(SIGPIPE, http_server_signal);
        #endif
    }
    else /* transport == udp */
    {
        #if HTTP_SUPPORT_UDP
        http_client_t *client;

        // udp, create a permanent client and use server socket for it
        //
        client = http_client_create(resources, false);
        if (! client)
        {
            HTTP_ERROR("Can't make UDP client");
        }
        client->stream = iostream_create_from_socket(server->socket);
        if (! client->stream)
        {
            HTTP_ERROR("Can't make stream");
            http_client_free(client);
            close_socket(server->socket);
            return -1;
        }
        client->transport = transport;
        client->socket = server->socket;
        server->clients = client;
        server->socket = INVALID_SOCKET;

        http_log(1, "serving UDP %son %u\n",
                server->secure ? "(secure) " : "", server->port);
        #else
        HTTP_ERROR("UDP not supported in build");
        return -1;
        #endif
    }
    return 0;
}

void http_server_cleanup(http_server_t *server)
{
    http_client_t *client;

    if (! server)
    {
        HTTP_ERROR("no server");
        return;
    }
    while (server->clients)
    {
        client = server->clients;
        server->clients = server->clients->next;
        server->connections--;

        if (client->stream)
        {
            client->stream->close(client->stream);
        }
    }
    if (server->socket != INVALID_SOCKET)
    {
        close_socket(server->socket);
    }
    server->socket = INVALID_SOCKET;
}

int http_server_abort(http_server_t *server)
{
    if (server)
    {
        server->aborted = true;
    }
    return 0;
}

int http_server_slice(http_server_t *server, int to_secs, int to_usecs)
{
    int result;
    bool clients_active;

    // check for connection in server socket
    //
    if (! server)
    {
        HTTP_ERROR("Need context with socket");
        return -1;
    }
    clients_active = false;

    if (server->clients)
    {
        http_client_t *client;
        bool found_dead;

        // iterate through client list procession any data / advancing state
        //
        for (client = server->clients; client; client = client->next)
        {
            if (client->stream)
            {
                result = http_client_slice(client);
                if (result || (client->state == httpDone))
                {
                    http_log(3, "client done on port %u\n", server->port);
                    if (client->stream)
                    {
                        client->stream->close(client->stream);
                        client->stream = NULL;
                    }
                    result = 0;
                }
            }
        }
        // iterate through client list culling dead clients
        //
        do
        {
            http_client_t *prevclient = NULL;

            found_dead = false;

            for (client = server->clients; client; client = client->next)
            {
                if (! client->stream)
                {
                    if (prevclient)
                    {
                        prevclient->next = client->next;
                    }
                    else
                    {
                        server->clients = client->next;
                    }
                    http_log(5, "culling client %u on port %u\n",
                           client->id, server->port);
                    http_client_free(client);
                    server->connections--;
                    found_dead = true;
                    break;
                }
                prevclient = client;
            }
        }
        while (found_dead);
    }
    // check for new client connections
    //
    if (server->connections < server->max_connections)
    {
        if (server->socket != INVALID_SOCKET)
        {
            result = http_pending_client_connection(server->socket, to_secs, to_usecs);
            if (result < 0)
            {
                HTTP_ERROR("server socket broken");
                return result;
            }
        }
        else
        {
            result = 0;
        }
        if (result > 0)
        {
            http_client_t *client;
            socket_t client_socket;

            client_socket = http_get_client_connection(server->socket);
            if (client_socket == INVALID_SOCKET)
            {
                HTTP_ERROR("Server socket broken");
                http_client_free(client);
                return -1;
            }
            client = http_client_create(server->resources, false);
            if (! client)
            {
                HTTP_ERROR("Can't alloc client");
                close_socket(client_socket);
                return -1;
            }
            client->socket = client_socket;
            client->secure = server->secure;

            client->stream = iostream_create_from_socket(client_socket);
            if (! client->stream)
            {
                HTTP_ERROR("Can't make stream");
                http_client_free(client);
                close_socket(client_socket);
                return -1;
            }
    #if HTTP_SUPPORT_TLS
            // upgrade to TLS if indicated
            //
            if (client->secure)
            {
                iostream_t *stream;

                stream = iostream_tls_create_from_iostream(client->stream, false);
                if (! stream)
                {
                    HTTP_ERROR("TLS upgrade failed");
                    client->stream = NULL;
                    http_client_free(client);
                    return 0;
                }
                client->stream = stream;
            }
    #endif
            client->next = server->clients;
            server->clients = client;
            server->connections++;

            http_log(2, "connection %u on port %u\n",
                   server->connections, server->port);
            result = 0;
        }
    }
    return result;
}

int http_wait_for_server_event(http_server_t *servers)
{
    http_server_t *server;
    http_client_t *client;
    int result;
    int client_count, server_count;
    int to_secs, to_usecs;

#if defined(Linux) || defined(OSX)
    fd_set rfds;
    fd_set wfds;
    fd_set efds;
    int nfds;
    struct timeval timeout;

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    nfds = 0;
    client_count = 0;

    to_secs = 10;
    to_usecs = 250000;

    // put all sockets in one big select and wait a long time, but timeout
    // occasionally to allow for long timeout processing in clients, etc.
    //
    for (server = servers, server_count = 0; server; server = server->next)
    {
        server_count++;
        for (client = server->clients; client; client = client->next)
        {
            if (client->socket != INVALID_SOCKET)
            {
                // if client has data pending, and isn't in a write-state
                // assume it can make progress and call it an event
                //
                if (! http_client_blocked(client))
                {
                    return 0;
                }
                // poll client socket for write if have data, else read
                if (client->socket > nfds)
                {
                    nfds = client->socket;
                }
                if (client->out.count)
                {
                    FD_SET((int)client->socket, &wfds);
                }
                else
                {
                    FD_SET((int)client->socket, &rfds);
                }
                FD_SET((int)client->socket, &efds);
            }
            client_count++;
        }
        if (server->socket != INVALID_SOCKET && client_count < server->max_connections)
        {
            // poll server socket for readability (new client connection)
            if (server->socket > nfds)
            {
                nfds = server->socket;
            }
            FD_SET((int)server->socket, &rfds);
            FD_SET((int)server->socket, &efds);
        }
    }
    timeout.tv_sec = to_secs;
    timeout.tv_usec = to_usecs;

    result = select(
                    nfds + 1,
                    &rfds,
                    &wfds,
                    &efds,
                    (to_secs >= 0 && to_usecs >= 0) ? &timeout : NULL
                   );
    if (result < 0)
    {
        // ignore error here, let individual poll find it
        result = 0;
    }
#else
    // wait for client and server events iteratively, waiting longer for
    // server events if no clients, etc.
    //
    result = 0;
    to_secs = 0;
    to_usecs = 8000;

    for (server = servers, server_count = 0; server; server = server->next)
    {
        server_count++;
        for (client = server->clients; client; client = client->next)
        {
            client_count++;
        }
        if (client_count > 0)
        {
            result = http_wait_for_client_event(server->clients, to_secs, to_usecs);
            if (result < 0)
            {
                // poll error, let client's own poll discover this and close it
                HTTP_ERROR("Client error");
                result = 0;
                break;
            }
            else if (result > 0)
            {
                return result;
            }
        }
    }
    if (! result)
    {
        if (server_count == 1 && client_count == 0)
        {
            to_secs = 1;
        }
        // no client event, wait for a client connection on each server
        //
        for (server = servers; server; server = server->next)
        {
            if (server->connections < server->max_connections)
            {
                result = http_pending_client_connection(server->socket, to_secs, to_usecs);
                if (result < 0)
                {
                    HTTP_ERROR("server socket broken");
                    return 0;
                }
                if (result > 0)
                {
                    break;
                }
            }
        }
    }
#endif
    return result;
}

int http_serve(http_server_t *servers, http_idle_callback_t on_idle, void *priv)
{
    http_server_t *server;
    http_server_t *prevserver;
    int result;

    do
    {
        // wait for something to do (new client, or client read/write ready)
        // in some systems, this is a one-by-one poll
        //
        result = http_wait_for_server_event(servers);
        if (result > 0)
        {
            result = 0;
        }
        prevserver = NULL;

        for (server = servers; ! result && server != NULL; server = server->next)
        {
            if (server->aborted)
            {
                if (server == servers)
                {
                    servers = server->next;
                }
                else
                {
                    prevserver->next = server->next;
                }
                http_server_cleanup(server);
                break;
            }
            result = http_server_slice(server, 0, 0);
            if (result < 0)
            {
                // remove server?
            }
            prevserver = server;
        }
        if (on_idle)
        {
            result = on_idle(priv);
            if (result)
            {
                butil_log(1, "Idle callback stops serving\n");
            }
        }
    }
    while (! result && servers);

    butil_log(4, "HTTP Server ends\n");
    return result;
}

