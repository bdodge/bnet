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
#include "bsasl.h"
#include "bstreamio.h"
#include "butil.h"

const char *bsasl_method_name(bsasl_auth_type_t type)
{
    switch (type)
    {
    case bsaslAuthNone:         return "NONE";
    case bsaslAuthPLAIN:        return "PLAIN";
    case bsaslAuthOAUTH2:       return "X-OAUTH2";
    case bsaslAuthSCRAMSHA1:    return "SCRAM-SHA-1";
    default:                    return "???";
    }
}

static int bsasl_create_nonce(uint8_t *nonce, size_t n)
{
    static uint8_t s_nonce_text[] = "thisissosillyyesitis";
    time_t now;
    uint8_t nmask[4];
    int i;

    time(&now);

    nmask[0] = (now >> 8) & 0xff;
    nmask[1] = now & 0xff;
    nmask[2] = (now >> 24) & 0xff;
    nmask[3] = (now >> 16) & 0xff;

    for (i = 0; i < n; i++)
    {
        nonce[i] = s_nonce_text[i & 0xF];
//        nonce[i] = s_nonce_text[i & 0xF] ^ nmask[i & 0x3];
    }
    return 0;
}

static int bsasl_store_password(
                                uint8_t *dest,
                                size_t ndest,
                                uint8_t *password,
                                size_t passlen,
                                uint8_t *nonce,
                                size_t nonce_len
                                )
{
    strcpy((char *)dest, (char*)password);
    return passlen;
}

static void bsasl_dump_item(const char *label, uint8_t *item, size_t len)
{
    char hv[4];
    int level = 5;
    uint8_t xv;
    size_t i;

    butil_log(level, "%s=\n", label);
    for (i = 0; i < len; i++)
    {
        butil_log(level, "%c", (item[i] >= 32 && item[i] < 127) ? item[i] : ' ');
    }
    butil_log(level, "\n");
    for (i = 0; i < len; i++)
    {
        snprintf(hv, sizeof(hv), "%02X", item[i]);
        butil_log(level, "%c", hv[0]);
    }
    butil_log(level, "\n");
    for (i = 0; i < len; i++)
    {
        snprintf(hv, sizeof(hv), "%02X", item[i]);
        butil_log(level, "%c", hv[1]);
    }
    butil_log(level, "\n\n");
}

int bsasl_start_authentication(bsasl_auth_t *sasl, bsasl_auth_type_t authtype)
{
    size_t len;
    time_t now;
    int i;
    int result;

    sasl->authtype = authtype;
    result = -1;

    switch (authtype)
    {
    case bsaslAuthNone:
    case bsaslAuthPLAIN:
    case bsaslAuthOAUTH2:

        len = 0;

        // skip authorization idendity (let server derive)
        sasl->authMessage[len++] = '\0';

        // add username plain text
        for (i = 0; len < sizeof(sasl->authMessage) - 2; i++)
        {
            sasl->authMessage[len++] = sasl->username[i];
            if (sasl->username[i] == '\0')
            {
                // note 0 is left in message
                break;
            }
        }
        if (len == sizeof(sasl->authMessage) - 2)
        {
            BERROR("username overflow");
            result = -1;
            break;
        }
        // add password plain text
        for (i = 0; len < sizeof(sasl->authMessage) - 1; i++)
        {
            if (sasl->password[i] == '\0')
            {
                break;
            }
            sasl->authMessage[len++] = sasl->password[i];
        }
        if (len == sizeof(sasl->authMessage) - 1)
        {
            BERROR("password overflow");
            result = -1;
            break;
        }
        sasl->authMessage[len] = '\0';

        // base64 encode auth/user/pass into message
        //
        result = butil_base64_encode(
                                    (char*)sasl->clientFinalMessage,
                                    sizeof(sasl->clientFinalMessage),
                                    sasl->authMessage,
                                    len,
                                    false, false
                                    );
        if (result <= 0)
        {
            BERROR("encode overflow");
            result = -1;
            break;
        }
        sasl->clientFinalMessage_len = result;
        result = 0;
        break;

    case bsaslAuthSCRAMSHA1:

        // gs2,name,nonce=
        len = snprintf((char*)sasl->authMessage, sizeof(sasl->authMessage), "n,,n=%s,r=", sasl->username);
        if (len < 0 || len >= sizeof(sasl->authMessage))
        {
            BERROR("user overflow");
            result = -1;
            break;
        }
        // copy nonce into message
        for (i = 0; i < BSASL_NONCE_SIZE && len < sizeof(sasl->authMessage); i++)
        {
            sasl->authMessage[len++] = sasl->clientNonce[i];
        }
        if (len >= sizeof(sasl->authMessage))
        {
            BERROR("user overflow");
            result = -1;
            break;
        }
        // base64 encode it
        result = butil_base64_encode(
                                    (char*)sasl->clientFinalMessage,
                                    sizeof(sasl->clientFinalMessage),
                                    sasl->authMessage,
                                    len,
                                    false,
                                    false
                                    );

        if (result <= 0)
        {
            BERROR("encode overflow");
            result = -1;
            break;
        }
        sasl->clientFinalMessage_len = result;
        result = 0;
        break;

    default:
        BERROR("Not an auth type");
        result = -1;
        break;
    }
    return result;
}

int bsasl_finish_authentication(bsasl_auth_t *sasl, const char *challenge)
{
    size_t challenge_len;
    uint8_t *pc;
    uint8_t *pe;
    size_t len;
    size_t iterations;
    int i;
    int result;

    result = -1;

    switch (sasl->authtype)
    {
    case bsaslAuthPLAIN:
    case bsaslAuthOAUTH2:
        // nothing to do
        result = 0;
        break;

    case bsaslAuthSCRAMSHA1:

        result = butil_base64_decode(sasl->authMessage, sizeof(sasl->authMessage), challenge);
        if (result < BSASL_NONCE_SIZE)
        {
            butil_log(1, "Not enough challenge\n");
            return -1;
        }
        challenge_len = result;
        bsasl_dump_item("Challenge", sasl->authMessage, challenge_len);

        pc = sasl->authMessage;
        if (pc[0] != 'r' || pc[1] != '=')
        {
            butil_log(1, "challenge not r=\n");
            return -1;
        }
        pc+= 2;

        // make sure server replied with our nonce
        //
        if (memcmp(pc, sasl->clientNonce, BSASL_NONCE_SIZE))
        {
            butil_log(1, "Client nonce mismatch\n");
            return -1;
        }
        // get server nonce
        //
        pe = (uint8_t*)strstr((char*)pc, ",s=");
        if (! pe)
        {
            butil_log(1, "No salt section\n");
            return -1;
        }
        sasl->serverNonce_len = (pe - pc);

        if (sasl->serverNonce_len > sizeof(sasl->serverNonce))
        {
            butil_log(1, "nonce overflow");
            return -1;
        }
        memcpy(sasl->serverNonce, pc, sasl->serverNonce_len);

        bsasl_dump_item("serverNonce", sasl->serverNonce, sasl->serverNonce_len);

        // get salt
        //
        pc = pe + 3;
        pe = (uint8_t*)strstr((char*)pc, ",i=");
        if (! pe)
        {
            butil_log(1, "No iterations\n");
            return -1;
        }
        *pe = '\0';
        result = butil_base64_decode(sasl->serverSalt, sizeof(sasl->serverSalt), (char*)pc);
        *pe = ',';

        if (result < 0)
        {
            butil_log(1, "salt overflow");
            return -1;
        }
        sasl->serverSalt_len = result;

        bsasl_dump_item("serverSalt", sasl->serverSalt, sasl->serverSalt_len);

        pe += 3;
        if (! *pe)
        {
            butil_log(1, "No interations\n");
        }
        iterations = strtoul((char*)pe, NULL, 10);

        butil_log(5, "Iterations: %u\n", iterations);

        //clientFinalMessageBare = "c=biws,r=" .. serverNonce

        memcpy(sasl->clientFinalMessageBare, "c=biws,r=", 9);
        memcpy(sasl->clientFinalMessageBare + 9, sasl->serverNonce, sasl->serverNonce_len);

        sasl->clientFinalMessageBare_len = 9 + sasl->serverNonce_len;

        bsasl_dump_item("clientFinalMessageBare",
               sasl->clientFinalMessageBare, sasl->clientFinalMessageBare_len);

        //saltedPassword = PBKDF2-SHA-1(normalizedPassword, salt, i)
        //
        result = iostream_pkcs5_pbkdf2_hmac(
                                sasl->saltedPassword,
                                BSASL_SHA1_KEY_SIZE,
                                sasl->password,
                                strlen((char*)sasl->password),
                                sasl->serverSalt,
                                sasl->serverSalt_len,
                                iterations
                                );
        if (result)
        {
            butil_log(1, "Salting passwd failed\n");
            return -1;
        }
        bsasl_dump_item("saltedPassword", sasl->saltedPassword, BSASL_SHA1_KEY_SIZE);

        //clientKey = HMAC-SHA-1(saltedPassword, "Client Key")

        result = iostream_sha1_hmac(
                                sasl->saltedPassword,
                                BSASL_SHA1_KEY_SIZE,
                                (uint8_t*)"Client Key",
                                10,
                                sasl->clientKey
                                );

        if (result)
        {
            butil_log(1, "clientKey sha failed\n");
            return -1;
        }
        bsasl_dump_item("clientKey", sasl->clientKey, BSASL_SHA1_KEY_SIZE);

        //storedKey = SHA-1(clientKey)
        //
        result = iostream_sha1_hash(sasl->storedKey, sasl->clientKey, BSASL_SHA1_KEY_SIZE);
        if (result)
        {
            butil_log(1, "storedKey sha failed\n");
            return -1;
        }
        bsasl_dump_item("storedKey", sasl->storedKey, BSASL_SHA1_KEY_SIZE);

        //authMessage = initialMessage .. "," .. serverFirstMessage .. "," .. clientFinalMessageBare
        //
        // initial message was sent from clientFinalMessage, note we skip gs2 (n,,) here
        //
        // gs2,name,nonce=
        len = snprintf((char*)sasl->authMessage, sizeof(sasl->authMessage), "n=%s,r=", sasl->username);
        if (len >= sizeof(sasl->authMessage))
        {
            BERROR("user overflow");
            return -1;
        }
        // copy nonce into message
        for (i = 0; i < BSASL_NONCE_SIZE && len < sizeof(sasl->authMessage); i++)
        {
            sasl->authMessage[len++] = sasl->clientNonce[i];
        }
        if ((len + 1) >= sizeof(sasl->authMessage))
        {
            BERROR("user overflow");
            return -1;
        }
        sasl->authMessage[len++] = ',';

        // re-base64 decode server challenge here (since authmessage buf)
        //
        result = butil_base64_decode(sasl->authMessage + len, sizeof(sasl->authMessage) - len, challenge);
        if (result < BSASL_NONCE_SIZE)
        {
            butil_log(1, "Not enough challenge\n");
            return -1;
        }
        len += result;
        if (len >= sizeof(sasl->authMessage))
        {
            butil_log(1, "auth overflow");
            return -1;
        }
        sasl->authMessage[len++] = ',';

        for (i = 0; i < sasl->clientFinalMessageBare_len && len < sizeof(sasl->authMessage); i++)
        {
            sasl->authMessage[len++] = sasl->clientFinalMessageBare[i];
        }
        if (len >= sizeof(sasl->authMessage))
        {
            butil_log(1, "auth overflow");
            return -1;
        }
        sasl->authMessage[len] = '\0';
        sasl->authMessage_len = len;

        bsasl_dump_item("authMessage", sasl->authMessage, sasl->authMessage_len);

        //clientSignature = HMAC-SHA-1(storedKey, authMessage)

        result = iostream_sha1_hmac(
                                sasl->storedKey,
                                BSASL_SHA1_KEY_SIZE,
                                sasl->authMessage,
                                sasl->authMessage_len,
                                sasl->clientSignature
                                );
        if (result)
        {
            butil_log(1, "clientSignature sha failed\n");
            return -1;
        }
        bsasl_dump_item("clientSignature", sasl->clientSignature, BSASL_SHA1_KEY_SIZE);

        //clientProof = clientKey XOR clientSignature

        for (i = 0; i < BSASL_SHA1_KEY_SIZE; i++)
        {
            sasl->clientProof[i] = sasl->clientKey[i] ^ sasl->clientSignature[i];
        }
        bsasl_dump_item("clientProof", sasl->clientProof, BSASL_SHA1_KEY_SIZE);

        //serverKey = HMAC-SHA-1(saltedPassword, "Server Key")

        result = iostream_sha1_hmac(
                                sasl->saltedPassword,
                                BSASL_SHA1_KEY_SIZE,
                                (uint8_t*)"Server Key",
                                10,
                                sasl->serverKey
                                );

        if (result)
        {
            butil_log(1, "serverKey sha failed\n");
            return -1;
        }
        bsasl_dump_item("serverKey", sasl->serverKey, BSASL_SHA1_KEY_SIZE);

        //serverSignature = HMAC-SHA-1(serverKey, authMessage)
        //
        result = iostream_sha1_hmac(
                                sasl->serverKey,
                                BSASL_SHA1_KEY_SIZE,
                                sasl->authMessage,
                                sasl->authMessage_len,
                                sasl->serverSignature
                                );

        if (result)
        {
            butil_log(1, "serverSignature sha failed\n");
            return -1;
        }
        bsasl_dump_item("serverSignature", sasl->serverSignature, BSASL_SHA1_KEY_SIZE);

        //clientFinalMessage = clientFinalMessageBare .. ",p=" .. base64(clientProof)
        //
        len = sasl->clientFinalMessageBare_len;

        if ((len + BSASL_SHA1_KEY_SIZE + 4) >= sizeof(sasl->clientFinalMessage))
        {
            butil_log(1, "final message overflow");
            return -1;
        }
        // re-use authMessage to format final message first, cause needs encoding
        //
        memcpy(sasl->authMessage, sasl->clientFinalMessageBare, len);

        sasl->authMessage[len++] = ',';
        sasl->authMessage[len++] = 'p';
        sasl->authMessage[len++] = '=';
        sasl->authMessage[len] = '\0';

        result = butil_base64_encode(
                                    (char*)sasl->authMessage + len,
                                    sizeof(sasl->authMessage) - len,
                                    sasl->clientProof,
                                    BSASL_SHA1_KEY_SIZE,
                                    false, false
                                    );
        if (result < 0)
        {
            butil_log(1, "proof encode overflow");
            return -1;
        }
        len += result;

        bsasl_dump_item("clientFinalMessage", sasl->authMessage, len);

        result = butil_base64_encode(
                                    (char*)sasl->clientFinalMessage,
                                    sizeof(sasl->clientFinalMessage),
                                    sasl->authMessage,
                                    len,
                                    false, false
                                    );
        if (result < 0)
        {
            butil_log(1, "final message encode overflow");
            return -1;
        }
        sasl->clientFinalMessage_len = result - 1;
        result = 0;
        break;

    default:
        BERROR("Not an auth");
        return -1;
    }
    return result;
}

int bsasl_auth_context_destroy(bsasl_auth_t *sasl)
{
    if (sasl)
    {
        free(sasl);
        return 0;
    }
    return -1;
}

bsasl_auth_t *bsasl_auth_context_create(const char *username, const char *password)
{
    bsasl_auth_t *sasl;
    int result;

    sasl = (bsasl_auth_t *)malloc(sizeof(bsasl_auth_t));
    if (! sasl)
    {
        return NULL;
    }
    memset(sasl, 0, sizeof(bsasl_auth_t));

    // create a nonce
    //
    result = bsasl_create_nonce(sasl->clientNonce, BSASL_NONCE_SIZE);

    // safely store user encrypted with client nonce
    //
    result = bsasl_store_password(
                                sasl->username,
                                sizeof(sasl->username),
                                (uint8_t*)username,
                                strlen(username),
                                sasl->clientNonce,
                                BSASL_NONCE_SIZE
                                );
    if (result < 0)
    {
        BERROR("user overflow");
        free(sasl);
        return NULL;
    }
    // save password for later, but saslPREP it and encrypt it with the
    // the client nonce for security
    //
    result = bsasl_store_password(
                                sasl->password,
                                sizeof(sasl->password),
                                (uint8_t*)password,
                                strlen(password),
                                sasl->clientNonce,
                                BSASL_NONCE_SIZE
                                );
    if (result < 0)
    {
        BERROR("password overflow");
        free(sasl);
        return NULL;
    }
    sasl->password_len = result;

    return sasl;
}

