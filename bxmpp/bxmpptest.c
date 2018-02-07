#include "bxmpp.h"

int main(int argc, char **argv)
{
	bxmpp_t *bxp;
	int result;

	bxp = bxmpp_create("jabber.org", BXMPP_PORT, "me", "jabber.org/echo");
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
