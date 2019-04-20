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
                        "talk"
                      );
    if (! gcp->bxp)
    {
        BERROR("Can't create context\n");
    }
    return 0;
}

int gcp_xmpp_slice(gcp_context_t *gcp)
{
    int result;

    if (! gcp || ! gcp->bxp)
    {
        return -1;
    }
    if (gcp->bxp->state == bxmppInit)
    {
        result = bxmpp_restart(gcp->bxp, gcp->xmpp_jid, gcp->access_token);
        if (result)
        {
            butil_log(1, "XMPP restart failed\n");
        }
    }
    if (gcp->bxp->state != bxmppDone)
    {
        result = bxmpp_setup(gcp->bxp);
        if (result)
        {
            butil_log(1, "XMPP setup failed\n");
            return result;
        }
    }
    return 0;
}

int gcp_xmpp_deinit(gcp_context_t *gcp)
{
    if (gcp && gcp->bxp)
    {
        bxmpp_destroy(gcp->bxp);
    }
    return 0;
}

