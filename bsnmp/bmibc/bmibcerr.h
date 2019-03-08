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
#ifndef BMIBCERR_H
#define BMIBCERR_H 1

#include "bnetheaders.h"
#include "bmibckeywords.h"
#include "bmibcsym.h"

typedef enum
{
    meInfo,
	meWarning,
	meError,
	meEOF,
	meNoFile,
	meUndefSym,
	meMultidefSym,
    meExpected,
	meUnexpected,
	meUnsupported,
	meMemory,
    meInternal
}
bmib_error_t;

struct tag_mib_context;

void bmib_error_multidef_sym(struct tag_mib_context *parser, const char *name, bmib_symbol_t *prevsym);
void bmib_error_undefined(struct tag_mib_context *parser, const char *name);
void bmib_error_multidef_node(struct tag_mib_context *parser, bmib_node_t *prevnode, const char *pnewval);
void bmib_error_token_size(struct tag_mib_context *parser, const char *token, int maxlen);
void bmib_error_expected(struct tag_mib_context *parser, const char *expected);
void bmib_error_token(struct tag_mib_context *parser, const char *msg);
void bmib_error_memory(struct tag_mib_context *parser, const char* msg);
void bmib_error_eof(struct tag_mib_context *parser, const char *msg, const char *detail);
void bmib_warning(struct tag_mib_context *parser, const char *msg, ...);
void bmib_error(struct tag_mib_context *parser, bmib_error_t code, const char *msg, ...);

#endif
