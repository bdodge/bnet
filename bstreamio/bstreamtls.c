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
#include "bstreamio.h"

#if BNET_TLS

#include "mbedtls/config.h"
#include "mbedtls/ssl.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509_csr.h"
#include "mbedtls/entropy.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/sha1.h"
#include "mbedtls/net.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"

// private key and crt generated with the following script
//
//openssl req \
//    -newkey rsa:2048 -nodes -keyout ht_domain.key \
//    -subj '/C=US/ST=Massachusetts/L=Boston/CN=ht' \
//    -out ht_domain.csr
//
//openssl x509 \
//    -signkey ht_domain.key \
//    -in ht_domain.csr \
//    -req -days 3650 -out ht_domain.crt
//
//xxd -i ht_domain.key ht_privkey.h
//
//xxd -i ht_domain.crt ht_ourcrt.h

// our own private key
//
#include "certs/ht_privkey.h"

// our own cert
//
#include "certs/ht_ourcrt.h"

// our ca chain, gotten by
//
//openssl s_client -host google.com -port 443 -prexit -showcerts
// (and hand editing the result to get just the certs)
//
#include "certs/ht_cachain.h"

// global TLS context
//
static mbedtls_x509_crt         s_tls_certificate_chain;
static mbedtls_x509_crt         s_tls_our_certificate;
static mbedtls_pk_context       s_tls_private_key;
static mbedtls_ctr_drbg_context s_tls_ctr_drbg;

// per connection TLS context
//
typedef struct
{
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config  conf;
    bool                isclient;
    iostream_t         *stream;
}
tlsctx_t;

#if defined(MBEDTLS_DEBUG_C)

/// set this from 0 to 5 to get debug output on console. 0 is none, 5 is verbose
///
#define TLS_DEBUG_LEVEL 1

static void tls_log(int level, const char *fmt, ...)
{
    va_list args;

    if (level > TLS_DEBUG_LEVEL)
    {
        return;
    }
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

static void iostream_tls_debug(
                        void *ctx, int level,
                        const char *file, int line,
                        const char *str
                     )
{
    tls_log(1, "%s:%04d: %s", file, line, str);
}

#else

#define tls_log

#endif

int iostream_sha1_hash(uint8_t *result, uint8_t *source, size_t bytes)
{
    mbedtls_sha1_context shactx;

    if (! source || ! bytes)
    {
        return -1;
    }
    mbedtls_sha1_init(&shactx);
    mbedtls_sha1_starts(&shactx);
    mbedtls_sha1_update(&shactx, source, bytes);
    mbedtls_sha1_finish(&shactx, result);
    mbedtls_sha1_free(&shactx);
    return 0;
}

int iostream_pkcs5_pbkdf2_hmac(
                                uint8_t *key,
                                size_t keylen,
                                const uint8_t *password,
                                size_t passlen,
                                const uint8_t *salt,
                                size_t saltlen,
                                size_t iterations
                                )
{
    mbedtls_md_context_t sha1_ctx;
    const mbedtls_md_info_t *info_sha1;
    int result;

    if (! key || ! keylen || ! password || ! passlen || ! salt || ! saltlen)
    {
        return -1;
    }
    mbedtls_md_init(&sha1_ctx);

    do // try
    {
        info_sha1 = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);
        if(info_sha1 == NULL)
        {
            result = -1;
            break;
        }
        result = mbedtls_md_setup(&sha1_ctx, info_sha1, 1);
        if (result)
        {
            break;
        }
        result = mbedtls_pkcs5_pbkdf2_hmac(
                                        &sha1_ctx,
                                        password, passlen,
                                        salt, saltlen,
                                        iterations,
                                        keylen, key
                                        );
    }
    while (0); // catch

    mbedtls_md_free(&sha1_ctx);
    return result;
}

int iostream_sha1_hmac(
                                const uint8_t *key,
                                size_t keylen,
                                const uint8_t *input,
                                size_t ilen,
                                uint8_t *output
                      )
{
    const mbedtls_md_info_t *info_sha1;
    int result;

    if (! key || ! keylen || ! input || ! ilen || ! output)
    {
        return -1;
    }
    do // try
    {
        info_sha1 = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);
        if(info_sha1 == NULL)
        {
            result = -1;
            break;
        }
        result = mbedtls_md_hmac(info_sha1, key, keylen, input, ilen, output);
        if (result)
        {
            break;
        }
    }
    while (0); // catch

    return result;
}

static int iostream_tls_read(iostream_t *stream, uint8_t *buf, int len)
{
    tlsctx_t *tls = (tlsctx_t *)stream->priv;
    int result;

    if (! tls || ! tls->stream)
    {
        return MBEDTLS_ERR_NET_INVALID_CONTEXT;
    }
    do
    {
        result = mbedtls_ssl_read(&tls->ssl, buf, len);

        if (result <= 0)
        {
            switch(result)
            {
            case MBEDTLS_ERR_SSL_WANT_READ:
            case MBEDTLS_ERR_SSL_WANT_WRITE:
                // need to drive protocol, try again
                result = 0;
                break;

            case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
                tls_log(5, "ssl closed\n");
                return -1;

            case MBEDTLS_ERR_NET_CONN_RESET:
                tls_log(5, "ssl reset\n");
                return -11;

            default:
                tls_log(1, "ssl error %X\n", -result);
                return -1;
            }
        }
    }
    while (! result);

    return result;
}

static int iostream_tls_write(iostream_t *stream, uint8_t *buf, int len)
{
    tlsctx_t *tls = (tlsctx_t *)stream->priv;
    int result;

    if (! tls || ! tls->stream)
    {
        return MBEDTLS_ERR_NET_INVALID_CONTEXT;
    }
    do
    {
        result = mbedtls_ssl_write(&tls->ssl, buf, len);

        if (result <= 0)
        {
            switch(result)
            {
            case MBEDTLS_ERR_SSL_WANT_READ:
            case MBEDTLS_ERR_SSL_WANT_WRITE:
                // need to drive protocol, try again
                result = 0;
                break;

            case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
                tls_log(5, "ssl closed\n");
                return -1;

            case MBEDTLS_ERR_NET_CONN_RESET:
                tls_log(5, "ssl reset\n");
                return -11;

            default:
                tls_log(1, "ssl error %X\n", -result);
                return -1;
            }
        }
    }
    while (! result);

    return result;
}

static int iostream_tls_poll(iostream_t *stream, polldir_t pollfor, int to_secs, int to_usecs)
{
    tlsctx_t *tls = (tlsctx_t *)stream->priv;
    size_t avail;

    if (! tls || ! tls->stream)
    {
        return MBEDTLS_ERR_NET_INVALID_CONTEXT;
    }
    avail = mbedtls_ssl_get_bytes_avail(&tls->ssl);
    if (avail)
    {
        return 1;
    }
    return iostream_posix_poll(tls->stream, pollfor, to_secs, to_usecs);
}

static int iostream_tls_close(iostream_t *stream)
{
    tlsctx_t *tls = (tlsctx_t *)stream->priv;

    if (! tls)
    {
        BERROR("No ctx");
        return -1;
    }
    if (tls->stream)
    {
        tls->stream->close(tls->stream);
    }
    mbedtls_ssl_config_free(&tls->conf);
    mbedtls_ssl_free(&tls->ssl);
    free(tls);

    return 0;
}

int iostream_tls_send(void *tlsctx, const unsigned char *buf, size_t len)
{
    tlsctx_t *tls = (tlsctx_t *)tlsctx;
    int wrote, wc;

    if (! tls || ! tls->stream)
    {
        return MBEDTLS_ERR_NET_INVALID_CONTEXT;
    }
    for (wrote = 0; wrote < len;)
    {
        wc = tls->stream->poll(tls->stream, writeable, 0, 10000);
        if (wc < 0)
        {
            return MBEDTLS_ERR_NET_CONN_RESET;
        }
        if (wc == 0)
        {
            return MBEDTLS_ERR_SSL_WANT_WRITE;
        }
        if (wc > 0)
        {
            wc = tls->stream->write(tls->stream, (uint8_t*)buf, len);
            if (wc <= 0)
            {
                tls_log(1, "wc=%d after poll\n", wc);
                return MBEDTLS_ERR_NET_CONN_RESET;
            }
            wrote += wc;
            buf += wc;
        }
    }
    return wrote;
}

int iostream_tls_recv(void *tlsctx, unsigned char *buf, size_t len)
{
    tlsctx_t *tls = (tlsctx_t *)tlsctx;
    int readtot, rc;

    if (! tls || ! tls->stream)
    {
        return MBEDTLS_ERR_NET_INVALID_CONTEXT;
    }
    readtot = 0;

    rc = tls->stream->poll(tls->stream, readable, 0, 10000);
    if (rc < 0)
    {
        return MBEDTLS_ERR_NET_CONN_RESET;
    }
    if (rc == 0)
    {
        return MBEDTLS_ERR_SSL_WANT_READ;
    }
    if (rc > 0)
    {
        rc = tls->stream->read(tls->stream, (uint8_t*)buf, len);
        if (rc <= 0)
        {
            return MBEDTLS_ERR_NET_CONN_RESET;
        }
        readtot += rc;
        buf += rc;
    }
    return readtot;
}

iostream_t *iostream_tls_create_from_iostream(iostream_t *instream, bool isclient)
{
    iostream_t *stream;
    tlsctx_t   *tls;
    int         result;
    mbedtls_x509_crt ca;

    if (! instream)
    {
        return NULL;
    }
    tls = (tlsctx_t *)malloc(sizeof(tlsctx_t));
    if (! tls)
    {
        BERROR("Can't alloc TLS ctx");
        instream->close(instream);
        return NULL;
    }
    // swallow input stream
    //
    tls->stream = instream;
    tls->isclient = isclient;

    // allocate new stream object to hold the tls context
    //
    stream = iostream_alloc();
    if (! stream)
    {
        BERROR("Can't alloc stream");
        instream->close(instream);
        free(tls);
        return NULL;
    }
    stream->read    = iostream_tls_read;
    stream->write   = iostream_tls_write;
    stream->poll    = iostream_tls_poll;
    stream->close   = iostream_tls_close;
    stream->priv    = (void*)tls;

    // init ssl context and configuration
    //
    mbedtls_ssl_init(&tls->ssl);
    mbedtls_ssl_config_init(&tls->conf);

    // set conf for client or server use
    //
    if (mbedtls_ssl_config_defaults(
                                    &tls->conf,
                                    isclient ? MBEDTLS_SSL_IS_CLIENT : MBEDTLS_SSL_IS_SERVER,
                                    MBEDTLS_SSL_TRANSPORT_STREAM,
                                    MBEDTLS_SSL_PRESET_DEFAULT
                                   )
    )
    {
        tls_log(2, "SSL config failed");
        iostream_tls_close(stream);
        return NULL;
    }
    #if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(TLS_DEBUG_LEVEL);
    mbedtls_ssl_conf_dbg(&tls->conf, iostream_tls_debug, NULL);
    #endif

    // configure random generator
    //
    mbedtls_ssl_conf_rng(&tls->conf, mbedtls_ctr_drbg_random, &s_tls_ctr_drbg);

    //#if defined(MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED)
    //    mbedtls_ssl_conf_psk(&tls->conf, psk, sizeof(psk),
    //                (const uint8_t *)psk_id, sizeof(psk_id) - 1);
    //#endif

    // configure our certificate
    //
    result = mbedtls_ssl_conf_own_cert(&tls->conf, &s_tls_our_certificate, &s_tls_private_key);
    if (result)
    {
        BERROR("SSL crt conf failed");
        iostream_tls_close(stream);
    }
    // configure the certificate chain for use by clients
    //
    if (isclient)
    {
        mbedtls_ssl_conf_ca_chain(&tls->conf, &s_tls_certificate_chain, NULL);
    }
    // configure the authorization failure mode
    //
    mbedtls_ssl_conf_authmode(&tls->conf,
              //        MBEDTLS_SSL_VERIFY_REQUIRED
                        MBEDTLS_SSL_VERIFY_OPTIONAL
                            );

    // setup the ssl context
    //
    if (mbedtls_ssl_setup(&tls->ssl, &tls->conf))
    {
        tls_log(2, "SSL setup failed");
        iostream_tls_close(stream);
        return NULL;
    }
    // setup I/O hooks for the TLS layer to read/write from our stream
    //
    mbedtls_ssl_set_bio(&tls->ssl, tls, iostream_tls_send, iostream_tls_recv, NULL);

    // now do a hand-shake
    //
    do {
        result = mbedtls_ssl_handshake(&tls->ssl);
        if (
               result
            && result != MBEDTLS_ERR_SSL_WANT_READ
            && result != MBEDTLS_ERR_SSL_WANT_WRITE
        )
        {
            break;
        }
    }
    while (result != 0);

    if (result)
    {
        BERROR("SSL handshake failed");
        iostream_tls_close(stream);
        return NULL;
    }
    return stream;
}

int iostream_tls_prolog(void)
{
    static bool tls_is_setup = false;
    mbedtls_entropy_context  entropy;
    const char *entropy_string = "this is random";
    int result;

    if (tls_is_setup)
    {
        return 0;
    }
    // init random number generator interface
    //
    mbedtls_ctr_drbg_init(&s_tls_ctr_drbg);
    mbedtls_entropy_init(&entropy);

    // and use it to seed the random number generator
    //
    if (mbedtls_ctr_drbg_seed(&s_tls_ctr_drbg, mbedtls_entropy_func, &entropy,
                       (const uint8_t*)entropy_string, strlen(entropy_string)))
    {
        BERROR("TLS drbg seed failed");
        return -1;
    }
    // init and parse the certificate authority chain
    //
    mbedtls_x509_crt_init(&s_tls_certificate_chain);

    result = mbedtls_x509_crt_parse(&s_tls_certificate_chain,
                ht_ca_cert_chain, ht_ca_cert_chain_len);
    if (result)
    {
        tls_log(1, "Ca chain load: %X\n", -result);
        BERROR("Certificate chain load failed");
        return -1;
    }
    // init and parse our certificate
    //
    mbedtls_x509_crt_init(&s_tls_our_certificate);

    result = mbedtls_x509_crt_parse(&s_tls_our_certificate,
                ht_domain_crt, ht_domain_crt_len);
    if (result)
    {
        tls_log(1, "CRT load: %X\n", -result);
        BERROR("Certificate load failed");
        return -1;
    }
    // init and parse the private key
    //
    mbedtls_pk_init(&s_tls_private_key);

    result = mbedtls_pk_parse_key(&s_tls_private_key,
                 ht_domain_key, ht_domain_key_len, NULL, 0);
    if (result)
    {
        tls_log(1, "PK load: %X\n", -result);
        BERROR("Private key load failed");
        return -1;
    }
    tls_is_setup = true;
    return 0;
}

int iostream_tls_epilog(void)
{
    return 0;
}

#endif // TLS SUPPORTED

