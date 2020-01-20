/*
 * Copyright 2020 Brian Dodge
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

#include "bmtp.h"

static const char *ptp_state_name(ptp_connection_t *ptpx)
{
    switch (ptpx->state)
    {
	case ptpReadCommand:		return "ReadCommand";
	case ptpReadCommandPayload:	return "ReadCommandPayload";
	case ptpReadEventPayload:	return "ReadEventPayload";
	case ptpReadDataPayload:	return "ReadDataPayload";
	case ptpCommand:			return "Command";
	case ptpSetupEvent:			return "SetupEvent";
	case ptpEvent:				return "Event";
	case ptpRun:				return "Run";
	case ptpError:				return "Error";
    }
    return "???";
}

int ptp_emit_event(ptp_connection_t *ptpx, uint8_t *event, size_t eventlen)
{
    uint32_t length, type;
    int rc;

    if (ptpx->event_sock >= 0)
    {
        length = 8 + eventlen;
        type = PTP_EVENT_REQ;
        rc = ptp_write(ptpx->event_sock, &ptpx->evtout, (uint8_t*)&length, 4);
        rc = ptp_write(ptpx->event_sock, &ptpx->evtout, (uint8_t*)&type, 4);
        rc = ptp_write(ptpx->event_sock, &ptpx->evtout, event, eventlen);
        rc = ptp_flush_out(ptpx->event_sock, &ptpx->evtout, 5, 0);
    }

    return rc;
}

static int serve_client(ptp_t *ptp, ptp_connection_t *ptpx, bool secure)
{
    struct sockaddr_in evt_addr;
    socklen_t evtlen;
    int flags;

    bool connected;
    int result;

    int tos, tous;

    uint32_t length;
    uint32_t type;

    // setup command io ring
    //
    ptpx->cmdin.size = sizeof(ptpx->cmdinbuf);
    ptpx->cmdin.data = ptpx->cmdinbuf;
    ptpx->cmdin.count = 0;
    ptpx->cmdin.head = 0;
    ptpx->cmdin.tail = 0;

    ptpx->cmdout.size = sizeof(ptpx->cmdoutbuf);
    ptpx->cmdout.data = ptpx->cmdoutbuf;
    ptpx->cmdout.count = 0;
    ptpx->cmdout.head = 0;
    ptpx->cmdout.tail = 0;

    // setup event io ring
    //
    ptpx->event_sock = INVALID_SOCKET;

    ptpx->evtin.size = sizeof(ptpx->evtinbuf);
    ptpx->evtin.data = ptpx->evtinbuf;
    ptpx->evtin.count = 0;
    ptpx->evtin.head = 0;
    ptpx->evtin.tail = 0;

    ptpx->evtout.size = sizeof(ptpx->evtoutbuf);
    ptpx->evtout.data = ptpx->evtoutbuf;
    ptpx->evtout.count = 0;
    ptpx->evtout.head = 0;
    ptpx->evtout.tail = 0;

    // start in run state
    //
    ptpx->state = ptpRun;

    connected = true;

    do
    {
        if (butil_get_log_level() > 0)
        {
            static uint32_t prevcmdin, prevcmdout, prevevtin, prevevtout, prevstate;

            if (
                    (uint32_t)ptpx->state != prevstate
                ||  ptpx->cmdin.count != prevcmdin
                ||  ptpx->cmdout.count != prevcmdout
                ||  ptpx->evtin.count != prevevtin
                ||  ptpx->evtout.count != prevevtout
            )
            {
                prevstate = (uint32_t)ptpx->state;
                prevcmdin = ptpx->cmdin.count;
                prevcmdout = ptpx->cmdout.count;
                prevevtin = ptpx->evtin.count;
                prevevtout = ptpx->evtout.count;
                butil_log(3, "St %d %s cmdin=%u cmdout=%u  evtin=%u evtout=%u\n",
                       ptpx->state, ptp_state_name(ptpx), ptpx->cmdin.count, ptpx->cmdout.count,
                       ptpx->evtin.count, ptpx->evtout.count);
            }
        }
        // check on input, event, and output sockets, in that order
        //
        if (ptpx->state == ptpReadCommandPayload)
        {
            tos = (ptpx->cmdin.count == 0) ? 2 : 0;
        }
        else
        {
            tos = 0;
        }

        if (
                ptpx->cmdin.count == 0
            &&  ptpx->cmdout.count == 0
            &&  ptpx->evtin.count == 0
            &&  ptpx->evtout.count == 0
        )
        {
            // all quiet, poll slowly for input and event
            tous = 5000;
        }
        else
        {
            // busy, put a minimal wait in
            tous = 2000;
        }

		// maybe poll client and event socket together?
		//
        result = ptp_fill_in(ptpx->client_sock, &ptpx->cmdin, tos, tous);
        if (result < 0)
        {
            butil_log(3, "End of cmd connection\n");
            connected = false;
            break;
        }

		if (ptpx->event_sock != INVALID_SOCKET)
		{
	        if (ptpx->state == ptpReadEventPayload)
	        {
	            tos = (ptpx->evtin.count == 0) ? 2 : 0;
				tous = (ptpx->evtin.count == 0) ? 0 : 2000;
	        }
	        else
	        {
	            tos = 0;
	            tous = 0;
	        }

	        result = ptp_fill_in(ptpx->event_sock, &ptpx->evtin, tos, tous);
	        if (result < 0)
	        {
	            // intitiator closed eventing, which is OK we suppose
	            //
	            butil_log(3, "end of evt connection\n");
	            close_socket(ptpx->event_sock);
	            ptpx->event_sock = INVALID_SOCKET;
	        }

			result = ptp_flush_out(ptpx->event_sock, &ptpx->evtout, 5, 0);
			if (result < 0)
			{
				connected = 0;
				break;
			}
		}

        // flush pending outputs
        //
        result = ptp_flush_out(ptpx->client_sock, &ptpx->cmdout, 5, 0);
        if (result < 0)
        {
            connected = 0;
            break;
        }

        switch (ptpx->state)
        {
        case ptpCommand:

            switch (ptpx->cmdtype)
            {
            case PTP_INIT_REQ:
                // start of an init sequence, clean up
                //
                if (ptpx->event_sock != INVALID_SOCKET)
                {
                    close_socket(ptpx->event_sock);
                    ptpx->event_sock = INVALID_SOCKET;
                }
                iostream_reset_ring(&ptpx->cmdout);
                iostream_reset_ring(&ptpx->evtin);
                iostream_reset_ring(&ptpx->evtout);

                // consume payload
                //
                ptp_consume(&ptpx->cmdin, ptpx->cmdlen);
                ptpx->cmdlen = 0;

                // ack with session id, guid, camera name
                //
                length = 4 + 4 + 4 + 16 + (2 * (ptp->device_name_len + 1)) + 4;
                type = PTP_INIT_REQ_ACK;
                result = ptp_write(ptpx->client_sock, &ptpx->cmdout, (uint8_t*)&length, 4);
                result = ptp_write(ptpx->client_sock, &ptpx->cmdout, (uint8_t*)&type, 4);
                result = ptp_write(ptpx->client_sock, &ptpx->cmdout, (uint8_t*)&ptpx->session_id, 4);
                result = ptp_write(ptpx->client_sock, &ptpx->cmdout, ptp->uuid_device, 16);
                result = ptp_write(ptpx->client_sock, &ptpx->cmdout, (uint8_t *)ptp->device_name, 2 * (ptp->device_name_len + 1));

                type = MTPIP_BINARY_PROTOCOL_VERSION;
				result = ptp_write(ptpx->client_sock, &ptpx->cmdout, (uint8_t*)&type, 4);
                result = ptp_flush_out(ptpx->client_sock, &ptpx->cmdout, 5, 0);
				result = 0;

				// go wait for event socket to connect
				//
                ptpx->state = ptpSetupEvent;
                break;

            case PTP_CMD_REQ:
				// get cmd type
                result = ptp_read(&ptpx->cmdin, (uint8_t*)&type, 4);
                ptpx->cmdlen -= 4;
                // ASSERT(result == 4 && type == 1);
                // the rest of this is a PTP payload, so handle it
                //
                result = ptp_dispatch(ptpx, &ptpx->cmdin, ptpx->cmdlen);
                ptpx->state = ptpRun;
                break;

            case PTP_START_DATA:
                // get transid
                result = ptp_read(&ptpx->cmdin, (uint8_t*)&ptpx->transid, 4);
                // get data length to expect
                result = ptp_read(&ptpx->cmdin, (uint8_t*)&ptpx->datalen, 4);
                // trailing 0 ?
                result = ptp_consume(&ptpx->cmdin, 4);
                ptpx->cmdlen -= 4;

                // tell ptp layer to expect this much data
                ptpx->lenread = 0;
                result = ptp_data_start(ptpx, &ptpx->cmdin, ptpx->datalen);
                if (result < 0)
                {
                    // fail
                    connected = false;
                    break;
                }
                // rest of data goes to ptp layer
                ptpx->state = ptpRun;
                break;

            case PTP_CANCEL:
                // transid
                result = ptp_read(&ptpx->cmdin, (uint8_t*)&ptpx->transid, 4);
                ptpx->cmdlen -= 4;
                ptpx->datalen = 0;
                ptpx->state = ptpRun;
                break;

            case PTP_DATA:
                result = ptp_read(&ptpx->cmdin, (uint8_t*)&ptpx->transid, 4);
                ptpx->cmdlen -= 4;
                ptpx->state = ptpReadDataPayload;
                break;

            case PTP_END_DATA:
                result = ptp_read(&ptpx->cmdin, (uint8_t*)&ptpx->transid, 4);
                ptpx->cmdlen -= 4;
                ptpx->state = ptpReadDataPayload;
                break;

            default:
                butil_log(3, "unhandled cmd %u\n", type);
                ptpx->cmdlen = 0;
                ptpx->state = ptpRun;
            }
            break;

        case ptpSetupEvent:
            // wait for client to connect an event socket
            //
            result = iostream_posix_poll_filedesc(ptp->server_sock, readable, 0, 0);
            if (result < 0)
            {
                butil_log(1, "server connection broken\n");
                connected = false;
                return result;
            }

            if (result == 0)
            {
                // no event socket connection yet
                break;
            }

			result = 0;
            ptpx->event_sock = accept(ptp->server_sock, (struct sockaddr *)&evt_addr, &evtlen);
            if (ptpx->event_sock < 0)
            {
                // poll > 0 and accept < 0 means socekt broken
                butil_log(1, "Can't accept\n");
                connected = false;
                break;
            }

            butil_log(3, "evt connected\n");

            // make event socket non-blocking
            //
            flags = fcntl(ptpx->event_sock, F_GETFL, 0);
            fcntl(ptpx->event_sock, F_SETFL, flags | O_NONBLOCK);
            ptpx->state = ptpRun;
            break;

        case ptpEvent:
            // consume session id
            result = ptp_read(&ptpx->evtin, (uint8_t*)&length, 4);
            ptpx->evtlen -= 4;
            switch (type)
            {
            case PTP_INIT_EVENT_REQ:
                // write ack
                length = 4 + 4;
                type = PTP_INIT_EVENT_REQ_ACK;
                result = ptp_write(ptpx->event_sock, &ptpx->evtout, (uint8_t*)&length, 4);
                result = ptp_write(ptpx->event_sock, &ptpx->evtout, (uint8_t*)&type, 4);
                ptpx->state = ptpRun;
                break;
            case PTP_CANCEL:
                // client saying stop what you were doing (i.e. data xfer)
                result = ptp_cancel(ptpx);
                ptp_consume(&ptpx->evtin, ptpx->evtlen);
                ptpx->evtlen = 0;
                ptpx->state = ptpRun;
                break;
            default:
                butil_log(3, "unhandled evt %u, consume %u data\n", type, ptpx->evtlen);
                // consume remaing event data
                ptp_consume(&ptpx->evtin, ptpx->evtlen);
                ptpx->evtlen = 0;
                ptpx->state = ptpRun;
            }
            break;

        case ptpRun:
            // todo - timeout of iocount < datalen
            if (ptpx->cmdin.count >= 8)
            {
                result = ptp_read(&ptpx->cmdin, (uint8_t*)&length, 4);
                // ASSERT(result == 4)
                result = ptp_read(&ptpx->cmdin, (uint8_t*)&type, 4);
                // ASSERT(result == 4)
                length -= 8;
                ptpx->cmdtype = type;
                ptpx->cmdlen  = length;
                ptpx->state = ptpReadCommandPayload;
                break;
            }

            if (ptpx->evtin.count >= 8)
            {
                result = ptp_read(&ptpx->evtin, (uint8_t*)&length, 4);
                // ASSERT(result == 4)
                result = ptp_read(&ptpx->evtin, (uint8_t*)&type, 4);
                // ASSERT(result == 4)
                length -= 8;
                ptpx->evttype = type;
                ptpx->evtlen  = length;
                ptpx->state = ptpReadEventPayload;
                break;
            }
            break;

        case ptpReadCommandPayload:
            // todo - timeout of iocount < datalen
            if (ptpx->cmdin.size < ptpx->cmdlen)
            {
                // for data and end-data packets, just
                // insist on the 4 bytes transid
                if (ptpx->cmdtype != PTP_DATA && ptpx->cmdtype != PTP_END_DATA)
                {
                    butil_log(3, "overflow cmd payload\n");
                    connected = 0;
                }
            }
            if (ptpx->cmdin.count < length)
            {
                if (ptpx->cmdtype != PTP_DATA && ptpx->cmdtype != PTP_END_DATA)
                {
                    break;
                }
                // its a data command, move to command state as
                // soon as we get a transid
                if (ptpx->cmdin.count < 4)
                {
                    break;
                }
            }
            ptpx->state = ptpCommand;
            break;

        case ptpReadEventPayload:
            // todo - timeout of incount < len
            if (ptpx->evtin.size < length)
            {
                butil_log(3, "overflow evt payload\n");
                connected = 0;
            }

            if (ptpx->evtin.count < ptpx->evtlen)
            {
                break;
            }
            ptpx->state = ptpEvent;
            break;

        case ptpReadDataPayload:
            // calculate amount of data in this cycle
            length = ptpx->cmdin.count;
            if (length > ptpx->cmdlen)
            {
                // limit read to packet len
                length = ptpx->cmdlen;
            }

            if (length > ptpx->datalen - ptpx->lenread)
            {
                // limit read to overall unread data
                length = ptpx->datalen - ptpx->lenread;
            }

            // feed data to ptp layer, this function must
            // take all data, returns status
            result = ptp_rx_data(ptpx, &ptpx->cmdin, length, ptpx->datalen);
            if (result < 0)
            {
                connected = false;
                break;
            }
            ptpx->cmdlen -= length;
            ptpx->lenread += length;
            if (ptpx->cmdlen == 0)
            {
                if (butil_get_log_level() > 1)
                {
                    butil_log(3, "end of data packet\n");
                }
                ptpx->state = ptpRun;
            }

            if (ptpx->lenread >= ptpx->datalen)
            {
                if (butil_get_log_level()  > 1)
                {
                    butil_log(3, "end of all data\n");
                }

                if (ptpx->state != ptpRun)
                {
                    butil_log(3, "? end of all data but not end of packet?\n");
                    ptpx->state = ptpRun;
                }
            }
            break;

        default:
            connected = 0;
            break;
        }
    }
    while (connected);

    if (ptpx->client_sock >= 0)
    {
        shutdown(ptpx->client_sock, SHUT_RDWR);
        close_socket(ptpx->client_sock);
        ptpx->client_sock = INVALID_SOCKET;
    }

    if (ptpx->event_sock >= 0)
    {
        close_socket(ptpx->event_sock);
        ptpx->event_sock = INVALID_SOCKET;
    }
    return 0;
}

int mtp_device_init(ptp_t *ptp, buuid_t uuid, const char *name)
{
	ptp_connection_t *ptpx;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t clilen;
    socket_t clisock;
    int flags;
    int enable;
    int sv;

	if (! ptp || ! uuid || ! name)
	{
		return -1;
	}
    // handle sigpipe ignore
    //
    signal(SIGPIPE, SIG_IGN);

	ptp->device_name_len = strlen(name);
	if (ptp->device_name_len >= PTP_MAX_STRING)
	{
		ptp->device_name_len = PTP_MAX_STRING - 1;
	}

	for (sv = 0; sv < ptp->device_name_len; sv++)
	{
		ptp->device_name[sv] = (uint16_t)name[sv];
	}
	ptp->device_name[sv] = 0;

	for (sv = 0; sv < sizeof(buuid_t); sv++)
	{
		ptp->uuid_device[sv] = uuid[sv];
	}

    ptp->sessionid = 0;
    ptp->server_sock = INVALID_SOCKET;

	ptpx = &ptp->connection;
    ptpx->client_sock = INVALID_SOCKET;
    ptpx->event_sock = INVALID_SOCKET;

    // init files
    //
    mtpd_files_init();

    // and properties
    //
    ptp_properties_init();

    // open server socket to listen on
    //
    ptp->server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (ptp->server_sock < 0)
    {
        butil_log(1, "Can't open server socket\n");
        return -1;
    }

    enable = 1;
    if (setsockopt(ptp->server_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        butil_log(1, "SO_REUSEADDR failed");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(((uint16_t)MTP_IP_PORT));

    if (bind(ptp->server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        close_socket(ptp->server_sock);
        ptp->server_sock = INVALID_SOCKET;
        butil_log(1, "Can't bind to port %u\n", MTP_IP_PORT);
        return -1;
    }
    else
    {
        listen(ptp->server_sock, MTP_IP_MAX_CONNECTIONS);
        butil_log(3, "Listing on port %u\n", MTP_IP_PORT);
    }

    // make server socket non blocking
    //
    flags = fcntl(ptp->server_sock, F_GETFL, 0);
    fcntl(ptp->server_sock, F_SETFL, flags | O_NONBLOCK);

    return 0;
}

int mtp_device_slice(ptp_t *ptp, int tos, int tous)
{
	ptp_connection_t *ptpx;
    struct sockaddr_in client_addr;
    socklen_t clilen;
    socket_t clisock;
    int flags;
    int enable;
    int result;

	ptpx = &ptp->connection;

    if (ptpx->client_sock != INVALID_SOCKET)
    {
        result = serve_client(ptp, ptpx, MTP_IP_PORT > 400 && MTP_IP_PORT < 500);

        if (result)
        {
            if (ptpx->client_sock != INVALID_SOCKET)
            {
                close_socket(ptpx->client_sock);
                ptpx->client_sock = INVALID_SOCKET;
            }

            if (ptpx->event_sock != INVALID_SOCKET)
            {
                close_socket(ptpx->event_sock);
                ptpx->event_sock = INVALID_SOCKET;
            }

            ptp_deinit(ptpx);

            butil_log(3, "End of connection\n");
        }
    }
    // no current connection, look for one
    //
    if (ptp->server_sock != INVALID_SOCKET)
    {
        result = iostream_posix_poll_filedesc(ptp->server_sock, readable, tos, tous);
        if (result < 0)
        {
            // server broken, end
            return -1;
        }

        if (result == 0)
        {
            // no client waiting
            return 0;
        }

        clilen = sizeof(client_addr);
        clisock = accept(ptp->server_sock, (struct sockaddr *)&client_addr, &clilen);

        if (clisock < 0)
        {
            butil_log(1, "Can't accept\n");
            return -1;
        }

        // make client socket non-blocking
        //
        flags = fcntl(clisock, F_GETFL, 0);
        fcntl(clisock, F_SETFL, flags | O_NONBLOCK);

        ptp->sessionid++;

        butil_log(3, "Serving client session %d on sock %u\n", ptp->sessionid, clisock);

        ptp_init(ptpx);

        ptpx->client_sock = clisock;
    }

    return 0;
}

int mtp_device_deinit(ptp_t *ptp)
{
    ptp_deinit(&ptp->connection);

    if (ptp->server_sock >= 0)
    {
        close_socket(ptp->server_sock);
        ptp->server_sock = INVALID_SOCKET;
    }
}

