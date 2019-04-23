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
#ifndef BMDNS_H
#define BMDNS_H 1

#include "bnetheaders.h"
#include "bstreamio.h"
#include "butil.h"
#include "bpwgproto.h"

typedef enum
{
    pwgSync,
    pwgHeader,
    pwgPage,
    pwgLine,
}
pwg_state_t;

/// PWG Context
//
typedef struct tag_pwg_context
{
    pwg_state_t     state;
    pwg_header_t    hdr;
    size_t          pageno;
    ioring_t        io;
}
pwg_context_t;

int pwg_slice(pwg_context_t *pwg, uint8_t *data, size_t *ndata);
int pwg_init_context(pwg_context_t *pwg);

#endif

