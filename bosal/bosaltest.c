#include "bnetheaders.h"
#include "bosal_msgq.h"

static pthread_t thread1, thread2;
static bos_msgq_t msgq;
static sem_t xsem;

void *t1func(void *arg)
{
    bos_msgq_t msgq;
    bos_msg_t  msg;
    int counter = 0;
    int result;

    msgq = (bos_msgq_t)arg;

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
    bos_msgq_t msgq;
    bos_msg_t  msg;
    int result;

    msgq = (bos_msgq_t)arg;

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
    return 0;
}

