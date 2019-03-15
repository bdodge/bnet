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

static size_t s_num_records;
static bmibc_record_t *s_records;
static const bmibc_oid_xref_t *s_xrefs;

int bsnmp_get_object_dimensionality(
                                    size_t rec,
                                    size_t *ndim,
                                    size_t *ntotal,
                                    size_t indices[BSNMP_MAX_DIMENSIONS]
                                  )
{
    const bmibc_oid_xref_t   *obxref;
    const bmibc_oid_xref_t   *dexref;
    bmibc_record_t     *objrecord;
    bmibc_record_t     *dexrecord;

    size_t index;
    size_t dex;
    size_t dexrec;
    size_t dexdim;
    size_t count;

    if (rec >= s_num_records || ! ndim)
    {
        return -1;
    }
    obxref    = &s_xrefs[rec];
    objrecord = &s_records[obxref->record_index];

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
    for (index = 0; index < BSNMP_MAX_DIMENSIONS; index++)
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
        if (dexrec >= s_num_records)
        {
            butil_log(0, "Invalid index record %zu\n", dexrec);
            return -1;
        }
        indices[dex] = dexrec;
        dexref = &s_xrefs[dexrec];
        dexrecord = &s_records[dexref->record_index];
        dexdim = dexrecord->dim;
        count *= dexdim;
    }
    while (dex < BSNMP_MAX_DIMENSIONS)
    {
        indices[dex++] = 0;
    }
    *ntotal = count;
    return 0;
}

int bsnmp_oid_from_name(bsnmp_oid_t *oid, const char *name)
{
#if BSNMP_OBJECT_NAMES
    size_t recdex;
    size_t ndim;
    size_t ntot;
    size_t indices[BSNMP_MAX_DIMENSIONS];
    int    result;

    if (! name || ! oid)
    {
        return -1;
    }
    for (recdex = 0; recdex < s_num_records; recdex++)
    {
        if (! strcmp(s_records[recdex].name, name))
        {
            result = bsnmp_oid_from_string(oid, s_xrefs[recdex].oidstr);
            if (result)
            {
                return result;
            }
            // assume-unindex oid
            result = bsnmp_get_object_dimensionality(recdex, &ndim, &ntot, indices);
            if (result)
            {
                break;
            }
            result = bsnmp_oid_pad_to_index(oid, oid, ndim);
            if (result)
            {
                return result;
            }
            return 0;
        }
    }
#endif
    return -1;
}

#if 1 // faster binary search
int bsnmp_find_record(bsnmp_oid_t *oid, bool exact, size_t *recdex)
{
    bmibc_oid_xref_t *xref;
    bsnmp_oid_t       recoid;
    size_t            mindex;
    size_t            maxdex;
    size_t            cmpdex;
    size_t            prev_cmpdex;
    size_t            prev_prev_cmpdex;
    bsnmp_oidcmp_t    cmp;
    size_t            index;
    char              dbg_buffer[64];
    int               result;

    if (! oid || ! recdex)
    {
        return -1;
    }
    if (s_num_records <= 0)
    {
        return -1;
    }
    // technically, this is wrong, but could avoid crash if caller ignores ret
    //
    *recdex = 0;

    mindex = 0;
    maxdex = s_num_records - 1;
    cmpdex = (maxdex - mindex) / 2;
    prev_cmpdex = cmpdex;
    prev_prev_cmpdex = cmpdex;

    result = bsnmp_oid_from_string(&recoid, s_xrefs[cmpdex].oidstr);
    if (result)
    {
        return result;
    }
    cmp = bsnmp_oid_cmp(&recoid, oid, &index);

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
                    cmpdex, s_xrefs[cmpdex].oidstr, mindex);

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
            if (cmpdex == (prev_cmpdex - 1))
            {
                // if only going down one slot, and been here before, that means
                // oid is a sub-oid of this record, or between this one
                // and the previous one, so avoid bouncing
                //
                if (cmpdex == prev_prev_cmpdex)
                {
                    if (exact)
                    {
                        butil_log(5, "Object %s is not in/after object record[%zu] %s either, so no match\n",
                                bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                                cmpdex, s_xrefs[cmpdex].oidstr);
                        result = -1;
                        break;
                    }
                    if (cmp == snmpCmpBinA)
                    {
                        // go back to record where B in A
                        cmpdex = maxdex;
                        butil_log(5, "Object %s is actually a suboid of object record[%zu] %s, so done\n",
                                bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                                cmpdex, s_xrefs[maxdex].oidstr);
                    }
                    else
                    {
                        // stay at lower record
                        butil_log(5, "Object %s is actually after(not in) object record[%zu] %s, so done\n",
                                bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                                cmpdex, s_xrefs[maxdex].oidstr);
                    }
                    *recdex = cmpdex;
                    return 0;
                }
            }
        }
        else if (cmp == snmpCmpAbeforeB)
        {
            butil_log(5, "Object %s is after object record[%zu] %s, move towards %zu\n",
                    bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                    cmpdex, s_xrefs[cmpdex].oidstr, maxdex);

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
                if (maxdex > (s_num_records - 1))
                {
                    butil_log(3, "Object %s is after the last object record[%zu] %s\n",
                            bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                            s_num_records - 1,
                            s_xrefs[s_num_records - 1].oidstr);
                    result = -1;
                    break;
                }
            }
        }
        else if (cmp == snmpCmpAinB)
        {
            size_t ndim;
            size_t ntot;
            size_t indices[BSNMP_MAX_DIMENSIONS];

            butil_log(5, "Object %s is super-oid of object record[%zu] %s\n",
                    bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                    cmpdex, s_xrefs[cmpdex].oidstr);

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
                        s_xrefs[cmpdex].oidstr);
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
                            s_xrefs[cmpdex].oidstr,
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
            result = bsnmp_oid_from_string(&recoid, s_xrefs[cmpdex].oidstr);
            if (result)
            {
                break;
            }
            cmp = bsnmp_oid_cmp(&recoid, oid, &index);
            prev_prev_cmpdex = prev_cmpdex;
            prev_cmpdex = cmpdex;
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
    bsnmp_oidcmp_t    cmp;
    bsnmp_oidcmp_t    prevcmp;
    bsnmp_oid_t       recoid;
    size_t            recdex;
    size_t            index;
    size_t            ndim;
    size_t            ntot;
    size_t            indices[BSNMP_MAX_DIMENSIONS];
    char              dbg_buffer[64];
    int               result;

    if (! oid || ! lower_dex)
    {
        return -1;
    }
    if (s_num_records <= 0)
    {
        return -1;
    }
    // technically, this is wrong, but could avoid crash if caller ignores ret
    //
    *lower_dex = 0;
    prevcmp = snmpCmpExact; /* not important, just fixes warning */

    for (recdex = 0; recdex <= s_num_records; recdex++)
    {
        if (recdex < s_num_records)
        {
            // convert record's oid string to an actual oid. this might
            // be cachable, but it would be a large table, so doing it
            // (over and over, kind of slowly) this way saves space
            //
            result = bsnmp_oid_from_string(&recoid, s_xrefs[recdex].oidstr);
            if (result)
            {
                return result;
            }
            cmp = bsnmp_oid_cmp(&recoid, oid, &index);

            butil_log(5, "Compare object %s to object record[%zu] %s gets %s, prev=%s\n",
                    bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                    recdex, s_xrefs[recdex].oidstr,
                    bsnmp_cmp_str(cmp), bsnmp_cmp_str(prevcmp));
        }
        else
        {
            cmp = snmpCmpAafterB;
        }
        // compare the record A to incoming oid B until gone past (AbeforeB)
        //
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
            // went past oid, so go back one and go from there
            //
            if (recdex < 1)
            {
                butil_log(3, "Object %s is before first record\n",
                    bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)));
                return -1;
            }
            recdex--;

            result = bsnmp_oid_from_string(&recoid, s_xrefs[recdex].oidstr);
            if (result)
            {
                return result;
            }
            // this record was the last one compared, so prevcmp is the result and
            // it had better be AinB or AbeforeB, since Exact or AafterB would have already been
            // handled
            //
            if (prevcmp == snmpCmpAinB)
            {
                butil_log(5, "Object %s is a super-oid of object record[%zu] %s\n",
                        bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                        recdex, s_xrefs[recdex].oidstr);

                *lower_dex = recdex;
                if (! exact)
                {
                    // found match for base record
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
                            s_xrefs[recdex].oidstr);
                        result = -1;
                        break;
                    }
                }
                else
                {
                    if (oid->len != (recoid.len + ndim))
                    {
                        butil_log(3, "Object %s has %d indices for record[%zu] %s, which has %d\n",
                                bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                                index, recdex,
                                s_xrefs[recdex].oidstr,
                                ndim);
                        result = -1;
                        break;
                    }
                }
                return 0;
            }
            else if (prevcmp == snmpCmpAbeforeB)
            {
                *lower_dex = recdex;
                if (! exact)
                {
                    // incoming oid is between prev and this record, but not even a
                    // partial match for either, so lower is base if not exact match
                    //
                    return 0;
                }
                butil_log(3, "Object %s is not in MIB after %s between %zu and %zu\n",
                        bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                        s_xrefs[recdex].oidstr, recdex, recdex + 1);
                return -1;
            }
            else
            {
                butil_log(3, "Expected prevcmp to be A<B or AinB for %s, was %dn",
                        bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)), prevcmp);
                return -1;
            }
        }
        else if (cmp == snmpCmpBinA)
        {
            butil_log(5, "Object %s is actually a suboid of object record[%zu] %s, so done\n",
                    bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)),
                    recdex, s_xrefs[recdex].oidstr);
            *lower_dex = recdex;
            if (exact)
            {
                return -1;
            }
            return 0;
        }
        /* else BinA or AbeforeB means keep going */
        prevcmp = cmp;
    }
    butil_log(5, "Object %s is after last object record\n",
            bsnmp_oid_string(oid, dbg_buffer, sizeof(dbg_buffer)));
    return -1;
}
#endif

int bsnmp_find_next_oid(bsnmp_oid_t *oid, bsnmp_oid_t *nextoid, bsnmp_errcode_t *err)
{
    bmibc_record_t         *obj;
    const bmibc_oid_xref_t *xref;
    bsnmp_oidcmp_t  cmp;
    bsnmp_oid_t     recoid;
    size_t          recdex;
    size_t          index;
    size_t          ndim;
    size_t          ntot;
    size_t          indices[BSNMP_MAX_DIMENSIONS];
    char            dbg_buffer[64];
    int             result;

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
    butil_log(5, "find_next: base oid[%zu]=%s\n", recdex, s_xrefs[recdex].oidstr);

    // get ptr to objects record
    //
    xref = &s_xrefs[recdex];
    obj  = &s_records[xref->record_index];

    // convert string to oid type for base record
    //
    result = bsnmp_oid_from_string(&recoid, xref->oidstr);
    if (result)
    {
        return result;
    }
    // if the base record is not indexed, then just return the next object's
    // record if the base is fully indexed, or this one if not
    //
    if (! obj->dim)
    {
        // if incoming oid is <= in length to base oid then it
        // isn't fully indexed. even for 0 dim record, it could
        // have a .0 added to make it complete, so use base for
        // next oid in these cases
        //
        if (oid->len > recoid.len)
        {
            if (recdex >= (s_num_records - 1))
            {
                butil_log(3, "find_next: past last record\n");
                *err = SNMP_ErrNoSuchName;
                return -1;
            }
            // next record
            recdex++;

            // convert to oid
            result = bsnmp_oid_from_string(&recoid, s_xrefs[recdex].oidstr);
            if (result)
            {
                return result;
            }
        }
        // get dimensionality of this object
        result = bsnmp_get_object_dimensionality(recdex, &ndim, &ntot, indices);
        if (result)
        {
            return result;
        }
        // add any .0 or .1's as needed to get to first index of object
        result = bsnmp_oid_pad_to_index(nextoid, &recoid, ndim);
        if (result)
        {
            return result;
        }
        butil_log(5, "find_next: next oid[%zu][%zu] = %s\n",
                    recdex, ndim, s_xrefs[recdex].oidstr);
        *err = SNMP_ErrNoError;
        return 0;
    }
    // recompare oid to record's oid to see where index is at
    //
    cmp = bsnmp_oid_cmp(&recoid, oid, &index);
    if (cmp != snmpCmpExact && cmp != snmpCmpAinB && cmp != snmpCmpBinA)
    {
        butil_log(0, "find_record failed?\n");
        return -1;
    }
    // get dimensionality of the object
    result = bsnmp_get_object_dimensionality(recdex, &ndim, &ntot, indices);
    if (result)
    {
        return result;
    }
    if (cmp == snmpCmpExact || cmp == snmpCmpBinA || index < ndim)
    {
        // the incoming OID has no indexing so just
        // pad oid out to full index [0][0]...
        //
        if (cmp == snmpCmpExact || cmp == snmpCmpBinA)
        {
            butil_log(5, "find_next: pad out non-indexed first val oid\n");
        }
        else
        {
            butil_log(5, "find_next: pad out semi-indexed first val oid\n");
        }
        // add any .0 or .1's as needed to get to first index of object
        result = bsnmp_oid_pad_to_index(nextoid, &recoid, ndim);
        if (result)
        {
            return result;
        }
        *err = SNMP_ErrNoError;
        return 0;
    }
    if (index > ndim)
    {
        // incoming specifies more indices than can be used. decide here
        // if this means return the next object or an error
        //
        butil_log(5, "find_next: oid over-indexed, expect %zu, got %zu\n",
                ndim, index);
        *err = SNMP_ErrNoSuchName;
        return -1;
    }
    // now the fun part, increment the indices
    //
    butil_log(5, "find_next: increment index in indexed oid\n");

    // increment the right most index until it reaches the total
    // number of values it can have, then wrap it to 0 and increment
    // the index to its left, etc. just like counting
    //
    for (index = 1; index <= ndim; index++)
    {
        // point to the record describing the index for
        // current dimension
        //
        obj = &s_records[xref->indices[ndim - index]];

        if ((oid->oid[oid->len - index] + 1) < 1)
        {
            // invalid index, not gonna do it
            butil_log(3, "find_next: invalid 0 index\n");
            *err = SNMP_ErrNoSuchName;
            return -1;
        }
        if ((oid->oid[oid->len - index] + 1) > obj->dim)
        {
            // reached limit on this index, if this is
            // the last index to left, this object can't
            // be indexed any further
            //
            if (index == ndim)
            {
                index++;
                break;
            }
            oid->oid[oid->len - index] = 0;
        }
        else
        {
            // increment current index and use it
            oid->oid[oid->len - index]++;
            break;
        }
    }
    // if went through indices and didn't increment one
    // then move to next record
    //
    if (index > ndim)
    {
        if (recdex >= (s_num_records - 1))
        {
            butil_log(3, "find_next: past last record\n");
            *err = SNMP_ErrNoSuchName;
            return -1;
        }
        // next record
        recdex++;

        // convert to oid
        result = bsnmp_oid_from_string(&recoid, s_xrefs[recdex].oidstr);
        if (result)
        {
            return result;
        }
        // get dimensionality of this object
        result = bsnmp_get_object_dimensionality(recdex, &ndim, &ntot, indices);
        if (result)
        {
            return result;
        }
        // add any .0 or .1 as needed to get to first index of object
        result = bsnmp_oid_pad_to_index(nextoid, &recoid, ndim);
        if (result)
        {
            return result;
        }
        butil_log(5, "find_next: past indexed limits next oid[%zu] = %s\n",
                    recdex, s_xrefs[recdex].oidstr);
    }
    else
    {
        bsnmp_oid_copy(nextoid, oid);

        butil_log(5, "find_next: incremented %zu dimensions\n", ndim);
    }
    *err = SNMP_ErrNoError;
    return 0;
}

int bsnmp_get_dim_offset(size_t recdex, bsnmp_oid_t *varoid, size_t *offset)
{
    bmibc_record_t *rec;
    size_t      ndim;
    size_t      ntot;
    size_t      indices[BSNMP_MAX_DIMENSIONS];
    size_t      dim_offset;
    size_t      objdex;
    int         result;

    dim_offset = 0;
    *offset = 0;

    result = bsnmp_get_object_dimensionality(recdex, &ndim, &ntot, indices);
    if (result)
    {
        return result;
    }
    if (ndim)
    {
        bmibc_record_t *obj;
        bsnmp_oid_t recoid;
        size_t index;
        size_t indval;
        size_t offsetmult;

        result = bsnmp_oid_from_string(&recoid, s_xrefs[recdex].oidstr);
        if (result)
        {
            return result;
        }
        if (varoid->len != recoid.len + ndim)
        {
            butil_log(3, "get_dim: bad index count on var oid\n");
            return -1;
        }
        // each index is described by an object which has  0..dim
        // as its range, so each index value is multiplied by all
        // indexing dimensions to their right to get storage offset
        //
        offsetmult = 1;

        for (index = 1; index <= ndim; index++)
        {
            // point to the record describing the index for
            // current dimension (note going right to left)
            //
            objdex = s_xrefs[recdex].indices[ndim - index];
            obj = &s_records[objdex];

            // add the oids index to storage offset
            //
            indval = varoid->oid[varoid->len - index];

            if (indval < 1 || indval > obj->dim)
            {
                butil_log(3, "get_dim: bad index value %zu at index %zu, range is 1-%zu\n",
                        indval, ndim - index, obj->dim);
                return -1;
            }
            butil_log(6, "Index %zu is %zu using obj %s with range %zu-%zu\n",
                    ndim - index, indval,
                    s_xrefs[objdex].oidstr,
                    obj->minv, obj->maxv);

            dim_offset += offsetmult * (indval - 1);

            // moving left, each index is multiplied
            // by this objects max dimensions
            //
            offsetmult *= obj->dim;
        }
    }
    *offset = dim_offset;
    return 0;
}

int bsnmp_var_from_record(const bmibc_oid_xref_t *xref, bsnmp_var_t *var, bsnmp_errcode_t *err)
{
    bmibc_record_t *rec;
    bsnmp_type_t    type;
    int             result;
    bool            isstr;
    size_t          dim_offset;
    intptr_t        valptr;

    if (! xref || ! var)
    {
        *err = SNMP_ErrGenErr;
        return -1;
    }
    isstr = (xref->asntype & 0x80) ? true : false;
    type = (bsnmp_type_t)(xref->asntype & ~0x80);

    var->type = type;
    var->len  = 0;
    var->alloc_len = 0;
    var->val.llVal = 0;

    result = bsnmp_get_dim_offset(xref->record_index, &var->oid, &dim_offset);
    if (result)
    {
        *err = SNMP_ErrNoSuchName;
        return result;
    }
    rec = &s_records[xref->record_index];

    // if the record has dimensions, is a string type, or takes
    // more than 32 bits, the value is stored outside the record
    // else the val ptr field is used directly for value store
    //
    if (rec->dim || isstr || rec->bits > 32)
    {
        valptr = (intptr_t)rec->value;
    }
    else
    {
        valptr = (intptr_t)&rec->value;
    }
    switch (var->type)
    {
    case SNMP_NULL:
        var->val.sVal = NULL;
        break;
    case SNMP_INTEGER:
        var->val.iVal = *((int32_t *)valptr + dim_offset);
        break;
    case SNMP_BOOLEAN:
    case SNMP_UNSIGNED:
    case SNMP_UNSIGNED32:
    case SNMP_IPADDRESS:
    case SNMP_COUNTER:
    case SNMP_TIMETICKS:
        var->val.iVal = *((uint32_t *)valptr + dim_offset);
        break;
    case SNMP_COUNTER64:
    case SNMP_UNSIGNED64:
        var->val.ullVal = *((uint64_t *)valptr + dim_offset);
        break;
    case SNMP_INTEGER64:
        var->val.llVal = *((int64_t *)valptr + dim_offset);
        break;
    case SNMP_OCTET_STRING:
        if (! isstr)
        {
            var->val.ucVal = *((uint8_t *)valptr + dim_offset);
        }
        else
        {
            // for strings, each is allocated up to maxv bytes
            dim_offset *= (rec->maxv + 1);
            var->val.sVal = (char *)valptr + dim_offset;

            // first byte stores length of string
            var->len = *var->val.sVal++;
        }
        break;
    case SNMP_OBJECT_ID:
        // for objects, each is allocated sizeof type
        var->val.oVal = (bsnmp_oid_t *)valptr + dim_offset;
        var->len = var->val.oVal->len;
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
    *err = SNMP_ErrNoError;
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
    xref = &s_xrefs[lower_dex];
    result = bsnmp_var_from_record(xref, var, err);
    if (result)
    {
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

int bsnmp_record_from_var(const bmibc_oid_xref_t *xref, bsnmp_var_t *var, bsnmp_errcode_t *err)
{
    bmibc_record_t *rec;
    bsnmp_type_t    rec_type;
    bsnmp_type_t    var_type;
    bool            rec_isstr;
    bool            var_isstr;
    int             result;
    size_t          dim_offset;
    intptr_t        valptr;

    if (! xref || ! var)
    {
        return -1;
    }
    result = bsnmp_get_dim_offset(xref->record_index, &var->oid, &dim_offset);
    if (result)
    {
        *err = SNMP_ErrNoSuchName;
        return result;
    }
    rec = &s_records[xref->record_index];

    rec_isstr = (xref->asntype & 0x80) ? true : false;
    rec_type = (bsnmp_type_t)(xref->asntype & ~0x80);

    var_isstr = (var->type & 0x80) ? true : false;
    var_type  = var->type & ~0x80;

    // if the record has dimensions, is a string type, or takes
    // more than 32 bits, the value is stored outside the record
    // else the val ptr field is used directly for value store
    //
    if (rec->dim || rec_isstr || rec->bits > 32)
    {
        valptr = (intptr_t)rec->value;
    }
    else
    {
        valptr = (intptr_t)&rec->value;
    }
    switch (rec_type)
    {
    case SNMP_NULL:
        break;
    case SNMP_INTEGER:
        if (var_type != SNMP_INTEGER)
        {
            *err = SNMP_ErrWrongType;
            return -1;
        }
        *((int32_t *)valptr + dim_offset) = var->val.iVal;
        break;
    case SNMP_BOOLEAN:
    case SNMP_UNSIGNED:
    case SNMP_UNSIGNED32:
    case SNMP_IPADDRESS:
    case SNMP_COUNTER:
    case SNMP_TIMETICKS:
        switch (var_type)
        {
        case SNMP_BOOLEAN:
        case SNMP_UNSIGNED:
        case SNMP_UNSIGNED32:
        case SNMP_IPADDRESS:
        case SNMP_COUNTER:
        case SNMP_TIMETICKS:
            *((uint32_t *)valptr + dim_offset) = var->val.uVal;
            break;
        default:
            *err = SNMP_ErrWrongType;
            return -1;
        }
        break;
    case SNMP_COUNTER64:
    case SNMP_UNSIGNED64:
        if (var_type != SNMP_COUNTER64 && var_type != SNMP_UNSIGNED64)
        {
            *err = SNMP_ErrWrongType;
            return -1;
        }
        *((uint64_t *)valptr + dim_offset) = var->val.ullVal;
        break;
    case SNMP_INTEGER64:
        if (var_type != SNMP_INTEGER64)
        {
            *err = SNMP_ErrWrongType;
            return -1;
        }
        *((int32_t *)valptr + dim_offset) = var->val.llVal;
        break;
    case SNMP_OCTET_STRING:
        if (var_type != SNMP_OCTET_STRING)
        {
            *err = SNMP_ErrWrongType;
            return -1;
        }
        if (!rec_isstr)
        {
            *((uint8_t *)valptr + dim_offset) = (uint32_t)var->val.ucVal;
        }
        else
        {
            size_t len;

            len = strlen(var->val.sVal);
            if (len >= (rec->maxv - 2))
            {
                butil_log(0, "String set overflow %s\n", var->val.sVal);
                len = rec->maxv - 2;
            }
            // for strings, each is allocated up to maxv bytes
            dim_offset *= (rec->maxv + 1);
            strncpy((char *)valptr + dim_offset + 1, var->val.sVal, len);
            *((char*)valptr + dim_offset + len + 1) = '\0';
            *((uint8_t*)valptr + dim_offset) = (uint8_t)len;
        }
        break;
    case SNMP_OBJECT_ID:
        if (var_type != SNMP_OBJECT_ID)
        {
            *err = SNMP_ErrWrongType;
            return -1;
        }
        if (var->val.oVal->len >= (rec->maxv - 2))
        {
            butil_log(0, "OID set overflow\n");
            *err = SNMP_ErrTooBig;
            return -1;
        }
        memcpy((char *)((bsnmp_oid_t *)valptr + dim_offset), var->val.oVal, sizeof(bsnmp_oid_t));
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
        *err = SNMP_ErrWrongType;
        butil_log(0, "Type not supported: %u\n", var->type);
        return -1;

    default:
        *err = SNMP_ErrWrongType;
        butil_log(0, "Not a type: %u\n", var->type);
        return -1;
    }
    return 0;
}

int bsnmp_set_object_value(
                            bsnmp_var_t *var,
                            bsnmp_errcode_t *err
                          )
{
    size_t recdex;
    int result;

    result = bsnmp_find_record(&var->oid, true, &recdex);
    if (result)
    {
        *err = SNMP_ErrNoSuchName;
        return result;
    }
    return bsnmp_record_from_var(&s_xrefs[recdex], var, err);
}

static bool bsnmp_is_object_name(const char *str)
{
    bool isstr;

    // if string is all numbers and dots, its an oid
    //
    while (*str)
    {
        if ((*str != '.') && (*str < '0' || *str > '9'))
        {
            return true;
        }
        str++;
    }
    return false;
}

int bsnmp_set_string_value(
                            const char *oidstr,
                            const char *str,
                            bsnmp_errcode_t *err
                          )
{
    bsnmp_var_t var;
    int result;

    if (bsnmp_is_object_name(oidstr))
    {
        result = bsnmp_oid_from_name(&var.oid, oidstr);
    }
    else
    {
        result = bsnmp_oid_from_string(&var.oid, oidstr);
    }
    if (result)
    {
        *err = SNMP_ErrNoSuchName;
        return result;
    }
    var.len = 0;
    var.alloc_len = 0;
    var.type = SNMP_OCTET_STRING;
    var.val.sVal = (char*)str;

    return bsnmp_set_object_value(&var, err);
}

int bsnmp_set_uint_value(
                            const char *oidstr,
                            uint32_t uval,
                            bsnmp_errcode_t *err
                          )
{
    bsnmp_var_t var;
    int result;

    if (bsnmp_is_object_name(oidstr))
    {
        result = bsnmp_oid_from_name(&var.oid, oidstr);
    }
    else
    {
        result = bsnmp_oid_from_string(&var.oid, oidstr);
    }
    if (result)
    {
        *err = SNMP_ErrNoSuchName;
        return result;
    }
    var.len = 0;
    var.alloc_len = 0;
    var.type = SNMP_UNSIGNED32;
    var.val.uVal = uval;

    return bsnmp_set_object_value(&var, err);
}

int bsnmp_set_int_value(
                            const char *oidstr,
                            int32_t ival,
                            bsnmp_errcode_t *err
                          )
{
    bsnmp_var_t var;
    int result;

    if (bsnmp_is_object_name(oidstr))
    {
        result = bsnmp_oid_from_name(&var.oid, oidstr);
    }
    else
    {
        result = bsnmp_oid_from_string(&var.oid, oidstr);
    }
    if (result)
    {
        return result;
    }
    var.len = 0;
    var.alloc_len = 0;
    var.type = SNMP_INTEGER;
    var.val.iVal = ival;

    return bsnmp_set_object_value(&var, err);
}

int bsnmp_init_objects(bmibc_record_t *records, const bmibc_oid_xref_t *xrefs, size_t num_records)
{
    if (! records || ! xrefs || ! num_records)
    {
        return -1;
    }
    s_num_records = num_records;
    s_records = records;
    s_xrefs = xrefs;
    return 0;
}

