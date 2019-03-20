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
#ifndef BSIP_H
#define BSIP_H 1

#include "bsipconfig.h"
#include "bhttpconfig.h"
#include "bhttp.h"

#define HTTP_MAX_SIP_SDP    1024
#define HTTP_MAX_SIP_VIA    256
#define HTTP_MAX_SIP_ADDR   128

typedef enum
{
    sipInvite,
    sipAck,
    sipPrack,
    sipCancel,
    sipUpdate,
    sipInfo,
    sipSubscribe,
    sipNotify,
    sipRefer,
    sipMessage,
    sipRegister,
    sipBye
}
sip_method_t;

typedef struct
{
    sip_method_t    method;
    bool            connected;
    char        	sdp[HTTP_MAX_SIP_SDP];
	ioring_t		sdpin;
    char            sip_via[HTTP_MAX_SIP_VIA];
    char            sip_from[HTTP_MAX_SIP_ADDR];
    char            sip_to[HTTP_MAX_SIP_ADDR];
    char            sip_contact[HTTP_MAX_SIP_ADDR];
    char            sip_callid[HTTP_MAX_SIP_ADDR];
    int             sip_depth;
    uint32_t        sip_cseq;
}
sip_server_context_t;

typedef enum
{
    sipIdle,
    sipInboundRinging,
    sipInboundAnswered,
    sipInboundCall,
    sipOutboundRinging,
    sipOutboundAnswered,
    sipOutboundCall
}
sip_state_t;

typedef struct
{
    sip_state_t state;
    char        call_id[HTTP_MAX_SIP_ADDR];
    char        contact[HTTP_MAX_SIP_ADDR];
    struct sockaddr_in remote_rtp_addr;
    socket_t    rtp_socket;
    uint16_t    rtp_port;
    socket_t    rtcp_socket;
    uint16_t    rtcp_port;
}
sip_session_t;

int sip_init_context    (sip_server_context_t *sip);
int sip_process_header  (sip_server_context_t *sip, const char *header);
int sip_slice           (http_client_t *client, sip_server_context_t *sip);
int sip_request         (struct http_client *client, sip_server_context_t *sip);

#endif
