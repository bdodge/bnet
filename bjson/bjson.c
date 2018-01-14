#include "bnetheaders.h"
#include "bjson.h"

static int bjson_hextou(char digit, uint8_t *val)
{
    if (digit >= 'a' && digit <= 'f')
    {
        *val = digit - 'a' + 10;
        return 0;
    }
    if (digit >= 'A' && digit <= 'F')
    {
        *val = digit - 'A' + 10;
        return 0;
    }
    if (digit >= '0' && digit <= '9')
    {
        *val = digit - '0';
        return 0;
    }
    return -1;
}

static int bjson_is_white(char ch)
{
    return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n');
}

static int bjson_is_number(char ch)
{
    return (ch >= '0') && (ch <= '9');
}

static int bjson_is_delimiter(char ch)
{
    return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n')
        || (ch == ']') || (ch == '}') || (ch == ',');
}

static size_t bjson_utf8_encode(uint32_t unicode, uint8_t utfbuf[5])
{
    unsigned short ca, cb, cc, cd;
    wchar_t  uc, nc;
    int  j, k;
    size_t i, len;

    j = 0;

    if (unicode < 0x80)
    {
        utfbuf[j++] = (unicode & 0xFF);
    }
    else if (unicode < 0x800) {
        utfbuf[j++] = 0xC0 | (unicode >> 6);
        utfbuf[j++] = 0x80 | (unicode & 0x3F);
    }
    else if (unicode < 0x10000) {
        utfbuf[j++] = 0xE0 |  (unicode >> 12);
        utfbuf[j++] = 0x80 | ((unicode >> 6) & 0x3F);
        utfbuf[j++] = 0x80 |  (unicode  & 0x3F);
    }
    else if (unicode < 0x200000) {
        utfbuf[j++] = 0xF0 |  (unicode >> 18);
        utfbuf[j++] = 0x80 | ((unicode >> 12) & 0x3F);
        utfbuf[j++] = 0x80 | ((unicode >> 6) & 0x3F);
        utfbuf[j++] = 0x80 |  (unicode  & 0x3F);
    }
    utfbuf[j] = '\0';
    return j;
}

int bjson_value_type(bjson_parser_t *pjx, const char *value, bjson_type_t *ptype)
{
    switch (*pjx->psrc)
    {
    case 'n':
        if (ptype)
        {
            *ptype = bjson_null;
        }
        break;
    case 'f':
        if (ptype)
        {
            *ptype = bjson_false;
        }
        break;
    case 't':
        if (ptype)
        {
            *ptype = bjson_true;
        }
        break;
    case '-':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        if (ptype)
        {
            *ptype = bjson_number;
        }
        break;
    case '\"':
        if (ptype)
        {
            *ptype = bjson_string;
        }
        break;
    case '{':
        if (ptype)
        {
            *ptype = bjson_object;
        }
        break;
    case '[':
        if (ptype)
        {
            *ptype = bjson_array;
        }
        break;
    default:
        return bjson_syntax;
    }
    return bjson_ok;
}

static int bjson_copy_keyword_value(bjson_parser_t *pjx, const char *kw, char *value, size_t nvalue)
{
    size_t kwl;

    if (! pjx || ! pjx->psrc || ! kw)
    {
        return bjson_parameter;
    }
    kwl = strlen(kw);

    if ((pjx->psrc + kwl - pjx->json) >= pjx->length)
    {
        return bjson_underflow;
    }
    if (strncmp(pjx->psrc, kw, kwl))
    {
        return bjson_syntax;
    }
    pjx->psrc += kwl;

    if (! bjson_is_delimiter(*pjx->psrc))
    {
        return bjson_syntax;
    }
    if (value)
    {
        if (nvalue < kwl + 1)
        {
            return bjson_overflow;
        }
        strcpy(value, kw);
    }
    return bjson_ok;
}

typedef enum
{
    ns_at_start, ns_leading_zero,
    ns_in_number, ns_in_fraction,
    ns_at_exponent, ns_in_exponent,
    ns_done
}
bjson_number_state_t;

static int bjson_copy_number_value(bjson_parser_t *pjx, char *value, size_t nvalue)
{
    bjson_number_state_t number_state;
    size_t i;

    if (! pjx || ! pjx->psrc)
    {
        return bjson_parameter;
    }
    if (value && (nvalue < 2))
    {
        return bjson_overflow;
    }
    if ((*pjx->psrc < '0' || *pjx->psrc > '9') && (*pjx->psrc != '-'))
    {
        return bjson_syntax;
    }
    number_state = ns_at_start;
    i = 0;

    while (number_state != ns_done)
    {
        switch (*pjx->psrc)
        {
        case '.':
            if (
                    number_state != ns_leading_zero
                &&  number_state != ns_in_number
            )
            {
                return bjson_syntax;
            }
            number_state = ns_in_fraction;
            if (value)
            {
                value[i++] = *pjx->psrc;
            }
            pjx->psrc++;
            break;
        case '-':
            if (
                    number_state != ns_at_start
                &&  number_state != ns_at_exponent
            )
            {
                return bjson_syntax;
            }
            if (number_state == ns_at_exponent)
            {
                number_state = ns_in_exponent;
            }
            else
            {
                number_state = ns_in_number;
            }
            if (value)
            {
                value[i++] = *pjx->psrc;
            }
            pjx->psrc++;
            break;
        case '+':
            if (number_state != ns_at_exponent)
            {
                return bjson_syntax;
            }
            number_state = ns_in_exponent;
            if (value)
            {
                value[i++] = *pjx->psrc;
            }
            pjx->psrc++;
            break;
        case 'e': case 'E':
            if(
                    number_state != ns_in_number
                &&  number_state != ns_in_fraction
            )
            {
                return bjson_syntax;
            }
            number_state = ns_at_exponent;
            if (value)
            {
                value[i++] = *pjx->psrc;
            }
            pjx->psrc++;
            break;
        case '0':
            if (number_state == ns_leading_zero)
            {
                return bjson_syntax;
            }
            if (number_state == ns_at_start)
            {
                number_state = ns_leading_zero;
            }
            if (value)
            {
                value[i++] = *pjx->psrc;
            }
            pjx->psrc++;
            break;
        default:
            if (bjson_is_number(*pjx->psrc))
            {
                if (number_state == ns_at_start)
                {
                    number_state = ns_in_number;
                }
                else if (number_state == ns_at_exponent)
                {
                    number_state = ns_in_exponent;
                }
                if (value)
                {
                    value[i++] = *pjx->psrc;
                }
                pjx->psrc++;
            }
            else
            {
                number_state = ns_done;
            }
            break;
        }
    }
    if (value)
    {
        if (i >= (nvalue - 1))
        {
            return bjson_overflow;
        }
        value[i] = '\0';
    }
    if (number_state != ns_done)
    {
        return bjson_syntax;
    }
    if (! bjson_is_delimiter(*pjx->psrc))
    {
        return bjson_syntax;
    }
    return bjson_ok;
}

static int bjson_copy_string_value(bjson_parser_t *pjx, char *value, size_t nvalue)
{
    size_t i;
    uint32_t uval;
    uint8_t  xval;
    char echar;
    uint8_t utf8_buffer[5];
    size_t utf8_len;
    size_t j;

    if (! pjx || ! pjx->psrc)
    {
        return bjson_parameter;
    }
    if (*pjx->psrc != '\"')
    {
        return bjson_syntax;
    }
    if (value)
    {
       if (nvalue < 3)
        {
            return bjson_overflow;
        }
        i = 0;
        value[i++] = *pjx->psrc;
    }
    pjx->psrc++;
    do
    {
        if (value && (i >= (nvalue - 2)))
        {
            return bjson_overflow;
        }
        if (*pjx->psrc == '\\')
        {
            echar = *++pjx->psrc;
            if (echar)
            {
                pjx->psrc++;
            }
            switch(echar)
            {
            case '"': case '\\': case '/':
                if (value)
                {
                    value[i++] = echar;
                }
                break;
            case 'b':
                if (value)
                {
                    value[i++] = '\b';
                }
                break;
            case 'f':
                if (value)
                {
                    value[i++] = '\f';
                }
                break;;
            case 'n':
                if (value)
                {
                    value[i++] = '\n';
                }
                break;
            case 'r':
                if (value)
                {
                    value[i++] = '\r';
                }
                break;
            case 't':
                if (value)
                {
                    value[i++] = '\t';
                }
                break;
            case 'u':
                if ((pjx->psrc - pjx->json) >= (pjx->length - 5))
                {
                    return bjson_underflow;
                }
                // go through the motions to validate chars even if no value
                echar = *pjx->psrc++;
                if (bjson_hextou(echar, &xval))
                {
                    return bjson_syntax;
                }
                uval = (uint32_t)xval;

                echar = *pjx->psrc++;
                if (bjson_hextou(echar, &xval))
                {
                    return bjson_syntax;
                }
                uval = (uval << 4) | (uint32_t)xval;

                echar = *pjx->psrc++;
                if (bjson_hextou(echar, &xval))
                {
                    return bjson_syntax;
                }
                uval = (uval << 4) | (uint32_t)xval;

                echar = *pjx->psrc++;
                if (bjson_hextou(echar, &xval))
                {
                    return bjson_syntax;
                }
                uval = (uval << 4) | (uint32_t)xval;

                if (value)
                {
                    utf8_len = bjson_utf8_encode(uval, utf8_buffer);
                    if ((i + utf8_len) >= (nvalue - 1))
                    {
                        return bjson_overflow;
                    }
                    for (j = 0; j < utf8_len; j++)
                    {
                        value[i++] = (char)utf8_buffer[j];
                    }
                }
                break;
            default:
                return bjson_syntax;
            }
        }
        else
        {
            if (*pjx->psrc < ' ')
            {
                // no control chars
                return bjson_syntax;
            }
            if (value)
            {
                value[i++] = *pjx->psrc;
            }
            pjx->psrc++;
            if (*pjx->psrc == '\"')
            {
                if (value)
                {
                    value[i++] = '\"';
                    value[i] = '\0';
                }
                pjx->psrc++;
                return bjson_ok;
            }
        }
    }
    while (*pjx->psrc);

    return bjson_syntax;
}

static int bjson_copy_array_or_object_value(
                                            bjson_parser_t *pjx,
                                            size_t *value_len,
                                            char *value,
                                            size_t nvalue
                                            )
{
    size_t nest;
    const char *startpos;
    char *copydest;
    char skipend[BJSON_MAX_NEST];
    size_t i;
    int result;

    if (! pjx || ! pjx->psrc)
    {
        return bjson_parameter;
    }
    startpos = pjx->psrc;
    nest = 0;
    i = 0;

    if (*pjx->psrc == '{')
    {
        if (value)
        {
            value[i++] = *pjx->psrc;
        }
        pjx->psrc++;
        skipend[nest++] = '}';
    }
    else if (*pjx->psrc == '[')
    {
        if (value)
        {
            value[i++] = *pjx->psrc;
        }
        pjx->psrc++;
        skipend[nest++] = ']';
    }
    else
    {
        return bjson_syntax;
    }
    if (value && (nvalue < 3))
    {
        return bjson_overflow;
    }
    while (*pjx->psrc)
    {
        if (value)
        {
            if (i >= (nvalue - 1))
            {
                return bjson_overflow;
            }
            copydest = value + i;
        }
        else
        {
            copydest = NULL;
        }
        while (bjson_is_white(*pjx->psrc))
        {
            pjx->psrc++;
        }
        if (! pjx->psrc)
        {
            return bjson_syntax;
        }
        switch (*pjx->psrc)
        {
        case 'n':
            result = bjson_copy_keyword_value(pjx, "null", copydest, nvalue - i);
            break;
        case 'f':
            result = bjson_copy_keyword_value(pjx, "false", copydest, nvalue - i);
            break;
        case 't':
            result = bjson_copy_keyword_value(pjx, "true", copydest, nvalue - i);
            break;
        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            result = bjson_copy_number_value(pjx, copydest, nvalue - i);
            break;
        case '\"':
            result = bjson_copy_string_value(pjx, copydest, nvalue - i);
            if (! result)
            {
                while (bjson_is_white(*pjx->psrc))
                {
                    pjx->psrc++;
                }
                if (! pjx->psrc)
                {
                    return bjson_syntax;
                }
                if (*pjx->psrc == ':')
                {
                    if (value)
                    {
                        i += strlen(copydest);
                        if (i >= (nvalue - 1))
                        {
                            return bjson_overflow;
                        }
                        value[i++] = ':';
                        value[i] = '\0';
                    }
                    pjx->psrc++;
                }
            }
            break;
        case '[':
            if (nest >= BJSON_MAX_NEST)
            {
                return bjson_overflow;
            }
            result = bjson_ok;
            if (value)
            {
                value[i++] = '[';
                value[i] = '\0';
            }
            pjx->psrc++;
            skipend[nest++] = ']';
            break;
        case '{':
            if (nest >= BJSON_MAX_NEST)
            {
                return bjson_overflow;
            }
            result = bjson_ok;
            if (value)
            {
                value[i++] = '{';
                value[i] = '\0';
            }
            pjx->psrc++;
            skipend[nest++] = '}';
            break;
        case ']':
        case '}':
            if (nest > 0)
            {
                nest--;
                if (skipend[nest] != *pjx->psrc)
                {
                    return bjson_syntax;
                }
                if (value)
                {
                    value[i++] = *pjx->psrc;
                    value[i] = '\0';
                }
                pjx->psrc++;
                if (nest == 0)
                {
                    if (value_len)
                    {
                        *value_len = pjx->psrc - startpos;
                    }
                    return bjson_ok;
                }
            }
            else
            {
                return bjson_syntax;
            }
            break;
        default:
            return bjson_syntax;
        }
        if (result)
        {
            return result;
        }
        if (value && value[i])
        {
            // add additional text to insert pos in value
            //
            i += strlen(value + i);
        }
        // skip white space after whatever it is we just skipped
        //
        while (bjson_is_white(*pjx->psrc))
        {
            pjx->psrc++;
        }
        if (*pjx->psrc == ',')
        {
            if (value)
            {
                if (i >= (nvalue - 1))
                {
                    return bjson_overflow;
                }
                value[i++] = ',';
            }
            pjx->psrc++;
        }
    }
    return bjson_syntax;
}

int bjson_copy_value(bjson_parser_t *pjx, char *value, int nvalue)
{
    if (! pjx || ! pjx->psrc)
    {
        return bjson_parameter;
    }
    switch (*pjx->psrc)
    {
    case 'n':
        return bjson_copy_keyword_value(pjx, "null", value, nvalue);

    case 'f':
        return bjson_copy_keyword_value(pjx, "false", value, nvalue);

    case 't':
        return bjson_copy_keyword_value(pjx, "true", value, nvalue);

    case '-':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return bjson_copy_number_value(pjx, value, nvalue);

    case '\"':
        return bjson_copy_string_value(pjx, value, nvalue);

    case '[':
    case '{':
        return bjson_copy_array_or_object_value(pjx, NULL, value, nvalue);

    default:
        return bjson_syntax;
    }
}

#define bjson_skip_keyword_value(pjx, kw)   \
    bjson_copy_keyword_value(pjx, kw, NULL, 0)

#define bjson_skip_number_value(pjx)    \
    bjson_copy_number_value(pjx, NULL, 0)

#define bjson_skip_string_value(pjx)    \
    bjson_copy_string_value(pjx, NULL, 0)

#define bjson_skip_array_or_object_value(pjx)   \
    bjson_copy_array_or_object_value(pjx, NULL, 0)

#define bjson_skip_value(pjx)   \
    bjson_copy_value(pjx, NULL, 0)

static int bjson_find_element(bjson_parser_t *pjx, int index, const char **value)
{
    int result;
    size_t atdex;

    if (! pjx || ! value)
    {
        return bjson_parameter;
    }
    if (! pjx->psrc || *pjx->psrc != '[')
    {
        return bjson_syntax;
    }
    pjx->psrc++;
    atdex = -1;

    while (*pjx->psrc)
    {
        while (bjson_is_white(*pjx->psrc))
        {
            pjx->psrc++;
        }
        if (! *pjx->psrc)
        {
            return bjson_not_found;
        }
        // found a value
        atdex++;
        if (atdex == index)
        {
            *value = pjx->psrc;
            return bjson_ok;
        }
        // not this index, skip over this value
        result = bjson_skip_value(pjx);
        if (result)
        {
            return result;
        }
        // skip any white space after value
        while (bjson_is_white(*pjx->psrc))
        {
            pjx->psrc++;
        }
        if (! *pjx->psrc)
        {
            return bjson_not_found;
        }
        // if this is the end of the array, bummer
        if (*pjx->psrc == ']')
        {
            return bjson_not_found;
        }
        // if it isn't a comma, its a syntax error
        if (*pjx->psrc != ',')
        {
            return bjson_syntax;
        }
        // skip over comma and get next element
        pjx->psrc++;
    }
    return bjson_not_found;
}

int bjson_get_key_value(bjson_parser_t *pjx, int index, const char **value)
{
    const char *pv;

    if (! pjx || ! value)
    {
        return bjson_parameter;
    }
    if (! pjx->pkey)
    {
        return bjson_off_key;
    }
    // start value search after colon after key
    for (pv = pjx->pkey + 1; *pv && *pv != '\"';)
    {
        pv++;
    }
    if (*pv != '\"')
    {
        return bjson_syntax;
    }
    pv++;
    while (bjson_is_white(*pv))
    {
        pv++;
    }
    if (*pv != ':')
    {
        return bjson_syntax;
    }
    pv++;
    pjx->psrc = pv;

    // find first non-white char of value
    while (bjson_is_white(*pjx->psrc))
    {
        if ((pjx->psrc - pjx->json) >= pjx->length)
        {
            return bjson_underflow;
        }
        pjx->psrc++;
    }
    if (! *pjx->psrc)
    {
        return bjson_underflow;
    }
    // insist on sane start of value at least
    //
    switch (*pjx->psrc)
    {
    case 'n':
    case 'f':
    case 't':
    case '-':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case '\"':
    case '{':
        if (index != 0)
        {
            return bjson_not_array;
        }
        *value = pjx->psrc;
        return bjson_ok;
    case '[':
        // fast-forward to index element
        return bjson_find_element(pjx, index, value);
    default:
        return bjson_syntax;
    }
}

int bjson_copy_key_value(bjson_parser_t *pjx, int index, char *value, int nvalue)
{
    const char *pv;
    int result;

    if (! pjx || ! value || ! nvalue)
    {
        return bjson_parameter;
    }
    result = bjson_get_key_value(pjx, index, &pv);
    if (result)
    {
        return result;
    }
    if (! pv)
    {
        return bjson_not_found;
    }
    return bjson_copy_value(pjx, value, nvalue);
}

static int bjson_find_keycomponent(
                                bjson_parser_t *pjx,
                                const char *component,
                                size_t remlength,
                                size_t keylength
                                )
{
    const char *pkey;
    const char *pend;

    do
    {
        // look for the text "component": from the current position
        //
        pkey = memmem(pjx->psrc, remlength, component, keylength);
        if (! pkey)
        {
            return bjson_not_found;
        }
        // make sure the key is quoted and that the start quote
        // isn't an escaped quote char
        //
        if (
               (pkey > pjx->psrc)
            && (pkey[-1] == '\"')
            &&  (
                    (pkey == (pjx->psrc + 1))
                ||  (pkey[-2] != '\\')
                )
            && ((pkey + keylength) < (pjx->json + pjx->length))
            && (pkey[keylength] == '\"')
        )
        {
            // found the key token string, if it has a colon after it
            // (ignoring white space) then this is our next key
            //
            pend = pkey + keylength + 1;
            while (*pend)
            {
                if (bjson_is_white(*pend))
                {
                    pend++;
                }
                if (*pend == ':')
                {
                    pjx->psrc = pend + 1;
                    pjx->pkey = pkey;
                    return bjson_ok;
                }
            }
            if (! *pend)
            {
                return bjson_not_found;
            }
        }
        // key not really here, try again
        //
        if (pjx->psrc < (pjx->json + pjx->length))
        {
            pjx->psrc++;
        }
    }
    while (pjx->psrc < (pjx->json + pjx->length));

    return bjson_not_found;
}

int bjson_find_next_key(bjson_parser_t *pjx, const char *keypath, const char pathdelim)
{
    bjson_parser_t subparser;
    const char *component;
    const char *component_end;
    size_t keylength;
    size_t remlength;
    const char *value;
    size_t value_len;
    bjson_type_t value_type;
    int result;

    if (! pjx || ! keypath)
    {
        return bjson_parameter;
    }
    pjx->pkey = NULL;

    if (! pjx->json)
    {
        return bjson_not_found;
    }
    if (! pjx->psrc)
    {
        pjx->psrc = pjx->json;
    }
    // initial length is all of json after the current spot
    //
    remlength = pjx->length - (pjx->psrc - pjx->json);

    // initial component is the first (or the whole) part of key
    //
    component = keypath;

    do
    {
        // get length of this component of the key path.  using length means
        // not having to null terminate things, so can use const data
        //
        component_end = strchr(component, pathdelim);
        if (! component_end || ! pathdelim)
        {
            // last component in key path
            //
            keylength = strlen(component);
        }
        else
        {
            keylength = component_end - component;
        }
        if (remlength < (keylength + 3))
        {
            // not enough length left to have qoute-component-quote-colon
            return bjson_not_found;
        }
        // find this key component in the json string of length remlength
        //
        result = bjson_find_keycomponent(pjx, component, remlength, keylength);
        if (result)
        {
            return result;
        }
        // found this key component. if at end of component list, all set
        //
        if (! pathdelim || ! component_end)
        {
            return bjson_ok;
        }
        // there is another component, find that next, but only if this key's
        // value is an object, since other json types don't have sub keys
        //
        result = bjson_get_key_value(pjx, 0, &value);
        if (result)
        {
            return result;
        }
        result = bjson_value_type(pjx, value, &value_type);
        if (result)
        {
            return result;
        }
        if (value_type != bjson_object)
        {
            return bjson_not_object;
        }
        // clone the parser state, and get length of this object
        //
        subparser.json = pjx->json;
        subparser.psrc = pjx->psrc;
        subparser.pkey = pjx->pkey;
        subparser.length = pjx->length;
        result = bjson_copy_array_or_object_value(&subparser, &value_len, NULL, 0);
        if (result)
        {
            return result;
        }
        // limit remaining length to just the length of this object
        remlength = value_len;

        // point to next component
        component = component_end + 1;
    }
    while (pathdelim && component_end && *component_end);

    return bjson_not_found;
}

int bjson_find_key(bjson_parser_t *pjx, const char *keypath, const char pathdelim)
{
    if (! pjx || ! keypath)
    {
        return bjson_parameter;
    }
    pjx->psrc = pjx->json;
    return bjson_find_next_key(pjx, keypath, pathdelim);
}

int bjson_find_key_value(
                        const char *json,
                        const char *keypath,
                        const char pathdelim,
                        int index,
                        const char **value
                        )

{
    bjson_parser_t *pjx;
    int result;

    pjx = bjson_parser_create(json);
    if (! pjx)
    {
        return bjson_memory;
    }
    result = bjson_find_key(pjx, keypath, pathdelim);
    if (result)
    {
        return result;
    }
    result = bjson_get_key_value(pjx, index, value);
    bjson_parser_free(pjx);
    return result;
}

int bjson_find_and_copy_key_value(
                                const char *json,
                                const char *keypath,
                                const char pathdelim,
                                int index,
                                char *value,
                                int nvalue
                                )
{
    bjson_parser_t *pjx;
    int result;

    pjx = bjson_parser_create(json);
    if (! pjx)
    {
        return bjson_memory;
    }
    result = bjson_find_key(pjx, keypath, pathdelim);
    if (result)
    {
        return result;
    }
    result = bjson_copy_key_value(pjx, index, value, nvalue);
    bjson_parser_free(pjx);
    return result;
}

int bjson_parser_free(bjson_parser_t *pjx)
{
    if (pjx)
    {
        free(pjx);
    }
    return 0;
}

bjson_parser_t *bjson_parser_create(const char *json)
{
    bjson_parser_t *pjx;

    if (! json || ! *json)
    {
        return NULL;
    }
    pjx = (bjson_parser_t *)malloc(sizeof(bjson_parser_t));
    if (pjx)
    {
        pjx->json = json;
        pjx->psrc = json;
        pjx->length = strlen(json);
        pjx->pkey = NULL;
    }
    return pjx;
}

