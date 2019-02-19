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
    {    "notusedkeep", bsaslAuthNone  },
    {    "SCRAM-SHA-1", bsaslAuthSCRAMSHA1  },
//   {    "PLAIN",       bsaslAuthPLAIN   },
};
#define BXMPP_NUM_AUTHS   (sizeof(s_authlist)/sizeof(struct tag_authlist))

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
                                        (char *)bxp->in.data, bxp->in.size,
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
            butil_log(5, "BAD XML? %s\n", bxp->pxp->xml);
            return result;
        }
    }
    butil_log(3, "error: %s\n", bxp->in.data);
    return -1;
}

static int bxmpp_start_authentication(bxmpp_t *bxp)
{
    const char *auth_mechanism;
    size_t len;
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
    result = bxml_format_element(
                            (char*)bxp->out.data,
                            bxp->out.size,
                            (size_t*)&bxp->out.count,
                            false,
                            "auth",
                            (char*)bxp->sasl->clientFinalMessage,
                            2,
                            "xmlns", "urn:ietf:params:xml:ns:xmpp-sasl",
                            "mechanism", auth_mechanism
                            );
    bxp->out.head = bxp->out.count;
    return result;
}

#define BXMPP_MAX_AUTHLEN 128

static int bxmpp_finish_authentication(bxmpp_t *bxp, const char *challenge)
{
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
                                (size_t*)&bxp->out.count,
                                false,
                                "response",
                                bxp->sasl->clientFinalMessage,
                                1,
                                "xmlns", "urn:ietf:params:xml:ns:xmpp-sasl"
                                );
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
    case bxmppBindReply:        return "BinReply";
    case bxmppSession:          return "Session";
    case bxmppSessionReply:     return "SessionReply";
    case bxmppConnected:        return "Connected";
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

static void bxmpp_push_state(bxmpp_t *bxp, bxmpp_state_t to_state, bxmpp_state_t ret_state)
{
    bxp->next_state = ret_state;
    bxp->state = to_state;
}

static void bxmpp_pop_state(bxmpp_t *bxp)
{
    bxp->state = bxp->next_state;
    bxp->next_state = bxmppInit;
}

int bxmpp_setup(bxmpp_t *bxp)
{
    iostream_t *stream;
    const char *ptag;
    const char *pval;
    size_t val_len;
    size_t count;
    int result;

    static int oldincount;
    static bxmpp_state_t oldstate;

    result = -1;

    if (bxp->state != oldstate || bxp->in.count != oldincount)
    {
        butil_log(5, "state %s layer %s  in=%u out=%u\n",
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
        bxp->stream = iostream_create_from_tcp_connection(
                bxp->host, bxp->port);

        if (! bxp->stream)
        {
            BERROR("Stream Create");
            bxp->state = bxmppDone;
        }
        else
        {
            butil_log(4, "Connnected to %s:%u\n", bxp->host, bxp->port);
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
                                (size_t *)&bxp->out.count,
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
            return result;
        }
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
                        butil_log(5, "starttttls=%s\n", pval);

                        // see if starttls is really required
                        //
                        result = strncmp(pval + 1, "required", 8);
                        if (! result)
                        {
                            butil_log(4, "TLS required\n");
                            bxp->state = bxmppTLS;
                        }
                    }
                }
                else if (result != bxml_not_found)
                {
                    butil_log(4, "Bad Reply XML:%s\n", bxp->pxp->xml);
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
                                        (char *)bxp->in.data, bxp->in.size,
                                        false, false
                                        );
                        if (! result)
                        {
                            butil_log(5, "mech[%d]=%s\n", i, bxp->in.data);

                            // match auth avail to auth configured
                            //
                            if (bxp->authtype == bsaslAuthNone)
                            {
                                for (j = 0; j < BXMPP_NUM_AUTHS; j++)
                                {
                                    if (! strcmp(s_authlist[j].authname, (char*)bxp->in.data))
                                    {
                                        butil_log(4, "Selected Auth %s\n", (char*)bxp->in.data);
                                        bxp->authtype = s_authlist[j].type;
                                        break;
                                    }
                                }
                            }
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
                    butil_log(5, "Can Bind\n");
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
                                (size_t*)&bxp->out.count,
                                false,
                                "starttls",
                                "",
                                1,
                                "xmlns", "urn:ietf:params:xml:ns:xmpp-tls"
                                );
        if (result)
        {
            return result;
        }
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
                butil_log(4, "TLS started\n");

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
                butil_log(4, "Bad Reply XML:%s\n", bxp->pxp->xml);
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
                            printf("REPLY SERVERSIG=\n");
                            for (j = 0; j < n; j++)
                            {
                                printf("%02X", (uint8_t)bxp->abuf[j]);
                            }
                            printf("\n");
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
                        butil_log(2, "Aborted in SASL\n");
                        explained_it = true;
                        break;
                    }
                    result = bxml_find_element(bxp->pxp, "failure", '\0', 0, &ptag);
                    if (result)
                    {
                        butil_log(4, "Bad SASL reply XML? %s\n", bxp->pxp->xml);
                        explained_it = true;
                        break;
                    }
                    // see if there is a text message
                    //
                    result = bxml_find_and_copy_element(
                                        bxp->pxp,
                                        "failure.text", '.',
                                         0,
                                        (char *)bxp->in.data, bxp->in.size,
                                        false, false
                                        );
                    if (! result)
                    {
                        butil_log(3, "SASL Failed: %s\n", bxp->in.data);
                        explained_it = true;
                    }
                }
                while (0); // Catch

                if (! explained_it)
                {
                    butil_log(3, "SASL Failure (unspecified)\n");
                }
                bxp->state = bxmppDone;
            }
            else
            {
                butil_log(4, "Bad Reply XML:%s\n", bxp->pxp->xml);
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
                                (size_t*)&bxp->out.count,
                                false,
                                "iq",
                                bxp->abuf,
                                2,
                                "type", "set",
                                "id", "0"
                                );
        if (result)
        {
            BERROR("Format IQ");
            return result;
        }
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
            butil_log(4, "Bound to ID:%s\n", bxp->jid);
        #if 0
            bxp->layer = bxmppLayerSession;
            bxp->state = bxmppSession;
        #else
            bxp->layer = bxmppLayerSession;
            bxp->state = bxmppConnected;

            // send a message to ourselves
            //
            result = bxml_format_element(
                                    (char*)bxp->out.data,
                                    bxp->out.size,
                                    (size_t*)&bxp->out.count,
                                    false,
                                    "message",
                                    "<body>Hello</body>",
                                    5,
                                    "from", bxp->jid,
                                    "to", bxp->jid,
                                    "version", "1.0",
                                    "xmlns", "jabber:client",
                                    "xmlns:stream", "http://etherx.jabber.org/streams"
                                    );
            if (result)
            {
                BERROR("Format IQ");
                return result;
            }
            bxp->out.head = bxp->out.count;
            bxmpp_push_state(bxp, bxmppOutPhase, bxmppConnected);
        #endif
        }
        break;

    case bxmppSession:

        bxp->state = bxmppDone;
        break;

    case bxmppSessionReply:

        bxp->state = bxmppDone;
        break;

    case bxmppConnected:

        // parse xml from server
        //
        bxp->pxp = bxml_parser_create(&bxp->xmlparser, bxp->ibuf);
        if (! bxp->pxp)
        {
            BERROR("No XML parser for sasl reply");
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
                result = bxml_find_element(bxp->pxp, "message.body", '.', 0, &ptag);
                if (! result)
                {
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
                    butil_log(2, "MESSAGE From %s:\n%s\n", bxp->abuf, bxp->out.data);
                }
                bxmpp_push_state(bxp, bxmppInPhase, bxmppConnected);
                result = 0;
            }
            else if (result != bxml_not_found)
            {
                break;
            }
            else
            {
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
        result = bxp->stream->poll(bxp->stream, writeable, 0, 10000);
        if (result < 0)
        {
            return result;
        }
        if (result == 0)
        {
            // todo - long timeout
            return 0;
        }
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
        butil_log(5, "XML REQ>>>=%s\n", bxp->out.data);

        // send it up stream
        //
        result = bxp->stream->write(bxp->stream, bxp->out.data + bxp->out.tail, count);
        if (result < 0)
        {
            return result;
        }
        bxp->out.tail += result;
        if (bxp->out.tail >= bxp->out.size)
        {
            bxp->out.tail = 0;
        }
        bxp->out.count -= result;

        // if sent it all, pop state
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
        result = bxp->stream->poll(bxp->stream, readable, 0, 10000);
        if (result < 0)
        {
            return result;
        }
        if (result == 0)
        {
            // todo - long timeout
            //
            return 0;
        }
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
                return result;
            }
            bxp->in.head += result;
            if (bxp->in.head > bxp->in.size)
            {
                bxp->in.head = 0;
            }
            bxp->in.count += result;

            result = bxp->stream->poll(bxp->stream, readable, 0, 4000);
            if (result < 0)
            {
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

        butil_log(5, "XML REPLY<<<<=%s\n", bxp->in.data);

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


bxmpp_t *bxmpp_create(
                        const char *host,
                        uint16_t    port,
                        const char *user,
                        const char *password,
                        const char *id
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
        bxp->in.size = BXMPP_IO_SIZE;
        bxp->in.data = (uint8_t *)bxp->ibuf;
        bxp->in.head = 0;
        bxp->in.tail = 0;
        bxp->in.count = 0;
        bxp->out.size = BXMPP_IO_SIZE;
        bxp->out.data = (uint8_t*)bxp->obuf;
        bxp->out.head = 0;
        bxp->out.tail = 0;
        bxp->out.count = 0;
        bxp->authtype = bsaslAuthNone;
        bxp->sasl = NULL;
        strcpy(bxp->host, host);
        bxp->port = port;
        strcpy(bxp->user, user);
        strcpy(bxp->pass, password);
        strcpy(bxp->id, id);
        bxp->layer = bxmppLayerNone;
        bxp->state = bxmppInit;
        bxp->next_state = bxmppInit;
        bxp->stream = NULL;
    }
    return bxp;
}

int bxmpp_destroy(bxmpp_t *bxp)
{
    if (bxp)
    {
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
        free(bxp);
        return 0;
    }
    return -1;
}


