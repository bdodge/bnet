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
#ifndef BIPPATTR_H
#define BIPPATTR_H 1

#include "bnetheaders.h"
#include "bippproto.h"

/// Special attribute record index values
//
#define IPP_RECDEX_TAG (0x70000000) ///< attribute is really a tag

/// IPP data types. This is for internal representation only. It would be
/// nice to use the same types as IPP but they are haphazard and can't
/// carry the extra meaning (bit fields) we use here which is done to
/// save (considerable) space in the large tables generated for attrs.
//
typedef enum
{
    IPP_NOTYPE      = 0x00, // meta-type
    IPP_INTEGER     = 0x03,
    IPP_BOOLEAN     = 0x04,
    IPP_ENUM        = 0x05,

    IPP_STRINGTYPE  = 0x20, // meta-type, bit indicates type is a string
    IPP_TEXT        = 0x21,
    IPP_CHARSET     = 0x22,
    IPP_LANGUAGE    = 0x23,
    IPP_KEYWORD     = 0x24,
    IPP_NAME        = 0x25,
    IPP_OCTETSTRING = 0x26,
    IPP_URI         = 0x27,
    IPP_URISCHEME   = 0x28,
    IPP_MIME        = 0x29,

    IPP_COMPOSITE   = 0x40, // meta-type bit indicates composite type
    IPP_RESOLUTION  = 0x41,
    IPP_RANGEOFINT  = 0x42,
    IPP_DATETIME    = 0x43,
    IPP_COLLECTION  = 0x44,

    IPP_ARRAY       = 0x80, // 1setOf, bit indicates array-of-type
}
ipp_syntax_t;

/// grouping codes. used for attribute table, not part of protocol
//
typedef enum
{
    IPP_GROUPING_DOCUMENT_DESCRIPTION = 0,
    IPP_GROUPING_DOCUMENT_STATUS,
    IPP_GROUPING_DOCUMENT_TEMPLATE,
    IPP_GROUPING_EVENT_NOTIFICATIONS,
    IPP_GROUPING_JOB_DESCRIPTION,
    IPP_GROUPING_JOB_STATUS,
    IPP_GROUPING_JOB_TEMPLATE,
    IPP_GROUPING_OPERATION,
    IPP_GROUPING_PRINTER_DESCRIPTION,
    IPP_GROUPING_PRINTER_STATUS,
    IPP_GROUPING_SUBSCRIPTION_STATUS,
    IPP_GROUPING_SUBSCRIPTION_TEMPLATE,
    IPP_GROUPING_MAX_GROUP
}
ipp_attr_grouping_code_t;

/// this stores one ipp syntax (type) enum, in a byte, to save space
//
typedef uint8_t ipp_syntax_enc_t;

/// some attributes can have one of a number of possible types
/// so we store all allowed types in an array
//
#define IPP_MAX_ALT_TYPES (4)

/// Allocating memori

/// IPP Attribute record. Describes a single attribute properties (not value)
//
typedef struct tag_attr_record
{
    const char             *name;       ///< attribute name, like "printer-uri-supported"
    ipp_syntax_enc_t        syntax[IPP_MAX_ALT_TYPES];     ///< attribute type(s)
    int32_t                 minval;     ///< smalled size (string) or value (int) attribute can take
    int32_t                 maxval;     ///< largest size (string) or value (int) attribute can take
}
ipp_attr_rec_t;

/// IPP Attribute. Hold value(s) for a single attribute
//
typedef struct tag_attr
{
    size_t                  recdex;     ///< index into attributes describing this one
    size_t                  value_len;  ///< length of encoded attribute (bytes)
    size_t                  alloc_len;  ///< bytes allocated for encoded store
    uint8_t                *value;      ///< encoded attribute value(s)
    struct tag_attr        *next;       ///< for listing, in general
}
ipp_attr_t;

/// Attribute value iterator context
//
typedef struct tag_attr_iter
{
    ipp_attr_t             *attr;       ///< iterator on this attribute
    size_t                  val_count;  ///< which value iteration is on
    uint8_t                *val_ptr;    ///< where in value
}
ipp_attr_iter_t;

int         ipp_find_attr_rec   (const char *name, size_t *pindex, ipp_attr_rec_t **pattr);
int         ipp_get_attr_rec    (ipp_attr_t *attr, ipp_attr_rec_t **pattrec);
const char *ipp_name_of_attr    (ipp_attr_t *attr);

int ipp_syntax_for_enc_type     (ipp_syntax_enc_t enctag[IPP_MAX_ALT_TYPES], ipp_tag_t *tag, bool *is_array);
int ipp_base_enc_syntax_for_attr(ipp_attr_t *attr, ipp_syntax_enc_t *penc);

int ipp_set_attr_value          (ipp_attr_t *attr, const uint8_t *value, size_t value_len);
int ipp_add_attr_value          (ipp_attr_t *attr, const uint8_t *value, size_t value_len);

int ipp_open_attr_value         (ipp_attr_t *attr, ipp_attr_iter_t **iter);
int ipp_close_attr_value        (ipp_attr_iter_t *iter);
int ipp_get_next_attr_value     (ipp_attr_t *attr, ipp_attr_iter_t *iter, uint8_t **value, size_t *value_len);
int ipp_get_next_attr_bool_value(ipp_attr_t *attr, ipp_attr_iter_t *iter, int *value);
int ipp_get_next_attr_int32_value(ipp_attr_t *attr, ipp_attr_iter_t *iter, int32_t *value);
int ipp_get_next_attr_string_value(ipp_attr_t *attr, ipp_attr_iter_t *iter, char *value, size_t nvalue);

int ipp_get_only_attr_value     (ipp_attr_t *attr, uint8_t **value, size_t *value_len);
int ipp_get_only_attr_bool_value(ipp_attr_t *attr, int *value);
int ipp_get_only_attr_int32_value(ipp_attr_t *attr, int32_t *value);
int ipp_get_only_attr_string_value(ipp_attr_t *attr, char *value, size_t nvalue);

int ipp_get_attr_for_grouping   (ipp_attr_grouping_code_t grouping, ipp_attr_t **pattrs);
int ipp_dupe_attr_grouping      (ipp_attr_grouping_code_t grouping, ipp_attr_t **pattrs);
int ipp_dupe_attr_list          (ipp_attr_t *attrlist, ipp_attr_t **pdupeattrs);

int ipp_get_group_attr_by_index (const size_t recdex, ipp_attr_grouping_code_t group, ipp_attr_t **pattr);
int ipp_get_group_attr_by_name  (const char *name, ipp_attr_grouping_code_t group, ipp_attr_t **pattr);

int ipp_set_group_attr_bool_value(
                                const char *name,
                                ipp_attr_grouping_code_t group,
                                size_t nvalues,
                                ...
                                /* parm list of type "bool val, ..." */
                                );
int ipp_set_group_attr_range_value(
                                const char *name,
                                ipp_attr_grouping_code_t group,
                                size_t nvalues,
                                ...
                                /* parm list of type "int32_t minval, int32_t maxval, ..." */
                                );
int ipp_set_group_attr_int32_value(
                                const char *name,
                                ipp_attr_grouping_code_t group,
                                size_t nvalues,
                                ...
                                /* parm list of type "int32_t val, ..." */
                                );
int ipp_set_group_attr_bytes_value(
                                const char *name,
                                ipp_attr_grouping_code_t group,
                                size_t nvalues,
                                ...
                                /* parm list of type "const uint8_t *value, size_t value_len, ..." */
                                );
int ipp_set_group_attr_string_value(
                                const char *name,
                                ipp_attr_grouping_code_t group,
                                int nstrings,
                                ...
                                /* parm list of type "const char *str, ..." */
                                );

int ipp_get_attr_by_name        (const char *name, ipp_attr_t *attrlist, ipp_attr_t **pattr);

int ipp_set_attr_attr_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                ipp_attr_t *vattr
                                );
int ipp_set_attr_bool_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                size_t nvalues,
                                ...
                                /* parm list of type "bool val, ..." */
                                );
int ipp_set_attr_range_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                size_t nvalues,
                                ...
                                /* parm list of type "int32_t minval, int32_t maxval, ..." */
                                );
int ipp_set_attr_int32_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                size_t nvalues,
                                ...
                                /* parm list of type "int32_t val, ..." */
                                );
int ipp_set_attr_bytes_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                size_t nvalues,
                                ...
                                /* parm list of type "const uint8_t *value, size_t value_len, ..." */
                                );
int ipp_set_attr_string_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                int nstrings,
                                ...
                                /* parm list of type "const char *str, ..." */
                                );

int         ipp_dupe_attr       (ipp_attr_t *attr, ipp_attr_t **dupeattr);
ipp_attr_t *ipp_create_attr     (size_t recdex, uint8_t *value, size_t value_len);
int         ipp_destroy_attr    (ipp_attr_t *attr);
int         ipp_destroy_attrlist(ipp_attr_t *attrlist);

int         ipp_attr_init       (void);

int test_find_attr_rec(void);
int test_set_get_string_attr(void);
int test_find_xref_rec(void);

#endif

