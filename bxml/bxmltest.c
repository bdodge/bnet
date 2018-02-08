#include "bnetheaders.h"
#include "bxml.h"

typedef struct
{
    const char *element;
    const char *attrib;
    size_t      index;
    const char *exp_value;
    int         exp_ret;
    const char *xml;
}
xml_test_entry_t;

#define dimoftest(test) (sizeof(test)/sizeof(xml_test_entry_t))

// basics
xml_test_entry_t s_tests_1[] =
{
    {
    "test", "", 0, "", 0,
    "<test/>"
    },
    {
    "hi", "", 0, "", bxml_syntax,
    ""
    },
    {
    "hi", "", 0, "", bxml_syntax,
    "<hi>"
    },
    {
    "hi", "", 0, "", bxml_syntax,
    "<hi>a<hi>"
    },
    {
    "test", "", 0, "result", 0,
    "<test>result</test>"
    },
    {
    "test", "", 0, "", 0,
    "<test/>"
    },
    {
    "test", "", 0, "result", 0,
    "<?xml?><test>result</test>"
    },
    {
    "hi", "", 0, "result", 0,
    "<?xml?><test><hi>result</hi></test>"
    },
    {
    "test", "", 0, "<hi>result</hi>", 0,
    "<?xml?><test><hi>result</hi></test>"
    },
    {
    "hi", "", 0, "result", 0,
    "<?xml?><test><hi>result</hi></test>"
    },
    {
    "bye", "", 0, "result", 0,
    "<?xml?><test><hi><bye>result</bye></hi></test>"
    },
    {
    "bye", "", 0, "result", 0,
    "<?xml?><test><hi>test<test>aaa</test><bye>result</bye></hi></test>"
    },
    {
    "test", "", 0, "done", 0,
    "<?xml?><t1/><t2/><test>done</test>"
    },
    {
    "test", "", 0, "done", 0,
    "<?xml?><t1/><t2/><test>done</test>"
    },
    {
    "test", "", 0, "done", 0,
    "<?xml?><t1/><t2/><test>done</test>"
    },
    {
    "test", "", 0, "done", 0,
    "<?xml?><t1/><t2/><test>done</test><test/>"
    },
    {
    "test", "", 0, "", bxml_not_found,
    "<?xml?><!--<test>-->done</test><test/>"
    },
    {
    "test", "", 0, "", bxml_not_found,
    "<?xml?><!--<test-->done</test><test/>"
    },
    {
    "test", "", 0, "", bxml_not_found,
    "<?xml?><!--test-->done<tester><tester/>"
    },
    {
    "test", "", 0, "", bxml_not_found,
    "<?xml?><!--test-->done<tester></tester>"
    },
    {
    "test", "", 0, "", bxml_not_found,
    "<?xml?><!--test--><teaster>test</teaster>"
    },
    {
    "test", "", 0, "", bxml_not_found,
    "<?xml?><!--test--><tester>test</tester>"
    },
    {
    "c", "", 0, "hello", 0,
    "<?xml?><a>junk<b>morejunk<c>hello</c>moretrail</b>trail</a>"
    },
    {
    "b", "", 0, "morejunk<c>hello</c>moretrail", 0,
    "<?xml?><a>junk<b>morejunk<c>hello</c>moretrail</b>trail</a>"
    },
    {
    "a", "", 0, "junk<b>morejunk<c>hello</c>moretrail</b>trail", 0,
    "<?xml?><a>junk<b>morejunk<c>hello</c>moretrail</b>trail</a>"
    },
    {
    "test", "", 0, "te<>st", 0,
    "<?xml?><test>te&lt;&gt;st</test>"
    },
    {
    "test", "", 0, "te<>", 0,
    "<?xml?><test>te&lt;&gt;</test>"
    },
    {
    "test", "", 0, "<>", 0,
    "<?xml?><test>&lt;&gt;</test>"
    },
    {
    "te&lt;st", "", 0, "<>", 0,
    "<?xml?><te&lt;st>&lt;&gt;</te&lt;st>"
    },
    {
    "test", "", 0, "tset", 0,
    "<?xml?><test:test test='test'>tset</test:test>"
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
    "  </test>\r\n"
    "  <xxxx>\r\n"
    "    <aaaa>\r\n"
    "      <bb>1</bb>\r\n"
    "      <bb>2</bb>\r\n"
    "      <bb>3</bb>\r\n"
    "    </aaaa>\r\n"
    "    <aaaa>\r\n"
    "      <bb>4</bb>\r\n"
    "      <bb>5</bb>\r\n"
    "      <bb>6</bb>\r\n"
    "    </aaaa>\r\n"
    "  </xxxx>\r\n";

// basic indexing
xml_test_entry_t s_tests_2[] =
{
    {
    "test[0]", "", 0, "aaaa", 0,
    s_abc
    },
    {
    "test[1]", "", 0, "bbbb", 0,
    s_abc
    },
    {
    "test[2]", "", 0, "ccc", 0,
    s_abc
    },
    {
    "test", "", 0, "aaaa", 0,
    s_abc
    },
    {
    "test", "", 1, "bbbb", 0,
    s_abc
    },
    {
    "test", "", 2, "ccc", 0,
    s_abc
    },
    {
    "test[1]", "", 1, "none", bxml_parameter,
    s_abc
    },
    {
    "bye", "", 1, "realresult", 0,
    "<?xml?><test><hi><bye>result</bye><bye>realresult</bye></hi></test>"
    },
    {
    "c[1]", "", 0, "bye", 0,
    "<?xml?><a>junk<b>morejunk<c>hello</c>moretrail<c>bye</c></b>trail</a>"
    },
    {
    "c[2]", "", 0, "yes", 0,
    "<?xml?><a>junk<b>morejunk<c>hello</c>moretrail<c>bye</c>ha<h>ha</h>ha<c>yes</c>jjj</b>trail</a>"
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
"      <catalog_itemx>\n"
"          nonono\n"
"      </catalog_itemx>\n"
"   </product>\n"
"</catalog>\n"
"\n";

// comments and processing instructions
xml_test_entry_t s_tests_3[] =
{
    {
    "aaaa.bb[0]", "", 0, "1", 0,
    s_abc
    },
    {
    "aaaa.bb[1]", "", 0, "2", 0,
    s_abc
    },
    {
    "aaaa.bb[2]", "", 0, "3", 0,
    s_abc
    },
    {
    "aaaa.bb[3]", "", 0, "", bxml_not_found,
    s_abc
    },
    {
    "aaaa[1].bb[0]", "", 0, "4", 0,
    s_abc
    },
    {
    "aaaa[1].bb[1]", "", 0, "5", 0,
    s_abc
    },
    {
    "aaaa[1].bb[2]", "", 0, "6", 0,
    s_abc
    },
    {
    "aaaa[1].bb[3]", "", 0, "", bxml_not_found,
    s_abc
    },
    {
    "price", "", 0, "39.95", 0,
    s_xml_1
    },
    {
    "catalog_item.price", "", 0, "39.95", 0,
    s_xml_1
    },
    {
    "catalog.catalog_item[1].price", "", 0, "42.50", 0,
    s_xml_1
    },
    {
    "catalog.catalog_item[1].size[2].color_swatch", "", 0, "Navy", 0,
    s_xml_1
    },
    {
    "catalog.catalog_item[1].size[2].color_swatch[1]", "", 0, "Black", 0,
    s_xml_1
    },
    {
    "catalog_item.catalog_itemx", "", 0, "", bxml_not_found,
    s_xml_1
    },
    {
    "catalog_item[0].size", "", 1,
            "<color_swatch image=\"red_cardigan.jpg\">Red</color_swatch> "
            "<color_swatch image=\"burgundy_cardigan.jpg\">Burgundy</color_swatch>",
            0,
    s_xml_1
    },
};

static const char s_xml_j[] =
    "<?xml version='1.0'?>"
    "<stream:stream to='gmail.com' xml:lang='en' version='1.0'"
    " xmlns:stream='http://etherx.jabber.org/streams'"
    " xmlns='jabber:client'"
    "></stream>";

xml_test_entry_t s_tests_4[] =
{
    {
    "aaaa", "a", 0, "a", 0,
    "<aaaa a=\"a\" b=\"b\"></aaaa>"
    },
    {
    "aaaa", "b", 0, "b", 0,
    "<aaaa a=\"a\" b=\"b\"></aaaa>"
    },
    {
    "aaaa", "a", 0, "c<", 0,
    "<aaaa a=\"c&lt;\" b=\"b&lt;\"></aaaa>"
    },
    {
    "aaaa", "b", 0, "b<", 0,
    "<aaaa a=\"a\" b=\"b&lt;\"></aaaa>"
    },
    {
    "a", "b", 0, "c", 0,
    "<a a=\"a\" b=\"c\"/>"
    },
    {
    "catalog.product.catalog_item.size.color_swatch[1]", "image", 0, "burgundy_cardigan.jpg", 0,
    s_xml_1
    },
    {
    "aaaa", "a", 0, "", -bxml_syntax,
    "<aaaa a=a></aaaa>"
    },
    {
    "aaaa", "b", 0, "", -bxml_syntax,
    "<aaaa a=a></aaaa>"
    },
    {
    "aaaa", "b", 0, "", -bxml_not_found,
    "<aaaa a=\"a\"></aaaa>"
    },
    {
    "stream", "to", 0, "gmail.com", 0,
    s_xml_j
    },
};

static int runtest(xml_test_entry_t *entry, int numentries, char pathdelim)
{
    char val[1024];
    const char *ptag;
    size_t val_len;
    int result;
    int i;

    for (i = 0; i < numentries; i++, entry++)
    {
        result = bxml_find_and_copy_nth_element(entry->xml, entry->element,
                pathdelim, entry->index, val, sizeof(val), false, false);
        printf(" Test %d\n", i);
        if ((result != entry->exp_ret) && (entry->exp_ret >= 0))
        {
            fprintf(stderr, "find error: %d, expected %d\n", result, entry->exp_ret);
            return -1;
        }
        if (entry->exp_ret <= 0)
        {
            if (entry->attrib && entry->attrib[0])
            {
                result = bxml_find_nth_element(entry->xml, entry->element,
                        pathdelim, entry->index, &ptag, NULL, NULL);

                if (result)
                {
                    fprintf(stderr, "re-find error: %d\n", result);
                    return -1;
                }
                result = bxml_find_and_copy_attribute(NULL, ptag,
                        entry->attrib, val, sizeof(val), false, false);
                if ((result != -entry->exp_ret) && (entry->exp_ret <= 0))
                {
                    fprintf(stderr, "attrib find error: %d, expected %d\n", result, -entry->exp_ret);
                    return -1;
                }
            }
            if (entry->exp_ret == 0)
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
                        printf("  %s:%s Value:%s\n",
                            (entry->attrib && entry->attrib[0]) ? "attribute" : "element",
                                entry->element, val);
                    }
                }
            }
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    bxml_parser_t *pjx;
    const char *element;
    size_t elindex;
    char val[1024];
    int result;

#if 1
    // basic
    printf("Basic Test\n");
    result = runtest(s_tests_1, sizeof(s_tests_1) / sizeof(xml_test_entry_t),'\0');
    if (result)
    {
        return result;
    }
#endif
#if 1
    // basic indexing
    printf("Basic Indexing Test\n");
    result = runtest(s_tests_2, dimoftest(s_tests_2), '\0');
    if (result)
    {
        return result;
    }
#endif
#if 1
    // real xml
    printf("Basic XML Test\n");
    result = runtest(s_tests_3, dimoftest(s_tests_3), '.');
    if (result)
    {
        return result;
    }
#endif
#if 1
    // real xml
    printf("Basic Attributes Test\n");
    result = runtest(s_tests_4, dimoftest(s_tests_4), '.');
    if (result)
    {
        return result;
    }
#endif
    return 0;
}
