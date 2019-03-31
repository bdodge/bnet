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
#ifndef BMDNSCONFIG_H
#define BMDNSCONFIG_H 1

/// Port
//
#define MDNS_PORT			(5353)

/// Multicast group IP4 Address
//
#define MDNS_MCAST_IP4ADDR 	"224.0.0.251"

/// Packet max size (UDP packet payload)
//
#define MDNS_IO_SIZE		(1500)

/// Packet pool size
//
#define MDNS_PACKET_POOL_SIZE	(8)

#endif

