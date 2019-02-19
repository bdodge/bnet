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

#define BXMPP_PORT 5222

#define BXMPP_IO_SIZE 1436

#define BXMPP_MAX_HOST  256
#define BXMPP_MAX_ADDR  128
#define BXMPP_MAX_JID   128

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
    bxmppSession,
    bxmppSessionReply,
    bxmppConnected,
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

typedef struct
{
    char host[BXMPP_MAX_HOST];
    char user[BXMPP_MAX_ADDR];
    char pass[BXMPP_MAX_ADDR];
    char id[BXMPP_MAX_ADDR];
    char jid[BXMPP_MAX_JID];
    uint16_t        port;
    bxmpp_layer_t   layer;
    bxmpp_state_t   state;
    bxmpp_state_t   next_state;
    bsasl_auth_type_t authtype;
    bsasl_auth_t   *sasl;
    bxml_parser_t   xmlparser;
    bxml_parser_t  *pxp;
    char            abuf[BXMPP_IO_SIZE];
    char            ibuf[BXMPP_IO_SIZE];
    char            obuf[BXMPP_IO_SIZE];
    ioring_t        in;
    ioring_t        out;
    iostream_t     *stream;
}
bxmpp_t;

int bxmpp_setup(bxmpp_t *bxp);

bxmpp_t *bxmpp_create(
                        const char *host,
                        uint16_t    port,
                        const char *user,
                        const char *password,
                        const char *id
                    );

int bxmpp_destroy(bxmpp_t *bxp);

#endif

