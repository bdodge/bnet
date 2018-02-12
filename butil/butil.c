#include "bnetheaders.h"
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

const char *butil_scheme_name(butil_url_scheme_t scheme)
{
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
    default:            return "";
    }
}

int butil_scheme_from_name(const char *name, butil_url_scheme_t *scheme)
{
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
    if (! strcasecmp(name, "sips"))
    {
        *scheme = schemeHTTPS;
        return 0;
    }
    if (! strcasecmp(name, "sip"))
    {
        *scheme = schemeHTTP;
        return 0;
    }
    return -1;
}

int butil_parse_url(
                    const char         *url,
                    butil_url_scheme_t *scheme,
                    char               *host,
                    size_t              nhost,
                    short              *port,
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
        pe = ps;
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
    if (! host)
    {
        BERROR("Missing Hostname");
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
    len = snprintf(url, nurl,
            "%s://%s:%u/%s", schemestr, host, useport, path);
    if (len < 0 || len >= nurl)
    {
        BERROR("Pasted url too long");
        return -1;
    }
    return 0;
}

