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

#include "bmtp.h"

#define PTPPROP_DBG_LEVEL 3

static ptp_var_t s_properties[PTP_MAX_PROPERTIES];

static ptp_var_t *ptpvar_from_code(uint16_t code)
{
    size_t slot;

    for (slot = 0; slot < PTP_MAX_PROPERTIES; slot++)
    {
        if (s_properties[slot].code == code)
        {
            return &s_properties[slot];
        }
    }
    return NULL;

}

static uint64_t ptp_unique_for_file(mtp_object_t *pmo)
{
    uint64_t val;
    const uint8_t *pn;
    size_t   n;

    if (! pmo)
    {
        return 0;
    }
    val = 0;
    pn = (const uint8_t*)pmo->name;
    for (n = 0; n < 8; n++)
    {
        if (*pn)
        {
            val <<= 8;
            val |= *pn++;
        }
        else
        {
            val <<= 8;
            val |= 0xab;
        }
    }
    val ^= ((uint64_t)pmo->size << 32);
    val ^= ((uint64_t)pmo->modt);
    return val;
}

uint16_t ptp_setobjectproperty_for(uint16_t objfmt, ptp_var_t *pvar, uint32_t handle)
{

    mtp_object_t *pmo;

    pmo = NULL;

    switch (objfmt) // object format code wanting obj prop for
    {
    case PTP_OBJFMT_UNDEF:
    case PTP_OBJFMT_ASSOC:
    case PTP_OBJFMT_MP3:
    case PTP_OBJFMT_FW:
    case PTP_OBJFMT_M3UPLAYLIST:
    case PTP_OBJFMT_ABSTRACTAUDIOVIDEOPLAYLIST:
    case PTP_OBJFMT_WAV:
    case PTP_OBJFMT_WMA:
    case PTP_OBJFMT_XMLDOCUMENT:
    case PTP_OBJFMT_ABSTRACTAUDIOALBUM:
        break;
    default:
        butil_log(3, "can't set prop %04X for fmt %04X\n",
            pvar ? pvar->code : 0xffff, objfmt);
        return PTP_RESP_Invalid_ObjectProp_Format;
    }

    if (handle > 0)
    {
        pmo = mtpd_from_handle(handle);
        if (! pmo)
        {
            return PTP_RESP_InvalidObjectHandle;
        }
        switch (pvar->code)
        {
        case PTP_PROP_StorageID:
            break;
        case PTP_PROP_ObjectFormat:
            pvar->current_value.uval = pmo->format;
            break;
        case PTP_PROP_ProtectionStatus:
            pvar->current_value.uval = pmo->access;
            break;
        case PTP_PROP_NonConsumable:
            break; // leave default
        case PTP_PROP_ParentObject:
            pvar->current_value.uval = pmo->parent;
            break;
        case PTP_PROP_AssociationType:
            pvar->current_value.uval = (pmo->format == PTP_OBJFMT_ASSOC) ? 1 : 0;
            break;
        case PTP_PROP_ObjectSize:
            pvar->current_value.uval = pmo->size;
            break;
        case PTP_PROP_ObjectFileName:
        case PTP_PROP_Name:
            pvar->current_value.sval = pmo->name;
            break;
        case PTP_PROP_DateModified:
            pvar->current_value.uval = pmo->modt;
            break;
        case PTP_PROP_PersistantUniqueObjectIdentifier:
            pvar->current_value.u64val = ptp_unique_for_file(pmo);
            if (PTPPROP_DBG_LEVEL > 0)
            {
                butil_log(3, "UID for %s is %lu\n", pmo->name, pvar->current_value.u64val);
            }
            break;
        default:
            return PTP_RESP_Invalid_ObjectPropCode;
        }
    }

    return PTP_RESP_OK;
}

ptp_var_t *ptp_getproperty(uint16_t code)
{
    ptp_var_t *pvar;

    pvar = ptpvar_from_code(code);
    return pvar;
}

ptp_var_t *ptp_getproperty_for_format(uint16_t code, uint16_t objfmt)
{
    ptp_var_t *pvar;

    pvar = ptpvar_from_code(code);

    // only some properties apply to some formats in theory
    //
    switch (objfmt)
    {
    case 0:
        return pvar;
    case PTP_OBJFMT_UNDEF:
        return NULL;
    case PTP_OBJFMT_MP3:
    case PTP_OBJFMT_FW:
    case PTP_OBJFMT_M3UPLAYLIST:
    case PTP_OBJFMT_WAV:
    case PTP_OBJFMT_WMA:
    case PTP_OBJFMT_XMLDOCUMENT:
        return pvar;
    case PTP_OBJFMT_ABSTRACTAUDIOALBUM:
    case PTP_OBJFMT_ABSTRACTAUDIOVIDEOPLAYLIST:
        return pvar;
    case PTP_OBJFMT_ASSOC:
        return pvar;
    default:
        return NULL;
    }
}

int ptp_properties_init()
{
    size_t i;

    for (i = 0; i < PTP_MAX_PROPERTIES; i++)
    {
        s_properties[i].code = 0;
    }
    // Device Properties
    //
    i = 0;
    s_properties[i].code = PTP_PROP_Synchronization_Partner;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_STR;
    s_properties[i].factory_value.sval = PTPDEV_MODEL;
    s_properties[i].current_value.sval = PTPDEV_MODEL;

    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_Device_Friendly_Name;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_STR;
    s_properties[i].factory_value.sval = PTPDEV_MODEL;
    s_properties[i].current_value.sval = PTPDEV_MODEL;

    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_Perceived_Device_Type;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_UINT32;
    s_properties[i].factory_value.uval = 0x2; // media player
    s_properties[i].current_value.uval = 0x2;

    // Object Properties
    //
    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_StorageID;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_UINT32;
    s_properties[i].factory_value.uval = MTPD_ROOT_STORE;
    s_properties[i].current_value.uval = MTPD_ROOT_STORE;

    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_ObjectFormat;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_UINT16;
    s_properties[i].factory_value.uval = PTP_OBJFMT_MP3;
    s_properties[i].current_value.uval = PTP_OBJFMT_MP3;

    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_ProtectionStatus;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_UINT16;
    s_properties[i].factory_value.uval = 0; // r/w
    s_properties[i].current_value.uval = 0;

    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_ObjectSize;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_UINT64;
    s_properties[i].factory_value.uval = 0;
    s_properties[i].current_value.uval = 0;

    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_AssociationType;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_UINT16;
    s_properties[i].factory_value.uval = 0; // file
    s_properties[i].current_value.uval = 0;

    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_ObjectFileName;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_STR;
    s_properties[i].factory_value.sval = "";
    s_properties[i].current_value.sval = "";

    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_ParentObject;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_UINT32;
    s_properties[i].factory_value.uval = 0;
    s_properties[i].current_value.uval = 0;

    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_PersistantUniqueObjectIdentifier;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_UINT128;
    s_properties[i].factory_value.u64val = 0;
    s_properties[i].current_value.u64val = 0;

    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_Name;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_STR;
    s_properties[i].factory_value.sval = "";
    s_properties[i].current_value.sval = "";

    /*
    if (i >= PTP_MAX_PROPERTIES - 1) {
        return -1;
    }

    i++;
    s_properties[i].code = PTP_PROP_NonConsumable;
    s_properties[i].access = PTP_PROP_READONLY;
    s_properties[i].type = PTP_TYPE_UINT8;
    s_properties[i].factory_value.sval = 0; // consumable
    s_properties[i].current_value.sval = 0;
    */
    return 0;
}

