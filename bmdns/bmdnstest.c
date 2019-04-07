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

int get_host_info(char* myhost, int nhost, bipv4addr_t *myipv4addr, bipv6addr_t *myipv6addr)
{
#ifdef Linux
    struct ifaddrs *addrs;
    struct ifaddrs *addr;
    struct sockaddr_in *sain4;
    struct sockaddr_in6 *sain6;
    bool gotv4;
    bool gotv6;

    gotv4 = false;
    gotv6 = false;

    if (gethostname(myhost, nhost))
    {
        BERROR("gethostname fails");
        return -1;
    }
    getifaddrs(&addrs);

    for (addr = addrs; addr && ! (gotv4 && gotv6); addr = addr->ifa_next)
    {
        if (addr->ifa_addr)
        {
            int family = addr->ifa_addr->sa_family;
            unsigned flags = addr->ifa_flags;

            // look only at interfaces up and not loopback
            //
            if ((flags & IFF_UP) && ! (flags & IFF_LOOPBACK))
            {
                if (family == AF_INET && ! gotv4)
                {
                    gotv4 = true;
                    sain4 = (struct sockaddr_in *)addr->ifa_addr;
                    myipv4addr->addr = sain4->sin_addr.s_addr;
                }
                else if (family == AF_INET6 && ! gotv6)
                {
                    gotv6 = true;
                    sain6 = (struct sockaddr_in6 *)addr->ifa_addr;
                    memcpy(myipv6addr->addr, &sain6->sin6_addr, sizeof(bipv6addr_t));
                }
            }
        }
    }
    freeifaddrs(addrs);

    if (! gotv4)
    {
        myipv4addr->addr = 0;
    }
    if (! gotv6)
    {
        memset(&myipv6addr->addr, 0, sizeof(bipv6addr_t));
    }
    if (! gotv4 && ! gotv6)
    {
        BERROR("no IP addr");
        return -1;
    }
#else
    struct hostent *phost;

    if (gethostname(myhost, nhost))
    {
        BERROR("gethostname fails");
        return -1;
    }
    phost = gethostbyname(myhost);
    if (! phost)
    {
        BERROR("gethostbyname fails");
        return -1;
    }
    memcpy(&myipv4addr->addr, phost->h_addr_list[0], 4);
#endif
    return 0;
}

static int usage (const char *program)
{
    fprintf(stderr, "Use: %s [-lu]\n", program);
    fprintf(stderr, "     -l    Set debug log level (default 1: errors/warnings only)\n");
    fprintf(stderr, "     -u    Run unit tests\n");
    fprintf(stderr, "     -U    Run unit tests and exit\n");
    return 1;
}

int main(int argc, char **argv)
{
    mdns_responder_t responder;
    char hostname[MDNS_MAX_DNTEXT];
    bipv4addr_t myipv4addr;
    bipv6addr_t myipv6addr;
    char *txtrecs;
    char *arg;
    char *program;
    uint32_t uval;
    int loglevel;
    bool unit_test;
    bool only_unit_test;
    int result;

#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        BERROR("WSAStartup failed");
        return -1;
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    loglevel = 6;
    butil_set_log_level(loglevel);
    unit_test = false;

    program = *argv++;
    argc--;

    while (argc > 0 && ! result)
    {
        arg = *argv;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 'l':
            {
                if (arg[2] == '\0')
                {
                    if (argc > 0)
                    {
                        argc--;
                        argv++;
                        uval = strtoul(*argv, NULL, 0);
                    }
                    else
                    {
                        return usage(program);
                    }
                }
                else
                {
                    uval = strtoul((arg + 2), NULL, 0);
                }
                if (arg[1] == 'l')
                {
                    loglevel = uval;
                }
                else
                {
                    return usage(program);
                }
                break;
            }
            case 'u':
                unit_test = true;
                break;
            case 'U':
                unit_test = true;
                only_unit_test = true;
                break;
            default:
                fprintf(stderr, "Bad Switch: %s\n", arg);
                break;
            }
            argc--;
            argv++;
        }
        else
        {
            return usage(program);
        }
    }
    butil_set_log_level(loglevel);

    // See "bonjour printing" https://developer.apple.com/bonjour/printing-specification/bonjourprinting-1.2.1.pdf
    //
    txtrecs =   "txtvers=1,"
                "adminurl=http://www.google.com,"
                "air=none,"
                "Binary=T,"
                "kind=document,"
                "note=Quail Run,"
                "pdl=application/postscript\\,image/jpeg\\,application/octet-stream,"
                "priority=50,"
                "qtotal=1,"
                "rp=ipp/print,"
                "TLS=1.2,"
                "ty=BNET IPP,"
                "product=BNET,"
                "Color=T,"
                "Copies=T,"
                "Duplex=F,"
                "PaperMax=legal-A4,"
                "Scan=F,"
                "Fax=F,"
                "UUID=68ECDD18-8319-42DD-BC73-87D192E900DD";

    result = mdns_responder_init(&responder);
    if (result)
    {
        return result;
    }
    get_host_info(hostname, sizeof(hostname), &myipv4addr, &myipv6addr);

    result = mdns_responder_add_interface(&responder, hostname, myipv4addr, myipv6addr, 30);
    if (result)
    {
        butil_log(0, "Can't add interface\n");
    }
    result = mdns_responder_add_service(
                                        &responder,
                                        responder.interfaces,
                                        "BNET PRT",
                                         "_ipp",
                                         MDNS_SRVPROTO_TCP,
                                         6310,
                                         txtrecs,
                                         10
                                       );
    if (result)
    {
        butil_log(0, "Can't add ipp service\n");
    }
    result = mdns_responder_add_service(
                                        &responder,
                                        responder.interfaces,
                                        "BNET PRT",
                                         "_ipps",
                                         MDNS_SRVPROTO_TCP,
                                         6311,
                                         txtrecs,
                                         10
                                       );
    if (result)
    {
        butil_log(0, "Can't add ipps service\n");
    }
    if (unit_test)
    {
        mdns_packet_t *pkt;

        pkt = mdns_pkt_alloc(&responder);
        if (! pkt)
        {
            butil_log(0, "Alloc failed\n");
            return -1;
        }
#if 1
        pkt->io.data = (uint8_t*)aop_packet;
        pkt->io.count = sizeof(aop_packet);
        pkt->io.tail = 0;
        pkt->io.head = pkt->io.count;

        result = mdns_handle_input(&responder, pkt);
        if (result)
        {
            butil_log(0, "Unit test: compressed names1  failed\n");
            mdns_pkt_free(&responder, pkt);
            return -1;
        }
#endif
#if 0
        pkt->io.data = (uint8_t*)drw_packet;
        pkt->io.count = sizeof(drw_packet);
        pkt->io.tail = 0;
        pkt->io.head = pkt->io.count;

        result = mdns_handle_input(&responder, pkt);
        if (result)
        {
            butil_log(0, "Unit test: compressed names 2 failed\n");
            mdns_pkt_free(&responder, pkt);
            return -1;
        }
#endif
#if 0
        pkt->io.data = (uint8_t*)bla_packet;
        pkt->io.count = sizeof(bla_packet);
        pkt->io.tail = 0;
        pkt->io.head = pkt->io.count;

        result = mdns_handle_input(&responder, pkt);
        if (result)
        {
            butil_log(0, "Unit test: compressed names 2 failed\n");
            mdns_pkt_free(&responder, pkt);
            return -1;
        }
#endif
        pkt->io.data = pkt->data;

        mdns_pkt_free(&responder, pkt);

        if (only_unit_test)
        {
            return 0;
        }
    }
    result = mdns_responder_run(&responder);
    if (result)
    {
        butil_log(0, "Responder Failed\n");
    }
    else
    {
        butil_log(2, "Responder Exits\n");
    }
    result = mdns_responder_deinit(&responder);
    return result;
}

