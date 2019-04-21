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
#ifndef BXMPP_H
#define BXMPP_H 1

#include "bnetheaders.h"
#include "bstreamio.h"
#include "butil.h"
#include "bxml.h"
#include "bsasl.h"

#define BXMPP_PORT          (5222)
#define BXMPP_TLS_PORT      (443)

#if SASL_SUPPORT_GOOGLE_EXTESNSIONS
#define BXMPP_IO_SIZE       (4096)
#else
#define BXMPP_IO_SIZE       (1436)
#endif
#define BXMPP_IO_TIMEOUT    (15)

#define BXMPP_MAX_HOST      (256)
#define BXMPP_MAX_ADDR      (256)
#define BXMPP_MAX_JID       (256)

struct tag_bxmpp;

typedef int (*xmpp_msg_callback_t)(struct tag_bxmpp *bxp, void *priv, const char *sender, const char *message);

typedef enum
{
    bxmppDone,
    bxmppInit,
    bxmppTransport,
    bxmppTransportReply,
    bxmppTLS,
    bxmppTLSproceed,
    bxmppSASL,
    bxmppSASLreply,
    bxmppSCRAM,
    bxmppSCRAMreply,
    bxmppBind,
    bxmppBindReply,
    bxmppConnected,
    bxmppCheckIQreply,
    bxmppOutPhase,
    bxmppInPhase
}
bxmpp_state_t;

typedef enum
{
    bxmppLayerNone,
    bxmppLayerTCP,
    bxmppLayerTLS,
    bxmppLayerSASL,
    bxmppLayerSession
}
bxmpp_layer_t;

typedef struct tag_bxmpp
{
    char                host[BXMPP_MAX_HOST];
    char                user[BXMPP_MAX_ADDR];
    char                pass[BXMPP_MAX_ADDR];
    char                id[BXMPP_MAX_ADDR];
    char                jid[BXMPP_MAX_JID];
    xmpp_msg_callback_t message_callback;
    void               *message_priv;
    uint16_t            port;
    bxmpp_layer_t       layer;
    bxmpp_state_t       state;
    bxmpp_state_t       next_state;
    bsasl_auth_type_t   authtype;
    bsasl_auth_type_t   authpreferred;
    bsasl_auth_t       *sasl;
    bxml_parser_t       xmlparser;
    bxml_parser_t      *pxp;
    uint32_t            next_id;
    char                idbuf[16];
    char                abuf[BXMPP_IO_SIZE];
    char                ibuf[BXMPP_IO_SIZE];
    char                obuf[BXMPP_IO_SIZE];
    ioring_t            in;
    ioring_t            out;
    iostream_t         *stream;
    time_t              ping_period;
    time_t              ping_time;
    time_t              long_timeout;
}
bxmpp_t;

int bxmpp_finished      (bxmpp_t *bxp);
int bxmpp_connected     (bxmpp_t *bxp);
int bxmpp_slice         (bxmpp_t *bxp);
int bxmpp_send_message  (bxmpp_t *bxp, const char *recipient, const char *msg);
int bxmpp_send_infoquery(bxmpp_t *bxp, bool is_query, const char *recipient, const char *msg);
int bxmpp_restart       (bxmpp_t *bxp, const char *user, const char *password);

bxmpp_t *bxmpp_create   (
                        const char        *host,
                        uint16_t           port,
                        bsasl_auth_type_t  preferred_auth,
                        const char        *user,
                        const char        *password,
                        const char        *id,
                        time_t             ping_period,
                        xmpp_msg_callback_t callback,
                        void              *priv
                        );

int bxmpp_destroy       (bxmpp_t *bxp);

#endif

