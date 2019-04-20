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
#include "bgcpnv.h"

static const char *s_regrep_test =
"{\r\n"
" \"success\": true,\r\n"
" \"request\": {\r\n"
"  \"time\": \"0\",\r\n"
"  \"params\": {\r\n"
"   \"setup_url\": [\r\n"
"    \"http://www.msn.com\"\r\n"
"   ],\r\n"
"   \"uuid\": [\r\n"
"    \"68ECDD18-8319-42DD-BC73-87D192E900DD\"\r\n"
"   ],\r\n"
"   \"manufacturer\": [\r\n"
"    \"bnet\"\r\n"
"   ],\r\n"
"   \"proxy\": [\r\n"
"    \"yoproxy\"\r\n"
"   ],\r\n"
"   \"gcp_version\": [\r\n"
"    \"2.0\"\r\n"
"   ],\r\n"
"   \"support_url\": [\r\n"
"    \"http://www.cnn.com\"\r\n"
"   ],\r\n"
"   \"name\": [\r\n"
"    \"BNET\"\r\n"
"   ],\r\n"
"   \"update_url\": [\r\n"
"    \"http://www.abc.com\"\r\n"
"   ],\r\n"
"   \"use_cdd\": [\r\n"
"    \"true\"\r\n"
"   ],\r\n"
"   \"model\": [\r\n"
"    \"gcp20\"\r\n"
"   ],\r\n"
"   \"firmware\": [\r\n"
"    \"1.1\"\r\n"
"   ]\r\n"
"  }\r\n"
" },\r\n"
" \"printers\": [\r\n"
"  {\r\n"
"   \"isTosAccepted\": false,\r\n"
"   \"displayName\": \"BNET\",\r\n"
"   \"supportUrl\": \"http://www.cnn.com\",\r\n"
"   \"description\": \"\",\r\n"
"   \"type\": \"GOOGLE\",\r\n"
"   \"notificationChannel\": \"XMPP_CHANNEL\",\r\n"
"   \"uuid\": \"68ECDD18-8319-42DD-BC73-87D192E900DD\",\r\n"
"   \"manufacturer\": \"bnet\",\r\n"
"   \"gcpVersion\": \"2.0\",\r\n"
"   \"defaultDisplayName\": \"\",\r\n"
"   \"semanticState\": {\r\n"
"    \"printer\": {\r\n"
"     \"state\": \"IDLE\"\r\n"
"    },\r\n"
"    \"version\": \"1.0\"\r\n"
"   },\r\n"
"   \"model\": \"gcp20\",\r\n"
"   \"id\": \"caabfb70-f2c5-248f-efa2-5f5b98a4d07d\",\r\n"
"   \"firmware\": \"1.1\",\r\n"
"   \"setupUrl\": \"http://www.msn.com\",\r\n"
"   \"capabilities\": {\r\n"
"    \"printer\": {\r\n"
"     \"page_orientation\": {\r\n"
"      \"option\": [\r\n"
"       {\r\n"
"        \"type\": \"PORTRAIT\",\r\n"
"        \"is_default\": true\r\n"
"       },\r\n"
"       {\r\n"
"        \"type\": \"LANDSCAPE\"\r\n"
"       }\r\n"
"      ]\r\n"
"     },\r\n"
"     \"collate\": {\r\n"
"      \"default\": true\r\n"
"     },\r\n"
"     \"pwg_raster_config\": {\r\n"
"      \"document_type_supported\": [\r\n"
"       \"ADOBE_RGB_8\",\r\n"
"       \"RGB_8\",\r\n"
"       \"SGRAY_8\",\r\n"
"       \"SRGB_8\"\r\n"
"      ],\r\n"
"      \"document_resolution_supported\": [\r\n"
"       {\r\n"
"        \"cross_feed_dir\": 300,\r\n"
"        \"feed_dir\": 300\r\n"
"       },\r\n"
"       {\r\n"
"        \"cross_feed_dir\": 600,\r\n"
"        \"feed_dir\": 600\r\n"
"       }\r\n"
"      ]\r\n"
"     },\r\n"
"     \"copies\": {\r\n"
"      \"default\": 1,\r\n"
"      \"max\": 99\r\n"
"     },\r\n"
"     \"media_size\": {\r\n"
"      \"option\": [\r\n"
"       {\r\n"
"        \"height_microns\": 279400,\r\n"
"        \"name\": \"NA_LETTER\",\r\n"
"        \"width_microns\": 215900,\r\n"
"        \"is_default\": true\r\n"
"       }\r\n"
"      ]\r\n"
"     },\r\n"
"     \"color\": {\r\n"
"      \"option\": [\r\n"
"       {\r\n"
"        \"vendor_id\": \"grayscale\",\r\n"
"        \"type\": \"STANDARD_MONOCHROME\"\r\n"
"       },\r\n"
"       {\r\n"
"        \"vendor_id\": \"color\",\r\n"
"        \"type\": \"STANDARD_COLOR\",\r\n"
"        \"is_default\": true\r\n"
"       }\r\n"
"      ]\r\n"
"     },\r\n"
"     \"supported_content_type\": [\r\n"
"      {\r\n"
"       \"content_type\": \"image/pwg-raster\"\r\n"
"      },\r\n"
"      {\r\n"
"       \"content_type\": \"image/jpeg\"\r\n"
"      }\r\n"
"     ],\r\n"
"     \"media_path\": [\r\n"
"      {\r\n"
"       \"vendor_id\": \"fileout\"\r\n"
"      }\r\n"
"     ]\r\n"
"    },\r\n"
"    \"version\": \"1.0\"\r\n"
"   },\r\n"
"   \"capsHash\": \"\",\r\n"
"   \"updateTime\": \"1555548831115\",\r\n"
"   \"tags\": [\r\n"
"    \"__cp_printer_passes_2018_cert__\\u003dUNKNOWN\"\r\n"
"   ],\r\n"
"   \"proxy\": \"yoproxy\",\r\n"
"   \"createTime\": \"1555548831115\",\r\n"
"   \"updateUrl\": \"http://www.abc.com\",\r\n"
"   \"name\": \"BNET\",\r\n"
"   \"status\": \"\",\r\n"
"   \"accessTime\": \"1555548831115\"\r\n"
"  }\r\n"
" ],\r\n"
" \"complete_invite_url\": \"https://goo.gl/printer/72WYb\",\r\n"
" \"invite_url\": \"https://www.google.com/cloudprint/claimprinter.html\",\r\n"
" \"oauth_scope\": \"https://www.googleapis.com/auth/cloudprint\",\r\n"
" \"invite_page_url\": \"https://www.google.com/cloudprint/regtokenpage?t\\u003d79MYv\\u0026dpi\\u003d300\\u0026pagesize\\u003d215900,279400\",\r\n"
" \"registration_token\": \"79MYv\",\r\n"
" \"token_duration\": \"899\",\r\n"
" \"automated_invite_url\": \"https://www.google.com/cloudprint/confirm?token\\u003d79MYv\",\r\n"
" \"polling_url\": \"https://www.google.com/cloudprint/getauthcode?printerid\\u003dcaabfb70-f2c5-248f-efa2-5f5b98a4d07d\\u0026oauth_client_id\\u003d\"\r\n"
"}\r\n";

static const char *s_authrep_test =
"{\r\n"
" \"success\": true,\r\n"
" \"xmpp_jid\": \"646ce47de8d08b7105ca0ff1570cf4d9@cloudprint.googleusercontent.com\",\r\n"
" \"confirmation_page_url\": \"https://www.google.com/cloudprint/regconfirmpage?printername\\u003dBNET\\u0026email\\u003dbnettester1111@gmail.com\\u0026dpi\\u003d300\\u0026pagesize\\u003d215900,279400\",\r\n"
" \"request\": {\r\n"
"  \"time\": \"0\",\r\n"
"  \"params\": {\r\n"
"   \"printerid\": [\r\n"
"    \"079fd7ad-1047-894f-a3c5-908a249a9c8d\"\r\n"
"   ],\r\n"
"   \"oauth_client_id\": [\r\n"
"    \"923224369786-m81b6t3vrehpr8an5uopp1ggeabsh8r8.apps.googleusercontent.com\"\r\n"
"   ]\r\n"
"  }\r\n"
" },\r\n"
" \"user_email\": \"bnettester1111@gmail.com\",\r\n"
" \"authorization_code\": \"4/MAHdZyBasQKkT3a9xluvtKH-I6eP8xZAp0YEhhFKrZvoa7wpSh_xQS4rAJqEhSSvJ8CbIE869wFvMWFgTCQhPyc\"\r\n"
"}\r\n";

static const char *s_xmpp_test =
"<stream:stream from=\"xmpp.google.com\" id=\"887DC951F6C8C24A\" version=\"1.0\" xmlns:stream=\"http://etherx.jabber.org/streams\" xmlns=\"jabber:client\"><stream:features><mechanisms xmlns=\"urn:ietf:params:xml:ns:xmpp-sasl\"><mechanism>X-OAUTH2</mechanism><mechanism>X-GOOGLE-TOKEN</mechanism><mechanism>PLAIN</mechanism></mechanisms></stream:features>";

int gcp_unit_test(gcp_context_t *gcp)
{
    int result;
    bool done;

    result = gcp_init(gcp, "yoproxy", "68ECDD18-8319-42DD-BC73-87D192E900DD");
    if (result)
    {
        butil_log(0, "Fail: gcp init\n");
        return result;
    }
    gcp->io.data = (uint8_t*)s_regrep_test;
    gcp->io.tail = 0;
    gcp->io.head = sizeof(s_regrep_test);
    gcp->io.count = gcp->io.head;
    gcp->state = gcpAnonRegister;

    result = gcp_anon_register_reply(gcp);
    if (result)
    {
        butil_log(0, "Fail: register reply\n");
        return result;
    }
    result = gcp_prompt_for_claim(gcp);
    if (result)
    {
        butil_log(0, "Fail: prompt claim\n");
    }
    result = gcp_wait_for_claim(gcp, &done);
    if (result)
    {
        butil_log(0, "Fail: wait claim\n");
    }
    if (done)
    {
        butil_log(2, "User cancels claim\n");
    }
    else
    {
         gcp->io.data = (uint8_t*)s_authrep_test;
         gcp->io.tail = 0;
         gcp->io.head = sizeof(s_authrep_test);
         gcp->io.count = gcp->io.head;
         gcp->state = gcpAuthorizationReply;

         result = gcp_authorization_reply(gcp, &done);
         if (result)
         {
             butil_log(0, "Fail: authorization reply\n");
             return result;
         }
    }
    gcp->io.data = gcp->io_data;
    gcp->io.size = sizeof(gcp->io_data);
   /*
    result = gcp_nv_write(gcp);
    if (result)
    {
        butil_log(0, "Fail: NV Write\n");
        return result;
    }
   */
    result = gcp_nv_init(gcp);
    if (result)
    {
        butil_log(0, "Fail: NV Init\n");
        return result;
    }
    result = gcp_nv_read(gcp);
    if (result)
    {
        butil_log(0, "Fail: NV Read\n");
        return result;
    }
    result = gcp_nv_write(gcp);
    if (result)
    {
        butil_log(0, "Fail: NV Write\n");
        return result;
    }
    result = gcp_nv_read(gcp);
    if (result)
    {
        butil_log(0, "Fail: NV re-Read\n");
        return result;
    }
    result = gcp_nv_deinit(gcp);

    gcp_deinit(gcp);
    return result;
}

static int usage(const char *program)
{
    fprintf(stderr, "Use: %s [-lsu]\n", program);
    fprintf(stderr, "     -u    Run unit tests\n");
    fprintf(stderr, "     -l    Set debug log level (default 1: errors/warnings only)\n");
    fprintf(stderr, "     -s    Use TLS\n");
    return 1;
}

int main(int argc, char **argv)
{
    const char *program;
    uint32_t uval;
    int loglevel;
    bool unit_test;
    bool secure;
    char *arg;
    int result;

    gcp_context_t gcp;

#ifdef Windows
    WSADATA wsaData;

    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0)
    {
        BERROR("WSAStartup failed");
        return -1;
    }
#else
    signal(SIGPIPE, SIG_IGN);
#endif
    program = *argv++;
    argc--;

    loglevel = 5;
    butil_set_log_level(loglevel);

    unit_test = false;
    secure = false;

    while (argc > 0 && ! result)
    {
        arg = *argv;
        if (arg[0] == '-')
        {
            switch (arg[1])
            {
            case 'l':
            case 's':
            {
                if (arg[2] == '\0')
                {
                    if (argc > 0)
                    {
                        argc--;
                        argv++;
                        uval = strtoul(*argv, NULL, 0);
                    }
                    else
                    {
                        return usage(program);
                    }
                }
                else
                {
                    uval = strtoul((arg + 2), NULL, 0);
                }
                if (arg[1] == 'l')
                {
                    loglevel = uval;
                }
                else if (arg[1] == 's')
                {
                    secure = true;
                }
                else
                {
                    return usage(program);
                }
                break;
            }
            case 'u':
                unit_test = true;
                break;
            default:
                fprintf(stderr, "Bad Switch: %s\n", arg);
                break;
            }
            argc--;
            argv++;
        }
        else
        {
            return usage(program);
        }
    }
    butil_set_log_level(loglevel);

    if (unit_test)
    {
        return gcp_unit_test(&gcp);
    }
    else
    {
#ifdef BMEM_H
        static uint8_t g_spool1[0x100000];
        static uint8_t g_spool2[0x100000];
        static uint8_t g_mpool[0x100000];
        static uint8_t g_lpool[0x100000];

        void *pool;
        size_t pool_size, pool_chunk;
        size_t size;
        int result;

        result = bmem_init();
        if (result)
        {
            BERROR("Can't init pool\n");
            return result;
        }
        pool_size = sizeof(g_spool1);
        pool_chunk = 32;
        pool = g_spool1;
        result = bmem_add_pool(pool, pool_size, pool_chunk);
        if (result)
        {
            BERROR("Can't add small pool 1\n");
            return result;
        }
        pool_size = sizeof(g_spool2);
        pool_chunk = 64;
        pool = g_spool2;
        result = bmem_add_pool(pool, pool_size, pool_chunk);
        if (result)
        {
            BERROR("Can't add small pool 2\n");
            return result;
        }
        pool_size = sizeof(g_mpool);
        pool_chunk = 512;
        pool = g_mpool;
        result = bmem_add_pool(pool, pool_size, pool_chunk);
        if (result)
        {
            BERROR("Can't add medium pool\n");
            return result;
        }
        pool_size = sizeof(g_lpool);
        pool_chunk = 16384;
        pool = g_lpool;
        result = bmem_add_pool(pool, pool_size, pool_chunk);
        if (result)
        {
            BERROR("Can't add large pool\n");
            return result;
        }
#endif
        result = gcp_init(&gcp, "yoproxy", "68ECDD18-8319-42DD-BC73-87D192E900DD");
        do
        {
            result = gcp_slice(&gcp);
        }
        while (! result);

        gcp_deinit(&gcp);

        #ifdef BMEM_H
        bmem_stats(5);
        bmem_deinit();
        #endif
        return result;
    }
}

