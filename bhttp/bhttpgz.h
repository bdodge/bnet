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
#ifndef BHTTPGZ_H
#define BHTTPGZ_H 1

#include "bhttp.h"

int http_open_compression	(http_client_t *client);
int http_compress_stream	(http_client_t *client, iostream_t *stream, uint8_t *outbuf, size_t *iocount);
int http_close_compression	(http_client_t *client);

int http_open_decompression	(http_client_t *client);
int http_decompress_stream	(http_client_t *client, iostream_t *stream, uint8_t *outbuf, size_t *iocount);
int http_close_decompression(http_client_t *client);

#endif

