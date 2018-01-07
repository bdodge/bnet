
#include "bmqtt.h"

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

int mqtt_connect(mqcontext_t *mqx)
{
    int result;
    uint8_t vh[10];
    uint8_t pl[32];
    size_t len;

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

int mqtt_ping(mqcontext_t *mqx)
{
    int result;

    // insist we're starting out fresh
    mqx->out.head = mqx->out.count = 0;

    return mqtt_format_message(
                        mqx,
                        MQPINGREQ, 0,
                        NULL, 0,
                        NULL, 0
                        );
}

int mqtt_disconnect(mqcontext_t *mqx)
{
    int result;

    // insist we're starting out fresh
    mqx->out.head = mqx->out.count = 0;

    return mqtt_format_message(
                        mqx,
                        MQDISCONNECT, 0,
                        NULL, 0,
                        NULL, 0
                        );
}

int mqtt_input_phase(mqcontext_t *mqx, uint8_t *data, size_t *count)
{
	int result;
	size_t bytes;
    uint8_t cmd, flags, ilen;

	bytes = *count;

    // assume we take all
    *count = bytes;

    http_log(4, "MQX state %d\n", mqx->state);

    cmd   = data[0] >> 4;
    flags = data[0] & 0xF;
    ilen  = data[1];

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
            result = mqtt_ping(mqx);
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

            // we are connected! do a ping to test
            result = mqtt_disconnect(mqx);
            if (result)
            {
                return -1;
            }
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
	return 0;
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

    mqx = (mqcontext_t *)client->ctxpriv;

    switch (cbtype)
    {
    case httpRequest:

        http_log(5, "MQTT: request\n");
        mqx->state = mqsTransport;
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
        case mqsTransport:
            result = mqtt_connect(mqx);
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
            if (mqx->out.count)
            {
                *data = mqx->out.data + mqx->out.tail;
                *count = mqx->out.count;
                mqx->out.count = 0;
                mqx->out.head = mqx->out.tail = 0;
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
		result = mqtt_input_phase(mqx, *data, count);
		if (result)
		{
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

static int mqtt_client_input(mqcontext_t *mqx, int to_secs, int to_usecs)
{
    time_t now;
    int result;
    int room;
    int count;
    int len;

    if (! mqx || ! mqx->stream)
    {
        return -1;
    }
    count = mqx->in.count;

    if (mqx->in.count < mqx->in.size)
    {
        if (mqx->in.count == 0)
        {
            // to avoid later renormalizing
            mqx->in.head = 0;
            mqx->in.tail = 0;
        }
        if (mqx->in.head < mqx->in.tail)
        {
            room = mqx->in.tail - mqx->in.head;
        }
        else
        {
            room = mqx->in.size - mqx->in.head;
        }
        // if there is a bunch of data in the buffer, don't wait for more
        //
        if (room < (mqx->in.size / 2))
        {
            to_secs  = 0;
            to_usecs = 0;
        }
        // don't read if less than N bytes room available.  this is to
        // ensure TLS read block size can alway fit
        //
        if (room >= 32)
        {
            result = mqx->stream->poll(mqx->stream, readable, to_secs, to_usecs);
            if (result < 0)
            {
                return result;
            }
            if (result > 0)
            {
                len = mqx->stream->read(mqx->stream,
                       mqx->in.data + mqx->in.head, room);
                if (len <= 0)
                {
                    if (mqx->in.count)
                    {
                        return 0;
                    }
                    // 0 read after ok poll means connection closed
                    http_log(5, "end of connection %u\n", mqx->id);
                    return -1;
                }
                mqx->in.head += len;
                if (mqx->in.head >= mqx->in.size)
                {
                    mqx->in.head = 0;
                }
                mqx->in.count += len;
                http_log(7, "read %d on id %u\n", len, mqx->id);
            }
        }
    }
    time(&now);

    if ((mqx->in.count == count) && (mqx->state == mqx->prev_state))
    {
        // check long timeout
        if (mqx->last_in_time == 0)
        {
            mqx->last_in_time = now;
        }
        if ((now - mqx->last_in_time) > mqx->long_timeout)
        {
            http_log(3, "cl:%u read timeout\n", mqx->id);
            return -1;
        }
    }
    else
    {
        // reset long timeout
        mqx->last_in_time = now;
    }
	return 0;
}

static int mqtt_client_output(mqcontext_t *mqx, int to_secs, int to_usecs)
{
	time_t now;
	int result;
	int count;
	int wc;

    if (mqx->out.count == 0)
    {
        mqx->last_out_time = 0;
        return 0;
    }
    if (mqx->out.tail < mqx->out.head)
    {
        count = mqx->out.head - mqx->out.tail;
    }
    else
    {
        count = mqx->out.size - mqx->out.tail;
    }
	wc = mqx->stream->poll(mqx->stream, writeable, to_secs, to_usecs);
	if (wc < 0)
	{
		BERROR("closed on send");
		return -1;
	}
	time(&now);

	if (wc == 0)
	{
		// check long timeout
		if (mqx->last_out_time == 0)
		{
			mqx->last_out_time = now;
		}
		if ((now - mqx->last_out_time) > mqx->long_timeout)
		{
			http_log(3, "mqx %u write timeout\n", mqx->id);
			return -1;
		}
		return 0;
	}
	else
	{
		mqx->last_out_time = now;
	}
	wc = mqx->stream->write(mqx->stream, mqx->out.data + mqx->out.tail, count);
	if (wc <= 0)
	{
		BERROR("closed on send");
		return -1;
	}
    mqx->out.count -= wc;
    mqx->out.tail += wc;
    if (mqx->out.tail >= mqx->out.size)
    {
        mqx->out.tail = 0;
    }
	if (mqx->out.count == 0)
	{
		mqx->out.head = 0;
		mqx->out.tail = 0;
	}
	return 0;
}

int mqtt_slice(mqcontext_t *mqx)
{
    int to_secs, to_usecs;
    bool busy;
    int result;

    if (mqx->transport == mqtWS || mqx->transport == mqtWSS)
    {
#if MQTT_SUPPORT_WEBSOCKET
        if (mqx->state == mqsInit)
        {
            result = http_client_request(mqx->client, httpGet,
                        mqx->url, httpTCP, false, NULL, mqx->resources);
            if (result)
            {
                return result;
            }
            mqx->state = mqsTransportInit;
        }
        else if (mqx->state == mqsDone)
        {
            return 0;
        }
        else
        {
            result = http_client_slice(mqx->client);
            if (result)
            {
                BERROR("Client error");
                return result;
            }
            // throw a rest in loop
            //
            to_secs = busy ? 0 : 0;
            to_usecs = busy ? 0 : 10000;
            result = http_wait_for_client_event(mqx->client, to_secs, to_usecs);
            if (result < 0)
            {
                return result;
            }
            result = 0;
        }
        return result;
#else
        return -1;
#endif
    }
    switch (mqx->state)
    {
    case mqsInit:
        mqx->stream = iostream_create_from_tcp_connection(mqx->url, mqx->port);
        if (! mqx->stream)
        {
            BERROR("Stream create");
            return -1;
        }
        mqx->state = mqsTransportInit;
        break;

    case mqsTransportInit:
        if (! mqx->stream)
        {
            BERROR("No stream");
            return -1;
        }
        // poll for writeable means connected
        //
        result = mqx->stream->poll(mqx->stream, writeable, 0, 2000);
        if (result < 0)
        {
            BERROR("stream");
            return -1;
        }
        if (result == 0)
        {
            // not connected, remain in init state (timeout?)
            break;
        }
        http_log(5, "Connected\n");

		// upgrade to TLS if indicated (scheme already checkd for support)
        //
        if (mqx->transport == mqtTLS)
        {
            iostream_t *stream;

            stream = iostream_tls_create_from_iostream(mqx->stream, true);
            if (! stream)
            {
                BERROR("TLS upgrade failed");
                mqx->stream = NULL;
                return -1;
            }
	        http_log(5, "TLS Connected\n");
            mqx->stream = stream;
        }
		mqx->state = mqsTransport;
        break;

	case mqsTransport:
		result = mqtt_connect(mqx);
		if (result)
		{
			BERROR("mqtt connect");
			return -1;
		}
		mqx->state = mqsConnect;
		break;

	default:
		// if any output pending, flush it
		if (mqx->out.count)
		{
			iostream_normalize_ring(&mqx->out, NULL);
			result = mqtt_client_output(mqx, 0, 10000);
			if (result)
			{
				BERROR("Output");
				return result;
			}
		}
		else
		{
			// check input
			result = mqtt_client_input(mqx, 0, 10000);
			if (result)
			{
				BERROR("Input");
				return result;
			}
			// if there is any data read, process it
			if (mqx->in.count)
			{
				size_t count;

				iostream_normalize_ring(&mqx->in, NULL);
				count = mqx->in.count;
				result = mqtt_input_phase(mqx, mqx->in.data + mqx->in.tail, &count);
				if (result)
				{
					BERROR("Input phase");
					return result;
				}
				if (count)
				{
					mqx->in.count -= count;
					mqx->in.tail += count;
					if (mqx->in.tail >= mqx->in.size)
					{
						mqx->in.tail = 0;
					}
				}
			}
		}
		break;

    case mqsDone:
        break;
    }
    return 0;
}

void mqtt_client_free(mqcontext_t *mqx)
{
    if (mqx->client)
    {
        http_client_free(mqx->client);
    }
    if (mqx->out.data)
    {
        free(mqx->out.data);
    }
    free(mqx);
}

mqcontext_t *mqtt_client_create(
                    const char *server,
                    uint16_t port,
                    mqtransport_t transport,
                    const char *client_id,
                    size_t io_max,
					time_t io_timeout
                    )
{
	static uint32_t s_idx = 0;
    mqcontext_t *mqx;
    int result;

    mqx = (mqcontext_t *)malloc(sizeof(mqcontext_t));
    if (! mqx)
    {
        BERROR("Can't alloc MQ CTX");
        return NULL;
    }
	mqx->id = ++s_idx;
    mqx->state = mqsInit;
    mqx->transport = transport;
    mqx->port = port;
    mqx->stream = NULL;
    mqx->resources = NULL;
    mqx->client = NULL;

	mqx->long_timeout = io_timeout;
	mqx->last_in_time = 0;
	mqx->last_out_time = 0;

    if (io_max < MQTT_MIN_IO_SIZE)
    {
        io_max = MQTT_MIN_IO_SIZE;
    }
    if (mqx->transport == mqtWS || mqx->transport == mqtWSS)
    {
		// websocket transport uses http client's in buffer
		mqx->in.size = 0;
		mqx->in.data = NULL;
	}
	else
	{
	    mqx->in.size = io_max;
	    mqx->in.data = (uint8_t *)malloc(mqx->in.size);
	    if (! mqx->in.data)
	    {
	        BERROR("Can't alloc data");
	        free(mqx);
	        return NULL;
	    }
	}
    mqx->in.tail = 0;
    mqx->in.head = 0;
    mqx->in.count = 0;

    mqx->out.size = io_max;
    mqx->out.data = (uint8_t *)malloc(mqx->out.size);
    if (! mqx->out.data)
    {
        BERROR("Can't alloc data");
        free(mqx);
        return NULL;
    }
    mqx->out.tail = 0;
    mqx->out.head = 0;
    mqx->out.count = 0;

    if (mqx->transport == mqtWS || mqx->transport == mqtWSS)
    {
#if MQTT_SUPPORT_WEBSOCKET
        result = http_add_func_resource(&mqx->resources, "/echo", NULL, mqtt_resource, NULL);
        if (result)
        {
            BERROR("Can't make resource");
            mqtt_client_free(mqx);
            return NULL;
        }
        http_paste_url(mqx->url, (transport == mqtWSS) ? httpWSS : httpWS, server, port, "mqtt");

        mqx->client = http_client_create(NULL, true);
        if (! mqx->client)
        {
            BERROR("No memory for client");
            mqtt_client_free(mqx);
            return NULL;
        }
        http_log(3, "MQTT Client to %s\n", mqx->url);

        mqx->client->ctxpriv = mqx;
        mqx->client->keepalive = false;
        mqx->client->ws_upgrade = true;
        snprintf(mqx->client->ws_proto, sizeof(mqx->client->ws_proto), "mqttv3.1");
        mqx->client->ws_version = 13;
#else
        BERROR("Not configured");
        return NULL;
#endif
    }
    else
    {
        strncpy(mqx->url, server, sizeof(mqx->url) - 1);
        mqx->url[sizeof(mqx->url) - 1] = '\0';
    }
    return mqx;
}

