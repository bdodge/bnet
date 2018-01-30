#ifndef BXML_H
#define BXML_H 1

/// max depth of element nesting
///
#define BXML_MAX_NEST  128

typedef enum
{
    bxml_ok = 0,
    bxml_memory,
    bxml_parameter,
    bxml_underflow,
    bxml_overflow,
    bxml_syntax,
    bxml_no_element,
    bxml_not_found,
}
bxml_error_t;

typedef enum
{
    bjson_null,
    bjson_tag,
    bjson_attribute_name,
    bjson_attribute_value,
    bjson_element,
}
bxml_type_t;

typedef struct
{
    const char *xml;
    const char *psrc;
    const char *pelement;
    size_t depth;
    size_t length;
}
bxml_parser_t;

int bxml_get_nth_element(
                                bxml_parser_t *pjx,
                                size_t index,
                                const char **element
                                );

int bxml_copy_nth_element(
                                bxml_parser_t *pjx,
                                size_t index,
                                char *element,
                                size_t nelement
                                );

int bxml_find_next_element(
                                bxml_parser_t *pjx,
                                const char *elementpath,
                                char pathdelim,
                                size_t *pindex
                                );

int bxml_find_first_element(
                                bxml_parser_t *pjx,
                                const char *elementpath,
                                char pathdelim,
                                size_t *pindex
                                );

int bxml_find_nth_element(
                                const char *xml,
                                const char *elementpath,
                                char pathdelim,
                                size_t index,
                                const char **element
                                );

int bxml_find_and_copy_nth_element(
                                const char *xml,
                                const char *elementpath,
                                const char pathdelim,
                                size_t index,
                                char *element,
                                size_t nelement
                                );

bxml_parser_t *bxml_parser_create(
                                const char *xml
                                );

int bxml_parser_free(
                                bxml_parser_t *pjx
                                );

#endif

