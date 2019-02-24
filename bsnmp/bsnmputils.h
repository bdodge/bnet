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
#ifndef BSNMPUTILS_H
#define BSNMPUTILS_H 1

#include "bsnmp.h"

typedef enum
{
    snmpCmpError = -1,
    snmpCmpExact  = 0,
    snmpCmpAbeforeB = 1,
    snmpCmpAafterB = 2,
    snmpCmpAinB = 3,
    snmpCmpBinA = 4,
}
bsnmp_oidcmp_t;

int            bsnmp_oid_copy(bsnmp_oid_t *dst, bsnmp_oid_t *src);
int            bsnmp_var_init(bsnmp_var_t *var, bsnmp_oid_t *oid, bsnmp_type_t type);
int            bsnmp_oid_from_string(bsnmp_oid_t *oid, const char *str);
const char    *bsnmp_oid_string(bsnmp_oid_t *oid, char *buffer, int nBuffer);
bsnmp_oidcmp_t bsnmp_oidcmp(bsnmp_oid_t *a, bsnmp_oid_t *b, size_t *index);
const char    *bsnmp_var_string(bsnmp_var_t *var, char *buffer, int nBuffer);
const char    *bsnmp_version_string(int32_t version);
const char    *bsnmp_type_string(bsnmp_type_t type);
const char    *bsnmp_opcode_string(bsnmp_opcode_t code);
const char    *bsnmp_errcode_string(bsnmp_errcode_t errcode);

#endif
