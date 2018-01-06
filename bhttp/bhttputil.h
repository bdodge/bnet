#ifndef HTTPUTIL_H
#define HTTPUTIL_H 1

/**
 *  Log
 */
#define HTTP_LOG_LEVEL(n) ((n) & 0xFFFF)
#define HTTP_LOG_TAG(n)   (((uint32_t)(n)) >> 16)
#define LOGTAG(t, n)      ((((uint32_t)(t)) << 16) | ((uint32_t)(n) & 0xFFFF))

void http_log(uint32_t level, const char *fmt, ...);
void http_set_log_level(uint32_t level);

int http_join_path(char *path, size_t room, const char *root, const char *base, const char *file);

int http_ncasecmp(const char *haystack, const char *needle);

const char *http_scheme_name(http_scheme_t method);
const char *http_scheme_base_name(http_scheme_t method);
int http_scheme_from_name(const char *name, http_scheme_t *scheme);

const char *http_method_name(http_method_t method);
int http_method_from_name(const char *name, http_method_t *method);

/**
 *  Split a URL into components
 */
int http_parse_url(
                    const char *url,
                    http_scheme_t *scheme,
                    char *host,
                    short *port,
                    char *path
                    );
/**
 *  Paste URL components into a URL
 */
int http_paste_url(
                    char *url,
                    const http_scheme_t scheme,
                    const char *host,
                    const uint16_t port,
                    const char *path
                    );

int http_base64_decode(uint8_t *out, size_t outsize, const char *src);
int http_base64_encode(char *out, size_t outsize, const uint8_t *src, size_t srcbytes, bool hexencode);

int http_auth_string_to_type(const char *auth_str, http_auth_type_t *auth_type);
const char *http_auth_type_to_string(http_auth_type_t auth_type);

time_t http_rfc2616_date_to_time(const char *date);
const char *http_time_to_rfc2616_date(time_t when, char *buf, size_t nbuf);

const char *http_mime_string_for_file(const char *path);
http_content_type_t http_content_type_for_file(const char *path);

const char *http_mime_string_for_content_type(http_content_type_t type);
http_content_type_t http_content_type_for_mime_string(const char *mime);

#endif

