#ifndef HTTPDAV_H
#define HTTPDAV_H 1

struct http_client;

typedef struct
{
    int     pathpos;    ///< end of path when entry pushed
    DIR     *d;         ///< dir enumeration state
}
webdav_findstate_t;

void http_webdav_findstate_init(struct http_client *client);
int http_webdav_find_slice(struct http_client *client);
int http_webdav_request(struct http_client *client);

#endif

