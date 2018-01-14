#ifndef BJSON_H
#define BJSON_H 1

/// max depth of array/object nesting
///
#define BJSON_MAX_NEST  128

typedef enum
{
    bjson_ok = 0,
    bjson_memory,
    bjson_parameter,
    bjson_underflow,
    bjson_overflow,
    bjson_syntax,
    bjson_off_key,
    bjson_not_object,
    bjson_not_array,
    bjson_not_found,
}
bjson_error_t;

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
bjson_type_t;

typedef struct
{
    const char *json;
    const char *psrc;
    const char *pkey;
    size_t length;
}
bjson_parser_t;

int bjson_value_type(
                                bjson_parser_t *pjx,
                                const char *value,
                                bjson_type_t *type
                                );

int bjson_copy_value(
                                bjson_parser_t *pjx,
                                char *value,
                                int nvalue
                                );

int bjson_get_key_value(
                                bjson_parser_t *pjx,
                                int index,
                                const char **value
                                );

int bjson_copy_key_value(
                                bjson_parser_t *pjx,
                                int index,
                                char *value,
                                int nvalue
                                );

int bjson_find_next_key(
                                bjson_parser_t *pjx,
                                const char *keypath,
                                char pathdelim
                                );

int bjson_find_key(
                                bjson_parser_t *pjx,
                                const char *keypath,
                                char pathdelim
                                );

int bjson_find_key_value(
                                const char *json,
                                const char *keypath,
                                char pathdelim,
                                int index,
                                const char **value
                                );

int bjson_find_and_copy_key_value(
                                const char *json,
                                const char *keypath,
                                char pathdelim,
                                int index,
                                char *value,
                                int nvalue
                                );

bjson_parser_t *bjson_parser_create(
                                const char *json
                                );

int bjson_parser_free(
                                bjson_parser_t *pjx
                                );

#endif

