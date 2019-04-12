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
#ifndef BIPPMEDIA_H
#define BIPPMEDIA_H 1

/// An entry in a media table
//
typedef struct tag_ipp_media
{
    const char         *name;               ///< media name
    const char         *type;               ///< media type
    const char         *source;             ///< media source
    int32_t             top_margin;         ///< top margin
    int32_t             bottom_margin;      ///< bottom margin
    int32_t             left_margin;        ///< left margin
    int32_t             right_margin;       ///< right margin
}
ipp_media_t;

#endif

