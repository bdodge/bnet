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
#include "bsip.h"
#include "butil.h"

static sip_session_t s_session;

static const char *sip_skip_white(const char *p)
{
    while (*(p) && ((*(p) == ' ') || (*(p) == '\t')))
    {
        p++;
    }
    return p;
}

static const char *sip_skip_nonwhite(const char *p)
{
    while (*(p) && (*(p) != ' ') && (*(p) != '\t'))
    {
        p++;
    }
    return p;
}

int sip_init_session(sip_server_context_t *sip, sip_session_t *session)
{
	if (! sip || ! session)
	{
		return -1;
	}
	memset(session, 0, sizeof(sip_session_t));
	session->state = sipIdle;
	return 0;
}

int sip_init_context(sip_server_context_t *sip)
{
	int result;

	if (! sip)
	{
		return -1;
	}
	sip->connected = false;
	sip->sdpin.data = (uint8_t*)sip->sdp;
	sip->sdpin.head = 0;
	sip->sdpin.tail = 0;
	sip->sdpin.count = 0;
	sip->sdpin.size = sizeof(sip->sdp);
    sip->sip_via[0] = '\0';
    sip->sip_from[0] = '\0';
    sip->sip_to[0] = '\0';
    sip->sip_contact[0] = '\0';
    sip->sip_callid[0] = '\0';
    sip->sip_depth = 0;
    sip->sip_cseq = 0;
	result = sip_init_session(sip, &s_session);
	return result;
}

int sip_process_header(sip_server_context_t *sip, const char *header)
{
    const char *value;

    butil_log(4, "sip hdr %s\n", header);

    header = sip_skip_white(header);
    value = sip_skip_nonwhite(header);
    value = sip_skip_white(value);

	if (! http_ncasecmp(header, "via:"))
    {
        if (strlen(value) >= sizeof(sip->sip_via))
        {
            BERROR("VIA len");
            return -1;
        }
        strcpy(sip->sip_via, value);
    }
    else if (! http_ncasecmp(header, "from:"))
    {
        if (strlen(value) >= sizeof(sip->sip_from))
        {
            BERROR("from len");
            return -1;
        }
        strcpy(sip->sip_from, value);
    }
    else if (! http_ncasecmp(header, "to:"))
    {
        if (strlen(value) >= sizeof(sip->sip_to))
        {
            BERROR("to len");
            return -1;
        }
        strcpy(sip->sip_to, value);
    }
    else if (! http_ncasecmp(header, "contact:"))
    {
        if (strlen(value) >= sizeof(sip->sip_contact))
        {
            BERROR("contact len");
            return -1;
        }
        strcpy(sip->sip_contact, value);
    }
    else if (! http_ncasecmp(header, "call-id:"))
    {
        if (strlen(value) >= sizeof(sip->sip_contact))
        {
            BERROR("call-id len");
            return -1;
        }
        strcpy(sip->sip_callid, value);
    }
    else if (! http_ncasecmp(header, "cseq:"))
    {
        sip->sip_cseq = strtoul(value, NULL, 10);
    }
	else if (! http_ncasecmp(header, "depth:"))
	{
		sip->sip_depth = (int)strtol(value, NULL, 10);
    }
	else
	{
		return 1;
	}
	return 0;
}

#if ORT_SIP

    case httpSipSlice:
    #if HTTP_SUPPORT_SIP
        result = http_sip_slice(client, sip);
        if (result)
        {
            // any errors that don't format a reply go to done state
            //
            if (sip->state == httpSipSlice)
            {
                sip->state = httpDone;
            }
            result = 0;
        }
    #endif
        break;

            case httpInvite:
            case httpAck:
            case httpPrack:
            case httpCancel:
            case httpUpdate:
            case httpInfo:
            case httpSubscribe:
            case httpNotify:
            case httpRefer:
            case httpMessage:
            case httpRegister:
            case httpBye:
                break;
#endif

static int sip_sdp_codec_supported(sip_session_t *session, const char *sdp, uint32_t format)
{
	char codec_attrib[128];
	const char *ps;

	// look for "a=rtpmap:<format> codec/rate
	//
	if (! session || ! sdp)
	{
		return 0;
	}
	snprintf(codec_attrib, sizeof(codec_attrib), "a=rtpmap:%u", format);
	ps = strstr(sdp, codec_attrib);
	if (! ps)
	{
		return 0;
	}
	ps += 9;
	while (*ps && (*ps >= '0' && *ps <= '9'))
	{
		ps++;
	}
	ps = sip_skip_white(ps);
	if (! strncmp(ps, "PCMU/8000", 9))
	{
		return 1;
	}
	return 0;
}

static int sip_parse_sdp(sip_session_t *session, const char *sdp)
{
	const char *ps;
	const char *pmedia_spec;
	char host[HTTP_MAX_HOSTNAME];
	uint32_t format;
	bool codec_found;
	bool isname;
	size_t addr_len;
	int i;

	butil_log(2, "SDP=\n%s\n", sdp);

    addr_len = sizeof(session->remote_rtp_addr);
    memset(&session->remote_rtp_addr, 0, addr_len);
    session->remote_rtp_addr.sin_family = AF_INET;

	// find media descriptor
	//
	ps = strstr(sdp, "m=");
	if (! ps)
	{
		BERROR("No SDP media descriptor");
		return -1;
	}
	// parse m=tag port transport n <n>
	//
	pmedia_spec = ps;
	ps+=2;
	ps = sip_skip_nonwhite(ps);
	ps = sip_skip_white(ps);
	if (*ps < '0' || *ps > '9')
	{
		BERROR("No port in SDP media desc");
		return -1;
	}
	session->remote_rtp_addr.sin_port = htons((uint16_t)strtoul(ps, (char **)&ps, 10));
	if (! ps || ! *ps)
	{
		BERROR("Incomplete SDP media desc");
		return -1;
	}
	ps = sip_skip_white(ps);
	ps = sip_skip_nonwhite(ps);
	ps = sip_skip_white(ps);

	// match rtp format types to our codec capabilities
	//
	codec_found = false;
	while (ps && *ps && (*ps >= '0' && *ps <= '9'))
	{
		format = strtoul(ps, (char **)&ps, 10);
		ps = sip_skip_white(ps);

		butil_log(3, "try codec %u\n", format);

		if (sip_sdp_codec_supported(session, sdp, format))
		{
			codec_found = true;
			break;
		}
	}
	if (! codec_found)
	{
		BERROR("No matching codec");
		return -1;
	}
	// find caller after media descriptor used
	ps = strstr(pmedia_spec, "c=");
	if (! ps)
	{
		ps = strstr(sdp, "c=");
		if (! ps)
		{
			BERROR("No SDP remote addr");
			return -1;
		}
	}
	// c=IN IP4 <host>
	ps+=2;
	ps = sip_skip_nonwhite(ps);
	ps = sip_skip_white(ps);
	if (strncmp(ps, "IP4", 3))
	{
		BERROR("Need IP4");
		return -1;
	}
	ps = sip_skip_nonwhite(ps);
	ps = sip_skip_white(ps);

	for (i = 0; i < sizeof(host) - 1; i++)
	{
		if (*ps == '\0' || *ps == ' ' || *ps == '\r' || *ps == '\n')
		{
			break;
		}
		host[i] = *ps++;
	}
	// resolve session hostname
	// if host is an IP address, use directly
    for (i = 0, isname = false; i < strlen(host); i++)
    {
        if ((host[i] < '0' || host[i] > '9') && host[i] != '.')
        {
            isname = true;
            break;
        }
    }
	host[i] = '\0';

    if (! isname)
    {
	    struct hostent *server;

        server = gethostbyname(host);
        if (server == NULL)
        {
            BERROR("Can't find host");
            return -1;
        }
        memcpy((char *)&session->remote_rtp_addr.sin_addr.s_addr,
				(char *)server->h_addr, server->h_length);
    }
    else
    {
        if (! inet_aton(host, &session->remote_rtp_addr.sin_addr))
        {
            BERROR("Invalid address");
            return -1;
        }
    }
	butil_log(2, "RTP dest address=%s:%u\n",
			inet_ntoa(session->remote_rtp_addr.sin_addr),
			ntohs(session->remote_rtp_addr.sin_port));

	return 0;
}

static int sip_session_start(sip_server_context_t *sip, sip_session_t *session)
{
	struct sockaddr_in rtp_addr;
	socklen_t addr_len;

	session->rtp_socket = http_create_server_socket(httpUDP, 0, 3);
	if (session->rtp_socket == INVALID_SOCKET)
	{
		BERROR("RTP socket");
		return -1;
	}
	addr_len = sizeof(rtp_addr);
	if (getsockname(session->rtp_socket, (struct sockaddr *)&rtp_addr, &addr_len))
	{
		close_socket(session->rtp_socket);
		session->rtp_socket = INVALID_SOCKET;
		BERROR("RTP port");
		return -1;
	}
	session->rtp_port = ntohs(rtp_addr.sin_port);

	snprintf(sip->sdp, sizeof(sip->sdp),
		"v=0\r\n"
		"o=- 3720446843 3720446844 IN IP4 192.168.1.177\r\n"
		"s=bsip\r\n"
		"b=AS:84\r\n"
		"t=0 0\r\n"
		"a=X-nat:0\r\n"
		"m=audio %u RTP/AVP 0\r\n"
		"c=IN IP4 192.168.1.177\r\n"
		"b=TIAS:64000\r\n"
		"a=rtcp:4003 IN IP4 192.168.1.177\r\n"
		"a=rtpmap:0 PCMU/8000\r\n",
		session->rtp_port
	);
	return 0;
}

static int sip_begin_reply(http_client_t *client, sip_server_context_t *sip, int code, const char *blurb)
{
	int result;

	result = http_begin_reply(client, code, blurb);
	result |= http_append_reply(client, "Via: %s", sip->sip_via);
	result |= http_append_reply(client, "From: %s", sip->sip_from);
	result |= http_append_reply(client, "To: %s", sip->sip_to);
	result |= http_append_reply(client, "Call-ID: %s", sip->sip_callid);
	result |= http_append_reply(client, "CSeq: %d %s",
			sip->sip_cseq, http_method_name(client->method));
	return result;
}

static int sip_read_rtp(uint8_t *buffer, int len)
{
	int tc, wc, inlen, cnt;
	uint8_t* pd;
	unsigned long type, mark;
	unsigned long v;

	inlen = len;

	if (len == 0)
	{
		return 0;
	}
	if(len < 12)
	{
		BERROR("Short RTP");
		return -1;
	}
	pd = buffer;

	// decode RTP packets as long as data is available
	//
	while (len > 0)
	{
		// RTP packet header
		//
		v =    (unsigned long)(unsigned char)pd[3];
		v |=  ((unsigned long)(unsigned char)pd[2]) << 8;
		v |=  ((unsigned long)(unsigned char)pd[1]) << 16;
		v |=  ((unsigned long)(unsigned char)pd[0]) << 24;

		butil_log(5, "RTP v=%08X\n", v);

		len -= 4;
		pd += 4;

		// get type and mark bit
		//
		type = (v & 0x007F0000) >> 16;
		mark = (v & 0x00800000);

		// skip timestamp and SSRC
		//
		len -= 8;
		pd += 8;

		// skip contributing sources if any
		//
		tc = (((v & 0x0F000000) >> 24) << 2); // count of contributing sources in bytes
		if(tc > 0)
		{
			pd += tc;
			len -= tc;
		}
		// handle extended header if present
		//
		if (v & 0x10000000)
		{
			// header extended
			v =    (unsigned long)(unsigned char)pd[3];
			v |=  ((unsigned long)(unsigned char)pd[2]) << 8;
			v |=  ((unsigned long)(unsigned char)pd[1]) << 16;
			v |=  ((unsigned long)(unsigned char)pd[0]) << 24;

			// length of header
			tc = (v & 0xFFFF) << 2;
			pd += tc + 4;
			len -= (tc + 4);
		}
		// decode rtp payload type
		//
		switch (type)
		{
		case 0x60:
			{
			unsigned long islen, lenoff;

			// asf packet, handle asf header
			//
			v =    (unsigned long)(unsigned char)pd[3];
			v |=  ((unsigned long)(unsigned char)pd[2]) << 8;
			v |=  ((unsigned long)(unsigned char)pd[1]) << 16;
			v |=  ((unsigned long)(unsigned char)pd[0]) << 24;

			pd += 4;
			len -= 4;

			islen  = v & 0x40000000;  // L/O bit
			lenoff = v & 0x00FFFFFF;  // Length or Offset

			if(v & 0x20000000)	// R bit (skip rel. stamp)
			{
				pd += 4;
				len -= 4;
			}
			if(v & 0x10000000) // D bit (skip duration)
			{
				pd += 4;
				len -= 4;
			}
			if(v & 0x08000000)	// I bit (skip Location id)
			{
				pd += 4;
				len -= 4;
			}
			if(islen)
			{
				// len == 1 means whole asf packet contained in cnt
				//
				cnt = v & 0x00FFFFFF;
			}
			else
			{
				// len == 0 means need more data to complete asf
				//
				cnt = len;
			}
			}
			break;

		default:
			cnt = len;
			break;
		}
		// hand data up to reader
		//
		for(tc = 0; tc < cnt;)
		{
			wc = cnt;
			tc += wc;
		}
		len -= cnt;
		pd += cnt;
	}
	return 0;
}

static int sip_read_audio(sip_session_t *session)
{
	char buf[2048];
    fd_set fds;
    int nfds;
    struct timeval timeout;
	size_t addr_len;
    int fileno;
    int result;

	int to_secs = 0;
	int to_usecs = 30000;

    FD_ZERO(&fds);
    FD_SET(session->rtp_socket, &fds);
    nfds = session->rtp_socket + 1;
    timeout.tv_sec = to_secs;
    timeout.tv_usec = to_usecs;

    result = select(
                    nfds,
                    &fds,
                    NULL,
                    NULL,
                    (to_secs >= 0 && to_usecs >= 0) ? &timeout : NULL
                   );
	if (result < 0)
	{
		butil_log(1, "RTP Rx Sel Error %d\n", errno);
		return result;
	}
	result = recv(session->rtp_socket, buf, sizeof(buf), 0);
	butil_log(((result > 0) ? 5 : 7), "RTP Rx: %d\n", result);
	if (result < 0)
	{
		if (errno == EAGAIN)
		{
			result = 0;
		}
		else
		{
			butil_log(1, "RTP Rx Error %d\n", errno);
			return result;
		}
	}
	if (result > 0)
	{
		int wc;

		addr_len = sizeof(session->remote_rtp_addr);
		wc = sendto(session->rtp_socket, buf, result, 0,
			(struct sockaddr *)&session->remote_rtp_addr, addr_len);
		butil_log(2, "RTP Tx=%d\n", wc);
	}
	if (result > 0)
	{
		result = sip_read_rtp((uint8_t*)buf, result);
	}
	return result;
}

int sip_slice(http_client_t *client, sip_server_context_t *sip)
{
	static sip_state_t prevstate = sipIdle;
	sip_session_t *session;
	int result;

	result = 0;
	session = &s_session;

	if (session->state != prevstate)
	{
		butil_log(5, "SIP state %u\n", session->state);
		prevstate = session->state;
	}
	switch (session->state)
	{
	case sipInboundRinging:

		// answer the call
		//
		result = sip_begin_reply(client, sip, 200, "OK");
		result |= http_append_reply(client, "Content-Length: %d", strlen(sip->sdp));
		result |= http_append_reply(client, "Content-Type: %s", "application/sdp");
		result |= http_append_reply(client, "");
		result |= http_append_reply(client, sip->sdp);
		if (result)
		{
			session->state = sipIdle;
			return result;
		}
		session->state = sipInboundAnswered;
		break;

	case sipInboundCall:

		// in a call, marshall data
		//
		result = sip_read_audio(session);
		if (result)
		{
			session->state = sipIdle;
		}
		break;

	default:

		break;
	}
	return result;
}

int sip_request(struct http_client *client, sip_server_context_t *sip)
{
	sip_session_t *session;
	int result;
	char *pv;
	int i;

    butil_log(5, "SIP %s %u %s\n", http_method_name(client->method),
            sip->sip_cseq, client->path);

	session = &s_session;

	// parse VIA header to get source of request
	//
	// Via: SIP/2.0/UDP 192.168.1.177:55114;rport;branch=z9hG4bKPj5.xVwkY0Feqvdg8h.0J4MZxK3UmBSrNX
	// Contact: <sip:192.168.1.177:5060;ob>

	if (! sip->sip_via[0])
	{
		BERROR("no via");
		return -1;
	}
	if (sip->sip_contact[0])
	{
		pv = sip->sip_contact;

		if (*pv == '<')
		{
			pv++;
		}
		if (! http_ncasecmp(pv, "sip:"))
		{
			pv += 4;
		}
		for (i = 0; i < (sizeof(client->out_host) - 1) && *pv && *pv != ':' && *pv != '>'; i++)
		{
			client->out_host[i] = *pv++;
		}
		client->out_host[i] = '\0';

		if (i >= sizeof(client->out_host) - 1)
		{
			BERROR("contact overflow");
			return -1;
		}
		if (*pv == '>')
		{
			client->out_port = 5060;
		}
		else if (*pv == ':')
		{
			client->out_port = (uint16_t)strtoul(pv + 1, NULL, 10);
		}
		else
		{
			BERROR("no port in contact");
			return -1;
		}
	}
	switch (sip->method)
    {
	case sipInvite:
		if (session->state != sipIdle)
		{
			result = sip_begin_reply(client, sip, 486, "Busy");
			result |= http_append_reply(client, "");
			if (result)
			{
				return result;
			}
			return http_send_out_data(client, httpSendReply, httpKeepAlive);
		}
		result = sip_init_session(sip, &s_session);

		butil_log(4, "Sip call from %s\n", sip->sip_from);

		result = sip_parse_sdp(session, sip->sdp);
		if (result)
		{
			result = sip_begin_reply(client, sip, 488, "Error");
			session->state = sipIdle;
		}
		else
		{
			result = sip_session_start(sip, session);
			if (result)
			{
				result = sip_begin_reply(client, sip, 500, "Error");
				session->state = sipIdle;
			}
			else
			{
				butil_log(4, "RINGING========\n");
				result = sip_begin_reply(client, sip, 180, "Ringing");
				session->state = sipInboundRinging;
			}
		}
		result |= http_append_reply(client, "");
		if (result)
		{
			return result;
		}
		return http_send_out_data(client, httpSendReply, httpUserMethod);

	case sipAck:
		if (session->state == sipInboundRinging || session->state == sipInboundAnswered)
		{
			butil_log(4, "Sip call started\n");
			session->state = sipInboundCall;
			client->state = httpKeepAlive;
		}
		else
		{
			butil_log(4, "ACK in state %d\n", session->state);
			BERROR("Unexpected ACK");
			client->state = httpKeepAlive;
		}
		break;

	case sipBye:
		butil_log(4, "Sip call ended\n");
		result = sip_begin_reply(client, sip, 200, "OK");
		result |= http_append_reply(client, "");
		session->state = sipIdle;
		return http_send_out_data(client, httpSendReply, httpDone);

	default:
		client->keepalive = 1;
		client->long_timeout = 100;
		result = sip_begin_reply(client, sip, 500, "Huh");
		result |= http_append_reply(client, "");
		if (result)
		{
			return result;
		}
		return http_send_out_data(client, httpSendReply, httpKeepAlive);
	}
	return 0;
}
