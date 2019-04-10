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
#include "bmem.h"

// bitfield uses native type for int
//
#if (BMEM_PTR_SIZE == 8)
    #define BMEM_BITFIELD_TYPE  uint64_t
    #define BMEM_BITFIELD_SHIFT 6
    #define BMEM_BITFIELD_MASK  0x3F
    #define BMEM_HASH_TYPE uint64_t
    #define BMEM_HASH_SHIFT 32
#else
    #define BMEM_BITFIELD_TYPE  uint32_t
    #define BMEM_BITFIELD_SHIFT 5
    #define BMEM_BITFIELD_MASK  0x1F
    #define BMEM_HASH_TYPE uint32_t
    #define BMEM_HASH_SHIFT 22
#endif

// chunks should be aligned on a machine cache-line boundary typically
// this defined the power of 2 for chunk alignment
//
#ifndef BMEM_ALIGNMENT
    #define BMEM_ALIGNMENT (BMEM_BITFIELD_SHIFT) /* 32/64 byte alignment */
#endif

typedef BMEM_BITFIELD_TYPE bmapf_t;

#define BMEM_ALIGN_RND  ((1 << BMEM_ALIGNMENT) - 1)
#define BMEM_ALIGN_MASK (~BMEM_ALIGN_RND)

#define BMEM_ALIGN_ADDR(a) ((void*)((((bmapf_t)(a)) + BMEM_ALIGN_RND) & BMEM_ALIGN_MASK))

#define BMEM_BITS_PER_ENTRY (8 * sizeof(bmapf_t))

#if BMEM_TRACE_ALLOCS

/// Record of allocs done at a specific spot
//
typedef struct
{
    const char *file;       ///< file name where alloc took place
    size_t line;            ///< line where alloc took place
    size_t total;           ///< current allocs from this location
    size_t max;             ///< high water mark, this file
}
bmemfr_t;

/// Record of a single alloc
//
typedef struct tag_alloc_rec
{
    void *ptr;              ///< ptr that was allocated
    size_t size;            ///< bytes allocated
    size_t frec;            ///< file rec used at alloc
}
bmemar_t;

#ifndef BMEM_MAX_FRECS
#define BMEM_MAX_FRECS (128)
#endif
#ifndef BMEM_MAX_ARECS
#define BMEM_MAX_ARECS (1024*16)
#endif
#endif

typedef struct tag_mpool
{
    struct tag_mpool *next; /// list of pools
    void *orig_base;        ///< base address passed into create
    void *base_phys;        ///< base address (physical) of alloc arena
    void *base_virt;        ///< virtual address of alloc arena
    size_t bytes;           ///< bytes total avail for alloc
    size_t remain;          ///< remaining bytes avail for alloc
    size_t blocksize;       ///< size of each allocable block
    size_t blocks;          ///< total blocks avail for alloc
    size_t entries;         ///< number of entries in the bitfield "bits"
    bmapf_t *bits;          ///< bitfield location
    size_t firstfree;       ///< cached location of first free block
    #if BMEM_TRACE_ALLOCS
    size_t   maxalloc;      ///< high water mark, this pool
    size_t   nfrecs;        ///< number of tracked file alloc records
    size_t   narecs;        ///< number of tracked alloc records
    bmemfr_t frecs[BMEM_MAX_FRECS]; ///< file-line alloc records
    bmemar_t arecs[BMEM_MAX_ARECS]; ///< individual alloc records
    #endif
}
bmempool_t;

// hash table for locating pools from address within
// is also the allocator for pools themselves

// hash mask defines size of pool of pools
#define BMEM_HASH_MASK 63
#define BMEM_HASH(v) (((BMEM_HASH_TYPE)v >> BMEM_HASH_SHIFT) & (BMEM_HASH_MASK))

#define BMEM_POOL_SIZE (BMEM_HASH_MASK + 1)
static bmempool_t *s_pooltab[BMEM_POOL_SIZE];

// sorted index of pools by size
//
static bmempool_t *s_pztab;

#if BMEM_MULTITHREADED
static pthread_mutex_t s_memlock;
#define MEM_LOCK()     pthread_mutex_lock(&s_memlock)
#define MEM_UNLOCK()   pthread_mutex_unlock(&s_memlock)
#else
#define MEM_LOCK()     do { } while(0)
#define MEM_UNLOCK()   do { } while(0)
#endif

int bmem_add_pool(void *base, size_t bytes, size_t blocksize)
{
    bmempool_t *pp;
    BMEM_HASH_TYPE hash, init_hash;
    size_t entry_bytes, orig_bytes;
    void *orig_base;

    if (bytes < (blocksize + sizeof(bmempool_t)))
    {
        return -1;
    }
    // align base to some minimal alignment
    orig_base = base;
    orig_bytes = bytes;
    base = BMEM_ALIGN_ADDR(base);

    hash = init_hash = BMEM_HASH((BMEM_HASH_TYPE)base);
    MEM_LOCK();
    do
    {
        if (s_pooltab[hash] == NULL)
        {
            size_t actual_bytes;
            size_t entry_bytes;

            // use part of alloc area as record
            //
            pp = (bmempool_t *)base;
            pp->next = NULL;
            pp->orig_base = orig_base;

            s_pooltab[hash] = base;

            base = BMEM_ALIGN_ADDR(((uint8_t*)pp + sizeof(bmempool_t)));
            bytes -= sizeof(bmempool_t);

            // use the next-lower portion of memory for a bitfield to track allocs,
            // the size of which depends on the block size. we need 1 bit to
            // track each block so one byte to track 8 blocks. so the total
            // bytes are divided into (blocks + 7 / 8) for tracking
            // and blocks * blocksize for allocating from, so the total bytes
            //
            // ((blocks + 7) / 8) + blocks * blocksize = bytes
            //
            // blocks + 7 + blocks * blocksize * 8 = bytes * 8
            //
            // 7 + (1 + blocksize * 8) * blocks = bytes * 8
            //
            pp->blocksize = blocksize;
            pp->blocks = (bytes * 8 - 7) / (1 + blocksize * 8);
            pp->bits   = (bmapf_t *)base;

            // bytes need to track blocks
            //
            entry_bytes = (pp->blocks + 7) / 8;

            // place alloc areana, aligned, after these bytes
            //
            pp->base_phys = BMEM_ALIGN_ADDR((uint8_t*)pp->bits + entry_bytes);
            pp->base_virt = pp->base_phys;

            // re-calculate based on where things ended up
            //
            actual_bytes = bytes - ((uint8_t*)pp->base_phys - (uint8_t*)pp->orig_base);
            pp->blocks   = actual_bytes / pp->blocksize;
            pp->bytes    = pp->blocks * blocksize;

            entry_bytes = (pp->blocks + 7) / 8;
            pp->entries = (entry_bytes + sizeof(bmapf_t) - 1) / sizeof(bmapf_t);

            // sort pool into list by block size so allocs are done out of
            // the pool with the closest fit to alloc size
            //
            if (! s_pztab || (pp->blocksize < s_pztab->blocksize))
            {
                    pp->next = s_pztab;
                    s_pztab = pp;
            }
            else
            {
                bmempool_t *ppn, *ppv;

                for (ppn = s_pztab, ppv = NULL; ppn->next; ppn = ppn->next)
                {
                    if (ppn->next->blocksize > pp->blocksize)
                    {
                        pp->next = ppn->next;
                        ppn->next = pp;
                        break;
                    }
                    ppv = ppn;
                }
                if (! ppn->next)
                {
                    ppn->next = pp;
                }
            }

            pp->remain = pp->bytes;
            memset(pp->bits, 0, entry_bytes);
            pp->firstfree = 0;

            #if BMEM_TRACE_ALLOCS
            pp->nfrecs = 0;
            pp->narecs = 0;
            pp->maxalloc = 0;
            #endif

            butil_log(4, "Pool Init:%zu bytes created pool at %zu with %zu blocks of %zu for %zu\n",
                    orig_bytes, hash, pp->blocks, pp->blocksize, pp->bytes);

            MEM_UNLOCK();
            return 0;
        }
        if (hash == (BMEM_POOL_SIZE - 1))
        {
            hash = 0;
        }
        else
        {
            hash++;
        }
    }
    while (hash != init_hash);

    MEM_UNLOCK();
    return -1;
}

#if BMEM_TRACE_ALLOCS
static void bmem_trace_alloc(bmempool_t *pool, void *ptr, size_t bytes, const char *file, size_t line)
{
    size_t used;
    size_t rec;
    bool tracked;

    butil_log(7, "Alloc %p = %zu from Pool %p from %s:%zu\n", ptr, bytes, pool, file, line);

    // log the actual alloc
    if (pool->narecs < BMEM_MAX_ARECS)
    {
        pool->arecs[pool->narecs].ptr  = ptr;
        pool->arecs[pool->narecs].size = bytes;
        pool->arecs[pool->narecs].frec = BMEM_MAX_FRECS;
        pool->narecs++;
    }
    // log the file/line the alloc was made from
    for (rec = 0, tracked = false; rec < pool->nfrecs; rec++)
    {
        if (pool->frecs[rec].line == line && ! strcmp(pool->frecs[rec].file, file))
        {
            tracked = true;
            break;
        }
    }
    if (! tracked)
    {
        if (pool->nfrecs >= BMEM_MAX_FRECS)
        {
            butil_log(5, "Can't log file for alloc\n");
            return;
        }
        rec = pool->nfrecs++;
        pool->frecs[rec].file = file;
        pool->frecs[rec].line = line;
        pool->frecs[rec].total = 0;
        pool->frecs[rec].max = 0;
    }
    pool->frecs[rec].total += bytes;
    if (pool->frecs[rec].total > pool->frecs[rec].max)
    {
        pool->frecs[rec].max = pool->frecs[rec].total;
    }
    used = pool->bytes - pool->remain;
    if (used > pool->maxalloc)
    {
        pool->maxalloc = used;
    }
    // back annotate the frec used in the arec
    if (BMEM_MAX_ARECS > 0)
    {
        pool->arecs[pool->narecs - 1].frec = rec;
    }
}

static void bmem_trace_free(bmempool_t *pool, void *ptr)
{
    size_t rec;
    bool tracked;

    butil_log(7, "Free %p from Pool %p\n", ptr, pool);

    // de-log the actual alloc
    for (rec = 0; rec < pool->narecs; rec++)
    {
        if (pool->arecs[rec].ptr == ptr && pool->arecs[rec].size != 0)
        {
            // remove this many bytes from file/line record
            if (pool->arecs[rec].frec < pool->nfrecs)
            {
                pool->frecs[pool->arecs[rec].frec].total -=
                        pool->arecs[rec].size;
            }
            pool->arecs[rec].size = 0;
            return;
        }
    }
    if (BMEM_MAX_ARECS > 0)
    {
        butil_log(1, "Freeing unlogged ptr %p\n", ptr);
    }
}
#endif

void bmem_stats(int log_level)
{
    bmempool_t *pp;
    size_t hash, rec;

    for (hash = 0; hash < BMEM_POOL_SIZE; hash++)
    {
        pp = s_pooltab[hash];
        if (pp)
        {
            butil_log(log_level, "POOL %p  %zu bytes in %zu blocks. Used:%zu ",
                    pp, pp->bytes, pp->blocks, pp->bytes - pp->remain);
#if BMEM_TRACE_ALLOCS
            butil_log(log_level, "MaxUsed:%zu\n", pp->maxalloc);

            butil_log(log_level, "%20s %-8s    Total     Max\n", "File", "Line");

            for (rec = 0; rec < pp->nfrecs; rec++)
            {
                butil_log(log_level, "%20s:%-8zu %8zu %8zu\n",
                    pp->frecs[rec].file, pp->frecs[rec].line,
                    pp->frecs[rec].total, pp->frecs[rec].max);
            }
            if (pp->narecs > 0)
            {
                size_t nallocs;

                for (rec = 0, nallocs = 0; rec < pp->narecs; rec++)
                {
                    if (pp->arecs[rec].size > 0)
                    {
                        nallocs++;
                    }
                }
                if (nallocs > 0)
                {
                    butil_log(log_level, " List of %zu current allocations in pool %p\n", nallocs, pp);
                    for (rec = 0; rec < pp->narecs; rec++)
                    {
                        if (pp->arecs[rec].size > 0)
                        {
                            butil_log(log_level, "  %20s:%-8zu  %p %zu bytes\n",
                                    pp->frecs[pp->arecs[rec].frec].file,
                                    pp->frecs[pp->arecs[rec].frec].line,
                                    pp->arecs[rec].ptr,
                                    pp->arecs[rec].size);
                        }
                    }
                }
                else
                {
                    butil_log(log_level, "  No allocations in pool %p\n", pp);
                }
            }
#else
            butil_log(log_level, "\n");
#endif
        }
    }
}

static void *bmem_alloc_from_pool(bmempool_t *pool, size_t bytes)
{
    bmapf_t ent_offset;
    bmapf_t bit_offset;
    bmapf_t bit_mask;
    bmapf_t all_bits;
    bmapf_t bits_per;
    uint8_t *ptr;

    all_bits = (bmapf_t)0 - 1;
    bits_per = 8 * sizeof(bmapf_t);

    for (ent_offset = pool->firstfree; ent_offset < pool->entries; ent_offset++)
    {
        if (pool->bits[ent_offset] != all_bits)
        {
            for (
                    bit_offset = 0, bit_mask = 0x1;
                    bit_mask;
                    bit_offset++, bit_mask <<= 1
            )
            {
                    if (! (pool->bits[ent_offset] & bit_mask))
                    {
                        break;
                    }
            }
            if ((uint8_t*)&pool->bits[ent_offset] >= (uint8_t*)pool->base_phys)
            {
                volatile int a = ent_offset;
                volatile int b = a;
            }
            pool->bits[ent_offset] |= bit_mask;
            ptr = (uint8_t*)pool->base_phys +
                        ((ent_offset * bits_per + bit_offset) * pool->blocksize);
            pool->remain -= pool->blocksize;
            memset(ptr, 0, pool->blocksize);
            return (void*)ptr;
        }
    }
    return NULL;
}

static void bmem_free_from_pool(bmempool_t *pool, void *ptr)
{
    bmapf_t bit_offset;
    bmapf_t ent_offset;

    bit_offset = ((uint8_t *)ptr - (uint8_t *)pool->base_phys) / pool->blocksize;
    ent_offset = bit_offset >> BMEM_BITFIELD_SHIFT;
    bit_offset &= BMEM_BITFIELD_MASK;
    #if 0
    if ((uint8_t*)pool->base_phys + (((ent_offset << BMEM_BITFIELD_SHIFT) + bit_offset) * pool->blocksize) != ptr)
    {
        butil_log(0, "ptr has offset, not freeing\n");
        return;
    }
    #endif
    // clear the bit corresponding to the block
    if (!(pool->bits[ent_offset] & ((bmapf_t)1 << bit_offset)))
    {
        butil_log(0, "Attempt to free %p from [%u].%u which is not alloced\n",
                ent_offset, bit_offset);
    }
    pool->bits[ent_offset] &= ~((bmapf_t)1 << bit_offset);
    pool->remain += pool->blocksize;
    if (ent_offset < pool->firstfree)
    {
        pool->firstfree = ent_offset;
    }
}

#if BMEM_TRACE_ALLOCS
void *bmem_alloc_x(size_t bytes, const char *file, size_t line)
#else
void *bmem_alloc_x(size_t bytes)
#endif
{
    bmempool_t *pp;
    void *ptr;

    // find pool with chunk >= bytes by going through sorted list
    //
    ptr = NULL;

    MEM_LOCK();
    for (pp = s_pztab; pp; pp = pp->next)
    {
        if (pp->blocksize >= bytes)
        {
            if (pp->remain >= bytes)
            {
                ptr = bmem_alloc_from_pool(pp, bytes);
                break;
            }
        }
    }
    #if BMEM_TRACE_ALLOCS
    if (ptr)
    {
        bmem_trace_alloc(pp, ptr, bytes, file, line);
    }
    else
    {
        butil_log(4, "Failed to alloc %zu from %s:%zu\n", bytes, file, line);
    }
    #endif
    MEM_UNLOCK();
    return ptr;
}

void bmem_free_x(void *ptr)
{
    bmempool_t *pp;
    BMEM_HASH_TYPE hash, init_hash;

    if (! ptr)
    {
        // hacky, but matches malloc
        return;
    }
    hash = init_hash = BMEM_HASH((BMEM_HASH_TYPE)ptr);
    MEM_LOCK();
    do
    {
        pp = s_pooltab[hash];
        if (pp)
        {
            if (
                    (uint8_t *)pp->base_phys <= (uint8_t *)ptr
                 && (uint8_t *)ptr < ((uint8_t*)pp->base_phys + pp->bytes)
            )
            {
                bmem_free_from_pool(pp, ptr);
                #if BMEM_TRACE_ALLOCS
                bmem_trace_free(pp, ptr);
                #endif
                MEM_UNLOCK();
                return;
            }
        }
        if (hash == 0)
        {
            hash = BMEM_POOL_SIZE - 1;
        }
        else
        {
            hash--;
        }
    }
    while (hash != init_hash);
    MEM_UNLOCK();
    butil_log(0, "Freeing ptr %p is not in any pool\n", ptr);
    return;
}

int bmem_pool_overhead()
{
    return sizeof(bmempool_t);
}

int bmem_deinit()
{
    int result;

    result = 0;
    #if BMEM_MULTITHREADED
    result = pthread_mutex_destroy(&s_memlock);
    #endif
    s_pztab = NULL;
    return result;
}

int bmem_init()
{
    int result;

    result = 0;

    s_pztab = NULL;
    memset(s_pooltab, 0, sizeof(s_pooltab));

    #if BMEM_MULTITHREADED
    result = pthread_mutex_init(&s_memlock, NULL);
    #endif
    return result;
}

