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
#include "bmibcerr.h"
#include "bmibc.h"

void bmib_error_multidef_sym(struct tag_mib_context *parser, const char *name, bmib_symbol_t *prevsym)
{
    if (prevsym)
    {
        switch (prevsym->f_type)
        {
        case bstNumber:
            bmib_warning(parser, "Symbol %s already defined as \"%d\" -redefinition ignored-",
                    name, prevsym->f_value.u_iv);
            break;

        case bstString:
            bmib_warning(parser, "Symbol %s already defined as \"%s\" -redefinition ignored-",
                    name, prevsym->f_value.u_sv);
            break;

        case bstObject:
            bmib_warning(parser, "Symbol %s already defined as \"%d\" -redefinition ignored-",
                name, prevsym->f_value.u_ov ? prevsym->f_value.u_ov->f_oid : -1);
            break;

        default:
            bmib_warning(parser, "Symbol %s already defined\n -redefinition ignored-", name);
        }
    }
}

void bmib_error_undefined(struct tag_mib_context *parser, const char* name)
{
    bmib_error(parser, meUndefSym, "Undefine Symbol \"%s\" referenced", name);
}

void bmib_error_multidef_node(struct tag_mib_context *parser, bmib_node_t *prevnode, const char *newval)
{
    char msg[512];

    bmib_error(parser, meMultidefSym, "OID %d already defined as %s: redefinition to %s illegal",
        prevnode ?
				prevnode->f_oid
			:	0,
        (prevnode && prevnode->f_sym && prevnode->f_sym->f_name) ?
                prevnode->f_sym->f_name
			: 	"<nil>",
        newval ?
	            newval
			: 	"<nil"
    );
	{
		int oidlen;
		boid_t oid[32];

		oidlen = bmib_get_oid(parser, prevnode, oid, sizeof(oid)/sizeof(boid_t));

		bmib_error(parser, meMultidefSym, "Existing Object at %s", bmib_format_oid(oid, oidlen));
	}
}

void bmib_error_token_size(struct tag_mib_context *parser, const char *token, int maxlen)
{
    bmib_warning(parser, "Token %s Exceeds max length of %d -token truncated-", token, maxlen);
}

void bmib_error_expected(struct tag_mib_context *parser, const char *expected)
{
    bmib_error(parser, meExpected, "Expected \"%s\" but parsed \"%s\"", expected, parser->f_token);
}

void bmib_error_memory(struct tag_mib_context *parser, const char* msg)
{
    bmib_error(parser, meMemory, "Can't alloc for %s near \"%s\"", msg, parser->f_token);
}

void bmib_error_token(struct tag_mib_context *parser, const char* msg)
{
    bmib_error(parser, meError, "%s \"%s\"", msg, parser->f_token);
}

void bmib_error_eof(struct tag_mib_context *parser, const char *msg, const char *detail)
{
	bmib_error(parser, meEOF, "Unexpected End-Of-File  %s: %s", msg, detail);
}

void bmib_warning(struct tag_mib_context *parser, const char *msg, ...)
{
    va_list args;
    char emsg[512];

    va_start(args, msg);

    vsnprintf(emsg, sizeof(emsg), msg, args);
    parser->f_warnings++;

	if (parser->f_errfunc)
	{
	    if (! parser->f_errfunc(meWarning, emsg))
	    {
	        parser->f_state = mpsError;
	        parser->f_abort = true;
	    }
	}
	va_end(args);
}

void bmib_error(struct tag_mib_context *parser, bmib_error_t code, const char *msg, ...)
{
    va_list args;
    char emsg[512];
	int room;
	int len;

    va_start(args, msg);

    parser->f_inerror = true;
    parser->f_state = mpsError;
	room = sizeof(emsg);

	len = snprintf(emsg, room, "%s: %d - ", parser->f_fname, parser->f_lines);
    vsnprintf(emsg + len, room - len, msg, args);

	parser->f_errors++;

	if (parser->f_errfunc)
	{
	    if (! parser->f_errfunc(code, emsg))
	    {
	        parser->f_state = mpsError;
	        parser->f_abort = true;
	    }
	}
	va_end(args);
}

