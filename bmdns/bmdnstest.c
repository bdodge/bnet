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
#include "bmdns.h"

int main(int argc, char **argv)
{
    mdns_responder_t responder;
    int result;

#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        BERROR("WSAStartup failed");
        return -1;
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    butil_set_log_level(5);

    result = mdns_responder_init(&responder);
    if (result)
    {
        return result;
    }
    result = mdns_responder_run(&responder);
    if (result)
    {
        butil_log(0, "Responder Failed\n");
    }
    else
    {
        butil_log(2, "Responder Exits\n");
    }
    result = mdns_responder_deinit(&responder);
    return result;
}

