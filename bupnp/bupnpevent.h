/*
 * Copyright 2020 Brian Dodge
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    upnp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef BUPNPEVENT_H
#define BUPNPEVENT_H 1

int upnp_handle_notify(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                        );

int upnp_subscribe(
                        struct upnp_server  *server,
                        upnp_subscription_t *sub_desc,
                        uint16_t            *ecode
                        );

int upnp_unsubscribe(
                        struct upnp_server  *server,
                        upnp_subscription_t *sub_desc,
                        uint16_t            *ecode
                        );

int upnp_notify_subscribers(
                        struct upnp_server  *server,
                        struct upnp_service *service
                        );
#endif
