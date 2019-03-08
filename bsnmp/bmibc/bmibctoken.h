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
#ifndef BMIBCTOKEN_H
#define BMIBCTOKEN_H 1

#include "bmibckeywords.h"
#include "bmibcsym.h"
#include "bmibc.h"
#include "bmibcerr.h"
#include "bmibctoken.h"
#include "bmibcparse.h"


typedef enum
{
    mttNumber,
    mttLabel,
    mttEqual,
    mttRange,
    mttLiteral,
    mttString,
    mttPunctuation,
    mttComment,
    mttControl
}
bmib_token_type_t;

bool bmib_get_nc_token(
                        bmib_context_t     *parser,
                        bmib_token_type_t  *toktype,
                        bmib_keyword_t     *kwtype,
                        const char         *tfor,
                        bool               *tokerr
                        );


#endif

