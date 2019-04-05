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
//    unicast reply
//    probe
//    conflict resolution
//    finish announce
//
int mdns_handle_output(mdns_responder_t *res, mdns_packet_t *pkt)
{
    struct sockaddr_in dstaddr;
    int count;
    int result;

    if (! res || ! res->in_sock || ! pkt)
    {
        return -1;
    }
    if (! res || res->in_sock == INVALID_SOCKET)
    {
        BERROR("bad context");
        return -1;
    }
    dstaddr.sin_family = AF_INET;
    dstaddr.sin_addr.s_addr = inet_addr(MDNS_MCAST_IP4ADDR);
    dstaddr.sin_port = htons(MDNS_PORT);

    result = iostream_posix_poll_filedesc(res->in_sock, writeable, 0, 100000);
    if (result < 0)
    {
        BERROR("socket closed?");
        return result;
    }
    if (result == 0)
    {
        return 0;
    }
    count = sendto(res->in_sock, pkt->io.data, pkt->io.count,
                             0, (struct sockaddr *)&dstaddr,  sizeof(pkt->srcaddr));

    if (count < 0)
    {
        return count;
    }
    if (count != pkt->io.count)
    {
        butil_log(1, "Partial write of pkt\n");
        return -1;
    }
    butil_log(5, "\nTX %d\n", count);
    return 0;
}

int mdns_check_input(mdns_responder_t *res)
{
    mdns_packet_t *pkt;
    socklen_t srclen;
    int count;
    int result;

    if (! res || res->in_sock == INVALID_SOCKET)
    {
        BERROR("bad context");
        return -1;
    }
    result = iostream_posix_poll_filedesc(res->in_sock, readable, 0, 100000);
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
    srclen = sizeof(pkt->srcaddr);
    count = recvfrom(res->in_sock, pkt->io.data, pkt->io.size,
                         0, (struct sockaddr *)&pkt->srcaddr, &srclen);

    if (count)
    {
        butil_log(6, "\nRX %d from %s port %u\n",
                count, mdns_str_for_sockaddr(&pkt->srcaddr), (uint32_t)htons(pkt->srcaddr.sin_port));

        pkt->io.count += count;
        pkt->io.head += count;
        if (pkt->io.head >= pkt->io.size)
        {
            pkt->io.head = 0;
        }
        // append packet to responder's in list
        //
        if (! res->inpkts)
        {
            res->inpkts = pkt;
        }
        else
        {
            mdns_packet_t *pp;

            for (pp = res->inpkts; pp->next;)
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
                            mdns_responder_t *res,
                            mdns_packet_t *outpkt,
                            dns_domain_name_t *fqdn,
                            uint16_t type,
                            uint16_t clas,
                            uint32_t ttl,
                            mdns_interface_t *iface,
                            mdns_service_t *service,
                            dns_domain_name_t *dname
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
    result |= mdns_write_uint16(&outpkt->io, clas);
    if (result)
    {
        return result;
    }
    // ttl
    result = mdns_write_uint32(&outpkt->io, 10/*ttl*/);
    if (result)
    {
        return result;
    }
    switch (type)
    {
    case DNS_RRTYPE_PTR:
        reslen = DNS_NAME_LENGTH(dname);
        result  = mdns_write_uint16(&outpkt->io, reslen);
        result |= mdns_write_name(&outpkt->io, dname);
        if (result)
        {
            return result;
        }
        break;
    case DNS_RRTYPE_TXT:
        reslen = DNS_TXT_LENGTH(dname);
        result  = mdns_write_uint16(&outpkt->io, reslen);
        result |= mdns_write_text(&outpkt->io, dname);
        if (result)
        {
            return result;
        }
        break;
    case DNS_RRTYPE_SRV:
        reslen = DNS_NAME_LENGTH(dname) + 6;
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
        result |= mdns_write_name(&outpkt->io, dname);
        if (result)
        {
            return result;
        }
        break;
    case DNS_RRTYPE_A:
        result = mdns_write_uint16(&outpkt->io, 4);
        result |= mdns_write_uint32(&outpkt->io, iface->ipv4addr);
        if (result)
        {
            return result;
        }
        break;
    case DNS_RRTYPE_AAAA:
    case DNS_RRTYPE_ANY:
    default:
        butil_log(5, "  RR type %d is ignored in PTR\n", type);
        return -1;
    }
    return 0;
}

static int mdns_answer_question(
                                    mdns_responder_t  *res,
                                    dns_domain_name_t *dname,
                                    uint16_t type,
                                    uint16_t clas,
                                    mdns_packet_t *outpkt
                                 )
{
    mdns_interface_t *iface;
    mdns_service_t *service;
    int cmp;
    bool added_srv;
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
    result = 0;
    added_srv = false;

    if (type == DNS_RRTYPE_ANY)
    {
        // just call the individual types by hand
        //
        result = mdns_answer_question(res, dname, DNS_RRTYPE_PTR, clas, outpkt);
        result |= mdns_answer_question(res, dname, DNS_RRTYPE_TXT, clas, outpkt);
        result |= mdns_answer_question(res, dname, DNS_RRTYPE_SRV, clas, outpkt);
        result |= mdns_answer_question(res, dname, DNS_RRTYPE_A, clas, outpkt);
        result |= mdns_answer_question(res, dname, DNS_RRTYPE_AAAA, clas, outpkt);
        return result;
    }
    // match against all the names we care about (host, ip, services)
    //
    if (
            type == DNS_RRTYPE_PTR
        ||  type == DNS_RRTYPE_A
        ||  type == DNS_RRTYPE_AAAA
    )
    {
        for (iface = res->interfaces; iface && ! result; iface = iface->next)
        {
            cmp = mdns_compare_names(&iface->hostname, dname);
            if (! cmp)
            {
                butil_log(4, "**==Matched iface %s\n", mdns_str_for_domain_name(dname));
                result = mdns_query_respond(res, outpkt, dname, type, clas, iface->ttl, iface, NULL, dname);
            }
        }
        for (service = res->services; service; service = service->next)
        {
            cmp = mdns_compare_names(&service->usr_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**==Matched usr service\n");
                outpkt->ancount++;
                result = mdns_query_respond(res, outpkt, dname, type, clas, service->ttl,
                                                     service->interface, service, &service->usr_domain_name);
            }
            cmp = mdns_compare_names(&service->srv_domain_name, dname);
            if (! cmp)
            {
                outpkt->ancount++;
                butil_log(4, "**==Matched srv service\n");
                result = mdns_query_respond(res, outpkt, dname, type, clas, service->ttl,
                                                     service->interface, service, &service->usr_domain_name);
            }
            cmp = mdns_compare_names(&service->sub_domain_name, dname);
            if (! cmp)
            {
                outpkt->ancount++;
                butil_log(4, "**==Matched sub service\n");
                result = mdns_query_respond(res, outpkt, dname, type, clas, service->ttl,
                                                     service->interface, service, &service->usr_domain_name);
            }
        }
    }
    if (type == DNS_RRTYPE_SRV || type == DNS_RRTYPE_ANY)
    {
        for (service = res->services; service; service = service->next)
        {
            cmp = mdns_compare_names(&service->usr_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**==Matched usr service\n");
                outpkt->ancount++;
                result = mdns_query_respond(res, outpkt, dname, type, clas, service->ttl,
                                                service->interface, service, &service->interface->hostname);
                added_srv = true;
            }
            cmp = mdns_compare_names(&service->srv_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**==Matched srv service\n");
                outpkt->ancount++;
                result = mdns_query_respond(res, outpkt, dname, type, clas, service->ttl,
                                                service->interface, service, &service->interface->hostname);
                added_srv = true;
            }
            cmp = mdns_compare_names(&service->sub_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**==Matched sub service\n");
                outpkt->ancount++;
                result = mdns_query_respond(res, outpkt, dname, type, clas, service->ttl,
                                                service->interface, service, &service->interface->hostname);
                added_srv = true;
            }
        }
    }
    if (type == DNS_RRTYPE_TXT || type == DNS_RRTYPE_ANY)
    {
        for (service = res->services; service; service = service->next)
        {
            cmp = mdns_compare_names(&service->usr_domain_name, dname);
            if (! cmp || type == DNS_RRTYPE_ANY)
            {
                butil_log(4, "**t=Matched usr service\n");
                outpkt->ancount++;
                result = mdns_query_respond(res, outpkt, dname, type, clas, service->ttl,
                                                service->interface, service, &service->txt_records);
            }
            cmp = mdns_compare_names(&service->srv_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**t=Matched srv service\n");
                outpkt->ancount++;
                result = mdns_query_respond(res, outpkt, dname, type, clas, service->ttl,
                                               service->interface, service, &service->txt_records);
            }
            cmp = mdns_compare_names(&service->sub_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**t=Matched sub service\n");
                outpkt->ancount++;
                result = mdns_query_respond(res, outpkt, dname, type, clas, service->ttl,
                                                service->interface, service, &service->txt_records);
            }
        }
    }
    if (added_srv)
    {
        // add address record if added a srv record to match service instance, but in additional section
        //
        for (service = res->services; service; service = service->next)
        {
            cmp = mdns_compare_names(&service->usr_domain_name, dname);
            if (! cmp || type == DNS_RRTYPE_ANY)
            {
                butil_log(4, "**S=Matched usr service\n");
                outpkt->arcount++;
                result = mdns_query_respond(res, outpkt, &service->interface->hostname,
                                                DNS_RRTYPE_A, clas, service->ttl,
                                                service->interface, service, &service->usr_domain_name);
            }
            cmp = mdns_compare_names(&service->srv_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**S=Matched srv service\n");
                outpkt->arcount++;
                result = mdns_query_respond(res, outpkt, &service->interface->hostname,
                                                DNS_RRTYPE_A, clas, service->ttl,
                                                service->interface, service, &service->usr_domain_name);
            }
            cmp = mdns_compare_names(&service->sub_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**S=Matched sub service\n");
                outpkt->arcount++;
                result = mdns_query_respond(res, outpkt, &service->interface->hostname,
                                                DNS_RRTYPE_A, clas, service->ttl,
                                                service->interface, service, &service->usr_domain_name);
            }
        }
    }
    return false;
}

static bool mdns_question_relates(
                                    mdns_responder_t  *res,
                                    dns_domain_name_t *dname,
                                    uint16_t type,
                                    uint16_t clas
                                 )
{
    mdns_interface_t *iface;
    mdns_service_t *service;
    int cmp;

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
    // match against all the names we care about (host, ip, services)
    //
    if (
            type == DNS_RRTYPE_PTR
        ||  type == DNS_RRTYPE_A
        ||  type == DNS_RRTYPE_AAAA
        ||  type == DNS_RRTYPE_ANY
    )
    {
        for (iface = res->interfaces; iface; iface = iface->next)
        {
            cmp = mdns_compare_names(&iface->hostname, dname);
            if (! cmp)
            {
                butil_log(4, "**Matched iface %s\n", mdns_str_for_domain_name(dname));
                return true;
            }
        }
    }
    if (
            type == DNS_RRTYPE_PTR
        ||  type == DNS_RRTYPE_SRV
        ||  type == DNS_RRTYPE_TXT
    )
    {
        for (service = res->services; service; service = service->next)
        {
            cmp = mdns_compare_names(&service->usr_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**Matched usr service\n");
                return true;
            }
            cmp = mdns_compare_names(&service->srv_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**Matched srv service\n");
                return true;
            }
            cmp = mdns_compare_names(&service->sub_domain_name, dname);
            if (! cmp)
            {
                butil_log(4, "**Matched sub service\n");
                return true;
            }
        }
    }
    return false;
}

int mdns_handle_input(mdns_responder_t *res, mdns_packet_t *inpkt)
{
    mdns_packet_t *outpkt;
    dns_domain_name_t domain_name;
    dns_domain_name_t *matched_name;
    char fqdn[MDNS_MAX_TEXT];
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

    mdns_dump_packet("RX", inpkt, 5);

    // restore tail to start of query
    inpkt->io.tail = query_start_tail;
    inpkt->io.count = inpkt->io.head - inpkt->io.tail;

    #if 1
    // silently ignore authoritative packets from random port
    //
    if (MDNS_FLAG_AA(inpkt->flags) && htons(inpkt->srcaddr.sin_port) != MDNS_PORT)
    {
        butil_log(8, "Ingore AA on port %u\n", (uint32_t)htons(inpkt->srcaddr.sin_port));
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

        we_care = mdns_question_relates(res, &domain_name, type, clas);
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
            }
            result = mdns_answer_question(res, &domain_name, type, clas, outpkt);
        }
        else
        {
            // a response
            //
        }
    }
    if (outpkt)
    {
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
            butil_log(5, "Made %d bytes for pkt %p\n", outpkt->io.count, outpkt);

            if (! res->outpkts)
            {
                res->outpkts = outpkt;
            }
            else
            {
                mdns_packet_t *pend;

                butil_log(1, "OUT PKTS not null\n");

                for (pend = res->outpkts; pend->next;)
                {
                    pend = pend->next;
                }
                pend->next = outpkt;
            }
            outpkt->next = NULL;
        }
    }
    return result;
}

int mdns_announce(mdns_responder_t *res)
{
    mdns_service_t *service;
    mdns_packet_t *outpkt;
    int result;

    for (service = res->services; service; service = service->next)
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

        result = mdns_answer_question(res, &service->usr_domain_name, DNS_RRTYPE_ANY, DNS_CLASS_IN, outpkt);
        if (result)
        {
            break;
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
            butil_log(5, "Made %d bytes for pkt %p\n", outpkt->io.count, outpkt);

            if (! res->outpkts)
            {
                res->outpkts = outpkt;
            }
            else
            {
                mdns_packet_t *pend;

                for (pend = res->outpkts; pend->next;)
                {
                    pend = pend->next;
                }
                pend->next = outpkt;
            }
            outpkt->next = NULL;
        }
    }
    return result;
}

int mdns_responder_slice(mdns_responder_t *res)
{
    mdns_packet_t *pkt;
    int result;

    switch (res->state)
    {
    case MDNS_PROBE_1:
    case MDNS_PROBE_2:
    case MDNS_PROBE_3:
        res->state = MDNS_ANNOUNCE_2;
        break;
    case MDNS_ANNOUNCE_1:
    case MDNS_ANNOUNCE_2:
        res->state = MDNS_RUN;
        result = mdns_announce(res);
        break;
    default:
        break;
    }
    // check output queue
    //
    if (res->outpkts)
    {
        pkt = res->outpkts;
        res->outpkts = pkt->next;

        result = mdns_handle_output(res, pkt);

        mdns_pkt_free(res, pkt);

        if (result)
        {
            return result;
        }
    }
    // check input socket
    //
    result = mdns_check_input(res);
    if (result)
    {
        return result;
    }
    // check input queue
    //
    if (res->inpkts)
    {
        pkt = res->inpkts;
        res->inpkts = pkt->next;

        result = mdns_handle_input(res, pkt);

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
        mdns_interface_t *iface;

        butil_log(5, "Starting MDNS with interfaces:\n");
        for (iface = res->interfaces; iface; iface = iface->next)
        {
            mdns_dump_interface(iface, 5);
        }
    }
    if (! res->services)
    {
        butil_log(1, "No services for responder\n");
        return -1;
    }
    else
    {
        mdns_service_t *service;

        butil_log(5, "Starting MDNS with services:\n");
        for (service = res->services; service; service = service->next)
        {
            mdns_dump_service(service, 5);
        }
    }
    do
    {
        result = mdns_responder_slice(res);
    }
    while (! result);
}

int mdns_responder_add_interface(mdns_responder_t *res, const char *hostname, uint32_t ipv4addr, uint32_t ttl)
{
    mdns_interface_t *iface;

    if (! res || ! hostname)
    {
        return -1;
    }
    iface = mdns_interface_alloc(res, hostname, ipv4addr);
    if (! iface)
    {
        butil_log(0, "Can't alloc interface for %s\n", hostname);
        return -1;
    }
    iface->ttl = ttl;
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
    char name[MDNS_MAX_TEXT];
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
    service->next = res->services;
    res->services = service;
    return 0;
}

int mdns_responder_init(mdns_responder_t *res)
{
    mdns_packet_t *pkt;
    struct ip_mreq mreq;
    int iparm;
    int count;
    int result;

    if (! res)
    {
        return -1;
    }
    memset(res, 0, sizeof(mdns_responder_t));

    res->in_sock = INVALID_SOCKET;
    res->out_sock = INVALID_SOCKET;

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

    // create a udp socket on port 5353 to listen
    //
    res->in_sock = iostream_create_udp_socket();
    if (res->in_sock == INVALID_SOCKET)
    {
        butil_log(0, "Can't make in socket\n");
        return -1;
    }
    result = iostream_bind_socket(res->in_sock, MDNS_PORT);
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

    result = setsockopt(res->in_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
    if (result < 0)
    {
        butil_log(0, "Can't join mcast group: %d\n", errno);
        return result;
    }
    // disable loopback for the socket, to avoid read loops
    //
    iparm = 0;
    result = setsockopt(res->in_sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&iparm, sizeof(iparm));
    if (result < 0)
    {
        butil_log(0, "Can't disable loopback: %d\n", errno);
        return result;
    }
    iparm = 1;
    result = setsockopt(res->in_sock, SOL_SOCKET, SO_BROADCAST, (char*)&iparm, sizeof(iparm));
    if (result < 0)
    {
        butil_log(0, "Can't enable bcast: %d\n", errno);
        return result;
    }
    // create probe records for any registered services
    // (todo)
    //

    // start out in probe state
    //
    res->state = MDNS_PROBE_1;

    return result;
}

int mdns_responder_deinit(mdns_responder_t *res)
{
    if (! res)
    {
        return -1;
    }
    if (res->in_sock != INVALID_SOCKET)
    {
        close_socket(res->in_sock);
    }
    if (res->out_sock != INVALID_SOCKET)
    {
        close_socket(res->out_sock);
    }
    if (res->pkt_pool)
    {
        free (res->pkt_pool);
    }
    return 0;
}

