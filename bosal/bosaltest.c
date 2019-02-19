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
#include "bnetheaders.h"
#include "bosal.h"

static pthread_t thread1, thread2;
static bosal_msgq_t msgq;
static sem_t xsem;

void *t1func(void *arg)
{
    bosal_msgq_t msgq;
    bosal_msg_t  msg;
    int counter = 0;
    int result;

    msgq = (bosal_msgq_t)arg;

    while (1)
    {
        bosal_sleep(5, 0);
        printf("T1\n");
        msg.msg = counter++;
        result = bosal_msgq_send(msgq, &msg);
        if (result < 0)
        {
            fprintf(stderr, "T1 msgq tx fail\n");
            return NULL;
        }
    }
    return NULL;
}

void *t2func(void *arg)
{
    bosal_msgq_t msgq;
    bosal_msg_t  msg;
    int result;

    msgq = (bosal_msgq_t)arg;

    while (1)
    {
        result = bosal_msgq_receive(msgq, &msg, 2, 0);

        if (result < 0)
        {
            fprintf(stderr, "T2 msgq rx fail\n");
            return NULL;
        }
        else if (result > 0)
        {
            printf("T2\n");
        }
        else
        {
            printf("Msg from T1=%u\n", msg.msg);
            if (msg.msg == 5)
            {
                result = sem_post(&xsem);

                if (result < 0)
                {
                    fprintf(stderr, "T2 sem put fail\n");
                    return NULL;
                }
            }
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    int result;

    // create semaphore
    //
    result = sem_init(&xsem, PTHREAD_PROCESS_PRIVATE, 0);

    if (result)
    {
        fprintf(stderr, "No sem\n");
        return -1;
    }
    // create msgq
    //
    msgq = bosal_msgq_create(10);
    if (! msgq)
    {
        fprintf(stderr, "No msgq\n");
        return -1;
    }
    // create thread 1
    //
    result = pthread_create(&thread1, NULL, t1func, msgq);
    if (result)
    {
        fprintf(stderr, "No thread1\n");
        return -1;
    }
    // create thread 2
    //
    result = pthread_create(&thread2, NULL, t2func, msgq);
    if (result)
    {
        fprintf(stderr, "No thread2\n");
        return -1;
    }
    // wait for signal to exit
    //
    while (1)
    {
        struct timespec ts;

        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec  += 10;
        ts.tv_nsec += 0;
        result = sem_timedwait(&xsem, &ts);

        if (result < 0)
        {
            if (errno == ETIMEDOUT)
            {
                result = 1;
            }
            else
            {
                fprintf(stderr, "sem wait fail %d\n", errno);
                return -1;
            }
        }
        if (result == 0)
        {
            printf("test over\n");
            break;
        }
        printf("Sleepy..\n");
    }
    sem_destroy(&xsem);
    return 0;
}

