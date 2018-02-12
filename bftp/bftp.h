#ifndef BFTP_H
#define BFTP_H 1

#include "bnetheaders.h"
#include "bstreamio.h"
#include "butil.h"

#define BFTP_IO_SIZE (2*1436)

#define BFTP_MAX_USER	64
#define BFTP_MAX_HOST	256
#define BFTP_MAX_PATH	256

#define BFTP_MAX_EXPECTED_CODES	32

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

typedef struct tag_ftpcontext
{
	bftp_method_t	method;
	bool			usepassive;
	char			username[BFTP_MAX_USER];
	char			password[BFTP_MAX_USER];
	char			host[BFTP_MAX_HOST];
	uint16_t		port;
	char			path[BFTP_MAX_HOST];
	bftp_state_t	state;
	bftp_state_t	next_state;
	uint32_t		expected_codes[BFTP_MAX_EXPECTED_CODES];
	size_t			num_codes;
	uint32_t		last_code;
	uint8_t			iobuf[BFTP_IO_SIZE];
	ioring_t		io;
	size_t			xfer_data_total;
	size_t			xfer_file_total;
	iostream_t		*ctrl_stream;
	iostream_t		*data_stream;
	iostream_t		*file_stream;
}
bftpcontext_t;

int bftp_slice(
					bftpcontext_t *bftp
					);

int bftp_get_file(
					const char	*remotepath,
					const char	*localpath,
					const char  *username,
					const char  *password
					);

int bftp_put_file(
					const char	*remotepath,
					const char	*localpath,
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
                    const char  *password
                 	);

void bftp_client_destroy(
					bftpcontext_t *bftp
					);

bftpcontext_t *bftp_client_create(void);

#endif
