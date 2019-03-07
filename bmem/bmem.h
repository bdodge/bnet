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
#ifndef BMEM_H
#define BMEM_H 1

#include "bnetheaders.h"
#include "butil.h"

#ifndef BMEM_MULTITHREADED
    #define BMEM_MULTITHREADED (0)
#endif
#if (BMEM_MULTITHREADED)
    #include "bosal.h"
#endif
#if defined(__IA64__) || defined(_WIN64) || defined(_M_IA64) || \
        defined(_M_X64) || defined(__x86_64__) || defined(X86_64) || defined(AMD64)
    // 64 bit CPU, ptrs and longs are 8 bytes, ints are 4
    #define BMEM_PTR_SIZE    (8)
#else
    #define BMEM_PTR_SIZE    (4)
#endif

int     bmem_add_pool   (void *base, size_t bytes, size_t blocksize);
void    bmem_stats      (void);
int     bmem_init       (void);
int     bmem_deinit     (void);

#ifndef BMEM_TRACE_ALLOCS
    #define BMEM_TRACE_ALLOCS (1)
#endif

#if BMEM_TRACE_ALLOCS
void   *bmem_alloc_x    (size_t bytes, const char *file, size_t line);
void    bmem_free_x     (void *ptr);
#define bmem_alloc(bytes)   bmem_alloc_x(bytes, __FILE__, __LINE__)
#define bmem_free(ptr)      bmem_free_x(ptr)
#else
void   *bmem_alloc_x    (size_t bytes);
void    bmem_free_x     (void *ptr);
#define bmem_alloc(bytes)   bmem_alloc_x(bytes)
#define bmem_free(ptr)      bmem_free_x(ptr)
#endif

#endif
