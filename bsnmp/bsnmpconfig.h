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
#ifndef BSNMPCONIG_H
#define BSNMPCONIG_H 1

/// port for SNMP requests
//
#define SNMP_AGENT_PORT     161

/// port for traps
//
#define SNMP_MGMT_PORT      162

/// max community string
//
#define SNMP_MAX_COMMUNITY  64

/// maximum length of an OID handled
//
#define SNMP_MAX_OID 64

/// maximum array dimesionality handled
//
#define SNMP_MAX_DIMENSIONALITY	1

/// buffer size for reading snmp messages
//
#define SNMP_IO_SIZE 8192

#endif
