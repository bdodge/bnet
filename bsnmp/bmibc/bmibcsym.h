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
#ifndef BMIBCSYM_H
#define BMIBCSYM_H 1

#include "bnetheaders.h"
#include "bsnmp.h"

struct tag_mib_context;

struct tag_asn_enum_type;
struct tag_asn_range;
struct tag_asn_typespec;
struct tag_asn_defval;
struct tag_asn_indexval;
struct tag_mib_symbol;
struct tag_mib_node;

// generic list of ASN types, used for enumerations,
//
typedef struct tag_asn_enum_type
{
    int                         f_val;
    int                         f_out;
    bool                        f_nostring;
    struct tag_mib_symbol      *f_sym;
    struct tag_asn_enum_type   *f_next;
    struct tag_asn_enum_type   *f_nextalloc;
}
bmib_enum_t;

// generic list of ASN ranges
//
typedef struct tag_asn_range
{
    int                         f_lowval;
    int                         f_highval;
    struct tag_asn_range       *f_next;
    struct tag_asn_range       *f_nextalloc;
}
bmib_range_t;

// ASN type spec
//
typedef struct tag_asn_typespec
{
    bmib_keyword_t              f_kwtype;
    char                       *f_desc;
    unsigned int                f_typemods;
    struct tag_mib_symbol      *f_sym;
    struct tag_asn_typespec    *f_basetype;
    struct tag_asn_typespec    *f_sibling;
    struct tag_asn_enum_type   *f_enums;
    struct tag_asn_range       *f_ranges;
    struct tag_asn_indexval    *f_indices;
}
bmib_typespec_t, bmib_group_t;

// generic list of default values
//
typedef struct tag_asn_defval
{
    char                       *f_text;
    struct tag_asn_defval      *f_next;
    struct tag_asn_defval      *f_nextalloc;
}
bmib_defval_t;

// generic list of index names to resolve post-parse
//
typedef struct tag_asn_indexval
{
    char                       *f_text;
    int                         f_ordinal;
    struct tag_mib_node        *f_node;
    struct tag_asn_indexval    *f_next;
    struct tag_asn_indexval    *f_nextalloc;
}
bmib_indexval_t;

// access to object
//
typedef enum
{
    maNone, maReadOnly = 1, maReadWrite = 3, maReadCreate = 5, maTrap = 8
}
bmib_access_t;

// type modifiers
//
#define  STRING_OF      0x0001
#define  SEQUENCE_OF    0x0002

typedef enum
{
    bstNumber, bstString, bstObject, bstType, bstEnum, bstMacro
}
bmib_symtype_t;

// wrapper for one MIB tree entry (object)
//
typedef struct tag_mib_node
{
    boid_t                      f_oid;
    struct tag_mib_symbol      *f_sym;
    struct tag_asn_typespec    *f_datatype;
    bmib_access_t               f_access;
    char                       *f_desc;
    bmib_enum_t                *f_enums;
    bmib_range_t               *f_ranges;
    bmib_defval_t              *f_defvals;
    bmib_indexval_t            *f_indices;
    int                         f_typemods;
    int                         f_offset;
    bool                        f_volatile;
    struct tag_mib_node        *f_parent;
    struct tag_mib_node        *f_child;
    struct tag_mib_node        *f_sibling;
}
bmib_node_t;

// binary tree symbol table
//
typedef struct tag_mib_symbol
{
    char                       *f_name;
    bmib_symtype_t              f_type;
    union
    {
        char               *u_sv;
        uint32_t            u_iv;
        bmib_node_t        *u_ov;
        bmib_typespec_t    *u_tv;
        bmib_enum_t        *u_ev;
    }                           f_value;

    struct tag_mib_symbol      *f_left;
    struct tag_mib_symbol      *f_right;
}
bmib_symbol_t;

bmib_enum_t        *bmib_create_enum        (
                                            struct tag_mib_context *parser,
                                            bmib_symbol_t *sym,
                                            int val,
                                            bmib_enum_t *sibling
                                            );
void                bmib_destroy_enum       (
                                            struct tag_mib_context *parser,
                                            bmib_enum_t *enumtype
                                            );

bmib_range_t       *bmib_create_range       (
                                            struct tag_mib_context *parser,
                                            int lowval,
                                            int highval,
                                            bmib_range_t *sibling
                                            );
void                bmib_destroy_range      (
                                            struct tag_mib_context *parser,
                                            bmib_range_t *range
                                            );

bmib_typespec_t    *bmib_create_type        (
                                            struct tag_mib_context *parser,
                                            bmib_symbol_t *sym,
                                            bmib_keyword_t kwtype,
                                            bmib_typespec_t *sibling,
                                            bmib_typespec_t *basetype
                                            );
void                bmib_destroy_type       (
                                            struct tag_mib_context *parser,
                                            bmib_typespec_t *type
                                            );
void                bmib_destroy_typelist   (
                                            struct tag_mib_context *parser,
                                            bmib_typespec_t *typelist
                                            );

bmib_defval_t      *bmib_create_defval      (
                                            struct tag_mib_context *parser,
                                            const char *name
                                            );
void                bmib_destroy_defval     (
                                            struct tag_mib_context *parser,
                                            bmib_defval_t *defval
                                            );

bmib_indexval_t    *bmib_create_index       (
                                            struct tag_mib_context *parser,
                                            const char *name,
                                            int dex
                                            );
void                bmib_destroy_index      (
                                            struct tag_mib_context *parser,
                                            bmib_indexval_t *indexval
                                            );

bmib_node_t        *bmib_create_node        (
                                            struct tag_mib_context *parser,
                                            struct tag_mib_symbol *sym,
                                            boid_t oid,
                                            struct tag_mib_node *parent
                                            );
void                bmib_destroy_node       (
                                            struct tag_mib_context *parser,
                                            bmib_node_t *node
                                            );

bmib_symbol_t      *bmib_create_char_symbol (
                                            struct tag_mib_context *parser,
                                            const char *name,
                                            const char *value
                                            );
bmib_symbol_t      *bmib_create_int_symbol  (
                                            struct tag_mib_context *parser,
                                            const char *name,
                                            int value
                                            );
bmib_symbol_t      *bmib_create_node_symbol (
                                            struct tag_mib_context *parser,
                                            const char *name,
                                            bmib_node_t *node
                                            );
void                bmib_destroy_symbol     (
                                            struct tag_mib_context *parser,
                                            bmib_symbol_t *sym
                                            );

bmib_symbol_t      *bmib_add_symbol         (
                                            struct tag_mib_context *parser,
                                            bmib_symbol_t *sym,
                                            bmib_symbol_t** tree
                                            );
void                bmib_dump_symtree       (
                                            bmib_symbol_t *tree
                                            );
void                bmib_destroy_symtree    (
                                            struct tag_mib_context *parser,
                                            bmib_symbol_t *sym
                                            );
bmib_symbol_t      *bmib_find_symbol        (
                                            const char *name,
                                            bmib_symbol_t *tree
                                            );

void                bmib_destroy_nodetree   (
                                            struct tag_mib_context *parser,
                                            bmib_node_t *parent
                                            );
bmib_node_t        *bmib_add_node           (
                                            struct tag_mib_context *parser,
                                            boid_t *oid,
                                            int oidlen,
                                            bmib_node_t *node,
                                            bmib_node_t **nodetree
                                            );
bmib_node_t        *bmib_find_node          (
                                            boid_t *oid,
                                            int oidlen,
                                            bmib_node_t *tree
                                            );

#endif

