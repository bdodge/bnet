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
#include "bsmtp.h"

static int bsmtp_check_response(bsmtp_client_t *bsmtp, bsmtp_state_t next_state, size_t numcodes, ...)
{
    va_list args;
    size_t i;

    iostream_normalize_ring(&bsmtp->io, NULL);

    va_start(args, numcodes);
    for (i = 0; i < numcodes && i < BSMTP_MAX_EXPECTED_CODES; i++)
    {
        bsmtp->expected_codes[i] = (uint32_t)va_arg(args, unsigned);
    }
    va_end(args);

    bsmtp->num_codes = i;

    if (bsmtp->line_count < 3)
    {
        butil_log(5, "check resp: no data\n");
        bsmtp->state = bsmtpDone;
        return -1;
    }
    bsmtp->last_code = strtoul((char*)bsmtp->line, NULL, 0);

    for (i = 0; i < bsmtp->num_codes && i < BSMTP_MAX_EXPECTED_CODES; i++)
    {
        if (bsmtp->last_code == bsmtp->expected_codes[i])
        {
            butil_log(5, "Code %u is OK\n", bsmtp->expected_codes[i]);
            bsmtp->num_codes = 0;
            bsmtp->state = next_state;
            bsmtp->next_state = bsmtpInit;
            return 0;
        }
    }
    butil_log(5, "Error: got code %u, expected one of:\n", bsmtp->last_code);
    for (i = 0; i < bsmtp->num_codes && i < BSMTP_MAX_EXPECTED_CODES; i++)
    {
        butil_log(5, "  %u\n", bsmtp->expected_codes[i]);
    }
    bsmtp->state = bsmtpDone;
    bsmtp->num_codes = 0;
    return 0;
}

static int bsmtp_format_credentials(bsmtp_client_t *bsmtp, char *buf, int nbuf)
{
    char encbuf[BSMTP_MAX_ADDRESS * 2];
    char username[BSMTP_MAX_ADDRESS];
    char *pu;
    int  enclen;
    int result;

    // extract username from sender
    snprintf(username, sizeof(username), "%s", bsmtp->sender);
    pu = strchr(username, '@');
    if (pu)
    {
        *pu = '\0';
    }
    switch (bsmtp->auth_method)
    {
    case bsmtpPLAIN:
        enclen = snprintf(encbuf, sizeof(encbuf), "%s%c%s%c%s",
                    username, 0, bsmtp->sender, 0, bsmtp->password);
        break;

    case bsmtpLOGIN:
        enclen = snprintf(encbuf, sizeof(encbuf), "%s", bsmtp->sender);
        break;

    default:
        BERROR("unsupported method");
        return -1;
    }
    if (enclen < 0 || enclen >= sizeof(encbuf))
    {
        BERROR("encode buffer overflow");
        return -1;
    }
    // base64 encode string
    result = butil_base64_encode(buf, nbuf, (uint8_t *)encbuf, enclen, false, false);
    if (result < 0)
    {
        BERROR("encoding failed");
        return result;
    }
    butil_log(5, "cred encode %s -> %s\n", encbuf, buf);
    return 0;
}

static int bsmtp_client_output(bsmtp_client_t *bsmtp)
{
    int count;
    int wc;
    time_t now;

    if (! bsmtp || ! bsmtp->ctrl_stream)
    {
        return -1;
    }
    if (bsmtp->io.count == 0)
    {
        return 0;
    }
    wc = bsmtp->ctrl_stream->poll(bsmtp->ctrl_stream, writeable, 0, 100000);
    if (wc < 0)
    {
        BERROR("closed on send poll");
        return -1;
    }
    time(&now);

    if (wc == 0)
    {
        // check long timeout
        if (bsmtp->last_out_time == 0)
        {
            bsmtp->last_out_time = now;
        }
        if ((now - bsmtp->last_out_time) > bsmtp->long_timeout)
        {
            butil_log(3, "bsmtp write timeout\n");
            return -1;
        }
        return 0;
    }
    else
    {
        bsmtp->last_out_time = now;
    }
    if (bsmtp->io.tail >= bsmtp->io.head)
    {
        count = bsmtp->io.size - bsmtp->io.tail;
    }
    else
    {
        count = bsmtp->io.count;
    }
    wc = bsmtp->ctrl_stream->write(bsmtp->ctrl_stream,
                bsmtp->io.data + bsmtp->io.tail, count);
    if (wc <= 0)
    {
        BERROR("closed on send");
        return -1;
    }
    bsmtp->io.tail += wc;
    if (bsmtp->io.tail >= bsmtp->io.size)
    {
        bsmtp->io.tail = 0;
    }
    bsmtp->io.count -= wc;
    if (bsmtp->io.count == 0)
    {
        bsmtp->io.head = 0;
        bsmtp->io.tail = 0;
    }
    return wc;
}

static int bsmtp_begin_headers(bsmtp_client_t *bsmtp)
{
    bsmtp->io.count = 0;
    bsmtp->io.head = 0;
    bsmtp->io.tail = 0;
    return 0;
}

static int bsmtp_append_header(bsmtp_client_t *bsmtp, const char *content, ...)
{
    va_list args;
    int len;

    va_start(args, content);

    len = vsnprintf(
            (char*)bsmtp->io.data + bsmtp->io.head,
            bsmtp->io.size - bsmtp->io.count - 4,
            content,
            args
            );
    va_end(args);

    if (len >= (bsmtp->io.size - bsmtp->io.count - 2))
    {
        BERROR("Reply buffer overflow");
        return -1;
    }
    bsmtp->io.data[bsmtp->io.head + len] = '\r';
    bsmtp->io.data[bsmtp->io.head + len + 1] = '\n';
    bsmtp->io.data[bsmtp->io.head + len + 2] = '\0';
    len += 2;
    bsmtp->io.count += len;
    bsmtp->io.head += len;
    return 0;
}


static int bsmtp_send_command(bsmtp_client_t *bsmtp, bsmtp_state_t next_state, const char *cmd, const char *data)
{
    int len;

    if (cmd)
    {
        len = snprintf((char *)bsmtp->io.data, bsmtp->io.size,
                "%s%s%s\r\n", cmd,
                (data && *data) ? " " : "", (data && *data) ? data : "");

        if (len < 0 || len >= bsmtp->io.size)
        {
            BERROR("Command overflow");
            return -1;
        }
        bsmtp->io.count = len;
        bsmtp->io.head = len;
        bsmtp->io.tail = 0;
    }
    butil_log((bsmtp->state == bsmtpSendBody || bsmtp->state == bsmtpSendAttachment)
                ? 6 : 5, "<<==Cmd:%s\n", bsmtp->io.data);

    bsmtp->next_state = next_state;
    bsmtp->state = bsmtpOutPhase;
    return 0;
}

static int bsmtp_client_input(bsmtp_client_t *bsmtp, int to_secs, int to_usecs)
{
    time_t now;
    int result;
    int room;
    int count;
    int len;

    if (! bsmtp || ! bsmtp->ctrl_stream)
    {
        return -1;
    }
    count = bsmtp->io.count;

    if (bsmtp->io.count < bsmtp->io.size)
    {
        if (bsmtp->io.count == 0)
        {
            // to avoid later renormalizing
            bsmtp->io.head = 0;
            bsmtp->io.tail = 0;
        }
        if (bsmtp->io.head < bsmtp->io.tail)
        {
            room = bsmtp->io.tail - bsmtp->io.head;
        }
        else
        {
            room = bsmtp->io.size - bsmtp->io.head;
        }
        // if there is a bunch of data in the buffer, don't wait for more
        //
        if (room < (bsmtp->io.size / 2))
        {
            to_secs  = 0;
            to_usecs = 0;
        }
        // don't read if less than N bytes room available.  this is to
        // ensure TLS read block size can alway fit
        //
        if (room >= BSMTP_IO_MIN_ROOM)
        {
            result = bsmtp->ctrl_stream->poll(bsmtp->ctrl_stream, readable, to_secs, to_usecs);
            if (result < 0)
            {
                return result;
            }
            if (result > 0)
            {
                len = bsmtp->ctrl_stream->read(bsmtp->ctrl_stream,
                       bsmtp->io.data + bsmtp->io.head, room);
                if (len <= 0)
                {
                    if (bsmtp->io.count)
                    {
                        return 0;
                    }
                    // 0 read after ok poll means connection closed
                    butil_log(5, "end of connection\n");
                    return -1;
                }
                bsmtp->io.head += len;
                if (bsmtp->io.head >= bsmtp->io.size)
                {
                    bsmtp->io.head = 0;
                }
                bsmtp->io.count += len;
                butil_log(5, "read %d\n", len);
            }
        }
    }
    time(&now);

    if ((bsmtp->io.count == count) && (bsmtp->state == bsmtp->prev_state))
    {
        // check long timeout
        if (bsmtp->last_in_time == 0)
        {
            bsmtp->last_in_time = now;
        }
        if ((now - bsmtp->last_in_time) > bsmtp->long_timeout)
        {
            butil_log(3, "read timeout\n");
            return -1;
        }
    }
    else
    {
        // reset long timeout
        bsmtp->last_in_time = now;
    }
    return 0;
}

typedef enum
{
    besmtp_none     = 0,
    besmtp_auth     = 0x01,
    besmtp_size     = 0x02,
    besmtp_starttls = 0x04,
    besmtp_8bitmime = 0x08
}
bsmtp_feature_t;

static struct tag_esmtp_feature
{
    const char *name;
    bsmtp_feature_t feature;
}
s_esmtp_features[] =
{
    { "AUTH",     besmtp_auth },
    { "SIZE",     besmtp_size },
    { "STARTTLS", besmtp_starttls },
    { "8BITMIME", besmtp_8bitmime }
};

#define BSMTP_NUM_FEATURES (sizeof(s_esmtp_features) / sizeof(struct tag_esmtp_feature))

static struct tag_esmtp_authmethod
{
    const char *name;
    bsmtp_auth_type_t method;
}
s_esmtp_authmethods[] =
{
    { "PLAIN",      bsmtpPLAIN },
    { "LOGIN",      bsmtpLOGIN },
    { "GSSAPI",     bsmtpGSSAPI },
    { "DIGEST_MD5", bsmtpDIGEST_MD5 },
    { "MD5",        bsmtpMD5 },
    { "CRAM_MD5",   bsmtpCRAM_MD5 },
    { "XOAUTH",     bsmtpOAUTH1 },
    { "XOAUTH2",    bsmtpOAUTH2 }
};

#define BSMTP_NUM_AUTH_METHODS (sizeof(s_esmtp_authmethods) / sizeof(struct tag_esmtp_authmethod))

static const char *bsmtp_auth_method_string(bsmtp_auth_type_t method)
{
    int i;

    for (i = 0; i < BSMTP_NUM_AUTH_METHODS; i++)
    {
        if (s_esmtp_authmethods[i].method == method)
        {
            return s_esmtp_authmethods[i].name;
        }
    }
    return "???";
}

static int bsmtp_parse_feature(bsmtp_client_t *bsmtp)
{
    bsmtp_feature_t feature;
    int i;
    size_t len;
    uint16_t code;
    char *pf;
    char *pe;

    // expect feature in line, like "250-name value"
    //
    pf = (char*)bsmtp->line;
    code = strtoul(pf, &pf, 10);
    if (code != 250 || ! pf)
    {
        return 0;
    }
    if (*pf != '-')
    {
        return 0;
    }
    pf++;
    feature = besmtp_none;

    // if this is a feature we care about?
    for (i = 0; i < BSMTP_NUM_FEATURES; i++)
    {
        len = strlen(s_esmtp_features[i].name);
        if (! strncasecmp(pf, s_esmtp_features[i].name, len))
        {
            feature = s_esmtp_features[i].feature;
            pf+= len;
            break;
        }
    }
    bsmtp->esmtp_features |= (uint32_t)feature;

    // skip to value part of line
    while (butil_is_white(*pf))
    {
        pf++;
    }
    if (! *pf)
    {
        return 0;
    }
    // and terminate it
    for (pe = pf; *pe && ! butil_is_white(*pe);)
    {
        pe++;
    }
    *pe = '\0';

    switch (feature)
    {
    case besmtp_auth:
        while (*pf)
        {
            // if this is a method we care about?
            for (i = 0; i < BSMTP_NUM_AUTH_METHODS; i++)
            {
                if (! strcasecmp(pf, s_esmtp_authmethods[i].name))
                {
                    butil_log(5, "Adding auth method %s\n", s_esmtp_authmethods[i].name);
                    bsmtp->auth_supported |= (uint32_t)s_esmtp_authmethods[i].method;
                    break;
                }
            }
            pf = pe;

            if (pf >= (char*)bsmtp->line + bsmtp->line_count)
            {
                return 0;
            }
            pf++;

            if (! *pf)
            {
                return 0;
            }
            // skip to next token
            while (butil_is_white(*pf))
            {
                pf++;
            }
            // and terminate
            for (pe = pf; *pe && ! butil_is_white(*pe);)
            {
                pe++;
            }
            *pe = '\0';
        }
        break;
    case besmtp_size:
        bsmtp->max_message = strtoul(pf, NULL, 10);
        butil_log(5, "Setting max message size to %zu\n", bsmtp->max_message);
        break;
    case besmtp_starttls:
    case besmtp_8bitmime:
        break;
    default:
        break;
    }
    return 0;
}

static mime_content_type_t bsmtp_sniff_contents(bsmtp_client_t *bsmtp, const char *content, size_t content_length)
{
    if (! content || ! content_length)
    {
        return butil_mime_bin;
    }
    if (strstr(content, "<html"))
    {
        return butil_mime_html;
    }
    if (strstr(content, "<xml"))
    {
        return butil_mime_xml;
    }
    return butil_mime_text;
}

static void bsmtp_get_line(bsmtp_client_t *bsmtp, bsmtp_state_t next_state)
{
    bsmtp->next_state  = next_state;
    bsmtp->state       = bsmtpReadLine;
    bsmtp->line_count  = 0;
    bsmtp->reported_line_error = false;
}

static const char *bsmtp_state_name(bsmtp_state_t state)
{
    switch (state)
    {
    case bsmtpInit:             return "Init";
    case bsmtpTransportInit:    return "TransportInit";
    case bsmtpTransport:        return "Transport";
    case bsmtpBanner:           return "Banner";
    case bsmtpHello:            return "Hello";
    case bsmtpHelloReply:       return "HelloReply";
    case bsmtpHelloFeatures:    return "HelloFeatures";
    case bsmtpStartTLS:         return "StartTLS";
    case bsmtpStartTLSreply:    return "StartTLSreply";
    case bsmtpAuthenticate:     return "Authenticate";
    case bsmtpAuthenticateReply:return "AuthenticateReply";
    //case bsmtpAuthChallenge:    return "AuthenticateChallenge";
    case bsmtpLoggedIn:         return "LoggedIn";
    case bsmtpFrom:             return "From";
    case bsmtpFromReply:        return "FromReply";
    case bsmtpTo:               return "To";
    case bsmtpToReply:          return "ToReply";
    case bsmtpData:             return "Fata";
    case bsmtpDataReply:        return "DataReply";
    case bsmtpHeaders:          return "Headers";
    case bsmtpHeadersReply:     return "HeadersReply";
    case bsmtpSendBody:         return "Body";
    case bsmtpSendAttachment:   return "Attachment";
    case bsmtpBodyReply:        return "BodyReply";
    case bsmtpQuit:             return "Quit";
    case bsmtpQuitReply:        return "QuitReply";
    case bsmtpError:            return "Error";
    case bsmtpReadLine:         return "ReadLine";
    case bsmtpOutPhase:         return "OutPhase";
    case bsmtpDone:             return "Done";
    default:                    return "????";
    }
}

int bsmtp_slice(bsmtp_client_t *bsmtp)
{
    const char *ps;
    char *pd;
    int chunk;
    int result;

    result = -1;

    if (bsmtp->state != bsmtp->prev_state)
    {
        butil_log(5, "st %-20s io=%4u   last_code=%3u\n",
                bsmtp_state_name(bsmtp->state), bsmtp->io.count, bsmtp->last_code);
        bsmtp->prev_state = bsmtp->state;
    }
    switch (bsmtp->state)
    {
    case bsmtpInit:

        // open TCP connection (plain text) to smtp server
        //
        bsmtp->ctrl_stream = iostream_create_from_tcp_connection(bsmtp->host, bsmtp->port);
        if (! bsmtp->ctrl_stream)
        {
            BERROR("Can not make ctrl_stream");
            return -1;
        }
        bsmtp->tls_setup = false;
        bsmtp->state = bsmtpTransportInit;
        result = 0;
        break;

    case bsmtpTransportInit:

        // poll for writeable means connected
        //
        result = bsmtp->ctrl_stream->poll(bsmtp->ctrl_stream, writeable, 0, 10000);
        if (result < 0)
        {
            BERROR("ctrl_stream poll");
            return -1;
        }
        if (result == 0)
        {
            // todo -long timeout
            return 0;
        }
        butil_log(5, "Connected\n");
        bsmtp->state = bsmtpTransport;
        result = 0;
        break;

    case bsmtpTransport:

        // read the server's banner and move to banner when done
        //
        bsmtp_get_line(bsmtp, bsmtpBanner);
        result = 0;
        break;

    case bsmtpReadLine:

        result = bsmtp_client_input(bsmtp, 0, 10000);
        if (result)
        {
            BERROR("Connection error");
            return -1;
        }
        while (bsmtp->io.count > 0)
        {
            uint8_t next_in = bsmtp->io.data[bsmtp->io.tail++];

            bsmtp->io.count--;
            if (bsmtp->io.tail >= bsmtp->io.size)
            {
                bsmtp->io.tail = 0;
            }
            if (
                    (next_in == '\n')
                 && (bsmtp->line_count > 0)
                 && (bsmtp->line[bsmtp->line_count - 1] == '\r')
            )
            {
                // note line termination not included in result line
                bsmtp->line[bsmtp->line_count - 1] = '\0';
                bsmtp->state = bsmtp->next_state;
                bsmtp->reported_line_error = false;
                butil_log(5, "==>>Line:%s\n", bsmtp->line);
                break;
            }
            bsmtp->line[bsmtp->line_count++] = next_in;
            if (bsmtp->line_count >= BSMTP_MAX_LINE - 1)
            {
                // policy is to truncate lines that are too long
                // and let them through since they can often be ignored
                //
                if (! bsmtp->reported_line_error)
                {
                    BERROR("Line too long, truncating");
                    bsmtp->reported_line_error = true;
                }
                bsmtp->line_count--;
            }
        }
        break;

    case bsmtpOutPhase:

        if (bsmtp->io.count > 0)
        {
            result = bsmtp_client_output(bsmtp);
            if (result < 0)
            {
                return result;
            }
            result = 0;
        }
        else
        {
            // data sent, get reply unless in body phase
            //
            if (
                    bsmtp->next_state != bsmtpSendBody
                &&  bsmtp->next_state != bsmtpSendAttachment
            )
            {
                // sent a command, so fetch reply
                //
                bsmtp_get_line(bsmtp, bsmtp->next_state);
            }
            else
            {
                // sent some body/header bytes, continue
                //
                bsmtp->state = bsmtp->next_state;
            }
            result = 0;
        }
        break;

    case bsmtpBanner:

        // check the welcome message and move to hello state if ok
        //
        result = bsmtp_check_response(bsmtp, bsmtpHello, 1, 220);
        break;

    case bsmtpHello:

        // just use sender email domain for now
        pd = strchr(bsmtp->sender, '@');
        if (pd)
        {
            pd++;
        }
        result = bsmtp_send_command(bsmtp, bsmtpHelloReply, "EHLO", pd);
        break;

    case bsmtpHelloReply:

        // EHLO replies can have a bunch of lines responding, so keep reading
        // until there's no more data pending on input and then go to next state
        //
        result = bsmtp_check_response(bsmtp, bsmtpHelloFeatures, 1, 250);
        break;

    case bsmtpHelloFeatures:

        // give server some time to fill response into socket
        result = bsmtp_client_input(bsmtp, 0, 10000);
        if (result)
        {
            BERROR("Connection error");
            return -1;
        }
        if (bsmtp->io.count == 0)
        {
            bsmtp_state_t next_state;

            // if no data pending, move to next state
            //
            next_state = bsmtpAuthenticate;

            if (bsmtp->transport == bsmtpTLS && !bsmtp->tls_setup)
            {
                next_state = bsmtpStartTLS;
            }
            butil_log(5, "Hello Feeature done, go to %s\n", bsmtp_state_name(next_state));
            bsmtp->state = next_state;
            result = 0;
            break;
        }
        // parse this line to see if we care about it
        result = bsmtp_parse_feature(bsmtp);
        if (result)
        {
            break;
        }
        // more data pending in hello reply, parse it out
        bsmtp_get_line(bsmtp, bsmtpHelloReply);
        result = 0;
        break;

    case bsmtpStartTLS:

        result = bsmtp_send_command(bsmtp, bsmtpStartTLSreply, "STARTTLS", NULL);
        break;

    case bsmtpStartTLSreply:

        result = bsmtp_check_response(bsmtp, bsmtpAuthenticate, 1, 220);
        if (! result)
        {
            iostream_t *tls_stream;

            // wrap stream in a tls stream
            //
            tls_stream = iostream_tls_create_from_iostream(bsmtp->ctrl_stream, true);
            if (tls_stream)
            {
                bsmtp->ctrl_stream = tls_stream;
                bsmtp->tls_setup = true;
                bsmtp->state = bsmtpBanner;
            }
            else
            {
                butil_log(0, "Can't make TLS stream\n");
                result = 1;
            }
        }
        break;

    case bsmtpAuthenticate:

        if (bsmtp->auth_method == bsmtpLOGIN)
        {
            // default auth specified, see what server wants, if anything
            //
            // pick a method to auth, based on esmtp features and what we can do
            //
            if (bsmtp->auth_supported & bsmtpLOGIN)
            {
                bsmtp->auth_method = bsmtpLOGIN;
            }
            else if (bsmtp->auth_supported & bsmtpPLAIN)
            {
                bsmtp->auth_method = bsmtpPLAIN;
            }
            else if (bsmtp->auth_supported)
            {
                butil_log(0, "We don't support any authentication method server supports\n");
                result = -1;
                break;
            }
        }
        butil_log(5, "Authenticating, method %s\n", bsmtp_auth_method_string(bsmtp->auth_method));

        switch (bsmtp->auth_method)
        {
        case bsmtpPLAIN:
            // auth plain - put encoded credentials in the command and get single reply
            //
            result = bsmtp_format_credentials(bsmtp, bsmtp->cmdbuf, sizeof(bsmtp->cmdbuf));
            if (result < 0)
            {
                break;
            }
            result = bsmtp_send_command(bsmtp, bsmtpAuthenticateReply, "AUTH PLAIN", bsmtp->cmdbuf);
            break;

        case bsmtpLOGIN:
            // auth login - get reply, and go from there
            //
            result = bsmtp_send_command(bsmtp, bsmtpAuthenticateReply, "AUTH LOGIN", NULL);
            break;

        default:
            butil_log(0, "Unsupported authentication method\n");
            result = -1;
            break;
        }
        break;

    case bsmtpAuthenticateReply:

        result = bsmtp_check_response(bsmtp, bsmtpAuthenticateReply, 2, 334, 235);
        if (! result)
        {
            // get prompt out of line, should be "Username" or "Password
            //
            uint8_t *pp;

            result = 0;

            if (bsmtp->last_code == 235)
            {
                // success, go to logged in state
                //
                bsmtp->state = bsmtpLoggedIn;
                break;
            }
            // 334 reply, more stuff to send, see what server wants
            //
            for (pp = bsmtp->line; *pp; pp++)
            {
                if (*pp < '0' || *pp > '9')
                {
                    break;
                }
            }
            while (*pp == ' ' || *pp == '\t')
            {
                pp++;
            }
            result = butil_base64_decode((uint8_t*)bsmtp->cmdbuf, sizeof(bsmtp->cmdbuf), (char*)pp);
            if (result < 0)
            {
                BERROR("decode failed");
                break;
            }
            butil_log(5, "Auth prompt=\"%s\"\n", bsmtp->cmdbuf);

            if (! strcasecmp(bsmtp->cmdbuf, "Username:") || ! strcasecmp(bsmtp->cmdbuf, "user:"))
            {
                // encode username
                result = bsmtp_format_credentials(bsmtp, bsmtp->cmdbuf, sizeof(bsmtp->cmdbuf));
                if (result < 0)
                {
                    break;
                }
                result = bsmtp_send_command(bsmtp, bsmtpAuthenticateReply, bsmtp->cmdbuf, NULL);
            }
            else if (! strcasecmp(bsmtp->cmdbuf, "Password:") || ! strcasecmp(bsmtp->cmdbuf, "pass:"))
            {
                // base64 encode password
                result = butil_base64_encode(bsmtp->cmdbuf, sizeof(bsmtp->cmdbuf),
                        (uint8_t *)bsmtp->password, strlen(bsmtp->password), false, false);
                if (result < 0)
                {
                    BERROR("encoding failed");
                }
                butil_log(5, "pass encode %s -> %s\n", bsmtp->password, bsmtp->cmdbuf);
                result = bsmtp_send_command(bsmtp, bsmtpAuthenticateReply, bsmtp->cmdbuf, NULL);
            }
            else
            {
                butil_log(0, "Unknown prompt %s\n", bsmtp->cmdbuf);
                result = -1;
                break;
            }
        }
        break;

    case bsmtpLoggedIn:

        butil_log(4, "Logged In\n");
        bsmtp->state = bsmtpFrom;
        result = 0;
        break;

    case bsmtpFrom:

        snprintf(bsmtp->cmdbuf, sizeof(bsmtp->cmdbuf), "<%s>", bsmtp->sender);
        result = bsmtp_send_command(bsmtp, bsmtpFromReply, "MAIL FROM:", bsmtp->cmdbuf);
        break;

    case bsmtpFromReply:

        result = bsmtp_check_response(bsmtp, bsmtpTo, 1, 250);
        break;

    case bsmtpTo:

        // tokenize next recipient into cmdbuf
        pd = bsmtp->cmdbuf;
        *pd++ = '<';

        for (
                ps = bsmtp->recipient, chunk = sizeof(bsmtp->cmdbuf);

                (chunk > 3) && *ps && ! butil_is_white(*ps) && (*ps != ';') && (*ps != ',');

                chunk--
        )
        {
            *pd++ = *ps++;
        }
        // wrap recipient token in <> and null terminate
        *pd++ = '>';
        *pd = '\0';

        // skip white space and semicolons/commas to next recipient
        while (*ps && (butil_is_white(*ps) || (*ps == ';') ||( *ps == ',')))
        {
            ps++;
        }
        // point to next recipient
        bsmtp->recipient = ps;

        butil_log(5, "Add Recipient %s\n", bsmtp->cmdbuf);

        result = bsmtp_send_command(bsmtp, bsmtpToReply, "RCPT TO:", bsmtp->cmdbuf);
        break;

    case bsmtpToReply:

        // go back to To if more recipients, else start data
        result = bsmtp_check_response(bsmtp,
                (bsmtp->recipient && *bsmtp->recipient) ? bsmtpTo : bsmtpData, 1, 250);
        break;

    case bsmtpData:

        result = bsmtp_send_command(bsmtp, bsmtpDataReply, "DATA", NULL);
        break;

    case bsmtpDataReply:

        result = bsmtp_check_response(bsmtp, bsmtpHeaders, 1, 354);
        break;

    case bsmtpHeaders:

        result = bsmtp_begin_headers(bsmtp);
        if (result)
        {
            break;
        }
        result = bsmtp_append_header(bsmtp, "From: <%s>", bsmtp->sender);
        if (result)
        {
            break;
        }
        result = bsmtp_append_header(bsmtp, "To: <%s>", bsmtp->recipient);
        if (result)
        {
            break;
        }
        result = bsmtp_append_header(bsmtp, "Subject: %s", bsmtp->subject);
        if (result)
        {
            break;
        }
        butil_time_to_rfc2616_date(bsmtp->last_in_time, bsmtp->cmdbuf, sizeof(bsmtp->cmdbuf));
        result = bsmtp_append_header(bsmtp, "Date: %s", bsmtp->cmdbuf);
        if (result)
        {
            break;
        }
#if BSMTP_SUPPORT_MULTIPART
        result = bsmtp_append_header(bsmtp, "MIME-Version: 1.0", NULL);
        if (result)
        {
            break;
        }
        result = bsmtp_append_header(bsmtp,
                            "Content-Type: multipart/mixed; boundary=\"%s\"",
                            bsmtp->boundary);
        if (result)
        {
            break;
        }
        result = bsmtp_append_header(bsmtp,
                            "\r\nNote that this is a multi-part message in MIME format.\r\n");
        if (result)
        {
            break;
        }
        if (bsmtp->body_mime_type == butil_mime_bin)
        {
            // if there is no initial body, get some now to sniff body mime type
            //
            if (! bsmtp->body || bsmtp->body_count == 0)
            {
                if (bsmtp->body_callback)
                {
                    result = bsmtp->body_callback(bsmtpGetBody, bsmtp->priv, &bsmtp->body, &bsmtp->body_count);
                    if (result)
                    {
                        butil_log(1, "Body callback cancels\n");
                        break;
                    }
                }
            }
            if (bsmtp->body && bsmtp->body_count > 0)
            {
                bsmtp->body_mime_type = bsmtp_sniff_contents(bsmtp, bsmtp->body, bsmtp->body_count);
            }
            butil_log(5, "Sniffed type %s for body\n", butil_mime_string_for_content_type(bsmtp->body_mime_type));
        }
        // setup multipart for body text
        //
        result = bsmtp_append_header(bsmtp,
                            "--%s\r\nContent-Type: %s; charset=UTF-8",
                            bsmtp->boundary,
                            butil_mime_string_for_content_type(bsmtp->body_mime_type));
        if (result)
        {
            break;
        }
        result = bsmtp_append_header(bsmtp,
                            "Content-Disposition: inline");
        if (result)
        {
            break;
        }
        // blank line to start body
        //
        result = bsmtp_append_header(bsmtp, "");
        if (result)
        {
            break;
        }
#else
        // blank line to start body
        //
        result = bsmtp_append_header(bsmtp, "");
        if (result)
        {
            break;
        }
#endif
        bsmtp->state = bsmtpSendBody;
        break;

    case bsmtpHeadersReply:

        BERROR("Unused state");
        result = -1;
        break;

    case bsmtpSendBody:

        // this state gets here only when io buffer is emtpy, expcept the first time
        // when it comes with headers preloaded into normalized io buffer
        {
            int room;
            int termsize;

            if (bsmtp->body_count == 0 || bsmtp->body == NULL)
            {
                if (bsmtp->file_stream)
                {
                    // in an attachment, fill body from file
                    //
                    result = bsmtp->file_stream->poll(bsmtp->file_stream, readable, 1, 0);
                    if (result > 0)
                    {
                        // limit read count to 3/4 size of encoding buffer to ensure we can encode
                        // the data for sending if needed, so just use 1/2 size
                        //
                        if (bsmtp->attachment_encoded)
                        {
                            room = sizeof(bsmtp->cmdbuf) / 2;

                            if (room > sizeof(bsmtp->fiobuf))
                            {
                                room = sizeof(bsmtp->fiobuf);
                            }
                        }
                        else
                        {
                            room = sizeof(bsmtp->fiobuf);
                        }
                        // ensure buffer is atomically encodable by being mod 4
                        room &= ~3;

                        // read the data
                        chunk = bsmtp->file_stream->read(bsmtp->file_stream, bsmtp->fiobuf, room);
                        result = 0;
                    }
                    else
                    {
                        // poll failed
                        chunk = -1;
                    }
                    if (chunk <= 0)
                    {
                        // no more data in file
                        //
                        butil_log(5, "Attachment stream ends\n");
                        bsmtp->file_stream->close(bsmtp->file_stream);
                        bsmtp->file_stream = NULL;
                        bsmtp->body = NULL;
                        bsmtp->body_count = 0;
                        result = 0;
                    }
                    else
                    {
                        // source body data from read file contents
                        //
                        if (bsmtp->attachment_encoded)
                        {
                            // server cant take binary and data is, so encode it into cmdbuf
                            //
                            result = butil_base64_encode(
                                                        bsmtp->cmdbuf, sizeof(bsmtp->cmdbuf),
                                                        bsmtp->fiobuf, chunk,
                                                        false, false
                                                        );
                            if (result < 0)
                            {
                                BERROR("Encode failed");
                                break;
                            }
                            bsmtp->body = bsmtp->cmdbuf;
                            bsmtp->body_count = result;
                        }
                        else
                        {
                            // server is binary-safe, or the contents are text so use
                            // file io buffer directly
                            //
                            bsmtp->body = (const char *)bsmtp->fiobuf;
                            bsmtp->body_count = chunk;
                        }
                        butil_log(5, "Attachment provides %zu more\n", bsmtp->body_count);
                        result = 0;
                    }
                }
                else if (bsmtp->body_callback)
                {
                    // if there is no body, callback to get more body data
                    //
                    result = bsmtp->body_callback(bsmtpGetBody, bsmtp->priv, &bsmtp->body, &bsmtp->body_count);
                    if (result)
                    {
                        butil_log(1, "Body callback cancels\n");
                        break;
                    }
                }
                if (bsmtp->body_count == 0 || bsmtp->body == NULL)
                {
                    // no more body, move on to attachments after sending all
                    // the body data left to the server to ensure room
                    //
                    butil_log(5, "End of body, sending data\n");

                    result = bsmtp_send_command(bsmtp, bsmtpSendAttachment, NULL, NULL);
                    break;
                }
                butil_log(5, "Body %zu more added\n", bsmtp->body_count);
            }
            // this puts any existing io data to left of ring, but there
            // should not be any in here typically
            //
            iostream_normalize_ring(&bsmtp->io, NULL);

            room = bsmtp->io.size - bsmtp->io.count - 1;
            if (room > bsmtp->body_count)
            {
                room = bsmtp->body_count;
            }
            if (room > 0)
            {
                butil_log(5, "Adding %zu to send\n", room);

                // copy as much of body as will fit into io buffer and send it
                //
                memcpy(bsmtp->io.data + bsmtp->io.head, bsmtp->body, room);
                bsmtp->io.count += room;
                bsmtp->io.head += room;
                if (bsmtp->io.head >= bsmtp->io.size)
                {
                    bsmtp->io.head = 0;
                }
                bsmtp->io.data[bsmtp->io.head] = '\0';
                bsmtp->body += room;
                bsmtp->body_count -= room;
            }
            // check buffer size left and see if we need to send some data to make room
            //
            room = bsmtp->io.size - bsmtp->io.count - 1;
            termsize = 6;
            #if BSMTP_SUPPORT_MULTIPART
            termsize += strlen(bsmtp->boundary) + 4;
            #endif
            if (room < termsize)
            {
                // buffer close to full, flush to server
                //
                butil_log(5, "Flush body buffer of %d\n", bsmtp->io.count);

                result = bsmtp_send_command(bsmtp, bsmtpSendBody, NULL, NULL);
            }
            else
            {
                // room in buffer, keep filling body
                //
                result = 0;
            }
        }
        break;

    case bsmtpSendAttachment:

        #if BSMTP_SUPPORT_MULTIPART
        if (bsmtp->body_callback)
        {
            result = bsmtp->body_callback(bsmtpGetAttachment, bsmtp->priv, &bsmtp->body, &bsmtp->body_count);
            if (result)
            {
                butil_log(1, "Body callback cancels\n");
                break;
            }
        }
        if (bsmtp->io.count != 0)
        {
            butil_log(0, "Expect no data to send\n");
        }
        bsmtp_begin_headers(bsmtp);

        if (bsmtp->body == NULL || bsmtp->body[0] == '\0')
        #endif
        {
            // no more attachments, all done, io buffer should be empty
            //
            #if BSMTP_SUPPORT_MULTIPART
            result = bsmtp_append_header(bsmtp, "\r\n--%s--\r\n.", bsmtp->boundary);
            #else
            result = bsmtp_append_header(bsmtp, "\r\n.", bsmtp->boundary);
            #endif
            result = bsmtp_send_command(bsmtp, bsmtpBodyReply, NULL, NULL);
            break;
        }
        #if BSMTP_SUPPORT_MULTIPART
        // attach a file, filename is in body

        // get mime type based on file name
        bsmtp->attachment_mime_type = butil_content_type_for_file(bsmtp->body);

        // if its binary, and server isn't binary safe, encode the attachment data
        switch (bsmtp->attachment_mime_type)
        {
        case butil_mime_css:
        case butil_mime_html:
        case butil_mime_js:
        case butil_mime_json:
        case butil_mime_text:
        case butil_mime_xml:
            if (! (bsmtp->esmtp_features & besmtp_8bitmime))
            {
                // assume server is not 8 bit safe
                bsmtp->attachment_encoded = true;
            }
            else
            {
                bsmtp->attachment_encoded = false;
            }
            break;
        default:
            bsmtp->attachment_encoded = true;
            break;
        }
        butil_log(5, "Sending attachment %s type %s\n",
                bsmtp->body, butil_mime_string_for_content_type(bsmtp->attachment_mime_type));

        // try and open file
        //
        bsmtp->file_stream = iostream_create_reader_from_file(bsmtp->body);
        if (! bsmtp->file_stream)
        {
            butil_log(1, "File %s not opened\n", bsmtp->body);
            // Note: we could just not attach this and move on, but really, its an error
            result = -1;
            break;
        }
        // setup multipart for attachment
        //
        result = bsmtp_append_header(bsmtp,
                            "--%s\r\nContent-Type: %s; name=\"%s\"",
                            bsmtp->boundary,
                            butil_mime_string_for_content_type(bsmtp->attachment_mime_type),
                            bsmtp->body);
        if (result)
        {
            break;
        }
        if (bsmtp->attachment_encoded)
        {
            result = bsmtp_append_header(bsmtp,
                                "Content-Transfer-Encoding: base64");
            if (result)
            {
                break;
            }
        }
        result = bsmtp_append_header(bsmtp,
                            "Content-Disposition: attachment; filename=\"%s\"",
                            bsmtp->body);
        if (result)
        {
            break;
        }
        // blank line to start of body
        //
        result = bsmtp_append_header(bsmtp, "");
        if (result)
        {
            break;
        }
        bsmtp->state = bsmtpSendBody;
        #endif
        break;

    case bsmtpBodyReply:

        result = bsmtp_check_response(bsmtp, bsmtpQuit, 1, 250);
        break;

    case bsmtpQuit:

        result = bsmtp_send_command(bsmtp, bsmtpQuitReply, "QUIT", NULL);
        break;

    case bsmtpQuitReply:

        result = bsmtp_check_response(bsmtp, bsmtpQuit, 1, 221);
        if (result)
        {
            butil_log(0, "Ignoring QUIT reply %u\n", bsmtp->last_code);
            result = 0;
        }
        bsmtp->state = bsmtpDone;
        break;

    case bsmtpError:

        result = -1;
        break;

    case bsmtpDone:

        if (bsmtp->ctrl_stream)
        {
            bsmtp->ctrl_stream->close(bsmtp->ctrl_stream);
            bsmtp->ctrl_stream = NULL;
        }
        if (bsmtp->file_stream)
        {
            bsmtp->file_stream->close(bsmtp->file_stream);
            bsmtp->file_stream = NULL;
        }
        butil_log(5, "Done");
        result = 0;
        break;
    }
    return result;
}

int bsmtp_send_mail(
                const char  *server_url,
                bsmtp_transport_t transport,
                const char  *recipient_email,
                const char  *sender_email,
                const char  *sender_password,
                const char  *subject,
                const char  *initial_body,
                bsmtp_body_callback_t body_callback,
                void        *priv
                 )
{
    bsmtp_client_t *bsmtp;
    butil_url_scheme_t scheme;
    int result;

    if (! server_url || ! recipient_email || ! sender_email)
    {
        return -1;
    }
    bsmtp = bsmtp_client_create();
    if (! bsmtp)
    {
        butil_log(0, "Client alloc failed\n");
        return -1;
    }
    result = -1;

    bsmtp->transport = transport;
    bsmtp->esmtp_features = 0;
    bsmtp->auth_supported = 0;
    bsmtp->auth_method    = bsmtpLOGIN;

    bsmtp->recipient = recipient_email;
    bsmtp->sender = sender_email;
    bsmtp->password = sender_password;

    if (! subject)
    {
        subject = "";
    }
    if (! initial_body)
    {
        initial_body = "";
    }
    bsmtp->subject = subject;
    bsmtp->body = initial_body;
    bsmtp->body_count = strlen(initial_body);
    bsmtp->body_mime_type = butil_mime_bin;

    bsmtp->body_callback = body_callback;
    bsmtp->priv = priv;

    // parse server url
    //
    result = butil_parse_url(
                            server_url,
                            &bsmtp->scheme,
                            bsmtp->host, sizeof(bsmtp->host),
                            &bsmtp->port,
                            bsmtp->path, sizeof(bsmtp->path)
                            );
    if (result)
    {
        butil_log(0, "Bad URL %s\n", server_url);
        bsmtp_client_destroy(bsmtp);
        return result;
    }
    butil_log(5, "Host=%s Port=%u Path=%s\n", bsmtp->host, bsmtp->port, bsmtp->path);

    do
    {
        result = bsmtp_slice(bsmtp);
    }
    while (!result && bsmtp->state != bsmtpDone);

    if (bsmtp)
    {
        bsmtp_client_destroy(bsmtp);
    }
    return result;
}

void bsmtp_client_destroy(bsmtp_client_t *bsmtp)
{
    if (bsmtp)
    {
        if (bsmtp->ctrl_stream)
        {
            bsmtp->ctrl_stream->close(bsmtp->ctrl_stream);
            bsmtp->ctrl_stream = NULL;
        }
        if (bsmtp->file_stream)
        {
            bsmtp->file_stream->close(bsmtp->file_stream);
            bsmtp->file_stream = NULL;
        }
        free(bsmtp);
    }
}

bsmtp_client_t *bsmtp_client_create()
{
    bsmtp_client_t *bsmtp;
    int result;

    bsmtp = (bsmtp_client_t *)malloc(sizeof(bsmtp_client_t));
    if (bsmtp)
    {
        memset(bsmtp, 0, sizeof(bsmtp_client_t));

        bsmtp->transport = bsmtpPlainText;
        bsmtp->priv = NULL;

        bsmtp->io.data = bsmtp->iobuf;
        bsmtp->io.size = sizeof(bsmtp->iobuf);
        bsmtp->io.count = 0;
        bsmtp->io.head = 0;
        bsmtp->io.tail = 0;

        bsmtp->fio.data = bsmtp->fiobuf;
        bsmtp->fio.size = sizeof(bsmtp->fiobuf);
        bsmtp->fio.count = 0;
        bsmtp->fio.head = 0;
        bsmtp->fio.tail = 0;

        bsmtp->line_count = 0;
        bsmtp->long_timeout = 15;

        bsmtp->state = bsmtpInit;
        bsmtp->next_state = bsmtpInit;
        bsmtp->prev_state = bsmtpInit;
        bsmtp->num_codes = 0;

        bsmtp->ctrl_stream = NULL;
        bsmtp->file_stream = NULL;

        strncpy(bsmtp->boundary, "bnetbnetbnetbnet", BSMTP_MAX_BOUNDARY);
        bsmtp->boundary[BSMTP_MAX_BOUNDARY - 1] = '\0';
    }
    return bsmtp;
}

