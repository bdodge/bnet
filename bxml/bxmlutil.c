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
#include "bnetheaders.h"
#include "bxml.h"

static struct tag_entity_tab
{
    const char* name;
    uint16_t    unicode;
}
s_bxml_entities[] =
{
    { "quot",   34 },
    { "amp",    38 },
    { "apos",   39 },
    { "lt",     60 },
    { "gt",     62 },
},
s_bxml_html_entities[] =
{
    { "OElig",  338 },
    { "oelig",  339 },
    { "Scaron", 352 },
    { "scaron", 353 },
    { "Yuml",   376 },
    { "circ",   710 },
    { "tilde",  732 },
    { "ensp",   8194 },
    { "emsp",   8195 },
    { "thinsp", 8201 },
    { "zwnj",   8204 },
    { "zwj",    8205 },
    { "lrm",    8206 },
    { "rlm",    8207 },
    { "ndash",  8211 },
    { "mdash",  8212 },
    { "lsquo",  8216 },
    { "rsquo",  8217 },
    { "sbquo",  8218 },
    { "ldquo",  8220 },
    { "rdquo",  8221 },
    { "bdquo",  8222 },
    { "dagger", 8224 },
    { "Dagger", 8225 },
    { "permil", 8240 },
    { "lsaquo", 8249 },
    { "rsaquo", 8250 },
    { "euro",   8364 }
};

#define BXML_ENTITY_TABSIZE			(sizeof(s_bxml_entities)/sizeof(struct tag_entity_tab))
#define BXML_HTML_ENTITY_TABSIZE	(sizeof(s_bxml_html_entities)/sizeof(struct tag_entity_tab))

int bxml_decode_entity(const char *entity, size_t inlen, size_t *outlen, uint16_t *ucode)
{
	const char *name;
	size_t nlen;
	int i;

	if (! entity || ! inlen || ! ucode)
	{
		return bxml_parameter;
	}
	if (entity[0] == '&')
	{
		entity++;
		inlen--;
	}
	for (i = 0; i < inlen; i++)
	{
		if (entity[i] == ';')
		{
			inlen = i;
			break;
		}
	}
	if (entity[i] != ';')
	{
		return bxml_syntax;
	}
	for (i = 0; i < BXML_ENTITY_TABSIZE; i++)
	{
		name = s_bxml_entities[i].name;
		if (entity[1] == name[1])
		{
			nlen = strlen(name);
			if (nlen == inlen)
			{
				if (! strncmp(entity, name, nlen))
				{
					*outlen = nlen + 1;
					*ucode = s_bxml_entities[i].unicode;
					return bxml_ok;
				}
			}
		}
	}
	return bxml_not_found;
}

int bxml_format_header(
				char *buf,
				size_t nbuf,
				size_t *nout,
				const char *version,
				const char *encoding
				)
{
	int nfmt;

	if (! buf || ! nbuf || ! nout)
	{
		return -1;
	}

	nfmt = snprintf(buf, nbuf, "<?xml version=\"%s\" encoding=\"%s\"?>",
			version ? version : "1.0", encoding ? encoding : "utf-8");
	*nout = nfmt;

	if (nfmt < nbuf && nfmt > 0)
	{
		return -1;
	}

	return 0;
}

int bxml_format_element(
				char *buf,
				size_t nbuf,
				size_t *nout,
				bool start,
				const char *tag,
				const char *value,
				size_t nattribs,
				...
				)
{
	const char *attr;
	const char *val;
	size_t nmade;
	int nfmt;

	if (! buf || ! nbuf || ! nout || ! tag)
	{
		return -1;
	}
	nmade = 0;
	if (start)
	{
		nfmt = bxml_format_header(buf, nbuf, &nmade, NULL, NULL);
		if (nfmt < 0)
		{
			return nfmt;
		}
		nbuf -= nmade;
		buf += nmade;
	}

	nfmt = snprintf(buf, nbuf, "<%s", tag);

	if (nfmt < 0 || nfmt >= nbuf)
	{
		return -1;
	}

	nmade += nfmt;
	nbuf -= nfmt;
	buf += nfmt;

	if (nattribs > 0)
	{
	    va_list args;

		va_start(args, nattribs);
		while (nattribs-- > 0 && nbuf > 0)
		{
			attr = (const char *)va_arg(args, char *);
			val  = (const char *)va_arg(args, char *);

			nfmt = snprintf(buf, nbuf, " %s=\'%s\'", attr, val);
			if (nfmt < 0 || nfmt >= nbuf)
			{
				return -1;
			}
			nmade += nfmt;
			nbuf -= nfmt;
			buf += nfmt;
		}
		va_end(args);
	}
	if (value && *value)
	{
		nfmt = snprintf(buf, nbuf, ">%s</%s>", value, tag);
	}
	else if (value)
	{
		nfmt = snprintf(buf, nbuf, "/>");
	}
	else
	{
		nfmt = snprintf(buf, nbuf, ">");
	}
	if (nfmt < 0 || nfmt >= nbuf)
	{
		return -1;
	}
	nmade += nfmt;
	*nout = nmade;
	return 0;
}

int bxml_format_endtag(
                char *buf,
                size_t nbuf,
                size_t *nout,
                const char *tag
                )
{
	int nfmt;

	if (! buf || ! nbuf || ! nout || ! tag)
	{
		return -1;
	}
	nfmt = snprintf(buf, nbuf, "</%s>", tag);
	if (nfmt < 0 || nfmt >= nbuf)
	{
		return -1;
	}
	*nout = nfmt;

	return 0;
}
