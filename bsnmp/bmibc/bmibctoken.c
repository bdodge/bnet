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

static int bmib_next_char(bmib_context_t *parser, char *b, bool *isEOF)
{
    if (! parser->f_fp)
    {
        *isEOF = true;
        return -1;
    }
    int nc = fgetc(parser->f_fp);

    if (nc == EOF)
    {
        *isEOF = true;
        return -1;
    }
    *isEOF = false;
    *b = (char)nc;
    if (nc == '\n')
    {
        parser->f_lines++;
    }
    return 0;
}

static int bmib_end_token(bmib_context_t *parser, bmib_keyword_t *kw)
{
    bmib_symbol_t *key;

    parser->f_pbc = 0;
    parser->f_token[parser->f_toklen] = '\0';
    key = bmib_find_symbol(parser->f_token, parser->f_keysyms);
    if (key != NULL)
    {
        *kw = (bmib_keyword_t)key->f_value.u_iv;
    }
    else
    {
        *kw = kwNONE;
    }
    return parser->f_toklen;
}

static int bmib_get_binary_token(bmib_context_t *parser, bmib_keyword_t *kw, int radix)
{
    int i, j, k, bits;
    unsigned long v;
    char xt[128];

    xt[0] = '\"';

    for (v = 0, i = j = 1, bits = 0; i < parser->f_toklen - 1; i++)
    {
        v *= radix;

        switch (parser->f_token[i])
        {
        case '0':
            break;
        case '1':
            v += 1;
            break;
        case '2': case '3': case '4': case '5': case '6': case '7':
            if (radix < 10)
            {
                bmib_error_token(parser, "digit outside radix in \'n\' octet string");
                return 0;
            }
            v += parser->f_token[i] - '0';
            break;
        case '8': case '9':
            if (radix < 10)
            {
                bmib_error_token(parser, "digit outside radix in \'n\' octet string");
                return 0;
            }
            v += parser->f_token[i] - '0';
            break;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            if (radix < 16)
            {
                bmib_error_token(parser, "digit outside radix in \'n\' octet string");
                return 0;
            }
            v += parser->f_token[i] - 'a' + 10;
            break;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            if (radix < 16)
            {
                bmib_error_token(parser, "digit outside radix in \'n\' octet string");
                return 0;
            }
            v += parser->f_token[i] - 'A' + 10;
            break;
        case '\'':
            bits--;
            break;
        default:
            bmib_error_token(parser, "non digit in \'n\' octet string");
            return 0;
        }
        switch (radix)
        {
        case 2:     bits++;   break;
        case 8:     bits+= 3; break;
        case 10:    bits+= 4; break; // !! this is wrong, but ''d is not right so no worries
        case 16:    bits+= 4; break;
        default:
            bmib_error_token(parser, "bad radix in \'n\' octet string");
            return 0;
        }
        if (bits >= 8)
        {
#if 0
            xt[j++] = (char)v;
#else
            k = snprintf(xt + j, 8, "\\x%02X", (unsigned char)v);
            j += k;
#endif
            v = 0;
            bits = 0;
        }
    }
    if (v)
    {
        bmib_error_token(parser, "incomplete bits specified in \'n\' octet string");
        return 0;
    }
    xt[j++] = '\"';
    xt[j] = '\0';
    strcpy(parser->f_token, xt);
    parser->f_toklen = j;
    return parser->f_toklen;
}

bool bmib_check_end_token(bmib_context_t *parser, char nc, bmib_keyword_t *kw)
{
    bmib_symbol_t *pKey;

    if (parser->f_toklen > 0)
    {
        parser->f_pbc = nc;
        parser->f_token[parser->f_toklen] = '\0';
        if ((pKey = bmib_find_symbol(parser->f_token, parser->f_keysyms)) != NULL)
        {
            *kw = (bmib_keyword_t)pKey->f_value.u_iv;
        }
        else
        {
            *kw = kwNONE;
        }
        return true;
    }
    else
    {
        return false;
    }
}

static bool bmib_add_to_token(bmib_context_t *parser, char nc)
{
    if (parser->f_toklen >= (parser->f_maxtok - 2))
    {
        return false;
    }
    parser->f_token[parser->f_toklen++] = nc;
    return true;
}

static int bmib_get_token(bmib_context_t *parser, bmib_token_type_t *type, bmib_keyword_t *keyword)
{
    int   ec;
    char  nc;
    bool  bEOF;

    if (parser->f_abort)
    {
        return -1;
    }
    parser->f_toklen = 0;

    do
    {
        if (parser->f_pbc)
        {
            // start with char that ended last token
            nc      = parser->f_pbc;
            parser->f_pbc = '\0';
            ec      = 0;
            bEOF    = false;
        }
        else
        {
            ec = bmib_next_char(parser, &nc, &bEOF);
        }
        if (bEOF) return -1;
    }
    while (
                (ec == 0)
            &&
                (
                (nc == ' ' || nc == '\t' ||
                 nc == '\n' || nc == '\r')
                )
    );

    switch (nc)
    {
    case '\r':

        if (bmib_check_end_token(parser, nc, keyword))
        {
            return parser->f_toklen;
        }
        break;

    case '\n':

        if (bmib_check_end_token(parser, nc, keyword))
        {
            return parser->f_toklen;
        }
        break;

    case '\'': // apostrophy means quoted value

        if (bmib_check_end_token(parser, nc, keyword))
        {
            return parser->f_toklen;
        }
        *type = mttString;
        bmib_add_to_token(parser, nc);
        do
        {
            ec = bmib_next_char(parser, &nc, &bEOF);
            if (ec != 0 || bEOF)
            {
                return bmib_end_token(parser, keyword);
            }
            bmib_add_to_token(parser, nc);
        }
        while (nc != '\'');

        ec = bmib_next_char(parser, &nc, &bEOF);
        if (ec != 0 || bEOF)
        {
            return bmib_end_token(parser, keyword);
        }
        bmib_end_token(parser, keyword);
        if (nc == 'b' || nc == 'B')
        {
            return bmib_get_binary_token(parser, keyword, 2);
        }
        else if (nc == 'o' || nc == 'O')
        {
            return bmib_get_binary_token(parser, keyword, 8);
        }
        else if (nc == 'h' || nc == 'H')
        {
            return bmib_get_binary_token(parser, keyword, 16);
        }
        else
        {
            bmib_error_token(parser, "bad radix in \'n\' octet string");
            return 0;
        }
        break;

    case '"':

        if (bmib_check_end_token(parser, nc, keyword))
        {
            return parser->f_toklen;
        }
        *type = mttString;
        bmib_add_to_token(parser, nc);
        do
        {
            ec = bmib_next_char(parser, &nc, &bEOF);
            if (ec != 0 || bEOF)
            {
                return bmib_end_token(parser, keyword);
            }
            bmib_add_to_token(parser, nc);
        }
        while (nc != '"');

        return bmib_end_token(parser, keyword);

    case '-':

        if (bmib_check_end_token(parser, nc, keyword))
        {
            return parser->f_toklen;
        }
        bmib_add_to_token(parser, nc);

        ec = bmib_next_char(parser, &nc, &bEOF);
        if (ec != 0 || bEOF)
        {
            return bmib_end_token(parser, keyword);
        }
        if (nc == '-')
        {
            bool fnw = true;

            bmib_add_to_token(parser, nc);
            *type = mttComment;
            do
            {
                ec = bmib_next_char(parser, &nc, &bEOF);
                if (ec != 0 || bEOF)
                {
                    return bmib_end_token(parser, keyword);
                }
                if (parser->f_enx && fnw && (nc == '['))
                {
                    *type = mttControl;
                }
                if (nc != ' ' && nc != '\t')
                {
                    fnw = false;
                }
                if (nc == '-')
                {
                    bmib_add_to_token(parser, nc);
                    ec = bmib_next_char(parser, &nc, &bEOF);
                    if (ec != 0 || bEOF)
                    {
                        return bmib_end_token(parser, keyword);
                    }
                    if (nc == '-' || nc == '\n' || nc == '\r')
                    {
                        bmib_add_to_token(parser, nc);
                        return bmib_end_token(parser, keyword);
                    }
                }
                else if (nc == '\n')
                {
                    return bmib_end_token(parser, keyword);
                }
                bmib_add_to_token(parser, nc);
            }
            while (ec == 0 && ! bEOF);

            return bmib_end_token(parser, keyword);
        }
        else if (nc < '0' || nc > '9')
        {
            *type = mttPunctuation;
            bmib_check_end_token(parser, nc, keyword);
            return parser->f_toklen;
        }
        else
        {
            *type = mttNumber;
            do
            {
                bmib_add_to_token(parser, nc);
                ec = bmib_next_char(parser, &nc, &bEOF);
                if (ec != 0 || bEOF)
                {
                    return bmib_end_token(parser, keyword);
                }
                if ((nc < '0') || (nc > '9'))
                {
                    bmib_check_end_token(parser, nc, keyword);
                    return parser->f_toklen;
                }
            }
            while (ec == 0 && ! bEOF);
        }
        break;

    case '(': case ')':
    case '{': case '}':
    case '[': case ']':
    case ';': case ',':
    case '|':

        if (bmib_check_end_token(parser, nc, keyword))
        {
            return parser->f_toklen;
        }
        bmib_add_to_token(parser, nc);
        *type = mttPunctuation;
        return bmib_end_token(parser, keyword);
        break;

    case '.':

        if (bmib_check_end_token(parser, nc, keyword))
        {
            return parser->f_toklen;
        }
        bmib_add_to_token(parser, nc);
        ec = bmib_next_char(parser, &nc, &bEOF);
        if (ec != 0 || bEOF)
        {
            return bmib_end_token(parser, keyword);
        }
        if (nc == '.')
        {
            *type = mttRange;
            return bmib_end_token(parser, keyword);
        }
        else
        {
            *type = mttLabel;
            return bmib_check_end_token(parser, nc, keyword);
        }
        break;

    case ':':

        if (bmib_check_end_token(parser, nc, keyword))
        {
            return parser->f_toklen;
        }
        bmib_add_to_token(parser, nc);
        ec = bmib_next_char(parser, &nc, &bEOF);
        if (ec != 0 || bEOF)
        {
            return bmib_end_token(parser, keyword);
        }
        if (nc == ':')
        {
            bmib_add_to_token(parser, nc);
            ec = bmib_next_char(parser, &nc, &bEOF);
            if (ec != 0 || bEOF)
            {
                return bmib_end_token(parser, keyword);
            }
            if (nc == '=')
            {
                bmib_add_to_token(parser, nc);
                *type = mttEqual;
                return bmib_end_token(parser, keyword);
            }
            else
            {
                *type = mttLabel;
                return bmib_check_end_token(parser, nc, keyword);
            }
        }
        else
        {
            *type = mttLabel;
            return bmib_check_end_token(parser, nc, keyword);
        }
        break;

    default:

        if (nc >= '0' && nc <= '9')
        {
            *type = mttNumber;
        }
        else
        {
            *type = mttLabel;
        }
        do
        {
            bmib_add_to_token(parser, nc);
            ec = bmib_next_char(parser, &nc, &bEOF);
            if (ec != 0 || bEOF)
            {
                return bmib_end_token(parser, keyword);
            }
            // if non-alpha-numeric get out
            if (
                ! (
                    ((nc >= 'A') && (nc <= 'Z'))        ||
                    ((nc >= 'a') && (nc <= 'z'))        ||
                    ((nc >= '0') && (nc <= '9'))        ||
                    ((nc == '-') || (nc == '_'))
                  )
            )
            {
                bmib_check_end_token(parser, nc, keyword);
                return parser->f_toklen;
            }
        }
        while (ec == 0 && ! bEOF);
        break;
    }
    return parser->f_toklen;
}

static int bmib_eval_numeric(bmib_context_t *parser, const char *pt, bool *tokerr, int level)
{
    char  token[MAX_MIB_TOKEN];
    bool  isnum = true;
    int   i;

    level++;

    while (*pt == ' ' || *pt == '\t')
    {
        pt++;
    }
    for (i = 0; pt[i] != '\0' && i < MAX_MIB_TOKEN - 1; i++)
    {
        if (pt[i] == ']' || pt[i] == ' ' || pt[i] == '\t')
        {
            break;
        }
        if (pt[i] < '0' || pt[i] > '9')
        {
            isnum = false;
        }
        token[i] = pt[i];
    }
    token[i] = '\0';

    // if its all numbers just return value
    //
    if (isnum)
    {
        return strtol(token, NULL, 0);
    }
    // its a symbolic reference, get from environment
    //
    pt = getenv(token);
    if (! pt)
    {
        if (level > 1)
        {
            // the "value" of a token that was defined in the environent
            // is also a string-token and that is not defined, so I could
            // return non-zero as in "ifdef" as opposed to 0 here
            //
            return 0;
        }
        else
        {
            return 0;
        }
    }
    if (level < 6)
    {
        return bmib_eval_numeric(parser, pt, tokerr, level);
    }
    else
    {
        bmib_error_token(parser, "Stopping Recursive symbolic reference");
        return 0;
    }
}

static bool bmib_skipping_input(bmib_context_t *parser)
{
    bool skip = false;
    int  lev;

    for (lev = 0; lev <= parser->f_skiplev; lev++)
    {
        skip |= parser->f_skipping[lev];
    }
    return skip;
}

static bool bmib_eval_conditional(bmib_context_t *parser, const char *pt, bool *tokerr)
{
    char  token[MAX_MIB_TOKEN];
    int   i;
    bool  isifeq;
    bool  istrue;

    while (*pt == ' ' || *pt == '\t')
    {
        pt++;
    }
    // get operation code (if/ifnot/else/endif)
    //
    for (i = 0; pt[i] != '\0' && i < MAX_MIB_TOKEN; i++)
    {
        if (pt[i] == ']' || pt[i] == ' ' || pt[i] == '\t')
        {
            break;
        }
        token[i] = pt[i];
    }
    token[i++] = '\0';
    pt += i;

    while (*pt == ' ' || *pt == '\t')
    {
        pt++;
    }
    istrue = false;

    if (! strcmp(token, "if"))
    {
        istrue = bmib_eval_numeric(parser, pt, tokerr, 0) == 0;
    }
    else if (! strcmp(token, "ifnot"))
    {
        istrue = bmib_eval_numeric(parser, pt, tokerr, 0) != 0;
    }
    else if ((isifeq = (! strcmp(token, "ifeq"))) || ! strcmp(token, "ifneq"))
    {
        char ntok[MAX_MIB_TOKEN], *pv;

        // get first token
        for (i = 0; pt[i] != '\0' && i < MAX_MIB_TOKEN; i++)
        {
            if (pt[i] == ']' || pt[i] == ' ' || pt[i] == '\t')
            {
                break;
            }
            token[i] = pt[i];
        }
        token[i++] = '\0';
        pt += i;

        if ((pv = getenv(token)) != NULL)
        {
            snprintf(token, MAX_MIB_TOKEN, "%s", pv);
        }
        while (*pt == ' ' || *pt == '\t')
        {
            pt++;
        }
        // get second token
        for (i = 0; pt[i] != '\0' && i < MAX_MIB_TOKEN; i++)
        {
            if (pt[i] == ']' || pt[i] == ' ' || pt[i] == '\t')
            {
                break;
            }
            ntok[i] = pt[i];
        }
        ntok[i++] = '\0';

        if ((pv = getenv(token)) != NULL)
        {
            snprintf(token, MAX_MIB_TOKEN, "%s", pv);
        }
        istrue = (isifeq ^ (strcmp(token, ntok) == 0));
    }
    else if (! strcmp(token, "else"))
    {
        if (parser->f_skiplev < 0)
        {
            bmib_error_token(parser, "Stray \"else\" ignored");
            *tokerr = true;
            return false;
        }
        parser->f_skipping[parser->f_skiplev] = ! parser->f_skipping[parser->f_skiplev];
        return bmib_skipping_input(parser);
    }
    else if (! strcmp(token, "endif"))
    {
        if (parser->f_skiplev >= 0)
        {
            parser->f_skiplev--;
        }
        else
        {
            bmib_error_token(parser, "Stray \"endif\" ignored");
            *tokerr = true;
        }
        return bmib_skipping_input(parser);
    }
    else
    {
        bmib_error_token(parser, "Malformed Conditional Control");
        *tokerr = true;
        return false;
    }
    if (parser->f_skiplev >= MAX_EVAL_LEVEL)
    {
        bmib_error_token(parser, "Conditional Controls Nested too deep");
        *tokerr = true;
        return bmib_skipping_input(parser);
    }
    parser->f_skiplev++;
    parser->f_skipping[parser->f_skiplev] = istrue;
    return bmib_skipping_input(parser);
}

static void bmib_eval_extension(bmib_context_t *parser, const char* pt, bool *tokerr)
{
    char token[MAX_MIB_TOKEN];
    int   i;

    while (*pt == ' ' || *pt == '\t')
    {
        pt++;
    }
    for (i = 0; pt[i] != '\0' && i < MAX_MIB_TOKEN; i++)
    {
        if (pt[i] == ']' || pt[i] == ' ' || pt[i] == '\t')
        {
            break;
        }
        token[i] = pt[i];
    }
    token[i] = '\0';
    pt += i;

    while (*pt == ' ' || *pt == '\t')
    {
        pt++;
    }
    if (! strcasecmp(token, "volatile"))
    {
        parser->f_volatile = true;
    }
    else if (! strcasecmp(token, "nonvolatile") || ! strcasecmp(token, "non-volatile") || ! strcasecmp(token, "nv"))
    {
        parser->f_volatile = false;
    }
    else if (! strcasecmp(token, "offset"))
    {
        // set fixed offset
        //
        for (i = 0; pt[i] != '\0' && i < MAX_MIB_TOKEN; i++)
        {
            if (pt[i] == ']' || pt[i] == ' ' || pt[i] == '\t')
            {
                break;
            }
            token[i] = pt[i];
        }
        token[i++] = '\0';
        pt += i;

        parser->f_offset = strtol(token, NULL, 0);
    }
    else if (! strcasecmp(token, "nostring") || ! strcasecmp(token, "nosid"))
    {
        // suppress string table reference
        //
        if (parser->f_enums)
        {
            parser->f_enums->f_nostring = true;
        }
        else
        {
            bmib_error_token(parser, "No Enumeration to String Suppress");
            *tokerr = true;
        }
    }
    else
    {
        bmib_error_token(parser, "Unknown Control");
        *tokerr = true;
    }
}

bool bmib_get_nc_token(
                        bmib_context_t    *parser,
                        bmib_token_type_t *toktype,
                        bmib_keyword_t    *kwtype,
                        const char        *tfor,
                        bool              *tokerr
                        )
{
    bool skipping = false;

    *tokerr = false;

    do
    {
        if (bmib_get_token(parser, toktype, kwtype) <= 0)
        {
            *tokerr = true;
            if (/*parser->f_state == mpsMIBV ||*/ parser->f_state == mpsEnd)
            {
                // eof is expected here, just return true
                //
                return true;
            }
            else
            {
                butil_log(4, "state at eof=%d\n", parser->f_state);
            }
            bmib_error_eof(parser, tfor, parser->f_lval);
        }
        if (*toktype == mttControl)
        {
            char *pt = parser->f_token;

            // special control token for conditional compilation
            //
            while (*pt != '[' && *pt != '\0')
            {
                pt++;
            }
            while (*pt == '[' || *pt == ' ' || *pt == '\t')
            {
                pt++;
            }
            switch (*pt)
            {
            case '$':   // replace token with numeric value of symbol
                snprintf(parser->f_token, MAX_MIB_TOKEN, "%d",
                        bmib_eval_numeric(parser, pt + 1, tokerr, 0));
                *toktype = mttNumber;
                break;
            case '?':   // evaluate "if" and turn on/off skip
                skipping = bmib_eval_conditional(parser, pt + 1, tokerr);
                *toktype = mttComment;
                break;
            case '*':   // evaluate misc control
                bmib_eval_extension(parser, pt + 1, tokerr);
                *toktype = mttComment;
                break;
            default:
                // assume its a regular comment
                *toktype = mttComment;
                break;
            }
        }
    }
    while (! *tokerr && ((*toktype == mttComment) || skipping));

    return *tokerr;
}

