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
#define MDNS_MAX_DNTEXT (256)

// Limit payload of RR
//
#define MDNS_MAX_TRTEXT (MDNS_IO_SIZE - 256)

// Number of distinct RR types we respond to: A,AAAA,PTR,SRV,TXT
//
#define MDNS_MAX_RRTYPEDEX  5

typedef struct tag_dns_label
{
    char        *name;                  ///< domain name component
    int         length;                 ///< length of name component
    int         alloc_len;              ///< allocated length of name component
}
dns_label_t;

typedef struct tag_dns_domain_name
{
    dns_label_t labels[MDNS_MAX_LABELS];///< list of components
    int         num_labels;             ///< count of labels set
    int         tot_len;                ///< total length of labels
    time_t      last_sent[MDNS_MAX_RRTYPEDEX]; ///< time, in seconds, this domain name output
}
dns_domain_name_t, dns_txt_records_t;

/// DNS RR Record descriptor
//
typedef struct tag_rr_rec
{
    dns_domain_name_t dname;
    uint16_t type;
    uint16_t clas;
    uint32_t ttl;
    uint16_t reslen;
    uint8_t  res[MDNS_MAX_TRTEXT];
}
dns_rr_rec_t;

// the only difference between dns names and txtrec storage is
// that dns names will have a 0 byte terminator
//
#define DNS_NAME_LENGTH(d) ((d)->tot_len + (d)->num_labels + 1)
#define DNS_TXT_LENGTH(d) ((d)->tot_len + (d)->num_labels)

struct tag_mdns_service;

typedef enum
{
    mdnsIPv4,
    mdnsIPv6,
    mdnsIPvALL
}
mdns_addrt_t;

typedef struct tag_dns_packet
{
    ioring_t            io;                     ///< io ring for parsing data
    uint8_t             data[MDNS_IO_SIZE];     ///< the packet itself

    mdns_addrt_t        dst_addrt;              ///< what type of ip to send pkt to (v4, v6, both)
    mdns_addrt_t        src_addrt;              ///< what type of ip pkt came from (v4, v6)
    bipv4addr_t         srcaddr4;               ///< source address of packet (if ipv4)
    bipv6addr_t         srcaddr6;               ///< source address of packet (if ipv6)
    uint16_t            srcport;                ///< source port of packet

    uint32_t            tts_secs;               ///< when to send, econds absolute
    uint32_t            tts_usecs;              ///< when to send, microseconds absolute
    bool                unicast;                ///< send unicast

    bool                added_srv;              ///< if there is an SRV record in the packet
    bool                added_srvptr;           ///< if there is a PTR to SRV record in the packet
    dns_domain_name_t   added_srvname;    ///< the domain name for service instance if added_srvptr

    /// header
    uint16_t            id;
    uint16_t            flags;
    uint16_t            qdcount;
    uint16_t            ancount;
    uint16_t            nscount;
    uint16_t            arcount;

    uint32_t            ttl;                    ///< TTL in seconds in cache for answer

    struct tag_dns_packet *next;        ///< link
}
mdns_packet_t;

/// State of mdns on a particular interface
//
typedef enum
{
    MDNS_PROBE_1,
    MDNS_PROBE_2,
    MDNS_PROBE_3,
    MDNS_PROBE_FLUSH,
    MDNS_ANNOUNCE_1,
    MDNS_ANNOUNCE_2,
    MDNS_RUN,
    MDNS_BYEBYE,
    MDNS_DONE
}
mdns_state_t;

/// Interface descriptor
//
typedef struct tag_in_interface
{
    dns_domain_name_t   hostname;       ///< interface's hostname
    mdns_state_t        state;          ///< current state
    struct tag_mdns_service
                       *services;       ///< list of managed services

    bipv4addr_t         ipv4addr;       ///< IPv4 address
    bipv6addr_t         ipv6addr;       ///< IPv6 address

    dns_domain_name_t   rev_ipv4;       ///< Rerverse domain mapping of ipv4 address
    dns_domain_name_t   rev_ipv6;       ///< Rerverse domain mapping of ipv6 address

    uint32_t            ttl;            ///< time to live
    socket_t            udpm4_sock;     ///< udp listenter bound to MDNS port 5353 (IPv4)
    socket_t            udpm6_sock;     ///< udp listenter bound to MDNS port 5353 (IPv6)
    socket_t            udpu4_sock;     ///< udp unicast bound to random high port (IPv4)
    socket_t            udpu6_sock;     ///< udp unicast bound to random high port (IPv6)
    mdns_packet_t      *inpkts;         ///< list of packets to look at from input
    mdns_packet_t      *outpkts;        ///< list of packets to send, sorted by delay

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

/// Service descriptor
//
typedef struct tag_mdns_service
{
    mdns_interface_t *interface;        ///< Interface service is providing its service on
    dns_domain_name_t usr_domain_name;  ///< FDQN with srv instance name, in a dns struct (like "My Pages._http._tcp.local")
    dns_domain_name_t srv_domain_name;  ///< FDQN with srv name, in a dns struct (like "_http._tcp.local")
    dns_domain_name_t sub_domain_name;  ///< FDQN with sub srv, in a dns struct (like "_universal._sub._http._tcp.local")
    dns_domain_name_t sd_domain_name;   ///< FDQN with dns-sd dicovery PTR, in a dns struct (like "_dns-sd._udp.local")
    mdns_service_protocol_t proto;      ///< Protocol (tcp or udp)
    uint16_t                port;       ///< service port (like 80)
    dns_txt_records_t       txt_records;///< service's txt records (like "txtvers=1")
    uint32_t                ttl;        ///< time to live for service answers
    uint32_t                probes;     ///< number of times probed (conflicted)
    bool                    add_srv;    ///< a SRV record was added to a reponse, append A and AAAA at end
    struct tag_mdns_service *next;      ///< link
}
mdns_service_t;

/// The responder
//
typedef struct tag_mdns_responder
{
    mdns_interface_t   *interfaces;     ///< list of managed interfaces
    dns_domain_name_t   domain_name;    ///< general use domain name for processing requests
    dns_rr_rec_t        known_answers[MDNS_MAX_ANSWERS]; ///< List of answers in query the match_
    int                 answer_count;   ///< count of known answers
    int                 to_secs;        ///< input poll dwell, seconds
    int                 to_usecs;       ///< input poll dwell, micro-seconds
    bool                stopped;        ///< request to stop run loop
    bool                fatal;          ///< a fatal error occured, exit
    bool                unit_testing;   ///< dont make sockets, etc.
    mdns_packet_t      *pkt_free;       ///< alloc packet list
    mdns_packet_t      *pkt_pool;       ///< alloc pool of packets
#if MDNS_TRACE_ALLOCS
    uint8_t            *label_pool;     ///< pool for label alloc if tracing
    uint8_t            *txt_pool;       ///< pool for txtrec alloc if tracing
#endif
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

int mdns_responder_slice        (mdns_responder_t *res);
int mdns_responder_run          (mdns_responder_t *res, int poll_secs, int poll_usecs);
int mdns_responder_stop         (mdns_responder_t *res);
int mdns_responder_add_interface(
                                mdns_responder_t *responder,
                                const char *hostname,
                                uint32_t     iface_index,
                                bipv4addr_t *ipv4addr,
                                bipv6addr_t *ipv6addr,
                                uint32_t ttl
                                );
int mdns_responder_add_service(
                                mdns_responder_t *responder,
                                mdns_interface_t *iface,
                                const char *srvname,
                                const char *dnsname,
                                const char *subname,
                                mdns_service_protocol_t proto,
                                uint16_t port,
                                const char *txtrecs,
                                uint32_t ttl
                                );
int mdns_responder_init(mdns_responder_t *responder);
int mdns_responder_deinit(mdns_responder_t *responder);

#endif
