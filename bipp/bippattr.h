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

/// IPP data types. This is for internal representation only. It would be
/// nice to use the same types as IPP but they are haphazard and can't
/// carry the extra meaning (bit fields) we use here which is done to
/// save (considerable) space in the large tables generated for attrs.
//
typedef enum
{
    IPP_NOTYPE      = 0x00, // meta-type
    IPP_CHARSET     = 0x01,
    IPP_LANGUAGE    = 0x02,
    IPP_INTEGER     = 0x03,
    IPP_BOOLEAN     = 0x04,
    IPP_ENUM        = 0x05,
    IPP_KEYWORD     = 0x06,

    IPP_STRINGTYPE  = 0x20, // meta-type, bit indicates type is a string
    IPP_TEXT        = 0x21,
    IPP_NAME        = 0x22,
    IPP_OCTETSTRING = 0x23,
    IPP_URI         = 0x24,
    IPP_URISCHEME   = 0x25,
    IPP_MIME        = 0x26,

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
}
ipp_attr_grouping_code_t;

/// this stores one ipp syntax (type) enum, in a byte, to save space
//
typedef uint8_t ipp_syntax_enc_t;

/// some attributes can have one of a number of possible types
/// so we store all allowed types in an array
//
#define IPP_MAX_ALT_TYPES (4)

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
    size_t                  len;        ///< length of encoded attribute (bytes)
    size_t                  alloc_len;  ///< bytes allocated for encoded store
    uint8_t                *val;        ///< encoded attribute value(s)
    struct tag_attr        *next;       ///< for listing, in general
}
ipp_attr_t;

#endif

