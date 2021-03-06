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
#ifndef BSASL_H
#define BSASL_H 1

#include "bnetheaders.h"

/// define non-0 to support Google extensions like OAuth2 for XMPP
//
#define SASL_SUPPORT_GOOGLE_EXTENSIONS  (1)

#define BSASL_NONCE_SIZE    24
#define BSASL_MAX_NONCE     128
#if SASL_SUPPORT_GOOGLE_EXTENSIONS
#define BSASL_MAX_AUTHLEN   2048
#else
#define BSASL_MAX_AUTHLEN   256
#endif
#define BSASL_SHA1_KEY_SIZE 20
#define BSASL_MAX_KEY_SIZE  20

typedef enum
{
    bsaslAuthNone,
    bsaslAuthPLAIN,
#if SASL_SUPPORT_GOOGLE_EXTENSIONS
    bsaslAuthOAUTH2,
    bsaslAuthGOOGLETOKEN,
#endif
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

