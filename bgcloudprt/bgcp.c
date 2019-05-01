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
#include "bgcp.h"
#include "bgcpio.h"
#include "bgcpxmpp.h"
#include "bgcplocalprt.h"
#include "bgcpnv.h"
#include "bgcpcdd.h"

bool gcp_is_registered(gcp_context_t *gcp)
{
    if (gcp && gcp->printer_id[0] && gcp->refresh_token[0])
    {
        return true;
    }
    return false;
}

int gcp_anon_register(gcp_context_t *gcp)
{
    char verstring[8];
    int result;

    #if GCP_SUPPORT_LOCAL_PRT
    char localprt[256];

    snprintf(localprt, sizeof(localprt),
                "{\r\n"
                " \"current\": {\r\n"
                "  \"local_discovery\": %s,\r\n"
                "  \"access_token_enabled\": true,\r\n"
                "  \"printer/local_printing_enabled\": %s,\r\n"
                "  \"printer/conversion_printing_enabled\": true,\r\n"
                "  \"xmpp_timeout_value\": %u\r\n"
                " }\r\n"
                "}\r\n",
                gcp->responding ? "true" : "false",
                gcp->local_prt_enabled ? "true" : "false",
                GCP_XMPP_CLIENT_PING_PERIOD
                );
    #endif

    if (! gcp)
    {
        return -1;
    }
    if (gcp->state != gcpAnonRegister)
    {
        BERROR("wrong state");
        return -1;
    }
    butil_log(5, "Registering - Anonymous\n");

    snprintf(verstring, sizeof(verstring), "%d.%d", GCP_VER_MAJ, GCP_VER_MIN);

    // mime encode the parameters
    //
    result = gcp_encode_parameters(
                gcp,
                "name", butil_mime_text, "BNET",
                "proxy", butil_mime_text, gcp->proxy_id,
                "uuid", butil_mime_text, gcp->uuid,
                "manufacturer", butil_mime_text, "bnet",
                "model", butil_mime_text, "gcp20",
                "gcp_version", butil_mime_text, verstring,
                "setup_url", butil_mime_text, "http://www.msn.com",
                "support_url", butil_mime_text, "http://www.cnn.com",
                "update_url", butil_mime_text, "http://www.abc.com",
                "firmware", butil_mime_text, "1.1",
                "semantic_state", butil_mime_json, gcp->cds,
                "use_cdd", butil_mime_text, "true",
                "capabilities", butil_mime_json, gcp->cdd,
    #if GCP_SUPPORT_LOCAL_PRT
                "local_settings", butil_mime_json, localprt,
    #endif
                "sentinal", butil_mime_text, NULL
            );
    if (result)
    {
        return result;
    }
    result = gcp_request(gcp, GCP_CLOUD_URL, "register");

    return result;
}

int gcp_anon_register_reply(gcp_context_t *gcp)
{
    char value[GCP_SHORT_TOKEN];
    bool success;
    int result;

    // check reply status
    //
    result = gcp_reply_status(gcp, &success);
    if (result)
    {
        return result;
    }
    if (! success)
    {
        return -2;
    }
    // get things we need out of reply
    //
    result = gcp_reply_value(gcp, "printers.id",
                    gcp->printer_id, sizeof(gcp->printer_id));
    if (! result)
    {
        butil_log(5, "Registering Printer ID:%s\n", gcp->printer_id);
    }

    result |= gcp_reply_value(gcp, "invite_url",
                    gcp->invite_url, sizeof(gcp->invite_url));
    result |= gcp_reply_value(gcp, "complete_invite_url",
                    gcp->complete_invite_url, sizeof(gcp->complete_invite_url));
    result |= gcp_reply_value(gcp, "oauth_scope",
                    gcp->oath_scope, sizeof(gcp->oath_scope));
    result |= gcp_reply_value(gcp, "invite_page_url",
                    gcp->invite_page_url, sizeof(gcp->invite_page_url));
    result |= gcp_reply_value(gcp, "registration_token",
                    gcp->registration_token, sizeof(gcp->registration_token));
    result |= gcp_reply_value(gcp, "automated_invite_url",
                    gcp->automated_invite_url, sizeof(gcp->automated_invite_url));
    result |= gcp_reply_value(gcp, "polling_url",
                    gcp->polling_url, sizeof(gcp->polling_url));
    result |= gcp_reply_value(gcp, "token_duration",
                    value, sizeof(value));
    gcp->token_duration = strtoul(value, NULL, 10);

    return result;
}

int gcp_poll_for_authorization(gcp_context_t *gcp)
{
    int result;

    if (! gcp)
    {
        return -1;
    }
    if (gcp->state != gcpPollForAuthorization)
    {
        BERROR("wrong state");
        return -1;
    }
    butil_log(5, "Poll for Authorization Code\n");

    // mime encode the parameters
    //
    result = gcp_encode_parameters(
                gcp,
                "printerid", butil_mime_text, gcp->printer_id,
                "oauth_client_id", butil_mime_text, GCP_CLIENT_ID,
                "sentinal", butil_mime_text, NULL
            );
    if (result)
    {
        return result;
    }
    result = gcp_request(gcp, GCP_CLOUD_URL, "getauthcode");

    return result;
}

int gcp_authorization_reply(gcp_context_t *gcp, bool *done)
{
    bool success;
    int result;

    // check reply status
    //
    *done = false;

    result = gcp_reply_status(gcp, &success);
    if (result)
    {
        return result;
    }
    if (! success)
    {
        return 0;
    }
    *done = true;

    // get things we need out of reply
    //
    result = 0;

    result |= gcp_reply_value(gcp, "user_email",
                    gcp->user_email, sizeof(gcp->user_email));
    result |= gcp_reply_value(gcp, "authorization_code",
                    gcp->authorization_code, sizeof(gcp->authorization_code));
    result |= gcp_reply_value(gcp, "xmpp_jid",
                    gcp->xmpp_jid, sizeof(gcp->xmpp_jid));
    result |= gcp_reply_value(gcp, "confirmation_page_url",
                    gcp->confirmation_page_url, sizeof(gcp->confirmation_page_url));
    return result;
}

int gcp_get_oauth2_token(gcp_context_t *gcp)
{
    int result;

    if (! gcp)
    {
        return -1;
    }
    if (gcp->state != gcpGetOAuth2Token)
    {
        BERROR("wrong state");
        return -1;
    }
    butil_log(5, "Get OAuth2 Token\n");

    // mime encode the parameters
    //
    result = gcp_encode_parameters(
                gcp,
                "client_id", butil_mime_text, GCP_CLIENT_ID,
                "redirect_uri", butil_mime_text, "oob",
                "client_secret", butil_mime_text, GCP_CLIENT_SECRET,
                "grant_type", butil_mime_text, "authorization_code",
//                "code", butil_mime_xwwwformurl, gcp->authorization_code,
                "code", butil_mime_text, gcp->authorization_code,
                "sentinal", butil_mime_text, NULL
            );
    if (result)
    {
        return result;
    }
    result = gcp_request(gcp, GCP_OAUTH2_URL, "o/oauth2/token");

    return result;
}

int gcp_oauth2_token_reply(gcp_context_t *gcp)
{
    int result;

    // check reply status
    //
    // get things we need out of reply
    //
    result = 0;

    result |= gcp_reply_value(gcp, "refresh_token",
                    gcp->refresh_token, sizeof(gcp->refresh_token));
    result |= gcp_reply_value(gcp, "access_token",
                    gcp->access_token, sizeof(gcp->access_token));
    return result;
}

int gcp_get_access_token(gcp_context_t *gcp)
{
    int result;

    if (! gcp)
    {
        return -1;
    }
    if (gcp->state != gcpGetAccessToken)
    {
        BERROR("wrong state");
        return -1;
    }
    butil_log(5, "Get OAuth2 Token\n");

    // mime encode the parameters
    //
    result = gcp_encode_parameters(
                gcp,
                "client_id", butil_mime_text, GCP_CLIENT_ID,
                "redirect_uri", butil_mime_text, "oob",
                "client_secret", butil_mime_text, GCP_CLIENT_SECRET,
                "grant_type", butil_mime_text, "refresh_token",
                "refresh_token", butil_mime_text, gcp->refresh_token,
                "sentinal", butil_mime_text, NULL
            );
    if (result)
    {
        return result;
    }
    result = gcp_request(gcp, GCP_OAUTH2_URL, "o/oauth2/token");

    return result;
}

int gcp_access_token_reply(gcp_context_t *gcp)
{
    int result;

    // get things we need out of reply
    //
    result = 0;

    result |= gcp_reply_value(gcp, "access_token",
                    gcp->access_token, sizeof(gcp->access_token));
    return result;
}

int gcp_list_printer(gcp_context_t *gcp)
{
    int result;

    if (! gcp)
    {
        return -1;
    }
    if (gcp->state != gcpListPrinter)
    {
        BERROR("wrong state");
        return -1;
    }
    butil_log(5, "List Printer\n");

    // mime encode the parameters
    //
    result = gcp_encode_parameters(
                gcp,
                "proxy", butil_mime_text, gcp->proxy_id,
                "sentinal", butil_mime_text, NULL
            );
    if (result)
    {
        return result;
    }
    result = gcp_request(gcp, GCP_CLOUD_URL, "list");

    return result;
}

int gcp_list_printer_reply(gcp_context_t *gcp)
{
    bool success;
    int result;

    // check reply status
    //
    result = gcp_reply_status(gcp, &success);
    if (result)
    {
        return result;
    }
    if (! success)
    {
        return -1;
    }
    // get things we need out of reply
    //
    result = 0;
    return result;
}

int gcp_fetch(gcp_context_t *gcp)
{
    int result;

    if (! gcp)
    {
        return -1;
    }
    if (gcp->state != gcpFetch)
    {
        BERROR("wrong state");
        return -1;
    }
    butil_log(5, "Fetch Jobs\n");

    // mime encode the parameters
    //
    result = gcp_encode_parameters(
                gcp,
                "printerid", butil_mime_text, gcp->printer_id,
                "sentinal", butil_mime_text, NULL
            );
    if (result)
    {
        return result;
    }
    result = gcp_request(gcp, GCP_CLOUD_URL, "fetch");

    return result;
}

int gcp_fetch_reply(gcp_context_t *gcp)
{
    char text[GCP_SHORT_TOKEN];
    char url[HTTP_MAX_URL];
    bool success;
    int result;

    // check reply status
    //
    result = gcp_reply_status(gcp, &success);
    if (result)
    {
        return result;
    }
    if (! success)
    {
        butil_log(4, "No Jobs for printer %s\n", gcp->printer_id);
        return 0;
    }
    result = gcp_reply_value(gcp, "jobs[0].id", text, sizeof(text));
    if (! result)
    {
        butil_log(4, "JOB ID   =%s\n", text);
        result = gcp_reply_value(gcp, "jobs[0].title", text, sizeof(text));
        if (! result)
        {
            butil_log(4, "    Title =%s\n", text);
        }
        result = gcp_reply_value(gcp, "jobs[0].title", text, sizeof(text));
        if (! result)
        {
            butil_log(4, "    Status=%s\n", text);
        }
        result = gcp_reply_value(gcp, "jobs[0].rasterUrl", url, sizeof(url));
        if (! result)
        {
            butil_log(4, "       url=%s\n", url);
        }
    }
    else
    {
        butil_log(4, "No Jobs\n");
    }
    return 0;
}

int gcp_prompt_for_claim(gcp_context_t *gcp)
{
    butil_log(0, "\nYou can register your printer by going to:\n"
                 "\n"
                 "%s\n"
                 "\n"
                 "Your registraion token is: %s\n\n"
                 "Press \'C\' to continue or any other letter to cancel\n\n"
                 " > ",
                 gcp->complete_invite_url,
                 gcp->registration_token
             );
    return 0;
}

int gcp_wait_for_claim(gcp_context_t *gcp, bool *done, bool *cancel)
{
    int ic;
#if 1
    ic = iostream_posix_poll_filedesc(fileno(stdin), readable, 0, 10000);
    if (ic > 0)
    {
        int rc;
        char ib;

        rc = read(fileno(stdin), &ib, 1);
        ic = (int)(uint32_t)ib;
    }
    else
    {
        *done = false;
        *cancel = false;
        return 0;
    }
#else
    ic = getchar();
#endif

    *done = true;
    if (ic == 'c' || ic == 'C')
    {
        *cancel = false;
    }
    else
    {
        *cancel = true;
    }
    return 0;
}

const char *gcp_state_string(gcp_state_t state)
{
    switch (state)
    {
    case gcpInit:
        return "Init";
    case gcpAnonRegister:
        return "Registering(Anon)";
    case gcpAnonRegisterReply:
        return "Parse Register(Anon) Reply";
    case gcpPromptForClaim:
        return "Prompt For Claim";
    case gcpWaitForClaim:
        return "Waiting For Claim-continue";
    case gcpGetAuthCode:
        return "Get Authorization Code";
    case gcpPollForAuthorization:
        return "Polling For Authorization Code";
    case gcpAuthorizationReply:
        return "Checking Authorization Code";
    case gcpGetOAuth2Token:
        return "Get OAuth2 Token";
    case gcpOAuth2TokenReply:
        return "Parse OAuth2 Token";
    case gcpGetAccessToken:
        return "Get Access Token";
    case gcpAccessTokenReply:
        return "Parse Access Token";
    case gcpListPrinter:
        return "List Printer";
    case gcpListPrinterReply:
        return "List Printer";
    case gcpFetch:
        return "Fetch Jobs";
    case gcpFetchReply:
        return "Parsing Fetched Job List";
    case gcpPollWait:
        return "Poll Time-wait";
    case gcpGetReply:
        return "Getting Reply from cloud";
    case gcpError:
        return "Error";
    case gcpFatal:
        return "Fatal";
    default:
        return "????";
    }
}

int gcp_slice(gcp_context_t *gcp)
{
    time_t now;
    bool done;
    bool cancel;
    int result;

    if (! gcp)
    {
        return -1;
    }
    if (gcp->state != gcp->prevstate)
    {
        butil_log(5, "State: %s\n", gcp_state_string(gcp->state));
        gcp->prevstate = gcp->state;
    }
    time(&now);

    switch (gcp->state)
    {
    case gcpInit:
        if (gcp->authorization_code[0])
        {
            if (gcp->refresh_token[0])
            {
                // there is an existing auth code, and refresh
                // token, so just need to get access token
                //
                gcp->state = gcpGetAccessToken;
            }
            else
            {
                // authorized (registered) but no OAuth2 token
                //
                gcp->state = gcpGetOAuth2Token;
            }
        }
        else
        {
            // no auth-code restart registration
            gcp->state = gcpAnonRegister;
        }
        break;

    case gcpAnonRegister:
        result = gcp_anon_register(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->nextstate = gcpAnonRegisterReply;
        gcp->state  = gcpGetReply;
        break;

    case gcpAnonRegisterReply:
        result = gcp_anon_register_reply(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->state = gcpPromptForClaim;
        break;

    case gcpPromptForClaim:
        result = gcp_prompt_for_claim(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->state = gcpWaitForClaim;
        break;

    case gcpWaitForClaim:
        result = gcp_wait_for_claim(gcp, &done, &cancel);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        if (! done)
        {
            return 0;
        }
        if (cancel)
        {
            // user wishes to NOT claim printer
            return 1;
        }
        gcp->state = gcpPollForAuthorization;
        break;

    case gcpPollForAuthorization:
        result = gcp_poll_for_authorization(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->nextstate = gcpAuthorizationReply;
        gcp->state = gcpGetReply;
        break;

    case gcpAuthorizationReply:
        result = gcp_authorization_reply(gcp, &done);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        if (done)
        {
            // snapshot state
            (void)gcp_nv_write(gcp);

            gcp->state = gcpGetOAuth2Token;
            break;
        }
        time(&gcp->next_poll);
        gcp->next_poll += GCP_AUTHCODE_POLL_PERIOD;
        gcp->nextstate = gcpPollForAuthorization;
        gcp->state = gcpPollWait;
        break;

    case gcpGetOAuth2Token:
        result = gcp_get_oauth2_token(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->nextstate = gcpOAuth2TokenReply;
        gcp->state = gcpGetReply;
        break;

    case gcpOAuth2TokenReply:
        result = gcp_oauth2_token_reply(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        // snapshot state
        (void)gcp_nv_write(gcp);

        // bypass GetAccessToken, got one already with refresh token
        gcp->state = gcpListPrinter;
        break;

    case gcpGetAccessToken:
        result = gcp_get_access_token(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->nextstate = gcpAccessTokenReply;
        gcp->state = gcpGetReply;
        break;

    case gcpAccessTokenReply:
        result = gcp_access_token_reply(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->state = gcpListPrinter;
        break;

    case gcpListPrinter:
        result = gcp_list_printer(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->nextstate = gcpListPrinterReply;
        gcp->state = gcpGetReply;
        break;

    case gcpListPrinterReply:
        result = gcp_list_printer_reply(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->state = gcpFetch;
        break;

    case gcpFetch:
        result = gcp_fetch(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->nextstate = gcpFetchReply;
        gcp->state = gcpGetReply;
        break;

    case gcpFetchReply:
        result = gcp_fetch_reply(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->next_poll = now + GCP_JOBFETCH_POLL_PERIOD;
        gcp->nextstate = gcpFetch;
        gcp->state = gcpPollWait;
        break;

    case gcpPollWait:
        if (now >= gcp->next_poll)
        {
            gcp->state = gcp->nextstate;
        }
        if (gcp->bxp)
        {
            result = gcp_xmpp_slice(gcp);
            if (result)
            {
                butil_log(1, "XMPP failed\n");
            }
        }
#if GCP_SUPPORT_LOCAL_PRT
        if (gcp->responding)
        {
            result = gcp_local_prt_slice(gcp);
            if (result)
            {
                butil_log(1, "MDSNR failed\n");
            }
        }
        else
#endif
        {
            sleep(1);
        }
        break;

    case gcpGetReply:
        result = gcp_check_on_reply(gcp, &done);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        if (done)
        {
            gcp->state = gcp->nextstate;
        }
        break;

    case gcpError:
    case gcpFatal:
        return -1;

    default:
        gcp->state = gcpFatal;
        break;
    }
    return 0;
}

int gcp_init(
                gcp_context_t *gcp,
                const char *serial_no,
                const char *fw_revision
            )
{
    int result;

    if (! gcp)
    {
        return -1;
    }
    memset(gcp, 0, sizeof(gcp_context_t));

    gcp->io.data = gcp->io_data;
    gcp->io.size = sizeof(gcp->io_data);

    result = iostream_tls_prolog();
    if (result)
    {
        BERROR("Can't init TLS");
        return -1;
    }
    result = gcp_nv_init(gcp);
    if (result)
    {
        BERROR("Can't init NVMEM");
        return -1;
    }
    result = gcp_xmpp_init(gcp);
    if (result)
    {
        BERROR("Can't init XMPP");
        return -1;
    }
    result = gcp_format_cdd(gcp);
    if (result)
    {
        BERROR("Can't init CDD");
        return -1;
    }
    result = gcp_format_cds(gcp);
    if (result)
    {
        BERROR("Can't init CDS");
        return -1;
    }
    strncpy(gcp->proxy_id, GCP_PROXY_ID, sizeof(gcp->proxy_id) - 1);
    gcp->proxy_id[sizeof(gcp->proxy_id) - 1] = '\0';

    strncpy(gcp->uuid, GCP_PRT_UUID, sizeof(gcp->uuid) - 1);
    gcp->uuid[sizeof(gcp->uuid) - 1] = '\0';

    strncpy(gcp->serial_no, serial_no, sizeof(gcp->serial_no) - 1);
    gcp->serial_no[sizeof(gcp->serial_no) - 1] = '\0';

    strncpy(gcp->fw_revision, fw_revision, sizeof(gcp->fw_revision) - 1);
    gcp->fw_revision[sizeof(gcp->fw_revision) - 1] = '\0';

    http_generate_boundary(gcp->boundary, sizeof(gcp->boundary));

    gcp->http_client = http_client_create(NULL, true);
    if (! gcp->http_client)
    {
        BERROR("http client");
        return -1;
    }
    // create a resource for http client use to hook the post callback func in
    //
    gcp->http_client_resources = NULL;
    result = http_add_func_resource(&gcp->http_client_resources, schemeHTTPS,  "/", NULL, gcp_resource_func, gcp);
    if (result)
    {
        BERROR("add resource");
        return result;
    }
    #if GCP_SUPPORT_LOCAL_PRT
    result = gcp_init_local_prt(gcp);
    if (result)
    {
        BERROR("add local printing");
        return result;
    }
    #endif
    time(&gcp->start_time);

    gcp->prevstate = gcpAnonRegister;
    gcp->nextstate = gcpAnonRegister;
    gcp->state = gcpInit;

    return 0;
}

int gcp_deinit(gcp_context_t *gcp)
{
    if (! gcp)
    {
        return -1;
    }
    gcp_xmpp_deinit(gcp);
    gcp_nv_deinit(gcp);
    return 0;
}

