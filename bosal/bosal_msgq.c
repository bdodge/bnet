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
#include "bosal.h"

#define BOSAL_MSGQ_TAG 0x1010ABCA

typedef struct tag_msgq
{
    uint32_t        tag;
    sem_t           sem;
    pthread_mutex_t lock;
    bosal_msg_t     *pool;
    size_t          size;
    size_t          head;
    size_t          tail;
    size_t          count;
}
bmsgq_t;

bosal_msgq_t bosal_msgq_create(size_t max_count)
{
    bmsgq_t *q;
    int result;

    if (max_count == 0)
    {
        return NULL;
    }
    q = (bmsgq_t *)malloc(sizeof(bmsgq_t));
    if (! q)
    {
        return NULL;
    }
    q->pool = (bosal_msg_t *)malloc(max_count * sizeof(bosal_msg_t));
    if (! q->pool)
    {
        free(q);
        return NULL;
    }
    result = pthread_mutex_init(&q->lock, NULL);
    if (result)
    {
        free(q->pool);
        free(q);
        return NULL;
    }
    result = sem_init(&q->sem, PTHREAD_PROCESS_PRIVATE, 0);
    if (result)
    {
        pthread_mutex_destroy(&q->lock);
        free(q->pool);
        free(q);
        return NULL;
    }
    q->tag = BOSAL_MSGQ_TAG;

    q->size  = max_count;
    q->head  = 0;
    q->tail  = 0;
    q->count = 0;
    return (void*)q;
}

int bosal_msgq_destroy(bosal_msgq_t msgq)
{
    bmsgq_t *q;

    if (! msgq)
    {
        return -1;
    }
    q = (bmsgq_t *)msgq;

    if (q->tag != BOSAL_MSGQ_TAG)
    {
        return -1;
    }
    if (q->pool)
    {
        free(q->pool);
    }
    sem_destroy(&q->sem);

    pthread_mutex_destroy(&q->lock);
    free(q);
    return 0;
}

int bosal_msgq_send(bosal_msgq_t msgq, bosal_msg_t *msg)
{
    bmsgq_t *q;
    bosal_msg_t *m;
    int result;

    if (! msgq || ! msg)
    {
        return -1;
    }
    q = (bmsgq_t *)msgq;

    if (q->tag != BOSAL_MSGQ_TAG)
    {
        return -1;
    }
    if (q->count >= q->size)
    {
        return -2;
    }
    result = pthread_mutex_lock(&q->lock);
    if (result)
    {
        return result;
    }
    m = &q->pool[q->head];
    m->msg          = msg->msg;
    m->flags        = msg->flags;
    m->param        = msg->param;
    m->value.ullval = msg->value.ullval;
    q->head++;
    if (q->head >= q->size)
    {
        q->head = 0;
    }
    q->count++;
    result = pthread_mutex_unlock(&q->lock);
    if (result)
    {
        return result;
    }
    result = sem_post(&q->sem);
    return result;
}

int bosal_msgq_receive(bosal_msgq_t msgq, bosal_msg_t *msg, int tosecs, int tousecs)
{
    bmsgq_t *q;
    bosal_msg_t *m;
    int result;

    if (! msgq || ! msg)
    {
        return -1;
    }
    q = (bmsgq_t *)msgq;

    if (q->tag != BOSAL_MSGQ_TAG)
    {
        return -1;
    }
    if (tosecs == 0 && tousecs == 0 && q->count == 0)
    {
        return 1;
    }
    if (tosecs == -1 || tousecs == -1)
    {
        result = sem_wait(&q->sem);
    }
    else
    {
        struct timespec ts;

        clock_gettime(CLOCK_REALTIME, &ts);

        ts.tv_sec  += tosecs;
        ts.tv_nsec += tousecs * 1000;
        while (ts.tv_nsec > 1000000000)
        {
           ts.tv_sec++;
           ts.tv_nsec -= 1000000000;
        }
        result = sem_timedwait(&q->sem, &ts);
        if (result)
        {
           if (errno == ETIMEDOUT)
           {
               return 1;
           }
        }
    }
    if (result)
    {
        return -1;
    }
    if (q->count == 0)
    {
        return 1;
    }
    m = &q->pool[q->tail];
    msg->msg        = m->msg;
    msg->flags      = m->flags;
    msg->param      = m->param;
    msg->value.ullval = m->value.ullval;
    q->tail++;
    if (q->tail >= q->size)
    {
        q->tail = 0;
    }
    q->count--;
    result = pthread_mutex_unlock(&q->lock);
    return result ? -1 : 0;
}

size_t bosal_msgq_count(bosal_msgq_t msgq)
{
    bmsgq_t *q;
    bosal_msg_t *m;
    int result;

    if (! msgq)
    {
        return -1;
    }
    q = (bmsgq_t *)msgq;

    if (q->tag != BOSAL_MSGQ_TAG)
    {
        return -1;
    }
    return q->count;
}


