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
#ifndef BXML_H
#define BXML_H 1

#include "bnetheaders.h"

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
    bxttNone,
    bxttStart,
    bxttEnd
}
bxml_tag_type_t;

typedef struct
{
    bool selfowned;
    const char *xml;
    const char *root;
    const char *psrc;
    size_t line;
    size_t error_line;
    bxml_error_t error_code;
    size_t length;
}
bxml_parser_t;

/// Format an xml header
///
/// @param buf      [out] result of formatting
/// @param nbuf     [in] size of buf, in bytes
/// @param nout     [out] bytes written to buf
/// @param version  [in] version string to use, if NULL, assume "1.0"
/// @param encoding [in] encoding to use, if NULL, assume utf8
///
/// Examples:
///   bxml_format_header(buf, sizeof(buf), &outcount, NULL, NULL);
///       creates in buf "<?xml version="1.0" encoding="utf-8"?>"
///   bxml_format_header(buf, sizeof(buf), &outcount, "hi", "bye");
///       creates in buf "<?xml version="hi" encoding="byte"?>"
///
/// @returns non-0 on error (like buffer overflow would happen)
///
int bxml_format_header(
                char *buf,
                size_t nbuf,
                size_t *nout,
                const char *version,
                const char *encoding
                );

/// Format an element from tag and optional value and attributes
///
/// @param buf      [out] result of formatting
/// @param nbuf     [in] size of buf, in bytes
/// @param nout     [out] bytes written to buf
/// @param start    [in] true if a document header should be prepended
/// @param tag      [in] the element's tag
/// @param value    [in] the element's value.
///                         If NULL, the element is not closed
///                         If non-NULL but blank, the element is self-closed
///                         otherwise element is closed around value
/// @param nattribs [in] number of attribute-value pairs that follow, can be 0
/// @param attribute-value pairs [in] two const char* of attribute and value
///
/// Examples:
///   bxml_format_element(buf, sizeof(buf), &outcount, false, "hi", "bye", 1, "this", "value");
///       creates in buf "<hi this='value'>bye</hi>"
///   bxml_format_element(buf, sizeof(buf), &outcount, true, "hi", "", 0);
///       creates in buf "<?xml version='1.0'?><hi/>"
///   bxml_format_element(buf, sizeof(buf), &outcount, false, "hi", NULL, 0);
///       creates in buf "<hi>"
///
/// @returns non-0 on error (like buffer overflow would happen)
///
int bxml_format_element(
                char *buf,
                size_t nbuf,
                size_t *nout,
                bool start,
                const char *tag,
                const char *value,
                size_t nattribs,
                ...
                );

/// Close the element started by a call to @bxml_format_element()
///
/// @param buf      [out] result of formatting
/// @param nbuf     [in] size of buf, in bytes
/// @param nout     [out] bytes written to buf
/// @param tag [in] the tag that started the element
///
/// Examples:
///   bxml_format_endtag(buf, sizeof(buf), &outcount, "hi");
///       creates in buf "</hi>"
///
/// @returns non-0 on error
///
int bxml_format_endtag(
                char *buf,
                size_t nbuf,
                size_t *nout,
                const char *tag
                );

/// Find the attribute in the tag
///
/// @param pxp      [in]  the xml parser, may be NULL, used for error checking only
/// @param ptag     [in]  the start tag of the element to get attribute from
/// @param pattrib  [in]  the attribute name to look for
/// @param attrib_value [out]  pointer to attribute value, set if found
/// @param attrib_value_len [out]  set to length in bytes in attrib_value
///
/// @returns non-0 on error
///
int bxml_find_attribute(
                                bxml_parser_t *pxp,
                                const char *ptag,
                                const char *pattrib,
                                const char **attrib_value,
                                size_t *attrib_value_len
                                );

/// Find the attribute in the tag and copy the value, expanding entities, etc.
///
/// @param pxp      [in]  the xml parser, may be NULL, used for error checking only
/// @param ptag     [in]  the start tag of the element to get attribute from
/// @param pattrib  [in]  the attribute name to look for
/// @param attrib_value [out]  pointer to buffer for attribute value
/// @param attrib_value_len [in]  bytes in attrib_value
/// @param keep_white    [in] set non-0 to preserve all white space in values
/// @param keep_entities [in] set non-0 to suppress expansion of entities in value copy
///
/// @returns non-0 on error
///
int bxml_find_and_copy_attribute(
                                bxml_parser_t *pxp,
                                const char *ptag,
                                const char *pattrib,
                                char *attrib_value,
                                size_t attrib_value_len,
                                bool keep_white,
                                bool keep_entities
                                );

/// Copy an elements value into a buffer, decoding and extracting any
/// literal entities, etc. The parser must be on the start of the
/// value, as after a call to @bxml_parse_value().
///
/// @param element  [out] the buffer to copy element in to
/// @param nelement [in]  the size if element in bytes
/// @param value    [in]  the pointer to the value of an element in pxp
/// @param value_length [in] the number of bytes in the value
/// @param keep_white    [in] set non-0 to preserve all white space in values
/// @param keep_entities [in] set non-0 to suppress expansion of entities in value copy
///
int bxml_copy_element(
                                char *element,
                                size_t nelement,
                                const char *value,
                                size_t value_length,
                                bool keep_white,
                                bool keep_entities
                                );

/// Parse to the end of an element, getting the length of the enclosed
/// value. Also can be used to find a child tag in an element
/// The xml parser must be on the start of the element to get the
/// value of, as by after a callto @bxml_find_nth_element()
/// If used for finding a child element, the value is not set but the
/// end_tag parameter is set to where the parsing stopped, if the
/// child tag was found before the end_tag of the enclosing element
///
/// @param pxp      [in] xml parser
/// @param from_tag [in] the start tag of the enclosing element
/// @param till_tag [in] if non-NULL, look for this child elemenet before the
///                      end tag for from_tag (and not the value.  If NULL,
///                      look for the end tag to match from_tag and set value
/// @param tag_len  [in] if till_tag non-NULL, the length in bytes of till_tag
/// @param end_tag [out] if till_tag non-NULL, will be set to start of till_tag in xml
/// @param value   [out] if till_tag NULL, will be set to start of value if from_tag
/// @param value_len [out] if value set, this is set to length of value in bytes
///
/// @returns non-0 on error
///
int bxml_parse_value(
                                bxml_parser_t *pxp,
                                const char *from_tag,
                                const char *till_tag,
                                size_t tag_len,
                                const char **end_tag,
                                const char **value,
                                size_t *value_len
                                );

/// Find (a sequence of) xml elements by tag, starting from
/// the current position in the parser. This can be called
/// after a call to @bxml_find_and_copy_nth_element() or
/// @bxml_find_nth_element to get the next sequential instance
/// of the elementpath.
///
/// The element path is a sequence of nested tags with optional
/// indices. The tags do not have to correspond to exact nesting
/// as the xml being parse, just the same general nesting.
/// see @bxml_find_and_copy_nth_element() for examples.
///
/// @param pxp          [in] the xml parser
/// @param elementpath  [in] the element (sequence) to define
/// @param pathdelim    [in] the character that delimits the elementpath
/// @param index        [in] which of a series of inner element to get
/// @param start_tag    [out] pointer to the tag where the value starts
///
/// The start_tag parameter will be set to the start of the tag in the
/// xml where the inner element of elementpath starts. To get the
/// value of that element, call @bxml_parse_value() and then, if that
/// succedes, call bxml_copy_element() to extract the actual value
///
/// @returns non-0 on error
///
int bxml_find_next_element(
                                bxml_parser_t *pxp,
                                const char *elementpath,
                                char pathdelim,
                                size_t index,
                                const char **start_tag
                                );

/// Find (a sequence of) xml elements by tag and point to it
/// Starting from the top of the xml in the parser
///
/// The element path is a sequence of nested tags with optional
/// indices. The tags do not have to correspond to exact nesting
/// as the xml being parse, just the same general nesting.
/// see @bxml_find_and_copy_nth_element() for examples.
///
/// @param pxp          [in] the xml parser
/// @param elementpath  [in] the element (sequence) to define
/// @param pathdelim    [in] the character that delimits the elementpath
/// @param index        [in] which of a series of inner element to get
/// @param tag_start    [out] pointer to the start of innter element's start tag
///
/// @returns non-0 on error
///
int bxml_find_element(
                        bxml_parser_t *pxp,
                        const char *elementpath,
                        const char pathdelim,
                        size_t index,
                        const char **tag_start
                     );

/// Find (a sequence of) xml elements by tag and copy its value
/// into a buffer. See @bxml_find_element() and @bml_copy_element()
///
/// The element path is a sequence of nested tags with optional
/// indices. The tags do not have to correspond to exact nesting
/// as the xml being parse, just the same general nesting.
/// see @bxml_find_and_copy_nth_element() for examples.
///
/// @param pxp          [in] the xml parser
/// @param elementpath  [in] the element (sequence) to define
/// @param pathdelim    [in] the character that delimits the elementpath
/// @param index        [in] which of a series of inner element to get
/// @param value        [in]  the pointer to the value of an element in pxp
/// @param value_length [in] the number of bytes in the value
/// @param keep_white   [in] set non-0 to preserve all white space in values
/// @param keep_entities[in] set non-0 to suppress expansion of entities in value copy
///
/// @returns non-0 on error
///
int bxml_find_and_copy_element(
                                bxml_parser_t *pxp,
                                const char *elementpath,
                                const char pathdelim,
                                size_t index,
                                char *value,
                                size_t value_len,
                                bool keep_white,
                                bool keep_entities
                             );

/// Find (a sequence of) xml elements by tag and point to it
/// Starting from the top of the xml in the parser
///
/// The element path is a sequence of nested tags with optional
/// indices. The tags do not have to correspond to exact nesting
/// as the xml being parse, just the same general nesting.
/// see @bxml_find_and_copy_nth_element() for examples.
///
/// @param xml          [in] the xml to parse
/// @param elementpath  [in] the element (sequence) to define
/// @param pathdelim    [in] the character that delimits the elementpath
/// @param index        [in] which of a series of inner element to get
/// @param tag_start    [out] pointer to inner start tag of element path in xml
/// @param element      [out] pointer to the value of the element (if found)
/// @param element_length [out] length of element in bytes
///
/// @returns non-0 on error
///
int bxml_find_nth_element(
                                const char *xml,
                                const char *elementpath,
                                char pathdelim,
                                size_t index,
                                const char **tag_start,
                                const char **element,
                                size_t *element_length
                                );

/// Find (a sequence of) xml elements by tag and copy the value
/// contained in the inner element into the element parameter
///
/// The element path is a sequence of nested tags with optional
/// indices. The tags do not have to correspond to exact nesting
/// as the xml being parse, just the same general nesting
///
/// @param xml          [in] the xml to parse
/// @param elementpath  [in] the element (sequence) to define
/// @param pathdelim    [in] the character that delimits the elementpath
/// @param index        [in] which of a series of inner element to get
/// @param element      [out] the value of the element (if found)
/// @param nelement     [in] the size, in bytes, in element
/// @param keep_white    [in] set non-0 to preserve all white space in values
/// @param keep_entities [in] set non-0 to suppress expansion of entities in value copy
///
/// Examples: for XML of
///
/// <aaa>
///  <bbb>
///   <ccc>
///    <ddd>
///     value1
///    </ddd>
///    <ddd>
///     value2
///    </ddd>
///   </ccc>
///   <ccc>
///    <ddd>
///     value3
///    </ddd>
///    <ddd>
///     value4
///     and more
///    </ddd>
///   </ccc>
///  </bbb>
/// </aaa>
///
/// These element paths are equivalent where "." is the pathdelim
///
///   "aaa.bbb.ccc.ddd"
///   "bbb.ccc.ddd"
///   "ddd"
///   "aaa.ddd"
///
///  The inner value can be indexed using the element path like:
///
///    "ccc.ddd[1]"     - second value of first ccc - "value2"
///    "ccc[1].ddd[1]"  - second value of second ccc - "value4andmore"
///
///  Or using 1 as the index in the call and not using the [1] at
///  the end of the element path. You can not specify both a non-0
///  index parameter and a non-0 index at the end of the element path
///
/// @returns non-0 on errors
///
int bxml_find_and_copy_nth_element(
                                const char *xml,
                                const char *elementpath,
                                const char pathdelim,
                                size_t index,
                                char *element,
                                size_t nelement,
                                bool keep_white,
                                bool keep_entities
                                );

/// Create an XML parser (reader)
///
/// @param shell        [in] a context to (re)use if not wanting to allocate
/// @param xml          [in] the xml to parse. Can not be NULL
///
/// @returns a possibly allocated xml parser object, or NULL on error (bad xml)
///
bxml_parser_t *bxml_parser_create(
                                bxml_parser_t *shell,
                                const char *xml
                                );

/// Free an XML parser created with @bxml_parser_create()
/// If the parser shell was passed in to the create call
/// it will not be freed in thia function, and you don't
/// need to call this.  If you did not pass the shell in
/// to the call, them you do need to call this
///
/// @param pxp [in] pointer to xml parser to free
///
/// @returns non-0 on error (no such parser)
///
int bxml_parser_destroy(
                                bxml_parser_t *pxp
                                );

#endif

