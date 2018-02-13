
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

