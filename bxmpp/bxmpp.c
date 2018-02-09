#include "bxmpp.h"

static struct tag_authlist
{
    const char *authname;
    bxmpp_auth_type_t type;
}
s_authlist[] =
{
    {    "notusedkeep", bxmppAuthNone  },
    {    "SCRAM-SHA-1", bxmppAuthSCRAMSHA1  },
//    {    "PLAIN",       bxmppAuthPLAIN   },
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
    result = bxml_find_element(bxp->pxp, "error", '.', 0, &etag);
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
    result = bxml_parse_value(bxp->pxp, etag, NULL, 0, NULL, &eval, &eval_len);
    if (! result)
    {
        result = bxml_copy_element((char*)bxp->in.data, bxp->in.size, eval, eval_len, false, false);
        if (! result)
        {
            butil_log(3, "error: %s\n", bxp->in.data);
        }
    }
    return -1;
}

static int bxmpp_start_authentication(bxmpp_t *bxp)
{
    char authbuf[256], encbuf[256];
    uint32_t nonce;
    size_t len;
    extern const char *s_username, *s_password;
    int result;

    switch (bxp->authtype)
    {
    case bxmppAuthNone:
    case bxmppAuthPLAIN:

        authbuf[0] = 0;
        strcpy(authbuf + 1, s_username);
        len = 2 + strlen(s_username);
        strcpy(authbuf + len, s_password);
        len += strlen(s_password);

        result = butil_base64_encode(encbuf, sizeof(encbuf), (uint8_t*)authbuf, len, false, false);

        // Send sasl auth
        //
        result = bxml_format_element(
                                (char*)bxp->out.data,
                                bxp->out.size,
                                (size_t*)&bxp->out.count,
                                false,
                                "auth",
                                encbuf,
                                2,
                                "xmlns", "urn:ietf:params:xml:ns:xmpp-sasl",
                                "mechanism", "PLAIN"
                                );
        if (result)
        {
            return result;
        }
        bxp->out.head = bxp->out.count;
        break;

    case bxmppAuthSCRAMSHA1:

        time(&nonce);
        result = butil_base64_encode(encbuf, sizeof(encbuf), (uint8_t*)&nonce, 4, false, false);

        len = snprintf(authbuf, sizeof(authbuf),
            "n,,n=%s,r=%s", s_username, encbuf);

        result = butil_base64_encode(encbuf, sizeof(encbuf), (uint8_t*)authbuf, len, false, false);
        result = butil_base64_decode(authbuf, sizeof(authbuf), (uint8_t*)encbuf);
        butil_log(3, "auth=%s\n", authbuf);

        // Send sasl auth
        //
        result = bxml_format_element(
                                (char*)bxp->out.data,
                                bxp->out.size,
                                (size_t*)&bxp->out.count,
                                false,
                                "auth",
                                encbuf,
                                2,
                                "xmlns", "urn:ietf:params:xml:ns:xmpp-sasl",
                                "mechanism", "SCRAM-SHA-1"
                                );
        if (result)
        {
            return result;
        }
        bxp->out.head = bxp->out.count;
        break;

    default:
        BERROR("Not an auth");
        return -1;
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

    char authbuf[256], encbuf[256];
    size_t len;
    extern const char *s_username, *s_password;

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
                                "to", bxp->to,
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
        bxp->pxp = bxml_parser_create((char*)bxp->in.data);
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
                        result = bxml_find_element(bxp->pxp, "mechanism", '.', i, &ptag);
                        if (! result)
                        {
                            result = bxml_parse_value(bxp->pxp, ptag, NULL, 0, NULL, &pval, &val_len);
                            if (! result)
                            {
                                result = bxml_copy_element(
                                              (char *)bxp->in.data,
                                              bxp->in.size,
                                              pval,
                                              val_len,
                                              false,
                                              false
                                              );
                                butil_log(5, "mech[%d]=%s\n", i, bxp->in.data);

                                // match auth avail to auth configured
                                //
                                if (bxp->authtype == bxmppAuthNone)
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
        bxml_parser_destroy(bxp->pxp);
        bxp->pxp = NULL;
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
        bxp->pxp = bxml_parser_create((char*)bxp->in.data);
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
        bxml_parser_destroy(bxp->pxp);
        bxp->pxp = NULL;
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
        bxp->pxp = bxml_parser_create((char*)bxp->in.data);
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
            // check for stream features: starttls
            //
            result = bxml_find_element(bxp->pxp, "success", '\0', 0, &ptag);
            if (! result)
            {
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
                    result = bxml_find_element(bxp->pxp, "failure.text", '.', 0, &ptag);
                    if (result)
                    {
                        break;
                    }
                    result = bxml_parse_value(bxp->pxp, ptag, NULL, 0, NULL, &pval, &val_len);
                    if (result)
                    {
                        break;
                    }
                    result = bxml_copy_element((char*)bxp->in.data, bxp->in.size, pval, val_len, false, false);
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
        bxml_parser_destroy(bxp->pxp);
        bxp->pxp = NULL;
        break;

    case bxmppBind:

        // Send bind
        //
        result = bxml_format_element(
                                authbuf,
                                sizeof(authbuf),
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
                                authbuf,
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

        // go to output phase, then to sasl reply if ok
        //
        bxmpp_push_state(bxp, bxmppOutPhase, bxmppBindReply);
        result = 0;
        break;

    case bxmppBindReply:

        // parse xml reply
        //
        bxp->pxp = bxml_parser_create((char*)bxp->in.data);
        if (! bxp->pxp)
        {
            BERROR("No XML parser for sasl reply");
            return -1;
        }
        bxp->in.count = 0;
        bxp->in.head = bxp->in.tail = 0;

        bxp->state = bxmppDone;

        // check for errors from server
        //
        result = bxmpp_check_errors(bxp);
        if (! result)
        {
            // check for bind results: jid
            //
            result = bxml_find_element(bxp->pxp, "iq.bind.jid", '.', 0, &ptag);
            if (result)
            {
                butil_log(3, "No JID in bind result\n");
                break;
            }
            result = bxml_parse_value(bxp->pxp, ptag, NULL, 0, NULL, &pval, &val_len);
            if (result)
            {
                BERROR("Parse JID");
                break;
            }
            result = bxml_copy_element(bxp->jid, sizeof(bxp->jid), pval, val_len, false, false);
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
        bxml_parser_destroy(bxp->pxp);
        bxp->pxp = NULL;
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
        bxp->pxp = bxml_parser_create((char*)bxp->in.data);
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
                                                    authbuf,
                                                    sizeof(authbuf),
                                                    false,
                                                    false
                                                    );
                if (result)
                {
                    authbuf[0] = '\0';
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
                    butil_log(2, "MESSAGE From %s:\n%s\n", authbuf, bxp->out.data);
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

        bxml_parser_destroy(bxp->pxp);
        bxp->pxp = NULL;
        break;

    case bxmppDone:

        if (bxp->stream)
        {
            bxp->stream->close(bxp->stream);
            bxp->stream = NULL;
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


bxmpp_t *bxmpp_create(const char *host, uint16_t port, const char *from, const char *to)
{
    bxmpp_t *bxp;

    if (! host || ! from || ! to)
    {
        return NULL;
    }
    if (strlen(host) >= BXMPP_MAX_HOST)
    {
        return NULL;
    }
    if (strlen(from) >= BXMPP_MAX_ADDR)
    {
        return NULL;
    }
    if (strlen(to) >= BXMPP_MAX_ADDR)
    {
        return NULL;
    }
    bxp = (bxmpp_t *)malloc(sizeof(bxmpp_t));
    if (bxp)
    {
        bxp->in.size = BXMPP_IO_SIZE;
        bxp->in.data = (uint8_t *)malloc(bxp->in.size);
        if (! bxp->in.data)
        {
            BERROR("Can't alloc in data");
            free(bxp);
            return NULL;
        }
        bxp->out.size = BXMPP_IO_SIZE;
        bxp->out.data = (uint8_t *)malloc(bxp->out.size);
        if (! bxp->out.data)
        {
            BERROR("Can't alloc out data");
            free(bxp->in.data);
            free(bxp);
            return NULL;
        }
        bxp->authtype = bxmppAuthNone;
        strcpy(bxp->host, host);
        strcpy(bxp->from, from);
        strcpy(bxp->to, to);
        bxp->port = port;
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
        if (bxp->pxp)
        {
            bxml_parser_destroy(bxp->pxp);
            bxp->pxp = NULL;
        }
        if (bxp->stream)
        {
            bxp->stream->close(bxp->stream);
            bxp->stream = NULL;
        }
        if (bxp->in.data)
        {
            free(bxp->in.data);
        }
        if (bxp->out.data)
        {
            free(bxp->out.data);
        }
        free(bxp);
        return 0;
    }
    return -1;
}


