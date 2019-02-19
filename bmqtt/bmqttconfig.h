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

