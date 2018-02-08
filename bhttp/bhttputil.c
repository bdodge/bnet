#include "bhttp.h"

int http_ncasecmp(const char *haystack, const char *needle)
{
    size_t len = strlen(needle);

    return strncasecmp(haystack, needle, len);
}

const char *http_scheme_name(http_scheme_t scheme)
{
    switch (scheme)
    {
    case httpHTTP:      return "HTTP";
    case httpHTTPS:     return "HTTPS";
    #if HTTP_SUPPORT_WEBSOCKET
    case httpWSS:       return "WSS";
    case httpWS:        return "WS";
    #endif
    #if HTTP_SUPPORT_SIP
    case httpSIP:       return "SIP";
    case httpSIPS:      return "SIPS";
    #endif
    default:            return "";
    }
}

const char *http_scheme_base_name(http_scheme_t scheme)
{
    switch (scheme)
    {
    case httpHTTP:      return "HTTP";
    case httpHTTPS:     return "HTTP";
    #if HTTP_SUPPORT_WEBSOCKET
    case httpWS:        return "HTTP";
    case httpWSS:       return "HTTP";
    #endif
    #if HTTP_SUPPORT_SIP
    case httpSIP:       return "SIP";
    case httpSIPS:      return "SIP";
    #endif
    default:            return "";
    }
}

int http_scheme_from_name(const char *name, http_scheme_t *scheme)
{
    if (! strcasecmp(name, "https"))
    {
        *scheme = httpHTTPS;
        return 0;
    }
    if (! strcasecmp(name, "http"))
    {
        *scheme = httpHTTP;
        return 0;
    }
    #if HTTP_SUPPORT_WEBSOCKET
    if (! strcasecmp(name, "wss"))
    {
        *scheme = httpWSS;
        return 0;
    }
    if (! strcasecmp(name, "ws"))
    {
        *scheme = httpWS;
        return 0;
    }
    #endif
    #if HTTP_SUPPORT_SIP
    if (! strcasecmp(name, "sips"))
    {
        *scheme = httpHTTPS;
        return 0;
    }
    if (! strcasecmp(name, "sip"))
    {
        *scheme = httpHTTP;
        return 0;
    }
    #endif
    return -1;
}

const char *http_method_name(http_method_t method)
{
    switch (method)
    {
    case httpOptions:   return "OPTIONS";
    case httpGet:       return "GET";
    case httpHead:      return "HEAD";
    case httpPost:      return "POST";
    case httpPut:       return "PUT";
    case httpDelete:    return "DELETE";
    case httpTrace:     return "TRACE";
    case httpConnect:   return "CONNECT";
#if HTTP_SUPPORT_WEBDAV
    case httpPropFind:  return "PROPFIND";
    case httpPropPatch: return "PROPPATCH";
    case httpCopy:      return "COPY";
    case httpMkCol:     return "MKCOL";
    case httpMove:      return "MOVE";
    case httpLock:      return "LOCK";
    case httpUnlock:    return "UNLOCK";
#endif
#if HTTP_SUPPORT_SIP
    case httpInvite:    return "INVITE";
    case httpAck:       return "ACK";
    case httpPrack:     return "PRACK";
    case httpCancel:    return "CANCEL";
    case httpUpdate:    return "UPDATE";
    case httpInfo:      return "INFO";
    case httpSubscribe: return "SUBSCRIBE";
    case httpNotify:    return "NOTIFY";
    case httpRefer:     return "REFER";
    case httpMessage:   return "MESSAGE";
    case httpRegister:  return "REGISTER";
    case httpBye:       return "BYE";
#endif
    default:            return "- bad method -";
    }
}

int http_method_from_name(const char *name, http_method_t *method)
{
    if (! http_ncasecmp(name, "OPTIONS"))
    {
        *method = httpOptions;
        return 0;
    }
    if (! http_ncasecmp(name, "GET"))
    {
        *method = httpGet;
        return 0;
    }
    if (! http_ncasecmp(name, "HEAD"))
    {
        *method = httpHead;
        return 0;
    }
    if (! http_ncasecmp(name, "POST"))
    {
        *method = httpPost;
        return 0;
    }
    if (! http_ncasecmp(name, "PUT"))
    {
        *method = httpPut;
        return 0;
    }
    if (! http_ncasecmp(name, "DELETE"))
    {
        *method = httpDelete;
        return 0;
    }
    if (! http_ncasecmp(name, "TRACE"))
    {
        *method = httpTrace;
        return 0;
    }
    if (! http_ncasecmp(name, "CONNECT"))
    {
        *method = httpConnect;
        return 0;
    }
#if HTTP_SUPPORT_WEBDAV
    if (! http_ncasecmp(name, "PROPFIND"))
    {
        *method = httpPropFind;
        return 0;
    }
    if (! http_ncasecmp(name, "PROPPATCH"))
    {
        *method = httpPropPatch;
        return 0;
    }
    if (! http_ncasecmp(name, "COPY"))
    {
        *method = httpCopy;
        return 0;
    }
    if (! http_ncasecmp(name, "MKCOL"))
    {
        *method = httpMkCol;
        return 0;
    }
    if (! http_ncasecmp(name, "MOVE"))
    {
        *method = httpMove;
        return 0;
    }
    if (! http_ncasecmp(name, "LOCK"))
    {
        *method = httpLock;
        return 0;
    }
    if (! http_ncasecmp(name, "UNLOCK"))
    {
        *method = httpUnlock;
        return 0;
    }
#endif
#if HTTP_SUPPORT_SIP
    if (! http_ncasecmp(name, "INVITE"))
    {
        *method = httpInvite;
        return 0;
    }
    if (! http_ncasecmp(name, "ACK"))
    {
        *method = httpAck;
        return 0;
    }
    if (! http_ncasecmp(name, "PRACK"))
    {
        *method = httpPrack;
        return 0;
    }
    if (! http_ncasecmp(name, "CANCEL"))
    {
        *method = httpCancel;
        return 0;
    }
    if (! http_ncasecmp(name, "UPDATE"))
    {
        *method = httpUpdate;
        return 0;
    }
    if (! http_ncasecmp(name, "INFO"))
    {
        *method = httpInfo;
        return 0;
    }
    if (! http_ncasecmp(name, "SUBSCRIBE"))
    {
        *method = httpSubscribe;
        return 0;
    }
    if (! http_ncasecmp(name, "NOTIFY"))
    {
        *method = httpNotify;
        return 0;
    }
    if (! http_ncasecmp(name, "REFER"))
    {
        *method = httpRefer;
        return 0;
    }
    if (! http_ncasecmp(name, "MESSAGE"))
    {
        *method = httpMessage;
        return 0;
    }
    if (! http_ncasecmp(name, "REGISTER"))
    {
        *method = httpRegister;
        return 0;
    }
    if (! http_ncasecmp(name, "BYE"))
    {
        *method = httpBye;
        return 0;
    }
#endif
    *method = httpUnsupported;
    return -1;
}

static inline uint8_t byte_from_hex(char x)
{
    if (x >= '0' && x <= '9')
    {
        return x - '0';
    }
    if (x >= 'A' && x <= 'F')
    {
        return x - 'A' + 10;
    }
    if (x >= 'a' && x <= 'f')
    {
        return x - 'a' + 10;
    }
    HTTP_ERROR("Bad hex digit");
    return 0;
}

int http_join_path(char *path, size_t room, const char *root, const char *base, const char *file)
{
    size_t i;
    size_t filepos;

    if (! path || ! root || ! root[0])
    {
        HTTP_ERROR("No root");
        return -1;
    }
    if (! file)
    {
        file = "";
    }
    // remove any path delimter in front of filename
    //
    while (file[0] == '/' || file[0] == '\\')
    {
        file++;
    }
    // put root path into result
    //
    for (i = 0; i < room && root[i]; i++)
    {
        if (root[i] == '/' || path[i] == '\\')
        {
            path[i] = HTTP_LOCAL_PATH_CHAR;
        }
        else
        {
            path[i] = root[i];
        }
    }
    if (i >= (room - 4) || i == 0)
    {
        HTTP_ERROR("Path overflow");
        return -1;
    }
    // ensure the path ends with a delimiter
    //
    if (path[i - 1] != HTTP_LOCAL_PATH_CHAR)
    {
        path[i++] = HTTP_LOCAL_PATH_CHAR;
    }
    // remember spot where file portion of path goes
    //
    filepos = i;

    // url-decode file in-site during the copy to path operation
    //
    while ((i < room) && *file)
    {
        if (*file == '/' || *file == '\\')
        {
            file++;
            path[i++] = HTTP_LOCAL_PATH_CHAR;
        }
        else if (*file == '%')
        {
            file++;
            if (file[0] && file[1])
            {
                uint8_t msn, lsn;

                msn = byte_from_hex(file[0]);
                lsn = byte_from_hex(file[1]);
                file += 2;

                path[i++] = (char)((msn << 4) | lsn);
            }
            else
            {
                break;
            }
        }
        else if (*file == '+')
        {
            file++;
            path[i++] = ' ';
        }
        else if (*file == '?')
        {
            break;
        }
        else
        {
            path[i++] = *file++;
        }
    }
    if (i >= room)
    {
        HTTP_ERROR("Path overflow");
        return -1;
    }
    path[i] = '\0';

    if (base && base[0])
    {
        size_t baselen = strlen(base);
        size_t namelen = strlen(path + filepos);
        size_t remlen;

        if (base[0] == '/' || base[0] == '\\')
        {
            base++;
            baselen--;
        }
        if (namelen >= baselen)
        {
            if (! strncmp(path + filepos, base, baselen))
            {
                remlen = namelen - baselen;
                if (remlen > 0)
                {
                    if (path[filepos + baselen] == HTTP_LOCAL_PATH_CHAR)
                    {
                        baselen++;
                    }
                    memmove(path + filepos, path + filepos + baselen, remlen);
                }
                path[filepos + remlen] = '\0';
            }
        }
    }
    return 0;
}

int http_parse_url(
                    const char *url,
                    http_scheme_t *scheme,
                    char *host,
                    short *port,
                    char *path
                  )
{
    const char *ps, *pe;
    uint16_t portnum = 80;
    size_t len;

    if (scheme)
    {
        *scheme = httpHTTP;
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
        HTTP_ERROR("Invalid Parameter");
        return -1;
    }
    // search forward for ://
    ps = url;
    pe = strstr(ps, "://");
    if (pe)
    {
        len = pe - ps;
        pe += 3;

        if ((len == 5) && ! strncasecmp(ps, "https", len))
        {
            portnum = 443;
        }
    }
    else
    {
        pe = ps;
        ps = "http";
        len = 4;
    }
    if (len >= HTTP_MAX_SCHEME)
    {
        HTTP_ERROR("Scheme too long");
        return -1;
    }
    if (scheme)
    {
        char schemestr[HTTP_MAX_SCHEME];

        strncpy(schemestr, ps, len);
        schemestr[len] = '\0';

        if (http_scheme_from_name(schemestr, scheme))
        {
            HTTP_ERROR("Bad Scheme");
            return -1;
        }
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
    if (len >= HTTP_MAX_HOSTNAME)
    {
        HTTP_ERROR("Hostname too long");
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
        if (len > HTTP_MAX_PORTSPEC)
        {
            HTTP_ERROR("Malformed port number");
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
        HTTP_ERROR("Malformed path");
        return -1;
    }
    len = strlen(ps);
    if (len >= HTTP_MAX_PATH)
    {
        HTTP_ERROR("Path too long");
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

int http_paste_url(
                    char *url,
                    const http_scheme_t scheme,
                    const char *host,
                    const uint16_t port,
                    const char *path
                  )
{
    size_t len;
    uint16_t useport = port;
    const char *schemestr;

    if (! url)
    {
        HTTP_ERROR("Invalid Parameter");
        return -1;
    }
    schemestr = http_scheme_name(scheme);
    if (! schemestr)
    {
        HTTP_ERROR("Invalid Scheme");
        return -1;
    }
    if (! host)
    {
        HTTP_ERROR("Missing Hostname");
        return -1;
    }
    if (! useport)
    {
        useport = strcasecmp(schemestr, "https") ? 443 : 80;
    }
    if (! path)
    {
        path = "";
    }
    len = snprintf(url, HTTP_MAX_URL,
            "%s://%s:%u/%s", schemestr, host, useport, path);
    if (len < 0 || len >= HTTP_MAX_URL)
    {
        HTTP_ERROR("Pasted url too long");
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

time_t http_rfc2616_date_to_time(const char *date)
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

const char *http_time_to_rfc2616_date(time_t when, char *buf, size_t nbuf)
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

int http_auth_string_to_type(const char *auth_str, http_auth_type_t *auth_type)
{
    if (! http_ncasecmp(auth_str, "basic"))
    {
        *auth_type = httpAuthBasic;
    }
    else if (! http_ncasecmp(auth_str, "bearer"))
    {
        *auth_type = httpAuthBearer;
    }
    else if (! http_ncasecmp(auth_str, "digest"))
    {
        *auth_type = httpAuthDigest;
    }
    else if (! http_ncasecmp(auth_str, "mutual"))
    {
        *auth_type = httpAuthMutual;
    }
    else if (! http_ncasecmp(auth_str, "oauth"))
    {
        *auth_type = httpAuthOAuth;
    }
    else
    {
        *auth_type = httpAuthNone;
        return -1;
    }
    return 0;
}

const char *http_auth_type_to_string(http_auth_type_t auth_type)
{
    switch (auth_type)
    {
    case httpAuthNone:      return "None";
    case httpAuthBasic:     return "Basic";
    case httpAuthBearer:    return "Bearer";
    case httpAuthDigest:    return "Digest";
    case httpAuthMutual:    return "Mutual";
    case httpAuthOAuth:     return "OAuth";
    default:                return "???";
    }
}

typedef struct mime_tab
{
    http_content_type_t type;
    const char         *ext;
    const char         *mime;
}
http_mime_entry_t;

static http_mime_entry_t s_mimetab[] =
{
    { htmtbin,  ".bin",   "application/octet-stream" },
    { htmtcss,  ".css",   "text/css" },
    { htmtgif,  ".gif",   "image/gif" },
    { htmthtml, ".htm",   "text/html" },
    { htmthtml, ".html",  "text/html" },
    { htmtico,  ".ico",   "image/x-icon" },
    { htmtjar,  ".jar",   "application/java-archive" },
    { htmtjpeg, ".jpeg",  "image/jpeg" },
    { htmtjpeg, ".jpg",   "image/jpeg" },
    { htmtjs,   ".js",    "application/javascript" },
    { htmtjson, ".json",  "application/json" },
    { htmttext, ".text",  "text/plain" },
    { htmttext, ".txt",   "text/plain" },
    { htmttiff, ".tif",   "image/tiff" },
    { htmttiff, ".tiff",  "image/tiff" },
    { htmtxml,  ".xml",   "application/xml" },
#if HTTP_SUPPORT_MULTIPART
    { htmtmulti,".",      "multipart/form-data" },
#endif
#if HTTP_SUPPORT_EXTENDED_MIME_TYPES
    { htmtaac,  ".aac",   "audio/aac" },
    { htmtabw,  ".abw",   "application/x-abiword" },
    { htmtarc,  ".arc",   "application/octet-stream" },
    { htmtavi,  ".avi",   "video/x-msvideo" },
    { htmtazw,  ".azw",   "application/vnd.amazon.ebook" },
    { htmtbz,   ".bz",    "application/x-bzip" },
    { htmtbz2,  ".bz2",   "application/x-bzip2" },
    { htmtcsh,  ".csh",   "application/x-csh" },
    { htmtcsv,  ".csv",   "text/csv" },
    { htmtdoc,  ".doc",   "application/msword" },
    { htmteot,  ".eot",   "application/vnd.ms-fontobject" },
    { htmtepub, ".epub",  "application/epub+zip" },
    { htmtics,  ".ics",   "text/calendar" },
    { htmtmidi, ".mid",   "audio/midi" },
    { htmtmidi, ".midi",  "audio/midi" },
    { htmtmpeg, ".mpeg",  "video/mpeg" },
    { htmtmpkg, ".mpkg",  "application/vnd.apple.installer+xml" },
    { htmtodp,  ".odp",   "application/vnd.oasis.opendocument.presentation" },
    { htmtods,  ".ods",   "application/vnd.oasis.opendocument.spreadsheet" },
    { htmtodt,  ".odt",   "application/vnd.oasis.opendocument.text" },
    { htmtoga,  ".oga",   "audio/ogg" },
    { htmtogv,  ".ogv",   "video/ogg" },
    { htmtogx,  ".ogx",   "application/ogg" },
    { htmtotf,  ".otf",   "font/otf" },
    { htmtpng,  ".png",   "image/png" },
    { htmtpdf,  ".pdf",   "application/pdf" },
    { htmtppt,  ".ppt",   "application/vnd.ms-powerpoint" },
    { htmtrar,  ".rar",   "application/x-rar-compressed" },
    { htmtrtf,  ".rtf",   "application/rtf" },
    { htmtsh,   ".sh",    "application/x-sh" },
    { htmtsvg,  ".svg",   "image/svg+xml" },
    { htmtswf,  ".swf",   "application/x-shockwave-flash" },
    { htmttar,  ".tar",   "application/x-tar" },
    { htmtts,   ".ts",    "application/typescript" },
    { htmtttf,  ".ttf",   "font/ttf" },
    { htmtvsd,  ".vsd",   "application/vnd.visio" },
    { htmtwav,  ".wav",   "audio/x-wav" },
    { htmtweba, ".weba",  "audio/webm" },
    { htmtwebm, ".webm",  "video/webm" },
    { htmtwebp, ".webp",  "image/webp" },
    { htmtwoff, ".woff",  "font/woff" },
    { htmtwoff2,".woff2", "font/woff2" },
    { htmtxhtml,".xhtml", "application/xhtml+xml" },
    { htmtxls,  ".xls",   "application/vnd.ms-excel" },
    { htmtxlsx, ".xlsx",  "application/vnd.ms-excel,application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" },
    { htmtxul,  ".xul",   "application/vnd.mozilla.xul+xml" },
    { htmtzip,  ".zip",   "application/zip" },
    { htmt3gp,  ".3gp",   "video/3gpp" },
    { htmt3g2,  ".3g2",   "video/3gpp2" },
    { htmt7z,   ".7z",    "application/x-7z-compressed" },
#endif
};

#define NUM_MIME_ENTRIES (sizeof(s_mimetab) / sizeof(http_mime_entry_t))

const char *http_mime_string_for_file(const char *path)
{
    const char *pext;
    int i;

    if (! path || ! *path)
    {
       return HTTP_DEFAULT_MIME;
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
       return HTTP_DEFAULT_MIME;
    }
    pext++;

    for (i = 0; i < NUM_MIME_ENTRIES; i++)
    {
        if (! strcasecmp(pext, s_mimetab[i].ext + 1))
        {
            return s_mimetab[i].mime;
        }
    }
    return HTTP_DEFAULT_MIME;
}

http_content_type_t http_content_type_for_file(const char *path)
{
    const char *pext;
    int i;

    if (! path || ! *path)
    {
       return htmtbin;
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
       return htmtbin;
    }
    pext++;

    for (i = 0; i < NUM_MIME_ENTRIES; i++)
    {
        if (! strcasecmp(pext, s_mimetab[i].ext + 1))
        {
            return s_mimetab[i].type;
        }
    }
    return htmtbin;
}

const char *http_mime_string_for_content_type(http_content_type_t type)
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
    return HTTP_DEFAULT_MIME;
}

http_content_type_t http_content_type_for_mime_string(const char *mime)
{
    int i;

    if (! mime || ! *mime)
    {
       return htmtbin;
    }
    for (i = 0; i < NUM_MIME_ENTRIES; i++)
    {
        if (! http_ncasecmp(mime, s_mimetab[i].mime))
        {
            return s_mimetab[i].type;
        }
    }
    return htmtbin;
}

