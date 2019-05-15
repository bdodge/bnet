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

int get_host_info(char* myhost, int nhost, bipv4addr_t *myipv4addr, bipv6addr_t *myipv6addr, int *iface_index)
{
#if defined(Linux) || defined(OSX)
    struct ifaddrs *addrs;
    struct ifaddrs *addr;
    struct sockaddr_in *sain4;
    struct sockaddr_in6 *sain6;
    bool gotv4;
    bool gotv6;

    gotv4 = false;
    gotv6 = false;

    *iface_index = 0;

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
                    butil_log(4, "IPv4 on iface %d  %s\n", if_nametoindex(addr->ifa_name), addr->ifa_name);
                }
                else if (family == AF_INET6 && ! gotv6)
                {
                    gotv6 = true;
                    sain6 = (struct sockaddr_in6 *)addr->ifa_addr;
                    memcpy(myipv6addr->addr, &sain6->sin6_addr, sizeof(bipv6addr_t));
                    *iface_index = if_nametoindex(addr->ifa_name);
                    butil_log(4, "IPv6 on iface %d  %s\n", *iface_index, addr->ifa_name);
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
    fprintf(stderr, "Use: %s [-lst<N> -uU]\n", program);
    fprintf(stderr, "     -l    Set debug log level to N (default 1: errors/warnings only)\n");
    fprintf(stderr, "     -s    Stop after N seconds to test BYE\n");
    fprintf(stderr, "     -t    Set TTL to N for iface/services\n");
    fprintf(stderr, "     -4    Use only IPv4\n");
    fprintf(stderr, "     -6    Use only IPv6\n");
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
    uint32_t iface_index;
    char *txtrecs;
    char *arg;
    char *program;
    uint32_t uval;
    uint32_t ttl;
    uint32_t runtime;
    time_t endtime;
    int loglevel;
    bool unit_test;
    bool only_unit_test;
    bool ipv4only;
    bool ipv6only;
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
    loglevel = 5;
    ttl = 30;
    runtime = 0;
    butil_set_log_level(loglevel);
    unit_test = false;
    ipv4only = false;
    ipv6only = false;

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
            case 's':
            case 't':
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
                else if (arg[1] == 's')
                {
                    runtime = uval;
                }
                else if (arg[1] == 't')
                {
                    ttl = uval;
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
            case '4':
                ipv4only = true;
                ipv6only = false;
                break;
            case '6':
                ipv6only = true;
                ipv4only = false;
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
    responder.unit_testing = unit_test;

    // run unit tests that don't need networking
    if (unit_test)
    {
        result = mdns_basic_unit_test(&responder);
        if (result)
        {
            butil_log(0, "Basic Unit Test Failed\n");
            return result;
        }
    }
    get_host_info(hostname, sizeof(hostname), &myipv4addr, &myipv6addr, &iface_index);

    if (ipv4only)
    {
        myipv6addr.addr[0] = 0;
    }
    if (ipv6only)
    {
        myipv4addr.addr = 0;
    }
    result = mdns_responder_add_interface(&responder, hostname, iface_index, &myipv4addr, &myipv6addr, ttl);
    if (result)
    {
        butil_log(0, "Can't add interface\n");
    }
    result = mdns_responder_add_service(
                                        &responder,
                                        responder.interfaces,
                                        "BNET PRT",
                                         "_ipp",
                                         "_universal",
                                         MDNS_SRVPROTO_TCP,
                                         6310,
                                         txtrecs,
                                         ttl
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
                                         "_universal",
                                         MDNS_SRVPROTO_TCP,
                                         6311,
                                         txtrecs,
                                         ttl
                                       );
    if (result)
    {
        butil_log(0, "Can't add ipps service\n");
    }
    // per bonjour printing. LPD is the "flagship" service for printing and it should be
    // added/defended even if unimplemented, using a port of 0 for unimplemented
    //
    result = mdns_responder_add_service(
                                        &responder,
                                        responder.interfaces,
                                        "BNET PRT",
                                         "_printer",
                                         "_universal",
                                         MDNS_SRVPROTO_TCP,
                                         0,
                                         txtrecs,
                                         ttl
                                       );
    if (result)
    {
        butil_log(0, "Can't add lpd service\n");
    }
    if (unit_test)
    {
        result = mdns_unit_test(&responder);

        if (result)
        {
            butil_log(0, "Unit Test Failed\n");
        }
        if (only_unit_test)
        {
            return result;
        }
    }
    if (runtime > 0)
    {
        time_t now;

        time(&endtime);
        endtime += runtime;

        do
        {
            result = mdns_responder_slice(&responder);
            if (! result)
            {
                time(&now);
                if (now > endtime)
                {
                    butil_log(2, "ending loop\n");
                    result = mdns_responder_stop(&responder);
                    break;
                }
            }
        }
        while (! result && ! responder.fatal);
    }
    else
    {
        result = mdns_responder_run(&responder, 0, 50000);
    }
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

