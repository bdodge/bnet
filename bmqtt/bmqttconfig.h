#ifndef BMQTTCONFIG_H
#define BMQTTCONFIG_H 1

/// Smallest IO chunk
///
#define MQTT_MIN_IO_SIZE	128

/// Largest supported client ID
///
#define MQTT_MAX_ID			64

/// Largest supported topic
///
#define MQTT_MAX_TOPIC		128

/// Support HTTP-WebSockets transport
///
#define MQTT_SUPPORT_WEBSOCKET 1

/// Run internal self-test
///
#define MQTT_SUPPORT_RUN_SELFTEST 1

#endif

