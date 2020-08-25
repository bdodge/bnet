/*
 * Copyright 2020 Brian Dodge
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
#ifndef BUI_H
#define BUI_H 1

#include "bnetheaders.h"

typedef struct tag_rect
{
    int top;
    int left;
    int bottom;
    int right;
}
rect_t;

typedef struct tag_point
{
    int x;
    int y;
}
point_t;

typedef uint32_t COLORREF;

#define RGB(r,g,b)      ((((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff))
#define RGBA(r,g,b,a)   (((((a)&0xff)<<24)|((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff))

#include "buigdi.h"
#include "buidialogs.h"
#include "buiwindows.h"

#endif
