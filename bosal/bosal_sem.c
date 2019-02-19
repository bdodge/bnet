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

#ifdef __APPLE__

struct tag_sem_t
{
    pthread_mutex_t count_lock;
    pthread_cond_t  count_bump;
    unsigned count;
};

typedef struct tag_sem_t bosal_sem_t;

int sem_init(sem_t *psem, int flags, unsigned count)
{
    bosal_sem_t *pnewsem;
    int result;

    pnewsem = (bosal_sem_t *)malloc(sizeof(bosal_sem_t));
    if (! pnewsem)
    {
        free(pnewsem);
        return -1;
    }
        result = pthread_mutex_init(&pnewsem->count_lock, NULL);
    if (result)
    {
        free(pnewsem);
        return result;
    }
    result = pthread_cond_init(&pnewsem->count_bump, NULL);
    if (result)
    {
        pthread_mutex_destroy(&pnewsem->count_lock);
        free(pnewsem);
        return result;
    }
    pnewsem->count = count;
    *psem = (sem_t)pnewsem;
    return 0;
}

int sem_destroy(sem_t *psem)
{
    bosal_sem_t *poldsem;

    if (! psem)
    {
        return EINVAL;
    }
    poldsem = (bosal_sem_t *)*psem;

    pthread_mutex_destroy(&poldsem->count_lock);
    pthread_cond_destroy(&poldsem->count_bump);
    free(poldsem);
    return 0;
}

int sem_post(sem_t *psem)
{
    bosal_sem_t *pxsem;
    int result, xresult;

    if (! psem)
    {
        return EINVAL;
    }
    pxsem = (bosal_sem_t *)*psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    pxsem->count = pxsem->count + 1;

    xresult = pthread_cond_signal(&pxsem->count_bump);

    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

int sem_trywait(sem_t *psem)
{
    bosal_sem_t *pxsem;
    int result, xresult;

    if (! psem)
    {
        return EINVAL;
    }
    pxsem = (bosal_sem_t *)*psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    xresult = 0;

    if (pxsem->count > 0)
    {
        pxsem->count--;
    }
    else
    {
        xresult = EAGAIN;
    }
    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

int sem_wait(sem_t *psem)
{
    bosal_sem_t *pxsem;
    int result, xresult;

    if (! psem)
    {
        return EINVAL;
    }
    pxsem = (bosal_sem_t *)*psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    xresult = 0;

    if (pxsem->count == 0)
    {
        xresult = pthread_cond_wait(&pxsem->count_bump, &pxsem->count_lock);
    }
    if (! xresult)
    {
        if (pxsem->count > 0)
        {
            pxsem->count--;
        }
    }
    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

int sem_timedwait(sem_t *psem, const struct timespec *abstim)
{
    bosal_sem_t *pxsem;
    int result, xresult;

    if (! psem)
    {
        return EINVAL;
    }
    pxsem = (bosal_sem_t *)*psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    xresult = 0;

    if (pxsem->count == 0)
    {
        xresult = pthread_cond_timedwait(&pxsem->count_bump, &pxsem->count_lock, abstim);
    }
    if (! xresult)
    {
        if (pxsem->count > 0)
        {
            pxsem->count--;
        }
    }
    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

#endif

