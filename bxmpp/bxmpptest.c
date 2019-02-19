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

const char *s_username = "bnet_test_account";
//const char *s_server   = "xmpp.jp";
const char *s_server   = "jwchat.org";
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
