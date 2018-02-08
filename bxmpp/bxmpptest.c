#include "bxmpp.h"

const char *s_username = "bnet_test_account@xmpp.jp";
const char *s_password = "jabberwocky";

int main(int argc, char **argv)
{
	bxmpp_t *bxp;
	int result;

	bxp = bxmpp_create("xmpp.jp"/*"jabber.org"*/, BXMPP_PORT, "xmpp.jp",
		"xmpp.jp");
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
