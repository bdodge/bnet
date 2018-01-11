
#include "bnetheaders.h"
#include "bjson.h"

typedef struct
{
	const char *key;
	const char *exp_value;
	int         exp_ret;
	const char *json;
}
json_test_entry_t;

json_test_entry_t s_tests[] =
{
	{
	"name", "null", 0,
	"{\n"
	"	\"name\": null\n"
	"}\n"
	},
	{
	"name", "false", 0,
	"{\n"
	"	\"name\": false\n"
	"}\n"
	},
	{
	"name", "true", 0,
	"{\n"
	"	\"name\": true\n"
	"}\n"
	},
	{
	"name", "123", 0,
	"{\n"
	"	\"name\": 123\n"
	"}\n"
	}
};

static const char *jt33 =
"{\n"
"	\"items\":\n"
"		{\n"
"			\"item\":\n"
"				[\n"
"					{\n"
"						\"id\": \"0001\",\n"
"						\"type\": \"donut\",\n"
"						\"name\": \"Cake\",\n"
"						\"ppu\": 0.55,\n"
"						\"batters\":\n"
"							{\n"
"								\"batter\":\n"
"									[\n"
"										{ \"id\": \"1001\", \"type\": \"Regular\" },\n"
"										{ \"id\": \"1002\", \"type\": \"Chocolate\" },\n"
"										{ \"id\": \"1003\", \"type\": \"Blueberry\" },\n"
"										{ \"id\": \"1004\", \"type\": \"Devil's Food\" }\n"
"									]\n"
"							},\n"
"						\"topping\":\n"
"							[\n"
"								{ \"id\": \"5001\", \"type\": \"None\" },\n"
"								{ \"id\": \"5002\", \"type\": \"Glazed\" },\n"
"								{ \"id\": \"5005\", \"type\": \"Sugar\" },\n"
"								{ \"id\": \"5007\", \"type\": \"Powdered Sugar\" },\n"
"								{ \"id\": \"5006\", \"type\": \"Chocolate with Sprinkles\" },\n"
"								{ \"id\": \"5003\", \"type\": \"Chocolate\" },\n"
"								{ \"id\": \"5004\", \"type\": \"Maple\" }\n"
"							]\n"
"					}\n"
"				]\n"
"		}\n"
"}\n";

int main(int argc, char **argv)
{
	json_parser_t *pjx;
	const char *key;
	json_test_entry_t *entry;
	int i;
	char smallval[16];
	char val[128];
	char bigval[1024];
	int result;

	for (i = 0; i < sizeof(s_tests) / sizeof(json_test_entry_t); i++)
	{
		entry = &s_tests[i];
		result = bjson_find_key_value(entry->json, entry->key, 0, val, sizeof(val));
		printf("Test %d\n", i);
		if (result != entry->exp_ret)
		{
			fprintf(stderr, "find error: %d, expected %d\n", result, entry->exp_ret);
			return -1;
		}
		if (strcmp(entry->exp_value, val))
		{
			fprintf(stderr, "value error: %s, expected %s\n", val, entry->exp_value);
			return -1;
		}
		else
		{
			printf("Key:%s Value:%s\n", entry->key, val);
		}
	}
	pjx = bjson_parser_create(jt33);
	if (! pjx)
	{
		fprintf(stderr, "can't create parser\n");
		return -1;
	}
	key = "name";
	result = bjson_find_key(pjx, key);
	if (result)
	{
		fprintf(stderr, "find error: %d\n", result);
		return -1;
	}
	result = bjson_get_key_value(pjx, key, 0, val, sizeof(val));
	if (result)
	{
		fprintf(stderr, "get error: %d\n", result);
		return -1;
	}
	printf("Key:%s Value:%s\n", key, val);
    return 0;
}
