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
#ifndef BGCP_H
#define BGCP_H 1

#include "bgcpconfig.h"
#include "bhttpconfig.h"
#include "bhttp.h"
#include "bjson.h"
#include "bstreamio.h"
#include "butil.h"

#if 0       // define to include memory tracing debug
#define BMEM_TRACE_ALLOCS 1
#include "bmem.h"
#define malloc bmem_alloc
#define free bmem_free
#endif

typedef enum
{
    gcpInit,
    gcpAnonRegister,
    gcpAnonRegisterReply,
    gcpPromptForClaim,
    gcpWaitForClaim,
    gcpGetAuthCode,
    gcpPollForAuthorization,
    gcpAuthorizationReply,
    gcpGetAccessToken,
    gcpAccessTokenReply,
    gcpListPrinter,
    gcpListPrinterReply,
    gcpFetch,
    gcpFetchReply,
    gcpPollWait,
    gcpGetReply,
    gcpError,
    gcpFatal
}
gcp_state_t;

/// Context for an cloud print entity (a printer)
//
typedef struct tag_gcp_context
{
    gcp_state_t         state;
    gcp_state_t         prevstate;
    gcp_state_t         nextstate;
    char                uuid[64];
    char                proxy_id[64];
    char                printer_id[GCP_SHORT_TOKEN];
    char                complete_invite_url[HTTP_MAX_URL];
    char                invite_url[HTTP_MAX_URL];
    char                invite_page_url[HTTP_MAX_URL];
    char                registration_token[GCP_SHORT_TOKEN];
    char                oath_scope[HTTP_MAX_URL];
    char                automated_invite_url[HTTP_MAX_URL];
    char                polling_url[HTTP_MAX_URL];
    time_t              token_duration;
    char                confirmation_page_url[HTTP_MAX_URL];
    char                xmpp_jid[GCP_SHORT_TOKEN];
    time_t              next_poll;
    char                authorization_code[GCP_MAX_TOKEN];
    char                refresh_token[GCP_MAX_TOKEN];
    char                access_token[GCP_MAX_TOKEN];
    http_client_t      *http_client;
    http_resource_t    *http_resources;
    char                cds[GCP_MAX_CDS];
    char                cdd[GCP_MAX_CDD];
    ioring_t            io;
    uint8_t             io_data[GCP_IO_SIZE];
}
gcp_context_t;

const char *gcp_state_string(gcp_state_t state);
#if 1
#define gcp_set_error(a, b) ipp_set_error_dbg(__FILE__, __LINE__, a, b)
int gcp_set_error_dbg (const char* fname, int line, gcp_context_t *gcp, int ecode);
#else
int gcp_set_error     (gcp_context_t *gcp, int ecode);
#endif
int gcp_init    (gcp_context_t *gcp, const char *proxy_id, const char *uuid);
int gcp_deinit  (gcp_context_t *gcp);
int gcp_slice   (gcp_context_t *gcp);

// for unit test
int gcp_anon_register_reply(gcp_context_t *gcp);
int gcp_prompt_for_claim(gcp_context_t *gcp);
int gcp_wait_for_claim(gcp_context_t *gcp, bool *cancel);
int gcp_authorization_reply(gcp_context_t *gcp, bool *done);

#endif

