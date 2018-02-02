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

static int bxml_error(bxml_parser_t *pxp, int errcode)
{
    if (pxp)
    {
        pxp->error_code = errcode;
        pxp->error_line = pxp->line;
    }
    return errcode;
}

int bxml_compare_tags(const char *tag1, const char *tag2, size_t tag2_len)
{
    size_t i;
    
    if (! tag1)
    {
        return -1;
    }
    if (! tag2)
    {
        return 1;
    }
    i = 0;
    while (*tag1 && *tag2)
    {
        if (bxml_is_white(*tag1) || *tag1 == '>')
        {
            if (bxml_is_white(*tag2) || *tag2 == '>')
            {
                return 0;
            }
            return -1;
        }
        else if (bxml_is_white(*tag2) || *tag2 == '>')
        {
            if (*tag1 == '>')
            {
                return 0;
            }
            return 1;
        }
        if (*tag1 < *tag2)
        {
            return -1;
        }
        if (*tag1 > *tag2)
        {
            return 1;
        }
        i++;
        if (tag2_len > 0 && i >= tag2_len)
        {
            return 0;
        }
        tag1++;
        tag2++;
    }
    if (*tag1)
    {
        return 1;
    }
    if (*tag2)
    {
        return -1;
    }
    return 0;
}

static int bxml_parse_pi(bxml_parser_t *pxp)
{
    size_t remlength;
    
    if (! pxp->psrc)
    {
        return bxml_ok;
    }
    remlength = pxp->length - (pxp->psrc - pxp->xml);

    if (pxp->psrc[0] != '<' || pxp->psrc[1] != '?' || remlength < 4)
    {
        return bxml_error(pxp, bxml_syntax);
    }
    pxp->psrc += 2;

    while (*pxp->psrc && remlength > 1)
    {
        if (*pxp->psrc == '\n')
        {
            pxp->line++;
        }
        else if (*pxp->psrc == '?')
        {
            if (pxp->psrc[1] == '>')
            {
                pxp->psrc+= 2;
                remlength-= 2;
                while (bxml_is_white(*pxp->psrc) && remlength > 0)
                {
                    pxp->psrc++;
                    remlength--;
                }
                return bxml_ok;
            }
        }
        pxp->psrc++;
        remlength--;
    }
    return bxml_error(pxp, bxml_syntax);
}

static int bxml_parse_entity(bxml_parser_t *pxp)
{
    size_t remlength;
    
    if (! pxp->psrc)
    {
        return bxml_ok;
    }
    remlength = pxp->length - (pxp->psrc - pxp->xml);

    if (pxp->psrc[0] != '<' || pxp->psrc[1] != '!' || remlength < 3)
    {
        return bxml_error(pxp, bxml_syntax);
    }
    pxp->psrc += 2;

    while (*pxp->psrc && remlength > 1)
    {
        if (*pxp->psrc == '\n')
        {
            pxp->line++;
        }
        else if (*pxp->psrc == '>')
        {
            pxp->psrc++;
            remlength--;
            while (bxml_is_white(*pxp->psrc) && remlength > 0)
            {
                pxp->psrc++;
                remlength--;
            }
            return bxml_ok;
        }
        pxp->psrc++;
        remlength--;
    }
    return bxml_error(pxp, bxml_syntax);
}

static int bxml_parse_comment(bxml_parser_t *pxp)
{
    size_t remlength;
    int result;
    
    if (! pxp->psrc)
    {
        return bxml_error(pxp, bxml_syntax);
    }
    remlength = pxp->length - (pxp->psrc - pxp->xml);
    if (remlength < 7)
    {
        return bxml_error(pxp, bxml_syntax);
    }
    if (
            pxp->psrc[0] != '<'
        ||  pxp->psrc[1] != '!'
        ||  pxp->psrc[2] != '-'
        ||  pxp->psrc[3] != '-'
    )
    {
        return bxml_error(pxp, bxml_syntax);
    }
    pxp->psrc += 4;
    remlength -= 4;
    
    while (remlength > 2 && *pxp->psrc)
    {
        if (
                pxp->psrc[0] == '-'
            &&  pxp->psrc[1] == '-'
        )
        {
            if (pxp->psrc[2] == '>')
            {
                pxp->psrc += 3;
                remlength -= 3;
                
                while (remlength > 0 && bxml_is_white(*pxp->psrc))
                {
                    pxp->psrc++;
                    remlength--;
                }
                return bxml_ok;
            }
            else
            {
                return bxml_error(pxp, bxml_syntax);
            }
        }
        else
        {
            if (*pxp->psrc == '\n')
            {
                pxp->line++;
            }
            pxp->psrc++;
            remlength--;
        }
    }
    return bxml_error(pxp, bxml_syntax);
}

static int bxml_parse_tag(
                            bxml_parser_t *pxp,
                            bool require_tag,
                            const char **ptag,
                            bxml_tag_type_t *type
                          )
{
    size_t remlength;
    int result;
    
    if (! pxp->psrc)
    {
        return bxml_error(pxp, bxml_syntax);
    }
    *type = bxttNone;
    *ptag = NULL;
    do
    {
        *ptag = pxp->psrc;
        remlength = pxp->length - (pxp->psrc - pxp->xml);
        if (remlength < 3)
        {
            break;
        }
        if (*pxp->psrc != '<')
        {
            return bxml_error(pxp, bxml_syntax);
        }
        if (pxp->psrc[1] == '!')
        {
            if (pxp->psrc[2] == '-')
            {
                if (remlength > 3 && pxp->psrc[3] == '-')
                {
                    result = bxml_parse_comment(pxp);
                    if (result)
                    {
                        return result;
                    }
                    if (! require_tag)
                    {
                        return bxml_ok;
                    }
                }
                else
                {
                    return bxml_error(pxp, bxml_syntax);
                }
            }
            else 
            {
                result = bxml_parse_entity(pxp);
                if (result)
                {
                    return result;
                }
                if (! require_tag)
                {
                    return bxml_ok;
                }
            }
        }
        else if (pxp->psrc[1] == '?')
        {
            result = bxml_parse_pi(pxp);
            if (result)
            {
                return result;
            }
            if (! require_tag)
            {
                return bxml_ok;
            }
        }
        else
        {
            if (pxp->psrc[1] == '/')
            {
                *type = bxttEnd;
            }
            else
            {
                *type = bxttStart;
            }
            pxp->psrc++;
            remlength--;
    
            while (*pxp->psrc != '>' && *pxp->psrc && remlength > 0)
            {
                if (*pxp->psrc == '\n')
                {
                    pxp->line++;
                }
                pxp->psrc++;
                remlength--;
            }
            if (! *pxp->psrc)
            {
                return bxml_error(pxp, bxml_syntax);
            }
            if (pxp->psrc[-1] == '/')
            {
                *type = bxttEnd;
            }
            pxp->psrc++;
            if (remlength > 0)
            {
                remlength--;
                while (remlength > 0 && bxml_is_white(*pxp->psrc))
                {
                    if (*pxp->psrc == '\n')
                    {
                        pxp->line++;
                    }
                    pxp->psrc++;
                }
            }
            return bxml_ok;
        }
    }
    while (remlength > 0);

    return bxml_error(pxp, bxml_syntax);
}

static int bxml_parse_value(
                            bxml_parser_t *pxp,
                            const char *from_tag,
                            const char *till_tag,
                            size_t tag_len,
                            const char **end_tag,
                            const char **value,
                            size_t *value_len
                            )
{
    const char *pele;
    const char *ptag;
    size_t depth;
    size_t remlength;
    bxml_tag_type_t tag_type;
    int result;
    
    if (!pxp || ! pxp->psrc)
    {
        return bxml_ok;
    }
    // starting on inside of starting tag, so relative depth is 1
    //
    depth = 1;

    // look for the closing tag for this, or the till-tag, whichever
    // comes first, have to start with an actual starting tag
    //
    pele = from_tag;
    pele++;
    if (*pele == '/')
    {
        return bxml_error(pxp, bxml_syntax);
    }
    if (end_tag)
    {
        *end_tag = NULL;
    }
    if (value)
    {
        *value = pxp->psrc;
    }
    remlength = pxp->length - (pxp->psrc - pxp->xml);

    while (*pxp->psrc && remlength > 3 /* min end tag */)
    {
        if (*pxp->psrc == '<')
        {
            // possible tag, parse it (could be a comment, etc.)
            ptag = pxp->psrc;
            result = bxml_parse_tag(pxp, false, &ptag, &tag_type);
            if (result)
            {
                return result;
            }
            if (tag_type == bxttStart)
            {
                depth++;
                
                if (till_tag)
                {
                    if (! bxml_compare_tags(ptag + 1, till_tag, tag_len))
                    {
                        if (end_tag)
                        {
                            *end_tag = ptag;
                        }
                        return bxml_ok;
                    }
                }
            }
            else if (tag_type == bxttEnd)
            {
                depth--;
            }
            else
            {
                if (1)
                {
                    // move value past comments / processing instructions ?
                    if (value)
                    {
                        *value = pxp->psrc;
                    }
                }
            }
            remlength = pxp->length - (pxp->psrc - pxp->xml);
            
            // if it was a tag, compare it our starting element if at starting depth
            // and this is an end tag
            //
            if (depth == 0)
            {
                if (tag_type != bxttEnd)
                {
                    // can't be back to the start level after parsing a tag
                    // if it wasn't our end tag
                    //
                    return bxml_error(pxp, bxml_syntax);
                }
                if (value && value_len)
                {
                    *value_len = ptag - *value;
                }
                ptag++;
                if (*ptag == '/')
                {
                    ptag++;
                }
                result = bxml_compare_tags(pele, ptag, 0);
                if (result == 0)
                {
                    // this is the end tag for "from tag"
                    //
                    if (till_tag)
                    {
                        // didn't find till tag, so its not found
                        //
                        return bxml_not_found;
                    }
                    return bxml_ok;
                }
            }
        }
        else
        {
            // skipping over non-tag value text
            if (*pxp->psrc == '\n')
            {
                pxp->line++;
            }
            pxp->psrc++;
            remlength--;
        }
    }
    if (till_tag)
    {
        return bxml_not_found;
    }
    return bxml_error(pxp, bxml_syntax);
}

int bxml_get_nth_element(
                        bxml_parser_t *pxp,
                        size_t index,
                        const char **element
                        )
{
    return 0;
}

int bxml_copy_element(
                        bxml_parser_t *pxp,
                        char *element,
                        size_t nelement,
                        const char *value,
                        size_t value_len
                        )
{
    size_t len = nelement - 1;
    
    if (! element || ! nelement || ! value)
    {
        return bxml_parameter;
    }
    while (value_len > 0 && bxml_is_white(value[value_len - 1]))
    {
        value_len--;
    }
    if (value_len < len)
    {
        len = value_len;
    }
    strncpy(element, value, len);
    element[len] = '\0';
    return bxml_ok;
}

static int bxml_find_element_component(
                                bxml_parser_t *pxp,
                                const char *component,
                                size_t tagindex,
                                size_t remlength,
                                size_t taglength,
                                const char **tag_start
                                )
{
    const char *start_psrc;
    const char *ptag;
    size_t index;
    bxml_tag_type_t tag_type;
    int result;

    if (! pxp || ! component || ! tag_start)
    {
        return bxml_parameter;
    }
    start_psrc = pxp->psrc;
    *tag_start = NULL;
    
    // parse forward for the element from the current position
    //
    index = 0;
    do
    {
        if (*pxp->psrc != '<')
        {
            return bxml_error(pxp, bxml_syntax);
        }
        result = bxml_parse_tag(pxp, true, &ptag, &tag_type);
        if (result)
        {
            return result;
        }        
        if (! ptag)
        {
            // can't happen
            return bxml_error(pxp, bxml_syntax);
        }        
        if (ptag[1] == '/')
        {
            ptag++;
        }
        if (bxml_compare_tags(ptag + 1, component, taglength))
        {
            // parse the element's value if not our component. this will
            // stop at the first occurance of tag, if found
            //
            result = bxml_parse_value(pxp, ptag, component, taglength, tag_start, NULL, NULL);
            if (result && result != bxml_not_found)
            {
                return result;
            }        
            else if (result == bxml_ok)
            {
                if (index == tagindex)
                {
                    // found our tag inside the value
                    return bxml_ok;
                }
            }
            // restart at location of tag we were looking for
            //
            ptag = *tag_start;
        }
        else
        {
            if (tag_type != bxttStart)
            {
                return bxml_error(pxp, bxml_syntax);
            }        
            if (index == tagindex)
            {
                // found our tag
                *tag_start = ptag;
                return bxml_ok;
            }
        }
        if (! *pxp->psrc)
        {
            return bxml_not_found;
        }
        result = bxml_parse_value(pxp, ptag, NULL, 0, tag_start, NULL, NULL);
        if (result)
        {
            return result;
        }
        index++;
    }
    while (
            *pxp->psrc
        &&  (pxp->psrc - start_psrc) < remlength
        &&  pxp->psrc < (pxp->xml + pxp->length)
    );
    return bxml_not_found;
}

int bxml_find_next_element(
                            bxml_parser_t *pxp,
                            const char *elementpath,
                            const char pathdelim,
                            size_t index,
                            const char **tag_start
                          )
{
    bxml_parser_t subparser;
    const char *component;
    char *component_end;
    size_t keylength;
    size_t remlength;
    size_t keyindex;
    bool wasindexed;
    int result;

    if (! pxp || ! elementpath || ! tag_start)
    {
        return bxml_parameter;
    }
    if (! pxp->xml || ! pxp->root)
    {
        return bxml_error(pxp, bxml_syntax);
    }
    if (! pxp->psrc)
    {
        pxp->psrc = pxp->root;
    }
    *tag_start = NULL;
    
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
                if (*component_end != pathdelim || ! pathdelim)
                {
                    // this is the last component
                    //
                   /* uncommenting this code disallows indexing of the last key
                     * component but it is a handy shortcut depending on how the user
                     * uses the api.
                     * it gets wierd if the user says "give me component[4], index 5"
                     *
                    // only allow indexing on non last key component
                    return bxml_parameter;
                    */
                }
                wasindexed = true;
                break;
            }
        }
        do // try
        {
            if (! pathdelim || ! component_end || ! *component_end)
            {
                // looking at last component, allow only inded or keyindex
                //
                if (index && keyindex)
                {
                    // can only supply index in key or in param, not both
                    return bxml_parameter;
                }
                if (! keyindex)
                {
                    keyindex = index;
                }
            }
            if (remlength < (keylength + 2))
            {
                // not enough length left to have <component>
                return bxml_not_found;
            }
            // find this component in the xml string of length remlength
            //
            result = bxml_find_element_component(pxp, component, keyindex, remlength, keylength, tag_start);
            if (result)
            {
                break;
            }
            // found this component. if at end of component list, all set
            //
            if (! pathdelim || ! component_end || ! *component_end)
            {
                return bxml_ok;
            }
            // there is a child component, find that next
            //
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
            // to look for inner element, back parser to the start tag of
            // this element, since finding the component assumes we're on that
            //
            if (! *tag_start)
            {
                // can't happen, but don't ever let it
                result = bxml_syntax;
                break;
            }
            pxp->psrc = *tag_start;
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

int bxml_find_element(
                        bxml_parser_t *pxp,
                        const char *elementpath,
                        const char pathdelim,
                        size_t index,
                        const char **tag_start
                     )
{
    size_t remlength;
    
    if (! pxp || ! elementpath || ! tag_start)
    {
        return bxml_parameter;
    }
    pxp->psrc = pxp->root;
    *tag_start = NULL;

    // should be on root element now
    //
    if (*pxp->psrc != '<')
    {
        return bxml_syntax;
    }
    return bxml_find_next_element(pxp, elementpath, pathdelim, index, tag_start);
}

int bxml_find_nth_element(
                        const char *xml,
                        const char *elementpath,
                        const char pathdelim,
                        size_t index,
                        const char **element,
                        size_t *element_size
                        )

{
    bxml_parser_t *pxp;
    const char *tag_start;
    int result;

    pxp = bxml_parser_create(xml);
    if (! pxp)
    {
        return bxml_memory;
    }
    if (! pxp->root)
    {
        return bxml_error(pxp, bxml_syntax);
    }
    result = bxml_find_element(pxp, elementpath, pathdelim, index, &tag_start);
    if (result)
    {
        bxml_parser_free(pxp);
        return result;
    }
    result = bxml_parse_value(pxp, tag_start, NULL, 0, NULL, element, element_size);
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
    const char *tag_start;
    const char *value;
    size_t value_len;
    int result;

    pxp = bxml_parser_create(xml);
    if (! pxp)
    {
        return bxml_memory;
    }
    if (! pxp->root)
    {
        return bxml_error(pxp, bxml_syntax);
    }
    result = bxml_find_element(pxp, elementpath, pathdelim, index, &tag_start);
    if (result)
    {
        bxml_parser_free(pxp);
        return result;
    }
    result = bxml_parse_value(pxp, tag_start, NULL, 0, NULL, &value, &value_len);
    if (! result)
    {
        result = bxml_copy_element(pxp, element, nelement, value, value_len);
    }
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
    size_t remlength;
    int result;
    
    if (! xml)
    {
        return NULL;
    }
    pxp = (bxml_parser_t *)malloc(sizeof(bxml_parser_t));
    if (pxp)
    {
        while (bxml_is_white(*xml))
        {
            xml++;
        }
        pxp->xml  = xml;
        pxp->root = NULL;
        pxp->psrc = xml;
        pxp->length = strlen(xml);
        
        pxp->line = 1;
        pxp->error_line = 0;
        pxp->error_code = 0;
        
        // locate root element
        result = 0;
        do
        {
            remlength = pxp->length - (pxp->psrc - pxp->xml);
            if (remlength >= 4)
            {
                if (pxp->psrc[0] == '<' && pxp->psrc[1] == '?')
                {
                    result = bxml_parse_pi(pxp);
                    if (result)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        while (remlength >= 4 && ! result);
        
        if (! result && pxp->psrc[0] == '<')
        {
            pxp->root = pxp->psrc;
        }
    }
    return pxp;
}

