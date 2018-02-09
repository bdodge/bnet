#include "bxmpp.h"

//const char *s_username = "bnet_test_account@xmpp.jp";
const char *s_username = "bnet_test_account";
const char *s_server   = "xmpp.jp";
//const char *s_server   = "jwchat.org";
const char *s_password = "jabberwocky";

int main(int argc, char **argv)
{
	bxmpp_t *bxp;
	int result;

	butil_set_log_level(5);

	bxp = bxmpp_create(
						s_server, BXMPP_PORT,
						s_username, s_password,
						"talk"
						);
	if (! bxp)
	{
		BERROR("Can't create context\n");
	}
	while (bxp->state != bxmppDone)
	{
		result = bxmpp_setup(bxp);
		if (result)
		{
			break;
		}
	}
	bxmpp_destroy(bxp);
	return 0;
}
