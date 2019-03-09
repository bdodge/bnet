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
#include "bsnmpobject.h"
#include "bsnmp.h"
#include "bsnmputils.h"
#include "bsnmpvar.h"
#include "butil.h"

// include the (generated) mib header and source here
//
#include "mibc_generated.h"
#include "mibc_generated.c"

int bsnmp_find_record(bsnmp_oid_t *oid, const bmibc_oid_xref_t **recref)
{
	size_t recdex;
	size_t index;
	bmibc_oid_xref_t *xref;
	bsnmp_oidcmp_t cmp;
	bsnmp_oid_t recoid;
	char dbg_buffer[64];
	int result;

	if (! recref)
	{
		return -1;
	}
	*recref = NULL;
	if (! oid)
	{
		return -1;
	}
	for (recdex = 0; recdex < BMIBC_NUM_RECORDS; recdex++)
	{
		// convert record's oid string to an actual oid. this might
		// be cachable, but it would be a large table, so doing it
		// (over and over, kind of slowly) this way saves space
		//
		result = bsnmp_oid_from_string(&recoid, g_oidxreftab[recdex].oidstr);
		if (result)
		{
			return result;
		}
		// compare this to incoming oid
		//
		cmp = bsnmp_oidcmp(&recoid, oid, &index);
		if (cmp == snmpCmpExact)
		{
			// incoming oid exactly matches one in table
			//
			butil_log(3, "Object %s is matched at %d\n",
					bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)), recdex);
			break;
		}
		else if (cmp == snmpCmpAafterB)
		{
			// inccoming oid is before this table entry
			// so check previous entry again
			//
			if (recdex < 1)
			{
				// incoming oid starts before first object
				//
				butil_log(3, "Object %s is before first object\n",
						bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)));
				return -1;
			}
			recdex--;
			cmp = bsnmp_oidcmp(&recoid, oid, &index);
			if (cmp == snmpCmpAinB)
			{
				// Look at how far inside A is in B, and make
				// sure the dimensionality is a match
				//
				size_t lendiff = oid->len - recoid.len;

				if (lendiff != index)
				{
					butil_log(3, "Object %s has %d indices, but oid %s specifies %d\n",
							bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
							index,
							g_oidxreftab[recdex].oidstr,
							lendiff);
					return -1;
				}
				butil_log(3, "Object %s with %d indices is matched at %d\n",
						bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)), recdex);
				break;
			}
			butil_log(3, "Object %s is before %s but not after %s, not match\n",
					bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
					g_oidxreftab[recdex + 1].oidstr,
					g_oidxreftab[recdex].oidstr);
			return -1;
		}
	}
	*recref = &g_oidxreftab[recdex];
	return 0;
}

int bsnmp_find_next_oid(bsnmp_oid_t *oid, bsnmp_oid_t *nextoid, bsnmp_errcode_t *err)
{
	size_t recdex;
	size_t index;
	bmibc_oid_xref_t *xref;
	bsnmp_oidcmp_t cmp;
	bsnmp_oid_t recoid;
	char dbg_buffer[64];
	int result;

	if (! oid || ! nextoid)
	{
		return -1;
	}
	nextoid->len = 0;

	for (recdex = 0; recdex < BMIBC_NUM_RECORDS; recdex++)
	{
		// convert record's oid string to an actual oid. this might
		// be cachable (see note in bsnmp_find_record)
		//
		result = bsnmp_oid_from_string(&recoid, g_oidxreftab[recdex].oidstr);
		if (result)
		{
			return result;
		}
		// compare this to incoming oid
		//
		cmp = bsnmp_oidcmp(&recoid, oid, &index);
		if (cmp == snmpCmpExact || cmp == snmpCmpBinA)
		{
			// incoming oid exactly matches one in table, or is a
			// sub-oid of one in table
			//
			// if the object is indexed, the next object is the first
			// indexed actual object, i.e. [0][0]... so just append
			// 0 oids to end of oid to complete full index
			//
			// pad oid out to full index [0][0]...
			//
			result = bsnmp_oid_pad_to_index(nextoid, &recoid, index);
			if (result)
			{
				*err = SNMP_ErrGenErr;
				return result;
			}
			butil_log(3, "Object %s is partial oid of rec %d %s indexed %d 0s\n",
					bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
					recdex,
					g_oidxreftab[recdex].oidstr,
					index);
			*err = SNMP_ErrNoError;
			return 0;
		}
		else if (cmp == snmpCmpAinB)
		{
			// incoming oid is a super-oid of this table entry
			// this is the fun case where the oid might be a partial
			// or complete indexed oid of the records oid, in which
			// case we pad or increment the index from right to left and
			// use that, unless the index is at the full dimension of
			// the object in which case we move to the next record.
			//
			result = bsnmp_oid_pad_to_index(nextoid, &recoid, index);
			if (result)
			{
				*err = SNMP_ErrGenErr;
				return result;
			}
		}
		else if (cmp == snmpCmpAafterB)
		{
			// incoming oid clearly before record, so stop here
			//
			result = bsnmp_oid_pad_to_index(nextoid, &recoid, index);
			if (result)
			{
				*err = SNMP_ErrGenErr;
				return result;
			}
			butil_log(3, "Object %s is before oid of rec %d %s indexed %d 0s\n",
					bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
					recdex,
					g_oidxreftab[recdex].oidstr,
					index);
			*err = SNMP_ErrNoError;
			return 0;
		}
	}
	*err = SNMP_ErrNoSuchName;
	return 1;
}

int bsnmp_var_from_record(const bmibc_oid_xref_t *xref, bsnmp_var_t *var)
{
	bmibc_record_t *rec;

	if (! xref || ! var)
	{
		return -1;
	}
	rec = &g_mib_objects[xref->record_index];

	var->type = (bsnmp_type_t)xref->asntype;
	var->len  = 0;
	var->val.llVal = 0;

	switch (var->type)
	{
    case SNMP_NULL:
		var->val.sVal = NULL;
        break;
    case SNMP_INTEGER:
		var->val.iVal = 33;
        break;
    case SNMP_BOOLEAN:
    case SNMP_UNSIGNED:
    case SNMP_UNSIGNED32:
    case SNMP_IPADDRESS:
    case SNMP_COUNTER:
    case SNMP_TIMETICKS:
        break;
    case SNMP_COUNTER64:
    case SNMP_UNSIGNED64:
        break;
    case SNMP_INTEGER64:
        break;
    case SNMP_OCTET_STRING:
        break;
    case SNMP_OBJECT_ID:
        break;
    case SNMP_PRINTABLE_STR:
    case SNMP_UTC_TIME:
    case SNMP_OPAQUE:
    case SNMP_NSAPADDRESS:
    case SNMP_SEQUENCE:
    case SNMP_SETOF:
    case SNMP_BIT_STRING:
    case SNMP_FLOAT:
    case SNMP_DOUBLE:
    case SNMP_FLOAT32:
    case SNMP_FLOAT64:
    case SNMP_FLOAT128:
        butil_log(0, "Type not supported: %u\n", var->type);
        return -1;

    default:
        butil_log(0, "Not a type: %u\n", var->type);
        return -1;
 	}
	return 0;
}

int bsnmp_get_object_value(bsnmp_oid_t *oid, bsnmp_var_t *var, bsnmp_errcode_t *err)
{
	const bmibc_oid_xref_t *xref;
	int result;

	result = bsnmp_find_record(oid, &xref);
	if (result)
	{
		*err = SNMP_ErrNoSuchName;
		return result;
	}
	result = bsnmp_var_from_record(xref, var);
	if (result)
	{
		*err = SNMP_ErrBadValue;
		return result;
	}
	*err = SNMP_ErrNoError;
	return 0;
}

int bsnmp_get_next_object_value(bsnmp_oid_t *oid, bsnmp_var_t *var, bsnmp_errcode_t *err)
{
	bsnmp_oid_t nextoid;
	int result;

	result = bsnmp_find_next_oid(oid, &nextoid, err);
	if (result)
	{
		return result;
	}
	return bsnmp_get_object_value(&nextoid, var, err);
}

int bsnmp_set_object_value(bsnmp_oid_t *oid, bsnmp_var_t *var)
{
	return 0;
}

