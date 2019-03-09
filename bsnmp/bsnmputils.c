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
#include "bsnmputils.h"
#include "bsnmp.h"
#include "bsnmpio.h"
#include "butil.h"

int bsnmp_oid_copy(bsnmp_oid_t *dst, bsnmp_oid_t *src)
{
    if (! dst || ! src)
    {
        return -1;
    }
    dst->len = src->len;
    memcpy(dst->oid, src->oid, src->len * sizeof(boid_t));
    return 0;
}

int bsnmp_oid_from_string(bsnmp_oid_t *oid, const char *str)
{
    int      i;
    uint32_t n;

    if (! oid || ! str)
    {
        return -1;
    }
    for (i = 0; *str && (i < SNMP_MAX_OID); i++)
    {
        if (*str < '0' || *str > '9')
        {
            SNMP_ERROR("OID Non-Numeric");
            return SNMP_ErrWrongEncoding;
        }
        // convert number entry to unsigned int. note we are radix agnostic
        //
        n = strtoul(str, (char**)&str, 0);
        if (n > 0xFFFF)
        {
            SNMP_ERROR("OID Over-Range");
            return SNMP_ErrTooBig;
        }
        oid->oid[i] = n;

        if (*str)
        {
            if (*str != '.')
            {
                SNMP_ERROR("OID Not-dot");
                return SNMP_ErrWrongEncoding;
            }
            str++;
        }
    }
    if (i >= SNMP_MAX_OID && *str)
    {
        return SNMP_ErrTooBig;
    }
    oid->len = i;
    return 0;
}

const char *bsnmp_oid_string(bsnmp_oid_t *oid, char *buffer, int nBuffer)
{
    int   i, j, k;

    if (! buffer || ! nBuffer)
    {
        return "";
    }
    if (! oid || oid->len == 0)
    {
        buffer[0] = '\0';
        return buffer;
    }
    for (i = j = 0; (i < (oid->len - 1)) && (j < nBuffer); i++)
    {
        k = snprintf(buffer + j, nBuffer -j, "%u.", oid->oid[i]);
        j += k;
    }
    if (j < nBuffer)
    {
        snprintf(buffer + j, nBuffer - j, "%u", oid->oid[i]);
    }
    return buffer;
}

bsnmp_oidcmp_t bsnmp_oidcmp(bsnmp_oid_t *a, bsnmp_oid_t *b, size_t *index)
{
    size_t i;
    size_t lena;
    size_t lenb;

    if (! a || ! b)
    {
        return snmpCmpError;
    }
    lena = a->len;
    lenb = b->len;

    for (i = 0; i <= SNMP_MAX_OID; i++)
    {
        if (lena == 0)
        {
            if (lenb == 0)
            {
                *index = 0;
                return snmpCmpExact;
            }
            else
            {
                if (index)
                {
                    *index = lenb;
                }
                // A is a sub-oid of B
                return snmpCmpAinB;
            }
        }
        else if (lenb == 0)
        {
            if (index)
            {
                *index = lena;
            }
            // B is a sub-oid of A
            return snmpCmpBinA;
        }
        else if (a->oid[i] < b->oid[i])
        {
            if (index)
            {
                *index = 0;
            }
            // A is an oid that comes before B
            return snmpCmpAbeforeB;
        }
        else if (a->oid[i] > b->oid[i])
        {
            if (index)
            {
                *index = 0;
            }
            // A is an oid that comes after B
            return snmpCmpAafterB;
        }
        lena--;
        lenb--;
    }
    // can't get here with valid oids
    //
    return snmpCmpError;
}

int bsnmp_oid_pad_to_index(bsnmp_oid_t *oid, bsnmp_oid_t *baseoid, int index)
{
    if ((baseoid->len + index) >= SNMP_MAX_OID)
    {
        return -1;
    }
    while (oid->len < baseoid->len)
    {
        oid->oid[oid->len] = baseoid->oid[oid->len];
        oid->len++;
    }
    if (index == 0)
    {
        // insure the resultant object is
        // a scalar if there's no index
        //
        if (oid->oid[oid->len] != 0)
        {
            index = 1;
        }
    }
    while (index-- > 0)
    {
        oid->oid[oid->len++] = 0;
    }
    return 0;
}

const char *bsnmp_type_string(bsnmp_type_t type)
{
    switch(type)
    {
    case SNMP_BOOLEAN:          return "BOOLEAN";
    case SNMP_INTEGER:          return "INTEGER";
    case SNMP_BIT_STRING:       return "BIT-STRING";
    case SNMP_OCTET_STRING:     return "OCTET-STRING";
    case SNMP_NULL:             return "NULL";
    case SNMP_OBJECT_ID:        return "OBJECT-ID";
    case SNMP_SEQUENCE:         return "SEQUENCE";
    case SNMP_SETOF:            return "SET";
    case SNMP_PRINTABLE_STR:    return "PRINTABLE_STR";
    case SNMP_UTC_TIME:         return "UTC_TIME";
    case SNMP_IPADDRESS:        return "IP-ADDRESS";
    case SNMP_COUNTER:          return "COUNTER";
    case SNMP_UNSIGNED:         return "UNSIGNED";
    case SNMP_TIMETICKS:        return "TIME-TICKS";
    case SNMP_OPAQUE:           return "OPAQUE";
    case SNMP_COUNTER64:        return "COUNTER64";
    case SNMP_UNSIGNED32:       return "UNSIGNED32";
    case SNMP_FLOAT:            return "FLOAT";
    case SNMP_DOUBLE:           return "DOUBLE";
    case SNMP_INTEGER64:        return "INTEGER64";
    case SNMP_UNSIGNED64:       return "UNSIGNED64";
    case SNMP_FLOAT32:          return "FLOAT32";
    case SNMP_FLOAT64:          return "FLOAT64";
    case SNMP_FLOAT128:         return "FLOAT128";
    default:                    return "UNKNOWN";
    }
}

const char *bsnmp_version_string(int32_t version)
{
    switch (version)
    {
    case SNMP_V1:               return "V1";
    case SNMP_V2C:              return "V2C";
    case SNMP_V3:               return "V3";
    default:                    return "V??";
    }
}

const char *bsnmp_opcode_string(bsnmp_opcode_t code)
{
    switch (code)
    {
    case SNMP_GET:              return "GET";
    case SNMP_GETNEXT:          return "GETNEXT";
    case SNMP_RESPONSE:         return "RESPONSE";
    case SNMP_SET:              return "SET";
    case SNMP_TRAP:             return "TRAP";
    case SNMP_GETBULK:          return "GETBULK";
    case SNMP_INFORM:           return "INFORM";
    case SNMP_NOTIFY:           return "NOTIFY";
    case SNMP_REPORT:           return "REPORT";
    default:                    return "???";
    }
}

const char *bsnmp_errcode_string(bsnmp_errcode_t errcode)
{
    switch (errcode)
    {
    case SNMP_ErrNoError:            return "NoError";
    case SNMP_ErrTooBig:             return "TooBig";
    case SNMP_ErrNoSuchName:         return "NoSuchName";
    case SNMP_ErrBadValue:           return "BadValue";
    case SNMP_ErrReadOnly:           return "ReadOnly";
    case SNMP_ErrGenErr:             return "GenErr";
    case SNMP_ErrNoAccess:           return "NoAccess";
    case SNMP_ErrWrongType:          return "WrongType";
    case SNMP_ErrWrongLength:        return "WrongLength";
    case SNMP_ErrWrongEncoding:      return "WrongEncoding";
    case SNMP_ErrWrongValue:         return "WrongValue";
    case SNMP_ErrNoCreation:         return "NoCreation";
    case SNMP_ErrInconsistentValue:  return "InconsistentValue";
    case SNMP_ErrResourceUnavailable:return "ResourceUnavailable";
    case SNMP_ErrCommitFailed:       return "CommitFailed";
    case SNMP_ErrUndoFailed:         return "UndoFailed";
    case SNMP_ErrAuthorizationError: return "AuthorizationError";
    case SNMP_ErrNotWritable:        return "NotWritable";
    case SNMP_ErrInconsistentName:   return "InconsistentName";
    default:                    return "????";
    }
}

