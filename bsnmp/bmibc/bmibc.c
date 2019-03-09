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
#include "bmibc.h"
#include "bsnmp.h"
#include "butil.h"

bmib_node_t     *s_nodes       = NULL;
bmib_node_t     *s_curnode     = NULL;
bmib_symbol_t   *s_nodesyms    = NULL;
bmib_symbol_t   *s_typesyms    = NULL;

int g_offset     = 0;
int g_max_offset = 0;
int g_nvsize     = 0;
int g_nvautosize = 1;
int g_manifest   = 0;
int g_outlevel   = 1;
int g_oldstyle   = 0;
int g_defnonv    = 0;
int g_genstringids = 0;
int g_stringId   = 0x100;

// default size of NVRAM
#define NVRAM_SIZE  4096

//#define SIZE_OFFSET_MAP 32768*8
unsigned char *g_offset_map= NULL;

#if 1
#define TS_UINT8        "uint8_t"
#define TS_LPUINT8      "uint8_t*"
#define TS_UINT16       "uint16_t"
#define TS_UINT32       "uint32_t"
#define TS_INT32        "int32_t"
#define TS_UINT64       "uint64_t"
#define TS_INT64        "int64_t"
#define TS_ENUM         "int"
#else
#define TS_UINT8        "unsigned char"
#define TS_LPUINT8      "unsigned char*"
#define TS_UINT16       "unsigned short"
#define TS_UINT32       "unsigned"
#define TS_INT32        "int"
#define TS_UINT64       "unsigned long long"
#define TS_INT64        "long long"
#define TS_ENUM         "int"
#endif

#define MAX_NODE_INDEX  3

static void bmibc_gen_hdr_defs(bmib_node_t* node, FILE* file)
{
    int i, b;
    char pad[80];

    // add a define for an object's name
    //
    b = fprintf(file, "#define %s ", node->f_sym->f_name);

    for (i = 0; b < 40; i++, b++)
    {
        pad[i] = ' ';
    }
    pad[i] = '\0';
    fprintf(file, "%s%d\n", pad, g_manifest++);
}

static void bmibc_gen_hdr_enums(bmib_node_t *node, FILE *file)
{
    bmib_enum_t    *enump;
    char            enb[256];
    char            pad[128];
    char            sname[256];
    char            xname[256];
    char           *ename;
    char           *vname;
    int             nenums;
    int             i, j, k;

    // for any enumeration lists, define a list here with a generated name
    //
    // for properties that point to an enumeration type, use the type name
    // not the property name to form the enum name.  Also have to mark that
    // type as listed so other referers dont relist it here

    if (! node->f_enums)
    {
        return;
    }
    if (node->f_enums->f_out)
    {
        return;
    }
    node->f_enums->f_out = 1;

    if (node->f_datatype && node->f_datatype->f_enums)
    {
        ename = node->f_datatype->f_sym->f_name;
    }
    else
    {
        ename = node->f_sym->f_name;
    }
    strcpy(sname, ename);

    // sanitize symname for "C"
    //
    for (j = 0; j < sizeof(sname); j++)
    {
        if (! sname[j])
        {
            break;
        }
        switch (sname[j])
        {
        case '-': sname[j] = '_';
            break;
        default:
            break;
        }
    }
    j = fprintf(file, "/* enumeration for %s ", sname);

    for (i = 0; j  < 60; i++, j++)
    {
        enb[i] = '-';
    }
    enb[i] = 0;

    fprintf(file, "%s\n*/\n", enb);

    fprintf(file, "typedef enum {\n");

    for (enump = node->f_enums, nenums = 0; enump; enump = enump->f_next, nenums++)
    {
        if (enump->f_sym && enump->f_sym->f_name)
        {
            ename = enump->f_sym->f_name;
        }
        else
        {
            sprintf(enb, "%s_%d", node->f_sym->f_name, enump->f_val);
            ename = enb;
        }
        strcpy(xname, ename);

        // sanitize enumname for "C"
        for (j = 0; j < sizeof(xname); j++)
        {
            if (! xname[j])
            {
                break;
            }
            switch (xname[j])
            {
            case '-': xname[j] = '_';
                break;
            default:
                break;
            }
        }
        fprintf(file, "\t\t\t%s = %d%c\n", xname, enump->f_val, enump->f_next ? ',':' ');
    }
    fprintf(file, "}\ne_%s;\n\n", sname);

    // make a #define for each enumeration to use as configurators in source
    //
    fprintf(file, "\n\n/**** for use in ifdefs in your C code ****/\n");

    for (enump = node->f_enums, nenums = 0; enump; enump = enump->f_next, nenums++)
    {
        if (enump->f_sym && enump->f_sym->f_name)
        {
            vname = enump->f_sym->f_name;
        }
        else
        {
            sprintf(enb, "%s_%d", node->f_sym->f_name, enump->f_val);
            vname = enb;
        }
        strcpy(xname, vname);

        // sanitize enumname for "C"
        for (j = 0; j < sizeof(xname); j++)
        {
            if (! xname[j])
            {
                break;
            }
            switch (xname[j])
            {
            case '-': xname[j] = '_';
                break;
            default:
               break;
            }
        }
        for (j = 0, i = 36 - strlen(sname) - strlen(xname); i > 0; i--, j++)
        {
            pad[j] = ' ';
        }
        pad[j++] = ' ';
        pad[j] = '\0';
        fprintf(file, "#define have_%s%s%d\n", xname, pad, enump->f_val);
    }
    if (g_genstringids)
    {
        // emit a default String ID for the enum value, include a generated string header before
        // the properties header to override the string id
        //
        fprintf(file, "\n\n/**** string IDs ****/\n");

        for (enump = node->f_enums, nenums = 0; enump; enump = enump->f_next, nenums++)
        {
            if (enump->f_sym && enump->f_sym->f_name)
            {
                vname = enump->f_sym->f_name;
            }
            else
            {
                sprintf(enb, "%s_%d", node->f_sym->f_name, enump->f_val);
                vname = enb;
            }
            // sanitize enumname for "C", and take out underscores
            // which are never found in gid defines
            //
            for (j = k = 0; j < sizeof(xname); k++)
            {
                switch (vname[k])
                {
                case '-': case '_': case '+': case '@': case '!': case '&': case '%': case '^':
                case '*': case '(': case ')':
                    break;
                default:
                    xname[j++] = vname[k];
                    break;
                }
                if (! vname[k])
                {
                    break;
                }
            }
            for (j = 0, i = 36 - strlen(sname) - strlen(xname); i > 0; i--, j++)
            {
                pad[j] = ' ';
            }
            pad[j++] = ' ';
            pad[j] = '\0';
            fprintf(file, "#ifndef   gid%s\n", xname);
            fprintf(file, "#define   gid%s%s%d\n", xname, pad, g_stringId++);
            fprintf(file, "#endif\n");
        }
    }
    // also create a list of the actual legal values of the enumeration
    //
    fprintf(file, "\n\n#define ne_%s\t%d\n", sname, nenums);
    fprintf(file, "extern int ve_%s[%d];\n\n", sname, nenums);

    if (g_genstringids)
    {
        fprintf(file, "extern int ze_%s[%d];\n\n", sname, nenums);
    }
}

static void bmibc_gen_src_enums(bmib_node_t *node, FILE *file)
{
    bmib_enum_t    *enump;
    char            enb[256];
    char            sname[256];
    char            xname[256];
    char           *ename;
    int             nenums;
    int             i, j, k;

    if (! node->f_enums)
    {
        return;
    }
    if (node->f_enums->f_out)
    {
        return;
    }
    node->f_enums->f_out = 1;

    if (node->f_datatype && node->f_datatype->f_enums)
    {
        ename = node->f_datatype->f_sym->f_name;
    }
    else
    {
        ename = node->f_sym->f_name;
    }
    strcpy(sname, ename);

    // sanitize symname for "C"
    //
    for (j = 0; j < sizeof(sname); j++)
    {
        if (! sname[j])
        {
            break;
        }
        switch (sname[j])
        {
        case '-': sname[j] = '_';
            break;
        default:
            break;
        }
    }
    j = fprintf(file, "/* enumerated values for %s ", ename);

    for (i = 0; j  < 60; i++, j++)
    {
        enb[i] = '-';
    }
    enb[i] = 0;

    fprintf(file, "%s\n*/\n", enb);

    for (enump = node->f_enums, nenums = 0; enump; enump = enump->f_next)
    {
        nenums++;
    }
    fprintf(file, "int ve_%s[%d] = {\n", sname, nenums);

    for (enump = node->f_enums; enump; enump = enump->f_next)
    {
        if (enump->f_sym && enump->f_sym->f_name)
        {
            ename = enump->f_sym->f_name;
        }
        else
        {
            sprintf(enb, "%s_%d", node->f_sym->f_name, enump->f_val);
            ename = enb;
        }
        strcpy(xname, ename);

        // sanitize enumname for "C"
        for (j = 0; j < sizeof(xname); j++)
        {
            if (! xname[j])
            {
                break;
            }
            switch (xname[j])
            {
            case '-': xname[j] = '_';
                break;
            default:
               break;
            }
        }
        fprintf(file, "\t\t\t%d%c\t\t/* %s\t*/\n", enump->f_val, (enump->f_next ? ',':' '), xname);
    }
    fprintf(file, "};\n\n");

    if (g_genstringids)
    {
        fprintf(file, "int ze_%s[%d] = {\n", sname, nenums);

        for (enump = node->f_enums; enump; enump = enump->f_next)
        {
            if (node->f_enums->f_nostring && ! g_oldstyle)
            {
                fprintf(file, "\t\t\t-1%c\n", (enump->f_next ? ',':' '));
            }
            else
            {
                if (enump->f_sym && enump->f_sym->f_name)
                {
                    ename = enump->f_sym->f_name;
                }
                else
                {
                    sprintf(enb, "%s_%d", node->f_sym->f_name, enump->f_val);
                    ename = enb;
                }
                strcpy(xname, ename);

                // sanitize enumname for "C", and take out underscores
                // which are never found in gid defines
                //
                for (j = k = 0; j < sizeof(xname); k++)
                {
                    switch (ename[k])
                    {
                    case '-': case '_': case '+': case '@': case '!': case '&': case '%': case '^':
                    case '*': case '(': case ')':
                        break;
                    default:
                        xname[j++] = ename[k];
                        break;
                    }
                    if (! ename[k])
                    {
                        break;
                    }
                }
                fprintf(file, "\t\t\t%cid%s%c\n", (g_oldstyle ? 's' : 'g'), xname, (enump->f_next ? ',':' '));
            }
        }
        fprintf(file, "};\n\n");
    }
}

static int bmibc_bits_needed_for_string(bmib_range_t *range, int64_t *minv, int64_t *maxv);
static char *bmibc_encode_string(char *text, char *etext, int netext);

static int bmibc_is_type_atomic(int kwType)
{
    switch(kwType)
    {
    case kwOBJECT:
    case kwOCTET:
    case kwINTEGER:
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
    case kwBITSTRING:
    case kwBIT:
    case kwBITS:
        return 1; // type is codable as an asn type
    default:
        return 0; // type is a user type
    }
}

static int bmibc_get_node_dim(bmib_context_t *parser, bmib_node_t *node, bmib_node_t *leafs)
{
    bmib_node_t     *leaf;
    bmib_defval_t   *defval;
    bmib_indexval_t *index;
    int dim, idim;
    int indexisindex;

    if (node->f_indices)
    {
        // if node has indices, multiply the dimensions of each index to get total dimensions
        //
        for (dim = 1, index = node->f_indices; index; index = index->f_next)
        {
            indexisindex = 0;

            // get property of object that is the index of this node
            //
            for (leaf = leafs; leaf; leaf = leaf->f_sibling)
            {
                if (index->f_node)
                {
                    if (leaf->f_sym == index->f_node->f_sym)
                    {
                        // get dimensions of the property
                        //
                        if (leaf == node)
                        {
                            // this IS at one of the indices for the parent table
                            // so use it's defval dimension and stop there since the
                            // indices themselves can only be indexed up to the left indices
                            //
                            if (node->f_defvals)
                            {
                                for (idim = 0, defval = node->f_defvals; defval; defval = defval->f_next)
                                {
                                    idim++;
                                }
                            }
                            else
                            {
                                // make a default value of 1 for this index since indexes can't
                                // really be 0
                                //
                                node->f_defvals = bmib_create_defval(parser, "1");
                                if (! node->f_defvals)
                                {
                                    bmib_error_memory(parser, "Default Value");
                                    return 0;
                                }
                                idim = 1;
                            }
                            indexisindex = 1;
                            //printf("%s IS index, dim %d\n", leaf->f_sym->f_name, idim);
                        }
                        else
                        {
                            //printf("%s has index %s\n", node->f_sym->f_name, leaf->f_sym->f_name);
                            idim = bmibc_get_node_dim(parser, leaf, leafs);
                        }
                        break;
                    }
                }
            }
            if (! leaf)
            {
                fprintf(stderr, "Can't find node for index %s of node %s\n",
                        index->f_text, node->f_sym->f_name);
            }
            dim = dim * idim;
            if (indexisindex)
            {
                // one of the indices was the node itself so that means
                // we stop here
                break;
            }
        }
        // if it also has default values, check that the number matches
        // and whine if it doesn't
        //
        if (node->f_defvals)
        {
            int defdim;

            for (defdim = 0, defval = node->f_defvals; defval; defval = defval->f_next)
            {
                defdim++;
            }
            if (defdim < dim)
            {
                fprintf(stderr, "Warning, not enough initializers for %s\n",
                        node->f_sym->f_name);
            }
            else if (defdim > dim)
            {
                fprintf(stderr, "Warning, too many initializers for %s\n",
                        node->f_sym->f_name);
            }
        }
        return dim;
    }
    else
    {
        // if node has default values, dimension 1 per value unless it has only one
        // default and NO index.  It is ok to have an array property with only one
        // possible index value so if the node has an index and only one defval
        // still make it dimable
        //
        if (node->f_defvals && (node->f_defvals->f_next || node->f_indices))
        {
            for (dim = 0, defval = node->f_defvals; defval; defval = defval->f_next)
            {
                dim++;
            }
            //printf("sym %s dim is %d\n", node->f_sym->f_name, dim);
        }
        else
        {
            //printf("sym %s had no defval and no indices\n", node->f_sym->f_name);
            dim = 0;
        }
    }
    return dim;
}

void bmibc_gen_hdr_dim_store(bmib_context_t *parser, bmib_node_t *node, bmib_node_t *leafs, FILE *file)
{
    bmib_typespec_t *pType;
    bmib_defval_t   *defval;

    int   dim, actualdim;
    bool  isunsigned = false;
    bool  isstring   = false;
    char  tbuf[256];
    const char *typestr = "int";
    const char *basestr = TS_UINT8;

    for (pType = node->f_datatype; pType && pType->f_basetype;)
    {
        if (bmibc_is_type_atomic(pType->f_kwtype))
        {
            break;
        }
        pType = pType->f_basetype;
    }
    if (pType)
    {
        switch (pType->f_kwtype)
        {
        case kwOCTET:
        default:

            if (node->f_typemods & STRING_OF)
            {
                typestr = TS_LPUINT8;
                basestr = TS_UINT8;
                isunsigned = false;
                isstring = true;
            }
            else
            {
                typestr = TS_UINT8;
                isunsigned = true;
            }
            break;

        case kwINTEGER:
        case kwINTEGER32:

            isunsigned = false;
            if (node->f_typemods & STRING_OF)
            {
                typestr = "int*";
                basestr = "int";
                isunsigned = false;
                isstring = true;
            }
            else
            {
                typestr = "int";

        #if 0
                // this code would type storage for enum values based on the
                // enum type, which could be 1,2,4, etc. bytes wide, but code
                // that uses the generated records always accesses them via
                // *(int*) to that would fail, so we always store enums as ints
                //
                if (node->f_enums)
                {
                    if (! node->f_datatype->f_basetype)
                    {
                        // nodes that enumerate directly point directly to
                        // a base integer type as their first type down, so use the node's name
                        //
                        sprintf(tbuf, "e_%s", node->f_sym->f_name);
                    }
                    else
                    {
                        // node had an aliased type or typedef chain, use lower type's name
                        //
                        sprintf(tbuf, "e_%s", pType->f_sym->f_name);
                    }
                    typestr = tbuf;
                }
        #endif
            }
            break;

        case kwUINTEGER32:
        case kwUNSIGNED32:
        case kwCOUNTER:
        case kwCOUNTER32:
        case kwGAUGE:
        case kwGAUGE32:
        case kwTIMETICKS:

            isunsigned = true;
            if (node->f_typemods & STRING_OF)
            {
                typestr = TS_UINT32"*";
                basestr = TS_UINT32;
                isunsigned = false;
                isstring = true;
            }
            else
            {
                if (node->f_enums)
                {
                    typestr = TS_ENUM;
                }
                else
                {
                    typestr = TS_UINT32;
                }
            }
            break;

        case kwCOUNTER64:

            isunsigned = true;
            if (node->f_typemods & STRING_OF)
            {
                typestr = "uint64_t*";
                basestr = TS_UINT64;
                isunsigned = false;
                isstring = true;
            }
            else
            {
                if (node->f_enums)
                {
                    typestr = TS_ENUM;
                }
                else
                {
                    typestr = TS_UINT64;
                }
            }
            break;
        }
    }
    if (node->f_typemods & STRING_OF)
    {
        int64_t maxv, minv;

        bmibc_bits_needed_for_string(node->f_ranges, &minv, &maxv);

        fprintf(file, "/* storage for string value(s) for %s\n*/\n",
                node->f_sym->f_name);
        fprintf(file, "#define MAX_%s_LEN\t\t%"LIFS"\n", node->f_sym->f_name, maxv);

        dim = bmibc_get_node_dim(parser, node, leafs);
        if (dim == 0)
        {
            dim = 1;
        }
        fprintf(file, "extern       %s _vvs_%s[%"LIFS"];\n",
                basestr, node->f_sym->f_name, dim * (maxv+1));
        fprintf(file, "extern const %s _fvs_%s[/*max of %"LIFS"*/];\n",
                basestr, node->f_sym->f_name, dim * (maxv+1));
        fprintf(file, "\n\n");
    }
    else
    {
        actualdim = bmibc_get_node_dim(parser, node, leafs);
        if (actualdim > 0)
        {
            fprintf(file, "/* storage for multi-dimensional property %s\n*/\n",
                    node->f_sym->f_name);
            dim = actualdim;
        }
        else
        {
            dim = 1;
        }
        if (pType->f_kwtype == kwCOUNTER64)
        {
            fprintf(file, "/* storage for property %s which has encoding more than 32 bits\n*/\n",
                    node->f_sym->f_name);
            dim *= sizeof(uint64_t) + 1;
            typestr = TS_UINT8;
            // treat long int storage as strings
            fprintf(file, "extern       %s _vva_%s[%d];\n", typestr, node->f_sym->f_name, dim);
            fprintf(file, "extern const %s _fva_%s[%d];\n", typestr, node->f_sym->f_name, dim);
            fprintf(file, "\n\n");
        }
        else if (actualdim > 0)
        {
            fprintf(file, "extern       %s _vva_%s[%d];\n", typestr, node->f_sym->f_name, dim);
            fprintf(file, "extern const %s _fva_%s[%d];\n", typestr, node->f_sym->f_name, dim);
            fprintf(file, "\n\n");
        }
    }
}

static int bmibc_encode_bitcount(int bits)
{
    return bits;
}

#define MAX_FACTORY_STRING_LEN 2048

int bmibc_encoded_str_len(char *text)
{
    int len;
    int elen;
    unsigned char c;

    len = strlen(text);
    elen = 0;

    while (len > 0)
    {
        c = *text++;
        len--;
        if (c == '\\')
        {
            if (len > 0)
            {
                c = *text++;
                len--;
                elen++;

                if (c != '\\')
                {
                    do
                    {
                        c = *text++;
                        len--;
                    }
                    while (
                                (c >= '0' && c <= '9')      ||
                                (c >= 'a' && c <= 'f')      ||
                                (c >= 'A' && c <= 'F')      ||
                                (c == 'x' || c == 'X')
                        );
                    text--;
                    len++;
                }
            }
        }
        else
        {
            elen++;
        }
    }
    return elen;
}

//***********************************************************************
static char *bmibc_encode_string(char *text, char *etext, int netext)
{
    int len, tlen, inn, i;
    unsigned char c;

    if (! text)
    {
        text = (char*)"";
    }
    if (*text == '\"')
    {
        text++;
        if (text[strlen(text) - 1] == '\"')
        {
            text[strlen(text) - 1] = '\0';
        }
    }
    len = strlen(text);
    tlen = bmibc_encoded_str_len(text);

    if (tlen > MAX_FACTORY_STRING_LEN)
    {
        fprintf(stderr, "Factory String %s Exceeds %d, truncted\n",
                text, MAX_FACTORY_STRING_LEN);
        tlen = MAX_FACTORY_STRING_LEN;
    }
    i = 0;
    etext[i++] = '\"';

    if (tlen < 255)
    {
        i += sprintf(etext+i, "\\x%02X", tlen);
    }
    else
    {
        i += sprintf(etext+i, "\\xFF\\x%02X\\x%02X", (tlen >> 16), tlen & 0xff);
    }
    inn = 1;

    while (len > 0)
    {
        c = *text++;

        if (
                inn &&
                (
                    (c >= '0' && c <= '9')      ||
                    (c >= 'a' && c <= 'f')      ||
                    (c >= 'A' && c <= 'F')
                )
        )
        {
            i += sprintf(etext+i, "\\x%02X", c);
            inn = 1;
        }
        else if (c < 32 || c >= 127)
        {
            i += sprintf(etext+i, "\\x%02X", c);
            inn = 1;
        }
        else
        {
            etext[i++] = c;
            inn = 0;
        }
        len--;
    }
    etext[i++] = '\"';
    etext[i] = '\0';
    return etext;
}

static int bmibc_bits_needed_for_int(
                        bmib_range_t *range,
                        bmib_enum_t  *enump,
                        int64_t *minv,
                        int64_t *maxv,
                        int baseSize
                    )
{
    int64_t minval, maxval;
    uint64_t valrange;
    int bits;

    if (baseSize == 64)
    {
#if defined(_MSC_VER)
        minval = 0x8000000000000000L;
        maxval = 0x7FFFFFFFFFFFFFFFL;
#else
        minval = 0x8000000000000000LL;
        maxval = 0x7FFFFFFFFFFFFFFFLL;
#endif
    }
    else if (baseSize == 32)
    {
        minval = (int64_t)(int32_t)0x80000000UL;
        maxval = 0x7FFFFFFFL;
    }
    else if (baseSize == 16)
    {
        minval = (int64_t)(int32_t)(int16_t)0x8000U;
        maxval = 0x7FFF;
    }
    else
    {
        // default to baseSize 8
        //
        minval = (int64_t)(int32_t)(int16_t)(int8_t)0x80;
        maxval = 0x7F;
    }
    *minv = minval;
    *maxv = maxval;

    // for any enumeration list, set min/max to enum range
    //
    if (enump != NULL)
    {
        minval = maxval = (unsigned long)enump->f_val;

        while (enump)
        {
            if (enump->f_val < minval)
            {
                minval = enump->f_val;
            }
            if (enump->f_val > maxval)
            {
                maxval = enump->f_val;
            }
            enump = enump->f_next;
        }
    }

    // for any ranges, set min/max
    //
    if (range != NULL)
    {
        if (! enump)
        {
            minval = range->f_lowval;
            maxval = range->f_highval;
        }
        while (range)
        {
            if (range->f_lowval < minval)
            {
                minval = range->f_lowval;
            }
            if (range->f_highval > maxval)
            {
                maxval = range->f_highval;
            }
            range = range->f_next;
        }
    }
    if (minval == maxval && maxval == 0)
    {
        return baseSize;
    }
    valrange = maxval - minval + 1;
    if (valrange == 0)
    {
        return baseSize;
    }
    for (bits = 1; bits <= baseSize; bits++)
    {
        if (((uint64_t)1 << bits) >= valrange)
        {
            break;
        }
    }
    *minv = minval;
    *maxv = maxval;
    return bits;
}

static int bmibc_bits_needed_for_uint(
                        bmib_range_t *range,
                        bmib_enum_t  *enump,
                        uint64_t *minv,
                        uint64_t *maxv,
                        int baseSize
                      )
{
    uint64_t minval, maxval;
    uint64_t valrange;
    int bits;

    if (baseSize == 64)
    {
#if defined(_MSC_VER)
        minval = 0x0000000000000000UL;
        maxval = 0xFFFFFFFFFFFFFFFFUL;
#else
        minval = 0x0000000000000000ULL;
        maxval = 0xFFFFFFFFFFFFFFFFULL;
#endif
    }
    else if (baseSize == 32)
    {
        minval = 0x00000000UL;
        maxval = 0xFFFFFFFFUL;
    }
    else if (baseSize == 16)
    {
        minval = 0x0000U;
        maxval = 0xFFFFU;
    }
    else
    {
        // default to baseSize 8
        //
        minval = 0x00U;
        maxval = 0xFFU;
    }
    *minv = minval;
    *maxv = maxval;

    // for any enumeration list, set min/max to enum range
    //
    if (enump != NULL)
    {
        minval = maxval = (uint64_t)enump->f_val;

        while (enump)
        {
            if ((uint64_t)enump->f_val < minval)
            {
                minval = (uint64_t)enump->f_val;
            }
            if ((uint64_t)enump->f_val > maxval)
            {
                maxval = (uint64_t)enump->f_val;
            }
            enump = enump->f_next;
        }
    }

    // for any ranges, set min/max
    //
    if (range != NULL)
    {
        if (! enump)
        {
            minval = range->f_lowval;
            maxval = range->f_highval;
        }
        while (range)
        {
            if ((uint64_t)range->f_lowval < minval)
            {
                minval = (uint64_t)range->f_lowval;
            }
            if ((uint64_t)range->f_highval > maxval)
            {
                maxval = (uint64_t)range->f_highval;
            }
            range = range->f_next;
        }
    }
    if (minval == maxval && maxval == 0)
    {
        return baseSize;
    }
    valrange = maxval - minval + 1;
    if (valrange == 0)
    {
        return baseSize;
    }
    for (bits = 0; bits < baseSize; bits++)
    {
        if (((uint64_t)1 << bits) >= valrange)
        {
            break;
        }
    }
    *minv = minval;
    *maxv = maxval;
    return bits;
}

int bmibc_bits_needed_for_string(bmib_range_t *range, int64_t *minv, int64_t *maxv)
{
    int64_t minval;
    int64_t maxval;
    int64_t retval;

    minval = 0;
    maxval = 254;

    if (range != NULL)
    {
        minval = range->f_lowval;
        maxval = range->f_highval;

        while (range)
        {
            if (range->f_lowval < minval)
            {
                minval = range->f_lowval;
            }
            if (range->f_highval > maxval)
            {
                maxval = range->f_highval;
            }
            range = range->f_next;
        }
    }
    retval = maxval;
    if (maxval > minval)
    {
        // length-specified string, need more bytes
        if (maxval >= 255)
        {
            retval += 3;
        }
        else
        {
            retval += 1;
        }
    }
    *minv = minval;
    *maxv = maxval;
    return (int)retval * 8;
}

static int bmibc_bits_needed_for_object(bmib_context_t *parser, bmib_node_t *node, bool *isstring)
{
    bmib_typespec_t *type;

    int64_t     minval, maxval;
    uint64_t    uminval, umaxval;
    int         bits;
    bool        isunsigned;

    *isstring = false;

    for (type = node->f_datatype; type && type->f_basetype;)
    {
        if (bmibc_is_type_atomic(type->f_kwtype))
        {
            break;
        }
        type = type->f_basetype;
    }
    /*
    butil_log(1, "Node %s basetype %d\n", node->f_sym? node->f_sym->f_name:"??",
                type? type->f_kwtype : -1);
    */
    if (type)
    {
        switch (type->f_kwtype)
        {
        case kwOCTET:
        default:

            if (node->f_typemods & STRING_OF)
            {
                isunsigned = false;
                *isstring = true;
                bits = bmibc_bits_needed_for_string(node->f_ranges, &minval, &maxval);
            }
            else
            {
                isunsigned = true;
                bits = bmibc_bits_needed_for_uint(node->f_ranges, node->f_enums, &uminval, &umaxval, 8);
            }
            break;

        case kwINTEGER:
        case kwINTEGER32:

            isunsigned = false;
            bits = bmibc_bits_needed_for_int(node->f_ranges, node->f_enums, &minval, &maxval, 32);
            break;

        case kwUINTEGER32:
        case kwUNSIGNED32:
        case kwCOUNTER:
        case kwCOUNTER32:
        case kwGAUGE:
        case kwGAUGE32:
        case kwTIMETICKS:

            bits = bmibc_bits_needed_for_uint(node->f_ranges, node->f_enums, &uminval, &umaxval, 32);
            break;

        case kwCOUNTER64:

            isunsigned = true;
            bits = bmibc_bits_needed_for_uint(node->f_ranges, node->f_enums, &uminval, &umaxval, 64);
            break;
        }
    }
    return bits;
}

static void bmibc_gen_record(bmib_context_t *parser, bmib_node_t *node, bmib_node_t *leafs, FILE *file)
{
    bmib_typespec_t     *type;
    bmib_defval_t       *defval;
    //bmib_enum_t       *enump;
    //bmib_range_t      *range;

    int64_t minval;
    int64_t maxval;
    uint64_t uminval;
    uint64_t umaxval;

    long offset;
    int  bits;
    int  dim;
    bool  isunsigned;
    bool  isstring = false;
    const char *facval;
    const char *volval;
    //char* dimstr;
    char  facbuf[256];
    char  valbuf[256];
    const char *typestr;
    const char *methodstr;
    const char *accessstr;
    char *ename = NULL;

    minval = maxval = 0;

    // and the type is
    //
    typestr = "?";

    for (type = node->f_datatype; type && type->f_basetype;)
    {
        if (bmibc_is_type_atomic(type->f_kwtype))
        {
            break;
        }
        type = type->f_basetype;
    }
    if (! strcmp(node->f_sym->f_name, "ifHCInOctets"))
    {
        volatile int a;
        a = 1;
    }
    //printf("base type of %s is %s\n", node->f_sym->f_name, type->f_sym->f_name);
    if (type)
    {
        switch (type->f_kwtype)
        {
        case kwOCTET:
        default:

            typestr = "BMIBC_OCTET";
            if (node->f_typemods & STRING_OF)
            {
                isunsigned = false;
                isstring = true;
                bits = bmibc_bits_needed_for_string(node->f_ranges, &minval, &maxval);
            }
            else
            {
                isunsigned = true;
                bits = bmibc_bits_needed_for_uint(node->f_ranges, node->f_enums, &uminval, &umaxval, 8);
            }
            break;

        case kwINTEGER:
        case kwINTEGER32:

            isunsigned = false;
            if (node->f_enums)
            {
                typestr = "BMIBC_ENUM";
            }
            else
            {
                typestr = "BMIBC_INT32";
            }
            bits = bmibc_bits_needed_for_int(node->f_ranges, node->f_enums, &minval, &maxval, 32);
            break;

        case kwUINTEGER32:
        case kwUNSIGNED32:
        case kwCOUNTER:
        case kwCOUNTER32:
        case kwGAUGE:
        case kwGAUGE32:
        case kwTIMETICKS:

            isunsigned = true;
            if (node->f_enums)
            {
                typestr = "BMIBC_ENUM";
            }
            else
            {
                typestr = "BMIBC_UINT32";
            }
            bits = bmibc_bits_needed_for_uint(node->f_ranges, node->f_enums, &uminval, &umaxval, 32);
            break;

        case kwCOUNTER64:

            isunsigned = true;
            typestr = "BMIBC_UINT64";

            // assume counter 64 has full range always, as ranges are probably clipped
            // to 32 bits and including SNMPv2-SMI will set a 32 bit range on 32 bit cpus
        #if (1)
            bits = bmibc_bits_needed_for_uint(node->f_ranges, node->f_enums, &uminval, &umaxval, 64);
        #else
            bits = 64;
        #endif
            break;
        }
    }
    // figure dimensions of property based on count of default values
    // for multi-dimensional properties, the value pointers point to
    // other allocated arrays instead of holding the data directly
    // Also for data types > 32 bits, use array method
    //
    dim = bmibc_get_node_dim(parser, node, leafs);

    if (node->f_typemods & STRING_OF)
    {
        if (node->f_defvals)
        {
            sprintf(facbuf, "_fvs_%s", node->f_sym->f_name);
            facval = facbuf;
        }
        else
        {
            facval = "NULL";
        }
        sprintf(valbuf, "_vvs_%s", node->f_sym->f_name);
        volval = valbuf;
    }
    else
    {
        if (dim > 0 || bits > 32 /*|| type == kwCOUNTER64 handled by forcing > 32 bits above*/)
        {
            if (dim > 0)
            {
                sprintf(facbuf, "_fva_%s", node->f_sym->f_name);
                facval = facbuf;
            }
            else
            {
                if (node->f_defvals)
                {
                    sprintf(facbuf, "_fva_%s", node->f_sym->f_name);
                    facval = facbuf;
                }
                else
                {
                    facval = "NULL";
                }
            }
            sprintf(valbuf, "_vva_%s", node->f_sym->f_name);
            volval = valbuf;
        }
        else
        {
            if (node->f_defvals)
            {
                facval = node->f_defvals->f_text;
            }
            else
            {
                facval = "NULL";
            }
            volval = "NULL";
        }
    }
    // what type of access does this have?
    //
    switch (node->f_access)
    {
    case maNone:
    default:
        accessstr = "BMIBC_NONE";
        break;
    case maReadOnly:
    case maReadCreate:
        accessstr = "BMIBC_READ";
        break;
    case maReadWrite:
        accessstr = "BMIBC_READWRITE";
        break;
    case maTrap:
        accessstr = "BMIBC_NONE /* TRAP ONLY */";
        break;
    }
    // method is direct
    //
    methodstr = "BMIBC_DIRECT";

    if (! node->f_volatile)
    {
        if (node->f_offset >= 0)
        {
            offset = node->f_offset;
        }
        else
        {
            int b, o, byte;
            unsigned char mask;

            // always byte align strings and long long qtys
            if (isstring || bits > 32)
            {
                while (g_offset & 0x7)
                {
                    g_offset++;
                }
            }
            if (g_offset_map)
            {
                // bump offset until enough room in offsetmap
                // for this object
                //
                do
                {
                    for (b = bits, o = g_offset; b > 0;)
                    {
                        byte = o >> 3;
                        mask = 0xFF;
                        if (byte == (g_offset >> 3))
                        {
                            mask >>= (o & 7);
                        }
                        if (byte == ((g_offset + bits) >> 3))
                        {
                            mask &= ~(0xFF >> ((o + b) & 7));
                        }
                        if (g_offset_map[byte] & mask)
                        {
                            break;
                        }
                        b-= 8 - (o & 7);
                        o+= 8 - (o & 7);
                    }
                    if (b > 0)
                    {
                        if (isstring)
                        {
                            g_offset+= 8;
                        }
                        else
                        {
                            g_offset++;
                        }
                    }
                    if ((g_offset >> 3) >= g_nvsize)
                    {
                        fprintf(stderr, "Out of room for object %s at offset %d for nv-size %d\n",
                                node->f_sym->f_name, g_offset, g_nvsize);
                        exit(6);
                    }
                }
                while (b > 0);

                // color the map with bits being allocated
                //
                for (b = bits, o = g_offset; b > 0; b-= 8, o+= 8)
                {
                    byte = o >> 3;
                    mask = 0xFF >> (o & 7);
                    if (((o + b) >> 3) == byte)
                    {
                        mask &= ~(0xFF >> ((o + b) & 7));
                    }
                    g_offset_map[byte] |= mask;
                }
            }
            offset = g_offset;
            g_offset += bits;
        }
        if ((offset + bits) > g_max_offset)
        {
            g_max_offset = offset + bits;
        }
    }
    else
    {
        offset = -1;
    }
    fprintf(file, "/********* %s \n", node->f_sym->f_name);
    fprintf(file, "*/\n");
    fprintf(file, "{\n");
    fprintf(file, "  /*   name:*/ \"%s\",\n",          node->f_sym->f_name);

    // for now at least, dont allow initializers outside valid range
    //
    if (isunsigned)
    {
#ifdef _MSC_VER
        if (umaxval & 0xFFFFFFFF00000000UL)
#else
        if (umaxval & 0xFFFFFFFF00000000ULL)
#endif
        {
            // if umax is just maxint, this isn't a warning because ignoring
            // the range still gets what we want
#ifdef _MSC_VER
            if (umaxval != 0xFFFFFFFFFFFFFFFFUL)
#else
            if (umaxval != 0xFFFFFFFFFFFFFFFFULL)
#endif
            {
                fprintf(stderr, "Warning: unsigned maximum is >32bits, ignoring\n");
            }
            // both 0 means "dont check range"
            umaxval = 0x0;
        }
#ifdef _MSC_VER
        if (uminval & 0xFFFFFFFF00000000UL)
#else
        if (uminval & 0xFFFFFFFF00000000ULL)
#endif
        {
            fprintf(stderr, "Warning: unsigned minimum is >32bits, ignoring\n");
            // both 0 means "dont check range"
            uminval = 0x0;
        }
    }
    else
    {
        if (maxval > 0)
        {
#ifdef _MSC_VER
            if (maxval & 0xFFFFFFFF00000000UL)
#else
            if (maxval & 0xFFFFFFFF00000000ULL)
#endif
            {
                // if umax is just maxint, this isn't a warning because ignoring
                // the range still gets what we want
#ifdef _MSC_VER
                if (umaxval != 0x7FFFFFFFFFFFFFFFUL)
#else
                if (umaxval != 0x7FFFFFFFFFFFFFFFULL)
#endif
                {
                    fprintf(stderr, "Warning: signed positive max is >32bits, ignoring\n");
                }
                // both 0 means "dont check range"
                maxval = 0x0;
                minval = 0x0;
            }
        }
        else if(maxval < 0)
        {
#ifdef _MSC_VER
            if ((maxval & 0xFFFFFFFF00000000UL) != 0xFFFFFFFF00000000UL)
#else
            if ((maxval & 0xFFFFFFFF00000000ULL) != 0xFFFFFFFF00000000ULL)
#endif
            {
                fprintf(stderr, "Warning: signed negative max is >32bits, ignoring\n");
                // both 0 means "dont check range"
                maxval = 0x0;
                minval = 0x0;
            }
        }
        if (minval > 0)
        {
#ifdef _MSC_VER
            if (minval & 0xFFFFFFFF00000000UL)
#else
            if (minval & 0xFFFFFFFF00000000ULL)
#endif
            {
                fprintf(stderr, "Warning: signed positive min is >32bits, ignoring\n");
                // both 0 means "dont check range"
                maxval = 0x0;
                minval = 0x0;
            }
        }
        else if(minval < 0)
        {
#ifdef _MSC_VER
            if ((minval & 0xFFFFFFFF00000000UL) != 0xFFFFFFFF00000000UL)
#else
            if ((minval & 0xFFFFFFFF00000000ULL) != 0xFFFFFFFF00000000ULL)
#endif
            {
                fprintf(stderr, "Warning: signed negative min is >32bits, ignoring\n");
                // both 0 means "dont check range"
                maxval = 0x0;
                minval = 0x0;
            }
        }
    }
    if (isunsigned)
    {
        fprintf(file, "  /*min,max:*/ %"LUFS"UL, %"LUFS"UL,\n",
                uminval, umaxval);
    }
    else
    {
        int minint = 0;

        // special case max negative number to account for C99's constant limitiation
        //
        switch (bits)
        {
        case 32:
            if (
#if defined(_MSC_VER)
                (uint64_t)minval == (uint64_t)0xffffffff80000000UL
#else
                (uint64_t)minval == (uint64_t)0xffffffff80000000ULL
#endif
            )
            {
                fprintf(file, "  /*min,max:*/ 0x80000000UL /* min-int */, %"LIFS",\n", maxval);
                minint = 1;
            }
            break;
        case 64:
            // this is only valid if minval and maxval are 64 bits in the properties record
            // which they aren't (for now) so this never hits
            if (
#if defined(_MSC_VER)
                (uint64_t)minval == (uint64_t)0x8000000000000000UL
#else
                (uint64_t)minval == (uint64_t)0x8000000000000000ULL
#endif
            )
            {
                fprintf(file,
                    "  /*min,max:*/ 0x8000000000000000ULL /* min-int */, %"LIFS",\n", maxval);
                minint = 1;
            }
            break;
        default:
            break;
        }
        if (! minint)
        {
            fprintf(file, "  /*min,max:*/ %"LIFS", %"LIFS",\n", minval, maxval);
        }
    }
    fprintf(file, "  /*    dim:*/ %d,\n",          dim);
    fprintf(file, "  /* offset:*/ %ld,\n",         offset);
    fprintf(file, "  /*encbits:*/ %d,\n",          bmibc_encode_bitcount(bits));
    fprintf(file, "  /*   type:*/ %s,\n",          typestr);
    fprintf(file, "  /* method:*/ %s,\n",          methodstr);
    fprintf(file, "  /* access:*/ %s,\n",          accessstr);
    fprintf(file, "  /*subsrib:*/ NULL,\n"         );
    fprintf(file, "  /*fac val:*/ (void*)%s,\n",   facval);
    fprintf(file, "  /*vol val:*/ (void*)%s\n",    volval);
    fprintf(file, "}%c\n\n", node->f_sibling ? ',' : ' ' );
}

static void bmibc_gen_src_dim_store(bmib_context_t *parser, bmib_node_t *node, bmib_node_t *leafs, FILE *file)
{
    bmib_typespec_t *type;
    bmib_defval_t   *defval;

    int   dim, actualdim;
    bool  isunsigned = false;
    bool  isstring   = false;
    char  tbuf[256];
    const char *typestr = "int";
    const char *basestr = TS_UINT8;

    for (type = node->f_datatype; type && type->f_basetype;)
    {
        if (bmibc_is_type_atomic(type->f_kwtype))
        {
            break;
        }
        type = type->f_basetype;
    }
    if (type)
    {
        switch (type->f_kwtype)
        {
        case kwOCTET:
        default:

            if (node->f_typemods & STRING_OF)
            {
                typestr = TS_LPUINT8;
                basestr = TS_UINT8;
                isunsigned = false;
                isstring = true;
            }
            else
            {
                typestr = TS_UINT8;
                isunsigned = true;
            }
            break;

        case kwINTEGER:
        case kwINTEGER32:

            isunsigned = false;
            if (node->f_typemods & STRING_OF)
            {
                typestr = "int*";
                basestr = "int";
                isunsigned = false;
                isstring = true;
            }
            else
            {
                typestr = "int";
        #if 0
                // this code would type storage for enum values based on the
                // enum type, which could be 1,2,4, etc. bytes wide, but code
                // that uses the generated records always accesses them via
                // *(int*) to that would fail, so we always store enums as ints
                //
                if (node->f_enums)
                {
                    if (! node->f_datatype->f_basetype)
                    {
                        // nodes that enumerate directly point directly to
                        // a base integer type as their first type down, so use the node's name
                        //
                        sprintf(tbuf, "e_%s", node->f_sym->f_name);
                    }
                    else
                    {
                        // node had an aliased type or typedef chain, use lower type's name
                        //
                        sprintf(tbuf, "e_%s", type->f_sym->f_name);
                    }
                    typestr = tbuf;
                }
        #endif
            }
            break;

        case kwUINTEGER32:
        case kwUNSIGNED32:
        case kwCOUNTER:
        case kwCOUNTER32:
        case kwGAUGE:
        case kwGAUGE32:
        case kwTIMETICKS:

            isunsigned = true;
            if (node->f_typemods & STRING_OF)
            {
                typestr = TS_UINT32"*";
                basestr = TS_UINT32;
                isunsigned = false;
                isstring = true;
            }
            else
            {
                if (node->f_enums)
                {
                    typestr = TS_ENUM;
                }
                else
                {
                    typestr = TS_UINT32;
                }
            }
            break;

        case kwCOUNTER64:

            isunsigned = true;
            if (node->f_typemods & STRING_OF)
            {
                typestr = TS_UINT64"*";
                basestr = TS_UINT64;
                isunsigned = false;
                isstring = true;
            }
            else
            {
                if (node->f_enums)
                {
                    typestr = TS_ENUM;
                }
                else
                {
                    typestr = TS_UINT64;
                }
            }
            break;
        }
    }
    actualdim = bmibc_get_node_dim(parser, node, leafs);
    if (actualdim == 0)
    {
        dim = 1;
    }
    else
    {
        dim = actualdim;
    }
    if (node->f_typemods & STRING_OF)
    {
        int64_t maxv, minv;
        char *facval;

        bmibc_bits_needed_for_string(node->f_ranges, &minv, &maxv);

        fprintf(file, "/* storage for string value(s) for %s\n*/\n",
                node->f_sym->f_name);
        fprintf(file, "%s _vvs_%s[%"LIFS"];\n", basestr, node->f_sym->f_name, dim * (maxv+1));

        if (node->f_defvals)
        {
            bmib_defval_t*    defval;
            static char etext[MAX_FACTORY_STRING_LEN * 4];

            fprintf(file, "const %s _fvs_%s[/*max of %"LIFS"*/] = {\n", basestr, node->f_sym->f_name, dim * (maxv+1));

            for (defval = node->f_defvals; defval; defval = defval->f_next)
            {
                facval = bmibc_encode_string(defval->f_text, etext, sizeof(etext));
                fprintf(file, "        %s\n", etext);
            }
            fprintf(file, "};");
        }
        fprintf(file, "\n\n");
    }
    else
    {
        if (actualdim > 0)
        {
            fprintf(file, "/* storage for multi-dimensional property %s\n*/\n",
                    node->f_sym->f_name);
        }
        if (type->f_kwtype == kwCOUNTER64)
        {
            int64_t sll;

            fprintf(file, "/* storage for property %s which has encoding more than 32 bits\n*/\n",
                    node->f_sym->f_name);

            dim *= sizeof(uint64_t) + 1;
            typestr = TS_UINT8;

            // treat long int storage as strings
            fprintf(file, "%s _vva_%s[%d];\n", typestr, node->f_sym->f_name, dim);
            fprintf(file, "const %s _fva_%s[%d] = {\n", typestr, node->f_sym->f_name, dim);

            for (defval = node->f_defvals; defval; defval = defval->f_next)
            {
                // note that this does NOT handle integers > max32!!!
                sll = (int64_t)strtol(defval->f_text, NULL, 0);

                // note little-endian layout!
                fprintf(file,
                        "        8,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X%s",
                        (unsigned char)(sll),
                        (unsigned char)(sll >> 8),
                        (unsigned char)(sll >> 16),
                        (unsigned char)(sll >> 24),
                        (unsigned char)(sll >> 32),
                        (unsigned char)(sll >> 40),
                        (unsigned char)(sll >> 48),
                        (unsigned char)(sll >> 56),
                        defval->f_next ? ",\n" : "\n");
            }
            fprintf(file, "};\n\n");
        }
        else if (actualdim > 0)
        {
            fprintf(file, "      %s _vva_%s[%d];\n", typestr, node->f_sym->f_name, dim);
            fprintf(file, "const %s _fva_%s[%d] = { ", typestr, node->f_sym->f_name, dim);
            for (defval = node->f_defvals; defval; defval = defval->f_next)
            {
                fprintf(file, "%s%s ", defval->f_text, defval->f_next ? "," : "");
            }
            fprintf(file, "};\n\n");
        }
    }
}

static void bmibc_gen_enum_ptr(bmib_node_t *node, FILE *file)
{
    bmib_typespec_t *type;
    bmib_enum_t     *enump;

    char  *ename = NULL;

    type = node->f_datatype;

    if (! node->f_enums)
    {
        if (g_genstringids)
        {
            fprintf(file, "\t\t\t{ 0, NULL, NULL }%c\n", node->f_sibling ? ',' : ' ');
        }
        else
        {
            fprintf(file, "\t\t\t{ 0, NULL }%c\n", node->f_sibling ? ',' : ' ');
        }
        return;
    }
    else
    {
        if (node->f_datatype && node->f_datatype->f_enums)
        {
            ename = node->f_datatype->f_sym->f_name;
        }
        else
        {
            ename = node->f_sym->f_name;
        }
        if (g_genstringids)
        {
            fprintf(file, "\t\t\t{ ne_%s, ve_%s, ze_%s }%c\n", ename, ename, ename, node->f_sibling ? ',' : ' ');
        }
        else
        {
            fprintf(file, "\t\t\t{ ne_%s, ve_%s }%c\n", ename, ename, node->f_sibling ? ',' : ' ');
        }
    }
}

static uint8_t bmibc_type_for_keyword(bmib_keyword_t kwType)
{
    switch(kwType)
    {
    case kwOBJECT:      return SNMP_OBJECT_ID;
    case kwOCTET:       return SNMP_OCTET_STRING;
    case kwINTEGER:     return SNMP_INTEGER;
    case kwINTEGER32:   return SNMP_INTEGER;
    case kwUINTEGER32:  return SNMP_UNSIGNED;
    case kwUNSIGNED32:  return SNMP_UNSIGNED;
    case kwCOUNTER:     return SNMP_COUNTER;
    case kwCOUNTER32:   return SNMP_COUNTER;
    case kwGAUGE:       return SNMP_GAUGE;
    case kwGAUGE32:     return SNMP_GAUGE;
    case kwCOUNTER64:   return SNMP_COUNTER64;
    case kwTIMETICKS:   return SNMP_TIMETICKS;
    case kwIPADDRESS:   return SNMP_IPADDRESS;
    case kwNETWORKADDRESS: return SNMP_IPADDRESS;
    case kwNSAPADDRESS: return SNMP_IPADDRESS;
    case kwBITSTRING:
    case kwBIT:
    case kwBITS:        return SNMP_BIT_STRING;
    default:
        fprintf(stderr, "MIBC - Unhandled kwtype %d\n", kwType);
        return          SNMP_INTEGER;
    }
}

static void bmibc_gen_OID_xref(bmib_node_t *node, bmib_node_t *leafs, boid_t *oid, int oidlen, int fragoff, FILE *file)
{
    bmib_node_t     *parent;
    bmib_node_t     *leaf;
    bmib_typespec_t *type;
    bmib_indexval_t *index;
    bmib_defval_t   *defval;

    int typeid;
    int typemods;
    int dex;
    int propnum;
    int indices[MAX_NODE_INDEX];

    char *namestr;
    const char *oidstr;
    const char *oidfrag;
    char oidbuf[1024];

    // resolve base type for object, or at least as far back an atomic type
    //
    for (type = node->f_datatype; type && type->f_basetype;)
    {
        if (bmibc_is_type_atomic(type->f_kwtype))
        {
            break;
        }
        type = type->f_basetype;
    }
    typeid = type->f_kwtype;

    typemods = 0;
    for (dex = 0; dex < MAX_NODE_INDEX; dex++)
    {
        indices[dex] = 0;
    }
    for (parent = node; parent; parent = parent->f_parent)
    {
        type = parent->f_datatype;
        if (type && type->f_kwtype == kwOBJECT)
        {
            typeid = kwOBJECT;
        }
    }
    typemods |= node->f_typemods;

    for (dex = 0, index = node->f_indices; index; index = index->f_next, dex++)
    {
        // get property enum of object that is the index
        //
        for (leaf = leafs, propnum = 0; leaf; leaf = leaf->f_sibling, propnum++)
        {
            if (index->f_node)
            {
                if (leaf->f_sym == index->f_node->f_sym)
                {
                    indices[dex] = propnum;
                    break;
                }
            }
        }
        if (! leaf)
        {
            fprintf(stderr, "Can't find node for index %s of node %s\n",
                    index->f_text, node->f_sym->f_name);
        }
    }
    // convert internal kwtype to the ASN.1 BER value for filing
    //
    typeid = bmibc_type_for_keyword(typeid);

    if (typemods & STRING_OF)
    {
        typeid |= 0x80;
    }
    // full oid
    oidstr  = bmib_format_oid(oid, oidlen);
    strncpy(oidbuf, oidstr, sizeof(oidbuf) - 1);
    oidbuf[sizeof(oidbuf) - 1] = '\0';

    oidfrag = bmib_format_oid(oid + fragoff, oidlen - fragoff);
    namestr = node->f_sym->f_name;

    fprintf(file, "\t{ \"%s%s\", %s, %d, {",
            (fragoff ? "." : ""), oidfrag, namestr, typeid);

    for (dex = 0; dex < MAX_NODE_INDEX; dex++)
    {
        fprintf(file, " %d%c",
                indices[dex], dex < (MAX_NODE_INDEX - 1) ? ',' : ' ');
    }
    fprintf(file, "} }%c    /* %s */\n",
            node->f_sibling ? ',' : ' ',
            oidbuf
            );
}

static bool bmibc_parser_err_callback(bmib_error_t code, const char *msg)
{
    const char *cn;
    int level;

    switch (code)
    {
    case meInfo:
        level = 0;
        cn = "Info";
        break;
    case meWarning:
        level = 1;
        cn = "Info";
        break;
    default:
    case meError:
    case meEOF:
    case meNoFile:
    case meUndefSym:
    case meMultidefSym:
    case meExpected:
    case meUnexpected:
    case meUnsupported:
    case meMemory:
    case meInternal:
        level = 2;
        cn = "Error";
        break;
    }
    if (level >= g_outlevel)
    {
        fprintf(stderr, "MIBC %s: %s\n", cn, msg);
    }
    return false;
}

static int bmibc_useage_blurb(const char *progname)
{
    fprintf(stderr, "Use: %s [-rnx] -o outfile infile [infile2 ...]\n", progname);
    fprintf(stderr, "     -l    Set debug log level (default 1: errors/warnings only)\n");
    fprintf(stderr, "     -r    Specify target NV-RAM storage size in bytes (default 4096)\n");
    fprintf(stderr, "     -n    Assume all properties are non-volatile (default assume volatile)\n");
    fprintf(stderr, "     -s    Generate string ids for enums (default 0: no)\n");
    return 1;
}

static int bmibc_nofile_blurb(const char *name)
{
    fprintf(stderr, "Can't open file: %s\n", name);
    return 1;
}

static int bmibc_nomem_blurb(const char *msg)
{
    fprintf(stderr, "Can't allocate: %s\n", msg);
    return 1;
}

#define NVSIGSIZE   32

int main(int argc, char **argv)
{
    bmib_context_t *parser;

    FILE       *infile;
    FILE       *hdrfile;
    FILE       *srcfile;
    char       *progname;
    char       *infilename;
    char        infilepath[MAX_PATH*2];
    char        infilepaths[MAX_PATH*MAX_INCLS];
    char        hdrfilename[MAX_PATH];
    char        srcfilename[MAX_PATH];
    char        nvramStamp[NVSIGSIZE + 1];
    int         loglevel;
    int         err;

    boid_t      oid[1024];
    boid_t      prevoid[1024];
    int         oidlen;
    int         prevoidlen;
    int         isfrag;

    bmib_symbol_t  *signaturesym;
    bmib_symbol_t  *sym;
    bmib_node_t    *node;
    bmib_node_t    *leafs;
    bmib_node_t    *curleaf;
    bmib_node_t    *leaf;

    time_t      now;
    int         nleafs;
    int         i;

    loglevel = 2;
    butil_set_log_level(loglevel);

    progname = *argv;
    if (argc < 4)
    {
        return bmibc_useage_blurb(progname);
    }
    argc--;
    argv++;

    hdrfilename[0] = '\0';
    infilepaths[0] = '\0';
    infilepaths[1] = '\0';

#ifdef BMEM_H
    static uint8_t g_lpool[0x100000];
    static uint8_t g_spool[0x400000];
    static uint8_t g_mpool[0x400000];

    void *pool;
    size_t pool_size, pool_chunk;
    size_t size;
    int result;

    result = bmem_init();
    if (result)
    {
        return bmibc_nomem_blurb("Can't init pool\n");
    }
    pool_size = sizeof(g_spool);
    pool_chunk = 128;
    pool = g_spool;
    result = bmem_add_pool(pool, pool_size, pool_chunk);
    if (result)
    {
        return bmibc_nomem_blurb("Can't add small pool\n");
    }
    pool_size = sizeof(g_mpool);
    pool_chunk = 512;
    pool = g_mpool;
    result = bmem_add_pool(pool, pool_size, pool_chunk);
    if (result)
    {
        return bmibc_nomem_blurb("Can't add medium pool\n");
    }
    pool_size = sizeof(g_lpool);
    pool_chunk = 16384;
    pool = g_lpool;
    result = bmem_add_pool(pool, pool_size, pool_chunk);
    if (result)
    {
        return bmibc_nomem_blurb("Can't add large pool\n");
    }
#endif
    while (argc > 0 && *argv && *argv[0] == '-')
    {
        if (argv[0][1] == 'o')
        {
            if (argc > 1)
            {
                argc--;
                argv++;
                strcpy(hdrfilename, *argv);
                strcat(hdrfilename, ".h");
            }
            else
            {
                return bmibc_useage_blurb(progname);
            }
        }
        else if (argv[0][1] == 'l')
        {
            if (argv[0][2] == '\0')
            {
                if (argc > 1)
                {
                    argc--;
                    argv++;
                    loglevel = strtoul(*argv, NULL, 0);
                }
                else
                {
                    return bmibc_useage_blurb(progname);
                }
            }
            else
            {
                loglevel = strtoul((*argv + 2), NULL, 0);
            }
        }
        else if (argv[0][1] == 'r')
        {
            if (argv[0][2] == '\0')
            {
                if (argc > 1)
                {
                    argc--;
                    argv++;
                    g_nvsize = strtoul(*argv, NULL, 0);
                    g_nvautosize = 0;
                }
                else
                {
                    return bmibc_useage_blurb(progname);
                }
            }
            else
            {
                g_nvsize = strtoul((*argv + 2), NULL, 0);
                g_nvautosize = 0;
            }
        }
        else if (argv[0][1] == 'n')
        {
            g_defnonv = 1;
        }
        else if (argv[0][1] == 's')
        {
            g_genstringids = 1;
        }
        else if (argv[0][1] == 'I')
        {
            int len;
            const char *pip;

            // add path to include paths
            //
            pip = *argv + 2;
            if (! *pip)
            {
                if (argc > 0)
                {
                    argv++;
                    argc--;
                    pip = *argv;
                }
            }
            if (*pip)
            {
                len = strlen(infilepaths);
                snprintf(infilepaths + len, sizeof(infilepaths) - len, "%s%s",
                        (len > 0 ? "!" : ""), argv[0] + 2);
            }
        }
        else
        {
            return bmibc_useage_blurb(progname);
        }
        argc--;
        argv++;
    }
    butil_set_log_level(loglevel);

    if (! hdrfilename[0] || argc < 1)
    {
        return bmibc_useage_blurb(progname);
    }
    parser = bmib_create_parser();
    if (! parser)
    {
        return bmibc_nomem_blurb("parser\n");
    }
    if (g_nvsize <= 0)
    {
        g_nvsize = NVRAM_SIZE;
        butil_log(1, "Warning: No NVRAM size specified, using default max of %d, fit to use\n", g_nvsize);
    }
    if (g_nvsize > 0)
    {
        g_offset_map = (uint8_t *)malloc(g_nvsize);
        if (! g_offset_map)
        {
            bmib_error(parser, meMemory, "Can't alloc map\n");
            return -1;
        }
        memset(g_offset_map, 0, g_nvsize);
    }
    else
    {
        g_offset_map = 0;
    }
    while (argc-- > 0)
    {
        if (! (infile = fopen(*argv, "rb")))
        {
            return bmibc_nofile_blurb(*argv);
        }
        fclose(infile);
        strcpy(infilepath, *argv++);
        for (
                infilename = infilepath + strlen(infilepath) - 1;
                infilename >= infilepath;
                infilename--
        )
        {
            if (*infilename == '/' || *infilename == '\\')
            {
                break;
            }
        }
        if (infilename >= infilepath)
        {
            int len;

            *infilename = '\0';

            // add path of file to include file path
            //
            len = strlen(infilepaths);
            snprintf(infilepaths + len, sizeof(infilepaths) - len, "%s%s",
                    (len > 0 ? "!" : ""), infilepath);
        }
        infilename++;

        err = bmib_parse_mib(
                            parser,
                            infilename,
                            infilepaths,
                            bmibc_parser_err_callback,
                            g_defnonv
                            );

        if (err)
        {
            fprintf(stderr, "MIBC - Parsing errors\n");
            return err;
        }
    }
    s_nodes       = parser->f_nodes;
    s_nodesyms    = parser->f_nodesyms;
    s_typesyms    = parser->f_typesyms;

    // Add fixed internal signature leaf as signature
    //
    time(&now);
    i = sprintf(nvramStamp, "NV-%s", ctime(&now));
    if (i > 0 && nvramStamp[i-1] == '\n')
    {
        i--;
    }
    while (i < NVSIGSIZE)
    {
        nvramStamp[i++] = ' ';
    }
    nvramStamp[i] = '\0';

    signaturesym = bmib_create_char_symbol(parser, "NVRAMSIGNATURE", "");
    if (! signaturesym)
    {
        return bmibc_nomem_blurb("Signature");
    }
    leafs = bmib_create_node(parser, signaturesym, 1, NULL);
    if (! leafs)
    {
        return bmibc_nomem_blurb("Root leaf");
    }
    leafs->f_parent = NULL;
    sym = bmib_find_symbol("OCTET", s_typesyms);
    leafs->f_datatype  = sym->f_value.u_tv;
    leafs->f_typemods  = STRING_OF;
    leafs->f_access    = maReadOnly;
    leafs->f_enums     = NULL;
    leafs->f_ranges    = bmib_create_range(parser, NVSIGSIZE, NVSIGSIZE, NULL);
    if (! leafs->f_ranges)
    {
        return bmibc_nomem_blurb("Range");
    }
    leafs->f_defvals   = bmib_create_defval(parser, nvramStamp);
    if (! leafs->f_defvals)
    {
        return bmibc_nomem_blurb("Default Value");
    }
    leafs->f_indices   = NULL;

    // traverse the node tree, placing each leaf node
    // into a new list of leafs only
    //
    for (node = s_nodes, curleaf = leafs, nleafs = 1; node;)
    {
        if (! node->f_child && node->f_datatype)
        {
            leaf = bmib_create_node(parser, node->f_sym, node->f_oid, NULL);
            if (! leaf)
            {
                return bmibc_nomem_blurb("New leaf");
            }
            if (! curleaf)
            {
                leafs = leaf;
            }
            else
            {
                curleaf->f_sibling = leaf;
            }
            curleaf = leaf;
            leaf->f_parent     = node->f_parent;
            leaf->f_datatype   = node->f_datatype;
            leaf->f_typemods   = node->f_typemods;
            leaf->f_access     = node->f_access;
            leaf->f_enums      = node->f_enums;
            leaf->f_ranges     = node->f_ranges;
            leaf->f_defvals    = node->f_defvals;
            leaf->f_indices    = node->f_indices;
            leaf->f_offset     = node->f_offset;
            leaf->f_volatile   = node->f_volatile;
            nleafs++;
        }
        if (node->f_child)
        {
            node = node->f_child;
        }
        else if (node->f_sibling)
        {
            node = node->f_sibling;
        }
        else
        {
            while ((node = node->f_parent) != NULL)
            {
                if (node->f_sibling)
                {
                    node = node->f_sibling;
                    break;
                }
            }
        }
    }
    if (nleafs == 0)
    {
        fprintf(stderr, "No objects generated\n");
        return 4;
    }
    // for any leaf that has an nvram offset specified
    // allocate those bytes from the offset map
    //
    for (leaf = leafs; leaf; leaf = leaf->f_sibling)
    {
        if (leaf->f_offset >= 0)
        {
            int o, offset, b, bits, byte;
            unsigned char mask;
            bool isstring;

            b = bits    = bmibc_bits_needed_for_object(parser, leaf, &isstring);
            o = offset  = leaf->f_offset;

#if 1
            // this code forces all unallocated records
            // to start AFTER any allocated regions, to
            // avoid "holes" in allocated regions being
            // used
            //
            if ((o + b) > g_offset)
            {
                g_offset = o + b;
            }
#endif
            if (isstring)
            {
                if (o & 3)
                {
                    fprintf(stderr, "string object %s specifed non byte-aligned offset %d\n",
                        leaf->f_sym->f_name, leaf->f_offset);
                    return sizeof(uint64_t) + 1;
                }
            }
            if (leaf->f_volatile)
            {
                fprintf(stderr, "offset specified for volatile object %s, ignoring offset\n",
                    leaf->f_sym->f_name);
            }
            else if (g_offset_map)
            {
                while (b > 0)
                {
                    byte = o >> 3;
                    mask = 0xFF;
                    if (byte == (offset >> 3))
                        mask >>= (o & 7);
                    if (byte == ((offset + bits) >> 3))
                        mask &= ~(0xFF >> ((o + b) & 7));
                    if (g_offset_map[byte] & mask)
                    {
                        fprintf(stderr, "object %s at offset %d overlaps other object\n",
                            leaf->f_sym->f_name, leaf->f_offset);
                        return 6;
                    }
                    g_offset_map[byte] |= mask;
                    b-= 8 - (o & 7);
                    o+= 8 - (o & 7);
                }
            }
        }
    }

    if (! (hdrfile = fopen(hdrfilename, "wb")))
    {
        return bmibc_nofile_blurb(hdrfilename);
    }
    strcpy(srcfilename, hdrfilename);
    srcfilename[strlen(srcfilename) - 1] = 'c';

    if (! (srcfile = fopen(srcfilename, "wb")))
    {
        return bmibc_nofile_blurb(srcfilename);
    }
    fprintf(hdrfile, "\n/*\n * This is a generated file - Do Not Edit!\n */\n\n");
    fprintf(srcfile, "\n/*\n * This is a generated file - Do Not Edit!\n */\n\n");

    fprintf(srcfile, "#ifndef _BMIBC_GENERATED_C\n");
    fprintf(srcfile, "#define _BMIBC_GENERATED_C 1\n\n");

    fprintf(hdrfile, "#ifndef _BMIBC_GENERATED_H\n");
    fprintf(hdrfile, "#define _BMIBC_GENERATED_H 1\n\n");

    fprintf(hdrfile, "#ifndef BSNMPOBJECT_H\n");
    #ifdef BSNMPOBJECT_H
    // inherit object record type from snmp library
    fprintf(hdrfile, "%s\n%s\n%s\n%s\n",
         BMIBC_VALUE_TYPE_STR,
         BMIBC_ACCESS_METHOD_STR,
         BMIBC_ACCESS_PERM_STR,
         BMIBC_OBJECT_RECORD_STR);
    #else
    fprintf(hdrfile, "typedef enum {\n");
    fprintf(hdrfile, "    BMIBC_VOID,\n");
    fprintf(hdrfile, "    BMIBC_ENUM,\n    BMIBC_INT,\n    BMIBC_INT32,\n");
    fprintf(hdrfile, "    BMIBC_UINT32,\n    BMIBC_INT64,\n    BMIBC_UINT64,\n");
    fprintf(hdrfile, "    BMIBC_OCTET,\n    BMIBC_OBJECT\n");
    fprintf(hdrfile, "} bmibc_value_type_t;\n\n");

    fprintf(hdrfile, "typedef enum {\n");
    fprintf(hdrfile, "    BMIBC_DIRECT,\n    BMIBC_FUNCTION,\n    BMIBC_GLOBAL,\n");
    fprintf(hdrfile, "} bmibc_access_method_t;\n\n");

    fprintf(hdrfile, "typedef enum {\n");
    fprintf(hdrfile, "    BMIBC_NONE = 0,\n");
    fprintf(hdrfile, "    BMIBC_READ = 1,\n");
    fprintf(hdrfile, "    BMIBC_WRITE = 2,\n");
    fprintf(hdrfile, "    BMIBC_READWRITE = 3,\n");
    fprintf(hdrfile, "    BMIBC_PASSWORD = 4\n");
    fprintf(hdrfile, "} bmibc_access_perm_t;\n\n");

    fprintf(hdrfile, "typedef struct {\n    const char *name;\n");
    fprintf(hdrfile, "    int minv;\n    int maxv;\n    int dim;\n");
    fprintf(hdrfile, "    size_t offset;\n    size_t bits;\n");
    fprintf(hdrfile, "    bmibc_value_type_t type;\n    bmibc_access_method_t method;\n");
    fprintf(hdrfile, "    bmibc_access_perm_t access;\n    void *subs;\n");
    fprintf(hdrfile, "    void *factory_value;\n    void *value;\n} bmibc_record_t;\n\n");
    #endif
    fprintf(hdrfile, "#endif\n");

    fprintf(hdrfile, "/* Number of precompiled object records\n*/\n");
    fprintf(hdrfile, "#define BMIBC_NUM_RECORDS %d\n", nleafs);
    fprintf(hdrfile, "\nextern bmibc_record_t g_mib_objects[BMIBC_NUM_RECORDS];\n\n\n");

#ifndef NO_INCLUDE_INCLUDES
    fprintf(srcfile, "#ifndef BSNMPOBJECT_H\n");
    fprintf(srcfile, "#include <stdio.h>\n");
    fprintf(srcfile, "#include <stdint.h>\n");
    fprintf(srcfile, "#endif\n");
    fprintf(srcfile, "#include \"%s\"\n\n", hdrfilename);
#endif

    fprintf(hdrfile, "/* Definitions for property tags\n*/\n");
    for (leaf = leafs; leaf; leaf = leaf->f_sibling)
    {
        bmibc_gen_hdr_defs(leaf, hdrfile);
    }

    fprintf(hdrfile, "\n/* Enumerations for properties with enumerated type\n*/\n");
    for (leaf = leafs; leaf; leaf = leaf->f_sibling)
    {
        bmibc_gen_hdr_enums(leaf, hdrfile);
    }

    fprintf(hdrfile, "\n/* Declarations for storage for multi-dim properties\n*/\n");
    for (leaf = leafs; leaf; leaf = leaf->f_sibling)
    {
        bmibc_gen_hdr_dim_store(parser, leaf, leafs, hdrfile);
    }

    fprintf(srcfile, "/* Object records array\n*/\n");
    fprintf(srcfile, "bmibc_record_t g_mib_objects[BMIBC_NUM_RECORDS] = {\n");
    for (leaf = leafs; leaf; leaf = leaf->f_sibling)
    {
        if (leaf->f_enums)
            leaf->f_enums->f_out = 0;
        bmibc_gen_record(parser, leaf, leafs, srcfile);
    }
    fprintf(srcfile, "};\n\n");

    fprintf(srcfile, "/* lists of values for enumerations\n*/\n");
    for (leaf = leafs; leaf; leaf = leaf->f_sibling)
    {
        bmibc_gen_src_enums(leaf, srcfile);
    }

    fprintf(hdrfile, "\n\n/* pointer to enumeration values\n*/\n");
    if (g_genstringids)
    {
        fprintf(hdrfile, "typedef struct { int count; int* values; int* names; } bmib_enumvals_t;\n");
    }
    else
    {
        fprintf(hdrfile, "typedef struct { int count; int* values; } bmib_enumvals_t;\n");
    }
    fprintf(hdrfile, "\nextern const bmib_enumvals_t g_mib_enums[BMIBC_NUM_RECORDS];\n");

    fprintf(srcfile, "\n\n/* pointers to enumeration values array\n*/\n");
    fprintf(srcfile, "const bmib_enumvals_t g_mib_enums[BMIBC_NUM_RECORDS] = {\n");
    for (leaf = leafs; leaf; leaf = leaf->f_sibling)
    {
        bmibc_gen_enum_ptr(leaf, srcfile);
    }
    fprintf(srcfile, "};\n\n");

    fprintf(hdrfile, "\n/* Total size of allocated non-volatile bits\n*/\n");
    fprintf(hdrfile, "#define BMIBC_MAX_NV_OFFSET %d\n", g_max_offset - 1);
    fprintf(hdrfile, "#define BMIBC_MAX_NV_BYTES  %d\n", (g_max_offset + 7) / 8);

    if (g_nvautosize)
    {
        // adjust nvsize to max use only
        //
        g_nvsize = (g_max_offset + 7) / 8;
    }
    fprintf(hdrfile, "\n/* NV-RAM size, if not defined externally\n*/\n");
    fprintf(hdrfile, "#ifndef BMIBC_NVRAM_SIZE\n#define BMIBC_NVRAM_SIZE %d\n#endif\n\n", g_nvsize);

    fprintf(srcfile, "\n/* Storage for multi-dim properties or properties which need more than 32 bits\n*/\n");
    for (leaf = leafs; leaf; leaf = leaf->f_sibling)
    {
        bmibc_gen_src_dim_store(parser, leaf, leafs, srcfile);
    }

    fprintf(hdrfile, "\n\n/* OID to obejct record index cross reference table\n*/\n");
    fprintf(hdrfile, "#define BMIBC_MAX_NODE_INDEX %d /* number of indices supported */\n", MAX_NODE_INDEX);

    fprintf(hdrfile, "typedef struct {\n    const char *oidstr;\n    size_t record_index;\n    %s asntype;\n", TS_UINT8);
    fprintf(hdrfile, "    size_t indices[BMIBC_MAX_NODE_INDEX];\n} bmibc_oid_xref_t;\n\n");

    fprintf(hdrfile, "\nextern const bmibc_oid_xref_t g_oidxreftab[BMIBC_NUM_RECORDS];\n");

    fprintf(srcfile, "\n\n/* OID to Index cross reference table\n*/\n");
    fprintf(srcfile, "const bmibc_oid_xref_t g_oidxreftab[BMIBC_NUM_RECORDS] = {\n\n");
    prevoidlen = 0;

    for (leaf = leafs; leaf; leaf = leaf->f_sibling)
    {
        oidlen = bmib_get_oid(parser, leaf, oid, 1024);
        for (i = 0; i < prevoidlen && i < oidlen; i++)
        {
            if(prevoid[i] != oid[i])
            {
                break;
            }
        }
#if 1
        // this makes lookups easier in runtime code, at the expense of more
        // bytes taken in table
        //
        isfrag = 0;
#else
        // this compresses the xref table, but makes runtime more expensive
        //
        if (i == (prevoidlen - 1) && i <= (oidlen - 1))
        {
            isfrag = i; // new oid starts difference only at last oid of prev
        }
        else
        {
            isfrag = 0;
        }
#endif
        bmibc_gen_OID_xref(leaf, leafs, oid, oidlen, isfrag, srcfile);
        for (i = 0; i < oidlen; i++)
        {
            prevoid[i] = oid[i];
        }
        prevoidlen = oidlen;
    }
    fprintf(srcfile, "};\n\n");

    fprintf(hdrfile, "\n#endif /* _BMIBC_GENERATED_H */\n");
    fprintf(srcfile, "\n#endif /* _BMIBC_GENERATED_C */\n");

    fclose(hdrfile);
    fclose(srcfile);

    if (signaturesym)
    {
        bmib_destroy_symbol(parser, signaturesym);
    }
    bmib_destroy_nodetree(parser, leafs);

    bmib_destroy_parser(parser);

    if (g_offset_map)
    {
        free(g_offset_map);
    }
    #ifdef BMEM_H
    bmem_stats(5);
    bmem_deinit();
    #endif
    return 0;
}
