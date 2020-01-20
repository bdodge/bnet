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
#ifndef BMTP_H
#define BMTP_H 1

#include "bnetheaders.h"
#include "bhttp.h"
#include "bupnp.h"

#define MTP_IP_PORT 			(15740)
#define MTP_IP_MAX_CONNECTIONS	(2)

#define MTP_ERROR BERROR

// size of IO buffers, and hence max single xfer
//
#define PTP_IO_SIZE 						(1024)
#define PTP_MAX_STRING          			(256)

#define PTP_MAX_SUP_CODES       			(32+32)
#define PTP_MAX_SUP_EVENTS      			(16+16)
#define PTP_MAX_SUP_PROPS       			(32+32)
#define PTP_MAX_CAP_FRMTS       			(64+64)
#define PTP_MAX_IMG_FRMTS       			(64+64)

// PTP Device Info structure
//
struct ptp_device_info
{
    uint16_t      version;
    uint32_t      extension_id;
    uint16_t      extension_ver;
    char          extension_desc[PTP_MAX_STRING];
    uint16_t      functional_mode;
    uint32_t      num_codes;
    uint32_t      sup_codes [PTP_MAX_SUP_CODES];
    uint32_t      num_events;
    uint32_t      sup_events[PTP_MAX_SUP_EVENTS];
    uint32_t      num_props;
    uint32_t      sup_props [PTP_MAX_SUP_PROPS];
    uint32_t      num_cfrmts;
    uint32_t      cpt_formats[PTP_MAX_CAP_FRMTS];
    uint32_t      num_ifrmts;
    uint32_t      img_formats[PTP_MAX_IMG_FRMTS];
    char          manuf[PTP_MAX_STRING];
    char          model[PTP_MAX_STRING];
    char          device_ver[PTP_MAX_STRING];
    char          serial_num[PTP_MAX_STRING];
}
ptp_device_info_t;

// PTP Object Info
//
struct ptp_object_info
{
    uint32_t      storage_id;
    uint16_t      object_format;
    uint16_t      prot_status;
    uint32_t      compressed_size;
    uint16_t      thumb_format;
    uint32_t      thumb_compressed_size;
    uint32_t      thumb_pix_w;
    uint32_t      thumb_pix_h;
    uint32_t      img_pix_w;
    uint32_t      img_pix_h;
    uint32_t      img_pix_d;
    uint32_t      parent_object;
    uint16_t      assoc_type;
    uint32_t      assoc_desc;
    uint32_t      seq_num;
    char          filename[PTP_MAX_STRING];
    char          cap_date[PTP_MAX_STRING];
    char          mod_date[PTP_MAX_STRING];
    char          keywords[PTP_MAX_STRING];
}
ptp_object_info_t;

// largest expected one-read data payload
// (for all commands except get/set object which stream)
#define PTP_MAX_XFER 1024

#define PTP_MAX_PARMS 5

typedef struct ptp_connection
{
	struct ptp *ptp;

    enum {
			ptpReadCommand,
            ptpReadCommandPayload,
            ptpReadEventPayload,
            ptpReadDataPayload,
            ptpCommand,
			ptpSetupEvent,
			ptpEvent,
            ptpRun,
			ptpError
    }
    state, nextstate;

	uint32_t cmdtype;
    uint32_t cmdlen;
    uint32_t evttype;
    uint32_t evtlen;
    uint32_t transid;
    uint32_t datalen;
    uint32_t lenread;
	socket_t client_sock;
    socket_t event_sock;

    ioring_t cmdin;
    ioring_t cmdout;
    ioring_t evtin;
    ioring_t evtout;

	uint8_t  cmdinbuf[PTP_IO_SIZE];
	uint8_t  cmdoutbuf[PTP_IO_SIZE];
	uint8_t  evtinbuf[PTP_IO_SIZE];
	uint8_t  evtoutbuf[PTP_IO_SIZE];

    uint32_t  ip_session_id;                // ip session id
    uint32_t  session_id;                   // session id
    uint32_t  trans_id;                     // transaction id
    uint32_t  active_trans_id;              // active transaction id
    uint32_t  cancelflag;                   // set non-0 to indicate xfer canceled

	size_t    dataexpected;					// how much data we get in dataphase
	size_t    datareceived;                 // how much have gotten so far
	ioring_t  data;							// object data
	uint8_t   databuf[PTP_IO_SIZE];

    uint32_t  opcode;                       // response code
    uint32_t  cmdparms[PTP_MAX_PARMS];      // list of returned parameters
    size_t    ncmdparms;                    // how many parms returned
}
ptp_connection_t;

// a PTP device context
//
typedef struct ptp
{
	buuid_t uuid_device;
	uint16_t device_name[PTP_MAX_STRING];
	uint32_t device_name_len;

	uint32_t sessionid;
    socket_t server_sock;

	ptp_connection_t connection;
}
ptp_t;

#include "bptpproto.h"
#include "bptpio.h"
#include "bptpfiles.h"
#include "bptpprops.h"

// Version of PTP we support "1"
#define PTPDEV_PTP_VERSION  100

// Extended PTP version ID, and Version. Windows Media Player wants to see 6
// 6 seems to be what WMP wants to see
#define PTPDEV_EXT_ID       6
#define PTPDEV_EXT_VERSION  100
// Windows wont play with us unless this is in the extended description
#define PTPDEV_EXT_DESC     "" /*"microsoft.com: 1.0;"*/

// the WPDNA enables wifi provisioning?
//#define PTPDEV_EXT_DESC     "microsoft.com: 1.0; microsoft.com/WPDNA: 1.0"
#define PTPDEV_FUNCMODE     	0
#define PTPDEV_MANUFACTURE  	"Bnet"
#define PTPDEV_MODEL        	"bNet-MTP"
#define PTPDEV_DEVICE_VERSION   "1.0"
#define PTPDEV_DEVICE_SERIALNUM "1234"

#define PTPDEV_STORAGE_DESC 	"Media"
#define PTPDEV_VOLUME_ID    	"Files"

int ptp_emit_event(ptp_connection_t *ptpx, uint8_t *event, size_t eventlen);

int mtp_device_init(ptp_t *ptp, buuid_t uuid, const char *name);
int mtp_device_slice(ptp_t *ptp, int tos, int tous);

#endif
