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

int bjson_is_white(char ch)
{
    return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n');
}

/*
static int bjson_unescape(
    if (*ve == '\\') {
      ve++;
      switch(*ve) {
      case '"': case '\\': case '/':
        value[i] = *ve++;
        break;
      case 'b':
        value[i] = '\b';
        ve++;
        break;
      case 'f':
        value[i] = '\f';
        ve++;
        break;;
      case 'n':
        value[i] = '\n';
        ve++;
        break;
      case 'r':
        value[i] = '\r';
        ve++;
        break;
      case 't':
        value[i] = '\t';
        ve++;
        break;
      case 'u':
        ve += 1;
        if (! *ve) {
          return e_json_syntax_error;
        }
        nv = digitToUint(*ve++);
        if (! *ve) {
          return e_json_syntax_error;
        }
        nv <<= 4;
        nv |= digitToUint(*ve++);
        if (! *ve) {
          return e_json_syntax_error;
        }
        nv <<= 4;
        nv |= digitToUint(*ve++);
        if (! *ve) {
          return e_json_syntax_error;
        }
        nv <<= 4;
        nv |= digitToUint(*ve++);
        // @todo - UTF8-encode this unicode char!
        value[i] = (char)(uint8_t)nv;
        // @todo - check nvalue too when expansion to utf8?
        break;
      default:
    #if 1
        return e_json_syntax_error;
    #else
        if (*ve) {
          value[i] = *ve++;
        }
    #endif
*/

int bjson_get_key_value(json_parser_t *pjx, const char *key, int index, char *value, int nvalue)
{
    const char *pv;
    size_t keylength;

    if (! pjx || ! key)
    {
        return -1;
    }
    if (! pjx->pkey)
    {
        return bjson_off_key;
    }
    keylength = strlen(key);

    // make sure parser context is on key
    if (strncmp(pjx->pkey, key, keylength))
    {
        return bjson_off_key;
    }
    // make sure psrc is near pkey end?

    // get first non-white char of value
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
    switch (*pjx->psrc)
    {
    case 'n':
        if ((pjx->psrc + 4 - pjx->json) >= pjx->length)
        {
            return bjson_underflow;
        }
        if (strncmp(pjx->psrc, "null", 4))
        {
            return bjson_syntax;
        }
        if (value)
        {
            if (nvalue < 5)
            {
                return bjson_overflow;
            }
            strcpy(value, "null");
            pjx->psrc += 4;
        }
        break;
    case 'f':
        break;
    case 't':
        break;
    case '\"':
        break;
    case '[':
        break;
    case '{':
        break;
    default:
        return bjson_syntax;
    }
    return 0;
}

int bjson_find_next_key(json_parser_t *pjx, const char *key)
{
    const char *pkey;
    const char *pend;
    size_t keylength;
    size_t remlength;

    if (! pjx || ! key)
    {
        return -1;
    }
    pjx->pkey = NULL;

    if (! pjx->json)
    {
        return -1;
    }
    if (! pjx->psrc)
    {
        pjx->psrc = pjx->json;
    }
    keylength = strlen(key);
    if (! keylength)
    {
        return bjson_not_found;
    }
    remlength = pjx->length - (pjx->psrc - pjx->json);
    if (remlength < (keylength + 3))
    {
        return bjson_not_found;
    }
    do
    {
        // look for the text "key": from the current position
        //
        pkey = memmem(pjx->psrc, remlength, key, keylength);
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
                    pjx->state = bjsValue;
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

int bjson_find_key(json_parser_t *pjx, const char *key)
{
    if (! pjx || ! key)
    {
        return -1;
    }
    pjx->psrc = pjx->json;
    pjx->state = bjsInit;
    pjx->index = 0;
    pjx->object_nest = 0;
    pjx->array_nest = 0;
    return bjson_find_next_key(pjx, key);
}

int bjson_find_key_value(const char *json, const char *key, int index, char *value, int nvalue)
{
    json_parser_t *pjx;
    int result;

    pjx = bjson_parser_create(json);
    if (! pjx)
    {
        return -1;
    }
    result = bjson_find_key(pjx, key);
    if (result)
    {
        return result;
    }
    result = bjson_get_key_value(pjx, key, index, value, nvalue);
    bjson_parser_free(pjx);
    return result;
}

int bjson_parser_free(json_parser_t *pjx)
{
    if (pjx)
    {
        free(pjx);
    }
    return 0;
}

json_parser_t *bjson_parser_create(const char *json)
{
    json_parser_t *pjx;

    if (! json || ! *json)
    {
        return NULL;
    }
    pjx = (json_parser_t *)malloc(sizeof(json_parser_t));
    if (pjx)
    {
        pjx->state = bjsInit;
        pjx->json = json;
        pjx->psrc = json;
        pjx->length = strlen(json);
        pjx->pkey = NULL;
        pjx->pfirstval = NULL;
        pjx->object_nest = 0;
        pjx->array_nest = 0;
    }
    return pjx;
}

