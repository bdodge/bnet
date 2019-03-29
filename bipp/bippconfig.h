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
#ifndef BIPPCONFIG_H
#define BIPPCONFIG_H 1

/// Is PDF supported
//
#define IPP_SUPPORT_PDF       (0)

/// Is JPEG supported
//
#define IPP_SUPPORT_JPEG      (1)

/// Is Plain Text supported
//
#define IPP_SUPPORT_PLAINTEXT (1)


/// Bytes in io buffersm should hold a whole TCP/IP packet
//
#define IPP_IO_SIZE         (2048)

/// Size of job pool. IPP wants to know about historic
/// jobs so this should be larger than the jobs in-flight
//
#define IPP_MAX_JOBS        (32)

/// Size of request pool. max number of concurrent ipp
/// requests in process. If this is more than one, then
/// locking is implemented for db access
//
#define IPP_MAX_REQUESTS    (1)

/// version supported range
///
/// minimum major/minor
///
#define IPP_MAJOR_VERSION_MIN   (1)
#define IPP_MINOR_VERSION_MIN   (1)

#define IPP_MAJOR_VERSION_MAX   (2)
#define IPP_MINOR_VERSION_MAX   (2)

/// how long a job can go without data before it gets aborted
//
#define IPP_MAX_JOB_IDLE_SECONDS (10)

#endif

