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
#include "bsnmpber.h"
#include "bsnmpio.h"
#include "bsnmputils.h"
#include "bsnmp.h"
#include "butil.h"

int bsnmp_length_from_ber(bsnmp_server_t *server, uint32_t *len)
{
    int result;
    uint8_t b;
    uint32_t val;

    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    if (b & 0x80)
    {
        uint32_t vallen;

        // length is number of bytes of length, not actual length
        vallen = b & 0x7F;
        if (vallen > 4)
        {
            server->last_error = SNMP_ErrTooBig;
            return -1;
        }
        while (vallen > 0)
        {
            result = bsnmp_next_byte(server, &b);
            if (result)
            {
                return result;
            }
            val <<= 8;
            val |= (uint32_t)b;
            vallen--;
        }
    }
    else
    {
        // simple small len
        val = (uint32_t)b;
    }
    *len = val;
    return 0;
}

int bsnmp_typecode_from_ber(bsnmp_server_t *server, bsnmp_type_t *typecode)
{
    int result;
    uint8_t b;

    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    if ((b & 0x1F) != 0x1F) // not [IMPLICIT]
    {
        *typecode = b;
        return 0;
    }
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    if (b >= 0x30)
    {
        b -= 0x30;
    }
    *typecode = (bsnmp_type_t)b;
    return 0;
}

int bsnmp_typelen_from_ber(bsnmp_server_t *server, bsnmp_type_t *typecode, uint32_t *len)
{
    int result;

    result = bsnmp_typecode_from_ber(server, typecode);
    if (result)
    {
        return result;
    }
    return bsnmp_length_from_ber(server, len);
}

int bsnmp_request_from_ber(bsnmp_server_t *server, bsnmp_opcode_t *opcode, uint32_t *len)
{
    int result;

    result = bsnmp_typelen_from_ber(server, (bsnmp_type_t*)opcode, len);
    if (result)
    {
        return result;
    }
    if ((*opcode < SNMP_GET) || (*opcode > SNMP_REPORT))
    {
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    return 0;
}

int bsnmp_struct_from_ber(bsnmp_server_t *server, uint32_t *len)
{
    int result;
    bsnmp_type_t typecode;

    result = bsnmp_typelen_from_ber(server, &typecode, len);
    if (result)
    {
        return result;
    }
    if (typecode != asnSTRUCT)
    {
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    return 0;
}

int bsnmp_null_from_ber(bsnmp_server_t *server, uint32_t *len)
{
    int result;
    bsnmp_type_t typecode;

    result = bsnmp_typelen_from_ber(server, &typecode, len);
    if (result)
    {
        return result;
    }
    if (typecode != asnNULL)
    {
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    return 0;
}

int bsnmp_int32_from_ber(bsnmp_server_t *server, int32_t *val)
{
    int result;
    int ival;
    uint8_t vallen;
    uint8_t b;

    *val = 0;

    // TYPE
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    switch ((bsnmp_type_t)b)
    {
    case SNMP_INTEGER:
    case SNMP_COUNTER:
        break;
    default:
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    // LENGTH
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    vallen = b;
    if (vallen > 4)
    {
        *val = 0;
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    if (vallen == 0)
    {
        *val = 0;
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    // VALUE
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    if (b & 0x80)
    {
        ival = -1;
    }
    else
    {
        ival = 0;
    }
    ival <<= 8;
    ival |= (int32_t)(uint32_t)b;
    vallen--;

    while (vallen > 0)
    {
        result = bsnmp_next_byte(server, &b);
        if (result)
        {
            return result;
        }
        ival <<= 8;
        ival |= (int32_t)(uint32_t)b;
        vallen--;
    }
    *val = ival;
    return 0;
}

int bsnmp_uint32_from_ber(bsnmp_server_t *server, uint32_t *val)
{
    int result;
    uint32_t uval;
    uint8_t vallen;
    uint8_t b;

    *val = 0;

    // TYPE
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    switch ((bsnmp_type_t)b)
    {
    case SNMP_BOOLEAN:
    case SNMP_IPADDRESS:
    case SNMP_COUNTER:
    case SNMP_UNSIGNED:
    case SNMP_TIMETICKS:
    case SNMP_NSAPADDRESS:
    case SNMP_UNSIGNED32:
        break;
    default:
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    // LENGTH
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    vallen = b;
    if (vallen > 4)
    {
        *val = 0;
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    if (vallen == 0)
    {
        *val = 0;
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    // VALUE
    uval = 0;
    while (vallen > 0)
    {
        result = bsnmp_next_byte(server, &b);
        if (result)
        {
            return result;
        }
        uval <<= 8;
        uval |= (uint32_t)b;
        vallen--;
    }
    *val = uval;
    return 0;
}

int bsnmp_int64_from_ber(bsnmp_server_t *server, int64_t *val)
{
    int result;
    int64_t ival;
    uint8_t vallen;
    uint8_t b;

    *val = 0;

    // TYPE
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    switch ((bsnmp_type_t)b)
    {
    case SNMP_INTEGER64:
        break;
    default:
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    // LENGTH
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    vallen = b;
    if (vallen > 8)
    {
        *val = 0;
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    if (vallen == 0)
    {
        *val = 0;
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    // VALUE
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    if (b & 0x80)
    {
        ival = -1;
    }
    else
    {
        ival = 0;
    }
    ival <<= 8;
    ival |= (int32_t)(uint32_t)b;
    vallen--;

    while (vallen > 0)
    {
        result = bsnmp_next_byte(server, &b);
        if (result)
        {
            return result;
        }
        ival <<= 8;
        ival |= (int64_t)(uint32_t)b;
        vallen--;
    }
    *val = ival;
    return 0;
}

int bsnmp_uint64_from_ber(bsnmp_server_t *server, uint64_t *val)
{
    int result;
    uint64_t uval;
    uint8_t vallen;
    uint8_t b;

    *val = 0;

    // TYPE
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    switch ((bsnmp_type_t)b)
    {
    case SNMP_UNSIGNED64:
    case SNMP_COUNTER64:
        break;
    default:
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    // LENGTH
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    vallen = b;
    if (vallen > 8)
    {
        *val = 0;
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    if (vallen == 0)
    {
        *val = 0;
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    // VALUE
    uval = 0;
    while (vallen > 0)
    {
        result = bsnmp_next_byte(server, &b);
        if (result)
        {
            return result;
        }
        uval <<= 8;
        uval |= (uint64_t)b;
        vallen--;
    }
    *val = uval;
    return 0;
}

int bsnmp_oid_from_ber(bsnmp_server_t *server, bsnmp_oid_t *oid)
{
    int result;
    int i;
    uint16_t oval;
    uint32_t vallen;
    uint8_t b;

    oid->len = 0;

    // TYPE
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    if ((bsnmp_type_t)b != SNMP_OBJECT_ID)
    {
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    // LENGTH
    result = bsnmp_length_from_ber(server, &vallen);
    if (result)
    {
        return result;
    }
    if (vallen > SNMP_MAX_OID)
    {
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    if (vallen == 0)
    {
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    // VALUE
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    // first byte is specially encoded
    //
    oval = (boid_t)b;
    oid->oid[0] = oval / 40;
    oid->oid[1] = oval - (oid->oid[0] * 40);
    vallen--;

    // next bytes are oid types ber encoded
    //
    i = 2;
    while (vallen > 0 && i < SNMP_MAX_OID)
    {
        result = bsnmp_next_byte(server, &b);
        if (result)
        {
            return result;
        }
        oval = (boid_t)(b & 0x7F);

        while ((b & 0x80) && (vallen > 0))
        {
            result = bsnmp_next_byte(server, &b);
            if (result)
            {
                return result;
            }
            oval <<= 7;
            oval |= (boid_t)(b & 0x7F);
        }
        oid->oid[i++] = oval;
        vallen--;
    }
    if (vallen != 0)
    {
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    oid->len = i;
    return 0;
}

int bsnmp_bytes_from_ber(bsnmp_server_t *server, uint8_t *val, uint32_t maxlen, uint32_t *lenout)
{
    int result;
    int i;
    uint32_t vallen;
    uint8_t b;

    // TYPE
    result = bsnmp_next_byte(server, &b);
    if (result)
    {
        return result;
    }
    if ((bsnmp_type_t)b != asnOCTET_STRING)
    {
        server->last_error = SNMP_ErrWrongEncoding;
        return -1;
    }
    // LENGTH
    result = bsnmp_length_from_ber(server, &vallen);
    if (result)
    {
        return result;
    }
    if (vallen > maxlen)
    {
        server->last_error = SNMP_ErrTooBig;
        return -1;
    }
    if (vallen == 0)
    {
        *val = 0;
        *lenout = 0;
        return 0;
    }
    // VALUE
    for (i = 0; vallen > 0; vallen--)
    {
        result = bsnmp_next_byte(server, &b);
        if (result)
        {
            return result;
        }
        val[i++] = b;
    }
    // convenience 0 terminate
    if (i < maxlen)
    {
        val[i] = 0;
    }
    *lenout = i;
    return 0;
}

int bsnmp_string_from_ber(bsnmp_server_t *server, char *val, uint32_t maxlen, uint32_t *lenout)
{
    return bsnmp_bytes_from_ber(server, (uint8_t *)val, maxlen, lenout);
}

int bsnmp_ber_from_length(bsnmp_server_t *server, uint32_t lenin, uint32_t *lenout)
{
    int result;
    int outlen = 0;

    if (lenin < 128)
    {
        result = bsnmp_out_byte(server, (const uint8_t)lenin);
        if (result)
        {
            return result;
        }
        outlen++;
    }
    else
    {
        uint32_t lenbytesneeded;
        uint32_t lenbytes;

        for (lenbytesneeded = 0, lenbytes = lenin; lenbytes; lenbytesneeded++)
        {
            lenbytes >>= 8;
        }
        result = bsnmp_out_byte(server, (const uint8_t)(0x80 | lenbytesneeded));
        if (result)
        {
            return result;
        }
        outlen++;

        while (lenbytesneeded > 0)
        {
            result = bsnmp_out_byte(server, (const uint8_t)(lenin >> (8 * (lenbytesneeded - 1))));
            if (result)
            {
                return result;
            }
            lenbytesneeded--;
            outlen++;
        }
    }
    *lenout = outlen;
    return 0;
}

int bsnmp_ber_reserve_length(bsnmp_server_t *server, uint32_t len)
{
    int result;

    while (len-- > 0)
    {
        result = bsnmp_out_byte(server, 0);
        if (result)
        {
            return result;
        }
    }
    return 0;
}

int bsnmp_ber_from_typecode(bsnmp_server_t *server, const bsnmp_type_t typecode)
{
    return bsnmp_out_byte(server, (const uint8_t)typecode);
}

int bsnmp_ber_from_typelen(bsnmp_server_t *server, const bsnmp_type_t typecode, const uint32_t lenin, uint32_t *lenout)
{
    int result;
    int outlen;

    result = bsnmp_out_byte(server, (const uint8_t)typecode);
    if (result)
    {
        return result;
    }
    result = bsnmp_ber_from_length(server, lenin, &outlen);
    if (result)
    {
        return result;
    }
    *lenout = outlen + 1;
    return 0;
}

int bsnmp_ber_from_int32(bsnmp_server_t *server, const bsnmp_type_t type, const int32_t val)
{
    uint32_t bytesneeded;
    uint32_t bytes;
    int result;

    if (val == 0)
    {
        bytesneeded = 1;
    }
    else if (val < 0)
    {
        for (bytes = (uint32_t)val, bytesneeded = 4; bytes; bytesneeded--)
        {
            if ((bytes & 0xFF800000) != 0xFF800000)
            {
                break;
            }
        }
    }
    else
    {
        for (bytes = (uint32_t)val, bytesneeded = 4; bytes; bytesneeded--)
        {
            if ((bytes & 0xFF000000) != 0)
            {
                break;
            }
            bytes <<= 8;
        }
    }
    result = bsnmp_ber_from_typelen(server, type, bytesneeded, &bytes);
    if (result)
    {
        return result;
    }
    while (bytesneeded > 0)
    {
        result = bsnmp_out_byte(server, (const uint8_t)(val >> (8 * (bytesneeded - 1))));
        if (result)
        {
            return result;
        }
        bytesneeded--;
    }
    return result;
}

int bsnmp_ber_from_uint32(bsnmp_server_t *server, const bsnmp_type_t type, const uint32_t val)
{
    uint32_t bytesneeded;
    uint32_t bytes;
    int result;

    for (bytes = (uint32_t)val, bytesneeded = 4; bytes; bytesneeded--)
    {
        if ((bytes & 0xFF000000) != 0)
        {
            break;
        }
        bytes <<= 8;
    }
    result = bsnmp_ber_from_typelen(server, type, bytesneeded, &bytes);
    if (result)
    {
        return result;
    }
    while (bytesneeded > 0)
    {
        result = bsnmp_out_byte(server, (const uint8_t)(val >> (8 * (bytesneeded - 1))));
        if (result)
        {
            return result;
        }
        bytesneeded--;
    }
    return result;
}

int bsnmp_ber_from_int64(bsnmp_server_t *server, const bsnmp_type_t type, const int64_t val)
{
}

int bsnmp_ber_from_uint64(bsnmp_server_t *server, const bsnmp_type_t type, const uint64_t val)
{
}

static int bsnmp_ber_from_single_oid(bsnmp_server_t *server, boid_t oidb)
{
    int result;
    uint8_t a, b, c, d, e;

    a = (unsigned char)(oidb & 0x7F);
    oidb >>= 7;
    b = (unsigned char)(oidb & 0x7F);
    oidb >>= 7;
    c = (unsigned char)(oidb & 0x7F);
    oidb >>= 7;
    d = (unsigned char)(oidb & 0x7F);
    oidb >>= 7;
    e = (unsigned char)(oidb & 0x7F);

    if (e)
    {
        result = bsnmp_out_byte(server, (e | 0x80));
        if (result)
        {
            return result;
        }
    }
    if (d || e)
    {
        result = bsnmp_out_byte(server, (d | 0x80));
        if (result)
        {
            return result;
        }
    }
    if (c || d || e)
    {
        result = bsnmp_out_byte(server, (c | 0x80));
        if (result)
        {
            return result;
        }
    }
    if (b || c || d || e)
    {
        result = bsnmp_out_byte(server, (b | 0x80));
        if (result)
        {
            return result;
        }
    }
    result = bsnmp_out_byte(server, a);
    return result;
}

int bsnmp_ber_from_oid(bsnmp_server_t *server, const bsnmp_oid_t *oid)
{
    ioring_t lenpos;
    int reserve;
    int oiddex;
    int result;

    if (! oid)
    {
        server->last_error = SNMP_ErrBadValue;
        return 1;
    }
    if (oid->len >= SNMP_MAX_OID)
    {
        server->last_error = SNMP_ErrTooBig;
        return 1;
    }
    result = bsnmp_ber_from_typecode(server, SNMP_OBJECT_ID);
    if (result)
    {
        return result;
    }
    if (SNMP_MAX_OID > 127)
    {
        reserve = 5;
    }
    else
    {
        reserve = 1;
    }
    // save position for back annotating
    bsnmp_stream_save_out_state(server, &lenpos);

    // put in expected length bytes
    result = bsnmp_ber_reserve_length(server, reserve);
    if (result)
    {
        return result;
    }
    if (oid->len > 1)
    {
        result = bsnmp_ber_from_single_oid(server, oid->oid[0] * 40 + oid->oid[1]);
        if (result)
        {
            return result;
        }
        oiddex = 2;
    }
    else
    {
        oiddex = 0;
    }
    while (oiddex < oid->len)
    {
        result = bsnmp_ber_from_single_oid(server, oid->oid[oiddex]);
        if (result)
        {
            return result;
        }
        oiddex++;
    }
    // back annotate length byte(s)
    result = bsnmp_stream_annotate_length(server, &lenpos, reserve);
    return result;
}

int bsnmp_ber_from_bytes(bsnmp_server_t *server, const bsnmp_type_t type, const uint8_t *val, uint32_t len)
{
    uint32_t outlen;
    uint32_t outdex;
    int result;

    result = bsnmp_ber_from_typelen(server, type, len, &outlen);
    if (result)
    {
        return result;
    }
    for (outdex = 0; outdex <  len; outdex++)
    {
        result = bsnmp_out_byte(server, val[outdex]);
        if (result)
        {
            return result;
        }
    }
    return result;
}

int bsnmp_ber_from_string(bsnmp_server_t *server, const char *val, uint32_t len)
{
    return bsnmp_ber_from_bytes(server, SNMP_OCTET_STRING, (uint8_t*)val, len);
}

