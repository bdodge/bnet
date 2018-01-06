
#include "bmqtt.h"

typedef enum
{
    mqsInit,
    mqsConnect,
    mqsPing,
    mqsDisconnect,
    mqsDone,
}
mqstate_t;

#define MQTT_IO_SIZE 1024

typedef struct
{
    ioring_t out;
    mqstate_t state;
}
mqcontext_t;

#define mqtt_client_id "hello"

int mqtt_format_message(
            mqcontext_t *mqx,
            uint8_t cmd, uint8_t flags,
            uint8_t *varhdr_data, size_t varhdr_size,
            uint8_t *payload_data, size_t payload_size
            )
{
    size_t totlen, i;
    int head;

    // make sure it can all fit
    totlen = varhdr_size + payload_size;
    if ((totlen + 6) > (mqx->out.size - mqx->out.count))
    {
        BERROR("No room for message");
        return -1;
    }
    head = mqx->out.head;
    mqx->out.data[head++] = (cmd << 4) | (flags & 0xF);
    mqx->out.data[head++] = totlen;
    for (i = 0; i < varhdr_size; i++)
    {
        mqx->out.data[head++] = varhdr_data[i];
    }
    for (i = 0; i < payload_size; i++)
    {
        mqx->out.data[head++] = payload_data[i];
    }
    mqx->out.count += (head - mqx->out.head);
    mqx->out.head = head;
    return 0;
}

int mqtt_connect(http_client_t *client)
{
    mqcontext_t *mqx;
    int result;
    uint8_t vh[10];
    uint8_t pl[32];
    size_t len;

    mqx = (mqcontext_t *)client->ctxpriv;

    // insist we're starting out fresh
    mqx->out.head = mqx->out.count = 0;

    // format variable header

    // protocol name
    vh[0] = 0;
    vh[1] = 4;
    vh[2] = 'M';
    vh[3] = 'Q';
    vh[4] = 'T';
    vh[5] = 'T';

    // level
    vh[6] = 4;

    // flags
    vh[7] = 0;

    // keep-alive
    vh[8] = 0;
    vh[9] = 10;

    // format payload (client ID)
    //
    len = strlen(mqtt_client_id);
    if (len >= sizeof(pl))
    {
        BERROR("No room for client ID");
        return -1;
    }
    pl[0] = (len >> 8) & 0xFF;
    pl[1] = len & 0xFF;
    strncpy((char *)pl + 2, mqtt_client_id, sizeof(pl) - 1);
    pl[sizeof(pl) - 1] = '\0';

    return mqtt_format_message(
                        mqx,
                        MQCONNECT, 0,
                        vh, 10,
                        pl, len + 2
                        );
}

int mqtt_ping(http_client_t *client)
{
    mqcontext_t *mqx;
    int result;

    mqx = (mqcontext_t *)client->ctxpriv;

    // insist we're starting out fresh
    mqx->out.head = mqx->out.count = 0;

    return mqtt_format_message(
                        mqx,
                        MQPINGREQ, 0,
                        NULL, 0,
                        NULL, 0
                        );
}

int mqtt_disconnect(http_client_t *client)
{
    mqcontext_t *mqx;
    int result;

    mqx = (mqcontext_t *)client->ctxpriv;

    // insist we're starting out fresh
    mqx->out.head = mqx->out.count = 0;

    return mqtt_format_message(
                        mqx,
                        MQDISCONNECT, 0,
                        NULL, 0,
                        NULL, 0
                        );
}

int mqtt_resource(
                http_client_t       *client,
                http_resource_t     *resource,
                http_callback_type_t cbtype,
                uint8_t            **data,
                size_t              *count
                 )
{
    mqcontext_t *mqx;
    int result;
    size_t bytes;
    uint8_t *pmsg;
    uint8_t cmd, flags, ilen;

    mqx = (mqcontext_t *)client->ctxpriv;

    switch (cbtype)
    {
    case httpRequest:

        http_log(5, "MQTT: request\n");
        break;

    case httpUploadData:

        bytes = *count;
        *count = 0;

        if (! mqx)
        {
            BERROR("No CTX");
            return -1;
        }
        http_log(4, "MQX state %d\n", mqx->state);

        switch (mqx->state)
        {
        case mqsInit:
            result = mqtt_connect(client);
            if (result)
            {
                return -1;
            }
            mqx->state = mqsConnect;
            break;

        case mqsConnect:
            // waiting for connack or timeout
            if (mqx->out.count)
            {
                *data = mqx->out.data + mqx->out.tail;
                *count = mqx->out.count;
                mqx->out.count = 0;
                mqx->out.head = mqx->out.tail = 0;
            }
            break;

        case mqsPing:
            // waiting for pong or timeout
            if (mqx->out.count)
            {
                *data = mqx->out.data + mqx->out.tail;
                *count = mqx->out.count;
                mqx->out.count = 0;
                mqx->out.head = mqx->out.tail = 0;
            }
            break;

        case mqsDisconnect:
            result = mqtt_disconnect(client);
            if (result)
            {
                return -1;
            }
            mqx->state = mqsDone;
            break;

        case mqsDone:
            if (mqx->out.count)
            {
                *data = mqx->out.data + mqx->out.tail;
                *count = mqx->out.count;
                mqx->out.count = 0;
                mqx->out.head = mqx->out.tail = 0;
            }
            break;

        default:
            BERROR("Bad State");
            return -1;
        }
        if (*count > 0)
        {
            http_log(5, "MQTT: Sending Bytes: %u\n", *count);
        }
        break;

    case httpDownloadData:

        http_log(5, "MQTT: Getting Bytes: %u\n", *count);
        bytes = *count;
        if (bytes < 2)
        {
            // wait for enough bytes
            *count = 0;
            break;
        }
        // assume we take all
        *count = bytes;

        http_log(4, "MQX state %d\n", mqx->state);

        pmsg = *data;
        cmd = pmsg[0] >> 4;
        flags = pmsg[0] & 0xF;
        ilen = pmsg[1];

        switch (mqx->state)
        {
        case mqsInit:
            // can't happen? absorb and ignore
            break;

        case mqsConnect:
            // waiting for connack
            if (cmd == MQCONNACK)
            {
                http_log(4, "MQTT Connected\n");

                // we are connected! do a ping to test
                result = mqtt_ping(client);
                if (result)
                {
                    return -1;
                }
                mqx->state = mqsPing;

                // only take these command bytes out
                bytes = 2 + ilen;
                *count = bytes;
                break;
            }
            // any other msg is an error
            BERROR("Dropping: not connack");
            return -1;

        case mqsPing:
            if (cmd == MQPINGRESP)
            {
                http_log(4, "MQTT Pong\n");

                mqx->state = mqsDisconnect;

                // only take these command bytes out
                bytes = 2 + ilen;
                *count = bytes;
                break;
            }
            // any other msg is an error
            BERROR("Dropping: not pingresp");
            return -1;

        default:
            BERROR("Bad State");
            return -1;
        }
        break;

    case httpComplete:

        http_log(5, "MQTT: complete\n");
        break;

    default:
        return -1;
    }
    return 0;
}

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

    mqx = (mqcontext_t *)malloc(sizeof(mqcontext_t));
    if (! mqx)
    {
        BERROR("Can't alloc mqx");
        return -1;
    }
    mqx->out.size = MQTT_IO_SIZE;
    mqx->out.data = (uint8_t *)malloc(mqx->out.size);
    if (! mqx->out.data)
    {
        BERROR("Can't alloc in data");
        free(mqx);
        return -1;
    }
    mqx->out.tail = 0;
    mqx->out.head = 0;
    mqx->out.count = 0;

    resources = NULL;
    result = http_add_func_resource(&resources, "/echo", NULL, mqtt_resource, NULL);
    if (result)
    {
        BERROR("Can't make resource");
        free(mqx);
        return -1;
    }
    port = 8080;
    snprintf(url, sizeof(url), "ws://test.mosquitto.org:%u/mqtt", port);
    method = httpGet;

    http_log(3, "%s %s\n", http_method_name(method), url);

    client = http_client_create(NULL, true);
    if (! client)
    {
        BERROR("No memory for client");
        free(mqx);
        return -1;
    }
    client->ctxpriv = mqx;
    client->keepalive = false;
    client->ws_upgrade = true;
    snprintf(client->ws_proto, sizeof(client->ws_proto), "mqttv3.1");
    client->ws_version = 13;

    do
    {
        result = http_client_request(client, method, url, httpTCP, false, NULL, resources);

        while (! result)
        {
            result = http_client_slice(client);
            if (result)
            {
                BERROR("Client error");
                break;
            }
            if (client->state == httpDone)
            {
                http_log(3, "http client complete\n");
                break;
            }
            to_secs = busy ? 0 : 0;
            to_usecs = busy ? 0 : 10000;
            result = http_wait_for_client_event(client, to_secs, to_usecs);
            if (result < 0)
            {
                break;
            }
            result = 0;
        }
    }
    while (! result && client->response >= 300 && client->response < 400);

    http_log(1, "client ends\n");
    http_client_free(client);
    iostream_tls_epilog();
    free(mqx);
    return result;
}

