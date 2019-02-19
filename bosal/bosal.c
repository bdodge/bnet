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

