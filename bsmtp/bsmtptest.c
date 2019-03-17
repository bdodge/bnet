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
#include "bsmtp.h"

#define USE_GOOGLE 1

#define MAX_EMAIL_ATTACHMENTS 4

typedef struct tag_email_context
{
    const char *body;
    size_t     cur_attachment;
    size_t     num_attachments;
    const char *attachments[MAX_EMAIL_ATTACHMENTS];
}
email_context_t;

int body_callback(bsmtp_callback_type_t type, void *priv, const char **body, size_t *body_count)
{
    email_context_t *ctx = (email_context_t *)priv;

    if (! ctx)
    {
        return -1;
    }
    switch (type)
    {
    case bsmtpGetBody:
        if (ctx->body)
        {
            *body = ctx->body;
            *body_count = strlen(ctx->body);
            ctx->body = NULL;
        }
        break;

    case bsmtpGetAttachment:
        if (ctx->cur_attachment < ctx->num_attachments)
        {
            *body = ctx->attachments[ctx->cur_attachment];
            *body_count = 0;
            ctx->cur_attachment++;
        }
        else
        {
            *body = NULL;
            *body_count = 0;
        }
        break;
    default:
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    const char *program;
    uint16_t port;
    bsmtp_transport_t transport;
    char url[BSMTP_MAX_PATH];
    email_context_t email_context;
    int result;
#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        BERROR("WSAStartup failed");
        return -1;
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    butil_set_log_level(5);

    program = *argv++;
    argc--;

#if BSMTP_SUPPORT_TLS
    result = iostream_tls_prolog();
    if (result)
    {
        BERROR("Can't init TLS");
        return -1;
    }
    transport = bsmtpTLS;
#else
    transport = bsmtpPlainText;
#endif

    switch (transport)
    {
    default:
    case bsmtpPlainText:
        port = 25;
        break;
    case bsmtpSSL:
        port = 465;
        break;
    case bsmtpTLS:
        port = 587;
        break;
    }
#if USE_GOOGLE
    snprintf(url, sizeof(url), "//smtp.gmail.com:%u", port);
#else
    snprintf(url, sizeof(url), "//smtp.outlook.com:%u", port);
#endif

    email_context.body = "Callback provided body\r\n";
    email_context.cur_attachment = 0;
    email_context.num_attachments = 0;
    email_context.attachments[email_context.num_attachments++] = "test_attachment.txt";
    email_context.attachments[email_context.num_attachments++] = "test_attachment.jpg";

    result = bsmtp_send_mail(
            url,
            transport,
            "bnet_tester_2222@outlook.com",
#if USE_GOOGLE
            "bnettester1111@gmail.com",
#else
            "bnet_test_1111@outlook.com",
#endif
            "youwantspam1111",
//            "bnet_test_1111@outlook.com",
//            "bnet_tester_2222@outlook.com",
//            "youwantspam2222",
            "Hello",
            "<html><h2>Html Test Email</h2><h3>This is a test</h3>\n",
            body_callback,
            &email_context
            );
    if (result)
    {
        butil_log(0, "Send call failed\n");
        return result;
    }
    return result;
}

