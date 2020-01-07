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
#include "butil.h"

static uint32_t s_log_level;

void butil_log(uint32_t level, const char *fmt, ...)
{
    va_list args;

    if (level > s_log_level)
    {
        return;
    }
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void butil_set_log_level(uint32_t newlevel)
{
    s_log_level = newlevel;
}

int butil_hextou(char digit, uint8_t *val)
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

int butil_is_white(char ch)
{
    return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n');
}

int butil_is_number(char ch)
{
    return (ch >= '0') && (ch <= '9');
}

size_t butil_utf8_encode(uint32_t unicode, uint8_t utfbuf[5])
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

int butil_base64_decode(uint8_t *out, size_t outsize, const char *src)
{
    uint8_t *base = out;
    uint32_t bits, c, v, outchars;
    size_t errors = 0;

    outchars = 0;
    bits = 0;

    while ((c = *src++) != '\0')
    {
        if (outsize < 4)
        {
            errors++;
            break;
        }
        if (c >= 'A' && c <= 'Z')
        {
            v = c - 'A';
        }
        else if (c >= 'a' && c <= 'z')
        {
            v = c - 'a' + 26;
        }
        else if (c >= '0' && c <= '9')
        {
            v = c - '0' + 52;
        }
        else if (c == '+' || c == '-')
        {
            v = 62;
        }
        else if (c == '/' || c == '_')
        {
            v = 63;
        }
        else
        {
            break;
        }
        bits += v;
        outchars++;

        if (outchars == 4)
        {
            *out++ = (bits >> 16);
            *out++ = (bits >> 8) & 0xff;
            *out++ = (bits & 0xff);
            bits = 0;
            outchars = 0;
        }
        else
        {
            bits <<= 6;
        }
    }
    switch(outchars)
    {
    case 1:
        errors++;
        break;
    case 2:
        if (outsize < 2)
        {
            errors++;
            break;
        }
        *out++ = (bits >> 10);
        break;
    case 3:
        if (outsize < 3)
        {
            errors++;
            break;
        }
        *out++ = (bits >> 16);
        *out++ = (bits >> 8) & 0xff;
        break;
    }
    *out = '\0';
    return errors ? -1 : (out - base);
}

static inline int base64_hexencode_byte(char *out, uint8_t byte)
{
    if (byte == '+')
    {
        *out++ = '%';
        *out++ = '2';
        *out++ = 'B';
        return 3;
    }
    else if(byte == '/')
    {
        *out++ = '%';
        *out++ = '2';
        *out++ = 'F';
        return 3;
    }
    else if(byte == '=')
    {
        *out++ = '%';
        *out++ = '3';
        *out++ = 'D';
        return 3;
    }
    else
    {
        *out++ = byte;
        return 1;
    }
}

int butil_base64_encode(
                      char           *out,
                      size_t          outsize,
                      const uint8_t  *src,
                      size_t          srcbytes,
                      bool            urlencode,
                      bool            hexescape
                    )
{
    char *base = out;
    const char *alphabet;
    uint8_t  b1, b2, b3;
    uint32_t d;
    size_t j = 0, k = 0;
    int i;

    static const char *s_alphabet64_base =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static const char *s_alphabet64_url =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    if (urlencode)
    {
        alphabet = s_alphabet64_url;
    }
    else
    {
        alphabet = s_alphabet64_base;
    }
    while (j < (outsize - 4) && k < srcbytes)
    {
        b1 = src[k];
        b2 = (k+1 < srcbytes) ? src[k+1] : 0;
        b3 = (k+2 < srcbytes) ? src[k+2] : 0;

        d = (b1 << 16) | (b2 << 8) | b3;

        if(hexescape)
        {
            char b;

            b = alphabet[(d>>18)];
            i = base64_hexencode_byte(out, b);
            j += i;
            out += i;

            b = alphabet[(d>>12) & 0x3f];
            i = base64_hexencode_byte(out, b);
            j += i;
            out += i;

            if (k+1 < srcbytes)
            {
                b = alphabet[(d>>6) & 0x3f];
                i = base64_hexencode_byte(out, b);
                j += i;
                out += i;
            }
            else {
                i = base64_hexencode_byte(out, '=');
                j += i;
                out += i;
            }
            if(k+2 < srcbytes)
            {
                b = alphabet[(d) & 0x3f];
                i = base64_hexencode_byte(out, b);
                j += i;
                out += i;
            }
            else {
                i = base64_hexencode_byte(out, '=');
                j += i;
                out += i;
            }
        }
        else {
            *out++ = alphabet[((d>>18)        )];
            *out++ = alphabet[((d>>12) & 0x3f )];

          if (k + 1 < srcbytes)
          {
              *out++ = alphabet[((d>> 6) & 0x3f )];
          }
          else if (! urlencode)
          {
              *out++ = '=';
          }
          if (k + 2 < srcbytes)
          {
              *out++ = alphabet[((d) & 0x3f )];
          }
          else if (! urlencode)
          {
              *out++ = '=';
          }
            j += 4;
        }
        k+= 3;
    }
    *out = '\0';
    return (k < srcbytes) ? -1 : (out - base);
}

static struct tag_user_scheme
{
    char name[BUTIL_MAX_URL_SCHEME];
}
s_user_schemes[BUTIL_NUM_USER_SCHEMES] =
{
    { "" }
};

int butil_register_scheme   (
                            const char         *name,
                            butil_url_scheme_t *scheme
                            )
{
    size_t scheme_num;

    for (scheme_num = 0; scheme_num < BUTIL_NUM_USER_SCHEMES; scheme_num++)
    {
        if (! s_user_schemes[scheme_num].name[0])
        {
            *scheme = scheme_num + BUTIL_FIRST_USER_SCHEME;
            strncpy(s_user_schemes[scheme_num].name, name, BUTIL_MAX_URL_SCHEME - 1);
            s_user_schemes[scheme_num].name[BUTIL_MAX_URL_SCHEME - 1] = '\0';
            return 0;
        }
    }
    return -1;
}

const char *butil_scheme_name(butil_url_scheme_t scheme)
{
    size_t scheme_num;

    switch (scheme)
    {
    case schemeFTP:     return "FTP";
    case schemeSFTP:    return "SFTP";
    case schemeHTTP:    return "HTTP";
    case schemeHTTPS:   return "HTTPS";
    case schemeWS:      return "WS";
    case schemeWSS:     return "WSS";
    case schemeSIP:     return "SIP";
    case schemeSIPS:    return "SIPS";
    case schemeMAILTO:  return "MAILTO";
    default:
        scheme_num = scheme - BUTIL_FIRST_USER_SCHEME;
        if (scheme_num < BUTIL_NUM_USER_SCHEMES)
        {
            if (s_user_schemes[scheme_num].name[0])
            {
                return s_user_schemes[scheme_num].name;
            }
        }
    }
    return "????";
}

int butil_scheme_from_name(const char *name, butil_url_scheme_t *scheme)
{
    size_t scheme_num;

    if (! strcasecmp(name, "https"))
    {
        *scheme = schemeHTTPS;
        return 0;
    }
    if (! strcasecmp(name, "http"))
    {
        *scheme = schemeHTTP;
        return 0;
    }
    if (! strcasecmp(name, "sftp"))
    {
        *scheme = schemeSFTP;
        return 0;
    }
    if (! strcasecmp(name, "ftp"))
    {
        *scheme = schemeFTP;
        return 0;
    }
    if (! strcasecmp(name, "wss"))
    {
        *scheme = schemeWSS;
        return 0;
    }
    if (! strcasecmp(name, "ws"))
    {
        *scheme = schemeWS;
        return 0;
    }
    if (! strcasecmp(name, "mailto"))
    {
        *scheme = schemeMAILTO;
        return 0;
    }
    for (scheme_num = 0; scheme_num < BUTIL_NUM_USER_SCHEMES; scheme_num++)
    {
        if (s_user_schemes[scheme_num].name[0])
        {
            if (! strncasecmp(name, s_user_schemes[scheme_num].name,
                    strlen(s_user_schemes[scheme_num].name)))
            {
                *scheme = scheme_num + BUTIL_FIRST_USER_SCHEME;
                return 0;
            }
        }
    }
    return -1;
}

int butil_parse_url(
                    const char         *url,
                    butil_url_scheme_t *scheme,
                    char               *host,
                    size_t              nhost,
                    uint16_t           *port,
                    char               *path,
                    size_t              npath
                  )
{
    char schemestr[BUTIL_MAX_URL_SCHEME];
    butil_url_scheme_t localscheme;
    const char *ps, *pe;
    uint16_t portnum = 80;
    size_t len;

    if (scheme)
    {
        *scheme = schemeHTTP;
    }
    if (host)
    {
        *host = '\0';
    }
    if (port)
    {
        *port = portnum;
    }
    if (path)
    {
        *path = '\0';
    }
    if (! url)
    {
        BERROR("Invalid Parameter");
        return -1;
    }
    // search forward for ://
    ps = url;
    pe = strstr(ps, "://");
    if (pe)
    {
        // schemem present
        //
        len = pe - ps;
        pe += 3;
    }
    else
    {
        // no scheme found, assume http
        //
        pe = strstr(ps, "//");
        if (pe)
        {
            pe+= 2;
        }
        else
        {
            pe = ps;
        }
        ps = "http";
        len = 4;
    }
    if (len >= BUTIL_MAX_URL_SCHEME)
    {
        BERROR("Scheme too long");
        return -1;
    }
    if (! scheme)
    {
        scheme = &localscheme;
    }
    strncpy(schemestr, ps, len);
    schemestr[len] = '\0';

    if (butil_scheme_from_name(schemestr, scheme))
    {
        BERROR("Unknown Scheme");
        return -1;
    }
    // set default port based on scheme
    //
    switch (*scheme)
    {
    case schemeFTP:
        portnum = 21;
        break;
    case schemeSFTP:
        portnum = 23;
        break;
    case schemeHTTPS:
        portnum = 443;
        break;
    case schemeSIP:
        portnum = 5061;
        break;
    case schemeSIPS:
        portnum = 5062;
        break;
    case schemeMAILTO:
        portnum = 25;
        break;
    case schemeHTTP:
    default:
        portnum = 80;
        break;
    }
    // extract hostname
    ps = pe;
    pe = strchr(ps, ':');
    if (! pe)
    {
        pe = strchr(ps, '/');
        if (! pe)
        {
            // remainder of string is hostname
            pe = ps + strlen(ps);
        }
    }
    len = pe - ps;
    if (len >= nhost)
    {
        BERROR("Hostname too long");
        return -1;
    }
    if (host)
    {
        strncpy(host, ps, len);
        host[len] = '\0';
    }
    ps = pe;
    if (*ps == ':')
    {
        // port is specified
        portnum = (uint16_t)strtoul(ps + 1, (char **)&pe, 10);
        len = pe - ps;
        if (len > BUTIL_MAX_PORTSPEC)
        {
            BERROR("Malformed port number");
            return -1;
        }
        ps = pe;
    }
    if (port)
    {
        *port = portnum;
    }
    if (*ps != '/' && *ps)
    {
        BERROR("Malformed path");
        return -1;
    }
    len = strlen(ps);
    if (len >= npath)
    {
        BERROR("Path too long");
        return -1;
    }
    if (! len)
    {
        ps = "/";
        len = 1;
    }
    if (path && len)
    {
        strncpy(path, ps, len);
        path[len] = '\0';
    }
    return 0;
}


const char *butil_str_for_ipv4(uint32_t ipv4addr, char *ipbuf, size_t nipbuf)
{
    snprintf(ipbuf, nipbuf, "%d.%d.%d.%d",
                (ipv4addr & 0xFF),
                ((ipv4addr >> 8) & 0xFF),
                ((ipv4addr >> 16) & 0xFF),
                ((ipv4addr >> 24) & 0xFF)
            );
    return ipbuf;
}

const char *butil_str_for_ipv6(bipv6addr_t *ipv6addr, char *ipbuf, size_t nipbuf)
{
    uint16_t addr[8];
    uint16_t *sa;
    int i;

    sa = ipv6addr->addr;
    for (i = 0; i < 8; i++)
    {
        addr[i] = htons(sa[i]);
    }
    snprintf(ipbuf, nipbuf, "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7]);
    return ipbuf;
}

int butil_paste_url(
                   char               *url,
                   size_t              nurl,
                   const butil_url_scheme_t scheme,
                   const char         *host,
                   const uint16_t      port,
                   const char         *path
                  )
{
    size_t len;
    uint16_t useport = port;
    const char *schemestr;
    char schemelc[BUTIL_MAX_URL_SCHEME];
    char *ps;

    if (! url)
    {
        BERROR("Invalid Parameter");
        return -1;
    }
    schemestr = butil_scheme_name(scheme);
    if (! schemestr)
    {
        BERROR("Invalid Scheme");
        return -1;
    }
    // convert scheme name to lower case to be consistent with url schemes
    //
    strncpy(schemelc, schemestr, sizeof(schemelc) - 1);
    schemelc[sizeof(schemelc) - 1] = '\0';
    for (ps = schemelc; *ps; ps++)
    {
        if (*ps >= 'A' && *ps <= 'Z')
        {
            *ps = *ps - 'A' + 'a';
        }
    }
    if (! host)
    {
        BERROR("Missing Hostname");
        return -1;
    }
    if (! useport)
    {
        useport = strcasecmp(schemelc, "https") ? 443 : 80;
    }
    if (! path)
    {
        path = "";
    }
    if (*path == '/')
    {
        path++;
    }
    len = snprintf(url, nurl,
            "%s://%s:%u/%s", schemelc, host, useport, path);
    if (len < 0 || len >= nurl)
    {
        BERROR("Pasted url too long");
        return -1;
    }
    return 0;
}

#if ! defined(__GNUC__) || defined(_WIN32)
struct tm *gmtime_r(time_t *timeval, struct tm *stm)
{
    struct tm *ptm;

    ptm = gmtime(timeval);
    *stm = *ptm;
    return stm;
}
#endif

time_t butil_rfc2616_date_to_time(const char *date)
{
    int day, year, monthdex, hh, mm, ss;
    struct tm when;
    char dayofweek[8];
    char month[8];
    char zone[8];
    time_t converted;
    time_t gmtoff;
    time_t err_time;
    struct tm lct, gmt;
    int sret;

    err_time = (time_t)0;

    // expect string like: "Tue, 15 Nov 1994 08:12:31 GMT"
    //
    if (! date || ! *date)
    {
        return err_time;
    }
    // get time for the end of day number 2 of epoch
    converted = 48 * 3600;

    // convert to local time
    memcpy(&lct, localtime(&converted), sizeof(struct tm));

    // and convert to gmtime
    gmtime_r(&converted, &gmt);

    // calculate seconds in each time and get the difference between gmt and local time
    //
    gmtoff  = gmt.tm_mday * 24 * 3600 + gmt.tm_hour * 3600 + gmt.tm_min * 60 + gmt.tm_sec;
    gmtoff -= lct.tm_mday * 24 * 3600 + lct.tm_hour * 3600 + lct.tm_min * 60 + lct.tm_sec;

    sret = sscanf(date, "%3s, %d %3s %d %d:%d:%d %3s",
              dayofweek, &day, month, &year, &hh, &mm, &ss, zone);

    if (sret < 0)
    {
        return err_time;
    }
    if (day < 0 || day > 31)
    {
        return err_time;
    }
    if (hh < 0 || hh > 23)
    {
        return err_time;
    }
    if (mm < 0 || mm > 59)
    {
        return err_time;
    }
    if (ss < 0 || ss > 59)
    {
        return err_time;
    }
    switch (month[0])
    {
    case 'J': case 'j':
        if (month[1] == 'a' || month[1] == 'A')
        {
            if (month[2] == 'n' || month[2] == 'N')
            {
                monthdex = 0;
                break;
            }
        }
        else
        {
            if (month[2] == 'n' || month[2] == 'N')
            {
                monthdex = 5;
                break;
            }
            else if (month[2] == 'l' || month[2] == 'L')
            {
                monthdex = 6;
                break;
            }
        }
        return err_time;
    case 'F': case 'f':
        if (month[1] == 'e' || month[1] == 'E')
        {
            if (month[2] == 'b' || month[2] == 'B')
            {
                monthdex = 1;
                break;
            }
        }
        return err_time;
    case 'M': case 'm':
        if (month[1] == 'a' || month[1] == 'A')
        {
            if (month[2] == 'r' || month[2] == 'R')
            {
                monthdex = 2;
                break;
            }
            if (month[2] == 'y' || month[2] == 'Y')
            {
                monthdex = 4;
                break;
            }
        }
        return err_time;
    case 'A': case 'a':
        if (month[1] == 'u' || month[1] == 'U')
        {
            if (month[2] == 'g' || month[2] == 'G')
            {
                monthdex = 7;
                break;
            }
        }
        if (month[1] == 'p' || month[1] == 'P')
        {
            if (month[2] == 'r' || month[2] == 'R')
            {
                monthdex = 3;
                break;
            }
        }
        return err_time;
    case 'S': case 's':
        if (month[1] == 'e' || month[1] == 'E')
        {
            if (month[2] == 'p' || month[2] == 'P')
            {
                monthdex = 8;
                break;
            }
        }
        return err_time;
    case 'O': case 'o':
        if (month[1] == 'c' || month[1] == 'C')
        {
            if (month[2] == 't' || month[2] == 'T')
            {
                monthdex = 9;
                break;
            }
        }
        return err_time;
    case 'N': case 'n':
        if (month[1] == 'o' || month[1] == 'O')
        {
            if (month[2] == 'v' || month[2] == 'V')
            {
                monthdex = 10;
                break;
            }
        }
        return err_time;
    case 'D': case 'd':
        if (month[1] == 'e' || month[1] == 'E')
        {
            if (month[2] == 'c' || month[2] == 'C')
            {
                monthdex = 11;
                break;
            }
        }
        return err_time;
    default:
        return err_time;
    }
    memset(&when, 0, sizeof(when));

    when.tm_year = year - 1900;
    when.tm_mday = day;
    when.tm_mon  = monthdex;

    when.tm_hour = hh;
    when.tm_min  = mm;
    when.tm_sec  = ss;

    converted = mktime(&when);

    // remove local timezone from converted to make it gmt
    converted -= gmtoff;

    return converted;
}

const char *butil_time_to_rfc2616_date(time_t when, char *buf, size_t nbuf)
{
    struct tm tm, *ptm;
    const char *bad_date = "Mon, 1 Jan 2015 00:00:00 GMT";

    if (! buf || ! nbuf) {
        return bad_date;
    }
    *buf = '\0';
    ptm = gmtime_r(&when, &tm);
    if (ptm) {
        int len = strftime(buf, nbuf, "%a, %d %b %Y %H:%M:%S GMT", ptm);
        if (len <= 0) {
            // if strftime fails
            return bad_date;
        }
    } else {
        return bad_date;
    }
    return buf;
}

typedef struct tag_mime_tab
{
    mime_content_type_t type;
    const char         *ext;
    const char         *mime;
}
butil_mime_entry_t;

static butil_mime_entry_t s_mimetab[] =
{
    { butil_mime_bin,  ".bin",   "application/octet-stream" },
    { butil_mime_css,  ".css",   "text/css" },
    { butil_mime_gif,  ".gif",   "image/gif" },
    { butil_mime_html, ".htm",   "text/html" },
    { butil_mime_html, ".html",  "text/html" },
    { butil_mime_ico,  ".ico",   "image/x-icon" },
    { butil_mime_jar,  ".jar",   "application/java-archive" },
    { butil_mime_jpeg, ".jpeg",  "image/jpeg" },
    { butil_mime_jpeg, ".jpg",   "image/jpeg" },
    { butil_mime_js,   ".js",    "application/javascript" },
    { butil_mime_json, ".json",  "application/json" },
    { butil_mime_text, ".text",  "text/plain" },
    { butil_mime_text, ".txt",   "text/plain" },
    { butil_mime_tiff, ".tif",   "image/tiff" },
    { butil_mime_tiff, ".tiff",  "image/tiff" },
    { butil_mime_xml,  ".xml",   "text/xml" },
    { butil_mime_xwwwformurl,".www", "application/x-www-form-urlencoded" },
#if BUTIL_SUPPORT_MULTIPART
    { butil_mime_multi,".",      "multipart/form-data" },
#endif
#if BUTIL_SUPPORT_EXTENDED_MIME_TYPES
    { butil_mime_aac,  ".aac",   "audio/aac" },
    { butil_mime_abw,  ".abw",   "application/x-abiword" },
    { butil_mime_arc,  ".arc",   "application/octet-stream" },
    { butil_mime_avi,  ".avi",   "video/x-msvideo" },
    { butil_mime_azw,  ".azw",   "application/vnd.amazon.ebook" },
    { butil_mime_bz,   ".bz",    "application/x-bzip" },
    { butil_mime_bz2,  ".bz2",   "application/x-bzip2" },
    { butil_mime_csh,  ".csh",   "application/x-csh" },
    { butil_mime_csv,  ".csv",   "text/csv" },
    { butil_mime_doc,  ".doc",   "application/msword" },
    { butil_mime_eot,  ".eot",   "application/vnd.ms-fontobject" },
    { butil_mime_epub, ".epub",  "application/epub+zip" },
    { butil_mime_ics,  ".ics",   "text/calendar" },
    { butil_mime_midi, ".mid",   "audio/midi" },
    { butil_mime_midi, ".midi",  "audio/midi" },
    { butil_mime_mpeg, ".mpeg",  "video/mpeg" },
    { butil_mime_mpkg, ".mpkg",  "application/vnd.apple.installer+xml" },
    { butil_mime_odp,  ".odp",   "application/vnd.oasis.opendocument.presentation" },
    { butil_mime_ods,  ".ods",   "application/vnd.oasis.opendocument.spreadsheet" },
    { butil_mime_odt,  ".odt",   "application/vnd.oasis.opendocument.text" },
    { butil_mime_oga,  ".oga",   "audio/ogg" },
    { butil_mime_ogv,  ".ogv",   "video/ogg" },
    { butil_mime_ogx,  ".ogx",   "application/ogg" },
    { butil_mime_otf,  ".otf",   "font/otf" },
    { butil_mime_png,  ".png",   "image/png" },
    { butil_mime_pdf,  ".pdf",   "application/pdf" },
    { butil_mime_ppt,  ".ppt",   "application/vnd.ms-powerpoint" },
    { butil_mime_rar,  ".rar",   "application/x-rar-compressed" },
    { butil_mime_rtf,  ".rtf",   "application/rtf" },
    { butil_mime_sh,   ".sh",    "application/x-sh" },
    { butil_mime_svg,  ".svg",   "image/svg+xml" },
    { butil_mime_swf,  ".swf",   "application/x-shockwave-flash" },
    { butil_mime_tar,  ".tar",   "application/x-tar" },
    { butil_mime_ts,   ".ts",    "application/typescript" },
    { butil_mime_ttf,  ".ttf",   "font/ttf" },
    { butil_mime_vsd,  ".vsd",   "application/vnd.visio" },
    { butil_mime_wav,  ".wav",   "audio/x-wav" },
    { butil_mime_weba, ".weba",  "audio/webm" },
    { butil_mime_webm, ".webm",  "video/webm" },
    { butil_mime_webp, ".webp",  "image/webp" },
    { butil_mime_woff, ".woff",  "font/woff" },
    { butil_mime_woff2,".woff2", "font/woff2" },
    { butil_mime_xhtml,".xhtml", "application/xhtml+xml" },
    { butil_mime_xls,  ".xls",   "application/vnd.ms-excel" },
    { butil_mime_xlsx, ".xlsx",  "application/vnd.ms-excel,application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" },
    { butil_mime_xul,  ".xul",   "application/vnd.mozilla.xul+xml" },
    { butil_mime_zip,  ".zip",   "application/zip" },
    { butil_mime_3gp,  ".3gp",   "video/3gpp" },
    { butil_mime_3g2,  ".3g2",   "video/3gpp2" },
    { butil_mime_7z,   ".7z",    "application/x-7z-compressed" },
#endif
};

#define NUM_MIME_ENTRIES (sizeof(s_mimetab) / sizeof(butil_mime_entry_t))

const char *butil_mime_string_for_file(const char *path)
{
    const char *pext;
    int i;

    if (! path || ! *path)
    {
       return BUTIL_DEFAULT_MIME;
    }
    pext = path + strlen(path) - 1;
    while (pext > path)
    {
        if (*pext-- == '.')
        {
            pext++;
            break;
        }
    }
    if (*pext != '.')
    {
       return BUTIL_DEFAULT_MIME;
    }
    pext++;

    for (i = 0; i < NUM_MIME_ENTRIES; i++)
    {
        if (! strcasecmp(pext, s_mimetab[i].ext + 1))
        {
            return s_mimetab[i].mime;
        }
    }
    return BUTIL_DEFAULT_MIME;
}

mime_content_type_t butil_content_type_for_file(const char *path)
{
    const char *pext;
    int i;

    if (! path || ! *path)
    {
       return butil_mime_bin;
    }
    pext = path + strlen(path) - 1;
    while (pext > path)
    {
        if (*pext-- == '.')
        {
            pext++;
            break;
        }
    }
    if (*pext != '.')
    {
       return butil_mime_bin;
    }
    pext++;

    for (i = 0; i < NUM_MIME_ENTRIES; i++)
    {
        if (! strcasecmp(pext, s_mimetab[i].ext + 1))
        {
            return s_mimetab[i].type;
        }
    }
    return butil_mime_bin;
}

const char *butil_mime_string_for_content_type(mime_content_type_t type)
{
    int i;

    for (i = 0; i < NUM_MIME_ENTRIES; i++)
    {
        if (type == s_mimetab[i].type)
        {
            return s_mimetab[i].mime;
        }
    }
    // can't happen?
    return BUTIL_DEFAULT_MIME;
}

mime_content_type_t butil_content_type_for_mime_string(const char *mime)
{
    size_t len;
    int i;

    if (! mime || ! *mime)
    {
       return butil_mime_bin;
    }
    for (i = 0; i < NUM_MIME_ENTRIES; i++)
    {
        len = strlen(s_mimetab[i].mime);
        if (! strncasecmp(mime, s_mimetab[i].mime, len))
        {
            return s_mimetab[i].type;
        }
    }
    return butil_mime_bin;
}

int butil_get_host_info(char* myhost, int nhost, bipv4addr_t *myipv4addr, bipv6addr_t *myipv6addr, int *iface_index)
{
#if defined(Linux) || defined(OSX)
    struct ifaddrs *addrs;
    struct ifaddrs *addr;
    struct sockaddr_in *sain4;
    struct sockaddr_in6 *sain6;
    bool gotv4;
    bool gotv6;

    gotv4 = false;
    gotv6 = false;

    *iface_index = 0;

    if (gethostname(myhost, nhost))
    {
        BERROR("gethostname fails");
        return -1;
    }
    getifaddrs(&addrs);

    for (addr = addrs; addr && ! (gotv4 && gotv6); addr = addr->ifa_next)
    {
        if (addr->ifa_addr)
        {
            int family = addr->ifa_addr->sa_family;
            unsigned flags = addr->ifa_flags;

            // look only at interfaces up and not loopback
            //
            if ((flags & IFF_UP) && ! (flags & IFF_LOOPBACK))
            {
                if (family == AF_INET && ! gotv4)
                {
                    gotv4 = true;
                    sain4 = (struct sockaddr_in *)addr->ifa_addr;
                    myipv4addr->addr = sain4->sin_addr.s_addr;
                    //butil_log(4, "IPv4 on iface %d  %s\n", if_nametoindex(addr->ifa_name), addr->ifa_name);
                }
                else if (family == AF_INET6 && ! gotv6)
                {
                    gotv6 = true;
                    sain6 = (struct sockaddr_in6 *)addr->ifa_addr;
                    memcpy(myipv6addr->addr, &sain6->sin6_addr, sizeof(bipv6addr_t));
                    *iface_index = if_nametoindex(addr->ifa_name);
                    //butil_log(4, "IPv6 on iface %d  %s\n", *iface_index, addr->ifa_name);
                }
            }
        }
    }
    freeifaddrs(addrs);

    if (! gotv4)
    {
        myipv4addr->addr = 0;
    }
    if (! gotv6)
    {
        memset(&myipv6addr->addr, 0, sizeof(bipv6addr_t));
    }
    if (! gotv4 && ! gotv6)
    {
        BERROR("no IP addr");
        return -1;
    }
#else
    struct hostent *phost;

    if (gethostname(myhost, nhost))
    {
        BERROR("gethostname fails");
        return -1;
    }
    phost = gethostbyname(myhost);
    if (! phost)
    {
        BERROR("gethostbyname fails");
        return -1;
    }
    memcpy(&myipv4addr->addr, phost->h_addr_list[0], 4);
#endif
    return 0;
}

