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

#include "bmtp.h"

#define PTPIO_DBG_LEVEL 5

static void hexdump(uint8_t *data, size_t ndata)
{
    size_t i, j, k;

    for (i = j = 0; i < ndata; i++)
    {
        butil_log(3, "%02X ", data[i]);
        if ((i & 0xF) == 0xF)
        {
            butil_log(3, "  |");
            for (k = 0; k < 16; k++)
            {
                butil_log(3, "%c", (data[j] >= 0x20 && data[j] <= 0x7E) ? data[j] : ' ');
                j++;
            }

            butil_log(3, "|\n");
        }
    }
    if (j < ndata)
    {
        while (i & 0xf)
        {
            butil_log(3, "   ");
            i++;
        }

        butil_log(3, "  |");
        while (j < ndata)
        {
            butil_log(3, "%c", (data[j] >= 0x20 && data[j] <= 0x7E) ? data[j] : ' ');
            j++;
        }

        while (j & 0xF)
        {
            butil_log(3, " ");
            j++;
        }

        butil_log(3, "|\n");
    }
}

int ptp_fill_in(socket_t sock, ioring_t *input, int tos, int tous)
{
    uint32_t room;
    int rc;

    if (sock == INVALID_SOCKET)
    {
        return -1;
    }

    room = input->size - input->count;
    if (room > 0)
    {
        if (input->head >= input->tail)
        {
            room = input->size - input->head;
        }
        else
        {
            room = input->tail - input->head;
        }

        rc = iostream_posix_poll_filedesc(sock, readable, tos, tous);
        if (rc > 0)
        {
            rc = read(sock, input->data + input->head, room);
            if (rc > 0)
            {
                if (PTPIO_DBG_LEVEL <= butil_get_log_level())
                {
                    butil_log(PTPIO_DBG_LEVEL, "----read %d %u\n", sock, rc);
                    if (PTPIO_DBG_LEVEL > 4)
                    {
                        hexdump(input->data + input->head, rc);
                    }
                }

                input->head += rc;
                if (input->head >= input->size)
                {
                    input->head = 0;
                }

                input->count += rc;
            }
            else if (rc == 0)
            {
                // read of 0 after select 1 means connection over
                //
                rc = -1;
            }
        }
    }

    return (input->count > 0) ? 1 : ((rc >= 0) ? 0 : rc);
}

uint32_t ptp_read(ioring_t *input, uint8_t *buf, size_t count)
{
    size_t moved;

    if (count <= 0)
    {
        return 0;
    }

    if (input->count >= count)
    {
        for (moved = 0; moved < count; moved++)
        {
            *buf++ = input->data[input->tail++];
            if (input->tail >= input->size)
            {
                input->tail = 0;
            }
        }
        input->count -= count;
        return count;
    }

    return 0;
}

uint32_t ptp_consume(ioring_t *input, size_t count)
{
    size_t moved;

    if (count <= 0)
    {
        return 0;
    }

    if (input->count >= count)
    {
        for (moved = 0; moved < count; moved++)
        {
            if (++input->tail >= input->size)
            {
                input->tail = 0;
            }
        }
        input->count -= count;
        return count;
    }

    return 0;
}

int ptp_flush_out(socket_t sock, ioring_t *output, int tos, int tous)
{
    uint32_t tosend;
    int wc;

    if (sock == INVALID_SOCKET)
    {
        return -1;
    }

    while (output->count > 0)
    {
        tosend = output->count;
        if (output->head < output->tail)
        {
            tosend = output->size - output->head;
        }

        wc = iostream_posix_poll_filedesc(sock, writeable, tos, tous);
        if (wc > 0)
        {
            if (PTPIO_DBG_LEVEL <= butil_get_log_level())
            {
                butil_log(3, "----write %d %u\n", sock, tosend);
                if (PTPIO_DBG_LEVEL > 5)
                {
                    hexdump(output->data + output->tail, tosend);
                }
            }

            wc = write(sock, output->data + output->tail, tosend);
            if (wc > 0)
            {
                output->tail += wc;
                if (output->tail >= output->size)
                {
                    output->tail = 0;
                }

                output->count -= wc;
            }
            else if (wc == 0)
            {
                // client closed
                wc = -1;
                break;
            }
        }
        else if (wc < 0)
        {
            break;
        }
    }
    if (output->count == 0)
    {
        output->head = 0;
        output->tail = 0;
    }

    return (output->count > 0) ? -1 : wc;
}

uint32_t ptp_write(socket_t sock, ioring_t *output, const uint8_t *buf, size_t count)
{
    size_t moved;

    if (count <= 0)
    {
        return 0;
    }

    if ((output->count + count) > output->size)
    {
        int rc;

        rc = ptp_flush_out(sock, output, 5, 0);
        if (rc < 0)
        {
            return 0;
        }
    }

    if (output->count <= (output->size + count))
    {
        for (moved = 0; moved < count; moved++)
        {
            output->data[output->head++] = *buf++;
            if (output->head >= output->size)
            {
                output->head = 0;
            }
        }
        output->count += count;
        return count;
    }

    return 0;
}


