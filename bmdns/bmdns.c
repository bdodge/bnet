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

// TODO
//    compress outgoing names
//    probe
//    conflict resolution
//    finish announce
//
int mdns_handle_output(mdns_responder_t *res, mdns_interface_t *iface, mdns_packet_t *pkt)
{
    struct sockaddr *dstaddr;
    struct sockaddr_in dst4addr;
    struct sockaddr_in6 dst6addr;
    int dstsize;
    int count;
    int result;

    if (! res || ! iface || ! pkt)
    {
        return -1;
    }
    if (! res || iface->in_sock == INVALID_SOCKET)
    {
        BERROR("bad context");
        return -1;
    }
    if (pkt->unicast)
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
    result = iostream_posix_poll_filedesc(iface->in_sock, writeable, 0, 100000);
    if (result < 0)
    {
        BERROR("socket closed?");
        return result;
    }
    if (result == 0)
    {
        return 0;
    }
    count = sendto(iface->in_sock, pkt->io.data, pkt->io.count,
                             0, (struct sockaddr *)dstaddr,  dstsize);

    if (pkt->unicast)
    {
        butil_log(5, "TX %d to %s %u  res %d (%d)\n", pkt->io.count,
                        mdns_str_for_pktaddr(pkt), (uint32_t)pkt->srcport, count, errno);
    }
    else
    {
        butil_log(5, "TX %d multicast res %d (%d)\n", pkt->io.count, count, errno);
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
    return 0;
}

static int mdns_enqueue_out(mdns_responder_t *res, mdns_interface_t *iface, mdns_packet_t *outpkt)
{
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
        // queue output packet for sending
        //
        mdns_dump_packet(">>>>>> TX", outpkt, 6);

        if (! iface->outpkts)
        {
            iface->outpkts = outpkt;
        }
        else
        {
            mdns_packet_t *pend;

            for (pend = iface->outpkts; pend->next;)
            {
                pend = pend->next;
            }
            pend->next = outpkt;
        }
        outpkt->next = NULL;
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

    if (! res || ! iface || iface->in_sock == INVALID_SOCKET)
    {
        BERROR("bad context");
        return -1;
    }
    result = iostream_posix_poll_filedesc(iface->in_sock, readable, 0, 100000);
    if (result < 0)
    {
        BERROR("socket closed?");
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
    count = recvfrom(iface->in_sock, pkt->io.data, pkt->io.size,
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

static int mdns_query_respond(
                            mdns_interface_t *iface,
                            mdns_packet_t *outpkt,
                            dns_domain_name_t *fqdn,
                            uint16_t type,
                            uint16_t clas,
                            mdns_service_t *service
                         )
{
    int reslen;
    int result;

    // caller should have incremented whichever count
    // this response goes in before calling here
    //
    // (re) write packet header with updated info
    result = mdns_rewrite_header(&outpkt->io, outpkt);
    if (result)
    {
        return result;
    }
    // write name
    result = mdns_write_name(&outpkt->io, fqdn);
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
    result = mdns_write_uint32(&outpkt->io, service ? service->ttl : iface->ttl);
    if (result)
    {
        return result;
    }
    switch (type)
    {
    case DNS_RRTYPE_PTR:
        if (! service)
        {
            reslen = DNS_NAME_LENGTH(&iface->hostname);
            result  = mdns_write_uint16(&outpkt->io, reslen);
            result |= mdns_write_name(&outpkt->io, &iface->hostname);
        }
        else
        {
            reslen = DNS_NAME_LENGTH(&service->usr_domain_name);
            result  = mdns_write_uint16(&outpkt->io, reslen);
            result |= mdns_write_name(&outpkt->io, &service->usr_domain_name);
        }
        break;
    case DNS_RRTYPE_TXT:
        reslen = DNS_TXT_LENGTH(&service->txt_records);
        result  = mdns_write_uint16(&outpkt->io, reslen);
        result |= mdns_write_text(&outpkt->io, &service->txt_records);
        break;
    case DNS_RRTYPE_SRV:
        reslen = DNS_NAME_LENGTH(&service->interface->hostname) + 6;
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
        // write port
        result |= mdns_write_uint16(&outpkt->io, service->port);
        if (result)
        {
            return result;
        }
        // write target
        result |= mdns_write_name(&outpkt->io, &service->interface->hostname);
        break;
    case DNS_RRTYPE_A:
        result = mdns_write_uint16(&outpkt->io, 4);
        result |= mdns_write_uint32(&outpkt->io, htonl(iface->ipv4addr.addr));
        break;
    case DNS_RRTYPE_AAAA:
        result = mdns_write_uint16(&outpkt->io, 16);
        for (reslen = 0; reslen < 16; reslen++)
        {
            result |= mdns_write_uint32(&outpkt->io, iface->ipv6addr.addr[reslen]);
        }
        break;
    case DNS_RRTYPE_ANY:
    default:
        butil_log(4, "  RR type %d is ignored in PTR\n", type);
        result = 0;
    }
    return result;
}

static int mdns_answer_question(
                                    mdns_interface_t  *iface,
                                    dns_domain_name_t *dname,
                                    uint16_t type,
                                    uint16_t clas,
                                    mdns_packet_t *outpkt
                                 )
{
    mdns_service_t *service;
    mdns_service_t *added_srv;
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
    lma = 5; // log level for match debug

    result = 0;

    if (type == DNS_RRTYPE_ANY)
    {
        // just call the individual types by hand
        //
        result = mdns_answer_question(iface, dname, DNS_RRTYPE_PTR, clas, outpkt);
        result |= mdns_answer_question(iface, dname, DNS_RRTYPE_TXT, clas, outpkt);
        result |= mdns_answer_question(iface, dname, DNS_RRTYPE_SRV, clas, outpkt);
        result |= mdns_answer_question(iface, dname, DNS_RRTYPE_A, clas, outpkt);
        result |= mdns_answer_question(iface, dname, DNS_RRTYPE_AAAA, clas, outpkt);
        return result;
    }
    // match against all the names we care about (host, ip, services)
    //
    cmp = mdns_compare_names(&iface->hostname, dname);
    if (! cmp)
    {
        butil_log(lma, "**==Matched iface %s\n", mdns_str_for_domain_name(dname));
        result = mdns_query_respond(iface, outpkt, dname, type, clas, NULL);
    }
    added_srv = NULL;

    for (service = iface->services; service; service = service->next)
    {
        cmp = mdns_compare_names(&service->usr_domain_name, dname);
        if (! cmp)
        {
            butil_log(lma, "**I==Matched usr service\n");
        }
        else
        {
            cmp = mdns_compare_names(&service->srv_domain_name, dname);
            if (! cmp)
            {
                butil_log(lma, "**V==Matched srv service\n");
            }
            else
            {
                cmp = mdns_compare_names(&service->sub_domain_name, dname);
                if (! cmp)
                {
                    butil_log(lma, "**S==Matched sub service\n");
                }
            }
        }
        if (! cmp)
        {
            outpkt->ancount++;
            if (type == DNS_RRTYPE_SRV)
            {
                added_srv = service;
            }
            result = mdns_query_respond(iface, outpkt, dname, type, clas, service);
        }
    }
    if (added_srv)
    {
        // add address record if added a srv record to match service instance, but in additional section
        // there could only technically be on matched service on a single interface
        //
        outpkt->arcount++;
        result = mdns_query_respond(iface, outpkt,
                            &added_srv->interface->hostname, DNS_RRTYPE_A, clas, added_srv);
    }
    return false;
}

static bool mdns_question_relates(
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
    lma = 6; // log level for match debug

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
                butil_log(4, "**I Matched usr service\n");
                return true;
            }
            cmp = mdns_compare_names(&service->srv_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**V Matched srv service\n");
                return true;
            }
            cmp = mdns_compare_names(&service->sub_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**S Matched sub service\n");
                return true;
            }
        }
    }
    return false;
}

int mdns_handle_input(mdns_responder_t *res, mdns_interface_t *iface, mdns_packet_t *inpkt)
{
    mdns_packet_t *outpkt;
    dns_domain_name_t domain_name;
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

    if (! res || ! inpkt)
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

    mdns_dump_packet("<<<<< RX", inpkt, 5);

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

    for (question_num = 0; question_num < inpkt->qdcount && ! result; question_num++)
    {
        // read the name
        //
        result = mdns_read_name(&inpkt->io, &domain_name);
        if (result)
        {
            break;
        }
        // and flatten it into a compareable string (todo, do a real lable by label cmp)
        //
        result = mdns_flatten_name(&domain_name, fqdn, sizeof(fqdn));
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

        we_care = mdns_question_relates(iface, &domain_name, type, clas);
        if (we_care)
        {
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
        butil_log(5, "--Process %d Answers\n", tot_answers);
    }
    for (
            answer_num = 0;
            answer_num < tot_answers && ! result;
            answer_num++
    )
    {
        // read the name
        //
        result = mdns_read_name(&inpkt->io, &domain_name);
        if (result)
        {
            break;
        }
        // and flatten it into a compareable string (todo, do a real lable by label cmp)
        //
        result = mdns_flatten_name(&domain_name, fqdn, sizeof(fqdn));
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
        inpkt->io.tail += reslen;
        inpkt->io.count -= reslen;

        butil_log(5, "AN: %d of %d Name=%s= type=%d clas=%04X uni=%d ttl=%d rlen=%u\n",
                   answer_num + 1, tot_answers, fqdn, type, clas, isunicast, inpkt->ttl, reslen);

        // care about classes we care about
        //
        if (clas == DNS_CLASS_IN || clas == DNS_CLASS_ANY)
        {
            switch (type)
            {
            case DNS_RRTYPE_A:
            case DNS_RRTYPE_PTR:
            case DNS_RRTYPE_AAAA:
            case DNS_RRTYPE_ANY:
            default:
                break;
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
        butil_log(5, "\n--Process %d Questions\n", inpkt->qdcount);
    }
    outpkt = NULL;

    for (question_num = 0; question_num < inpkt->qdcount && ! result; question_num++)
    {
        // read the name
        //
        result = mdns_read_name(&inpkt->io, &domain_name);
        if (result)
        {
            break;
        }
        // and flatten it into a compareable string (todo, do a real lable by label cmp)
        //
        result = mdns_flatten_name(&domain_name, fqdn, sizeof(fqdn));
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
            result = mdns_answer_question(iface, &domain_name, type, clas, outpkt);
        }
        else
        {
            // a response
            //
        }
    }
    if (outpkt)
    {
        result = mdns_enqueue_out(res, iface, outpkt);
    }
    return result;
}

int mdns_announce(mdns_responder_t *res, mdns_interface_t *iface)
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

        result = mdns_answer_question(iface, &service->usr_domain_name, DNS_RRTYPE_TXT, DNS_CLASS_IN, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_answer_question(iface, &service->sub_domain_name, DNS_RRTYPE_PTR, DNS_CLASS_IN, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_answer_question(iface, &service->srv_domain_name, DNS_RRTYPE_PTR, DNS_CLASS_IN, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_enqueue_out(res, iface, outpkt);
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

        result = mdns_answer_question(iface, &service->usr_domain_name, DNS_RRTYPE_SRV, DNS_CLASS_IN, outpkt);
        if (result)
        {
            break;
        }
        result = mdns_enqueue_out(res, iface, outpkt);
    }
    return result;
}

int mdns_responder_slice(mdns_responder_t *res, mdns_interface_t *iface)
{
    mdns_packet_t *pkt;
    int result;

    switch (iface->state)
    {
    case MDNS_PROBE_1:
    case MDNS_PROBE_2:
    case MDNS_PROBE_3:
        iface->state = MDNS_ANNOUNCE_2;
        break;
    case MDNS_ANNOUNCE_1:
    case MDNS_ANNOUNCE_2:
        result = mdns_announce(res, iface);
        iface->state = MDNS_RUN;
        break;
    default:
        break;
    }
    // check output queue
    //
    if (iface->outpkts)
    {
        pkt = iface->outpkts;
        iface->outpkts = pkt->next;

        result = mdns_handle_output(res, iface, pkt);

        mdns_pkt_free(res, pkt);

        if (result)
        {
            return result;
        }
    }
    // check input socket
    //
    result = mdns_check_input(res, iface);
    if (result)
    {
        return result;
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
            return result;
        }
    }
    return result;
}

int mdns_responder_run(mdns_responder_t *res)
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

    while (! result)
    {
        for (iface = res->interfaces; iface && ! result; iface = iface->next)
        {
            result = mdns_responder_slice(res, iface);
        }
    }
    return result;
}

int mdns_responder_add_interface(
                                mdns_responder_t *res,
                                const char *hostname,
                                bipv4addr_t ipv4addr,
                                bipv6addr_t ipv6addr,
                                uint32_t ttl
                                )
{
    mdns_interface_t *iface;
    struct ip_mreq mreq;
    int iparm;
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
    iface->services = NULL;

    memcpy(&iface->ipv4addr, &ipv4addr, sizeof(bipv4addr_t));
    memcpy(&iface->ipv6addr, &ipv6addr, sizeof(bipv6addr_t));

    iface->ttl = ttl;

    // create a udp socket on port 5353 to listen
    //
    iface->in_sock = iostream_create_udp_socket();
    if (iface->in_sock == INVALID_SOCKET)
    {
        butil_log(0, "Can't make in socket\n");
        return -1;
    }
    result = iostream_bind_socket(iface->in_sock, MDNS_PORT);
    if (result)
    {
        butil_log(0, "Can't bind in socket\n");
        return -1;
    }
    // join the multicast group for mdns
    //
    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(MDNS_MCAST_IP4ADDR);
    mreq.imr_interface.s_addr = INADDR_ANY;

    result = setsockopt(iface->in_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
    if (result < 0)
    {
        butil_log(0, "Can't join mcast group: %d\n", errno);
        return result;
    }
    // disable loopback for the socket, to avoid read loops
    //
    iparm = 0;
    result = setsockopt(iface->in_sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&iparm, sizeof(iparm));
    if (result < 0)
    {
        butil_log(0, "Can't disable loopback: %d\n", errno);
        return result;
    }
    iparm = 1;
    result = setsockopt(iface->in_sock, SOL_SOCKET, SO_BROADCAST, (char*)&iparm, sizeof(iparm));
    if (result < 0)
    {
        butil_log(0, "Can't enable bcast: %d\n", errno);
        return result;
    }
    iface->inpkts  = NULL;
    iface->outpkts = NULL;

    // create probe records for any registered services
    // (todo)
    //

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
                                const char *usrname,
                                const char *srvname,
                                mdns_service_protocol_t proto,
                                uint16_t port,
                                const char *txtrecs,
                                uint32_t ttl
                              )
{
    mdns_service_t *service;
    char name[MDNS_MAX_DNTEXT];
    int result;

    if (! res || ! name)
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

    // create a domain name like "_username._servicename._proto.local"
    //
    result = mdns_assemble_name(
                                name,
                                sizeof(name),
                                4,
                                usrname,
                                srvname,
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
    result = mdns_unflatten_name(name, &service->usr_domain_name);
    if (result)
    {
        free(service);
        return result;
    }
    // create a domain name like "_servicename._proto.local"
    //
    result = mdns_assemble_name(
                                name,
                                sizeof(name),
                                3,
                                srvname,
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
    result = mdns_unflatten_name(name, &service->srv_domain_name);
    if (result)
    {
        free(service);
        return result;
    }
    // finally create a domain name like "_universal._sub._servicename._proto.local"
    //
    result = mdns_assemble_name(
                                name,
                                sizeof(name),
                                5,
                                "_universal",
                                "_sub",
                                srvname,
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
    result = mdns_unflatten_name(name, &service->sub_domain_name);
    if (result)
    {
        free(service);
        return result;
    }
    // break comma separated txt recs into labels
    //
    result = mdns_unflatten_txt(txtrecs, &service->txt_records);
    if (result)
    {
        free(service);
        return result;
    }
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

    // create a packet pool
    //
    res->pkt_pool = (mdns_packet_t *)malloc(MDNS_PACKET_POOL_SIZE * sizeof(mdns_packet_t));
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

    return 0;
}

int mdns_responder_deinit(mdns_responder_t *res)
{
    mdns_interface_t *iface;

    if (! res)
    {
        return -1;
    }
    for (iface = res->interfaces; iface; iface = iface->next)
    {
        if (iface->in_sock != INVALID_SOCKET)
        {
            close_socket(iface->in_sock);
        }
    }
    if (res->pkt_pool)
    {
        free (res->pkt_pool);
    }
    return 0;
}

