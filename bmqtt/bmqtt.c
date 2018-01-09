
#include "bmqtt.h"

#define mqtt_client_id "hello"

typedef enum
{
	mqvhtUint8 = 1,
	mqvhtUint16,
	mqvhtUint32,
	mqvhtString
}
mqvhtype_t;

static int mqtt_format_message(
            mqcontext_t *mqx,
            uint8_t cmd, uint8_t flags,
            uint8_t *payload_data, size_t payload_size,
			size_t varhdr_count, ...
            )
{
    va_list args;
	mqvhtype_t varhdr_type;
    uint8_t *varhdr_data;
	size_t varhdr_len;
    size_t tot_len, totvarhdr_len, arg, i;
	uint8_t valbuf[4];
	uint32_t uval;
    int head;

	tot_len = 0;
	totvarhdr_len = 0;

	// see how big the variable headers add up to
    va_start(args, varhdr_count);
	for (arg = 0; arg < varhdr_count; arg++)
	{
		varhdr_type = (mqvhtype_t)va_arg(args, int);
		switch (varhdr_type)
		{
		case mqvhtUint8:
			uval = (uint32_t)va_arg(args, unsigned);
			varhdr_len = 1;
			break;
		case mqvhtUint16:
			uval = (uint32_t)va_arg(args, unsigned);
			varhdr_len = 2;
			break;
		case mqvhtUint32:
			uval = (uint32_t)va_arg(args, unsigned);
			varhdr_len = 4;
			break;
		case mqvhtString:
			varhdr_data  = va_arg(args, uint8_t*);
			if (varhdr_data)
			{
				varhdr_len = 2 + strlen(varhdr_data);
			}
			else
			{
				varhdr_len = 0;
			}
			break;
		default:
			varhdr_len = 0;
			break;
		}
		totvarhdr_len += varhdr_len;
	}
	va_end(args);

    // make sure it can all fit
    tot_len = totvarhdr_len + payload_size;
    if ((tot_len + 6) > (mqx->out.size - mqx->out.count))
    {
        BERROR("No room for message");
        return -1;
    }
	// start with fixed header
    head = mqx->out.head;
    mqx->out.data[head++] = (cmd << 4) | (flags & 0xF);
    mqx->out.data[head++] = tot_len;

	// append variable headers
    va_start(args, varhdr_count);
	for (arg = 0; arg < varhdr_count; arg++)
	{
		varhdr_type = (mqvhtype_t)va_arg(args, int);
		varhdr_data = valbuf;
		switch (varhdr_type)
		{
		case mqvhtUint8:
			uval = (uint32_t)va_arg(args, unsigned);
			varhdr_data[0] = uval & 0xFF;
			varhdr_len = 1;
			break;
		case mqvhtUint16:
			uval = (uint32_t)va_arg(args, unsigned);
			varhdr_data[0] = (uval >> 8) & 0xFF;
			varhdr_data[1] = uval & 0xFF;
			varhdr_len = 2;
			break;
		case mqvhtUint32:
			uval = (uint32_t)va_arg(args, unsigned);
			varhdr_data[0] = (uval >> 24) & 0xFF;
			varhdr_data[1] = (uval >> 16) & 0xFF;
			varhdr_data[2] = (uval >> 8) & 0xFF;
			varhdr_data[3] = uval & 0xFF;
			varhdr_len = 4;
			break;
		case mqvhtString:
			varhdr_data = va_arg(args, uint8_t*);
			if (varhdr_data)
			{
				varhdr_len = strlen(varhdr_data);
		        mqx->out.data[head++] = (varhdr_len >> 8) & 0xFF;
		        mqx->out.data[head++] = varhdr_len & 0xFF;
			}
			else
			{
				varhdr_len = 0;
			}
			break;
		default:
			varhdr_len = 0;
			break;
		}
	    for (i = 0; i < varhdr_len; i++)
	    {
	        mqx->out.data[head++] = varhdr_data[i];
	    }
	}
	va_end(args);

	// append payload data
    for (i = 0; i < payload_size; i++)
    {
        mqx->out.data[head++] = payload_data[i];
    }
    mqx->out.count += (head - mqx->out.head);
    mqx->out.head = head;
    return 0;
}

static int mqtt_connect(mqcontext_t *mqx, const char *client_id, uint16_t keepalive)
{
    return mqtt_format_message(
                        mqx,
                        MQCONNECT, 0,
                        NULL, 0,
                        5,
						mqvhtString, "MQTT",		// protocol name
//						mqvhtString, "MQIsdp",		// protocol name
						mqvhtUint8, (unsigned)4,	// level
						mqvhtUint8, (unsigned)0,	// flags
						mqvhtUint16, (unsigned)keepalive,
						/* note that client_id is really payload, per spec,
						   but it is more convenient to add it in this way */
						mqvhtString, client_id
                        );
}

static int mqtt_subscribe(
					mqcontext_t *mqx,
					const char *topic,
					mqqos_t qos
				)
{
	static uint16_t packet_id = 0;

	packet_id++;

    return mqtt_format_message(
                        mqx,
                        MQSUBSCRIBE, 2, /* note magic 2 flag, per spec */
						NULL, 0,
						/* note, spec says these are payload not variable
						   headers, but this works the same and is easier */
						3,
						mqvhtUint16, (unsigned)packet_id,
						mqvhtString, topic,
						mqvhtUint8,  (unsigned)qos
                        );
}

static int mqtt_publish(
					mqcontext_t *mqx,
					mqqos_t qos,
					const char *topic,
					uint8_t *payload_data,
					size_t payload_size
				)
{
	static uint16_t packet_id = 0;

	packet_id++;

    return mqtt_format_message(
                        mqx,
                        MQPUBLISH, ((qos & 0x3) << 1),
                        payload_data, payload_size,
						(qos != mqqosMostOnce) ? 2 : 1,
						mqvhtString, topic,
						mqvhtUint16, (unsigned)packet_id
                        );
}

static int mqtt_publish_release(mqcontext_t *mqx, uint16_t packet_id)
{
    return mqtt_format_message(
                        mqx,
                        MQPUBREL, 2,
                        NULL, 0,
                        1,
						mqvhtUint16, (unsigned)packet_id
                        );
}

static int mqtt_publish_received(mqcontext_t *mqx, uint16_t packet_id)
{
    return mqtt_format_message(
                        mqx,
                        MQPUBREC, 0,
                        NULL, 0,
                        1,
						mqvhtUint16, (unsigned)packet_id
                        );
}

static int mqtt_publish_complete(mqcontext_t *mqx, uint16_t packet_id)
{
    return mqtt_format_message(
                        mqx,
                        MQPUBCOMP, 0,
                        NULL, 0,
                        1,
						mqvhtUint16, (unsigned)packet_id
                        );
}

static int mqtt_ping(mqcontext_t *mqx)
{
    return mqtt_format_message(
                        mqx,
                        MQPINGREQ, 0,
                        NULL, 0,
                        0
                        );
}

static int mqtt_disconnect(mqcontext_t *mqx)
{
    int result;

    // insist we're starting out fresh
    mqx->out.head = mqx->out.count = 0;

    return mqtt_format_message(
                        mqx,
                        MQDISCONNECT, 0,
                        NULL, 0,
                        0
                        );
}

static int mqtt_notify(
						mqcontext_t *mqx,
						mqqos_t qos,
						const char *topic,
						uint16_t packet_id,
						uint8_t *payload_data,
						size_t payload_size
				)
{
	http_log(4, "MQTT Got Publish: %s %04X\n", topic, packet_id);
	return 0;
}

static int mqtt_input_phase(mqcontext_t *mqx, uint8_t *data, size_t *count)
{
	int result;
	size_t bytes;
	uint32_t length, hdr_length;
	uint8_t *pdata;
    uint8_t cmd, flags, ilen;
	uint16_t packet_id;
	uint8_t qos;
	uint16_t len;
	char topic[MQTT_MAX_TOPIC];

	bytes = *count;

	if (mqx->state == mqsInit && mqx->state == mqsDone)
	{
		BERROR("Not connected\n");
		return -1;
	}
	if (bytes < 2)
	{
		// wait for enough bytes to get fixed header
		*count = 0;
		return 0;
	}
    // assume we take all
    *count = bytes;
	pdata = data;

    cmd   = *pdata >> 4;
    flags = *pdata++ & 0xF;
    ilen  = *pdata++;

	length = ilen & 0x7F;
	if (ilen & 0x80)
	{
		if (bytes < 3)
		{
			*count = 0;
			return 0;
		}
		ilen = *pdata++;
		length += (uint32_t)(ilen & 0x7F) << 7;
		if (ilen & 0x80)
		{
			if (bytes < 4)
			{
				*count = 0;
				return 0;
			}
			ilen = *pdata++;
			length += (uint32_t)(ilen & 0x7F) << 14;
			if (ilen & 0x80)
			{
				if (bytes < 5)
				{
					*count = 0;
					return 0;
				}
				ilen = *pdata++;
				length += (uint32_t)(ilen & 0x7F) << 21;
				if (ilen & 0x80)
				{
					// protocol framing error? discard all
					*count = bytes;
					return -1;
				}
			}
		}
	}
	// take fixed header out of bytes available
	//
	hdr_length = pdata - data;
	bytes -= hdr_length;

	if (length > mqx->out.size)
	{
		BERROR("packet overflow");
		return -1;
	}
	// for now, wait for all length bytes to be buffered.
	//
	if (length > bytes)
	{
		*count = 0;
		return 0;
	}
    http_log(4, "MQX state %d  cmd=%u len=%u\n", mqx->state, cmd, length);

	// absorb only header and length from input
	//
	*count = hdr_length + length;

	switch (cmd)
	{
	case MQCONNACK:
        http_log(4, "MQTT Connected\n");
        mqx->state = mqsConnected;
		result = 0;
		if (mqx->selftest)
		{
			// testing: do ping when connected
			result = mqtt_ping(mqx);
		}
		break;

	case MQPUBLISH:
		http_log(4, "MQTT PUBLISH\n");
		qos = (flags >> 1) & 0x3;
		if (qos == 0x3)
		{
			BERROR("Bad QOS");
			return -1;
		}
		len = (pdata[0] << 8) | pdata[1];
		if (len >= MQTT_MAX_TOPIC || len >= ilen)
		{
			BERROR("Topic Length");
			return -1;
		}
		pdata += 2;
		strncpy(topic, pdata, len);
		topic[sizeof(topic) - 1] = '\0';
		pdata += len;
		packet_id = (pdata[0] << 8) | pdata[1];
		pdata += 2;

		result = mqtt_notify(mqx, qos, topic, packet_id, pdata, ilen - len - 4);
		if (result)
		{
			BERROR("Notify");
			return -1;
		}
		if (qos == mqqosLeastOnce)
		{
			result = mqtt_format_message(
									mqx,
									MQPUBACK, 0,
									NULL, 0,
									1,
									mqvhtUint16, (unsigned)packet_id
									);
		}
		else if (qos == mqqosOnlyOnce)
		{
			result = mqtt_publish_received(mqx, packet_id);
		}
		break;

	case MQPUBREL:
		http_log(4, "MQTT PUBREL\n");
		packet_id = (data[0] << 8) | data[1];
		result = mqtt_publish_complete(mqx, packet_id);
        break;

	case MQPUBACK:
		packet_id = (data[0] << 8) | data[1];
		http_log(4, "MQTT PUBACK id=%04X\n", packet_id);
		result = 0;
        break;

	case MQPUBREC:
		packet_id = (data[0] << 8) | data[1];
		http_log(4, "MQTT PUBREC id=%04X\n", packet_id);
        result = mqtt_publish_release(mqx, packet_id);
        break;

	case MQPUBCOMP:
		packet_id = (data[0] << 8) | data[1];
		http_log(4, "MQTT PUBCOMP id=%04X\n", packet_id);
		result = 0;
        break;

	case MQSUBACK:
        http_log(4, "MQTT Subscribed\n");
		if (mqx->selftest)
		{
			// testing: do publish when subscribed
	        result = mqtt_publish(mqx, mqx->qos, "hithere", NULL, 0);
		}
		break;

	case MQPINGRESP:
		http_log(4, "MQTT PINGRESP\n");
		result = 0;
		if (mqx->selftest)
		{
			// testing: subscribe on a ping response
            result = mqtt_subscribe(mqx, "hithere", mqx->qos);
		}
		break;
	}
    if (result)
	{
		return -1;
	}
	return 0;
}

static int mqtt_resource(
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

        mqx->state = mqsTransportInit;
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
        case mqsTransportInit:
			// setup outgoing websocket format and mask
			//
			result = http_websocket_set_format(client, wsdfBinary, true, "mask");
			if (result)
			{
				return -1;
			}
			result = mqtt_connect(mqx, mqx->client_id, mqx->keepalive);
            if (result)
            {
                return -1;
            }
			mqx->state = mqsTransport;
            break;

		case mqsTransport:
        case mqsConnected:
            // waiting for cmd or timeout
            if (mqx->out.count)
            {
                *data = mqx->out.data + mqx->out.tail;
                *count = mqx->out.count;
                mqx->out.count = 0;
                mqx->out.head = mqx->out.tail = 0;
            }
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

static int mqtt_tcp_slice(mqcontext_t *mqx)
{
	int result;

	// TCP or TLS transport
	//
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
		time(&mqx->last_in_time);
        break;

    case mqsTransportInit:
        if (! mqx->stream)
        {
            BERROR("No stream");
            return -1;
        }
        // poll for writeable means connected
        //
        result = mqx->stream->poll(mqx->stream, writeable, 0, 10000);
        if (result < 0)
        {
            BERROR("stream");
            return -1;
        }
        if (result == 0)
        {
			time_t now;

            // not connected, remain in init state
			//
			time(&now);
			if ((now - mqx->last_in_time) > mqx->long_timeout)
			{
				BERROR("Timeout connecting");
				return -1;
			}
            break;
        }
        http_log(5, "Connected\n");

		// upgrade to TLS if indicated
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
		mqx->last_in_time = 0;
		mqx->state = mqsTransport;
        break;

	case mqsTransport:
		result = mqtt_connect(mqx, mqx->client_id, mqx->keepalive);
		if (result)
		{
			BERROR("mqtt connect");
			return -1;
		}
		mqx->state = mqsConnected;
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
	mqx->prev_state = mqx->state;
    return 0;
}

static int mqtt_ws_slice(mqcontext_t *mqx)
{
	int result;

#if MQTT_SUPPORT_WEBSOCKET
    if (mqx->state == mqsInit)
    {
		// start the http request to open the websocket
		//
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
		// any input data will cause callback for
		// resource to get called above, which drives
		//
        result = http_client_slice(mqx->client);
        if (result)
        {
            BERROR("Client error");
            return result;
        }
        // throw a rest in loop if not busy
        //
        result = http_wait_for_client_event(mqx->client, 0, 10000);
        if (result < 0)
        {
            return result;
        }
        result = 0;
    }
#else
    result = -1;
#endif
    return result;
}

int mqtt_slice(mqcontext_t *mqx)
{
    int result;

    if (mqx->transport == mqtWS || mqx->transport == mqtWSS)
    {
		result = mqtt_ws_slice(mqx);
	}
	else
	{
		result = mqtt_tcp_slice(mqx);
	}
	return result;
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
                    const char     *client_id,
                    const char 	   *server,
                    uint16_t 		port,
                    mqtransport_t   transport,
					uint16_t		keepalive,
					mqqos_t 		qos,
                    size_t 			io_max,
					time_t 			io_timeout
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
	mqx->selftest	= true;

	mqx->id 		= ++s_idx;
    mqx->state 		= mqsInit;
	mqx->prev_state = mqsDone;
    mqx->transport 	= transport;
    mqx->port 		= port;
	mqx->keepalive 	= keepalive;
	mqx->qos 		= qos;
	mqx->long_timeout = io_timeout;
	strncpy(mqx->client_id, client_id, sizeof(mqx->client_id) - 1);
	mqx->client_id[sizeof(mqx->client_id) - 1] = '\0';

	mqx->stream = NULL;
    mqx->resources = NULL;
    mqx->client = NULL;
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
		// allocate in buffer for tcp/tls transport
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

	// all transports have an out buffer, where messages are formatted
	//
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
		// tcp or tls connections just want host:port
		//
        strncpy(mqx->url, server, sizeof(mqx->url) - 1);
        mqx->url[sizeof(mqx->url) - 1] = '\0';
    }
    return mqx;
}

