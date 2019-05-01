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
#include "bgcpprivet.h"
#include "bgcplocalprt.h"
#include "bgcp.h"

#if GCP_SUPPORT_LOCAL_PRT

static int gcp_local_info(gcp_context_t *gcp)
{
    time_t now;
    int len;
    char apitxt[256];

    if (! gcp)
    {
        return -1;
    }
    time(&now);

    if (gcp_is_registered(gcp))
    {
        if (gcp->local_prt_enabled)
        {
            snprintf(apitxt, sizeof(apitxt),
                "    \"/privet/info\",\r\n"
                "    \"/privet/accesstoken\",\r\n"
                "    \"/privet/capabilities\",\r\n"
                "    \"/privet/printer/submitdoc\",\r\n"
                "    \"/privet/printer/createjob\",\r\n"
                "    \"/privet/printer/jobstate\"\r\n"
                );
        }
        else
        {
            snprintf(apitxt, sizeof(apitxt),
                "    \"/privet/info\",\r\n"
                "    \"/privet/accesstoken\",\r\n"
                "    \"/privet/capabilities\"\r\n"
                );
        }
    }
    else
    {
        snprintf(apitxt, sizeof(apitxt),
            "    \"/privet/info\",\r\n"
            "    \"/privet/register\"\r\n"
            );
    }
    len = snprintf(
        (char*)gcp->io.data,
        gcp->io.size,
        "{\r\n"
        "\"version\": \"1.0\",\r\n"
        "\"name\": \"%s\",\r\n"                 /* printer description */
        "\"description\": \"Printer connected through Chrome connector\",\r\n"
        "\"url\": \"https://www.google.com/cloudprint\",\r\n"
        "\"type\": [\r\n"
        "        \"printer\"\r\n"
        "],\r\n"
        "\"id\": \"%s\",\r\n"                   /* printer id */
        "\"device_state\": \"%s\",\r\n"         /* device state */
        "\"connection_state\": \"%s\",\r\n"     /* connection state */
        "\"manufacturer\": \"BNET\",\r\n"
        "\"model\": \"IPP\",\r\n"
        "\"serial_number\": \"%s\",\r\n"        /* serial number */
        "\"firmware\": \"%s\",\r\n"             /* f/w revision */
        "\"uptime\": %ld,\r\n"                  /* up-time */
        "\"setup_url\": \"http://support.google.com/cloudprint/answer/1686197/?hl=en\",\r\n"
        "\"support_url\": \"http://support.google.com/cloudprint/?hl=en\",\r\n"
        "\"update_url\": \"http://support.google.com/cloudprint/?hl=en\",\r\n"
        "\"x-privet-token\": \"%s\",\r\n"       /* local access token */
        "\"api\": [\r\n"
        "%s"                                    /* API */
        "]\r\n"
        "}\r\n",
        GCP_PRT_MODEL,
        gcp->printer_id,
        "idle",
        (gcp->xmpp_state == gcpxConnected) ? "online" : "offline",
        gcp->serial_no,
        gcp->fw_revision,
        now - gcp->start_time,
        gcp->local_access_token,
        apitxt
    );
    if (len < 0)
    {
        butil_log(0, "result buffer overflow\n");
        return -1;
    }
    gcp->io.tail = 0;
    gcp->io.count = len;
    gcp->io.head = len;
    return 0;
}

static int gcp_local_capabilities(gcp_context_t *gcp)
{
    int len;

    len = snprintf(
                (char*)gcp->io.data,
                gcp->io.size,
                "%s",
                gcp->cdd
                );
    if (len < 0)
    {
        butil_log(0, "result buffer overflow\n");
        return -1;
    }
    gcp->io.tail = 0;
    gcp->io.count = len;
    gcp->io.head = len;
    return 0;
}

static int gcp_local_prt_resource_func(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    gcp_context_t *gcp = (gcp_context_t *)resource->priv;
    char *header;
    int chunk;
    int result;

    switch (cbtype)
    {
    case httpRequestHeader:
        header = (char *)*data;
        if (header)
        {
            char *value;

            value = strchr(header, ':');
            if (value)
            {
                while (*value == ':' || *value == ' ')
                {
                    value++;
                }
            }
            if (! http_ncasecmp(header, "x-privet-token:"))
            {
                int len;

                if (value[0] == '\"')
                {
                    value++;
                    len = strlen(value);
                    value[len - 1] = '\0';
                }
                len = snprintf(gcp->local_access_token, sizeof(gcp->local_access_token), "%s", value);
            }
        }
        break;

    case httpRequest:
        switch (client->method)
        {
        case httpGet:
            http_log(5, "Get %s\n", client->path);
            client->keepalive = false;
            if (! strcmp(client->path, "/privet/info"))
            {
                result = gcp_local_info(gcp);
                if (! result)
                {
                    client->out_content_length = gcp->io.count;
                    client->out_content_type = butil_mime_json;
                    client->out_transfer_type = httpLength;
                    result += http_append_request(client,
                            "x-privet-token: %s", gcp->local_access_token);
                }
                break;
            }
            if (! strcmp(client->path, "/privet/capabilities"))
            {
                result = gcp_local_capabilities(gcp);
                if (! result)
                {
                    client->out_content_length = gcp->io.count;
                    client->out_content_type = butil_mime_json;
                    client->out_transfer_type = httpLength;
                    result += http_append_request(client,
                            "x-privet-token: %s", gcp->local_access_token);
                }
                break;
            }
            butil_log(5, "Unsupported URL %s\n", client->path);
            result = http_error_reply(client, 404, "Not found", true);
            client->out_content_length = 0;
            break;

        case httpPost:
            http_log(7, "post %d\n", gcp->io.count);
            /*
            client->out_content_length = gcp->io.count;
            client->out_content_type = butil_mime_multi;
            strncpy(client->boundary, gcp->boundary, sizeof(client->boundary) - 1);
            client->boundary[sizeof(client->boundary) - 1] = '\0';
            // add headers needed
            result = http_append_request(client, "X-CloudPrint-Proxy: Yes");
            if (gcp->state >= gcpListPrinter)
            {
                // add auth header for "real" requests, past registration
                //
                result += http_append_request(client, "Authorization: OAuth %s",
                                                                gcp->access_token);
            }
            */
            break;

        default:
            HTTP_ERROR("Bad method for gcp data");
            return -1;
        }
        break;

    case httpDownloadData:
        butil_log(5, "Download %d\n", *count);
        /*
        iostream_normalize_ring(&gcp->io, NULL);
        chunk = gcp->io.size - gcp->io.count - 1;
        if (chunk > *count)
        {
            chunk = *count;
        }
        if (chunk == 0)
        {
            butil_log(1, "IO Overflow\n");
            // drop data
            return 0;
        }
        memcpy(gcp->io.data + gcp->io.head, *data, chunk);
        gcp->io.head += chunk;
        gcp->io.count += chunk;
        gcp->io.data[gcp->io.head] = '\0';
        */
        return 0;

    case httpDownloadDone:
        return 0;

    case httpUploadData:
        butil_log(5, "Upload %d\n", *count);
        chunk = *count;
        if (chunk > gcp->io.count)
        {
            chunk = gcp->io.count;
        }
        butil_log(3, "move %u of %u\n", chunk, gcp->io.count);
        if (chunk > 0)
        {
            if (*data)
            {
                memcpy(*data, gcp->io.data + gcp->io.tail, chunk);
            }
            else
            {
                *data = gcp->io.data + gcp->io.tail;
            }
            gcp->io.tail += chunk;
            gcp->io.count -= chunk;
        }
        if (gcp->io.count == 0)
        {
            gcp->io.head = 0;
            gcp->io.tail = 0;
            gcp->io.data[0] = '\0';
        }
        *count = chunk;
        break;;

    case httpComplete:
        break;

    default:
        return -1;
    }
    return 0;
}

int gcp_local_prt_slice(gcp_context_t *gcp)
{
    int result;

    result  = http_server_slice(&gcp->http_server, 0, 25000);
    result |= gcp_local_disco_slice(gcp);
    return result;
}

int gcp_init_local_prt(gcp_context_t *gcp)
{
    int result;

    if (! gcp)
    {
        return -1;
    }
    // create a /privet resource for access to the google api on this printer
    //
    gcp->http_server_resources = NULL;
    result = http_add_func_resource(
                                    &gcp->http_server_resources,
                                    schemeHTTP,
                                    "/privet",
                                    NULL,
                                    gcp_local_prt_resource_func,
                                    gcp
                                  );
    if (result)
    {
        BERROR("add resource");
        return result;
    }
    // http server for local chrome api access
    //
    result = http_server_init(
                                    &gcp->http_server,
                                    gcp->http_server_resources,
                                    GCP_LOCAL_HTTP_PORT,
                                    httpTCP,
                                    1,
                                    false
                                  );
    if (result)
    {
        butil_log(0, "Can't add http server on port %u\n", (uint32_t)GCP_LOCAL_HTTP_PORT);
        return result;
    }
    // local discovery
    //
    result = gcp_init_local_disco(gcp);
    if (result)
    {
        butil_log(0, "Can't add http server on port %u\n", (uint32_t)GCP_LOCAL_HTTP_PORT);
        return result;
    }
    else
    {
        gcp->local_prt_enabled = true;
    }
    return result;
}

#endif
