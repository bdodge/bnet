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
#ifndef BMIBCPARSE_H
#define BMIBCPARSE_H 1

#define MAX_INCLS   16
#define MAX_MIB_TOKEN   256

#define MAX_OID_ELEMENT 65535

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define MAX_EVAL_LEVEL  32

struct tag_mib_context;

// callback function for parsing mib
//
typedef bool (*OnErrorFunc)(bmib_error_t code, const char* err);

// callback function for traversing tree
//
typedef bool (*OnNodeFunc)(struct tag_mib_context *ctx, bmib_node_t *tree, bmib_node_t *node, int level);

typedef enum {
    mpsError,
    mpsTop,
    mpsDefine,
    mpsBegin,
    mpsMIBL,
    mpsMIBV,
    mpsMIBA,
    mpsMIBR,
    mpsEnd
}
bmib_parse_state_t;

// mib parser context
//
typedef struct tag_mib_context
{
    FILE               *f_fp;

    bmib_node_t        *f_curnode;

    bmib_parse_state_t  f_state;

    char                f_fname[MAX_PATH];
    char                f_mibpath[MAX_PATH * MAX_INCLS];
    char                f_mibname[MAX_PATH];
    char                f_token[MAX_MIB_TOKEN];

    char                f_lval  [MAX_MIB_TOKEN];
    char                f_ltype [MAX_MIB_TOKEN];
    bmib_typespec_t    *f_objtype;
    bmib_access_t       f_access;
    char               *f_desc;
    int                 f_offset;
    bool                f_volatile;
    int                 f_typemods;
    bmib_defval_t      *f_defvals;
    bmib_indexval_t    *f_indices;
    bmib_enum_t        *f_enums;
    bmib_range_t       *f_ranges;
    char                f_pbc;
    bool                f_skipping[MAX_EVAL_LEVEL];
    int                 f_skiplev;
    int                 f_maxtok;
    int                 f_toklen;
    int                 f_lines;
    int                 f_errors;
    int                 f_warnings;
    bool                f_inerror;
    bool                f_defaultnv;

    bmib_node_t        *f_nodes;

    bmib_symbol_t      *f_keysyms;
    bmib_symbol_t      *f_nodesyms;
    bmib_symbol_t      *f_enumsyms;
    bmib_symbol_t      *f_typesyms;
    bmib_symbol_t      *f_macrosyms;
    bmib_symbol_t      *f_groupsyms;

    bmib_typespec_t    *f_types;
    bmib_group_t       *f_groups;
    bmib_typespec_t    *f_grouptypes;

    bmib_symbol_t      *f_keywords;

    bmib_enum_t        *f_enumallocs;
    bmib_range_t       *f_rangeallocs;
    bmib_indexval_t    *f_indexallocs;
    bmib_defval_t      *f_defvalallocs;

    int                 f_files;
    int                 f_level;
    bool                f_abort;
    bool                f_enx;
    OnErrorFunc         f_errfunc;

    char s_mibpath[MAX_PATH * MAX_INCLS];
}
bmib_context_t;

bmib_context_t *bmib_create_parser(void);
void            bmib_destroy_parser(bmib_context_t *parser);

int             bmib_parse_mib(
                                bmib_context_t *parser,
                                const char     *filename,
                                const char     *mibpath,
                                OnErrorFunc     errfunc,
                                bool            defaultnv
                              );


#endif

