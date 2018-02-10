#include "bsasl.h"

/* Username:	*/
static const char *s_username = "user";

/* Password:	*/
static const char *s_password = "pencil";

/* Client generates the random nonce */
static const uint8_t s_clientNonce[] = "fyko+d2lbbFgONRv9qkxdawL";

/* Initial message:	*/
static const uint8_t s_clientFirstMessage[] = "n,,n=user,r=fyko+d2lbbFgONRv9qkxdawL";

/* Server generates the random nonce */
static const uint8_t s_serverNonce[] = "3rfcNHYJY1ZVvWVs7j";

/* Server replies:	*/
static const uint8_t s_serverFirstMessage[] = "r=fyko+d2lbbFgONRv9qkxdawL3rfcNHYJY1ZVvWVs7j,s=QSXCR+Q6sek8bf92,i=4096";

/* The salt (hex):	*/
static const uint8_t s_serverSalt[] = "4125c247e43ab1e93c6dff76";

/* Client final message bare:	*/
static const uint8_t s_clientFinalMessageBare[] = "c=biws,r=fyko+d2lbbFgONRv9qkxdawL3rfcNHYJY1ZVvWVs7j";

/* Salted password (hex):	*/
static const uint8_t s_saltedPassword[] =  "1d96ee3a529b5a5f9e47c01f229a2cb8a6e15f7d";

/* Client key (hex):	*/
static const uint8_t s_clientKey[] = "e234c47bf6c36696dd6d852b99aaa2ba26555728";

/* Stored key (hex):	*/
static const uint8_t s_storedKey[] = "e9d94660c39d65c38fbad91c358f14da0eef2bd6";

/* Auth message:	*/
static const uint8_t s_authMessage[] = "n=user,r=fyko+d2lbbFgONRv9qkxdawL,r=fyko+d2lbbFgONRv9qkxdawL3rfcNHYJY1ZVvWVs7j,s=QSXCR+Q6sek8bf92,i=4096,c=biws,r=fyko+d2lbbFgONRv9qkxdawL3rfcNHYJY1ZVvWVs7j";

/* Client signature (hex):	*/
static const uint8_t s_clientSignature[] = "5d7138c486b0bfabdf49e3e2da8bd6e5c79db613";

/* Client proof (hex):	*/
static const uint8_t s_clientProof[] = "bf45fcbf7073d93d022466c94321745fe1c8e13b";

/* Server key (hex):	*/
static const uint8_t s_serverKey[] = "0fe09258b3ac852ba502cc62ba903eaacdbf7d31";

/* Server signature (hex):	*/
static const uint8_t s_serverSignature[] = "ae617da6a57c4bbb2e0286568dae1d251905b0a4";

/* Client final message:	*/
static const uint8_t s_clientFinalMessage[] = "c=biws,r=fyko+d2lbbFgONRv9qkxdawL3rfcNHYJY1ZVvWVs7j,p=v0X8v3Bz2T0CJGbJQyF0X+HI4Ts=";

/* Server final message:	*/
static const uint8_t s_serverFinalMessage[] = "v=rmF9pqV8S7suAoZWja4dJRkFsKQ=";

static int compareRaw(const char *label, uint8_t *actual,  const uint8_t *expected, size_t actual_len, size_t expected_len)
{
    char hv[4];
    int level = 5;
    size_t i;

	butil_log(level, "Compare %s:\n", label);
	if (actual_len != expected_len)
	{
		fprintf(stderr, "Actual Len=%u  Expected=%u\n", actual_len, expected_len);
		return -1;
	}
    for (i = 0; i < actual_len; i++)
    {
        butil_log(level, "%c", (actual[i] >= 32 && actual[i] < 127) ? actual[i] : ' ');
    }
    butil_log(level, "\n");
    for (i = 0; i < actual_len; i++)
    {
        snprintf(hv, sizeof(hv), "%02X", actual[i]);
        butil_log(level, "%c", hv[0]);
    }
    butil_log(level, "\n");
    for (i = 0; i < actual_len; i++)
    {
        snprintf(hv, sizeof(hv), "%02X", actual[i]);
        butil_log(level, "%c", hv[1]);
    }
    butil_log(level, "\n");
    for (i = 0; i < actual_len; i++)
    {
		if (actual[i] != expected[i])
		{
			butil_log(1, "^");
			butil_log(1, "  Expected: %02X, not %02X\n", expected[i], actual[i]);
			fprintf(stderr, "  Failed\n");
			return -1;
		}
		else
		{
			butil_log(1, " ");
		}
    }
    butil_log(level, "\n");
	return 0;
}

static int compareHex(const char *label, uint8_t *actual,  const uint8_t *expected, size_t actual_len, size_t expected_len)
{
	uint8_t rawval[256];
    uint8_t xv, vv;
	int result;
	int i;

	if (expected_len != 2 * actual_len)
	{
		fprintf(stderr, "Actual Len=%u  Expected=%u, should be %u\n", actual_len, expected_len, 2 * actual_len);
		return -1;
	}
	for (i = 0; i < actual_len; i++)
	{
		result =  butil_hextou(expected[2 * i], &xv);
		result += butil_hextou(expected[2 * i + 1], &vv);
		if (result)
		{
			fprintf(stderr, "Non-hex in expected at offset %d\n", 2 * i);
			return -1;
		}
		rawval[i] = (xv << 4) | vv;
	}
	return compareRaw(label, actual, rawval, actual_len, actual_len);
}

int main(int argc, char **argv)
{
	bsasl_auth_t *sasl;
	uint8_t abuf[2048];
	int result;

	butil_set_log_level(5);

	result = iostream_tls_prolog();
	if (result)
	{
		fprintf(stderr, "Failed to init TLS\n");
		return -1;
	}
	sasl = bsasl_auth_context_create(s_username, s_password);
	if (! sasl)
	{
		fprintf(stderr, "Failed to create\n");
		return -1;
	}
	// overwrite the generated nonce (todo and re-store user/pw)
	memcpy(sasl->clientNonce, s_clientNonce, sizeof(s_clientNonce));

	result = bsasl_start_authentication(sasl, bsaslAuthSCRAMSHA1);
	if (result)
	{
		fprintf(stderr, "Failed to start %d\n", result);
	}
	result = butil_base64_encode(abuf, sizeof(abuf), s_serverFirstMessage, sizeof(s_serverFirstMessage) - 1, false, false);
	if (result < 0)
	{
		fprintf(stderr, "Failed to encode server first message %d\n", result);
	}
	result = bsasl_finish_authentication(sasl, abuf);
	if (result)
	{
		fprintf(stderr, "Failed to finish %d\n", result);
	}
	// compare expected to gotten
	//
	result = compareHex("serverSalt", sasl->serverSalt, s_serverSalt, sasl->serverSalt_len, sizeof(s_serverSalt) - 1);
	if (result)
	{
		return -1;
	}
	result = compareRaw("clientFinalMessageBare", sasl->clientFinalMessageBare, s_clientFinalMessageBare, sasl->clientFinalMessageBare_len, sizeof(s_clientFinalMessageBare) - 1);
	if (result)
	{
		return -1;
	}
	result = compareHex("saltedPassword", sasl->saltedPassword, s_saltedPassword, BSASL_SHA1_KEY_SIZE, sizeof(s_saltedPassword) - 1);
	if (result)
	{
		return -1;
	}
	result = compareHex("clientKey", sasl->clientKey, s_clientKey, BSASL_SHA1_KEY_SIZE, sizeof(s_clientKey) - 1);
	if (result)
	{
		return -1;
	}
	result = compareHex("storedKey", sasl->storedKey, s_storedKey, BSASL_SHA1_KEY_SIZE, sizeof(s_storedKey) - 1);
	if (result)
	{
		return -1;
	}
	result = compareHex("clientSignature", sasl->clientSignature, s_clientSignature, BSASL_SHA1_KEY_SIZE, sizeof(s_clientSignature) - 1);
	if (result)
	{
		return -1;
	}
	result = compareHex("clientProof", sasl->clientProof, s_clientProof, BSASL_SHA1_KEY_SIZE, sizeof(s_clientProof) - 1);
	if (result)
	{
		return -1;
	}
	result = compareHex("serverKey", sasl->serverKey, s_serverKey, BSASL_SHA1_KEY_SIZE, sizeof(s_serverKey) - 1);
	if (result)
	{
		return -1;
	}
	result = compareHex("serverSignature", sasl->serverSignature, s_serverSignature, BSASL_SHA1_KEY_SIZE, sizeof(s_serverSignature) - 1);
	if (result)
	{
		return -1;
	}
	result = butil_base64_encode(abuf, sizeof(abuf), s_clientFinalMessage, sizeof(s_clientFinalMessage) - 1, false, false);
	if (result < 0)
	{
		fprintf(stderr, "decode cfm failed\n");
		return -1;
	}
	result = compareRaw("clientFinalMessage", sasl->clientFinalMessage, abuf, sasl->clientFinalMessage_len, result - 1);
	if (result)
	{
		return -1;
	}
	fprintf(stdout, "Pass\n");
	return 0;
}

