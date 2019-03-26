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

int ipp_set_attr_value(ipp_attr_t *attr, uint8_t *value, size_t value_len)
{
    if (! attr || ! value)
    {
        return -1;
    }
    if (attr->alloc_len < value_len || ! attr->value)
    {
        if (attr->value)
        {
            free(attr->value);
            attr->value = NULL;
        }
        attr->alloc_len = value_len;
        attr->value = (uint8_t *)malloc(value_len);
        if (! attr->value)
        {
            return -1;
        }
    }
    attr->value_len = value_len;
    memcpy(attr->value, value, value_len);
    return 0;
}

int ipp_add_attr_value(ipp_attr_t *attr, uint8_t *value, size_t *value_len)
{
}

int ipp_get_first_attr_value(ipp_attr_t *attr, ipp_attr_iter_t *iter, uint8_t **value, size_t *value_len)
{
    if (! attr)
    {
        return -1;
    }
    if (! attr->value)
    {
        return -2;
    }
    if (value)
    {
        *value = attr->value;
    }
    if (value_len)
    {
        *value_len = attr->value_len;
    }
    return 0;
}

int ipp_get_next_attr_value(ipp_attr_t *attr, ipp_attr_iter_t iter, uint8_t **value, size_t *value_len)
{
}

int ipp_get_attr_by_index(const size_t recdex, ipp_attr_grouping_code_t group, ipp_attr_t **pattr)
{
    ipp_group_xref_t *gxref;
    ipp_attr_t *attr;
    size_t count;

    if (group >= IPP_GROUPING_MAX_GROUP)
    {
        return -1;
    }
    ///@todo sort the attributes in group by record and binary search on them?
    /// instead of this linear search. it wouldn't be hard in ianaimport

    gxref = &s_ipp_group_xref[group];

    for (attr = gxref->group_attrs, count = 0; count < gxref->num_attr; attr++)
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
    return 1;
}

int ipp_get_attr_by_name(const char *name, ipp_attr_grouping_code_t group, ipp_attr_t **pattr)
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
    return ipp_get_attr_by_index(recdex, group, pattr);
}

int ipp_set_attr_string_value_by_name(const char *name, ipp_attr_grouping_code_t group, const char *str)
{
    ipp_attr_t *attr;
    int result;

    if (! name || ! str)
    {
        return -1;
    }
    result = ipp_get_attr_by_name(name, group, &attr);
    if (result)
    {
        return result;
    }
    result = ipp_set_attr_value(attr, (uint8_t *)str, strlen(str));
    if (result)
    {
        return result;
    }
    return 0;
}

int ipp_get_attr_value_by_name(const char *name, ipp_attr_grouping_code_t group, uint8_t **value, size_t *value_len)
{
    ipp_attr_t *attr;
    int result;

    result = ipp_get_attr_by_name(name, group, &attr);
    if (result)
    {
        return result;
    }
    result = ipp_get_first_attr_value(attr, NULL, value, value_len);
    if (result)
    {
        return result;
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
        return NULL;
    }
    attr->recdex = recdex;
    attr->value_len = 0;
    attr->alloc_len = 0;
    attr->value = NULL;

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
    if (attr->alloc_len)
    {
        free(attr->value);
    }
    free(attr);
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
    uint8_t *value;
    size_t   value_len;
    uint16_t vlen;
    char *strval;
    int result;

    strval = "this is OK";

    result = ipp_set_attr_string_value_by_name("printer-uri-supported", IPP_GROUPING_PRINTER_STATUS, strval);
    if (result)
    {
        butil_log(0, "set str attr failed\n");
        return result;
    }
    result = ipp_get_attr_value_by_name("printer-uri-supported", IPP_GROUPING_PRINTER_STATUS, &value, &value_len);
    if (result)
    {
        butil_log(0, "get attr failed\n");
        return result;
    }
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
    return 0;

}

