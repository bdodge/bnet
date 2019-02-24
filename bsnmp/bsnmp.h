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
#ifndef BSNMP_H
#define BSNMP_H 1

#include "bsnmpconfig.h"
#include "bstreamio.h"
#include "bnetheaders.h"

#define SNMP_ERROR BERROR

// Notes: BER = ASN.1 Basic Encoding Rules defined int  ITU-TX.690
//              https://www.itu.int/ITU-T/studygroups/com17/languages/X.690-0207.pdf
//

/// Supported SNMP Versions
//
#define SNMP_V1     (0)
#define SNMP_V2C    (1)
#define SNMP_V3     (2)

/// ASN.1 Type Tags in BER per X.208
//
#define asnUNIVERSAL        (0x0)
#define asnBOOLEAN          (0x1)
#define asnINTEGER          (0x2)
#define asnBIT_STRING       (0x3)
#define asnOCTET_STRING     (0x4)
#define asnNULL             (0x5)
#define asnOBJECT_ID        (0x6)
#define asnSEQUENCE         (0x10)
#define asnSET              (0x11)
#define asnPRINTABLE_STR    (0x13)
#define asnUTC_TIME         (0x17)

/// ASN.1 Type Class in BER per X.690
//
#define asnEXTENSION_ID     (0x1F)
#define asnCONSTRUCTOR      (0x20)
#define asnSTRUCT           (0x30)
#define asnAPPLICATION      (0x40)
#define asnCONTEXT_SPECIFIC (0x80)
#define asnPRIVATE          (0xC0)

typedef enum
{
    /// SNMP Basic Types per ASN.1 BER
    //
    SNMP_BOOLEAN        = (asnBOOLEAN),
    SNMP_INTEGER        = (asnINTEGER),
    SNMP_BIT_STRING     = (asnBIT_STRING),
    SNMP_OCTET_STRING   = (asnOCTET_STRING),
    SNMP_NULL           = (asnNULL),
    SNMP_OBJECT_ID      = (asnOBJECT_ID),
    SNMP_SEQUENCE       = (asnSEQUENCE),
    SNMP_SETOF          = (asnSET),
    SNMP_PRINTABLE_STR  = (asnPRINTABLE_STR),
    SNMP_UTC_TIME       = (asnUTC_TIME),

    /// SNMP Types as per RFC-1442, pre-formatted per BER
    //
    SNMP_IPADDRESS      = (asnAPPLICATION | 0x0),
    SNMP_COUNTER        = (asnAPPLICATION | 0x1),
    SNMP_GAUGE          = (asnAPPLICATION | 0x2),
    SNMP_UNSIGNED       = (asnAPPLICATION | 0x2),
    SNMP_TIMETICKS      = (asnAPPLICATION | 0x3),
    SNMP_OPAQUE         = (asnAPPLICATION | 0x4),
    SNMP_NSAPADDRESS    = (asnAPPLICATION | 0x5),
    SNMP_COUNTER64      = (asnAPPLICATION | 0x6),
    SNMP_UNSIGNED32     = (asnAPPLICATION | 0x7),
    SNMP_FLOAT          = (asnAPPLICATION | 0x8),
    SNMP_DOUBLE         = (asnAPPLICATION | 0x9),

    /// SNMP Types per RFC3781 = (Next gen SMI)
    //
    SNMP_INTEGER64      = (asnAPPLICATION | 0xA),
    SNMP_UNSIGNED64     = (asnAPPLICATION | 0xB),
    SNMP_FLOAT32        = (asnAPPLICATION | 0xC),
    SNMP_FLOAT64        = (asnAPPLICATION | 0xD),
    SNMP_FLOAT128       = (asnAPPLICATION | 0xE)
}
bsnmp_type_t;

#define SNMP_CONTEXT_SPEC   (asnCONTEXT_SPECIFIC)
#define SNMP_NOINSTANCE     (asnCONTEXT_SPECIFIC | 0x1)
#define SNMP_PRIVATE        (asnPRIVATE)


/// SNMP Operation codes per RFC-1442
//
typedef enum
{
    SNMP_GET            = 0xA0,
    SNMP_GETNEXT        = 0xA1,
    SNMP_RESPONSE       = 0xA2,
    SNMP_SET            = 0xA3,
    SNMP_TRAP           = 0xA4,
    SNMP_GETBULK        = 0xA5,
    SNMP_INFORM         = 0xA6,
    SNMP_NOTIFY         = 0xA7,
    SNMP_REPORT         = 0xA8
}
bsnmp_opcode_t;

/// SNMP Error codes from RFC 1422
//
typedef enum
{
    SNMP_ErrNoError             = 0,
    SNMP_OK                     = 0,
    SNMP_ErrTooBig              = 1,
    SNMP_ErrNoSuchName          = 2,
    SNMP_ErrBadValue            = 3,
    SNMP_ErrReadOnly            = 4,
    SNMP_ErrGenErr              = 5,
    SNMP_ErrNoAccess            = 6,
    SNMP_ErrWrongType           = 7,
    SNMP_ErrWrongLength         = 8,
    SNMP_ErrWrongEncoding       = 9,
    SNMP_ErrWrongValue          = 10,
    SNMP_ErrNoCreation          = 11,
    SNMP_ErrInconsistentValue   = 12,
    SNMP_ErrResourceUnavailable = 13,
    SNMP_ErrCommitFailed        = 14,
    SNMP_ErrUndoFailed          = 15,
    SNMP_ErrAuthorizationError  = 16,
    SNMP_ErrNotWritable         = 17,
    SNMP_ErrInconsistentName    = 18
}
bsnmp_errcode_t;

/// Type of an single oid entry (16 bits)
//
typedef uint16_t boid_t;

/// an OID
//
typedef struct tag_oid
{
    uint32_t    len;
    boid_t      oid[SNMP_MAX_OID];
}
bsnmp_oid_t;

/// variant type to hold snmp values
//
typedef struct tag_snmp_variant
{
	struct tag_snmp_variant *next;

    bsnmp_type_t type;
    bsnmp_oid_t  oid;
    uint32_t     len;
    uint32_t     alloc_len;

    union
    {
        int8_t      cVal;
        uint8_t     ucVal;
        int16_t     wVal;
        uint16_t    uwVal;
        int32_t     iVal;
        uint32_t    uVal;
        int64_t     llVal;
        uint64_t    ullVal;
        float       fVal;
        double      lfVal;

        char        *sVal;
        uint8_t     *bVal;
        bsnmp_oid_t *oVal;
    }
    val;
}
bsnmp_var_t;

/// Request context
//
typedef struct tag_snmp_request
{
    uint32_t            len;
    int32_t             version;
    char                community[SNMP_MAX_COMMUNITY];
    bsnmp_opcode_t      code;
    uint32_t            id;
    int32_t             non_repeaters;
    int32_t             max_repeating;
    bsnmp_errcode_t     errmsg;
    int32_t             errdex;
    bsnmp_var_t        *values;
}
bsnmp_request_t;

/// Callback to process requests
//
struct tag_snmp_server;
typedef int (*bsnmp_callback_t)(
                                struct tag_snmp_server *server,
                                bsnmp_request_t *req,
                                bsnmp_var_t *var
                               );

/// an SNMP server context
//
typedef struct tag_snmp_server
{
    uint32_t            version;
    char                community[SNMP_MAX_COMMUNITY];
    uint16_t            port;
    bsnmp_errcode_t     last_error;
    bsnmp_callback_t    callback;
    iostream_t         *stream;
    ioring_t            in;
    ioring_t            out;
	struct sockaddr_in  srcaddr;
}
bsnmp_server_t;


int bsnmp_server_init           (
                                bsnmp_server_t *server,
                                uint16_t version,
                                uint16_t port,
                                bsnmp_callback_t callback
                                );
int bsnmp_format_reply          (bsnmp_server_t *server, bsnmp_request_t *req, bsnmp_var_t *var);
int bsnmp_serve                 (bsnmp_server_t *server);
int bsnmp_server_cleanup        (bsnmp_server_t *server);

#endif

