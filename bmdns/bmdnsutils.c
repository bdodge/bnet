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
#include "bmdnsutils.h"
#include "bmdns.h"

const char *str_for_sockaddr(struct sockaddr_in *sockaddr)
{
    static char ipbuf[32];
    uint32_t ip_addr;

    if (! sockaddr)
    {
        return "<nil>";
    }
    ip_addr = (uint32_t)sockaddr->sin_addr.s_addr;

    snprintf(ipbuf, sizeof(ipbuf), "%d:%d:%d:%d %u",
                (ip_addr & 0xFF),
                ((ip_addr >> 8) & 0xFF),
                ((ip_addr >> 16) & 0xFF),
                ((ip_addr >> 24) & 0xFF),
                (uint32_t)(uint16_t)sockaddr->sin_port
            );
    return ipbuf;
}

mdns_packet_t *mdns_pkt_alloc(mdns_responder_t *res)
{
    mdns_packet_t *pkt;

    if (! res || ! res->pkt_free)
    {
        return NULL;
    }
    pkt = res->pkt_free;
    res->pkt_free = res->pkt_free->next;

    pkt->io.head = 0;
    pkt->io.tail = 0;
    pkt->io.count = 0;
    pkt->io.size = sizeof(pkt->data);
    pkt->io.data = pkt->data;
    pkt->next = NULL;

    return pkt;
}

void mdns_pkt_free(mdns_responder_t *res, mdns_packet_t *pkt)
{
    if (! res || ! pkt)
    {
        BERROR("no pkt");
        return;
    }
    pkt->next = res->pkt_free;
    res->pkt_free = pkt;
}

void mdns_dump_packet(const char *because, mdns_packet_t *pkt, int level)
{
    int i, j;
    char pkt_text[18];
    uint8_t data;
    int tail;

    butil_log(level, "Pkt %4d bytes  %s\n", pkt ? pkt->io.count : 0, because);

    if (! pkt)
    {
        butil_log(level, "<nil>\n");
        return;
    }
    if (! pkt->io.count)
    {
        butil_log(level, "<empty>\n");
        return;
    }
    if (pkt->io.count < MDNS_OFF_QUESTION)
    {
        butil_log(level, "<truncated>\n");
        return;
    }
    tail = pkt->io.tail;

    butil_log(level, "ID=%u\n", (uint32_t)pkt->id);
    butil_log(level, "Flags=%04X\n", pkt->flags);
    butil_log(level, "Questions=%u\n", (uint32_t)pkt->qdcount);
    butil_log(level, "Answer RRs=%u\n", (uint32_t)pkt->ancount);
    butil_log(level, "Authority RRs=%u\n", (uint32_t)pkt->nscount);
    butil_log(level, "Additional RRs=%u\n", (uint32_t)pkt->arcount);

    pkt->io.tail = tail;

    for (i = j = 0; i < pkt->io.count; i++)
    {
        data = pkt->io.data[tail];

        butil_log(level, "%02X ", data);
        if (data >= ' ' && data <= '~')
        {
            pkt_text[j] = data;
        }
        else
        {
            pkt_text[j] = '.';
        }
        pkt_text[++j] = '\0';

        tail++;
        if (tail == pkt->io.size)
        {
            tail = 0;
        }
        if (j == 16)
        {
            butil_log(level, "    | %s |\n", pkt_text);
            j = 0;
        }
    }
    if (j > 0)
    {
        while (j < 16)
        {
            butil_log(level, "   ");
            pkt_text[j++] = '-';
        }
        pkt_text[j] = '\0';
        butil_log(level, "    | %s |\n", pkt_text);
    }
}

int mdns_read_uint8(ioring_t *in, uint8_t *val)
{
    if (! in || ! val)
    {
        return -1;
    }
    if (in->count > 0)
    {
        *val = in->data[in->tail];
        in->count--;
        in->tail++;
        return 0;
    }
    return 1;
}

int mdns_read_uint16(ioring_t *in, uint16_t *val)
{
    uint16_t uval;

    if (! in || ! val)
    {
        return -1;
    }
    if (in->count > 1)
    {
        uval = in->data[in->tail];
        in->count--;
        in->tail++;
        uval <<= 8;
        uval |= in->data[in->tail];
        in->count--;
        in->tail++;
        *val = uval;
        return 0;
    }
    return 1;
}

int mdns_read_uint32(ioring_t *in, uint32_t *val)
{
    uint32_t uval;

    if (! in || ! val)
    {
        return -1;
    }
    if (in->count > 3)
    {
        uval = in->data[in->tail];
        in->count--;
        in->tail++;

        uval <<= 8;
        uval |= in->data[in->tail];
        in->count--;
        in->tail++;

        uval <<= 8;
        uval |= in->data[in->tail];
        in->count--;
        in->tail++;

        uval <<= 8;
        uval |= in->data[in->tail];
        in->count--;
        in->tail++;

        *val = uval;
        return 0;
    }
    return 1;
}

int mdns_read_int8(ioring_t *in, int8_t *val)
{
    return mdns_read_uint8(in, (uint8_t*)val);
}

int mdns_read_int16(ioring_t *in, int16_t *val)
{
    return mdns_read_uint16(in, (uint16_t*)val);
}

int mdns_read_int32(ioring_t *in, int32_t *val)
{
    return mdns_read_uint32(in, (uint32_t*)val);
}

int mdns_read_text(ioring_t *in, char *text, uint16_t len)
{
    if (! in || ! text)
    {
        return -1;
    }
    if (in->count < len)
    {
        return 1;
    }
    while (len-- > 0)
    {
        *text++ = in->data[in->tail];
        in->count--;
        in->tail++;
    }
    return 0;
}

int mdns_write_uint8(ioring_t *out, uint8_t val)
{
    if (! out)
    {
        return -1;
    }
    if (out->count < out->size)
    {
        out->data[out->head] = val;
        out->count++;
        out->head++;
        return 0;
    }
    return 1;
}

int mdns_write_uint16(ioring_t *out, uint16_t val)
{
    uint16_t uval;

    if (! out)
    {
        return -1;
    }
    if (out->count < (out->size - 1))
    {
        out->data[out->head] = (uint8_t)((val >> 8) & 0xFF);
        out->count++;
        out->head++;
        out->data[out->head] = (uint8_t)(val & 0xFF);
        out->count++;
        out->head++;
        return 0;
    }
    return 1;
}

int mdns_write_uint32(ioring_t *out, uint32_t val)
{
    uint32_t uval;

    if (! out)
    {
        return -1;
    }
    if (out->count < (out->size - 3))
    {
        out->data[out->head] = (uint8_t)((val >> 24) & 0xFF);
        out->count++;
        out->head++;
        out->data[out->head] = (uint8_t)((val >> 16) & 0xFF);
        out->count++;
        out->head++;
        out->data[out->head] = (uint8_t)((val >> 8) & 0xFF);
        out->count++;
        out->head++;
        out->data[out->head] = (uint8_t)(val & 0xFF);
        out->count++;
        out->head++;
        return 0;
    }
    return 1;
}

int mdns_write_int8(ioring_t *out, int8_t val)
{
    return mdns_write_uint8(out, (uint8_t)val);
}

int mdns_write_int16(ioring_t *out, int16_t val)
{
    return mdns_write_uint16(out, (uint16_t)val);
}

int mdns_write_int32(ioring_t *out, int32_t val)
{
    return mdns_write_uint32(out, (uint32_t)val);
}

int mdns_write_bytes(ioring_t *out, const uint8_t *bytes, int len)
{
    if (! out || ! bytes)
    {
        return -1;
    }
    if (out->count > (out->size - len))
    {
        return 1;
    }
    memcpy(out->data + out->head, bytes, len);
    out->head += len;
    out->count += len;

    return 0;
}

int mdns_write_text(ioring_t *out, const char *text, int len)
{
    return mdns_write_bytes(out, (const uint8_t *)text, len);
}


