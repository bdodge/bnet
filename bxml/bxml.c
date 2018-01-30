#include "bnetheaders.h"
#include "bxml.h"

static int bxml_hextou(char digit, uint8_t *val)
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

static int bxml_is_white(char ch)
{
    return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n');
}

static int bxml_is_number(char ch)
{
    return (ch >= '0') && (ch <= '9');
}

static int bxml_is_delimiter(char ch)
{
    return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n')
        || (ch == ']') || (ch == '}') || (ch == ',');
}

static size_t bxml_utf8_encode(uint32_t unicode, uint8_t utfbuf[5])
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

int bxml_get_nth_element(
                        bxml_parser_t *pjx,
                        size_t index,
                        const char **element
                        )
{
    return 0;
}

int bxml_copy_nth_element(
                        bxml_parser_t *pjx,
                        size_t index,
                        char *element,
                        size_t nelement
                        )
{
    return 0;
}

static int bxml_find_element_component(
                                bxml_parser_t *pxp,
                                const char *component,
                                size_t remlength,
                                size_t keylength
                                )
{
    const char *pkey;
    const char *pend;

    // parse forward for the element from the current position
    // (from the current parse-state)
    //
    do
    {
        break;
    }
    while (pxp->psrc < (pxp->xml + pxp->length));

    return bxml_not_found;
}

int bxml_find_next_element(
                            bxml_parser_t *pxp,
                            const char *elementpath,
                            const char pathdelim,
                            size_t *pindex
                          )
{
    bxml_parser_t subparser;
    const char *component;
    char *component_end;
    size_t keylength;
    size_t remlength;
    size_t keyindex;
    bool wasindexed;
    const char *element;
    size_t element_len;
    bxml_type_t element_type;
    int result;

    if (! pxp || ! elementpath)
    {
        return bxml_parameter;
    }
    pxp->pelement = NULL;

    if (! pxp->xml)
    {
        return bxml_not_found;
    }
    if (! pxp->psrc)
    {
        pxp->psrc = pxp->xml;
    }
    if (pindex)
    {
        *pindex = 0;
    }
    // initial length is all of xml after the current spot
    //
    remlength = pxp->length - (pxp->psrc - pxp->xml);

    // initial component is the first (or the whole) part of key
    //
    component = elementpath;

    do
    {
        // get length of this component of the key path.  using length means
        // not having to null terminate things, so can use const data
        // stop at array index if found and remember it
        //
        keyindex = 0;
        wasindexed = false;
        component_end = NULL;

        for(keylength = 0; component[keylength]; keylength++)
        {
            if (component[keylength] == pathdelim)
            {
                // got to end of this component
                // (if pathdelim is 0, loop would already exit at component end)
                //
                component_end = (char*)component + keylength;
                break;
            }
            if (component[keylength] == '[')
            {
                // key components like "component[13]" parse to component, and index 13,
                // which lets the user get "a[4].b[2].c[5]" for example, instead of
                // recursively fetching the next part
                //
                keyindex = strtoul(component + keylength + 1, &component_end, 10);
                if (! component_end)
                {
                    return bxml_syntax;
                }
                while (bxml_is_white(*component_end))
                {
                    component_end++;
                }
                if (*component_end != ']')
                {
                    return bxml_syntax;
                }
                component_end++;
                while (bxml_is_white(*component_end))
                {
                    component_end++;
                }
                /* uncommenting this code disallows indexing of the last key
                 * component but it is a handy shortcut depending on how the user
                 * uses the api.
                 * it gets wierd if the user says "give me component[4], index 5"
                 *
                if (*component_end != pathdelim || ! pathdelim)
                {
                    // only allow indexing on non last key component
                    return bxml_parameter;
                }
                */
                wasindexed = true;
                break;
            }
        }
        do // try
        {
            if (remlength < (keylength + 3))
            {
                // not enough length left to have <component>
                return bxml_not_found;
            }
            // find this component in the xml string of length remlength
            //
            result = bxml_find_element_component(pxp, component, remlength, keylength);
            if (result)
            {
                break;
            }
            // found this component. if at end of component list, all set
            //
            if (! pathdelim || ! component_end || ! *component_end)
            {
                if (wasindexed)
                {
                    // ended in an index, so return that
                    if (pindex)
                    {
                        *pindex = keyindex;
                    }
                }
                return bxml_ok;
            }
            // there is a child component, find that next, first skip
            // to the nth element of this one, at this level
            //
            result = bxml_get_nth_element(pxp, keyindex, &element);
            if (result)
            {
                break;
            }
            // clone the parser state, and get length of this element
            //
            subparser.xml = pxp->xml;
            subparser.psrc = pxp->psrc;
            subparser.pelement = pxp->pelement;
            subparser.length = pxp->length;
          //  result = bxml_copy_element(&subparser, &element_len, NULL, 0);
            if (result)
            {
                break;
            }
            // limit remaining length to just the length of this object
            remlength = element_len;

            // point to next component if there is one
            if (*component_end)
            {
                component = component_end + 1;
                if (! *component)
                {
                    // dangling delim
                    result = bxml_syntax;
                    break;
                }
            }
            else
            {
                // got to end of component (must have been indexed)
                //
                if (wasindexed)
                {
                    return bxml_ok;
                }
                else
                {
                    // this can't happen
                    result = bxml_syntax;
                    break;
                }
            }
        }
        while (0); // catch

        if (result)
        {
            return result;
        }
    }
    while (pathdelim && component_end && *component_end);

    return bxml_not_found;
}

int bxml_find_first_element(
                        bxml_parser_t *pxp,
                        const char *elementpath,
                        const char pathdelim,
                        size_t *pindex
                     )
{
    if (! pxp || ! elementpath)
    {
        return bxml_parameter;
    }
    pxp->psrc = pxp->xml;
    return bxml_find_next_element(pxp, elementpath, pathdelim, pindex);
}

int bxml_find_nth_element(
                        const char *xml,
                        const char *elementpath,
                        const char pathdelim,
                        size_t index,
                        const char **element
                        )

{
    bxml_parser_t *pxp;
    size_t elindex;
    int result;

    pxp = bxml_parser_create(xml);
    if (! pxp)
    {
        return bxml_memory;
    }
    result = bxml_find_first_element(pxp, elementpath, pathdelim, &elindex);
    if (result)
    {
        bxml_parser_free(pxp);
        return result;
    }
    if (elindex > 0)
    {
        if (index > 0)
        {
            // last key index specified in text AND parameter
            bxml_parser_free(pxp);
            return bxml_parameter;
        }
        index = elindex;
    }
    result = bxml_get_nth_element(pxp, index, element);
    bxml_parser_free(pxp);
    return result;
}

int bxml_find_and_copy_nth_element(
                                const char *xml,
                                const char *elementpath,
                                const char pathdelim,
                                size_t index,
                                char *element,
                                size_t nelement
                                )
{
    bxml_parser_t *pxp;
    size_t elindex;
    int result;

    pxp = bxml_parser_create(xml);
    if (! pxp)
    {
        return bxml_memory;
    }
    result = bxml_find_first_element(pxp, elementpath, pathdelim, &elindex);
    if (result)
    {
        bxml_parser_free(pxp);
        return result;
    }
    if (elindex > 0)
    {
        if (index > 0)
        {
            // last key index specified in text AND parameter
            bxml_parser_free(pxp);
            return bxml_parameter;
        }
        index = elindex;
    }
    result = bxml_copy_nth_element(pxp, index, element, nelement);
    bxml_parser_free(pxp);
    return result;
}

int bxml_parser_free(bxml_parser_t *pxp)
{
    if (pxp)
    {
        free(pxp);
    }
    return 0;
}

bxml_parser_t *bxml_parser_create(const char *xml)
{
    bxml_parser_t *pxp;

    if (! xml || ! *xml)
    {
        return NULL;
    }
    pxp = (bxml_parser_t *)malloc(sizeof(bxml_parser_t));
    if (pxp)
    {
        pxp->xml  = xml;
        pxp->psrc = xml;
        pxp->length = strlen(xml);
        pxp->depth  = 0;
        pxp->pelement = NULL;
    }
    return pxp;
}

