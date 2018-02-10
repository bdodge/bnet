#ifndef BSASL_H
#define BSASL_H 1

#include "bnetheaders.h"
#include "bstreamio.h"
#include "butil.h"

#define BSASL_NONCE_SIZE    24
#define BSASL_MAX_NONCE     128
#define BSASL_MAX_AUTHLEN   128

#define BSASL_SHA1_KEY_SIZE 20
#define BSASL_MAX_KEY_SIZE  20

typedef enum
{
    bsaslAuthNone,
    bsaslAuthPLAIN,
    bsaslAuthOAUTH2,
    bsaslAuthSCRAMSHA1
}
bsasl_auth_type_t;

typedef struct
{
    bsasl_auth_type_t authtype;
    uint8_t username[BSASL_MAX_AUTHLEN];
    uint8_t password[BSASL_MAX_AUTHLEN];
    size_t  password_len;
    uint8_t clientNonce[BSASL_NONCE_SIZE];
    uint8_t serverNonce[BSASL_MAX_NONCE];
    size_t  serverNonce_len;
    uint8_t serverSalt[BSASL_MAX_KEY_SIZE];
    size_t  serverSalt_len;
    uint8_t saltedPassword[BSASL_MAX_KEY_SIZE];
    uint8_t clientKey[BSASL_MAX_KEY_SIZE];
    uint8_t storedKey[BSASL_MAX_KEY_SIZE];
    uint8_t clientSignature[BSASL_MAX_KEY_SIZE];
    uint8_t clientProof[BSASL_MAX_KEY_SIZE];
    uint8_t serverKey[BSASL_MAX_KEY_SIZE];
    uint8_t serverSignature[BSASL_MAX_KEY_SIZE];
    uint8_t authMessage[3 * BSASL_MAX_AUTHLEN];
    size_t  authMessage_len;
    uint8_t clientFinalMessageBare[2 * BSASL_MAX_AUTHLEN];
    size_t  clientFinalMessageBare_len;
    uint8_t clientFinalMessage[2 * BSASL_MAX_AUTHLEN];
    size_t  clientFinalMessage_len;
}
bsasl_auth_t;

const char *bsasl_method_name(bsasl_auth_type_t type);

int bsasl_start_authentication(bsasl_auth_t *sasl, bsasl_auth_type_t authtype);
int bsasl_finish_authentication(bsasl_auth_t *sasl, const char *challenge);

int bsasl_auth_context_destroy(bsasl_auth_t *sasl);
bsasl_auth_t *bsasl_auth_context_create(const char *username, const char *password);

#endif

