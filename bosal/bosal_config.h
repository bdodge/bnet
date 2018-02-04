#ifndef BOSAL_CONFIG_H
#define BOSAL_CONFIG_H

#ifndef BOSAL_DEBUG_LOCKS
    /// Define non-0 for extra mutex and semaphore debugging info
	///
    #define BOSAL_DEBUG_LOCKS 0
#endif
#ifndef BOSAL_DEBUG_QUEUES
    /// Define non-0 for extra msg queue debugging info
    ///
    #define BOSAL_DEBUG_QUEUES 0
#endif
#ifndef BOSAL_DEBUG_THREADS
    /// Define non-0 for extra thread debugging info
    ///
    #define BOSAL_DEBUG_THREADS 0
#endif
#ifndef BOSAL_THREAD_TRACE
    /// Define non-0 to provide thread execution timing info
    ///
    #define BOSAL_THREAD_TRACE 0
#endif

#endif
