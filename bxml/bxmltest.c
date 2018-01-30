#include "bnetheaders.h"
#include "bxml.h"

typedef struct
{
    const char *element;
    size_t      index;
    const char *exp_value;
    int         exp_ret;
    const char *xml;
}
xml_test_entry_t;

xml_test_entry_t s_tests_1[] =
{
    {
    "test", 0, "result", 0,
    "<test>result</test>"
    },
};

int main(int argc, char **argv)
{
    bxml_parser_t *pjx;
    const char *element;
    size_t elindex;
    xml_test_entry_t *entry;
    int i;
    char val[128];
    char bigval[1024];
    int result;

#if 1
    // basic
    for (i = 0; i < sizeof(s_tests_1) / sizeof(xml_test_entry_t); i++)
    {
        entry = &s_tests_1[i];
        result = bxml_find_and_copy_nth_element(entry->xml, entry->element, '\0', 0, val, sizeof(val));
        printf("Basic Test %d\n", i);
        if (result != entry->exp_ret)
        {
            fprintf(stderr, "find error: %d, expected %d\n", result, entry->exp_ret);
            return -1;
        }
        if (! entry->exp_ret)
        {
            if (strcmp(entry->exp_value, val))
            {
                fprintf(stderr, "value error: %s, expected %s\n", val, entry->exp_value);
                return -1;
            }
            else
            {
                printf("  element:%s Value:%s\n", entry->element, val);
            }
        }
    }
#endif
    return 0;
}
