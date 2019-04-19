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
#include "bgcpio.h"
#include "bgcp.h"

int gcp_reply_status(gcp_context_t *gcp, bool *success)
{
    bjson_parser_t *pjx;
    const char *pvalue;
    char value[128];
    int result;

    *success = false;

    pjx = bjson_parser_create((char*)gcp->io.data);
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
                                            (char*)gcp->io.data,
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

int gcp_resource_func(
                        http_client_t       *client,
                        http_resource_t     *resource,
                        http_callback_type_t cbtype,
                        uint8_t            **data,
                        size_t              *count
                     )
{
    gcp_context_t *gcp = (gcp_context_t *)resource->priv;
    int chunk;
    int result;

    switch (cbtype)
    {
    case httpRequest:
        switch (client->method)
        {
        case httpPost:
            http_log(7, "post %d\n", gcp->io.count);
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
            break;

        default:
            HTTP_ERROR("Bad method for gcp data");
            return -1;
        }
        break;

    case httpDownloadData:
        butil_log(5, "Download %d\n", *count);
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
        return 0;

    case httpDownloadDone:
        return 0;

    case httpUploadData:
        chunk = *count;
        if (chunk > gcp->io.count)
        {
            chunk = gcp->io.count;
        }
        http_log(3, "move %u of %u\n", chunk, gcp->io.count);
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

int gcp_encode_parameters(gcp_context_t *gcp, ...)
{
    va_list args;
    char *name;
    mime_content_type_t type;
    char *value;
    int len;
    char content_type[64];
    int result;

    if (! gcp)
    {
        return -1;

    }
    gcp->io.head = 0;
    gcp->io.tail = 0;
    gcp->io.count = 0;

    va_start(args, gcp);

    while (1)
    {
        name = (char *)va_arg(args, char *);
        if (! name)
        {
            break;
        }
        type = (mime_content_type_t)va_arg(args, mime_content_type_t);
        if (! type)
        {
            type = butil_mime_text;
        }
        value = (char *)va_arg(args, char *);
        if (! value)
        {
            break;
        }
        if (type != butil_mime_text)
        {
            snprintf(content_type, sizeof(content_type),
                        "Content-Type: %s\r\n", butil_mime_string_for_content_type(type));
        }
        else
        {
            content_type[0] = '\0';
        }
        len = snprintf(
                        (char*)gcp->io.data + gcp->io.head,
                        gcp->io.size - gcp->io.count,
                        "--%s\r\nContent-Disposition: form-data; name=\"%s\"\r\n"
                        "%s"
                        "\r\n%s\r\n",
                        gcp->boundary,
                        name,
                        content_type,
                        value
                      );
        if (len < 0)
        {
            BERROR("parms overflow");
            va_end(args);
            return -1;
        }
        gcp->io.head += len;
        gcp->io.count += len;
    }
    va_end(args);

    len = snprintf(
                    (char*)gcp->io.data + gcp->io.head,
                    gcp->io.size - gcp->io.count,
                    "--%s--\r\n",
                    gcp->boundary
                  );
    if (len < 0 || gcp->io.count == gcp->io.size)
    {
        BERROR("parms overflow");
        return -1;
    }
    gcp->io.head += len;
    gcp->io.count += len;
    return 0;
}

int gcp_request(gcp_context_t *gcp, const char *url, const char *path)
{
    char fullurl[HTTP_MAX_URL];
    http_method_t method;
    uint16_t port;
    int to_secs, to_usecs;
    int redirects;
    int result;

    method = httpPost;

    snprintf(fullurl, sizeof(fullurl), "%s/%s", url, path);

    butil_log(3, "%s %s\n", http_method_name(method), fullurl);

    http_client_reinit(gcp->http_client, true);
    gcp->http_client->keepalive = false;
    redirects = 0;

    result = http_client_request(gcp->http_client, method, fullurl, httpTCP, false, NULL, gcp->http_resources);
    if (result)
    {
        butil_log(1, "Failed to request %s\n", path);
        return result;
    }
    return result;
}

int gcp_check_on_reply(gcp_context_t *gcp, bool *done)
{
    int result;

    *done = false;
    result = http_client_slice(gcp->http_client);
    if (result)
    {
        butil_log(5, "HTTP request error\n");
        return result;
    }
    if (gcp->http_client->state == httpDone)
    {
        butil_log(5, "HTTP request complete\n");
        butil_log(5, "Reply:\n%s\n", gcp->io.data);

        *done = true;
        if (gcp->http_client->response > 400)
        {
            butil_log(1, "HTTP Response: %d\n", gcp->http_client->response);
            return -1;
        }
        return 0;
    }
    result = http_wait_for_client_event(gcp->http_client, 0, 50000);
    if (result < 0)
    {
        butil_log(1, "HTTP Response Disconnect\n");
        return result;
    }
    if (result > 0)
    {
        result = 0;
    }
    return result;
}


