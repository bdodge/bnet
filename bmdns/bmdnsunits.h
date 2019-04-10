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
#ifndef BMDNSUNITS_H
#define BMDNSUNITS_H 1

#include "bmdnsconfig.h"
#include "bnetheaders.h"
#include "bstreamio.h"
#include "butil.h"

// not part of public api
int mdns_compare_resource_to_name(uint8_t *answer, int anslen, dns_domain_name_t *dname);
int mdns_rr_index(uint16_t type);
int mdns_ask_question(
                            mdns_interface_t  *iface,
                            dns_domain_name_t *fqdn,
                            uint16_t type,
                            uint16_t clas,
                            mdns_packet_t *outpkt
                           );
int mdns_query_respond(
                            mdns_interface_t *iface,
                            dns_rr_rec_t *answers,
                            int answer_count,
                            mdns_packet_t *outpkt,
                            dns_domain_name_t *fqdn,
                            uint16_t type,
                            uint16_t clas,
                            mdns_service_t *service
                         );
int mdns_answer_question(
                            mdns_interface_t  *iface,
                            dns_domain_name_t *dname,
                            uint16_t type,
                            uint16_t clas,
                            dns_rr_rec_t *answers,
                            int answer_count,
                            mdns_packet_t *outpkt
                         );
int mdns_handle_input(mdns_responder_t *res, mdns_interface_t *iface, mdns_packet_t *inpkt);

int mdns_basic_unit_test(mdns_responder_t *responder);
int mdns_unit_test(mdns_responder_t *responder);

#endif
