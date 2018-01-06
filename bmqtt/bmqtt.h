
#include "bhttp.h"

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
	MQUNSUBSCRIBE = 10,	//Client to Server:	Unsubscribe request
	MQUNSUBACK = 11,	// Server to Client:	Unsubscribe acknowledgment
	MQPINGREQ = 12,		// Client to Server:	PING request
	MQPINGRESP = 13,	// Server to Client:	PING response
	MQDISCONNECT = 14,	// Client to Server:	Client is disconnecting
}
mqtt_command_t;

