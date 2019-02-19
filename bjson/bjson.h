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
#ifndef BJSON_H
#define BJSON_H 1

#include "bnetheaders.h"

/// max depth of array/object nesting. parsing over values
/// needs a stack of up to this size. it is allocated in
/// the parser object to avoid large automatic variables
///
#define BJSON_MAX_NEST  128

/// size of max key sequence that can be unescaped. to use keys
/// with special characters (the json escaped chars) the key
/// has to be decoded before the json is scanned. instead of
/// allocating and freeing, the key is decoded into a fixed buf
///
#define BJSON_MAX_UNESCAPE  128

/// Internal error codes, usefull for debugging
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

/// JSON value type
///
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

/// JSON parser object
///
typedef struct
{
    const char *json;                   ///< the json string, null terminated
    const char *psrc;                   ///< current parsing location (internal use only)
    const char *pkey;                   ///< after parsing key, points to keys location
    size_t length;                      ///< size in bytes of json string
    char skipstack[BJSON_MAX_NEST];     ///< stack for parsing nested arrays/objects
    char keybuf[BJSON_MAX_UNESCAPE];    ///< buffer to unescape key components
}
bjson_parser_t;

/// Return the json type of value pointed to
///
/// @param pjx          [in] the json parser containing the json to parse
/// @param value        [in] the value to get the type of
/// @param type         [out] the resulting type, if found properly
///
/// @returns non-0 on errors (not a type)
///
int bjson_value_type(
                                bjson_parser_t *pjx,
                                const char *value,
                                bjson_type_t *type
                                );

/// Copy the json key value pointed to into a buffer decoding escapes
///
/// The parser key value pointer must be inside the
/// json contained in the parser object.  Resets the
/// parser to point to the end of the value parsed
///
/// @param pjx          [in] the json parser containing the json to parse
/// @param keyval       [in] the value to copy
/// @param value        [out] the buffer to copy the value into
/// @param nvalue       [in] the size, in bytes, of value
///
/// @returns non-0 on errors
///
int bjson_copy_key_value(
                                bjson_parser_t *pjx,
                                const char *keyval,
                                char *value,
                                size_t nvalue
                                );

/// Find (a sequence of) json objects by keys and point to the
/// value contained in the inner key, given a json parser
///
/// Similar to @bjson_find_key_value() but parsing begines at
/// the current spot in the json contained in the parser
///
/// The key path is a sequence of nested key components with optional
/// indices. The keys do not have to correspond to exact nesting
/// as the json being parsed, just the same general nesting
///
/// @param pjx          [in] the json parser containing the json to parse
/// @param keypath      [in] the key (sequence) to find
/// @param pathdelim    [in] the character that delimits keypath
/// @param index        [in] which of a series of inner element to get (array type)
/// @param value        [out] the pointer set to the value after the found inner key
///
/// See @bjson_find_and_copy_json_key_value() for examples
///
/// @returns non-0 on errors
///
int bjson_find_next_key(
                                bjson_parser_t *pjx,
                                const char *keypath,
                                char pathdelim,
                                size_t index,
                                const char **value
                                );

/// Find (a sequence of) json objects by keys and point to the
/// value contained in the inner key, given a json parser
///
/// The key path is a sequence of nested key components with optional
/// indices. The keys do not have to correspond to exact nesting
/// as the json being parsed, just the same general nesting
///
/// @param pjx          [in] the json parser containing the json to parse
/// @param keypath      [in] the key (sequence) to find
/// @param pathdelim    [in] the character that delimits keypath
/// @param index        [in] which of a series of inner element to get (array type)
/// @param value        [out] the pointer set to the value after the found inner key
///
/// See @bjson_find_and_copy_json_key_value() for examples
///
/// @returns non-0 on errors
///
int bjson_find_key_value(
                                bjson_parser_t *pjx,
                                const char *keypath,
                                char pathdelim,
                                size_t index,
                                const char **value
                                );

/// Find (a sequence of) json objects by keys and point to the
/// value contained in the inner key
///
/// The key path is a sequence of nested key components with optional
/// indices. The keys do not have to correspond to exact nesting
/// as the json being parsed, just the same general nesting
///
/// @param json         [in] the json to parse
/// @param keypath      [in] the key (sequence) to find
/// @param pathdelim    [in] the character that delimits keypath
/// @param index        [in] which of a series of inner element to get (array type)
/// @param value        [out] the pointer set to the value after the found inner key
///
/// See @bjson_find_and_copy_json_key_value() for examples
///
/// @returns non-0 on errors
///
int bjson_find_json_key_value(
                                const char *json,
                                const char *keypath,
                                char pathdelim,
                                size_t index,
                                const char **value
                                );

/// Find (a sequence of) json objects by keys and copy the value
/// contained in the inner key into the value parameter
///
/// The key path is a sequence of nested key components with optional
/// indices. The keys do not have to correspond to exact nesting
/// as the json being parsed, just the same general nesting
///
/// @param json         [in] the json to parse
/// @param keypath      [in] the key (sequence) to find
/// @param pathdelim    [in] the character that delimits keypath
/// @param index        [in] which of a series of inner element to get (array type)
/// @param value        [out] the value buffer to copy the inner keys value into
/// @param nvalue       [in] the size, in bytes, of value
///
/// Examples: for JSON of
///
/// {"aaa": [ { "bbb": [1,2,3], "ccc": [4,5,6] }, { "bbb": [7,8,9], "ccc": [10,11,12] } ] }
///
/// These element paths are equivalent where "." is the pathdelim
///
///   "aaa.bbb"
///   "bbb"
///
///  The inner value can be indexed using the element path like:
///
///    "ccc[1]"  - second value of first ccc -> 5
///    "aaa[1].ccc[2]" - third value of ccc in second aaa value -> 12
///
///  Or you can use an index in the call and not using the [] at
///  the end of the element path. You can not specify both a non-0
///  index parameter and a non-0 index at the end of the element path
///
/// @returns non-0 on errors
///
int bjson_find_and_copy_json_key_value(
                                const char *json,
                                const char *keypath,
                                const char pathdelim,
                                size_t index,
                                char *value,
                                size_t nvalue
                                );

/// Create a JSON parser (reader)
///
/// @param json     [in] the json to parse. Can't be NULL
///
/// @returns an allocated json parser object, or NULL on error
///
bjson_parser_t *bjson_parser_create(
                                const char *json
                                );

/// Free a JSON parser created with bjson_parser_create
///
/// @param pjx [in] pointer to json parser to free
///
/// @returns non-0 on error (no such parser)
///
int bjson_parser_destroy(
                                bjson_parser_t *pjx
                                );

#endif

