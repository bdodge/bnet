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
#ifndef BGCPPRIVET_H
#define BGCPPRIVET_H 1

#include "bmdns.h"
#include "bgcp.h"

int gcp_local_disco_slice (gcp_context_t *gcp);
int gcp_init_local_disco  (gcp_context_t *gcp);

#endif
