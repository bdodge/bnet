/*
 * Copyright 2019 Brian Dodge
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    upnp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef BUPNPUUID_H
#define BUPNPUUID_H 1

#define UUID_LENGTH (16)

typedef uint8_t uuid_t[UUID_LENGTH];

#define uuid_clear(a) memset((a), 0, UUID_LENGTH)
#define uuid_compare(a, b) memcmp((a), (b), UUID_LENGTH)
#define uuid_copy(dst, src) memcpy((dst), (src), UUID_LENGTH)

#define uuid_format(a, dst, ndst) \
	snprintf(dst, ndst, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x", 	\
			a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],a[10],a[11],a[12],a[13],a[14],a[15])
#endif
