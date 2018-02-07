#include "bxmpp.h"

static const char *s_starttls_xml   =   "<starttls xmlns='urn:ietf:params:xml:ns:xmpp-tls'/>";

static const char *s_bind_xml       =   "<iq type='set' id='0'>"
                                          "<bind xmlns='urn:ietf:params:xml:ns:xmpp-bind'>"
                                            "<resource>%s</resource>"
                                          "</bind>"
                                        "</iq>";


int bxmpp_setup(bxmpp_t *bxp)
{
	char xml[1024];
	size_t nxml;
	int result;

	switch (bxp->state)
	{
	case bxmppInit:

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
		}
		return 0;
		break;

	case bxmppTransport:
//		"<message to='jabber.org/echo'><body>hello</body></message>",

		// attempt to start tls
		//
		result = bxml_format_element(
								xml,
								sizeof(xml),
								&nxml,
								true,
								"stream",
								s_starttls_xml,
								4,
								"to", bxp->to,
          						"xmlns", "jabber:client",
						        "xmlns:stream", "urn:ietf:params:xml:ns:xmpp-streams", // "http://etherx.jabber.org/streams",
						        "version", "1.0"
								);
		if (result)
		{
			return result;
		}
		fprintf(stderr, "XML=\n%s\n", xml);
		result = bxp->stream->poll(bxp->stream, writeable, 5, 0);
		if (result <= 0)
		{
			return -1;
		}
		result = bxp->stream->write(bxp->stream, (uint8_t *)xml, nxml);
		if (result <= 0)
		{
			return -1;
		}
		result = bxp->stream->poll(bxp->stream, readable, 5, 0);
		if (result <= 0)
		{
			return -1;
		}
		result = bxp->stream->read(bxp->stream, (uint8_t *)xml, sizeof(xml));
		if (result < 0)
		{
			return result;
		}
		fprintf(stderr, "XML REPLY=\n%s\n", xml);
		bxp->state = bxmppSASL;
		break;

	case bxmppTLS:
		break;

	case bxmppSASL:
		break;

	case bxmppConnected:
		break;

	case bxmppDone:

		if (bxp->stream)
		{
			bxp->stream->close(bxp->stream);
			bxp->stream = NULL;
		}
		return 0;

	default:
		BERROR("Can't happen");
		return -1;
	}
	return -1;
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
		strcpy(bxp->host, host);
		strcpy(bxp->from, from);
		strcpy(bxp->to, to);
		bxp->port = port;
		bxp->state = bxmppInit;
		bxp->stream = NULL;
	}
	return bxp;
}

int bxmpp_destroy(bxmpp_t *bxp)
{
	if (bxp)
	{
		free(bxp);
		return 0;
	}
	return -1;
}


