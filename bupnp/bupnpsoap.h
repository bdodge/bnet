/*
 * Copyright 2019 Brian Dodge
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
#ifndef BUPNPSOAP_H
#define BUPNPSOAP_H 1

int upnp_get_arg_value(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        upnp_val_t **pval
                        );

int upnp_get_arg_value_as_string(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        char *buf,
                        size_t nbuf,
                        const char **value,
                        size_t *slen
                        );

int upnp_get_arg_value_as_int(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        int *pval
                        );

int upnp_set_arg_value_from_string(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        const char *sval
                        );

int upnp_set_arg_value_from_int(
                        upnp_service_t *service,
                        const char *action_name,
                        const char *arg_name,
                        int ival
                        );

int upnp_handle_control_url(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                        );

int upnp_handle_event_url(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                        );

#endif
