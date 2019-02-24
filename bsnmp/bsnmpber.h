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
#ifndef BSNMPBER_H
#define BSNMPBER_H 1

#include "bsnmp.h"

int bsnmp_length_from_ber   (bsnmp_server_t *server, uint32_t *len);
int bsnmp_typecode_from_ber (bsnmp_server_t *server, bsnmp_type_t *typecode);
int bsnmp_typelen_from_ber  (bsnmp_server_t *server, bsnmp_type_t *typecode, uint32_t *len);
int bsnmp_request_from_ber  (bsnmp_server_t *server, bsnmp_opcode_t *opcode, uint32_t *len);
int bsnmp_struct_from_ber   (bsnmp_server_t *server, uint32_t *len);
int bsnmp_null_from_ber     (bsnmp_server_t *server, uint32_t *len);
int bsnmp_int32_from_ber    (bsnmp_server_t *server, int32_t *val);
int bsnmp_uint32_from_ber   (bsnmp_server_t *server, uint32_t *val);
int bsnmp_int64_from_ber    (bsnmp_server_t *server, int64_t *val);
int bsnmp_uint64_from_ber   (bsnmp_server_t *server, uint64_t *val);
int bsnmp_oid_from_ber      (bsnmp_server_t *server, bsnmp_oid_t *oid);
int bsnmp_bytes_from_ber    (bsnmp_server_t *server, uint8_t *val, uint32_t maxlen, uint32_t *lenout);
int bsnmp_string_from_ber   (bsnmp_server_t *server, char *val, uint32_t maxlen, uint32_t *lenout);

int bsnmp_ber_from_length   (bsnmp_server_t *server, uint32_t len, uint32_t *lenout);
int bsnmp_ber_reserve_length(bsnmp_server_t *server, uint32_t len);
int bsnmp_ber_from_typecode (bsnmp_server_t *server, const bsnmp_type_t typecode);
int bsnmp_ber_from_typelen  (bsnmp_server_t *server, const bsnmp_type_t typecode, const uint32_t lenin, uint32_t *lenout);
int bsnmp_ber_from_int32    (bsnmp_server_t *server, const bsnmp_type_t type, const int32_t val);
int bsnmp_ber_from_uint32   (bsnmp_server_t *server, const bsnmp_type_t type, const uint32_t val);
int bsnmp_ber_from_int64    (bsnmp_server_t *server, const bsnmp_type_t type, const int64_t val);
int bsnmp_ber_from_uint64   (bsnmp_server_t *server, const bsnmp_type_t type, const uint64_t val);
int bsnmp_ber_from_oid      (bsnmp_server_t *server, const bsnmp_oid_t *oid);
int bsnmp_ber_from_bytes    (bsnmp_server_t *server, const bsnmp_type_t type, const uint8_t *val, const uint32_t len);
int bsnmp_ber_from_string   (bsnmp_server_t *server, const char *val, const uint32_t len);

#endif

