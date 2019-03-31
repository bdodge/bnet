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

int mdns_check_input(mdns_responder_t *res)
{
    mdns_packet_t *pkt;
    socklen_t srclen;
    int result;
    int count;

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
        butil_log(5, "RX %d from %s\n", count, str_for_sockaddr(&pkt->srcaddr));

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
    return 0;
}

int mdns_handle_output(mdns_responder_t *res)
{
    return 0;
}

int mdns_handle_input(mdns_responder_t *res)
{
    mdns_packet_t *pkt;
    int tail;

    if (! res || ! res->inpkts)
    {
        return 0;
    }
    // detach packet from list
    //
    pkt = res->inpkts;
    res->inpkts = pkt->next;
    pkt->next = NULL;

    tail = pkt->io.tail;

    mdns_read_uint16(&pkt->io, &pkt->id);
    mdns_read_uint16(&pkt->io, &pkt->flags);
    mdns_read_uint16(&pkt->io, &pkt->qdcount);
    mdns_read_uint16(&pkt->io, &pkt->ancount);
    mdns_read_uint16(&pkt->io, &pkt->nscount);
    mdns_read_uint16(&pkt->io, &pkt->arcount);

    pkt->io.tail = tail;

    mdns_dump_packet("RX", pkt, 5);

    // return to free pool
    //
    mdns_pkt_free(res, pkt);

    return 0;
}

int mdns_responder_slice(mdns_responder_t *res)
{
    int result;

    // check output q
    //
    if (res->outpkts)
    {
        result = mdns_handle_output(res);
        if (result)
        {
            return result;
        }
    }
    // check input
    //
    result = mdns_check_input(res);
    if (result)
    {
        return result;
    }
    // check input q
    //
    if (res->inpkts)
    {
        result = mdns_handle_input(res);
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

    do
    {
        result = mdns_responder_slice(res);
    }
    while (! result);
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
    if (result)
    {
        butil_log(0, "Can't join mcast group: %d\n", errno);
        return result;
    }
    iparm = 1;
    result = setsockopt(res->in_sock, SOL_SOCKET, SO_BROADCAST, (char*)&iparm, sizeof(iparm));
    if (result)
    {
        butil_log(0, "Can't enable bcast: %d\n", errno);
        return result;
    }
    // create probe records for any registered services
    // (todo)
    //
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

