#ifndef BJSON_H
#define BJSON_H 1

typedef enum
{
    bjson_ok = 0,
    bjson_underflow,
    bjson_overflow,
    bjson_syntax,
    bjson_off_key,
    bjson_not_found,
}
json_error_t;

typedef enum
{
    bjson_null,
    bjson_false,
    bjson_true,
    bjson_number,
    bjson_string,
    bjson_array,
    bjson_object,
}
json_type_t;

typedef enum
{
    bjsInit,
    bjsObject,
    bjsValue,
    bjsElement,
    bjsQuote,
}
json_parse_state_t;

typedef struct
{
    const char *json;
    const char *psrc;
    const char *pkey;
    const char *pfirstval;
    json_parse_state_t state;
    size_t length;
    size_t index;
    size_t object_nest;
    size_t array_nest;
}
json_parser_t;

int bjson_get_key_value(json_parser_t *pjx, const char *key, int index, char *value, int nvalue);
int bjson_find_key(json_parser_t *pjx, const char *key);
int bjson_find_key_value(const char *json, const char *key, int index, char *value, int nvalue);
json_parser_t *bjson_parser_create(const char *json);
int bjson_parser_free(json_parser_t *pjx);

#endif

