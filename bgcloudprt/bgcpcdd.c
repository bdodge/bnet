/*
 * Copyright 2019 Brian Dodge
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "bgcpcdd.h"
#include "bgcp.h"

static const char *s_printer_cdd =
"{\r\n"
"  \"version\": \"1.0\",\r\n"
"  \"printer\": {\r\n"
"    \"supported_content_type\": [\r\n"
"        {\r\n"
"          \"content_type\": \"image/pwg-raster\"\r\n"
"        },\r\n"
"        {\r\n"
"          \"content_type\": \"image/jpeg\"\r\n"
"        }\r\n"
"    ],\r\n"
"    \"pwg_raster_config\": {\r\n"
"      \"document_resolution_supported\": [\r\n"
"        {\r\n"
"          \"cross_feed_dir\": 300,\r\n"
"          \"feed_dir\": 300\r\n"
"        },\r\n"
"        {\r\n"
"          \"cross_feed_dir\": 600,\r\n"
"          \"feed_dir\": 600\r\n"
"        }\r\n"
"      ],\r\n"
"      \"document_type_supported\": [\r\n"
"       \"ADOBE_RGB_8\",\r\n"
"       \"RGB_8\",\r\n"
"       \"SGRAY_8\",\r\n"
"       \"SRGB_8\"\r\n"
"      ]\r\n"
"    },\r\n"
"    \"color\": {\r\n"
"      \"option\": [\r\n"
"        {\r\n"
"          \"vendor_id\": \"grayscale\",\r\n"
"          \"type\": \"STANDARD_MONOCHROME\"\r\n"
"        },\r\n"
"        {\r\n"
"          \"vendor_id\": \"color\",\r\n"
"          \"type\": \"STANDARD_COLOR\",\r\n"
"          \"is_default\": true\r\n"
"        }\r\n"
"      ]\r\n"
"    },\r\n"
"    \"copies\": {\r\n"
"      \"default\": 1,\r\n"
"      \"max\": 99\r\n"
"    },\r\n"
"    \"collate\": {\r\n"
"      \"default\": true\r\n"
"    },\r\n"
"    \"page_orientation\": {\r\n"
"      \"option\": [\r\n"
"        {\r\n"
"            \"type\": \"PORTRAIT\",\r\n"
"            \"is_default\": true\r\n"
"        },\r\n"
"        {\r\n"
"            \"type\": \"LANDSCAPE\"\r\n"
"        }\r\n"
"      ]\r\n"
"    },\r\n"
"    \"media_size\": {\r\n"
"      \"option\": [\r\n"
"        {\r\n"
"          \"name\": \"NA_LETTER\",\r\n"
"          \"width_microns\": 215900,\r\n"
"          \"height_microns\": 279400,\r\n"
"          \"is_default\": true\r\n"
"        }\r\n"
"      ]\r\n"
"    },\r\n"
"    \"media_path\": [{\r\n"
"        \"vendor_id\": \"fileout\"\r\n"
"    }]\r\n"
"  }\r\n"
"}\r\n"
;

static const char *s_printer_cds =
"{\r\n"
"  \"version\": \"1.0\",\r\n"
"  \"printer\":\r\n"
"  {\r\n"
"    \"state\": \"%s\",\r\n"
"%s"
"  }\r\n"
"}\r\n";


int gcp_format_cds(gcp_context_t *gcp)
{
    int len;

    len = snprintf(gcp->cds, sizeof(gcp->cds), s_printer_cds, "IDLE", "");
    if (len < 0)
    {
        BERROR("CDS Overflow");
        return -1;
    }
    return 0;
}

int gcp_format_cdd(gcp_context_t *gcp)
{
    int len;

    len = snprintf(gcp->cdd, sizeof(gcp->cdd), "%s", s_printer_cdd);
    if (len < 0)
    {
        BERROR("CDD Overflow");
        return -1;
    }
    return 0;
}

