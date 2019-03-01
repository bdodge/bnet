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
#ifndef SNMPVAR_H
#define SNMPVAR_H 1

#include "bsnmp.h"

bsnmp_var_t    *bsnmp_var_create            (void);
void            bsnmp_var_destroy           (bsnmp_var_t *var);
int             bsnmp_var_init              (bsnmp_var_t *var, bsnmp_oid_t *oid, bsnmp_type_t type);
int             bsnmp_var_setvalue          (bsnmp_var_t *var, bval_t *val);
bsnmp_var_t    *bsnmp_varlist_appendvar     (bsnmp_var_t *varlist, bsnmp_var_t *var);
void            bsnmp_varlist_cleanup       (bsnmp_var_t *varlist);
const char     *bsnmp_var_string            (bsnmp_var_t *var, char *buffer, int nBuffer);

#endif
