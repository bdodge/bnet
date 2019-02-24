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

int bsnmp_var_init(bsnmp_var_t *var, bsnmp_oid_t *oid, bsnmp_type_t type)
{
    int result;
    
    if (! var)
    {
        return -1;
    }
    if (oid)
    {
        result = bsnmp_oid_copy(&var->oid, oid);
    }
    else
    {
        result = 0;
        var->oid.len = 0;
        var->oid.oid[0] = 0;
    }
    var->type = type;
    var->len  = 0;
    var->alloc_len = 0;
    return result;
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
        
        if (*str && *str != '.')
        {
            SNMP_ERROR("OID Not-dot");
            return SNMP_ErrWrongEncoding;
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

const char *bsnmp_var_string(bsnmp_var_t *var, char *buffer, int nBuffer)
{
    char   *pd;
    int     i, j;
    int     room;
    int     printable;

    room = nBuffer;

    // format the data type as a string
    //
    switch (var->type)
    {
    default:
    case SNMP_NULL:
        
        buffer[0] = '\0';
        break;
        
    case SNMP_INTEGER:

        snprintf(buffer, room, "%d", var->val.iVal);
        break;

    case SNMP_BOOLEAN:
    case SNMP_UNSIGNED:
    case SNMP_COUNTER:

        snprintf(buffer, room, "%u", var->val.uVal);
        break;

    case SNMP_IPADDRESS:

        {
            struct in_addr  addr;
            unsigned long   val = htonl(var->val.uVal);
            char           *aip;

            memcpy(&addr, &val, 4);
            aip = inet_ntoa(addr);
            snprintf(buffer, room, "%s", aip);
        }
        break;

    case SNMP_TIMETICKS:

        {
            int d, h, m, s, z;

            z = var->val.uVal;
            d = z / (60 * 60 * 24 * 100);
            z -= (d * 60 * 60 * 24 * 100);
            h = z / (60 * 60 * 100);
            z -= h * (60 * 60 * 100);
            m = z / (60 * 100);
            z -= m * (60 * 100);
            s = z / 100;
            z -= s * 100;

            snprintf(buffer, room, "(%u) %d days, %d:%d:%d.%d",
                    var->val.uVal, d, h, m, s, z);
        }
        break;

    case SNMP_INTEGER64:

        snprintf(buffer, room, "%ld", var->val.llVal);
        break;

    case SNMP_COUNTER64:

        snprintf(buffer, room, "%lu", var->val.ullVal);
        break;

    case SNMP_FLOAT:
    case SNMP_FLOAT32:

        snprintf(buffer, room, "%f", var->val.fVal);
        break;

    case SNMP_FLOAT64:
    case SNMP_FLOAT128:

        snprintf(buffer, room, "%f", var->val.lfVal);
        break;

    case SNMP_OBJECT_ID:

        for (i = 0, pd = buffer; i < var->val.oVal->len - 1; i++)
        {
            snprintf(pd, room, "%u.", var->val.oVal->oid[i]);
            while (*pd)
            {
                pd++;
                room--;
            }
        }
        snprintf(pd, room, "%u", var->val.oVal->oid[i]);
        break;

    case SNMP_OCTET_STRING:

        // for "string" looking strings, return string
        // else return hex printout
        //
        for (i = 0, printable = 1; printable && (i < var->len); i++)
        {
            if (var->val.sVal[i] < 9 || var->val.sVal[i] > 127)
            {
                printable = 0;
            }
        }
        if (printable)
        {
            for (i = 0; i < room-1 && i < var->len; i++)
            {
                buffer[i] = var->val.sVal[i];
            }
            buffer[i] = 0;
        }
        else
        {
            for (i = j = 0; (i < var->len) && ((j + 3) < room); i++)
            {
                j += snprintf(buffer + j, room - j, "%02X ", var->val.bVal[i]);
            }
            buffer[j ? (j-1) : j] = '\0';
        }
        break;
    }
    return buffer;
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

