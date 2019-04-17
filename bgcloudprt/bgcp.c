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

int gcp_register(gcp_context_t *gcp)
{
    char verstring[8];
    int result;

    if (! gcp)
    {
        return -1;
    }
    if (gcp->state != gcpRegister)
    {
        BERROR("wrong state");
        return -1;
    }
    butil_log(5, "Registering\n");

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

int gcp_reply_status(gcp_context_t *gcp)
{
    bjson_parser_t *pjx;
    const char *pvalue;
    char value[128];
    int result;

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
            result = -1;
            break;
        }
        else if (strcmp(value, "true"))
        {
            butil_log(2, "Success not true/false\n");
            result = -1;
            break;
        }
        result = 0;
        break;
    }
    while (0); // catch

    bjson_parser_destroy(pjx);

    return result;
}

int gcp_register_reply(gcp_context_t *gcp)
{
    bjson_parser_t *pjx;
    const char *pvalue;
    char value[128];
    int result;

    // check reply status
    //
    result = gcp_reply_status(gcp);
    if (result)
    {
        return result;
    }
    // get things we need out of reply
    //
    pjx = bjson_parser_create(gcp->io.data);
    if (! pjx)
    {
        BERROR("can't create parser");
        return -1;
    }
    do // try
    {
        result = bjson_find_and_copy_json_key_value(
                                            gcp->io.data,
                                            "registration_token",
                                            '.',
                                            0,
                                            gcp->registration_token,
                                            sizeof(gcp->registration_token)
                                            );
        if (result)
        {
            break;
        }
        result = bjson_find_and_copy_json_key_value(
                                            gcp->io.data,
                                            "registration_token",
                                            '.',
                                            0,
                                            gcp->registration_token,
                                            sizeof(gcp->registration_token)
                                            );
        if (result)
        {
            break;
        }
        result = bjson_find_and_copy_json_key_value(
                                            gcp->io.data,
                                            "invite_url",
                                            '.',
                                            0,
                                            gcp->invite_url,
                                            sizeof(gcp->invite_url)
                                            );
        if (result)
        {
            break;
        }
        result = bjson_find_and_copy_json_key_value(
                                            gcp->io.data,
                                            "complete_invite_url",
                                            '.',
                                            0,
                                            gcp->complete_invite_url,
                                            sizeof(gcp->complete_invite_url)
                                            );
        if (result)
        {
            break;
        }
        result = bjson_find_and_copy_json_key_value(
                                            gcp->io.data,
                                            "invite_page_url",
                                            '.',
                                            0,
                                            gcp->invite_page_url,
                                            sizeof(gcp->invite_page_url)
                                            );
        if (result)
        {
            break;
        }
        result = bjson_find_and_copy_json_key_value(
                                            gcp->io.data,
                                            "polling_url",
                                            '.',
                                            0,
                                            gcp->polling_url,
                                            sizeof(gcp->polling_url)
                                            );
        if (result)
        {
            break;
        }
        result = bjson_find_and_copy_json_key_value(
                                            gcp->io.data,
                                            "token_duration",
                                            '.',
                                            0,
                                            value,
                                            sizeof(value)
                                            );
        if (result)
        {
            break;
        }
        gcp->token_duration = strtoul(value, NULL, 10);
    }
    while (0); // catch

    return result;
}

const char *gcp_state_string(gcp_state_t state)
{
    switch (state)
    {
    case gcpInit:
        return "Init";
    case gcpRegister:
        return "Registering";
    case gcpRegisterReply:
        return "Parse Register Reply";
    case gcpGetReply:
        return "Getting a Reply";
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
        gcp->state = gcpRegister;
        break;

    case gcpRegister:
        result = gcp_register(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
        }
        gcp->nextstate = gcpRegisterReply;
        gcp->state  = gcpGetReply;
        break;

    case gcpRegisterReply:
        result = gcp_register_reply(gcp);
        if (result)
        {
            gcp->state = gcpError;
            break;
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
    gcp->prevstate = gcpRegister;
    gcp->nextstate = gcpRegister;
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

