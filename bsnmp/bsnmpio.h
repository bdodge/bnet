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
#ifndef SNMPIO_H
#define SNMPIO_H 1

#include "bsnmp.h"

int bsnmp_next_byte                 (bsnmp_server_t *server, uint8_t *b);
int bsnmp_out_byte                  (bsnmp_server_t *server, const uint8_t b);
int bsnmp_stream_save_in_state      (bsnmp_server_t *server, ioring_t *iostate);
int bsnmp_stream_restore_in_state   (bsnmp_server_t *server, ioring_t *iostate);
int bsnmp_stream_save_out_state     (bsnmp_server_t *server, ioring_t *iostate);
int bsnmp_stream_restore_out_state  (bsnmp_server_t *server, ioring_t *iostate);
int bsnmp_stream_annotate_length    (bsnmp_server_t *server, ioring_t *frompos, uint32_t bytes_reserved);
int bsnmp_stream_init               (bsnmp_server_t *server, uint16_t version, uint16_t port);
int bsnmp_stream_read               (bsnmp_server_t *server, int tos, int tous);
int bsnmp_stream_write              (bsnmp_server_t *server, int tos, int tous);

#endif
