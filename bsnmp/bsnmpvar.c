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
#include "bsnmpvar.h"
#include "bsnmpber.h"
#include "bsnmputils.h"
#include "bsnmpio.h"
#include "bsnmp.h"
#include "butil.h"

bsnmp_var_t *bsnmp_var_create()
{
    bsnmp_var_t *var;

	var = (bsnmp_var_t*)malloc(sizeof(bsnmp_var_t));
    if (var)
    {
        var->type = SNMP_NULL;
        var->len  = 0;
        var->alloc_len = 0;
        var->next = NULL;
    }
    else
    {
        butil_log(0, "VB: Alloc var\n");
    }
    return var;
}

void bsnmp_var_destroy(bsnmp_var_t *var)
{
    if (var)
    {
        if (var->alloc_len)
        {
            switch (var->type)
            {
            case SNMP_OCTET_STRING:
                if (var->val.sVal)
                {
                    free(var->val.sVal);
                }
                break;
            case SNMP_OBJECT_ID:
                if (var->val.oVal)
                {
                    free(var->val.oVal);
                }
                break;
            default:
                butil_log(0, "VD: alloced for non string type?\n");
                break;
            }
        }
        free(var);
    }
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

int bsnmp_var_setvalue(bsnmp_var_t *var, bval_t *val)
{
	if (! var || ! val)
	{
		return -1;
	}
	switch (var->type)
	{
	default:
		return -1;
	}
	return 0;
}

bsnmp_var_t *bsnmp_varlist_appendvar(bsnmp_var_t *varlist, bsnmp_var_t *var)
{
	var->next = NULL;

	if (! varlist)
	{
		varlist = var;
	}
	else
	{
	    while (varlist->next)
	    {
	        varlist = varlist->next;
	    }
		varlist->next = var;
	}
	return varlist;
}

void bsnmp_varlist_cleanup(bsnmp_var_t *varlist)
{
    bsnmp_var_t *nv;

    while (varlist)
    {
        nv = varlist->next;
        bsnmp_var_destroy(varlist);
        varlist = nv;
    }
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

        snprintf(buffer, room, "%"LIFS, var->val.llVal);
        break;

    case SNMP_COUNTER64:

        snprintf(buffer, room, "%"LUFS, var->val.ullVal);
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


