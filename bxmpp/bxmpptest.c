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
#include "bxmpp.h"

#if 0
//const char *s_username = "bnettester1111@gmail.com";
const char *s_server   = "xmpp.google.com";
const char *s_password = "ya29.GlvxBuoI01xMDojrU1N0Zbim40GFZ1WqVRs1Mnpet8_5d5DjD8B1Z1PclYqxCkEccnpsjCgN6O3Uw7p2POXNxh2tHYhqUgUmnnynhoKjUxJpAk3AChBNU3noDq5y";
const char *s_username = "441b6afa351b24e78ef471243fda7d80@cloudprint.googleusercontent.com";
bsasl_auth_type_t s_saslAuth = bsaslAuthOAUTH2;
#else
const char *s_username = "bnet_test_account";
//const char *s_server   = "xmpp.jp";
const char *s_server   = "jwchat.org";
const char *s_password = "jabberwocky";
bsasl_auth_type_t s_saslAuth = bsaslAuthPLAIN;
//bsasl_auth_type_t s_saslAuth = bsaslAuthSCRAMSHA1;
#endif

int mcb(bxmpp_t *bxp, void *priv, const char *sender, const char *msg)
{
	if (bxp && sender && msg)
	{
		butil_log(2, "Message from %s: %s\n", sender, msg);
	}
	return 0;
}

int main(int argc, char **argv)
{
	bxmpp_t *bxp;
	int result;

	butil_set_log_level(7);

	bxp = bxmpp_create(
						s_server, BXMPP_PORT,
						s_saslAuth,
						s_username, s_password,
						"talk",
						10,
						mcb, NULL
						);
	if (! bxp)
	{
		BERROR("Can't create context\n");
	}
	while (! bxmpp_connected(bxp))
	{
		result = bxmpp_slice(bxp);
		if (result)
		{
			break;
		}
		if (bxmpp_finished(bxp))
		{
			break;
		}
	}
	result = bxmpp_send_message(bxp, bxp->jid, "Hey this is testing\n");
	if (result)
	{
		butil_log(0, "Fail: send message\n");
	}
	while (! bxmpp_finished(bxp))
	{
		result = bxmpp_slice(bxp);
		if (result)
		{
			break;
		}
	}
	bxmpp_destroy(bxp);
	return 0;
}
