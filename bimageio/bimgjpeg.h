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
#ifndef BIMGJPEG_H
#define BIMGJPEG_H 1

#include "bimageio.h"

#define JPEG_IO_SIZE 1024

int image_open_jpeg (image_stream_t *istream, image_open_intent_t intent);
int image_read_jpeg (image_stream_t *istream, uint8_t *line, size_t nline);
int image_write_jpeg(image_stream_t *istream, uint8_t *line, size_t nline);
int image_close_jpeg(image_stream_t *istream);

#endif

