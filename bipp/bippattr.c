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
#include "bipp.h"

// include generated attribute tables
//
#include "biana_attribs.h"
#include "biana_attribs.c"

ipp_attr_t *ipp_create_attr(size_t recdex, size_t value_len, uint8_t *value)
{
    ipp_attr_t *attr;

    attr = (ipp_attr_t *)malloc(sizeof(ipp_attr_t));
    if (! attr)
    {
        return NULL;
    }
    attr->recdex = recdex;
    attr->len = 0;
    attr->alloc_len = 0;
    attr->val = NULL;

    if (value && value_len)
    {
        attr->alloc_len = value_len;
        attr->val = (uint8_t *)malloc(attr->alloc_len);
        if (! attr->val)
        {
            free(attr);
            return NULL;
        }
        memcpy(attr->val, value, value_len);
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
        free(attr->val);
    }
    free(attr);
    return 0;
}

int ipp_find_attr_rec(const char *name, size_t *index, ipp_attr_rec_t **pattr)
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
            if (pattr)
            {
                *pattr = &s_ipp_attributes[cur];
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
    if (pattr)
    {
        *pattr = NULL;
    }
    butil_log(6, "Didn't find %s\n", name);
    return -1;
}

const char *ipp_name_of_attr(ipp_attr_t *attr)
{
    if (! attr || attr->recdex >= NUM_IPP_ATTRIBUTES)
    {
        return "<nil>";
    }
    return s_ipp_attributes[attr->recdex].name;
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
