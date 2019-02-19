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
#ifndef HTTPDAV_H
#define HTTPDAV_H 1

struct http_client;

typedef struct
{
    int     pathpos;    ///< end of path when entry pushed
    DIR     *d;         ///< dir enumeration state
}
webdav_findstate_t;

void http_webdav_findstate_init(struct http_client *client);
int http_webdav_find_slice(struct http_client *client);
int http_webdav_request(struct http_client *client);

#endif

