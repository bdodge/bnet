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

#define mqtt_client_id "hello"

typedef enum
{
	mqvhtUint8 = 1,
	mqvhtUint16,
	mqvhtUint32,
	mqvhtString
}
mqvhtype_t;

static void smqtt_subrec_destroy(mqsubrec_t *subrec)
{
	if (subrec)
	{
		free(subrec);
	}
}

static mqsubrec_t *smqtt_subrec_create(const char *topic, mqqos_t qos)
{
	mqsubrec_t *subrec;
	int result;

	subrec = (mqsubrec_t *)malloc(sizeof(mqsubrec_t));
	if (! subrec)
	{
		return NULL;
	}
	strncpy(subrec->topic, topic, sizeof(subrec->topic));
	subrec->qos = qos;
	subrec->notify = NULL;
	subrec->priv = NULL;
	time(&subrec->time);
	return subrec;
}

static int smqtt_clean_subrecs(mqcontext_t *mqx)
{
	mqsubrec_t *subrec;
	mqsubrec_t *nxtrec;

	subrec = mqx->subrecs;
	mqx->subrecs = NULL;
	while (subrec)
	{
		nxtrec = subrec->next;
		smqtt_subrec_destroy(subrec);
		subrec = nxtrec;
	}
	subrec = mqx->subrecs_leaving;
	mqx->subrecs_leaving = NULL;
	while (subrec)
	{
		nxtrec = subrec->next;
		smqtt_subrec_destroy(subrec);
		subrec = nxtrec;
	}
	subrec = mqx->subrecs_pending;
	mqx->subrecs_pending = NULL;
	while (subrec)
	{
		nxtrec = subrec->next;
		smqtt_subrec_destroy(subrec);
		subrec = nxtrec;
	}
	subrec = mqx->subrecs_published;
	mqx->subrecs_published = NULL;
	while (subrec)
	{
		nxtrec = subrec->next;
		smqtt_subrec_destroy(subrec);
		subrec = nxtrec;
	}
	return 0;
}

static int smqtt_format_message(
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
				varhdr_len = 2 + strlen((char*)varhdr_data);
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
				varhdr_len = strlen((char*)varhdr_data);
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

static int smqtt_connect(mqcontext_t *mqx, const char *client_id, uint16_t keepalive)
{
	butil_log(5, "-> MQTT Connect\n");
    return smqtt_format_message(
                        mqx,
                        MQCONNECT, 0,
                        NULL, 0,
                        5,
						mqvhtString, "MQTT",		// protocol name
						mqvhtUint8, (unsigned)4,	// level
						mqvhtUint8, (unsigned)0,	// flags
						mqvhtUint16, (unsigned)keepalive,
						/* note that client_id is really payload, per spec,
						   but it is more convenient to add it in this way */
						mqvhtString, client_id
                        );
}

static int smqtt_subscribe(
					mqcontext_t *mqx,
					mqsubrec_t  *subrec
				)
{
	if (! mqx || ! subrec)
	{
		return -1;
	}
	subrec->id = mqx->packet_id++;

	butil_log(5, "-> MQTT SUBSCRIBE id=%04X\n", (unsigned)subrec->id);

    return smqtt_format_message(
                        mqx,
                        MQSUBSCRIBE, 2, /* note magic flag bit, as per spec */
						NULL, 0,
						/* note, spec says these are payload not variable
						   headers, but this works the same and is easier */
						3,
						mqvhtUint16, (unsigned)subrec->id,
						mqvhtString, subrec->topic,
						mqvhtUint8,  (unsigned)subrec->qos
                        );
}

static int smqtt_unsubscribe(
					mqcontext_t *mqx,
					mqsubrec_t  *subrec
				)
{
	if (! mqx || ! subrec)
	{
		return -1;
	}
	subrec->id = mqx->packet_id++;

	butil_log(5, "-> MQTT UNSUBSCRIBE id=%04X\n", (unsigned)subrec->id);

    return smqtt_format_message(
                        mqx,
                        MQUNSUBSCRIBE, 2, /* note magic flag bit, as per spec */
						NULL, 0,
						/* note, spec says these are payload not variable
						   headers, but this works the same and is easier */
						2,
						mqvhtUint16, (unsigned)subrec->id,
						mqvhtString, subrec->topic
                        );
}

static int smqtt_publish(
					mqcontext_t *mqx,
					mqsubrec_t *subrec,
					uint8_t *payload_data,
					size_t payload_size
				)
{
	subrec->id = mqx->packet_id++;

	butil_log(5, "-> MQTT PUBLISH id=%04X\n", (unsigned)subrec->id);

	return smqtt_format_message(
                        mqx,
                        MQPUBLISH, ((subrec->qos & 0x3) << 1),
                        payload_data, payload_size,
						(subrec->qos != mqqosMostOnce) ? 2 : 1,
						mqvhtString, subrec->topic,
						mqvhtUint16, (unsigned)subrec->id
                        );
}

static int smqtt_publish_release(mqcontext_t *mqx, uint16_t packet_id)
{
	butil_log(5, "-> MQTT PUBREL id=%04X\n", (unsigned)packet_id);

	return smqtt_format_message(
                        mqx,
                        MQPUBREL, 2,
                        NULL, 0,
                        1,
						mqvhtUint16, (unsigned)packet_id
                        );
}

static int smqtt_publish_ack(mqcontext_t *mqx, uint16_t packet_id)
{
	butil_log(5, "-> MQTT PUBACK id=%04X\n", (unsigned)packet_id);

	return smqtt_format_message(
                        mqx,
                        MQPUBACK, 0,
                        NULL, 0,
                        1,
						mqvhtUint16, (unsigned)packet_id
                        );
}

static int smqtt_publish_received(mqcontext_t *mqx, uint16_t packet_id)
{
	butil_log(5, "-> MQTT PUBREC id=%04X\n", (unsigned)packet_id);

	return smqtt_format_message(
                        mqx,
                        MQPUBREC, 0,
                        NULL, 0,
                        1,
						mqvhtUint16, (unsigned)packet_id
                        );
}

static int smqtt_publish_complete(mqcontext_t *mqx, uint16_t packet_id)
{
	butil_log(5, "-> MQTT PUBCOMP id=%04X\n", (unsigned)packet_id);

	return smqtt_format_message(
                        mqx,
                        MQPUBCOMP, 0,
                        NULL, 0,
                        1,
						mqvhtUint16, (unsigned)packet_id
                        );
}

static int smqtt_ping(mqcontext_t *mqx)
{
	butil_log(5, "-> MQTT PING\n");

	return smqtt_format_message(
                        mqx,
                        MQPINGREQ, 0,
                        NULL, 0,
                        0
                        );
}

static int smqtt_disconnect(mqcontext_t *mqx)
{
    int result;

	butil_log(5, "-> MQTT DISCONNECT\n");

	// insist we're starting out fresh
    mqx->out.head = mqx->out.count = 0;

    return smqtt_format_message(
                        mqx,
                        MQDISCONNECT, 0,
                        NULL, 0,
                        0
                        );
}

static int smqtt_handle_puback(
						mqcontext_t *mqx,
						bool is_pubrec,
						uint16_t packet_id
				)
{
	mqsubrec_t *subrec, *prevsubrec;

	prevsubrec = NULL;

	for (
			subrec = mqx->subrecs_published;
			subrec;
			subrec = subrec->next
	)
	{
		if (subrec->id == packet_id)
		{
			// unlink from published
			if (prevsubrec)
			{
				prevsubrec->next = subrec->next;
			}
			else
			{
				mqx->subrecs_published = subrec->next;
			}
			butil_log(5, "Removing record of published topic %s id=%04X\n",
						subrec->topic, subrec->id);
			smqtt_subrec_destroy(subrec);
			return 0;
		}
		prevsubrec = subrec;
	}
	butil_log(1, "MQTT - No such id=%04X for puback/rec\n");
	// this is non-fatal, it could have been yanked out?
	return 0;
}

static int smqtt_handle_suback(
						mqcontext_t *mqx,
						bool is_unsub,
						uint16_t packet_id
				)
{
	mqsubrec_t *subrec, *prevsubrec;

	prevsubrec = NULL;

	for (
			subrec = is_unsub ?
				mqx->subrecs_leaving : mqx->subrecs_pending;
			subrec;
			subrec = subrec->next
	)
	{
		if (subrec->id == packet_id)
		{
			// move from pending to subscribed
			if (prevsubrec)
			{
				prevsubrec->next = subrec->next;
			}
			else
			{
				if (is_unsub)
				{
					mqx->subrecs_leaving = subrec->next;
				}
				else
				{
					mqx->subrecs_pending = subrec->next;
				}
			}
			subrec->next = mqx->subrecs;
			mqx->subrecs = subrec;

			butil_log(5, "Successfully %ssubscribed to %s\n",
						is_unsub ? "un" : "", subrec->topic);
			return 0;
		}
		prevsubrec = subrec;
	}
	butil_log(1, "MQTT - No such id=%04X for un/suback\n");
	// this is non-fatal, it could have been yanked out?
	return 0;
}

static int smqtt_selftest_notify(mqcontext_t *mqx, const char *topic, void *priv)
{
	int result;

	butil_log(4, "MQTT TEST ------ topic:%s priv=%s\n", topic, priv ? (char*)priv : "error");

	// unsubscribe in the callback
	result = mqtt_unsubscribe(mqx, topic);
	return result;
}

static int smqtt_notify(
						mqcontext_t *mqx,
						const char *topic,
						mqqos_t qos,
						uint16_t packet_id,
						uint8_t *payload_data,
						size_t payload_size
				)
{
	butil_log(4, "MQTT Got Notified of: %s %04X\n", topic, packet_id);
	mqsubrec_t *subrec;
	int result;

	for (subrec = mqx->subrecs; subrec; subrec = subrec->next)
	{
		if (! strcmp(subrec->topic, topic))
		{
			result = 0;

			if (subrec->notify)
			{
				result = subrec->notify(mqx, topic, subrec->priv);
			}
			return result;
		}
	}
	butil_log(1, "MQTT - No subscription for %s\n", topic);
	return 0;
}

static int smqtt_input_phase(mqcontext_t *mqx, uint8_t *data, size_t *count)
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
    butil_log(4, "MQX state %d  cmd=%u len=%u\n", mqx->state, cmd, length);

	// absorb only header and length from input
	//
	*count = hdr_length + length;

	switch (cmd)
	{
	case MQCONNACK:
        butil_log(5, "<- MQTT CONNACK\n");
        mqx->state = mqsConnected;
		result = 0;
		if (mqx->selftest)
		{
			// testing: do ping when connected
			result = smqtt_ping(mqx);
		}
		break;

	case MQPUBLISH:
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
		strncpy(topic, (char*)pdata, len);
		topic[sizeof(topic) - 1] = '\0';
		pdata += len;
		packet_id = (pdata[0] << 8) | pdata[1];
		pdata += 2;

		butil_log(5, "<- MQTT PUBLISH id=%04X\n", (unsigned)packet_id);

		result = smqtt_notify(mqx, topic, qos, packet_id, pdata, ilen - len - 4);
		if (result)
		{
			BERROR("Notify");
			return -1;
		}
		if (qos == mqqosLeastOnce)
		{
			result = smqtt_publish_ack(mqx, packet_id);
		}
		else if (qos == mqqosOnlyOnce)
		{
			result = smqtt_publish_received(mqx, packet_id);
		}
		break;

	case MQPUBREL:
		packet_id = (pdata[0] << 8) | pdata[1];
		butil_log(5, "<- MQTT PUBREL id=%04X\n", packet_id);
		result = smqtt_publish_complete(mqx, packet_id);
        break;

	case MQPUBACK:
		packet_id = (pdata[0] << 8) | pdata[1];
		butil_log(5, "<- MQTT PUBACK id=%04X\n", packet_id);
		result = smqtt_handle_puback(mqx, false, packet_id);
        break;

	case MQPUBREC:
		packet_id = (pdata[0] << 8) | pdata[1];
		butil_log(5, "<- MQTT PUBREC id=%04X\n", packet_id);
		result = smqtt_handle_puback(mqx, true, packet_id);
		if (! result)
		{
	        result = smqtt_publish_release(mqx, packet_id);
		}
        break;

	case MQPUBCOMP:
		packet_id = (pdata[0] << 8) | pdata[1];
		butil_log(5, "<- MQTT PUBCOMP id=%04X\n", packet_id);
		result = 0;
        break;

	case MQSUBACK:
		packet_id = (pdata[0] << 8) | pdata[1];
        butil_log(5, "<- MQTT SUBACK id=%04X\n", packet_id);
		// process ack
		result = smqtt_handle_suback(mqx, false, packet_id);
		if (mqx->selftest && ! result)
		{
			// testing: do publish when subscribed
	        result = mqtt_publish(mqx, "hithere", mqx->qos, NULL, 0);
		}
		break;

	case MQUNSUBACK:
		packet_id = (pdata[0] << 8) | pdata[1];
        butil_log(5, "<- MQTT UNSUBACK id=%04X\n", packet_id);
		// process ack
		result = smqtt_handle_suback(mqx, true, packet_id);
		if (mqx->selftest && ! result)
		{
			// testing: disconnect now
	        result = mqtt_disconnect(mqx);
		}
		break;

	case MQPINGRESP:
		butil_log(5, "<- MQTT PINGRESP\n");
		result = 0;
		if (mqx->selftest)
		{
			// testing: subscribe on the ping response
            result = mqtt_subscribe(mqx, "hithere",
					mqx->qos, smqtt_selftest_notify, (void*)"hi");
		}
		break;

	default:
		butil_log(1, "<- ??? 0x%02X\n", cmd);
		break;
	}
    if (result)
	{
		return -1;
	}
	return 0;
}

static int smqtt_resource(
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

    mqx = (mqcontext_t *)resource->priv;

    switch (cbtype)
    {
    case httpRequestHeader:
		break;

    case httpRequest:

        butil_log(5, "MQTT: request\n");

        mqx->state = mqsTransportInit;
        client->ws_upgrade = true;
        break;

    case httpUploadData:

        bytes = *count;
        *count = 0;

        if (! mqx)
        {
            BERROR("No CTX");
            return -1;
        }
        butil_log(6, "MQX state %d\n", mqx->state);

        switch (mqx->state)
        {
        case mqsInit:
        case mqsTransportInit:
			if (! client->ws_stream)
			{
				break;
			}

			// setup outgoing websocket format and mask
			//
			result = http_websocket_set_format(client, wsdfBinary, true, "mask");
			if (result)
			{
				return -1;
			}
			result = smqtt_connect(mqx, mqx->client_id, mqx->keepalive);
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
            butil_log(5, "MQTT: Sending Bytes: %u\n", *count);
        }
        break;

    case httpDownloadData:

        butil_log(5, "MQTT: Getting Bytes: %u\n", *count);
        bytes = *count;
        if (bytes < 2)
        {
            // wait for enough bytes
            *count = 0;
            break;
        }
		result = smqtt_input_phase(mqx, *data, count);
		if (result)
		{
			return -1;
		}
        break;

    case httpDownloadDone:
        return 0;

    case httpComplete:

        butil_log(5, "MQTT: complete\n");
        break;

    default:
        return -1;
    }
    return 0;
}

static int smqtt_client_input(mqcontext_t *mqx, int to_secs, int to_usecs)
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
                    butil_log(5, "end of connection %u\n", mqx->id);
                    return -1;
                }
                mqx->in.head += len;
                if (mqx->in.head >= mqx->in.size)
                {
                    mqx->in.head = 0;
                }
                mqx->in.count += len;
                butil_log(7, "read %d on id %u\n", len, mqx->id);
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
            butil_log(3, "cl:%u read timeout\n", mqx->id);
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

static int smqtt_client_output(mqcontext_t *mqx, int to_secs, int to_usecs)
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
			butil_log(3, "mqx %u write timeout\n", mqx->id);
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

static int smqtt_tcp_connect(mqcontext_t *mqx)
{
	int result;

	// open a tcp socket to broker
	//
	mqx->stream = iostream_create_from_tcp_connection(mqx->url, mqx->port);
	if (! mqx->stream)
	{
		BERROR("Stream create");
		return -1;
	}
	mqx->state = mqsTransportInit;
	time(&mqx->last_in_time);
	return 0;
}

static int smqtt_tcp_slice(mqcontext_t *mqx)
{
	int result;

	// TCP or TLS transport
	//
    switch (mqx->state)
    {
    case mqsInit:
		// shouldn't really get here, call connect first
		BERROR("Call connect function");
		result = 1;

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
			// wait more
			break;
		}
		butil_log(5, "Connected\n");

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
			butil_log(5, "TLS Connected\n");
			mqx->stream = stream;
		}
		mqx->state = mqsTransport;
		mqx->last_in_time = 0;
		break;

	case mqsTransport:
		result = smqtt_connect(mqx, mqx->client_id, mqx->keepalive);
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
			result = smqtt_client_output(mqx, 0, 10000);
			if (result)
			{
				BERROR("Output");
				return result;
			}
		}
		// check input
		result = smqtt_client_input(mqx, 0, 10000);
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

			result = smqtt_input_phase(mqx, mqx->in.data + mqx->in.tail, &count);
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
		break;

    case mqsDone:
        break;
    }
	mqx->prev_state = mqx->state;
    return 0;
}

static int smqtt_ws_connect(mqcontext_t *mqx)
{
	int result;

	// start the http request to open the websocket
	//
	result = http_client_request(mqx->client, httpGet,
			mqx->url, httpTCP, false, NULL, mqx->resources);
	if (result)
	{
		return result;
	}
	mqx->state = mqsTransportInit;
	return 0;
}

static int smqtt_ws_slice(mqcontext_t *mqx)
{
	int result;

#if MQTT_SUPPORT_WEBSOCKET
    if (mqx->state == mqsInit)
    {
		BERROR("Call connect first");
		return -1;
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
        result = http_wait_for_client_event(mqx->client, 0, 100000);
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

int mqtt_subscribe(
					mqcontext_t *mqx,
					const char *topic,
					mqqos_t qos,
					mqnotify_func_t *notify,
					void *priv
				)
{
	mqsubrec_t *subrec;
	int result;

	subrec = smqtt_subrec_create(topic, qos);
	if (! subrec)
	{
		BERROR("Can't alloc subrec");
		return -1;
	}
	subrec->notify = notify;
	subrec->priv = priv;

	// format the subscribe
	result = smqtt_subscribe(mqx, subrec);
	if (result)
	{
		BERROR("Can't subscribe");
		free(subrec);
		return result;
	}
	// place subrec on the to-ack list
	subrec->next = mqx->subrecs_pending;
	mqx->subrecs_pending = subrec;
	return 0;
}

int mqtt_unsubscribe(
					mqcontext_t *mqx,
					const char *topic
				)
{
	mqsubrec_t *subrec;
	mqsubrec_t *prevsubrec;
	int result;

	prevsubrec = NULL;
	for (subrec = mqx->subrecs; subrec; subrec = subrec->next)
	{
		if (! strcmp(subrec->topic, topic))
		{
			// move from listed to leaving
			if (prevsubrec)
			{
				prevsubrec->next = subrec->next;
			}
			else
			{
				mqx->subrecs = subrec->next;
			}
			// format the unsubscribe
			result = smqtt_unsubscribe(mqx, subrec);
			if (result)
			{
				BERROR("Can't unsubscribe");
				free(subrec);
				return result;
			}
			// place on leavers and expect to ack
			subrec->next = mqx->subrecs_leaving;
			mqx->subrecs_leaving = subrec;
			return 0;
		}
		prevsubrec = subrec;
	}
	butil_log(1, "MQTT - No such subscription for %s\n", topic);
	return -1;
}

int mqtt_publish(
					mqcontext_t *mqx,
					const char *topic,
					mqqos_t qos,
					uint8_t *payload_data,
					size_t payload_size
				)
{
	mqsubrec_t *subrec;
	int result;

	subrec = smqtt_subrec_create(topic, qos);
	if (! subrec)
	{
		BERROR("Can't alloc subrec");
		return -1;
	}
	// format the publish
	result = smqtt_publish(mqx, subrec, payload_data, payload_size);
	if (result)
	{
		BERROR("Can't publish");
		free(subrec);
		return result;
	}
	// place subrec on the to-ack list if especting an ack
	if (qos != mqqosMostOnce)
	{
		subrec->next = mqx->subrecs_published;
		mqx->subrecs_published = subrec;
	}
	else
	{
		free(subrec);
	}
	return 0;
}

int mqtt_disconnect(mqcontext_t *mqx)
{
	int result;
	time_t now, then;

	if (
			mqx->state == mqsInit
		||	mqx->state == mqsDone
	)
	{
		return 0;
	}
	result = 0;

	if (mqx->state == mqsConnected)
	{
		result = smqtt_disconnect(mqx);
		if (result)
		{
			BERROR("Disconnect");
			return result;
		}
	}
	return result;
}

int mqtt_connect(mqcontext_t *mqx)
{
	int result;

	if (mqx->state == mqsInit || mqx->state == mqsDone)
	{
	    if (mqx->transport == mqtWS || mqx->transport == mqtWSS)
	    {
			result = smqtt_ws_connect(mqx);
		}
		else
		{
			result = smqtt_tcp_connect(mqx);
		}
		return result;
	}
	else
	{
		BERROR("Already connecting");
		return -1;
	}
}

int mqtt_slice(mqcontext_t *mqx)
{
    int result;

    if (mqx->transport == mqtWS || mqx->transport == mqtWSS)
    {
		result = smqtt_ws_slice(mqx);
	}
	else
	{
		result = smqtt_tcp_slice(mqx);
	}
	return result;
}

void mqtt_client_free(mqcontext_t *mqx)
{
	// make sure disconnected
	mqtt_disconnect(mqx);

	// remove any pub-sub records
	smqtt_clean_subrecs(mqx);

#if MQTT_SUPPORT_WEBSOCKET
	// cleanup http if in use
    if (mqx->client)
    {
        http_client_free(mqx->client);
    }
	if (mqx->resources)
	{
		http_resource_free(mqx->resources);
	}
#endif
	// make sure stream is closed and free
	if (mqx->stream)
	{
		mqx->stream->close(mqx->stream);
		mqx->stream = NULL;
	}
	// clean out
    if (mqx->out.data)
    {
        free(mqx->out.data);
    }
	// clean out
    if (mqx->in.data)
    {
        free(mqx->in.data);
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
	mqx->id 		= ++s_idx;
	mqx->packet_id  = 1;
    mqx->state 		= mqsInit;
	mqx->prev_state = mqsDone;
    mqx->transport 	= transport;
    mqx->port 		= port;
	mqx->keepalive 	= keepalive;
	mqx->qos 		= qos;
	mqx->long_timeout = io_timeout;
	strncpy(mqx->client_id, client_id, sizeof(mqx->client_id) - 1);
	mqx->client_id[sizeof(mqx->client_id) - 1] = '\0';

	mqx->subrecs_published = NULL;
	mqx->subrecs_pending = NULL;
	mqx->subrecs_leaving = NULL;
	mqx->subrecs = NULL;

	mqx->selftest	= false;
#ifdef MQTT_SUPPORT_RUN_SELFTEST
#if MQTT_SUPPORT_RUN_SELFTEST
	mqx->selftest	= true;
#endif
#endif
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
        result = http_add_func_resource(&mqx->resources,
						(transport == mqtWSS) ? schemeWSS : schemeWS,
						"/mqtt", NULL, smqtt_resource, mqx);
        if (result)
        {
            BERROR("Can't make resource");
            mqtt_client_free(mqx);
            return NULL;
        }
        butil_paste_url(mqx->url, sizeof(mqx->url),
				(transport == mqtWSS) ? schemeWSS : schemeWS, server, port, "mqtt");

        mqx->client = http_client_create(NULL, true);
        if (! mqx->client)
        {
            BERROR("No memory for client");
            mqtt_client_free(mqx);
            return NULL;
        }
        butil_log(3, "MQTT Client to %s\n", mqx->url);

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

