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
#include "butil.h"
#include "bmibckeywords.h"
#include "bmibctoken.h"
#include "bmibcsym.h"
#include "bmibc.h"

static bool DefaultErrFunc(bmib_error_t code, const char* msg)
{
#ifdef Windows
    MessageBox(0, msg, "Unhandled BMIBC Error", MB_OK);
#else
    fprintf(stderr, "BMIBC - %s\n", msg);
#endif
    return false;
}

bmib_node_t     *bmib_get_objects       (bmib_context_t *parser)  { return parser->f_nodes;       }
bmib_symbol_t   *bmib_get_symbols       (bmib_context_t *parser)  { return parser->f_nodesyms;    }
bmib_symbol_t   *bmib_get_types         (bmib_context_t *parser)  { return parser->f_typesyms;    }
bmib_symbol_t   *bmib_get_groups        (bmib_context_t *parser)  { return parser->f_groupsyms;   }
bmib_symbol_t   *bmib_get_keywords      (bmib_context_t *parser)  { return parser->f_keysyms;     }
bmib_symbol_t   *bmib_get_macros        (bmib_context_t *parser)  { return parser->f_macrosyms;   }
bmib_typespec_t *bmib_get_mibtypes      (bmib_context_t *parser)  { return parser->f_types;       }
bmib_typespec_t *bmib_get_grouptypes    (bmib_context_t *parser)  { return parser->f_grouptypes;  }
bmib_group_t    *bmib_get_mibgroups     (bmib_context_t *parser)  { return parser->f_groups;      }

bmib_context_t *bmib_create_parser()
{
    bmib_context_t *parser;

    parser = (bmib_context_t *)malloc(sizeof(bmib_context_t));
    if (!  parser)
    {
        return NULL;
    }
    parser->f_fp = NULL;
    parser->f_pbc = 0;
    parser->f_toklen = 0;
    parser->f_maxtok = MAX_MIB_TOKEN;
    parser->f_lines = 0;
    parser->f_errors = 0;
    parser->f_warnings = 0;
    parser->f_objtype = NULL;
    parser->f_access = maNone;
    parser->f_typemods = 0;
    parser->f_enums = NULL;
    parser->f_defvals = NULL;
    parser->f_indices = NULL;
    parser->f_ranges = NULL;
    parser->f_skiplev = -1;
    parser->f_desc = NULL;
    parser->f_mibname[0] = '\0';
    parser->f_fname[0] = '\0';

    parser->f_nodes     = NULL;
    parser->f_nodesyms  = NULL;
    parser->f_typesyms  = NULL;
    parser->f_groupsyms = NULL;
    parser->f_enumsyms  = NULL;
    parser->f_macrosyms = NULL;
    parser->f_types     = NULL;
    parser->f_groups    = NULL;
    parser->f_grouptypes= NULL;
    parser->f_keywords  = NULL;

    parser->f_enumallocs    = NULL;
    parser->f_rangeallocs   = NULL;
    parser->f_indexallocs   = NULL;
    parser->f_defvalallocs  = NULL;

    parser->f_files     = 0;
    parser->f_level     = 0;
    parser->f_abort     = false;
    parser->f_enx       = true;
    parser->f_errfunc   = DefaultErrFunc;

    parser->f_mibpath[0] = '\0';

    return parser;
}

void bmib_destroy_parser(bmib_context_t *parser)
{
    bmib_destroy_nodetree(parser, parser->f_nodes);
    bmib_destroy_symtree(parser, parser->f_nodesyms);
    bmib_destroy_symtree(parser, parser->f_typesyms);
    bmib_destroy_symtree(parser, parser->f_groupsyms);
    bmib_destroy_symtree(parser, parser->f_enumsyms);
    bmib_destroy_symtree(parser, parser->f_macrosyms);
    bmib_destroy_typelist(parser, parser->f_types);
    bmib_destroy_typelist(parser, parser->f_groups);
    bmib_destroy_typelist(parser, parser->f_grouptypes);
    bmib_destroy_symtree(parser, parser->f_keysyms);

    bmib_enum_t *enumt;

    while (parser->f_enumallocs)
    {
        enumt = parser->f_enumallocs->f_nextalloc;
        bmib_destroy_enum(parser, parser->f_enumallocs);
        parser->f_enumallocs = enumt;
    }

    bmib_range_t *range;

    while (parser->f_rangeallocs)
    {
        range = parser->f_rangeallocs->f_nextalloc;
        bmib_destroy_range(parser, parser->f_rangeallocs);
        parser->f_rangeallocs = range;
    }

    bmib_defval_t *defval;

    while (parser->f_defvalallocs)
    {
        defval = parser->f_defvalallocs->f_nextalloc;
        bmib_destroy_defval(parser, parser->f_defvalallocs);
        parser->f_defvalallocs = defval;
    }

    bmib_indexval_t *indexval;

    while (parser->f_indexallocs)
    {
        indexval = parser->f_indexallocs->f_nextalloc;
        bmib_destroy_index(parser, parser->f_indexallocs);
        parser->f_indexallocs = indexval;
    }

    if (parser->f_desc)
    {
        free(parser->f_desc);
    }
    free(parser);
}

static bool bmib_dump_tree_node(bmib_context_t *parser, bmib_node_t *node, int level)
{
    char msg[256];
    char* pmsg;
    int   i, j, room;

    for (i = 0, pmsg = msg, room = 256; i < level; i++)
    {
        j = snprintf(pmsg, room, "    ");
        room -= j;
        pmsg += j;
    }
    if (! node || ! node->f_sym)
    {
        j = snprintf(pmsg, room, "Bad node\n");
        room -= j;
        pmsg += j;
    }
    else
    {
        j = snprintf(pmsg, room, "%3d %s\n", node->f_oid, node->f_sym->f_name);
        room -= j;
        pmsg += j;
    }
    parser->f_errfunc(meInfo, msg);
    return true;
}

void bmib_traverse_tree(bmib_context_t *parser, OnNodeFunc func, bmib_node_t *tree, bmib_node_t *branch, int level)
{

    if (! tree || ! branch)
    {
        return;
    }
    if (! func(parser, tree, branch, level))
    {
        return;
    }
    if (tree->f_child)
    {
        bmib_traverse_tree(parser, func, tree, branch->f_child, level+1);
    }
    if (tree->f_sibling)
    {
        bmib_traverse_tree(parser, func, tree, branch->f_sibling, level);
    }
}

static void bmib_load_keywords(bmib_context_t *parser)
{
    bmib_symbol_t *sym;

    // load keyword symbol table
    //
    bmib_keyword_map_t *pKey = g_mib_keywords;

    for (; pKey->f_name; pKey++)
    {
        sym = bmib_create_int_symbol(parser, pKey->f_name, pKey->f_value);
        if (! sym)
        {
            bmib_error_memory(parser, "Keyword");
            return;
        }
        bmib_add_symbol(parser, sym, &parser->f_keysyms);
    }
    // load the builtin type symbol table
    //
    bmib_builtin_type_t *pType = g_mib_types;

    for (; pType->f_name; pType++)
    {
        sym = bmib_create_int_symbol(parser, pType->f_name, 0);
        if (! sym)
        {
            bmib_error_memory(parser, "Keyword");
            return;
        }
        sym->f_type = bstType;
        sym->f_value.u_tv = bmib_create_type(parser, sym, pType->f_kw, parser->f_types, NULL);
        if (! sym->f_value.u_tv)
        {
            bmib_error_memory(parser, "Type");
            break;
        }
        parser->f_types = sym->f_value.u_tv;
        bmib_add_symbol(parser, sym, &parser->f_typesyms);
    }
    // set string type mod into builtin string types now that OCTET is in the table
    //
    for (pType = g_mib_types; pType->f_name; pType++)
    {
        switch (pType->f_kw)
        {
        case kwOBJECT:
            sym = bmib_find_symbol(pType->f_name, parser->f_typesyms);
            if (sym)
            {
                sym->f_value.u_tv->f_basetype = (bmib_find_symbol("OCTET", parser->f_typesyms))->f_value.u_tv;
                sym->f_value.u_tv->f_ranges = bmib_create_range(parser, 0, 255, NULL);
                if (! sym->f_value.u_tv->f_ranges)
                {
                    bmib_error_memory(parser, "Range");
                    break;
                }
                sym->f_value.u_tv->f_typemods = STRING_OF;
            }
            break;
       case kwIPADDRESS:
            sym = bmib_find_symbol(pType->f_name, parser->f_typesyms);
            if (sym)
            {
                sym->f_value.u_tv->f_basetype = (bmib_find_symbol("OCTET", parser->f_typesyms))->f_value.u_tv;
                sym->f_value.u_tv->f_ranges = bmib_create_range(parser, 4, 4, NULL);
                if (! sym->f_value.u_tv->f_ranges)
                {
                    bmib_error_memory(parser, "Range");
                    break;
                }
                sym->f_value.u_tv->f_typemods = STRING_OF;
            }
            break;
        default:
            break;
        }
    }
    // load the builtin macros table
    //
    bmib_builtin_macro_t *pMacro = g_mib_macros;

    for (; pMacro->f_name; pMacro++)
    {
        sym = bmib_create_int_symbol(parser, pMacro->f_name, 0);
        if (! sym)
        {
            bmib_error_memory(parser, "Keyword Object");
            return;
        }
        if (! sym)
        {
            bmib_error_memory(parser, "Keyword");
            return;
        }
        sym->f_type = bstMacro;
        bmib_add_symbol(parser, sym, &parser->f_macrosyms);
    }
    // load the builtin object (iso) as the first node
    //
    boid_t oidISO[2] = { 1, 0 };

    bmib_add_node(parser, oidISO, 1, NULL, &parser->f_nodes);
    if (! parser->f_nodes)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    sym = bmib_create_node_symbol(parser, "iso", parser->f_nodes);
    if (! sym)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    parser->f_nodes->f_sym = sym;
    bmib_add_symbol(parser, sym, &parser->f_nodesyms);

#ifndef NO_PREDEF_NODES

    bmib_node_t *node = parser->f_nodes;

    // load the basic iso trees
    //
    boid_t oidORG[2] = { 3, 0 };

    node->f_child  = NULL;
    node   = bmib_add_node(parser, oidORG, 1, node, &node->f_child);
    if (! node)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    sym     = bmib_create_node_symbol(parser, "org", node);
    if (! sym)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    node->f_sym = sym;
    bmib_add_symbol(parser, sym, &parser->f_nodesyms);

    boid_t oidDOD[2] = { 6, 0 };

    node->f_child  = NULL;
    node   = bmib_add_node(parser, oidDOD, 1, node, &node->f_child);
    if (! node)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    sym     = bmib_create_node_symbol(parser, "dod", node);
    if (! sym)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    node->f_sym = sym;
    bmib_add_symbol(parser, sym, &parser->f_nodesyms);

    boid_t oidINT[2] = { 1, 0 };

    node->f_child  = NULL;
    node   = bmib_add_node(parser, oidINT, 1, node, &node->f_child);
    if (! node)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    sym     = bmib_create_node_symbol(parser, "internet", node);
    if (! sym)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    node->f_sym = sym;
    bmib_add_symbol(parser, sym, &parser->f_nodesyms);

    boid_t oidMGM[2] = { 2, 0 };

    node->f_child  = NULL;
    node   = bmib_add_node(parser, oidMGM, 1, node, &node->f_child);
    if (! node)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    sym     = bmib_create_node_symbol(parser, "mgmt", node);
    if (! sym)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    node->f_sym = sym;
    bmib_add_symbol(parser, sym, &parser->f_nodesyms);

    boid_t oidMIB2[2] = { 1, 0 };

    node->f_child  = NULL;
    node   = bmib_add_node(parser, oidMIB2, 1, node, &node->f_child);
    if (! node)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    sym     = bmib_create_node_symbol(parser, "mib-2", node);
    if (! sym)
    {
        bmib_error_memory(parser, "Keyword Object");
        return;
    }
    node->f_sym = sym;
    bmib_add_symbol(parser, sym, &parser->f_nodesyms);

#endif

}

static void bmib_restart_object(bmib_context_t *parser)
{
    strcpy(parser->f_lval, parser->f_token);
    parser->f_objtype  = NULL;
    parser->f_access   = maNone;
    parser->f_typemods = 0;
    parser->f_enums    = NULL;
    parser->f_ranges   = NULL;
    parser->f_defvals  = NULL;
    parser->f_indices  = NULL;
    parser->f_desc     = NULL;
    parser->f_offset   = -1;
    parser->f_volatile = ! parser->f_defaultnv;
}

static bool bmib_parse_for_keyword(bmib_context_t *parser, bmib_keyword_t kw, const char *name)
{
    bmib_token_type_t toktype;
    bmib_keyword_t    kwtype;
    bool              tokerr;

    do
    {
        if (bmib_get_nc_token(parser, &toktype, &kwtype, "Keyword", &tokerr))
        {
            bmib_error_eof(parser, "Parsing for Keyword", name);
            return false;
        }
        if (toktype == mttLabel && kwtype == kw)
        {
            return true;
        }
        else if (toktype != mttComment)
        {
            bmib_error_expected(parser, name);
            return false;
        }
    }
    while (1);
}

static bool bmib_parse_for_type(bmib_context_t *parser, bmib_token_type_t type, const char *name)
{
    bmib_token_type_t toktype;
    bmib_keyword_t    kwtype;
    bool              tokerr;

    do
    {
        if (bmib_get_nc_token(parser, &toktype, &kwtype, "Type", &tokerr) )
        {
            bmib_error_eof(parser, "Parsing for type", name);
            return false;
        }
        if (toktype == type)
        {
            return true;
        }
        else if (toktype != mttComment)
        {
            bmib_error_expected(parser, name);
            return false;
        }
    }
    while (1);
}

static bool bmib_parse_number_list(bmib_context_t *parser, int *resNums, int *nNums)
{
    return true;
}

static void bmib_parse_imports(bmib_context_t *parser)
{
    bmib_token_type_t toktype;
    bmib_keyword_t    kwtype;
    bool              tokerr;

    bmib_symbol_t    *imports;
    bmib_symbol_t    *sym;

    butil_log(5, "Parsing Imports\n");
    imports = NULL;

    do
    {
        if (bmib_get_nc_token(parser, &toktype, &kwtype, "IMPORTS", &tokerr))
        {
            bmib_error_eof(parser, "Parsing IMPORTS near", parser->f_token);
            break;
        }
        if (toktype == mttLabel)
        {
            if (kwtype == kwFROM)
            {
                if (bmib_get_nc_token(parser, &toktype, &kwtype, "Imports MIB", &tokerr))
                {
                    bmib_error_eof(parser, "Parsing IMPORTS near", parser->f_token);
                    break;
                }
                if (toktype != mttLabel)
                {
                    bmib_error_expected(parser, "IMPORTS FROM MIB");
                }
                if (! imports)
                {
                    butil_log(5, "%s => Skipping IMPORT: Needed Symbols already defined\n", parser->f_token);
                }
                else
                {
                    butil_log(5, "%s => IMPORTing file to get:", parser->f_token);
                    bmib_dump_symtree(imports);

                    // have to import the mib
                    //
                    char fname[MAX_PATH + 32];

                    strcpy(fname, parser->f_token);
                    if (bmib_parse_mib(parser, fname, parser->f_mibpath, parser->f_errfunc, parser->f_defaultnv))
                    {
                        // probably the failure prompted its own message
                        // so no need to bark any more
                        ;
                    }
                    bmib_destroy_symtree(parser, imports);
                    imports = NULL;
                }
            }
            else
            {
                if ((sym = bmib_find_symbol(parser->f_token, parser->f_nodesyms)) != NULL)
                {
                    butil_log(5, "%s => Object Already defined: skipping import\n", parser->f_token);
                }
                else if ((sym = bmib_find_symbol(parser->f_token, parser->f_macrosyms)) != NULL)
                {
                    butil_log(5, "%s => Macro Builtin: skipping import\n", parser->f_token);
                }
                else if ((sym = bmib_find_symbol(parser->f_token, parser->f_typesyms)) != NULL)
                {
                    butil_log(5, "%s => Type Builtin: skipping import\n", parser->f_token);
                }
                else
                {
                    sym = bmib_create_int_symbol(parser, parser->f_token, 0);
                    if (! sym)
                    {
                        bmib_error_memory(parser, "Import");
                        break;
                    }
                    bmib_add_symbol(parser, sym, &imports);
                }
            }
        }
    }
    while (toktype != mttPunctuation || parser->f_token[0] != ';');

    if (imports)
    {
        bmib_destroy_symtree(parser, imports);
    }
    parser->f_state = mpsMIBL;
}

static void bmib_parse_macro(bmib_context_t *parser)
{
    bmib_token_type_t toktype;
    bmib_keyword_t    kwtype;
    bool              tokerr;

    butil_log(5, "Parsing Macro\n");

    do
    {
        bmib_get_nc_token(parser, &toktype, &kwtype, "MACRO Body", &tokerr);
    }
    while (! tokerr && (toktype != mttLabel || kwtype != kwEND));

    parser->f_state = mpsMIBL;
}

static void bmib_parse_module_identity(bmib_context_t *parser)
{
    bmib_token_type_t toktype;
    bmib_keyword_t    kwtype;
    bool              tokerr;

    butil_log(5, "Parsing Module Identity\n");
    parser->f_state = mpsMIBR;

    do
    {
        if (bmib_get_nc_token(parser, &toktype, &kwtype, "MODULE-IDENTITY", &tokerr))
        {
            bmib_error_eof(parser, "MODULE-IDENTITY For ", parser->f_lval);
            break;
        }
    }
    while (toktype != mttEqual);

    parser->f_state = mpsMIBR;
/*
    do
    {
        if (bmib_get_token(parser, &toktype, &kwtype) <= 0)
        {
            bmib_error_eof(parser, "MODULE-IDENTITY For ", parser->f_lval);
            break;
        }
    }
    while (toktype != mttPunctuation || parser->f_token[0] != '}');

    parser->f_state = mpsMIBL;
*/
}

static void bmib_parse_module_compliance(bmib_context_t *parser)
{
    bmib_token_type_t toktype;
    bmib_keyword_t    kwtype;
    bool              tokerr;

    butil_log(5, "Parsing Module Compliance\n");
    do
    {
        if (bmib_get_nc_token(parser, &toktype, &kwtype, "MODULE-COMPLIANCE", &tokerr))
        {
            bmib_error_eof(parser, "MODULE-COMPLIANCE For ", parser->f_lval);
            break;
        }
    }
    while (toktype != mttEqual);

    parser->f_state = mpsMIBR;

    do
    {
        if (bmib_get_nc_token(parser, &toktype, &kwtype, "MODULE-COMPLIANCE", &tokerr))
        {
            bmib_error_eof(parser, "MODULE-COMPLIANCE For ", parser->f_lval);
            break;
        }
    }
    while (toktype != mttPunctuation || parser->f_token[0] != '}');

    parser->f_state = mpsMIBL;
}

static void bmib_parse_sequence(bmib_context_t *parser)
{
    bmib_token_type_t toktype;
    bmib_keyword_t    kwtype;
    bool              tokerr;

    butil_log(5, "Parsing Sequence\n");
    do
    {
        if (bmib_get_nc_token(parser, &toktype, &kwtype, "SEQUENCE", &tokerr))
        {
            bmib_error_eof(parser, "SEQUENCE For ", parser->f_lval);
            break;
        }
    }
    while (toktype != mttEqual);
    parser->f_state = mpsMIBR;
}

static void bmib_parse_object_group(bmib_context_t *parser)
{
    bmib_symbol_t    *sym;
    bmib_group_t     *pgrpe;
    bmib_group_t     *groups;
    bmib_token_type_t toktype;
    bmib_keyword_t    kwtype;
    bool              tokerr;

    butil_log(5, "Parsing Object Group %s\n", parser->f_lval);

    do
    {
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-GROUP", &tokerr);
        if (tokerr || toktype != mttLabel || kwtype != kwOBJECTS)
        {
            bmib_error_expected(parser, "OBJECTS in OBJECT-GROUP");
            break;
        }
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-GROUP", &tokerr);
        if (tokerr || toktype != mttPunctuation || parser->f_token[0] != '{')
        {
            bmib_error_expected(parser, "{ after OBJECTS");
            break;
        }
        // create a sym for group name, and add a type to groups list referencing sym
        sym     = bmib_create_int_symbol(parser, parser->f_lval, 0);
        if (! sym)
        {
            bmib_error_memory(parser, "Group Name");
            break;
        }
        groups  = bmib_create_type(parser, sym, kwNONE, parser->f_groups, NULL);
        if (! groups)
        {
            bmib_error_memory(parser, "Group");
            break;
        }
        parser->f_groups    = groups;
        sym->f_value.u_tv   = parser->f_groups;
        sym->f_type         = bstType;
        bmib_add_symbol(parser, sym, &parser->f_groupsyms);

        // for each label in the group list, create a blank sym to point to hold pointer to group type
        do
        {
            bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-GROUP", &tokerr);
            if (tokerr)
            {
                break;
            }
            if (toktype == mttLabel)
            {
                sym = bmib_create_int_symbol(parser, parser->f_token, 0);
                if (! sym)
                {
                    bmib_error_memory(parser, "Group");
                    break;
                }
                pgrpe = bmib_create_type(parser, sym, kwNONE, parser->f_grouptypes, NULL);
                if (! pgrpe)
                {
                    bmib_error_memory(parser, "Group Type");
                    break;
                }
                parser->f_grouptypes = pgrpe;
                sym->f_value.u_tv = pgrpe;
                sym->f_type       = bstType;
                //parser->f_groups->f_basetype = pgrpe;
                bmib_add_symbol(parser, sym, &parser->f_groupsyms);
            }
        }
        while (! tokerr && (toktype != mttPunctuation || parser->f_token[0] != '}'));

        // read status
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-GROUP", &tokerr);
        if (toktype != mttLabel && kwtype != kwSTATUS)
        {
            bmib_error_expected(parser, "STATUS to follow ACCESS in OBJECT-TYPE");
            break;
        }
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-GROUP", &tokerr);
        if (toktype != mttLabel)
        {
            bmib_error_expected(parser, "Status to follow STATUS in OBJECT-GROUP");
            break;
        }
        switch (kwtype)
        {
        case kwCURRENT:
        case kwOPTIONAL:
        case kwMANDATORY:
        case kwDEPRECATED:
        case kwOBSOLETE:
            break;
        default:
            bmib_error_expected(parser, "Proper Status in OBJECT-GROUP");
            break;
        }
        // read description
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-GROUP", &tokerr);
        if (toktype != mttLabel || kwtype != kwDESCRIPTION)
        {
            bmib_error_expected(parser, "DESCRIPTION to follow ACCESS in OBJECT-GROUP");
            break;
        }
        // read description text
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
        if (toktype != mttString)
        {
            bmib_error_expected(parser, "DESCRIPTION Quoted Text in OBJECT-GROUP");
            break;
        }
        parser->f_desc = malloc(strlen(parser->f_token) * 2 + 2);
        if (! parser->f_desc)
        {
            bmib_error_memory(parser, "Description");
            break;
        }
        bmib_format_description(parser->f_desc, parser->f_token);
    }
    while (0);

    parser->f_state = mpsMIBV;
}

static void bmib_parse_notification_type(bmib_context_t *parser)
{
    bmib_token_type_t toktype;
    bmib_keyword_t    kwtype;
    bool              tokerr;

    butil_log(5, "Parsing Notification Type %s\n", parser->f_lval);
    do
    {
        if (bmib_get_nc_token(parser, &toktype, &kwtype, "NOTFICATION-TYPE", &tokerr))
        {
            bmib_error_eof(parser, "NOTIFICATION-TYPE For ", parser->f_lval);
            break;
        }
    }
    while (toktype != mttEqual);

    parser->f_state = mpsMIBR;
}

static void bmib_parse_range(
                        bmib_context_t     *parser,
                        bmib_token_type_t  *toktype,
                        bmib_keyword_t     *kwtype,
                        const char         *tfor
                      )
{
    bmib_range_t   *prange;
    int             lowval;
    int             hival;
    bool            tokerr;

    // parse the range spec
    //
    do
    {
        bmib_get_nc_token(parser, toktype, kwtype, tfor, &tokerr);
        if (*toktype != mttNumber)
        {
            bmib_error_expected(parser, "Numeric value inside ()");
            break;
        }
        lowval = strtol(parser->f_token, NULL, 0);

        bmib_get_nc_token(parser, toktype, kwtype, tfor, &tokerr);
        if (*toktype == mttRange)
        {
            bmib_get_nc_token(parser, toktype, kwtype, tfor, &tokerr);
            if (*toktype != mttNumber)
            {
                bmib_error_expected(parser, "Numeric value after \"..\" ");
                break;
            }
            hival = strtol(parser->f_token, NULL, 0);
            bmib_get_nc_token(parser, toktype, kwtype, tfor, &tokerr);
        }
        else
        {
            if (*toktype != mttPunctuation || (parser->f_token[0] != ')' && parser->f_token[0] != '|'))
            {
                bmib_error_expected(parser, " \"..\" or \")\" or \"|\" ");
                break;
            }
            else
            {
                hival = lowval;
            }
        }
        prange = bmib_create_range(parser, lowval, hival, NULL);
        if (! prange)
        {
            bmib_error_memory(parser, "Range");
            break;
        }
        if (! parser->f_ranges)
        {
            parser->f_ranges = prange;
        }
        else
        {
            bmib_range_t *pr;

            for (pr = parser->f_ranges; pr->f_next;)
            {
                pr = pr->f_next;
            }
            pr->f_next = prange;
        }
    }
    while ((*toktype != mttPunctuation || parser->f_token[0] != ')') && parser->f_state != mpsError);
}

static void bmib_parse_asn_type(bmib_context_t *parser, bmib_token_type_t *toktype, bmib_keyword_t *kwtype)
{
    bmib_keyword_t  objnamekwtype;
    bool            tokerr;

    bmib_symbol_t      *sym;
    bmib_typespec_t    *type;

    parser->f_enums  = NULL;
    parser->f_ranges = NULL;

    butil_log(5, "Parse Type for %s\n", parser->f_lval);

    do
    {
        if (*toktype != mttLabel)
        {
            if (*toktype == mttPunctuation && parser->f_token[0] == '[')
            {
                // [APPLICATION N] IMPLICIT
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                if (tokerr)
                {
                    break;
                }
                // todo - set realm
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                if (tokerr)
                {
                    break;
                }
                // todo - set number
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                if (tokerr)
                {
                    break;
                }
                if (*toktype != mttPunctuation || parser->f_token[0] != ']')
                {
                    bmib_error_expected(parser, "]");
                    break;
                }
                else
                {
                    bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                    if (tokerr)
                    {
                        break;
                    }
                    if (*toktype == mttLabel && *kwtype == kwIMPLICIT)
                    {
                        bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                    }
                }
            }
            else
            {
                bmib_error_expected(parser, "ASN Type");
                break;
            }
        }
        objnamekwtype = *kwtype;
        strcpy(parser->f_ltype, parser->f_token);

        // look ahead one token
        bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);

        switch (objnamekwtype)
        {
        case kwOBJECT:
            if (*toktype != mttLabel || *kwtype != kwIDENTIFIER)
            {
                bmib_error_expected(parser, "IDENTIFER to follow OBJECT");
            }
            else
            {
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
            }
            break;

        case kwSEQUENCE:
            parser->f_typemods |= SEQUENCE_OF;
            if (*toktype == mttLabel && *kwtype == kwOF)
            {
                // read type of in sequence
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
            }
            else if (*toktype == mttPunctuation && parser->f_token[0] == '{')
            {
                // read enumeration of sequence
                do
                {
                    bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                }
                while ((*toktype != mttPunctuation || parser->f_token[0] != '}') && ! tokerr);
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
            }
            break;

        case kwOCTET:
        default:
            if (*toktype == mttLabel && *kwtype == kwSTRING)
            {
                parser->f_typemods |= STRING_OF;
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
            }
            break;
        }
        // lookup the ltype in the type table and see if this is
        // already a defined type and if it is, substitute the
        // defined name for the type name, although type references
        // can form a long chain down to the atomic type, the
        // actual type keyword number bubbles up to the top of the list
        // so no need to decend now
        //
        type = NULL;

        if ((sym = bmib_find_symbol(parser->f_ltype, parser->f_typesyms)) != NULL)
        {
            type = sym->f_value.u_tv;
        }
        else
        {
            // forward reference to a new type as yet to be defined
            //
            sym  = bmib_create_int_symbol(parser, parser->f_ltype, 0);
            if (! sym)
            {
                bmib_error_memory(parser, "Type");
                break;
            }
            sym->f_type = bstType;
            sym->f_value.u_tv = type =  bmib_create_type(parser, sym, kwNONE, parser->f_types, NULL);
            if (! type)
            {
                bmib_error_memory(parser, "Type");
                break;
            }
            parser->f_types = type;

            // add the undefined type to the typesyms so any definition of it later
            // modifies this version not a new one
            //
            bmib_add_symbol(parser, sym, &parser->f_typesyms);
        }
        parser->f_objtype = type;

        // determine what to look for next based on type, and if type is a string type
        //
        int isStringType = (type && (type->f_typemods & STRING_OF)) | (parser->f_typemods & STRING_OF);

        if (isStringType)
        {
            // parse "SIZE"
            //
            if (*toktype == mttPunctuation && parser->f_token[0] == '(')
            {
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                if (*toktype == mttLabel && *kwtype == kwSIZE)
                {
                    bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                    if (*toktype != mttPunctuation || parser->f_token[0] != '(')
                    {
                        bmib_error_expected(parser, " \"(\" After SIZE");
                    }
                    bmib_parse_range(parser, toktype, kwtype, "ASN Size");
                    bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                }
                while ((*toktype != mttPunctuation || parser->f_token[0] != ')') && parser->f_state != mpsError)
                {
                    bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                }
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
            }
            else
            {
                bmib_range_t *prange;

                // string unbounded in size, can't have that, so just make it 255 ?
                //
                if (type && type->f_ranges)
                {
                    prange = type->f_ranges;
                }
                else
                {
                    prange = bmib_create_range(parser, 0, 255, NULL);
                    if (! prange)
                    {
                        bmib_error_memory(parser, "Range");
                        break;
                    }
               }
                if (! parser->f_ranges)
                {
                    parser->f_ranges = prange;
                }
                else
                {
                    bmib_range_t *pr;

                    for (pr = parser->f_ranges; pr->f_next;)
                    {
                        pr = pr->f_next;
                    }
                    pr->f_next = prange;
                }
            }
        }
        else switch(type ? type->f_kwtype : kwDESCRIPTION)
        {
        case kwOBJECT:
        case kwSEQUENCE:
            break;

        case kwCHOICE:

            while ((*toktype != mttPunctuation || parser->f_token[0] != '}') && parser->f_state != mpsError)
            {
                bmib_get_nc_token(parser, toktype, kwtype, "ASN CHOICE", &tokerr);
            }
            bmib_get_nc_token(parser, toktype, kwtype, "ASN CHOICE", &tokerr);
            break;

        case kwOCTET:
        case kwBIT:
        case kwBITS:
        case kwBITSTRING:
        case kwINTEGER:
#ifndef NO_PREDEF_TYPES
        case kwINTEGER32:
        case kwUINTEGER32:
        case kwUNSIGNED32:
        case kwCOUNTER:
        case kwCOUNTER32:
        case kwGAUGE:
        case kwGAUGE32:
        case kwCOUNTER64:
#endif
            // these can be enumerated or ranges too
            if (*toktype == mttPunctuation)
            {
                if (parser->f_token[0] == '{')
                {
                    bmib_symbol_t  *esym;
                    bmib_enum_t    *penum;
                    char            esymname[MAX_MIB_TOKEN];
                    int             eval;
                    bool            gotval;

                    // parse the enumeration
                    //
                    do
                    {
                        esymname[0] = 0;
                        esym        = NULL;
                        gotval      = false;
                        bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                        if (*toktype == mttLabel)
                        {
                            // modify symname with type name to make it unique
                            strcpy(esymname, parser->f_lval);
                            strcat(esymname, "_");
                            strcat(esymname, parser->f_token);
                            bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                            if (*toktype != mttPunctuation || parser->f_token[0] != '(')
                            {
                                bmib_error_expected(parser, "(value after Enumeration Name)");
                                break;
                            }
                            bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                            if (*toktype != mttNumber)
                            {
                                bmib_error_expected(parser, "Numeric value inside (");
                                break;
                            }
                            eval = strtol(parser->f_token, NULL, 0);
                            bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                            if (*toktype != mttPunctuation || parser->f_token[0] != ')')
                            {
                                bmib_error_expected(parser, "Closing parenthesis");
                                break;
                            }
                            esym = bmib_create_int_symbol(parser, esymname, eval);
                            if (! sym)
                            {
                                bmib_error_memory(parser, "Fwd Type");
                                break;
                            }
                            bmib_add_symbol(parser, esym, &parser->f_enumsyms);
                            gotval = true;
                        }
                        else if (*toktype == mttNumber)
                        {
                            eval = strtol(parser->f_token, NULL, 0);
                            gotval = true;
                        }
                        else if (*toktype != mttComment && *toktype != mttPunctuation)
                        {
                            bmib_error_expected(parser, "Enumeration Symbol or Number in Syntax");
                            break;
                        }
                        if (gotval)
                        {
                            penum = bmib_create_enum(parser, esym, eval, NULL);
                            if (! penum)
                            {
                                bmib_error_memory(parser, "Enum");
                                break;
                            }
                            if (! parser->f_enums)
                            {
                                parser->f_enums = penum;
                            }
                            else
                            {
                                bmib_enum_t *pe;

                                for (pe = parser->f_enums; pe->f_next;)
                                {
                                    pe = pe->f_next;
                                }
                                pe->f_next = penum;
                            }
                            bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
                        }
                    }
                    while ((*toktype != mttPunctuation || parser->f_token[0] != '}') && parser->f_state != mpsError);
                }
                else if (parser->f_token[0] == '(')
                {
                    bmib_parse_range(parser, toktype, kwtype, "ASN Range");
                }
                else
                {
                    bmib_error_token(parser, "Bad Character after Type");
                    break;
                }
                bmib_get_nc_token(parser, toktype, kwtype, "ASN Type", &tokerr);
            }
            break;

        case kwOPAQUE:
            break;

        case kwNULL:
#ifndef NO_PREDEF_TYPES
        case kwTIMETICKS:
        case kwNETWORKADDRESS:
        case kwNSAPADDRESS:
#endif
            break;

        case kwNONE:

            butil_log(5, "Forward reference to %s\n", parser->f_ltype);
            break;

        default:

            bmib_error(parser, meUndefSym, "Undefined Type Referenced: %s\n", parser->f_ltype);
            break;
        }
    }
    while (0);

    parser->f_state = mpsMIBL;
}

static void bmib_define_type(bmib_context_t *parser, const char *tname, const char *tref)
{
    bmib_symbol_t   *sym;
    bmib_symbol_t   *refsym;
    bmib_typespec_t *type;

    // add this type to the type list and type symbols
    //

    // see if the sym has been defined (via a forward reference) or as
    // an already built in extended type
    //
    sym = bmib_find_symbol(tname, parser->f_typesyms);

    // lookup the referenced type (insist it is defined)
    //
    refsym = bmib_find_symbol(tref, parser->f_typesyms);

    if (! refsym)
    {
        bmib_error(parser, meUndefSym, "Undefined Type Referenced: %s\n", tref);
    }
    else
    {
        bool wasforward;

        if (! sym)
        {
            wasforward = false;
            sym = bmib_create_int_symbol(parser, tname, 0);
            if (! sym)
            {
                bmib_error_memory(parser, "Fwd Type");
                return;
            }
        }
        else
        {
            wasforward = true;
        }
        // note the inheritance of base type's kwtype, unless
        // the new type already has a type (it's attempting
        // to be redefined) in which case I let it keep its
        // first type (this happens to preloaded asn types
        // like "counter32"
        //
        type    = bmib_create_type(
                                parser,
                                sym,
                                sym->f_value.u_tv ?
                                    sym->f_value.u_tv->f_kwtype :
                                    refsym->f_value.u_tv->f_kwtype,
                                parser->f_types,
                                refsym->f_value.u_tv
                              );
        if (! type)
        {
            bmib_error_memory(parser, "Type");
            return;
        }
        parser->f_types = type;

        sym->f_type         = bstType;
        sym->f_value.u_tv   = type;
        type->f_enums       = parser->f_enums;
        type->f_ranges      = parser->f_ranges;
        type->f_desc        = parser->f_desc;
        type->f_typemods    = refsym->f_value.u_tv->f_typemods;
        type->f_typemods    |= parser->f_typemods;
        if (parser->f_indices && ! type->f_indices)
        {
            type->f_indices = parser->f_indices;
        }
        else
        {
            type->f_indices = refsym->f_value.u_tv->f_indices;
        }
        parser->f_enums     = NULL;
        parser->f_ranges    = NULL;
        parser->f_desc      = NULL;
        parser->f_typemods  = 0;
        parser->f_indices   = NULL;

        if (! wasforward)
        {
            // add to typesyms if hadn't found it
            //
            bmib_add_symbol(parser, sym, &parser->f_typesyms);
        }
        else
        {
            // This was a forward declared symbol, so that
            // means there could be objects referencing this
            // type which don't have local version of the
            // typemods, etc.  so back-annotate them now
            //
            ; // TODO (done after running now)
        }
    }
    return;
}

static void bmib_parse_textual_convention(bmib_context_t *parser)
{
    bmib_token_type_t toktype;
    bmib_keyword_t kwtype;
    bool        tokerr;

    butil_log(5, "Parsing Textual-Convention\n");
    do
    {
        // read status
        //
        do
        {
            // expect a SYNTAX element
            //
            bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
            if (toktype != mttLabel || kwtype != kwSTATUS)
            {
                if (toktype == mttLabel && kwtype == kwDISPLAY_HINT)
                {
                    bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                }
                else
                {
                    bmib_error_expected(parser, "STATUS in TEXTUAL-CONVENTION");
                    break;
                }
            }
        }
        while (! tokerr && (kwtype != kwSTATUS));

        // read acess
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "TEXTUAL-CONVENTION", &tokerr);
        if (toktype != mttLabel)
        {
            bmib_error_expected(parser, "Status to follow STATUS in TEXTUAL-CONVENTION");
            break;
        }
        switch (kwtype)
        {
        case kwCURRENT:
        case kwOPTIONAL:
        case kwMANDATORY:
        case kwDEPRECATED:
        case kwOBSOLETE:
            break;
        default:
            bmib_error_expected(parser, "Proper Status in TEXTUAL-CONVENTION");
            break;
        }
        // read description
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "TEXTUAL-CONVENTION", &tokerr);
        if (toktype != mttLabel || kwtype != kwDESCRIPTION)
        {
            bmib_error_expected(parser, "DESCRIPTION to follow STATUS in TEXTUAL-CONVENTION");
            break;
        }
        // read description text
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "TEXTUAL-CONVENTION", &tokerr);
        if (toktype != mttString)
        {
            bmib_error_expected(parser, "DESCRIPTION Quoted Text in TEXTUAL-CONVENTION");
            break;
        }
        parser->f_desc = malloc(strlen(parser->f_token) * 2 + 2);
        if (! parser->f_desc)
        {
            bmib_error_memory(parser, "Description");
            break;
        }
        bmib_format_description(parser->f_desc, parser->f_token);

        // expect a SYNTAX OR REFERENCE element
        //
        do
        {
            bmib_get_nc_token(parser, &toktype, &kwtype, "TEXTUAL-CONVENTION", &tokerr);
            if (toktype != mttLabel || kwtype != kwSYNTAX)
            {
                if (toktype == mttLabel && kwtype == kwREFERENCE)
                {
                    bmib_get_nc_token(parser, &toktype, &kwtype, "TEXTUAL-CONVENTION", &tokerr);
                }
                else
                {
                    bmib_error_expected(parser, "SYNTAX in TEXTUAL-CONVENTION");
                    break;
                }
            }
        }
        while (kwtype != kwSYNTAX);

        // after syntax comes a valid ASN type
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "TEXTUAL-CONVENTION", &tokerr);
        bmib_parse_asn_type(parser, &toktype, &kwtype);
    }
    while (0);

    if (parser->f_state != mpsError)
    {
        bmib_define_type(parser, parser->f_lval, parser->f_ltype);

        // left a token dangling, it should be the lvalue of the next operation
        //
        bmib_restart_object(parser);

        if (toktype == mttLabel)
        {
            if (kwtype == kwEND)
            {
                parser->f_state = mpsEnd;
            }
            else
            {
                parser->f_state = mpsMIBV;
            }
            return;
        }
        else if (toktype != mttComment)
        {
            // funny, this is kindof an error, so just bmib_restart_object anyway
            // and hope it all works out later
            //
            parser->f_state = mpsMIBV;
            return;
        }
    }
    parser->f_state = mpsMIBL;
}

static void bmib_parse_object_type(bmib_context_t *parser)
{
    bmib_token_type_t   toktype;
    bmib_keyword_t      kwtype;
    bool                tokerr;
    int                 dimdex;

    // SYNTAX, [MAX-|MIN-|]ACCESS, STATUS, [DESCRIPTION|INDEX|DEFVAL|REFERENCE|AUGMENTS]

    butil_log(5, "Parsing Object Type %s\n", parser->f_lval);

    do
    {
        do
        {
            // expect a SYNTAX element
            //
            bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
            if (toktype != mttLabel || kwtype != kwSYNTAX)
            {
                if (toktype == mttLabel && kwtype == kwDISPLAY_HINT)
                {
                    bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                }
                else
                {
                    bmib_error_expected(parser, "SYNTAX in OBJECT-TYPE");
                    break;
                }
            }
        }
        while (! tokerr && (kwtype != kwSYNTAX));

        // after syntax comes a valid ASN type
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
        bmib_parse_asn_type(parser, &toktype, &kwtype);

        // the syntax object should have been parsed, so now expect access
        // or units and access
        //
        if (parser->f_state == mpsError)
        {
            break;
        }
        if (toktype != mttLabel)
        {
            bmib_error_expected(parser, "ACCESS or UNITS after SYNTAX in OBJECT-TYPE");
            break;
        }
        if (kwtype == kwUNITS)
        {
            bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
            if (! tokerr)
            {
                if (toktype != mttString)
                {
                    bmib_error_expected(parser, "Quoted String Units after UNITS in OBJECT-TYPE");
                }
            }
            bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
        }
        switch (kwtype)
        {
        case kwMIN_ACCESS:
        case kwMAX_ACCESS:
        case kwACCESS:
            break;
        default:
            bmib_error_expected(parser, "ACCESS in OBJECT-TYPE");
            break;
        }
        // read acess
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
        if (toktype != mttLabel)
        {
            bmib_error_expected(parser, "Access-Level to follow ACCESS in OBJECT-TYPE");
            break;
        }
        switch (kwtype)
        {
        case kwNOT_ACCESSIBLE:
            parser->f_access = maNone;
            break;
        case kwREAD_ONLY:
            parser->f_access = maReadOnly;
            break;
        case kwREAD_WRITE:
            parser->f_access = maReadWrite;
            break;
        case kwREAD_CREATE:
            parser->f_access = maReadCreate;
            break;
        case kwACCESSIBLE_FOR_NOTIFY:
            parser->f_access = maTrap;
            break;
        default:
            bmib_error_expected(parser, "Proper Acess-Level in OBJECT-TYPE");
            break;
        }
        // read status
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
        if (toktype != mttLabel && kwtype != kwSTATUS)
        {
            bmib_error_expected(parser, "STATUS to follow ACCESS in OBJECT-TYPE");
            break;
        }
        // read acess
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
        if (toktype != mttLabel)
        {
            bmib_error_expected(parser, "Status to follow STATUS in OBJECT-TYPE");
            break;
        }
        switch (kwtype)
        {
        case kwCURRENT:
        case kwOPTIONAL:
        case kwMANDATORY:
        case kwDEPRECATED:
        case kwOBSOLETE:
            break;
        default:
            bmib_error_expected(parser, "Proper Status in OBJECT-TYPE");
            break;
        }
        // read description
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
        if (toktype != mttLabel || kwtype != kwDESCRIPTION)
        {
            bmib_error_expected(parser, "DESCRIPTION to follow ACCESS in OBJECT-TYPE");
            break;
        }
        // read description text
        //
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
        if (toktype != mttString)
        {
            bmib_error_expected(parser, "DESCRIPTION Quoted Text in OBJECT-TYPE");
            break;
        }
        parser->f_desc = malloc(strlen(parser->f_token) * 2 + 2);
        if (! parser->f_desc)
        {
            bmib_error_memory(parser, "Description");
            break;
        }
        bmib_format_description(parser->f_desc, parser->f_token);

        // after this all labels are optional
        //
        do
        {
            bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
            if (toktype == mttLabel)
            {
                char defval[256];

                switch (kwtype)
                {
                case kwDEFVAL:
                    bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                    if (toktype != mttPunctuation || parser->f_token[0] != '{')
                    {
                        bmib_error_expected(parser, "{ after DEFVAL");
                        break;
                    }
                    do
                    {
                        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);

                        if (toktype == mttLabel)
                        {
                            bmib_symbol_t*  pvs;
                            bmib_symbol_t*  ptree;
                            bmib_typespec_t* ptype;

                            if (parser->f_objtype)
                            {
                                for (ptype = parser->f_objtype; ptype->f_basetype;)
                                    ptype = ptype->f_basetype;

                                // should be defined, just parsed the type above
                                switch (ptype->f_kwtype)
                                {
                                    // [TODO] - pick symbol tree appropriately by type
                                case kwINTEGER:
                                default:
                                    ptree = parser->f_enumsyms;
                                    break;
                                }
                            }
                            else
                            {
                                ptree = parser->f_enumsyms;
                            }
                            if (ptree)
                            {
                                char mod_enum[256];

                                // an enum tag name, mangle
                                // token to our scope first
                                strcpy(mod_enum, parser->f_lval);
                                strcat(mod_enum, "_");
                                strcat(mod_enum, parser->f_token);

                                pvs = bmib_find_symbol(mod_enum, ptree);
                                if (! pvs)
                                {
                                    // not in our scope, if there's a typename, try
                                    // scoping in type's space
                                    strcpy(mod_enum, parser->f_ltype);
                                    strcat(mod_enum, "_");
                                    strcat(mod_enum, parser->f_token);

                                    pvs = bmib_find_symbol(mod_enum, ptree);

                                    if (! pvs)
                                    {
                                        // not in type's scope, try global scope
                                        pvs = bmib_find_symbol(parser->f_token, ptree);
                                        if (! pvs)
                                        {
                                            bmib_error_undefined(parser, parser->f_token);
                                            break;
                                        }
                                    }
                                }
                                // [TODO] - verify value
                                //
                                switch (pvs->f_type)
                                {
                                case bstNumber:
                                    snprintf(defval, 256, "%u", pvs->f_value.u_iv);
                                    break;

                                case bstString:
                                    strncpy(defval, pvs->f_value.u_sv, 255);
                                    defval[255] = 0;
                                    break;

                                default:
                                    bmib_error(parser, meUnexpected, "Bad Type for DEFVAL");
                                    break;
                                }
                            }
                        }
                        else if (toktype == mttNumber)
                        {
                            //snprintf(defval, 256, "%ld", strtol(f_token, NULL, 0));
                            // save the actual text as specified, cause we don't
                            // know if it's signed or unsigned yet
                            strncpy(defval, parser->f_token, 255);
                            defval[255] = 0;
                        }
                        else if (toktype == mttString)
                        {
                            strncpy(defval, parser->f_token, 255);
                            defval[255] = 0;
                        }
                        else
                        {
                            bmib_error_expected(parser, "Valid Default Value");
                            break;
                        }
                        if (defval[0])
                        {
                            bmib_defval_t* values;
                            bmib_defval_t* val = bmib_create_defval(parser, defval);
                            if (! val)
                            {
                                bmib_error_memory(parser, "Default Value");
                                break;;
                            }
                            if (! parser->f_defvals)
                            {
                                parser->f_defvals = val;
                            }
                            else
                            {
                                for (values = parser->f_defvals; values->f_next;)
                                {
                                    values = values->f_next;
                                }
                                values->f_next = val;
                            }
                        }
                        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                    }
                    while (parser->f_state != mpsError && (toktype != mttPunctuation || parser->f_token[0] != '}'));
                    break;

                case kwINDEX:
                    bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                    if (toktype != mttPunctuation || parser->f_token[0] != '{')
                    {
                        bmib_error_expected(parser, "{ after INDEX");
                        break;
                    }
                    dimdex = 0;
                    do
                    {
                        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                        if (toktype == mttLabel && kwtype == kwNONE)
                        {
                            dimdex++;

                            bmib_indexval_t *indices;
                            bmib_indexval_t *index = bmib_create_index(parser, parser->f_token, dimdex);

                            if (! index)
                            {
                                bmib_error_memory(parser, "Index");
                                break;
                            }
                            if (! parser->f_indices)
                            {
                                parser->f_indices = index;
                            }
                            else
                            {
                                for (indices = parser->f_indices; indices->f_next;)
                                {
                                    indices = indices->f_next;
                                }
                                indices->f_next = index;
                            }
                            butil_log(5, "index %d = %s\n", dimdex, parser->f_token);
                        }
                    }
                    while (parser->f_state != mpsError && (toktype != mttPunctuation || parser->f_token[0] != '}'));
                    break;

                case kwAUGMENTS:
                    bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                    if (toktype != mttPunctuation || parser->f_token[0] != '{')
                    {
                        bmib_error_expected(parser, "{ after AUGMENTS");
                        break;
                    }
                    do
                    {
                        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                    }
                    while (parser->f_state != mpsError && (toktype != mttPunctuation || parser->f_token[0] != '}'));
                    break;

                case kwREFERENCE:
                    bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                    if (toktype != mttString)
                    {
                        bmib_error_expected(parser, "REFERENCE Quoted Text in OBJECT-TYPE");
                        break;
                    }
                    break;
#ifndef NO_EXTENSIONS

                case kwOFFSET:
                    bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                    if (toktype != mttNumber)
                    {
                        bmib_error_expected(parser, "Number Value after OFFSET");
                        break;
                    }
                    parser->f_offset = strtol(parser->f_token, NULL, 0);
                    do
                    {
                        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                    }
                    while (parser->f_state != mpsError && (toktype != mttEqual) && (toktype != mttPunctuation || parser->f_token[0] != '}'));
                    break;

                case kwVOLATILE:
                    parser->f_volatile = true;
                    do
                    {
                        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
                    }
                    while (parser->f_state != mpsError && (toktype != mttEqual) && (toktype != mttPunctuation || parser->f_token[0] != '}'));
                    break;
#endif

                default:
                    bmib_error_expected(parser, "INDEX|DEFVAL|AUGMENTS|REFERENCE");
                    break;
                }
            }
        }
        while (toktype == mttPunctuation && parser->f_token[0] == '}' && parser->f_state != mpsError);
    }
    while (0);

    while ((parser->f_state != mpsError) && (toktype != mttEqual))
    {
        bmib_get_nc_token(parser, &toktype, &kwtype, "OBJECT-TYPE", &tokerr);
    }

    parser->f_state = mpsMIBR;
}

static void bmib_parse_object_identity(bmib_context_t *parser)
{
    bmib_token_type_t toktype;
    bmib_keyword_t    kwtype;
    bool              tokerr;

    butil_log(5, "Parsing Object Identity %s\n", parser->f_lval);

    do
    {
        if (bmib_get_nc_token(parser, &toktype, &kwtype,  "OBJECT-IDENTITY", &tokerr))
        {
            bmib_error_eof(parser, "OBJECT-IDENTITY For ", parser->f_lval);
            break;
        }
    }
    while (toktype != mttEqual && parser->f_state != mpsError);
    parser->f_state = mpsMIBR;
}

static void bmib_parse_object_OID(bmib_context_t *parser)
{
    // parse  [ sym | num | sym(num) ] ...
    // for the each specifier, if symbolic, find the place
    // in the tree where it is else create a new top level node
    // add the rest of the symbols as children
    //
    // things like { 10 1 2 } are legal, as well as things like
    // { pwg(2699) junk(1) 5 morejunk evenmore(6) }
    //
    bmib_node_t *parent = NULL;
    bmib_node_t *curkid = NULL;

    bmib_symbol_t  *sym  = NULL;
    bmib_node_t    *node = NULL;
    bmib_node_t    *ins;
    bmib_node_t    *pins;

    bmib_token_type_t   toktype;
    bmib_keyword_t      kwtype;
    bool                tokerr;

    bool        gottok = false;
    bool        first  = true;

    enum {
            nlpsStart,
            nlpsSym,
            nlpsLeft,
            nlpsVal,
            nlpsRight,
            nlpsEnd
    }
    nlps;

    char        symname[sizeof(parser->f_token)/sizeof(char)];
    int         symval;
    bool        valdef;
    bool        symdef;
    bool        notend = true;

    butil_log(5, "Parsing Object Identifier %s\n", parser->f_lval);

    // list of symbols or numbers  may have (numeric) values with them which are checked
    //
    do
    {
        nlps       = nlpsStart;
        symname[0] = '\0';
        symval = 0;
        symdef = false;
        valdef = false;

        while (parser->f_state != mpsError && nlps != nlpsEnd)
        {
            if (! gottok)
            {
                if (bmib_get_nc_token(parser, &toktype, &kwtype, "OID", &tokerr))
                {
                    bmib_error_eof(parser, "Parsing OID", parser->f_lval);
                    break;
                }
            }
            else
            {
                gottok = false;
            }
            butil_log(5, "OID tt=%d kw=%d tok=%s\n", toktype, kwtype, parser->f_token);

            switch (toktype)
            {
            case mttPunctuation:

                if (parser->f_token[0] == '}')
                {
                    if (nlps == nlpsVal)
                    {
                        bmib_error_token(parser, "Unterminated parenthesis");
                    }
                    nlps  = nlpsEnd;
                    notend = false;
                }
                else if (parser->f_token[0] == '(')
                {
                    if (nlps != nlpsSym)
                    {
                        bmib_error_token(parser, "Unexpected");
                    }
                    nlps = nlpsVal;
                }
                else if (parser->f_token[0] == ')')
                {
                    if (nlps != nlpsRight)
                    {
                        bmib_error_token(parser, "Unbalanced parenthesis");
                    }
                    valdef = true;
                    nlps = nlpsEnd;
                }
                else if (parser->f_token[0] != ',')
                {
                    bmib_error_token(parser, "Unexpected punctuation");
                }
                break;

            case mttNumber:

                if (symdef && (nlps != nlpsVal))
                {
                    // already parsed a symbol, boot
                    nlps   = nlpsEnd;
                    gottok = true;
                    break;
                }
                symval = strtol(parser->f_token, NULL, 0);
                valdef = true;

                if (nlps == nlpsVal)
                {
                    nlps = nlpsRight;
                }
                else
                {
                    nlps = nlpsEnd;
                }
                break;

            case mttLabel:

                if (symdef || (nlps != nlpsStart))
                {
                    // already parsed a symbol, boot
                    nlps   = nlpsEnd;
                    gottok = true;
                    break;
                }
                strcpy(symname, parser->f_token);
                symdef = true;
                valdef = false;
                nlps = nlpsSym;
                break;

            default:

                break;
            }
        }
        if (parser->f_state != mpsError)
        {
            // got an oid to add to tree

            sym  = NULL;
            node = NULL;

            if (! symdef && ! valdef)
            {
                if (notend)
                {
                    bmib_error(parser, meUnexpected, "OID %s Doesn't Specify Value or Label\n", symname);
                    notend = false;
                }
                break;
            }
            if (! valdef)
            {
                symval = MAX_OID_ELEMENT;
            }
            if (symdef)
            {
                sym = bmib_find_symbol(symname, parser->f_nodesyms);
                if (sym)
                {
                    if (valdef && ((boid_t)symval != sym->f_value.u_ov->f_oid))
                    {
                        bmib_error(parser, meMultidefSym, "Redefintion of symbol ", sym->f_name);
                        notend = false;
                        break;
                    }
                    else if (! valdef)
                    {
                        symval = sym->f_value.u_ov->f_oid;
                    }
                }
                else
                {
                    butil_log(5, "Sym of name %s not found\n", symname);
                }
            }
            if (first)
            {
                first = false;

                // if node in tree already, use it as parent and skip this
                //
                if (symdef && sym)
                {
                    parent = sym->f_value.u_ov;
                    //butil_log(5, " Firstnode %d (existing %s)\n", parent->f_oid, sym->f_name);
                }
                else
                {
                    // node could be in tree already, just not symbolically
                    // (i.e. a forward reference like thing) so check that
                    //
                    for (ins = parser->f_nodes, pins = NULL; ins;)
                    {
                        if (ins->f_oid >= (boid_t)symval)
                        {
                            break;
                        }
                        pins = ins;
                        ins = ins->f_sibling;
                    }
                    if (ins && ins->f_oid == (boid_t)symval)
                    {
                        // the node IS in the tree already, it just doesn't
                        // have a symbolic reference yet
                        //
                        parent = ins;
                        //butil_log(5, " Firstnode %d (existing)\n", ins->f_oid);
                    }
                    else
                    {
                        // the node isn't in the tree, add it at the top level
                        // of the global tree for now (it gets rerouted if the
                        // symbolic ref is defined later below the top)
                        //
                        parent = bmib_create_node(parser, NULL, (boid_t)symval, NULL);
                        if (! parent)
                        {
                            bmib_error_memory(parser, "Object Node");
                            return;
                        }
                        parent->f_sibling = ins;
                        if (pins)
                        {
                            pins->f_sibling = parent;
                        }
                        else
                        {
                            parser->f_nodes = parent;
                        }
                        if (! parser->f_nodes)
                        {
                            parser->f_nodes = parent;
                        }
                        //butil_log(5, " Firstnode %d (new)\n", parent->f_oid);
                    }
                }
                curkid = parent;
            }
            else
            {
                // See if this is already a child node of previous node
                //
                for (ins = curkid->f_child; ins; ins = ins->f_sibling)
                {
                    if (ins->f_oid == (boid_t)symval)
                    {
                        break;
                    }
                }
                if (ins)
                {
                    // node already there as kid, just use it
                    //
                    node = ins;
                }
                else
                {
                    // make a new child node of current kid
                    //
                    node = bmib_create_node(parser, NULL, (boid_t)symval, curkid);
                    if (! node)
                    {
                        bmib_error_memory(parser, "Object Node");
                        return;
                    }
                    if (curkid->f_child)
                    {
                        for (ins = curkid->f_child, pins = NULL; ins;)
                        {
                            if (ins->f_oid > (boid_t)symval)
                            {
                                break;
                            }
                            pins = ins;
                            ins = ins->f_sibling;
                        }
                        node->f_sibling = ins;
                        if (pins)
                        {
                            pins->f_sibling = node;
                        }
                        else
                        {
                            curkid->f_child = node;
                        }
                    }
                    else
                    {
                        curkid->f_child = node;
                    }
                }
                curkid = node;
            }
            if (symdef)
            {
                if (! sym)
                {
                    sym = bmib_create_node_symbol(parser, symname, curkid);
                    if (! sym)
                    {
                        bmib_error_memory(parser, "Object");
                        return;
                    }
                    bmib_add_symbol(parser, sym, &parser->f_nodesyms);
                }
                curkid->f_oid = symval;
                curkid->f_sym = sym;
            }
        }
        if (parser->f_state == mpsError)
        {
            notend = false;
        }
    }
    while (notend);

    if (parser->f_state != mpsError)
    {
        if (! curkid)
        {
            bmib_error(parser, meInternal, "No node made for ", parser->f_lval);
            return;
        }
        // now add the lvalue into the last child node added
        //
        // first, see if the lvalue is already defined in the
        // node table, which would mean that it may have been
        // forward referenced, and is now being defined
        //
        sym = bmib_find_symbol(parser->f_lval, parser->f_nodesyms);

        if (! sym)
        {
            // first time seeing this symbol,
            // just add it to the list
            //
            // first, if the node already has a symbol, then
            // this is a redef of the same node
            //
            if (curkid->f_sym)
            {
                bmib_error_multidef_node(parser, curkid, parser->f_lval);
                return;
            }
            sym = bmib_create_node_symbol(parser, parser->f_lval, curkid);
            if (! sym)
            {
                bmib_error_memory(parser, "Object");
                return;
            }
            curkid->f_sym = sym;
            bmib_add_symbol(parser, sym, &parser->f_nodesyms);

            // if any type information is available
            // put it in the node, preference given
            // to locally defines ranges over type
            // defined ones
            //
            curkid->f_datatype = parser->f_objtype;
            curkid->f_access   = parser->f_access;
            curkid->f_desc     = parser->f_desc;
            curkid->f_offset   = parser->f_offset;
            curkid->f_volatile = parser->f_volatile;

            parser->f_desc = NULL;

            curkid->f_typemods = parser->f_typemods;
            if (parser->f_objtype)
            {
                curkid->f_typemods |= parser->f_objtype->f_typemods;
            }
            if (parser->f_enums)
            {
                curkid->f_enums = parser->f_enums;
            }
            else if (parser->f_objtype)
            {
                curkid->f_enums = parser->f_objtype->f_enums;
            }
            if (parser->f_ranges)
            {
                curkid->f_ranges = parser->f_ranges;
            }
            else if (parser->f_objtype)
            {
                curkid->f_ranges = parser->f_objtype->f_ranges;
            }
            if (parser->f_defvals)
            {
                curkid->f_defvals = parser->f_defvals;
            }
            if (parser->f_indices)
            {
                curkid->f_indices = parser->f_indices;
            }
        }
        else
        {
            // already had added a node with this name, so
            // if the node is parentless, we know where to
            // put it now
            //
            node = sym->f_value.u_ov;

            if (! node)
            {
                // this is really an error, but why stop?
                sym->f_value.u_ov = curkid;
                curkid->f_sym = sym;
            }
            else
            {
                if (node->f_parent || node->f_oid != MAX_OID_ELEMENT)
                {
                    // this node exists already, no need to add
                    ;
                }
                else
                {
                    // re-route this node and its kids in place of the current one
                    // and unlink the node from the sibling list at the top level

                    if (parser->f_nodes == node)
                    {
                        parser->f_nodes = node->f_sibling;
                        node->f_sibling = NULL;
                    }
                    else
                    {
                        for (ins = parser->f_nodes; ins->f_sibling && ins->f_sibling != node;)
                        {
                            ins = ins->f_sibling;
                        }
                        if (ins->f_sibling)
                        {
                            ins->f_sibling = ins->f_sibling->f_sibling;
                            node->f_sibling = NULL;
                        }
                        else
                        {
                            bmib_error(parser, meInternal,
                                    "%s: Expected existing undefined node to be at Top ", parser->f_lval);
                        }
                    }
                    curkid->f_sym   = sym;
                    curkid->f_child = node->f_child;
                    //curkid->f_val   = node->f_val;

                    for (ins = node->f_child; ins; ins = ins->f_sibling)
                    {
                        ins->f_parent = curkid;
                    }
                    node->f_child = NULL;
                    node->f_sym   = NULL;
                    bmib_destroy_node(parser, node);

                    // reroute the symbol to point to the new node
                    //
                    sym->f_value.u_ov = curkid;
                }
            }
        }
    }
    parser->f_state = mpsMIBL;
}

static void bmib_parse_object(bmib_context_t *parser)
{
    butil_log(5, "Parsing Object\n");

    if (! bmib_parse_for_keyword(parser, kwIDENTIFIER, "IDENTIFIER"))
    {
        bmib_error_expected(parser, "IDENTIFIER");
        return;
    }
    if (! bmib_parse_for_type(parser, mttEqual, "::="))
    {
        bmib_error_expected(parser, "::= after OBJECT IDENTIFIER");
        return;
    }
    parser->f_state = mpsMIBR;
}

static bool bmib_check_forward_references(bmib_context_t *parser, bmib_node_t *tree, bmib_node_t *node, int level)
{
    bmib_typespec_t *pType, *pSubType;

    if (node)
    {
        pType = node->f_datatype;

        // if the type's symbol has type-mods that the type, or node
        // doesn't, then bubble them up, but DONT bubble up types
        // from type or sym to type, or node to node
        //
        if (pType && pType->f_sym && pType->f_sym->f_value.u_tv)
        {
            pSubType = pType->f_sym->f_value.u_tv;
            if ((pSubType->f_typemods | pType->f_typemods) != pType->f_typemods)
            {
                /*
                printf("%s inherit subtype to type %04X += %04X\n",
                        node->f_sym->f_name,
                        pType->f_typemods, pSubType->f_typemods
                    );
                */
                pType->f_typemods |= pSubType->f_typemods;
            }
            if (pSubType->f_indices && ! pType->f_indices)
            {
                /*
                printf("%s inherit indices subtype to type\n",
                        node->f_sym->f_name
                    );
                */
                pType->f_indices = pSubType->f_indices;
            }
            if ((node->f_typemods | pType->f_typemods) != node->f_typemods)
            {
                /*
                printf("%s inherit type to node %04X += %04X\n",
                        node->f_sym->f_name,
                        node->f_typemods, pType->f_typemods
                    );
                */
                node->f_typemods |= pType->f_typemods;
            }
            if (pType->f_indices && ! node->f_indices)
            {
                /*
                printf("%s inherit indices type to node\n",
                        node->f_sym->f_name
                    );
                */
                node->f_indices = pType->f_indices;
            }
        }
    }
    return true;
}

static bool bmib_propagate_indexes(bmib_context_t *parser, bmib_node_t *tree, bmib_node_t *node, int level)
{
    if (node)
    {
        if (node->f_parent && node->f_parent->f_indices && ! node->f_indices)
        {
            /*
            printf("%s inherits indices from %s\n", node->f_sym->f_name,
                node->f_parent->f_sym->f_name);
            */
            node->f_indices = node->f_parent->f_indices;
        }
    }
    return true;
}

static bool bmib_check_index_references(bmib_context_t *parser, bmib_node_t *tree, bmib_node_t *node, int level)
{
    bmib_indexval_t *index;
    bmib_symbol_t *sym;

    if (node)
    {
        for (index = node->f_indices; index; index = index->f_next)
        {
            // each index should refer to a defined type
            //
            if ((sym = bmib_find_symbol(index->f_text, parser->f_nodesyms)) == NULL)
            {
                bmib_error(parser, meUndefSym, "Node \"%s\" uses undefined index \"%s\"",
                       node->f_sym->f_name, index->f_text);
            }
            else
            {
                bmib_node_t *node = sym->f_value.u_ov;

                index->f_node = node;
            }
        }
    }
    return true;
}

int bmib_parse_mib(
                    bmib_context_t *parser,
                    const char     *filename,
                    const char     *mibpath,
                    OnErrorFunc     errfunc,
                    bool            defaultnv
                  )
{
    bmib_token_type_t   toktype;
    bmib_keyword_t      kwtype;
    bool                tokerr;

    int         errcode = 0;

    FILE       *fp;
    char       *ppath;
    char       *pnp;
    char        tc;

    char        vfname[MAX_PATH];
    char        vmname[MAX_PATH];
    bmib_parse_state_t vps;
    FILE       *vfp;
    int         vlines;
    int         vtoklen;
    char        vpbc;

    parser->f_errfunc = errfunc;

    if (mibpath != parser->f_mibpath)
    {
        strncpy(parser->f_mibpath, mibpath, sizeof(parser->f_mibpath));
        parser->f_mibpath[sizeof(parser->f_mibpath)-1] = '\0';
    }
    if (filename)
    {
        ppath = parser->f_mibpath;

        // first try opening the file in the current dir
        //
        fp = fopen(filename, "rb");
        while (! fp && ppath && *ppath)
        {
            // point to end of this path and terminate
            //
            for (pnp = ppath; *pnp && *pnp != '!';)
            {
                pnp++;
            }
            tc = *pnp;
            *pnp = '\0';
            strcpy(vfname, ppath);
            *pnp = tc;
            if (*pnp == '!')
            {
                pnp++;
            }
            if (*pnp)
            {
                ppath = pnp;
            }
            else
            {
                ppath = NULL;
            }
            strcat(vfname, "/");
            strcat(vfname, filename);
            fp = fopen(vfname, "rb");
        }
        if (! fp)
        {
            bmib_error(parser, meNoFile, "Can't Open: %s", filename);
            return 1;
        }
    }
    else
    {
        filename = "stdin";
        fp = stdin;
    }

    // push the state of parent parser, cause
    // this may be a recursive call
    //
    strcpy(vfname, parser->f_fname);
    strcpy(vmname, parser->f_mibname);
    vps     = parser->f_state;
    vfp     = parser->f_fp;
    vlines  = parser->f_lines;
    vtoklen = parser->f_toklen;
    vpbc    = parser->f_pbc;

    if (! parser->f_keysyms)
    {
        bmib_load_keywords(parser);
    }
    parser->f_defaultnv = defaultnv;

    butil_log(5, "Parsing MIB: %s\n", filename);

    parser->f_files++;
    parser->f_level++;

    // set current state
    //
    parser->f_fp        = fp;
    strcpy(parser->f_fname, filename);
    parser->f_mibname[0] = 0;
    parser->f_lines     = 1;
    parser->f_inerror   = false;
    parser->f_state     = mpsTop;
    parser->f_token[0]  = 0;
    parser->f_toklen    = 0;
    parser->f_pbc       = 0;

    // Generally, parsing a mib is really
    // parsing LVALUE ::= ATTRIBS RVALUE
    // with a bunch of special cases.
    //
    while (parser->f_state != mpsError)
    {
        if (bmib_get_nc_token(parser, &toktype, &kwtype, "Token", &tokerr))
        {
            break;
        }
        butil_log(5, "state: %d  tok:%s  tt:%d kw:%d\n",
                parser->f_state, parser->f_token, toktype, kwtype);

        if (toktype == mttComment || toktype == mttControl)
        {
            continue;
        }
        switch (parser->f_state)
        {
        case mpsError:

            // abort processing
            butil_log(0, "Parsing Aborted: ", parser->f_fname);
            break;

        case mpsTop:

            // above a mib, expect a Label followed by DEFINITIONS
            //
            if (toktype != mttLabel || kwtype != kwNONE)
            {
                bmib_error_expected(parser, "MIB name (Label)");
                break;
            }
            snprintf(parser->f_mibname, sizeof(parser->f_mibname)/sizeof(char), "%s", parser->f_token);
            parser->f_state = mpsDefine;
            break;

        case mpsDefine:

            // got a mib label, should get definitions then equals
            // (ignores tokens left of equals)
            //
            if (kwtype != kwDEFINITIONS)
            {
                bmib_error_expected(parser, "DEFINITIONS");
                break;
            }
            if (bmib_get_nc_token(parser, &toktype, &kwtype, "DEFINITIONS", &tokerr))
            {
                bmib_error_eof(parser, "Parsing for :", "::=");
                break;
            }
            if (toktype != mttEqual)
            {
                bmib_error_expected(parser, "::= after DEFINITIONS");
                break;
            }
            parser->f_state = mpsBegin;
            break;

        case mpsBegin:

            // got a mib label, should get definitions then equals
            //
            if (kwtype != kwBEGIN)
            {
                bmib_error_expected(parser, "BEGIN");
                break;
            }
            parser->f_state = mpsMIBL;
            break;

        case mpsMIBL:

            // lval in mib, expect a label, or a few select keywords
            //
            if (toktype == mttLabel && kwtype == kwNONE)
            {
                // this is the lvalue
                bmib_restart_object(parser);
                parser->f_state = mpsMIBV;
                break;
            }
            else if (kwtype != kwNONE)
            {
                switch (kwtype)
                {
                case kwEND:
                    parser->f_state = mpsEnd;
                    break;

                case kwIMPORTS:
                    bmib_parse_imports(parser);
                    break;

                default:
                    // probably a macro definition of a builtin macro
                    //
                    bmib_restart_object(parser);
                    bmib_get_nc_token(parser, &toktype, &kwtype, "MACRO", &tokerr);
                    if (! tokerr)
                    {
                        if (toktype == mttLabel && kwtype == kwMACRO)
                        {
                            bmib_parse_macro(parser);
                            break;
                        }
                        else if (toktype == mttEqual)
                        {
                            parser->f_state = mpsMIBR;
                            break;
                        }
                    }
                    bmib_error_token(parser, "Unsupported LV Keyword:");
                    break;
                }
            }
            else
            {
                // non-label, non-keyword
                bmib_error_expected(parser, "Label or Keyword");
            }
            break;

        case mpsMIBV:

            // expect a macro verb or an equals
            //
            if (toktype == mttEqual)
            {
                parser->f_state = mpsMIBR;
                break;
            }
            else if (kwtype != kwNONE)
            {
                // lvalue is defined by a macro,
                switch (kwtype)
                {
                case kwOBJECT:
                    bmib_parse_object(parser);
                    break;

                case kwOBJECT_IDENTITY:
                    bmib_parse_object_identity(parser);
                    break;

                case kwOBJECT_TYPE:
                    bmib_parse_object_type(parser);
                    break;

                case kwOBJECT_GROUP:
                    bmib_parse_object_group(parser);
                    break;

                case kwNOTIFICATION_GROUP:
                case kwNOTIFICATION_TYPE:
                    bmib_parse_notification_type(parser);
                    break;

                case kwMODULE_IDENTITY:
                    bmib_parse_module_identity(parser);
                    break;

                case kwMODULE_COMPLIANCE:
                    bmib_parse_module_compliance(parser);
                    break;

                case kwMACRO:
                    bmib_parse_macro(parser);
                    break;

#ifndef NO_PREDEF_TYPES
                case kwINTEGER32:
                case kwUINTEGER32:
                case kwUNSIGNED32:
                case kwCOUNTER:
                case kwCOUNTER32:
                case kwGAUGE:
                case kwGAUGE32:
                case kwCOUNTER64:
                case kwTIMETICKS:
                case kwIPADDRESS:
                case kwNETWORKADDRESS:
                case kwNSAPADDRESS:
                    break;
#endif
                default:
                    bmib_error_token(parser, "Unsupported Verb Keyword:");
                }
            }
            else
            {
                // non-label, non-keyword
                bmib_error_expected(parser, "Label or Keyword");
            }
            break;

        case mpsMIBR:

            // expect a { } Object ID definition or OBJECT-TYPE or,
            // at the very least, a TEXTUAL-CONVENTION
            //
            if (toktype == mttPunctuation && parser->f_token[0] == '{')
            {
                bmib_parse_object_OID(parser);
                parser->f_state = mpsMIBL;
                break;
            }
            else if (
                        (toktype == mttLabel && kwtype != kwNONE) ||
                        (toktype == mttPunctuation && parser->f_token[0] == '[')
            )
            {
                switch (kwtype)
                {
                case kwTEXTUAL_CONVENTION:
                    bmib_parse_textual_convention(parser);
                    break;

                default:
                    bmib_parse_asn_type(parser, &toktype, &kwtype);
                    if (parser->f_state != mpsError)
                    {
                        // just defined a type, so place it
                        //
                        bmib_define_type(parser, parser->f_lval, parser->f_ltype);

                        // parse asn type reads till the next lval, so use it
                        //
                        bmib_restart_object(parser);
                        parser->f_state = mpsMIBV;
                    }
                    break;
                }
            }
            else
            {
                bmib_error_expected(parser, "{ ASN Value }");
            }
            break;

        case mpsMIBA:
        case mpsEnd:

            break;
        }
    }
    // back-annotate the mib to bubble up type-mods of objects
    // that were forward-referenced earlier in the mib
    //
    bmib_traverse_tree(parser, bmib_check_forward_references, parser->f_nodes, parser->f_nodes, 0);

    // for each node, if its parent has indice(s) then the child
    // is also indexed by the same indice(s) because the child is
    // an entry in that object
    //
    bmib_traverse_tree(parser, bmib_propagate_indexes, parser->f_nodes, parser->f_nodes, 0);

    // check each indexed object for valid nodes as indices
    //
    bmib_traverse_tree(parser, bmib_check_index_references, parser->f_nodes, parser->f_nodes, 0);

    char emsg[512];

    butil_log(5, "Finished Parsing: %s\n", parser->f_fname);
    butil_log(5, "Errors: %d     Warnings: %d\n", parser->f_errors, parser->f_warnings);
    butil_log(5, "Error State = %s\n", ((parser->f_state == mpsError) ? "Yes" : "No"));

    // POP state of parent parser, cause
    // this may be a recursive call
    //
    bmib_parse_state_t mystate = parser->f_state;
    if (parser->f_fp != stdin)
    {
        fclose(parser->f_fp);
    }
    parser->f_fp = NULL;

    strcpy(parser->f_fname, vfname);
    strcpy(parser->f_mibname, vmname);

    parser->f_lines     = vlines;
    parser->f_state     = vps;
    parser->f_fp        = vfp;
    parser->f_toklen    = vtoklen;
    parser->f_pbc       = vpbc;
    parser->f_level--;

    return mystate == mpsError;
}

