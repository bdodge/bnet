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
#ifndef BUTIL_H
#define BUTIL_H 1

#include "bnetheaders.h"

/// Is multipart mime supported
/// define this as 0 to save space if needed
//
#ifndef BUTIL_SUPPORT_MULTIPART
#define BUTIL_SUPPORT_MULTIPART (1)
#endif

/// Support a larger set of mime types (more strings space)
//
#ifndef BUTIL_SUPPORT_EXTENDED_MIME_TYPES
#define BUTIL_SUPPORT_EXTENDED_MIME_TYPES (1)
#endif

#define BUTIL_MAX_URL_SCHEME 8
#define BUTIL_MAX_PORTSPEC   5

typedef enum
{
    butil_mime_bin,
    butil_mime_css,
    butil_mime_gif,
    butil_mime_html,
    butil_mime_ico,
    butil_mime_jpeg,
    butil_mime_jar,
    butil_mime_js,
    butil_mime_json,
    butil_mime_text,
    butil_mime_tiff,
    butil_mime_xml,
    butil_mime_xwwwformurl,
#if BUTIL_SUPPORT_MULTIPART
    butil_mime_multi,
#endif
#if BUTIL_SUPPORT_EXTENDED_MIME_TYPES
    butil_mime_aac,
    butil_mime_abw,
    butil_mime_arc,
    butil_mime_avi,
    butil_mime_azw,
    butil_mime_bz,
    butil_mime_bz2,
    butil_mime_csh,
    butil_mime_csv,
    butil_mime_doc,
    butil_mime_eot,
    butil_mime_epub,
    butil_mime_ics,
    butil_mime_midi,
    butil_mime_mpeg,
    butil_mime_mpkg,
    butil_mime_odp,
    butil_mime_ods,
    butil_mime_odt,
    butil_mime_oga,
    butil_mime_ogv,
    butil_mime_ogx,
    butil_mime_otf,
    butil_mime_png,
    butil_mime_pdf,
    butil_mime_ppt,
    butil_mime_rar,
    butil_mime_rtf,
    butil_mime_sh,
    butil_mime_svg,
    butil_mime_swf,
    butil_mime_tar,
    butil_mime_ts,
    butil_mime_ttf,
    butil_mime_vsd,
    butil_mime_wav,
    butil_mime_weba,
    butil_mime_webm,
    butil_mime_webp,
    butil_mime_woff,
    butil_mime_woff2,
    butil_mime_xhtml,
    butil_mime_xls,
    butil_mime_xlsx,
    butil_mime_xul,
    butil_mime_zip,
    butil_mime_3gp,
    butil_mime_3g2,
    butil_mime_7z,
#endif
}
mime_content_type_t;

#define BUTIL_DEFAULT_MIME "text/plain"

void butil_log              (uint32_t level, const char *fmt, ...);
void butil_set_log_level    (uint32_t level);
uint32_t butil_get_log_level(void);

int butil_hextou            (char digit, uint8_t *val);
int butil_is_white          (char ch);
int butil_is_number         (char ch);

size_t butil_utf8_encode    (uint32_t unicode, uint8_t utfbuf[5]);
size_t butil_utf8_decode    (uint8_t *utfbuf, size_t nutf, uint32_t *unicode);

int butil_base64_decode     (
                            uint8_t        *out,
                            size_t          outsize,
                            const char     *src
                            );

int butil_base64_encode     (
                            char           *out,
                            size_t          outsize,
                            const uint8_t  *src,
                            size_t          srcbytes,
                            bool            urlencode,
                            bool            hexescape
                            );

typedef enum
{
    schemeFILE,
    schemeFTP,
    schemeSFTP,
    schemeHTTP,
    schemeHTTPS,
    schemeWS,
    schemeWSS,
    schemeSIP,
    schemeSIPS,
    schemeMAILTO,
    schemeSSH,
    schemeDAV,
    schemeUSER1,
    schemeUSER2,
    schemeUSER3,
    schemeUSER4
}
butil_url_scheme_t;

#define BUTIL_FIRST_USER_SCHEME ((int)schemeUSER1)
#define BUTIL_NUM_USER_SCHEMES  ((int)schemeUSER4 - (int)schemeUSER1 + 1)

const char *butil_scheme_name(
                            butil_url_scheme_t scheme
                            );

int butil_scheme_from_name  (
                            const char         *name,
                            butil_url_scheme_t *scheme
                            );

int butil_register_scheme   (
                            const char         *name,
                            butil_url_scheme_t *scheme
                            );

int butil_parse_url         (
                            const char         *url,
                            butil_url_scheme_t *scheme,
                            char               *host,
                            size_t              nhost,
                            uint16_t           *port,
                            char               *path,
                            size_t              npath
                            );

int butil_paste_url         (
                            char               *url,
                            size_t              nurl,
                            const butil_url_scheme_t scheme,
                            const char         *host,
                            const uint16_t      port,
                            const char         *path
                            );


int butil_ncasecmp           (const char *haystack, const char *needle);

time_t butil_rfc2616_date_to_time(const char *date);
const char *butil_time_to_rfc2616_date(time_t when, char *buf, size_t nbuf);

const char *butil_mime_string_for_file(const char *path);
mime_content_type_t butil_content_type_for_file(const char *path);

const char *butil_mime_string_for_content_type(mime_content_type_t type);
mime_content_type_t butil_content_type_for_mime_string(const char *mime);

const char *butil_str_for_ipv4(uint32_t ipv4addr, char *ipbuf, size_t nipbuf);
const char *butil_str_for_ipv6(bipv6addr_t *ipv6addr, char *ipbuf, size_t nipbuf);

int butil_get_host_info     (
                            char* myhost,
                            int nhost,
                            bipv4addr_t *myipv4addr,
                            bipv6addr_t *myipv6addr,
                            int *iface_index
                            );

#endif

