#if HTTP_SUPPORT_SIP
    case httpInvite:    return "INVITE";
    case httpAck:       return "ACK";
    case httpPrack:     return "PRACK";
    case httpCancel:    return "CANCEL";
    case httpUpdate:    return "UPDATE";
    case httpInfo:      return "INFO";
    case httpSubscribe: return "SUBSCRIBE";
    case httpNotify:    return "NOTIFY";
    case httpRefer:     return "REFER";
    case httpMessage:   return "MESSAGE";
    case httpRegister:  return "REGISTER";
    case httpBye:       return "BYE";
#endif
#if HTTP_SUPPORT_SIP
    if (! http_ncasecmp(name, "INVITE"))
    {
        *method = httpInvite;
        return 0;
    }
    if (! http_ncasecmp(name, "ACK"))
    {
        *method = httpAck;
        return 0;
    }
    if (! http_ncasecmp(name, "PRACK"))
    {
        *method = httpPrack;
        return 0;
    }
    if (! http_ncasecmp(name, "CANCEL"))
    {
        *method = httpCancel;
        return 0;
    }
    if (! http_ncasecmp(name, "UPDATE"))
    {
        *method = httpUpdate;
        return 0;
    }
    if (! http_ncasecmp(name, "INFO"))
    {
        *method = httpInfo;
        return 0;
    }
    if (! http_ncasecmp(name, "SUBSCRIBE"))
    {
        *method = httpSubscribe;
        return 0;
    }
    if (! http_ncasecmp(name, "NOTIFY"))
    {
        *method = httpNotify;
        return 0;
    }
    if (! http_ncasecmp(name, "REFER"))
    {
        *method = httpRefer;
        return 0;
    }
    if (! http_ncasecmp(name, "MESSAGE"))
    {
        *method = httpMessage;
        return 0;
    }
    if (! http_ncasecmp(name, "REGISTER"))
    {
        *method = httpRegister;
        return 0;
    }
    if (! http_ncasecmp(name, "BYE"))
    {
        *method = httpBye;
        return 0;
    }
#endif

#include "bhttp.h"

#if HTTP_SUPPORT_SIP

#define HTTP_MAX_SIP_SDP 1024

#if 0
// code moved out of bhttp

    #if HTTP_SUPPORT_SIP
    client->sip_via[0] = '\0';
    client->sip_from[0] = '\0';
    client->sip_to[0] = '\0';
    client->sip_contact[0] = '\0';
    client->sip_callid[0] = '\0';
    client->sip_depth = 0;
    client->sip_cseq = 0;
    #endif


    #if HTTP_SUPPORT_SIP
    else if (! http_ncasecmp(header, "via:"))
    {
        if (strlen(value) >= sizeof(client->sip_via))
        {
            HTTP_ERROR("VIA len");
            return -1;
        }
        strcpy(client->sip_via, value);
    }
    else if (! http_ncasecmp(header, "from:"))
    {
        if (strlen(value) >= sizeof(client->sip_from))
        {
            HTTP_ERROR("from len");
            return -1;
        }
        strcpy(client->sip_from, value);
    }
    else if (! http_ncasecmp(header, "to:"))
    {
        if (strlen(value) >= sizeof(client->sip_to))
        {
            HTTP_ERROR("to len");
            return -1;
        }
        strcpy(client->sip_to, value);
    }
    else if (! http_ncasecmp(header, "contact:"))
    {
        if (strlen(value) >= sizeof(client->sip_contact))
        {
            HTTP_ERROR("contact len");
            return -1;
        }
        strcpy(client->sip_contact, value);
    }
    else if (! http_ncasecmp(header, "call-id:"))
    {
        if (strlen(value) >= sizeof(client->sip_contact))
        {
            HTTP_ERROR("call-id len");
            return -1;
        }
        strcpy(client->sip_callid, value);
    }
    else if (! http_ncasecmp(header, "cseq:"))
    {
        client->sip_cseq = strtoul(value, NULL, 10);
    }
    #endif
        else
        {
            client->sip_depth = (int)strtol(value, &value, 10);
        }
                    else
                    {
                        client->sip_depth = -2;
                    }

#if HTTP_SUPPORT_SIP
        ||  client->scheme == schemeSIPS
#endif

    case httpSipSlice:
    #if HTTP_SUPPORT_SIP
        result = http_sip_slice(client);
        if (result)
        {
            // any errors that don't format a reply go to done state
            //
            if (client->state == httpSipSlice)
            {
                client->state = httpDone;
            }
            result = 0;
        }
    #endif
        break;

            #if HTTP_SUPPORT_SIP
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
                if (! client->resource)
                {
                    HTTP_ERROR("No SIP resource");
                    result = http_error_reply(client, 405, "Bad Request", false);
                    if (result)
                    {
                        return http_slice_fatal(client, result);
                    }
                    return 0;
                }
                result = http_sip_request(client);
                if (result)
                {
                    // any errors that don't format a reply go to done state, else
                    // assume the request handler sent a specific reply already
                    //
                    if (client->state != httpSendReply)
                    {
                        client->state = httpDone;
                    }
                    result = 0;
                }
                break;
            #endif


        #if HTTP_SUPPORT_SIP
        if (! http_ncasecmp(pline, "SIP/"))
        {
            pline += 4;
            client->scheme = schemeSIP;
            client->vmaj = (uint8_t)strtoul(pline, &pline, 10);
            if (*pline)
            {
                pline++;
                client->vmin = (uint8_t)strtoul(pline, &pline, 10);
            }
        }
        #endif
        #if HTTP_SUPPORT_SIP
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
        #endif
    else if (isphone)
    {
        result = sip_phone(argc, argv);
    }
#endif

typedef enum
{
	sipIdle,
	sipInboundRinging,
	sipInboundAnswered,
	sipInboundCall,
	sipOutboundRinging,
	sipOutboundAnswered,
	sipOutboundCall
}
sip_state_t;

typedef struct
{
	sip_state_t	state;
	char		call_id[HTTP_MAX_SIP_ADDR];
	char		contact[HTTP_MAX_SIP_ADDR];
	char		sdp[HTTP_MAX_SIP_SDP];
	struct sockaddr_in remote_rtp_addr;
	socket_t	rtp_socket;
	uint16_t	rtp_port;
	socket_t	rtcp_socket;
	uint16_t	rtcp_port;
}
sip_session_t;

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

	http_log(2, "SDP=\n%s\n", sdp);

    addr_len = sizeof(session->remote_rtp_addr);
    memset(&session->remote_rtp_addr, 0, addr_len);
    session->remote_rtp_addr.sin_family = AF_INET;

	// find media descriptor
	//
	ps = strstr(sdp, "m=");
	if (! ps)
	{
		HTTP_ERROR("No SDP media descriptor");
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
		HTTP_ERROR("No port in SDP media desc");
		return -1;
	}
	session->remote_rtp_addr.sin_port = htons((uint16_t)strtoul(ps, (char **)&ps, 10));
	if (! ps || ! *ps)
	{
		HTTP_ERROR("Incomplete SDP media desc");
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

		http_log(3, "try codec %u\n", format);

		if (sip_sdp_codec_supported(session, sdp, format))
		{
			codec_found = true;
			break;
		}
	}
	if (! codec_found)
	{
		HTTP_ERROR("No matching codec");
		return -1;
	}
	// find caller after media descriptor used
	ps = strstr(pmedia_spec, "c=");
	if (! ps)
	{
		HTTP_ERROR("No SDP remote addr");
		return -1;
	}
	// c=IN IP4 <host>
	ps+=2;
	ps = sip_skip_nonwhite(ps);
	ps = sip_skip_white(ps);
	if (strncmp(ps, "IP4", 3))
	{
		HTTP_ERROR("Need IP4");
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
            HTTP_ERROR("Can't find host");
            return -1;
        }
        memcpy((char *)&session->remote_rtp_addr.sin_addr.s_addr,
				(char *)server->h_addr, server->h_length);
    }
    else
    {
        if (! inet_aton(host, &session->remote_rtp_addr.sin_addr))
        {
            HTTP_ERROR("Invalid address");
            return -1;
        }
    }
	http_log(2, "RTP dest address=%s:%u\n",
			inet_ntoa(session->remote_rtp_addr.sin_addr),
			ntohs(session->remote_rtp_addr.sin_port));

	return 0;
}

static int sip_session_start(sip_session_t *session)
{
	struct sockaddr_in rtp_addr;
	socklen_t addr_len;

	session->rtp_socket = http_create_server_socket(httpUDP, 0);
	if (session->rtp_socket == INVALID_SOCKET)
	{
		HTTP_ERROR("RTP socket");
		return -1;
	}
	addr_len = sizeof(rtp_addr);
	if (getsockname(session->rtp_socket, (struct sockaddr *)&rtp_addr, &addr_len))
	{
		close_socket(session->rtp_socket);
		session->rtp_socket = INVALID_SOCKET;
		HTTP_ERROR("RTP port");
		return -1;
	}
	session->rtp_port = ntohs(rtp_addr.sin_port);

	snprintf(session->sdp, sizeof(session->sdp),
		"v=0\r\n"
		"o=- 3720446843 3720446844 IN IP4 192.168.1.177\r\n"
		"s=bhttpsip\r\n"
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

static int sip_begin_reply(http_client_t *client, int code, const char *blurb)
{
	int result;

	result = http_begin_reply(client, code, blurb);
	result |= http_append_reply(client, "Via: %s", client->sip_via);
	result |= http_append_reply(client, "From: %s", client->sip_from);
	result |= http_append_reply(client, "To: %s", client->sip_to);
	result |= http_append_reply(client, "Call-ID: %s", client->sip_callid);
	result |= http_append_reply(client, "CSeq: %d %s",
			client->sip_cseq, http_method_name(client->method));
	return result;
}

static int sip_read_rtp(uint8_t *buffer, int len)
{
	int tc, wc, inlen, cnt;
	char* pd;
	unsigned long type, mark;
	unsigned long v;

	inlen = len;

	if (len == 0)
	{
		return 0;
	}
	if(len < 12)
	{
		HTTP_ERROR("Short RTP");
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

		http_log(5, "RTP v=%08X\n", v);

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
		http_log(1, "RTP Rx Sel Error %d\n", errno);
		return result;
	}
	result = recv(session->rtp_socket, buf, sizeof(buf), 0);
	http_log(2, "RTP Rx: %d\n", result);
	if (result < 0)
	{
		if (errno == EAGAIN)
		{
			result = 0;
		}
		else
		{
			http_log(1, "RTP Rx Error %d\n", errno);
			return result;
		}
	}
	if (result > 0)
	{
		int wc;

		addr_len = sizeof(session->remote_rtp_addr);
		wc = sendto(session->rtp_socket, buf, result, 0,
			(struct sockaddr *)&session->remote_rtp_addr, addr_len);
		http_log(2, "RTP Tx=%d\n", wc);
	}
	if (result > 0)
	{
		result = sip_read_rtp(buf, result);
	}
	return result;
}

int http_sip_slice(http_client_t *client)
{
	sip_session_t *session;
	int result;

	result = 0;
	session = &s_session;

	switch (client->method)
	{
	case httpInvite:
		if (session->state == sipInboundRinging)
		{
			result = sip_begin_reply(client, 200, "OK");
			result |= http_append_reply(client, "Content-Length: %d", strlen(session->sdp));
			result |= http_append_reply(client, "Content-Type: %s", "application/sdp");
			result |= http_append_reply(client, "");
			result |= http_append_reply(client, session->sdp);
			if (result)
			{
				session->state = sipIdle;
				return result;
			}
			session->state = sipInboundAnswered;
			return http_send_out_data(client, httpSendReply, httpSipSlice);
		}
		if (session->state == sipInboundAnswered)
		{
			session->state = sipInboundCall;
		}
		if (session->state == sipInboundCall)
		{
			session->state = sipInboundCall;
			result = sip_read_audio(session);
			if (result)
			{
				session->state = sipIdle;
			}
		}
		break;

	default:
		break;
	}
	return result;
}

int http_sip_request(struct http_client *client)
{
	sip_session_t *session;
	int result;
	char *pv;
	int i;

    http_log(5, "SIP %s %u %s\n", http_method_name(client->method),
            client->sip_cseq, client->path);

	session = &s_session;

	// parse VIA header to get source of request
	//
	// Via: SIP/2.0/UDP 192.168.1.177:55114;rport;branch=z9hG4bKPj5.xVwkY0Feqvdg8h.0J4MZxK3UmBSrNX
	// Contact: <sip:192.168.1.177:5060;ob>

	if (! client->sip_via[0])
	{
		HTTP_ERROR("no via");
		return -1;
	}
	if (client->sip_contact[0])
	{
		pv = client->sip_contact;

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
			HTTP_ERROR("contact overflow");
			return -1;
		}
		if (*pv == '>')
		{
			client->out_port == 5060;
		}
		else if (*pv == ':')
		{
			client->out_port = (uint16_t)strtoul(pv + 1, NULL, 10);
		}
		else
		{
			HTTP_ERROR("no port in contact");
			return -1;
		}
	}
	switch (client->method)
    {
	case httpInvite:
		if (session->state != sipIdle)
		{
			result = sip_begin_reply(client, 486, "Busy");
			result |= http_append_reply(client, "");
			if (result)
			{
				return result;
			}
			return http_send_out_data(client, httpSendReply, httpKeepAlive);
		}
		http_log(4, "Sip call from %s\n", client->sip_from);

		// copy sdp body from outbuf
		if (client->out.count == 0)
		{
			HTTP_ERROR("no SDP");
			session->state = sipIdle;
			return -1;
		}
		if (client->out.count >= sizeof(session->sdp))
		{
			HTTP_ERROR("SDP overflow");
			session->state = sipIdle;
			return -1;
		}
		memcpy(session->sdp, client->out.data + client->out.tail, client->out.count);
		session->sdp[client->out.count] = '\0';

		result = sip_parse_sdp(session, session->sdp);
		if (result)
		{
			result = sip_begin_reply(client, 488, "Error");
			session->state = sipIdle;
		}
		else
		{
			result = sip_session_start(session);
			if (result)
			{
				result = sip_begin_reply(client, 500, "Error");
				session->state = sipIdle;
			}
			else
			{
				result = sip_begin_reply(client, 180, "Ringing");
				session->state = sipInboundRinging;
			}
		}
		result |= http_append_reply(client, "");
		if (result)
		{
			return result;
		}
		return http_send_out_data(client, httpSendReply, httpSipSlice);

	case httpAck:
		if (session->state == sipInboundRinging || session->state == sipInboundAnswered)
		{
			http_log(4, "Sip call started\n");
			session->state = sipInboundCall;
			client->state = httpKeepAlive;
		}
		else
		{
			HTTP_ERROR("Unexpected ACK");
			client->state = httpKeepAlive;
		}
		break;

	case httpBye:
		http_log(4, "Sip call ended\n");
		session->state = sipIdle;
		client->state = httpKeepAlive;
		break;

	default:
		client->keepalive = 1;
		client->long_timeout = 100;
		result = sip_begin_reply(client, 500, "Huh");
		result |= http_append_reply(client, "");
		if (result)
		{
			return result;
		}
		return http_send_out_data(client, httpSendReply, httpKeepAlive);
	}
	return 0;
}

int sip_phone(int argc, char **argv)
{
	http_server_t sip_server;
	http_resource_t *sip_resource, *sip_resources = NULL;
	http_credentials_t sip_creds;
	uint16_t sip_port;
	int result;

    sip_port = 5062;

	s_session.state = sipIdle;
#if 1
    sip_creds.user = "";
    sip_creds.pass = "";
    sip_creds.type = httpAuthNone;
#else
    sip_creds.user = "aaa";
    sip_creds.pass = "bbb";
    sip_creds.type = httpAuthBasic;
#endif
    result = http_add_sip_resource(&sip_resources, "sip:*", "./", &sip_creds);
    if (result)
    {
        HTTP_ERROR("Can't make resource");
        return result;
    }
    result = http_server_init(&sip_server, sip_resources, sip_port, httpUDP, sip_port == 5061);
    if (result)
    {
        HTTP_ERROR("can't start server");
        return result;
    }
    result = http_serve(&sip_server);
    if (result)
    {
        http_log(2, "server on port %u ends\n", sip_port);
    }
    http_server_cleanup(&sip_server);
	return result;
}

#endif

