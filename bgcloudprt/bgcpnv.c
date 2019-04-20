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
#include "bgcpnv.h"
#include "bgcp.h"
#include "bgcpio.h"

#define GCP_NV_FILENAME "bgcpnv.json"


int gcp_nv_read(gcp_context_t *gcp)
{
    iostream_t *nvs;
    int result;

    if (! gcp)
    {
        return -1;
    }
    nvs = iostream_create_reader_from_file(GCP_NV_FILENAME);
    if (! nvs)
    {
        butil_log(0, "Can't open NV file %s\n", GCP_NV_FILENAME);
        return -1;
    }
    gcp->io.tail = 0;

    // read backing file into io buffer
    result = nvs->read(nvs, gcp->io.data, gcp->io.size - 1);

    nvs->close(nvs);

    if (result < 0)
    {
        butil_log(0, "Can't read NV file\n");
        return -1;
    }
    // stringify the result
    gcp->io.count = result;
    gcp->io.head  = result;
    gcp->io.data[result] = '\0';

    // parse out json into our context.
    // note the helper routines that are used to parse json from the cloud
    // are re-used here since the nv backing file is now in the context's io buffer
    // just like as if it came from the cloud
    // note also that some of these can have no value, but should all have keys
    //
    result = 0;
    result |= gcp_reply_value(gcp, "printer_id",
                    gcp->printer_id, sizeof(gcp->printer_id));
    result |= gcp_reply_value(gcp, "registration_token",
                    gcp->registration_token, sizeof(gcp->registration_token));
    result |= gcp_reply_value(gcp, "user_email",
                    gcp->user_email, sizeof(gcp->user_email));
    result |= gcp_reply_value(gcp, "oauth_scope",
                    gcp->oath_scope, sizeof(gcp->oath_scope));
    result |= gcp_reply_value(gcp, "authorization_code",
                    gcp->authorization_code, sizeof(gcp->authorization_code));
    result |= gcp_reply_value(gcp, "xmpp_jid",
                    gcp->xmpp_jid, sizeof(gcp->xmpp_jid));
    result |= gcp_reply_value(gcp, "refresh_token",
                    gcp->refresh_token, sizeof(gcp->refresh_token));
    return result;
}

int gcp_nv_write(gcp_context_t *gcp)
{
    iostream_t *nvs;
    int result;

    if (! gcp)
    {
        return -1;
    }
    gcp->io.tail = 0;
    gcp->io.count = 0;
    gcp->io.head  = 0;

    result = snprintf(
                        (char*)gcp->io.data + gcp->io.tail,
                        gcp->io.size - gcp->io.count,
                        "{\n"
                        "  \"printer\":\n"
                        "    {\n"
                        "      \"printer_id\": \"%s\",\n"
                        "      \"registration_token\": \"%s\",\n"
                        "      \"user_email\": \"%s\",\n"
                        "      \"oauth_scope\": \"%s\",\n"
                        "      \"authorization_code\": \"%s\",\n"
                        "      \"xmpp_jid\": \"%s\",\n"
                        "      \"refresh_token\": \"%s\"\n"
                        "    }\n"
                        "}\n",
                        gcp->printer_id,
                        gcp->registration_token,
                        gcp->user_email,
                        gcp->oath_scope,
                        gcp->authorization_code,
                        gcp->xmpp_jid,
                        gcp->refresh_token
                    );
    if (result < 0)
    {
        butil_log(0, "Can't format NV\n");
        return -1;
    }
    gcp->io.count = result;
    gcp->io.head = result;

    nvs = iostream_create_writer_from_file(GCP_NV_FILENAME);
    if (! nvs)
    {
        butil_log(0, "Can't open NV file %s\n", GCP_NV_FILENAME);
        return -1;
    }
    result = nvs->write(nvs, gcp->io.data, gcp->io.count);

    nvs->close(nvs);

    if (result < 0)
    {
        butil_log(0, "Can't write NV file\n");
        return -1;
    }
    return 0;
}

int gcp_nv_init(gcp_context_t *gcp)
{
    iostream_t *nvs;
    int result;

    if (! gcp)
    {
        return -1;
    }
    nvs = iostream_create_reader_from_file("bgcpnv.json");
    if (! nvs)
    {
        // first time file? write, then re-read
        //
        butil_log(5, "Can't open NV file %s\n", GCP_NV_FILENAME);

        result = gcp_nv_write(gcp);
        if (result)
        {
            return result;
        }
        result = gcp_nv_read(gcp);
        if (result)
        {
            return result;
        }
    }
    else
    {
        result = gcp_nv_read(gcp);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int gcp_nv_deinit(gcp_context_t *gcp)
{
    return 0;
}

