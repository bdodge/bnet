/*
 * Generated File -- Consider not editing
 */
#include "biana_attribs.h"

// Members of collection baling.finishings-col
//
ipp_attr_rec_t s_ipp_col_baling__finishings_col[] = 
{
    {   // index 0
        "baling-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "baling-when",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection binding.finishings-col
//
ipp_attr_rec_t s_ipp_col_binding__finishings_col[] = 
{
    {   // index 0
        "binding-reference-edge",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "binding-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection coating.finishings-col
//
ipp_attr_rec_t s_ipp_col_coating__finishings_col[] = 
{
    {   // index 0
        "coating-sides",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "coating-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection cover-back
//
ipp_attr_rec_t s_ipp_col_cover_back[] = 
{
    {   // index 0
        "cover-type",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 2
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection cover-back-actual
//
ipp_attr_rec_t s_ipp_col_cover_back_actual[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection cover-back-default
//
ipp_attr_rec_t s_ipp_col_cover_back_default[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection cover-front
//
ipp_attr_rec_t s_ipp_col_cover_front[] = 
{
    {   // index 0
        "cover-type",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 2
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection cover-front-actual
//
ipp_attr_rec_t s_ipp_col_cover_front_actual[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection cover-front-default
//
ipp_attr_rec_t s_ipp_col_cover_front_default[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection cover-sheet-info
//
ipp_attr_rec_t s_ipp_col_cover_sheet_info[] = 
{
    {   // index 0
        "from-name",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "logo",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 3
        "organization-name",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 4
        "subject",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 5
        "to-name",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection cover-sheet-info-default
//
ipp_attr_rec_t s_ipp_col_cover_sheet_info_default[] = 
{
    {   // index 0
        "*cover-sheet-info",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection covering.finishings-col
//
ipp_attr_rec_t s_ipp_col_covering__finishings_col[] = 
{
    {   // index 0
        "covering-name",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection destination-accesses
//
ipp_attr_rec_t s_ipp_col_destination_accesses[] = 
{
    {   // index 0
        "access-oauth-token",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "access-oauth-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "access-password",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 3
        "access-pin",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 4
        "access-user-name",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 5
        "access-x509-certificate",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 6
        "access-x509-certificate(obsolete)",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection destination-statuses
//
ipp_attr_rec_t s_ipp_col_destination_statuses[] = 
{
    {   // index 0
        "destination-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "images-completed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "transmission-status",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection destination-uri-ready
//
ipp_attr_rec_t s_ipp_col_destination_uri_ready[] = 
{
    {   // index 0
        "destination-attributes",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "destination-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "destination-info",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 3
        "destination-is-directory",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 4
        "destination-mandatory-access-attributes",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 5
        "destination-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 6
        "destination-oauth-scope",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 7
        "destination-oauth-token",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 8
        "destination-oauth-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 9
        "destination-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection destination-uris
//
ipp_attr_rec_t s_ipp_col_destination_uris[] = 
{
    {   // index 0
        "destination-attributes",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "destination-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "post-dial-string",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 3
        "pre-dial-string",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 4
        "t33-subaddress",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection document-access
//
ipp_attr_rec_t s_ipp_col_document_access[] = 
{
    {   // index 0
        "access-oauth-token",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "access-oauth-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "access-password",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 3
        "access-pin",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 4
        "access-user-name",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 5
        "access-x509-certificate",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 6
        "access-x509-certificate(obsolete)",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection document-format-details
//
ipp_attr_rec_t s_ipp_col_document_format_details[] = 
{
    {   // index 0
        "document-format",   
        { IPP_MIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "document-format-device-id",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 2
        "document-format-version",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 3
        "document-natural-language",   
        { IPP_LANGUAGE | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 4
        "document-source-application-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 5
        "document-source-application-version",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 6
        "document-source-os-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 40
    },
    {   // index 7
        "document-source-os-version",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 40
    },
};

// Members of collection document-format-details-default
//
ipp_attr_rec_t s_ipp_col_document_format_details_default[] = 
{
    {   // index 0
        "*document-format-details",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection document-format-details-detected
//
ipp_attr_rec_t s_ipp_col_document_format_details_detected[] = 
{
    {   // index 0
        "*document-format-details",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection document-format-details-supplied
//
ipp_attr_rec_t s_ipp_col_document_format_details_supplied[] = 
{
    {   // index 0
        "*document-format-details",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection finishings-col
//
ipp_attr_rec_t s_ipp_col_finishings_col[] = 
{
    {   // index 0
        "baling",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "binding",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "coating",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 3
        "covering",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 4
        "finishing-template",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 5
        "finishing-template(extension)",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 6
        "folding",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 7
        "imposition-template",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 8
        "laminating",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 9
        "media-sheets-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 10
        "media-size",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 11
        "media-size-name",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 12
        "punching",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 13
        "stitching",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 14
        "trimming",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection finishings-col-actual
//
ipp_attr_rec_t s_ipp_col_finishings_col_actual[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection finishings-col-default
//
ipp_attr_rec_t s_ipp_col_finishings_col_default[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection finishings-col-ready
//
ipp_attr_rec_t s_ipp_col_finishings_col_ready[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection folding.finishings-col
//
ipp_attr_rec_t s_ipp_col_folding__finishings_col[] = 
{
    {   // index 0
        "folding-direction",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "folding-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "folding-reference-edge",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection impressions-col
//
ipp_attr_rec_t s_ipp_col_impressions_col[] = 
{
    {   // index 0
        "blank",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "blank-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "full-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 3
        "full-color-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 4
        "highlight-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 5
        "highlight-color-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 6
        "monochrome",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 7
        "monochrome-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection impressions-completed-col
//
ipp_attr_rec_t s_ipp_col_impressions_completed_col[] = 
{
    {   // index 0
        "*impressions-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection input-attributes
//
ipp_attr_rec_t s_ipp_col_input_attributes[] = 
{
    {   // index 0
        "input-auto-scaling",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "input-auto-skew-correction",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "input-brightness",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 3
        "input-color-mode",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 4
        "input-content-type",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 5
        "input-contrast",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 6
        "input-film-scan-mode",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 7
        "input-images-to-transfer",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 8
        "input-media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 9
        "input-orientation-requested",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 10
        "input-quality",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 11
        "input-resolution",   
        { IPP_RESOLUTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 12
        "input-scaling-height",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 1000
    },
    {   // index 13
        "input-scaling-width",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 1000
    },
    {   // index 14
        "input-scan-regions",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 15
        "input-sharpness",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 16
        "input-sides",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 17
        "input-source",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection input-scan-regions.input-attributes
//
ipp_attr_rec_t s_ipp_col_input_scan_regions__input_attributes[] = 
{
    {   // index 0
        "x-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "x-origin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "y-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 3
        "y-origin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection input-scan-regions-supported
//
ipp_attr_rec_t s_ipp_col_input_scan_regions_supported[] = 
{
    {   // index 0
        "x-dimension",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "x-origin",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "y-dimension",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 3
        "y-origin",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection insert-sheet
//
ipp_attr_rec_t s_ipp_col_insert_sheet[] = 
{
    {   // index 0
        "insert-after-page-number",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "insert-count",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 3
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection insert-sheet-actual
//
ipp_attr_rec_t s_ipp_col_insert_sheet_actual[] = 
{
    {   // index 0
        "*insert-sheet",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection insert-sheet-default
//
ipp_attr_rec_t s_ipp_col_insert_sheet_default[] = 
{
    {   // index 0
        "*insert-sheet",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-accounting-sheets
//
ipp_attr_rec_t s_ipp_col_job_accounting_sheets[] = 
{
    {   // index 0
        "job-accounting-output-bin",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "job-accounting-sheets-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 2
        "media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 3
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-accounting-sheets-actual
//
ipp_attr_rec_t s_ipp_col_job_accounting_sheets_actual[] = 
{
    {   // index 0
        "*job-accounting-sheets",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-accounting-sheets-default
//
ipp_attr_rec_t s_ipp_col_job_accounting_sheets_default[] = 
{
    {   // index 0
        "*job-accounting-sheets",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-constraints-supported
//
ipp_attr_rec_t s_ipp_col_job_constraints_supported[] = 
{
    {   // index 0
        "resolver-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection job-cover-back
//
ipp_attr_rec_t s_ipp_col_job_cover_back[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-cover-back-actual
//
ipp_attr_rec_t s_ipp_col_job_cover_back_actual[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-cover-back-default
//
ipp_attr_rec_t s_ipp_col_job_cover_back_default[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-cover-front
//
ipp_attr_rec_t s_ipp_col_job_cover_front[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-cover-front-actual
//
ipp_attr_rec_t s_ipp_col_job_cover_front_actual[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-cover-front-default
//
ipp_attr_rec_t s_ipp_col_job_cover_front_default[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-error-sheet
//
ipp_attr_rec_t s_ipp_col_job_error_sheet[] = 
{
    {   // index 0
        "job-error-sheet-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "job-error-sheet-when",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 3
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-error-sheet-actual
//
ipp_attr_rec_t s_ipp_col_job_error_sheet_actual[] = 
{
    {   // index 0
        "*job-error-sheet",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-error-sheet-default
//
ipp_attr_rec_t s_ipp_col_job_error_sheet_default[] = 
{
    {   // index 0
        "*job-error-sheet",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-finishings-col
//
ipp_attr_rec_t s_ipp_col_job_finishings_col[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-finishings-col-actual
//
ipp_attr_rec_t s_ipp_col_job_finishings_col_actual[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-finishings-col-default
//
ipp_attr_rec_t s_ipp_col_job_finishings_col_default[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-finishings-col-ready
//
ipp_attr_rec_t s_ipp_col_job_finishings_col_ready[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-impressions-col
//
ipp_attr_rec_t s_ipp_col_job_impressions_col[] = 
{
    {   // index 0
        "blank",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "blank-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "full-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 3
        "full-color-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 4
        "highlight-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 5
        "highlight-color-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 6
        "monochrome",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 7
        "monochrome-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection job-impressions-completed-col
//
ipp_attr_rec_t s_ipp_col_job_impressions_completed_col[] = 
{
    {   // index 0
        "*job-impressions-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-media-sheets-col
//
ipp_attr_rec_t s_ipp_col_job_media_sheets_col[] = 
{
    {   // index 0
        "blank",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "full-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "highlight-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 3
        "monochrome",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection job-media-sheets-completed-col
//
ipp_attr_rec_t s_ipp_col_job_media_sheets_completed_col[] = 
{
    {   // index 0
        "*job-media-sheets-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-pages-col
//
ipp_attr_rec_t s_ipp_col_job_pages_col[] = 
{
    {   // index 0
        "full-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "monochrome",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection job-pages-completed-col
//
ipp_attr_rec_t s_ipp_col_job_pages_completed_col[] = 
{
    {   // index 0
        "*job-pages-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-presets-supported
//
ipp_attr_rec_t s_ipp_col_job_presets_supported[] = 
{
    {   // index 0
        "preset-name",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection job-resolvers-supported
//
ipp_attr_rec_t s_ipp_col_job_resolvers_supported[] = 
{
    {   // index 0
        "resolver-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection job-save-disposition
//
ipp_attr_rec_t s_ipp_col_job_save_disposition[] = 
{
    {   // index 0
        "save-disposition",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "save-info",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-sheets-col
//
ipp_attr_rec_t s_ipp_col_job_sheets_col[] = 
{
    {   // index 0
        "job-sheets",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 2
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-sheets-col-actual
//
ipp_attr_rec_t s_ipp_col_job_sheets_col_actual[] = 
{
    {   // index 0
        "*job-sheets-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-sheets-col-default
//
ipp_attr_rec_t s_ipp_col_job_sheets_col_default[] = 
{
    {   // index 0
        "*job-sheets-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection job-triggers-supported
//
ipp_attr_rec_t s_ipp_col_job_triggers_supported[] = 
{
    {   // index 0
        "preset-name",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection laminating.finishings-col
//
ipp_attr_rec_t s_ipp_col_laminating__finishings_col[] = 
{
    {   // index 0
        "laminating-sides",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "laminating-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection materials-col
//
ipp_attr_rec_t s_ipp_col_materials_col[] = 
{
    {   // index 0
        "material-amount",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "material-amount-units",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "material-color",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 3
        "material-diameter",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 4
        "material-diameter-tolerance",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 5
        "material-fill-density",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 6
        "material-key",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 7
        "material-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 8
        "material-purpose",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 9
        "material-rate",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 10
        "material-rate-units",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 11
        "material-shell-thickness",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 12
        "material-temperature",   
        { IPP_INTEGER, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 13
        "material-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection media-col
//
ipp_attr_rec_t s_ipp_col_media_col[] = 
{
    {   // index 0
        "media-back-coating",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "media-bottom-margin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "media-color",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 3
        "media-front-coating",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 4
        "media-grain",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 5
        "media-hole-count",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 6
        "media-info",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 255
    },
    {   // index 7
        "media-key",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 8
        "media-left-margin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 9
        "media-order-count",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 10
        "media-pre-printed",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 11
        "media-recycled",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 12
        "media-right-margin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 13
        "media-size",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 14
        "media-size-name",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 15
        "media-source",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 16
        "media-thickness",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 17
        "media-tooth",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 18
        "media-top-margin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 19
        "media-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 20
        "media-weight-metric",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection media-col-actual
//
ipp_attr_rec_t s_ipp_col_media_col_actual[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection media-col-database
//
ipp_attr_rec_t s_ipp_col_media_col_database[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "media-source-properties",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection media-col-default
//
ipp_attr_rec_t s_ipp_col_media_col_default[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection media-col-ready
//
ipp_attr_rec_t s_ipp_col_media_col_ready[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "media-source-properties",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection media-sheets-col
//
ipp_attr_rec_t s_ipp_col_media_sheets_col[] = 
{
    {   // index 0
        "blank",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "full-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "highlight-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 3
        "monochrome",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection media-sheets-completed-col
//
ipp_attr_rec_t s_ipp_col_media_sheets_completed_col[] = 
{
    {   // index 0
        "*media-sheets-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection media-size.media-col
//
ipp_attr_rec_t s_ipp_col_media_size__media_col[] = 
{
    {   // index 0
        "x-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "y-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection media-size-supported
//
ipp_attr_rec_t s_ipp_col_media_size_supported[] = 
{
    {   // index 0
        "x-dimension",   
        { IPP_INTEGER, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 1
        "y-dimension",   
        { IPP_INTEGER, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
};

// Members of collection media-source-properties.media-col-database
//
ipp_attr_rec_t s_ipp_col_media_source_properties__media_col_database[] = 
{
    {   // index 0
        "media-source-feed-direction",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "media-source-feed-orientation",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection object-offset.print-objects
//
ipp_attr_rec_t s_ipp_col_object_offset__print_objects[] = 
{
    {   // index 0
        "x-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "y-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "z-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection object-size.print-objects
//
ipp_attr_rec_t s_ipp_col_object_size__print_objects[] = 
{
    {   // index 0
        "x-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 1
        "y-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 2
        "z-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
};

// Members of collection output-attributes
//
ipp_attr_rec_t s_ipp_col_output_attributes[] = 
{
    {   // index 0
        "noise-removal",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 1
        "output-compression-quality-factor",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
};

// Members of collection overrides
//
ipp_attr_rec_t s_ipp_col_overrides[] = 
{
    {   // index 0
        "<Any Job Template attribute>",   
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "document-copies",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "document-numbers",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 3
        "pages",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection overrides-actual
//
ipp_attr_rec_t s_ipp_col_overrides_actual[] = 
{
    {   // index 0
        "*overrides",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection pages-col
//
ipp_attr_rec_t s_ipp_col_pages_col[] = 
{
    {   // index 0
        "full-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "monochrome",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection pages-completed-col
//
ipp_attr_rec_t s_ipp_col_pages_completed_col[] = 
{
    {   // index 0
        "*pages-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection pdl-init-file
//
ipp_attr_rec_t s_ipp_col_pdl_init_file[] = 
{
    {   // index 0
        "pdl-init-file-entry",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "pdl-init-file-location",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "pdl-init-file-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection pdl-init-file-default
//
ipp_attr_rec_t s_ipp_col_pdl_init_file_default[] = 
{
    {   // index 0
        "*pdl-init-file",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection print-accuracy
//
ipp_attr_rec_t s_ipp_col_print_accuracy[] = 
{
    {   // index 0
        "accuracy-units",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "x-accuracy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "y-accuracy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 3
        "z-accuracy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection print-accuracy-supported
//
ipp_attr_rec_t s_ipp_col_print_accuracy_supported[] = 
{
    {   // index 0
        "accuracy-units",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "x-accuracy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "y-accuracy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 3
        "z-accuracy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection print-objects
//
ipp_attr_rec_t s_ipp_col_print_objects[] = 
{
    {   // index 0
        "document-number",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 1
        "object-offset",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "object-size",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 3
        "object-uuid",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection printer-icc-profiles
//
ipp_attr_rec_t s_ipp_col_printer_icc_profiles[] = 
{
    {   // index 0
        "profile-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "profile-url",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection printer-volume-supported
//
ipp_attr_rec_t s_ipp_col_printer_volume_supported[] = 
{
    {   // index 0
        "x-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 1
        "y-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 2
        "z-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
};

// Members of collection printer-xri-supported
//
ipp_attr_rec_t s_ipp_col_printer_xri_supported[] = 
{
    {   // index 0
        "xri-authentication",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "xri-security",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "xri-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection proof-print
//
ipp_attr_rec_t s_ipp_col_proof_print[] = 
{
    {   // index 0
        "media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "proof-print-copies",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

// Members of collection proof-print-default
//
ipp_attr_rec_t s_ipp_col_proof_print_default[] = 
{
    {   // index 0
        "*proof-print",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection punching.finishings-col
//
ipp_attr_rec_t s_ipp_col_punching__finishings_col[] = 
{
    {   // index 0
        "punching-locations",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "punching-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "punching-reference-edge",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection save-info.job-save-disposition
//
ipp_attr_rec_t s_ipp_col_save_info__job_save_disposition[] = 
{
    {   // index 0
        "save-document-format",   
        { IPP_MIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "save-location",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "save-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
};

// Members of collection separator-sheets
//
ipp_attr_rec_t s_ipp_col_separator_sheets[] = 
{
    {   // index 0
        "media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 1
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "separator-sheets-type",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection separator-sheets-actual
//
ipp_attr_rec_t s_ipp_col_separator_sheets_actual[] = 
{
    {   // index 0
        "*separator-sheets",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection separator-sheets-default
//
ipp_attr_rec_t s_ipp_col_separator_sheets_default[] = 
{
    {   // index 0
        "*separator-sheets",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection stitching.finishings-col
//
ipp_attr_rec_t s_ipp_col_stitching__finishings_col[] = 
{
    {   // index 0
        "stitching-angle",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 359
    },
    {   // index 1
        "stitching-locations",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 2
        "stitching-method",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 3
        "stitching-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 4
        "stitching-reference-edge",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

// Members of collection trimming.finishings-col
//
ipp_attr_rec_t s_ipp_col_trimming__finishings_col[] = 
{
    {   // index 0
        "trimming-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 1
        "trimming-reference-edge",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "trimming-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 3
        "trimming-when",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
};

struct tag_col_xref s_ipp_collection_xref[] =
{
    { "baling.finishings-col", s_ipp_col_baling__finishings_col, (sizeof(s_ipp_col_baling__finishings_col)/sizeof(ipp_attr_rec_t)) },
    { "binding.finishings-col", s_ipp_col_binding__finishings_col, (sizeof(s_ipp_col_binding__finishings_col)/sizeof(ipp_attr_rec_t)) },
    { "coating.finishings-col", s_ipp_col_coating__finishings_col, (sizeof(s_ipp_col_coating__finishings_col)/sizeof(ipp_attr_rec_t)) },
    { "cover-back", s_ipp_col_cover_back, (sizeof(s_ipp_col_cover_back)/sizeof(ipp_attr_rec_t)) },
    { "cover-back-actual", s_ipp_col_cover_back_actual, (sizeof(s_ipp_col_cover_back_actual)/sizeof(ipp_attr_rec_t)) },
    { "cover-back-default", s_ipp_col_cover_back_default, (sizeof(s_ipp_col_cover_back_default)/sizeof(ipp_attr_rec_t)) },
    { "cover-front", s_ipp_col_cover_front, (sizeof(s_ipp_col_cover_front)/sizeof(ipp_attr_rec_t)) },
    { "cover-front-actual", s_ipp_col_cover_front_actual, (sizeof(s_ipp_col_cover_front_actual)/sizeof(ipp_attr_rec_t)) },
    { "cover-front-default", s_ipp_col_cover_front_default, (sizeof(s_ipp_col_cover_front_default)/sizeof(ipp_attr_rec_t)) },
    { "cover-sheet-info", s_ipp_col_cover_sheet_info, (sizeof(s_ipp_col_cover_sheet_info)/sizeof(ipp_attr_rec_t)) },
    { "cover-sheet-info-default", s_ipp_col_cover_sheet_info_default, (sizeof(s_ipp_col_cover_sheet_info_default)/sizeof(ipp_attr_rec_t)) },
    { "covering.finishings-col", s_ipp_col_covering__finishings_col, (sizeof(s_ipp_col_covering__finishings_col)/sizeof(ipp_attr_rec_t)) },
    { "destination-accesses", s_ipp_col_destination_accesses, (sizeof(s_ipp_col_destination_accesses)/sizeof(ipp_attr_rec_t)) },
    { "destination-statuses", s_ipp_col_destination_statuses, (sizeof(s_ipp_col_destination_statuses)/sizeof(ipp_attr_rec_t)) },
    { "destination-uri-ready", s_ipp_col_destination_uri_ready, (sizeof(s_ipp_col_destination_uri_ready)/sizeof(ipp_attr_rec_t)) },
    { "destination-uris", s_ipp_col_destination_uris, (sizeof(s_ipp_col_destination_uris)/sizeof(ipp_attr_rec_t)) },
    { "document-access", s_ipp_col_document_access, (sizeof(s_ipp_col_document_access)/sizeof(ipp_attr_rec_t)) },
    { "document-format-details", s_ipp_col_document_format_details, (sizeof(s_ipp_col_document_format_details)/sizeof(ipp_attr_rec_t)) },
    { "document-format-details-default", s_ipp_col_document_format_details_default, (sizeof(s_ipp_col_document_format_details_default)/sizeof(ipp_attr_rec_t)) },
    { "document-format-details-detected", s_ipp_col_document_format_details_detected, (sizeof(s_ipp_col_document_format_details_detected)/sizeof(ipp_attr_rec_t)) },
    { "document-format-details-supplied", s_ipp_col_document_format_details_supplied, (sizeof(s_ipp_col_document_format_details_supplied)/sizeof(ipp_attr_rec_t)) },
    { "finishings-col", s_ipp_col_finishings_col, (sizeof(s_ipp_col_finishings_col)/sizeof(ipp_attr_rec_t)) },
    { "finishings-col-actual", s_ipp_col_finishings_col_actual, (sizeof(s_ipp_col_finishings_col_actual)/sizeof(ipp_attr_rec_t)) },
    { "finishings-col-default", s_ipp_col_finishings_col_default, (sizeof(s_ipp_col_finishings_col_default)/sizeof(ipp_attr_rec_t)) },
    { "finishings-col-ready", s_ipp_col_finishings_col_ready, (sizeof(s_ipp_col_finishings_col_ready)/sizeof(ipp_attr_rec_t)) },
    { "folding.finishings-col", s_ipp_col_folding__finishings_col, (sizeof(s_ipp_col_folding__finishings_col)/sizeof(ipp_attr_rec_t)) },
    { "impressions-col", s_ipp_col_impressions_col, (sizeof(s_ipp_col_impressions_col)/sizeof(ipp_attr_rec_t)) },
    { "impressions-completed-col", s_ipp_col_impressions_completed_col, (sizeof(s_ipp_col_impressions_completed_col)/sizeof(ipp_attr_rec_t)) },
    { "input-attributes", s_ipp_col_input_attributes, (sizeof(s_ipp_col_input_attributes)/sizeof(ipp_attr_rec_t)) },
    { "input-scan-regions.input-attributes", s_ipp_col_input_scan_regions__input_attributes, (sizeof(s_ipp_col_input_scan_regions__input_attributes)/sizeof(ipp_attr_rec_t)) },
    { "input-scan-regions-supported", s_ipp_col_input_scan_regions_supported, (sizeof(s_ipp_col_input_scan_regions_supported)/sizeof(ipp_attr_rec_t)) },
    { "insert-sheet", s_ipp_col_insert_sheet, (sizeof(s_ipp_col_insert_sheet)/sizeof(ipp_attr_rec_t)) },
    { "insert-sheet-actual", s_ipp_col_insert_sheet_actual, (sizeof(s_ipp_col_insert_sheet_actual)/sizeof(ipp_attr_rec_t)) },
    { "insert-sheet-default", s_ipp_col_insert_sheet_default, (sizeof(s_ipp_col_insert_sheet_default)/sizeof(ipp_attr_rec_t)) },
    { "job-accounting-sheets", s_ipp_col_job_accounting_sheets, (sizeof(s_ipp_col_job_accounting_sheets)/sizeof(ipp_attr_rec_t)) },
    { "job-accounting-sheets-actual", s_ipp_col_job_accounting_sheets_actual, (sizeof(s_ipp_col_job_accounting_sheets_actual)/sizeof(ipp_attr_rec_t)) },
    { "job-accounting-sheets-default", s_ipp_col_job_accounting_sheets_default, (sizeof(s_ipp_col_job_accounting_sheets_default)/sizeof(ipp_attr_rec_t)) },
    { "job-constraints-supported", s_ipp_col_job_constraints_supported, (sizeof(s_ipp_col_job_constraints_supported)/sizeof(ipp_attr_rec_t)) },
    { "job-cover-back", s_ipp_col_job_cover_back, (sizeof(s_ipp_col_job_cover_back)/sizeof(ipp_attr_rec_t)) },
    { "job-cover-back-actual", s_ipp_col_job_cover_back_actual, (sizeof(s_ipp_col_job_cover_back_actual)/sizeof(ipp_attr_rec_t)) },
    { "job-cover-back-default", s_ipp_col_job_cover_back_default, (sizeof(s_ipp_col_job_cover_back_default)/sizeof(ipp_attr_rec_t)) },
    { "job-cover-front", s_ipp_col_job_cover_front, (sizeof(s_ipp_col_job_cover_front)/sizeof(ipp_attr_rec_t)) },
    { "job-cover-front-actual", s_ipp_col_job_cover_front_actual, (sizeof(s_ipp_col_job_cover_front_actual)/sizeof(ipp_attr_rec_t)) },
    { "job-cover-front-default", s_ipp_col_job_cover_front_default, (sizeof(s_ipp_col_job_cover_front_default)/sizeof(ipp_attr_rec_t)) },
    { "job-error-sheet", s_ipp_col_job_error_sheet, (sizeof(s_ipp_col_job_error_sheet)/sizeof(ipp_attr_rec_t)) },
    { "job-error-sheet-actual", s_ipp_col_job_error_sheet_actual, (sizeof(s_ipp_col_job_error_sheet_actual)/sizeof(ipp_attr_rec_t)) },
    { "job-error-sheet-default", s_ipp_col_job_error_sheet_default, (sizeof(s_ipp_col_job_error_sheet_default)/sizeof(ipp_attr_rec_t)) },
    { "job-finishings-col", s_ipp_col_job_finishings_col, (sizeof(s_ipp_col_job_finishings_col)/sizeof(ipp_attr_rec_t)) },
    { "job-finishings-col-actual", s_ipp_col_job_finishings_col_actual, (sizeof(s_ipp_col_job_finishings_col_actual)/sizeof(ipp_attr_rec_t)) },
    { "job-finishings-col-default", s_ipp_col_job_finishings_col_default, (sizeof(s_ipp_col_job_finishings_col_default)/sizeof(ipp_attr_rec_t)) },
    { "job-finishings-col-ready", s_ipp_col_job_finishings_col_ready, (sizeof(s_ipp_col_job_finishings_col_ready)/sizeof(ipp_attr_rec_t)) },
    { "job-impressions-col", s_ipp_col_job_impressions_col, (sizeof(s_ipp_col_job_impressions_col)/sizeof(ipp_attr_rec_t)) },
    { "job-impressions-completed-col", s_ipp_col_job_impressions_completed_col, (sizeof(s_ipp_col_job_impressions_completed_col)/sizeof(ipp_attr_rec_t)) },
    { "job-media-sheets-col", s_ipp_col_job_media_sheets_col, (sizeof(s_ipp_col_job_media_sheets_col)/sizeof(ipp_attr_rec_t)) },
    { "job-media-sheets-completed-col", s_ipp_col_job_media_sheets_completed_col, (sizeof(s_ipp_col_job_media_sheets_completed_col)/sizeof(ipp_attr_rec_t)) },
    { "job-pages-col", s_ipp_col_job_pages_col, (sizeof(s_ipp_col_job_pages_col)/sizeof(ipp_attr_rec_t)) },
    { "job-pages-completed-col", s_ipp_col_job_pages_completed_col, (sizeof(s_ipp_col_job_pages_completed_col)/sizeof(ipp_attr_rec_t)) },
    { "job-presets-supported", s_ipp_col_job_presets_supported, (sizeof(s_ipp_col_job_presets_supported)/sizeof(ipp_attr_rec_t)) },
    { "job-resolvers-supported", s_ipp_col_job_resolvers_supported, (sizeof(s_ipp_col_job_resolvers_supported)/sizeof(ipp_attr_rec_t)) },
    { "job-save-disposition", s_ipp_col_job_save_disposition, (sizeof(s_ipp_col_job_save_disposition)/sizeof(ipp_attr_rec_t)) },
    { "job-sheets-col", s_ipp_col_job_sheets_col, (sizeof(s_ipp_col_job_sheets_col)/sizeof(ipp_attr_rec_t)) },
    { "job-sheets-col-actual", s_ipp_col_job_sheets_col_actual, (sizeof(s_ipp_col_job_sheets_col_actual)/sizeof(ipp_attr_rec_t)) },
    { "job-sheets-col-default", s_ipp_col_job_sheets_col_default, (sizeof(s_ipp_col_job_sheets_col_default)/sizeof(ipp_attr_rec_t)) },
    { "job-triggers-supported", s_ipp_col_job_triggers_supported, (sizeof(s_ipp_col_job_triggers_supported)/sizeof(ipp_attr_rec_t)) },
    { "laminating.finishings-col", s_ipp_col_laminating__finishings_col, (sizeof(s_ipp_col_laminating__finishings_col)/sizeof(ipp_attr_rec_t)) },
    { "materials-col", s_ipp_col_materials_col, (sizeof(s_ipp_col_materials_col)/sizeof(ipp_attr_rec_t)) },
    { "media-col", s_ipp_col_media_col, (sizeof(s_ipp_col_media_col)/sizeof(ipp_attr_rec_t)) },
    { "media-col-actual", s_ipp_col_media_col_actual, (sizeof(s_ipp_col_media_col_actual)/sizeof(ipp_attr_rec_t)) },
    { "media-col-database", s_ipp_col_media_col_database, (sizeof(s_ipp_col_media_col_database)/sizeof(ipp_attr_rec_t)) },
    { "media-col-default", s_ipp_col_media_col_default, (sizeof(s_ipp_col_media_col_default)/sizeof(ipp_attr_rec_t)) },
    { "media-col-ready", s_ipp_col_media_col_ready, (sizeof(s_ipp_col_media_col_ready)/sizeof(ipp_attr_rec_t)) },
    { "media-sheets-col", s_ipp_col_media_sheets_col, (sizeof(s_ipp_col_media_sheets_col)/sizeof(ipp_attr_rec_t)) },
    { "media-sheets-completed-col", s_ipp_col_media_sheets_completed_col, (sizeof(s_ipp_col_media_sheets_completed_col)/sizeof(ipp_attr_rec_t)) },
    { "media-size.media-col", s_ipp_col_media_size__media_col, (sizeof(s_ipp_col_media_size__media_col)/sizeof(ipp_attr_rec_t)) },
    { "media-size-supported", s_ipp_col_media_size_supported, (sizeof(s_ipp_col_media_size_supported)/sizeof(ipp_attr_rec_t)) },
    { "media-source-properties.media-col-database", s_ipp_col_media_source_properties__media_col_database, (sizeof(s_ipp_col_media_source_properties__media_col_database)/sizeof(ipp_attr_rec_t)) },
    { "object-offset.print-objects", s_ipp_col_object_offset__print_objects, (sizeof(s_ipp_col_object_offset__print_objects)/sizeof(ipp_attr_rec_t)) },
    { "object-size.print-objects", s_ipp_col_object_size__print_objects, (sizeof(s_ipp_col_object_size__print_objects)/sizeof(ipp_attr_rec_t)) },
    { "output-attributes", s_ipp_col_output_attributes, (sizeof(s_ipp_col_output_attributes)/sizeof(ipp_attr_rec_t)) },
    { "overrides", s_ipp_col_overrides, (sizeof(s_ipp_col_overrides)/sizeof(ipp_attr_rec_t)) },
    { "overrides-actual", s_ipp_col_overrides_actual, (sizeof(s_ipp_col_overrides_actual)/sizeof(ipp_attr_rec_t)) },
    { "pages-col", s_ipp_col_pages_col, (sizeof(s_ipp_col_pages_col)/sizeof(ipp_attr_rec_t)) },
    { "pages-completed-col", s_ipp_col_pages_completed_col, (sizeof(s_ipp_col_pages_completed_col)/sizeof(ipp_attr_rec_t)) },
    { "pdl-init-file", s_ipp_col_pdl_init_file, (sizeof(s_ipp_col_pdl_init_file)/sizeof(ipp_attr_rec_t)) },
    { "pdl-init-file-default", s_ipp_col_pdl_init_file_default, (sizeof(s_ipp_col_pdl_init_file_default)/sizeof(ipp_attr_rec_t)) },
    { "print-accuracy", s_ipp_col_print_accuracy, (sizeof(s_ipp_col_print_accuracy)/sizeof(ipp_attr_rec_t)) },
    { "print-accuracy-supported", s_ipp_col_print_accuracy_supported, (sizeof(s_ipp_col_print_accuracy_supported)/sizeof(ipp_attr_rec_t)) },
    { "print-objects", s_ipp_col_print_objects, (sizeof(s_ipp_col_print_objects)/sizeof(ipp_attr_rec_t)) },
    { "printer-icc-profiles", s_ipp_col_printer_icc_profiles, (sizeof(s_ipp_col_printer_icc_profiles)/sizeof(ipp_attr_rec_t)) },
    { "printer-volume-supported", s_ipp_col_printer_volume_supported, (sizeof(s_ipp_col_printer_volume_supported)/sizeof(ipp_attr_rec_t)) },
    { "printer-xri-supported", s_ipp_col_printer_xri_supported, (sizeof(s_ipp_col_printer_xri_supported)/sizeof(ipp_attr_rec_t)) },
    { "proof-print", s_ipp_col_proof_print, (sizeof(s_ipp_col_proof_print)/sizeof(ipp_attr_rec_t)) },
    { "proof-print-default", s_ipp_col_proof_print_default, (sizeof(s_ipp_col_proof_print_default)/sizeof(ipp_attr_rec_t)) },
    { "punching.finishings-col", s_ipp_col_punching__finishings_col, (sizeof(s_ipp_col_punching__finishings_col)/sizeof(ipp_attr_rec_t)) },
    { "save-info.job-save-disposition", s_ipp_col_save_info__job_save_disposition, (sizeof(s_ipp_col_save_info__job_save_disposition)/sizeof(ipp_attr_rec_t)) },
    { "separator-sheets", s_ipp_col_separator_sheets, (sizeof(s_ipp_col_separator_sheets)/sizeof(ipp_attr_rec_t)) },
    { "separator-sheets-actual", s_ipp_col_separator_sheets_actual, (sizeof(s_ipp_col_separator_sheets_actual)/sizeof(ipp_attr_rec_t)) },
    { "separator-sheets-default", s_ipp_col_separator_sheets_default, (sizeof(s_ipp_col_separator_sheets_default)/sizeof(ipp_attr_rec_t)) },
    { "stitching.finishings-col", s_ipp_col_stitching__finishings_col, (sizeof(s_ipp_col_stitching__finishings_col)/sizeof(ipp_attr_rec_t)) },
    { "trimming.finishings-col", s_ipp_col_trimming__finishings_col, (sizeof(s_ipp_col_trimming__finishings_col)/sizeof(ipp_attr_rec_t)) },
};

ipp_attr_rec_t s_ipp_attributes[] =
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 1
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 2
        "*cover-sheet-info",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 3
        "*document-format-details",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 4
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 5
        "*impressions-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 6
        "*insert-sheet",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 7
        "*job-accounting-sheets",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 8
        "*job-error-sheet",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 9
        "*job-impressions-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 10
        "*job-media-sheets-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 11
        "*job-pages-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 12
        "*job-sheets-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 13
        "*media-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 14
        "*media-sheets-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 15
        "*overrides",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 16
        "*pages-col",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 17
        "*pdl-init-file",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 18
        "*proof-print",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 19
        "*separator-sheets",   // indirect reference to
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 20
        "<Any Job Template attribute>",   
        { IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 21
        "access-oauth-token",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 22
        "access-oauth-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 23
        "access-password",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 24
        "access-pin",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 25
        "access-user-name",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 26
        "access-x509-certificate",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 27
        "access-x509-certificate(obsolete)",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 28
        "accuracy-units",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 29
        "accuracy-units-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 30
        "attributes-charset",   
        { IPP_CHARSET, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 31
        "attributes-natural-language",   
        { IPP_LANGUAGE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 32
        "baling",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 33
        "baling-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 34
        "baling-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 35
        "baling-when",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 36
        "baling-when-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 37
        "binding",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 38
        "binding-reference-edge",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 39
        "binding-reference-edge-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 40
        "binding-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 41
        "binding-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 42
        "blank",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 43
        "blank-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 44
        "charge-info-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 45
        "charset-configured",   
        { IPP_CHARSET, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 46
        "charset-supported",   
        { IPP_CHARSET | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 47
        "coating",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 48
        "coating-sides",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 49
        "coating-sides-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 50
        "coating-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 51
        "coating-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 52
        "color-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 53
        "compression",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 54
        "compression-accepted",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 55
        "compression-supplied",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 56
        "compression-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 57
        "confirmation-sheet-print",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 58
        "confirmation-sheet-print-default",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 59
        "copies",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 60
        "copies-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 61
        "copies-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 62
        "copies-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 63
        "cover-back",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 64
        "cover-back-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 65
        "cover-back-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 66
        "cover-back-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 67
        "cover-front",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 68
        "cover-front-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 69
        "cover-front-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 70
        "cover-front-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 71
        "cover-sheet-info",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 72
        "cover-sheet-info-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 73
        "cover-sheet-info-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 74
        "cover-type",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 75
        "covering",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 76
        "covering-name",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 77
        "covering-name-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 78
        "current-page-order",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 79
        "date-time-at-completed",   
        { IPP_DATETIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 80
        "date-time-at-creation",   
        { IPP_DATETIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 81
        "date-time-at-processing",   
        { IPP_DATETIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 82
        "destination-accesses",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 83
        "destination-accesses-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 84
        "destination-attributes",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 85
        "destination-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 86
        "destination-info",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 87
        "destination-is-directory",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 88
        "destination-mandatory-access-attributes",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 89
        "destination-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 90
        "destination-oauth-scope",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 91
        "destination-oauth-token",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 92
        "destination-oauth-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 93
        "destination-statuses",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 94
        "destination-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 95
        "destination-uri-ready",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 96
        "destination-uri-schemes-supported",   
        { IPP_URISCHEME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 97
        "destination-uris",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 98
        "destination-uris-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 99
        "detailed-status-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 100
        "detailed-status-messages",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 101
        "device-service-count",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 102
        "device-uuid",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 45
    },
    {   // index 103
        "document-access",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 104
        "document-access-error",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 105
        "document-access-errors",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 106
        "document-access-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 107
        "document-charset",   
        { IPP_CHARSET, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 108
        "document-charset-default",   
        { IPP_CHARSET, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 109
        "document-charset-supplied",   
        { IPP_CHARSET, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 110
        "document-charset-supported",   
        { IPP_CHARSET | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 111
        "document-copies",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 112
        "document-creation-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 113
        "document-data-get-interval",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 114
        "document-data-wait",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 115
        "document-digital-signature",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 116
        "document-digital-signature-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 117
        "document-digital-signature-supplied",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 118
        "document-digital-signature-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 119
        "document-format",   
        { IPP_MIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 120
        "document-format-accepted",   
        { IPP_MIME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 121
        "document-format-default",   
        { IPP_MIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 122
        "document-format-details",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 123
        "document-format-details-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 124
        "document-format-details-detected",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 125
        "document-format-details-supplied",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 126
        "document-format-details-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 127
        "document-format-detected",   
        { IPP_MIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 128
        "document-format-device-id",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 129
        "document-format-ready",   
        { IPP_MIME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 130
        "document-format-supplied",   
        { IPP_MIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 131
        "document-format-supported",   
        { IPP_MIME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 132
        "document-format-varying-attributes",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 133
        "document-format-version",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 134
        "document-format-version-default",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 135
        "document-format-version-detected",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 136
        "document-format-version-supplied",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 137
        "document-format-version-supported",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 138
        "document-job-id",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 139
        "document-job-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 140
        "document-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 141
        "document-message-supplied",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 142
        "document-metadata",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 143
        "document-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 144
        "document-name-supplied",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 145
        "document-natural-language",   
        { IPP_LANGUAGE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 146
        "document-natural-language-default",   
        { IPP_LANGUAGE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 147
        "document-natural-language-supplied",   
        { IPP_LANGUAGE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 148
        "document-natural-language-supported",   
        { IPP_LANGUAGE | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 149
        "document-number",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 150
        "document-numbers",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 151
        "document-password",   
        { IPP_OCTETSTRING, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 1023
    },
    {   // index 152
        "document-password-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 1023
    },
    {   // index 153
        "document-preprocessed",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 154
        "document-printer-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 155
        "document-privacy-attributes",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 156
        "document-privacy-scope",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 157
        "document-source-application-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 158
        "document-source-application-version",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 159
        "document-source-os-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 40
    },
    {   // index 160
        "document-source-os-version",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 40
    },
    {   // index 161
        "document-state",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 162
        "document-state-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 163
        "document-state-reasons",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 164
        "document-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 165
        "document-uuid",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 45
    },
    {   // index 166
        "errors-count",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 167
        "feed-orientation",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 168
        "feed-orientation-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 169
        "feed-orientation-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 170
        "fetch-document-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 171
        "fetch-status-code",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 172
        "fetch-status-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 173
        "finishing-template",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 174
        "finishing-template(extension)",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 175
        "finishing-template-supported",   
        { IPP_NAME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 176
        "finishings",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 177
        "finishings-actual",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 178
        "finishings-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 179
        "finishings-col-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 180
        "finishings-col-database",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 181
        "finishings-col-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 182
        "finishings-col-ready",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 183
        "finishings-col-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 184
        "finishings-default",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 185
        "finishings-ready",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 186
        "finishings-supported",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 187
        "first-index",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 188
        "folding",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 189
        "folding-direction",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 190
        "folding-direction-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 191
        "folding-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 192
        "folding-offset-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 193
        "folding-reference-edge",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 194
        "folding-reference-edge-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 195
        "font-name-requested",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 196
        "font-name-requested-default",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 197
        "font-name-requested-supported",   
        { IPP_NAME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 198
        "font-size-requested",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 199
        "font-size-requested-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 200
        "font-size-requested-supported",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 201
        "force-front-side",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 202
        "force-front-side-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 203
        "force-front-side-default (under review)",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 204
        "force-front-side-supported (under review)",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 205
        "from-name",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 206
        "from-name-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 1023
    },
    {   // index 207
        "full-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 208
        "full-color-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 209
        "generated-natural-language-supported",   
        { IPP_LANGUAGE | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 210
        "highlight-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 211
        "highlight-color-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 212
        "identify-actions",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 213
        "identify-actions-default",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 214
        "identify-actions-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 215
        "images-completed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 216
        "imposition-template",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 217
        "imposition-template-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 218
        "imposition-template-default",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 219
        "imposition-template-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 220
        "impressions",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 221
        "impressions-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 222
        "impressions-completed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 223
        "impressions-completed-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 224
        "impressions-completed-current-copy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 225
        "input-attributes",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 226
        "input-attributes-actual",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 227
        "input-attributes-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 228
        "input-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 229
        "input-auto-scaling",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 230
        "input-auto-skew-correction",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 231
        "input-brightness",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 232
        "input-color-mode",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 233
        "input-color-mode-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 234
        "input-content-type",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 235
        "input-content-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 236
        "input-contrast",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 237
        "input-film-scan-mode",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 238
        "input-film-scan-mode-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 239
        "input-images-to-transfer",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 240
        "input-media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 241
        "input-media-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 242
        "input-orientation-requested",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 243
        "input-orientation-requested-supported",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 244
        "input-quality",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 245
        "input-quality-supported",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 246
        "input-resolution",   
        { IPP_RESOLUTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 247
        "input-resolution-supported",   
        { IPP_RESOLUTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 248
        "input-scaling-height",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 1000
    },
    {   // index 249
        "input-scaling-width",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 1000
    },
    {   // index 250
        "input-scan-regions",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 251
        "input-scan-regions-supported",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 252
        "input-sharpness",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 253
        "input-sides",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 254
        "input-sides-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 255
        "input-source",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 256
        "input-source-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 257
        "insert-after-page-number",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 258
        "insert-after-page-number-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 259
        "insert-count",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 260
        "insert-count-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 261
        "insert-sheet",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 262
        "insert-sheet-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 263
        "insert-sheet-default",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 264
        "insert-sheet-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 265
        "ipp-attribute-fidelity",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 266
        "ipp-features-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 267
        "ipp-versions-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 268
        "ippget-event-life",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        15, 2147483647
    },
    {   // index 269
        "job-account-id",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 270
        "job-account-id-actual",   
        { IPP_NAME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 271
        "job-account-id-default",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 272
        "job-account-id-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 273
        "job-account-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 274
        "job-account-type-actual",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 275
        "job-account-type-default",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 276
        "job-account-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 277
        "job-accounting-output-bin",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 278
        "job-accounting-sheets",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 279
        "job-accounting-sheets-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 280
        "job-accounting-sheets-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 281
        "job-accounting-sheets-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 282
        "job-accounting-sheets-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 283
        "job-accounting-user-id",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 284
        "job-accounting-user-id-actual",   
        { IPP_NAME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 285
        "job-accounting-user-id-default",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 286
        "job-accounting-user-id-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 287
        "job-attribute-fidelity",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 288
        "job-authorization-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 289
        "job-authorization-uri-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 290
        "job-charge-info",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 291
        "job-collation-type",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 292
        "job-collation-type-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 293
        "job-constraints-supported",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 294
        "job-copies",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 295
        "job-copies-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 296
        "job-copies-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 297
        "job-copies-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 298
        "job-cover-back",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 299
        "job-cover-back-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 300
        "job-cover-back-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 301
        "job-cover-back-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 302
        "job-cover-front",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 303
        "job-cover-front-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 304
        "job-cover-front-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 305
        "job-cover-front-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 306
        "job-creation-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 307
        "job-delay-output-until",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 308
        "job-delay-output-until-default",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 309
        "job-delay-output-until-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 310
        "job-delay-output-until-time",   
        { IPP_DATETIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 311
        "job-delay-output-until-time-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 312
        "job-destination-spooling-supported",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 313
        "job-detailed-status-messages",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 314
        "job-document-access-errors",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 315
        "job-error-action",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 316
        "job-error-action-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 317
        "job-error-action-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 318
        "job-error-sheet",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 319
        "job-error-sheet-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 320
        "job-error-sheet-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 321
        "job-error-sheet-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 322
        "job-error-sheet-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 323
        "job-error-sheet-when",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 324
        "job-finishings",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 325
        "job-finishings-actual",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 326
        "job-finishings-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 327
        "job-finishings-col-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 328
        "job-finishings-col-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 329
        "job-finishings-col-ready",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 330
        "job-finishings-col-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 331
        "job-finishings-default",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 332
        "job-finishings-ready",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 333
        "job-finishings-supported",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 334
        "job-hold-until",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 335
        "job-hold-until-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 336
        "job-hold-until-default",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 337
        "job-hold-until-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 338
        "job-hold-until-time",   
        { IPP_DATETIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 339
        "job-hold-until-time-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 340
        "job-id",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 341
        "job-ids",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 342
        "job-ids-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 343
        "job-impressions",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 344
        "job-impressions-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 345
        "job-impressions-completed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 346
        "job-impressions-completed-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 347
        "job-impressions-estimated",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 348
        "job-impressions-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 349
        "job-k-octets",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 350
        "job-k-octets-processed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 351
        "job-k-octets-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 352
        "job-mandatory-attributes",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 353
        "job-media-sheets",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 354
        "job-media-sheets-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 355
        "job-media-sheets-completed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 356
        "job-media-sheets-completed-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 357
        "job-media-sheets-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 358
        "job-message-from-operator",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 359
        "job-message-to-operator",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 360
        "job-message-to-operator-actual",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 361
        "job-message-to-operator-default",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 362
        "job-message-to-operator-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 363
        "job-more-info",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 364
        "job-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 365
        "job-originating-user-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 366
        "job-originating-user-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 367
        "job-pages",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 368
        "job-pages-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 369
        "job-pages-completed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 370
        "job-pages-completed-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 371
        "job-pages-completed-current-copy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 372
        "job-pages-per-set",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 373
        "job-pages-per-set-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 374
        "job-password",   
        { IPP_OCTETSTRING, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 255
    },
    {   // index 375
        "job-password-encryption",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 376
        "job-password-encryption-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 377
        "job-password-length-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 255
    },
    {   // index 378
        "job-password-repertoire-configured",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 379
        "job-password-repertoire-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 380
        "job-password-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 255
    },
    {   // index 381
        "job-phone-number",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 382
        "job-phone-number-default",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 383
        "job-phone-number-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 384
        "job-presets-supported",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 385
        "job-printer-up-time",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 386
        "job-printer-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 387
        "job-priority",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 100
    },
    {   // index 388
        "job-priority-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 100
    },
    {   // index 389
        "job-priority-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 100
    },
    {   // index 390
        "job-priority-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 100
    },
    {   // index 391
        "job-privacy-attributes",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 392
        "job-privacy-scope",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 393
        "job-recipient-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 394
        "job-recipient-name-default",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 395
        "job-recipient-name-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 396
        "job-resolvers-supported",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 397
        "job-save-disposition",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 398
        "job-save-printer-make-and-model",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 399
        "job-settable-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 400
        "job-sheet-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 401
        "job-sheet-message-actual",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 402
        "job-sheet-message-default",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 403
        "job-sheet-message-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 404
        "job-sheets",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 405
        "job-sheets-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 406
        "job-sheets-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 407
        "job-sheets-col-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 408
        "job-sheets-col-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 409
        "job-sheets-col-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 410
        "job-sheets-default",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 411
        "job-sheets-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 412
        "job-spooling-supported",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 413
        "job-state",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 414
        "job-state-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 415
        "job-state-reasons",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 416
        "job-triggers-supported",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 417
        "job-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 418
        "job-uuid",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 419
        "jpeg-features-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 420
        "jpeg-k-octets-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 421
        "jpeg-x-dimension-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 65535
    },
    {   // index 422
        "jpeg-y-dimension-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 65535
    },
    {   // index 423
        "k-octets",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 424
        "k-octets-processed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 425
        "laminating",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 426
        "laminating-sides",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 427
        "laminating-sides-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 428
        "laminating-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 429
        "laminating-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 430
        "last-document",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 431
        "limit",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 432
        "logo",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 433
        "logo-uri-formats-supported",   
        { IPP_MIME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 434
        "logo-uri-schemes-supported",   
        { IPP_URISCHEME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 435
        "material-amount",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 436
        "material-amount-units",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 437
        "material-amount-units-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 438
        "material-color",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 439
        "material-diameter",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 440
        "material-diameter-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 441
        "material-diameter-tolerance",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 442
        "material-fill-density",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 443
        "material-key",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 444
        "material-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 445
        "material-purpose",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 446
        "material-purpose-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 447
        "material-rate",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 448
        "material-rate-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 449
        "material-rate-units",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 450
        "material-rate-units-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 451
        "material-shell-thickness",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 452
        "material-shell-thickness-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 453
        "material-temperature",   
        { IPP_INTEGER, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 454
        "material-temperature-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 455
        "material-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 456
        "material-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 457
        "materials-col",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 458
        "materials-col-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 459
        "materials-col-database",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 460
        "materials-col-default",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 461
        "materials-col-ready",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 462
        "materials-col-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 463
        "max-materials-col-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 464
        "max-save-info-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 465
        "max-stitching-locations-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 466
        "media",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 467
        "media-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 468
        "media-back-coating",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 469
        "media-back-coating-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 470
        "media-bottom-margin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 471
        "media-bottom-margin-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 472
        "media-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 473
        "media-col-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 474
        "media-col-database",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 475
        "media-col-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 476
        "media-col-ready",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 477
        "media-col-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 478
        "media-color",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 479
        "media-color-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 480
        "media-default",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 481
        "media-front-coating",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 482
        "media-front-coating-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 483
        "media-grain",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 484
        "media-grain-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 485
        "media-hole-count",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 486
        "media-hole-count-supported",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 487
        "media-info",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 255
    },
    {   // index 488
        "media-info-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 489
        "media-input-tray-check",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 490
        "media-input-tray-check-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 491
        "media-key",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 492
        "media-key-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 493
        "media-left-margin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 494
        "media-left-margin-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 495
        "media-order-count",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 496
        "media-order-count-supported",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 497
        "media-pre-printed",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 498
        "media-pre-printed-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 499
        "media-ready",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 500
        "media-recycled",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 501
        "media-recycled-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 502
        "media-right-margin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 503
        "media-right-margin-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 504
        "media-sheets",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 505
        "media-sheets-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 506
        "media-sheets-completed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 507
        "media-sheets-completed-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 508
        "media-sheets-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 509
        "media-size",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 510
        "media-size-name",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 511
        "media-size-supported",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 512
        "media-source",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 513
        "media-source-feed-direction",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 514
        "media-source-feed-orientation",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 515
        "media-source-properties",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 516
        "media-source-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 517
        "media-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 518
        "media-thickness",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 519
        "media-thickness-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 520
        "media-tooth",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 521
        "media-tooth-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 522
        "media-top-margin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 523
        "media-top-margin-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 524
        "media-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 525
        "media-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 526
        "media-weight-metric",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 527
        "media-weight-metric-supported",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 528
        "message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 529
        "message-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 1023
    },
    {   // index 530
        "monochrome",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 531
        "monochrome-two-sided",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 532
        "more-info",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 533
        "multiple-destination-uris-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 534
        "multiple-document-handling",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 535
        "multiple-document-handling-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 536
        "multiple-document-handling-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 537
        "multiple-document-handling-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 538
        "multiple-document-jobs-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 539
        "multiple-object-handling",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 540
        "multiple-object-handling-actual",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 541
        "multiple-object-handling-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 542
        "multiple-object-handling-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 543
        "multiple-operation-time-out",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 544
        "multiple-operation-time-out-action",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 545
        "my-jobs",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 546
        "natural-language-configured",   
        { IPP_LANGUAGE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 547
        "noise-removal",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 548
        "notify-attributes",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 549
        "notify-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 550
        "notify-charset",   
        { IPP_CHARSET, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 551
        "notify-events",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 552
        "notify-events-default",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 553
        "notify-events-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 554
        "notify-get-interval",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 555
        "notify-job-id",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 556
        "notify-lease-duration",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 67108863
    },
    {   // index 557
        "notify-lease-duration-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 67108863
    },
    {   // index 558
        "notify-lease-duration-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 67108863
    },
    {   // index 559
        "notify-lease-expiration-time",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 560
        "notify-max-events-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        2, 2147483647
    },
    {   // index 561
        "notify-natural-language",   
        { IPP_LANGUAGE, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 562
        "notify-printer-up-time",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 563
        "notify-printer-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 564
        "notify-pull-method",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 565
        "notify-pull-method-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 566
        "notify-recipient-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 567
        "notify-schemes-supported",   
        { IPP_URISCHEME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 568
        "notify-sequence-number",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 569
        "notify-sequence-numbers",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 570
        "notify-status-code",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 571
        "notify-subscribed-event",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 572
        "notify-subscriber-user-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 573
        "notify-subscriber-user-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 574
        "notify-subscription-id",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 575
        "notify-subscription-ids",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 576
        "notify-subscription-uuid",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 577
        "notify-text",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 578
        "notify-time-interval",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 579
        "notify-user-data",   
        { IPP_OCTETSTRING, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 63
    },
    {   // index 580
        "notify-wait",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 581
        "number-of-documents",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 582
        "number-of-intervening-jobs",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 583
        "number-of-retries",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 584
        "number-of-retries-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 585
        "number-of-retries-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 586
        "number-up",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 587
        "number-up-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 588
        "number-up-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 589
        "number-up-supported",   
        { IPP_INTEGER, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 590
        "oauth-authorization-server-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 591
        "object-offset",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 592
        "object-size",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 593
        "object-uuid",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 594
        "operations-supported",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 595
        "organization-name",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 596
        "organization-name-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 1023
    },
    {   // index 597
        "orientation-requested",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 598
        "orientation-requested-actual",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 599
        "orientation-requested-default",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 600
        "orientation-requested-supported",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 601
        "original-requesting-user-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 602
        "output-attributes",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 603
        "output-attributes-actual",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 604
        "output-attributes-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 605
        "output-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 606
        "output-bin",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 607
        "output-bin-actual",   
        { IPP_NAME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 608
        "output-bin-default",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 609
        "output-bin-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 610
        "output-compression-quality-factor",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 100
    },
    {   // index 611
        "output-device",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 612
        "output-device-actual",   
        { IPP_NAME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 613
        "output-device-assigned",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 614
        "output-device-document-state",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 615
        "output-device-document-state-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 616
        "output-device-document-state-reasons",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 617
        "output-device-job-state",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 618
        "output-device-job-state-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 619
        "output-device-job-state-reasons",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 620
        "output-device-job-states",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 621
        "output-device-supported",   
        { IPP_NAME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 622
        "output-device-uuid",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 623
        "output-device-uuid-assigned",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 624
        "output-device-uuid-supported",   
        { IPP_URI | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 625
        "overrides",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 626
        "overrides-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 627
        "overrides-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 628
        "page-delivery",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 629
        "page-delivery-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 630
        "page-delivery-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 631
        "page-delivery-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 632
        "page-order-received",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 633
        "page-order-received-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 634
        "page-order-received-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 635
        "page-order-received-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 636
        "page-ranges",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 637
        "page-ranges-actual",   
        { IPP_RANGEOFINT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 638
        "page-ranges-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 639
        "pages",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 640
        "pages-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 641
        "pages-completed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 642
        "pages-completed-col",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 643
        "pages-completed-current-copy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 644
        "pages-per-minute",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 645
        "pages-per-minute-color",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 646
        "pages-per-subset",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 647
        "pages-per-subset-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 648
        "parent-printers-supported",   
        { IPP_URI | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 649
        "pclm-raster-back-side",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 650
        "pclm-source-resolution",   
        { IPP_RESOLUTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 651
        "pclm-source-resolution-supported",   
        { IPP_RESOLUTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 652
        "pclm-strip-height-preferred",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 653
        "pclm-strip-height-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 654
        "pdf-features-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 655
        "pdf-k-octets-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 656
        "pdf-versions-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 657
        "pdl-init-file",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 658
        "pdl-init-file-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 659
        "pdl-init-file-entry",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 660
        "pdl-init-file-entry-supported",   
        { IPP_NAME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 661
        "pdl-init-file-location",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 662
        "pdl-init-file-location-supported",   
        { IPP_URI | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 663
        "pdl-init-file-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 664
        "pdl-init-file-name-subdirectory-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 665
        "pdl-init-file-name-supported",   
        { IPP_NAME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 666
        "pdl-init-file-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 667
        "pdl-override-guaranteed-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 668
        "pdl-override-supported",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 669
        "platform-shape",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 670
        "platform-temperature",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 671
        "platform-temperature-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 672
        "platform-temperature-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 673
        "platform-temperature-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 674
        "post-dial-string",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 675
        "pre-dial-string",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 676
        "preferred-attributes",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 677
        "preferred-attributes-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 678
        "presentation-direction-number-up",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 679
        "presentation-direction-number-up-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 680
        "presentation-direction-number-up-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 681
        "presentation-direction-number-up-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 682
        "preset-name",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 683
        "print-accuracy",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 684
        "print-accuracy-actual",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 685
        "print-accuracy-supported",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 686
        "print-base",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 687
        "print-base-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 688
        "print-base-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 689
        "print-base-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 690
        "print-color-mode",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 691
        "print-color-mode-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 692
        "print-color-mode-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 693
        "print-color-mode-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 694
        "print-content-optimize",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 695
        "print-content-optimize-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 696
        "print-content-optimize-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 697
        "print-content-optimize-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 698
        "print-objects",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 699
        "print-objects-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 700
        "print-objects-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 701
        "print-quality",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 702
        "print-quality-actual",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 703
        "print-quality-default",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 704
        "print-quality-supported",   
        { IPP_ENUM | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 705
        "print-rendering-intent",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 706
        "print-rendering-intent-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 707
        "print-rendering-intent-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 708
        "print-rendering-intent-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 709
        "print-scaling",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 710
        "print-scaling-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 711
        "print-scaling-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 712
        "print-supports",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 713
        "print-supports-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 714
        "print-supports-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 715
        "print-supports-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 716
        "printer-alert",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 717
        "printer-alert-description",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 718
        "printer-camera-image-uri",   
        { IPP_URI | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 719
        "printer-charge-info",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 720
        "printer-charge-info-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 721
        "printer-config-change-date-time",   
        { IPP_DATETIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 722
        "printer-config-change-time",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 723
        "printer-current-time",   
        { IPP_DATETIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 724
        "printer-detailed-status-messages",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 725
        "printer-device-id",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 1023
    },
    {   // index 726
        "printer-dns-sd-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 63
    },
    {   // index 727
        "printer-driver-installer",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 728
        "printer-fax-log-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 729
        "printer-fax-modem-info",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 730
        "printer-fax-modem-name",   
        { IPP_NAME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 731
        "printer-fax-modem-number",   
        { IPP_URI | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 732
        "printer-finisher",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 733
        "printer-finisher-description",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 734
        "printer-finisher-supplies",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 735
        "printer-finisher-supplies-description",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 736
        "printer-geo-location",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 737
        "printer-get-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 738
        "printer-icc-profiles",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 739
        "printer-icons",   
        { IPP_URI | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 740
        "printer-info",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 741
        "printer-input-tray",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 742
        "printer-is-accepting-jobs",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 743
        "printer-kind",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 744
        "printer-location",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 745
        "printer-make-and-model",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 746
        "printer-mandatory-job-attributes",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 747
        "printer-message-date-time",   
        { IPP_DATETIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 748
        "printer-message-from-operator",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 749
        "printer-message-time",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 750
        "printer-more-info",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 751
        "printer-more-info-manufacturer",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 752
        "printer-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 127
    },
    {   // index 753
        "printer-organization",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 754
        "printer-organizational-unit",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 755
        "printer-output-tray",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 756
        "printer-privacy-policy-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 757
        "printer-resolution",   
        { IPP_RESOLUTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 758
        "printer-resolution-actual",   
        { IPP_RESOLUTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 759
        "printer-resolution-default",   
        { IPP_RESOLUTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 760
        "printer-resolution-supported",   
        { IPP_RESOLUTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 761
        "printer-settable-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 762
        "printer-state",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 763
        "printer-state-change-date-time",   
        { IPP_DATETIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 764
        "printer-state-change-time",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 765
        "printer-state-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 766
        "printer-state-reasons",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 767
        "printer-static-resource-directory-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 768
        "printer-static-resource-k-octets-free",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 769
        "printer-static-resource-k-octets-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 770
        "printer-strings-languages-supported",   
        { IPP_LANGUAGE | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 771
        "printer-strings-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 772
        "printer-supply",   
        { IPP_OCTETSTRING | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 773
        "printer-supply-description",   
        { IPP_TEXT | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 774
        "printer-supply-info-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 775
        "printer-up-time",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 776
        "printer-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 777
        "printer-uri-supported",   
        { IPP_URI | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 778
        "printer-uuid",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 45
    },
    {   // index 779
        "printer-volume-supported",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 780
        "printer-xri-supported",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 781
        "profile-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 782
        "profile-uri-actual",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 783
        "profile-url",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 784
        "proof-print",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 785
        "proof-print-copies",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 786
        "proof-print-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 787
        "proof-print-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 788
        "punching",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 789
        "punching-hole-diameter-configured",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 790
        "punching-locations",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 791
        "punching-locations-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 792
        "punching-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 793
        "punching-offset-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 794
        "punching-reference-edge",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 795
        "punching-reference-edge-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 796
        "pwg-raster-document-resolution-supported",   
        { IPP_RESOLUTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 797
        "pwg-raster-document-sheet-back",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 798
        "pwg-raster-document-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 799
        "queued-job-count",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 800
        "reference-uri-schemes-supported",   
        { IPP_URISCHEME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 801
        "repertoire-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 802
        "requested-attributes",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 803
        "requesting-user-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 804
        "requesting-user-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 805
        "requesting-user-uri-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 806
        "resolver-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 807
        "retry-interval",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 808
        "retry-interval-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 809
        "retry-interval-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 810
        "retry-time-out",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 811
        "retry-time-out-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 812
        "retry-time-out-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 813
        "save-disposition",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 814
        "save-disposition-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 815
        "save-document-format",   
        { IPP_MIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 816
        "save-document-format-default",   
        { IPP_MIME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 817
        "save-document-format-supported",   
        { IPP_MIME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 818
        "save-info",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 819
        "save-location",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 820
        "save-location-default",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 821
        "save-location-supported",   
        { IPP_URI | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 822
        "save-name",   
        { IPP_NAME, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 823
        "save-name-subdirectory-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 824
        "save-name-supported",   
        { IPP_BOOLEAN, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 825
        "separator-sheets",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 826
        "separator-sheets-actual",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 827
        "separator-sheets-default",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 828
        "separator-sheets-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 829
        "separator-sheets-type",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 830
        "sheet-collate",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 831
        "sheet-collate-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 832
        "sheet-collate-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 833
        "sheet-collate-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 834
        "sheet-completed-copy-number",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 835
        "sheet-completed-document-number",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 836
        "sides",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 837
        "sides-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 838
        "sides-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 839
        "sides-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 840
        "status-message",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 255
    },
    {   // index 841
        "stitching",   
        { IPP_COLLECTION, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 842
        "stitching-angle",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 359
    },
    {   // index 843
        "stitching-angle-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 359
    },
    {   // index 844
        "stitching-locations",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 845
        "stitching-locations-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 846
        "stitching-method",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 847
        "stitching-method-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 848
        "stitching-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 849
        "stitching-offset-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 850
        "stitching-reference-edge",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 851
        "stitching-reference-edge-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 852
        "subject",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 853
        "subject-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 1023
    },
    {   // index 854
        "subordinate-printers-supported",   
        { IPP_URI | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 855
        "subscription-privacy-attributes",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 856
        "subscription-privacy-scope",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 857
        "t33-subaddress",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 858
        "time-at-completed",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 859
        "time-at-creation",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 860
        "time-at-processing",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 861
        "to-name",   
        { IPP_TEXT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 862
        "to-name-supported",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 1023
    },
    {   // index 863
        "transmission-status",   
        { IPP_ENUM, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 864
        "trimming",   
        { IPP_COLLECTION | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 865
        "trimming-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 866
        "trimming-offset-supported",   
        { IPP_INTEGER | IPP_ARRAY, IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 867
        "trimming-reference-edge",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 868
        "trimming-reference-edge-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 869
        "trimming-type",   
        { IPP_KEYWORD, IPP_NAME, IPP_NOTYPE, IPP_NOTYPE },
        0, IPP_MAX_TEXT
    },
    {   // index 870
        "trimming-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 871
        "trimming-when",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 872
        "trimming-when-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 873
        "uri-authentication-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 874
        "uri-security-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 875
        "user-defined-values-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 876
        "warnings-count",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 877
        "which-jobs",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 878
        "which-jobs-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 879
        "x-accuracy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 880
        "x-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 881
        "x-image-position",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 882
        "x-image-position-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 883
        "x-image-position-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 884
        "x-image-position-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 885
        "x-image-shift",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 886
        "x-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 887
        "x-image-shift-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 888
        "x-image-shift-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 889
        "x-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 890
        "x-origin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 891
        "x-side1-image-shift",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 892
        "x-side1-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 893
        "x-side1-image-shift-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 894
        "x-side1-image-shift-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 895
        "x-side2-image-shift",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 896
        "x-side2-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 897
        "x-side2-image-shift-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 898
        "x-side2-image-shift-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 899
        "xri-authentication",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 900
        "xri-authentication-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 901
        "xri-security",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 902
        "xri-security-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 903
        "xri-uri",   
        { IPP_URI, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 904
        "xri-uri-scheme-supported",   
        { IPP_URISCHEME | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 905
        "y-accuracy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 906
        "y-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 907
        "y-image-position",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 908
        "y-image-position-actual",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 909
        "y-image-position-default",   
        { IPP_KEYWORD, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 910
        "y-image-position-supported",   
        { IPP_KEYWORD | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 911
        "y-image-shift",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 912
        "y-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 913
        "y-image-shift-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 914
        "y-image-shift-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 915
        "y-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 916
        "y-origin",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 0
    },
    {   // index 917
        "y-side1-image-shift",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 918
        "y-side1-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 919
        "y-side1-image-shift-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 920
        "y-side1-image-shift-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 921
        "y-side2-image-shift",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 922
        "y-side2-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 923
        "y-side2-image-shift-default",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 924
        "y-side2-image-shift-supported",   
        { IPP_RANGEOFINT, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        -2147483648, 2147483647
    },
    {   // index 925
        "z-accuracy",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
    {   // index 926
        "z-dimension",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        1, 2147483647
    },
    {   // index 927
        "z-offset",   
        { IPP_INTEGER, IPP_NOTYPE, IPP_NOTYPE, IPP_NOTYPE },
        0, 2147483647
    },
};

#define NUM_IPP_ATTRIBUTES (sizeof(s_ipp_attributes) / sizeof(ipp_attr_rec_t))

ipp_attr_t s_ipp_attr_group_decdesc[] =
{
    {
        142, // document-metadata
        0, 0,
        NULL, NULL
    },
    {
        143, // document-name
        0, 0,
        NULL, NULL
    },
    {
        220, // impressions
        0, 0,
        NULL, NULL
    },
    {
        221, // impressions-col
        0, 0,
        NULL, NULL
    },
    {
        423, // k-octets
        0, 0,
        NULL, NULL
    },
    {
        504, // media-sheets
        0, 0,
        NULL, NULL
    },
    {
        505, // media-sheets-col
        0, 0,
        NULL, NULL
    },
    {
        639, // pages
        0, 0,
        NULL, NULL
    },
    {
        640, // pages-col
        0, 0,
        NULL, NULL
    },
};

ipp_attr_t s_ipp_attr_group_docstat[] =
{
    {
        30, // attributes-charset
        0, 0,
        NULL, NULL
    },
    {
        31, // attributes-natural-language
        0, 0,
        NULL, NULL
    },
    {
        53, // compression
        0, 0,
        NULL, NULL
    },
    {
        60, // copies-actual
        0, 0,
        NULL, NULL
    },
    {
        64, // cover-back-actual
        0, 0,
        NULL, NULL
    },
    {
        68, // cover-front-actual
        0, 0,
        NULL, NULL
    },
    {
        78, // current-page-order
        0, 0,
        NULL, NULL
    },
    {
        79, // date-time-at-completed
        0, 0,
        NULL, NULL
    },
    {
        80, // date-time-at-creation
        0, 0,
        NULL, NULL
    },
    {
        81, // date-time-at-processing
        0, 0,
        NULL, NULL
    },
    {
        100, // detailed-status-messages
        0, 0,
        NULL, NULL
    },
    {
        105, // document-access-errors
        0, 0,
        NULL, NULL
    },
    {
        107, // document-charset
        0, 0,
        NULL, NULL
    },
    {
        115, // document-digital-signature
        0, 0,
        NULL, NULL
    },
    {
        119, // document-format
        0, 0,
        NULL, NULL
    },
    {
        122, // document-format-details
        0, 0,
        NULL, NULL
    },
    {
        124, // document-format-details-detected
        0, 0,
        NULL, NULL
    },
    {
        127, // document-format-detected
        0, 0,
        NULL, NULL
    },
    {
        129, // document-format-ready
        0, 0,
        NULL, NULL
    },
    {
        130, // document-format-supplied
        0, 0,
        NULL, NULL
    },
    {
        133, // document-format-version
        0, 0,
        NULL, NULL
    },
    {
        135, // document-format-version-detected
        0, 0,
        NULL, NULL
    },
    {
        138, // document-job-id
        0, 0,
        NULL, NULL
    },
    {
        139, // document-job-uri
        0, 0,
        NULL, NULL
    },
    {
        140, // document-message
        0, 0,
        NULL, NULL
    },
    {
        141, // document-message-supplied
        0, 0,
        NULL, NULL
    },
    {
        144, // document-name-supplied
        0, 0,
        NULL, NULL
    },
    {
        145, // document-natural-language
        0, 0,
        NULL, NULL
    },
    {
        149, // document-number
        0, 0,
        NULL, NULL
    },
    {
        154, // document-printer-uri
        0, 0,
        NULL, NULL
    },
    {
        161, // document-state
        0, 0,
        NULL, NULL
    },
    {
        162, // document-state-message
        0, 0,
        NULL, NULL
    },
    {
        163, // document-state-reasons
        0, 0,
        NULL, NULL
    },
    {
        164, // document-uri
        0, 0,
        NULL, NULL
    },
    {
        165, // document-uuid
        0, 0,
        NULL, NULL
    },
    {
        166, // errors-count
        0, 0,
        NULL, NULL
    },
    {
        177, // finishings-actual
        0, 0,
        NULL, NULL
    },
    {
        179, // finishings-col-actual
        0, 0,
        NULL, NULL
    },
    {
        202, // force-front-side-actual
        0, 0,
        NULL, NULL
    },
    {
        217, // imposition-template-actual
        0, 0,
        NULL, NULL
    },
    {
        222, // impressions-completed
        0, 0,
        NULL, NULL
    },
    {
        223, // impressions-completed-col
        0, 0,
        NULL, NULL
    },
    {
        224, // impressions-completed-current-copy
        0, 0,
        NULL, NULL
    },
    {
        226, // input-attributes-actual
        0, 0,
        NULL, NULL
    },
    {
        262, // insert-sheet-actual
        0, 0,
        NULL, NULL
    },
    {
        424, // k-octets-processed
        0, 0,
        NULL, NULL
    },
    {
        430, // last-document
        0, 0,
        NULL, NULL
    },
    {
        458, // materials-col-actual
        0, 0,
        NULL, NULL
    },
    {
        467, // media-actual
        0, 0,
        NULL, NULL
    },
    {
        473, // media-col-actual
        0, 0,
        NULL, NULL
    },
    {
        490, // media-input-tray-check-actual
        0, 0,
        NULL, NULL
    },
    {
        506, // media-sheets-completed
        0, 0,
        NULL, NULL
    },
    {
        507, // media-sheets-completed-col
        0, 0,
        NULL, NULL
    },
    {
        532, // more-info
        0, 0,
        NULL, NULL
    },
    {
        540, // multiple-object-handling-actual
        0, 0,
        NULL, NULL
    },
    {
        587, // number-up-actual
        0, 0,
        NULL, NULL
    },
    {
        598, // orientation-requested-actual
        0, 0,
        NULL, NULL
    },
    {
        603, // output-attributes-actual
        0, 0,
        NULL, NULL
    },
    {
        607, // output-bin-actual
        0, 0,
        NULL, NULL
    },
    {
        613, // output-device-assigned
        0, 0,
        NULL, NULL
    },
    {
        614, // output-device-document-state
        0, 0,
        NULL, NULL
    },
    {
        615, // output-device-document-state-message
        0, 0,
        NULL, NULL
    },
    {
        616, // output-device-document-state-reasons
        0, 0,
        NULL, NULL
    },
    {
        629, // page-delivery-actual
        0, 0,
        NULL, NULL
    },
    {
        633, // page-order-received-actual
        0, 0,
        NULL, NULL
    },
    {
        637, // page-ranges-actual
        0, 0,
        NULL, NULL
    },
    {
        641, // pages-completed
        0, 0,
        NULL, NULL
    },
    {
        642, // pages-completed-col
        0, 0,
        NULL, NULL
    },
    {
        643, // pages-completed-current-copy
        0, 0,
        NULL, NULL
    },
    {
        671, // platform-temperature-actual
        0, 0,
        NULL, NULL
    },
    {
        679, // presentation-direction-number-up-actual
        0, 0,
        NULL, NULL
    },
    {
        684, // print-accuracy-actual
        0, 0,
        NULL, NULL
    },
    {
        687, // print-base-actual
        0, 0,
        NULL, NULL
    },
    {
        691, // print-color-mode-actual
        0, 0,
        NULL, NULL
    },
    {
        695, // print-content-optimize-actual
        0, 0,
        NULL, NULL
    },
    {
        699, // print-objects-actual
        0, 0,
        NULL, NULL
    },
    {
        702, // print-quality-actual
        0, 0,
        NULL, NULL
    },
    {
        706, // print-rendering-intent-actual
        0, 0,
        NULL, NULL
    },
    {
        713, // print-supports-actual
        0, 0,
        NULL, NULL
    },
    {
        758, // printer-resolution-actual
        0, 0,
        NULL, NULL
    },
    {
        775, // printer-up-time
        0, 0,
        NULL, NULL
    },
    {
        826, // separator-sheets-actual
        0, 0,
        NULL, NULL
    },
    {
        834, // sheet-completed-copy-number
        0, 0,
        NULL, NULL
    },
    {
        837, // sides-actual
        0, 0,
        NULL, NULL
    },
    {
        858, // time-at-completed
        0, 0,
        NULL, NULL
    },
    {
        859, // time-at-creation
        0, 0,
        NULL, NULL
    },
    {
        860, // time-at-processing
        0, 0,
        NULL, NULL
    },
    {
        876, // warnings-count
        0, 0,
        NULL, NULL
    },
    {
        882, // x-image-position-actual
        0, 0,
        NULL, NULL
    },
    {
        886, // x-image-shift-actual
        0, 0,
        NULL, NULL
    },
    {
        892, // x-side1-image-shift-actual
        0, 0,
        NULL, NULL
    },
    {
        896, // x-side2-image-shift-actual
        0, 0,
        NULL, NULL
    },
    {
        908, // y-image-position-actual
        0, 0,
        NULL, NULL
    },
    {
        912, // y-image-shift-actual
        0, 0,
        NULL, NULL
    },
    {
        918, // y-side1-image-shift-actual
        0, 0,
        NULL, NULL
    },
    {
        922, // y-side2-image-shift-actual
        0, 0,
        NULL, NULL
    },
};

ipp_attr_t s_ipp_attr_group_doctemp[] =
{
};

ipp_attr_t s_ipp_attr_group_evtnot[] =
{
    {
        340, // job-id
        0, 0,
        NULL, NULL
    },
    {
        345, // job-impressions-completed
        0, 0,
        NULL, NULL
    },
    {
        413, // job-state
        0, 0,
        NULL, NULL
    },
    {
        415, // job-state-reasons
        0, 0,
        NULL, NULL
    },
    {
        418, // job-uuid
        0, 0,
        NULL, NULL
    },
    {
        550, // notify-charset
        0, 0,
        NULL, NULL
    },
    {
        561, // notify-natural-language
        0, 0,
        NULL, NULL
    },
    {
        563, // notify-printer-uri
        0, 0,
        NULL, NULL
    },
    {
        568, // notify-sequence-number
        0, 0,
        NULL, NULL
    },
    {
        571, // notify-subscribed-event
        0, 0,
        NULL, NULL
    },
    {
        574, // notify-subscription-id
        0, 0,
        NULL, NULL
    },
    {
        576, // notify-subscription-uuid
        0, 0,
        NULL, NULL
    },
    {
        577, // notify-text
        0, 0,
        NULL, NULL
    },
    {
        579, // notify-user-data
        0, 0,
        NULL, NULL
    },
    {
        723, // printer-current-time
        0, 0,
        NULL, NULL
    },
    {
        742, // printer-is-accepting-jobs
        0, 0,
        NULL, NULL
    },
    {
        762, // printer-state
        0, 0,
        NULL, NULL
    },
    {
        766, // printer-state-reasons
        0, 0,
        NULL, NULL
    },
    {
        775, // printer-up-time
        0, 0,
        NULL, NULL
    },
};

ipp_attr_t s_ipp_attr_group_jodesc[] =
{
    {
        78, // current-page-order
        0, 0,
        NULL, NULL
    },
    {
        142, // document-metadata
        0, 0,
        NULL, NULL
    },
    {
        290, // job-charge-info
        0, 0,
        NULL, NULL
    },
    {
        291, // job-collation-type
        0, 0,
        NULL, NULL
    },
    {
        352, // job-mandatory-attributes
        0, 0,
        NULL, NULL
    },
    {
        358, // job-message-from-operator
        0, 0,
        NULL, NULL
    },
    {
        360, // job-message-to-operator-actual
        0, 0,
        NULL, NULL
    },
    {
        364, // job-name
        0, 0,
        NULL, NULL
    },
    {
        398, // job-save-printer-make-and-model
        0, 0,
        NULL, NULL
    },
};

ipp_attr_t s_ipp_attr_group_jobstat[] =
{
    {
        30, // attributes-charset
        0, 0,
        NULL, NULL
    },
    {
        31, // attributes-natural-language
        0, 0,
        NULL, NULL
    },
    {
        55, // compression-supplied
        0, 0,
        NULL, NULL
    },
    {
        60, // copies-actual
        0, 0,
        NULL, NULL
    },
    {
        64, // cover-back-actual
        0, 0,
        NULL, NULL
    },
    {
        68, // cover-front-actual
        0, 0,
        NULL, NULL
    },
    {
        79, // date-time-at-completed
        0, 0,
        NULL, NULL
    },
    {
        80, // date-time-at-creation
        0, 0,
        NULL, NULL
    },
    {
        81, // date-time-at-processing
        0, 0,
        NULL, NULL
    },
    {
        109, // document-charset-supplied
        0, 0,
        NULL, NULL
    },
    {
        117, // document-digital-signature-supplied
        0, 0,
        NULL, NULL
    },
    {
        125, // document-format-details-supplied
        0, 0,
        NULL, NULL
    },
    {
        129, // document-format-ready
        0, 0,
        NULL, NULL
    },
    {
        130, // document-format-supplied
        0, 0,
        NULL, NULL
    },
    {
        136, // document-format-version-supplied
        0, 0,
        NULL, NULL
    },
    {
        141, // document-message-supplied
        0, 0,
        NULL, NULL
    },
    {
        144, // document-name-supplied
        0, 0,
        NULL, NULL
    },
    {
        147, // document-natural-language-supplied
        0, 0,
        NULL, NULL
    },
    {
        166, // errors-count
        0, 0,
        NULL, NULL
    },
    {
        177, // finishings-actual
        0, 0,
        NULL, NULL
    },
    {
        179, // finishings-col-actual
        0, 0,
        NULL, NULL
    },
    {
        202, // force-front-side-actual
        0, 0,
        NULL, NULL
    },
    {
        217, // imposition-template-actual
        0, 0,
        NULL, NULL
    },
    {
        224, // impressions-completed-current-copy
        0, 0,
        NULL, NULL
    },
    {
        226, // input-attributes-actual
        0, 0,
        NULL, NULL
    },
    {
        262, // insert-sheet-actual
        0, 0,
        NULL, NULL
    },
    {
        270, // job-account-id-actual
        0, 0,
        NULL, NULL
    },
    {
        274, // job-account-type-actual
        0, 0,
        NULL, NULL
    },
    {
        279, // job-accounting-sheets-actual
        0, 0,
        NULL, NULL
    },
    {
        284, // job-accounting-user-id-actual
        0, 0,
        NULL, NULL
    },
    {
        287, // job-attribute-fidelity
        0, 0,
        NULL, NULL
    },
    {
        292, // job-collation-type-actual
        0, 0,
        NULL, NULL
    },
    {
        295, // job-copies-actual
        0, 0,
        NULL, NULL
    },
    {
        299, // job-cover-back-actual
        0, 0,
        NULL, NULL
    },
    {
        303, // job-cover-front-actual
        0, 0,
        NULL, NULL
    },
    {
        313, // job-detailed-status-messages
        0, 0,
        NULL, NULL
    },
    {
        314, // job-document-access-errors
        0, 0,
        NULL, NULL
    },
    {
        319, // job-error-sheet-actual
        0, 0,
        NULL, NULL
    },
    {
        325, // job-finishings-actual
        0, 0,
        NULL, NULL
    },
    {
        327, // job-finishings-col-actual
        0, 0,
        NULL, NULL
    },
    {
        335, // job-hold-until-actual
        0, 0,
        NULL, NULL
    },
    {
        340, // job-id
        0, 0,
        NULL, NULL
    },
    {
        343, // job-impressions
        0, 0,
        NULL, NULL
    },
    {
        344, // job-impressions-col
        0, 0,
        NULL, NULL
    },
    {
        345, // job-impressions-completed
        0, 0,
        NULL, NULL
    },
    {
        346, // job-impressions-completed-col
        0, 0,
        NULL, NULL
    },
    {
        349, // job-k-octets
        0, 0,
        NULL, NULL
    },
    {
        350, // job-k-octets-processed
        0, 0,
        NULL, NULL
    },
    {
        353, // job-media-sheets
        0, 0,
        NULL, NULL
    },
    {
        354, // job-media-sheets-col
        0, 0,
        NULL, NULL
    },
    {
        355, // job-media-sheets-completed
        0, 0,
        NULL, NULL
    },
    {
        356, // job-media-sheets-completed-col
        0, 0,
        NULL, NULL
    },
    {
        363, // job-more-info
        0, 0,
        NULL, NULL
    },
    {
        365, // job-originating-user-name
        0, 0,
        NULL, NULL
    },
    {
        366, // job-originating-user-uri
        0, 0,
        NULL, NULL
    },
    {
        367, // job-pages
        0, 0,
        NULL, NULL
    },
    {
        368, // job-pages-col
        0, 0,
        NULL, NULL
    },
    {
        369, // job-pages-completed
        0, 0,
        NULL, NULL
    },
    {
        370, // job-pages-completed-col
        0, 0,
        NULL, NULL
    },
    {
        371, // job-pages-completed-current-copy
        0, 0,
        NULL, NULL
    },
    {
        385, // job-printer-up-time
        0, 0,
        NULL, NULL
    },
    {
        386, // job-printer-uri
        0, 0,
        NULL, NULL
    },
    {
        388, // job-priority-actual
        0, 0,
        NULL, NULL
    },
    {
        401, // job-sheet-message-actual
        0, 0,
        NULL, NULL
    },
    {
        405, // job-sheets-actual
        0, 0,
        NULL, NULL
    },
    {
        407, // job-sheets-col-actual
        0, 0,
        NULL, NULL
    },
    {
        413, // job-state
        0, 0,
        NULL, NULL
    },
    {
        414, // job-state-message
        0, 0,
        NULL, NULL
    },
    {
        415, // job-state-reasons
        0, 0,
        NULL, NULL
    },
    {
        417, // job-uri
        0, 0,
        NULL, NULL
    },
    {
        418, // job-uuid
        0, 0,
        NULL, NULL
    },
    {
        458, // materials-col-actual
        0, 0,
        NULL, NULL
    },
    {
        467, // media-actual
        0, 0,
        NULL, NULL
    },
    {
        473, // media-col-actual
        0, 0,
        NULL, NULL
    },
    {
        490, // media-input-tray-check-actual
        0, 0,
        NULL, NULL
    },
    {
        535, // multiple-document-handling-actual
        0, 0,
        NULL, NULL
    },
    {
        540, // multiple-object-handling-actual
        0, 0,
        NULL, NULL
    },
    {
        581, // number-of-documents
        0, 0,
        NULL, NULL
    },
    {
        582, // number-of-intervening-jobs
        0, 0,
        NULL, NULL
    },
    {
        587, // number-up-actual
        0, 0,
        NULL, NULL
    },
    {
        598, // orientation-requested-actual
        0, 0,
        NULL, NULL
    },
    {
        601, // original-requesting-user-name
        0, 0,
        NULL, NULL
    },
    {
        603, // output-attributes-actual
        0, 0,
        NULL, NULL
    },
    {
        607, // output-bin-actual
        0, 0,
        NULL, NULL
    },
    {
        612, // output-device-actual
        0, 0,
        NULL, NULL
    },
    {
        613, // output-device-assigned
        0, 0,
        NULL, NULL
    },
    {
        617, // output-device-job-state
        0, 0,
        NULL, NULL
    },
    {
        618, // output-device-job-state-message
        0, 0,
        NULL, NULL
    },
    {
        619, // output-device-job-state-reasons
        0, 0,
        NULL, NULL
    },
    {
        623, // output-device-uuid-assigned
        0, 0,
        NULL, NULL
    },
    {
        626, // overrides-actual
        0, 0,
        NULL, NULL
    },
    {
        629, // page-delivery-actual
        0, 0,
        NULL, NULL
    },
    {
        633, // page-order-received-actual
        0, 0,
        NULL, NULL
    },
    {
        637, // page-ranges-actual
        0, 0,
        NULL, NULL
    },
    {
        671, // platform-temperature-actual
        0, 0,
        NULL, NULL
    },
    {
        679, // presentation-direction-number-up-actual
        0, 0,
        NULL, NULL
    },
    {
        684, // print-accuracy-actual
        0, 0,
        NULL, NULL
    },
    {
        687, // print-base-actual
        0, 0,
        NULL, NULL
    },
    {
        691, // print-color-mode-actual
        0, 0,
        NULL, NULL
    },
    {
        695, // print-content-optimize-actual
        0, 0,
        NULL, NULL
    },
    {
        699, // print-objects-actual
        0, 0,
        NULL, NULL
    },
    {
        702, // print-quality-actual
        0, 0,
        NULL, NULL
    },
    {
        706, // print-rendering-intent-actual
        0, 0,
        NULL, NULL
    },
    {
        713, // print-supports-actual
        0, 0,
        NULL, NULL
    },
    {
        758, // printer-resolution-actual
        0, 0,
        NULL, NULL
    },
    {
        826, // separator-sheets-actual
        0, 0,
        NULL, NULL
    },
    {
        831, // sheet-collate-actual
        0, 0,
        NULL, NULL
    },
    {
        834, // sheet-completed-copy-number
        0, 0,
        NULL, NULL
    },
    {
        835, // sheet-completed-document-number
        0, 0,
        NULL, NULL
    },
    {
        837, // sides-actual
        0, 0,
        NULL, NULL
    },
    {
        858, // time-at-completed
        0, 0,
        NULL, NULL
    },
    {
        859, // time-at-creation
        0, 0,
        NULL, NULL
    },
    {
        860, // time-at-processing
        0, 0,
        NULL, NULL
    },
    {
        876, // warnings-count
        0, 0,
        NULL, NULL
    },
    {
        882, // x-image-position-actual
        0, 0,
        NULL, NULL
    },
    {
        886, // x-image-shift-actual
        0, 0,
        NULL, NULL
    },
    {
        892, // x-side1-image-shift-actual
        0, 0,
        NULL, NULL
    },
    {
        896, // x-side2-image-shift-actual
        0, 0,
        NULL, NULL
    },
    {
        908, // y-image-position-actual
        0, 0,
        NULL, NULL
    },
    {
        912, // y-image-shift-actual
        0, 0,
        NULL, NULL
    },
    {
        918, // y-side1-image-shift-actual
        0, 0,
        NULL, NULL
    },
    {
        922, // y-side2-image-shift-actual
        0, 0,
        NULL, NULL
    },
};

ipp_attr_t s_ipp_attr_group_jobtemp[] =
{
    {
        57, // confirmation-sheet-print
        0, 0,
        NULL, NULL
    },
    {
        59, // copies
        0, 0,
        NULL, NULL
    },
    {
        63, // cover-back
        0, 0,
        NULL, NULL
    },
    {
        67, // cover-front
        0, 0,
        NULL, NULL
    },
    {
        71, // cover-sheet-info
        0, 0,
        NULL, NULL
    },
    {
        93, // destination-statuses
        0, 0,
        NULL, NULL
    },
    {
        97, // destination-uris
        0, 0,
        NULL, NULL
    },
    {
        167, // feed-orientation
        0, 0,
        NULL, NULL
    },
    {
        176, // finishings
        0, 0,
        NULL, NULL
    },
    {
        178, // finishings-col
        0, 0,
        NULL, NULL
    },
    {
        195, // font-name-requested
        0, 0,
        NULL, NULL
    },
    {
        198, // font-size-requested
        0, 0,
        NULL, NULL
    },
    {
        201, // force-front-side
        0, 0,
        NULL, NULL
    },
    {
        216, // imposition-template
        0, 0,
        NULL, NULL
    },
    {
        261, // insert-sheet
        0, 0,
        NULL, NULL
    },
    {
        269, // job-account-id
        0, 0,
        NULL, NULL
    },
    {
        273, // job-account-type
        0, 0,
        NULL, NULL
    },
    {
        278, // job-accounting-sheets
        0, 0,
        NULL, NULL
    },
    {
        283, // job-accounting-user-id
        0, 0,
        NULL, NULL
    },
    {
        294, // job-copies
        0, 0,
        NULL, NULL
    },
    {
        298, // job-cover-back
        0, 0,
        NULL, NULL
    },
    {
        302, // job-cover-front
        0, 0,
        NULL, NULL
    },
    {
        307, // job-delay-output-until
        0, 0,
        NULL, NULL
    },
    {
        310, // job-delay-output-until-time
        0, 0,
        NULL, NULL
    },
    {
        315, // job-error-action
        0, 0,
        NULL, NULL
    },
    {
        318, // job-error-sheet
        0, 0,
        NULL, NULL
    },
    {
        324, // job-finishings
        0, 0,
        NULL, NULL
    },
    {
        326, // job-finishings-col
        0, 0,
        NULL, NULL
    },
    {
        334, // job-hold-until
        0, 0,
        NULL, NULL
    },
    {
        338, // job-hold-until-time
        0, 0,
        NULL, NULL
    },
    {
        359, // job-message-to-operator
        0, 0,
        NULL, NULL
    },
    {
        372, // job-pages-per-set
        0, 0,
        NULL, NULL
    },
    {
        381, // job-phone-number
        0, 0,
        NULL, NULL
    },
    {
        387, // job-priority
        0, 0,
        NULL, NULL
    },
    {
        393, // job-recipient-name
        0, 0,
        NULL, NULL
    },
    {
        397, // job-save-disposition
        0, 0,
        NULL, NULL
    },
    {
        400, // job-sheet-message
        0, 0,
        NULL, NULL
    },
    {
        404, // job-sheets
        0, 0,
        NULL, NULL
    },
    {
        406, // job-sheets-col
        0, 0,
        NULL, NULL
    },
    {
        457, // materials-col
        0, 0,
        NULL, NULL
    },
    {
        466, // media
        0, 0,
        NULL, NULL
    },
    {
        472, // media-col
        0, 0,
        NULL, NULL
    },
    {
        489, // media-input-tray-check
        0, 0,
        NULL, NULL
    },
    {
        534, // multiple-document-handling
        0, 0,
        NULL, NULL
    },
    {
        539, // multiple-object-handling
        0, 0,
        NULL, NULL
    },
    {
        583, // number-of-retries
        0, 0,
        NULL, NULL
    },
    {
        586, // number-up
        0, 0,
        NULL, NULL
    },
    {
        597, // orientation-requested
        0, 0,
        NULL, NULL
    },
    {
        606, // output-bin
        0, 0,
        NULL, NULL
    },
    {
        611, // output-device
        0, 0,
        NULL, NULL
    },
    {
        625, // overrides
        0, 0,
        NULL, NULL
    },
    {
        628, // page-delivery
        0, 0,
        NULL, NULL
    },
    {
        632, // page-order-received
        0, 0,
        NULL, NULL
    },
    {
        636, // page-ranges
        0, 0,
        NULL, NULL
    },
    {
        646, // pages-per-subset
        0, 0,
        NULL, NULL
    },
    {
        650, // pclm-source-resolution
        0, 0,
        NULL, NULL
    },
    {
        657, // pdl-init-file
        0, 0,
        NULL, NULL
    },
    {
        670, // platform-temperature
        0, 0,
        NULL, NULL
    },
    {
        678, // presentation-direction-number-up
        0, 0,
        NULL, NULL
    },
    {
        683, // print-accuracy
        0, 0,
        NULL, NULL
    },
    {
        686, // print-base
        0, 0,
        NULL, NULL
    },
    {
        690, // print-color-mode
        0, 0,
        NULL, NULL
    },
    {
        694, // print-content-optimize
        0, 0,
        NULL, NULL
    },
    {
        698, // print-objects
        0, 0,
        NULL, NULL
    },
    {
        701, // print-quality
        0, 0,
        NULL, NULL
    },
    {
        705, // print-rendering-intent
        0, 0,
        NULL, NULL
    },
    {
        709, // print-scaling
        0, 0,
        NULL, NULL
    },
    {
        712, // print-supports
        0, 0,
        NULL, NULL
    },
    {
        757, // printer-resolution
        0, 0,
        NULL, NULL
    },
    {
        784, // proof-print
        0, 0,
        NULL, NULL
    },
    {
        807, // retry-interval
        0, 0,
        NULL, NULL
    },
    {
        810, // retry-time-out
        0, 0,
        NULL, NULL
    },
    {
        825, // separator-sheets
        0, 0,
        NULL, NULL
    },
    {
        830, // sheet-collate
        0, 0,
        NULL, NULL
    },
    {
        836, // sides
        0, 0,
        NULL, NULL
    },
    {
        881, // x-image-position
        0, 0,
        NULL, NULL
    },
    {
        885, // x-image-shift
        0, 0,
        NULL, NULL
    },
    {
        891, // x-side1-image-shift
        0, 0,
        NULL, NULL
    },
    {
        895, // x-side2-image-shift
        0, 0,
        NULL, NULL
    },
    {
        907, // y-image-position
        0, 0,
        NULL, NULL
    },
    {
        911, // y-image-shift
        0, 0,
        NULL, NULL
    },
    {
        917, // y-side1-image-shift
        0, 0,
        NULL, NULL
    },
    {
        921, // y-side2-image-shift
        0, 0,
        NULL, NULL
    },
};

ipp_attr_t s_ipp_attr_group_operation[] =
{
    {
        30, // attributes-charset
        0, 0,
        NULL, NULL
    },
    {
        31, // attributes-natural-language
        0, 0,
        NULL, NULL
    },
    {
        44, // charge-info-message
        0, 0,
        NULL, NULL
    },
    {
        53, // compression
        0, 0,
        NULL, NULL
    },
    {
        54, // compression-accepted
        0, 0,
        NULL, NULL
    },
    {
        82, // destination-accesses
        0, 0,
        NULL, NULL
    },
    {
        99, // detailed-status-message
        0, 0,
        NULL, NULL
    },
    {
        103, // document-access
        0, 0,
        NULL, NULL
    },
    {
        104, // document-access-error
        0, 0,
        NULL, NULL
    },
    {
        107, // document-charset
        0, 0,
        NULL, NULL
    },
    {
        113, // document-data-get-interval
        0, 0,
        NULL, NULL
    },
    {
        114, // document-data-wait
        0, 0,
        NULL, NULL
    },
    {
        115, // document-digital-signature
        0, 0,
        NULL, NULL
    },
    {
        119, // document-format
        0, 0,
        NULL, NULL
    },
    {
        120, // document-format-accepted
        0, 0,
        NULL, NULL
    },
    {
        122, // document-format-details
        0, 0,
        NULL, NULL
    },
    {
        140, // document-message
        0, 0,
        NULL, NULL
    },
    {
        142, // document-metadata
        0, 0,
        NULL, NULL
    },
    {
        143, // document-name
        0, 0,
        NULL, NULL
    },
    {
        145, // document-natural-language
        0, 0,
        NULL, NULL
    },
    {
        149, // document-number
        0, 0,
        NULL, NULL
    },
    {
        151, // document-password
        0, 0,
        NULL, NULL
    },
    {
        153, // document-preprocessed
        0, 0,
        NULL, NULL
    },
    {
        171, // fetch-status-code
        0, 0,
        NULL, NULL
    },
    {
        172, // fetch-status-message
        0, 0,
        NULL, NULL
    },
    {
        187, // first-index
        0, 0,
        NULL, NULL
    },
    {
        212, // identify-actions
        0, 0,
        NULL, NULL
    },
    {
        225, // input-attributes
        0, 0,
        NULL, NULL
    },
    {
        265, // ipp-attribute-fidelity
        0, 0,
        NULL, NULL
    },
    {
        288, // job-authorization-uri
        0, 0,
        NULL, NULL
    },
    {
        334, // job-hold-until
        0, 0,
        NULL, NULL
    },
    {
        340, // job-id
        0, 0,
        NULL, NULL
    },
    {
        341, // job-ids
        0, 0,
        NULL, NULL
    },
    {
        343, // job-impressions
        0, 0,
        NULL, NULL
    },
    {
        344, // job-impressions-col
        0, 0,
        NULL, NULL
    },
    {
        347, // job-impressions-estimated
        0, 0,
        NULL, NULL
    },
    {
        349, // job-k-octets
        0, 0,
        NULL, NULL
    },
    {
        352, // job-mandatory-attributes
        0, 0,
        NULL, NULL
    },
    {
        353, // job-media-sheets
        0, 0,
        NULL, NULL
    },
    {
        354, // job-media-sheets-col
        0, 0,
        NULL, NULL
    },
    {
        358, // job-message-from-operator
        0, 0,
        NULL, NULL
    },
    {
        364, // job-name
        0, 0,
        NULL, NULL
    },
    {
        367, // job-pages
        0, 0,
        NULL, NULL
    },
    {
        368, // job-pages-col
        0, 0,
        NULL, NULL
    },
    {
        374, // job-password
        0, 0,
        NULL, NULL
    },
    {
        375, // job-password-encryption
        0, 0,
        NULL, NULL
    },
    {
        413, // job-state
        0, 0,
        NULL, NULL
    },
    {
        414, // job-state-message
        0, 0,
        NULL, NULL
    },
    {
        415, // job-state-reasons
        0, 0,
        NULL, NULL
    },
    {
        417, // job-uri
        0, 0,
        NULL, NULL
    },
    {
        430, // last-document
        0, 0,
        NULL, NULL
    },
    {
        431, // limit
        0, 0,
        NULL, NULL
    },
    {
        528, // message
        0, 0,
        NULL, NULL
    },
    {
        545, // my-jobs
        0, 0,
        NULL, NULL
    },
    {
        554, // notify-get-interval
        0, 0,
        NULL, NULL
    },
    {
        569, // notify-sequence-numbers
        0, 0,
        NULL, NULL
    },
    {
        575, // notify-subscription-ids
        0, 0,
        NULL, NULL
    },
    {
        580, // notify-wait
        0, 0,
        NULL, NULL
    },
    {
        601, // original-requesting-user-name
        0, 0,
        NULL, NULL
    },
    {
        602, // output-attributes
        0, 0,
        NULL, NULL
    },
    {
        620, // output-device-job-states
        0, 0,
        NULL, NULL
    },
    {
        622, // output-device-uuid
        0, 0,
        NULL, NULL
    },
    {
        676, // preferred-attributes
        0, 0,
        NULL, NULL
    },
    {
        748, // printer-message-from-operator
        0, 0,
        NULL, NULL
    },
    {
        775, // printer-up-time
        0, 0,
        NULL, NULL
    },
    {
        776, // printer-uri
        0, 0,
        NULL, NULL
    },
    {
        782, // profile-uri-actual
        0, 0,
        NULL, NULL
    },
    {
        802, // requested-attributes
        0, 0,
        NULL, NULL
    },
    {
        803, // requesting-user-name
        0, 0,
        NULL, NULL
    },
    {
        804, // requesting-user-uri
        0, 0,
        NULL, NULL
    },
    {
        840, // status-message
        0, 0,
        NULL, NULL
    },
    {
        877, // which-jobs
        0, 0,
        NULL, NULL
    },
};

ipp_attr_t s_ipp_attr_group_prtdesc[] =
{
    {
        29, // accuracy-units-supported
        0, 0,
        NULL, NULL
    },
    {
        34, // baling-type-supported
        0, 0,
        NULL, NULL
    },
    {
        36, // baling-when-supported
        0, 0,
        NULL, NULL
    },
    {
        39, // binding-reference-edge-supported
        0, 0,
        NULL, NULL
    },
    {
        41, // binding-type-supported
        0, 0,
        NULL, NULL
    },
    {
        45, // charset-configured
        0, 0,
        NULL, NULL
    },
    {
        46, // charset-supported
        0, 0,
        NULL, NULL
    },
    {
        49, // coating-sides-supported
        0, 0,
        NULL, NULL
    },
    {
        51, // coating-type-supported
        0, 0,
        NULL, NULL
    },
    {
        52, // color-supported
        0, 0,
        NULL, NULL
    },
    {
        56, // compression-supported
        0, 0,
        NULL, NULL
    },
    {
        58, // confirmation-sheet-print-default
        0, 0,
        NULL, NULL
    },
    {
        61, // copies-default
        0, 0,
        NULL, NULL
    },
    {
        62, // copies-supported
        0, 0,
        NULL, NULL
    },
    {
        65, // cover-back-default
        0, 0,
        NULL, NULL
    },
    {
        66, // cover-back-supported
        0, 0,
        NULL, NULL
    },
    {
        69, // cover-front-default
        0, 0,
        NULL, NULL
    },
    {
        70, // cover-front-supported
        0, 0,
        NULL, NULL
    },
    {
        72, // cover-sheet-info-default
        0, 0,
        NULL, NULL
    },
    {
        73, // cover-sheet-info-supported
        0, 0,
        NULL, NULL
    },
    {
        77, // covering-name-supported
        0, 0,
        NULL, NULL
    },
    {
        83, // destination-accesses-supported
        0, 0,
        NULL, NULL
    },
    {
        95, // destination-uri-ready
        0, 0,
        NULL, NULL
    },
    {
        96, // destination-uri-schemes-supported
        0, 0,
        NULL, NULL
    },
    {
        98, // destination-uris-supported
        0, 0,
        NULL, NULL
    },
    {
        106, // document-access-supported
        0, 0,
        NULL, NULL
    },
    {
        108, // document-charset-default
        0, 0,
        NULL, NULL
    },
    {
        110, // document-charset-supported
        0, 0,
        NULL, NULL
    },
    {
        112, // document-creation-attributes-supported
        0, 0,
        NULL, NULL
    },
    {
        116, // document-digital-signature-default
        0, 0,
        NULL, NULL
    },
    {
        118, // document-digital-signature-supported
        0, 0,
        NULL, NULL
    },
    {
        121, // document-format-default
        0, 0,
        NULL, NULL
    },
    {
        123, // document-format-details-default
        0, 0,
        NULL, NULL
    },
    {
        126, // document-format-details-supported
        0, 0,
        NULL, NULL
    },
    {
        131, // document-format-supported
        0, 0,
        NULL, NULL
    },
    {
        134, // document-format-version-default
        0, 0,
        NULL, NULL
    },
    {
        137, // document-format-version-supported
        0, 0,
        NULL, NULL
    },
    {
        146, // document-natural-language-default
        0, 0,
        NULL, NULL
    },
    {
        148, // document-natural-language-supported
        0, 0,
        NULL, NULL
    },
    {
        152, // document-password-supported
        0, 0,
        NULL, NULL
    },
    {
        155, // document-privacy-attributes
        0, 0,
        NULL, NULL
    },
    {
        156, // document-privacy-scope
        0, 0,
        NULL, NULL
    },
    {
        168, // feed-orientation-default
        0, 0,
        NULL, NULL
    },
    {
        169, // feed-orientation-supported
        0, 0,
        NULL, NULL
    },
    {
        170, // fetch-document-attributes-supported
        0, 0,
        NULL, NULL
    },
    {
        175, // finishing-template-supported
        0, 0,
        NULL, NULL
    },
    {
        180, // finishings-col-database
        0, 0,
        NULL, NULL
    },
    {
        181, // finishings-col-default
        0, 0,
        NULL, NULL
    },
    {
        182, // finishings-col-ready
        0, 0,
        NULL, NULL
    },
    {
        183, // finishings-col-supported
        0, 0,
        NULL, NULL
    },
    {
        184, // finishings-default
        0, 0,
        NULL, NULL
    },
    {
        185, // finishings-ready
        0, 0,
        NULL, NULL
    },
    {
        186, // finishings-supported
        0, 0,
        NULL, NULL
    },
    {
        190, // folding-direction-supported
        0, 0,
        NULL, NULL
    },
    {
        192, // folding-offset-supported
        0, 0,
        NULL, NULL
    },
    {
        194, // folding-reference-edge-supported
        0, 0,
        NULL, NULL
    },
    {
        196, // font-name-requested-default
        0, 0,
        NULL, NULL
    },
    {
        197, // font-name-requested-supported
        0, 0,
        NULL, NULL
    },
    {
        199, // font-size-requested-default
        0, 0,
        NULL, NULL
    },
    {
        200, // font-size-requested-supported
        0, 0,
        NULL, NULL
    },
    {
        203, // force-front-side-default (under review)
        0, 0,
        NULL, NULL
    },
    {
        204, // force-front-side-supported (under review)
        0, 0,
        NULL, NULL
    },
    {
        206, // from-name-supported
        0, 0,
        NULL, NULL
    },
    {
        209, // generated-natural-language-supported
        0, 0,
        NULL, NULL
    },
    {
        213, // identify-actions-default
        0, 0,
        NULL, NULL
    },
    {
        214, // identify-actions-supported
        0, 0,
        NULL, NULL
    },
    {
        218, // imposition-template-default
        0, 0,
        NULL, NULL
    },
    {
        219, // imposition-template-supported
        0, 0,
        NULL, NULL
    },
    {
        227, // input-attributes-default
        0, 0,
        NULL, NULL
    },
    {
        228, // input-attributes-supported
        0, 0,
        NULL, NULL
    },
    {
        233, // input-color-mode-supported
        0, 0,
        NULL, NULL
    },
    {
        235, // input-content-type-supported
        0, 0,
        NULL, NULL
    },
    {
        238, // input-film-scan-mode-supported
        0, 0,
        NULL, NULL
    },
    {
        241, // input-media-supported
        0, 0,
        NULL, NULL
    },
    {
        243, // input-orientation-requested-supported
        0, 0,
        NULL, NULL
    },
    {
        245, // input-quality-supported
        0, 0,
        NULL, NULL
    },
    {
        247, // input-resolution-supported
        0, 0,
        NULL, NULL
    },
    {
        251, // input-scan-regions-supported
        0, 0,
        NULL, NULL
    },
    {
        254, // input-sides-supported
        0, 0,
        NULL, NULL
    },
    {
        256, // input-source-supported
        0, 0,
        NULL, NULL
    },
    {
        258, // insert-after-page-number-supported
        0, 0,
        NULL, NULL
    },
    {
        260, // insert-count-supported
        0, 0,
        NULL, NULL
    },
    {
        263, // insert-sheet-default
        0, 0,
        NULL, NULL
    },
    {
        264, // insert-sheet-supported
        0, 0,
        NULL, NULL
    },
    {
        266, // ipp-features-supported
        0, 0,
        NULL, NULL
    },
    {
        267, // ipp-versions-supported
        0, 0,
        NULL, NULL
    },
    {
        268, // ippget-event-life
        0, 0,
        NULL, NULL
    },
    {
        271, // job-account-id-default
        0, 0,
        NULL, NULL
    },
    {
        272, // job-account-id-supported
        0, 0,
        NULL, NULL
    },
    {
        275, // job-account-type-default
        0, 0,
        NULL, NULL
    },
    {
        276, // job-account-type-supported
        0, 0,
        NULL, NULL
    },
    {
        280, // job-accounting-sheets-default
        0, 0,
        NULL, NULL
    },
    {
        281, // job-accounting-sheets-supported
        0, 0,
        NULL, NULL
    },
    {
        285, // job-accounting-user-id-default
        0, 0,
        NULL, NULL
    },
    {
        286, // job-accounting-user-id-supported
        0, 0,
        NULL, NULL
    },
    {
        289, // job-authorization-uri-supported
        0, 0,
        NULL, NULL
    },
    {
        293, // job-constraints-supported
        0, 0,
        NULL, NULL
    },
    {
        296, // job-copies-default
        0, 0,
        NULL, NULL
    },
    {
        297, // job-copies-supported
        0, 0,
        NULL, NULL
    },
    {
        300, // job-cover-back-default
        0, 0,
        NULL, NULL
    },
    {
        301, // job-cover-back-supported
        0, 0,
        NULL, NULL
    },
    {
        304, // job-cover-front-default
        0, 0,
        NULL, NULL
    },
    {
        305, // job-cover-front-supported
        0, 0,
        NULL, NULL
    },
    {
        306, // job-creation-attributes-supported
        0, 0,
        NULL, NULL
    },
    {
        308, // job-delay-output-until-default
        0, 0,
        NULL, NULL
    },
    {
        309, // job-delay-output-until-supported
        0, 0,
        NULL, NULL
    },
    {
        311, // job-delay-output-until-time-supported
        0, 0,
        NULL, NULL
    },
    {
        312, // job-destination-spooling-supported
        0, 0,
        NULL, NULL
    },
    {
        316, // job-error-action-default
        0, 0,
        NULL, NULL
    },
    {
        317, // job-error-action-supported
        0, 0,
        NULL, NULL
    },
    {
        320, // job-error-sheet-default
        0, 0,
        NULL, NULL
    },
    {
        321, // job-error-sheet-supported
        0, 0,
        NULL, NULL
    },
    {
        328, // job-finishings-col-default
        0, 0,
        NULL, NULL
    },
    {
        329, // job-finishings-col-ready
        0, 0,
        NULL, NULL
    },
    {
        330, // job-finishings-col-supported
        0, 0,
        NULL, NULL
    },
    {
        331, // job-finishings-default
        0, 0,
        NULL, NULL
    },
    {
        332, // job-finishings-ready
        0, 0,
        NULL, NULL
    },
    {
        333, // job-finishings-supported
        0, 0,
        NULL, NULL
    },
    {
        336, // job-hold-until-default
        0, 0,
        NULL, NULL
    },
    {
        337, // job-hold-until-supported
        0, 0,
        NULL, NULL
    },
    {
        339, // job-hold-until-time-supported
        0, 0,
        NULL, NULL
    },
    {
        342, // job-ids-supported
        0, 0,
        NULL, NULL
    },
    {
        348, // job-impressions-supported
        0, 0,
        NULL, NULL
    },
    {
        351, // job-k-octets-supported
        0, 0,
        NULL, NULL
    },
    {
        357, // job-media-sheets-supported
        0, 0,
        NULL, NULL
    },
    {
        361, // job-message-to-operator-default
        0, 0,
        NULL, NULL
    },
    {
        362, // job-message-to-operator-supported
        0, 0,
        NULL, NULL
    },
    {
        373, // job-pages-per-set-supported
        0, 0,
        NULL, NULL
    },
    {
        376, // job-password-encryption-supported
        0, 0,
        NULL, NULL
    },
    {
        377, // job-password-length-supported
        0, 0,
        NULL, NULL
    },
    {
        378, // job-password-repertoire-configured
        0, 0,
        NULL, NULL
    },
    {
        379, // job-password-repertoire-supported
        0, 0,
        NULL, NULL
    },
    {
        380, // job-password-supported
        0, 0,
        NULL, NULL
    },
    {
        382, // job-phone-number-default
        0, 0,
        NULL, NULL
    },
    {
        383, // job-phone-number-supported
        0, 0,
        NULL, NULL
    },
    {
        384, // job-presets-supported
        0, 0,
        NULL, NULL
    },
    {
        389, // job-priority-default
        0, 0,
        NULL, NULL
    },
    {
        390, // job-priority-supported
        0, 0,
        NULL, NULL
    },
    {
        391, // job-privacy-attributes
        0, 0,
        NULL, NULL
    },
    {
        392, // job-privacy-scope
        0, 0,
        NULL, NULL
    },
    {
        394, // job-recipient-name-default
        0, 0,
        NULL, NULL
    },
    {
        395, // job-recipient-name-supported
        0, 0,
        NULL, NULL
    },
    {
        396, // job-resolvers-supported
        0, 0,
        NULL, NULL
    },
    {
        402, // job-sheet-message-default
        0, 0,
        NULL, NULL
    },
    {
        403, // job-sheet-message-supported
        0, 0,
        NULL, NULL
    },
    {
        408, // job-sheets-col-default
        0, 0,
        NULL, NULL
    },
    {
        409, // job-sheets-col-supported
        0, 0,
        NULL, NULL
    },
    {
        410, // job-sheets-default
        0, 0,
        NULL, NULL
    },
    {
        411, // job-sheets-supported
        0, 0,
        NULL, NULL
    },
    {
        412, // job-spooling-supported
        0, 0,
        NULL, NULL
    },
    {
        416, // job-triggers-supported
        0, 0,
        NULL, NULL
    },
    {
        419, // jpeg-features-supported
        0, 0,
        NULL, NULL
    },
    {
        420, // jpeg-k-octets-supported
        0, 0,
        NULL, NULL
    },
    {
        421, // jpeg-x-dimension-supported
        0, 0,
        NULL, NULL
    },
    {
        422, // jpeg-y-dimension-supported
        0, 0,
        NULL, NULL
    },
    {
        427, // laminating-sides-supported
        0, 0,
        NULL, NULL
    },
    {
        429, // laminating-type-supported
        0, 0,
        NULL, NULL
    },
    {
        433, // logo-uri-formats-supported
        0, 0,
        NULL, NULL
    },
    {
        434, // logo-uri-schemes-supported
        0, 0,
        NULL, NULL
    },
    {
        437, // material-amount-units-supported
        0, 0,
        NULL, NULL
    },
    {
        440, // material-diameter-supported
        0, 0,
        NULL, NULL
    },
    {
        446, // material-purpose-supported
        0, 0,
        NULL, NULL
    },
    {
        448, // material-rate-supported
        0, 0,
        NULL, NULL
    },
    {
        450, // material-rate-units-supported
        0, 0,
        NULL, NULL
    },
    {
        452, // material-shell-thickness-supported
        0, 0,
        NULL, NULL
    },
    {
        454, // material-temperature-supported
        0, 0,
        NULL, NULL
    },
    {
        456, // material-type-supported
        0, 0,
        NULL, NULL
    },
    {
        459, // materials-col-database
        0, 0,
        NULL, NULL
    },
    {
        460, // materials-col-default
        0, 0,
        NULL, NULL
    },
    {
        461, // materials-col-ready
        0, 0,
        NULL, NULL
    },
    {
        462, // materials-col-supported
        0, 0,
        NULL, NULL
    },
    {
        463, // max-materials-col-supported
        0, 0,
        NULL, NULL
    },
    {
        464, // max-save-info-supported
        0, 0,
        NULL, NULL
    },
    {
        465, // max-stitching-locations-supported
        0, 0,
        NULL, NULL
    },
    {
        469, // media-back-coating-supported
        0, 0,
        NULL, NULL
    },
    {
        471, // media-bottom-margin-supported
        0, 0,
        NULL, NULL
    },
    {
        474, // media-col-database
        0, 0,
        NULL, NULL
    },
    {
        475, // media-col-default
        0, 0,
        NULL, NULL
    },
    {
        476, // media-col-ready
        0, 0,
        NULL, NULL
    },
    {
        477, // media-col-supported
        0, 0,
        NULL, NULL
    },
    {
        479, // media-color-supported
        0, 0,
        NULL, NULL
    },
    {
        480, // media-default
        0, 0,
        NULL, NULL
    },
    {
        482, // media-front-coating-supported
        0, 0,
        NULL, NULL
    },
    {
        484, // media-grain-supported
        0, 0,
        NULL, NULL
    },
    {
        486, // media-hole-count-supported
        0, 0,
        NULL, NULL
    },
    {
        488, // media-info-supported
        0, 0,
        NULL, NULL
    },
    {
        492, // media-key-supported
        0, 0,
        NULL, NULL
    },
    {
        494, // media-left-margin-supported
        0, 0,
        NULL, NULL
    },
    {
        496, // media-order-count-supported
        0, 0,
        NULL, NULL
    },
    {
        498, // media-pre-printed-supported
        0, 0,
        NULL, NULL
    },
    {
        499, // media-ready
        0, 0,
        NULL, NULL
    },
    {
        501, // media-recycled-supported
        0, 0,
        NULL, NULL
    },
    {
        503, // media-right-margin-supported
        0, 0,
        NULL, NULL
    },
    {
        511, // media-size-supported
        0, 0,
        NULL, NULL
    },
    {
        516, // media-source-supported
        0, 0,
        NULL, NULL
    },
    {
        517, // media-supported
        0, 0,
        NULL, NULL
    },
    {
        519, // media-thickness-supported
        0, 0,
        NULL, NULL
    },
    {
        521, // media-tooth-supported
        0, 0,
        NULL, NULL
    },
    {
        523, // media-top-margin-supported
        0, 0,
        NULL, NULL
    },
    {
        525, // media-type-supported
        0, 0,
        NULL, NULL
    },
    {
        527, // media-weight-metric-supported
        0, 0,
        NULL, NULL
    },
    {
        529, // message-supported
        0, 0,
        NULL, NULL
    },
    {
        533, // multiple-destination-uris-supported
        0, 0,
        NULL, NULL
    },
    {
        536, // multiple-document-handling-default
        0, 0,
        NULL, NULL
    },
    {
        537, // multiple-document-handling-supported
        0, 0,
        NULL, NULL
    },
    {
        538, // multiple-document-jobs-supported
        0, 0,
        NULL, NULL
    },
    {
        541, // multiple-object-handling-default
        0, 0,
        NULL, NULL
    },
    {
        542, // multiple-object-handling-supported
        0, 0,
        NULL, NULL
    },
    {
        543, // multiple-operation-time-out
        0, 0,
        NULL, NULL
    },
    {
        544, // multiple-operation-time-out-action
        0, 0,
        NULL, NULL
    },
    {
        546, // natural-language-configured
        0, 0,
        NULL, NULL
    },
    {
        549, // notify-attributes-supported
        0, 0,
        NULL, NULL
    },
    {
        552, // notify-events-default
        0, 0,
        NULL, NULL
    },
    {
        553, // notify-events-supported
        0, 0,
        NULL, NULL
    },
    {
        557, // notify-lease-duration-default
        0, 0,
        NULL, NULL
    },
    {
        558, // notify-lease-duration-supported
        0, 0,
        NULL, NULL
    },
    {
        565, // notify-pull-method-supported
        0, 0,
        NULL, NULL
    },
    {
        567, // notify-schemes-supported
        0, 0,
        NULL, NULL
    },
    {
        584, // number-of-retries-default
        0, 0,
        NULL, NULL
    },
    {
        585, // number-of-retries-supported
        0, 0,
        NULL, NULL
    },
    {
        588, // number-up-default
        0, 0,
        NULL, NULL
    },
    {
        589, // number-up-supported
        0, 0,
        NULL, NULL
    },
    {
        590, // oauth-authorization-server-uri
        0, 0,
        NULL, NULL
    },
    {
        594, // operations-supported
        0, 0,
        NULL, NULL
    },
    {
        596, // organization-name-supported
        0, 0,
        NULL, NULL
    },
    {
        599, // orientation-requested-default
        0, 0,
        NULL, NULL
    },
    {
        600, // orientation-requested-supported
        0, 0,
        NULL, NULL
    },
    {
        604, // output-attributes-default
        0, 0,
        NULL, NULL
    },
    {
        605, // output-attributes-supported
        0, 0,
        NULL, NULL
    },
    {
        608, // output-bin-default
        0, 0,
        NULL, NULL
    },
    {
        609, // output-bin-supported
        0, 0,
        NULL, NULL
    },
    {
        621, // output-device-supported
        0, 0,
        NULL, NULL
    },
    {
        624, // output-device-uuid-supported
        0, 0,
        NULL, NULL
    },
    {
        627, // overrides-supported
        0, 0,
        NULL, NULL
    },
    {
        630, // page-delivery-default
        0, 0,
        NULL, NULL
    },
    {
        631, // page-delivery-supported
        0, 0,
        NULL, NULL
    },
    {
        634, // page-order-received-default
        0, 0,
        NULL, NULL
    },
    {
        635, // page-order-received-supported
        0, 0,
        NULL, NULL
    },
    {
        638, // page-ranges-supported
        0, 0,
        NULL, NULL
    },
    {
        647, // pages-per-subset-supported
        0, 0,
        NULL, NULL
    },
    {
        648, // parent-printers-supported
        0, 0,
        NULL, NULL
    },
    {
        649, // pclm-raster-back-side
        0, 0,
        NULL, NULL
    },
    {
        651, // pclm-source-resolution-supported
        0, 0,
        NULL, NULL
    },
    {
        652, // pclm-strip-height-preferred
        0, 0,
        NULL, NULL
    },
    {
        653, // pclm-strip-height-supported
        0, 0,
        NULL, NULL
    },
    {
        654, // pdf-features-supported
        0, 0,
        NULL, NULL
    },
    {
        655, // pdf-k-octets-supported
        0, 0,
        NULL, NULL
    },
    {
        656, // pdf-versions-supported
        0, 0,
        NULL, NULL
    },
    {
        658, // pdl-init-file-default
        0, 0,
        NULL, NULL
    },
    {
        660, // pdl-init-file-entry-supported
        0, 0,
        NULL, NULL
    },
    {
        662, // pdl-init-file-location-supported
        0, 0,
        NULL, NULL
    },
    {
        664, // pdl-init-file-name-subdirectory-supported
        0, 0,
        NULL, NULL
    },
    {
        665, // pdl-init-file-name-supported
        0, 0,
        NULL, NULL
    },
    {
        666, // pdl-init-file-supported
        0, 0,
        NULL, NULL
    },
    {
        667, // pdl-override-guaranteed-supported
        0, 0,
        NULL, NULL
    },
    {
        668, // pdl-override-supported
        0, 0,
        NULL, NULL
    },
    {
        669, // platform-shape
        0, 0,
        NULL, NULL
    },
    {
        672, // platform-temperature-default
        0, 0,
        NULL, NULL
    },
    {
        673, // platform-temperature-supported
        0, 0,
        NULL, NULL
    },
    {
        677, // preferred-attributes-supported
        0, 0,
        NULL, NULL
    },
    {
        680, // presentation-direction-number-up-default
        0, 0,
        NULL, NULL
    },
    {
        681, // presentation-direction-number-up-supported
        0, 0,
        NULL, NULL
    },
    {
        685, // print-accuracy-supported
        0, 0,
        NULL, NULL
    },
    {
        688, // print-base-default
        0, 0,
        NULL, NULL
    },
    {
        689, // print-base-supported
        0, 0,
        NULL, NULL
    },
    {
        692, // print-color-mode-default
        0, 0,
        NULL, NULL
    },
    {
        693, // print-color-mode-supported
        0, 0,
        NULL, NULL
    },
    {
        696, // print-content-optimize-default
        0, 0,
        NULL, NULL
    },
    {
        697, // print-content-optimize-supported
        0, 0,
        NULL, NULL
    },
    {
        700, // print-objects-supported
        0, 0,
        NULL, NULL
    },
    {
        703, // print-quality-default
        0, 0,
        NULL, NULL
    },
    {
        704, // print-quality-supported
        0, 0,
        NULL, NULL
    },
    {
        707, // print-rendering-intent-default
        0, 0,
        NULL, NULL
    },
    {
        708, // print-rendering-intent-supported
        0, 0,
        NULL, NULL
    },
    {
        710, // print-scaling-default
        0, 0,
        NULL, NULL
    },
    {
        711, // print-scaling-supported
        0, 0,
        NULL, NULL
    },
    {
        714, // print-supports-default
        0, 0,
        NULL, NULL
    },
    {
        715, // print-supports-supported
        0, 0,
        NULL, NULL
    },
    {
        718, // printer-camera-image-uri
        0, 0,
        NULL, NULL
    },
    {
        719, // printer-charge-info
        0, 0,
        NULL, NULL
    },
    {
        720, // printer-charge-info-uri
        0, 0,
        NULL, NULL
    },
    {
        723, // printer-current-time
        0, 0,
        NULL, NULL
    },
    {
        725, // printer-device-id
        0, 0,
        NULL, NULL
    },
    {
        726, // printer-dns-sd-name
        0, 0,
        NULL, NULL
    },
    {
        727, // printer-driver-installer
        0, 0,
        NULL, NULL
    },
    {
        728, // printer-fax-log-uri
        0, 0,
        NULL, NULL
    },
    {
        729, // printer-fax-modem-info
        0, 0,
        NULL, NULL
    },
    {
        730, // printer-fax-modem-name
        0, 0,
        NULL, NULL
    },
    {
        731, // printer-fax-modem-number
        0, 0,
        NULL, NULL
    },
    {
        736, // printer-geo-location
        0, 0,
        NULL, NULL
    },
    {
        737, // printer-get-attributes-supported
        0, 0,
        NULL, NULL
    },
    {
        738, // printer-icc-profiles
        0, 0,
        NULL, NULL
    },
    {
        739, // printer-icons
        0, 0,
        NULL, NULL
    },
    {
        740, // printer-info
        0, 0,
        NULL, NULL
    },
    {
        743, // printer-kind
        0, 0,
        NULL, NULL
    },
    {
        744, // printer-location
        0, 0,
        NULL, NULL
    },
    {
        745, // printer-make-and-model
        0, 0,
        NULL, NULL
    },
    {
        746, // printer-mandatory-job-attributes
        0, 0,
        NULL, NULL
    },
    {
        751, // printer-more-info-manufacturer
        0, 0,
        NULL, NULL
    },
    {
        752, // printer-name
        0, 0,
        NULL, NULL
    },
    {
        753, // printer-organization
        0, 0,
        NULL, NULL
    },
    {
        754, // printer-organizational-unit
        0, 0,
        NULL, NULL
    },
    {
        756, // printer-privacy-policy-uri
        0, 0,
        NULL, NULL
    },
    {
        759, // printer-resolution-default
        0, 0,
        NULL, NULL
    },
    {
        760, // printer-resolution-supported
        0, 0,
        NULL, NULL
    },
    {
        767, // printer-static-resource-directory-uri
        0, 0,
        NULL, NULL
    },
    {
        769, // printer-static-resource-k-octets-supported
        0, 0,
        NULL, NULL
    },
    {
        770, // printer-strings-languages-supported
        0, 0,
        NULL, NULL
    },
    {
        771, // printer-strings-uri
        0, 0,
        NULL, NULL
    },
    {
        779, // printer-volume-supported
        0, 0,
        NULL, NULL
    },
    {
        780, // printer-xri-supported
        0, 0,
        NULL, NULL
    },
    {
        786, // proof-print-default
        0, 0,
        NULL, NULL
    },
    {
        787, // proof-print-supported
        0, 0,
        NULL, NULL
    },
    {
        789, // punching-hole-diameter-configured
        0, 0,
        NULL, NULL
    },
    {
        791, // punching-locations-supported
        0, 0,
        NULL, NULL
    },
    {
        793, // punching-offset-supported
        0, 0,
        NULL, NULL
    },
    {
        795, // punching-reference-edge-supported
        0, 0,
        NULL, NULL
    },
    {
        796, // pwg-raster-document-resolution-supported
        0, 0,
        NULL, NULL
    },
    {
        797, // pwg-raster-document-sheet-back
        0, 0,
        NULL, NULL
    },
    {
        798, // pwg-raster-document-type-supported
        0, 0,
        NULL, NULL
    },
    {
        800, // reference-uri-schemes-supported
        0, 0,
        NULL, NULL
    },
    {
        801, // repertoire-supported
        0, 0,
        NULL, NULL
    },
    {
        805, // requesting-user-uri-supported
        0, 0,
        NULL, NULL
    },
    {
        808, // retry-interval-default
        0, 0,
        NULL, NULL
    },
    {
        809, // retry-interval-supported
        0, 0,
        NULL, NULL
    },
    {
        811, // retry-time-out-default
        0, 0,
        NULL, NULL
    },
    {
        812, // retry-time-out-supported
        0, 0,
        NULL, NULL
    },
    {
        814, // save-disposition-supported
        0, 0,
        NULL, NULL
    },
    {
        816, // save-document-format-default
        0, 0,
        NULL, NULL
    },
    {
        817, // save-document-format-supported
        0, 0,
        NULL, NULL
    },
    {
        820, // save-location-default
        0, 0,
        NULL, NULL
    },
    {
        821, // save-location-supported
        0, 0,
        NULL, NULL
    },
    {
        823, // save-name-subdirectory-supported
        0, 0,
        NULL, NULL
    },
    {
        824, // save-name-supported
        0, 0,
        NULL, NULL
    },
    {
        827, // separator-sheets-default
        0, 0,
        NULL, NULL
    },
    {
        828, // separator-sheets-supported
        0, 0,
        NULL, NULL
    },
    {
        832, // sheet-collate-default
        0, 0,
        NULL, NULL
    },
    {
        833, // sheet-collate-supported
        0, 0,
        NULL, NULL
    },
    {
        838, // sides-default
        0, 0,
        NULL, NULL
    },
    {
        839, // sides-supported
        0, 0,
        NULL, NULL
    },
    {
        843, // stitching-angle-supported
        0, 0,
        NULL, NULL
    },
    {
        845, // stitching-locations-supported
        0, 0,
        NULL, NULL
    },
    {
        847, // stitching-method-supported
        0, 0,
        NULL, NULL
    },
    {
        849, // stitching-offset-supported
        0, 0,
        NULL, NULL
    },
    {
        851, // stitching-reference-edge-supported
        0, 0,
        NULL, NULL
    },
    {
        853, // subject-supported
        0, 0,
        NULL, NULL
    },
    {
        854, // subordinate-printers-supported
        0, 0,
        NULL, NULL
    },
    {
        855, // subscription-privacy-attributes
        0, 0,
        NULL, NULL
    },
    {
        856, // subscription-privacy-scope
        0, 0,
        NULL, NULL
    },
    {
        862, // to-name-supported
        0, 0,
        NULL, NULL
    },
    {
        866, // trimming-offset-supported
        0, 0,
        NULL, NULL
    },
    {
        868, // trimming-reference-edge-supported
        0, 0,
        NULL, NULL
    },
    {
        870, // trimming-type-supported
        0, 0,
        NULL, NULL
    },
    {
        872, // trimming-when-supported
        0, 0,
        NULL, NULL
    },
    {
        873, // uri-authentication-supported
        0, 0,
        NULL, NULL
    },
    {
        874, // uri-security-supported
        0, 0,
        NULL, NULL
    },
    {
        875, // user-defined-values-supported
        0, 0,
        NULL, NULL
    },
    {
        878, // which-jobs-supported
        0, 0,
        NULL, NULL
    },
    {
        883, // x-image-position-default
        0, 0,
        NULL, NULL
    },
    {
        884, // x-image-position-supported
        0, 0,
        NULL, NULL
    },
    {
        887, // x-image-shift-default
        0, 0,
        NULL, NULL
    },
    {
        888, // x-image-shift-supported
        0, 0,
        NULL, NULL
    },
    {
        893, // x-side1-image-shift-default
        0, 0,
        NULL, NULL
    },
    {
        894, // x-side1-image-shift-supported
        0, 0,
        NULL, NULL
    },
    {
        897, // x-side2-image-shift-default
        0, 0,
        NULL, NULL
    },
    {
        898, // x-side2-image-shift-supported
        0, 0,
        NULL, NULL
    },
    {
        909, // y-image-position-default
        0, 0,
        NULL, NULL
    },
    {
        910, // y-image-position-supported
        0, 0,
        NULL, NULL
    },
    {
        913, // y-image-shift-default
        0, 0,
        NULL, NULL
    },
    {
        914, // y-image-shift-supported
        0, 0,
        NULL, NULL
    },
    {
        919, // y-side1-image-shift-default
        0, 0,
        NULL, NULL
    },
    {
        920, // y-side1-image-shift-supported
        0, 0,
        NULL, NULL
    },
    {
        923, // y-side2-image-shift-default
        0, 0,
        NULL, NULL
    },
    {
        924, // y-side2-image-shift-supported
        0, 0,
        NULL, NULL
    },
};

ipp_attr_t s_ipp_attr_group_prtstat[] =
{
    {
        101, // device-service-count
        0, 0,
        NULL, NULL
    },
    {
        102, // device-uuid
        0, 0,
        NULL, NULL
    },
    {
        132, // document-format-varying-attributes
        0, 0,
        NULL, NULL
    },
    {
        399, // job-settable-attributes-supported
        0, 0,
        NULL, NULL
    },
    {
        644, // pages-per-minute
        0, 0,
        NULL, NULL
    },
    {
        645, // pages-per-minute-color
        0, 0,
        NULL, NULL
    },
    {
        716, // printer-alert
        0, 0,
        NULL, NULL
    },
    {
        717, // printer-alert-description
        0, 0,
        NULL, NULL
    },
    {
        721, // printer-config-change-date-time
        0, 0,
        NULL, NULL
    },
    {
        722, // printer-config-change-time
        0, 0,
        NULL, NULL
    },
    {
        724, // printer-detailed-status-messages
        0, 0,
        NULL, NULL
    },
    {
        732, // printer-finisher
        0, 0,
        NULL, NULL
    },
    {
        733, // printer-finisher-description
        0, 0,
        NULL, NULL
    },
    {
        734, // printer-finisher-supplies
        0, 0,
        NULL, NULL
    },
    {
        735, // printer-finisher-supplies-description
        0, 0,
        NULL, NULL
    },
    {
        741, // printer-input-tray
        0, 0,
        NULL, NULL
    },
    {
        742, // printer-is-accepting-jobs
        0, 0,
        NULL, NULL
    },
    {
        747, // printer-message-date-time
        0, 0,
        NULL, NULL
    },
    {
        748, // printer-message-from-operator
        0, 0,
        NULL, NULL
    },
    {
        749, // printer-message-time
        0, 0,
        NULL, NULL
    },
    {
        750, // printer-more-info
        0, 0,
        NULL, NULL
    },
    {
        755, // printer-output-tray
        0, 0,
        NULL, NULL
    },
    {
        761, // printer-settable-attributes-supported
        0, 0,
        NULL, NULL
    },
    {
        762, // printer-state
        0, 0,
        NULL, NULL
    },
    {
        763, // printer-state-change-date-time
        0, 0,
        NULL, NULL
    },
    {
        764, // printer-state-change-time
        0, 0,
        NULL, NULL
    },
    {
        765, // printer-state-message
        0, 0,
        NULL, NULL
    },
    {
        766, // printer-state-reasons
        0, 0,
        NULL, NULL
    },
    {
        768, // printer-static-resource-k-octets-free
        0, 0,
        NULL, NULL
    },
    {
        772, // printer-supply
        0, 0,
        NULL, NULL
    },
    {
        773, // printer-supply-description
        0, 0,
        NULL, NULL
    },
    {
        774, // printer-supply-info-uri
        0, 0,
        NULL, NULL
    },
    {
        775, // printer-up-time
        0, 0,
        NULL, NULL
    },
    {
        777, // printer-uri-supported
        0, 0,
        NULL, NULL
    },
    {
        778, // printer-uuid
        0, 0,
        NULL, NULL
    },
    {
        799, // queued-job-count
        0, 0,
        NULL, NULL
    },
    {
        900, // xri-authentication-supported
        0, 0,
        NULL, NULL
    },
    {
        902, // xri-security-supported
        0, 0,
        NULL, NULL
    },
    {
        904, // xri-uri-scheme-supported
        0, 0,
        NULL, NULL
    },
};

ipp_attr_t s_ipp_attr_group_substat[] =
{
    {
        555, // notify-job-id
        0, 0,
        NULL, NULL
    },
    {
        559, // notify-lease-expiration-time
        0, 0,
        NULL, NULL
    },
    {
        562, // notify-printer-up-time
        0, 0,
        NULL, NULL
    },
    {
        563, // notify-printer-uri
        0, 0,
        NULL, NULL
    },
    {
        568, // notify-sequence-number
        0, 0,
        NULL, NULL
    },
    {
        570, // notify-status-code
        0, 0,
        NULL, NULL
    },
    {
        572, // notify-subscriber-user-name
        0, 0,
        NULL, NULL
    },
    {
        573, // notify-subscriber-user-uri
        0, 0,
        NULL, NULL
    },
    {
        574, // notify-subscription-id
        0, 0,
        NULL, NULL
    },
    {
        576, // notify-subscription-uuid
        0, 0,
        NULL, NULL
    },
};

ipp_attr_t s_ipp_attr_group_subtemp[] =
{
    {
        548, // notify-attributes
        0, 0,
        NULL, NULL
    },
    {
        550, // notify-charset
        0, 0,
        NULL, NULL
    },
    {
        551, // notify-events
        0, 0,
        NULL, NULL
    },
    {
        556, // notify-lease-duration
        0, 0,
        NULL, NULL
    },
    {
        560, // notify-max-events-supported
        0, 0,
        NULL, NULL
    },
    {
        561, // notify-natural-language
        0, 0,
        NULL, NULL
    },
    {
        564, // notify-pull-method
        0, 0,
        NULL, NULL
    },
    {
        566, // notify-recipient-uri
        0, 0,
        NULL, NULL
    },
    {
        578, // notify-time-interval
        0, 0,
        NULL, NULL
    },
    {
        579, // notify-user-data
        0, 0,
        NULL, NULL
    },
};

ipp_group_xref_t s_ipp_group_xref[] =
{
    { IPP_GROUPING_DOCUMENT_DESCRIPTION, s_ipp_attr_group_decdesc, (sizeof(s_ipp_attr_group_decdesc)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_DOCUMENT_STATUS, s_ipp_attr_group_docstat, (sizeof(s_ipp_attr_group_docstat)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_DOCUMENT_TEMPLATE, s_ipp_attr_group_doctemp, (sizeof(s_ipp_attr_group_doctemp)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_EVENT_NOTIFICATIONS, s_ipp_attr_group_evtnot, (sizeof(s_ipp_attr_group_evtnot)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_JOB_DESCRIPTION, s_ipp_attr_group_jodesc, (sizeof(s_ipp_attr_group_jodesc)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_JOB_STATUS, s_ipp_attr_group_jobstat, (sizeof(s_ipp_attr_group_jobstat)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_JOB_TEMPLATE, s_ipp_attr_group_jobtemp, (sizeof(s_ipp_attr_group_jobtemp)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_OPERATION, s_ipp_attr_group_operation, (sizeof(s_ipp_attr_group_operation)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_PRINTER_DESCRIPTION, s_ipp_attr_group_prtdesc, (sizeof(s_ipp_attr_group_prtdesc)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_PRINTER_STATUS, s_ipp_attr_group_prtstat, (sizeof(s_ipp_attr_group_prtstat)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_SUBSCRIPTION_STATUS, s_ipp_attr_group_substat, (sizeof(s_ipp_attr_group_substat)/sizeof(ipp_attr_t)) },
    { IPP_GROUPING_SUBSCRIPTION_TEMPLATE, s_ipp_attr_group_subtemp, (sizeof(s_ipp_attr_group_subtemp)/sizeof(ipp_attr_t)) },
};

