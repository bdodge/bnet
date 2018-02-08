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
						char 		   *out,
						size_t 			outsize,
						const uint8_t  *src,
						size_t 			srcbytes,
						bool 			urlencode,
						bool 			hexescape
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
    return (k <= srcbytes) ? -1 : (out - base);
}


