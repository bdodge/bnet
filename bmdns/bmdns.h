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
#ifndef BMDNS_H
#define BMDNS_H 1

#include "bmdnsconfig.h"
#include "bnetheaders.h"
#include "bstreamio.h"
#include "butil.h"

typedef struct tag_dns_packet
{
    uint32_t tts_sec;    ///< when to send, seconds absolute
    uint32_t tts_usec;   ///< when to send, microseconds absolute
    struct tag_dns_packet *next;
}
mdns_packet_t;

typedef struct tag_in_interface
{
    struct tag_in_interface *next;
}
mdns_interface_t;

typedef struct tag_mdns_service
{
    struct tag_mdns_service *next;
}
mdns_service_t;

typedef struct tag_mdns_responder
{
    mdns_service_t *service; ///< list of service to be announced/discovered
    mdns_interface_t *interfaces; ///< list of interfaces being managed

    socket_t in_sock;  ///< udp listenter bound to MDNS port 5353
    socket_t out_sock; ///< udp multicast sender bound to high port

    mdns_packet_t *inpkts; ///< list of packets to look at
    mdns_packet_t *outpkts; ///< list of packets to send, sorted by delay

    ioring_t    in;       ///< stream buffer on a packet being parsed
    ioring_t    out;      ///< stream buffer on a packet being assembled
}
mdns_responder_t;

int mdns_responder_init(mdns_responder_t *responder);
int mdns_responder_deinit(mdns_responder_t *responder);

#endif
