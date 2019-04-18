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
#ifndef BGCPCONFIG_H
#define BGCPCONFIG_H 1

/// OAuth2 credentials
//
#define GCP_CLIENT_ID     "923224369786-m81b6t3vrehpr8an5uopp1ggeabsh8r8.apps.googleusercontent.com"
#define GCP_CLIENT_SECRET "DnDZ5u8ocEtXUrnsMMr_aGTs"

/// how often to poll for auth code (no sooner than 5 secs per spec)
//
#define GCP_AUTHCODE_POLL_PERIOD (6)

/// Bytes in io buffers, should hold a whole gcp request/response
//
#define GCP_IO_SIZE         (8192)

/// URL to cloud
//
#define GCP_URL            "https://www.google.com/cloudprint"
//#define GCP_URL             "http://localhost:8080/cloudprint"

#define GCP_VER_MAJ         (2)
#define GCP_VER_MIN         (0)

/// Authorization codes and Access tokens can be up to 2k bytes
//
#define GCP_MAX_TOKEN       (2048)

/// Most token/names are otherwise reasonable
//
#define GCP_SHORT_TOKEN     (128)

#define GCP_MAX_CDS         (1024)
#define GCP_MAX_CDD         (8192)

#endif

