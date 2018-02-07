#ifndef BXMPP_H
#define BXMPP_H 1

#include "bnetheaders.h"
#include "bxml.h"
#include "bstreamio.h"

#define BXMPP_PORT 5222

#define BXMPP_MAX_HOST  256
#define BXMPP_MAX_ADDR  128

typedef enum
{
    bxmppDone,
    bxmppInit,
    bxmppTransport,
    bxmppTLS,
    bxmppSASL,
    bxmppConnected
}
bxmpp_state_t;

typedef struct
{
    char host[BXMPP_MAX_HOST];
    char to[BXMPP_MAX_ADDR];
    char from[BXMPP_MAX_ADDR];
    uint16_t port;

    bxmpp_state_t   state;

    iostream_t *stream;
}
bxmpp_t;

int bxmpp_setup(bxmpp_t *bxp);

bxmpp_t *bxmpp_create(const char *host, uint16_t port, const char *from, const char *to);

int bxmpp_destroy(bxmpp_t *bxp);

#endif

