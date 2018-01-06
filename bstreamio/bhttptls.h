#ifndef HTTPTLS_H
#define HTTPTLS_H 1

int http_sha1_hash(uint8_t *result, uint8_t *source, size_t bytes);

iostream_t *iostream_tls_create_from_iostream(iostream_t *stream, bool isclient);

int http_tls_prolog(void);
int http_tls_epilog(void);

#endif

