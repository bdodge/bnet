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
#include "bmdns.h"

int mdns_responder_init(mdns_responder_t *responder)
{
    int result;

    if (! responder)
    {
        return -1;
    }
    memset(responder, 0, sizeof(mdns_responder_t));

    responder->in_sock = INVALID_SOCKET;
    responder->out_sock = INVALID_SOCKET;

    // create a udp socket in port 5353 to listen
    //
    responder->in_sock = iostream_create_udp_socket();
    if (responder->in_sock == INVALID_SOCKET)
    {
        butil_log(0, "Can't make in socket\n");
        return -1;
    }
    result = iostream_bind_socket(responder->in_sock, MDNS_PORT);
    if (result)
    {
        butil_log(0, "Can't bind in socket\n");
        return -1;
    }
    return result;
}

int mdns_responder_deinit(mdns_responder_t *responder)
{
    if (! responder)
    {
        return -1;
    }
    if (responder->in_sock != INVALID_SOCKET)
    {
        close_socket(responder->in_sock);
    }
    if (responder->out_sock != INVALID_SOCKET)
    {
        close_socket(responder->out_sock);
    }
    return 0;
}

