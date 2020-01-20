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

#define PTPPROT_DBG_LEVEL 3

typedef enum { eFullDesc, eValueOnly } ePropReportLevel;

static void ptp_pack_u8(uint8_t **pb, size_t *proom, uint8_t val)
{
    size_t room = *proom;
    uint8_t *pv = *pb;

    if (room >= 1)
	{
        *pv++ = (uint8_t)val;
        room -= 1;
        *proom = room;
        *pb = pv;
    }
	else
	{
        *proom = 0;
    }
}

static void ptp_pack_u16(uint8_t **pb, size_t *proom, uint16_t val)
{
    size_t room = *proom;
    uint8_t *pv = *pb;

    if (room >= 2)
	{
        *pv++ = (uint8_t)val;
        *pv++ = (uint8_t)(val >> 8);
        room -= 2;
        *proom = room;
        *pb = pv;
    }
	else
	{
        *proom = 0;
    }
}

static void ptp_pack_u32(uint8_t **pb, size_t *proom, uint32_t val)
{
    size_t room = *proom;
    uint8_t *pv = *pb;

    if (room >= 4)
	{
        *pv++ = (uint8_t)val;
        *pv++ = (uint8_t)(val >> 8);
        *pv++ = (uint8_t)(val >> 16);
        *pv++ = (uint8_t)(val >> 24);
        room -= 4;
        *proom = room;
        *pb = pv;
    }
	else
	{
        *proom = 0;
    }
}

static void ptp_pack_u64(uint8_t **pb, size_t *proom, uint64_t val)
{
    size_t room = *proom;
    uint8_t *pv = *pb;

    if (room >= 8)
	{
        *pv++ = (uint8_t)val;
        *pv++ = (uint8_t)(val >> 8);
        *pv++ = (uint8_t)(val >> 16);
        *pv++ = (uint8_t)(val >> 24);
        *pv++ = (uint8_t)(val >> 32);
        *pv++ = (uint8_t)(val >> 40);
        *pv++ = (uint8_t)(val >> 48);
        *pv++ = (uint8_t)(val >> 56);
        room -= 8;
        *proom = room;
        *pb = pv;
    }
	else
	{
        *proom = 0;
    }
}

static void ptp_pack_str(uint8_t **pb, size_t *proom, const char *val)
{
    size_t room;
    size_t slen;
    uint8_t *pv = *pb;

    ///@TODO - convert from utf8 to UCS!6
    slen = strlen(val);
    if (slen > 254)
	{
        butil_log(1, "string overflow\n");
        slen = 254;
    }

    room = *proom;
    if (room >= 1)
	{
        *pv++ = (uint8_t)(slen + 1);
        room -= 1;
    }

    while ((room >= 4) && *val)
	{
        *pv++ = (uint8_t)*val++;
        *pv++ = '\0';
        room -= 2;
    }

    if (room >= 2)
	{
        *pv++ = '\0';
        *pv++ = '\0';
        room -= 2;
    }
    *pb = pv;
    *proom = room;
}

static uint16_t ptp_unpack_u16(uint8_t **pb, size_t *proom)
{
    size_t room = *proom;
    uint8_t *pv = *pb;
    uint16_t val = 0;

    if (room >= 2)
	{
        val = *pv++;
        val |= (uint16_t)*pv++ << 8;
        room -= 2;
        *proom = room;
        *pb = pv;
    }
	else
	{
        *proom = 0;
    }
    return val;
}

static uint32_t ptp_unpack_u32(uint8_t **pb, size_t *proom)
{
    size_t room = *proom;
    uint8_t *pv = *pb;
    uint16_t val = 0;

    if (room >= 4)
	{
        val = *pv++;
        val |= (uint32_t)*pv++ << 8;
        val |= (uint32_t)*pv++ << 16;
        val |= (uint32_t)*pv++ << 24;
        room -= 4;
        *proom = room;
        *pb = pv;
    }
	else
	{
        *proom = 0;
    }
    return val;
}

static int ptp_unpack_str(uint8_t **pb, size_t *proom, char *str, size_t nstr)
{
    size_t room = *proom;
    uint8_t *pv = *pb;
    size_t len;

    *str = '\0';
    if (room >= 1)
	{
        len = *pv++;

        if ((room * 2) >= len)
		{
            while (len > 0)
			{
                if (nstr > 0)
				{
                    *str++ = *pv;
                    nstr--;
                }
                pv+= 2;
                room -= 2;
                len--;
            }
        }
        *str = '\0';
        *proom = room;
        *pb = pv;
    }
	else
	{
        *proom = 0;
    }
    return 0;
}

int ptp_response(ptp_connection_t *ptpx, ioring_t *io, uint16_t opcode, size_t nparms)
{
    uint32_t length, type;
    int i;

    if (opcode != PTP_RESP_OK)
	{
        butil_log(2, "NON-ok resp %04X\n", opcode);
    }

    // ip hdr
    length = 8 + 2 + 4 + nparms * 4;
    type   = PTP_CMD_RESP;
    ptp_write(ptpx->client_sock, io, (uint8_t*)&length, 4);
    ptp_write(ptpx->client_sock, io, (uint8_t*)&type, 4);

    // ptp hdr
    ptp_write(ptpx->client_sock, io, (uint8_t*)&opcode, 2);
    ptp_write(ptpx->client_sock, io, (uint8_t*)&ptpx->trans_id, 4);
    for (i = 0; i < nparms; i++)
	{
        ptp_write(ptpx->client_sock, io, (uint8_t*)&ptpx->cmdparms[i], 4);
    }
    return ptp_flush_out(ptpx->client_sock, io, 5, 0);
}

int ptp_send_data_ex(ptp_connection_t *ptpx, ioring_t *io, uint8_t *data, size_t datalen, size_t totallen, int isfirst, int islast)
{
    uint32_t length, type, chunk;
    int rc;

    // send start data packet if first send (total != 0)
    //
    if (isfirst)
    {
        // ip hdr
        length = 8 + 4 + 4 + 4;
        type   = PTP_START_DATA;
        ptp_write(ptpx->client_sock, io, (uint8_t*)&length, 4);
        ptp_write(ptpx->client_sock, io, (uint8_t*)&type, 4);
        ptp_write(ptpx->client_sock, io, (uint8_t*)&ptpx->trans_id, 4);

        // send data length
        length = (uint32_t)totallen;
        ptp_write(ptpx->client_sock, io, (uint8_t*)&length, 4);
        length = 0; // upper length ?
        ptp_write(ptpx->client_sock, io, (uint8_t*)&length, 4);

        rc = ptp_flush_out(ptpx->client_sock, io, 5, 0);
        if (rc < 0)
		{
            return rc;
        }
    }

    if ((datalen > 0) || islast)
	{
        // if data can all fit in one write, do it in an end packet only
        //     if its the last packett, else use data packets
        //
        while ((datalen > (io->size - io->count - 12)) || (datalen > 0 && ! islast))
		{
            // ip hdr
            chunk = io->size - io->count - 12;
            if (chunk > datalen)
			{
                // in case this is just a middle packet
                chunk = datalen;
            }
            length = 8 + 4 + chunk;
            type   = PTP_DATA;
            ptp_write(ptpx->client_sock, io, (uint8_t*)&length, 4);
            ptp_write(ptpx->client_sock, io, (uint8_t*)&type, 4);
            ptp_write(ptpx->client_sock, io, (uint8_t*)&ptpx->trans_id, 4);
            ptp_write(ptpx->client_sock, io, data, chunk);
            rc = ptp_flush_out(ptpx->client_sock, io, 5, 0);
            if (rc < 0)
			{
                return rc;
            }
            datalen -= chunk;
            data += chunk;
        }

        if (islast)
		{
            // ip hdr
            length = 8 + 4 + datalen;
            type   = PTP_END_DATA;
            ptp_write(ptpx->client_sock, io, (uint8_t*)&length, 4);
            ptp_write(ptpx->client_sock, io, (uint8_t*)&type, 4);
            ptp_write(ptpx->client_sock, io, (uint8_t*)&ptpx->trans_id, 4);
            ptp_write(ptpx->client_sock, io, data, datalen);
            rc = ptp_flush_out(ptpx->client_sock, io, 5, 0);
            if (rc < 0)
			{
                return rc;
            }
        }
    }
    return 0;
}

int ptp_send_data(ptp_connection_t *ptpx, ioring_t *io, uint8_t *data, size_t datalen)
{
    return ptp_send_data_ex(ptpx, io, data, datalen, datalen, 1, 1);
}

#define PTP_SUPPORT_LEGACY_OBJPROPS 1
#define PTP_SUPPORT_OBJPROPLIST 1
#define PTP_SUPPORT_OBJREFS 1

static uint8_t *ptp_create_devinfo(ptp_connection_t *ptpx, uint8_t *buf, size_t nbuf, size_t *reslen)
{
    uint8_t *pb;
    size_t room, numcodes, numformats;

    if (! buf || nbuf < 64)
	{
        return NULL;
    }
    pb = buf;
    room = nbuf;

    ptp_pack_u16(&pb, &room, PTPDEV_PTP_VERSION);
    ptp_pack_u32(&pb, &room, PTPDEV_EXT_ID);
    ptp_pack_u16(&pb, &room, PTPDEV_EXT_VERSION);
    ptp_pack_str(&pb, &room, PTPDEV_EXT_DESC);

    ptp_pack_u16(&pb, &room, PTPDEV_FUNCMODE);

    // supported codes
    //
    numcodes = 16;
#if PTP_SUPPORT_LEGACY_OBJPROPS
    numcodes += 4;
#endif
#if PTP_SUPPORT_OBJPROPLIST
    numcodes += 2;
#endif
#if PTP_SUPPORT_OBJREFS
    numcodes += 2;
#endif

    ptp_pack_u32(&pb, &room, numcodes);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetDeviceInfo);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_OpenSession);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_CloseSession);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetStorageIDs);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetStorageInfo);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetNumObjects);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetObjectHandles);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetObjectInfo);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetObject);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_DeleteObject);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_SendObjectInfo);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_SendObject);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetDevicePropDesc);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetDevicePropValue);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_SetDevicePropValue);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_ResetDevicePropValue);

#if PTP_SUPPORT_LEGACY_OBJPROPS
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetObjectPropsSupported);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetObjectPropDesc);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetObjectPropValue);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_SetObjectPropValue);
#endif
#if PTP_SUPPORT_OBJPROPLIST
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetObjectPropList);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_SetObjectPropList);
#endif
#if PTP_SUPPORT_OBJREFS
    ptp_pack_u16(&pb, &room, PTP_OPCODE_GetObjectReferences);
    ptp_pack_u16(&pb, &room, PTP_OPCODE_SetObjectReferences);
#endif
    // supported events
    //
    ptp_pack_u32(&pb, &room, 5);
    ptp_pack_u16(&pb, &room, PTP_EVENT_ObjectAdded);
    ptp_pack_u16(&pb, &room, PTP_EVENT_ObjectRemoved);
    ptp_pack_u16(&pb, &room, PTP_EVENT_StoreAdded);
    ptp_pack_u16(&pb, &room, PTP_EVENT_StoreRemoved);
    ptp_pack_u16(&pb, &room, PTP_EVENT_DeviceReset);

    // supported properties
    //
    ptp_pack_u32(&pb, &room, 3);
    ptp_pack_u16(&pb, &room, PTP_PROP_Synchronization_Partner);
    ptp_pack_u16(&pb, &room, PTP_PROP_Device_Friendly_Name);
    ptp_pack_u16(&pb, &room, PTP_PROP_Perceived_Device_Type);

    // capture formats
    //
    ptp_pack_u32(&pb, &room, 0);

    // image formats (playback formats)
    //
    numformats = 7;
#if PTP_SUPPORT_WINDOWS_FORMATS
    numformats += 3;
#endif
    ptp_pack_u32(&pb, &room, numformats);
    ptp_pack_u16(&pb, &room, PTP_OBJFMT_UNDEF);
    ptp_pack_u16(&pb, &room, PTP_OBJFMT_ASSOC);
    ptp_pack_u16(&pb, &room, PTP_OBJFMT_MP3);
    ptp_pack_u16(&pb, &room, PTP_OBJFMT_FW);
    ptp_pack_u16(&pb, &room, PTP_OBJFMT_M3UPLAYLIST);
    ptp_pack_u16(&pb, &room, PTP_OBJFMT_ABSTRACTAUDIOVIDEOPLAYLIST);
    ptp_pack_u16(&pb, &room, PTP_OBJFMT_XMLDOCUMENT);
#if PTP_SUPPORT_WINDOWS_FORMATS
    ptp_pack_u16(&pb, &room, PTP_OBJFMT_WAV);
    ptp_pack_u16(&pb, &room, PTP_OBJFMT_WMA);
    ptp_pack_u16(&pb, &room, PTP_OBJFMT_ABSTRACTAUDIOALBUM);
#endif
    ptp_pack_str(&pb, &room, PTPDEV_MANUFACTURE);
    ptp_pack_str(&pb, &room, PTPDEV_MODEL);
    ptp_pack_str(&pb, &room, PTPDEV_DEVICE_VERSION);
    ptp_pack_str(&pb, &room, PTPDEV_DEVICE_SERIALNUM);

    if (room == 0)
	{
        butil_log(1, "devinfo overflow\n");
        return NULL;
    }

    if (reslen)
	{
        *reslen = pb - buf;
    }
    return buf;
}

int ptp_return_deviceinfo(ptp_connection_t *ptpx, ioring_t *io)
{
    uint8_t *devinfo, *infobuf;
    size_t devlen;
    int rc;

    infobuf = (uint8_t*)malloc(400); ///@TODO - size right
    devinfo = ptp_create_devinfo(ptpx, infobuf, 400, &devlen);

    // ptp response
    if (! devinfo)
	{
        free(infobuf);
        return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
    }

    rc = ptp_send_data(ptpx, io, devinfo, devlen);
    free(infobuf);
    if (rc < 0)
	{
        return rc;
    }

    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

static uint8_t *ptp_create_devpropdesc(
                                    ptp_connection_t *ptpx,
                                    ptp_var_t *prop,
                                    ePropReportLevel level,
                                    uint8_t *buf,
                                    size_t nbuf,
                                    size_t *reslen
                                   )
{
    uint8_t *pb;
    size_t room;

    if (! buf || nbuf < 64)
	{
        return NULL;
    }
    pb = buf;
    room = nbuf;

    if (level == eFullDesc)
	{
        ptp_pack_u16(&pb, &room, prop->code);
        ptp_pack_u16(&pb, &room, prop->type);
        ptp_pack_u8 (&pb, &room, prop->access);
    }

    switch (prop->type)
	{
    case PTP_TYPE_STR:
        ptp_pack_str(&pb, &room, prop->factory_value.sval);
        ptp_pack_str(&pb, &room, prop->current_value.sval);
        break;
    case PTP_TYPE_UINT8:
        ptp_pack_u8(&pb, &room, prop->factory_value.uval);
        ptp_pack_u8(&pb, &room, prop->current_value.uval);
        break;
    case PTP_TYPE_UINT16:
        ptp_pack_u16(&pb, &room, prop->factory_value.uval);
        ptp_pack_u16(&pb, &room, prop->current_value.uval);
        break;
    case PTP_TYPE_UINT32:
        ptp_pack_u32(&pb, &room, prop->factory_value.uval);
        ptp_pack_u32(&pb, &room, prop->current_value.uval);
        break;
    case PTP_TYPE_UINT64:
        ptp_pack_u64(&pb, &room, prop->factory_value.u64val);
        ptp_pack_u64(&pb, &room, prop->current_value.u64val);
        break;
    case PTP_TYPE_UINT128:
        ptp_pack_u64(&pb, &room, prop->factory_value.u64val);
        ptp_pack_u64(&pb, &room, 0);
        ptp_pack_u64(&pb, &room, prop->current_value.u64val);
        ptp_pack_u64(&pb, &room, 0);
        break;
    default:
        butil_log(1, "Unhandled type\n");
        break;
    }

    if (level == eFullDesc)
	{
        ptp_pack_u8 (&pb, &room, 0); // form flag
    }

    if (room == 0)
	{
        butil_log(1, "propdesc overflow\n");
        return NULL;
    }

    if (reslen)
	{
        *reslen = pb - buf;
    }

    return buf;
}

int ptp_return_devproperty(ptp_connection_t *ptpx, ioring_t *io, int fulldesc)
{
    uint8_t descbuf[128], *desc;
    size_t desclen;
    int rc;
    ptp_var_t *prop;

    prop = ptp_getproperty(ptpx->cmdparms[0]);
    if (! prop)
	{
        return ptp_response(ptpx, io, PTP_RESP_DevicePropNotSupported, 0);
    }

    desc = ptp_create_devpropdesc(ptpx, prop, (fulldesc ? eFullDesc : eValueOnly),
            descbuf, sizeof(descbuf), &desclen);

    if (! desc)
	{
        return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
    }

    rc = ptp_send_data(ptpx, io, desc, desclen);
    if (rc < 0)
	{
        return rc;
    }

    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

static uint8_t *ptp_create_objpropdesc(
                                    ptp_connection_t *ptpx,
                                    ptp_var_t *prop,
                                    ePropReportLevel level,
                                    uint8_t *buf,
                                    size_t nbuf,
                                    size_t *reslen
                                   )
{
    uint8_t *pb;
    size_t room;

    if (! buf || nbuf < 64)
	{
        return NULL;
    }

    pb = buf;
    room = nbuf;

    if (level == eFullDesc)
	{
        ptp_pack_u16(&pb, &room, prop->code);
        ptp_pack_u16(&pb, &room, prop->type);
        ptp_pack_u8 (&pb, &room, prop->access);
    }

    switch (prop->type)
	{
    case PTP_TYPE_STR:
        if (level == eFullDesc)
		{
            ptp_pack_str(&pb, &room, prop->factory_value.sval);
            ptp_pack_u32(&pb, &room, prop->group);
        }
		else
		{
            ptp_pack_str(&pb, &room, prop->current_value.sval);
        }
        break;
    case PTP_TYPE_UINT8:
        if (level == eFullDesc)
		{
            ptp_pack_u8(&pb, &room, prop->factory_value.uval);
            ptp_pack_u32(&pb, &room, prop->group);
        }
		else
		{
            ptp_pack_u8(&pb, &room, prop->current_value.uval);
        }
        break;
    case PTP_TYPE_UINT16:
        if (level == eFullDesc)
		{
            ptp_pack_u16(&pb, &room, prop->factory_value.uval);
            ptp_pack_u32(&pb, &room, prop->group);
        }
		else
		{
            ptp_pack_u16(&pb, &room, prop->current_value.uval);
        }
        break;
    case PTP_TYPE_UINT32:
        if (level == eFullDesc)
		{
            ptp_pack_u32(&pb, &room, prop->factory_value.uval);
            ptp_pack_u32(&pb, &room, prop->group);
        }
		else
		{
            ptp_pack_u32(&pb, &room, prop->current_value.uval);
        }
        break;
    case PTP_TYPE_UINT64:
        if (level == eFullDesc)
		{
            ptp_pack_u64(&pb, &room, prop->factory_value.u64val);
            ptp_pack_u32(&pb, &room, prop->group);
        }
		else
		{
            ptp_pack_u64(&pb, &room, prop->current_value.u64val);
        }
        break;
    case PTP_TYPE_UINT128:
        if (level == eFullDesc)
		{
            ptp_pack_u64(&pb, &room, prop->factory_value.u64val);
            ptp_pack_u64(&pb, &room, 0);
            ptp_pack_u32(&pb, &room, prop->group);
        }
		else
		{
            ptp_pack_u64(&pb, &room, prop->current_value.u64val);
            ptp_pack_u64(&pb, &room, 0);
        }
        break;
    default:
        butil_log(1, "Unhandled type\n");
        break;
    }

    if (level == eFullDesc)
	{
        ptp_pack_u8 (&pb, &room, 0); // form flag
    }

	if (room == 0)
	{
        butil_log(1, "propdesc overflow\n");
        return NULL;
    }

	if (reslen)
	{
        *reslen = pb - buf;
    }
    return buf;
}

int ptp_return_objproperty(ptp_connection_t *ptpx, ioring_t *io, int fulldesc)
{
    uint8_t descbuf[128], *desc;
    size_t desclen;
    uint16_t resp, objfmt;
    int rc;
    ptp_var_t *prop;
    uint32_t handle;
    uint32_t propcode;

    if (fulldesc)
	{
        // GetObjPropDesc
        handle = 0;
        propcode = ptpx->cmdparms[0];
        objfmt = ptpx->cmdparms[1];
    }
	else
	{
        // GetObjPropValue
        handle = ptpx->cmdparms[0];
        propcode = ptpx->cmdparms[1];
        objfmt = PTP_OBJFMT_UNDEF;
    }

    prop = ptp_getproperty(propcode);
    if (! prop)
	{
        return ptp_response(ptpx, io, PTP_RESP_Invalid_ObjectPropCode, 0);
    }

    resp = ptp_setobjectproperty_for(objfmt, prop, handle);
    if (resp != PTP_RESP_OK)
	{
        return ptp_response(ptpx, io, resp, 0);
    }

    desc = ptp_create_objpropdesc(ptpx, prop, (fulldesc ? eFullDesc : eValueOnly),
            descbuf, sizeof(descbuf), &desclen);

    if (! desc)
	{
        return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
    }

    rc = ptp_send_data(ptpx, io, desc, desclen);
    if (rc < 0)
	{
        return rc;
    }

    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

static uint8_t *ptp_create_proplistdesc(
                                    ptp_connection_t *ptpx,
                                    uint32_t handle,
                                    ptp_var_t *prop,
                                    uint8_t *buf,
                                    size_t nbuf,
                                    size_t *reslen
                                   )
{
    uint8_t *pb;
    size_t room;

    if (! buf || nbuf < 64)
	{
        return NULL;
    }

    pb = buf;
    room = nbuf;

    ptp_pack_u32(&pb, &room, handle);
    ptp_pack_u16(&pb, &room, prop->code);
    ptp_pack_u16(&pb, &room, prop->type);

    switch (prop->type)
	{
    case PTP_TYPE_STR:
        ptp_pack_str(&pb, &room, prop->current_value.sval);
        break;
    case PTP_TYPE_UINT8:
        ptp_pack_u8(&pb, &room, prop->current_value.uval);
        break;
    case PTP_TYPE_UINT16:
        ptp_pack_u16(&pb, &room, prop->current_value.uval);
        break;
    case PTP_TYPE_UINT32:
        ptp_pack_u32(&pb, &room, prop->current_value.uval);
        break;
    case PTP_TYPE_UINT64:
        ptp_pack_u64(&pb, &room, prop->current_value.u64val);
        break;
    case PTP_TYPE_UINT128:
        ptp_pack_u64(&pb, &room, prop->current_value.u64val);
        ptp_pack_u64(&pb, &room, 0);
        break;
    default:
        butil_log(1, "Unhandled type\n");
        break;
    }

    if (room == 0)
	{
        butil_log(1, "proplistdesc overflow\n");
        return NULL;
    }

    if (reslen)
	{
        *reslen = pb - buf;
    }

    return buf;
}

int ptp_return_listobjproperty(ptp_connection_t *ptpx, ioring_t *io)
{
    uint8_t *descbuf, *desc;
    size_t descalloc, desclen, desctotlen, desccount;
    uint16_t resp, objfmt;
    mtp_object_t *pmo;
    ptp_var_t *prop;
    uint32_t handle, objhandle, firsthandle, lasthandle;
    uint16_t propcode, curpropcode;
    uint32_t group;
    uint32_t depth;
    int rc;

    handle   = ptpx->cmdparms[0];
    objfmt   = ptpx->cmdparms[1];
    propcode = ptpx->cmdparms[2];
    group    = ptpx->cmdparms[3];
    depth    = ptpx->cmdparms[4];

    if (propcode && (ptpx->cmdparms[2] != 0xFFFFFFFF))
	{
        prop = ptp_getproperty(propcode);
        if (! prop)
		{
            return ptp_response(ptpx, io, PTP_RESP_Invalid_ObjectPropCode, 0);
        }
    }

    // there are a billion ways object properties can be iterated
    // but only a few that windows uses
    //
    // if handle = ffffffff - iterate over all objects
    //           = 00000000 - iterate over all root level objects
    //
    // if objfmt = 00000000 - iterate over all object formats
    //           !=  -        for objects of this format only
    //
    // if propcode = 0xffffffff - iterate for all properties
    //             = 0x00000000 - interate by property group
    //
    // group code to use if propcode is 0
    //
    // level (0 = root only, ffffffff = all levels)
    //

    // ------ ignore spec by group for now
    if (propcode == 0)
	{
        return ptp_response(ptpx, io, PTP_RESP_Specification_By_Group_Unsupported, 0);
    }

    // ------ ignore spec by format for now
    if (objfmt != 0)
	{
        return ptp_response(ptpx, io, PTP_RESP_SpecificationByFormatUnsupported, 0);
    }

    // allocate a place to put all this list
    //
    descalloc = 1024; ///@TODO scope this?
    descbuf = (uint8_t*)malloc(descalloc);
    if (! descbuf)
	{
        return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
    }

    // put a place holder count in
    desclen = descalloc;
    desc = descbuf;
    ptp_pack_u32(&desc, &desclen, 0);
    desctotlen = 4;
    desccount  = 0;

    if (handle != 0 && handle != 0xFFFFFFFF)
	{
        firsthandle = handle;
        lasthandle  = handle;
    }
	else
	{
        firsthandle = 1;
        lasthandle  = MTPD_MAX_OBJECTS;
    }

    // iterate over each object in scope
    for (objhandle = firsthandle; objhandle <= lasthandle; objhandle++)
	{
        pmo = mtpd_from_handle(objhandle);

        // skip this object if its not at root level if iterating only that level
        if (pmo && (pmo->parent == 0xFFFFFFFF || handle != 0xFFFFFFFF))
		{
            // an object found for this handle, iterate over each property
            uint16_t firstprop, lastprop;

            if (propcode == 0xFFFF)
			{
                firstprop = PTP_PROP_Undefined + 1;
                lastprop  = PTP_PROP_LastProp;
            }
			else
			{
                firstprop = propcode;
                lastprop = propcode;
            }

            for (curpropcode = firstprop; curpropcode <= lastprop; curpropcode++)
			{
                // setup a property description for this property and object
                prop = ptp_getproperty(curpropcode);
                if (prop)
				{
                    resp = ptp_setobjectproperty_for(pmo->format, prop, objhandle);
                    // insist a real property if input, else ignore
                    if (
                            (propcode != 0 && propcode != 0xFFFF)
                        ||  (resp != PTP_RESP_Invalid_ObjectPropCode)
                    )
					{
                        if (resp != PTP_RESP_OK)
						{
                            butil_log(3, "RESP %04X for prop %04X\n", resp, curpropcode);
                            free(descbuf);
                            return ptp_response(ptpx, io, resp, 0);
                        }

                        // build a propery listing for this
                        desc = ptp_create_proplistdesc(
                                                    ptpx,
                                                    objhandle,
                                                    prop,
                                                    descbuf + desctotlen,
                                                    descalloc - desctotlen,
                                                    &desclen
                                                    );
                        if (! desc)
						{
                            free(descbuf);
                            return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
                        }

                        desctotlen += desclen;
                        desccount++;
                    }
                }
            }
        }
    }
    // back annotate count
    desclen = descalloc;
    desc = descbuf;
    ptp_pack_u32(&desc, &desclen, desccount);

    rc = ptp_send_data(ptpx, io, descbuf, desctotlen);
    free(descbuf);
    if (rc < 0)
	{
        return rc;
    }

    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

int ptp_return_objpropssupported(ptp_connection_t *ptpx, ioring_t *io)
{
    uint8_t propcodes[PTP_MAX_PROPERTIES * sizeof(uint16_t) + sizeof(uint32_t)];
    int rc;
    uint8_t *pb;
    uint16_t objfmt;
    uint16_t code;
    uint32_t propcount;
    size_t room, proplen;

    pb = propcodes;
    room = sizeof(propcodes);

    if (ptpx->ncmdparms > 0)
	{
        objfmt = ptpx->cmdparms[0];
    }
	else
	{
        objfmt = 0;
    }

    // place holder for count
    ptp_pack_u32(&pb, &room, 0);

    // count obj props, packing code in along the way
    for (
            propcount = 0, code = PTP_PROP_StorageID;
            propcount < PTP_MAX_PROPERTIES && code < PTP_PROP_LastProp;
            code++
    )
	{
        if (ptp_getproperty_for_format(code, objfmt) != NULL)
		{
            ptp_pack_u16(&pb, &room, code);
            propcount++;
        }
    }

    proplen = pb - propcodes + 4;

    // back annotate count
    pb = propcodes;
    room = sizeof(propcodes);
    ptp_pack_u32(&pb, &room, propcount);

    rc = ptp_send_data(ptpx, io, propcodes, proplen);
    if (rc < 0)
	{
        return rc;
    }

    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

int ptp_return_objecthandles(ptp_connection_t *ptpx, ioring_t *io)
{
    uint32_t storageId, objfmt, parent;
    size_t allhandles, numhandles, iterator;
    uint32_t *handles, handle;
    int rc;

    storageId = ptpx->cmdparms[0];
    if (ptpx->ncmdparms > 1)
	{
        objfmt = ptpx->cmdparms[1];
    }
	else
	{
        objfmt = 0;
    }

    if (ptpx->ncmdparms > 2)
	{
        parent = ptpx->cmdparms[2];
    }
	else
	{
        parent = 0;
    }

    allhandles = mtpd_file_count();
    handles = (uint32_t *)malloc((allhandles + 1) * sizeof(uint32_t));
    if (! handles)
	{
        return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
    }

    if (PTPPROT_DBG_LEVEL > 0)
	{
        butil_log(4, "handles of %08X\n", parent);
    }

    numhandles = 0;
    for (iterator = 0; iterator < allhandles; iterator++)
	{
        handle = mtpd_file_handle(iterator);
        if (handle)
		{
            mtp_object_t *pmo;
            int match = 1;

            pmo = mtpd_from_handle(handle);
            if (pmo)
			{
                if (objfmt && (pmo->format != objfmt))
				{
                    match = 0;
                }

                if (parent != 0 && (pmo->parent != parent))
				{
                    match = 0;
                }

                if (pmo->parent == 0)
				{
                    // shouldn't get here, but 0 is an invalid handle
                    match = 0;
                }

                if (match)
				{
                    if (PTPPROT_DBG_LEVEL > 0)
					{
                        mtp_object_t *pmo;

                        pmo = mtpd_from_handle(handle);
                        butil_log(4, "  %08X %s\n", handle, pmo ? pmo->name : NULL);
                    }
                    handles[++numhandles] = handle;
                }
            }
        }
    }

    handles[0] = numhandles;
    rc = ptp_send_data(ptpx, io, (uint8_t *)handles, (numhandles + 1) * sizeof(uint32_t));
    free(handles);
    if (rc < 0)
	{
        return rc;
    }

    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

int ptp_return_objrefs(ptp_connection_t *ptpx, ioring_t *io)
{
    int rc;
    uint32_t parent;
    uint32_t handle;
    size_t allhandles, numhandles, iterator;
    uint32_t *handles;

    parent = ptpx->cmdparms[0];

    allhandles = mtpd_file_count();
    handles = (uint32_t *)malloc((allhandles + 1) * sizeof(uint32_t));
    if (! handles)
	{
        return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
    }

    if (PTPPROT_DBG_LEVEL > 0)
	{
        butil_log(4, "refs (handles of %08X)\n", parent);
    }

    numhandles = 0;
    for (iterator = 0; iterator < allhandles; iterator++)
	{
        handle = mtpd_file_handle(iterator);
        if (handle)
		{
            mtp_object_t *pmo;
            int match = 1;

            pmo = mtpd_from_handle(handle);
            if (pmo)
			{
                if (parent != 0 && (pmo->parent != parent))
				{
                    match = 0;
                }

                if (pmo->parent == 0)
				{
                    // shouldn't get here, but 0 is an invalid handle
                    match = 0;
                }

                if (match)
				{
                    if (PTPPROT_DBG_LEVEL > 0)
					{
                        mtp_object_t *pmo;

                        pmo = mtpd_from_handle(handle);
                        butil_log(4, "  %08X %s\n", handle, pmo ? pmo->name : NULL);
                    }

                    handles[++numhandles] = handle;
                }
            }
        }
    }

    handles[0] = numhandles;
    rc = ptp_send_data(ptpx, io, (uint8_t *)handles, (numhandles + 1) * sizeof(uint32_t));
    free(handles);
    if (rc < 0)
	{
        return rc;
    }

    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

int ptp_return_storageids(ptp_connection_t *ptpx, ioring_t *io)
{
    int rc;

    // hack up default store unit 1, 1
    //
    uint32_t stores[2];

    stores[0] = 1;
    stores[1] = MTPD_ROOT_STORE;

    rc = ptp_send_data(ptpx, io, (uint8_t *)stores, sizeof(stores));
    if (rc < 0)
	{
        return rc;
    }

    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

static uint8_t *ptp_create_storageinfo(ptp_connection_t *ptpx, uint8_t *buf, size_t nbuf, size_t *reslen)
{
    uint8_t *pb;
    size_t room;
    uint64_t space, freespace, usedspace;
    uint32_t handle, freeobjs;
    mtp_object_t *pmo;

    if (! buf || nbuf < 28)
	{
        return NULL;
    }

    pb = buf;
    room = nbuf;

    //free_obj_count = MTPD_MAX_OBJECTS - mtpd_file_count();
    space = 4;
    space *= 1024;
    space *= 1024;
    space *= 1024; // 4 gb

    usedspace = 0;
    // iterate over all objects and add sizes (todo, used list not array?)
    for (handle = 1; handle <= MTPD_MAX_OBJECTS; handle++)
	{
        pmo = mtpd_from_handle(handle);
        if (pmo)
		{
            usedspace += pmo->size;
        }
    }

    freespace = space - usedspace;
    freeobjs = MTPD_MAX_OBJECTS - mtpd_file_count();

    ptp_pack_u16(&pb, &room, 0x0003); // storage type: fixed ram
    ptp_pack_u16(&pb, &room, 0x0002); // filesystem type: tree
    ptp_pack_u16(&pb, &room, PTP_OBJ_READWRITE); // access caps: r/w
    ptp_pack_u64(&pb, &room, space); // max cap
    ptp_pack_u64(&pb, &room, freespace);
    ptp_pack_u32(&pb, &room, freeobjs);
    ptp_pack_str(&pb, &room, PTPDEV_STORAGE_DESC);
    ptp_pack_str(&pb, &room, PTPDEV_VOLUME_ID);

    if (room == 0)
	{
        butil_log(1, "storageinfo overflow\n");
        return NULL;
    }

    if (reslen)
	{
        *reslen = pb - buf;
    }

    return buf;
}

int ptp_return_storageinfo(ptp_connection_t *ptpx, ioring_t *io)
{
    uint8_t *info, *infobuf;
    size_t infolen;
    int rc;

    infobuf = (uint8_t*)malloc(512); ///@TODO - size right
    info = ptp_create_storageinfo(ptpx, infobuf, 512, &infolen);

    // ptp response
    if (! info)
	{
        free(infobuf);
        return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
    }

    rc = ptp_send_data(ptpx, io, info, infolen);
    free(infobuf);
    if (rc < 0)
	{
        return rc;
    }

    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

static uint8_t *ptp_create_objinfo(ptp_connection_t *ptpx, uint32_t objhandle, uint8_t *buf, size_t nbuf, size_t *reslen)
{
    uint8_t *pb;
    size_t room;
    mtp_object_t *fileinfo;

    if (! buf || nbuf < 64)
	{
        return NULL;
    }

    pb = buf;
    room = nbuf;

    fileinfo = mtpd_file_info(objhandle);

    //butil_log(4, "objinfo for handle %08X\n", objhandle);
    ptp_pack_u32(&pb, &room, MTPD_ROOT_STORE);  // storage id
    ptp_pack_u16(&pb, &room, fileinfo ? fileinfo->format : 0);  // object format
    ptp_pack_u16(&pb, &room, fileinfo ? fileinfo->access : PTP_OBJ_READONLY);   // protection status
    ptp_pack_u32(&pb, &room, fileinfo ? fileinfo->size : 0);    // compressed size
    ptp_pack_u16(&pb, &room, 0);    // thumb format
    ptp_pack_u32(&pb, &room, 0);    // thumb compressed size
    ptp_pack_u32(&pb, &room, 0);    // thumb width
    ptp_pack_u32(&pb, &room, 0);    // thumb height
    ptp_pack_u32(&pb, &room, 0);    // image width
    ptp_pack_u32(&pb, &room, 0);    // image height
    ptp_pack_u32(&pb, &room, 0);    // image depth
    ptp_pack_u32(&pb, &room, fileinfo ?
                ((fileinfo->parent == 0xFFFFFFFF) ? 0 : fileinfo->parent) : 0); // parent object

	if (fileinfo && (fileinfo->format == PTP_OBJFMT_ASSOC))
	{
        ptp_pack_u16(&pb, &room, 1);    // assoc type = generic folder
        ptp_pack_u32(&pb, &room, 0);    // assoc desc
    }
	else
	{
        ptp_pack_u16(&pb, &room, 0);    // assoc type
        ptp_pack_u32(&pb, &room, 0);    // assoc desc
    }

    ptp_pack_u32(&pb, &room, objhandle);    // sequence

    ptp_pack_str(&pb, &room, fileinfo ? fileinfo->name : "-- gone --");

    {
        char tstamp[32];
        struct tm tm, *ptm;
        time_t mtime = fileinfo ? fileinfo->modt : 0;

        ptm = gmtime_r(&mtime, &tm);
        if (ptm) {
            strftime(tstamp, sizeof(tstamp), "%Y%d%mT%H%M%S", ptm);
        } else {
            strcpy(tstamp, "");
        }
        ptp_pack_str(&pb, &room, tstamp);   // date created
        ptp_pack_str(&pb, &room, tstamp);   // date modified
    }

    ptp_pack_str(&pb, &room, ""); // key words

    if (room == 0)
	{
        butil_log(1, "devinfo overflow\n");
        return NULL;
    }

    if (reslen)
	{
        *reslen = pb - buf;
    }

    return buf;
}

static int ptp_read_objinfo(uint32_t handle, uint8_t *data, size_t ndata)
{
    mtp_object_t *fileinfo;
    uint8_t *pb = data;
    size_t  room = ndata;

    fileinfo = mtpd_file_info(handle);
    if (! fileinfo)
	{
        return -1;
    }
                        ptp_unpack_u32(&pb, &room); // storage id
    fileinfo->format =  ptp_unpack_u16(&pb, &room); // object format
                        ptp_unpack_u16(&pb, &room); // protection status
    fileinfo->size =    ptp_unpack_u32(&pb, &room); // compressed size
                        ptp_unpack_u16(&pb, &room); // thumb format
                        ptp_unpack_u32(&pb, &room); // thumb compressed size
                        ptp_unpack_u32(&pb, &room); // thumb width
                        ptp_unpack_u32(&pb, &room); // thumb height
                        ptp_unpack_u32(&pb, &room); // image width
                        ptp_unpack_u32(&pb, &room); // image height
                        ptp_unpack_u32(&pb, &room); // image depth
                        ptp_unpack_u32(&pb, &room); // parent object
                        ptp_unpack_u16(&pb, &room); // assoc type
                        ptp_unpack_u32(&pb, &room); // assoc desc
                        ptp_unpack_u32(&pb, &room); // sequence

    ptp_unpack_str(&pb, &room, fileinfo->name, MTPD_MAX_NAME - 1);
    if (PTPPROT_DBG_LEVEL > 0)
	{
        butil_log(4, "FileINFO %s  f=%04X z=%lu\n", fileinfo->name, fileinfo->format, fileinfo->size);
    }

    return 0;
}

int ptp_return_objectinfo(ptp_connection_t *ptpx, ioring_t *io)
{
    mtp_object_t *pmo;
    int rc;
    uint32_t handle;

    // get handle from parms
    //
    handle = ptpx->cmdparms[0];
    pmo = mtpd_from_handle(handle);
    if (! pmo)
	{
        return ptp_response(ptpx, io, PTP_RESP_InvalidObjectHandle, 0);
    }

    // fill out an object info for this file
    //
    uint8_t *objinfo, *infobuf;
    size_t infolen;

    infobuf = (uint8_t*)malloc(512); ///@TODO - size right
    objinfo = ptp_create_objinfo(ptpx, handle, infobuf, 512, &infolen);

    if (! objinfo)
	{
        free(infobuf);
        return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
    }

    rc = ptp_send_data(ptpx, io, objinfo, infolen);
    free(infobuf);
    if (rc < 0)
	{
        return rc;
    }

    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

int ptp_return_object(ptp_connection_t *ptpx, ioring_t *io, uint32_t handle)
{
    mtp_object_t *pmo;
    uint8_t iobuf[1024];
    size_t chunk, rtot;
    int rc;

    pmo = mtpd_from_handle(handle);
    if (! pmo)
	{
        return ptp_response(ptpx, io, PTP_RESP_InvalidObjectHandle, 0);
    }

    rc = mtpd_openfile(ptpx->cmdparms[0], 0);
    if (rc < 0)
	{
        return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
    }

    rtot = 0;

    do
	{
        rc = mtpd_readfile(ptpx->cmdparms[0], iobuf, sizeof(iobuf), &chunk);
        if (rc >= 0 && chunk > 0)
		{
            if ((rtot + chunk) > pmo->size)
			{
                butil_log(1, "file size err\n");
                chunk = pmo->size - rtot;
            }

            rtot += chunk;
            if (PTPPROT_DBG_LEVEL > 2)
			{
                butil_log(5, "%lu  %lu of %lu\n", chunk, rtot, pmo->size);
            }

            rc = ptp_send_data_ex(ptpx, io, iobuf, chunk, pmo->size, (rtot == chunk), (rtot >= pmo->size));
        }
    }
	while (rc >= 0 && chunk > 0 && rtot < pmo->size);

    mtpd_closefile(ptpx->cmdparms[0]);

    rc = ptp_response(ptpx, io, (rc < 0) ? PTP_RESP_GeneralError : PTP_RESP_OK, 0);
    return rc;
}

int ptp_return_acceptobj(ptp_connection_t *ptpx, ioring_t *io, uint32_t handle)
{
    mtp_object_t *pmold, *pmnew;
    uint32_t storeid;
    uint32_t parentid;
    uint32_t oldhandle;
    int rc;

    storeid = ptpx->cmdparms[0];
    parentid = ptpx->cmdparms[1];

    pmnew = mtpd_from_handle(handle);
    if (! pmnew)
	{
        ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
    }

    if (PTPPROT_DBG_LEVEL > 0)
	{
        if (storeid != 0)
		{
            butil_log(4, "set store=%08X\n", storeid);
        }

        if (parentid != 0)
		{
            butil_log(4, "set parentid=%08X\n", parentid);
        }
    }

    // parse the object info
    rc = ptp_read_objinfo(ptpx->cmdparms[2], ptpx->data, ptpx->datacount);

    // no longer need data
    free(ptpx->data);
    ptpx->data = NULL;
    ptpx->datasize = 0;
    ptpx->datahead = 0;
    if (rc)
	{
        mtpd_destroyfile(handle);
        return rc;
    }

    // make sure parent id exists and is a directory
    //
    if (parentid == 0)
	{
        parentid = 0xFFFFFFFF;
        ptpx->cmdparms[1] = parentid;
    }

    if (parentid != 0xFFFFFFFF)
	{
        pmold = mtpd_from_handle(parentid);
        if (! pmold || (pmold->format != PTP_OBJFMT_ASSOC))
		{
            if (PTPPROT_DBG_LEVEL > 0)
			{
                butil_log(1, "bad parent\n");
            }
            mtpd_destroyfile(handle);
            return ptp_response(ptpx, io, PTP_RESP_InvalidParentObject, 0);
        }

        pmold = NULL;
    }

    // make sure we support the format
    //
    if (! ptp_getproperty_for_format(PTP_PROP_ObjectFormat, pmnew->format))
	{
        if (PTPPROT_DBG_LEVEL > 0)
		{
            butil_log(2, "not taking format %04X\n", pmnew->format);
        }
        mtpd_destroyfile(handle);
        return ptp_response(ptpx, io, PTP_RESP_InvalidParentObject, 0);
    }

    // check for existing items of same name/type/parent.  if exists,
    // for files, over-write them if writeable, for directories, just
    // point at exiting object and remove the new one and be OK.
    //
    rc = mtpd_from_info(&oldhandle, storeid, pmnew->name, parentid, pmnew->format);
    if (! rc)
	{
        // got an existing file, look up info
        pmold = mtpd_from_handle(oldhandle);
    }
	else
	{
        // no existing file (or issue with it)
        pmold = NULL;
        rc = 0;
    }

    if (pmold)
	{
        // a matching item exists in store
        if (pmold->format == PTP_OBJFMT_ASSOC)
		{
            if (PTPPROT_DBG_LEVEL > 0)
			{
                butil_log(4, "use existing dir %s\n", pmold->name);
            }
        }
		else
		{
            if (PTPPROT_DBG_LEVEL > 0)
			{
                butil_log(4, "overwrite exiting file %s\n", pmnew->name);
            }
        }

        // use exiting item in objects and free up new item
        mtpd_destroyfile(handle);
        handle = oldhandle;
        pmnew = pmold;
        ptpx->cmdparms[2] = handle;
    }
	else
	{
        // make sure new object has proper parent
        pmnew->parent = parentid;
    }

    rc = 0;
    if (pmnew->format != PTP_OBJFMT_ASSOC)
	{
        // if no format set, set it now based on name
        if (pmnew->format == 0 || pmnew->format == PTP_OBJFMT_UNDEF)
		{
            pmnew->format = mtpd_format_from_name(pmnew->name);
        }

        rc = mtpd_openfile(handle, 1);
        if (rc < 0)
		{
            mtpd_destroyfile(handle);
            return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
        }
		else
		{
            return ptp_response(ptpx, io, PTP_RESP_OK, 3);
        }

        if (pmnew->size == 0)
		{
            // 0 size files won't have any data sent, so close file too
            mtpd_closefile(handle);
        }
    }
	else
	{
        if (pmnew != pmold)
		{
            rc = mtpd_makedir(handle);
            if (rc < 0)
			{
                mtpd_destroyfile(handle);
                return ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
            }
        }

        return ptp_response(ptpx, io, PTP_RESP_OK, 3);
    }
}

int ptp_return_delobject(ptp_connection_t *ptpx, ioring_t *io, uint32_t handle)
{
    mtp_object_t *pmo;
    uint8_t iobuf[1024];
    size_t chunk, rtot;
    int rc;

    pmo = mtpd_from_handle(handle);
    if (! pmo)
	{
        return ptp_response(ptpx, io, PTP_RESP_InvalidObjectHandle, 0);
    }

    if (pmo->access == PTP_OBJ_READONLY)
	{
        if (PTPPROT_DBG_LEVEL > 0)
		{
            butil_log(2, "not deleting readonly %s\n", pmo->name);
        }

        return ptp_response(ptpx, io, PTP_RESP_AccessDenied, 0);
    }

    mtpd_deletefile(handle);
    return ptp_response(ptpx, io, PTP_RESP_OK, 0);
}

static int ptp_event(
					ptp_connection_t *ptpx,
                    uint32_t sessionId,
                    uint32_t transactionId,
                    uint16_t code,
                    size_t   nparms,
                    uint32_t parm0,
                    uint32_t parm1,
                    uint32_t parm2
                    )
{
    uint8_t eventbuf[32], *evt;
    size_t  evtlen;

    evt = eventbuf;
    evtlen = sizeof(eventbuf);

    ptp_pack_u16(&evt, &evtlen, code);
//  ptp_pack_u32(&evt, &evtlen, sessionId); // spec lies vs real world?
    ptp_pack_u32(&evt, &evtlen, transactionId);
    if (nparms > 0)
	{
        ptp_pack_u32(&evt, &evtlen, parm0);
    }

    if (nparms > 1)
	{
        ptp_pack_u32(&evt, &evtlen, parm1);
    }

    if (nparms > 2)
	{
        ptp_pack_u32(&evt, &evtlen, parm2);
    }

    return ptp_emit_event(ptpx, eventbuf, (evt - eventbuf));
}

static int ptp_perform(ptp_connection_t *ptpx, ioring_t *io)
{
    int rc;

    rc = 0;

    switch (ptpx->opcode)
	{
    case PTP_OPCODE_GetDeviceInfo:
        rc = ptp_return_deviceinfo(ptpx, io);
        break;
    case PTP_OPCODE_OpenSession:
        ptpx->session_id = ptpx->cmdparms[0];
        rc = ptp_response(ptpx, io, PTP_RESP_OK, 0);
        break;
    case PTP_OPCODE_CloseSession:
        rc = ptp_response(ptpx, io, PTP_RESP_OK, 0);
        ptpx->session_id = 0xFFFFFFFF;
        break;
    case PTP_OPCODE_GetStorageIDs:
        rc = ptp_return_storageids(ptpx, io);
        break;
    case PTP_OPCODE_GetStorageInfo:
        rc = ptp_return_storageinfo(ptpx, io);
        break;
    case PTP_OPCODE_GetNumObjects:
        ptpx->cmdparms[0] = mtpd_file_count();
        rc = ptp_response(ptpx, io, PTP_RESP_OK, 1);
        break;
        break;
    case PTP_OPCODE_GetObjectHandles:
        rc = ptp_return_objecthandles(ptpx, io);
        break;
    case PTP_OPCODE_GetObjectInfo:
        rc = ptp_return_objectinfo(ptpx, io);
        break;
    case PTP_OPCODE_GetObject:
        rc = ptp_return_object(ptpx, io, ptpx->cmdparms[0]);
        break;
    case PTP_OPCODE_DeleteObject:
        rc = ptp_return_delobject(ptpx, io, ptpx->cmdparms[0]);
        break;
    case PTP_OPCODE_SendObjectInfo:
        if (ptpx->data && (ptpx->datacount || (ptpx->datasize == 0)))
		{
            // last call in here, decode info and start data xfer. note that
            // ptpx->data is also used as a flag to indicate we started data phase
            rc = ptp_return_acceptobj(ptpx, io, ptpx->cmdparms[2]);
        }
		else
		{
            // first call in here, setup a straw-man proto-object to be filled in
            // and save the new handle in cmdparms
            rc = mtpd_createfile("", 0, 0, 0, PTP_OBJ_READWRITE, &ptpx->cmdparms[2]);
            if (PTPPROT_DBG_LEVEL > 0) {
                butil_log(3, "Created new handle %08X\n", ptpx->cmdparms[2]);
            }
        }

        if (rc < 0)
		{
            ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
        }

        rc = 0;
        break;
    case PTP_OPCODE_SendObject:
        if (ptpx->data && (ptpx->datahead || (ptpx->datasize == 0)))
		{
            if (ptpx->datahead)
			{
                // append this data to open file
                mtpd_appendfile(ptpx->cmdparms[2], ptpx->data, ptpx->datahead);
                ptpx->datahead = 0;
            }

            if (ptpx->datacount >= ptpx->datatotal)
			{
                // at end of last data, send response
                mtpd_closefile(ptpx->cmdparms[2]);
                rc = ptp_response(ptpx, io, PTP_RESP_OK, 0);

                // emit object added event
                if (rc >= 0)
				{
                    rc = ptp_event(
									ptpx,
                                    ptpx->session_id,
                                    ptpx->trans_id,
                                    PTP_EVENT_ObjectAdded,
                                    1,
                                    ptpx->cmdparms[2], 0, 0
                                  );
                }
            }
			else
			{
                // still expect more data
                rc = 0;
            }
        }
		else
		{
            // wait for dataphase
            rc = 0;
        }
        break;
    case PTP_OPCODE_GetDevicePropDesc:
        rc = ptp_return_devproperty(ptpx, io, 1);
        break;
    case PTP_OPCODE_GetDevicePropValue:
        rc = ptp_return_devproperty(ptpx, io, 0);
        break;
    case PTP_OPCODE_SetDevicePropValue:
    case PTP_OPCODE_ResetDevicePropValue:
        rc = ptp_response(ptpx, io, PTP_RESP_AccessDenied, 0);
        break;
        // mtp object properties
    case PTP_OPCODE_GetObjectPropsSupported:
        rc = ptp_return_objpropssupported(ptpx, io);
        break;
    case PTP_OPCODE_GetObjectPropDesc:
        rc = ptp_return_objproperty(ptpx, io, 1);
        break;
    case PTP_OPCODE_GetObjectPropValue:
        rc = ptp_return_objproperty(ptpx, io, 0);
        break;
    case PTP_OPCODE_SetObjectPropValue:
        rc = ptp_response(ptpx, io, PTP_RESP_AccessDenied, 0);
        break;
    case PTP_OPCODE_GetObjectPropList:
        rc = ptp_return_listobjproperty(ptpx, io);
        break;
    case PTP_OPCODE_SetObjectPropList:
        rc = ptp_response(ptpx, io, PTP_RESP_AccessDenied, 0);
        break;
    case PTP_OPCODE_GetObjectReferences:
        rc = ptp_return_objrefs(ptpx, io);
        break;
    case PTP_OPCODE_SetObjectReferences:
        butil_log(4, "SET OBJREF %08X\n", ptpx->cmdparms[0]);
        rc = ptp_response(ptpx, io, PTP_RESP_OperationNotSupported, 0);
        break;
    case PTP_OPCODE_GetThumb:
    case PTP_OPCODE_InitiateCapture:
    case PTP_OPCODE_FormatStore:
    case PTP_OPCODE_ResetDevice:
    case PTP_OPCODE_SelfTest:
    case PTP_OPCODE_SetObjectProtection:
    case PTP_OPCODE_PowerDown:
    case PTP_OPCODE_TerminateOpenCapture:
    case PTP_OPCODE_MoveObject:
    case PTP_OPCODE_CopyObject:
    case PTP_OPCODE_GetPartialObject:
    case PTP_OPCODE_InitiateOpenCapture:
        rc = ptp_response(ptpx, io, PTP_RESP_OperationNotSupported, 0);
        break;
    default:
        butil_log(2, "Unhandled ptp code %04X  trans %08X\n",
            ptpx->opcode, ptpx->trans_id);
        rc = ptp_response(ptpx, io, PTP_RESP_OperationNotSupported, 0);
        break;
    }

    return rc;
}

int ptp_dispatch(ptp_connection_t *ptpx, ioring_t *io, size_t count)
{
    int rc;
    uint16_t opcode;
    uint32_t parm;
    size_t nparms;

    if (count < 6)
	{
        return -1;
    }

    rc = ptp_read(io, (uint8_t*)&opcode, 2);
    if (rc != 2)
	{
        return -1;
    }

    ptpx->cancelflag = 0;
    rc = ptp_read(io, (uint8_t*)&ptpx->trans_id, 4);
    if (rc != 4)
	{
        return -1;
    }

    count -= 6;

    nparms = count / 4;
    if (nparms > PTP_MAX_PARMS)
	{
        butil_log(1, "overflow parms\n");
        return -1;
    }

    for (ptpx->ncmdparms = 0; ptpx->ncmdparms < nparms; ptpx->ncmdparms++)
	{
        rc = ptp_read(io, (uint8_t*)&parm, 4);
        if (rc != 4)
		{
            return -1;
        }

        count -= 4;
        ptpx->cmdparms[ptpx->ncmdparms] = parm;
    }

    if (opcode != PTP_OPCODE_OpenSession && opcode != PTP_OPCODE_GetDeviceInfo)
	{
        if (ptpx->session_id == 0xFFFFFFFF)
		{
            ptp_response(ptpx, io, PTP_RESP_SessionNotOpen, 0);
            ptp_consume(io, count);
            return 0;
        }
    }
	else if (opcode != PTP_OPCODE_GetDeviceInfo)
	{
        if (ptpx->session_id != 0xFFFFFFFF)
		{
            ptp_response(ptpx, io, PTP_RESP_GeneralError, 0);
            ptp_consume(io, count);
            return 0;
        }
    }

    ptpx->opcode = opcode;

    if (PTPPROT_DBG_LEVEL > 0)
	{
        size_t n;

        butil_log(4, "OPCODE=%04X ", opcode);
        for (n = 0; n < nparms; n++) {
            butil_log(4, "%08X ", ptpx->cmdparms[n]);
        }

        butil_log(4, "\n");
    }

    // data follows command, so first time in, make sure there's no data
	//
    if (ptpx->data)
	{
        free(ptpx->data);
        ptpx->data = NULL;
        ptpx->datacount = 0;
        ptpx->datasize = 0;
    }

    // do the command (phase 1)
    rc = ptp_perform(ptpx, io);

    if (count > 0)
	{
        ptp_consume(io, count);
    }

    return rc;
}

int ptp_rx_data(ptp_connection_t *ptpx, ioring_t *io, size_t count, size_t oftotal)
{
    size_t chunk;
    int rc;
    int incremental_ok;

    // note: still do all this for counts of 0 of 0, it could be first-last-and-only chunk
    // and we need to drive protocol regardless, and 0 data is valid for some files, for example
    //
    if (count == 0 && oftotal != 0)
	{
        return 0;
    }

    incremental_ok = 0;
    if (ptpx->opcode == PTP_OPCODE_SendObject)
	{
        incremental_ok = 1;
    }

    rc = 0;
    if (ptpx->data)
	{
        do
		{
            chunk = count;
            if (chunk > (ptpx->datasize - ptpx->datahead))
			{
                // no room in buffer for all of this read, limit
                // it in incremental (streaming) case, or fail
                // for not (since buffer was prealloced big enough)
                if (incremental_ok)
				{
                    chunk = (ptpx->datasize - ptpx->datahead);
                }
				else
				{
                    rc = -1;
                    break;
                }
            }

            if (chunk > 0)
			{
                rc = ptp_read(io, ptpx->data + ptpx->datahead, chunk);
                if (PTPPROT_DBG_LEVEL > 1)
				{
                    butil_log(5, " data read %d of %lu\n", rc, chunk);
                }

                if (rc < 0)
				{
                    return rc;
                }

                ptpx->datahead+= rc;
                ptpx->datacount+= rc;
                count-= rc;
            }

            // when got to total expected, re-perform the command
            // this time with data (or if 0 data sent)
			//
            if (ptpx->datacount >= oftotal)
			{
                rc = ptp_perform(ptpx, io);
                // count really must be 0 at this point
            }
			else
			{
                if (incremental_ok)
				{
                    // incrementally add any data to object which should make room
                    rc = ptp_perform(ptpx, io);
                    ptpx->datahead = 0;
                }
				else
				{
                    rc = 0;
                }
            }
        }
		while ((count > 0) && (rc >= 0));
    }
	else
	{
        // overflow or error (todo - handle this ?)
        butil_log(2, "absorb %lu bytes\n", count);
        ptp_consume(io, count);
        rc = 0;
    }

    return rc;
}

int ptp_data_start(ptp_connection_t *ptpx, ioring_t *io, size_t total)
{
    if (PTPPROT_DBG_LEVEL > 0)
	{
        butil_log(4, "PTP DATA %lu\n", total);
    }

    if (ptpx->data)
	{
        free(ptpx->data);
        ptpx->data = NULL;
        ptpx->datahead = 0;
        ptpx->datacount = 0;
        ptpx->datatotal = 0;
        ptpx->datasize = 0;
    }

    ptpx->datatotal = total;

    // limit total size to transfer buffer of object data
    // else alloc for all needed since other commands expect it all
    if (total > PTP_MAX_XFER)
	{
        if (ptpx->opcode != PTP_OPCODE_SendObject)
		{
            butil_log(2, "Too much %lu data for max xfer %u\n", total, PTP_MAX_XFER);
            return -1;
        }

        total = PTP_MAX_XFER;
    }
    ptpx->datahead = 0;
    ptpx->datacount = 0;

    // if not getting any data, call rx data ourselves to drive protocol
    //
    if (total == 0)
	{
        // alloc for some data as it's also used as a flag TODO - use a real flag?
        ptpx->data = (uint8_t *)malloc(32);
    }
	else
	{
        ptpx->data = (uint8_t *)malloc(total);
    }

    if (ptpx->data)
	{
        ptpx->datasize = total;
        return 0;
    }
	else
	{
        ptpx->datasize = 0;
        ptpx->datatotal = 0;
        return -1;
    }
}

int ptp_cancel(ptp_connection_t *ptpx)
{
    ptpx->cancelflag = 1;
    if (PTPPROT_DBG_LEVEL > 0)
	{
        butil_log(3, "Xfer Canceled by client\n");
    }

    return 0;
}

int ptp_init(ptp_connection_t *ptpx)
{
    memset(ptpx, 0, sizeof(ptp_connection_t));
    ptpx->session_id = 0xFFFFFFFF;
    return 0;
}

void ptp_deinit(ptp_connection_t *ptpx)
{
    memset(ptpx, 0, sizeof(ptp_connection_t));
    ptpx->session_id = 0xFFFFFFFF;
    return;
}


