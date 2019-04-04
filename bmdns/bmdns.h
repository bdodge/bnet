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


// RFC 1035 Limits
//
#define MDNS_MAX_LABEL  (64)
#define MDNS_MAX_TEXT   (256)

typedef struct tag_dns_label
{
    char        name[MDNS_MAX_LABEL];   ///< domain name component
    int         length;                 ///< length of name component
    int         offset;                 ///< offset into reply where located
}
dns_label_t;

typedef struct tag_dns_domain_name
{
    dns_label_t labels[MDNS_MAX_LABELS];///< list of components
    int         num_labels;             ///< count of labels set
    int         tot_len;                ///< total length of labels
}
dns_domain_name_t;


typedef struct tag_dns_packet
{
    ioring_t    io;                     ///< io ring for parsing data
    uint8_t     data[MDNS_IO_SIZE];     ///< the packet itself

    struct sockaddr_in  srcaddr;        ///< source address of packet
    uint32_t tts_msec;                  ///< when to send, milliseconds absolute

    /// header
    uint16_t    id;
    uint16_t    flags;
    uint16_t    qdcount;
    uint16_t    ancount;
    uint16_t    nscount;
    uint16_t    arcount;

    uint32_t    ttl;                    /// TTL in seconds in cache for answer

    struct tag_dns_packet *next;        ///< link
}
mdns_packet_t;

typedef struct tag_in_interface
{
    dns_domain_name_t hostname;         ///< interface's hostname
    uint32_t          ipv4addr;         ///< IPv4 Address
    struct tag_in_interface *next;      ///< link
}
mdns_interface_t;

typedef enum
{
    MDNS_SRVPROTO_NONE,
    MDNS_SRVPROTO_UDP,
    MDNS_SRVPROTO_TCP
}
mdns_service_protocol_t;

typedef struct tag_mdns_service
{
    dns_domain_name_t usr_domain_name;  ///< FDQN with user name, in a dns struct (like "My Pages._http._tcp.local")
    dns_domain_name_t srv_domain_name;  ///< FDQN with srv name, in a dns struct (like "_http._tcp.local")
    dns_domain_name_t sub_domain_name;  ///< FDQN with sub srv, in a dns struct (like "_universal._sub._http._tcp.local")
    mdns_service_protocol_t proto;      ///< Protocol (tcp or udp)
    uint16_t                port;       ///< service port (like 80)
    struct tag_mdns_service *next;      ///< link
}
mdns_service_t;

typedef struct tag_mdns_responder
{
    mdns_interface_t *interfaces;       ///< list of managed interfaces
    mdns_service_t   *services;         ///< list of managed services

    socket_t in_sock;                   ///< udp listenter bound to MDNS port 5353
    socket_t out_sock;                  ///< udp multicast sender bound to high port

    mdns_packet_t *inpkts;              ///< list of packets to look at
    mdns_packet_t *outpkts;             ///< list of packets to send, sorted by delay

    mdns_packet_t *pkt_free;            ///< alloc packet list
    mdns_packet_t *pkt_pool;            ///< alloc pool of packets
}
mdns_responder_t;

// headers offsets in packet for bits
#define MDNS_OFF_HEADER         (0)
#define MDNS_OFF_QUESTION       (12)

// header flag bits
#define MDNS_FLAG_QR(f)         ((f) & (1 << 15))
#define MDNS_FLAG_OPCODE(f)     (((f) >> 14) & 0xF)
#define MDNS_FLAG_AA(f)         ((f) & (1 << 10))
#define MDNS_FLAG_TC(f)         ((f) & (1 << 9))
#define MDNS_FLAG_RD(f)         ((f) & (1 << 8))
#define MDNS_FLAG_RA(f)         ((f) & (1 << 7))
#define MDNS_FLAG_Z(f)          (((f) >>4) & 0x3)
#define MDNS_FLAG_RCODE(f)      (((f) & 0xF)
#define MDNS_FLAG_ALL           (0xFFFF)

// record types
#define DNS_RRTYPE_A            (1)         // Host IPv4 address
#define DNS_RRTYPE_NS           (2)         // Authoritative name server
#define DNS_RRTYPE_MD           (3)         // deprecated
#define DNS_RRTYPE_MF           (4)         // deprecated
#define DNS_RRTYPE_CNAME        (5)         // Alias
#define DNS_RRTYPE_SOA          (6)         // Start Of Authority
#define DNS_RRTYPE_MB           (7)         // Mailbox (EXPERIMENTAL)
#define DNS_RRTYPE_MG           (8)         // Mail Group (EXPERIMENTAL)
#define DNS_RRTYPE_MR           (9)         // Mail Rename (EXPERIMENTAL)
#define DNS_RRTYPE_NULL         (10)        // Null (EXPERIMENTAL)
#define DNS_RRTYPE_WKS          (11)        // Well Known Service
#define DNS_RRTYPE_PTR          (12)        // domain name pointer
#define DNS_RRTYPE_HINFO        (13)        // Host Info
#define DNS_RRTYPE_MINFO        (14)        // Mailbox Info
#define DNS_RRTYPE_MX           (15)        // Mail eXchange
#define DNS_RRTYPE_TXT          (16)        // Text Record
#define DNS_RRTYPE_AAAA         (28)        // Host IPv6 address
#define DNS_RRTYPE_SRV          (33)        // Service
#define DNS_RRTYPE_ANY          (255)       // Any

#define DNS_CLASS_IN            (1)         // Internet
#define DNS_CLASS_ANY           (0xFF)      // Any
#define DNS_CLASS_FLUSH         (0x800)     // Flush

int mdns_responder_run(mdns_responder_t *res);
int mdns_responder_add_interface(
                                mdns_responder_t *responder,
                                const char *hostname,
                                uint32_t ipv4addr
                                );
int mdns_responder_add_service(
                                mdns_responder_t *responder,
                                const char *srvname,
                                const char *dnsname,
                                mdns_service_protocol_t proto,
                                uint16_t port
                                );
int mdns_responder_init(mdns_responder_t *responder);
int mdns_responder_deinit(mdns_responder_t *responder);

#endif
