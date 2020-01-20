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
#ifndef PTPPROPS_H
#define PTPPROPS_H 1

#define PTP_MAX_PROPERTIES 16

typedef union ptp_val
{
    const char *sval;
    uint32_t    uval;
    uint64_t    u64val;
}
ptp_val_t;

typedef struct ptp_var
{
    uint16_t    code;
    uint16_t    type;
    uint8_t     access;
    uint32_t    group;
    ptp_val_t   factory_value;
    ptp_val_t   current_value;
}
ptp_var_t;

ptp_var_t  *ptp_getproperty(uint16_t code);
ptp_var_t  *ptp_getproperty(uint16_t code);
ptp_var_t  *ptp_getproperty_for_format(uint16_t code, uint16_t objfmt);
uint16_t    ptp_setobjectproperty_for(uint16_t objfmt, ptp_var_t *pvar, uint32_t handle);
int         ptp_properties_init(void);

#endif

