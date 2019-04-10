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
#ifndef BMDNSUTILS_H
#define BMDNSUTILS_H 1

#include "bmdnsconfig.h"
#include "bmdns.h"
#include "bnetheaders.h"
#include "bstreamio.h"
#include "butil.h"

#if MDNS_TRACE_ALLOCS
#include "bmem.h"
// for tracing memory use
#define mdns_alloc(r, z) bmem_alloc(z)
#define mdns_free(r, p) bmem_free(p)
#else
// normal alloc
#define mdns_alloc(r, z) malloc(z)
#define mdns_free(r, p) free(p)

#endif

int mdns_compare_nametext	(const char *ours, const char *theirs, int len);
int mdns_compare_names      (const dns_domain_name_t *ours, const dns_domain_name_t *theirs);
char *mdns_label_alloc		(mdns_responder_t *res, int size);
void mdns_label_free		(mdns_responder_t *res, char *labelname);
int mdns_copy_name			(mdns_responder_t *res, dns_domain_name_t *dstname, dns_domain_name_t *srcname);
int mdns_clear_name			(mdns_responder_t *res, dns_domain_name_t *dname);
int mdns_init_name			(dns_domain_name_t *dname);

int mdns_get_wall_time		(uint32_t *secs, uint32_t *usecs);

const char *mdns_reverse_ipv4addr	(bipv4addr_t *ipv4addr);
const char *mdns_reverse_ipv6addr	(bipv6addr_t *ipv6addr);

uint32_t 	mdns_get_rnd_delay_ms	(uint32_t max_delay);
uint32_t 	mdns_get_pkt_delay_ms	(mdns_packet_t *pkt);
const char *mdns_str_for_domain_name(const dns_domain_name_t *dname);
const char *mdns_srv_proto_name     (mdns_service_protocol_t proto);
const char *mdns_str_for_sockaddr   (struct sockaddr *sockaddr);
const char *mdns_str_for_pktaddr   	(mdns_packet_t *pkt);

void *mdns_alloc_x			(mdns_responder_t *res, int size);
void mdns_free_x			(mdns_responder_t *res, void *ptr);
void *mdns_large_alloc		(mdns_responder_t *res, int size);
void mdns_large_free		(mdns_responder_t *res, void *ptr);

mdns_packet_t *mdns_pkt_alloc       (mdns_responder_t *res);
void mdns_pkt_free                  (mdns_responder_t *res, mdns_packet_t *pkt);
void mdns_dump_packet               (const char *because, mdns_packet_t *pkt, int level);

mdns_interface_t *mdns_interface_alloc  (mdns_responder_t *res, const char *hostname);
void mdns_interface_free            (mdns_responder_t *res, mdns_interface_t *iface);
void mdns_dump_interface            (mdns_interface_t *iface, int level);

mdns_service_t *mdns_service_alloc  (mdns_responder_t *res);
void mdns_service_free              (mdns_responder_t *res, mdns_service_t *service);
void mdns_dump_service              (mdns_service_t *service, int level);

int mdns_read_uint8         (ioring_t *in, uint8_t *val);
int mdns_read_uint16        (ioring_t *in, uint16_t *val);
int mdns_read_uint32        (ioring_t *in, uint32_t *val);

int mdns_read_int8          (ioring_t *in, int8_t *val);
int mdns_read_int16         (ioring_t *in, int16_t *val);
int mdns_read_int32         (ioring_t *in, int32_t *val);

int mdns_read_name          (mdns_responder_t *res, ioring_t *in, dns_domain_name_t *dname);

int mdns_write_uint8        (ioring_t *in, uint8_t val);
int mdns_write_uint16       (ioring_t *in, uint16_t val);
int mdns_write_uint32       (ioring_t *in, uint32_t val);

int mdns_write_int8         (ioring_t *in, int8_t val);
int mdns_write_int16        (ioring_t *in, int16_t val);
int mdns_write_int32        (ioring_t *in, int32_t val);

int mdns_flatten_name       (const dns_domain_name_t *dname, char *name, int nname);
int mdns_unflatten_name		(mdns_responder_t *res, const char *name, dns_domain_name_t *dname);
int mdns_unflatten_txt		(mdns_responder_t *res, const char *name, dns_txt_records_t *txtrec);
int mdns_assemble_name      (char *buffer, int nbuffer, int ncomponents, ...);
int mdns_write_text         (ioring_t *out, const dns_txt_records_t *txtrec);
int mdns_write_name         (ioring_t *in, const dns_domain_name_t *dname, int compress_start);

#endif

