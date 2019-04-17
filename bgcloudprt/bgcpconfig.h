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

/// Bytes in io buffers, should hold a whole gcp request/response
//
#define GCP_IO_SIZE         (8192)

/// URL to cloud
//
#define GCP_URL            "https://www.google.com/cloudprint"
//#define GCP_URL             "http://localhost:8080/cloudprint"

#define GCP_VER_MAJ         (2)
#define GCP_VER_MIN         (0)

#define GCP_MAX_TOKEN       (128)

#define GCP_MAX_CDS         (1024)
#define GCP_MAX_CDD         (8192)

#endif

