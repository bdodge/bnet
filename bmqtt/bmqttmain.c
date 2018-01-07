
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
    http_set_log_level(5);

    result = iostream_tls_prolog();
    if (result)
    {
        BERROR("Can't init TLS");
        return -1;
    }
    program = *argv++;
    argc--;

    mqx = mqtt_client_create(
                "test.mosquitto.org",
            #if 0
                8080,
                mqtWS,
            #elif 1
                1883,
                mqtTCP,
            #else
                8883,
                mqtTLS,
            #endif
                "hello",
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
        do
        {
            result = mqtt_slice(mqx);
        }
        while (! result && mqx->state != mqsDone);
    }
    iostream_tls_epilog();
    mqtt_client_free(mqx);
    return result;
}

