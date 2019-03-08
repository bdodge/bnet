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
#ifndef BMIBC_H
#define BMIBC_H 1

#include "bnetheaders.h"

#if 0// define to include memory tracing debug
#define BMEM_TRACE_ALLOCS 1
#include "bmem.h"
#define malloc bmem_alloc
#define free bmem_free
#endif

// format strings for int64 types
#if defined(__IA64__) || defined(_WIN64) || defined(_M_IA64) || \
        defined(_M_X64) || defined(__x86_64__) || defined(X86_64) || defined(AMD64)
    // 64 bit CPU, ptrs and longs are 8 bytes, ints are 4
#include <inttypes.h>
#define LIFS    PRId64
#define LUFS    PRIu64
#else
#define LIFS    "ld"
#define LUFS    "lu"
#endif

#include "bmibcerr.h"
#include "bmibctoken.h"
#include "bmibckeywords.h"
#include "bmibcutils.h"
#include "bmibcsym.h"
#include "bmibcparse.h"

#endif

