#ifndef BMEM_H
#define BMEM_H 1

#include "bnetheaders.h"
#include "butil.h"
#include "bosal.h"

#if defined(__IA64__) || defined(_WIN64) || defined(_M_IA64) || defined(_M_X64)
    // 64 bit CPU, ptrs and longs are 8 bytes, ints are 4
    #define BMEM_PTR_SIZE    8
#else
    #define BMEM_PTR_SIZE    4
#endif

int bmem_add_pool(void *base, size_t bytes, size_t blocksize);
void bmem_stats(void);
int bmem_init(void);
int bmem_deinit(void);

#ifndef BMEM_TRACE_ALLOCS
#define BMEM_TRACE_ALLOCS 1
#endif

#if BMEM_TRACE_ALLOCS
void *bmem_alloc_x(size_t bytes, const char *file, size_t line);
void bmem_free_x(void *ptr);
#define bmem_alloc(bytes) bmem_alloc_x(bytes, __FILE__, __LINE__)
#define bmem_free(ptr) bmem_free_x(ptr)
#else
void *bmem_alloc_x(size_t bytes);
void bmem_free_x(void *ptr);
#define bmem_alloc(bytes) bmem_alloc_x(bytes)
#define bmem_free(ptr) bmem_free_x(ptr)
#endif

#endif
