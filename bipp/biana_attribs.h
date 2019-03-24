/*
 * Generated File -- Consider not editing
 *
*/
#ifndef BIANA_ATTRIBS_H
#define BIANA_ATTRIBS_H 1

#include "bippattr.h"

typedef struct tag_col_xref
{
    char           *name;
    ipp_attr_rec_t *col_attr;
    size_t          num_members;
}
ipp_collection_xref_t;

typedef struct tag_grp_xref
{
    ipp_attr_grouping_code_t  group_pname;
    ipp_attr_t               *group_attrs;
    size_t                    num_attr;
}
ipp_group_xref_t;

#endif
