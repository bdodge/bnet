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
#ifndef BOSAL_H
#define BOSAL_H 1

#include "bnetheaders.h"
#include <pthread.h>
#ifdef __APPLE__
#include "bosal_sem.h"
#else
#include <semaphore.h>
#endif
#include <errno.h>

#define BOSAL_INVALIDMSGQ   NULL

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
bosal_msg_t;

/// Mailbox handle
typedef void *bosal_msgq_t;

/// Create a message queue
///
/// @param max_length [in] total number of messages queue can hold
///
/// @returns the queue handle, or BOSAL_INVALIDMSQ on failure (allocation, etc.)
///
bosal_msgq_t bosal_msgq_create(size_t max_length);

/// Destroy a message queue
///
/// @param msgq [in] queue to destroy
///
/// @returns non-0 on error (no such queue)
///
int bosal_msgq_destroy(bosal_msgq_t msgq);

/// Send a message to a message queue
///
/// @param msgq [in] message queue to send on
/// @param msg  [in] message to send
///
/// @returns -1 on error (no such queue)
///          -2 on overflow
///           0 no error
///
int bosal_msgq_send(
                    bosal_msgq_t  msgq,
                    bosal_msg_t  *msg
                    );

/// Wait for a message to appear in a message queue.  Timeout after
/// waiting for @param secs seconds and @param usecs microseconds.
/// If no timeout is specified, the function return immediately (peek)
/// Use -1,-1 for secs/usecs to wait forever.
///
/// @param msgq     [in] message queue to wait for message on
/// @param msg      [in] place to hold message received
/// @param secs     [in] seconds to wait for, use -1 to wait forever
/// @param usecs    [in] microseconds to wait for, ingored if secs is -1
///
/// @returns < 0 on error (no such queue, queue destroyed, etc.)
///          > 0 on timeout
///            0 there was a message placed in @param msg
///
int bosal_msgq_receive(
                    bosal_msgq_t    msgq,
                    bosal_msg_t    *msg,
                    int           secs,
                    int           usecs
                    );


/// Get count of messages in a queue
///
/// @param msgq [in] queue to get count of messages from
///
/// @returns the count of messages in the message queue or < 0 on error (no such queue)
///
size_t bosal_msgq_count(bosal_msgq_t msgq);

/// Sleep for seconds + microseconds
///
/// @param secs     [in] seconds to wait for, use -1 to wait forever
/// @param usecs    [in] microseconds to wait for, ingored if secs is -1
///
/// @returns 0
///
int bosal_sleep(int secs, int usecs);

#endif

