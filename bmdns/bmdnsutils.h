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

const char *str_for_sockaddr	(struct sockaddr_in *sockaddr);
mdns_packet_t *mdns_pkt_alloc	(mdns_responder_t *res);;
void mdns_pkt_free				(mdns_responder_t *res, mdns_packet_t *pkt);
void mdns_dump_packet			(const char *because, mdns_packet_t *pkt, int level);

int mdns_read_uint8    	(ioring_t *in, uint8_t *val);
int mdns_read_uint16   	(ioring_t *in, uint16_t *val);
int mdns_read_uint32   	(ioring_t *in, uint32_t *val);

int mdns_read_int8    	(ioring_t *in, int8_t *val);
int mdns_read_int16   	(ioring_t *in, int16_t *val);
int mdns_read_int32     (ioring_t *in, int32_t *val);

int mdns_read_text      (ioring_t *in, char *text, uint16_t len);

int mdns_write_uint8    (ioring_t *in, uint8_t val);
int mdns_write_uint16   (ioring_t *in, uint16_t val);
int mdns_write_uint32   (ioring_t *in, uint32_t val);

int mdns_write_int8     (ioring_t *in, int8_t val);
int mdns_write_int16    (ioring_t *in, int16_t val);
int mdns_write_int32    (ioring_t *in, int32_t val);

int mdns_write_bytes	(ioring_t *out, const uint8_t *bytes, int len);
int mdns_write_text     (ioring_t *out, const char *text, int len);

#endif

