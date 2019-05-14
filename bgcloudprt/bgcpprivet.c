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
#include "bgcpprivet.h"
#include "bgcp.h"

#if GCP_SUPPORT_LOCAL_PRT


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
    memset(myipv6addr->addr, 0, sizeof(myipv6addr));
    *iface_index = 0;
#endif
    return 0;
}

int gcp_local_disco_slice(gcp_context_t *gcp)
{
    int result;

    if (! gcp)
    {
        return -1;
    }
    if (! gcp->responding)
    {
        return 0;
    }
    result = mdns_responder_slice(&gcp->mdns_responder);
    return result;
}

int gcp_init_local_disco(gcp_context_t *gcp)
{
    char hostname[MDNS_MAX_DNTEXT];
    bipv4addr_t myipv4addr;
    bipv6addr_t myipv6addr;
    char model_str[64];
    char txtrecs[256];
    int iface_index;
    int result;

    if (! gcp)
    {
        return -1;
    }
    get_host_info(hostname, sizeof(hostname), &myipv4addr, &myipv6addr, &iface_index);

    snprintf(model_str, sizeof(model_str), "%s %s", GCP_PRT_MANUFACTURER, GCP_PRT_MODEL);

    // mdns responder for discovery
    //
    // See https://developers.google.com/cloud-print/docs/privet
    //
    snprintf(txtrecs, sizeof(txtrecs),
                "txtvers=1,"
                "ty=%s,"
                "note=%s,"
                "url=https://www.google.com/cloudprint,"
                "type=printer,"
                "id=%s,"
                "cs=%s",
                model_str,
                gcp->location,
                gcp->printer_id,
                gcp->printer_id[0] ? "online" : "not_configured"
            );

    result = mdns_responder_init(&gcp->mdns_responder);
    if (result)
    {
        return result;
    }
    result = mdns_responder_add_interface(&gcp->mdns_responder, hostname, iface_index, &myipv4addr, &myipv6addr, 75);
    if (result)
    {
        butil_log(0, "Can't add interface\n");
    }
    result = mdns_responder_add_service(
                                        &gcp->mdns_responder,
                                        gcp->mdns_responder.interfaces,
                                        model_str,
                                         "_privet",
                                         "_printer",
                                         MDNS_SRVPROTO_TCP,
                                         GCP_LOCAL_HTTP_PORT,
                                         txtrecs,
                                         120
                                       );
    if (result)
    {
        butil_log(0, "Can't add privet service\n");
    }
    else
    {
        gcp->responding = true;
    }
    return result;
}

#endif
