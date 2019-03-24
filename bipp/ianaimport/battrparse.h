#ifndef BATTRPARSE_H
#define BATTRPARSE_H 1

#include "bnetheaders.h"

int iana_parse_attributes(
                        const char *fname,
                        const char *hdrname,
                        FILE *infile,
                        FILE *hdrfile,
                        FILE *srcfile
                        );
#endif

