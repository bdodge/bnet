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
#include "bmqtt.h"

#define HELLO_IO_SIZE 1024

int main(int argc, char **argv)
{
    mqcontext_t *mqx;
    char url[HTTP_MAX_LINE];
    const char *program, *arg;
    http_method_t method;
    uint16_t port;
    http_client_t *client;
    http_resource_t *resources;
    int to_secs, to_usecs;
    bool busy;
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

    result = iostream_tls_prolog();
    if (result)
    {
        BERROR("Can't init TLS");
        return -1;
    }
    program = *argv++;
    argc--;

    mqx = mqtt_client_create(
                "hello",
//                "test.mosquitto.org",
                "broker.mqttdashboard.com",
            #if 1
//                8080,
                8000,
                mqtWS,
            #elif 0
                1883,
                mqtTCP,
            #else
                8883,
                mqtTLS,
            #endif
                123,
            //    mqqosMostOnce,
            //  mqqosLeastOnce,
              mqqosOnlyOnce,
                HELLO_IO_SIZE,
                15
                );
    if (! mqx)
    {
        BERROR("Can't alloc mqx");
        result = -1;
    }
    else
    {
        result = mqtt_connect(mqx);

        while (! result && mqx->state != mqsDone)
        {
            result = mqtt_slice(mqx);
        }
    }
    iostream_tls_epilog();
    mqtt_client_free(mqx);
    return result;
}

