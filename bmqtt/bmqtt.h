#ifndef BMQTT_H
#define BMQTT_H 1

#include "bmqttconfig.h"
#include "bnetheaders.h"
#include "bstreamio.h"

#if MQTT_SUPPORT_WEBSOCKET
#include "bhttp.h"
#endif

typedef enum
{
	MQCONNECT = 1,		// Client to Server: 	Client request to connect to Server
	MQCONNACK = 2,		// Server to Client: 	Connect acknowledgment
	MQPUBLISH = 3,		// Client <-> Server: 	Publish message
	MQPUBACK = 4,		// Client <-> Server: 	Publish acknowledgment
	MQPUBREC = 5,		// Client <-> Server: 	Publish received (assured delivery part 1)
	MQPUBREL = 6,		// Client <-> Server: 	Publish release (assured delivery part 2)
	MQPUBCOMP = 7,		// Client <-> Server: 	Publish complete (assured delivery part 3)
	MQSUBSCRIBE = 8,	// Client to Server:	Client subscribe request
	MQSUBACK = 9,		// Server to Client:	Subscribe acknowledgment
	MQUNSUBSCRIBE = 10,	//Client to Server:		Unsubscribe request
	MQUNSUBACK = 11,	// Server to Client:	Unsubscribe acknowledgment
	MQPINGREQ = 12,		// Client to Server:	PING request
	MQPINGRESP = 13,	// Server to Client:	PING response
	MQDISCONNECT = 14,	// Client to Server:	Client is disconnecting
}
mqtt_command_t;

typedef enum
{
	mqqosMostOnce = 0,
	mqqosLeastOnce = 1,
	mqqosOnlyOnce = 2
}
mqqos_t;

typedef enum
{
	mqtTCP,
	mqtTLS,
	mqtWS,
	mqtWSS
}
mqtransport_t;

typedef enum
{
    mqsInit,
    mqsTransportInit,
    mqsTransport,
    mqsConnect,
    mqsPing,
	mqsSubscribe,
	mqsPublish,
	mqsPublishRelease,
	mqsPublishComplete,
	mqsIdle,
    mqsDisconnect,
    mqsDone,
}
mqstate_t;

typedef struct
{
	uint32_t 		id;
	mqqos_t 		qos;
    mqstate_t 		state;
    mqstate_t 		prev_state;
	mqtransport_t 	transport;
	uint16_t 		keepalive;
	char 			client_id[MQTT_MAX_ID];
	char 			url[HTTP_MAX_URL];
	uint16_t 		port;
	iostream_t 	   *stream;
	time_t 			long_timeout;
	time_t 			last_in_time;
	time_t 			last_out_time;
    ioring_t 		in;
    ioring_t 		out;
#if MQTT_SUPPORT_WEBSOCKET
	http_client_t  *client;
    http_resource_t*resources;
#endif
}
mqcontext_t;

int mqtt_slice(mqcontext_t *mqx);
void mqtt_client_free(mqcontext_t *mqx);
mqcontext_t *mqtt_client_create(
					const char     *client_id,
					const char     *server,
					uint16_t 	   	port,
					mqtransport_t  	transport,
					uint16_t 	   	keepalive,
					mqqos_t 	   	qos,
					size_t 			io_max,
					time_t 			io_timeout
					);

#endif
