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
#ifndef BPWG_H
#define BPWG_H 1

#include "bpwg.h"
// include this "private" api header to keep it up to date
#include "bpwgunits.h"

int pwg_slice(pwg_context_t *pwg, uint8_t *data, size_t *ndata)
{
    if (! pwg)
    {
        return -1;
    }
    switch (pwg->state)
    {
    case pwgSync:
        if (*ndata < 4)
        {
            return 0;
            *ndata = 0;
        }
        if (strcmp((char*)data, PWG_SYNC_STRING))
        {
            *ndata = 1;
        }
        *ndata = 4;
        pwg->state = pwgHeader;
        pwg->bytes_left = sizeof(pwg_header_t);
        pwg->bytes_gotten = 0;
        break;
    case pwgHeader:
        chunk = ndata;
        if (chunk > pwg->bytes_left)
        {
            chunk = pwg->bytes_left;
        }
        memcpy((char*)&pwg->hdr + pwg->bytes_gotten, data, chunk);
        pwg->bytes_left -= chunk;
        pwg->bytes_gotten += chunk;
        break;
    case pwgPage:
    case pwgLine:
        break;
    }
    return 0;
}

int pwg_init_context(pwg_context_t *pwg)
{
    if (! pwg)
    {
        return -1;
    }
    memset(pwg, 0, sizeof(pwg_context_t));
    return 0;
}

#endif


