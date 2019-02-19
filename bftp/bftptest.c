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
#include "bftp.h"

static int on_listed_item(const char *item)
{
    butil_log(5, "%s\n", item);
    return 0;
}

static int on_listed_item_not_present(const char *item, void *priv)
{
    on_listed_item(item);
    return 0;
}

static int on_listed_item_present(const char *item, void *priv)
{
    on_listed_item(item);
    return 0;
}

int main(int argc, char **argv)
{
    bftpcontext_t *bftp;
    const char *program;
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

    #if 1
    result = bftp_get_file(
                    "ftp://ftp.driveHQ.com/test.jpg",
                    "fromremote.jpg",
                    "bnet_test_account",
                    "jabberwocky"
                    );
    if (result)
    {
        fprintf(stderr, "Can't get file\n");
    }
    #endif
    #if 1
    result = bftp_put_file(
                    "ftp://ftp.driveHQ.com/testret.jpg",
                    "fromremote.jpg",
                    "bnet_test_account",
                    "jabberwocky"
                    );
    if (result)
    {
        fprintf(stderr, "Can't put file\n");
    }
    #endif
    #if 1
    result = bftp_list_directory(
                    "ftp://ftp.driveHQ.com/.",
                    "bnet_test_account",
                    "jabberwocky",
                    on_listed_item_not_present,
                    (void*)"testret.jpg"
                    );
    if (result)
    {
        fprintf(stderr, "Can't put file\n");
    }
    #endif
    #if 1
    result = bftp_delete_file(
                    "ftp://ftp.driveHQ.com/testret.jpg",
                    "bnet_test_account",
                    "jabberwocky"
                    );
    if (result)
    {
        fprintf(stderr, "Can't delete file\n");
    }
    #endif
    #if 1
    result = bftp_list_directory(
                    "ftp://ftp.driveHQ.com/.",
                    "bnet_test_account",
                    "jabberwocky",
                    on_listed_item_not_present,
                    (void*)"testret.jpg"
                    );
    if (result)
    {
        fprintf(stderr, "Can't put file\n");
    }
    #endif
    return result;
}

