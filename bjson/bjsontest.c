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
#include "bnetheaders.h"
#include "bjson.h"

typedef struct
{
    const char *key;
    size_t      index;
    const char *exp_value;
    int         exp_ret;
    const char *json;
}
json_test_entry_t;

#define dimoftest(test) (sizeof(test)/sizeof(json_test_entry_t))

json_test_entry_t s_tests_1[] =
{
    {
    "name", 0, "null", 0,
    "{\n"
    "   \"name\": null\n"
    "}\n"
    },
    {
    "name", 0, "false", 0,
    "{\n"
    "   \"name\": false\n"
    "}\n"
    },
    {
    "name", 0, "true", 0,
    "{\n"
    "   \"name\": true\n"
    "}\n"
    },
    {
    "name", 0, "", bjson_syntax,
    "{\n"
    "   \"name\": true1\n"
    "}\n"
    },
    {
    "name", 0, "", bjson_syntax,
    "{\n"
    "   \"name\": tru\n"
    "}\n"
    },
    {
    "name", 0, "123", 0,
    "{\n"
    "   \"name\": 123\n"
    "}\n"
    },
    {
    "name", 0, "\"hello\"", 0,
    "{\n"
    "   \"name\": \"hello\"\n"
    "}\n"
    },
    {
    "name", 0, "hello", bjson_syntax,
    "{\n"
    "   \"name\": 123hello\n"
    "}\n"
    },
    {
    "name", 0, "hello", bjson_syntax,
    "{\n"
    "   \"name\": hello\n"
    "}\n"
    },
    {
    "name", 0, "1.23", 0,
    "{\n"
    "   \"name\": 1.23\n"
    "}\n"
    },
    {
    "name", 0, "12.3", 0,
    "{\n"
    "   \"name\": 12.3\n"
    "}\n"
    },
    {
    "name", 0, "none", bjson_syntax,
    "{\n"
    "   \"name\": 12.3.4\n"
    "}\n"
    },
    {
    "name", 0, "123.3", 0,
    "{\n"
    "   \"name\": 123.3}\n"
    "}\n"
    },
    {
    "name", 0, "124.3", 0,
    "{\n"
    "   \"name\": 124.3,\n"
    "}\n"
    },
    {
    "name", 0, "125.3", 0,
    "{\n"
    "   \"name\": 125.3]\n"
    "}\n"
    },
    {
    "name", 0, "126.3", 0,
    "{\n"
    "   \"name\": 126.3"
    "}\n"
    },
    {
    "name", 0, "-127.3", 0,
    "{\n"
    "   \"name\": -127.3"
    "}\n"
    },
    {
    "name", 0, "-128.3e4", 0,
    "{\n"
    "   \"name\": -128.3e4"
    "}\n"
    },
    {
    "name", 0, "-129.3e-4", 0,
    "{\n"
    "   \"name\": -129.3e-4"
    "}\n"
    },
    {
    "name", 0, "-120.3E+5", 0,
    "{\n"
    "   \"name\": -120.3E+5"
    "}\n"
    },
    {
    "name", 0, "13E-3", 0,
    "{\n"
    "   \"name\": 13E-3"
    "}\n"
    },
    {
    "name", 0, "none", bjson_syntax,
    "{\n"
    "   \"name\": -12-\n"
    "}\n"
    },
    {
    "name", 0, "none", bjson_syntax,
    "{\n"
    "   \"name\": -12e+-1\n"
    "}\n"
    },
    {
    "name", 0, "none", bjson_syntax,
    "{\n"
    "   \"name\": .12\n"
    "}\n"
    },
    {
    "name", 0, "none", bjson_syntax,
    "{\n"
    "   \"name\": +12\n"
    "}\n"
    },
    {
    "name", 0, "none", bjson_syntax,
    "{\n"
    "   \"name\": -12e.4\n"
    "}\n"
    },
    {
    "name", 0, "none", bjson_syntax,
    "{\n"
    "   \"name\": 1A\n"
    "}\n"
    },
    {
    "name", 0, "none", bjson_syntax,
    "{\n"
    "   \"name\": 004\n"
    "}\n"
    },
    {
    "name", 0, "none", bjson_memory,
    ""
    },
    {
    "name", 0, "none", bjson_not_found,
    " "
    },
    {
    "a", 0, "\"a\"", 0,
    "{\"a\":\"a\""
    },
    {
    "hi\\n\\rbye", 0, "\"yo\"", 0,
    "{\"hi\n\rbye\":\"yo\"}"
    },
    {
    "key", 0, "\"abcdef\"", 0,
    "\"key\": \"ab\\u0063\\u0064ef\""
    },
    {
    "key", 0, "\"abcd\"", 0,
    "\"key\": \"ab\\u0063\\u0064\""
    },
    {
    "key", 0, "\"\"", 0,
    "\"key\": \"\""
    }
};

json_test_entry_t s_tests_2[] =
{
    {
    "number_array", 1, "2", 0,
    "{\n"
    "   \"number_array\": [1,2,3,4,5,6]\n"
    "}\n"
    },
    {
    "number_array", 0, "1", 0,
    "{\n"
    "   \"number_array\": [ 1, 2, 3, 4, 5, 6 ]\n"
    "}\n"
    },
    {
    "number_array", 1, "2", 0,
    "{\n"
    "   \"number_array\": [1,2,3,4,5,6]\n"
    "}\n"
    },
    {
    "number_array", 2, "3", 0,
    "{\n"
    "   \"number_array\": [ 1,  { \"id\": 1, \"val\": \"junk\" },  3, 4 ,  5   ,  6  ]\n"
    "}\n"
    },
    {
    "number_array", 3, "4", 0,
    "{\n"
    "   \"number_array\": [ 1, { \"i\\\"{d\": 1, \"val\": \"j}unk\" }, 3, 4, 5, 6 ]\n"
    "}\n"
    },
    {
    "number_array", 4, "5", 0,
    "{\n"
    "   \"number_array\": [ 1, \"hello\", 3, 4, 5, 6 ]\n"
    "}\n"
    },
    {
    "number_array", 5, "6", 0,
    "{\n"
        "   \"number_array\": [ 1, [ 2, 3 ], { \"id\": 1, \"crap\":{\"val\":\"junk\"}}, 4, 5, 6 ]\n"
    "}\n"
    },
    {
    "number_array", 6, "x", bjson_not_found,
    "{\n"
    "   \"number_array\": [ 1, 2, 3, 4, 5, 6 ]\n"
    "}\n"
    },
    {
    "number_array", 5, "x", bjson_syntax,
    "{\n"
    "   \"number_array\": [ 1, 2, 3, 4, 5, ]\n"
    "}\n"
    },
};

json_test_entry_t s_tests_3[] =
{
    {
    " space ..name ", 0, "\" this is a test of spaces and . in key and value \"", 0,
    "{\n"
    "   \" space ..name \": \" this is a test of spaces and . in key and value \"\n"
    "}\n"
    },
    {
    "name", 0, "\"h\b\f\"\\a\"", 0,
    "{\n"
    "   \"name\": \"h\\b\\f\\\"\\\\a\"\n"
    "}\n"
    },
    {
    "name", 0, "none", bjson_syntax,
    "{\n"
    "   \"name\": \"h\\x\"\n"
    "}\n"
    },
};

json_test_entry_t s_tests_4[] =
{
    {
    "object", 0, "{\"id\":1,\"value\":2}", 0,
    "{ \"object\" : {\n"
    "   \"id\": 1,\n"
    "   \"value\": 2\n"
    "}\n"
    "}\n"
    },
    {
    "object", 0, "{\"id\":1,\"value\":2}", 0,
    "{ \"object\" : {\n"
    "   \"id\": 1,\n"
    "   \"value\": 2\n"
    "}\n"
    "}\n"
    },
};

static const char s_json_5[] =
    "{\n"
    "  \"object\":[ {\n"
    "   \"id1\": 1,\n"
    "   \"value1\": {\n"
    "     \"id2\": 2,\n"
    "     \"value2\": {\n"
    "      \"id3\": 3,\n"
    "      \"value3\": {\n"
    "       \"inner\": 5\n"
    "      }\n"
    "     }\n"
    "   }\n"
    " },{\n"
    "   \"id21\": 21,\n"
    "   \"value21\": {\n"
    "     \"id22\": 22,\n"
    "     \"value22\": {\n"
    "      \"id23\": 23,\n"
    "      \"value23\": {\n"
    "       \"inner2\": 25\n"
    "      }\n"
    "     }\n"
    "   }\n"
    "  }]\n"
    "}\n";

json_test_entry_t s_tests_5[] =
{
    {
    "object", 0, "{\"id1\":1,\"value1\":{\"id2\":2,\"value2\":{\"id3\":3,\"value3\":{\"inner\":5}}}}", 0,
    "{\n"
    "  \"object\" : {\n"
    "   \"id1\": 1,\n"
    "   \"value1\": {\n"
    "     \"id2\": 2,\n"
    "     \"value2\": {\n"
    "      \"id3\": 3,\n"
    "      \"value3\": {\n"
    "       \"inner\": 5\n"
    "      }\n"
    "     }\n"
    "   }\n"
    " }\n"
    "}\n"
    },
    {
    "object", 0, "{\"id1\":1,\"value1\":{\"id2\":2,\"value2\":{\"id3\":3,\"value3\":{\"inner\":5}}}}", 0,
    s_json_5
    },
    {
    "object", 1, "{\"id21\":21,\"value21\":{\"id22\":22,\"value22\":{\"id23\":23,\"value23\":{\"inner2\":25}}}}", 0,
    s_json_5
    },
};

static const char s_json_6[] =
"{\"aaa\":[{\"bbb\":[1,2,3],\"ccc\":[4,5,6]},{\"bbb\":[7,8,9],\"ccc\":[10,11,12]}]}";

static const char s_json_33[] =
"{\n"
"   \"items\":\n"
"       {\n"
"           \"item\":\n"
"               [\n"
"                   {\n"
"                       \"id\": \"0001\",\n"
"                       \"type\": \"donut\",\n"
"                       \"name\": \"Cake\",\n"
"                       \"ppu\": 0.55,\n"
"                       \"batters\":\n"
"                           {\n"
"                               \"batter\":\n"
"                                   [\n"
"                                       { \"id\": \"1001\", \"type\": \"Regular\" },\n"
"                                       { \"id\": \"1002\", \"type\": \"Chocolate\" },\n"
"                                       { \"id\": \"1003\", \"type\": \"Blueberry\" },\n"
"                                       { \"id\": \"1004\", \"type\": \"Devil's Food\" }\n"
"                                   ]\n"
"                           },\n"
"                       \"topping\":\n"
"                           [\n"
"                               { \"id\": \"5001\", \"type\": \"None\" },\n"
"                               { \"id\": \"5002\", \"type\": \"Glazed\" },\n"
"                               { \"id\": \"5005\", \"type\": \"Sugar\" },\n"
"                               { \"id\": \"5007\", \"type\": \"Powdered Sugar\" },\n"
"                               { \"id\": \"5006\", \"type\": \"Chocolate with Sprinkles\" },\n"
"                               { \"id\": \"5003\", \"type\": \"Chocolate\" },\n"
"                               { \"id\": \"5004\", \"type\": \"Maple\" }\n"
"                           ]\n"
"                   }\n"
"               ]\n"
"       }\n"
"}\n";

json_test_entry_t s_tests_33[] =
{
    {
    "aaa.ccc", 0, "4", 0,
    s_json_6
    },
    {
    "ccc[1]", 0, "5", 0,
    s_json_6
    },
    {
    "aaa[1].ccc[2]", 0, "12", 0,
    s_json_6
    },
    {
    "items.item.ppu", 0, "0.55", 0,
    s_json_33
    },
    {
    "items.item.batters.batter.type", 0, "\"Regular\"", 0,
    s_json_33
    },
    {
    "items.item.batters.batter", 3, "{\"id\":\"1004\",\"type\":\"Devil\'s Food\"}", 0,
    s_json_33
    },
    {
    "items.item.batters.batter[3]", 0, "{\"id\":\"1004\",\"type\":\"Devil\'s Food\"}", 0,
    s_json_33
    },
    {
    "items.item.batters.batter[1]", 2, "", bjson_parameter,
    s_json_33
    },
    {
    "items.item.batters.batter[2]", 0, "{\"id\":\"1003\",\"type\":\"Blueberry\"}", 0,
    s_json_33
    },
    {
    "items.item.batters.batter[2].type", 0, "\"Blueberry\"", 0,
    s_json_33
    },
    {
    "items.item.topping[6].id", 0, "\"5004\"", 0,
    s_json_33
    },
    {
    "items.item.topping[7].id", 0, "none", bjson_not_found,
    s_json_33
    },
    {
    "items.item.id[0].id", 0, "none", bjson_not_object,
    s_json_33
    },
    {
    // overflow if used with smaller buffer
    "items.item[0]", 0, "none", bjson_overflow,
    s_json_33
    },
    {
    "items.item[1]", 0, "none", bjson_not_found,
    s_json_33
    },
    {
        // explicitly allow "skipping" objects inside the two objects in key path
        "value1.value3", 0, "{\"inner\":5}", 0,
        s_json_5
    },
    {
        // explicitly don't allow "skipping" across objects
        "value1.value22", 0, "none", bjson_not_found,
        s_json_5
    },
};

static int runtest(json_test_entry_t *entry, size_t numentries, const char *blurb, const char pathdelim)
{
    char val[128]; // note, keep this size as overflow test depends on it
    int result;
    size_t i;

    for (i = 0; i < numentries; i++, entry++)
    {
        printf("%s %zu\n", blurb ? blurb : "", i);
        result = bjson_find_and_copy_json_key_value(
                    entry->json, entry->key, pathdelim, entry->index, val, sizeof(val));
        if (result != entry->exp_ret)
        {
            fprintf(stderr, "find error: %d, expected %d\n", result, entry->exp_ret);
            return -1;
        }
        if (! entry->exp_ret)
        {
            if (strcmp(entry->exp_value, val))
            {
                fprintf(stderr, "value error: %s, expected %s\n", val, entry->exp_value);
                return -1;
            }
            else
            {
                if (entry->index)
                {
                    printf("  Key:%s[%zu] Value:%s\n", entry->key, entry->index, val);
                }
                else
                {
                    printf("  Key:%s Value:%s\n", entry->key, val);
                }
            }
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    bjson_parser_t *pjx;
    const char *key;
    const char *value;
    char val[128];
    int result;

#if 1
    // general keywords and basic types
    result = runtest(s_tests_1, dimoftest(s_tests_1), "Basic", '\0');
    if (result)
    {
        return result;
    }
#endif
#if 1
    // number arrays
    result = runtest(s_tests_2, dimoftest(s_tests_2), "NumArrays", '\0');
    if (result)
    {
        return result;
    }
#endif
#if 1
    // harder strings
    result = runtest(s_tests_3, dimoftest(s_tests_3), "Strings", '\0');
    if (result)
    {
        return result;
    }
#endif
#if 1
    // object values
    result = runtest(s_tests_4, dimoftest(s_tests_4), "Objects", '\0');
    if (result)
    {
        return result;
    }
#endif
#if 1
    // nested object values
    result = runtest(s_tests_5, dimoftest(s_tests_5), "NestedObjects", '\0');
    if (result)
    {
        return result;
    }
#endif
#if 1
    pjx = bjson_parser_create(s_json_33);
    if (! pjx)
    {
        fprintf(stderr, "can't create parser\n");
        return -1;
    }
    // by-hand test
    key = "name";
    result = bjson_find_key_value(pjx, key, '\0', 0, &value);
    if (result)
    {
        fprintf(stderr, "find error: %d\n", result);
        return -1;
    }
    result = bjson_copy_key_value(pjx, value, val, sizeof(val));
    if (result)
    {
        fprintf(stderr, "get error: %d\n", result);
        return -1;
    }
    if (strcmp(val, "\"Cake\""))
    {
        fprintf(stderr, "Expected Cake, got %s\n", val);
        return -1;
    }
    key = "name[123]";
    result = bjson_find_key_value(pjx, key, '\0', 0, &value);
    if (result != bjson_not_array)
    {
        fprintf(stderr, "find error: %d, expected %d\n", result, bjson_not_array);
        return -1;
    }
#endif
#if 1
    // nested object values
    result = runtest(s_tests_33, dimoftest(s_tests_33), "key-paths", '.');
    if (result)
    {
        return result;
    }
#endif
    return 0;
}

