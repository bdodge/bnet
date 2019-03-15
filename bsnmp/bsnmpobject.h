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
#ifndef BSNMPOBJECT_H
#define BSNMPOBJECT_H 1

#include "bnetheaders.h"
#include "bsnmp.h"

// max expected simensionality (index count)
//
#define BSNMP_MAX_DIMENSIONS 3

// whether object names are included in xref
#ifndef BSNMP_OBJECT_NAMES
#define BSNMP_OBJECT_NAMES 1
#endif

#ifndef _BMIBC_GENERATED_H

/// Types pointable to in the generated records
//
typedef enum {
    BMIBC_VOID,
    BMIBC_ENUM,
    BMIBC_INT,
    BMIBC_INT32,
    BMIBC_UINT32,
    BMIBC_INT64,
    BMIBC_UINT64,
    BMIBC_OCTET,
    BMIBC_OBJECT
} bmibc_value_type_t;

#define BMIBC_VALUE_TYPE_STR \
"typedef enum {\n"  \
"    BMIBC_VOID,\n"  \
"    BMIBC_ENUM,\n"  \
"    BMIBC_INT,\n"  \
"    BMIBC_INT32,\n"  \
"    BMIBC_UINT32,\n"  \
"    BMIBC_INT64,\n"  \
"    BMIBC_UINT64,\n"  \
"    BMIBC_OCTET,\n"  \
"    BMIBC_OBJECT\n"  \
"} bmibc_value_type_t;\n"

/// How to access vars in a record
//
typedef enum {
    BMIBC_DIRECT,
    BMIBC_FUNCTION,
    BMIBC_GLOBAL,
} bmibc_access_method_t;

#define BMIBC_ACCESS_METHOD_STR \
"typedef enum {\n"  \
"    BMIBC_DIRECT,\n"  \
"    BMIBC_FUNCTION,\n"  \
"    BMIBC_GLOBAL,\n"  \
"} bmibc_access_method_t;\n"

/// Access permissions of a record (bitfield)
//
typedef enum {
    BMIBC_NONE = 0,
    BMIBC_READ = 1,
    BMIBC_WRITE = 2,
    BMIBC_READWRITE = 3,
    BMIBC_PASSWORD = 4
} bmibc_access_perm_t;

#define BMIBC_ACCESS_PERM_STR \
"typedef enum {\n"  \
"    BMIBC_NONE = 0,\n"  \
"    BMIBC_READ = 1,\n"  \
"    BMIBC_WRITE = 2,\n"  \
"    BMIBC_READWRITE = 3,\n"  \
"    BMIBC_PASSWORD = 4\n"  \
"} bmibc_access_perm_t;\n"  \

/// An object record: descriptor and
///  backing store pointers/value
//
typedef struct {
    #if BSNMP_OBJECT_NAMES
    const char *name;
    #endif
    int minv;
    int maxv;
    int dim;
    size_t offset;
    size_t bits;
    bmibc_value_type_t type;
    bmibc_access_method_t method;
    bmibc_access_perm_t access;
    void *subs;
    void *factory_value;
    void *value;
} bmibc_record_t;

#if BSNMP_OBJECT_NAMES
#define BMIBC_OBJECT_RECORD_STR \
"typedef struct {\n"  \
"    const char *name;\n"  \
"    int minv;\n"  \
"    int maxv;\n"  \
"    int dim;\n"  \
"    size_t offset;\n"  \
"    size_t bits;\n"  \
"    bmibc_value_type_t type;\n"  \
"    bmibc_access_method_t method;\n"  \
"    bmibc_access_perm_t access;\n"  \
"    void *subs;\n"  \
"    void *factory_value;\n"  \
"    void *value;\n"  \
"} bmibc_record_t;\n"
#else
"typedef struct {\n"  \
"    int minv;\n"  \
"    int maxv;\n"  \
"    int dim;\n"  \
"    size_t offset;\n"  \
"    size_t bits;\n"  \
"    bmibc_value_type_t type;\n"  \
"    bmibc_access_method_t method;\n"  \
"    bmibc_access_perm_t access;\n"  \
"    void *subs;\n"  \
"    void *factory_value;\n"  \
"    void *value;\n"  \
"} bmibc_record_t;\n"
#endif

typedef struct {
    const char *oidstr;
    size_t record_index;
    uint8_t asntype;
    size_t indices[BSNMP_MAX_DIMENSIONS];
} bmibc_oid_xref_t;

#define BMIBC_OID_XREF_STR \
"typedef struct {\n"            \
"    const char *oidstr;\n"     \
"    size_t record_index;\n"    \
"    uint8_t asntype;\n"        \
"    size_t indices[BSNMP_MAX_DIMENSIONS];\n"  \
"} bmibc_oid_xref_t;\n"

#endif // _BMIBC_GENERATED

int bsnmp_get_object_dimensionality (
                                    size_t rec,
                                    size_t *ndim,
                                    size_t *ntotal,
                                    size_t *indices
                                    );
int bsnmp_find_record               (
                                    bsnmp_oid_t *oid,
                                    bool exact,
                                    size_t *recdex
                                    );
int bsnmp_get_object_value          (
                                    bsnmp_oid_t *oid,
                                    bsnmp_var_t *var,
                                    bsnmp_errcode_t *err
                                    );
int bsnmp_get_next_object_value     (
                                    bsnmp_oid_t *oid,
                                    bsnmp_var_t *var,
                                    bsnmp_errcode_t *err
                                    );
int bsnmp_set_object_value          (
                                    bsnmp_var_t *var,
                                    bsnmp_errcode_t *err
                                    );
int bsnmp_set_string_value          (
                                    const char *oidstr,
                                    const char *str,
                                    bsnmp_errcode_t *err
                                    );
int bsnmp_set_uint_value            (
                                    const char *oidstr,
                                    uint32_t uval,
                                    bsnmp_errcode_t *err
                                    );
int bsnmp_set_int_value             (
                                    const char *oidstr,
                                    int32_t ival,
                                    bsnmp_errcode_t *err
                                    );
int bsnmp_init_objects              (
                                    bmibc_record_t *records,
                                    const bmibc_oid_xref_t *xrefs,
                                    size_t num_records
                                    );

#endif
