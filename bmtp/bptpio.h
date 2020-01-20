/*
 * Copyright 2020 Brian Dodge
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
#ifndef BPTPIO_H
#define BPTPIO_H 1

int      ptp_fill_in(socket_t sock, ioring_t *input, int tos, int tous);
uint32_t ptp_read(ioring_t *input, uint8_t *buf, size_t count);
uint32_t ptp_consume(ioring_t *input, size_t count);
int      ptp_flush_out(socket_t sock, ioring_t *output, int tos, int tous);
uint32_t ptp_write(socket_t sock, ioring_t *output, const uint8_t *buf, size_t count);

#endif

