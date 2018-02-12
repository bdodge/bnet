
#include "bftp.h"

static int bftp_check_response(bftpcontext_t *bftp, bftp_state_t next_state, size_t numcodes, ...)
{
    va_list args;
    size_t i;

    bftp->state = bftpInPhase;
    bftp->next_state = next_state;

    iostream_normalize_ring(&bftp->io, NULL);

    va_start(args, numcodes);
    for (i = 0; i < numcodes && i < BFTP_MAX_EXPECTED_CODES; i++)
    {
        bftp->expected_codes[i] = (uint32_t)va_arg(args, unsigned);
    }
    bftp->num_codes = i;
    return 0;
}

static int bftp_check_response_check(bftpcontext_t *bftp)
{
    int result;
    size_t i;

    if (bftp->io.count < 3)
    {
        bftp->state = bftpDone;
        return -1;
    }
    bftp->last_code = strtoul((char*)bftp->io.data + bftp->io.tail, NULL, 0);

    for (i = 0; i < bftp->num_codes && i < BFTP_MAX_EXPECTED_CODES; i++)
    {
        if (bftp->last_code == bftp->expected_codes[i])
        {
            butil_log(5, "Code %u is OK\n", bftp->expected_codes[i]);
            bftp->num_codes = 0;
            bftp->state = bftp->next_state;
            bftp->next_state = bftpInit;
            return 0;
        }
    }
    butil_log(5, "Error: got code %u\n", bftp->last_code);
    bftp->state = bftpDone;
    bftp->num_codes = 0;
    return 0;
}

static int bftp_send_command(bftpcontext_t *bftp, bftp_state_t next_state, const char *cmd, const char *data)
{
    int len;

    len = snprintf((char *)bftp->io.data, bftp->io.size,
            "%s%s%s\r\n", cmd,
            (data && *data) ? " " : "", (data && *data) ? data : "");

    if (len < 0 || len >= bftp->io.size)
    {
        BERROR("Comman overflow");
        return -1;
    }
    bftp->io.count = len;
    bftp->io.head = len;
    bftp->io.tail = 0;

    bftp->next_state = next_state;
    bftp->state = bftpOutPhase;
    return 0;
}

static int sbftp_number_from_io(bftpcontext_t *bftp, uint32_t *num)
{
    size_t i;
    uint32_t val;
    uint8_t c;

    if (! bftp || ! num)
    {
        return -1;
    }
    if (bftp->io.count < 1)
    {
        return -1;
    }
    val = 0;

    while (bftp->io.count > 0)
    {
        c = bftp->io.data[bftp->io.tail];
        if (c < '0' || c > '9')
        {
            break;
        }
        bftp->io.tail++;
        bftp->io.count--;
        val = val * 10;
        val = val + (c - '0');
    }
    while (bftp->io.count > 0)
    {
        c = bftp->io.data[bftp->io.tail];
        if (c >= '0' && c <= '9')
        {
            break;
        }
        bftp->io.tail++;
        bftp->io.count--;
    }
    *num = val;
    return 0;
}

static int bftp_begin_passive(bftpcontext_t *bftp)
{
    const char *pip;
    uint32_t num;
    size_t i;
    uint8_t ipaddr[4];
    uint8_t ipport[2];
    char ipaddr_string[32];
    uint16_t port;
    int result;

    // look forward in reply buffer for the set (ip,ip,ip,ip,pt,pt)
    //
    for (i = 3; i < bftp->io.count; i++)
    {
        if (bftp->io.data[i] == '(')
        {
            break;
        }
    }
    if (i >= bftp->io.count)
    {
        return -1;
    }
    for (i++; i < bftp->io.count; i++)
    {
        if (bftp->io.data[i] >= '0' && bftp->io.data[i] <= '9')
        {
            break;
        }
    }
    if (i >= bftp->io.count)
    {
        return -1;
    }
    bftp->io.tail = i;
    bftp->io.count-= i;

    for (i = 0; i < 4; i++)
    {
        result = sbftp_number_from_io(bftp, &num);
        if (result)
        {
            return result;
        }
        ipaddr[i] = num;
    }
    for (i = 0; i < 2; i++)
    {
        result = sbftp_number_from_io(bftp, &num);
        if (result)
        {
            return result;
        }
        ipport[i] = num;
    }
    snprintf(ipaddr_string, sizeof(ipaddr_string), "%d.%d.%d.%d",
            ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);

    port = ((uint16_t)ipport[0] << 8) | (uint16_t)ipport[1];

    butil_log(5, "Passive Host: %s : %u\n", ipaddr_string, port);

    bftp->data_stream = iostream_create_from_tcp_connection(ipaddr_string, port);
    if (! bftp->data_stream)
    {
        BERROR("Can not make data_stream");
        return -1;
    }
    return 0;
}

int sbftp_list_items(bftpcontext_t *bftp)
{
    char item[BFTP_MAX_LIST_ITEM];
    size_t i;
    char c;
    int result;

    result = 0;

    while (bftp->io.count > 0)
    {
        for (i = 0; bftp->io.count > 0;)
        {
            if (i >= BFTP_MAX_LIST_ITEM)
            {
                BERROR("item overflow");
                return -1;
            }
            c = (char)bftp->io.data[bftp->io.tail];
            if (c == '\n')
            {
                if (i > 0)
                {
                    item[i] = '\0';
                    if (bftp->callback)
                    {
                        result = bftp->callback(item, bftp->priv);
                    }
                    break;
                }
            }
            else if (c != '\r')
            {
                item[i++] = c;
            }
            bftp->io.tail++;
            if (bftp->io.tail >= bftp->io.size)
            {
                bftp->io.tail = 0;
            }
            bftp->io.count--;
        }
    }
    // if not a complete line, push back into io ring (rare if ever)
    //
    while (i > 0)
    {
        if (bftp->io.tail == 0)
        {
            bftp->io.tail = bftp->io.size - 1;
        }
        else
        {
            bftp->io.tail--;
        }
        bftp->io.data[bftp->io.tail] = item[--i];
        bftp->io.count++;
    }
    return result;
}

const char *bftp_state_name(bftp_state_t state)
{
    switch (state)
    {
    case bftpInit:              return "Init";
    case bftpControlTransportInit:return "ControlTransportInit";
    case bftpControlTransport:  return "ControlTransport";
    case bftpLogin:             return "Login";
    case bftpLoginReply:        return "LoginReply";
    case bftpAuthenticate:      return "Authenticate";
    case bftpAuthenticateReply: return "AuthenticateReply";
    case bftpLoggedIn:          return "LoggedIn";
    case bftpPassiveReply:      return "PassiveReply";
    case bftpSetBinary:         return "SetBinary";
    case bftpBinaryReply:       return "BinaryReply";
    case bftpDataTransport:     return "DataTransport";
    case bftpMethod:            return "Method";
    case bftpMethodReply:       return "MethodReply";
    case bftpMethodDispatch:    return "MethodDispatch";
    case bftpDispatchReply:     return "DispatchReply";
    case bftpUpload:            return "Upload";
    case bftpListing:           return "Listing";
    case bftpDownload:          return "Download";
    case bftpTransferReply:     return "TransferReply";
    case bftpInPhase:           return "InPhase";
    case bftpOutPhase:          return "OutPhase";
    case bftpDone:              return "Done";
    default:                    return "????";
    }
}

int bftp_slice(bftpcontext_t *bftp)
{
    int chunk;
    int result;
    static bftp_state_t prevstate = bftpInit;

    result = -1;

    if (bftp->state != prevstate)
    {
        butil_log(5, "st %-20s io=%4u   last_code=%3u\n",
                bftp_state_name(bftp->state), bftp->io.count, bftp->last_code);
        prevstate = bftp->state;
    }
    switch (bftp->state)
    {
    case bftpInit:

        bftp->ctrl_stream = iostream_create_from_tcp_connection(bftp->host, bftp->port);
        if (! bftp->ctrl_stream)
        {
            BERROR("Can not make ctrl_stream");
            return -1;
        }
        bftp->state = bftpControlTransportInit;
        result = 0;
        break;

    case bftpControlTransportInit:

        // poll for writeable means connected
        //
        result = bftp->ctrl_stream->poll(bftp->ctrl_stream, writeable, 0, 10000);
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
        bftp->state = bftpControlTransport;
        result = 0;
        break;

    case bftpControlTransport:

        // check the welcome message and move to login state if ok
        //
        result = bftp_check_response(bftp, bftpLogin, 1, 220);
        break;

    case bftpLogin:

        result = bftp_send_command(bftp, bftpLoginReply, "USER", bftp->username);
        break;

    case bftpLoginReply:

        result = bftp_check_response(bftp, bftpAuthenticate, 3, 230, 331, 332);
        break;

    case bftpAuthenticate:

        result = bftp_send_command(bftp, bftpAuthenticateReply, "PASS", bftp->password);
        break;

    case bftpAuthenticateReply:

        result = bftp_check_response(bftp, bftpLoggedIn, 3, 202, 230, 332);
        break;

    case bftpLoggedIn:

        butil_log(4, "Logged In as %s OK (%3u)\n", bftp->username, bftp->last_code);
        if (bftp->usepassive && bftp->method != bftpDel)
        {
            result = bftp_send_command(bftp, bftpPassiveReply, "PASV", NULL);
        }
        else
        {
            bftp->state = bftpSetBinary;
            result = 0;
        }
        break;

    case bftpPassiveReply:

        result = bftp_check_response(bftp, bftpSetBinary, 1, 227);
        break;

    case bftpSetBinary:

        if (bftp->usepassive && bftp->method != bftpDel)
        {
            result = bftp_begin_passive(bftp);
            if (result)
            {
                bftp->state = bftpDone;
                break;
            }
        }
        result = bftp_send_command(bftp, bftpBinaryReply, "TYPE", "I");
        //bftp->state =  bftpDataTransport; // for testing
        break;

    case bftpBinaryReply:

        result = bftp_check_response(bftp,
            ((bftp->method == bftpDel) ? bftpMethod : bftpDataTransport), 2, 200, 226);
        break;

    case bftpDataTransport:

        if (! bftp->usepassive)
        {
            bftp->state = bftpMethod;
            result = 0;
            break;
        }
        // poll for writeable means connected
        //
        result = bftp->ctrl_stream->poll(bftp->data_stream, writeable, 0, 10000);
        if (result < 0)
        {
            BERROR("data_stream poll");
            return -1;
        }
        if (result == 0)
        {
            // todo -long timeout
            return 0;
        }
        butil_log(5, "Data Connected\n");
        bftp->state = bftpMethod;
        result = 0;
        break;

    case bftpMethod:

        switch (bftp->method)
        {
        case bftpGet:
            result = bftp_send_command(bftp, bftpMethodReply, "RETR", bftp->path + 1);
            break;

        case bftpPut:
            result = bftp_send_command(bftp, bftpMethodReply, "STOR", bftp->path + 1);
            break;

        case bftpDir:
            result = bftp_send_command(bftp, bftpMethodReply, "LIST", bftp->path + 1);
            break;

        case bftpDel:
            result = bftp_send_command(bftp, bftpMethodDispatch, "DELE", bftp->path + 1);
            break;

        default:
            BERROR("Bad state");
            bftp->state = bftpDone;
            break;
        }
        result= 0;
        break;

    case bftpMethodReply:

        result = bftp_check_response(bftp, bftpMethodDispatch, 1, 150);
        break;

    case bftpMethodDispatch:

        // prep io ring for data xfer
        //
        bftp->io.tail = 0;
        bftp->io.head = 0;
        bftp->io.count = 0;

        bftp->xfer_data_total = 0;
        bftp->xfer_file_total = 0;

        switch (bftp->method)
        {
        case bftpGet:
            bftp->state = bftpDownload;
            break;

        case bftpPut:
            bftp->state = bftpUpload;
            break;

        case bftpDir:
            bftp->state = bftpListing;
            break;

        case bftpDel:
            result = bftp_check_response(bftp, bftpDispatchReply, 1, 250);
            break;

        default:
            BERROR("Bad state");
            bftp->state = bftpDone;
            break;
        }
        result= 0;
        break;

    case bftpDispatchReply:

        butil_log(4, "Method Complete\n");
        bftp->state = bftpDone;
        result = 0;
        break;

    case bftpUpload:

        if (! bftp->file_stream)
        {
            result = -1;
            break;
        }
        if (bftp->io.head >= bftp->io.tail && bftp->io.count < bftp->io.size)
        {
            chunk = bftp->io.size - bftp->io.head;
        }
        else
        {
            chunk = bftp->io.tail - bftp->io.head;
        }
        if (chunk > 0)
        {
            result = bftp->file_stream->read(bftp->file_stream, bftp->io.data + bftp->io.head, chunk);
            if (result < 0)
            {
                BERROR("file read");
                break;
            }
            if (result == 0)
            {
                // end of file
                //
                if (bftp->io.count == 0)
                {
                    // and all written out
                    //
                    butil_log(4, "data_stream EOF, Read Done\n");
                    bftp->data_stream->close(bftp->data_stream);
                    bftp->data_stream = NULL;
                    result = bftp_check_response(bftp, bftpTransferReply, 1, 226);
                    return result;
                }
            }
            bftp->io.head += result;
            if (bftp->io.head >= bftp->io.size)
            {
                bftp->io.head = 0;
            }
            bftp->io.count += result;
            bftp->xfer_file_total += result;
        }
        result = bftp->data_stream->poll(bftp->data_stream, writeable, 0, 10000);
        if (result < 0)
        {
            BERROR("data_stream poll");
            return -1;
        }
        if (result == 0)
        {
            // stream busy, check on ctrl stream for cancel
            //
            result = bftp->ctrl_stream->poll(bftp->ctrl_stream, readable, 0, 10000);
            if (result < 0)
            {
                BERROR("ctrl_stream poll while data poll");
                return -1;
            }
            if (result > 0)
            {
                // got ctrl data while polling for more xfer data, go check it and finish
                //
                result = bftp_check_response(bftp, bftpTransferReply, 1, 226);
                return result;
            }
            // todo -long timeout on data
            return 0;
        }
        if (bftp->io.head >= bftp->io.tail)
        {
            chunk = bftp->io.count;
        }
        else
        {
            chunk = bftp->io.size - bftp->io.tail;
        }
        if (chunk > 0)
        {
            result = bftp->data_stream->write(bftp->data_stream, bftp->io.data + bftp->io.tail, chunk);
            if (result < 0)
            {
                BERROR("data_stream write");
                bftp->state = bftpDone;
                result = -1;
                break;
            }
            if (result == 0)
            {
                butil_log(4, "data_stream closed, Write Done\n");
                result = bftp_check_response(bftp, bftpTransferReply, 1, 226);
                return result;
            }
            butil_log(6, "Put %d Data, chunk=%u\n", result, chunk);

            bftp->io.count -= result;
            bftp->io.tail += result;
            if (bftp->io.tail >= bftp->io.size)
            {
                bftp->io.tail = 0;
            }
            if (bftp->io.count == 0)
            {
                bftp->io.head = 0;
                bftp->io.tail = 0;
            }
            bftp->xfer_data_total += result;
        }
        result = 0;
        break;

    case bftpListing:

        result = bftp->data_stream->poll(bftp->data_stream, readable, 0, 10000);
        if (result < 0)
        {
            BERROR("data_stream poll");
            return -1;
        }
        if (result == 0)
        {
            // no data (yet), check on ctrl stream for cancel
            //
            result = bftp->ctrl_stream->poll(bftp->ctrl_stream, readable, 0, 10000);
            if (result < 0)
            {
                BERROR("ctrl_stream poll while data poll");
                return -1;
            }
            if (result > 0)
            {
                // got ctrl data while polling for more xfer data, go check it and finish
                //
                result = bftp_check_response(bftp, bftpDispatchReply, 3, 150, 200, 226);
                return result;
            }
            // todo -long timeout on data
            return 0;
        }
        if (bftp->io.head >= bftp->io.tail && bftp->io.count < bftp->io.size)
        {
            chunk = bftp->io.size - bftp->io.head;
        }
        else
        {
            chunk = bftp->io.tail - bftp->io.head;
        }
        if (chunk > 0)
        {
            result = bftp->data_stream->read(bftp->data_stream, bftp->io.data + bftp->io.head, chunk);
            if (result < 0)
            {
                BERROR("data_stream read");
                bftp->state = bftpDone;
                result = -1;
                break;
            }
            if (result == 0)
            {
                butil_log(4, "data_stream closed, Read Done\n");
                result = bftp_check_response(bftp, bftpTransferReply, 1, 226);
                return result;
            }
            butil_log(5, "Listed %u Data\n", result);

            bftp->io.count += result;
            bftp->io.head += result;
            if (bftp->io.head >= bftp->io.size)
            {
                bftp->io.head = 0;
            }
            bftp->xfer_data_total += result;
        }
        if (bftp->io.count > 0)
        {
            result = sbftp_list_items(bftp);
        }
        else
        {
            result = 0;
        }
        break;

    case bftpDownload:

        result = bftp->data_stream->poll(bftp->data_stream, readable, 0, 10000);
        if (result < 0)
        {
            BERROR("data_stream poll");
            return -1;
        }
        if (result == 0)
        {
            // no data (yet), check on ctrl stream for cancel
            //
            result = bftp->ctrl_stream->poll(bftp->ctrl_stream, readable, 0, 10000);
            if (result < 0)
            {
                BERROR("ctrl_stream poll while data poll");
                return -1;
            }
            if (result > 0)
            {
                // got ctrl data while polling for more xfer data, go check it and finish
                //
                result = bftp_check_response(bftp, bftpTransferReply, 1, 226);
                return result;
            }
            // todo -long timeout on data
            return 0;
        }
        if (bftp->io.head >= bftp->io.tail && bftp->io.count < bftp->io.size)
        {
            chunk = bftp->io.size - bftp->io.head;
        }
        else
        {
            chunk = bftp->io.tail - bftp->io.head;
        }
        if (chunk > 0)
        {
            result = bftp->data_stream->read(bftp->data_stream, bftp->io.data + bftp->io.head, chunk);
            if (result < 0)
            {
                BERROR("data_stream read");
                bftp->state = bftpDone;
                result = -1;
                break;
            }
            if (result == 0)
            {
                butil_log(4, "data_stream closed, Read Done\n");
                result = bftp_check_response(bftp, bftpTransferReply, 1, 226);
                return result;
            }
            butil_log(6, "Got %u Data\n", result);

            bftp->io.count += result;
            bftp->io.head += result;
            if (bftp->io.head >= bftp->io.size)
            {
                bftp->io.head = 0;
            }
            bftp->xfer_data_total += result;
        }
        if (bftp->file_stream)
        {
            if (bftp->io.head >= bftp->io.tail)
            {
                chunk = bftp->io.count;
            }
            else
            {
                chunk = bftp->io.size - bftp->io.tail;
            }
            if (chunk > 0)
            {
                result = bftp->file_stream->write(bftp->file_stream,
                            bftp->io.data + bftp->io.tail, chunk);
                if (result < 0)
                {
                    BERROR("file write");
                    break;
                }
                bftp->io.tail += result;
                if (bftp->io.tail >= bftp->io.size)
                {
                    bftp->io.tail = 0;
                }
                bftp->io.count -= result;
                if (bftp->io.count == 0)
                {
                    bftp->io.head = 0;
                    bftp->io.tail = 0;
                }
                bftp->xfer_file_total += result;
            }
        }
        else
        {
            // testing only
            bftp->xfer_file_total += bftp->io.count;
            bftp->io.count = 0;
            bftp->io.head = 0;
            bftp->io.tail = 0;
        }
        result = 0;
        break;

    case bftpTransferReply:

        butil_log(4, "Transfer Complete: %u bytes data, %u file\n",
                            bftp->xfer_data_total, bftp->xfer_file_total);
        bftp->state = bftpDone;
        result = 0;
        break;

    case bftpInPhase:

        result = bftp->ctrl_stream->poll(bftp->ctrl_stream, readable, 0, 10000);
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
        iostream_normalize_ring(&bftp->io, NULL);
        chunk = bftp->io.size - bftp->io.count;
        result = bftp->ctrl_stream->read(bftp->ctrl_stream, bftp->io.data + bftp->io.head, chunk);
        if (result <= 0)
        {
            BERROR("ctrl_stream read");
            bftp->state = bftpDone;
            result = -1;
            break;
        }
        bftp->io.count += result;
        bftp->io.head += result;
        if (bftp->io.head >= bftp->io.size)
        {
            bftp->io.head = 0;
        }
        result = bftp_check_response_check(bftp);
        break;

    case bftpOutPhase:

        result = bftp->ctrl_stream->poll(bftp->ctrl_stream, writeable, 0, 10000);
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
        result = bftp->ctrl_stream->write(bftp->ctrl_stream, bftp->io.data + bftp->io.tail, bftp->io.count);
        if (result <= 0)
        {
            BERROR("ctrl_stream write");
            bftp->state = bftpDone;
            result = -1;
            break;
        }
        bftp->io.count -= result;
        bftp->io.tail += result;
        if (bftp->io.tail >= bftp->io.size)
        {
            bftp->io.tail = 0;
        }
        if (bftp->io.count == 0)
        {
            bftp->state = bftp->next_state;
            bftp->next_state = bftpInit;
        }
        result = 0;
        break;

    case bftpDone:

        if (bftp->ctrl_stream)
        {
            bftp->ctrl_stream->close(bftp->ctrl_stream);
            bftp->ctrl_stream = NULL;
        }
        if (bftp->data_stream)
        {
            bftp->data_stream->close(bftp->data_stream);
            bftp->data_stream = NULL;
        }
        if (bftp->file_stream)
        {
            bftp->file_stream->close(bftp->file_stream);
            bftp->file_stream = NULL;
        }
        butil_log(5, "Done");
        result = 0;
        break;
    }
    return result;
}

int bftp_get_file(
                    const char  *remotepath,
                    const char  *localpath,
                    const char  *username,
                    const char  *password
                 )
{
    bftpcontext_t *bftp;
    butil_url_scheme_t scheme;
    int result;

    if (! remotepath || ! localpath)
    {
        return -1;
    }
    if (strlen(username) >= sizeof(bftp->username) - 1)
    {
        BERROR("username length");
        return -1;
    }
    if (strlen(password) >= sizeof(bftp->password) - 1)
    {
        BERROR("password length");
        return -1;
    }

    bftp = bftp_client_create();
    if (! bftp)
    {
        return -1;
    }
    result = -1;

    strcpy(bftp->username, username);
    strcpy(bftp->password, password);

    // open local file
    //
    bftp->file_stream = iostream_create_writer_from_file(localpath);
    if (! bftp->file_stream)
    {
        butil_log(1, "Can't open local file: %s\n", localpath);
        bftp_client_destroy(bftp);
        return -1;
    }

    // split remote path up
    //
    result = butil_parse_url(
                            remotepath,
                            &scheme,
                            bftp->host, sizeof(bftp->host),
                            &bftp->port,
                            bftp->path, sizeof(bftp->path)
                            );
    if (result)
    {
        bftp_client_destroy(bftp);
        return result;
    }
    butil_log(5, "Host=%s Port=%u Path=%s\n", bftp->host, bftp->port, bftp->path);

    bftp->method = bftpGet;

    do
    {
        result = bftp_slice(bftp);
    }
    while (! result && bftp->state != bftpDone);

    if (bftp)
    {
        bftp_client_destroy(bftp);
    }
    return result;
}

int bftp_put_file(
                    const char  *remotepath,
                    const char  *localpath,
                    const char  *username,
                    const char  *password
                 )
{
    bftpcontext_t *bftp;
    butil_url_scheme_t scheme;
    int result;

    if (! remotepath || ! localpath)
    {
        return -1;
    }
    if (strlen(username) >= sizeof(bftp->username) - 1)
    {
        BERROR("username length");
        return -1;
    }
    if (strlen(password) >= sizeof(bftp->password) - 1)
    {
        BERROR("password length");
        return -1;
    }

    bftp = bftp_client_create();
    if (! bftp)
    {
        return -1;
    }
    result = -1;

    strcpy(bftp->username, username);
    strcpy(bftp->password, password);

    // open local file
    //
    bftp->file_stream = iostream_create_reader_from_file(localpath);
    if (! bftp->file_stream)
    {
        butil_log(1, "Can't open local file: %s\n", localpath);
        bftp_client_destroy(bftp);
        return -1;
    }

    // split remote path up
    //
    result = butil_parse_url(
                            remotepath,
                            &scheme,
                            bftp->host, sizeof(bftp->host),
                            &bftp->port,
                            bftp->path, sizeof(bftp->path)
                            );
    if (result)
    {
        bftp_client_destroy(bftp);
        return result;
    }
    butil_log(5, "Host=%s Port=%u Path=%s\n", bftp->host, bftp->port, bftp->path);

    bftp->method = bftpPut;
    do
    {
        result = bftp_slice(bftp);
    }
    while (! result && bftp->state != bftpDone);

    if (bftp)
    {
        bftp_client_destroy(bftp);
    }
    return result;
}

int bftp_list_directory(
                    const char  *remotepath,
                    const char  *username,
                    const char  *password,
                    bftp_listing_cb_t *callback,
                    void        *priv
                 )
{
    bftpcontext_t *bftp;
    butil_url_scheme_t scheme;
    int result;

    if (! remotepath)
    {
        return -1;
    }
    if (strlen(username) >= sizeof(bftp->username) - 1)
    {
        BERROR("username length");
        return -1;
    }
    if (strlen(password) >= sizeof(bftp->password) - 1)
    {
        BERROR("password length");
        return -1;
    }

    bftp = bftp_client_create();
    if (! bftp)
    {
        return -1;
    }
    result = -1;

    strcpy(bftp->username, username);
    strcpy(bftp->password, password);

    bftp->callback = callback;
    bftp->priv = priv;

    // split remote path up
    //
    result = butil_parse_url(
                            remotepath,
                            &scheme,
                            bftp->host, sizeof(bftp->host),
                            &bftp->port,
                            bftp->path, sizeof(bftp->path)
                            );
    if (result)
    {
        bftp_client_destroy(bftp);
        return result;
    }
    butil_log(5, "Host=%s Port=%u Path=%s\n", bftp->host, bftp->port, bftp->path);

    bftp->method = bftpDir;
    do
    {
        result = bftp_slice(bftp);
    }
    while (! result && bftp->state != bftpDone);

    if (bftp)
    {
        bftp_client_destroy(bftp);
    }
    return result;
}

int bftp_delete_file(
                    const char  *remotepath,
                    const char  *username,
                    const char  *password
                 )
{
    bftpcontext_t *bftp;
    butil_url_scheme_t scheme;
    int result;

    if (! remotepath)
    {
        return -1;
    }
    if (strlen(username) >= sizeof(bftp->username) - 1)
    {
        BERROR("username length");
        return -1;
    }
    if (strlen(password) >= sizeof(bftp->password) - 1)
    {
        BERROR("password length");
        return -1;
    }

    bftp = bftp_client_create();
    if (! bftp)
    {
        return -1;
    }
    result = -1;

    strcpy(bftp->username, username);
    strcpy(bftp->password, password);

    // split remote path up
    //
    result = butil_parse_url(
                            remotepath,
                            &scheme,
                            bftp->host, sizeof(bftp->host),
                            &bftp->port,
                            bftp->path, sizeof(bftp->path)
                            );
    if (result)
    {
        bftp_client_destroy(bftp);
        return result;
    }
    butil_log(5, "Host=%s Port=%u Path=%s\n", bftp->host, bftp->port, bftp->path);

    bftp->method = bftpDel;
    do
    {
        result = bftp_slice(bftp);
    }
    while (! result && bftp->state != bftpDone);

    if (bftp)
    {
        bftp_client_destroy(bftp);
    }
    return result;
}

void bftp_client_destroy(bftpcontext_t *bftp)
{
    if (bftp)
    {
        if (bftp->ctrl_stream)
        {
            bftp->ctrl_stream->close(bftp->ctrl_stream);
            bftp->ctrl_stream = NULL;
        }
        if (bftp->data_stream)
        {
            bftp->data_stream->close(bftp->data_stream);
            bftp->data_stream = NULL;
        }
        if (bftp->file_stream)
        {
            bftp->file_stream->close(bftp->file_stream);
            bftp->file_stream = NULL;
        }
        free(bftp);
    }
}

bftpcontext_t *bftp_client_create()
{
    bftpcontext_t *bftp;
    int result;

    bftp = (bftpcontext_t *)malloc(sizeof(bftpcontext_t));
    if (bftp)
    {
        memset(bftp, 0, sizeof(bftpcontext_t));

        bftp->method = bftpGet;
        bftp->usepassive = true;

        bftp->callback = NULL;
        bftp->priv = NULL;

        bftp->io.data = bftp->iobuf;
        bftp->io.size = sizeof(bftp->iobuf);
        bftp->io.count = 0;
        bftp->io.head = 0;
        bftp->io.tail = 0;

        bftp->state = bftpInit;
        bftp->next_state = bftpInit;
        bftp->num_codes = 0;

        bftp->ctrl_stream = NULL;
        bftp->data_stream = NULL;
        bftp->file_stream = NULL;
            }
    return bftp;
}

