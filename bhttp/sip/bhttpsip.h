#ifndef BHTTPSIP_H
#define BHTTPSIP_H 1

struct http_client;

#define HTTP_MAX_SIP_VIA	256
#define HTTP_MAX_SIP_ADDR	128

typedef struct
{
    char                sip_via[HTTP_MAX_SIP_VIA];
    char                sip_from[HTTP_MAX_SIP_ADDR];
    char                sip_to[HTTP_MAX_SIP_ADDR];
    char                sip_contact[HTTP_MAX_SIP_ADDR];
    char                sip_callid[HTTP_MAX_SIP_ADDR];
    int                 sip_depth;
    uint32_t            sip_cseq;
}
web_sip_context_t;

typedef struct
{
	bool connected;
}
web_sipsession_t;

int http_sip_slice(struct http_client *client);
int http_sip_request(struct http_client *client);

int sip_phone(int argc, char **argv);

#endif

