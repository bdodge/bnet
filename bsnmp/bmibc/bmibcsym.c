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
#include "bmibctoken.h"
#include "bmibcsym.h"
#include "bmibc.h"

bmib_enum_t *bmib_create_enum(
                                    bmib_context_t *parser,
                                    struct tag_mib_symbol *sym,
                                    int val,
                                    bmib_enum_t *sibling
                                    )
{
    bmib_enum_t *enumtype;

    enumtype = (bmib_enum_t *)malloc(sizeof(bmib_enum_t));
    if (enumtype)
    {
        enumtype->f_sym       = sym;
        enumtype->f_out       = 0;
        enumtype->f_nostring  = false;
        enumtype->f_val       = val;
        enumtype->f_next      = sibling;

        enumtype->f_nextalloc = parser->f_enumallocs;
        parser->f_enumallocs  = enumtype;
    }
    return enumtype;
}

void bmib_destroy_enum(
                                bmib_context_t *parser,
                                bmib_enum_t *enumtype
                                )
{
    if (enumtype)
    {
        free(enumtype);
    }
}

bmib_range_t *bmib_create_range(
                                bmib_context_t *parser,
                                int lowval,
                                int highval,
                                bmib_range_t *sibling
                                )
{
    bmib_range_t *range;

    range = (bmib_range_t *)malloc(sizeof(bmib_range_t));
    if (range)
    {
        range->f_lowval    = lowval;
        range->f_highval   = highval;
        range->f_next      = sibling;

        range->f_nextalloc      = parser->f_rangeallocs;
        parser->f_rangeallocs   = range;
    }
    return range;
}

void bmib_destroy_range(
                                bmib_context_t *parser,
                                bmib_range_t *range
                            )
{
    if (range)
    {
        free(range);
    }
}

bmib_typespec_t *bmib_create_type(
                                bmib_context_t *parser,
                                bmib_symbol_t *sym,
                                bmib_keyword_t kwtype,
                                bmib_typespec_t *sibling,
                                bmib_typespec_t *basetype
                                )
{
    bmib_typespec_t *type;

    type = (bmib_typespec_t *)malloc(sizeof(bmib_typespec_t));
    if (type)
    {
        type->f_sym       = sym;
        type->f_kwtype    = kwtype;
        type->f_basetype  = basetype;
        type->f_typemods  = 0;
        type->f_indices   = NULL;
        type->f_sibling   = sibling;
        type->f_enums     = NULL;
        type->f_ranges    = NULL;
        type->f_desc      = NULL;
    }
    return type;
}

void bmib_destroy_type(
                                bmib_context_t *parser,
                                bmib_typespec_t *type
                            )
{
    if (type)
    {
        if (type->f_desc)
        {
           free(type->f_desc);
        }
        free(type);
    }
}

void bmib_destroy_typelist(
                                bmib_context_t *parser,
                                bmib_typespec_t *typelist
                            )
{
    bmib_typespec_t *type;

    while (typelist)
    {
        type = typelist->f_sibling;
        bmib_destroy_type(parser, typelist);
        typelist = type;
    }
}

bmib_defval_t *bmib_create_defval(
                                bmib_context_t *parser,
                                const char *name
                                    )
{
    bmib_defval_t *defval;

    defval = (bmib_defval_t *)malloc(sizeof(bmib_defval_t));
    if (defval)
    {
        defval->f_text = malloc(strlen(name) + 1);
        if (! defval->f_text)
        {
            free(defval);
            return NULL;
        }
        strcpy(defval->f_text, name);
        defval->f_next = NULL;

        defval->f_nextalloc      = parser->f_defvalallocs;
        parser->f_defvalallocs   = defval;
    }
    return defval;
}

void bmib_destroy_defval(
                                bmib_context_t *parser,
                                bmib_defval_t *defval
                            )
{
    if (defval)
    {
        if (defval->f_text)
        {
            free(defval->f_text);
        }
        free(defval);
    }
}

bmib_indexval_t *bmib_create_index(
                                bmib_context_t *parser,
                                const char *name,
                                int dex
                                )
{
    bmib_indexval_t *indval;

    indval = (bmib_indexval_t *)malloc(sizeof(bmib_indexval_t));
    if (indval)
    {
        indval->f_text = malloc(strlen(name) + 1);
        if (! indval->f_text)
        {
            free(indval);
            return NULL;
        }
        strcpy(indval->f_text, name);
        indval->f_ordinal = dex;
        indval->f_node  = NULL;
        indval->f_next  = NULL;

        indval->f_nextalloc     = parser->f_indexallocs;
        parser->f_indexallocs   = indval;
    }
    return indval;
}

void bmib_destroy_index(
                                bmib_context_t *parser,
                                bmib_indexval_t *indval
                            )
{
    if (indval)
    {
        if (indval->f_text)
        {
            free(indval->f_text);
        }
        free(indval);
    }
}

static bmib_symbol_t *bmib_create_symbol(
                                bmib_context_t *parser,
                                const char *name
                            )
{
    static bmib_symbol_t *sym;

    sym = (bmib_symbol_t *)malloc(sizeof(bmib_symbol_t));
    if (sym)
    {
        sym->f_name = malloc(strlen(name) + 1);
        if (! sym->f_name)
        {
            free(sym);
            return NULL;
        }
        strcpy(sym->f_name, name);
        sym->f_left = NULL;
        sym->f_right = NULL;
        sym->f_type = bstNumber;
        sym->f_value.u_iv = 0;
        sym->f_value.u_tv = NULL;
    }
    return sym;
}

bmib_symbol_t *bmib_create_char_symbol(
                                bmib_context_t *parser,
                                const char *name,
                                const char *value
                                     )
{
    bmib_symbol_t *sym;

    sym = bmib_create_symbol(parser, name);
    if (sym)
    {
        sym->f_value.u_sv = malloc(strlen(value) + 1);
        if (! sym->f_value.u_sv)
        {
            bmib_destroy_symbol(parser, sym);
            return NULL;
        }
        strcpy(sym->f_value.u_sv, value);
        sym->f_type = bstString;
    }
    return sym;
}

bmib_symbol_t *bmib_create_int_symbol(
                                bmib_context_t *parser,
                                const char *name,
                                int value
                                )
{
    bmib_symbol_t *sym;

    sym = bmib_create_symbol(parser, name);
    if (sym)
    {
        sym->f_value.u_iv = value;
        sym->f_type = bstNumber;
    }
    return sym;
}

bmib_symbol_t *bmib_create_node_symbol(
                                bmib_context_t *parser,
                                const char *name,
                                bmib_node_t *node
                                )
{
    bmib_symbol_t *sym;

    sym = bmib_create_symbol(parser, name);
    if (sym)
    {
        sym->f_value.u_ov = node;
        sym->f_type = bstObject;
    }
    return sym;
}

void bmib_destroy_symbol(
                                bmib_context_t *parser,
                                bmib_symbol_t *sym
                                )
{
    if (sym)
    {
        if (sym->f_name)
        {
            free(sym->f_name);
        }
        switch (sym->f_type)
        {
        case bstNumber:
            break;
        case bstObject:
            break;
        case bstType:
            break;
        case bstEnum:
            break;
        case bstMacro:
            break;
        case bstString:
            if (sym->f_value.u_sv)
            {
                free(sym->f_value.u_sv);
            }
            break;
        }
        free(sym);
    }
}

bmib_symbol_t *bmib_add_symbol(
                                bmib_context_t *parser,
                                bmib_symbol_t  *sym,
                                bmib_symbol_t **intree
                              )
{
    bmib_symbol_t *tree;

    if (! intree)
    {
        butil_log(0, "No Tree for adding\n");
        return sym;
    }
    tree = *intree;
    if (! tree)
    {
        *intree = sym;
        return sym;
    }
    int cmp = strcmp(sym->f_name, tree->f_name);

    if (cmp < 0)
    {
        return bmib_add_symbol(parser, sym, &tree->f_left);
    }
    if (cmp > 0)
    {
        return bmib_add_symbol(parser, sym, &tree->f_right);
    }
    bool isdiff = false;

    if (sym->f_type != tree->f_type)
    {
        isdiff = true;
    }
    else
    {
        switch (tree->f_type)
        {
        case bstNumber:
            isdiff = (sym->f_value.u_iv != tree->f_value.u_iv);
            break;
        case bstString:
        case bstMacro:
        case bstType:
        case bstEnum:
            isdiff = (0 != strcmp(sym->f_value.u_sv, tree->f_value.u_sv));
            break;
        case bstObject:
            isdiff = (sym->f_value.u_ov->f_oid != tree->f_value.u_ov->f_oid);
            break;
        }
    }
    if (isdiff)
    {
        bmib_error_multidef_sym(parser, sym->f_name, tree);
    }
    else
    {
        butil_log(5, "Ignoring same-value redefinition of %s\n", sym->f_name);
        bmib_destroy_symbol(parser, sym);
    }
    return tree;
}

void bmib_dump_symtree(bmib_symbol_t *tree)
{
    if (! tree)
    {
        return;
    }
    if (tree->f_left)
    {
        bmib_dump_symtree(tree->f_left);
    }
    butil_log(5, "%s\n", tree->f_name);

    if (tree->f_right)
    {
        bmib_dump_symtree(tree->f_right);
    }
}

void bmib_destroy_symtree(
                                bmib_context_t *parser,
                                bmib_symbol_t *sym
                        )
{
    if (! sym)
    {
        return;
    }
    bmib_destroy_symtree(parser, sym->f_left);
    sym->f_left = NULL;
    bmib_destroy_symtree(parser, sym->f_right);
    sym->f_right = NULL;
    bmib_destroy_symbol(parser, sym);
}

bmib_symbol_t *bmib_find_symbol(
                                const char *name,
                                bmib_symbol_t *tree
                            )
{
    if (! tree || ! name)
    {
        return NULL;
    }
    int cmp = strcmp(name, tree->f_name);

    if (cmp < 0)
    {
        return bmib_find_symbol(name, tree->f_left);
    }
    if (cmp > 0)
    {
        return bmib_find_symbol(name, tree->f_right);
    }
    return tree;
}


bmib_node_t *bmib_create_node(
                                bmib_context_t *parser,
                                bmib_symbol_t *sym,
                                boid_t oid,
                                bmib_node_t *parent
                                )
{
    bmib_node_t *node;

    node = (bmib_node_t *)malloc(sizeof(bmib_node_t));
    if (! node)
    {
        return NULL;
    }
    node->f_sym       = sym;
    node->f_oid       = oid;
    node->f_datatype  = NULL;
    node->f_access    = maNone;
    node->f_enums     = NULL;
    node->f_ranges    = NULL;
    node->f_defvals   = NULL;
    node->f_indices   = NULL;
    node->f_parent    = parent;
    node->f_child     = NULL;
    node->f_sibling   = NULL;
    node->f_desc      = NULL;
    node->f_offset    = -1;
    node->f_volatile  = false;
    return node;
}

void bmib_destroy_node(
                                bmib_context_t *parser,
                                bmib_node_t *node
                        )
{
    if (node)
    {
        if (node->f_desc)
        {
           free(node->f_desc);
        }
        free(node);
    }
};

void bmib_destroy_nodetree(
                                bmib_context_t *parser,
                                bmib_node_t *node
                            )
{
    if (! node)
    {
        return;
    }
    bmib_destroy_nodetree(parser, node->f_child);
    bmib_destroy_nodetree(parser, node->f_sibling);
    bmib_destroy_node(parser, node);
}

bmib_node_t *bmib_add_node(
                                bmib_context_t  *parser,
                                boid_t          *oid,
                                int              oidlen,
                                bmib_node_t     *node,
                                bmib_node_t    **nodetree)
{
    bmib_node_t *ins;
    bmib_node_t *pins;
    bmib_node_t *ret;

    if (! oid || ! oidlen)
    {
        return NULL;
    }
    if (! nodetree)
    {
        return NULL;
    }
    if (! *nodetree)
    {
        // no tree at all, create a starting spot
        //
        *nodetree = ins = bmib_create_node(parser, NULL, oid[0], node);
        if (! ins)
        {
            bmib_error_memory(parser, "Object Node");
            return NULL;
        }
    }
    else
    {
        // find starting point in tree among siblings of the node
        //
        for (ins = *nodetree; ins; ins = ins->f_sibling)
        {
            if (ins->f_oid == oid[0])
            {
                break;
            }
        }
        if (! ins)
        {
            // need to make a new sibling at this level for this object
            //
            ret = bmib_create_node(parser, NULL, oid[0], (*nodetree)->f_parent);
            if (! ret)
            {
                bmib_error_memory(parser, "Object Node");
                return NULL;
            }
            for (ins = node, pins = NULL; ins;)
            {
                if (ins->f_oid > oid[0])
                {
                    break;
                }
                pins = ins;
                ins = ins->f_sibling;
            }
            ret->f_sibling = ins;
            if (pins)
            {
                pins->f_sibling = ret;
            }
            else
            {
                if ((*nodetree)->f_parent && (*nodetree)->f_parent->f_child == node)
                {
                    (*nodetree)->f_parent->f_child = ret;
                }
            }
            ins = ret;
        }
    }
    if (oidlen > 1)
    {
        oidlen--;
        oid++;
        return bmib_add_node(parser, oid, oidlen, ins, &ins->f_child);
    }
    return ins;
}

bmib_node_t *bmib_find_node(
                                boid_t *oid,
                                int oidlen,
                                bmib_node_t *tree
                           )
{
    bmib_node_t *find;

    if (! oid || ! oidlen || ! tree)
    {
        return NULL;
    }
    // start at the top of the current tree, and match the tree
    // to the oid element-per-element, stopping if node not found

    for (find = tree; find; find = find->f_sibling)
    {
        if (find->f_oid == oid[0])
        {
            break;
        }
    }
    if (! find)
    {
        return NULL;
    }
    if (oidlen > 1)
    {
        oidlen--;
        oid++;
        return bmib_find_node(oid, oidlen, find);
    }
    return find;
}


