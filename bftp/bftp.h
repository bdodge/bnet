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
#ifndef BFTP_H
#define BFTP_H 1

#include "bnetheaders.h"
#include "bstreamio.h"
#include "butil.h"

#define BFTP_IO_SIZE (2*1436)

#define BFTP_MAX_USER    64
#define BFTP_MAX_HOST    256
#define BFTP_MAX_PATH    256
#define BFTP_MAX_LIST_ITEM 256

#define BFTP_MAX_EXPECTED_CODES  32

typedef enum
{
    bftpGet,
    bftpPut,
    bftpDir,
    bftpDel
}
bftp_method_t;

typedef enum
{
    bftpInit,
    bftpControlTransportInit,
    bftpControlTransport,
    bftpLogin,
    bftpLoginReply,
    bftpAuthenticate,
    bftpAuthenticateReply,
    bftpLoggedIn,
    bftpPassiveReply,
    bftpSetBinary,
    bftpBinaryReply,
    bftpDataTransport,
    bftpMethod,
    bftpMethodReply,
    bftpMethodDispatch,
    bftpDispatchReply,
    bftpUpload,
    bftpDownload,
    bftpListing,
    bftpTransferReply,
    bftpInPhase,
    bftpOutPhase,
    bftpDone,
}
bftp_state_t;

typedef int (bftp_listing_cb_t)(const char *item, void *priv);

typedef struct tag_ftpcontext
{
    bftp_method_t   method;
    bool            usepassive;
    char            username[BFTP_MAX_USER];
    char            password[BFTP_MAX_USER];
    char            host[BFTP_MAX_HOST];
    uint16_t        port;
    char            path[BFTP_MAX_HOST];
    bftp_listing_cb_t *callback;
    void            *priv;
    bftp_state_t    state;
    bftp_state_t    next_state;
    uint32_t        expected_codes[BFTP_MAX_EXPECTED_CODES];
    size_t          num_codes;
    uint32_t        last_code;
    uint8_t         iobuf[BFTP_IO_SIZE];
    ioring_t        io;
    size_t          xfer_data_total;
    size_t          xfer_file_total;
    iostream_t      *ctrl_stream;
    iostream_t      *data_stream;
    iostream_t      *file_stream;
}
bftpcontext_t;

int bftp_slice(
                bftpcontext_t *bftp
);

int bftp_get_file(
                const char  *remotepath,
                const char  *localpath,
                const char  *username,
                const char  *password
                );

int bftp_put_file(
                const char  *remotepath,
                const char  *localpath,
                const char  *username,
                const char  *password
                );

int bftp_delete_file(
                const char  *remotepath,
                const char  *username,
                const char  *password
                );

int bftp_list_directory(
                const char  *remotepath,
                const char  *username,
                const char  *password,
                bftp_listing_cb_t *callback,
                void        *priv
                );

void bftp_client_destroy(
                bftpcontext_t *bftp
                );

bftpcontext_t *bftp_client_create(void);

#endif
