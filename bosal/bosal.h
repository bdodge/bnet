#ifndef BOSAL_H
#define BOSAL_H

#include "bosal_config.h"

#ifndef BOSAL_DEBUG_LOCKS
    #define BOSAL_DEBUG_LOCKS 0
#endif
#ifndef BOSAL_DEBUG_QUEUES
    #define BOSAL_DEBUG_QUEUES 0
#endif
#ifndef BOSAL_DEBUG_THREADS
    #define BOSAL_DEBUG_THREADS 0
#endif
#ifndef BOSAL_THREAD_TRACE
    #define BOSAL_THREAD_TRACE 0
#endif

#define BOSAL_INVALIDMTX      NULL
#define BOSAL_INVALIDSEM      NULL
#define BOSAL_INVALIDTHREAD   NULL
#define BOSAL_INVALIDMSGQ     NULL

///  Msg - holds a mail message
///
typedef struct tag_msg
{
    uint32_t msg;     	///< First parameter or message code
    uint32_t param;		///< Second parameter
    uint32_t flags;		///< messsage flags
    union             	///< value
    {
    	int32_t   ival;
      	uint32_t  ulval;
        int64_t   llval;
        uint64_t  ullval;
        void     *ptrval;
    }
    value;
}
bos_msg_t;

/// Mailbox handle
typedef void *bos_msgq_t;

/// Thread handle
typedef void *bos_thread_t;

/// Mutex handle
typedef void *bos_mtx_t;

/// Semaphore handle
typedef void *bos_sem_t;

#define BOSAL_LOWEST_PRIORITY     0
#define BOSAL_LOW_PRIORITY        7
#define BOSAL_MIDDLE_PRIORITY     15
#define BOSAL_MEDIUM_PRIORITY     22
#define BOSAL_HIGHEST_PRIORITY    31

/// mask for holding interrupt flags
///
typedef void *bos_intmask_t;

/// Disable thread switching (h/w interrupts may or may not be disabled)
///
/// @returns state of task interrupts before turning off to be used to restore
///
typedef bos_intmask_t (*bos_ints_off_t)(void);

/// Re-enable thread switching (and possibly h/w interrupts)
///
/// @param bos_intmask_t [in] mask returned from the call to bos_intsoff_t function
///
typedef void (*bos_ints_on_t)(bos_intmask_t);

/// Create a thread and execute it
///
/// @param entry        [in] thread function
/// @param stacksize    [in] size (bytes) of stack needed
/// @param priority     [in] thread priority
/// @param arg,         [in] argument to pass to thread function
/// @param name         [in] name of thread (for debug)
///
/// @returns the id of the created task or INVALIDTHREAD on error
///
typedef bos_thread_t (*bos_thread_create_t)(
                                int   		(*entry)(void *),
                                uint32_t  	stacksize,
                                uint32_t  	priority,
                                void       *arg,
                                const char *name
                                );


/// Stop and destroy a running thread
///
/// @param thread [in] handle thread to destroy as returned from bos_thread_create_t function
///
/// @returns non-0 on error (no such thread)
///
typedef int (*bos_thread_destroy_t)(bos_thread_t thread);

/// Get thread handle of calling thread
///
/// @returns the thread handle for the thread calling the function
///          or NULL if not called in a thread context
///
typedef bos_thread_t (*bos_thread_self_t)(void);

/// Set a thread's CPU affinity.
///
/// Informs the underlying os that the thread represented by @param thread
/// should be run on a specific set of CPUs.
/// There is no checking at this level regarding the appropriatness or
/// existence of the cpu specified.
///
/// @param thread   [in] thread to set affinity for
/// @param cpu_mask [in] mask of CPUs to set affinity to (bit0 = 1 = cpu0, bit1 = 2 = cpu1 ...)
///
/// @returns non-0 on error (no such thread, or error from lower layer)
///
typedef int (*bos_set_affinity_t)(
                                bos_thread_t thread,
                                uint32_t     cpu_mask
                                );

/// Sleep for a period of at least seconds + microseconds
///   Threads may switch when calling this. even for 0 waits,
///   so this can be used for "yield" functionality
/// Time actually spend may be (much) more than specified
/// Sleep times might also be quantized to thread tick times
///
/// @param seconds      [in] seconds to sleep for
/// @param microseconds [in] additional microseconds to sleep for
///
/// @returns 0
///
typedef void (*bos_sleep_t)(uint32_t seconds, uint32_t microseconds);

/// Create a mutex (or non-counting semaphore)
/// @returns NULL on error, or the mutex handle
///
typedef bos_mtx_t (*box_mutex_create_t)(void);

/// Destroy a mutex created with a bos_mutex_create_t function
///
/// @param mtx [in] mutex to destroy
///
/// @returns non-0 on error (no such mutex)
///
typedef int (*bos_mutex_destroy_t)(bos_mtx_t mtx);

/// Non-blocking wait for a mutex.  Waits for up to secs+usecs to
/// aquire a mutex.  Use 0,0 for secs,usecs to not wait at all (try)
/// and use -1,-1 for secs,usecs to wait forever (blocking wait)
///
/// @param mtx      [in] mutex to wait for
/// @param secs     [in] seconds to wait for, use -1 to wait forever
/// @param usecs    [in] microseconds to wait for, ingored if secs is -1
///
/// @returns > 0 if the wait timed-out,
///          < 0 on error (no such mutex)
///            0 if the mutex is aquired
///
typedef int (*bos_mutex_aquire_t)(bos_mtx_t mtx, int secs, int usecs);

/// Release an aquired mutex
///
/// @param mtx [in] the mutex to release
/// @returns non-0 on error (no such mutex, mutex not held)
///
typedef int (*bos_mutex_release_t)(bos_mtx_t mtx);

/// Create a Semaphore
///
/// @param count [in] initial count
///
/// @returns BOSAL_INVALIDSEM on error, or the Semaphore handle
///
typedef bos_sem_t (*bos_sem_create_t)(uint32_t count);

/// Destroy a Semaphore created with TScreateSemaphore
///
/// @param sem [in] semaphore to destroy
///
/// @returns non-0 on error (no such Semaphore)
///
typedef int (*bos_sem_destroy_t)(bos_sem_t sem);

/// Non-blocking wait for a Semaphore.  Waits for up to secs+usecs to
/// aquire a Semaphore.  Use 0,0 for secs,usecs to not wait at all.
/// Use -1,-1 to wait forever. Decrements the semaphore's count
///
/// @param mtx      [in] mutex to wait for
/// @param secs     [in] seconds to wait for, use -1 to wait forever
/// @param usecs    [in] microseconds to wait for, ingored if secs is -1
///
/// @returns > 0 if the wait timed-out,
///          < 0 on error (no such Semaphore)
///            0 if the Sem is gotten
///
typedef int (*bos_sem_get_t)(bos_mtx_t mtx, int secs, int usecs);

/// Release an aquired semaphore. Incrememts semaphore's count
///
/// @param sem [in] the semaphore to put
///
/// @returns non-0 on error (no such semaphore, semaphore not gotten)
///
typedef int (*bos_sem_put_t)(bos_sem_t sem);

/// Create a message queue
///
/// @param max_length [in] total number of messages queue can hold
///
/// @returns the queue handle, or BOSAL_INVALIDMSQ on failure (allocation, etc.)
///
typedef bos_msgq_t (*bos_msgq_create_t)(size_t max_length);

/// Destroy a message queue
///
/// @param msgq [in] queue to destroy
///
/// @returns non-0 on error (no such queue)
///
typedef int (*bos_msgq_destroy_t)(bos_msgq_t msgq);

/// Send a message to a message queue
///
/// @param msgq [in] message queue to send on
/// @param msg    [in] message to send
///
/// @returns -1 on error (no such queue)
///          -2 on overflow
///           0 no error
///
typedef int (*bos_msgq_send_t)(
                            bos_msgq_t  msgq,
                            bos_msg_t  *msg
                            );

/// Wait for a message to appear in a message queue.  Timeout after
/// waiting for @param secs seconds and @param usecs microseconds.
/// If no timeout is specified, the function return immediately (peek)
/// Use -1,-1 for secs/usecs to wait forever.
///
/// @param msgq       [in] message queue to wait for message on
/// @param msg        [in] place to hold message received
/// @param secs       [in] seconds to wait for, use -1 to wait forever
/// @param usecs  [in] microseconds to wait for, ingored if secs is -1
///
/// @returns < 0 on error (no such queue, queue destroyed, etc.)
///          > 0 on timeout
///            0 there was a message placed in @param msg
///
typedef int (*bos_msgq_receive_t)(
                            bos_msgq_t    msgq,
                            bos_msg_t    *msg,
                            int           secs,
                            int           usecs
                            );


/// Get count of messages in a queue
///
/// @param msgq [in] queue to get count of messages from
///
/// @returns the count of messages in the message queue or < 0 on error (no such queue)
///
typedef size_t (*bos_msgq_count_t)(bos_msgq_t msgq);

/// OSAL Interface
///
typedef struct tag_bosal_interface
{
    bos_ints_off_t          threads_disable;
    bos_ints_on_t           threads_enable;
    bos_thread_create_t     thread_create;
    bos_thread_destroy_t    thread_destroy;
    bos_thread_self_t       thread_self;
    bos_set_affinity_t      thread_affinity;
    bos_sleep_t             sleep;
    box_mutex_create_t      mutex_create;
    bos_mutex_destroy_t     mutex_destroy;
    bos_mutex_aquire_t      mutex_aquire;
    bos_mutex_release_t     mutex_release;
    bos_sem_create_t        semaphore_create;
    bos_sem_destroy_t       semaphore_destroy;
    bos_sem_get_t           semaphore_get;
    bos_sem_put_t           semaphore_put;
    bos_msgq_create_t       msgq_create;
    bos_msgq_destroy_t      msgq_destoy;
    bos_msgq_send_t         msgq_send;
    bos_msgq_receive_t      msgq_receive;
    bos_msgq_count_t        msgq_count;
}
bos_interface_t;

/// Get the BOSAL Interface for the system
///
/// @returns pointer to the interface compiled
///
bos_interface_t *bosal_interface(void);

/// Standard implementation of msgq
///
bos_msgq_t  bosal_msgq_create(size_t max_count);
int         bosal_msgq_destroy(bos_msgq_t msgq);
int         bosal_msgq_send(bos_msgq_t msgq, bos_msg_t *msg);
int         bosal_msgq_receive(bos_msgq_t msgq, bos_msg_t *msg, int tosecs, int tousecs);
size_t      bosal_msgq_count(bos_msgq_t msgq);

#endif

