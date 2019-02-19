#ifndef BUTIL_H
#define BUTIL_H 1

#include "bnetheaders.h"

#define BUTIL_MAX_URL_SCHEME 8
#define BUTIL_MAX_PORTSPEC   5

void butil_log(uint32_t level, const char *fmt, ...);
void butil_set_log_level(uint32_t level);

int butil_hextou(char digit, uint8_t *val);
int butil_is_white(char ch);
int butil_is_number(char ch);

size_t butil_utf8_encode(uint32_t unicode, uint8_t utfbuf[5]);

int butil_base64_decode(
                        uint8_t        *out,
                        size_t          outsize,
                        const char     *src
                        );

int butil_base64_encode(
                        char           *out,
                        size_t          outsize,
                        const uint8_t  *src,
                        size_t          srcbytes,
                        bool            urlencode,
                        bool            hexescape
                        );

typedef enum
{
    schemeFTP,
    schemeSFTP,
    schemeHTTP,
    schemeHTTPS,
    schemeWS,
    schemeWSS,
    schemeSIP,
    schemeSIPS,
}
butil_url_scheme_t;

const char *butil_scheme_name(
                        butil_url_scheme_t method
                        );

int butil_scheme_from_name(
                        const char         *name,
                        butil_url_scheme_t *scheme
                        );

int butil_parse_url(
                        const char         *url,
                        butil_url_scheme_t *scheme,
                        char               *host,
                        size_t              nhost,
                        uint16_t           *port,
                        char               *path,
                        size_t              npath
                        );

int butil_paste_url(
                        char               *url,
                        size_t              nurl,
                        const butil_url_scheme_t scheme,
                        const char         *host,
                        const uint16_t      port,
                        const char         *path
                        );


#endif

