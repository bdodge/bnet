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
#include "bmibcutils.h"

const char *bmib_format_oid(boid_t *oid, int oidlen)
{
    static char oids[1024];
    int i;

    char* poid = oids;

    for (i = 0; i < oidlen; i++)
    {
        poid += snprintf(poid, sizeof(oids) - (poid - oids),
            "%d%s", oid[i], ((i == oidlen-1) ? "":"."));
        if ((1024 - (poid - oids)) < 8)
        {
            strcat(poid, "......");
            break;
        }
    }
    return oids;
}

int bmib_get_oid(struct tag_mib_context *parser, bmib_node_t *node, boid_t *oid, int cboid)
{
    bmib_node_t *pn;
    bmib_node_t *ppn = NULL;
    boid_t      *boid = oid;

    do
    {
        // go the parent link till right before last parent node
        for (pn = node; pn->f_parent != ppn; pn = pn->f_parent)
        {
            ;
        }
        ppn = pn;

        // add this node's oid to the list
        *oid++ = pn->f_oid;
        cboid--;

        if (cboid - 1 <= 0)
        {
            bmib_error(parser, meInternal, "Overflow getting oid");
            return oid - boid;
        }
    }
    while (pn != node);

    return oid - boid;
}

const char *bmib_get_oid_of(struct tag_mib_context *parser, const char *object, bmib_symbol_t *nodesyms)
{
    bmib_symbol_t *sym;
    bmib_node_t   *node;
    boid_t         oidbuf[256];
    int            oidlen;
    int            cboidlen;

    sym = bmib_find_symbol(object, nodesyms);
    if (! sym) return "";

    node = sym->f_value.u_ov;
    if (! node) return "";

    cboidlen = 256;
    oidlen   = bmib_get_oid(parser, node, oidbuf, cboidlen);
    if (! oidlen)
	{
		return "";
	}
    return bmib_format_oid(oidbuf, oidlen);
}

void bmib_format_description(char *dst, const char *src)
{
    if (*src == '\"')
	{
        src++;
	}
    while (*src && (*src != '\"'))
    {
        if (*src == '\r' || *src == '\n')
        {
            src++;

            while (*src == '\n' || *src == ' ' || *src == '\t')
			{
                src++;
			}
            *dst++ = ' ';
            if (*src == '\"')
			{
                break;
			}
        }
        *dst++ = *src++;
    }
    *dst = '\0';
}


