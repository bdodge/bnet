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
#include "bgcpxmpp.h"
#include "bgcp.h"

static int gcp_xmpp_on_message(bxmpp_t *bxp, void *priv,
                bxmpp_cb_type_t type, const char *sender, const char *msg)
{
    gcp_context_t *gcp;
    bxml_parser_t xmlparser;
    bxml_parser_t *pxp;
    const char *ptag;
    const char *pval;
    size_t val_len;
    char printer_id[GCP_SHORT_TOKEN];
    int result;

    if (! bxp || ! priv || ! sender || ! msg)
    {
        return -1;
    }
    gcp = (gcp_context_t *)priv;

    if (type == bxmppINFOQUERY)
    {
        butil_log(5, "IQ from:%s = %s\n", sender, msg);
        return 0;
    }
    butil_log(5, "MSG from:%s = %s\n", sender, msg);

    pxp = bxml_parser_create(&bxp->xmlparser, bxp->ibuf);
    if (! bxp->pxp)
    {
        BERROR("No XML parser for message");
        return -1;
    }
    do
    {
        // check for message
        //
        result = bxml_find_element(bxp->pxp, "message.push.data", '.', 0, &ptag);
        if (! result)
        {
            result = bxml_parse_value(bxp->pxp, ptag, NULL, 0, NULL, &pval, &val_len);
            if (result)
            {
                BERROR("Parse Message");
                break;
            }
            result = bxml_copy_element(printer_id, sizeof(printer_id), pval, val_len, false, false);
            if (result)
            {
                BERROR("Copy message");
                break;
            }
            butil_log(5, "Push Notification for printer %s\n", printer_id);

            if (gcp->state == gcpPollWait)
            {
                gcp->state = gcpFetch;
            }
        }
    }
    while (0);
    return 0;
}

int gcp_xmpp_init(gcp_context_t *gcp)
{
    if (! gcp)
    {
        return -1;
    }
    gcp->bxp = bxmpp_create(
                        GCP_XMPP_SERVER, BXMPP_PORT,
                        bsaslAuthOAUTH2,
                        gcp->xmpp_jid, gcp->access_token,
                        "talk",
                        GCP_XMPP_CLIENT_PING_PERIOD,
                        gcp_xmpp_on_message, gcp
                      );
    if (! gcp->bxp)
    {
        BERROR("Can't create context\n");
    }
    return 0;
}

int gcp_xmpp_message(gcp_context_t *gcp, const char *msg)
{
    return 0;
}

int gcp_xmpp_slice(gcp_context_t *gcp)
{
    time_t now;
    int result;

    if (! gcp || ! gcp->bxp)
    {
        return -1;
    }
    result = 0;

    switch (gcp->xmpp_state)
    {
    case gcpxInit:
        if (now < gcp->xmpp_restart)
        {
            return 0;
        }
        result = bxmpp_restart(gcp->bxp, gcp->xmpp_jid, gcp->access_token);
        if (result)
        {
            butil_log(2, "XMPP restart failed\n");
            gcp->xmpp_state = gcpxRestart;
            break;
        }
        gcp->xmpp_state = gcpxConnecting;
        break;

    case gcpxConnecting:
        if (! bxmpp_finished(gcp->bxp))
        {
            result = bxmpp_slice(gcp->bxp);
            if (result)
            {
                butil_log(1, "XMPP setup failed\n");
                gcp->xmpp_state = gcpxRestart;
                break;
            }
        }
        if (bxmpp_finished(gcp->bxp))
        {
            butil_log(4, "GCP XMPP Ends, restarting\n");
            gcp->xmpp_state = gcpxRestart;
            break;
        }
        if (bxmpp_connected(gcp->bxp))
        {
            char subscribe[256];
            size_t nmade;

            butil_log(4, "XMPP ------- Connected ------- \n");

            result = bxml_format_element(
                                subscribe,
                                sizeof(subscribe),
                                &nmade,
                                false,
                                "subscribe",
                                "<item channel='cloudprint.google.com' from='cloudprint.google.com'/>",
                                1,
                                "xmlns", "google:push"
                                );
            if (! result)
            {
                result = bxmpp_send_infoquery(gcp->bxp, false, gcp->xmpp_jid , subscribe);
            }
            if (result)
            {
                butil_log(1, "XMPP subscribe failed\n");
                gcp->xmpp_state = gcpxRestart;
                break;
            }
            gcp->xmpp_state = gcpxConnected;
        }
        break;

    case gcpxConnected:
        if (! bxmpp_finished(gcp->bxp))
        {
            result = bxmpp_slice(gcp->bxp);
            if (result)
            {
                butil_log(1, "XMPP failed\n");
                gcp->xmpp_state = gcpxRestart;
                break;
            }
        }
        if (bxmpp_finished(gcp->bxp))
        {
            butil_log(4, "GCP XMPP Ends, restarting\n");
            gcp->xmpp_state = gcpxRestart;
            break;
        }
        break;

    case gcpxRestart:
        gcp->xmpp_state = gcpxInit;
        gcp->xmpp_restart = now + GCP_XMPP_RECONNECT_PERIOD;
        result = 0;
        break;

    default:
        gcp->xmpp_state = gcpxRestart;
        result = 0;
        break;
    }
    return result;
}

int gcp_xmpp_deinit(gcp_context_t *gcp)
{
    if (gcp && gcp->bxp)
    {
        bxmpp_destroy(gcp->bxp);
    }
    return 0;
}

