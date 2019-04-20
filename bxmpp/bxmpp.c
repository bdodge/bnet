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
#include "bxmpp.h"

static struct tag_authlist
{
    const char *authname;
    bsasl_auth_type_t type;
}
s_authlist[] =
{
    {    "notusedkeep",         bsaslAuthNone       },
    {    "SCRAM-SHA-1",         bsaslAuthSCRAMSHA1  },
#if SASL_SUPPORT_GOOGLE_EXTENSIONS
    {    "X-OAUTH2",            bsaslAuthOAUTH2     },
    {    "X-GOOGLE-TOKEN",      bsaslAuthGOOGLETOKEN},
#endif
    {    "PLAIN",               bsaslAuthPLAIN      },
};
#define BXMPP_NUM_AUTHS   (sizeof(s_authlist)/sizeof(struct tag_authlist))

static uint32_t bxmpp_next_id(bxmpp_t *bxp)
{
    return bxp->next_id++;
}

static const char *bxmpp_next_id_str(bxmpp_t *bxp)
{
    snprintf(bxp->idbuf, sizeof(bxp->idbuf), "%u", bxmpp_next_id(bxp));
    return bxp->idbuf;
}

static int bxmpp_check_errors(bxmpp_t *bxp)
{
    const char *etag;
    const char *eval;
    size_t eval_len;
    int result;

    if (! bxp || ! bxp->pxp)
    {
        return -1;
    }
    // look for stream:error
    //
    result = bxml_find_and_copy_element(
                                        bxp->pxp,
                                        "error", '.',
                                         0,
                                        (char *)bxp->abuf, sizeof(bxp->abuf),
                                        false, false
                                        );
    if (result)
    {
        if (result == bxml_not_found)
        {
            return 0;
        }
        else
        {
            butil_log(3, "XMPP: BAD XML? %s\n", bxp->pxp->xml);
            return result;
        }
    }
    butil_log(3, "XMPP Error: %s\n", bxp->abuf);
    return -1;
}

static int bxmpp_start_authentication(bxmpp_t *bxp)
{
    const char *auth_mechanism;
    size_t len;
    size_t nmade;
    time_t now;
    int i;
    int result;

    bxp->sasl = bsasl_auth_context_create(bxp->user, bxp->pass);
    if (! bxp->sasl)
    {
        BERROR("No SASL ctx");
        return -1;
    }
    result = bsasl_start_authentication(bxp->sasl, bxp->authtype);
    if (result)
    {
        return result;
    }
    auth_mechanism = bsasl_method_name(bxp->authtype);

    // Send sasl auth
    //
    switch (bxp->authtype)
    {
    case bsaslAuthNone:
    case bsaslAuthPLAIN:
    case bsaslAuthSCRAMSHA1:
        result = bxml_format_element(
                            (char*)bxp->out.data,
                            bxp->out.size,
                            &nmade,
                            false,
                            "auth",
                            (char*)bxp->sasl->clientFinalMessage,
                            2,
                            "xmlns", "urn:ietf:params:xml:ns:xmpp-sasl",
                            "mechanism", auth_mechanism
                            );
        break;
#if SASL_SUPPORT_GOOGLE_EXTENSIONS
    case bsaslAuthOAUTH2:
    case bsaslAuthGOOGLETOKEN:
        result = bxml_format_element(
                            (char*)bxp->out.data,
                            bxp->out.size,
                            &nmade,
                            false,
                            "auth",
                            (char*)bxp->sasl->clientFinalMessage,
                            6,
                            "xmlns", "urn:ietf:params:xml:ns:xmpp-sasl",
                            "mechanism", auth_mechanism,
                            "auth:service", "chromiumsync",
                            "auth:allow-generated-jid", "true",
                            "auth:client-uses-full-bind-result", "true",
                            "xmlns:auth", "http://www.google.com/talk/protocol/auth"
                            );
        break;
#endif
    default:
        BERROR("Unsupported auth type\n");
        return -1;
    }
    bxp->out.count = nmade;
    bxp->out.head = bxp->out.count;
    return result;
}

#define BXMPP_MAX_AUTHLEN 128

static int bxmpp_finish_authentication(bxmpp_t *bxp, const char *challenge)
{
    size_t nmade;
    int result;

    switch (bxp->authtype)
    {
    default:
        break;

    case bsaslAuthSCRAMSHA1:

        result = bsasl_finish_authentication(bxp->sasl, challenge);
        if (result)
        {
            return result;
        }
        // Send sasl auth
        //
        result = bxml_format_element(
                                (char*)bxp->out.data,
                                bxp->out.size,
                                &nmade,
                                false,
                                "response",
                                (char*)bxp->sasl->clientFinalMessage,
                                1,
                                "xmlns", "urn:ietf:params:xml:ns:xmpp-sasl"
                                );
        bxp->out.count = nmade;
        bxp->out.head = bxp->out.count;
        return result;
    }
    return 0;
}

static const char *bxmpp_state(bxmpp_state_t state)
{
    switch (state)
    {
    case bxmppDone:             return "Done";
    case bxmppInit:             return "Init";
    case bxmppTransport:        return "Transport";
    case bxmppTransportReply:   return "TransportReply";
    case bxmppTLS:              return "TLS";
    case bxmppTLSproceed:       return "TLSproceed";
    case bxmppSASL:             return "SASL";
    case bxmppSASLreply:        return "SASLreply";
    case bxmppSCRAM:            return "SCRAM";
    case bxmppSCRAMreply:       return "SCRAMreply";
    case bxmppBind:             return "Bind";
    case bxmppBindReply:        return "BindReply";
    case bxmppConnected:        return "Connected";
    case bxmppCheckIQreply:     return "CheckIQreply";
    case bxmppOutPhase:         return "OutPhase";
    case bxmppInPhase:          return "InPhase";
    }
    return "???";
}

static const char *bxmpp_layer(bxmpp_layer_t layer)
{
    switch (layer)
    {
    case bxmppLayerNone:        return "None";
    case bxmppLayerTCP:         return "TCP";
    case bxmppLayerTLS:         return "TLS";
    case bxmppLayerSASL:        return "SASL";
    case bxmppLayerSession:     return "Session";
    }
    return "???";
}

int bxmpp_finished(bxmpp_t *bxp)
{
    if (! bxp)
    {
        return -1;
    }
    return (bxp->state == bxmppDone) ? 1 : 0;
}

int bxmpp_connected(bxmpp_t *bxp)
{
    if (! bxp)
    {
        return 0;
    }
    if (bxp->state == bxmppConnected)
    {
        return 1;
    }
    if (bxp->state == bxmppInPhase && bxp->next_state == bxmppConnected)
    {
        return 1;
    }
    return 0;
}

static void bxmpp_push_state(bxmpp_t *bxp, bxmpp_state_t to_state, bxmpp_state_t ret_state)
{
    bxp->long_timeout = 0;
    bxp->next_state = ret_state;
    bxp->state = to_state;
}

static void bxmpp_pop_state(bxmpp_t *bxp)
{
    bxp->state = bxp->next_state;
    bxp->next_state = bxmppInit;
}

int bxmpp_slice(bxmpp_t *bxp)
{
    iostream_t *stream;
    const char *ptag;
    const char *pval;
    size_t val_len;
    size_t nmade;
    size_t count;
    time_t now;
    int result;

    static int oldincount;
    static bxmpp_state_t oldstate;

    result = -1;

    if (bxp->state != oldstate || bxp->in.count != oldincount)
    {
        butil_log(6, "XMPP state=%s layer=%s  in=%u out=%u\n",
              bxmpp_state(bxp->state),
              bxmpp_layer(bxp->layer),
              bxp->in.count, bxp->out.count);
        oldstate = bxp->state;
        oldincount = bxp->in.count;
    }
    switch (bxp->state)
    {
    case bxmppInit:

        // init TLS layer
        //
        result = iostream_tls_prolog();
        if (result)
        {
            break;
        }
        // create a socket to host
        //
        bxp->stream = iostream_create_from_tcp_connection(bxp->host, bxp->port);
        if (! bxp->stream)
        {
            butil_log(4, "Didn't connect on XMPP port\n");
            bxp->stream = iostream_create_from_tcp_connection(bxp->host, BXMPP_TLS_PORT);
            if (bxp->stream)
            {
                butil_log(4, "XMPP Connected to TLS port\n");
                bxp->port = BXMPP_TLS_PORT;
            }
        }
        if (! bxp->stream)
        {
            BERROR("Stream Create");
            bxp->state = bxmppDone;
        }
        else
        {
            butil_log(4, "XMPP Connnected to %s:%u\n", bxp->host, bxp->port);
            bxp->state = bxmppTransport;
            bxp->next_state = bxmppInit;
            bxp->layer = bxmppLayerTCP;
            iostream_normalize_ring(&bxp->in, NULL);
            iostream_normalize_ring(&bxp->out, NULL);
        }
        result = 0;
        break;

    case bxmppTransport:

        // Send stream element
        //
        result = bxml_format_element(
                                (char*)bxp->out.data,
                                bxp->out.size,
                                &nmade,
                                true,
                                "stream:stream",
                                NULL,
                                4,
                                "to", bxp->host,
                                "xmlns", "jabber:client",
                                "xmlns:stream", "http://etherx.jabber.org/streams",
                                "version", "1.0"
                                );
        if (result)
        {
            butil_log(1, "XMPP Failed to format stream start\n");
            return result;
        }
        bxp->out.count = nmade;
        bxp->out.head = bxp->out.count;

        // go to output phase, then to transport reply if ok
        //
        bxmpp_push_state(bxp, bxmppOutPhase, bxmppTransportReply);
        result = 0;
        break;

    case bxmppTransportReply:

        // parse xml reply
        //
        bxp->pxp = bxml_parser_create(&bxp->xmlparser, bxp->ibuf);
        if (! bxp->pxp)
        {
            BERROR("No XML parser for reply");
            return -1;
        }
        bxp->in.count = 0;
        bxp->in.head = bxp->in.tail = 0;

        // check for errors from server
        //
        result = bxmpp_check_errors(bxp);
        if (! result)
        {
            switch(bxp->layer)
            {
            case bxmppLayerTCP:

                // default to SASL (no TLS)
                //
                bxp->state = bxmppSASL;

                // check for stream features: starttls
                //
                result = bxml_find_element(bxp->pxp, "stream.features.starttls", '.', 0, &ptag);
                if (! result)
                {
                    result = bxml_parse_value(bxp->pxp, ptag, NULL, 0, NULL, &pval, &val_len);
                    if (! result)
                    {
                        butil_log(7, "startttls=%s\n", pval);

                        // see if starttls is really required
                        //
                        result = strncmp(pval + 1, "required", 8);
                        if (! result)
                        {
                            butil_log(5, "XMPP TLS required\n");
                            bxp->state = bxmppTLS;
                        }
                    }
                }
                else if (result != bxml_not_found)
                {
                    butil_log(4, "XMPP Bad Reply XML:%s\n", bxp->pxp->xml);
                    bxp->state = bxmppDone;
                }
                break;

            case bxmppLayerTLS:

                // check for stream features: mechanisms
                //
                result = bxml_find_element(bxp->pxp, "stream.features.mechanisms", '.', 0, &ptag);
                if (! result)
                {
                    int i, j;

                    for (i = 0; i < 16; i++)
                    {
                        result = bxml_find_and_copy_element(
                                        bxp->pxp,
                                        "mechanism", '.',
                                         i,
                                        bxp->abuf, sizeof(bxp->abuf),
                                        false, false
                                        );
                        if (! result)
                        {
                            butil_log(6, "mechanism[%d]=%s\n", i, bxp->abuf);

                            // match auth avail to auths configured and then to preferred
                            //
                            if (bxp->authtype == bsaslAuthNone)
                            {
                                for (j = 0; j < BXMPP_NUM_AUTHS; j++)
                                {
                                    if (! strcmp(s_authlist[j].authname, (char*)bxp->abuf))
                                    {
                                        // this is an auth that we can do, if its preferred, do it
                                        //
                                        if (
                                                (s_authlist[j].type == bxp->authpreferred)
                                            ||  (bxp->authpreferred == bsaslAuthNone)
                                        )
                                        {
                                            butil_log(5, "XMPP Selected Auth %s\n", (char*)bxp->abuf);
                                            bxp->authtype = s_authlist[j].type;
                                            break;
                                        }
                                    }
                                }
                            }
                            // let outer loop continue to print out all available methods
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                bxp->state = bxmppSASL;
                result = 0;
                break;

            case bxmppLayerSASL:

                // check for stream features: bind
                //
                result = bxml_find_element(bxp->pxp, "stream.features.bind", '.', 0, &ptag);
                if (result)
                {
                    butil_log(2, "No bind feature, can't move past SASL\n");
                    bxp->state = bxmppDone;
                }
                else
                {
                    // Bind stream
                    butil_log(7, "Can Bind\n");
                    bxp->state = bxmppBind;
                }
                result = 0;
                break;

            default:
                break;
            }
        }
        break;

    case bxmppTLS:

        // Send starttls
        //
        result = bxml_format_element(
                                (char*)bxp->out.data,
                                bxp->out.size,
                                &nmade,
                                false,
                                "starttls",
                                "",
                                1,
                                "xmlns", "urn:ietf:params:xml:ns:xmpp-tls"
                                );
        if (result)
        {
            butil_log(1, "Failed to format starttls\n");
            return result;
        }
        bxp->out.count = nmade;
        bxp->out.head = bxp->out.count;

        // go to output phase, then to wait for tls proceed
        //
        bxmpp_push_state(bxp, bxmppOutPhase, bxmppTLSproceed);
        result = 0;
        break;

    case bxmppTLSproceed:

        // parse xml reply
        //
        bxp->pxp = bxml_parser_create(&bxp->xmlparser, bxp->ibuf);
        if (! bxp->pxp)
        {
            BERROR("No XML parser for proceed");
            return -1;
        }
        bxp->in.count = 0;
        bxp->in.head = bxp->in.tail = 0;

        // check for errors from server
        //
        result = bxmpp_check_errors(bxp);
        if (! result)
        {
            // check for stream features: starttls
            //
            result = bxml_find_element(bxp->pxp, "proceed", '\0', 0, &ptag);
            if (! result)
            {
                // open a TLS socket on top of the transport
                //
                stream = iostream_tls_create_from_iostream(bxp->stream, true);
                if (! stream)
                {
                    BERROR("TLS upgrade failed");
                    bxp->stream = NULL;
                    bxp->state = bxmppDone;
                }
                butil_log(5, "XMPP TLS started\n");

                // use tcp stream wrapped in tls io
                //
                bxp->stream = stream;

                // restart the stream on the TLS layer
                // RESTART STREAM --------------------
                bxp->layer = bxmppLayerTLS;
                bxp->state = bxmppTransport;
            }
            else if (result == bxml_not_found)
            {
                butil_log(4, "No proceed in starttls reply\n");
                bxp->state = bxmppDone;
            }
            else
            {
                butil_log(4, "XMPP Bad Reply XML:%s\n", bxp->pxp->xml);
                bxp->state = bxmppDone;
            }
        }
        break;

    case bxmppSASL:

        result = bxmpp_start_authentication(bxp);
        if (result)
        {
            break;
        }
        // go to output phase, then to sasl reply if ok
        //
        bxmpp_push_state(bxp, bxmppOutPhase, bxmppSASLreply);
        result = 0;
        break;

    case bxmppSASLreply:

        // parse xml reply
        //
        bxp->pxp = bxml_parser_create(&bxp->xmlparser, bxp->ibuf);
        if (! bxp->pxp)
        {
            BERROR("No XML parser for sasl reply");
            return -1;
        }
        bxp->in.count = 0;
        bxp->in.head = bxp->in.tail = 0;

        // check for errors from server
        //
        result = bxmpp_check_errors(bxp);
        if (! result)
        {
            // for scram type auth schemes we expect a challenge here
            //
            if (bxp->authtype == bsaslAuthSCRAMSHA1)
            {
                // check for challenge
                //
                result = bxml_find_and_copy_element(bxp->pxp, "challenge", '\0', 0,
                                            bxp->abuf, sizeof(bxp->abuf), false, false);
                if (! result)
                {
                    result = bxmpp_finish_authentication(bxp, bxp->abuf);
                    if (! result)
                    {
                        // formated a response, do that, and come back here and
                        // look for success next time in
                        //
                        bxmpp_push_state(bxp, bxmppOutPhase, bxmppSASLreply);
                        break;
                    }
                    // can't finish auth for some reason, fail
                    //
                    bxp->state = bxmppDone;
                    break;
                }
            }
            // check for stream features: starttls
            //
            result = bxml_find_element(bxp->pxp, "success", '\0', 0, &ptag);
            if (! result)
            {
                if (bxp->authtype == bsaslAuthSCRAMSHA1)
                {
                    // get the value of the success element
                    //
                    result = bxml_find_and_copy_element(bxp->pxp, "success", '\0', 0,
                                                bxp->abuf, sizeof(bxp->abuf), false, false);
                    if (! result)
                    {
                        int j, n;

                        // base-64 decode this and check against server signature
                        //
                        n = butil_base64_decode(bxp->out.data, bxp->out.size, bxp->abuf);
                        if (n > 2 && bxp->out.data[0] == 'v' && bxp->out.data[1] == '=')
                        {
                            n = butil_base64_decode((uint8_t*)bxp->abuf,
                                    sizeof(bxp->abuf), (char*)bxp->out.data + 2);
                            #if 0
                            printf("REPLY SERVERSIG=\n");
                            for (j = 0; j < n; j++)
                            {
                                printf("%02X", (uint8_t)bxp->abuf[j]);
                            }
                            printf("\n");
                            #endif
                        }
                    }
                }
                // restart the stream on the SASL layer
                // RESTART STREAM --------------------
                bxp->layer = bxmppLayerSASL;
                bxp->state = bxmppTransport;
            }
            else if (result == bxml_not_found)
            {
                bool explained_it = false;

                do // Try
                {
                    result = bxml_find_element(bxp->pxp, "aborted", '\0', 0, &ptag);
                    if (! result)
                    {
                        butil_log(2, "XMPP Aborted in SASL\n");
                        explained_it = true;
                        break;
                    }
                    result = bxml_find_element(bxp->pxp, "failure", '\0', 0, &ptag);
                    if (result)
                    {
                        butil_log(4, "XMPP Bad SASL reply XML? %s\n", bxp->pxp->xml);
                        explained_it = true;
                        break;
                    }
                    // see if there is a text message
                    //
                    result = bxml_find_and_copy_element(
                                        bxp->pxp,
                                        "failure.text", '.',
                                         0,
                                        bxp->abuf, sizeof(bxp->abuf),
                                        false, false
                                        );
                    if (! result)
                    {
                        butil_log(3, "XMPP SASL Failed: %s\n", bxp->abuf);
                        explained_it = true;
                    }
                }
                while (0); // Catch

                if (! explained_it)
                {
                    butil_log(3, "XMPP SASL Failure (unspecified)\n");
                }
                bxp->state = bxmppDone;
            }
            else
            {
                butil_log(4, "XMPP Bad Reply XML:%s\n", bxp->pxp->xml);
                bxp->state = bxmppDone;
            }
        }
        break;

    case bxmppBind:

        // Send bind
        //
        result = bxml_format_element(
                                bxp->abuf,
                                sizeof(bxp->abuf),
                                &val_len,
                                false,
                                "bind",
                                "<resource>talk</resource>",
                                1,
                                "xmlns", "urn:ietf:params:xml:ns:xmpp-bind"
                                );
        if (result)
        {
            BERROR("Format bind");
            return result;
        }
        result = bxml_format_element(
                                (char*)bxp->out.data,
                                bxp->out.size,
                                &nmade,
                                false,
                                "iq",
                                bxp->abuf,
                                2,
                                "type", "set",
                                "id", bxmpp_next_id_str(bxp)
                                );
        if (result)
        {
            BERROR("Format IQ");
            return result;
        }
        bxp->out.count = nmade;
        bxp->out.head = bxp->out.count;

        // go to output phase, then to bind reply if ok
        //
        bxmpp_push_state(bxp, bxmppOutPhase, bxmppBindReply);
        result = 0;
        break;

    case bxmppBindReply:

        // parse xml reply
        //
        bxp->pxp = bxml_parser_create(&bxp->xmlparser, bxp->ibuf);
        if (! bxp->pxp)
        {
            BERROR("No XML parser for sasl reply");
            return -1;
        }
        bxp->in.count = 0;
        bxp->in.head = bxp->in.tail = 0;

        // no longer needing sasl auth context
        //
        if (bxp->sasl)
        {
            bsasl_auth_context_destroy(bxp->sasl);
            bxp->sasl = NULL;
        }
        // assume error
        //
        bxp->state = bxmppDone;

        // check for errors from server
        //
        result = bxmpp_check_errors(bxp);
        if (! result)
        {
            // check for bind results: jid
            //
            result = bxml_find_and_copy_element(
                                                bxp->pxp,
                                                "iq.bind.jid", '.',
                                                0,
                                                bxp->jid,
                                                sizeof(bxp->jid),
                                                false,
                                                false
                                                );
            if (result)
            {
                BERROR("Copy JID");
                break;
            }
            butil_log(7, "Bound to ID:%s\n", bxp->jid);

            bxp->layer = bxmppLayerSession;
            bxp->state = bxmppConnected;
        }
        break;

    case bxmppConnected:

        // if no data, go get some
        //
        if (bxp->in.count == 0)
        {
            bxmpp_push_state(bxp, bxmppInPhase, bxmppConnected);
            result = 0;
            break;
        }
        // parse xml from server
        //
        bxp->pxp = bxml_parser_create(&bxp->xmlparser, bxp->ibuf);
        if (! bxp->pxp)
        {
            BERROR("No XML parser for message");
            return -1;
        }
        bxp->in.count = 0;
        bxp->in.head = bxp->in.tail = 0;

        bxp->state = bxmppDone;

        do // try
        {
            // check for errors from server
            //
            result = bxmpp_check_errors(bxp);
            if (result)
            {
                break;
            }
            // check for message
            //
            result = bxml_find_element(bxp->pxp, "message", '.', 0, &ptag);
            if (! result)
            {
                result = bxml_find_and_copy_attribute(
                                                    bxp->pxp,
                                                    ptag,
                                                    "from",
                                                    bxp->abuf,
                                                    sizeof(bxp->abuf),
                                                    false,
                                                    false
                                                    );
                if (result)
                {
                    bxp->abuf[0] = '\0';
                }
                result = bxml_parse_value(bxp->pxp, ptag, NULL, 0, NULL, &pval, &val_len);
                if (result)
                {
                    BERROR("Parse Message");
                    break;
                }
                result = bxml_copy_element((char *)bxp->out.data, bxp->out.size, pval, val_len, false, false);
                if (result)
                {
                    BERROR("Copy message");
                    break;
                }
                butil_log(6, "XMPP MESSAGE From %s:\n%s\n", bxp->abuf, bxp->out.data);
                if (bxp->message_callback)
                {
                    result = bxp->message_callback(bxp, bxp->message_priv, bxp->abuf, (char*)bxp->out.data);
                    if (result)
                    {
                        butil_log(1, "Message callback cancels\n");
                        bxp->state = bxmppDone;
                        return result;
                    }
                }
            }
            bxmpp_push_state(bxp, bxmppInPhase, bxmppConnected);
            result = 0;
        }
        while (0);

        break;

    case bxmppCheckIQreply:

        // parse xml from server
        //
        bxp->pxp = bxml_parser_create(&bxp->xmlparser, bxp->ibuf);
        if (! bxp->pxp)
        {
            BERROR("No XML parser for IQ reply");
            return -1;
        }
        bxp->in.count = 0;
        bxp->in.head = bxp->in.tail = 0;

        bxp->state = bxmppDone;

        do // try
        {
            // check for errors from server
            //
            result = bxmpp_check_errors(bxp);
            if (result)
            {
                break;
            }
            // check for iq
            //
            result = bxml_find_element(bxp->pxp, "iq", '.', 0, &ptag);
            if (! result)
            {
                result = bxml_find_and_copy_attribute(
                                                    bxp->pxp,
                                                    ptag,
                                                    "from",
                                                    bxp->abuf,
                                                    sizeof(bxp->abuf),
                                                    false,
                                                    false
                                                    );
                if (result)
                {
                    bxp->abuf[0] = '\0';
                }
                result = bxml_parse_value(bxp->pxp, ptag, NULL, 0, NULL, &pval, &val_len);
                if (result)
                {
                    BERROR("Parse IQ");
                    break;
                }
                result = bxml_copy_element((char *)bxp->out.data, bxp->out.size, pval, val_len, false, false);
                if (result)
                {
                    BERROR("Copy message");
                    break;
                }
                butil_log(6, "XMPP IQ From %s:\n%s\n", bxp->abuf, bxp->out.data);
                #if 0   // perhaps, in case we want to tell user?
                if (bxp->message_callback)
                {
                    result = bxp->message_callback(bxp, bxp->message_priv, bxp->abuf, (char*)bxp->out.data);
                    if (result)
                    {
                        butil_log(1, "Message callback cancels\n");
                        bxp->state = bxmppDone;
                        return result;
                    }
                }
                #endif
                bxmpp_push_state(bxp, bxmppInPhase, bxmppConnected);
                result = 0;
            }
        }
        while (0);
        break;

    case bxmppDone:

        if (bxp->stream)
        {
            bxp->stream->close(bxp->stream);
            bxp->stream = NULL;
        }
        if (bxp->sasl)
        {
            bsasl_auth_context_destroy(bxp->sasl);
            bxp->sasl = NULL;
        }
        result = 0;
        break;

    case bxmppOutPhase:

        // ensure stream is writeable
        //
        result = bxp->stream->poll(bxp->stream, writeable, 0, 50000);
        if (result < 0)
        {
            bxp->state = bxmppDone;
            return result;
        }
        if (result == 0)
        {
            time(&now);
            if (bxp->long_timeout == 0)
            {
                bxp->long_timeout = now + BXMPP_IO_TIMEOUT;
            }
            else
            {
                if (now > bxp->long_timeout)
                {
                    butil_log(2, "XMPP Output Timeout\n");
                    bxp->state = bxmppDone;
                    result = -1;
                    break;
                }
            }
            break;
        }
        // got data, reset timout
        bxp->long_timeout = 0;

        // get contiguous data to send
        //
        if (bxp->out.head > bxp->out.tail)
        {
            count = bxp->out.head - bxp->out.tail;
        }
        else
        {
            count = bxp->out.size - bxp->out.tail;
        }
        butil_log(6, "XMPP XML REQ>>>=%s\n", bxp->out.data);

        // send it up stream
        //
        result = bxp->stream->write(bxp->stream, bxp->out.data + bxp->out.tail, count);
        if (result < 0)
        {
            bxp->state = bxmppDone;
            return result;
        }
        bxp->out.tail += result;
        if (bxp->out.tail >= bxp->out.size)
        {
            bxp->out.tail = 0;
        }
        bxp->out.count -= result;

        // if sent it all the got get any reply, then go back to the
        // next state after the reply is i
        //
        if (bxp->out.count == 0)
        {
            bxp->out.head = bxp->out.tail = 0;
            bxmpp_pop_state(bxp);
            bxmpp_push_state(bxp, bxmppInPhase, bxp->state);
        }
        result = 0;
        break;

    case bxmppInPhase:

        // check for stream readable
        //
        result = bxp->stream->poll(bxp->stream, readable, 0, 50000);
        if (result < 0)
        {
            return result;
        }
        if (result == 0)
        {
            time(&now);
            if (bxp->long_timeout == 0)
            {
                bxp->long_timeout = now + BXMPP_IO_TIMEOUT;
            }
            else
            {
                if (now > bxp->long_timeout)
                {
                    if(bxp->next_state != bxmppConnected)
                    {
                        butil_log(2, "XMPP Input Timeout\n");
                        bxp->state = bxmppDone;
                        result = -1;
                    }
                    else
                    {
                        bxmpp_pop_state(bxp);
                    }
                }
            }
            break;
        }
        // got data, reset timout
        bxp->long_timeout = 0;

        do
        {
            // ensure max contiguous data to read, will always leave head > tail
            //
            iostream_normalize_ring(&bxp->in, NULL);

            // read the data
            //
            count = bxp->in.size - bxp->in.head - 1;

            result = bxp->stream->read(bxp->stream, bxp->in.data + bxp->in.head, count);
            if (result < 0)
            {
                bxp->state = bxmppDone;
                return result;
            }
            if (result == 0)
            {
                butil_log(2, "XMPP Connection closed\n");
                bxp->state = bxmppDone;
                return -1;
            }
            bxp->in.head += result;
            if (bxp->in.head > bxp->in.size)
            {
                bxp->in.head = 0;
            }
            bxp->in.count += result;

            result = bxp->stream->poll(bxp->stream, readable, 0, 50000);
            if (result < 0)
            {
                bxp->state = bxmppDone;
                return result;
            }
        }
        while (result > 0);

        // null terminate data
        //
        bxp->in.data[bxp->in.head] = '\0';
        bxp->in.head++;
        if (bxp->in.head > bxp->in.size)
        {
            bxp->in.head = 0;
        }
        bxp->in.count++;

        butil_log(6, "XMPP XML REPLY<<<<=%s\n", bxp->in.data);

        // return to state
        //
        bxmpp_pop_state(bxp);
        result = 0;
        break;

    default:
        BERROR("Can't happen");
        return -1;
    }
    return result;
}

int bxmpp_send_message(bxmpp_t *bxp, const char *recipient, const char *msg)
{
    size_t nmade;
    int result;

    if (! bxp || ! recipient || ! msg)
    {
        return -1;
    }
    if (bxp->state != bxmppConnected)
    {
        butil_log(2, "XMPP Not connected for send message\n");
        return -1;
    }
    result = bxml_format_element(
                            (char*)bxp->out.data,
                            bxp->out.size,
                            &nmade,
                            false,
                            "message",
                            msg,
                            5,
                            "from", bxp->jid,
                            "to", recipient,
                            "version", "1.0",
                            "xmlns", "jabber:client",
                            "xmlns:stream", "http://etherx.jabber.org/streams"
                            );
    if (result)
    {
        BERROR("Format Message");
        return result;
    }
    bxp->out.count = nmade;
    bxp->out.head = bxp->out.count;
    bxmpp_push_state(bxp, bxmppOutPhase, bxmppConnected);
    return 0;
}

int bxmpp_send_infoquery(bxmpp_t *bxp, const char *recipient, const char *msg)
{
    size_t nmade;
    int result;

    if (! bxp || ! recipient || ! msg)
    {
        return -1;
    }
    if (bxp->state != bxmppConnected)
    {
        butil_log(2, "XMPP Not connected for send message\n");
        return -1;
    }
    result = bxml_format_element(
                            (char*)bxp->out.data,
                            bxp->out.size,
                            &nmade,
                            false,
                            "iq",
                            msg,
                            2,
                            "type", "set",
                            "to", recipient,
                            "id", bxmpp_next_id_str(bxp)
                            );
    if (result)
    {
        BERROR("Format IQ");
        return result;
    }
    bxp->out.count = nmade;
    bxp->out.head = bxp->out.count;
    bxmpp_push_state(bxp, bxmppOutPhase, bxmppCheckIQreply);
    return 0;
}

int bxmpp_restart(bxmpp_t *bxp, const char *user, const char *password)
{
    if (! bxp || ! user || ! password)
    {
        return -1;
    }
    if (strlen(user) >= BXMPP_MAX_ADDR)
    {
        return -1;
    }
    if (strlen(password) >= BXMPP_MAX_ADDR)
    {
        return -1;
    }
    strcpy(bxp->user, user);
    strcpy(bxp->pass, password);

    if (bxp->stream)
    {
        bxp->stream->close(bxp->stream);
        bxp->stream = NULL;
    }
    if (bxp->sasl)
    {
        bsasl_auth_context_destroy(bxp->sasl);
        bxp->sasl = NULL;
    }
    bxp->layer = bxmppLayerNone;
    bxp->state = bxmppInit;
    bxp->next_state = bxmppInit;

    bxp->in.head = 0;
    bxp->in.tail = 0;
    bxp->in.count = 0;

    bxp->out.head = 0;
    bxp->out.tail = 0;
    bxp->out.count = 0;

    bxp->long_timeout = 0;

    bxp->authtype = bsaslAuthNone;

    bxp->state = bxmppInit;
    return 0;
}

bxmpp_t *bxmpp_create(
                        const char         *host,
                        uint16_t            port,
                        bsasl_auth_type_t   preferred_auth,
                        const char         *user,
                        const char         *password,
                        const char         *id,
                        xmpp_msg_callback_t callback,
                        void               *priv
                     )
{
    bxmpp_t *bxp;

    if (! host || ! user || ! password || ! id)
    {
        return NULL;
    }
    if (strlen(host) >= BXMPP_MAX_HOST)
    {
        return NULL;
    }
    if (strlen(user) >= BXMPP_MAX_ADDR)
    {
        return NULL;
    }
    if (strlen(password) >= BXMPP_MAX_ADDR)
    {
        return NULL;
    }
    if (strlen(id) >= BXMPP_MAX_ADDR)
    {
        return NULL;
    }
    bxp = (bxmpp_t *)malloc(sizeof(bxmpp_t));
    if (bxp)
    {
        if (bxmpp_restart(bxp, user, password))
        {
            free(bxp);
            return NULL;
        }
        bxp->message_callback = callback;
        bxp->message_priv = priv;
        bxp->in.size = BXMPP_IO_SIZE;
        bxp->in.data = (uint8_t *)bxp->ibuf;
        bxp->out.size = BXMPP_IO_SIZE;
        bxp->out.data = (uint8_t*)bxp->obuf;
        bxp->authpreferred = preferred_auth;
        strcpy(bxp->host, host);
        bxp->port = port;
        strcpy(bxp->user, user);
        strcpy(bxp->pass, password);
        strcpy(bxp->id, id);
        bxp->sasl = NULL;
        bxp->stream = NULL;
    }
    return bxp;
}

int bxmpp_destroy(bxmpp_t *bxp)
{
    if (bxp)
    {
        bxmpp_restart(bxp, "", "");
        free(bxp);
        return 0;
    }
    return -1;
}


