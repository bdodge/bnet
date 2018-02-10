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

