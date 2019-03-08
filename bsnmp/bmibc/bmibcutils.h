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
#ifndef BMIBCUTILS_H
#define BMIBCUTILS_H 1

#include "bmibctoken.h"
#include "bmibcsym.h"
#include "bmibcerr.h"

struct tag_mib_context;

const char *bmib_format_oid         (boid_t *oid, int oidlen);
int         bmib_get_oid            (struct tag_mib_context *parser, bmib_node_t *node, boid_t *oid, int cboid);
const char *bmib_get_oid_of         (struct tag_mib_context *parser, const char *object, bmib_symbol_t *nodesyms);
void        bmib_format_description (char *dst, const char *src);

#endif

