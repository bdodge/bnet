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
#include "bnetheaders.h"
#include "battrparse.h"
#include "butil.h"

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

static int ianaimp_useage_blurb(const char *progname)
{
    fprintf(stderr, "Use: %s [-rnx] -o outfile infile [infile2 ...]\n", progname);
    fprintf(stderr, "     -o    Output filename (base name)\n");
    fprintf(stderr, "     -l    Set debug log level (default 1: errors/warnings only)\n");
    fprintf(stderr, "     -a    Import IANA Attributes csv\n");
    return 1;
}

static int ianaimp_nofile_blurb(const char *name)
{
    fprintf(stderr, "Can't open file: %s\n", name);
    return 1;
}

static int ianaimp_nomem_blurb(const char *msg)
{
    fprintf(stderr, "Can't allocate: %s\n", msg);
    return 1;
}

int main(int argc, char **argv)
{
    FILE       *infile;
    FILE       *hdrfile;
    FILE       *srcfile;
    char       *progname;
    char       *infilename;
    char        infilepath[MAX_PATH*2];
    char        hdrfilename[MAX_PATH];
    char        srcfilename[MAX_PATH];
    bool        import_attr;
    int         loglevel;
    int         result;

    loglevel = 5;
    butil_set_log_level(loglevel);

    progname = *argv;
    if (argc < 3)
    {
        return ianaimp_useage_blurb(progname);
    }
    argc--;
    argv++;


    hdrfilename[0] = '\0';
    srcfilename[0] = '\0';

    while (argc > 0 && *argv && *argv[0] == '-')
    {
        if (argv[0][1] == 'o')
        {
            if (argc > 1)
            {
                argc--;
                argv++;
                strcpy(hdrfilename, *argv);
                strcat(hdrfilename, ".h");
                strcpy(srcfilename, *argv);
                strcat(srcfilename, ".c");
            }
            else
            {
                return ianaimp_useage_blurb(progname);
            }
        }
        else if (argv[0][1] == 'l')
        {
            if (argv[0][2] == '\0')
            {
                if (argc > 1)
                {
                    argc--;
                    argv++;
                    loglevel = strtoul(*argv, NULL, 0);
                }
                else
                {
                    return ianaimp_useage_blurb(progname);
                }
            }
            else
            {
                loglevel = strtoul((*argv + 2), NULL, 0);
            }
        }
        else if (argv[0][1] == 'a')
        {
            import_attr = true;
        }
        else
        {
            return ianaimp_useage_blurb(progname);
        }
        argc--;
        argv++;
    }
    butil_set_log_level(loglevel);

    if (! srcfilename[0] || argc < 1)
    {
        return ianaimp_useage_blurb(progname);
    }
    while (argc-- > 0)
    {
        if (! (infile = fopen(*argv, "rb")))
        {
            return ianaimp_nofile_blurb(*argv);
        }
        if (import_attr)
        {
            hdrfile = fopen(hdrfilename, "wb");
            if (! hdrfile)
            {
                fclose(infile);
                return ianaimp_nofile_blurb(hdrfilename);
            }
            srcfile = fopen(srcfilename, "wb");
            if (! srcfile)
            {
                fclose(infile);
                fclose(hdrfile);
                return ianaimp_nofile_blurb(srcfilename);
            }
            result = iana_parse_attributes(
                                        *argv,
                                        hdrfilename,
                                        infile,
                                        hdrfile,
                                        srcfile
                                        );
        }
        if (result)
        {
            fprintf(stderr, "%s - Parsing errors\n", progname);
            return result;
        }
    }
    fclose(infile);
    fclose(hdrfile);
    fclose(srcfile);
    return 0;
}
