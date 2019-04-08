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
#include <sys/time.h>

int mdns_compare_names(const dns_domain_name_t *ours, const dns_domain_name_t *theirs)
{
    int label_dex;
    int cmp;
    int ll;

    ll = 8;  // log level

    if (! ours || ! theirs)
    {
        return -1;
    }
    butil_log(ll, " name-cmp  A=%s= ", mdns_str_for_domain_name(ours));
    butil_log(ll, " B=%s= ", mdns_str_for_domain_name(theirs));

    if (ours->num_labels < theirs->num_labels)
    {
        butil_log(ll, " A has fewer labels\n");
        return -1;
    }
    if (ours->num_labels > theirs->num_labels)
    {
        butil_log(ll, " B has fewer labels\n");
        return 1;
    }
    for (label_dex = 0; label_dex < ours->num_labels; label_dex++)
    {
        cmp = strcmp(ours->labels[label_dex].name, theirs->labels[label_dex].name);
        if (cmp)
        {
            butil_log(ll, "Label %d different\n", label_dex);
            return cmp;
        }
    }
    return 0;
}

int mdns_get_wall_time(uint32_t *secs, uint32_t *usecs)
{
    struct timeval tv;
    int result;

    result = gettimeofday(&tv, NULL);

    if (secs)
    {
        *secs = tv.tv_sec;
    }
    if (usecs)
    {
        *usecs = tv.tv_usec;
    }
    if (result)
    {
        butil_log(2, "gtod failed %d\n", errno);
    }
    return result;
}

uint32_t mdns_get_rnd_delay_ms(uint32_t max_delay)
{
    return (uint32_t)rand() % max_delay;
}

uint32_t mdns_get_pkt_delay_ms(mdns_packet_t *pkt)
{
    uint32_t now_secs;
    uint32_t now_usecs;
    uint32_t delta;

    mdns_get_wall_time(&now_secs, &now_usecs);

    delta = 0;
    if (pkt->tts_secs > now_secs)
    {
        delta += (pkt->tts_secs - now_secs) * 1000;
        if (pkt->tts_usecs > now_usecs)
        {
           delta += (pkt->tts_usecs - now_usecs) / 1000;
        }
        else
        {
           delta -= (now_usecs - pkt->tts_usecs) / 1000;
        }
    }
    else if (pkt->tts_secs == now_secs)
    {
        if (pkt->tts_usecs > now_usecs)
        {
            delta += (pkt->tts_usecs - now_usecs) / 1000;
        }
    }
    return delta;
}

static const char *str_for_ipv4(uint32_t ipv4addr)
{
    static char ipbuf[32];

    snprintf(ipbuf, sizeof(ipbuf), "%d:%d:%d:%d",
                (ipv4addr & 0xFF),
                ((ipv4addr >> 8) & 0xFF),
                ((ipv4addr >> 16) & 0xFF),
                ((ipv4addr >> 24) & 0xFF)
            );
    return ipbuf;
}

static const char *str_for_ipv6(bipv6addr_t *ipv6addr)
{
    static char ipbuf[64];
    uint16_t addr[8];
    uint16_t *sa;
    int i;

    sa = ipv6addr->addr;
    for (i = 0; i < 8; i++)
    {
        addr[i] = htons(sa[i]);
    }
    snprintf(ipbuf, sizeof(ipbuf), "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7]);
    return ipbuf;
}

const char *mdns_str_for_sockaddr(struct sockaddr *sockaddr)
{
    if (! sockaddr)
    {
        return "<nil>";
    }
    if (sockaddr->sa_family == AF_INET)
    {
        uint32_t ip_addr;

        ip_addr = (uint32_t)((struct sockaddr_in *)sockaddr)->sin_addr.s_addr;
        return str_for_ipv4(ip_addr);
    }
    else
    {
        bipv6addr_t ipv6addr;

        memcpy(&ipv6addr.addr, &((struct sockaddr_in6 *)sockaddr)->sin6_addr, sizeof(bipv6addr_t));
        return str_for_ipv6(&ipv6addr);
    }
}

const char *mdns_str_for_pktaddr(mdns_packet_t *pkt)
{
    if (! pkt)
    {
        return "<nil>";
    }
    if (pkt->isv6addr)
    {
        return str_for_ipv6(&pkt->srcaddr6);
    }
    else
    {
        return str_for_ipv4(pkt->srcaddr4.addr);
    }
}

const char *mdns_srv_proto_name(mdns_service_protocol_t proto)
{
    switch (proto)
    {
    case MDNS_SRVPROTO_UDP:
        return "_udp";
    case MDNS_SRVPROTO_TCP:
        return "_tcp";
    default:
        break;
    }
    BERROR("Bad service protocol");
    return "???";
}

const char *mdns_str_for_domain_name(const dns_domain_name_t *dname)
{
    static char dnbuf[MDNS_MAX_DNTEXT];
    int result;

    result = mdns_flatten_name(dname, dnbuf, sizeof(dnbuf));
    if (result)
    {
        return "<domain_name failed>";
    }
    return dnbuf;
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

    pkt->id = 0;
    pkt->flags = 0;
    pkt->qdcount = 0;
    pkt->ancount = 0;
    pkt->nscount = 0;
    pkt->arcount = 0;

    pkt->tts_secs = 0;
    pkt->tts_usecs = 0;
    pkt->unicast = false;
    pkt->isv6addr = false;

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

    butil_log(level, "Pkt %4d bytes (+%u msecs) %s\n",
                        pkt ? pkt->io.count : 0,
                        mdns_get_pkt_delay_ms(pkt), because);

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

    butil_log(level, "ID=%u  (%d bytes)\n", (uint32_t)pkt->id, pkt->io.count);
    butil_log(level, "Flags=%04X\n", pkt->flags);
    butil_log(level, "Questions=%u\n", (uint32_t)pkt->qdcount);
    butil_log(level, "Answer RRs=%u\n", (uint32_t)pkt->ancount);
    butil_log(level, "Authority RRs=%u\n", (uint32_t)pkt->nscount);
    butil_log(level, "Additional RRs=%u\n", (uint32_t)pkt->arcount);

    level++;
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
    butil_log(level, "\n");
}

mdns_interface_t *mdns_interface_alloc(mdns_responder_t *res, const char *hostname)
{
    char name[MDNS_MAX_DNTEXT];
    mdns_interface_t *iface;
    int result;

    iface = (mdns_interface_t *)malloc(sizeof(mdns_interface_t));
    if (! iface)
    {
        BERROR("Alloc iface");
        return NULL;
    }
    result = mdns_assemble_name(name, sizeof(name), 2, hostname, "local");
    if (result)
    {
        butil_log(0, "Interface: hostname too big\n");
        free(iface);
        return NULL;
    }
    result = mdns_unflatten_name(name, &iface->hostname);
    if (result)
    {
        butil_log(0, "Interface: hostname conversion failed\n");
        free(iface);
        return NULL;
    }
    return iface;
}

void mdns_interface_free(mdns_responder_t *res, mdns_interface_t *iface)
{
    if (iface)
    {
        free(iface);
    }
}

void mdns_dump_interface(mdns_interface_t *iface, int level)
{
    if (! iface)
    {
        return;
    }
    butil_log(level, "  iface host=%s\n", mdns_str_for_domain_name(&iface->hostname));
    butil_log(level, "        IPv4=%s\n", str_for_ipv4(iface->ipv4addr.addr));
    butil_log(level, "        IPv6=%s\n", str_for_ipv6(&iface->ipv6addr));
}

mdns_service_t *mdns_service_alloc(mdns_responder_t *res)
{
    mdns_service_t *service;

    service = (mdns_service_t *)malloc(sizeof(mdns_service_t));
    if (! service)
    {
        BERROR("Alloc service");
        return NULL;
    }
    return service;
}

void mdns_service_free(mdns_responder_t *res, mdns_service_t *service)
{
    if (service)
    {
        free(service);
    }
}

void mdns_dump_service(mdns_service_t *service, int level)
{
    if (! service)
    {
        return;
    }
    butil_log(level, " service name=%s\n", mdns_str_for_domain_name(&service->usr_domain_name));
    butil_log(level, "     srv name=%s\n", mdns_str_for_domain_name(&service->srv_domain_name));
    butil_log(level, "     sub name=%s\n", mdns_str_for_domain_name(&service->sub_domain_name));
    butil_log(level, "     protocol=%s\n", mdns_srv_proto_name(service->proto));
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
        uval |= ((uint16_t)in->data[in->tail] << 8);
        in->count--;
        in->tail++;
        *val = htons(uval);
        return 0;
    }
    return 1;
}

int mdns_read_uint32(ioring_t *in, uint32_t *val)
{
    uint32_t aval;
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

        aval = in->data[in->tail];
        uval |= aval << 8;
        in->count--;
        in->tail++;

        aval = in->data[in->tail];
        uval |= aval << 16;
        in->count--;
        in->tail++;

        aval = in->data[in->tail];
        uval |= aval << 24;
        in->count--;
        in->tail++;

        *val = htonl(uval);
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

int mdns_flatten_name(const dns_domain_name_t *dname, char *name, int nname)
{
    int len;
    int off;
    int count;

    if (! dname || ! name || ! nname)
    {
        return -1;
    }
    if (dname->num_labels < 1)
    {
        name[0] = '\0';
        return 0;
    }
    for (count = 0, off = 0; count < dname->num_labels; count++)
    {
        len = snprintf(name + off, nname - off, "%s%s",
                    (count > 0) ? "." : "", dname->labels[count].name);
        if (len < 0)
        {
            BERROR("buffer overflow");
            return -1;
        }
        off += len;
    }
    butil_log(8, "FN=%s=\n", name);
    return 0;
}

static int mdns_insert_label(dns_domain_name_t *dname, const char *src, int count)
{
    int label_dex;

    if (count <= 0)
    {
        return 0;
    }
    if (count >= MDNS_MAX_LABEL)
    {
        butil_log(1, "Label of %d is > max %d\n", count, MDNS_MAX_LABEL - 1);
        return -1;
    }
    if ((count + dname->tot_len + dname->num_labels) >= (MDNS_MAX_DNTEXT - 1))
    {
        butil_log(1, "No room to add label of %d\n", count);
        return -1;
    }
    label_dex = dname->num_labels - 1;

    if (label_dex < 0 || label_dex >= (MDNS_MAX_LABELS - 1))
    {
        butil_log(1, "Too many labels\n");
        return -1;
    }
    dname->labels[label_dex].length = count;
    memcpy(dname->labels[label_dex].name, src, count);
    dname->labels[label_dex].name[count] = '\0';
    dname->tot_len += count;
    return 0;
}

int mdns_unflatten_name(const char *name, dns_domain_name_t *dname)
{
    int label_off;
    int off;
    char c;
    int result;

    if (! name || ! dname)
    {
        return -1;
    }
    memset(dname, 0, sizeof(dns_domain_name_t));

    dname->tot_len = 0;
    dname->num_labels = 0;

    off = 0;
    result = 0;

    do
    {
        c = name[off];
        if (c == '\0')
        {
            break;
        }
        if (dname->tot_len >= (MDNS_MAX_DNTEXT - 1))
        {
            butil_log(1, "Name overflow\n");
            result = -1;
            break;
        }
        if (dname->num_labels >= (MDNS_MAX_LABELS - 1))
        {
            butil_log(1, "Label count overflow\n");
            result = -1;
            break;
        }
        label_off = off++;
        dname->num_labels++;

        while (name[off] && name[off] != '.')
        {
            off++;
        }
        result = mdns_insert_label(dname, name + label_off, off - label_off);

        if (name[off] == '.')
        {
            off++;
        }
    }
    while (! result && c != '\0');

    return result;
}

int mdns_unflatten_txt(const char *name, dns_txt_records_t *txtrec)
{
    int label_dex;
    int off;
    int insoff;
    int added;
    char c;
    int result;

    if (! name || ! txtrec)
    {
        return -1;
    }
    memset(txtrec, 0, sizeof(dns_txt_records_t));

    txtrec->tot_len = 0;
    txtrec->num_labels = 0;

    off = 0;
    result = 0;

    do
    {
        c = name[off];
        if (c == '\0')
        {
            break;
        }
        if (txtrec->tot_len >= (MDNS_MAX_TRTEXT - 1))
        {
            butil_log(1, "Name overflow\n");
            result = -1;
            break;
        }
        if (txtrec->num_labels >= (MDNS_MAX_LABELS - 1))
        {
            butil_log(1, "Label count overflow\n");
            result = -1;
            break;
        }
        label_dex = txtrec->num_labels++;
        insoff = off;
        added  = 0;

        while (name[off] && name[off] != ',')
        {
            if (added >= (MDNS_MAX_TRTEXT - 2))
            {
                butil_log(1, "Label size overflow\n");
                result = -1;
                break;
            }
            txtrec->labels[label_dex].name[added++] = name[off];
            off++;
            if (name[off] == ',')
            {
                if (name[off - 1] == '\\')
                {
                    txtrec->labels[label_dex].name[added - 1] = name[off];
                    off++;
                }
            }
        }
        txtrec->labels[label_dex].length = added;
        txtrec->labels[label_dex].name[added] = '\0';
        txtrec->tot_len += added;

        if (name[off] == ',')
        {
            off++;
        }
    }
    while (! result && c != '\0');
}

int mdns_read_name(ioring_t *in, dns_domain_name_t *dname)
{
    uint16_t label_tail[MDNS_MAX_LABELS];
    uint16_t label_offset;
    int label_top;
    int count;
    int result;
    uint8_t c;

    if (! in || ! dname)
    {
        return -1;
    }
    memset(dname, 0, sizeof(dns_domain_name_t));

    label_top = 0;

    dname->tot_len = 0;
    dname->num_labels = 0;

    do
    {
        // read count byte
        //
        result = mdns_read_uint8(in, &c);
        if (result)
        {
            return result;
        }
        if (c != 0)
        {
            // Non-0 byte
            //
            if (dname->tot_len >= (MDNS_MAX_DNTEXT - 1))
            {
                butil_log(1, "Name overflow\n");
                result = -1;
                break;
            }
            if (dname->num_labels >= (MDNS_MAX_LABELS - 1))
            {
                butil_log(1, "Label count overflow\n");
                result = -1;
                break;
            }
        }
        else
        {
            // 0-byte
            //
            if (label_top > 0)
            {
                // got to end of indirect name, pop back to
                // previous tail position and continue parsing
                //
                label_top--;
                in->tail = label_tail[label_top];
                in->count = in->head - in->tail;
                continue;
            }
            else
            {
                // all done, end parsing
                //
                break;
            }
        }
        if ((c & 0xC0) == 0xC0)
        {
            // label is compressed, get offset in packet
            //
            if (label_top >= MDNS_MAX_LABELS)
            {
                butil_log(2, "Max compression indirection\n");
                result = -1;
                break;
            }
            label_offset = (c & 0x3F) << 8;

            result = mdns_read_uint8(in, &c);
            if (result)
            {
                break;
            }
            label_offset |= c;

            butil_log(7, "Offset %u for label\n", label_offset);

            if (label_offset >= (in->tail - 1) || label_offset < MDNS_OFF_QUESTION)
            {
                butil_log(2, "Offset %d out of bounds\n", label_offset);
                result = -1;
                break;
            }
            // push position of input buffer here to restore later
            //
            label_tail[label_top++] = in->tail;

            // and set input pointed at offset
            //
            in->tail = label_offset;
            in->count = in->head - in->tail;
        }
        else
        {
            dname->num_labels++;

            // normal text, validate count
            //
            count = (int)(unsigned)c;
            if (count > in->count)
            {
                butil_log(2, "name count %d is > %d remaining (truncated?)\n",
                                    count, in->count);
                result = -1;
                break;
            }
            // copy label text into name
            //
            result = mdns_insert_label(dname, in->data + in->tail, count);

            in->tail += count;
            if (in->tail > in->size)
            {
                in->tail = 0;
            }
            in->count -= count;
        }
    }
    while (c != 0 && result == 0);

    return result;
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
    uval = htons(val);

    if (out->count < (out->size - 1))
    {
        out->data[out->head] = (uint8_t)(uval & 0xFF);
        out->count++;
        out->head++;
        out->data[out->head] = (uint8_t)((uval >> 8) & 0xFF);
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
    uval = htonl(val);

    if (out->count < (out->size - 3))
    {
        out->data[out->head] = (uint8_t)(uval & 0xFF);
        out->count++;
        out->head++;
        out->data[out->head] = (uint8_t)((uval >> 8) & 0xFF);
        out->count++;
        out->head++;
        out->data[out->head] = (uint8_t)((uval >> 16) & 0xFF);
        out->count++;
        out->head++;
        out->data[out->head] = (uint8_t)((uval >> 24) & 0xFF);
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

int mdns_write_text(ioring_t *out, const dns_txt_records_t *txtrec)
{
    int totlen;
    int len;
    int label_dex;

    if (! out || ! txtrec)
    {
        return -1;
    }
    totlen = txtrec->tot_len + txtrec->num_labels;

    if (totlen > MDNS_MAX_TRTEXT)
    {
        butil_log(1, "TXT too long\n");
        return -1;
    }
    if ((out->count + totlen) > out->size)
    {
        return 1;
    }
    for (label_dex = 0; label_dex < txtrec->num_labels; label_dex++)
    {
        len = txtrec->labels[label_dex].length;

        out->data[out->head++] = (uint8_t)len;
        out->count++;

        memcpy(out->data + out->head, txtrec->labels[label_dex].name, len);

        out->head += len;
        out->count += len;
    }
    return 0;
}

int mdns_write_name(ioring_t *out, const dns_domain_name_t *dname)
{
    int totlen;
    int label_dex;
    int len;
    int result;

    if (! out || ! dname)
    {
        return -1;
    }
    totlen = dname->tot_len + dname->num_labels + 1;

    if (totlen > MDNS_MAX_DNTEXT)
    {
        return -1;
    }
    if ((out->count + totlen) > out->size)
    {
        return 1;
    }
    for (label_dex = 0; label_dex < dname->num_labels; label_dex++)
    {
        len = dname->labels[label_dex].length;

        out->data[out->head++] = (uint8_t)len;
        out->count++;

        memcpy(out->data + out->head, dname->labels[label_dex].name, len);

        out->head += len;
        out->count += len;
    }
    out->data[out->head++] = (uint8_t)0;
    out->count++;

    return 0;
}

int mdns_assemble_name(uint8_t *buffer, int nbuffer, int ncomponents, ...)
{
    const char *labels[MDNS_MAX_LABELS];
    int lablens[MDNS_MAX_LABELS];
    int laboffs[MDNS_MAX_LABELS];
    va_list args;
    int offset;
    int len;
    int i;
    int result;

    if (ncomponents >= MDNS_MAX_LABELS)
    {
        butil_log(0, "Too many labels to assemble\n");
        return -1;
    }
    va_start(args, ncomponents);

    offset = 0;
    result = 0;

    for (i = 0; i < ncomponents; i++)
    {
        labels[i] = (char *)va_arg(args, char *);
        lablens[i] = strlen(labels[i]);
        laboffs[i] = offset;
        offset += lablens[i];
    }
    va_end(args);

    offset  = 0;

    for (i = 0; i < ncomponents; i++)
    {
        len = snprintf(buffer + offset, nbuffer - offset, "%s%s",
                    labels[i],
                     (i < (ncomponents - 1)) ? "." : ""
                    );
        if (len < 0 || (offset + len) >= MDNS_MAX_DNTEXT)
        {
            butil_log(1, "Name building overflow\n");
            result = -1;
            break;
        }
        offset += len;
    }
    return result;
}

