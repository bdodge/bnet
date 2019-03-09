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

int bsnmp_get_object_dimensionality(
									size_t rec,
									size_t *ndim,
									size_t *ntotal,
									size_t indices[BMIBC_MAX_NODE_INDEX]
								  )
{
	const bmibc_oid_xref_t   *obxref;
	const bmibc_oid_xref_t   *dexref;
	bmibc_record_t 	   *objrecord;
	bmibc_record_t 	   *dexrecord;

	size_t index;
	size_t dex;
	size_t dexrec;
	size_t dexdim;
	size_t count;

	if (rec >= BMIBC_NUM_RECORDS || ! ndim)
	{
		return -1;
	}
	obxref    = &g_oidxreftab[rec];
	objrecord = &g_mib_objects[obxref->record_index];

	if (objrecord->dim == 0)
	{
		// not indexable, 0 dimensiond an 1 possible value
		//
		*ndim = 0;
		*ntotal = 1;
		return 0;
	}
	// count indices of record
	//
	for (index = 0; index < BMIBC_MAX_NODE_INDEX; index++)
	{
		if (! obxref->indices[index])
		{
			break;
		}
	}
	// this many total dimensions (indexes possible)
	//
	*ndim = index;

	// for each index, lookup the indexer and how many dim values
	// it can have (dim count is set by DEFVALs specified in MIB)
	//
	for (dex = 0, count = objrecord->dim; dex < index; dex++)
	{
		dexrec = obxref->indices[dex];
		if (dexrec >= BMIBC_NUM_RECORDS)
		{
			butil_log(0, "Invalid index record %zu\n", dexrec);
			return -1;
		}
		indices[dex] = dexrec;
		dexref = &g_oidxreftab[dexrec];
		dexrecord = &g_mib_objects[dexref->record_index];
		dexdim = dexrecord->dim;
		count *= dexdim;
	}
	*ntotal = count;
	return 0;
}

#if 1 // faster binary search
int bsnmp_find_record(bsnmp_oid_t *oid, bool exact, size_t *recdex)
{
	bmibc_oid_xref_t *xref;
	bsnmp_oid_t 	  recoid;
	size_t 			  mindex;
	size_t 			  maxdex;
	size_t 			  cmpdex;
	size_t 			  prev_cmpdex;
	bsnmp_oidcmp_t 	  cmp;
	size_t 			  index;
	char 			  dbg_buffer[64];
	int 			  result;

	if (! oid || ! recdex)
	{
		return -1;
	}
	if (BMIBC_NUM_RECORDS <= 0)
	{
		return -1;
	}
	// technically, this is wrong, but could avoid crash if caller ignores ret
	//
	*recdex = 0;

	mindex = 0;
	maxdex = BMIBC_NUM_RECORDS - 1;
	cmpdex = (maxdex - mindex) / 2;
	prev_cmpdex = cmpdex;

	result = bsnmp_oid_from_string(&recoid, g_oidxreftab[cmpdex].oidstr);
	if (result)
	{
		return result;
	}
	cmp = bsnmp_oidcmp(&recoid, oid, &index);

	// this binary search finds the "floor" record: which is the record
	// who's oid is closest to but <= the incoming oid
	//
	result = 0;

	while (! result)
	{
		if (cmp == snmpCmpAafterB || cmp == snmpCmpBinA)
		{
			butil_log(5, "Object %s is before object record[%zu] %s, move towards %zu\n",
					bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
					cmpdex,	g_oidxreftab[cmpdex].oidstr, mindex);

			// update newly found upper bound
			//
			maxdex = cmpdex;

			// record oid is above incoming oid, so move it even more lower
			//
			cmpdex = (cmpdex + mindex) / 2;

			if (cmpdex == prev_cmpdex)
			{
				// as low as can go, so fail
				//
				butil_log(3, "Object %s is before the first object record\n",
						bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)));
				result = -1;
				break;
			}
		}
		else if (cmp == snmpCmpAbeforeB)
		{
			butil_log(5, "Object %s is after object record[%zu] %s, move towards %zu\n",
					bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
					cmpdex,	g_oidxreftab[cmpdex].oidstr, maxdex);

			// update newly found closer lower bound
			//
			mindex = cmpdex;

			// lower record oid is clear below incoming oid, move it up half way
			//
			cmpdex = (cmpdex + maxdex + 1) / 2;

			if (cmpdex == prev_cmpdex)
			{
				// moving up didn't happen, at the upper limit
				//
				if (maxdex > (BMIBC_NUM_RECORDS - 1))
				{
					butil_log(3, "Object %s is after the last object record[%zu] %s\n",
							bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
							BMIBC_NUM_RECORDS - 1,
							g_oidxreftab[BMIBC_NUM_RECORDS - 1].oidstr);
					result = -1;
					break;
				}
			}
		}
		else if (cmp == snmpCmpAinB)
		{
			size_t ndim;
			size_t ntot;
			size_t indices[BMIBC_MAX_NODE_INDEX];

			butil_log(5, "Object %s is super-oid of object record[%zu] %s\n",
					bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
					cmpdex,	g_oidxreftab[cmpdex].oidstr);

			*recdex = cmpdex;
			if (! exact)
			{
				break;
			}
			// Look at how far inside A is in B, and make sure the dimensionality
			// is a match if wanting an exact match, else we already found it
			//
			result = bsnmp_get_object_dimensionality(cmpdex, &ndim, &ntot, indices);
			if (result)
			{
				break;
			}
			if (ndim == 0)
			{
				// if record is scalar, allow ONLY a ".0" after
				//
				if ((oid->len != recoid.len + 1) || (oid->oid[oid->len - 1] != 0))
				{
					butil_log(3, "Object %s attempts to index scaler object %s\n",
						bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
						g_oidxreftab[cmpdex].oidstr);
					result = -1;
				}
				break;
			}
			else
			{
				if (oid->len != (recoid.len + ndim))
				{
					butil_log(3, "Object %s has %d indices for record[%zu] %s, which has %d\n",
							bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
							index, cmpdex,
							g_oidxreftab[cmpdex].oidstr,
							ndim);
					result = -1;
				}
				break;
			}
			break;
		}
		else if (cmp == snmpCmpExact)
		{
			// incoming oid exactly matches one in table, which is
			// wrong if wanting exact, since objects are stored without indices and
			// scalers are addressed as ".0"
			//
			butil_log((exact) ? 3 : 5, "Object %s is matched exactly at %d\n",
					bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)), cmpdex);
			if (exact)
			{
				result = -1;
				break;
			}
			*recdex = cmpdex;
			return 0;
		}
		if (cmpdex != prev_cmpdex)
		{
			// recompare at the new lower index
			//
			result = bsnmp_oid_from_string(&recoid, g_oidxreftab[cmpdex].oidstr);
			if (result)
			{
				break;
			}
			cmp = bsnmp_oidcmp(&recoid, oid, &index);
			prev_cmpdex = cmpdex;
			butil_log(5, "cur=%zu min=%zu max=%zu\n", cmpdex, mindex, maxdex);
		}
		else
		{
			// done, if max and min are same or within one, cmp
			//
			butil_log(3, "Object search failed at %zu between %zu and %zu, cmp=%u\n",
					cmpdex, mindex, maxdex, cmp);
			result = -1;
			break;
		}
	}
	return result;
}
#else // slower linear search
int bsnmp_find_record(bsnmp_oid_t *oid, bool exact, size_t *lower_dex)
{
	bmibc_oid_xref_t *xref;
	bsnmp_oidcmp_t 	  cmp;
	bsnmp_oid_t 	  recoid;
	size_t 			  recdex;
	size_t 			  index;
	char 			  dbg_buffer[64];
	int 			  result;

	if (! oid || ! lower_dex)
	{
		return -1;
	}
	if (BMIBC_NUM_RECORDS <= 0)
	{
		return -1;
	}
	// technically, this is wrong, but could avoid crash if caller ignores ret
	//
	*lower_dex = 0;

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
			// incoming oid exactly matches one in table, which is
			// wrong if wanting exact, since objects are stored without indices and
			// scalers are addressed as ".0"
			//
			butil_log((exact) ? 3 : 5, "Object %s is matched exactly at %d\n",
					bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)), recdex);
			if (exact)
			{
				return -1;
			}
			*lower_dex = recdex;
			return 0;
		}
		else if (cmp == snmpCmpAafterB)
		{
			// incoming oid is before this table entry
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
			result = bsnmp_oid_from_string(&recoid, g_oidxreftab[recdex].oidstr);
			if (result)
			{
				return result;
			}
			cmp = bsnmp_oidcmp(&recoid, oid, &index);
			if (cmp == snmpCmpAinB)
			{
				size_t ndim;
				size_t ntot;
				size_t indices[BMIBC_MAX_NODE_INDEX];

				if (! exact)
				{
					*lower_dex = recdex;
					return 0;
				}
				// Look at how far inside A is in B, and make sure the dimensionality
				// is a match if wanting an exact match, else we already found it
				//
				result = bsnmp_get_object_dimensionality(recdex, &ndim, &ntot, indices);
				if (result)
				{
					break;
				}
				if (ndim == 0)
				{
					// if record is scalar, allow ONLY a ".0" after
					//
					if ((oid->len != recoid.len + 1) || (oid->oid[oid->len - 1] != 0))
					{
						butil_log(3, "Object %s attempts to index scaler object %s\n",
							bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
							g_oidxreftab[recdex].oidstr);
						result = -1;
					}
					break;
				}
				else
				{
					if (oid->len != (recoid.len + ndim))
					{
						butil_log(3, "Object %s has %d indices for record[%zu] %s, which has %d\n",
								bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
								index, recdex,
								g_oidxreftab[recdex].oidstr,
								ndim);
						return -1;
					}
				}
				*lower_dex = recdex;
				return 0;
			}
			butil_log(3, "Object %s is before %s but not after %s, not a match\n",
					bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
					g_oidxreftab[recdex + 1].oidstr,
					g_oidxreftab[recdex].oidstr);
			return -1;
		}
	}
	return -1;
}
#endif

int bsnmp_find_next_oid(bsnmp_oid_t *oid, bsnmp_oid_t *nextoid, bsnmp_errcode_t *err)
{
	bmibc_oid_xref_t *xref;
	bsnmp_oidcmp_t    cmp;
	bsnmp_oid_t       recoid;
	size_t            recdex;
	size_t            index;
	char 			  dbg_buffer[64];
	int 			  result;

	if (! oid || ! nextoid)
	{
		return -1;
	}
	*err = SNMP_ErrGenErr;
	nextoid->len = 0;

	// find lower bound record of oid
	//
	result = bsnmp_find_record(oid, false, &recdex);
	if (result)
	{
		*err = SNMP_ErrNoSuchName;
		return result;
	}
	// recompare oid to record's oid to get indexes etc
	//
	result = bsnmp_oid_from_string(&recoid, g_oidxreftab[recdex].oidstr);
	if (result)
	{
		return result;
	}
	cmp = bsnmp_oidcmp(&recoid, oid, &index);
	if (cmp != snmpCmpExact && cmp != snmpCmpAinB)
	{
		butil_log(0, "find_record failed?\n");
		return -1;
	}
	// pad oid out to full index [0][0]...
	//
	result = bsnmp_oid_pad_to_index(nextoid, &recoid, index);
	if (result)
	{
		return result;
	}
	*err = SNMP_ErrNoError;
	return 0;
}

int bsnmp_var_from_record(const bmibc_oid_xref_t *xref, bsnmp_var_t *var)
{
	bmibc_record_t *rec;
	bsnmp_type_t    type;
	bool			isstr;

	if (! xref || ! var)
	{
		return -1;
	}
	rec = &g_mib_objects[xref->record_index];

	isstr = (xref->asntype & 0x80) ? true : false;

	type = (bsnmp_type_t)(xref->asntype & ~0x80);

	var->type = type;
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
	size_t lower_dex;
	int result;

	// make sure var's oid matches one we're finding
	//
	result = bsnmp_oid_copy(&var->oid, oid);
	if (result)
	{
		*err = SNMP_ErrGenErr;
		return result;
	}
	result = bsnmp_find_record(oid, true, &lower_dex);
	if (result)
	{
		*err = SNMP_ErrNoSuchName;
		return result;
	}
	xref = &g_oidxreftab[lower_dex];
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
	char        dbg_buffer[64];
	int result;

	result = bsnmp_find_next_oid(oid, &nextoid, err);
	if (result)
	{
		return result;
	}
	butil_log(5, "Next object is %s\n",
			bsnmp_oid_string(&nextoid, dbg_buffer, sizeof(dbg_buffer)));
	return bsnmp_get_object_value(&nextoid, var, err);
}

int bsnmp_set_object_value(bsnmp_oid_t *oid, bsnmp_var_t *var)
{
	return 0;
}

