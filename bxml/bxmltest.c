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

// basics
xml_test_entry_t s_tests_1[] =
{
    {
    "hi", 0, "", bxml_syntax,
    ""
    },
    {
    "hi", 0, "", bxml_syntax,
    "<hi>"
    },
    {
    "hi", 0, "", bxml_syntax,
    "<hi>a<hi>"
    },
    {
    "hi", 0, "result", 0,
    "<?xml?><test><hi>result</hi></test>"
    },
    {
    "test", 0, "result", 0,
    "<test>result</test>"
    },
    {
    "test", 0, "result", 0,
    "<?xml?><test>result</test>"
    },
    //
    {
    "test", 0, "<hi>result</hi>", 0,
    "<?xml?><test><hi>result</hi></test>"
    },
    {
    "hi", 0, "result", 0,
    "<?xml?><test><hi>result</hi></test>"
    },
    {
    "bye", 0, "result", 0,
    "<?xml?><test><hi><bye>result</bye></hi></test>"
    },
    {
    "bye", 0, "result", 0,
    "<?xml?><test><hi>test<test>aaa</test><bye>result</bye></hi></test>"
    },
    {
    "test", 0, "done", 0,
    "<?xml?><t1/><t2/><test>done</test>"
    },
    {
    "test", 0, "done", 0,
    "<?xml?><t1/><t2/><test>done</test>"
    },
    {
    "test", 0, "done", 0,
    "<?xml?><t1/><t2/><test>done</test><test/>"
    },
};

static char s_abc[] =
    "<?xml?>\r\n"
    "  <test>\r\n"
    "    aaaa\r\n"
    "  </test>\r\n"
    "  <test>\r\n"
    "    bbbb\r\n"
    "  </test>\r\n"
    "  <test>\r\n"
    "    ccc\r\n"
    "  </test>\r\n";

// basic indexing
xml_test_entry_t s_tests_2[] =
{
    {
    "bye", 1, "realresult", 0,
    "<?xml?><test><hi><bye>result</bye><bye>realresult</bye></hi></test>"
    },
    {
    "test[0]", 0, "aaaa", 0,
    s_abc
    },
    {
    "test[1]", 0, "bbbb", 0,
    s_abc
    },
    {
    "test[2]", 0, "ccc", 0,
    s_abc
    },
    {
    "test", 0, "aaaa", 0,
    s_abc
    },
    {
    "test", 1, "bbbb", 0,
    s_abc
    },
    {
    "test", 2, "ccc", 0,
    s_abc
    },
    {
    "test[1]", 1, "none", bxml_parameter,
    s_abc
    },
};

const char s_xml_1[] =
"<?xml version=\"1.0\"?>\n"
"<?xml-stylesheet href=\"catalog.xsl\" type=\"text/xsl\"?>\n"
"<!DOCTYPE catalog SYSTEM \"catalog.dtd\">\n"
"<catalog>\n"
"   <product description=\"Cardigan Sweater\" product_image=\"cardigan.jpg\">\n"
"      <catalog_item gender=\"Men's\">\n"
"         <item_number>QWZ5671</item_number>\n"
"         <price>39.95</price>\n"
"         <size description=\"Medium\">\n"
"            <color_swatch image=\"red_cardigan.jpg\">Red</color_swatch>\n"
"            <color_swatch image=\"burgundy_cardigan.jpg\">Burgundy</color_swatch>\n"
"         </size>\n"
"         <size description=\"Large\">\n"
"            <color_swatch image=\"red_cardigan.jpg\">Red</color_swatch>\n"
"            <color_swatch image=\"burgundy_cardigan.jpg\">Burgundy</color_swatch>\n"
"         </size>\n"
"      </catalog_item>\n"
"      <catalog_item gender=\"Women's\">\n"
"         <item_number>RRX9856</item_number>\n"
"         <price>42.50</price>\n"
"         <size description=\"Small\">\n"
"            <color_swatch image=\"red_cardigan.jpg\">Red</color_swatch>\n"
"            <color_swatch image=\"navy_cardigan.jpg\">Navy</color_swatch>\n"
"            <color_swatch image=\"burgundy_cardigan.jpg\">Burgundy</color_swatch>\n"
"         </size>\n"
"         <size description=\"Medium\">\n"
"            <color_swatch image=\"red_cardigan.jpg\">Red</color_swatch>\n"
"            <color_swatch image=\"navy_cardigan.jpg\">Navy</color_swatch>\n"
"            <color_swatch image=\"burgundy_cardigan.jpg\">Burgundy</color_swatch>\n"
"            <color_swatch image=\"black_cardigan.jpg\">Black</color_swatch>\n"
"         </size>\n"
"         <size description=\"Large\">\n"
"            <color_swatch image=\"navy_cardigan.jpg\">Navy</color_swatch>\n"
"            <color_swatch image=\"black_cardigan.jpg\">Black</color_swatch>\n"
"         </size>\n"
"         <size description=\"Extra Large\">\n"
"            <color_swatch image=\"burgundy_cardigan.jpg\">Burgundy</color_swatch>\n"
"            <color_swatch image=\"black_cardigan.jpg\">Black</color_swatch>\n"
"         </size>\n"
"      </catalog_item>\n"
"   </product>\n"
"</catalog>\n"
"\n";
    
// comments and processing instructions
xml_test_entry_t s_tests_3[] =
{
    {
    "price", 0, "39.95", 0,
    s_xml_1
    },
    {
    "catalog_item.price", 0, "39.95", 0,
    s_xml_1
    },
    {
    "catalog.catalog_item[1].price", 0, "42.50", 0,
    s_xml_1
    },
    {
    "catalog.catalog_item[1].size[2].color_swatch", 0, "Navy", 0,
    s_xml_1
    },
    {
    "catalog.catalog_item[1].size[2].color_swatch[1]", 0, "Black", 0,
    s_xml_1
    },
    {
    "catalog_item[0].size", 1,
            "<color_swatch image=\"red_cardigan.jpg\">Red</color_swatch>\n"
    "            <color_swatch image=\"burgundy_cardigan.jpg\">Burgundy</color_swatch>",
            0,
    s_xml_1
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

#if 0
    // basic
    for (i = 0; i < sizeof(s_tests_1) / sizeof(xml_test_entry_t); i++)
    {
        entry = &s_tests_1[i];
        result = bxml_find_and_copy_nth_element(entry->xml, entry->element,
                '\0', entry->index, val, sizeof(val));
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
                if (entry->index)
                {
                    printf("  element:%s[%d] Value:%s\n", entry->element, (int)entry->index, val);
                }
                else
                {
                    printf("  element:%s Value:%s\n", entry->element, val);
                }
            }
        }
    }
#endif
#if 0
    // basic indexing
    for (i = 0; i < sizeof(s_tests_2) / sizeof(xml_test_entry_t); i++)
    {
        entry = &s_tests_2[i];
        result = bxml_find_and_copy_nth_element(entry->xml, entry->element,
                '\0', entry->index, val, sizeof(val));
        printf("Basic Indexing Test %d\n", i);
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
                if (entry->index)
                {
                    printf("  element:%s[%d] Value:%s\n", entry->element, (int)entry->index, val);
                }
                else
                {
                    printf("  element:%s Value:%s\n", entry->element, val);
                }
            }
        }
    }
#endif
#if 1
    // basic real xml
    for (i = 0; i < sizeof(s_tests_3) / sizeof(xml_test_entry_t); i++)
    {
        entry = &s_tests_3[i];
        result = bxml_find_and_copy_nth_element(entry->xml, entry->element,
                '.', entry->index, bigval, sizeof(bigval));
        printf("Basic XML Test %d\n", i);
        if (result != entry->exp_ret)
        {
            fprintf(stderr, "find error: %d, expected %d\n", result, entry->exp_ret);
            return -1;
        }
        if (! entry->exp_ret)
        {
            if (strcmp(entry->exp_value, bigval))
            {
                fprintf(stderr, "value error: %s, expected %s\n", bigval, entry->exp_value);
                return -1;
            }
            else
            {
                if (entry->index)
                {
                    printf("  element:%s[%d] Value:%s\n", entry->element, (int)entry->index, bigval);
                }
                else
                {
                    printf("  element:%s Value:%s\n", entry->element, bigval);
                }
            }
        }
    }
#endif
    return 0;
}
