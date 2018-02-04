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

