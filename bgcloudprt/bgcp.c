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
#include "bgcpcdd.h"

int gcp_reply_status(gcp_context_t *gcp, bool *success)
{
    bjson_parser_t *pjx;
    const char *pvalue;
    char value[128];
    int result;

    *success = false;

    pjx = bjson_parser_create(gcp->io.data);
    if (! pjx)
    {
        BERROR("can't create parser");
        return -1;
    }
    do // try
    {
        result = bjson_find_key_value(pjx, "success", '\0', 0, &pvalue);
        if (result)
        {
            butil_log(2, "No success in reply\n");
            break;
        }
        result = bjson_copy_key_value(pjx, pvalue, value, sizeof(value));
        if (result)
        {
            butil_log(2, "Can't copy result\n");
            break;
        }
        if (! strcmp(value, "false"))
        {
            result = bjson_find_key_value(pjx, "message", '\0', 0, &pvalue);
            if (! result)
            {
                result = bjson_copy_key_value(pjx, pvalue, value, sizeof(value));
                if (! result)
                {
                    butil_log(4, "Message: %s\n", value);
                }
            }
            result = 0;
            break;
        }
        else if (strcmp(value, "true"))
        {
            butil_log(2, "Success not true/false\n");
            result = -1;
            break;
        }
        *success = true;
        result = 0;
        break;
    }
    while (0); // catch

    bjson_parser_destroy(pjx);

    return result;
}

int gcp_reply_value(gcp_context_t *gcp, const char *key, char *value, size_t nvalue)
{
    int result;

    result = bjson_find_and_copy_json_key_value(
                                            gcp->io.data,
                                            key,
                                            '.',
                                            0,
                                            value,
                                            nvalue
                                            );
    // dequote string values
    if (! result && value[0] == '\"')
    {
        size_t len = strlen(value);

        if (len > 2)
        {
            memmove(value, value + 1, len - 2);
            value[len - 2] = '\0';
        }
        else
        {
            value[0] = '\0';
        }
    }
    return result;
}

int gcp_anon_register(gcp_context_t *gcp)
{
    char verstring[8];
    int result;

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
                "sentinal", butil_mime_text, NULL
            );
    if (result)
    {
        return result;
    }
    result = gcp_request(gcp, "register");

    return result;
}

int gcp_anon_register_reply(gcp_context_t *gcp)
{
    bjson_parser_t *pjx;
    const char *pvalue;
    char value[128];
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
    pjx = bjson_parser_create(gcp->io.data);
    if (! pjx)
    {
        BERROR("can't create parser");
        return -1;
    }
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
    result = gcp_request(gcp, "getauthcode");

    return result;
}

int gcp_authorization_reply(gcp_context_t *gcp, bool *done)
{
    bjson_parser_t *pjx;
    const char *pvalue;
    char value[128];
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

    result |= gcp_reply_value(gcp, "authorization_code",
                    gcp->authorization_code, sizeof(gcp->authorization_code));
    result |= gcp_reply_value(gcp, "xmpp_jid",
                    gcp->xmpp_jid, sizeof(gcp->xmpp_jid));
    result |= gcp_reply_value(gcp, "confirmation_page_url",
                    gcp->confirmation_page_url, sizeof(gcp->confirmation_page_url));
    return result;
}

int gcp_prompt_for_claim(gcp_context_t *gcp)
{
    butil_log(0, "\nYou can register your printer by going to:\n"
                 "\n"
                 "%s\n"
                 "\n"
                 "Your registraion token is: %s\n\n"
                 "Press \'C\' to cancel or any other letter to procede, and then Enter\n\n"
                 " > ",
                 gcp->complete_invite_url,
                 gcp->registration_token
             );
    return 0;
}

int gcp_wait_for_claim(gcp_context_t *gcp, bool *cancel)
{
    int ic;

    ic = getchar();

    if (ic == 'c' || ic == 'C')
    {
        *cancel = true;
    }
    else
    {
        *cancel = false;
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
    case gcpGetAccessToken:
        return "Get Access Token";
    case gcpListPrinter:
        return "List Printer";
    case gcpFetch:
        return "Fetching Jobs";
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
    switch (gcp->state)
    {
    case gcpInit:
        gcp->state = gcpAnonRegister;
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
        result = gcp_wait_for_claim(gcp, &done);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        if (done)
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
            gcp->state = gcpGetAccessToken;
            break;
        }
        time(&gcp->next_poll);
        gcp->next_poll += GCP_AUTHCODE_POLL_PERIOD;
        gcp->nextstate = gcpPollForAuthorization;
        gcp->state = gcpPollWait;
        break;

    case gcpGetAccessToken:
        break;

    case gcpListPrinter:
        break;

    case gcpFetch:
        break;

    case gcpPollWait:
        time(&now);
        if (now >= gcp->next_poll)
        {
            gcp->state = gcp->nextstate;
        }
        sleep(1);
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

int gcp_init(gcp_context_t *gcp, const char *proxy_id, const char *uuid)
{
    int result;

    if (! gcp)
    {
        return -1;
    }
    memset(gcp, 0, sizeof(gcp_context_t));

    result = iostream_tls_prolog();
    if (result)
    {
        BERROR("Can't init TLS");
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
    strncpy(gcp->proxy_id, proxy_id, sizeof(gcp->proxy_id) - 1);
    gcp->proxy_id[sizeof(gcp->proxy_id) - 1] = '\0';

    strncpy(gcp->uuid, uuid, sizeof(gcp->uuid) - 1);
    gcp->uuid[sizeof(gcp->uuid) - 1] = '\0';

    snprintf(gcp->boundary, sizeof(gcp->boundary), "aaaaaaaaaaaaaabbbbbbbbbbbbcccccccccc");

    gcp->io.data = gcp->io_data;
    gcp->io.size = sizeof(gcp->io_data);

    gcp->http_client = http_client_create(NULL, true);
    if (! gcp->http_client)
    {
        BERROR("http client");
        return -1;
    }
    gcp->http_resources = NULL;
    result = http_add_func_resource(&gcp->http_resources, schemeHTTPS,  "/", NULL, gcp_resource_func, gcp);
    if (result)
    {
        BERROR("add resource");
        return result;
    }
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
    return 0;
}

