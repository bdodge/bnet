#ifndef BHTTPSIP_H
#define BHTTPSIP_H 1

struct http_client;

#define HTTP_MAX_SIP_VIA	256
#define HTTP_MAX_SIP_ADDR	128

typedef struct
{
	bool connected;
}
webdav_sipsession_t;

int http_sip_slice(struct http_client *client);
int http_sip_request(struct http_client *client);
int sip_phone(int argc, char **argv);

#endif

