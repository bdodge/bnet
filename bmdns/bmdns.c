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
#include "bmdns.h"
#include "bmdnsutils.h"
// include this "private" api header to keep it up to date
#include "bmdnsunits.h"

// TODO
//    multi-packet known-answer-suppression (maybe)
//    add remove service/iface functions (maybe, easier to just bring it all down)

int mdns_handle_output(mdns_responder_t *res, mdns_interface_t *iface)
{
    struct sockaddr *dstaddr;
    struct sockaddr_in dst4addr;
    struct sockaddr_in6 dst6addr;
    mdns_packet_t *pkt;
    uint32_t delay;
    int dstsize;
    int count;
    int result;

    if (! res || ! iface)
    {
        return -1;
    }
    while (iface->outpkts)
    {
        pkt = iface->outpkts;

        if (! res || iface->udp_sock == INVALID_SOCKET)
        {
            BERROR("bad context");
            return -1;
        }
        delay = mdns_get_pkt_delay_ms(pkt);

        butil_log(8, "delay +%ums\n", delay);
        if (delay > 0)
        {
            // no reason to loop, nothing to send
            return 0;
        }
        result = iostream_posix_poll_filedesc(iface->udp_sock, writeable, 0, 100000);
        if (result < 0)
        {
            BERROR("network error");
            res->fatal = true;
            return result;
        }
        if (result == 0)
        {
            // not writeable keep packet
            return 0;
        }
        // dequeue packet
        //
        iface->outpkts = pkt->next;

        if (pkt->unicast || (pkt->srcport != MDNS_PORT))
        {
            if (pkt->isv6addr)
            {
                dst6addr.sin6_family = AF_INET6;
                dst6addr.sin6_port = htons(pkt->srcport);
                memcpy(&dst6addr.sin6_addr, pkt->srcaddr6.addr, sizeof(bipv6addr_t));
                dstaddr = (struct sockaddr *)&dst6addr;
                dstsize = sizeof(dst6addr);
            }
            else
            {
                dst4addr.sin_family = AF_INET;
                dst4addr.sin_port = htons(pkt->srcport);
                dst4addr.sin_addr.s_addr = pkt->srcaddr4.addr;
                dstaddr = (struct sockaddr *)&dst4addr;
                dstsize = sizeof(dst4addr);
            }
        }
        else
        {
            if (pkt->isv6addr)
            {
                dst6addr.sin6_family = AF_INET6;
                dst6addr.sin6_port = htons(MDNS_PORT);
                inet_pton(AF_INET6, MDNS_MCAST_IP4ADDR, &dst6addr.sin6_addr);
                dstaddr = (struct sockaddr *)&dst6addr;
                dstsize = sizeof(dst6addr);
            }
            else
            {
                dst4addr.sin_family = AF_INET;
                dst4addr.sin_port = htons(MDNS_PORT);
                dst4addr.sin_addr.s_addr = inet_addr(MDNS_MCAST_IP4ADDR);
                dstaddr = (struct sockaddr *)&dst4addr;
                dstsize = sizeof(dst4addr);
            }
        }
        count = sendto(iface->udp_sock, pkt->io.data, pkt->io.count,
                                 0, (struct sockaddr *)dstaddr,  dstsize);

        mdns_pkt_free(res, pkt);
        if (pkt->unicast)
        {
            butil_log(6, "TX %d to %s %u  res %d (%d)\n", pkt->io.count,
                            mdns_str_for_pktaddr(pkt), (uint32_t)pkt->srcport, count, errno);
        }
        else
        {
            butil_log(6, "TX %d multicast res %d (%d)\n", pkt->io.count, count, errno);
        }
        if (count < 0)
        {
            BERROR("cant send\n");
            return count;
        }
        if (count != pkt->io.count)
        {
            butil_log(1, "Partial write of pkt\n");
            return -1;
        }
    }
    return 0;
}

static int mdns_enqueue_out(mdns_responder_t *res, mdns_interface_t *iface, mdns_packet_t *outpkt, uint32_t delay_ms)
{
    uint32_t now_secs;
    uint32_t now_usecs;
    int result;

    if (! iface || ! outpkt)
    {
        return -1;
    }
    if (outpkt->io.count == 0)
    {
        // didn't use outpack
        //
        mdns_pkt_free(res, outpkt);
    }
    else
    {
        if (delay_ms > 0)
        {
            result = mdns_get_wall_time(&now_secs, &now_usecs);

            now_usecs += delay_ms * 1000;
            while (now_usecs > 1000000) // faster than calling divide/mod
            {
                now_secs++;
                now_usecs -= 1000000;
            }
            outpkt->tts_secs = now_secs;
            outpkt->tts_usecs = now_usecs;
        }
        else
        {
            outpkt->tts_secs = 0;
            outpkt->tts_usecs = 0;
        }
        butil_log(7, "Packet delay is %u\n", mdns_get_pkt_delay_ms(outpkt));

        // queue output packet for sending, doing insertion sort
        // based on when it gets sent
        //
        mdns_dump_packet(">>>>>> TX", outpkt, 6);

        outpkt->next = NULL;

        if (! iface->outpkts)
        {
            iface->outpkts = outpkt;
        }
        else
        {
            mdns_packet_t *pend;
            mdns_packet_t *prevpend;
            bool inserted;

            for (prevpend = NULL, pend = iface->outpkts, inserted = false; pend->next; pend = pend->next)
            {
                if (
                        outpkt->tts_secs < pend->tts_secs
                    ||  (outpkt->tts_secs == pend->tts_secs && outpkt->tts_usecs < pend->tts_usecs)
                )
                {
                    if (! prevpend)
                    {
                        outpkt->next = iface->outpkts;
                        iface->outpkts = outpkt;
                    }
                    else
                    {
                        outpkt->next = pend;
                        prevpend->next = outpkt;
                    }
                    inserted = true;
                    break;
                }
                prevpend = pend;
            }
            if (! inserted)
            {
                pend->next = outpkt;
            }
        }
    }
    return 0;
}

int mdns_check_input(mdns_responder_t *res, mdns_interface_t *iface)
{
    mdns_packet_t *pkt;
    struct sockaddr srcaddr;
    socklen_t srclen;
    int count;
    int result;

    if (! res || ! iface || iface->udp_sock == INVALID_SOCKET)
    {
        BERROR("bad context");
        return -1;
    }
    result = iostream_posix_poll_filedesc(iface->udp_sock, readable, res->to_secs, res->to_usecs);
    if (result < 0)
    {
        BERROR("socket closed?");
        res->fatal = true;
        return result;
    }
    if (result == 0)
    {
        return 0;
    }
    // grab an input packet to place data into
    //
    pkt = mdns_pkt_alloc(res);
    if (! pkt)
    {
        // todo - complain about this eventually
        return 0;
    }
    srclen = sizeof(srcaddr);
    count = recvfrom(iface->udp_sock, pkt->io.data, pkt->io.size,
                         0, &srcaddr, &srclen);

    if (count)
    {
        if (srcaddr.sa_family == AF_INET6)
        {
            pkt->isv6addr = true;
            memcpy(&pkt->srcaddr6.addr, &((struct sockaddr_in6 *)&srcaddr)->sin6_addr, sizeof(bipv6addr_t));
            pkt->srcport = htons(((struct sockaddr_in6 *)&srcaddr)->sin6_port);
        }
        else
        {
            pkt->isv6addr = false;
            pkt->srcaddr4.addr = ((struct sockaddr_in *)&srcaddr)->sin_addr.s_addr;
            pkt->srcport = htons(((struct sockaddr_in *)&srcaddr)->sin_port);
        }
        butil_log(6, "\nRX %d from %s %u\n", count, mdns_str_for_pktaddr(pkt), (uint32_t)pkt->srcport);

        pkt->io.count += count;
        pkt->io.head += count;
        if (pkt->io.head >= pkt->io.size)
        {
            pkt->io.head = 0;
        }
        // append packet to responder's in list
        //
        if (! iface->inpkts)
        {
            iface->inpkts = pkt;
        }
        else
        {
            mdns_packet_t *pp;

            for (pp = iface->inpkts; pp->next;)
            {
                pp = pp->next;
            }
            pp->next = pkt;
        }
        pkt->next = NULL;
    }
    else
    {
        butil_log(1, "RX 0 after OK poll?\n");
        mdns_pkt_free(res, pkt);
        return -1;
    }
    return 0;
}

static int mdns_rewrite_header(ioring_t *io, mdns_packet_t *outpkt)
{
    int cur_head;
    int cur_count;
    int result;

    result = 0;

    if (outpkt->io.count == 0)
    {
        // first time in, pretend we already have a header
        //
        cur_head = MDNS_OFF_QUESTION;
        cur_count = MDNS_OFF_QUESTION;
    }
    else
    {
        cur_head = io->head;
        cur_count = io->count;
    }
    io->head = 0;
    io->count = 0;

    result |= mdns_write_uint16(io, outpkt->id);
    result |= mdns_write_uint16(io, outpkt->flags);
    result |= mdns_write_uint16(io, outpkt->qdcount);
    result |= mdns_write_uint16(io, outpkt->ancount);
    result |= mdns_write_uint16(io, outpkt->nscount);
    result |= mdns_write_uint16(io, outpkt->arcount);

    io->head = cur_head;
    io->count = cur_count;

    return result;
}

int mdns_rr_index(uint16_t type)
{
    switch (type)
    {
    case DNS_RRTYPE_A:
        return 0;
    case DNS_RRTYPE_AAAA:
        return 1;
    case DNS_RRTYPE_PTR:
        return 2;
    case DNS_RRTYPE_SRV:
        return 3;
    case DNS_RRTYPE_TXT:
        return 4;
    default:
        break;
    }
    return 0;
}

int mdns_ask_question(
                    mdns_interface_t  *iface,
                    dns_domain_name_t *fqdn,
                    uint16_t type,
                    uint16_t clas,
                    mdns_packet_t *outpkt
                   )
{
    mdns_service_t *service;
    int cmp;
    int lma;
    int result;

    // only care about classes/types we care about
    //
    if (clas != DNS_CLASS_IN && clas != DNS_CLASS_ANY)
    {
        return false;
    }
    switch (type)
    {
    case DNS_RRTYPE_A:
    case DNS_RRTYPE_PTR:
    case DNS_RRTYPE_SRV:
    case DNS_RRTYPE_TXT:
    case DNS_RRTYPE_AAAA:
    case DNS_RRTYPE_ANY:
        break;
    default:
        return false;
    }
    lma = 7; // log level for debug

    outpkt->qdcount++;

    // (re) write packet header with updated info
    result = mdns_rewrite_header(&outpkt->io, outpkt);
    if (result)
    {
        return result;
    }
    // write name
    result = mdns_write_name(&outpkt->io, fqdn, outpkt->io.head);
    if (result)
    {
        return result;
    }
    // write type and class
    result  = mdns_write_uint16(&outpkt->io, type);
    result |= mdns_write_uint16(&outpkt->io, clas);
    return result;
}

int mdns_compare_resource_to_name(mdns_responder_t *res, uint8_t *answer, int anslen, dns_domain_name_t *dname)
{
    dns_domain_name_t res_name;
    ioring_t res_in;
    int reslen;
    int result;

    if (! answer || ! dname)
    {
        return -1;
    }
    reslen = DNS_NAME_LENGTH(dname);
    if (reslen != anslen)
    {
        return -1;
    }
    res_in.tail = 0;
    res_in.count = anslen;
    res_in.head  = anslen;
    res_in.size  = anslen;
    res_in.data  = answer;

    mdns_init_name(&res_name);

    // read the resource name in to a dns name. the
    // name can NOT have any compression or this doesn't
    // work, so make sure any answer text is flattened
    //
    result = mdns_read_name(res, &res_in, &res_name);
    if (result)
    {
        return -1;
    }
    // compare names label by label
    //
    result = mdns_compare_names(dname, &res_name);

    mdns_clear_name(res, &res_name);
    return result;
}

int mdns_query_respond(
                            mdns_responder_t  *res,
                            mdns_interface_t  *iface,
                            dns_domain_name_t *record_domain,
                            dns_rr_rec_t      *answers,
                            int                answer_count,
                            mdns_packet_t     *outpkt,
                            uint16_t           type,
                            uint16_t           clas,
                            uint32_t           ttl,
                            void              *resource
                         )
{
    dns_domain_name_t *resource_domain;
    dns_txt_records_t *resource_txt;
    int reslen;
    int newlen;
    int result;
    int adex;
    int cmp;
    int prevhead;
    int pkt_start;
    bool matched;
    time_t now;
    time_t answer_expires;
    time_t record_expires;
    int kas;

    kas = 5; // log level for known answer suppression debug

    time(&now);

    if (type == DNS_RRTYPE_TXT)
    {
        resource_txt = (dns_txt_records_t *)resource;
        resource_domain = NULL;
    }
    else
    {
        resource_domain = (dns_domain_name_t *)resource;
        resource_txt = NULL;
    }
    // check that there is no known answer for this question
    //
    for (adex = 0, matched = false; adex < answer_count && ! matched; adex++)
    {
        cmp = mdns_compare_names(&answers[adex].dname, record_domain);
        if (cmp)
        {
            continue;
        }
        if (
                answers[adex].type != type
            ||  answers[adex].clas != clas
        )
        {
            continue;
        }
        answer_expires = answers[adex].ttl;
        record_expires = ttl + (now - record_domain->last_sent[mdns_rr_index(type)]);
        if (answer_expires > record_expires / 2)
        {
            butil_log(kas, "Not surpressing known %d because rec exp %d before ans %d\n",
                                                adex, record_expires, answer_expires);
            break;
        }
        // check the response resource and see if that matches
        //
        switch (type)
        {
        case DNS_RRTYPE_PTR:
            matched = !mdns_compare_resource_to_name(res,
                            answers[adex].res, answers[adex].reslen, resource_domain);
            break;
        case DNS_RRTYPE_TXT:
            reslen = DNS_TXT_LENGTH(resource_txt);
            if (reslen == answers[adex].reslen)
            {
                ioring_t res_out;
                uint8_t nametext[MDNS_MAX_TRTEXT];

                // write our txtrecs out and compare to resource bytes
                //
                res_out.head = 0;
                res_out.tail = 0;
                res_out.count = 0;
                res_out.size  = sizeof(nametext);
                res_out.data  = nametext;

                result = mdns_write_text(&res_out, resource_txt);
                if (result)
                {
                    result = 0;
                    break;
                }
                // compare service->txt_records to resource answer[adex].res
                if (! memcmp(answers[adex].res, nametext, reslen))
                {
                    matched = true;
                }
            }
            break;
        case DNS_RRTYPE_SRV:
            // TODO compare weight/prority/port (but really, if name and service name match
            // we really want to match here anyway)
            matched = !mdns_compare_resource_to_name(res, answers[adex].res + 6, answers[adex].reslen - 6, resource_domain);
            break;
        case DNS_RRTYPE_A:
            if (answers[adex].reslen == 4)
            {
                uint32_t addr = htonl(iface->ipv4addr.addr);

                if(! memcmp(answers[adex].res, (char*)&addr, 4))
                {
                    matched = true;
                }
            }
            break;
        case DNS_RRTYPE_AAAA:
            if (answers[adex].reslen == 16)
            {
                ioring_t res_in;
                uint16_t addr;

                res_in.tail = 0;
                res_in.count = 16;
                res_in.head = 16;
                res_in.size = 16;
                res_in.data = answers[adex].res;

                for (reslen = 0, matched = true; matched && reslen < 8; reslen++)
                {
                    result = mdns_read_uint16(&res_in, &addr);

                    if (result != htons(iface->ipv6addr.addr[reslen]))
                    {
                        matched = false;
                    }
                }
            }
            break;
        }
    }
    if (matched)
    {
        butil_log(5, "Suppressed known answer to %s type %d\n",
                            mdns_str_for_domain_name(record_domain), type);
        return 0;
    }
    // time stamp the RR with now so TTL calculations can happen. There is one
    // stamp per RR type we handle so they are logically unique
    //
    time(&record_domain->last_sent[mdns_rr_index(type)]);

    // caller should have incremented whichever count
    // this response goes in before calling here
    //
    // (re) write packet header with updated info
    result = mdns_rewrite_header(&outpkt->io, outpkt);
    if (result)
    {
        return result;
    }
    // remember beginning of compression fodder
    //
    pkt_start = MDNS_OFF_QUESTION; //outpkt->io.head;

    // write name
    result = mdns_write_name(&outpkt->io, record_domain, pkt_start);
    if (result)
    {
        return result;
    }
    // write type and class
    result |= mdns_write_uint16(&outpkt->io, type);

    if (iface->state == MDNS_ANNOUNCE_1 || iface->state == MDNS_ANNOUNCE_2)
    {
        clas |= (1<<15); // cache flush
    }
    result |= mdns_write_uint16(&outpkt->io, clas);
    if (result)
    {
        return result;
    }
    // ttl
    result = mdns_write_uint32(&outpkt->io, ttl);
    if (result)
    {
        return result;
    }
    switch (type)
    {
    case DNS_RRTYPE_PTR:
        reslen = DNS_NAME_LENGTH(resource_domain);
        prevhead = outpkt->io.head;
        result  = mdns_write_uint16(&outpkt->io, reslen);
        result |= mdns_write_name(&outpkt->io, resource_domain, pkt_start);
        // back annotate len in case of name compression
        newlen = outpkt->io.head - prevhead - 2;
        if (newlen != reslen)
        {
            int lasthead = outpkt->io.head;

            outpkt->io.head = prevhead;
            result |= mdns_write_uint16(&outpkt->io, newlen);
            outpkt->io.head = lasthead;
            outpkt->io.count = outpkt->io.head - outpkt->io.tail;
        }
        break;
    case DNS_RRTYPE_TXT:
        reslen = DNS_TXT_LENGTH(resource_txt);
        result  = mdns_write_uint16(&outpkt->io, reslen);
        result |= mdns_write_text(&outpkt->io, resource_txt);
        break;
    case DNS_RRTYPE_SRV:
        reslen = DNS_NAME_LENGTH(resource_domain) + 6;
        prevhead = outpkt->io.head;
        result  = mdns_write_uint16(&outpkt->io, reslen);
        // write priority
        result |= mdns_write_uint16(&outpkt->io, 55);
        if (result)
        {
            return result;
        }
        // write weight
        result |= mdns_write_uint16(&outpkt->io, 555);
        if (result)
        {
            return result;
        }
        {
        mdns_service_t *service;
        uint16_t port = 0;

        // TODO - maybe pass service or port in as a param?
        for (service = iface->services; service; service = service->next)
        {
            if (! mdns_compare_names(&service->usr_domain_name, record_domain))
            {
                port = service->port;
                break;
            }
        }
        // write port
        result |= mdns_write_uint16(&outpkt->io, port);
        if (result)
        {
            return result;
        }
        }
        // write target
        result |= mdns_write_name(&outpkt->io, resource_domain, pkt_start);
        // back annotate len in case of name compression
        newlen = outpkt->io.head - prevhead - 2;
        if (newlen != reslen)
        {
            int lasthead = outpkt->io.head;

            outpkt->io.head = prevhead;
            result |= mdns_write_uint16(&outpkt->io, newlen);
            outpkt->io.head = lasthead;
        }
        break;
    case DNS_RRTYPE_A:
        result = mdns_write_uint16(&outpkt->io, 4);
        result |= mdns_write_uint32(&outpkt->io, htonl(iface->ipv4addr.addr));
        break;
    case DNS_RRTYPE_AAAA:
        result = mdns_write_uint16(&outpkt->io, 16);
        for (reslen = 0; reslen < 8; reslen++)
        {
            result |= mdns_write_uint16(&outpkt->io, htons(iface->ipv6addr.addr[reslen]));
        }
        break;
    case DNS_RRTYPE_ANY:
    default:
        butil_log(4, "  RR type %d is ignored in PTR\n", type);
        result = 0;
    }
    return result;
}

int mdns_answer_question(
                        mdns_responder_t *res,
                        mdns_interface_t  *iface,
                        dns_domain_name_t *dname,
                        uint16_t type,
                        uint16_t clas,
                        dns_rr_rec_t *answers,
                        int answer_count,
                        mdns_packet_t *outpkt
                     )
{
    mdns_service_t *service;
    mdns_service_t *added_srv;
    dns_domain_name_t *record_domain;
    void *resource_domain;
    int cmp;
    int lma;
    int result;

    // only care about classes/types we care about
    //
    if (clas != DNS_CLASS_IN && clas != DNS_CLASS_ANY)
    {
        return false;
    }
    switch (type)
    {
    case DNS_RRTYPE_A:
    case DNS_RRTYPE_PTR:
    case DNS_RRTYPE_SRV:
    case DNS_RRTYPE_TXT:
    case DNS_RRTYPE_AAAA:
    case DNS_RRTYPE_ANY:
        break;
    default:
        return false;
    }
    lma = 6; // log level for match debug

    result = 0;

    if (type == DNS_RRTYPE_ANY)
    {
        // just call the individual types by hand
        //
        result  = mdns_answer_question(res, iface, dname, DNS_RRTYPE_PTR, clas, answers, answer_count, outpkt);
        result |= mdns_answer_question(res, iface, dname, DNS_RRTYPE_TXT, clas, answers, answer_count, outpkt);
        result |= mdns_answer_question(res, iface, dname, DNS_RRTYPE_SRV, clas, answers, answer_count, outpkt);
        result |= mdns_answer_question(res, iface, dname, DNS_RRTYPE_A, clas, answers, answer_count, outpkt);
        result |= mdns_answer_question(res, iface, dname, DNS_RRTYPE_AAAA, clas, answers, answer_count, outpkt);
        return result;
    }
    // match against all the names we care about (host, ip, services)
    //
    cmp = mdns_compare_names(&iface->hostname, dname);
    if (! cmp)
    {
        butil_log(lma, "**==Matched iface %s\n", mdns_str_for_domain_name(dname));
        if (iface->state < MDNS_ANNOUNCE_1)
        {
            outpkt->nscount++;
        }
        else
        {
            outpkt->ancount++;
        }
        // TODO what is the proper resource domain answer for a hostname qr?
        result = mdns_query_respond(res, iface, &iface->hostname,
                                        answers, answer_count, outpkt, type, clas, iface->ttl, dname);
    }
    cmp = mdns_compare_names(&iface->rev_ipv4, dname);
    if (! cmp)
    {
        butil_log(lma, "**==Matched revipv4 %s\n", mdns_str_for_domain_name(dname));
        if (iface->state < MDNS_ANNOUNCE_1)
        {
            outpkt->nscount++;
        }
        else
        {
            outpkt->ancount++;
        }
        result = mdns_query_respond(res, iface, &iface->rev_ipv4,
                                        answers, answer_count, outpkt, type, clas, iface->ttl, &iface->hostname);
    }

    cmp = mdns_compare_names(&iface->rev_ipv6, dname);
    if (! cmp)
    {
        butil_log(lma, "**==Matched revipv6 %s\n", mdns_str_for_domain_name(dname));
        if (iface->state < MDNS_ANNOUNCE_1)
        {
            outpkt->nscount++;
        }
        else
        {
            outpkt->ancount++;
        }
        result = mdns_query_respond(res, iface, &iface->rev_ipv6,
                                        answers, answer_count, outpkt, type, clas, iface->ttl, &iface->hostname);
    }

    added_srv = NULL;

    for (service = iface->services; service; service = service->next)
    {
        cmp = mdns_compare_names(&service->usr_domain_name, dname);
        if (! cmp)
        {
            butil_log(lma, "**I==Matched usr service\n");
            record_domain = &service->usr_domain_name;
            switch (type)
            {
            case DNS_RRTYPE_A:
            case DNS_RRTYPE_AAAA:
            case DNS_RRTYPE_SRV:
            case DNS_RRTYPE_PTR:
            default:
                resource_domain = &iface->hostname;
                break;
            case DNS_RRTYPE_TXT:
                resource_domain = &service->txt_records;
                break;
            }
        }
        else
        {
            cmp = mdns_compare_names(&service->srv_domain_name, dname);
            if (! cmp)
            {
                butil_log(lma, "**V==Matched srv service\n");
                record_domain = &service->srv_domain_name;
                resource_domain = &service->usr_domain_name;
            }
            else
            {
                cmp = mdns_compare_names(&service->sub_domain_name, dname);
                if (! cmp)
                {
                    butil_log(lma, "**S==Matched sub service\n");
                    record_domain = &service->sub_domain_name;
                    resource_domain = &service->usr_domain_name;
                }
                else
                {
                    cmp = mdns_compare_names(&service->sd_domain_name, dname);
                    if (! cmp)
                    {
                        butil_log(lma, "**D==Matched dns-sd record\n");
                        record_domain = &service->sd_domain_name;
                        resource_domain = &service->srv_domain_name;
                    }
                }
            }
        }
        if (! cmp)
        {
            int prevcount;

            if (iface->state < MDNS_ANNOUNCE_1)
            {
                outpkt->nscount++;
            }
            else
            {
                outpkt->ancount++;
            }
            prevcount = outpkt->io.count;

            result = mdns_query_respond(res, iface, record_domain,
                                answers, answer_count, outpkt, type, clas, service->ttl, resource_domain);

            if (! result && type == DNS_RRTYPE_SRV && iface->state >= MDNS_ANNOUNCE_1 && outpkt->io.count > prevcount)
            {
                // for SRV records while running, add the address records for the hostname, but only if
                // the records wasn't suppressed (by a known answer) which is indicated by outpkt growing or not
                //
                added_srv = service;
            }
        }
    }
    if (added_srv)
    {
        // add address record if added a srv record to match service instance, but in additional section.
        // there could only technically be one matched service on a single interface
        //
        if (added_srv->interface->ipv4addr.addr != 0)
        {
            outpkt->arcount++;
            result = mdns_query_respond(res, iface, &added_srv->interface->hostname,
                                answers, answer_count, outpkt, DNS_RRTYPE_A, clas, iface->ttl, &iface->hostname);
        }
        if (added_srv->interface->ipv6addr.addr[0] != 0)
        {
            outpkt->arcount++;
            result = mdns_query_respond(res, iface, &added_srv->interface->hostname,
                                answers, answer_count, outpkt, DNS_RRTYPE_AAAA, clas, iface->ttl, &iface->hostname);
        }
        // and add NSEC records for the addresses we do NOT have to ensure we claim them as ours
        //
        // TODO!!
    }
    return result;
}

bool mdns_question_relates(
                            mdns_interface_t *iface,
                            dns_domain_name_t *dname,
                            uint16_t type,
                            uint16_t clas
                         )
{
    mdns_service_t *service;
    int cmp;
    int lma;

    // only care about classes/types we care about
    //
    if (clas != DNS_CLASS_IN && clas != DNS_CLASS_ANY)
    {
        return false;
    }
    switch (type)
    {
    case DNS_RRTYPE_A:
    case DNS_RRTYPE_PTR:
    case DNS_RRTYPE_SRV:
    case DNS_RRTYPE_TXT:
    case DNS_RRTYPE_AAAA:
    case DNS_RRTYPE_ANY:
        break;
    default:
        return false;
    }
    lma = 7; // log level for match debug

    // match against all the names we care about (host, ip, services)
    //
    if (
            type == DNS_RRTYPE_PTR
        ||  type == DNS_RRTYPE_A
        ||  type == DNS_RRTYPE_AAAA
        ||  type == DNS_RRTYPE_ANY
    )
    {
        cmp = mdns_compare_names(&iface->hostname, dname);
        if (! cmp)
        {
            butil_log(4, "**Matched iface %s\n", mdns_str_for_domain_name(dname));
            return true;
        }
        cmp = mdns_compare_names(&iface->rev_ipv4, dname);
        if (! cmp)
        {
            butil_log(lma, "**==Matched revipv4 %s\n", mdns_str_for_domain_name(dname));
            return true;
        }
        cmp = mdns_compare_names(&iface->rev_ipv6, dname);
        if (! cmp)
        {
            butil_log(lma, "**==Matched revipv6 %s\n", mdns_str_for_domain_name(dname));
            return true;
        }
    }
    if (
            type == DNS_RRTYPE_PTR
        ||  type == DNS_RRTYPE_SRV
        ||  type == DNS_RRTYPE_TXT
        ||  type == DNS_RRTYPE_ANY
    )
    {
        for (service = iface->services; service; service = service->next)
        {
            cmp = mdns_compare_names(&service->usr_domain_name, dname);
            if (! cmp)
            {
                butil_log(lma, "Q*I Matched usr service\n");
                return true;
            }
            cmp = mdns_compare_names(&service->srv_domain_name, dname);
            if (! cmp)
            {
                butil_log(lma, "Q*V Matched srv service\n");
                return true;
            }
            cmp = mdns_compare_names(&service->sub_domain_name, dname);
            if (! cmp)
            {
                butil_log(lma, "Q*S Matched sub service\n");
                return true;
            }
            cmp = mdns_compare_names(&service->sd_domain_name, dname);
            if (! cmp)
            {
                butil_log(lma, "Q*D Matched dns-sd ptr\n");
                return true;
            }
        }
    }
    return false;
}

int mdns_handle_input(mdns_responder_t *res, mdns_interface_t *iface, mdns_packet_t *inpkt)
{
    mdns_packet_t *outpkt;
    dns_domain_name_t *matched_name;
    char fqdn[MDNS_MAX_DNTEXT];
    int inpkt_start_tail;
    int query_start_tail;
    uint16_t reslen;
    uint16_t type;
    uint16_t clas;
    bool we_care;
    bool isunicast;
    int num_matches;
    int question_num;
    int answer_num;
    int tot_answers;
    int result;

    if (! res || ! iface || ! inpkt)
    {
        return 0;
    }
    inpkt_start_tail = inpkt->io.tail;

    mdns_read_uint16(&inpkt->io, &inpkt->id);
    mdns_read_uint16(&inpkt->io, &inpkt->flags);
    mdns_read_uint16(&inpkt->io, &inpkt->qdcount);
    mdns_read_uint16(&inpkt->io, &inpkt->ancount);
    mdns_read_uint16(&inpkt->io, &inpkt->nscount);
    mdns_read_uint16(&inpkt->io, &inpkt->arcount);

    isunicast = false;

    query_start_tail = inpkt->io.tail;

    // put tail back to start to dump whole packet
    inpkt->io.tail = inpkt_start_tail;
    inpkt->io.count = inpkt->io.head - inpkt->io.tail;

    mdns_dump_packet("<<<<< RX", inpkt, 6);

    // restore tail to start of query
    inpkt->io.tail = query_start_tail;
    inpkt->io.count = inpkt->io.head - inpkt->io.tail;

    #if 1
    // silently ignore authoritative packets from random port
    //
    if (MDNS_FLAG_AA(inpkt->flags) && inpkt->srcport != MDNS_PORT)
    {
        butil_log(8, "Ingore AA on port %u\n", (uint32_t)inpkt->srcport);
        return 0;
    }
    #endif
    // First, parse all of the questions, ignoring them, just to get to the answers.
    // We process the answers first, which allows us to NOT build replies that aren't
    // needed, and that simplifies everything else by never having to undo / unwind
    // replies and name tables, etc.
    //
    result = 0;
    num_matches = 0;

    // reset domain name and known answers
    //
    mdns_clear_name(res, &res->domain_name);

    for (
            answer_num = 0;
            answer_num < res->answer_count;
            answer_num++
    )
    {
        mdns_clear_name(res, &res->known_answers[answer_num].dname);
    }
    res->answer_count = 0;

    for (question_num = 0; question_num < inpkt->qdcount && ! result; question_num++)
    {
        // read the name
        //
        result = mdns_read_name(res, &inpkt->io, &res->domain_name);
        if (result)
        {
            break;
        }
        // and flatten it into a compareable string (todo, do a real lable by label cmp)
        //
        result = mdns_flatten_name(&res->domain_name, fqdn, sizeof(fqdn));
        if (result)
        {
            break;
        }
        // read class and type
        //
        result = mdns_read_uint16(&inpkt->io, &type);
        if (result)
        {
            break;
        }
        result = mdns_read_uint16(&inpkt->io, &clas);
        if (result)
        {
            break;
        }
        isunicast |= (clas & (1 << 15)) ? true : false;
        clas &= ~(1 << 15);

        we_care = mdns_question_relates(iface, &res->domain_name, type, clas);
        if (we_care)
        {
            butil_log(5, "Query for %s matches\n", fqdn);
            num_matches++;
        }
    }
    // no sense reparsing if bogus name
    //
    if (result)
    {
        return result;
    }
    // If this is a query, and there are no questions matching any of our
    // name or service names, then there's nothing to reply to, so leave
    //
    if (inpkt->qdcount > 0 && num_matches == 0)
    {
        return 0;
    }
    // Now, process the answer records, building a list of answers
    //
    tot_answers = inpkt->ancount + inpkt->nscount + inpkt->arcount;

    if (tot_answers > 0)
    {
        butil_log(5, "\n--Process %d Answers\n", tot_answers);
    }
    for (
            answer_num = 0;
            answer_num < tot_answers && ! result;
            answer_num++
    )
    {
        // read the name
        //
        result = mdns_read_name(res, &inpkt->io, &res->domain_name);
        if (result)
        {
            break;
        }
        // and flatten it into a string for debug
        //
        result = mdns_flatten_name(&res->domain_name, fqdn, sizeof(fqdn));
        if (result)
        {
            break;
        }
        // read class and type
        //
        result = mdns_read_uint16(&inpkt->io, &type);
        if (result)
        {
            break;
        }
        result = mdns_read_uint16(&inpkt->io, &clas);
        if (result)
        {
            break;
        }
        isunicast |= (clas & (1 << 15)) ? true : false;
        clas &= ~(1 << 15);

        // read TTL
        //
        result = mdns_read_uint32(&inpkt->io, &inpkt->ttl);
        if (result)
        {
            break;
        }
        // read resource length
        //
        result = mdns_read_uint16(&inpkt->io, &reslen);
        if (result)
        {
            break;
        }
        if (inpkt->io.count < reslen)
        {
            butil_log(2, "Truncated Resource\n");
            break;
        }
        butil_log(6, "AN: %d of %d Name=%s= type=%d clas=%04X uni=%d ttl=%d rlen=%u\n",
                   answer_num + 1, tot_answers, fqdn, type, clas, isunicast, inpkt->ttl, reslen);

        // care about classes we care about
        //
        if (clas == DNS_CLASS_IN || clas == DNS_CLASS_ANY)
        {
            we_care = mdns_question_relates(iface, &res->domain_name, type, clas);

            if (we_care)
            {
                mdns_service_t *service;
                char old_name[MDNS_MAX_LABEL];
                char *pp;
                int cmp;

                if (iface->state < MDNS_ANNOUNCE_1)
                {
                    // if in probe states, check for conflicts
                    //
                    butil_log(5, "Probe AN: %d of %d Name=%s= type=%d clas=%04X uni=%d ttl=%d rlen=%u\n",
                              answer_num + 1, tot_answers, fqdn, type, clas, isunicast, inpkt->ttl, reslen);

                    // if there is a conflict with a service instance, rename ours and retry probe
                    //
                    for (service = iface->services; service; service = service->next)
                    {
                        cmp = mdns_compare_names(&service->usr_domain_name, &res->domain_name);
                        if (! cmp)
                        {
                            butil_log(5, "Probe Matched service instance %s\n", fqdn);

                            // rename the service instance (+1) and retry probe from scratch
                            //
                            strcpy(old_name, res->domain_name.labels[0].name);

                            // look for existing (N)
                            //
                            for (pp = strchr(old_name, '('); pp;)
                            {
                                if (pp[1] >= '0' && pp[1] <= '9')
                                {
                                    if (! strchr(pp, '('))
                                    {
                                        break;
                                    }
                                }
                                pp = strchr(pp, '(');
                            }
                            if (pp)
                            {
                                *pp = '\0';
                            }
                            service->usr_domain_name.tot_len -= service->usr_domain_name.labels[0].length;
                            service->usr_domain_name.labels[0].length = snprintf(
                                                service->usr_domain_name.labels[0].name,
                                                MDNS_MAX_LABEL - 1,
                                                "%s (%d)",
                                                old_name,
                                                service->probes + 1
                                                );
                            service->usr_domain_name.tot_len += service->usr_domain_name.labels[0].length;
                            mdns_flatten_name(&service->usr_domain_name, fqdn, sizeof(fqdn));
                            butil_log(4, "Rename service instance to %s\n", fqdn);
                            iface->state = MDNS_PROBE_1;
                            return 0;
                        }
                    }
                }
                else if (inpkt->qdcount > 0)
                {
                    // add the answer record to the list, to check later, if any questions we'll be asked
                    //
                    butil_log(5, "Known AN: %d of %d Name=%s= type=%d clas=%04X uni=%d ttl=%d rlen=%u\n",
                              answer_num + 1, tot_answers, fqdn, type, clas, isunicast, inpkt->ttl, reslen);
                    if (res->answer_count >= MDNS_MAX_ANSWERS)
                    {
                        butil_log(1, "No storage for known answer\n");
                    }
                    else
                    {
                        dns_rr_rec_t *rr;

                        rr = &res->known_answers[res->answer_count++];

                        // copy answer to rr struct
                        //
                        mdns_clear_name(res, &rr->dname);
                        mdns_copy_name(res, &rr->dname, &res->domain_name);

                        rr->type = type;
                        rr->clas = clas;
                        rr->ttl  = inpkt->ttl;
                        rr->reslen = reslen;

                        // for resoure record types that contain domain names, read them
                        // and flatten them to place in answer uncompressed since we aren't
                        // storing the whole packet
                        //
                        if (type == DNS_RRTYPE_PTR || type == DNS_RRTYPE_SRV)
                        {
                            ioring_t resio;
                            dns_domain_name_t res_name;

                            resio.tail = 0;
                            resio.head = 0;
                            resio.count = 0;
                            resio.size = sizeof(rr->res);
                            resio.data = rr->res;

                            if (type == DNS_RRTYPE_SRV)
                            {
                                memcpy(resio.data, inpkt->io.data + inpkt->io.tail, 6);
                                resio.head += 6;
                                resio.count += 6;
                                inpkt->io.tail += 6;
                                inpkt->io.count -= 6;
                            }
                            // read the name, uncompressing it
                            //
                            mdns_init_name(&res_name);

                            result = mdns_read_name(res, &inpkt->io, &res_name);

                            // and write it into the resource which will remain
                            // flat since there are no other strings to compress with
                            //
                            result |= mdns_write_name(&resio, &res_name, -1);

                            // back annotate resource length
                            //
                            rr->reslen = resio.count;

                            mdns_clear_name(res, &res_name);

                            result = 0; // ignore failure here, not a bit loss
                        }
                        else
                        {
                            // just save binary blob resource
                            //
                            if (reslen > sizeof(rr->res))
                            {
                                butil_log(3, "Truncating resource of %s\n", fqdn);
                                // no sense copying partial resource really
                                memcpy(rr->res, inpkt->io.data + inpkt->io.tail, sizeof(rr->res));
                            }
                            else
                            {
                                memcpy(rr->res, inpkt->io.data + inpkt->io.tail, reslen);
                            }
                            inpkt->io.tail += reslen;
                            inpkt->io.count -= reslen;
                        }
                    }
                }
            }
        }
    }
    // restore tail to start of query
    inpkt->io.tail = query_start_tail;
    inpkt->io.count = inpkt->io.head - inpkt->io.tail;

    // Finally, re-traverse the questions and reply to questions that aren't handled
    // in the known answers
    //
    if (inpkt->qdcount > 0)
    {
        butil_log(5, "--Process %d Questions\n", inpkt->qdcount);
    }
    outpkt = NULL;

    for (question_num = 0; question_num < inpkt->qdcount && ! result; question_num++)
    {
        // read the name
        //
        result = mdns_read_name(res, &inpkt->io, &res->domain_name);
        if (result)
        {
            break;
        }
        // and flatten it into a string for debug
        //
        result = mdns_flatten_name(&res->domain_name, fqdn, sizeof(fqdn));
        if (result)
        {
            break;
        }
        // read class and type
        //
        result = mdns_read_uint16(&inpkt->io, &type);
        if (result)
        {
            break;
        }
        result = mdns_read_uint16(&inpkt->io, &clas);
        if (result)
        {
            break;
        }
        isunicast |= (clas & (1 << 15)) ? true : false;
        clas &= ~(1 << 15);

        butil_log(5, " QR: %d of %d Name=%s= type=%d clas=%04X ucast=%d\n",
                           question_num + 1, inpkt->qdcount, fqdn, type, clas, isunicast);

        // for questions we care about, either interface, or service, respond
        //
        if (! MDNS_FLAG_QR(inpkt->flags))
        {
            if (! outpkt)
            {
                // create an output packet to store reply in
                //
                outpkt = mdns_pkt_alloc(res);
                if (! outpkt)
                {
                    butil_log(3, "Can't respond: no outpkts\n");
                    return 0; // todo ?
                }
                outpkt->id = inpkt->id;
                outpkt->flags = MDNS_FLAG_QR(MDNS_FLAG_ALL);
                outpkt->unicast = isunicast;
                // copy inpkt address for unicast
                outpkt->srcport = inpkt->srcport;
                outpkt->isv6addr = inpkt->isv6addr;
                outpkt->srcaddr4.addr = inpkt->srcaddr4.addr;
                memcpy(&outpkt->srcaddr6.addr, &inpkt->srcaddr6.addr, sizeof(bipv6addr_t));
            }
            result = mdns_answer_question(res, iface, &res->domain_name, type, clas, res->known_answers, res->answer_count, outpkt);
        }
        else
        {
            // a response, if it compares against our service or host name,
            // defend it unless we're probing
            //
            we_care = mdns_question_relates(iface, &res->domain_name, type, clas);

            if (we_care)
            {
                mdns_service_t *service;
                int cmp;

                if (iface->state >= MDNS_ANNOUNCE_1)
                {
                    butil_log(5, "Matching Response QD Name=%s= type=%d clas=%04X\n", fqdn, type, clas);

                    for (service = iface->services; service; service = service->next)
                    {
                        cmp = mdns_compare_names(&service->usr_domain_name, &res->domain_name);
                        if (! cmp)
                        {
                            butil_log(5, "Response Matched service instance %s\n", fqdn);

                            // defend ourselves
                            //
                            result = mdns_answer_question(res, iface, &res->domain_name, type, clas, NULL, 0, outpkt);
                        }
                    }
                }
            }
        }
    }
    if (outpkt)
    {
        result = mdns_enqueue_out(res, iface, outpkt, 0);
    }
    return result;
}

int mdns_announce(mdns_responder_t *res, mdns_interface_t *iface, bool startup_delay)
{
    mdns_service_t *service;
    mdns_packet_t *outpkt;
    int result;

    result = 0;

    // announce the ptr and txt first, then the srv. it can be done all in one
    // packet, but txt can be big, and this ensures even with large txt recs
    // that all the data is sent. plus Apple does it that way is seems
    //
    for (service = iface->services; service; service = service->next)
    {
        // create an output packet to store reply in
        //
        outpkt = mdns_pkt_alloc(res);
        if (! outpkt)
        {
            butil_log(3, "Can't announce: no outpkts\n");
            return -1;
        }
        outpkt->id = 0;
        outpkt->flags = MDNS_FLAG_QR(MDNS_FLAG_ALL);

        butil_log(5, "Announcing service %s\n", mdns_str_for_domain_name(&service->usr_domain_name));

        // service instance record. text record
        //
        result = mdns_answer_question(res, iface, &service->usr_domain_name, DNS_RRTYPE_TXT, DNS_CLASS_IN, NULL, 0, outpkt);
        if (result)
        {
            break;
        }
        // service domain name record, PTR to our service instance
        //
        result = mdns_answer_question(res, iface, &service->srv_domain_name, DNS_RRTYPE_PTR, DNS_CLASS_IN, NULL, 0, outpkt);
        if (result)
        {
            break;
        }
        // sub service domain name record, PTR to our service instance
        //
        result = mdns_answer_question(res, iface, &service->sub_domain_name, DNS_RRTYPE_PTR, DNS_CLASS_IN, NULL, 0, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_enqueue_out(res, iface, outpkt, startup_delay ? 210 : 0);
        if (result)
        {
            break;
        }
        // create an output packet to store SRV in
        //
        outpkt = mdns_pkt_alloc(res);
        if (! outpkt)
        {
            butil_log(3, "Can't announce: no outpkts\n");
            result = -1;
            break;
        }
        outpkt->id = 0;
        outpkt->flags = MDNS_FLAG_QR(MDNS_FLAG_ALL);

        result = mdns_answer_question(res, iface, &service->usr_domain_name, DNS_RRTYPE_SRV, DNS_CLASS_IN, NULL, 0, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_enqueue_out(res, iface, outpkt, startup_delay ? 220 : 0);
    }
    outpkt = mdns_pkt_alloc(res);
    if (! outpkt)
    {
        butil_log(3, "Can't announce revip: no outpkts\n");
        return -1;
    }
    outpkt->id = 0;
    outpkt->flags = MDNS_FLAG_QR(MDNS_FLAG_ALL);

    // add iface's reverse address mappings
    //
    if (iface->ipv4addr.addr != 0)
    {
        result = mdns_answer_question(res, iface, &iface->rev_ipv4, DNS_RRTYPE_PTR, DNS_CLASS_IN, NULL, 0, outpkt);
        if (result)
        {
            return result;
        }
    }
    if (iface->ipv6addr.addr[0] != 0)
    {
        result = mdns_answer_question(res, iface, &iface->rev_ipv6, DNS_RRTYPE_PTR, DNS_CLASS_IN, NULL, 0, outpkt);
        if (result)
        {
            return result;
        }
    }
    /*
    for (service = iface->services; service; service = service->next)
    {
        // dns service discovery record. PTR to our service (srv_domain_name)
        //
        result = mdns_answer_question(res, iface, &service->sd_domain_name, DNS_RRTYPE_PTR, DNS_CLASS_IN, NULL, 0, outpkt);
        if (result)
        {
            return result;
        }
    }
    */
    result = mdns_enqueue_out(res, iface, outpkt, startup_delay ? 220 : 0);
    return result;
}

int mdns_saybyebye(mdns_responder_t *res, mdns_interface_t *iface)
{
    mdns_service_t *service;

    // a byebye is just an announce, but with 0 TTL
    //
    for (service = iface->services; service; service = service->next)
    {
        service->ttl = 0;
    }
    iface->ttl = 0;
    return mdns_announce(res, iface, false);
}

int mdns_probe(mdns_responder_t *res, mdns_interface_t *iface)
{
    mdns_service_t *service;
    mdns_packet_t *outpkt;
    uint32_t delay;
    int result;

    result = 0;

    // RFC6762 - random delay 0-250 probe1, 250 after
    switch (iface->state)
    {
    case MDNS_PROBE_1:
        delay = mdns_get_rnd_delay_ms(250);
        break;
    case MDNS_PROBE_2:
        delay = 250;
        break;
    case MDNS_PROBE_3:
        delay = 250;
        break;
    default:
        butil_log(1, "Why are we here\n");
        return -1;
    }
    // probe for each interface's address and each service instance
    //
    for (service = iface->services; service; service = service->next)
    {
        // bump probe count for each start of probing
        //
        if (iface->state == MDNS_PROBE_1)
        {
            service->probes++;
        }
        // create an output packet to store reply in
        //
        outpkt = mdns_pkt_alloc(res);
        if (! outpkt)
        {
            butil_log(3, "Can't Probe: no outpkts\n");
            return -1;
        }
        outpkt->id = 0;
        outpkt->flags = 0;

        butil_log(5, "Probing service %s\n", mdns_str_for_domain_name(&service->usr_domain_name));

        result = mdns_ask_question(iface, &service->interface->hostname, DNS_RRTYPE_ANY, DNS_CLASS_IN, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_ask_question(iface, &service->usr_domain_name, DNS_RRTYPE_ANY, DNS_CLASS_IN, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_answer_question(res, iface, &service->interface->hostname, DNS_RRTYPE_A, DNS_CLASS_IN, NULL, 0, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_answer_question(res, iface, &service->interface->hostname, DNS_RRTYPE_AAAA, DNS_CLASS_IN, NULL, 0, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_answer_question(res, iface, &service->usr_domain_name, DNS_RRTYPE_SRV, DNS_CLASS_IN, NULL, 0, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_enqueue_out(res, iface, outpkt, 150);
        if (result)
        {
            break;
        }
    }
    return result;
}

int mdns_responder_slice(mdns_responder_t *res)
{
    mdns_interface_t *iface;
    mdns_packet_t *pkt;
    int result;

    for (
            iface = res->interfaces, result = 0;
            iface && ! result && ! res->stopped;
            iface = iface->next
    )
    {
        switch (iface->state)
        {
        case MDNS_PROBE_1:
            result = mdns_probe(res, iface);
            iface->state = MDNS_PROBE_2;
            break;
        case MDNS_PROBE_2:
            if (iface->outpkts)
            {
                break;
            }
            result = mdns_probe(res, iface);
            iface->state = MDNS_PROBE_3;
            break;
        case MDNS_PROBE_3:
            if (iface->outpkts)
            {
                break;
            }
            result = mdns_probe(res, iface);
            iface->state = MDNS_PROBE_FLUSH;
            break;
        case MDNS_PROBE_FLUSH:
            if (iface->outpkts)
            {
                break;
            }
            iface->state = MDNS_ANNOUNCE_1;
            break;
        case MDNS_ANNOUNCE_1:
            result = mdns_announce(res, iface, true);
            iface->state = MDNS_ANNOUNCE_2;
            break;
        case MDNS_ANNOUNCE_2:
            result = mdns_announce(res, iface, true);
            iface->state = MDNS_RUN;
            break;
        case MDNS_BYEBYE:
            result = mdns_saybyebye(res, iface);
            iface->state = MDNS_DONE;
            break;
        case MDNS_DONE:
            if (iface->outpkts)
            {
                break;
            }
            return 0;
        default:
            break;
        }
        // check output queue
        //
        if (iface->outpkts)
        {
            result = mdns_handle_output(res, iface);
            if (result)
            {
                return result;
            }
        }
        if (iface->state > MDNS_PROBE_1 && iface->state < MDNS_BYEBYE)
        {
            // check input socket but (RFC6762 8.1 ignore conflicts before probe)
            //
            result = mdns_check_input(res, iface);
            if (result)
            {
                return result;
            }
        }
        // check input queue
        //
        if (iface->inpkts)
        {
            pkt = iface->inpkts;
            iface->inpkts = pkt->next;

            result = mdns_handle_input(res, iface, pkt);

            mdns_pkt_free(res, pkt);

            if (result)
            {
                if (res->fatal)
                {
                    return result;
                }
                // ignore regular errors unless fatal indicated
                // we have no idea what kind of bad packets well get
                //
                result = 0;
            }
        }
    }
    return result;
}

int mdns_responder_run(mdns_responder_t *res, int poll_secs, int poll_usecs)
{
    mdns_interface_t *iface;
    int result;

    if (! res)
    {
        return -1;
    }
    if (! res->interfaces)
    {
        butil_log(1, "No interfaces for responder\n");
        return -1;
    }
    else
    {
        res->to_secs = poll_secs;
        res->to_usecs = poll_usecs;

        butil_log(5, "Starting MDNS with interfaces:\n");

        for (iface = res->interfaces; iface; iface = iface->next)
        {
            mdns_dump_interface(iface, 5);
            if (! iface->services)
            {
                butil_log(1, "No services for responder\n");
            }
            else
            {
                mdns_service_t *service;

                butil_log(5, "Starting MDNS with services:\n");
                for (service = iface->services; service; service = service->next)
                {
                    mdns_dump_service(service, 5);
                }
            }
        }
    }
    result = 0;

    res->stopped = false;

    while (! result && ! res->stopped && ! res->fatal)
    {
        result = mdns_responder_slice(res);
    }
    return result;
}

int mdns_responder_stop(mdns_responder_t *res)
{
    mdns_interface_t *iface;
    int slice;
    int result;

    if (! res)
    {
        return -1;
    }
    // setup each interface to say bye, or not run
    //
    for (iface = res->interfaces; iface; iface = iface->next)
    {
        if (iface->state > MDNS_ANNOUNCE_1)
        {
            iface->state = MDNS_BYEBYE;
        }
        else
        {
            iface->state = MDNS_DONE;
        }
    }
    // slice a number of times to complete bye
    //
    for (slice = 0; slice < 100; slice++)
    {
        result = mdns_responder_slice(res);
    }
    res->stopped = true;
    return 0;
}

int mdns_responder_add_interface(
                                mdns_responder_t *res,
                                const char *hostname,
                                bipv4addr_t *ipv4addr,
                                bipv6addr_t *ipv6addr,
                                uint32_t ttl
                                )
{
    mdns_interface_t *iface;
    struct ip_mreq mreq;
    char buffer[MDNS_MAX_DNTEXT];
    int iparm;
    uint16_t port;
    int result;

    if (! res || ! hostname)
    {
        return -1;
    }
    iface = mdns_interface_alloc(res, hostname);
    if (! iface)
    {
        butil_log(0, "Can't alloc interface for %s\n", hostname);
        return -1;
    }
    memset(&iface->hostname.last_sent, 0, sizeof(iface->hostname.last_sent));
    iface->services = NULL;

    memcpy(&iface->ipv4addr, ipv4addr, sizeof(bipv4addr_t));
    memcpy(&iface->ipv6addr, ipv6addr, sizeof(bipv6addr_t));

    mdns_init_name(&iface->rev_ipv4);
    mdns_init_name(&iface->rev_ipv6);

    // build reverse mappings of ip addresses
    //
    result = mdns_assemble_name(buffer, sizeof(buffer), 3,
                            mdns_reverse_ipv4addr(ipv4addr), "in-addr", "arpa");
    result |= mdns_unflatten_name(res, buffer, &iface->rev_ipv4);
    if (result)
    {
        butil_log(0, "Can't build rev ipv4\n");
        return -1;
    }
    result = mdns_assemble_name(buffer, sizeof(buffer), 3,
                            mdns_reverse_ipv6addr(ipv6addr), "ip6", "arpa");
    result |= mdns_unflatten_name(res, buffer, &iface->rev_ipv6);
    if (result)
    {
        butil_log(0, "Can't build rev ipv4\n");
        return -1;
    }

    iface->ttl = ttl;

    // create a udp socket on port 5353 to listen
    //
    iface->udp_sock = iostream_create_udp_socket();
    if (iface->udp_sock == INVALID_SOCKET)
    {
        butil_log(0, "Can't make udp socket\n");
        return -1;
    }
    port = res->unit_testing ? 25353 : MDNS_PORT;

    result = iostream_bind_socket(iface->udp_sock, port);
    if (result)
    {
        butil_log(0, "Can't bind udp socket on port %u: %d\n",
               (uint32_t)port, errno);
        return -1;
    }
    // join the multicast group for mdns
    //
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(MDNS_MCAST_IP4ADDR);
    mreq.imr_interface.s_addr = INADDR_ANY;

    result = setsockopt(iface->udp_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
    if (result < 0)
    {
        butil_log(0, "Can't join mcast group: %d\n", errno);
        return result;
    }
    // disable loopback for the socket, to avoid read loops
    //
    iparm = 0;
    result = setsockopt(iface->udp_sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&iparm, sizeof(iparm));
    if (result < 0)
    {
        butil_log(0, "Can't disable loopback: %d\n", errno);
        return result;
    }
    iparm = 1;
    result = setsockopt(iface->udp_sock, SOL_SOCKET, SO_BROADCAST, (char*)&iparm, sizeof(iparm));
    if (result < 0)
    {
        butil_log(0, "Can't enable bcast: %d\n", errno);
        return result;
    }
    iface->inpkts  = NULL;
    iface->outpkts = NULL;

    // start out in probe state
    //
    iface->state = MDNS_PROBE_1;

    iface->next = res->interfaces;
    res->interfaces = iface;

    return 0;
}

int mdns_responder_add_service(
                                mdns_responder_t *res,
                                mdns_interface_t *iface,
                                const char *srvname,
                                const char *dnsname,
                                const char *subname,
                                mdns_service_protocol_t proto,
                                uint16_t port,
                                const char *txtrecs,
                                uint32_t ttl
                              )
{
    mdns_service_t *service;
    char name[MDNS_MAX_DNTEXT];
    int result;

    if (! res || ! iface || ! srvname || ! dnsname || ! subname)
    {
        return -1;
    }
    service = mdns_service_alloc(res);
    if (! service)
    {
        butil_log(0, "Can't alloc service for %s\n", name);
        return -1;
    }
    service->interface = iface;
    service->proto = proto;
    service->port = port;
    service->ttl = ttl;
    service->probes = 0;

    mdns_init_name(&service->usr_domain_name);
    mdns_init_name(&service->srv_domain_name);
    mdns_init_name(&service->sub_domain_name);
    mdns_init_name(&service->txt_records);

    // create a domain name like "_username._servicename._proto.local"
    //
    result = mdns_assemble_name(
                                name,
                                sizeof(name),
                                4,
                                srvname,
                                dnsname,
                                mdns_srv_proto_name(proto),
                                "local"
                                );
    if (result)
    {
        butil_log(0, "service: name too big\n");
        free(service);
        return result;
    }
    // convert to domain name struct
    //
    result = mdns_unflatten_name(res, name, &service->usr_domain_name);
    if (result)
    {
        free(service);
        return result;
    }
    memset(&service->usr_domain_name.last_sent, 0, sizeof(service->usr_domain_name.last_sent));

    // create a domain name like "_servicename._proto.local"
    //
    result = mdns_assemble_name(
                                name,
                                sizeof(name),
                                3,
                                dnsname,
                                mdns_srv_proto_name(proto),
                                "local"
                                );
    if (result)
    {
        butil_log(0, "service: name too big\n");
        free(service);
        return result;
    }
    // convert to domain name struct
    //
    result = mdns_unflatten_name(res, name, &service->srv_domain_name);
    if (result)
    {
        free(service);
        return result;
    }
    memset(&service->srv_domain_name.last_sent, 0, sizeof(service->srv_domain_name.last_sent));

    // create a domain name like "_universal._sub._dnsname._proto.local"
    //
    result = mdns_assemble_name(
                                name,
                                sizeof(name),
                                5,
                                subname,
                                "_sub",
                                dnsname,
                                mdns_srv_proto_name(proto),
                                "local"
                                );
    if (result)
    {
        butil_log(0, "subservice: name too big\n");
        free(service);
        return result;
    }
    // convert to domain name struct
    //
    result = mdns_unflatten_name(res, name, &service->sub_domain_name);
    if (result)
    {
        free(service);
        return result;
    }
    memset(&service->sub_domain_name.last_sent, 0, sizeof(service->sub_domain_name.last_sent));

    // finally create a domain name like "_dns-sd._udp.local" to point to the service
    //
    result = mdns_assemble_name(
                                name,
                                sizeof(name),
                                1,
                                "_services._dns-sd._udp.local"
                                );
    if (result)
    {
        butil_log(0, "can't make dns-sd name\n");
        free(service);
        return result;
    }
    // convert to domain name struct
    //
    result = mdns_unflatten_name(res, name, &service->sd_domain_name);
    if (result)
    {
        free(service);
        return result;
    }
    memset(&service->sd_domain_name.last_sent, 0, sizeof(service->sd_domain_name.last_sent));

    if (txtrecs)
    {
        // break comma separated txt recs into labels
        //
        result = mdns_unflatten_txt(res, txtrecs, &service->txt_records);
        if (result)
        {
            free(service);
            return result;
        }
    }
    memset(&service->txt_records.last_sent, 0, sizeof(service->txt_records.last_sent));

    // add service to interface list
    //
    service->next = iface->services;
    iface->services = service;
    return 0;
}

int mdns_responder_init(mdns_responder_t *res)
{
    mdns_packet_t *pkt;
    int count;
    int result;

    if (! res)
    {
        return -1;
    }
    memset(res, 0, sizeof(mdns_responder_t));

    // seed random generator
    //
    srand(time(NULL));

    res->answer_count = 0;
    mdns_init_name(&res->domain_name);

    res->to_secs = 0;
    res->to_usecs = 50000;

    res->stopped = false;
    res->fatal = false;

    // create a packet pool
    //
    res->pkt_pool = (mdns_packet_t *)mdns_large_alloc(res, MDNS_PACKET_POOL_SIZE * sizeof(mdns_packet_t));
    if (! res->pkt_pool)
    {
        BERROR("Alloc pool");
        return -1;
    }
    for (count = 0; count < MDNS_PACKET_POOL_SIZE - 1; count++)
    {
        pkt = &res->pkt_pool[count];
        pkt->io.head = 0;
        pkt->io.tail = 0;
        pkt->io.count = 0;
        pkt->io.size = sizeof(pkt->data);
        pkt->io.data = pkt->data;

        if (count < (MDNS_PACKET_POOL_SIZE - 1))
        {
            pkt->next = & res->pkt_pool[count + 1];
        }
        else
        {
            pkt->next = NULL;
        }
    }
    res->pkt_free = res->pkt_pool;

#if MDNS_TRACE_ALLOCS
    // create alloc pools for label and text strings
    //
    void *pool;
    size_t pool_size, pool_chunk;
    size_t size;

    result = bmem_init();
    if (result)
    {
        BERROR("Can't init pool\n");
        return -1;
    }
    pool_size = ((MDNS_LABEL_POOL + 2) * MDNS_MAX_LABEL) + 2 * bmem_pool_overhead();
    pool_chunk = MDNS_MAX_LABEL;
    res->label_pool = malloc(pool_size);
    if (! res->label_pool)
    {
        BERROR("Can't alloc label pool\n");
        return -1;
    }
    result = bmem_add_pool(res->label_pool, pool_size, pool_chunk);
    if (result)
    {
        BERROR("Can't add label pool\n");
        return -1;
    }
    pool_size = ((MDNS_TXTREC_POOL + 2) * MDNS_MAX_LABEL) + 2 * bmem_pool_overhead();
    pool_chunk = MDNS_MAX_DNTEXT;
    res->txt_pool = malloc(pool_size);
    if (! res->label_pool)
    {
        BERROR("Can't alloc txt pool\n");
        return -1;
    }
    result = bmem_add_pool(res->txt_pool, pool_size, pool_chunk);
    if (result)
    {
        BERROR("Can't add txt pool\n");
        return -1;
    }
#endif
    return 0;
}

int mdns_responder_deinit(mdns_responder_t *res)
{
    mdns_interface_t *iface;
    mdns_service_t *service;
    int answer_num;

    if (! res)
    {
        return -1;
    }
    for (iface = res->interfaces; iface; iface = iface->next)
    {
        if (iface->udp_sock != INVALID_SOCKET)
        {
            close_socket(iface->udp_sock);
        }
        for (service = iface->services; service; service = service->next)
        {
            mdns_clear_name(res, &service->usr_domain_name);
            mdns_clear_name(res, &service->srv_domain_name);
            mdns_clear_name(res, &service->sub_domain_name);
            mdns_clear_name(res, &service->txt_records);
            mdns_service_free(res, service);
        }
        mdns_clear_name(res, &iface->hostname);
        mdns_clear_name(res, &iface->rev_ipv4);
        mdns_clear_name(res, &iface->rev_ipv6);
    }
    while (res->interfaces)
    {
        iface = res->interfaces;
        res->interfaces = iface->next;
        mdns_interface_free(res, iface);
    }
    res->interfaces = NULL;

    for (
            answer_num = 0;
            answer_num < res->answer_count;
            answer_num++
    )
    {
        mdns_clear_name(res, &res->known_answers[answer_num].dname);
    }
    mdns_clear_name(res, &res->domain_name);

    if (res->pkt_pool)
    {
        mdns_large_free(res, res->pkt_pool);
    }
#if MDNS_TRACE_ALLOCS
    bmem_stats(5);
    free(res->label_pool);
    free(res->txt_pool);
#endif
    return 0;
}

