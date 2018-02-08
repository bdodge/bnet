#ifndef BUTIL_H
#define BUTIL_H 1

void butil_log(uint32_t level, const char *fmt, ...);
void butil_set_log_level(uint32_t level);

int butil_hextou(char digit, uint8_t *val);
int butil_is_white(char ch);
int butil_is_number(char ch);

size_t butil_utf8_encode(uint32_t unicode, uint8_t utfbuf[5]);

int butil_base64_decode(
         	        	uint8_t 	   *out,
                      	size_t 			outsize,
                        const char     *src
                       );

int butil_base64_encode(
	                 	char           *out,
	                    size_t          outsize,
	                    const uint8_t  *src,
	                    size_t          srcbytes,
                     	bool            urlencode,
                     	bool            hexescape
                   	   );

#endif

