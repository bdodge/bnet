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

int main(int argc, char **argv)
{
    const char *program;
    uint16_t port;
    bsmtp_transport_t transport;
    char url[BSMTP_MAX_PATH];
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
    snprintf(url, sizeof(url), "//smtp.outlook.com:%u", port);
//    snprintf(url, sizeof(url), "//smtp.gmail.com:%u", port);

    result = bsmtp_send_mail(
            url,
            transport,
            "bnet_tester_2222@outlook.com",
            "bnet_test_1111@outlook.com",
            "youwantspam1111",
//            "bnet_test_1111@outlook.com",
//            "bnet_tester_2222@outlook.com",
//            "youwantspam2222",
            "Hello",
            "This is a test",
            0
            );
    if (! result)
    {
        butil_log(0, "Send call failed\n");
        return result;
    }
    return result;
}

