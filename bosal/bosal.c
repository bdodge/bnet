
#include "bosal.h"

int bosal_sleep(int secs, int usecs)
{
    struct timespec ts;
    int result;

    ts.tv_sec = secs;
    ts.tv_nsec = usecs * 1000;
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

    return 0;
}

