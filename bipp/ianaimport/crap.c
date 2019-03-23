/*
 * Generated File -- 
 *
 * Collection,Name,Member Attribute,Sub-member Attribute,Syntax,Reference

*/

ipp_attr_rec_t s_ipp_col_cover_back[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_cover_back_actual[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_cover_back_default[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_cover_front[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_cover_front_actual[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_cover_front_default[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_cover_sheet_info[] = 
{
    {   // index 0
        "from-name",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "logo",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "organization-name",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 4
        "subject",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 5
        "to-name",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_cover_sheet_info_default[] = 
{
    {   // index 0
        "*cover-sheet-info",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_destination_accesses[] = 
{
    {   // index 0
        "access-oauth-token",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "access-oauth-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "access-password",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "access-pin",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 4
        "access-user-name",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 5
        "access-x509-certificate",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 6
        "access-x509-certificate(obsolete)",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_destination_accesses(extension)[] = 
{
};

ipp_attr_rec_t s_ipp_col_destination_statuses[] = 
{
    {   // index 0
        "destination-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "images-completed",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "transmission-status",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_destination_uri_ready[] = 
{
    {   // index 0
        "destination-attributes",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "destination-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "destination-info",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "destination-is-directory",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 4
        "destination-mandatory-access-attributes",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 5
        "destination-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 6
        "destination-oauth-scope",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 7
        "destination-oauth-token",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 8
        "destination-oauth-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 9
        "destination-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_destination_uris[] = 
{
    {   // index 0
        "destination-attributes",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "destination-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "post-dial-string",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "pre-dial-string",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 4
        "t33-subaddress",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_document_access[] = 
{
    {   // index 0
        "access-oauth-token",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "access-oauth-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "access-password",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "access-pin",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 4
        "access-user-name",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 5
        "access-x509-certificate",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 6
        "access-x509-certificate(obsolete)",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_document_access(extension)[] = 
{
};

ipp_attr_rec_t s_ipp_col_document_format_details[] = 
{
    {   // index 0
        "*document-format-details",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_document_format_details_default[] = 
{
    {   // index 0
        "*document-format-details",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_document_format_details_detected[] = 
{
    {   // index 0
        "*document-format-details",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_document_format_details_supplied[] = 
{
    {   // index 0
        "*document-format-details",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_finishings_col[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_finishings_col(extension)[] = 
{
};

ipp_attr_rec_t s_ipp_col_finishings_col_actual[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_finishings_col_database[] = 
{
};

ipp_attr_rec_t s_ipp_col_finishings_col_default[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_finishings_col_ready[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_impressions_col[] = 
{
    {   // index 0
        "blank",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "blank-two-sided",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "full-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "full-color-two-sided",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 4
        "highlight-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 5
        "highlight-color-two-sided",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 6
        "monochrome",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 7
        "monochrome-two-sided",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_impressions_completed_col[] = 
{
    {   // index 0
        "*impressions-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_input_attributes[] = 
{
    {   // index 0
        "input-auto-scaling",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "input-auto-skew-correction",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "input-brightness",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "input-color-mode",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 4
        "input-content-type",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 5
        "input-contrast",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 6
        "input-film-scan-mode",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 7
        "input-images-to-transfer",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 8
        "input-media",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 9
        "input-orientation-requested",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 10
        "input-quality",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 11
        "input-resolution",   
        { IPP_RESOLUTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 12
        "input-scaling-height",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 13
        "input-scaling-width",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 14
        "input-scan-regions",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 15
        "input-sharpness",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 16
        "input-sides",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 17
        "input-source",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_input_attributes_actual[] = 
{
};

ipp_attr_rec_t s_ipp_col_input_attributes_default[] = 
{
};

ipp_attr_rec_t s_ipp_col_input_scan_regions_supported[] = 
{
    {   // index 0
        "x-dimension",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "x-origin",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "y-dimension",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "y-origin",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_insert_sheet[] = 
{
    {   // index 0
        "*insert-sheet",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_insert_sheet_actual[] = 
{
    {   // index 0
        "*insert-sheet",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_insert_sheet_default[] = 
{
    {   // index 0
        "*insert-sheet",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_accounting_sheets[] = 
{
    {   // index 0
        "job-accounting-output-bin",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "job-accounting-sheets-type",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "media",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "media-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_accounting_sheets_actual[] = 
{
    {   // index 0
        "*job-accounting-sheets",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_accounting_sheets_default[] = 
{
    {   // index 0
        "*job-accounting-sheets",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_constraints_supported[] = 
{
    {   // index 0
        "resolver-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_cover_back[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_cover_back_actual[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_cover_back_default[] = 
{
    {   // index 0
        "*cover-back",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_cover_front[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_cover_front_actual[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_cover_front_default[] = 
{
    {   // index 0
        "*cover-front",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_error_sheet[] = 
{
    {   // index 0
        "job-error-sheet-type",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "job-error-sheet-when",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "media",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "media-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_error_sheet_actual[] = 
{
    {   // index 0
        "*job-error-sheet",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_error_sheet_default[] = 
{
    {   // index 0
        "*job-error-sheet",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_finishings_col[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_finishings_col_actual[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_finishings_col_default[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_finishings_col_ready[] = 
{
    {   // index 0
        "*finishings-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_impressions_col[] = 
{
    {   // index 0
        "blank",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "blank-two-sided",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "full-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "full-color-two-sided",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 4
        "highlight-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 5
        "highlight-color-two-sided",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 6
        "monochrome",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 7
        "monochrome-two-sided",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_impressions_completed_col[] = 
{
    {   // index 0
        "*job-impressions-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_media_sheets_col[] = 
{
    {   // index 0
        "blank",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "full-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "highlight-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "monochrome",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_media_sheets_completed_col[] = 
{
    {   // index 0
        "*job-media-sheets-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_pages_col[] = 
{
    {   // index 0
        "full-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "monochrome",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_pages_completed_col[] = 
{
    {   // index 0
        "*job-pages-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_presets_supported[] = 
{
    {   // index 0
        "preset-name",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_resolvers_supported[] = 
{
    {   // index 0
        "resolver-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_save_disposition[] = 
{
    {   // index 0
        "save-disposition",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "save-info",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_sheets_col[] = 
{
    {   // index 0
        "job-sheets",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "media",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "media-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_sheets_col_actual[] = 
{
    {   // index 0
        "*job-sheets-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_sheets_col_default[] = 
{
    {   // index 0
        "*job-sheets-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_job_triggers_supported[] = 
{
    {   // index 0
        "preset-name",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_materials_col[] = 
{
    {   // index 0
        "material-amount",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "material-amount-units",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "material-color",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "material-diameter",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 4
        "material-diameter-tolerance",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 5
        "material-fill-density",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 6
        "material-key",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 7
        "material-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 8
        "material-purpose",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 9
        "material-rate",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 10
        "material-rate-units",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 11
        "material-shell-thickness",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 12
        "material-temperature",   
        { IPP_INTEGER,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 13
        "material-type",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_materials_col_actual[] = 
{
};

ipp_attr_rec_t s_ipp_col_materials_col_database[] = 
{
};

ipp_attr_rec_t s_ipp_col_materials_col_default[] = 
{
};

ipp_attr_rec_t s_ipp_col_materials_col_ready[] = 
{
};

ipp_attr_rec_t s_ipp_col_media_col[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_media_col_actual[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_media_col_database[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "media-source-properties",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_media_col_default[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_media_col_ready[] = 
{
    {   // index 0
        "*media-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "media-source-properties",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_media_sheets_col[] = 
{
    {   // index 0
        "blank",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "full-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "highlight-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "monochrome",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_media_sheets_completed_col[] = 
{
    {   // index 0
        "*media-sheets-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_media_size_supported[] = 
{
    {   // index 0
        "x-dimension",   
        { IPP_INTEGER,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "y-dimension",   
        { IPP_INTEGER,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_output_attributes[] = 
{
    {   // index 0
        "noise-removal",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "output-compression-quality-factor",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_output_attributes_actual[] = 
{
};

ipp_attr_rec_t s_ipp_col_output_attributes_default[] = 
{
};

ipp_attr_rec_t s_ipp_col_overrides[] = 
{
    {   // index 0
        "<Any Job Template attribute>",   
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "document-copies",   
        { IPP_RANGEOFINT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "document-numbers",   
        { IPP_RANGEOFINT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "pages",   
        { IPP_RANGEOFINT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_overrides_actual[] = 
{
    {   // index 0
        "*overrides",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_pages_col[] = 
{
    {   // index 0
        "full-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "monochrome",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_pages_completed_col[] = 
{
    {   // index 0
        "*pages-col",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_pdl_init_file[] = 
{
    {   // index 0
        "*pdl-init-file",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_pdl_init_file_default[] = 
{
    {   // index 0
        "*pdl-init-file",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_preferred_attributes[] = 
{
};

ipp_attr_rec_t s_ipp_col_print_accuracy[] = 
{
    {   // index 0
        "accuracy-units",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "x-accuracy",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "y-accuracy",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "z-accuracy",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_print_accuracy_actual[] = 
{
};

ipp_attr_rec_t s_ipp_col_print_accuracy_supported[] = 
{
    {   // index 0
        "accuracy-units",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "x-accuracy",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "y-accuracy",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "z-accuracy",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_print_objects[] = 
{
    {   // index 0
        "document-number",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "object-offset",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "object-size",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "object-uuid",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_print_objects_actual[] = 
{
};

ipp_attr_rec_t s_ipp_col_printer_icc_profiles[] = 
{
    {   // index 0
        "profile-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "profile-url",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_printer_volume_supported[] = 
{
    {   // index 0
        "x-dimension",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "y-dimension",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "z-dimension",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_printer_xri_supported[] = 
{
    {   // index 0
        "xri-authentication",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "xri-security",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "xri-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_proof_print[] = 
{
    {   // index 0
        "media",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "media-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "proof-print-copies",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_proof_print_default[] = 
{
    {   // index 0
        "*proof-print",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_separator_sheets[] = 
{
    {   // index 0
        "*separator-sheets",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_separator_sheets_actual[] = 
{
    {   // index 0
        "*separator-sheets",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_rec_t s_ipp_col_separator_sheets_default[] = 
{
    {   // index 0
        "*separator-sheets",   // indirect reference to
        { IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

struct tag_col_xref
{
    char           *name;
    ipp_attr_rec_t *col_attr;
    size_t          num_members;
}
s_ipp_collection_xref[] =
{
    { "cover-back", s_ipp_col_cover_back, (sizeof(s_ipp_col_cover_back)/sizeof(ipp_attr_rec_t)) }
    { "cover-back-actual", s_ipp_col_cover_back_actual, (sizeof(s_ipp_col_cover_back_actual)/sizeof(ipp_attr_rec_t)) }
    { "cover-back-default", s_ipp_col_cover_back_default, (sizeof(s_ipp_col_cover_back_default)/sizeof(ipp_attr_rec_t)) }
    { "cover-front", s_ipp_col_cover_front, (sizeof(s_ipp_col_cover_front)/sizeof(ipp_attr_rec_t)) }
    { "cover-front-actual", s_ipp_col_cover_front_actual, (sizeof(s_ipp_col_cover_front_actual)/sizeof(ipp_attr_rec_t)) }
    { "cover-front-default", s_ipp_col_cover_front_default, (sizeof(s_ipp_col_cover_front_default)/sizeof(ipp_attr_rec_t)) }
    { "cover-sheet-info", s_ipp_col_cover_sheet_info, (sizeof(s_ipp_col_cover_sheet_info)/sizeof(ipp_attr_rec_t)) }
    { "cover-sheet-info-default", s_ipp_col_cover_sheet_info_default, (sizeof(s_ipp_col_cover_sheet_info_default)/sizeof(ipp_attr_rec_t)) }
    { "destination-accesses", s_ipp_col_destination_accesses, (sizeof(s_ipp_col_destination_accesses)/sizeof(ipp_attr_rec_t)) }
    { "destination-accesses(extension)", s_ipp_col_destination_accesses(extension), (sizeof(s_ipp_col_destination_accesses(extension))/sizeof(ipp_attr_rec_t)) }
    { "destination-statuses", s_ipp_col_destination_statuses, (sizeof(s_ipp_col_destination_statuses)/sizeof(ipp_attr_rec_t)) }
    { "destination-uri-ready", s_ipp_col_destination_uri_ready, (sizeof(s_ipp_col_destination_uri_ready)/sizeof(ipp_attr_rec_t)) }
    { "destination-uris", s_ipp_col_destination_uris, (sizeof(s_ipp_col_destination_uris)/sizeof(ipp_attr_rec_t)) }
    { "document-access", s_ipp_col_document_access, (sizeof(s_ipp_col_document_access)/sizeof(ipp_attr_rec_t)) }
    { "document-access(extension)", s_ipp_col_document_access(extension), (sizeof(s_ipp_col_document_access(extension))/sizeof(ipp_attr_rec_t)) }
    { "document-format-details", s_ipp_col_document_format_details, (sizeof(s_ipp_col_document_format_details)/sizeof(ipp_attr_rec_t)) }
    { "document-format-details-default", s_ipp_col_document_format_details_default, (sizeof(s_ipp_col_document_format_details_default)/sizeof(ipp_attr_rec_t)) }
    { "document-format-details-detected", s_ipp_col_document_format_details_detected, (sizeof(s_ipp_col_document_format_details_detected)/sizeof(ipp_attr_rec_t)) }
    { "document-format-details-supplied", s_ipp_col_document_format_details_supplied, (sizeof(s_ipp_col_document_format_details_supplied)/sizeof(ipp_attr_rec_t)) }
    { "finishings-col", s_ipp_col_finishings_col, (sizeof(s_ipp_col_finishings_col)/sizeof(ipp_attr_rec_t)) }
    { "finishings-col(extension)", s_ipp_col_finishings_col(extension), (sizeof(s_ipp_col_finishings_col(extension))/sizeof(ipp_attr_rec_t)) }
    { "finishings-col-actual", s_ipp_col_finishings_col_actual, (sizeof(s_ipp_col_finishings_col_actual)/sizeof(ipp_attr_rec_t)) }
    { "finishings-col-database", s_ipp_col_finishings_col_database, (sizeof(s_ipp_col_finishings_col_database)/sizeof(ipp_attr_rec_t)) }
    { "finishings-col-default", s_ipp_col_finishings_col_default, (sizeof(s_ipp_col_finishings_col_default)/sizeof(ipp_attr_rec_t)) }
    { "finishings-col-ready", s_ipp_col_finishings_col_ready, (sizeof(s_ipp_col_finishings_col_ready)/sizeof(ipp_attr_rec_t)) }
    { "impressions-col", s_ipp_col_impressions_col, (sizeof(s_ipp_col_impressions_col)/sizeof(ipp_attr_rec_t)) }
    { "impressions-completed-col", s_ipp_col_impressions_completed_col, (sizeof(s_ipp_col_impressions_completed_col)/sizeof(ipp_attr_rec_t)) }
    { "input-attributes", s_ipp_col_input_attributes, (sizeof(s_ipp_col_input_attributes)/sizeof(ipp_attr_rec_t)) }
    { "input-attributes-actual", s_ipp_col_input_attributes_actual, (sizeof(s_ipp_col_input_attributes_actual)/sizeof(ipp_attr_rec_t)) }
    { "input-attributes-default", s_ipp_col_input_attributes_default, (sizeof(s_ipp_col_input_attributes_default)/sizeof(ipp_attr_rec_t)) }
    { "input-scan-regions-supported", s_ipp_col_input_scan_regions_supported, (sizeof(s_ipp_col_input_scan_regions_supported)/sizeof(ipp_attr_rec_t)) }
    { "insert-sheet", s_ipp_col_insert_sheet, (sizeof(s_ipp_col_insert_sheet)/sizeof(ipp_attr_rec_t)) }
    { "insert-sheet-actual", s_ipp_col_insert_sheet_actual, (sizeof(s_ipp_col_insert_sheet_actual)/sizeof(ipp_attr_rec_t)) }
    { "insert-sheet-default", s_ipp_col_insert_sheet_default, (sizeof(s_ipp_col_insert_sheet_default)/sizeof(ipp_attr_rec_t)) }
    { "job-accounting-sheets", s_ipp_col_job_accounting_sheets, (sizeof(s_ipp_col_job_accounting_sheets)/sizeof(ipp_attr_rec_t)) }
    { "job-accounting-sheets-actual", s_ipp_col_job_accounting_sheets_actual, (sizeof(s_ipp_col_job_accounting_sheets_actual)/sizeof(ipp_attr_rec_t)) }
    { "job-accounting-sheets-default", s_ipp_col_job_accounting_sheets_default, (sizeof(s_ipp_col_job_accounting_sheets_default)/sizeof(ipp_attr_rec_t)) }
    { "job-constraints-supported", s_ipp_col_job_constraints_supported, (sizeof(s_ipp_col_job_constraints_supported)/sizeof(ipp_attr_rec_t)) }
    { "job-cover-back", s_ipp_col_job_cover_back, (sizeof(s_ipp_col_job_cover_back)/sizeof(ipp_attr_rec_t)) }
    { "job-cover-back-actual", s_ipp_col_job_cover_back_actual, (sizeof(s_ipp_col_job_cover_back_actual)/sizeof(ipp_attr_rec_t)) }
    { "job-cover-back-default", s_ipp_col_job_cover_back_default, (sizeof(s_ipp_col_job_cover_back_default)/sizeof(ipp_attr_rec_t)) }
    { "job-cover-front", s_ipp_col_job_cover_front, (sizeof(s_ipp_col_job_cover_front)/sizeof(ipp_attr_rec_t)) }
    { "job-cover-front-actual", s_ipp_col_job_cover_front_actual, (sizeof(s_ipp_col_job_cover_front_actual)/sizeof(ipp_attr_rec_t)) }
    { "job-cover-front-default", s_ipp_col_job_cover_front_default, (sizeof(s_ipp_col_job_cover_front_default)/sizeof(ipp_attr_rec_t)) }
    { "job-error-sheet", s_ipp_col_job_error_sheet, (sizeof(s_ipp_col_job_error_sheet)/sizeof(ipp_attr_rec_t)) }
    { "job-error-sheet-actual", s_ipp_col_job_error_sheet_actual, (sizeof(s_ipp_col_job_error_sheet_actual)/sizeof(ipp_attr_rec_t)) }
    { "job-error-sheet-default", s_ipp_col_job_error_sheet_default, (sizeof(s_ipp_col_job_error_sheet_default)/sizeof(ipp_attr_rec_t)) }
    { "job-finishings-col", s_ipp_col_job_finishings_col, (sizeof(s_ipp_col_job_finishings_col)/sizeof(ipp_attr_rec_t)) }
    { "job-finishings-col-actual", s_ipp_col_job_finishings_col_actual, (sizeof(s_ipp_col_job_finishings_col_actual)/sizeof(ipp_attr_rec_t)) }
    { "job-finishings-col-default", s_ipp_col_job_finishings_col_default, (sizeof(s_ipp_col_job_finishings_col_default)/sizeof(ipp_attr_rec_t)) }
    { "job-finishings-col-ready", s_ipp_col_job_finishings_col_ready, (sizeof(s_ipp_col_job_finishings_col_ready)/sizeof(ipp_attr_rec_t)) }
    { "job-impressions-col", s_ipp_col_job_impressions_col, (sizeof(s_ipp_col_job_impressions_col)/sizeof(ipp_attr_rec_t)) }
    { "job-impressions-completed-col", s_ipp_col_job_impressions_completed_col, (sizeof(s_ipp_col_job_impressions_completed_col)/sizeof(ipp_attr_rec_t)) }
    { "job-media-sheets-col", s_ipp_col_job_media_sheets_col, (sizeof(s_ipp_col_job_media_sheets_col)/sizeof(ipp_attr_rec_t)) }
    { "job-media-sheets-completed-col", s_ipp_col_job_media_sheets_completed_col, (sizeof(s_ipp_col_job_media_sheets_completed_col)/sizeof(ipp_attr_rec_t)) }
    { "job-pages-col", s_ipp_col_job_pages_col, (sizeof(s_ipp_col_job_pages_col)/sizeof(ipp_attr_rec_t)) }
    { "job-pages-completed-col", s_ipp_col_job_pages_completed_col, (sizeof(s_ipp_col_job_pages_completed_col)/sizeof(ipp_attr_rec_t)) }
    { "job-presets-supported", s_ipp_col_job_presets_supported, (sizeof(s_ipp_col_job_presets_supported)/sizeof(ipp_attr_rec_t)) }
    { "job-resolvers-supported", s_ipp_col_job_resolvers_supported, (sizeof(s_ipp_col_job_resolvers_supported)/sizeof(ipp_attr_rec_t)) }
    { "job-save-disposition", s_ipp_col_job_save_disposition, (sizeof(s_ipp_col_job_save_disposition)/sizeof(ipp_attr_rec_t)) }
    { "job-sheets-col", s_ipp_col_job_sheets_col, (sizeof(s_ipp_col_job_sheets_col)/sizeof(ipp_attr_rec_t)) }
    { "job-sheets-col-actual", s_ipp_col_job_sheets_col_actual, (sizeof(s_ipp_col_job_sheets_col_actual)/sizeof(ipp_attr_rec_t)) }
    { "job-sheets-col-default", s_ipp_col_job_sheets_col_default, (sizeof(s_ipp_col_job_sheets_col_default)/sizeof(ipp_attr_rec_t)) }
    { "job-triggers-supported", s_ipp_col_job_triggers_supported, (sizeof(s_ipp_col_job_triggers_supported)/sizeof(ipp_attr_rec_t)) }
    { "materials-col", s_ipp_col_materials_col, (sizeof(s_ipp_col_materials_col)/sizeof(ipp_attr_rec_t)) }
    { "materials-col-actual", s_ipp_col_materials_col_actual, (sizeof(s_ipp_col_materials_col_actual)/sizeof(ipp_attr_rec_t)) }
    { "materials-col-database", s_ipp_col_materials_col_database, (sizeof(s_ipp_col_materials_col_database)/sizeof(ipp_attr_rec_t)) }
    { "materials-col-default", s_ipp_col_materials_col_default, (sizeof(s_ipp_col_materials_col_default)/sizeof(ipp_attr_rec_t)) }
    { "materials-col-ready", s_ipp_col_materials_col_ready, (sizeof(s_ipp_col_materials_col_ready)/sizeof(ipp_attr_rec_t)) }
    { "media-col", s_ipp_col_media_col, (sizeof(s_ipp_col_media_col)/sizeof(ipp_attr_rec_t)) }
    { "media-col-actual", s_ipp_col_media_col_actual, (sizeof(s_ipp_col_media_col_actual)/sizeof(ipp_attr_rec_t)) }
    { "media-col-database", s_ipp_col_media_col_database, (sizeof(s_ipp_col_media_col_database)/sizeof(ipp_attr_rec_t)) }
    { "media-col-default", s_ipp_col_media_col_default, (sizeof(s_ipp_col_media_col_default)/sizeof(ipp_attr_rec_t)) }
    { "media-col-ready", s_ipp_col_media_col_ready, (sizeof(s_ipp_col_media_col_ready)/sizeof(ipp_attr_rec_t)) }
    { "media-sheets-col", s_ipp_col_media_sheets_col, (sizeof(s_ipp_col_media_sheets_col)/sizeof(ipp_attr_rec_t)) }
    { "media-sheets-completed-col", s_ipp_col_media_sheets_completed_col, (sizeof(s_ipp_col_media_sheets_completed_col)/sizeof(ipp_attr_rec_t)) }
    { "media-size-supported", s_ipp_col_media_size_supported, (sizeof(s_ipp_col_media_size_supported)/sizeof(ipp_attr_rec_t)) }
    { "output-attributes", s_ipp_col_output_attributes, (sizeof(s_ipp_col_output_attributes)/sizeof(ipp_attr_rec_t)) }
    { "output-attributes-actual", s_ipp_col_output_attributes_actual, (sizeof(s_ipp_col_output_attributes_actual)/sizeof(ipp_attr_rec_t)) }
    { "output-attributes-default", s_ipp_col_output_attributes_default, (sizeof(s_ipp_col_output_attributes_default)/sizeof(ipp_attr_rec_t)) }
    { "overrides", s_ipp_col_overrides, (sizeof(s_ipp_col_overrides)/sizeof(ipp_attr_rec_t)) }
    { "overrides-actual", s_ipp_col_overrides_actual, (sizeof(s_ipp_col_overrides_actual)/sizeof(ipp_attr_rec_t)) }
    { "pages-col", s_ipp_col_pages_col, (sizeof(s_ipp_col_pages_col)/sizeof(ipp_attr_rec_t)) }
    { "pages-completed-col", s_ipp_col_pages_completed_col, (sizeof(s_ipp_col_pages_completed_col)/sizeof(ipp_attr_rec_t)) }
    { "pdl-init-file", s_ipp_col_pdl_init_file, (sizeof(s_ipp_col_pdl_init_file)/sizeof(ipp_attr_rec_t)) }
    { "pdl-init-file-default", s_ipp_col_pdl_init_file_default, (sizeof(s_ipp_col_pdl_init_file_default)/sizeof(ipp_attr_rec_t)) }
    { "preferred-attributes", s_ipp_col_preferred_attributes, (sizeof(s_ipp_col_preferred_attributes)/sizeof(ipp_attr_rec_t)) }
    { "print-accuracy", s_ipp_col_print_accuracy, (sizeof(s_ipp_col_print_accuracy)/sizeof(ipp_attr_rec_t)) }
    { "print-accuracy-actual", s_ipp_col_print_accuracy_actual, (sizeof(s_ipp_col_print_accuracy_actual)/sizeof(ipp_attr_rec_t)) }
    { "print-accuracy-supported", s_ipp_col_print_accuracy_supported, (sizeof(s_ipp_col_print_accuracy_supported)/sizeof(ipp_attr_rec_t)) }
    { "print-objects", s_ipp_col_print_objects, (sizeof(s_ipp_col_print_objects)/sizeof(ipp_attr_rec_t)) }
    { "print-objects-actual", s_ipp_col_print_objects_actual, (sizeof(s_ipp_col_print_objects_actual)/sizeof(ipp_attr_rec_t)) }
    { "printer-icc-profiles", s_ipp_col_printer_icc_profiles, (sizeof(s_ipp_col_printer_icc_profiles)/sizeof(ipp_attr_rec_t)) }
    { "printer-volume-supported", s_ipp_col_printer_volume_supported, (sizeof(s_ipp_col_printer_volume_supported)/sizeof(ipp_attr_rec_t)) }
    { "printer-xri-supported", s_ipp_col_printer_xri_supported, (sizeof(s_ipp_col_printer_xri_supported)/sizeof(ipp_attr_rec_t)) }
    { "proof-print", s_ipp_col_proof_print, (sizeof(s_ipp_col_proof_print)/sizeof(ipp_attr_rec_t)) }
    { "proof-print-default", s_ipp_col_proof_print_default, (sizeof(s_ipp_col_proof_print_default)/sizeof(ipp_attr_rec_t)) }
    { "separator-sheets", s_ipp_col_separator_sheets, (sizeof(s_ipp_col_separator_sheets)/sizeof(ipp_attr_rec_t)) }
    { "separator-sheets-actual", s_ipp_col_separator_sheets_actual, (sizeof(s_ipp_col_separator_sheets_actual)/sizeof(ipp_attr_rec_t)) }
    { "separator-sheets-default", s_ipp_col_separator_sheets_default, (sizeof(s_ipp_col_separator_sheets_default)/sizeof(ipp_attr_rec_t)) }
};

ipp_attr_rec_t s_ipp_attributes[] =
{
    {   // index 0
        "accuracy-units-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 1
        "attributes-charset",   
        { IPP_CHARSET,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 2
        "attributes-natural-language",   
        { IPP_LANGUAGE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 3
        "baling-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 4
        "baling-when-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 5
        "binding-reference-edge-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 6
        "binding-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 7
        "charge-info-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 8
        "charset-configured",   
        { IPP_CHARSET,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 9
        "charset-supported",   
        { IPP_CHARSET | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 10
        "coating-sides-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 11
        "coating-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 12
        "color-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 13
        "compression",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 14
        "compression-accepted",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 15
        "compression-supplied",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 16
        "compression-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 17
        "confirmation-sheet-print",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 18
        "confirmation-sheet-print-default",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 19
        "copies",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 20
        "copies-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 21
        "copies-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 22
        "copies-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 23
        "cover-back",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 24
        "cover-back-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 25
        "cover-back-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 26
        "cover-back-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 27
        "cover-front",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 28
        "cover-front-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 29
        "cover-front-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 30
        "cover-front-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 31
        "cover-sheet-info",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 32
        "cover-sheet-info-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 33
        "cover-sheet-info-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 34
        "covering-name-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 35
        "current-page-order",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 36
        "date-time-at-completed",   
        { IPP_DATETIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 37
        "date-time-at-creation",   
        { IPP_DATETIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 38
        "date-time-at-processing",   
        { IPP_DATETIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 39
        "destination-accesses",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 40
        "destination-accesses(extension)",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 41
        "destination-accesses-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 42
        "destination-statuses",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 43
        "destination-uri-ready",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 44
        "destination-uri-schemes-supported",   
        { IPP_URISCHEME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 45
        "destination-uris",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 46
        "destination-uris-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 47
        "detailed-status-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 48
        "detailed-status-messages",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 49
        "device-service-count",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 50
        "device-uuid",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 51
        "document-access",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 52
        "document-access(extension)",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 53
        "document-access-error",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 54
        "document-access-errors",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 55
        "document-access-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 56
        "document-charset",   
        { IPP_CHARSET,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 57
        "document-charset-default",   
        { IPP_CHARSET,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 58
        "document-charset-supplied",   
        { IPP_CHARSET,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 59
        "document-charset-supported",   
        { IPP_CHARSET | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 60
        "document-creation-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 61
        "document-data-get-interval",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 62
        "document-data-wait",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 63
        "document-digital-signature",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 64
        "document-digital-signature-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 65
        "document-digital-signature-supplied",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 66
        "document-digital-signature-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 67
        "document-format",   
        { IPP_MIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 68
        "document-format-accepted",   
        { IPP_MIME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 69
        "document-format-default",   
        { IPP_MIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 70
        "document-format-details",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 71
        "document-format-details-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 72
        "document-format-details-detected",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 73
        "document-format-details-supplied",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 74
        "document-format-details-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 75
        "document-format-detected",   
        { IPP_MIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 76
        "document-format-ready",   
        { IPP_MIME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 77
        "document-format-supplied",   
        { IPP_MIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 78
        "document-format-supported",   
        { IPP_MIME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 79
        "document-format-varying-attributes",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 80
        "document-format-version",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 81
        "document-format-version-default",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 82
        "document-format-version-detected",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 83
        "document-format-version-supplied",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 84
        "document-format-version-supported",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 85
        "document-job-id",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 86
        "document-job-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 87
        "document-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 88
        "document-message-supplied",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 89
        "document-metadata",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 90
        "document-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 91
        "document-name-supplied",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 92
        "document-natural-language",   
        { IPP_LANGUAGE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 93
        "document-natural-language-default",   
        { IPP_LANGUAGE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 94
        "document-natural-language-supplied",   
        { IPP_LANGUAGE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 95
        "document-natural-language-supported",   
        { IPP_LANGUAGE | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 96
        "document-number",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 97
        "document-password",   
        { IPP_OCTETSTRING,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 98
        "document-password-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 99
        "document-preprocessed",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 100
        "document-printer-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 101
        "document-privacy-attributes",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 102
        "document-privacy-scope",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 103
        "document-state",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 104
        "document-state-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 105
        "document-state-reasons",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 106
        "document-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 107
        "document-uuid",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 108
        "errors-count",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 109
        "feed-orientation",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 110
        "feed-orientation-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 111
        "feed-orientation-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 112
        "fetch-document-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 113
        "fetch-status-code",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 114
        "fetch-status-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 115
        "finishing-template-supported",   
        { IPP_NAME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 116
        "finishing-template-supported(extension)",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 117
        "finishings",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 118
        "finishings-actual",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 119
        "finishings-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 120
        "finishings-col(extension)",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 121
        "finishings-col-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 122
        "finishings-col-database",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 123
        "finishings-col-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 124
        "finishings-col-ready",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 125
        "finishings-col-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 126
        "finishings-default",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 127
        "finishings-ready",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 128
        "finishings-supported",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 129
        "first-index",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 130
        "folding-direction-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 131
        "folding-offset-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 132
        "folding-offset-supported(extension)",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 133
        "folding-reference-edge-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 134
        "font-name-requested",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 135
        "font-name-requested-default",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 136
        "font-name-requested-supported",   
        { IPP_NAME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 137
        "font-size-requested",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 138
        "font-size-requested-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 139
        "font-size-requested-supported",   
        { IPP_RANGEOFINT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 140
        "force-front-side",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 141
        "force-front-side-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 142
        "force-front-side-default (under review)",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 143
        "force-front-side-supported (under review)",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 144
        "from-name-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 145
        "generated-natural-language-supported",   
        { IPP_LANGUAGE | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 146
        "identify-actions",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 147
        "identify-actions-default",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 148
        "identify-actions-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 149
        "imposition-template",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 150
        "imposition-template-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 151
        "imposition-template-default",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 152
        "imposition-template-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 153
        "impressions",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 154
        "impressions-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 155
        "impressions-completed",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 156
        "impressions-completed-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 157
        "impressions-completed-current-copy",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 158
        "impressions-completed-current-copy(deprecated)",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 159
        "input-attributes",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 160
        "input-attributes-actual",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 161
        "input-attributes-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 162
        "input-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 163
        "input-color-mode-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 164
        "input-content-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 165
        "input-film-scan-mode-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 166
        "input-media-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 167
        "input-orientation-requested-supported",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 168
        "input-quality-supported",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 169
        "input-resolution-supported",   
        { IPP_RESOLUTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 170
        "input-scan-regions-supported",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 171
        "input-sides-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 172
        "input-source-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 173
        "insert-after-page-number-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 174
        "insert-count-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 175
        "insert-sheet",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 176
        "insert-sheet-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 177
        "insert-sheet-default",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 178
        "insert-sheet-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 179
        "ipp-attribute-fidelity",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 180
        "ipp-features-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 181
        "ipp-versions-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 182
        "ippget-event-life",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 183
        "job-account-id",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 184
        "job-account-id-actual",   
        { IPP_NAME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 185
        "job-account-id-default",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 186
        "job-account-id-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 187
        "job-account-type",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 188
        "job-account-type-actual",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 189
        "job-account-type-default",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 190
        "job-account-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 191
        "job-accounting-sheets",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 192
        "job-accounting-sheets-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 193
        "job-accounting-sheets-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 194
        "job-accounting-sheets-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 195
        "job-accounting-user-id",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 196
        "job-accounting-user-id-actual",   
        { IPP_NAME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 197
        "job-accounting-user-id-default",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 198
        "job-accounting-user-id-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 199
        "job-attribute-fidelity",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 200
        "job-authorization-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 201
        "job-authorization-uri-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 202
        "job-charge-info",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 203
        "job-collation-type",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 204
        "job-collation-type(deprecated)",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 205
        "job-collation-type-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 206
        "job-constraints-supported",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 207
        "job-copies",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 208
        "job-copies-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 209
        "job-copies-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 210
        "job-copies-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 211
        "job-cover-back",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 212
        "job-cover-back-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 213
        "job-cover-back-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 214
        "job-cover-back-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 215
        "job-cover-front",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 216
        "job-cover-front-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 217
        "job-cover-front-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 218
        "job-cover-front-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 219
        "job-creation-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 220
        "job-delay-output-until",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 221
        "job-delay-output-until-default",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 222
        "job-delay-output-until-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 223
        "job-delay-output-until-time",   
        { IPP_DATETIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 224
        "job-delay-output-until-time-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 225
        "job-destination-spooling-supported",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 226
        "job-detailed-status-messages",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 227
        "job-document-access-errors",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 228
        "job-error-action",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 229
        "job-error-action-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 230
        "job-error-action-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 231
        "job-error-sheet",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 232
        "job-error-sheet-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 233
        "job-error-sheet-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 234
        "job-error-sheet-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 235
        "job-finishings",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 236
        "job-finishings-actual",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 237
        "job-finishings-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 238
        "job-finishings-col-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 239
        "job-finishings-col-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 240
        "job-finishings-col-ready",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 241
        "job-finishings-col-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 242
        "job-finishings-default",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 243
        "job-finishings-ready",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 244
        "job-finishings-supported",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 245
        "job-hold-until",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 246
        "job-hold-until-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 247
        "job-hold-until-default",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 248
        "job-hold-until-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 249
        "job-hold-until-time",   
        { IPP_DATETIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 250
        "job-hold-until-time-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 251
        "job-id",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 252
        "job-ids",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 253
        "job-ids-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 254
        "job-impressions",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 255
        "job-impressions-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 256
        "job-impressions-completed",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 257
        "job-impressions-completed-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 258
        "job-impressions-estimated",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 259
        "job-impressions-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 260
        "job-k-octets",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 261
        "job-k-octets-processed",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 262
        "job-k-octets-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 263
        "job-mandatory-attributes",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 264
        "job-media-sheets",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 265
        "job-media-sheets-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 266
        "job-media-sheets-completed",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 267
        "job-media-sheets-completed-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 268
        "job-media-sheets-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 269
        "job-message-from-operator",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 270
        "job-message-to-operator",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 271
        "job-message-to-operator-actual",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 272
        "job-message-to-operator-default",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 273
        "job-message-to-operator-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 274
        "job-more-info",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 275
        "job-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 276
        "job-originating-user-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 277
        "job-originating-user-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 278
        "job-pages",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 279
        "job-pages-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 280
        "job-pages-completed",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 281
        "job-pages-completed-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 282
        "job-pages-completed-current-copy",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 283
        "job-pages-per-set",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 284
        "job-pages-per-set-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 285
        "job-password",   
        { IPP_OCTETSTRING,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 286
        "job-password-encryption",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 287
        "job-password-encryption-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 288
        "job-password-length-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 289
        "job-password-repertoire-configured",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 290
        "job-password-repertoire-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 291
        "job-password-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 292
        "job-phone-number",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 293
        "job-phone-number-default",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 294
        "job-phone-number-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 295
        "job-presets-supported",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 296
        "job-printer-up-time",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 297
        "job-printer-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 298
        "job-priority",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 299
        "job-priority-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 300
        "job-priority-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 301
        "job-priority-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 302
        "job-privacy-attributes",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 303
        "job-privacy-scope",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 304
        "job-recipient-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 305
        "job-recipient-name-default",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 306
        "job-recipient-name-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 307
        "job-resolvers-supported",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 308
        "job-save-disposition",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 309
        "job-save-printer-make-and-model",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 310
        "job-settable-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 311
        "job-sheet-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 312
        "job-sheet-message-actual",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 313
        "job-sheet-message-default",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 314
        "job-sheet-message-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 315
        "job-sheets",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 316
        "job-sheets-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 317
        "job-sheets-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 318
        "job-sheets-col-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 319
        "job-sheets-col-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 320
        "job-sheets-col-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 321
        "job-sheets-default",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 322
        "job-sheets-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 323
        "job-spooling-supported",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 324
        "job-state",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 325
        "job-state-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 326
        "job-state-reasons",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 327
        "job-triggers-supported",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 328
        "job-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 329
        "job-uuid",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 330
        "jpeg-features-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 331
        "jpeg-k-octets-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 332
        "jpeg-x-dimension-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 333
        "jpeg-y-dimension-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 334
        "k-octets",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 335
        "k-octets-processed",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 336
        "laminating-sides-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 337
        "laminating-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 338
        "last-document",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 339
        "limit",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 340
        "logo-uri-formats-supported",   
        { IPP_MIME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 341
        "logo-uri-schemes-supported",   
        { IPP_URISCHEME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 342
        "material-amount-units-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 343
        "material-diameter-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 344
        "material-purpose-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 345
        "material-rate-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 346
        "material-rate-units-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 347
        "material-shell-thickness-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 348
        "material-temperature-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 349
        "material-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 350
        "materials-col",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 351
        "materials-col-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 352
        "materials-col-database",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 353
        "materials-col-default",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 354
        "materials-col-ready",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 355
        "materials-col-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 356
        "max-materials-col-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 357
        "max-save-info-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 358
        "max-stitching-locations-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 359
        "media",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 360
        "media-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 361
        "media-back-coating-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 362
        "media-bottom-margin-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 363
        "media-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 364
        "media-col-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 365
        "media-col-database",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 366
        "media-col-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 367
        "media-col-ready",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 368
        "media-col-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 369
        "media-color-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 370
        "media-default",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 371
        "media-front-coating-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 372
        "media-grain-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 373
        "media-hole-count-supported",   
        { IPP_RANGEOFINT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 374
        "media-info-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 375
        "media-input-tray-check",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 376
        "media-input-tray-check-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 377
        "media-key-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 378
        "media-left-margin-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 379
        "media-order-count-supported",   
        { IPP_RANGEOFINT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 380
        "media-pre-printed-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 381
        "media-ready",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 382
        "media-recycled-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 383
        "media-right-margin-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 384
        "media-sheets",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 385
        "media-sheets-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 386
        "media-sheets-completed",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 387
        "media-sheets-completed-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 388
        "media-size-supported",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 389
        "media-source-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 390
        "media-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 391
        "media-thickness-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 392
        "media-tooth-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 393
        "media-top-margin-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 394
        "media-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 395
        "media-weight-metric-supported",   
        { IPP_RANGEOFINT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 396
        "message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 397
        "message-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 398
        "more-info",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 399
        "multiple-destination-uris-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 400
        "multiple-document-handling",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 401
        "multiple-document-handling-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 402
        "multiple-document-handling-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 403
        "multiple-document-handling-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 404
        "multiple-document-jobs-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 405
        "multiple-object-handling",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 406
        "multiple-object-handling-actual",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 407
        "multiple-object-handling-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 408
        "multiple-object-handling-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 409
        "multiple-operation-time-out",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 410
        "multiple-operation-time-out-action",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 411
        "my-jobs",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 412
        "natural-language-configured",   
        { IPP_LANGUAGE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 413
        "notify-attributes",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 414
        "notify-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 415
        "notify-charset",   
        { IPP_CHARSET,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 416
        "notify-events",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 417
        "notify-events-default",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 418
        "notify-events-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 419
        "notify-get-interval",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 420
        "notify-job-id",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 421
        "notify-lease-duration",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 422
        "notify-lease-duration-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 423
        "notify-lease-duration-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 424
        "notify-lease-expiration-time",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 425
        "notify-max-events-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 426
        "notify-natural-language",   
        { IPP_LANGUAGE,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 427
        "notify-printer-up-time",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 428
        "notify-printer-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 429
        "notify-pull-method",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 430
        "notify-pull-method-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 431
        "notify-recipient-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 432
        "notify-schemes-supported",   
        { IPP_URISCHEME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 433
        "notify-sequence-number",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 434
        "notify-sequence-numbers",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 435
        "notify-status-code",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 436
        "notify-subscribed-event",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 437
        "notify-subscriber-user-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 438
        "notify-subscriber-user-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 439
        "notify-subscription-id",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 440
        "notify-subscription-ids",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 441
        "notify-subscription-uuid",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 442
        "notify-text",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 443
        "notify-time-interval",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 444
        "notify-user-data",   
        { IPP_OCTETSTRING,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 445
        "notify-wait",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 446
        "number-of-documents",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 447
        "number-of-intervening-jobs",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 448
        "number-of-retries",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 449
        "number-of-retries-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 450
        "number-of-retries-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 451
        "number-up",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 452
        "number-up-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 453
        "number-up-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 454
        "number-up-supported",   
        { IPP_INTEGER,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 455
        "oauth-authorization-server-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 456
        "operations-supported",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 457
        "organization-name-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 458
        "orientation-requested",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 459
        "orientation-requested-actual",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 460
        "orientation-requested-default",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 461
        "orientation-requested-supported",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 462
        "original-requesting-user-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 463
        "output-attributes",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 464
        "output-attributes-actual",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 465
        "output-attributes-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 466
        "output-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 467
        "output-bin",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 468
        "output-bin-actual",   
        { IPP_NAME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 469
        "output-bin-default",   
        { IPP_KEYWORD,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 470
        "output-bin-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 471
        "output-device",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 472
        "output-device-actual",   
        { IPP_NAME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 473
        "output-device-assigned",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 474
        "output-device-document-state",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 475
        "output-device-document-state-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 476
        "output-device-document-state-reasons",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 477
        "output-device-job-state",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 478
        "output-device-job-state-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 479
        "output-device-job-state-reasons",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 480
        "output-device-job-states",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 481
        "output-device-supported",   
        { IPP_NAME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 482
        "output-device-uuid",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 483
        "output-device-uuid-assigned",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 484
        "output-device-uuid-supported",   
        { IPP_URI | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 485
        "overrides",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 486
        "overrides-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 487
        "overrides-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 488
        "page-delivery",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 489
        "page-delivery-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 490
        "page-delivery-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 491
        "page-delivery-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 492
        "page-order-received",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 493
        "page-order-received-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 494
        "page-order-received-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 495
        "page-order-received-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 496
        "page-ranges",   
        { IPP_RANGEOFINT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 497
        "page-ranges-actual",   
        { IPP_RANGEOFINT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 498
        "page-ranges-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 499
        "pages",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 500
        "pages-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 501
        "pages-completed",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 502
        "pages-completed-col",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 503
        "pages-completed-current-copy",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 504
        "pages-per-minute",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 505
        "pages-per-minute-color",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 506
        "pages-per-subset",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 507
        "pages-per-subset-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 508
        "parent-printers-supported",   
        { IPP_URI | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 509
        "pclm-raster-back-side",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 510
        "pclm-source-resolution",   
        { IPP_RESOLUTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 511
        "pclm-source-resolution-supported",   
        { IPP_RESOLUTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 512
        "pclm-strip-height-preferred",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 513
        "pclm-strip-height-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 514
        "pdf-features-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 515
        "pdf-k-octets-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 516
        "pdf-versions-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 517
        "pdl-init-file",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 518
        "pdl-init-file-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 519
        "pdl-init-file-entry-supported",   
        { IPP_NAME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 520
        "pdl-init-file-location-supported",   
        { IPP_URI | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 521
        "pdl-init-file-name-subdirectory-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 522
        "pdl-init-file-name-supported",   
        { IPP_NAME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 523
        "pdl-init-file-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 524
        "pdl-override-guaranteed-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 525
        "pdl-override-supported",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 526
        "platform-shape",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 527
        "platform-temperature",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 528
        "platform-temperature-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 529
        "platform-temperature-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 530
        "platform-temperature-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 531
        "preferred-attributes",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 532
        "preferred-attributes-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 533
        "presentation-direction-number-up",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 534
        "presentation-direction-number-up-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 535
        "presentation-direction-number-up-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 536
        "presentation-direction-number-up-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 537
        "print-accuracy",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 538
        "print-accuracy-actual",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 539
        "print-accuracy-supported",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 540
        "print-base",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 541
        "print-base-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 542
        "print-base-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 543
        "print-base-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 544
        "print-color-mode",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 545
        "print-color-mode-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 546
        "print-color-mode-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 547
        "print-color-mode-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 548
        "print-content-optimize",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 549
        "print-content-optimize-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 550
        "print-content-optimize-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 551
        "print-content-optimize-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 552
        "print-objects",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 553
        "print-objects-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 554
        "print-objects-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 555
        "print-quality",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 556
        "print-quality-actual",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 557
        "print-quality-default",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 558
        "print-quality-supported",   
        { IPP_ENUM | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 559
        "print-rendering-intent",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 560
        "print-rendering-intent-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 561
        "print-rendering-intent-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 562
        "print-rendering-intent-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 563
        "print-scaling",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 564
        "print-scaling-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 565
        "print-scaling-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 566
        "print-supports",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 567
        "print-supports-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 568
        "print-supports-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 569
        "print-supports-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 570
        "printer-alert",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 571
        "printer-alert-description",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 572
        "printer-camera-image-uri",   
        { IPP_URI | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 573
        "printer-charge-info",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 574
        "printer-charge-info-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 575
        "printer-config-change-date-time",   
        { IPP_DATETIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 576
        "printer-config-change-time",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 577
        "printer-current-time",   
        { IPP_DATETIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 578
        "printer-detailed-status-messages",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 579
        "printer-device-id",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 580
        "printer-dns-sd-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 581
        "printer-driver-installer",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 582
        "printer-fax-log-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 583
        "printer-fax-modem-info",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 584
        "printer-fax-modem-name",   
        { IPP_NAME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 585
        "printer-fax-modem-number",   
        { IPP_URI | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 586
        "printer-finisher",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 587
        "printer-finisher-description",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 588
        "printer-finisher-supplies",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 589
        "printer-finisher-supplies-description",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 590
        "printer-geo-location",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 591
        "printer-get-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 592
        "printer-icc-profiles",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 593
        "printer-icons",   
        { IPP_URI | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 594
        "printer-info",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 595
        "printer-input-tray",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 596
        "printer-is-accepting-jobs",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 597
        "printer-kind",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 598
        "printer-location",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 599
        "printer-make-and-model",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 600
        "printer-mandatory-job-attributes",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 601
        "printer-message-date-time",   
        { IPP_DATETIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 602
        "printer-message-from-operator",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 603
        "printer-message-time",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 604
        "printer-more-info",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 605
        "printer-more-info-manufacturer",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 606
        "printer-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 607
        "printer-organization",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 608
        "printer-organizational-unit",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 609
        "printer-output-tray",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 610
        "printer-privacy-policy-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 611
        "printer-resolution",   
        { IPP_RESOLUTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 612
        "printer-resolution-actual",   
        { IPP_RESOLUTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 613
        "printer-resolution-default",   
        { IPP_RESOLUTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 614
        "printer-resolution-supported",   
        { IPP_RESOLUTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 615
        "printer-settable-attributes-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 616
        "printer-state",   
        { IPP_ENUM,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 617
        "printer-state-change-date-time",   
        { IPP_DATETIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 618
        "printer-state-change-time",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 619
        "printer-state-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 620
        "printer-state-reasons",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 621
        "printer-static-resource-directory-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 622
        "printer-static-resource-k-octets-free",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 623
        "printer-static-resource-k-octets-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 624
        "printer-strings-languages-supported",   
        { IPP_LANGUAGE | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 625
        "printer-strings-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 626
        "printer-supply",   
        { IPP_OCTETSTRING | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 627
        "printer-supply-description",   
        { IPP_TEXT | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 628
        "printer-supply-info-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 629
        "printer-up-time",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 630
        "printer-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 631
        "printer-uri-supported",   
        { IPP_URI | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 632
        "printer-uuid",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 633
        "printer-volume-supported",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 634
        "printer-xri-supported",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 635
        "profile-uri-actual",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 636
        "proof-print",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 637
        "proof-print-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 638
        "proof-print-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 639
        "punching-hole-diameter-configured",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 640
        "punching-locations-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 641
        "punching-offset-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 642
        "punching-reference-edge-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 643
        "pwg-raster-document-resolution-supported",   
        { IPP_RESOLUTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 644
        "pwg-raster-document-sheet-back",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 645
        "pwg-raster-document-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 646
        "queued-job-count",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 647
        "reference-uri-schemes-supported",   
        { IPP_URISCHEME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 648
        "repertoire-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NAME,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 649
        "requested-attributes",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 650
        "requesting-user-name",   
        { IPP_NAME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 651
        "requesting-user-uri",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 652
        "requesting-user-uri-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 653
        "retry-interval",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 654
        "retry-interval-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 655
        "retry-interval-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 656
        "retry-time-out",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 657
        "retry-time-out-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 658
        "retry-time-out-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 659
        "save-disposition-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 660
        "save-document-format-default",   
        { IPP_MIME,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 661
        "save-document-format-supported",   
        { IPP_MIME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 662
        "save-location-default",   
        { IPP_URI,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 663
        "save-location-supported",   
        { IPP_URI | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 664
        "save-name-subdirectory-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 665
        "save-name-supported",   
        { IPP_BOOLEAN,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 666
        "separator-sheets",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 667
        "separator-sheets-actual",   
        { IPP_COLLECTION | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 668
        "separator-sheets-default",   
        { IPP_COLLECTION,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 669
        "separator-sheets-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 670
        "sheet-collate",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 671
        "sheet-collate(deprecated)",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 672
        "sheet-collate-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 673
        "sheet-collate-actual(deprecated)",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 674
        "sheet-collate-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 675
        "sheet-collate-default(deprecated)",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 676
        "sheet-collate-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 677
        "sheet-collate-supported(deprecated)",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 678
        "sheet-completed-copy-number",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 679
        "sheet-completed-copy-number(deprecated)",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 680
        "sheet-completed-document-number",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 681
        "sheet-completed-document-number(deprecated)",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 682
        "sides",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 683
        "sides-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 684
        "sides-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 685
        "sides-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 686
        "status-message",   
        { IPP_TEXT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 687
        "stitching-angle-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 688
        "stitching-locations-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 689
        "stitching-method-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 690
        "stitching-offset-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 691
        "stitching-reference-edge-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 692
        "subject-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 693
        "subordinate-printers-supported",   
        { IPP_URI | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 694
        "subscription-privacy-attributes",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 695
        "subscription-privacy-scope",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 696
        "time-at-completed",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 697
        "time-at-creation",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 698
        "time-at-processing",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 699
        "to-name-supported",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 700
        "trimming-offset-supported",   
        { IPP_INTEGER | IPP_ARRAY,IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 701
        "trimming-reference-edge-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 702
        "trimming-type-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 703
        "trimming-when-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 704
        "uri-authentication-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 705
        "uri-security-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 706
        "user-defined-values-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 707
        "warnings-count",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 708
        "which-jobs",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 709
        "which-jobs-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 710
        "x-image-position",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 711
        "x-image-position-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 712
        "x-image-position-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 713
        "x-image-position-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 714
        "x-image-shift",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 715
        "x-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 716
        "x-image-shift-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 717
        "x-image-shift-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 718
        "x-side1-image-shift",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 719
        "x-side1-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 720
        "x-side1-image-shift-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 721
        "x-side1-image-shift-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 722
        "x-side2-image-shift",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 723
        "x-side2-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 724
        "x-side2-image-shift-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 725
        "x-side2-image-shift-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 726
        "xri-authentication-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 727
        "xri-security-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 728
        "xri-uri-scheme-supported",   
        { IPP_URISCHEME | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 729
        "y-image-position",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 730
        "y-image-position-actual",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 731
        "y-image-position-default",   
        { IPP_KEYWORD,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 732
        "y-image-position-supported",   
        { IPP_KEYWORD | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 733
        "y-image-shift",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 734
        "y-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 735
        "y-image-shift-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 736
        "y-image-shift-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 737
        "y-side1-image-shift",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 738
        "y-side1-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 739
        "y-side1-image-shift-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 740
        "y-side1-image-shift-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 741
        "y-side2-image-shift",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 742
        "y-side2-image-shift-actual",   
        { IPP_INTEGER | IPP_ARRAY,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 743
        "y-side2-image-shift-default",   
        { IPP_INTEGER,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
    {   // index 744
        "y-side2-image-shift-supported",   
        { IPP_RANGEOFINT,IPP_NOTYPE,IPP_NOTYPE,IPP_NOTYPE },
        0,55
    },
};

ipp_attr_t s_ipp_attr_group_decdesc[] =
{
    {
        89, // document-metadata
        0,0
        NULL,NULL
    },
    {
        90, // document-name
        0,0
        NULL,NULL
    },
    {
        153, // impressions
        0,0
        NULL,NULL
    },
    {
        154, // impressions-col
        0,0
        NULL,NULL
    },
    {
        334, // k-octets
        0,0
        NULL,NULL
    },
    {
        384, // media-sheets
        0,0
        NULL,NULL
    },
    {
        385, // media-sheets-col
        0,0
        NULL,NULL
    },
    {
        499, // pages
        0,0
        NULL,NULL
    },
    {
        500, // pages-col
        0,0
        NULL,NULL
    },
};

ipp_attr_t s_ipp_attr_group_docstat[] =
{
    {
        1, // attributes-charset
        0,0
        NULL,NULL
    },
    {
        2, // attributes-natural-language
        0,0
        NULL,NULL
    },
    {
        13, // compression
        0,0
        NULL,NULL
    },
    {
        20, // copies-actual
        0,0
        NULL,NULL
    },
    {
        24, // cover-back-actual
        0,0
        NULL,NULL
    },
    {
        28, // cover-front-actual
        0,0
        NULL,NULL
    },
    {
        35, // current-page-order
        0,0
        NULL,NULL
    },
    {
        36, // date-time-at-completed
        0,0
        NULL,NULL
    },
    {
        37, // date-time-at-creation
        0,0
        NULL,NULL
    },
    {
        38, // date-time-at-processing
        0,0
        NULL,NULL
    },
    {
        48, // detailed-status-messages
        0,0
        NULL,NULL
    },
    {
        54, // document-access-errors
        0,0
        NULL,NULL
    },
    {
        56, // document-charset
        0,0
        NULL,NULL
    },
    {
        63, // document-digital-signature
        0,0
        NULL,NULL
    },
    {
        67, // document-format
        0,0
        NULL,NULL
    },
    {
        70, // document-format-details
        0,0
        NULL,NULL
    },
    {
        72, // document-format-details-detected
        0,0
        NULL,NULL
    },
    {
        75, // document-format-detected
        0,0
        NULL,NULL
    },
    {
        76, // document-format-ready
        0,0
        NULL,NULL
    },
    {
        77, // document-format-supplied
        0,0
        NULL,NULL
    },
    {
        80, // document-format-version
        0,0
        NULL,NULL
    },
    {
        82, // document-format-version-detected
        0,0
        NULL,NULL
    },
    {
        85, // document-job-id
        0,0
        NULL,NULL
    },
    {
        86, // document-job-uri
        0,0
        NULL,NULL
    },
    {
        87, // document-message
        0,0
        NULL,NULL
    },
    {
        88, // document-message-supplied
        0,0
        NULL,NULL
    },
    {
        91, // document-name-supplied
        0,0
        NULL,NULL
    },
    {
        92, // document-natural-language
        0,0
        NULL,NULL
    },
    {
        96, // document-number
        0,0
        NULL,NULL
    },
    {
        100, // document-printer-uri
        0,0
        NULL,NULL
    },
    {
        103, // document-state
        0,0
        NULL,NULL
    },
    {
        104, // document-state-message
        0,0
        NULL,NULL
    },
    {
        105, // document-state-reasons
        0,0
        NULL,NULL
    },
    {
        106, // document-uri
        0,0
        NULL,NULL
    },
    {
        107, // document-uuid
        0,0
        NULL,NULL
    },
    {
        108, // errors-count
        0,0
        NULL,NULL
    },
    {
        118, // finishings-actual
        0,0
        NULL,NULL
    },
    {
        121, // finishings-col-actual
        0,0
        NULL,NULL
    },
    {
        141, // force-front-side-actual
        0,0
        NULL,NULL
    },
    {
        150, // imposition-template-actual
        0,0
        NULL,NULL
    },
    {
        155, // impressions-completed
        0,0
        NULL,NULL
    },
    {
        156, // impressions-completed-col
        0,0
        NULL,NULL
    },
    {
        157, // impressions-completed-current-copy
        0,0
        NULL,NULL
    },
    {
        160, // input-attributes-actual
        0,0
        NULL,NULL
    },
    {
        176, // insert-sheet-actual
        0,0
        NULL,NULL
    },
    {
        335, // k-octets-processed
        0,0
        NULL,NULL
    },
    {
        338, // last-document
        0,0
        NULL,NULL
    },
    {
        351, // materials-col-actual
        0,0
        NULL,NULL
    },
    {
        360, // media-actual
        0,0
        NULL,NULL
    },
    {
        364, // media-col-actual
        0,0
        NULL,NULL
    },
    {
        376, // media-input-tray-check-actual
        0,0
        NULL,NULL
    },
    {
        386, // media-sheets-completed
        0,0
        NULL,NULL
    },
    {
        387, // media-sheets-completed-col
        0,0
        NULL,NULL
    },
    {
        398, // more-info
        0,0
        NULL,NULL
    },
    {
        406, // multiple-object-handling-actual
        0,0
        NULL,NULL
    },
    {
        452, // number-up-actual
        0,0
        NULL,NULL
    },
    {
        459, // orientation-requested-actual
        0,0
        NULL,NULL
    },
    {
        464, // output-attributes-actual
        0,0
        NULL,NULL
    },
    {
        468, // output-bin-actual
        0,0
        NULL,NULL
    },
    {
        473, // output-device-assigned
        0,0
        NULL,NULL
    },
    {
        474, // output-device-document-state
        0,0
        NULL,NULL
    },
    {
        475, // output-device-document-state-message
        0,0
        NULL,NULL
    },
    {
        476, // output-device-document-state-reasons
        0,0
        NULL,NULL
    },
    {
        489, // page-delivery-actual
        0,0
        NULL,NULL
    },
    {
        493, // page-order-received-actual
        0,0
        NULL,NULL
    },
    {
        497, // page-ranges-actual
        0,0
        NULL,NULL
    },
    {
        501, // pages-completed
        0,0
        NULL,NULL
    },
    {
        502, // pages-completed-col
        0,0
        NULL,NULL
    },
    {
        503, // pages-completed-current-copy
        0,0
        NULL,NULL
    },
    {
        528, // platform-temperature-actual
        0,0
        NULL,NULL
    },
    {
        534, // presentation-direction-number-up-actual
        0,0
        NULL,NULL
    },
    {
        538, // print-accuracy-actual
        0,0
        NULL,NULL
    },
    {
        541, // print-base-actual
        0,0
        NULL,NULL
    },
    {
        545, // print-color-mode-actual
        0,0
        NULL,NULL
    },
    {
        549, // print-content-optimize-actual
        0,0
        NULL,NULL
    },
    {
        553, // print-objects-actual
        0,0
        NULL,NULL
    },
    {
        556, // print-quality-actual
        0,0
        NULL,NULL
    },
    {
        560, // print-rendering-intent-actual
        0,0
        NULL,NULL
    },
    {
        567, // print-supports-actual
        0,0
        NULL,NULL
    },
    {
        612, // printer-resolution-actual
        0,0
        NULL,NULL
    },
    {
        629, // printer-up-time
        0,0
        NULL,NULL
    },
    {
        667, // separator-sheets-actual
        0,0
        NULL,NULL
    },
    {
        678, // sheet-completed-copy-number
        0,0
        NULL,NULL
    },
    {
        683, // sides-actual
        0,0
        NULL,NULL
    },
    {
        696, // time-at-completed
        0,0
        NULL,NULL
    },
    {
        697, // time-at-creation
        0,0
        NULL,NULL
    },
    {
        698, // time-at-processing
        0,0
        NULL,NULL
    },
    {
        707, // warnings-count
        0,0
        NULL,NULL
    },
    {
        711, // x-image-position-actual
        0,0
        NULL,NULL
    },
    {
        715, // x-image-shift-actual
        0,0
        NULL,NULL
    },
    {
        719, // x-side1-image-shift-actual
        0,0
        NULL,NULL
    },
    {
        723, // x-side2-image-shift-actual
        0,0
        NULL,NULL
    },
    {
        730, // y-image-position-actual
        0,0
        NULL,NULL
    },
    {
        734, // y-image-shift-actual
        0,0
        NULL,NULL
    },
    {
        738, // y-side1-image-shift-actual
        0,0
        NULL,NULL
    },
    {
        742, // y-side2-image-shift-actual
        0,0
        NULL,NULL
    },
};

ipp_attr_t s_ipp_attr_group_doctemp[] =
{
};

ipp_attr_t s_ipp_attr_group_evtnot[] =
{
    {
        251, // job-id
        0,0
        NULL,NULL
    },
    {
        256, // job-impressions-completed
        0,0
        NULL,NULL
    },
    {
        324, // job-state
        0,0
        NULL,NULL
    },
    {
        326, // job-state-reasons
        0,0
        NULL,NULL
    },
    {
        329, // job-uuid
        0,0
        NULL,NULL
    },
    {
        415, // notify-charset
        0,0
        NULL,NULL
    },
    {
        426, // notify-natural-language
        0,0
        NULL,NULL
    },
    {
        428, // notify-printer-uri
        0,0
        NULL,NULL
    },
    {
        433, // notify-sequence-number
        0,0
        NULL,NULL
    },
    {
        436, // notify-subscribed-event
        0,0
        NULL,NULL
    },
    {
        439, // notify-subscription-id
        0,0
        NULL,NULL
    },
    {
        441, // notify-subscription-uuid
        0,0
        NULL,NULL
    },
    {
        442, // notify-text
        0,0
        NULL,NULL
    },
    {
        444, // notify-user-data
        0,0
        NULL,NULL
    },
    {
        577, // printer-current-time
        0,0
        NULL,NULL
    },
    {
        596, // printer-is-accepting-jobs
        0,0
        NULL,NULL
    },
    {
        616, // printer-state
        0,0
        NULL,NULL
    },
    {
        620, // printer-state-reasons
        0,0
        NULL,NULL
    },
    {
        629, // printer-up-time
        0,0
        NULL,NULL
    },
};

ipp_attr_t s_ipp_attr_group_jodesc[] =
{
    {
        35, // current-page-order
        0,0
        NULL,NULL
    },
    {
        89, // document-metadata
        0,0
        NULL,NULL
    },
    {
        202, // job-charge-info
        0,0
        NULL,NULL
    },
    {
        203, // job-collation-type
        0,0
        NULL,NULL
    },
    {
        204, // job-collation-type(deprecated)
        0,0
        NULL,NULL
    },
    {
        263, // job-mandatory-attributes
        0,0
        NULL,NULL
    },
    {
        269, // job-message-from-operator
        0,0
        NULL,NULL
    },
    {
        271, // job-message-to-operator-actual
        0,0
        NULL,NULL
    },
    {
        275, // job-name
        0,0
        NULL,NULL
    },
    {
        309, // job-save-printer-make-and-model
        0,0
        NULL,NULL
    },
};

ipp_attr_t s_ipp_attr_group_jobstat[] =
{
    {
        1, // attributes-charset
        0,0
        NULL,NULL
    },
    {
        2, // attributes-natural-language
        0,0
        NULL,NULL
    },
    {
        15, // compression-supplied
        0,0
        NULL,NULL
    },
    {
        20, // copies-actual
        0,0
        NULL,NULL
    },
    {
        24, // cover-back-actual
        0,0
        NULL,NULL
    },
    {
        28, // cover-front-actual
        0,0
        NULL,NULL
    },
    {
        36, // date-time-at-completed
        0,0
        NULL,NULL
    },
    {
        37, // date-time-at-creation
        0,0
        NULL,NULL
    },
    {
        38, // date-time-at-processing
        0,0
        NULL,NULL
    },
    {
        58, // document-charset-supplied
        0,0
        NULL,NULL
    },
    {
        65, // document-digital-signature-supplied
        0,0
        NULL,NULL
    },
    {
        73, // document-format-details-supplied
        0,0
        NULL,NULL
    },
    {
        76, // document-format-ready
        0,0
        NULL,NULL
    },
    {
        77, // document-format-supplied
        0,0
        NULL,NULL
    },
    {
        83, // document-format-version-supplied
        0,0
        NULL,NULL
    },
    {
        88, // document-message-supplied
        0,0
        NULL,NULL
    },
    {
        91, // document-name-supplied
        0,0
        NULL,NULL
    },
    {
        94, // document-natural-language-supplied
        0,0
        NULL,NULL
    },
    {
        108, // errors-count
        0,0
        NULL,NULL
    },
    {
        118, // finishings-actual
        0,0
        NULL,NULL
    },
    {
        121, // finishings-col-actual
        0,0
        NULL,NULL
    },
    {
        141, // force-front-side-actual
        0,0
        NULL,NULL
    },
    {
        150, // imposition-template-actual
        0,0
        NULL,NULL
    },
    {
        157, // impressions-completed-current-copy
        0,0
        NULL,NULL
    },
    {
        158, // impressions-completed-current-copy(deprecated)
        0,0
        NULL,NULL
    },
    {
        160, // input-attributes-actual
        0,0
        NULL,NULL
    },
    {
        176, // insert-sheet-actual
        0,0
        NULL,NULL
    },
    {
        184, // job-account-id-actual
        0,0
        NULL,NULL
    },
    {
        188, // job-account-type-actual
        0,0
        NULL,NULL
    },
    {
        192, // job-accounting-sheets-actual
        0,0
        NULL,NULL
    },
    {
        196, // job-accounting-user-id-actual
        0,0
        NULL,NULL
    },
    {
        199, // job-attribute-fidelity
        0,0
        NULL,NULL
    },
    {
        205, // job-collation-type-actual
        0,0
        NULL,NULL
    },
    {
        208, // job-copies-actual
        0,0
        NULL,NULL
    },
    {
        212, // job-cover-back-actual
        0,0
        NULL,NULL
    },
    {
        216, // job-cover-front-actual
        0,0
        NULL,NULL
    },
    {
        226, // job-detailed-status-messages
        0,0
        NULL,NULL
    },
    {
        227, // job-document-access-errors
        0,0
        NULL,NULL
    },
    {
        232, // job-error-sheet-actual
        0,0
        NULL,NULL
    },
    {
        236, // job-finishings-actual
        0,0
        NULL,NULL
    },
    {
        238, // job-finishings-col-actual
        0,0
        NULL,NULL
    },
    {
        246, // job-hold-until-actual
        0,0
        NULL,NULL
    },
    {
        251, // job-id
        0,0
        NULL,NULL
    },
    {
        254, // job-impressions
        0,0
        NULL,NULL
    },
    {
        255, // job-impressions-col
        0,0
        NULL,NULL
    },
    {
        256, // job-impressions-completed
        0,0
        NULL,NULL
    },
    {
        257, // job-impressions-completed-col
        0,0
        NULL,NULL
    },
    {
        260, // job-k-octets
        0,0
        NULL,NULL
    },
    {
        261, // job-k-octets-processed
        0,0
        NULL,NULL
    },
    {
        264, // job-media-sheets
        0,0
        NULL,NULL
    },
    {
        265, // job-media-sheets-col
        0,0
        NULL,NULL
    },
    {
        266, // job-media-sheets-completed
        0,0
        NULL,NULL
    },
    {
        267, // job-media-sheets-completed-col
        0,0
        NULL,NULL
    },
    {
        274, // job-more-info
        0,0
        NULL,NULL
    },
    {
        276, // job-originating-user-name
        0,0
        NULL,NULL
    },
    {
        277, // job-originating-user-uri
        0,0
        NULL,NULL
    },
    {
        278, // job-pages
        0,0
        NULL,NULL
    },
    {
        279, // job-pages-col
        0,0
        NULL,NULL
    },
    {
        280, // job-pages-completed
        0,0
        NULL,NULL
    },
    {
        281, // job-pages-completed-col
        0,0
        NULL,NULL
    },
    {
        282, // job-pages-completed-current-copy
        0,0
        NULL,NULL
    },
    {
        296, // job-printer-up-time
        0,0
        NULL,NULL
    },
    {
        297, // job-printer-uri
        0,0
        NULL,NULL
    },
    {
        299, // job-priority-actual
        0,0
        NULL,NULL
    },
    {
        312, // job-sheet-message-actual
        0,0
        NULL,NULL
    },
    {
        316, // job-sheets-actual
        0,0
        NULL,NULL
    },
    {
        318, // job-sheets-col-actual
        0,0
        NULL,NULL
    },
    {
        324, // job-state
        0,0
        NULL,NULL
    },
    {
        325, // job-state-message
        0,0
        NULL,NULL
    },
    {
        326, // job-state-reasons
        0,0
        NULL,NULL
    },
    {
        328, // job-uri
        0,0
        NULL,NULL
    },
    {
        329, // job-uuid
        0,0
        NULL,NULL
    },
    {
        351, // materials-col-actual
        0,0
        NULL,NULL
    },
    {
        360, // media-actual
        0,0
        NULL,NULL
    },
    {
        364, // media-col-actual
        0,0
        NULL,NULL
    },
    {
        376, // media-input-tray-check-actual
        0,0
        NULL,NULL
    },
    {
        401, // multiple-document-handling-actual
        0,0
        NULL,NULL
    },
    {
        406, // multiple-object-handling-actual
        0,0
        NULL,NULL
    },
    {
        446, // number-of-documents
        0,0
        NULL,NULL
    },
    {
        447, // number-of-intervening-jobs
        0,0
        NULL,NULL
    },
    {
        452, // number-up-actual
        0,0
        NULL,NULL
    },
    {
        459, // orientation-requested-actual
        0,0
        NULL,NULL
    },
    {
        462, // original-requesting-user-name
        0,0
        NULL,NULL
    },
    {
        464, // output-attributes-actual
        0,0
        NULL,NULL
    },
    {
        468, // output-bin-actual
        0,0
        NULL,NULL
    },
    {
        472, // output-device-actual
        0,0
        NULL,NULL
    },
    {
        473, // output-device-assigned
        0,0
        NULL,NULL
    },
    {
        477, // output-device-job-state
        0,0
        NULL,NULL
    },
    {
        478, // output-device-job-state-message
        0,0
        NULL,NULL
    },
    {
        479, // output-device-job-state-reasons
        0,0
        NULL,NULL
    },
    {
        483, // output-device-uuid-assigned
        0,0
        NULL,NULL
    },
    {
        486, // overrides-actual
        0,0
        NULL,NULL
    },
    {
        489, // page-delivery-actual
        0,0
        NULL,NULL
    },
    {
        493, // page-order-received-actual
        0,0
        NULL,NULL
    },
    {
        497, // page-ranges-actual
        0,0
        NULL,NULL
    },
    {
        528, // platform-temperature-actual
        0,0
        NULL,NULL
    },
    {
        534, // presentation-direction-number-up-actual
        0,0
        NULL,NULL
    },
    {
        538, // print-accuracy-actual
        0,0
        NULL,NULL
    },
    {
        541, // print-base-actual
        0,0
        NULL,NULL
    },
    {
        545, // print-color-mode-actual
        0,0
        NULL,NULL
    },
    {
        549, // print-content-optimize-actual
        0,0
        NULL,NULL
    },
    {
        553, // print-objects-actual
        0,0
        NULL,NULL
    },
    {
        556, // print-quality-actual
        0,0
        NULL,NULL
    },
    {
        560, // print-rendering-intent-actual
        0,0
        NULL,NULL
    },
    {
        567, // print-supports-actual
        0,0
        NULL,NULL
    },
    {
        612, // printer-resolution-actual
        0,0
        NULL,NULL
    },
    {
        667, // separator-sheets-actual
        0,0
        NULL,NULL
    },
    {
        672, // sheet-collate-actual
        0,0
        NULL,NULL
    },
    {
        673, // sheet-collate-actual(deprecated)
        0,0
        NULL,NULL
    },
    {
        678, // sheet-completed-copy-number
        0,0
        NULL,NULL
    },
    {
        679, // sheet-completed-copy-number(deprecated)
        0,0
        NULL,NULL
    },
    {
        680, // sheet-completed-document-number
        0,0
        NULL,NULL
    },
    {
        681, // sheet-completed-document-number(deprecated)
        0,0
        NULL,NULL
    },
    {
        683, // sides-actual
        0,0
        NULL,NULL
    },
    {
        696, // time-at-completed
        0,0
        NULL,NULL
    },
    {
        697, // time-at-creation
        0,0
        NULL,NULL
    },
    {
        698, // time-at-processing
        0,0
        NULL,NULL
    },
    {
        707, // warnings-count
        0,0
        NULL,NULL
    },
    {
        711, // x-image-position-actual
        0,0
        NULL,NULL
    },
    {
        715, // x-image-shift-actual
        0,0
        NULL,NULL
    },
    {
        719, // x-side1-image-shift-actual
        0,0
        NULL,NULL
    },
    {
        723, // x-side2-image-shift-actual
        0,0
        NULL,NULL
    },
    {
        730, // y-image-position-actual
        0,0
        NULL,NULL
    },
    {
        734, // y-image-shift-actual
        0,0
        NULL,NULL
    },
    {
        738, // y-side1-image-shift-actual
        0,0
        NULL,NULL
    },
    {
        742, // y-side2-image-shift-actual
        0,0
        NULL,NULL
    },
};

ipp_attr_t s_ipp_attr_group_jobtemp[] =
{
    {
        17, // confirmation-sheet-print
        0,0
        NULL,NULL
    },
    {
        19, // copies
        0,0
        NULL,NULL
    },
    {
        23, // cover-back
        0,0
        NULL,NULL
    },
    {
        27, // cover-front
        0,0
        NULL,NULL
    },
    {
        31, // cover-sheet-info
        0,0
        NULL,NULL
    },
    {
        42, // destination-statuses
        0,0
        NULL,NULL
    },
    {
        45, // destination-uris
        0,0
        NULL,NULL
    },
    {
        109, // feed-orientation
        0,0
        NULL,NULL
    },
    {
        117, // finishings
        0,0
        NULL,NULL
    },
    {
        119, // finishings-col
        0,0
        NULL,NULL
    },
    {
        120, // finishings-col(extension)
        0,0
        NULL,NULL
    },
    {
        134, // font-name-requested
        0,0
        NULL,NULL
    },
    {
        137, // font-size-requested
        0,0
        NULL,NULL
    },
    {
        140, // force-front-side
        0,0
        NULL,NULL
    },
    {
        149, // imposition-template
        0,0
        NULL,NULL
    },
    {
        175, // insert-sheet
        0,0
        NULL,NULL
    },
    {
        183, // job-account-id
        0,0
        NULL,NULL
    },
    {
        187, // job-account-type
        0,0
        NULL,NULL
    },
    {
        191, // job-accounting-sheets
        0,0
        NULL,NULL
    },
    {
        195, // job-accounting-user-id
        0,0
        NULL,NULL
    },
    {
        207, // job-copies
        0,0
        NULL,NULL
    },
    {
        211, // job-cover-back
        0,0
        NULL,NULL
    },
    {
        215, // job-cover-front
        0,0
        NULL,NULL
    },
    {
        220, // job-delay-output-until
        0,0
        NULL,NULL
    },
    {
        223, // job-delay-output-until-time
        0,0
        NULL,NULL
    },
    {
        228, // job-error-action
        0,0
        NULL,NULL
    },
    {
        231, // job-error-sheet
        0,0
        NULL,NULL
    },
    {
        235, // job-finishings
        0,0
        NULL,NULL
    },
    {
        237, // job-finishings-col
        0,0
        NULL,NULL
    },
    {
        245, // job-hold-until
        0,0
        NULL,NULL
    },
    {
        249, // job-hold-until-time
        0,0
        NULL,NULL
    },
    {
        270, // job-message-to-operator
        0,0
        NULL,NULL
    },
    {
        283, // job-pages-per-set
        0,0
        NULL,NULL
    },
    {
        292, // job-phone-number
        0,0
        NULL,NULL
    },
    {
        298, // job-priority
        0,0
        NULL,NULL
    },
    {
        304, // job-recipient-name
        0,0
        NULL,NULL
    },
    {
        308, // job-save-disposition
        0,0
        NULL,NULL
    },
    {
        311, // job-sheet-message
        0,0
        NULL,NULL
    },
    {
        315, // job-sheets
        0,0
        NULL,NULL
    },
    {
        317, // job-sheets-col
        0,0
        NULL,NULL
    },
    {
        350, // materials-col
        0,0
        NULL,NULL
    },
    {
        359, // media
        0,0
        NULL,NULL
    },
    {
        363, // media-col
        0,0
        NULL,NULL
    },
    {
        375, // media-input-tray-check
        0,0
        NULL,NULL
    },
    {
        400, // multiple-document-handling
        0,0
        NULL,NULL
    },
    {
        405, // multiple-object-handling
        0,0
        NULL,NULL
    },
    {
        448, // number-of-retries
        0,0
        NULL,NULL
    },
    {
        451, // number-up
        0,0
        NULL,NULL
    },
    {
        458, // orientation-requested
        0,0
        NULL,NULL
    },
    {
        467, // output-bin
        0,0
        NULL,NULL
    },
    {
        471, // output-device
        0,0
        NULL,NULL
    },
    {
        485, // overrides
        0,0
        NULL,NULL
    },
    {
        488, // page-delivery
        0,0
        NULL,NULL
    },
    {
        492, // page-order-received
        0,0
        NULL,NULL
    },
    {
        496, // page-ranges
        0,0
        NULL,NULL
    },
    {
        506, // pages-per-subset
        0,0
        NULL,NULL
    },
    {
        510, // pclm-source-resolution
        0,0
        NULL,NULL
    },
    {
        517, // pdl-init-file
        0,0
        NULL,NULL
    },
    {
        527, // platform-temperature
        0,0
        NULL,NULL
    },
    {
        533, // presentation-direction-number-up
        0,0
        NULL,NULL
    },
    {
        537, // print-accuracy
        0,0
        NULL,NULL
    },
    {
        540, // print-base
        0,0
        NULL,NULL
    },
    {
        544, // print-color-mode
        0,0
        NULL,NULL
    },
    {
        548, // print-content-optimize
        0,0
        NULL,NULL
    },
    {
        552, // print-objects
        0,0
        NULL,NULL
    },
    {
        555, // print-quality
        0,0
        NULL,NULL
    },
    {
        559, // print-rendering-intent
        0,0
        NULL,NULL
    },
    {
        563, // print-scaling
        0,0
        NULL,NULL
    },
    {
        566, // print-supports
        0,0
        NULL,NULL
    },
    {
        611, // printer-resolution
        0,0
        NULL,NULL
    },
    {
        636, // proof-print
        0,0
        NULL,NULL
    },
    {
        653, // retry-interval
        0,0
        NULL,NULL
    },
    {
        656, // retry-time-out
        0,0
        NULL,NULL
    },
    {
        666, // separator-sheets
        0,0
        NULL,NULL
    },
    {
        670, // sheet-collate
        0,0
        NULL,NULL
    },
    {
        671, // sheet-collate(deprecated)
        0,0
        NULL,NULL
    },
    {
        682, // sides
        0,0
        NULL,NULL
    },
    {
        710, // x-image-position
        0,0
        NULL,NULL
    },
    {
        714, // x-image-shift
        0,0
        NULL,NULL
    },
    {
        718, // x-side1-image-shift
        0,0
        NULL,NULL
    },
    {
        722, // x-side2-image-shift
        0,0
        NULL,NULL
    },
    {
        729, // y-image-position
        0,0
        NULL,NULL
    },
    {
        733, // y-image-shift
        0,0
        NULL,NULL
    },
    {
        737, // y-side1-image-shift
        0,0
        NULL,NULL
    },
    {
        741, // y-side2-image-shift
        0,0
        NULL,NULL
    },
};

ipp_attr_t s_ipp_attr_group_operation[] =
{
    {
        1, // attributes-charset
        0,0
        NULL,NULL
    },
    {
        2, // attributes-natural-language
        0,0
        NULL,NULL
    },
    {
        7, // charge-info-message
        0,0
        NULL,NULL
    },
    {
        13, // compression
        0,0
        NULL,NULL
    },
    {
        14, // compression-accepted
        0,0
        NULL,NULL
    },
    {
        39, // destination-accesses
        0,0
        NULL,NULL
    },
    {
        40, // destination-accesses(extension)
        0,0
        NULL,NULL
    },
    {
        47, // detailed-status-message
        0,0
        NULL,NULL
    },
    {
        51, // document-access
        0,0
        NULL,NULL
    },
    {
        52, // document-access(extension)
        0,0
        NULL,NULL
    },
    {
        53, // document-access-error
        0,0
        NULL,NULL
    },
    {
        56, // document-charset
        0,0
        NULL,NULL
    },
    {
        61, // document-data-get-interval
        0,0
        NULL,NULL
    },
    {
        62, // document-data-wait
        0,0
        NULL,NULL
    },
    {
        63, // document-digital-signature
        0,0
        NULL,NULL
    },
    {
        67, // document-format
        0,0
        NULL,NULL
    },
    {
        68, // document-format-accepted
        0,0
        NULL,NULL
    },
    {
        70, // document-format-details
        0,0
        NULL,NULL
    },
    {
        87, // document-message
        0,0
        NULL,NULL
    },
    {
        89, // document-metadata
        0,0
        NULL,NULL
    },
    {
        90, // document-name
        0,0
        NULL,NULL
    },
    {
        92, // document-natural-language
        0,0
        NULL,NULL
    },
    {
        96, // document-number
        0,0
        NULL,NULL
    },
    {
        97, // document-password
        0,0
        NULL,NULL
    },
    {
        99, // document-preprocessed
        0,0
        NULL,NULL
    },
    {
        113, // fetch-status-code
        0,0
        NULL,NULL
    },
    {
        114, // fetch-status-message
        0,0
        NULL,NULL
    },
    {
        129, // first-index
        0,0
        NULL,NULL
    },
    {
        146, // identify-actions
        0,0
        NULL,NULL
    },
    {
        159, // input-attributes
        0,0
        NULL,NULL
    },
    {
        179, // ipp-attribute-fidelity
        0,0
        NULL,NULL
    },
    {
        200, // job-authorization-uri
        0,0
        NULL,NULL
    },
    {
        245, // job-hold-until
        0,0
        NULL,NULL
    },
    {
        251, // job-id
        0,0
        NULL,NULL
    },
    {
        252, // job-ids
        0,0
        NULL,NULL
    },
    {
        254, // job-impressions
        0,0
        NULL,NULL
    },
    {
        255, // job-impressions-col
        0,0
        NULL,NULL
    },
    {
        258, // job-impressions-estimated
        0,0
        NULL,NULL
    },
    {
        260, // job-k-octets
        0,0
        NULL,NULL
    },
    {
        263, // job-mandatory-attributes
        0,0
        NULL,NULL
    },
    {
        264, // job-media-sheets
        0,0
        NULL,NULL
    },
    {
        265, // job-media-sheets-col
        0,0
        NULL,NULL
    },
    {
        269, // job-message-from-operator
        0,0
        NULL,NULL
    },
    {
        275, // job-name
        0,0
        NULL,NULL
    },
    {
        278, // job-pages
        0,0
        NULL,NULL
    },
    {
        279, // job-pages-col
        0,0
        NULL,NULL
    },
    {
        285, // job-password
        0,0
        NULL,NULL
    },
    {
        286, // job-password-encryption
        0,0
        NULL,NULL
    },
    {
        324, // job-state
        0,0
        NULL,NULL
    },
    {
        325, // job-state-message
        0,0
        NULL,NULL
    },
    {
        326, // job-state-reasons
        0,0
        NULL,NULL
    },
    {
        328, // job-uri
        0,0
        NULL,NULL
    },
    {
        338, // last-document
        0,0
        NULL,NULL
    },
    {
        339, // limit
        0,0
        NULL,NULL
    },
    {
        396, // message
        0,0
        NULL,NULL
    },
    {
        411, // my-jobs
        0,0
        NULL,NULL
    },
    {
        419, // notify-get-interval
        0,0
        NULL,NULL
    },
    {
        434, // notify-sequence-numbers
        0,0
        NULL,NULL
    },
    {
        440, // notify-subscription-ids
        0,0
        NULL,NULL
    },
    {
        445, // notify-wait
        0,0
        NULL,NULL
    },
    {
        462, // original-requesting-user-name
        0,0
        NULL,NULL
    },
    {
        463, // output-attributes
        0,0
        NULL,NULL
    },
    {
        480, // output-device-job-states
        0,0
        NULL,NULL
    },
    {
        482, // output-device-uuid
        0,0
        NULL,NULL
    },
    {
        531, // preferred-attributes
        0,0
        NULL,NULL
    },
    {
        602, // printer-message-from-operator
        0,0
        NULL,NULL
    },
    {
        629, // printer-up-time
        0,0
        NULL,NULL
    },
    {
        630, // printer-uri
        0,0
        NULL,NULL
    },
    {
        635, // profile-uri-actual
        0,0
        NULL,NULL
    },
    {
        649, // requested-attributes
        0,0
        NULL,NULL
    },
    {
        650, // requesting-user-name
        0,0
        NULL,NULL
    },
    {
        651, // requesting-user-uri
        0,0
        NULL,NULL
    },
    {
        686, // status-message
        0,0
        NULL,NULL
    },
    {
        708, // which-jobs
        0,0
        NULL,NULL
    },
};

ipp_attr_t s_ipp_attr_group_prtdesc[] =
{
    {
        0, // accuracy-units-supported
        0,0
        NULL,NULL
    },
    {
        3, // baling-type-supported
        0,0
        NULL,NULL
    },
    {
        4, // baling-when-supported
        0,0
        NULL,NULL
    },
    {
        5, // binding-reference-edge-supported
        0,0
        NULL,NULL
    },
    {
        6, // binding-type-supported
        0,0
        NULL,NULL
    },
    {
        8, // charset-configured
        0,0
        NULL,NULL
    },
    {
        9, // charset-supported
        0,0
        NULL,NULL
    },
    {
        10, // coating-sides-supported
        0,0
        NULL,NULL
    },
    {
        11, // coating-type-supported
        0,0
        NULL,NULL
    },
    {
        12, // color-supported
        0,0
        NULL,NULL
    },
    {
        16, // compression-supported
        0,0
        NULL,NULL
    },
    {
        18, // confirmation-sheet-print-default
        0,0
        NULL,NULL
    },
    {
        21, // copies-default
        0,0
        NULL,NULL
    },
    {
        22, // copies-supported
        0,0
        NULL,NULL
    },
    {
        25, // cover-back-default
        0,0
        NULL,NULL
    },
    {
        26, // cover-back-supported
        0,0
        NULL,NULL
    },
    {
        29, // cover-front-default
        0,0
        NULL,NULL
    },
    {
        30, // cover-front-supported
        0,0
        NULL,NULL
    },
    {
        32, // cover-sheet-info-default
        0,0
        NULL,NULL
    },
    {
        33, // cover-sheet-info-supported
        0,0
        NULL,NULL
    },
    {
        34, // covering-name-supported
        0,0
        NULL,NULL
    },
    {
        41, // destination-accesses-supported
        0,0
        NULL,NULL
    },
    {
        43, // destination-uri-ready
        0,0
        NULL,NULL
    },
    {
        44, // destination-uri-schemes-supported
        0,0
        NULL,NULL
    },
    {
        46, // destination-uris-supported
        0,0
        NULL,NULL
    },
    {
        55, // document-access-supported
        0,0
        NULL,NULL
    },
    {
        57, // document-charset-default
        0,0
        NULL,NULL
    },
    {
        59, // document-charset-supported
        0,0
        NULL,NULL
    },
    {
        60, // document-creation-attributes-supported
        0,0
        NULL,NULL
    },
    {
        64, // document-digital-signature-default
        0,0
        NULL,NULL
    },
    {
        66, // document-digital-signature-supported
        0,0
        NULL,NULL
    },
    {
        69, // document-format-default
        0,0
        NULL,NULL
    },
    {
        71, // document-format-details-default
        0,0
        NULL,NULL
    },
    {
        74, // document-format-details-supported
        0,0
        NULL,NULL
    },
    {
        78, // document-format-supported
        0,0
        NULL,NULL
    },
    {
        81, // document-format-version-default
        0,0
        NULL,NULL
    },
    {
        84, // document-format-version-supported
        0,0
        NULL,NULL
    },
    {
        93, // document-natural-language-default
        0,0
        NULL,NULL
    },
    {
        95, // document-natural-language-supported
        0,0
        NULL,NULL
    },
    {
        98, // document-password-supported
        0,0
        NULL,NULL
    },
    {
        101, // document-privacy-attributes
        0,0
        NULL,NULL
    },
    {
        102, // document-privacy-scope
        0,0
        NULL,NULL
    },
    {
        110, // feed-orientation-default
        0,0
        NULL,NULL
    },
    {
        111, // feed-orientation-supported
        0,0
        NULL,NULL
    },
    {
        112, // fetch-document-attributes-supported
        0,0
        NULL,NULL
    },
    {
        115, // finishing-template-supported
        0,0
        NULL,NULL
    },
    {
        116, // finishing-template-supported(extension)
        0,0
        NULL,NULL
    },
    {
        122, // finishings-col-database
        0,0
        NULL,NULL
    },
    {
        123, // finishings-col-default
        0,0
        NULL,NULL
    },
    {
        124, // finishings-col-ready
        0,0
        NULL,NULL
    },
    {
        125, // finishings-col-supported
        0,0
        NULL,NULL
    },
    {
        126, // finishings-default
        0,0
        NULL,NULL
    },
    {
        127, // finishings-ready
        0,0
        NULL,NULL
    },
    {
        128, // finishings-supported
        0,0
        NULL,NULL
    },
    {
        130, // folding-direction-supported
        0,0
        NULL,NULL
    },
    {
        131, // folding-offset-supported
        0,0
        NULL,NULL
    },
    {
        132, // folding-offset-supported(extension)
        0,0
        NULL,NULL
    },
    {
        133, // folding-reference-edge-supported
        0,0
        NULL,NULL
    },
    {
        135, // font-name-requested-default
        0,0
        NULL,NULL
    },
    {
        136, // font-name-requested-supported
        0,0
        NULL,NULL
    },
    {
        138, // font-size-requested-default
        0,0
        NULL,NULL
    },
    {
        139, // font-size-requested-supported
        0,0
        NULL,NULL
    },
    {
        142, // force-front-side-default (under review)
        0,0
        NULL,NULL
    },
    {
        143, // force-front-side-supported (under review)
        0,0
        NULL,NULL
    },
    {
        144, // from-name-supported
        0,0
        NULL,NULL
    },
    {
        145, // generated-natural-language-supported
        0,0
        NULL,NULL
    },
    {
        147, // identify-actions-default
        0,0
        NULL,NULL
    },
    {
        148, // identify-actions-supported
        0,0
        NULL,NULL
    },
    {
        151, // imposition-template-default
        0,0
        NULL,NULL
    },
    {
        152, // imposition-template-supported
        0,0
        NULL,NULL
    },
    {
        161, // input-attributes-default
        0,0
        NULL,NULL
    },
    {
        162, // input-attributes-supported
        0,0
        NULL,NULL
    },
    {
        163, // input-color-mode-supported
        0,0
        NULL,NULL
    },
    {
        164, // input-content-type-supported
        0,0
        NULL,NULL
    },
    {
        165, // input-film-scan-mode-supported
        0,0
        NULL,NULL
    },
    {
        166, // input-media-supported
        0,0
        NULL,NULL
    },
    {
        167, // input-orientation-requested-supported
        0,0
        NULL,NULL
    },
    {
        168, // input-quality-supported
        0,0
        NULL,NULL
    },
    {
        169, // input-resolution-supported
        0,0
        NULL,NULL
    },
    {
        170, // input-scan-regions-supported
        0,0
        NULL,NULL
    },
    {
        171, // input-sides-supported
        0,0
        NULL,NULL
    },
    {
        172, // input-source-supported
        0,0
        NULL,NULL
    },
    {
        173, // insert-after-page-number-supported
        0,0
        NULL,NULL
    },
    {
        174, // insert-count-supported
        0,0
        NULL,NULL
    },
    {
        177, // insert-sheet-default
        0,0
        NULL,NULL
    },
    {
        178, // insert-sheet-supported
        0,0
        NULL,NULL
    },
    {
        180, // ipp-features-supported
        0,0
        NULL,NULL
    },
    {
        181, // ipp-versions-supported
        0,0
        NULL,NULL
    },
    {
        182, // ippget-event-life
        0,0
        NULL,NULL
    },
    {
        185, // job-account-id-default
        0,0
        NULL,NULL
    },
    {
        186, // job-account-id-supported
        0,0
        NULL,NULL
    },
    {
        189, // job-account-type-default
        0,0
        NULL,NULL
    },
    {
        190, // job-account-type-supported
        0,0
        NULL,NULL
    },
    {
        193, // job-accounting-sheets-default
        0,0
        NULL,NULL
    },
    {
        194, // job-accounting-sheets-supported
        0,0
        NULL,NULL
    },
    {
        197, // job-accounting-user-id-default
        0,0
        NULL,NULL
    },
    {
        198, // job-accounting-user-id-supported
        0,0
        NULL,NULL
    },
    {
        201, // job-authorization-uri-supported
        0,0
        NULL,NULL
    },
    {
        206, // job-constraints-supported
        0,0
        NULL,NULL
    },
    {
        209, // job-copies-default
        0,0
        NULL,NULL
    },
    {
        210, // job-copies-supported
        0,0
        NULL,NULL
    },
    {
        213, // job-cover-back-default
        0,0
        NULL,NULL
    },
    {
        214, // job-cover-back-supported
        0,0
        NULL,NULL
    },
    {
        217, // job-cover-front-default
        0,0
        NULL,NULL
    },
    {
        218, // job-cover-front-supported
        0,0
        NULL,NULL
    },
    {
        219, // job-creation-attributes-supported
        0,0
        NULL,NULL
    },
    {
        221, // job-delay-output-until-default
        0,0
        NULL,NULL
    },
    {
        222, // job-delay-output-until-supported
        0,0
        NULL,NULL
    },
    {
        224, // job-delay-output-until-time-supported
        0,0
        NULL,NULL
    },
    {
        225, // job-destination-spooling-supported
        0,0
        NULL,NULL
    },
    {
        229, // job-error-action-default
        0,0
        NULL,NULL
    },
    {
        230, // job-error-action-supported
        0,0
        NULL,NULL
    },
    {
        233, // job-error-sheet-default
        0,0
        NULL,NULL
    },
    {
        234, // job-error-sheet-supported
        0,0
        NULL,NULL
    },
    {
        239, // job-finishings-col-default
        0,0
        NULL,NULL
    },
    {
        240, // job-finishings-col-ready
        0,0
        NULL,NULL
    },
    {
        241, // job-finishings-col-supported
        0,0
        NULL,NULL
    },
    {
        242, // job-finishings-default
        0,0
        NULL,NULL
    },
    {
        243, // job-finishings-ready
        0,0
        NULL,NULL
    },
    {
        244, // job-finishings-supported
        0,0
        NULL,NULL
    },
    {
        247, // job-hold-until-default
        0,0
        NULL,NULL
    },
    {
        248, // job-hold-until-supported
        0,0
        NULL,NULL
    },
    {
        250, // job-hold-until-time-supported
        0,0
        NULL,NULL
    },
    {
        253, // job-ids-supported
        0,0
        NULL,NULL
    },
    {
        259, // job-impressions-supported
        0,0
        NULL,NULL
    },
    {
        262, // job-k-octets-supported
        0,0
        NULL,NULL
    },
    {
        268, // job-media-sheets-supported
        0,0
        NULL,NULL
    },
    {
        272, // job-message-to-operator-default
        0,0
        NULL,NULL
    },
    {
        273, // job-message-to-operator-supported
        0,0
        NULL,NULL
    },
    {
        284, // job-pages-per-set-supported
        0,0
        NULL,NULL
    },
    {
        287, // job-password-encryption-supported
        0,0
        NULL,NULL
    },
    {
        288, // job-password-length-supported
        0,0
        NULL,NULL
    },
    {
        289, // job-password-repertoire-configured
        0,0
        NULL,NULL
    },
    {
        290, // job-password-repertoire-supported
        0,0
        NULL,NULL
    },
    {
        291, // job-password-supported
        0,0
        NULL,NULL
    },
    {
        293, // job-phone-number-default
        0,0
        NULL,NULL
    },
    {
        294, // job-phone-number-supported
        0,0
        NULL,NULL
    },
    {
        295, // job-presets-supported
        0,0
        NULL,NULL
    },
    {
        300, // job-priority-default
        0,0
        NULL,NULL
    },
    {
        301, // job-priority-supported
        0,0
        NULL,NULL
    },
    {
        302, // job-privacy-attributes
        0,0
        NULL,NULL
    },
    {
        303, // job-privacy-scope
        0,0
        NULL,NULL
    },
    {
        305, // job-recipient-name-default
        0,0
        NULL,NULL
    },
    {
        306, // job-recipient-name-supported
        0,0
        NULL,NULL
    },
    {
        307, // job-resolvers-supported
        0,0
        NULL,NULL
    },
    {
        313, // job-sheet-message-default
        0,0
        NULL,NULL
    },
    {
        314, // job-sheet-message-supported
        0,0
        NULL,NULL
    },
    {
        319, // job-sheets-col-default
        0,0
        NULL,NULL
    },
    {
        320, // job-sheets-col-supported
        0,0
        NULL,NULL
    },
    {
        321, // job-sheets-default
        0,0
        NULL,NULL
    },
    {
        322, // job-sheets-supported
        0,0
        NULL,NULL
    },
    {
        323, // job-spooling-supported
        0,0
        NULL,NULL
    },
    {
        327, // job-triggers-supported
        0,0
        NULL,NULL
    },
    {
        330, // jpeg-features-supported
        0,0
        NULL,NULL
    },
    {
        331, // jpeg-k-octets-supported
        0,0
        NULL,NULL
    },
    {
        332, // jpeg-x-dimension-supported
        0,0
        NULL,NULL
    },
    {
        333, // jpeg-y-dimension-supported
        0,0
        NULL,NULL
    },
    {
        336, // laminating-sides-supported
        0,0
        NULL,NULL
    },
    {
        337, // laminating-type-supported
        0,0
        NULL,NULL
    },
    {
        340, // logo-uri-formats-supported
        0,0
        NULL,NULL
    },
    {
        341, // logo-uri-schemes-supported
        0,0
        NULL,NULL
    },
    {
        342, // material-amount-units-supported
        0,0
        NULL,NULL
    },
    {
        343, // material-diameter-supported
        0,0
        NULL,NULL
    },
    {
        344, // material-purpose-supported
        0,0
        NULL,NULL
    },
    {
        345, // material-rate-supported
        0,0
        NULL,NULL
    },
    {
        346, // material-rate-units-supported
        0,0
        NULL,NULL
    },
    {
        347, // material-shell-thickness-supported
        0,0
        NULL,NULL
    },
    {
        348, // material-temperature-supported
        0,0
        NULL,NULL
    },
    {
        349, // material-type-supported
        0,0
        NULL,NULL
    },
    {
        352, // materials-col-database
        0,0
        NULL,NULL
    },
    {
        353, // materials-col-default
        0,0
        NULL,NULL
    },
    {
        354, // materials-col-ready
        0,0
        NULL,NULL
    },
    {
        355, // materials-col-supported
        0,0
        NULL,NULL
    },
    {
        356, // max-materials-col-supported
        0,0
        NULL,NULL
    },
    {
        357, // max-save-info-supported
        0,0
        NULL,NULL
    },
    {
        358, // max-stitching-locations-supported
        0,0
        NULL,NULL
    },
    {
        361, // media-back-coating-supported
        0,0
        NULL,NULL
    },
    {
        362, // media-bottom-margin-supported
        0,0
        NULL,NULL
    },
    {
        365, // media-col-database
        0,0
        NULL,NULL
    },
    {
        366, // media-col-default
        0,0
        NULL,NULL
    },
    {
        367, // media-col-ready
        0,0
        NULL,NULL
    },
    {
        368, // media-col-supported
        0,0
        NULL,NULL
    },
    {
        369, // media-color-supported
        0,0
        NULL,NULL
    },
    {
        370, // media-default
        0,0
        NULL,NULL
    },
    {
        371, // media-front-coating-supported
        0,0
        NULL,NULL
    },
    {
        372, // media-grain-supported
        0,0
        NULL,NULL
    },
    {
        373, // media-hole-count-supported
        0,0
        NULL,NULL
    },
    {
        374, // media-info-supported
        0,0
        NULL,NULL
    },
    {
        377, // media-key-supported
        0,0
        NULL,NULL
    },
    {
        378, // media-left-margin-supported
        0,0
        NULL,NULL
    },
    {
        379, // media-order-count-supported
        0,0
        NULL,NULL
    },
    {
        380, // media-pre-printed-supported
        0,0
        NULL,NULL
    },
    {
        381, // media-ready
        0,0
        NULL,NULL
    },
    {
        382, // media-recycled-supported
        0,0
        NULL,NULL
    },
    {
        383, // media-right-margin-supported
        0,0
        NULL,NULL
    },
    {
        388, // media-size-supported
        0,0
        NULL,NULL
    },
    {
        389, // media-source-supported
        0,0
        NULL,NULL
    },
    {
        390, // media-supported
        0,0
        NULL,NULL
    },
    {
        391, // media-thickness-supported
        0,0
        NULL,NULL
    },
    {
        392, // media-tooth-supported
        0,0
        NULL,NULL
    },
    {
        393, // media-top-margin-supported
        0,0
        NULL,NULL
    },
    {
        394, // media-type-supported
        0,0
        NULL,NULL
    },
    {
        395, // media-weight-metric-supported
        0,0
        NULL,NULL
    },
    {
        397, // message-supported
        0,0
        NULL,NULL
    },
    {
        399, // multiple-destination-uris-supported
        0,0
        NULL,NULL
    },
    {
        402, // multiple-document-handling-default
        0,0
        NULL,NULL
    },
    {
        403, // multiple-document-handling-supported
        0,0
        NULL,NULL
    },
    {
        404, // multiple-document-jobs-supported
        0,0
        NULL,NULL
    },
    {
        407, // multiple-object-handling-default
        0,0
        NULL,NULL
    },
    {
        408, // multiple-object-handling-supported
        0,0
        NULL,NULL
    },
    {
        409, // multiple-operation-time-out
        0,0
        NULL,NULL
    },
    {
        410, // multiple-operation-time-out-action
        0,0
        NULL,NULL
    },
    {
        412, // natural-language-configured
        0,0
        NULL,NULL
    },
    {
        414, // notify-attributes-supported
        0,0
        NULL,NULL
    },
    {
        417, // notify-events-default
        0,0
        NULL,NULL
    },
    {
        418, // notify-events-supported
        0,0
        NULL,NULL
    },
    {
        422, // notify-lease-duration-default
        0,0
        NULL,NULL
    },
    {
        423, // notify-lease-duration-supported
        0,0
        NULL,NULL
    },
    {
        430, // notify-pull-method-supported
        0,0
        NULL,NULL
    },
    {
        432, // notify-schemes-supported
        0,0
        NULL,NULL
    },
    {
        449, // number-of-retries-default
        0,0
        NULL,NULL
    },
    {
        450, // number-of-retries-supported
        0,0
        NULL,NULL
    },
    {
        453, // number-up-default
        0,0
        NULL,NULL
    },
    {
        454, // number-up-supported
        0,0
        NULL,NULL
    },
    {
        455, // oauth-authorization-server-uri
        0,0
        NULL,NULL
    },
    {
        456, // operations-supported
        0,0
        NULL,NULL
    },
    {
        457, // organization-name-supported
        0,0
        NULL,NULL
    },
    {
        460, // orientation-requested-default
        0,0
        NULL,NULL
    },
    {
        461, // orientation-requested-supported
        0,0
        NULL,NULL
    },
    {
        465, // output-attributes-default
        0,0
        NULL,NULL
    },
    {
        466, // output-attributes-supported
        0,0
        NULL,NULL
    },
    {
        469, // output-bin-default
        0,0
        NULL,NULL
    },
    {
        470, // output-bin-supported
        0,0
        NULL,NULL
    },
    {
        481, // output-device-supported
        0,0
        NULL,NULL
    },
    {
        484, // output-device-uuid-supported
        0,0
        NULL,NULL
    },
    {
        487, // overrides-supported
        0,0
        NULL,NULL
    },
    {
        490, // page-delivery-default
        0,0
        NULL,NULL
    },
    {
        491, // page-delivery-supported
        0,0
        NULL,NULL
    },
    {
        494, // page-order-received-default
        0,0
        NULL,NULL
    },
    {
        495, // page-order-received-supported
        0,0
        NULL,NULL
    },
    {
        498, // page-ranges-supported
        0,0
        NULL,NULL
    },
    {
        507, // pages-per-subset-supported
        0,0
        NULL,NULL
    },
    {
        508, // parent-printers-supported
        0,0
        NULL,NULL
    },
    {
        509, // pclm-raster-back-side
        0,0
        NULL,NULL
    },
    {
        511, // pclm-source-resolution-supported
        0,0
        NULL,NULL
    },
    {
        512, // pclm-strip-height-preferred
        0,0
        NULL,NULL
    },
    {
        513, // pclm-strip-height-supported
        0,0
        NULL,NULL
    },
    {
        514, // pdf-features-supported
        0,0
        NULL,NULL
    },
    {
        515, // pdf-k-octets-supported
        0,0
        NULL,NULL
    },
    {
        516, // pdf-versions-supported
        0,0
        NULL,NULL
    },
    {
        518, // pdl-init-file-default
        0,0
        NULL,NULL
    },
    {
        519, // pdl-init-file-entry-supported
        0,0
        NULL,NULL
    },
    {
        520, // pdl-init-file-location-supported
        0,0
        NULL,NULL
    },
    {
        521, // pdl-init-file-name-subdirectory-supported
        0,0
        NULL,NULL
    },
    {
        522, // pdl-init-file-name-supported
        0,0
        NULL,NULL
    },
    {
        523, // pdl-init-file-supported
        0,0
        NULL,NULL
    },
    {
        524, // pdl-override-guaranteed-supported
        0,0
        NULL,NULL
    },
    {
        525, // pdl-override-supported
        0,0
        NULL,NULL
    },
    {
        526, // platform-shape
        0,0
        NULL,NULL
    },
    {
        529, // platform-temperature-default
        0,0
        NULL,NULL
    },
    {
        530, // platform-temperature-supported
        0,0
        NULL,NULL
    },
    {
        532, // preferred-attributes-supported
        0,0
        NULL,NULL
    },
    {
        535, // presentation-direction-number-up-default
        0,0
        NULL,NULL
    },
    {
        536, // presentation-direction-number-up-supported
        0,0
        NULL,NULL
    },
    {
        539, // print-accuracy-supported
        0,0
        NULL,NULL
    },
    {
        542, // print-base-default
        0,0
        NULL,NULL
    },
    {
        543, // print-base-supported
        0,0
        NULL,NULL
    },
    {
        546, // print-color-mode-default
        0,0
        NULL,NULL
    },
    {
        547, // print-color-mode-supported
        0,0
        NULL,NULL
    },
    {
        550, // print-content-optimize-default
        0,0
        NULL,NULL
    },
    {
        551, // print-content-optimize-supported
        0,0
        NULL,NULL
    },
    {
        554, // print-objects-supported
        0,0
        NULL,NULL
    },
    {
        557, // print-quality-default
        0,0
        NULL,NULL
    },
    {
        558, // print-quality-supported
        0,0
        NULL,NULL
    },
    {
        561, // print-rendering-intent-default
        0,0
        NULL,NULL
    },
    {
        562, // print-rendering-intent-supported
        0,0
        NULL,NULL
    },
    {
        564, // print-scaling-default
        0,0
        NULL,NULL
    },
    {
        565, // print-scaling-supported
        0,0
        NULL,NULL
    },
    {
        568, // print-supports-default
        0,0
        NULL,NULL
    },
    {
        569, // print-supports-supported
        0,0
        NULL,NULL
    },
    {
        572, // printer-camera-image-uri
        0,0
        NULL,NULL
    },
    {
        573, // printer-charge-info
        0,0
        NULL,NULL
    },
    {
        574, // printer-charge-info-uri
        0,0
        NULL,NULL
    },
    {
        577, // printer-current-time
        0,0
        NULL,NULL
    },
    {
        579, // printer-device-id
        0,0
        NULL,NULL
    },
    {
        580, // printer-dns-sd-name
        0,0
        NULL,NULL
    },
    {
        581, // printer-driver-installer
        0,0
        NULL,NULL
    },
    {
        582, // printer-fax-log-uri
        0,0
        NULL,NULL
    },
    {
        583, // printer-fax-modem-info
        0,0
        NULL,NULL
    },
    {
        584, // printer-fax-modem-name
        0,0
        NULL,NULL
    },
    {
        585, // printer-fax-modem-number
        0,0
        NULL,NULL
    },
    {
        590, // printer-geo-location
        0,0
        NULL,NULL
    },
    {
        591, // printer-get-attributes-supported
        0,0
        NULL,NULL
    },
    {
        592, // printer-icc-profiles
        0,0
        NULL,NULL
    },
    {
        593, // printer-icons
        0,0
        NULL,NULL
    },
    {
        594, // printer-info
        0,0
        NULL,NULL
    },
    {
        597, // printer-kind
        0,0
        NULL,NULL
    },
    {
        598, // printer-location
        0,0
        NULL,NULL
    },
    {
        599, // printer-make-and-model
        0,0
        NULL,NULL
    },
    {
        600, // printer-mandatory-job-attributes
        0,0
        NULL,NULL
    },
    {
        605, // printer-more-info-manufacturer
        0,0
        NULL,NULL
    },
    {
        606, // printer-name
        0,0
        NULL,NULL
    },
    {
        607, // printer-organization
        0,0
        NULL,NULL
    },
    {
        608, // printer-organizational-unit
        0,0
        NULL,NULL
    },
    {
        610, // printer-privacy-policy-uri
        0,0
        NULL,NULL
    },
    {
        613, // printer-resolution-default
        0,0
        NULL,NULL
    },
    {
        614, // printer-resolution-supported
        0,0
        NULL,NULL
    },
    {
        621, // printer-static-resource-directory-uri
        0,0
        NULL,NULL
    },
    {
        623, // printer-static-resource-k-octets-supported
        0,0
        NULL,NULL
    },
    {
        624, // printer-strings-languages-supported
        0,0
        NULL,NULL
    },
    {
        625, // printer-strings-uri
        0,0
        NULL,NULL
    },
    {
        633, // printer-volume-supported
        0,0
        NULL,NULL
    },
    {
        634, // printer-xri-supported
        0,0
        NULL,NULL
    },
    {
        637, // proof-print-default
        0,0
        NULL,NULL
    },
    {
        638, // proof-print-supported
        0,0
        NULL,NULL
    },
    {
        639, // punching-hole-diameter-configured
        0,0
        NULL,NULL
    },
    {
        640, // punching-locations-supported
        0,0
        NULL,NULL
    },
    {
        641, // punching-offset-supported
        0,0
        NULL,NULL
    },
    {
        642, // punching-reference-edge-supported
        0,0
        NULL,NULL
    },
    {
        643, // pwg-raster-document-resolution-supported
        0,0
        NULL,NULL
    },
    {
        644, // pwg-raster-document-sheet-back
        0,0
        NULL,NULL
    },
    {
        645, // pwg-raster-document-type-supported
        0,0
        NULL,NULL
    },
    {
        647, // reference-uri-schemes-supported
        0,0
        NULL,NULL
    },
    {
        648, // repertoire-supported
        0,0
        NULL,NULL
    },
    {
        652, // requesting-user-uri-supported
        0,0
        NULL,NULL
    },
    {
        654, // retry-interval-default
        0,0
        NULL,NULL
    },
    {
        655, // retry-interval-supported
        0,0
        NULL,NULL
    },
    {
        657, // retry-time-out-default
        0,0
        NULL,NULL
    },
    {
        658, // retry-time-out-supported
        0,0
        NULL,NULL
    },
    {
        659, // save-disposition-supported
        0,0
        NULL,NULL
    },
    {
        660, // save-document-format-default
        0,0
        NULL,NULL
    },
    {
        661, // save-document-format-supported
        0,0
        NULL,NULL
    },
    {
        662, // save-location-default
        0,0
        NULL,NULL
    },
    {
        663, // save-location-supported
        0,0
        NULL,NULL
    },
    {
        664, // save-name-subdirectory-supported
        0,0
        NULL,NULL
    },
    {
        665, // save-name-supported
        0,0
        NULL,NULL
    },
    {
        668, // separator-sheets-default
        0,0
        NULL,NULL
    },
    {
        669, // separator-sheets-supported
        0,0
        NULL,NULL
    },
    {
        674, // sheet-collate-default
        0,0
        NULL,NULL
    },
    {
        675, // sheet-collate-default(deprecated)
        0,0
        NULL,NULL
    },
    {
        676, // sheet-collate-supported
        0,0
        NULL,NULL
    },
    {
        677, // sheet-collate-supported(deprecated)
        0,0
        NULL,NULL
    },
    {
        684, // sides-default
        0,0
        NULL,NULL
    },
    {
        685, // sides-supported
        0,0
        NULL,NULL
    },
    {
        687, // stitching-angle-supported
        0,0
        NULL,NULL
    },
    {
        688, // stitching-locations-supported
        0,0
        NULL,NULL
    },
    {
        689, // stitching-method-supported
        0,0
        NULL,NULL
    },
    {
        690, // stitching-offset-supported
        0,0
        NULL,NULL
    },
    {
        691, // stitching-reference-edge-supported
        0,0
        NULL,NULL
    },
    {
        692, // subject-supported
        0,0
        NULL,NULL
    },
    {
        693, // subordinate-printers-supported
        0,0
        NULL,NULL
    },
    {
        694, // subscription-privacy-attributes
        0,0
        NULL,NULL
    },
    {
        695, // subscription-privacy-scope
        0,0
        NULL,NULL
    },
    {
        699, // to-name-supported
        0,0
        NULL,NULL
    },
    {
        700, // trimming-offset-supported
        0,0
        NULL,NULL
    },
    {
        701, // trimming-reference-edge-supported
        0,0
        NULL,NULL
    },
    {
        702, // trimming-type-supported
        0,0
        NULL,NULL
    },
    {
        703, // trimming-when-supported
        0,0
        NULL,NULL
    },
    {
        704, // uri-authentication-supported
        0,0
        NULL,NULL
    },
    {
        705, // uri-security-supported
        0,0
        NULL,NULL
    },
    {
        706, // user-defined-values-supported
        0,0
        NULL,NULL
    },
    {
        709, // which-jobs-supported
        0,0
        NULL,NULL
    },
    {
        712, // x-image-position-default
        0,0
        NULL,NULL
    },
    {
        713, // x-image-position-supported
        0,0
        NULL,NULL
    },
    {
        716, // x-image-shift-default
        0,0
        NULL,NULL
    },
    {
        717, // x-image-shift-supported
        0,0
        NULL,NULL
    },
    {
        720, // x-side1-image-shift-default
        0,0
        NULL,NULL
    },
    {
        721, // x-side1-image-shift-supported
        0,0
        NULL,NULL
    },
    {
        724, // x-side2-image-shift-default
        0,0
        NULL,NULL
    },
    {
        725, // x-side2-image-shift-supported
        0,0
        NULL,NULL
    },
    {
        731, // y-image-position-default
        0,0
        NULL,NULL
    },
    {
        732, // y-image-position-supported
        0,0
        NULL,NULL
    },
    {
        735, // y-image-shift-default
        0,0
        NULL,NULL
    },
    {
        736, // y-image-shift-supported
        0,0
        NULL,NULL
    },
    {
        739, // y-side1-image-shift-default
        0,0
        NULL,NULL
    },
    {
        740, // y-side1-image-shift-supported
        0,0
        NULL,NULL
    },
    {
        743, // y-side2-image-shift-default
        0,0
        NULL,NULL
    },
    {
        744, // y-side2-image-shift-supported
        0,0
        NULL,NULL
    },
};

ipp_attr_t s_ipp_attr_group_prtstat[] =
{
    {
        49, // device-service-count
        0,0
        NULL,NULL
    },
    {
        50, // device-uuid
        0,0
        NULL,NULL
    },
    {
        79, // document-format-varying-attributes
        0,0
        NULL,NULL
    },
    {
        310, // job-settable-attributes-supported
        0,0
        NULL,NULL
    },
    {
        504, // pages-per-minute
        0,0
        NULL,NULL
    },
    {
        505, // pages-per-minute-color
        0,0
        NULL,NULL
    },
    {
        570, // printer-alert
        0,0
        NULL,NULL
    },
    {
        571, // printer-alert-description
        0,0
        NULL,NULL
    },
    {
        575, // printer-config-change-date-time
        0,0
        NULL,NULL
    },
    {
        576, // printer-config-change-time
        0,0
        NULL,NULL
    },
    {
        578, // printer-detailed-status-messages
        0,0
        NULL,NULL
    },
    {
        586, // printer-finisher
        0,0
        NULL,NULL
    },
    {
        587, // printer-finisher-description
        0,0
        NULL,NULL
    },
    {
        588, // printer-finisher-supplies
        0,0
        NULL,NULL
    },
    {
        589, // printer-finisher-supplies-description
        0,0
        NULL,NULL
    },
    {
        595, // printer-input-tray
        0,0
        NULL,NULL
    },
    {
        596, // printer-is-accepting-jobs
        0,0
        NULL,NULL
    },
    {
        601, // printer-message-date-time
        0,0
        NULL,NULL
    },
    {
        602, // printer-message-from-operator
        0,0
        NULL,NULL
    },
    {
        603, // printer-message-time
        0,0
        NULL,NULL
    },
    {
        604, // printer-more-info
        0,0
        NULL,NULL
    },
    {
        609, // printer-output-tray
        0,0
        NULL,NULL
    },
    {
        615, // printer-settable-attributes-supported
        0,0
        NULL,NULL
    },
    {
        616, // printer-state
        0,0
        NULL,NULL
    },
    {
        617, // printer-state-change-date-time
        0,0
        NULL,NULL
    },
    {
        618, // printer-state-change-time
        0,0
        NULL,NULL
    },
    {
        619, // printer-state-message
        0,0
        NULL,NULL
    },
    {
        620, // printer-state-reasons
        0,0
        NULL,NULL
    },
    {
        622, // printer-static-resource-k-octets-free
        0,0
        NULL,NULL
    },
    {
        626, // printer-supply
        0,0
        NULL,NULL
    },
    {
        627, // printer-supply-description
        0,0
        NULL,NULL
    },
    {
        628, // printer-supply-info-uri
        0,0
        NULL,NULL
    },
    {
        629, // printer-up-time
        0,0
        NULL,NULL
    },
    {
        631, // printer-uri-supported
        0,0
        NULL,NULL
    },
    {
        632, // printer-uuid
        0,0
        NULL,NULL
    },
    {
        646, // queued-job-count
        0,0
        NULL,NULL
    },
    {
        726, // xri-authentication-supported
        0,0
        NULL,NULL
    },
    {
        727, // xri-security-supported
        0,0
        NULL,NULL
    },
    {
        728, // xri-uri-scheme-supported
        0,0
        NULL,NULL
    },
};

ipp_attr_t s_ipp_attr_group_substat[] =
{
    {
        420, // notify-job-id
        0,0
        NULL,NULL
    },
    {
        424, // notify-lease-expiration-time
        0,0
        NULL,NULL
    },
    {
        427, // notify-printer-up-time
        0,0
        NULL,NULL
    },
    {
        428, // notify-printer-uri
        0,0
        NULL,NULL
    },
    {
        433, // notify-sequence-number
        0,0
        NULL,NULL
    },
    {
        435, // notify-status-code
        0,0
        NULL,NULL
    },
    {
        437, // notify-subscriber-user-name
        0,0
        NULL,NULL
    },
    {
        438, // notify-subscriber-user-uri
        0,0
        NULL,NULL
    },
    {
        439, // notify-subscription-id
        0,0
        NULL,NULL
    },
    {
        441, // notify-subscription-uuid
        0,0
        NULL,NULL
    },
};

struct tag_grp_xref
{
    char           *group_pname;
    ipp_attr_t     *group_attrs;
    size_t          num_attr;
}
s_ipp_group_xref[] =
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
};

