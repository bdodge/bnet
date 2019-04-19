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
#ifndef BGCPIO_H
#define BGCPIO_H 1

#include "bgcp.h"

int gcp_reply_status(gcp_context_t *gcp, bool *success);
int gcp_reply_value(gcp_context_t *gcp, const char *key, char *value, size_t nvalue);

int gcp_resource_func(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     );
int gcp_encode_parameters(gcp_context_t *gcp, ...);
int gcp_request(gcp_context_t *gcp, const char *path);
int gcp_check_on_reply(gcp_context_t *gcp, bool *done);

#endif

