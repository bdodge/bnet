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
#include "bippattr.h"
#include "bstreamio.h"
#include "bippreq.h"
#include "bipp.h"

// include generated attribute tables
//
#include "biana_attribs.h"
#include "biana_attribs.c"

static size_t ipp_alloc_size(size_t len)
{
    size_t a;

    for (a = 8; a < len;)
    {
        a <<= 1;
    }
    return a;
}

int ipp_find_attr_rec(const char *name, size_t *index, ipp_attr_rec_t **pattrec)
{
    ipp_attr_t *attr;
    size_t top;
    size_t bot;
    size_t prevtop;
    size_t prevbot;
    size_t cur;
    size_t prevcur;
    int cmp;

    if (NUM_IPP_ATTRIBUTES < 1)
    {
        return -1;
    }
    // binary search in pre-sorted attribute table for name
    //
    top = 0;
    bot = NUM_IPP_ATTRIBUTES - 1;

    prevcur = bot + 1;
    cur = (bot + top) / 2;
    do
    {
        prevtop = top;
        prevbot = bot;
        cmp = strcmp(name, s_ipp_attributes[cur].name);
        butil_log(6, "%zu %zu %zu  %s vs %s -> %d\n",
                top, cur, bot, name, s_ipp_attributes[cur].name, cmp);
        if (cmp == 0)
        {
            if (index)
            {
                *index = cur;
            }
            if (pattrec)
            {
                *pattrec = &s_ipp_attributes[cur];
            }
            return 0;
        }
        else if (cmp < 0)
        {
            // too low, look higher
            bot = cur;
        }
        else /* cmp > 0 */
        {
            // too high, look lower
            top = cur;
        }
        prevcur = cur;
        cur = (bot + top) / 2;
        if (cur == prevcur)
        {
            // edge case: test bottom if stalled since the
            // calculation there rounds down
            //
            if (cur != bot)
            {
                cur = bot;
            }
        }
    }
    while (prevtop != top || prevbot != bot);

    if (index)
    {
        *index = 0;
    }
    if (pattrec)
    {
        *pattrec = NULL;
    }
    butil_log(6, "Didn't find %s\n", name);
    return -1;
}

int ipp_get_attr_rec(ipp_attr_t *attr, ipp_attr_rec_t **pattrec)
{
    if (! attr)
    {
        return -1;
    }
    if (attr->recdex >= NUM_IPP_ATTRIBUTES)
    {
        return -2;
    }
    if (pattrec)
    {
        *pattrec = &s_ipp_attributes[attr->recdex];
    }
    return 0;
}

const char *ipp_name_of_attr(ipp_attr_t *attr)
{
    ipp_attr_rec_t *attrec;
    int result;

    result = ipp_get_attr_rec(attr, &attrec);
    if (result)
    {
        return "<nil>";
    }
    return attrec->name;
}

int ipp_syntax_for_enc_type(ipp_syntax_enc_t enctag[IPP_MAX_ALT_TYPES], ipp_tag_t *tag, bool *is_array)
{
    if (! tag || ! is_array)
    {
        return -1;
    }
    *is_array = (enctag[0] | IPP_ARRAY) ? true : false;

    switch (enctag[0] & ~IPP_ARRAY)
    {
    case IPP_CHARSET:
        *tag = IPP_TAG_CHARSET;
        break;
    case IPP_LANGUAGE:
        *tag = IPP_TAG_LANGUAGE;
        break;
    case IPP_INTEGER:
        *tag = IPP_TAG_INTEGER;
        break;
    case IPP_BOOLEAN:
        *tag = IPP_TAG_BOOLEAN;
        break;
    case IPP_ENUM:
        *tag = IPP_TAG_ENUM;
        break;
    case IPP_KEYWORD:
        *tag = IPP_TAG_KEYWORD;
        break;

    case IPP_TEXT:
        *tag = IPP_TAG_TEXT;
        break;
    case IPP_NAME:
        *tag = IPP_TAG_NAME;
        break;
    case IPP_OCTETSTRING:
        *tag = IPP_TAG_STRING;
        break;
    case IPP_URI:
        *tag = IPP_TAG_URI;
        break;
    case IPP_URISCHEME:
        *tag = IPP_TAG_URISCHEME;
        break;
    case IPP_MIME:
        *tag = IPP_TAG_MIMETYPE;
        break;

    case IPP_RESOLUTION:
        *tag = IPP_TAG_RESOLUTION;
        break;
    case IPP_RANGEOFINT:
        *tag = IPP_TAG_RANGE;
        break;
    case IPP_DATETIME:
        *tag = IPP_TAG_DATE;
        break;
    case IPP_COLLECTION:
        *tag = IPP_TAG_BEGIN_COLLECTION;
        break;

    default:
        BERROR("Bad enctype");
        *tag = IPP_TAG_UNKNOWN;
        return -1;
    }
    return 0;
}

int ipp_syntax_for_attr(ipp_attr_t *attr, ipp_tag_t *tag, bool *is_array)
{
    int result;

    if (! attr)
    {
        return 1;
    }
    result = ipp_syntax_for_enc_type(
                                    s_ipp_attributes[attr->recdex].syntax,
                                    tag,
                                    is_array
                                    );
    return result;
}

static struct tag_name_to_xtype
{
    const char     *name;
    ipp_syntax_enc_t type;
}
s_syntax_to_type[] =
{
    { "charset",        IPP_CHARSET     },
    { "naturalLanguage",IPP_LANGUAGE    },
    { "integer",        IPP_INTEGER     },
    { "boolean",        IPP_BOOLEAN     },
    { "enum",           IPP_ENUM        },
    { "keyword",        IPP_KEYWORD     },

    { "text",           IPP_TEXT        },
    { "name",           IPP_NAME        },
    { "octetString",    IPP_OCTETSTRING },
    { "uri",            IPP_URI         },
    { "uriScheme",      IPP_URISCHEME   },
    { "mimeMediaType",  IPP_MIME        },

    { "resolution",     IPP_RESOLUTION  },
    { "rangeOfInteger", IPP_RANGEOFINT  },
    { "dateTime",       IPP_DATETIME    },
    { "collection",     IPP_COLLECTION  },

    { "1setOf",         IPP_ARRAY       }, // adds array bit
    { "type1",          IPP_NOTYPE      }, // ignored
    { "type2",          IPP_NOTYPE      }, // ignored
    { "no-value",       IPP_NOTYPE      }, // ignored
    { "unknown",        IPP_NOTYPE      }, // ignored
    { "|",              IPP_NOTYPE      }, // advances type index
};

#define IPP_NUM_TYPE_ENCS (sizeof(s_syntax_to_type)/sizeof(struct tag_name_to_xtype))

int ipp_type_string(ipp_syntax_enc_t type[IPP_MAX_ALT_TYPES], char *buffer, size_t nbuffer)
{
    ipp_syntax_enc_t typecode;
    size_t typedex;
    size_t cmpdex;
    int room;
    int len;
    bool opened;

    room = nbuffer;

    buffer[0] = '\0';
    opened = false;

    for (typedex = 0; typedex < IPP_MAX_ALT_TYPES; typedex++)
    {
        typecode = type[typedex];

        if (typecode == IPP_NOTYPE)
        {
            break;
        }
        if (typecode & IPP_ARRAY)
        {
            len = snprintf(buffer, room, "1setOf ");
            buffer += len;
            room -= len;
        }
        typecode &= ~IPP_ARRAY;

        if (typecode == IPP_NOTYPE)
        {
            opened = true;
            len = snprintf(buffer, room, "( ");
            buffer += len;
            room -= len;
        }
        else
        {
            for (cmpdex = 0; cmpdex < IPP_NUM_TYPE_ENCS; cmpdex++)
            {
                if (s_syntax_to_type[cmpdex].type == typecode)
                {
                    break;
                }
            }
            if (cmpdex >= IPP_NUM_TYPE_ENCS)
            {
                len = snprintf(buffer, room, "??? ");
                buffer += len;
                room -= len;
            }
            else
            {
                len = snprintf(buffer, room, "%s %s",
                        s_syntax_to_type[cmpdex].name,
                        (typedex < (IPP_MAX_ALT_TYPES - 1) ?
                            ((type[typedex + 1] != IPP_NOTYPE) ? "| " : "")
                            :   "")
                        );
                buffer += len;
                room -= len;
            }
        }
    }
    if (opened)
    {
        len = snprintf(buffer, room, "}");
        buffer += len;
        room -= len;
    }
    return 0;
}

int ipp_check_type_is(ipp_attr_t *attr, size_t ntypes, ...)
{
    ipp_syntax_enc_t *encsyntax;
    ipp_syntax_enc_t attr_syntax;
    ipp_syntax_enc_t base_syntax[IPP_MAX_ALT_TYPES];
    ipp_syntax_enc_t ok_syntax;
    char actual[32];
    char expected[32];
    va_list args;
    size_t startdex;
    size_t xdex;
    int result;

    startdex = 0;
    encsyntax = s_ipp_attributes[attr->recdex].syntax;
    if (encsyntax[0] == IPP_ARRAY)
    {
        // ignore 1setof modifier alone
        startdex++;
    }

    va_start(args, ntypes);

    ok_syntax = (ipp_syntax_enc_t)va_arg(args, int);
    base_syntax[0] = ok_syntax;
    base_syntax[1] = IPP_NOTYPE;

    while (ntypes-- > 0)
    {
        for (xdex = startdex; xdex < IPP_MAX_ALT_TYPES; xdex++)
        {
            attr_syntax = encsyntax[xdex] & ~IPP_ARRAY;

            if (attr_syntax == ok_syntax)
            {
                return 0;
            }
            //butil_log(5, "type %02X is not %02X\n", attr_syntax, ok_syntax);
        }
        if (ntypes > 0)
        {
            ok_syntax = (ipp_syntax_enc_t)va_arg(args, int);
        }
    }
   ipp_type_string(encsyntax, actual, sizeof(actual));
   ipp_type_string(base_syntax, expected, sizeof(expected));

    butil_log(1, "Attempt to set %s, an %s, with a type like %s\n",
                   ipp_name_of_attr(attr), actual, expected);
    return -1;
}

int ipp_set_attr_value(ipp_attr_t *attr, const uint8_t *value, size_t value_len)
{
    size_t totlen;

    if (! attr)
    {
        return -1;
    }
    totlen = value_len + 2;

    if (attr->alloc_len < totlen || ! attr->value || ! value)
    {
        // no room for new value, or no new value at all
        // so free any exising value and alloc for new
        //
        if (attr->value)
        {
            if (attr->alloc_len)
            {
                free(attr->value);
            }
            attr->value = NULL;
            attr->alloc_len = 0;
        }
        // and if there is a new value, alloc for that
        //
        if (value)
        {
            attr->alloc_len = ipp_alloc_size(totlen);
            attr->value = (uint8_t *)malloc(attr->alloc_len);
            if (! attr->value)
            {
                BERROR("Alloc value");
                return -1;
            }
        }
    }
    if (value)
    {
        attr->value_len = 0;

        // append a 2 byte value-length (in network byte order!)
        //
        attr->value[attr->value_len++] = (uint8_t)(value_len >> 8);
        attr->value[attr->value_len++] = (uint8_t)(value_len & 0xFF);

        // then append value itself
        //
        memcpy(attr->value + attr->value_len, value, value_len);
        attr->value_len += value_len;
    }
    return 0;
}

int ipp_add_attr_value(ipp_attr_t *attr, const uint8_t *value, size_t value_len)
{
    ipp_tag_t tag;
    bool is_array;
    size_t totlen;
    int result;

    if (! attr || ! value || ! value_len)
    {
        return -1;
    }
    if (! attr->value)
    {
        // first value, just set as convenience
        return ipp_set_attr_value(attr, value, value_len);
    }
    result = ipp_syntax_for_attr(attr, &tag, &is_array);
    if (result)
    {
        return -1;
    }
    if (! is_array)
    {
        // no more than one value allowed
        return -1;
    }
    // get needed total len
    //
    totlen = attr->value_len + value_len + 5;
    if (totlen > attr->alloc_len)
    {
        size_t new_alloc_len;
        uint8_t *new_value;

        // need to realloc
        //
        new_alloc_len = ipp_alloc_size(totlen);
        new_value = (uint8_t*)malloc(new_alloc_len);
        if (new_value == NULL)
        {
            BERROR("Alloc add value");
            return -1;
        }
        memcpy(new_value, attr->value, attr->value_len);
        free(attr->value);
        attr->value = new_value;
        attr->alloc_len = new_alloc_len;
    }
    // append the tag
    //
    attr->value[attr->value_len++] = tag;

    // append a 2 byte 0-length namelength. this lets
    // the value be sent on the wire directly
    //
    attr->value[attr->value_len++] = 0;
    attr->value[attr->value_len++] = 0;

    // append a 2 byte value-length
    //
    attr->value[attr->value_len++] = (uint8_t)(value_len >> 8);
    attr->value[attr->value_len++] = (uint8_t)(value_len & 0xFF);

    // append new value
    memcpy(attr->value + attr->value_len, value, value_len);
    attr->value_len += value_len;

    return 0;
}

int ipp_get_next_attr_value(ipp_attr_t *attr, ipp_attr_iter_t *iter, uint8_t **value, size_t *value_len)
{
    size_t bytesleft;
    uint16_t vallen;

    if (! attr || ! iter)
    {
        return -1;
    }
    if (! attr->value || ! iter->val_ptr)
    {
        return -2;
    }
    // check if any values can remain
    //
    bytesleft = attr->value_len - (iter->val_ptr - attr->value);
    if (bytesleft < 2)
    {
        return 1;
    }
    // length of each value encoded as first 2 bytes in value bytes
    //
    vallen = *iter->val_ptr++;
    vallen <<= 8;
    vallen |= *iter->val_ptr++;

    bytesleft -= 2;

    if (bytesleft < vallen)
    {
        BERROR("value length issue");
        return 2;
    }
    // for array types, skip tag and namelength for subsequent values
    //
    if (iter->val_count > 0)
    {
        iter->val_ptr += 3;
    }
    if (value)
    {
        *value = iter->val_ptr;
    }
    if (value_len)
    {
        *value_len = (size_t)vallen;
    }
    iter->val_ptr += vallen;
    iter->val_count++;
    return 0;
}

int ipp_get_next_attr_bool_value(ipp_attr_t *attr, ipp_attr_iter_t *iter, int32_t *value)
{
    uint8_t *uval;
    size_t  vallen;
    int result;

    result = ipp_get_next_attr_value(attr, iter, &uval, &vallen);
    if (result)
    {
        return result;
    }
    if (vallen != 1)
    {
        butil_log(1, "Wrong size for bool\n");
        return -1;
    }
    *value = (uval != 0) ? 1 : 0;
    return 0;
}

int ipp_get_next_attr_int32_value(ipp_attr_t *attr, ipp_attr_iter_t *iter, int32_t *value)
{
    uint8_t *uval;
    size_t  vallen;
    int32_t ival;
    int result;

    result = ipp_get_next_attr_value(attr, iter, &uval, &vallen);
    if (result)
    {
        return result;
    }
    if (vallen != sizeof(int32_t))
    {
        butil_log(1, "Wrong size for int32\n");
        return -1;
    }
    memcpy((uint8_t *)&ival, uval, vallen);
    *value = htonl(ival);
    return 0;
}

int ipp_get_next_attr_string_value(ipp_attr_t *attr, ipp_attr_iter_t *iter, char *value, size_t nvalue)
{
    uint8_t *uval;
    size_t value_len;
    int result;

    result = ipp_get_next_attr_value(attr, iter, &uval, &value_len);
    if (result)
    {
        return result;
    }
    if (value && nvalue)
    {
        if (value_len > (nvalue - 1))
        {
            value_len = nvalue - 1;
        }
        memcpy(value, uval, value_len);
        value[value_len] = '\0';
    }
    return 0;
}

int ipp_open_attr_value(ipp_attr_t *attr, ipp_attr_iter_t **piter)
{
    ipp_attr_iter_t *iter;

    if (! attr || ! piter)
    {
        return -1;
    }
    iter = (ipp_attr_iter_t *)malloc(sizeof(ipp_attr_iter_t));
    if (! iter)
    {
        return -1;
    }
    iter->attr = attr;
    iter->val_count = 0;
    iter->val_ptr = attr->value;
    *piter = iter;
    return 0;
}

int ipp_close_attr_value(ipp_attr_iter_t *iter)
{
    if (! iter)
    {
        return -1;
    }
    free(iter);
    return 0;
}

int ipp_get_only_attr_value(ipp_attr_t *attr, uint8_t **value, size_t *value_len)
{
    ipp_attr_iter_t iter;

    iter.attr = attr;
    iter.val_count = 0;
    iter.val_ptr = attr->value;
    return ipp_get_next_attr_value(attr, &iter, value, value_len);
}

int ipp_get_only_attr_bool_value(ipp_attr_t *attr, int *value)
{
    ipp_attr_iter_t iter;

    iter.attr = attr;
    iter.val_count = 0;
    iter.val_ptr = attr->value;
    return ipp_get_next_attr_bool_value(attr, &iter, value);
}

int ipp_get_only_attr_int32_value(ipp_attr_t *attr, int32_t *value)
{
    ipp_attr_iter_t iter;

    iter.attr = attr;
    iter.val_count = 0;
    iter.val_ptr = attr->value;
    return ipp_get_next_attr_int32_value(attr, &iter, value);
}

int ipp_get_only_attr_string_value(ipp_attr_t *attr, char *value, size_t nvalue)
{
    ipp_attr_iter_t iter;

    iter.attr = attr;
    iter.val_count = 0;
    iter.val_ptr = attr->value;
    return ipp_get_next_attr_string_value(attr, &iter, value, nvalue);
}

int ipp_get_attr_for_grouping(ipp_attr_grouping_code_t grouping, ipp_attr_t **pattrs)
{
    ipp_group_xref_t *xreftab;

    if (! pattrs)
    {
        return -1;
    }
    if (grouping >= IPP_GROUPING_MAX_GROUP)
    {
        return -1;
    }
    xreftab = &s_ipp_group_xref[grouping];
    *pattrs = xreftab->group_attrs;
    return 0;
}

int ipp_dupe_attr_list(ipp_attr_t *attrlist, ipp_attr_t **pdupeattrs)
{
    ipp_attr_t *newlist;
    ipp_attr_t *attr;
    ipp_attr_t *nattr;
    int result;

    if (! pdupeattrs)
    {
        return -1;
    }
    *pdupeattrs = NULL;

    if (! attrlist)
    {
        return 0;
    }
    result = ipp_dupe_attr(attrlist, &attr);
    if (result)
    {
        return result;
    }
    newlist = attr;
    *pdupeattrs = newlist;

    attrlist = attrlist->next;

    while (attrlist)
    {
        result = ipp_dupe_attr(attrlist, &nattr);
        if (result)
        {
            break;
        }
        attr->next = nattr;
        attr = nattr;

        attrlist = attrlist->next;
    }
    return result;
}

int ipp_dupe_attr_grouping(ipp_attr_grouping_code_t grouping, ipp_attr_t **pattrs)
{
    ipp_attr_t *groupattrs;
    ipp_attr_t *newlist;
    int result;

    if (! pattrs)
    {
        return -1;
    }
    *pattrs = NULL;

    result = ipp_get_attr_for_grouping(grouping, &groupattrs);
    if (result)
    {
        return result;
    }
    if (! groupattrs)
    {
        return 0;
    }
    return ipp_dupe_attr_list(groupattrs, pattrs);
}

#define IPPATTR_XREF_LINEAR 0

int ipp_get_group_attr_by_index(const size_t recdex, ipp_attr_grouping_code_t group, ipp_attr_t **pattr)
{
    ipp_group_xref_t *xreftab;
    ipp_attr_t *attr;
#if IPPATTR_XREF_LINEAR
    size_t count;
#else
    size_t top;
    size_t bot;
    size_t cur;
    size_t prevtop;
    size_t prevbot;
    size_t prevcur;
#endif
    if (pattr)
    {
        *pattr = NULL;
    }
    if (group >= IPP_GROUPING_MAX_GROUP)
    {
        return -1;
    }
    xreftab = &s_ipp_group_xref[group];

#if IPPATTR_XREF_LINEAR
    // linear search
    for (attr = xreftab->group_attrs, count = 0; count < xreftab->num_attr; attr++)
    {
        if (attr->recdex == recdex)
        {
            if (pattr)
            {
                *pattr = attr;
            }
            return 0;
        }
    }
    return -1;
#else
    // binary search
    top = 0;
    bot = xreftab->num_attr - 1;
    cur = (top + bot) / 2;

    butil_log(6, "Look for dex %zu in group %u\n", recdex, group);

    if (top == bot)
    {
        return -1;
    }
    do
    {
        butil_log(7, " %zu %zu %zu\n", top, cur, bot);
        prevtop = top;
        prevbot = bot;
        prevcur = cur;

        if (xreftab->group_attrs[cur].recdex == recdex)
        {
            if (pattr)
            {
                *pattr = &xreftab->group_attrs[cur];
            }
            return 0;
        }
        if (xreftab->group_attrs[cur].recdex < recdex)
        {
            top = cur;
        }
        else // xref recdex > recdex
        {
            bot = cur;
        }
        cur = (top + bot) / 2;

        if (cur == prevcur && cur != bot)
        {
            // since averaging rounds down, special case this
            cur = bot;
        }
    }
    while (top != prevtop || bot != prevbot);

#endif
    return -1;
}

int ipp_get_group_attr_by_name(const char *name, ipp_attr_grouping_code_t group, ipp_attr_t **pattr)
{
    ipp_attr_t *attr;
    size_t recdex;
    int result;

    if (! name || ! pattr)
    {
        return -1;
    }
    result = ipp_find_attr_rec(name, &recdex, NULL);
    if (result)
    {
        return result;
    }
    result = ipp_get_group_attr_by_index(recdex, group, pattr);
    if (result)
    {
        butil_log(2, "Didn't find %s in group %u\n", name, group);
    }
    return result;
}

int ipp_set_group_attr_bool_value(
                                const char *name,
                                ipp_attr_grouping_code_t group,
                                size_t nvalues,
                                ...
                                /* parm list of type "bool val, ..." */
                                )
{
    ipp_attr_t *attr;
    ipp_syntax_enc_t encoded_syntax;
    int result;
    va_list args;
    int value;
    uint8_t bvalue;
    size_t value_len;

    if (! name)
    {
        return -1;
    }
    if (nvalues == 0)
    {
        // todo - think about this
        return 0;
    }
    result = ipp_get_group_attr_by_name(name, group, &attr);
    if (result)
    {
        return result;
    }
    result = ipp_check_type_is(attr, 1, IPP_BOOLEAN);
    if (result)
    {
        return result;
    }
    va_start(args, nvalues);

    value = (int)va_arg(args, int);
    bvalue = value ? 0x01 : 0x00;
    value_len = 1;

    nvalues--;
    result = ipp_set_attr_value(attr, &bvalue, value_len);
    if (result)
    {
        return result;
    }
    while (nvalues-- > 0)
    {
        value = (int)va_arg(args, int);
        bvalue = value ? 0x01 : 0x00;

        result = ipp_add_attr_value(attr, &bvalue, value_len);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int ipp_set_group_attr_range_value(
                                const char *name,
                                ipp_attr_grouping_code_t group,
                                size_t nvalues,
                                ...
                                /* parm list of type "int32_t minval, int32_t maxval, ..." */
                                )
{
    ipp_attr_t *attr;
    int result;
    va_list args;
    int32_t minvalue;
    int32_t maxvalue;
    uint8_t value[8];
    size_t value_len;

    if (! name)
    {
        return -1;
    }
    if (nvalues == 0)
    {
        // todo - think about this
        return 0;
    }
    result = ipp_get_group_attr_by_name(name, group, &attr);
    if (result)
    {
        return result;
    }
    result = ipp_check_type_is(attr, 1, IPP_RANGEOFINT);
    if (result)
    {
        return result;
    }
    va_start(args, nvalues);

    minvalue = (int32_t)va_arg(args, int32_t);
    minvalue = htonl(minvalue);
    maxvalue = (int32_t)va_arg(args, int32_t);
    maxvalue = htonl(maxvalue);

    memcpy(value, (uint8_t *)&minvalue, sizeof(int32_t));
    memcpy(value + sizeof(int32_t), (uint8_t*)&maxvalue, sizeof(int32_t));
    value_len = 2 * sizeof(int32_t);

    nvalues--;
    result = ipp_set_attr_value(attr, (uint8_t*)&value, value_len);
    if (result)
    {
        return result;
    }
    while (nvalues-- > 0)
    {
        minvalue = (int32_t)va_arg(args, int32_t);
        minvalue = htonl(minvalue);
        maxvalue = (int32_t)va_arg(args, int32_t);
        maxvalue = htonl(maxvalue);

        memcpy(value, (uint8_t *)&minvalue, sizeof(int32_t));
        memcpy(value + sizeof(int32_t), (uint8_t*)&maxvalue, sizeof(int32_t));

        result = ipp_add_attr_value(attr, (uint8_t*)&value, value_len);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int ipp_set_group_attr_int32_value(
                                const char *name,
                                ipp_attr_grouping_code_t group,
                                size_t nvalues,
                                ...
                                /* parm list of type "int32_t val, ..." */
                                )
{
    ipp_attr_t *attr;
    int result;
    va_list args;
    int32_t value;
    size_t value_len;

    if (! name)
    {
        return -1;
    }
    if (nvalues == 0)
    {
        // todo - think about this
        return 0;
    }
    result = ipp_get_group_attr_by_name(name, group, &attr);
    if (result)
    {
        return result;
    }
    result = ipp_check_type_is(attr, 2, IPP_INTEGER, IPP_ENUM);
    if (result)
    {
        return result;
    }
    va_start(args, nvalues);

    value = (int32_t)va_arg(args, int32_t);
    value = htonl(value);

    value_len = sizeof(int32_t);

    nvalues--;
    result = ipp_set_attr_value(attr, (uint8_t*)&value, value_len);
    if (result)
    {
        return result;
    }
    while (nvalues-- > 0)
    {
        value = (int32_t)va_arg(args, int32_t);
        value = htonl(value);

        result = ipp_add_attr_value(attr, (uint8_t*)&value, value_len);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int ipp_set_group_attr_bytes_value(
                                const char *name,
                                ipp_attr_grouping_code_t group,
                                size_t nvalues,
                                ...
                                /* parm list of type "const uint8_t *value, size_t value_len, ..." */
                                )
{
    ipp_attr_t *attr;
    int result;
    va_list args;
    uint8_t *value;
    size_t value_len;

    if (! name)
    {
        return -1;
    }
    if (nvalues == 0)
    {
        // todo - think about this
        return 0;
    }
    result = ipp_get_group_attr_by_name(name, group, &attr);
    if (result)
    {
        return result;
    }
    va_start(args, nvalues);

    value = (uint8_t *)va_arg(args, uint8_t *);
    if (! value)
    {
        return -1;
    }
    value_len = (size_t)va_arg(args, size_t);

    nvalues--;
    result = ipp_set_attr_value(attr, value, value_len);
    if (result)
    {
        return result;
    }
    while (nvalues-- > 0)
    {
        value = (uint8_t *)va_arg(args, uint8_t *);
        if (! value)
        {
            return -1;
        }
        value_len = (size_t)va_arg(args, size_t);

        result = ipp_add_attr_value(attr, value, value_len);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int ipp_set_group_attr_string_value(
                                const char *name,
                                ipp_attr_grouping_code_t group,
                                int nstrings,
                                ...
                                /* parm list of type "const char *str, ..." */
                                )
{
    ipp_attr_t *attr;
    int result;
    va_list args;
    char *value;
    size_t value_len;

    if (! name)
    {
        return -1;
    }
    if (nstrings == 0)
    {
        // todo - think about this
        return 0;
    }
    result = ipp_get_group_attr_by_name(name, group, &attr);
    if (result)
    {
        return result;
    }
    result = ipp_check_type_is(attr, 9,
                    IPP_TEXT, IPP_CHARSET, IPP_LANGUAGE, IPP_KEYWORD,
                    IPP_NAME, IPP_OCTETSTRING, IPP_URI, IPP_URISCHEME, IPP_MIME);
    if (result)
    {
        return result;
    }
    va_start(args, nstrings);

    value = (char *)va_arg(args, char *);
    if (! value)
    {
        return -1;
    }
    value_len = strlen(value);

    nstrings--;
    result = ipp_set_attr_value(attr, (uint8_t *)value, value_len);
    if (result)
    {
        return result;
    }
    while (nstrings-- > 0)
    {
        value = (char *)va_arg(args, char *);
        if (! value)
        {
            return -1;
        }
        value_len = strlen(value);

        result = ipp_add_attr_value(attr, (uint8_t*)value, value_len);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int ipp_get_attr_by_name(const char *name, ipp_attr_t *attrlist, ipp_attr_t **pattr)
{
    ipp_attr_t *attr;
    int result;

    if (! name || ! pattr || ! attrlist)
    {
        return -1;
    }
    // can't assume sort order of random attribute lists so have to
    // do a linear search
    //
    for (attr = attrlist; attr; attr = attr->next)
    {
        if (! strcmp(ipp_name_of_attr(attr), name))
        {
            *pattr = attr;
            return 0;
        }
    }
    return -1;
}

int ipp_set_attr_attr_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                ipp_attr_t *vattr
                                )
{
    ipp_attr_t *attr;
    int result;

    if (! name || ! attrlist)
    {
        return -1;
    }
    result = ipp_get_attr_by_name(name, attrlist, &attr);
    if (result)
    {
        return result;
    }
    if (attr->alloc_len < vattr->value_len || ! attr->value || ! vattr->value)
    {
        // no room for new value, or no new value at all
        // so free any exising value and alloc for new
        //
        if (attr->value)
        {
            if (attr->alloc_len)
            {
                free(attr->value);
            }
            attr->value = NULL;
            attr->alloc_len = 0;
        }
        // and if there is a new value, alloc for that
        //
        if (vattr->value)
        {
            attr->alloc_len = ipp_alloc_size(vattr->value_len);
            attr->value = (uint8_t *)malloc(attr->alloc_len);
            if (! attr->value)
            {
                BERROR("Alloc value");
                return -1;
            }
            memcpy(attr->value, vattr->value, vattr->value_len);
            attr->value_len = vattr->value_len;
        }
    }
    return 0;
}

int ipp_set_attr_bool_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                size_t nvalues,
                                ...
                                /* parm list of type "bool val, ..." */
                                )
{
    ipp_attr_t *attr;
    int result;
    va_list args;
    int value;
    uint8_t bvalue;
    size_t value_len;

    if (! name || ! attrlist)
    {
        return -1;
    }
    if (nvalues == 0)
    {
        // todo - think about this
        return 0;
    }
    result = ipp_get_attr_by_name(name, attrlist, &attr);
    if (result)
    {
        return result;
    }
    result = ipp_check_type_is(attr, 1, IPP_BOOLEAN);
    if (result)
    {
        return result;
    }
    va_start(args, nvalues);

    value = (int)va_arg(args, int);
    bvalue = value ? 0x01 : 0x00;
    value_len = 1;

    nvalues--;
    result = ipp_set_attr_value(attr, &bvalue, value_len);
    if (result)
    {
        return result;
    }
    while (nvalues-- > 0)
    {
        value = (int)va_arg(args, int);
        bvalue = value ? 0x01 : 0x00;

        result = ipp_add_attr_value(attr, &bvalue, value_len);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int ipp_set_attr_range_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                size_t nvalues,
                                ...
                                /* parm list of type "int32_t minval, int32_t maxval, ..." */
                                )
{
    ipp_attr_t *attr;
    int result;
    va_list args;
    int32_t minvalue;
    int32_t maxvalue;
    uint8_t value[8];
    size_t value_len;

    if (! name || ! attrlist)
    {
        return -1;
    }
    if (nvalues == 0)
    {
        // todo - think about this
        return 0;
    }
    result = ipp_get_attr_by_name(name, attrlist, &attr);
    if (result)
    {
        return result;
    }
    result = ipp_check_type_is(attr, 1, IPP_RANGEOFINT);
    if (result)
    {
        return result;
    }
    va_start(args, nvalues);

    minvalue = (int32_t)va_arg(args, int32_t);
    minvalue = htonl(minvalue);
    maxvalue = (int32_t)va_arg(args, int32_t);
    maxvalue = htonl(maxvalue);

    memcpy(value, (uint8_t *)&minvalue, sizeof(int32_t));
    memcpy(value + sizeof(int32_t), (uint8_t*)&maxvalue, sizeof(int32_t));
    value_len = 2 * sizeof(int32_t);

    nvalues--;
    result = ipp_set_attr_value(attr, (uint8_t*)&value, value_len);
    if (result)
    {
        return result;
    }
    while (nvalues-- > 0)
    {
        minvalue = (int32_t)va_arg(args, int32_t);
        minvalue = htonl(minvalue);
        maxvalue = (int32_t)va_arg(args, int32_t);
        maxvalue = htonl(maxvalue);

        memcpy(value, (uint8_t *)&minvalue, sizeof(int32_t));
        memcpy(value + sizeof(int32_t), (uint8_t*)&maxvalue, sizeof(int32_t));

        result = ipp_add_attr_value(attr, (uint8_t*)&value, value_len);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int ipp_set_attr_int32_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                size_t nvalues,
                                ...
                                /* parm list of type "int32_t val, ..." */
                                )
{
    ipp_attr_t *attr;
    int result;
    va_list args;
    int32_t value;
    size_t value_len;

    if (! name || ! attrlist)
    {
        return -1;
    }
    if (nvalues == 0)
    {
        // todo - think about this
        return 0;
    }
    result = ipp_get_attr_by_name(name, attrlist, &attr);
    if (result)
    {
        return result;
    }
    result = ipp_check_type_is(attr, 2, IPP_INTEGER, IPP_ENUM);
    if (result)
    {
        return result;
    }
    va_start(args, nvalues);

    value = (int32_t)va_arg(args, int32_t);
    value = htonl(value);

    value_len = sizeof(int32_t);

    nvalues--;
    result = ipp_set_attr_value(attr, (uint8_t*)&value, value_len);
    if (result)
    {
        return result;
    }
    while (nvalues-- > 0)
    {
        value = (int32_t)va_arg(args, int32_t);
        value = htonl(value);

        result = ipp_add_attr_value(attr, (uint8_t*)&value, value_len);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int ipp_set_attr_bytes_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                size_t nvalues,
                                ...
                                /* parm list of type "const uint8_t *value, size_t value_len, ..." */
                                )
{
    ipp_attr_t *attr;
    int result;
    va_list args;
    uint8_t *value;
    size_t value_len;

    if (! name || ! attrlist)
    {
        return -1;
    }
    if (nvalues == 0)
    {
        // todo - think about this
        return 0;
    }
    result = ipp_get_attr_by_name(name, attrlist, &attr);
    if (result)
    {
        return result;
    }
    va_start(args, nvalues);

    value = (uint8_t *)va_arg(args, uint8_t *);
    if (! value)
    {
        return -1;
    }
    value_len = (size_t)va_arg(args, size_t);

    nvalues--;
    result = ipp_set_attr_value(attr, value, value_len);
    if (result)
    {
        return result;
    }
    while (nvalues-- > 0)
    {
        value = (uint8_t *)va_arg(args, uint8_t *);
        if (! value)
        {
            return -1;
        }
        value_len = (size_t)va_arg(args, size_t);

        result = ipp_add_attr_value(attr, value, value_len);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int ipp_set_attr_string_value(
                                const char *name,
                                ipp_attr_t *attrlist,
                                int nstrings,
                                ...
                                /* parm list of type "const char *str, ..." */
                                )
{
    ipp_attr_t *attr;
    int result;
    va_list args;
    char *value;
    size_t value_len;

    if (! name || ! attrlist)
    {
        return -1;
    }
    if (nstrings == 0)
    {
        // todo - think about this
        return 0;
    }
    result = ipp_get_attr_by_name(name, attrlist, &attr);
    if (result)
    {
        return result;
    }
    result = ipp_check_type_is(attr, 9,
                    IPP_TEXT, IPP_CHARSET, IPP_LANGUAGE, IPP_KEYWORD,
                    IPP_NAME, IPP_OCTETSTRING, IPP_URI, IPP_URISCHEME, IPP_MIME);
    if (result)
    {
        return result;
    }
    va_start(args, nstrings);

    value = (char *)va_arg(args, char *);
    if (! value)
    {
        return -1;
    }
    value_len = strlen(value);

    nstrings--;
    result = ipp_set_attr_value(attr, (uint8_t *)value, value_len);
    if (result)
    {
        return result;
    }
    while (nstrings-- > 0)
    {
        value = (char *)va_arg(args, char *);
        if (! value)
        {
            return -1;
        }
        value_len = strlen(value);

        result = ipp_add_attr_value(attr, (uint8_t*)value, value_len);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int ipp_dupe_attr(ipp_attr_t *attr, ipp_attr_t **dupeattr)
{
    ipp_attr_t *nattr;
    int result;

    if (! attr || ! dupeattr)
    {
        return -1;
    }
    nattr = ipp_create_attr(attr->recdex, NULL, 0);

    *dupeattr = nattr;

    if (! nattr)
    {
        return -1;
    }
    nattr->alloc_len = attr->alloc_len;
    if (attr->value)
    {
        nattr->value = (uint8_t *)malloc(nattr->alloc_len);
        if (! nattr->value)
        {
            BERROR("Alloc dupe value");
            free (nattr);
            return -1;
        }
        memcpy(nattr->value, attr->value, attr->value_len);
        nattr->value_len = attr->value_len;
    }
    return 0;
}

ipp_attr_t *ipp_create_attr(size_t recdex, uint8_t *value, size_t value_len)
{
    ipp_attr_t *attr;
    size_t result;

    attr = (ipp_attr_t *)malloc(sizeof(ipp_attr_t));
    if (! attr)
    {
        BERROR("Alloc attr");
        return NULL;
    }
    attr->recdex = recdex;
    attr->value_len = 0;
    attr->alloc_len = 0;
    attr->value = NULL;
    attr->next = NULL;

    if (value)
    {
        result = ipp_set_attr_value(attr, value, value_len);
        if (result)
        {
            free(attr);
            return NULL;
        }
    }
    return attr;
}

int ipp_destroy_attr(ipp_attr_t *attr)
{
    if (! attr)
    {
        return -1;
    }
    if (attr->alloc_len && attr->value)
    {
        free(attr->value);
    }
    if (attr->next)
    {
        butil_log(2, "Destroying linked attribute\n");
    }
    free(attr);
    return 0;
}

int ipp_destroy_attrlist(ipp_attr_t *attrlist)
{
    ipp_attr_t *attr;
    ipp_attr_t *nattr;

    for (attr = attrlist; attr;)
    {
        nattr = attr->next;
        attr->next = NULL;
        ipp_destroy_attr(attr);
        attr = nattr;
    }
    return 0;
}

int ipp_attr_init()
{
    ipp_group_xref_t *xreftab;
    ipp_attr_grouping_code_t gdex;
    size_t adex;

    // link all the group tables for convenience
    //
    for (gdex = IPP_GROUPING_DOCUMENT_DESCRIPTION; gdex < IPP_GROUPING_MAX_GROUP; gdex++)
    {
        xreftab = &s_ipp_group_xref[gdex];

        if (xreftab->num_attr > 0)
        {
            for (adex = 0; adex < xreftab->num_attr - 1; adex++)
            {
                xreftab->group_attrs[adex].next = &xreftab->group_attrs[adex + 1];
            }
            xreftab->group_attrs[adex].next = NULL;
        }
    }
    return 0;
}

int test_find_attr_rec()
{
    ipp_attr_rec_t *rec;
    size_t dex;
    int result;

    for (dex = 0; dex < NUM_IPP_ATTRIBUTES; dex++)
    {
        result = ipp_find_attr_rec(s_ipp_attributes[dex].name, NULL, &rec);
        if (result)
        {
            butil_log(0, "Didnt find %s\n", s_ipp_attributes[dex].name);
            return result;
        }
        if (! rec)
        {
            BERROR("bad ret");
            return -1;
        }
        if (strcmp(s_ipp_attributes[dex].name, rec->name))
        {
            butil_log(0, "Found %s not %s\n", rec->name, s_ipp_attributes[dex].name);
            return -1;
        }
    }
    result = ipp_find_attr_rec("z-side", NULL, &rec);
    if (! result)
    {
        butil_log(0, "Found bogus rec\n", rec->name);
        return -1;
    }
    result = ipp_find_attr_rec("aaa", NULL, &rec);
    if (! result)
    {
        butil_log(0, "Found bogus rec\n", rec->name);
        return -1;
    }
    result = ipp_find_attr_rec("printer-urinatural-language", NULL, &rec);
    if (! result)
    {
        butil_log(0, "Found bogus rec\n", rec->name);
        return -1;
    }
    return 0;
}

int test_set_get_string_attr()
{
    ipp_attr_t *attr;
    ipp_attr_iter_t *iter;
    uint8_t *value;
    size_t   value_len;
    uint16_t vlen;
    char *strval;
    int result;

    strval = "this is OK";

    result = ipp_set_group_attr_string_value("printer-uri-supported", IPP_GROUPING_PRINTER_STATUS, 1, strval);
    if (result)
    {
        butil_log(0, "set str attr failed\n");
        return result;
    }
    result = ipp_get_group_attr_by_name("printer-uri-supported", IPP_GROUPING_PRINTER_STATUS, &attr);
    if (result)
    {
        butil_log(0, "get attr failed\n");
        return result;
    }
    result = ipp_open_attr_value(attr, &iter);
    if (result)
    {
        butil_log(0, "open attr failed\n");
        return result;
    }
    result = ipp_get_next_attr_value(attr, iter, &value, &value_len);
    if (value_len != strlen(strval))
    {
        butil_log(0, "expected %u chars, got %zu\n", strlen(strval), value_len);
        return -1;
    }
    if (strncmp((char*)value, strval, value_len))
    {
        butil_log(0, "value is not expected:%s\n", strval);
        return -1;
    }
    result = ipp_close_attr_value(iter);
    if (result)
    {
        return result;
    }

    result = ipp_set_group_attr_int32_value("operations-supported", IPP_GROUPING_PRINTER_DESCRIPTION,
                        10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    if (result)
    {
        butil_log(0, "set int attr failed\n");
        return result;
    }
    result = ipp_get_group_attr_by_name("operations-supported", IPP_GROUPING_PRINTER_DESCRIPTION, &attr);
    if (result)
    {
        butil_log(0, "get int attr failed\n");
        return result;
    }
    result = ipp_open_attr_value(attr, &iter);
    if (result)
    {
        butil_log(0, "open int attr failed\n");
        return result;
    }
    for (int i = 0; i < 10; i++)
    {
        int32_t ival;

        result = ipp_get_next_attr_int32_value(attr, iter, &ival);
        if (ival != i)
        {
            butil_log(0, "value is %d, not expected %d\n", ival, i);
            return -1;
        }
    }
    result = ipp_close_attr_value(iter);
    if (result)
    {
        return result;
    }
    return 0;
}

int test_find_xref_rec()
{
    ipp_attr_grouping_code_t group;
    ipp_group_xref_t *xreftab;
    ipp_attr_t *attr;
    ipp_attr_t *retattr;
    size_t count;
    int result;

    for (group = IPP_GROUPING_DOCUMENT_DESCRIPTION; group < IPP_GROUPING_MAX_GROUP; group++)
    {
        xreftab = &s_ipp_group_xref[group];

        for (attr = xreftab->group_attrs, count = 0; count < xreftab->num_attr; count++, attr++)
        {
            result = ipp_get_group_attr_by_index(attr->recdex, group, &retattr);
            if (result)
            {
                butil_log(0, "failed to get record %zu in group %u, looking for attr %zu of %zu\n",
                            attr->recdex, group, count, xreftab->num_attr);
                return result;
            }
            if (retattr == NULL)
            {
                butil_log(0, "failed to return record for dex %zu in group %u\n", attr->recdex, group);
                return -1;
            }
            if (retattr->recdex != attr->recdex)
            {
                butil_log(0, "returned record at wrong index %zu, not at %zu in group %u\n",
                           retattr->recdex, attr->recdex, group);
                return -1;
            }
        }
    }
    return 0;
}


