#define _GNU_SOURCE /* for thread affinity */
#include "bnetheaders.h"
#include "bosal.h"
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#ifndef OSX
#ifdef OSX
    // OSX posix threads only supports named semaphores
    //
    #define BOSAL_USE_NAMED_SEMS 1
#else
    #define BOSAL_USE_NAMED_SEMS 0
#endif

bos_intmask_t px_threads_disable()
{
	return 0;
}

void px_threads_enable(bos_intmask_t mask)
{
	return;
}

bos_thread_t px_thread_create(
                     int	 (*entry)(void *),
                     uint32_t    stacksize,
                     uint32_t    priority,
                     void       *arg,
                     const char *name
                    )
{
    pthread_t hthread;
    int result;

    result = pthread_create(&hthread, NULL, (void* (*)(void*))entry, arg);
    if (result)
    {
        return NULL;
    }
    return (bos_thread_t)hthread;
}

int px_thread_destroy(bos_thread_t thread)
{
    if (pthread_cancel((pthread_t)thread) != 0)
    {
        return -1;
    }
    pthread_join((pthread_t)thread, NULL);
    return 0;
}

bos_thread_t px_thread_self(void)
{
    return (bos_thread_t)pthread_self();
}

int px_thread_affinity(bos_thread_t thread, uint32_t cpu_mask)
{
    cpu_set_t cpuset;
    int i;

    CPU_ZERO(&cpuset);
    for (i = 0; cpu_mask; i++, cpu_mask >>= 1)
    {
        if (cpu_mask & 1)
        {
            CPU_SET(i, &cpuset);
        }
    }
    return pthread_setaffinity_np((pthread_t)thread, sizeof(cpu_set_t), &cpuset);
}

void px_sleep(uint32_t seconds, uint32_t microseconds)
{
    struct timespec ts;
    int result;

    ts.tv_sec = seconds;
    ts.tv_nsec = microseconds * 1000;
    while (ts.tv_nsec > 1000000000)
    {
        ts.tv_sec++;
        ts.tv_nsec -= 1000000000;
    }
    do
    {
        result = nanosleep(&ts, &ts);
    }
    while (result == -1 && errno == EINTR);
}

bos_mtx_t px_mutex_create()
{
    pthread_mutex_t *pmtx;
    int result;

    pmtx = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    if (! pmtx)
    {
        return NULL;
    }
    result = pthread_mutex_init(pmtx, NULL);
    if (result)
    {
        free(pmtx);
        pmtx = NULL;
    }
    return (bos_mtx_t)pmtx;
}

int px_mutex_destroy(bos_mtx_t mtx)
{
    pthread_mutex_t *pmtx = (pthread_mutex_t *)mtx;
    int result;

    result = pthread_mutex_destroy(pmtx);
    free(pmtx);
    return result;
}

int px_mutex_aquire(bos_mtx_t mtx, int secs, int usecs)
{
    pthread_mutex_t *pmtx = (pthread_mutex_t *)mtx;
    struct timespec ts;
    int result;

    if (secs > 0 || (secs == 0 && usecs > 0))
    {
        ts.tv_sec  = secs;
        ts.tv_nsec = usecs * 1000;
        result = pthread_mutex_timedlock(pmtx, &ts);
    }
    else
    {
        result = pthread_mutex_lock(pmtx);
    }
    return result;
}

int px_mutex_release(bos_mtx_t mtx)
{
    pthread_mutex_t *pmtx = (pthread_mutex_t *)mtx;

    return pthread_mutex_unlock(pmtx);
}

bos_sem_t px_semaphore_create(uint32_t count)
{
    sem_t *psem;
    static uint32_t s_numsems = 0;
#if BOSAL_USE_NAMED_SEMAPHORES
    char semname[32];

    snprintf(semname, 32, "sem%u", s_numsems);
    sem_unlink(semname);

    psem = sem_open(semname, O_CREAT, 0644, count);
#else
    psem = (sem_t *)malloc(sizeof(sem_t));
    if (psem)
    {
        sem_init(psem, PTHREAD_PROCESS_PRIVATE, count);
    }
#endif
    if (psem)
    {
        s_numsems++;
    }
    return (bos_sem_t)psem;
}

int px_semaphore_destroy(bos_sem_t sem)
{
    int result;

#if BOSAL_USE_NAMED_SEMAPHORES
    result = sem_close((sem_t *)sem);
#else
    result = sem_destroy((sem_t *)sem);
    free((sem_t *)sem);
#endif
    return result;
}

int px_semaphore_get(bos_sem_t sem, int secs, int usecs)
{
    struct timespec ts;
    int result;

    do
    {
        if (secs < 0 || (secs == 0 && usecs < 0))
        {
            result = sem_wait((sem_t *)sem);
        }
        else
        {
            clock_gettime(CLOCK_REALTIME, &ts);

            ts.tv_sec  += secs;
            ts.tv_nsec += usecs * 1000;
            while (ts.tv_nsec > 1000000000)
            {
                ts.tv_sec++;
                ts.tv_nsec -= 1000000000;
            }
            result = sem_timedwait((sem_t *)sem, &ts);
            if (result)
            {
                if (errno == ETIMEDOUT)
                {
                    return 1;
                }
            }
        }
    }
    while (result < 0 && errno == EINTR);

    return result;
}

int px_semaphore_put(bos_sem_t sem)
{
    return sem_post((sem_t *)sem);
}

static bool s_iface_initialized = false;

static bos_interface_t s_bosal_posix_interface =
{
  px_threads_disable,
  px_threads_enable,
  px_thread_create,
  px_thread_destroy,
  px_thread_self,
  px_thread_affinity,
  px_sleep,
  px_mutex_create,
  px_mutex_destroy,
  px_mutex_aquire,
  px_mutex_release,
  px_semaphore_create,
  px_semaphore_destroy,
  px_semaphore_get,
  px_semaphore_put,
  // use standard msgq code
  bosal_msgq_create,
  bosal_msgq_destroy,
  bosal_msgq_send,
  bosal_msgq_receive,
  bosal_msgq_count
};

bos_interface_t *bosal_interface()
{
    if (! s_iface_initialized)
    {
    }
    return &s_bosal_posix_interface;
}
#endif
