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
#include "bmdnsunits.h"

const uint8_t aop_packet[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x5F, 0x61, 0x69,
    0x72, 0x70, 0x6F, 0x72, 0x74, 0x04, 0x5F, 0x74, 0x63, 0x70, 0x05, 0x6C, 0x6F, 0x63, 0x61, 0x6C,
    0x00, 0x00, 0x0C, 0x00, 0x01, 0x07, 0x5F, 0x72, 0x64, 0x6C, 0x69, 0x6E, 0x6B, 0xC0, 0x15, 0x00,
    0x0C, 0x00, 0x01, 0x0F, 0x5F, 0x70, 0x64, 0x6C, 0x2D, 0x64, 0x61, 0x74, 0x61, 0x73, 0x74, 0x72,
    0x65, 0x61, 0x6D, 0xC0, 0x15, 0x00, 0x0C, 0x00, 0x01, 0x08, 0x5F, 0x70, 0x72, 0x69, 0x6E, 0x74,
    0x65, 0x72, 0xC0, 0x15, 0x00, 0x0C, 0x00, 0x01, 0x04, 0x5F, 0x69, 0x70, 0x70, 0xC0, 0x15, 0x00,
    0x0C, 0x00, 0x01, 0x07, 0x5F, 0x69, 0x70, 0x70, 0x75, 0x73, 0x62, 0xC0, 0x15, 0x00, 0x0C, 0x00,
    0x01, 0x08, 0x5F, 0x73, 0x63, 0x61, 0x6E, 0x6E, 0x65, 0x72, 0xC0, 0x15, 0x00, 0x0C, 0x00, 0x01,
    0x07, 0x5F, 0x75, 0x73, 0x63, 0x61, 0x6E, 0x73, 0xC0, 0x15, 0x00, 0x0C, 0x00, 0x01, 0x06, 0x5F,
    0x75, 0x73, 0x63, 0x61, 0x6E, 0xC0, 0x15, 0x00, 0x0C, 0x00, 0x01, 0x05, 0x5F, 0x69, 0x70, 0x70,
    0x73, 0xC0, 0x15, 0x00, 0x0C, 0x00, 0x01, 0x04, 0x5F, 0x70, 0x74, 0x70, 0xC0, 0x15, 0x00, 0x0C,
    0x00, 0x01, 0x0B, 0x5F, 0x67, 0x6F, 0x6F, 0x67, 0x6C, 0x65, 0x63, 0x61, 0x73, 0x74, 0xC0, 0x15,
    0x00, 0x0C, 0x00, 0x01, 0x0D, 0x5F, 0x61, 0x70, 0x70, 0x6C, 0x65, 0x2D, 0x6D, 0x6F, 0x62, 0x64,
    0x65, 0x76, 0xC0, 0x15, 0x00, 0x0C, 0x00, 0x01, 0x08, 0x39, 0x30, 0x39, 0x35, 0x30, 0x37, 0x61,
    0x65, 0x04, 0x5F, 0x73, 0x75, 0x62, 0x0E, 0x5F, 0x61, 0x70, 0x70, 0x6C, 0x65, 0x2D, 0x6D, 0x6F,
    0x62, 0x64, 0x65, 0x76, 0x32, 0xC0, 0x15, 0x00, 0x0C, 0x00, 0x01, 0x0F, 0x5F, 0x61, 0x70, 0x70,
    0x6C, 0x65, 0x2D, 0x70, 0x61, 0x69, 0x72, 0x61, 0x62, 0x6C, 0x65, 0xC0, 0x15, 0x00, 0x0C, 0x00,
    0x01, 0x00, 0x00, 0x29, 0x05, 0xA0, 0x00, 0x00, 0x11, 0x94, 0x00, 0x0C, 0x00, 0x04, 0x00, 0x08,
    0x00, 0x4E, 0x28, 0xCF, 0xE9, 0x15, 0xEB, 0x59
};

const uint8_t drw_packet[] =
{
    0x00, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x09, 0x44, 0x6F, 0x63,
    0x74, 0x6F, 0x72, 0x57, 0x68, 0x6F, 0x0B, 0x5F, 0x61, 0x66, 0x70, 0x6F, 0x76, 0x65, 0x72, 0x74,
    0x63, 0x70, 0x04, 0x5F, 0x74, 0x63, 0x70, 0x05, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x00, 0x00, 0x21,
    0x80, 0x01, 0x00, 0x00, 0x00, 0x78, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x02, 0x24, 0x09, 0x44,
    0x6F, 0x63, 0x74, 0x6F, 0x72, 0x57, 0x68, 0x6F, 0xC0, 0x27, 0xC0, 0x3E, 0x00, 0x1C, 0x80, 0x01,
    0x00, 0x00, 0x00, 0x78, 0x00, 0x10, 0xFE, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0xEA,
    0x96, 0xFF, 0xFE, 0xE9, 0x1E, 0x13, 0xC0, 0x3E, 0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78,
    0x00, 0x04, 0xC0, 0xA8, 0x01, 0x98, 0xC0, 0x3E, 0x00, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78,
    0x00, 0x04, 0xA9, 0xFE, 0x03, 0x44, 0xC0, 0x3E, 0x00, 0x2F, 0x80, 0x01, 0x00, 0x00, 0x00, 0x78,
    0x00, 0x08, 0xC0, 0x3E, 0x00, 0x04, 0x40, 0x00, 0x00, 0x08, 0xC0, 0x0C, 0x00, 0x2F, 0x80, 0x01,
    0x00, 0x00, 0x00, 0x78, 0x00, 0x09, 0xC0, 0x0C, 0x00, 0x05, 0x00, 0x00, 0x80, 0x00, 0x40,
};

const uint8_t bla_packet[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x42, 0x6C, 0x61,
    0x63, 0x6B, 0x41, 0x72, 0x6D, 0x6F, 0x72, 0x32, 0x32, 0x30, 0x05, 0x6C, 0x6F, 0x63, 0x61, 0x6C,
    0x00, 0x00, 0x01, 0x00, 0x01, 0x0D, 0x42, 0x6C, 0x61, 0x63, 0x6B, 0x41, 0x72, 0x6D, 0x6F, 0x72,
    0x32, 0x32, 0x30, 0x04, 0x5F, 0x73, 0x6D, 0x62, 0x04, 0x5F, 0x74, 0x63, 0x70, 0xC0, 0x1A, 0x00,
    0x10, 0x00, 0x01, 0xC0, 0x25, 0x00, 0x21, 0x00, 0x01
};

int mdns_test_read_packet(mdns_responder_t *res)
{
    dns_domain_name_t name;
    mdns_packet_t  packet;
    mdns_packet_t *pkt;
    int result;

    pkt = &packet;
    memset(pkt, 0, sizeof(packet));

#if 1
    pkt->io.tail = 0;
    pkt->io.data = (uint8_t*)aop_packet;
    pkt->io.count = sizeof(aop_packet);
    pkt->io.size = pkt->io.count;
    pkt->io.head = pkt->io.count;

    result = mdns_handle_input(res, res->interfaces, pkt);
    if (result)
    {
        butil_log(0, "Unit test: compressed names 1  failed\n");
        return -1;
    }
#endif
#if 1
    pkt->io.tail = 0;
    pkt->io.data = (uint8_t*)drw_packet;
    pkt->io.count = sizeof(drw_packet);
    pkt->io.size = pkt->io.count;
    pkt->io.head = pkt->io.count;

    result = mdns_handle_input(res, res->interfaces, pkt);
    if (result)
    {
        butil_log(0, "Unit test: compressed names 2 failed\n");
        return -1;
    }
#endif
#if 1
    pkt->io.tail = 0;
    pkt->io.data = (uint8_t*)bla_packet;
    pkt->io.count = sizeof(bla_packet);
    pkt->io.size = pkt->io.count;
    pkt->io.head = pkt->io.count;

    result = mdns_handle_input(res, res->interfaces, pkt);
    if (result)
    {
        butil_log(0, "Unit test: compressed names 3 failed\n");
        return -1;
    }
#endif
    return 0;
}

int mdns_basic_unit_test(mdns_responder_t *res)
{
    dns_domain_name_t dname1;
    dns_domain_name_t dname2;
    mdns_packet_t  packet;
    mdns_packet_t *pkt;
    ioring_t io;
    const char *name1;
    const char *name2;
    const char *expected;
    int name1_off;
    int name2_off;
    int i;
    char buffer[MDNS_MAX_DNTEXT];
    int result;

    pkt = &packet;
    memset(pkt, 0, sizeof(packet));

    // test name conversion in and out of domain name
    //
    name1 = "ABCD.EFGH.IJKL.MNOP.qrst.uvwx.yz";
    name2 = "abcd.EFGH.IjkL.MNOP.qrst.uvwx.yz";

    result = mdns_unflatten_name(name1, &dname1);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name1);
        return result;
    }
    result = mdns_flatten_name(&dname1, buffer, sizeof(buffer));
    if (result)
    {
        butil_log(0, "Failed to flatten %s\n", name1);
        return result;
    }
    if (strcmp(name1, buffer))
    {
        butil_log(0, "re-flattened name %s is not %s\n",
                            buffer, name1);
        return -1;
    }
    // check mixed case names compare
    //
    if (mdns_compare_nametext(name1, name2, strlen(name1)))
    {
        butil_log(0, "expected case insenstive compare of %s and %s\n",
                            name1, name2);
        return -1;
    }
    // check packet name compression with sub-name
    //
    name1 = "AAAA.BBBB.CCCC.DDDD";
    name2 = "BBBB.CCCC.DDDD";

    result = mdns_unflatten_name(name1, &dname1);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name1);
        return result;
    }
    result = mdns_unflatten_name(name2, &dname2);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name2);
        return result;
    }
    io.head = MDNS_OFF_QUESTION;
    io.count = io.head;
    io.tail = io.head;
    io.size = sizeof(buffer);
    io.data = (uint8_t *)buffer;

    name1_off = io.head;
    result = mdns_write_name(&io, &dname1);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name1);
        return result;
    }
    name2_off = io.head;
    result = mdns_write_name(&io, &dname2);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name2);
        return result;
    }
    expected = "\4AAAA\4BBBB\4CCCC\4DDDD\0\300\21\0";

    if (memcmp(io.data + name1_off, expected, 24))
    {
        butil_log(0, "Failed to compress name %s\n", name2);
        for (i = 0; i < 24; i++)
        {
            butil_log(1, "%02X-%02X ",
                    io.data[i + name1_off], (uint8_t)expected[i]);
        }
        return -1;
    }
    // read the name back and ensure it matches
    //
    result = mdns_read_name(&io, &dname1);
    if (result)
    {
        butil_log(0, "Coudn't re-read name1\n");
        return result;
    }
    result = mdns_read_name(&io, &dname2);
    if (result)
    {
        butil_log(0, "Coudn't re-read name2 (compressed)\n");
        return result;
    }
    result = mdns_flatten_name(&dname2, buffer, sizeof(buffer));
    if (result)
    {
        butil_log(0, "Coudn't unflatten name2 (compressed)\n");
        return result;
    }
    if (mdns_compare_nametext(name2, buffer, strlen(name2)))
    {
        butil_log(0, "Expected compressed/uncompressed to be %s but is %s\n",
                                name2, buffer);
        return -1;
    }
    // test compressing entire name
    //
    name1 = "ABCD.EFGH.IJKL.MNOP.qrst.uvwx.yz";
    name2 = "abcd.EFGH.IjkL.MNOP.qrst.uvwx.yz";

    result = mdns_unflatten_name(name1, &dname1);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name1);
        return result;
    }
    result = mdns_unflatten_name(name2, &dname2);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name2);
        return result;
    }
    io.head = MDNS_OFF_QUESTION;
    io.count = io.head;
    io.tail = io.head;
    io.size = sizeof(buffer);
    io.data = (uint8_t *)buffer;

    name1_off = io.head;
    result = mdns_write_name(&io, &dname1);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name1);
        return result;
    }
    name2_off = io.head;
    result = mdns_write_name(&io, &dname2);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name2);
        return result;
    }
    expected = "\300\14\0";

    if (memcmp(io.data + name2_off, expected, 3))
    {
        butil_log(0, "Failed to compress name %s\n", name2);
        for (i = 0; i < 3; i++)
        {
            butil_log(1, "%02X-%02X ",
                    io.data[i + name2_off], (uint8_t)expected[i]);
        }
        return -1;
    }
    // test compressing begining of name
    //
    name1 = "ABCD.EFGH.IJKL.MNOP.qrst.uvwx.yz";
    name2 = "abcd.EFGH.IjkL.MNOP.qrst.uvwx.yz.THIS.IS.A.TEST";

    result = mdns_unflatten_name(name1, &dname1);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name1);
        return result;
    }
    result = mdns_unflatten_name(name2, &dname2);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name2);
        return result;
    }
    io.head = MDNS_OFF_QUESTION;
    io.count = io.head;
    io.tail = io.head;
    io.size = sizeof(buffer);
    io.data = (uint8_t *)buffer;

    name1_off = io.head;
    result = mdns_write_name(&io, &dname1);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name1);
        return result;
    }
    name2_off = io.head;
    result = mdns_write_name(&io, &dname2);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name2);
        return result;
    }
    expected = "\300\14\4THIS\2IS\1A\4TEST\0";

    if (memcmp(io.data + name2_off, expected, 18))
    {
        butil_log(0, "Failed to compress name %s\n", name2);
        for (i = 0; i < 18; i++)
        {
            butil_log(1, "%02X-%02X ",
                    io.data[i + name2_off], (uint8_t)expected[i]);
        }
        return -1;
    }
    // test compressing Middle of name
    //
    name1 = "ABCD.EFGH.IJKL.MNOP.qrst.uvwx.yz";
    name2 = "AAAA.BBBB.qrst.uvwx.yz.THIS.IS.A.TEST";

    result = mdns_unflatten_name(name1, &dname1);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name1);
        return result;
    }
    result = mdns_unflatten_name(name2, &dname2);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name2);
        return result;
    }
    io.head = MDNS_OFF_QUESTION;
    io.count = io.head;
    io.tail = io.head;
    io.size = sizeof(buffer);
    io.data = (uint8_t *)buffer;

    name1_off = io.head;
    result = mdns_write_name(&io, &dname1);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name1);
        return result;
    }
    name2_off = io.head;
    result = mdns_write_name(&io, &dname2);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name2);
        return result;
    }
    expected = "\4AAAA\4BBBB\300\40\4THIS\2IS\1A\4TEST\0";

    if (memcmp(io.data + name2_off, expected, 18))
    {
        butil_log(0, "Failed to compress name %s\n", name2);
        for (i = 0; i < 18; i++)
        {
            butil_log(1, "%02X-%02X ",
                    io.data[i + name2_off], (uint8_t)expected[i]);
        }
        return -1;
    }
    // test compressing from multiple different names
    //
    name2 = "AAAA.BBBB.CCCC.DDDD.EEEE.FFFF";
    result = mdns_unflatten_name(name2, &dname2);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name2);
        return result;
    }
    io.head = MDNS_OFF_QUESTION;
    io.count = io.head;
    io.tail = io.head;
    io.size = sizeof(buffer);
    io.data = (uint8_t *)buffer;

    name1_off = io.head;

    name1 = "AAAA.BBBB";
    result = mdns_unflatten_name(name1, &dname1);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name1);
        return result;
    }
    result = mdns_write_name(&io, &dname1);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name1);
        return result;
    }
    name1 = "CCCC.DDDD";
    result = mdns_unflatten_name(name1, &dname1);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name1);
        return result;
    }
    result = mdns_write_name(&io, &dname1);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name1);
        return result;
    }
    name1 = "EEEE.FFFF";
    result = mdns_unflatten_name(name1, &dname1);
    if (result)
    {
        butil_log(0, "Failed to unflatten %s\n", name1);
        return result;
    }
    result = mdns_write_name(&io, &dname1);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name1);
        return result;
    }
    name2_off = io.head;
    result = mdns_write_name(&io, &dname2);
    if (result)
    {
        butil_log(0, "Failed to write name %s\n", name2);
        return result;
    }
    expected = "\300\14\300\27\300\42\0";

    if (memcmp(io.data + name2_off, expected, 7))
    {
        butil_log(0, "Failed to compress name %s\n", name2);
        for (i = 0; i < 7; i++)
        {
            butil_log(1, "%02X-%02X ",
                    io.data[i + name2_off], (uint8_t)expected[i]);
        }
        return -1;
    }
    return 0;
}

int mdns_test_iface_rr_type(mdns_responder_t *res, uint16_t rectype)
{
    mdns_interface_t *iface;
    dns_domain_name_t domain_name;
    mdns_packet_t *pkt;
    mdns_packet_t *outpkt;
    int result;

    if (! res || ! res->interfaces)
    {
        return -1;
    }
    iface = res->interfaces;

    pkt = mdns_pkt_alloc(res);
    if (! pkt)
    {
        butil_log(0, "Can't alloc QR pkt\n");
        return -1;
    }
    // pretend to be running, and clear any pending packets
    //
    iface->state = MDNS_RUN;
    while (iface->outpkts)
    {
        outpkt = iface->outpkts;
        iface->outpkts = outpkt->next;
        mdns_pkt_free(res, outpkt);
    }
    outpkt = NULL;

    do // try
    {
        uint16_t type;
        uint16_t clas;
        uint16_t reslen;
        uint16_t addr16;
        uint32_t addr;

        // build a query for type of record of interface hostname
        //
        result = mdns_ask_question(iface, &iface->hostname, rectype, DNS_CLASS_IN, pkt);
        if (result)
        {
            butil_log(0, "Can't build QR\n");
            break;
        }
        // process this packet
        //
        result = mdns_handle_input(res, iface, pkt);
        if (result)
        {
            butil_log(0, "Can't handle QR\n");
            break;
        }
        if (rectype == DNS_RRTYPE_SRV)
        {
            // we don't expect to query the hostname and get a service record
            // so complain if we do
            //
            if (iface->outpkts)
            {
                butil_log(0, "Did NOT expect a SRV on hostname to build reply\n");
                result = -1;
                break;
            }
            // test over for SRV
            result = 0;
            break;
        }
        // expect an output packet
        //
        if (! iface->outpkts)
        {
            butil_log(0, "Expected an output packet\n");
            result = -1;
            break;
        }
        outpkt = iface->outpkts;
        iface->outpkts = NULL;

        // expect it to have one answer, of right type
        //
        if (
                outpkt->qdcount != 0
            ||  outpkt->arcount != 0
            ||  outpkt->nscount != 0
            ||  outpkt->ancount != 1
        )
        {
            butil_log(0, "Expected one answer and no query\n");
            mdns_dump_packet("Failed packet", outpkt, 1);
            result = -1;
            break;
        }
        // read the name
        //
        outpkt->io.tail = MDNS_OFF_QUESTION;

        result = mdns_read_name(&outpkt->io, &domain_name);
        if (result)
        {
            break;
        }
        // read class and type
        //
        result = mdns_read_uint16(&outpkt->io, &type);
        if (result)
        {
            break;
        }
        result = mdns_read_uint16(&outpkt->io, &clas);
        if (result)
        {
            break;
        }
        clas &= ~(1 << 15);

        // read TTL
        //
        result = mdns_read_uint32(&outpkt->io, &outpkt->ttl);
        if (result)
        {
            break;
        }
        // read resource length
        //
        result = mdns_read_uint16(&outpkt->io, &reslen);
        if (result)
        {
            break;
        }
        if (outpkt->io.count < reslen)
        {
            butil_log(2, "Truncated Resource\n");
            break;
        }
        if (
                type != rectype
            ||  clas != DNS_CLASS_IN
        )
        {
            butil_log(0, "Expected an type %u record class IN packet, got type %u class %u\n",
                                    (uint32_t)rectype, (uint32_t)type, (uint32_t)clas);
            result = -1;
            break;
        }
        switch (rectype)
        {
        case DNS_RRTYPE_PTR:
            // ptr for host name is just host name
            result = mdns_read_name(&outpkt->io, &domain_name);
            if (result)
            {
                butil_log(0, "Can't read name from ptr\n");
                result = -1;
                break;
            }
            if (mdns_compare_names(&iface->hostname, &domain_name))
            {
                butil_log(0, "Expected PTR to have our hostname\n");
                result = -1;
                break;
            }
            break;

        case DNS_RRTYPE_A:
            if (reslen != 4)
            {
                butil_log(0, "Expected an IPv4 address resource\n");
                result = -1;
                break;
            }
            result = mdns_read_uint32(&outpkt->io, &addr);
            if (htonl(addr) != iface->ipv4addr.addr)
            {
                butil_log(0, "Expected an IPv4 address resource, got %d bytes\n", reslen);
                result = -1;
                break;
            }
            break;
        case DNS_RRTYPE_AAAA:
            if (reslen != 16)
            {
                butil_log(0, "Expected an IPv6 address resource, got %d bytes\n", reslen);
                result = -1;
                break;
            }
            for (reslen = 0; reslen < 8; reslen++)
            {
                result = mdns_read_uint16(&outpkt->io, &addr16);
                addr16 = htons(addr16);
                if (addr16 != iface->ipv6addr.addr[reslen])
                {
                    butil_log(0, "Expected our own IPv6 address %04X, got %04X at off %d\n",
                                       iface->ipv6addr.addr[reslen], addr16, reslen);
                    result = -1;
                    break;
                }
            }
            break;
        default:
            butil_log(0, "Unimplemented RR type %d\n", rectype);
            result = -1;
            break;
        }
    }
    while (0); //catch

    if (pkt)
    {
        mdns_pkt_free(res, pkt);
    }
    if (outpkt)
    {
        mdns_pkt_free(res, outpkt);
    }
    return result;
}

int mdns_test_service_rr_type(mdns_responder_t *res, uint16_t rectype)
{
    mdns_interface_t *iface;
    mdns_service_t *service;
    dns_domain_name_t domain_name;
    mdns_packet_t *pkt;
    mdns_packet_t *outpkt;
    int result;

    if (! res || ! res->interfaces)
    {
        butil_log(0, "No interface\n");
        return -1;
    }
    iface = res->interfaces;

    if (! iface->services)
    {
        butil_log(0, "No service\n");
        return -1;
    }
    for (service = iface->services; service; service = service->next)
    {
        pkt = mdns_pkt_alloc(res);
        if (! pkt)
        {
            butil_log(0, "Can't alloc QR pkt\n");
            return -1;
        }
        // pretend to be running, and clear any pending packets
        //
        iface->state = MDNS_RUN;
        while (iface->outpkts)
        {
            outpkt = iface->outpkts;
            iface->outpkts = outpkt->next;
            mdns_pkt_free(res, outpkt);
        }
        outpkt = NULL;

        do // try
        {
            uint16_t type;
            uint16_t clas;
            uint16_t reslen;
            uint16_t addr16;
            uint32_t addr;

            // build a query for type of record of service instance name
            //
            result = mdns_ask_question(iface, &service->usr_domain_name, rectype, DNS_CLASS_IN, pkt);
            if (result)
            {
                butil_log(0, "Can't build QR\n");
                break;
            }
            // process this packet
            //
            result = mdns_handle_input(res, iface, pkt);
            if (result)
            {
                butil_log(0, "Can't handle QR\n");
                break;
            }
            // For A and AAAA types on a service we don't output
            // TODO - should we?
            //
            if (rectype == DNS_RRTYPE_A || rectype == DNS_RRTYPE_AAAA)
            {
                if (iface->outpkts)
                {
                    butil_log(0, "Didn't expect service to respond to type %d\n", rectype);
                    result = -1;
                    break;
                }
                result = 0;
                break;
            }
            // expect an output packet
            //
            if (! iface->outpkts)
            {
                butil_log(0, "Expected an output packet\n");
                result = -1;
                break;
            }
            outpkt = iface->outpkts;
            iface->outpkts = NULL;

            // expect it to have one answer, of right type
            //
            if (
                    outpkt->qdcount != 0
                ||  outpkt->arcount != (rectype == DNS_RRTYPE_SRV) ? 1 : 0
                ||  outpkt->nscount != 0
                ||  outpkt->ancount != 1
            )
            {
                butil_log(0, "Expected one answer and no query\n");
                mdns_dump_packet("Failed packet", outpkt, 1);
                result = -1;
                break;
            }
            // read the name
            //
            outpkt->io.tail = MDNS_OFF_QUESTION;

            result = mdns_read_name(&outpkt->io, &domain_name);
            if (result)
            {
                break;
            }
            // read class and type
            //
            result = mdns_read_uint16(&outpkt->io, &type);
            if (result)
            {
                break;
            }
            result = mdns_read_uint16(&outpkt->io, &clas);
            if (result)
            {
                break;
            }
            clas &= ~(1 << 15);

            // read TTL
            //
            result = mdns_read_uint32(&outpkt->io, &outpkt->ttl);
            if (result)
            {
                break;
            }
            // read resource length
            //
            result = mdns_read_uint16(&outpkt->io, &reslen);
            if (result)
            {
                break;
            }
            if (outpkt->io.count < reslen)
            {
                butil_log(2, "Truncated Resource\n");
                break;
            }
            if (
                    type != rectype
                ||  clas != DNS_CLASS_IN
            )
            {
                butil_log(0, "Expected an type %u record class IN packet, got type %u class %u\n",
                                        (uint32_t)rectype, (uint32_t)type, (uint32_t)clas);
                result = -1;
                break;
            }
            switch (rectype)
            {
            case DNS_RRTYPE_PTR:
                // ptr for host name is just host name
                result = mdns_read_name(&outpkt->io, &domain_name);
                if (result)
                {
                    butil_log(0, "Can't read name from ptr\n");
                    result = -1;
                    break;
                }
                if (mdns_compare_names(&service->usr_domain_name, &domain_name))
                {
                    butil_log(0, "Expected PTR to have our hostname\n");
                    result = -1;
                    break;
                }
                break;

            case DNS_RRTYPE_A:
                if (reslen != 4)
                {
                    butil_log(0, "Expected an IPv4 address resource\n");
                    result = -1;
                    break;
                }
                result = mdns_read_uint32(&outpkt->io, &addr);
                if (htonl(addr) != iface->ipv4addr.addr)
                {
                    butil_log(0, "Expected an IPv4 address resource, got %d bytes\n", reslen);
                    result = -1;
                    break;
                }
                break;
            case DNS_RRTYPE_AAAA:
                if (reslen != 16)
                {
                    butil_log(0, "Expected an IPv6 address resource, got %d bytes\n", reslen);
                    result = -1;
                    break;
                }
                for (reslen = 0; reslen < 8; reslen++)
                {
                    result = mdns_read_uint16(&outpkt->io, &addr16);
                    addr16 = htons(addr16);
                    if (addr16 != iface->ipv6addr.addr[reslen])
                    {
                        butil_log(0, "Expected our own IPv6 address %04X, got %04X at off %d\n",
                                           iface->ipv6addr.addr[reslen], addr16, reslen);
                        result = -1;
                        break;
                    }
                }
                break;
            case DNS_RRTYPE_SRV:
                outpkt->io.tail += 6;
                outpkt->io.count -= 6;
                result = mdns_read_name(&outpkt->io, &domain_name);

                if (mdns_compare_names(
                                        &iface->hostname,
                                        &domain_name
                                        )
                )
                {
                    butil_log(0, "Expected service instance resource to be hostname\n");
                    butil_log(0, "res=%s\n", outpkt->io.data + outpkt->io.tail);
                    result = -1;
                    break;
                }
                break;
            default:
                butil_log(0, "Unimplemented RR type %d\n", rectype);
                result = -1;
                break;
            }
        }
        while (0); //catch

        if (pkt)
        {
            mdns_pkt_free(res, pkt);
        }
        if (outpkt)
        {
            mdns_pkt_free(res, outpkt);
        }
    }
    return result;
}

int mdns_test_service_known_answer(mdns_responder_t *res, uint32_t ttl)
{
    mdns_interface_t *iface;
    mdns_service_t *service;
    dns_domain_name_t domain_name;
    mdns_packet_t *pkt;
    mdns_packet_t *outpkt;
    uint16_t rectype = DNS_RRTYPE_SRV;
    int result;

    if (! res || ! res->interfaces)
    {
        butil_log(0, "No interface\n");
        return -1;
    }
    iface = res->interfaces;

    if (! iface->services)
    {
        butil_log(0, "No service\n");
        return -1;
    }
    for (service = iface->services; service; service = service->next)
    {
        pkt = mdns_pkt_alloc(res);
        if (! pkt)
        {
            butil_log(0, "Can't alloc QR pkt\n");
            return -1;
        }
        // pretend to be running, and clear any pending packets
        //
        iface->state = MDNS_RUN;
        while (iface->outpkts)
        {
            outpkt = iface->outpkts;
            iface->outpkts = outpkt->next;
            mdns_pkt_free(res, outpkt);
        }
        outpkt = NULL;

        do // try
        {
            uint16_t type;
            uint16_t clas;
            uint16_t reslen;
            uint16_t addr16;
            uint32_t addr;

            // build a query for type of record of service instance name
            //
            result = mdns_ask_question(iface, &service->usr_domain_name, rectype, DNS_CLASS_IN, pkt);
            if (result)
            {
                butil_log(0, "Can't build QR\n");
                break;
            }
            // answer the question ourselves
            //
            result = mdns_answer_question(iface, &service->usr_domain_name, rectype, DNS_CLASS_IN, NULL, 0, pkt);
            if (result)
            {
                butil_log(0, "Can't add answer\n");
                break;
            }
            // process this packet
            //
            result = mdns_handle_input(res, iface, pkt);
            if (result)
            {
                butil_log(0, "Can't handle QR\n");
                break;
            }
            // For A and AAAA types on a service we don't output
            // TODO - should we?
            //
            if (rectype == DNS_RRTYPE_A || rectype == DNS_RRTYPE_AAAA)
            {
                if (iface->outpkts)
                {
                    butil_log(0, "Didn't expect service to respond to type %d\n", rectype);
                    result = -1;
                    break;
                }
                result = 0;
                break;
            }
            // expect an output packet
            //
            if (! iface->outpkts)
            {
                butil_log(0, "Expected an output packet\n");
                result = -1;
                break;
            }
            outpkt = iface->outpkts;
            iface->outpkts = NULL;

            // expect it to have one answer, of right type
            //
            if (
                    outpkt->qdcount != 0
                ||  outpkt->arcount > 3
                ||  outpkt->nscount != 0
                ||  outpkt->ancount != 1
            )
            {
                butil_log(0, "Expected one answer and no query\n");
                mdns_dump_packet("Failed packet", outpkt, 1);
                result = -1;
                break;
            }
            // read the name
            //
            outpkt->io.tail = MDNS_OFF_QUESTION;

            result = mdns_read_name(&outpkt->io, &domain_name);
            if (result)
            {
                break;
            }
            // read class and type
            //
            result = mdns_read_uint16(&outpkt->io, &type);
            if (result)
            {
                break;
            }
            result = mdns_read_uint16(&outpkt->io, &clas);
            if (result)
            {
                break;
            }
            clas &= ~(1 << 15);

            // read TTL
            //
            result = mdns_read_uint32(&outpkt->io, &outpkt->ttl);
            if (result)
            {
                break;
            }
            // read resource length
            //
            result = mdns_read_uint16(&outpkt->io, &reslen);
            if (result)
            {
                break;
            }
            if (outpkt->io.count < reslen)
            {
                butil_log(2, "Truncated Resource\n");
                break;
            }
            if (
                    type != rectype
                ||  clas != DNS_CLASS_IN
            )
            {
                butil_log(0, "Expected an type %u record class IN packet, got type %u class %u\n",
                                        (uint32_t)rectype, (uint32_t)type, (uint32_t)clas);
                result = -1;
                break;
            }
            switch (rectype)
            {
            case DNS_RRTYPE_PTR:
                // ptr for host name is just host name
                result = mdns_read_name(&outpkt->io, &domain_name);
                if (result)
                {
                    butil_log(0, "Can't read name from ptr\n");
                    result = -1;
                    break;
                }
                if (mdns_compare_names(&service->usr_domain_name, &domain_name))
                {
                    butil_log(0, "Expected PTR to have our hostname\n");
                    result = -1;
                    break;
                }
                break;

            case DNS_RRTYPE_A:
                if (reslen != 4)
                {
                    butil_log(0, "Expected an IPv4 address resource\n");
                    result = -1;
                    break;
                }
                result = mdns_read_uint32(&outpkt->io, &addr);
                if (htonl(addr) != iface->ipv4addr.addr)
                {
                    butil_log(0, "Expected an IPv4 address resource, got %d bytes\n", reslen);
                    result = -1;
                    break;
                }
                break;
            case DNS_RRTYPE_AAAA:
                if (reslen != 16)
                {
                    butil_log(0, "Expected an IPv6 address resource, got %d bytes\n", reslen);
                    result = -1;
                    break;
                }
                for (reslen = 0; reslen < 8; reslen++)
                {
                    result = mdns_read_uint16(&outpkt->io, &addr16);
                    addr16 = htons(addr16);
                    if (addr16 != iface->ipv6addr.addr[reslen])
                    {
                        butil_log(0, "Expected our own IPv6 address %04X, got %04X at off %d\n",
                                           iface->ipv6addr.addr[reslen], addr16, reslen);
                        result = -1;
                        break;
                    }
                }
                break;
            case DNS_RRTYPE_SRV:
                outpkt->io.tail += 6;
                outpkt->io.count -= 6;
                result = mdns_read_name(&outpkt->io, &domain_name);

                if (mdns_compare_names(
                                        &iface->hostname,
                                        &domain_name
                                        )
                )
                {
                    butil_log(0, "Expected service instance resource to be hostname\n");
                    butil_log(0, "res=%s\n", outpkt->io.data + outpkt->io.tail);
                    result = -1;
                    break;
                }
                break;
            default:
                butil_log(0, "Unimplemented RR type %d\n", rectype);
                result = -1;
                break;
            }
        }
        while (0); //catch

        if (pkt)
        {
            mdns_pkt_free(res, pkt);
        }
        if (outpkt)
        {
            mdns_pkt_free(res, outpkt);
        }
    }
    return result;
}

int mdns_unit_test(mdns_responder_t *res)
{
    int result;

#if 0
    result = mdns_test_read_packet(res);
    if (result)
    {
        return result;
    }
    // Test Query for interface hostname
    //
    result = mdns_test_iface_rr_type(res, DNS_RRTYPE_A);
    if (result)
    {
        return result;
    }
    result = mdns_test_iface_rr_type(res, DNS_RRTYPE_AAAA);
    if (result)
    {
        return result;
    }
    result = mdns_test_iface_rr_type(res, DNS_RRTYPE_PTR);
    if (result)
    {
        return result;
    }
    result = mdns_test_iface_rr_type(res, DNS_RRTYPE_SRV);
    if (result)
    {
        return result;
    }
    // Test Query for SRV Instance
    //
    result = mdns_test_service_rr_type(res, DNS_RRTYPE_A);
    if (result)
    {
        return result;
    }
    result = mdns_test_service_rr_type(res, DNS_RRTYPE_AAAA);
    if (result)
    {
        return result;
    }
    result = mdns_test_service_rr_type(res, DNS_RRTYPE_PTR);
    if (result)
    {
        return result;
    }
    result = mdns_test_service_rr_type(res, DNS_RRTYPE_SRV);
    if (result)
    {
        return result;
    }
#endif
    // Test Query with known answers
    //
    result = mdns_test_service_known_answer(res, 5000);
    if (result)
    {
        return result;
    }
    return 0;
}

