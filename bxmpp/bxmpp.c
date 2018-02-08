#include "bxmpp.h"

int http_base64_decode(uint8_t *out, size_t outsize, const char *src)
{
    uint8_t *base = out;
    uint32_t bits, c, v, outchars;
    size_t errors = 0;

    outchars = 0;
    bits = 0;

    while ((c = *src++) != '\0')
    {
        if (outsize < 4)
        {
            errors++;
            break;
        }
        if (c >= 'A' && c <= 'Z')
        {
            v = c - 'A';
        }
        else if (c >= 'a' && c <= 'z')
        {
            v = c - 'a' + 26;
        }
        else if (c >= '0' && c <= '9')
        {
            v = c - '0' + 52;
        }
        else if (c == '+')
        {
            v = 62;
        }
        else if (c == '/')
        {
            v = 63;
        }
        else
        {
            break;
        }
        bits += v;
        outchars++;

        if (outchars == 4)
        {
            *out++ = (bits >> 16);
            *out++ = (bits >> 8) & 0xff;
            *out++ = (bits & 0xff);
            bits = 0;
            outchars = 0;
        }
        else
        {
            bits <<= 6;
        }
    }
    switch(outchars)
    {
    case 1:
        errors++;
        break;
    case 2:
        if (outsize < 2)
        {
            errors++;
            break;
        }
        *out++ = (bits >> 10);
        break;
    case 3:
        if (outsize < 3)
        {
            errors++;
            break;
        }
        *out++ = (bits >> 16);
        *out++ = (bits >> 8) & 0xff;
        break;
    }
    *out = '\0';
    return errors ? -1 : (out - base);
}

static inline int base64_hexencode_byte(char *out, uint8_t byte)
{
    if (byte == '+')
    {
        *out++ = '%';
        *out++ = '2';
        *out++ = 'B';
        return 3;
    }
    else if(byte == '/')
    {
        *out++ = '%';
        *out++ = '2';
        *out++ = 'F';
        return 3;
    }
    else if(byte == '=')
    {
        *out++ = '%';
        *out++ = '3';
        *out++ = 'D';
        return 3;
    }
    else
    {
        *out++ = byte;
        return 1;
    }
}

int http_base64_encode(char *out, size_t outsize, const uint8_t *src, size_t srcbytes, bool hexescape)
{
    char *base = out;
    uint8_t  b1, b2, b3;
    uint32_t d;
    size_t j = 0, k = 0;
    int i;

    static const char *s_alphabet64 =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    while (j < (outsize - 4) && k < srcbytes)
    {
        b1 = src[k];
        b2 = (k+1 < srcbytes) ? src[k+1] : 0;
        b3 = (k+2 < srcbytes) ? src[k+2] : 0;

        d = (b1 << 16) | (b2 << 8) | b3;

        if(hexescape)
        {
            char b;

            b = s_alphabet64[(d>>18)];
            i = base64_hexencode_byte(out, b);
            j += i;
            out += i;

            b = s_alphabet64[(d>>12) & 0x3f];
            i = base64_hexencode_byte(out, b);
            j += i;
            out += i;

            if (k+1 < srcbytes)
            {
                b = s_alphabet64[(d>>6) & 0x3f];
                i = base64_hexencode_byte(out, b);
                j += i;
                out += i;
            }
            else {
                i = base64_hexencode_byte(out, '=');
                j += i;
                out += i;
            }
            if(k+2 < srcbytes)
            {
                b = s_alphabet64[(d) & 0x3f];
                i = base64_hexencode_byte(out, b);
                j += i;
                out += i;
            }
            else {
                i = base64_hexencode_byte(out, '=');
                j += i;
                out += i;
            }
        }
        else {
            *out++ =                    s_alphabet64[((d>>18)        )];
            *out++ =                    s_alphabet64[((d>>12) & 0x3f )];
            *out++ = (k+1 < srcbytes) ? s_alphabet64[((d>> 6) & 0x3f )] : '=';
            *out++ = (k+2 < srcbytes) ? s_alphabet64[((d    ) & 0x3f )] : '=';
            j += 4;
        }
        k+= 3;
    }
    *out = '\0';
    return (k <= srcbytes) ? -1 : (out - base);
}

static const char *s_bind_xml       =   "<iq type='set' id='0'>"
                                          "<bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'>"
                                            "<resource>%s</resource>"
                                          "</bind>"
                                        "</iq>";

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
            printf("bad xml?=%s\n", bxp->pxp->xml);
            return result;
        }
    }
    result = bxml_parse_value(bxp->pxp, etag, NULL, 0, NULL, &eval, &eval_len);
    if (! result)
    {
        result = bxml_copy_element((char*)bxp->in.data, bxp->in.size, eval, eval_len, false, false);
        if (! result)
        {
            printf("error: %s\n", bxp->in.data);
        }
    }
    return -1;
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
    case bxmppLayerBound:       return "Connected";
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
        printf("state %s layer %s  in=%u out=%u\n",
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
            printf("Connnected to %s:%u\n", bxp->host, bxp->port);
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

        printf("OUT=%s\n", bxp->out.data);

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
                        printf("starttttls=%s\n", pval);

                        // see if starttls is really required
                        //
                        result = strncmp(pval + 1, "required", 8);
                        if (! result)
                        {
                            printf("TLS required\n");
                            bxp->state = bxmppTLS;
                        }
                    }
                }
                else if (result != bxml_not_found)
                {
                    printf("xml=%d\n", result);
                    BERROR("Bad Reply XML");
                    bxp->state = bxmppDone;
                }
                break;

            case bxmppLayerTLS:

                // check for stream features: mechanisms
                //
                result = bxml_find_element(bxp->pxp, "stream.features.mechanisms", '.', 0, &ptag);
                if (! result)
                {
                    int i;

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
                                printf("mech[%d]=%s\n", i, bxp->in.data);
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

                result = -1;
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
                printf("TLS started\n");

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
                printf("No proceed in starttls reply\n");
                bxp->state = bxmppDone;
            }
            else
            {
                printf("xml=%d\n", result);
                BERROR("Bad Reply XML");
                bxp->state = bxmppDone;
            }
        }
        bxml_parser_destroy(bxp->pxp);
        bxp->pxp = NULL;
        break;

    case bxmppSASL:

    {
        char authbuf[256], encbuf[256];
        size_t len;
        extern const char *s_username, *s_password;

        authbuf[0] = 0;
        strcpy(authbuf + 1, s_username);
        len = 2 + strlen(s_username);
        strcpy(authbuf + len, s_password);
        len += strlen(s_password);

        result = http_base64_encode(encbuf, sizeof(encbuf), (uint8_t*)authbuf, len, false);

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
    }
        // go to output phase, then to sasl reply if ok
        //
        bxmpp_push_state(bxp, bxmppOutPhase, bxmppSASLreply);
        result = 0;
        break;

    case bxmppSASLreply:
        break;

    case bxmppConnected:
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

        fprintf(stderr, "XML REPLY=\n%s\n", bxp->in.data);

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


