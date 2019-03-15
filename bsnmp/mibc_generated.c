
/*
 * This is a generated file - Do Not Edit!
 */

#ifndef _BMIBC_GENERATED_C
#define _BMIBC_GENERATED_C 1

#ifndef BSNMPOBJECT_H
#include <stdio.h>
#include <stdint.h>
#endif
#include "mibc_generated.h"

/* Object records array
*/
bmibc_record_t g_mib_objects[BMIBC_NUM_RECORDS] = {
/********* NVRAMSIGNATURE 
*/
{
  /*   name:*/ "NVRAMSIGNATURE",
  /*min,max:*/ 32, 32,
  /*    dim:*/ 0,
  /* offset:*/ 0,
  /*encbits:*/ 256,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fvs_NVRAMSIGNATURE,
  /*vol val:*/ (void*)_vvs_NVRAMSIGNATURE
},

/********* sysDescr 
*/
{
  /*   name:*/ "sysDescr",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_sysDescr
},

/********* sysObjectID 
*/
{
  /*   name:*/ "sysObjectID",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_sysObjectID
},

/********* sysUpTime 
*/
{
  /*   name:*/ "sysUpTime",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* sysContact 
*/
{
  /*   name:*/ "sysContact",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_sysContact
},

/********* sysName 
*/
{
  /*   name:*/ "sysName",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_sysName
},

/********* sysLocation 
*/
{
  /*   name:*/ "sysLocation",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_sysLocation
},

/********* sysServices 
*/
{
  /*   name:*/ "sysServices",
  /*min,max:*/ 0, 127,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 7,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* sysORLastChange 
*/
{
  /*   name:*/ "sysORLastChange",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* sysORIndex 
*/
{
  /*   name:*/ "sysORIndex",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_sysORIndex,
  /*vol val:*/ (void*)_vva_sysORIndex
},

/********* sysORID 
*/
{
  /*   name:*/ "sysORID",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_sysORID
},

/********* sysORDescr 
*/
{
  /*   name:*/ "sysORDescr",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_sysORDescr
},

/********* sysORUpTime 
*/
{
  /*   name:*/ "sysORUpTime",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_sysORUpTime,
  /*vol val:*/ (void*)_vva_sysORUpTime
},

/********* ifNumber 
*/
{
  /*   name:*/ "ifNumber",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifIndex 
*/
{
  /*   name:*/ "ifIndex",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifIndex,
  /*vol val:*/ (void*)_vva_ifIndex
},

/********* ifDescr 
*/
{
  /*   name:*/ "ifDescr",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_ifDescr
},

/********* ifType 
*/
{
  /*   name:*/ "ifType",
  /*min,max:*/ 1, 197,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 8,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifType,
  /*vol val:*/ (void*)_vva_ifType
},

/********* ifMtu 
*/
{
  /*   name:*/ "ifMtu",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifMtu,
  /*vol val:*/ (void*)_vva_ifMtu
},

/********* ifSpeed 
*/
{
  /*   name:*/ "ifSpeed",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifSpeed,
  /*vol val:*/ (void*)_vva_ifSpeed
},

/********* ifPhysAddress 
*/
{
  /*   name:*/ "ifPhysAddress",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_ifPhysAddress
},

/********* ifAdminStatus 
*/
{
  /*   name:*/ "ifAdminStatus",
  /*min,max:*/ 1, 3,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifAdminStatus,
  /*vol val:*/ (void*)_vva_ifAdminStatus
},

/********* ifOperStatus 
*/
{
  /*   name:*/ "ifOperStatus",
  /*min,max:*/ 1, 7,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifOperStatus,
  /*vol val:*/ (void*)_vva_ifOperStatus
},

/********* ifLastChange 
*/
{
  /*   name:*/ "ifLastChange",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifLastChange,
  /*vol val:*/ (void*)_vva_ifLastChange
},

/********* ifInOctets 
*/
{
  /*   name:*/ "ifInOctets",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifInOctets,
  /*vol val:*/ (void*)_vva_ifInOctets
},

/********* ifInUcastPkts 
*/
{
  /*   name:*/ "ifInUcastPkts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifInUcastPkts,
  /*vol val:*/ (void*)_vva_ifInUcastPkts
},

/********* ifInNUcastPkts 
*/
{
  /*   name:*/ "ifInNUcastPkts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifInNUcastPkts,
  /*vol val:*/ (void*)_vva_ifInNUcastPkts
},

/********* ifInDiscards 
*/
{
  /*   name:*/ "ifInDiscards",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifInDiscards,
  /*vol val:*/ (void*)_vva_ifInDiscards
},

/********* ifInErrors 
*/
{
  /*   name:*/ "ifInErrors",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifInErrors,
  /*vol val:*/ (void*)_vva_ifInErrors
},

/********* ifInUnknownProtos 
*/
{
  /*   name:*/ "ifInUnknownProtos",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifInUnknownProtos,
  /*vol val:*/ (void*)_vva_ifInUnknownProtos
},

/********* ifOutOctets 
*/
{
  /*   name:*/ "ifOutOctets",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifOutOctets,
  /*vol val:*/ (void*)_vva_ifOutOctets
},

/********* ifOutUcastPkts 
*/
{
  /*   name:*/ "ifOutUcastPkts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifOutUcastPkts,
  /*vol val:*/ (void*)_vva_ifOutUcastPkts
},

/********* ifOutNUcastPkts 
*/
{
  /*   name:*/ "ifOutNUcastPkts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifOutNUcastPkts,
  /*vol val:*/ (void*)_vva_ifOutNUcastPkts
},

/********* ifOutDiscards 
*/
{
  /*   name:*/ "ifOutDiscards",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifOutDiscards,
  /*vol val:*/ (void*)_vva_ifOutDiscards
},

/********* ifOutErrors 
*/
{
  /*   name:*/ "ifOutErrors",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifOutErrors,
  /*vol val:*/ (void*)_vva_ifOutErrors
},

/********* ifOutQLen 
*/
{
  /*   name:*/ "ifOutQLen",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifOutQLen,
  /*vol val:*/ (void*)_vva_ifOutQLen
},

/********* ifSpecific 
*/
{
  /*   name:*/ "ifSpecific",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_ifSpecific
},

/********* snmpInPkts 
*/
{
  /*   name:*/ "snmpInPkts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpOutPkts 
*/
{
  /*   name:*/ "snmpOutPkts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInBadVersions 
*/
{
  /*   name:*/ "snmpInBadVersions",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInBadCommunityNames 
*/
{
  /*   name:*/ "snmpInBadCommunityNames",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInBadCommunityUses 
*/
{
  /*   name:*/ "snmpInBadCommunityUses",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInASNParseErrs 
*/
{
  /*   name:*/ "snmpInASNParseErrs",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInTooBigs 
*/
{
  /*   name:*/ "snmpInTooBigs",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInNoSuchNames 
*/
{
  /*   name:*/ "snmpInNoSuchNames",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInBadValues 
*/
{
  /*   name:*/ "snmpInBadValues",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInReadOnlys 
*/
{
  /*   name:*/ "snmpInReadOnlys",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInGenErrs 
*/
{
  /*   name:*/ "snmpInGenErrs",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInTotalReqVars 
*/
{
  /*   name:*/ "snmpInTotalReqVars",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInTotalSetVars 
*/
{
  /*   name:*/ "snmpInTotalSetVars",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInGetRequests 
*/
{
  /*   name:*/ "snmpInGetRequests",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInGetNexts 
*/
{
  /*   name:*/ "snmpInGetNexts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInSetRequests 
*/
{
  /*   name:*/ "snmpInSetRequests",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInGetResponses 
*/
{
  /*   name:*/ "snmpInGetResponses",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpInTraps 
*/
{
  /*   name:*/ "snmpInTraps",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpOutTooBigs 
*/
{
  /*   name:*/ "snmpOutTooBigs",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpOutNoSuchNames 
*/
{
  /*   name:*/ "snmpOutNoSuchNames",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpOutBadValues 
*/
{
  /*   name:*/ "snmpOutBadValues",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpOutGenErrs 
*/
{
  /*   name:*/ "snmpOutGenErrs",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpOutGetRequests 
*/
{
  /*   name:*/ "snmpOutGetRequests",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpOutGetNexts 
*/
{
  /*   name:*/ "snmpOutGetNexts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpOutSetRequests 
*/
{
  /*   name:*/ "snmpOutSetRequests",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpOutGetResponses 
*/
{
  /*   name:*/ "snmpOutGetResponses",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpOutTraps 
*/
{
  /*   name:*/ "snmpOutTraps",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpEnableAuthenTraps 
*/
{
  /*   name:*/ "snmpEnableAuthenTraps",
  /*min,max:*/ 1, 2,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpSilentDrops 
*/
{
  /*   name:*/ "snmpSilentDrops",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* snmpProxyDrops 
*/
{
  /*   name:*/ "snmpProxyDrops",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrSystemUptime 
*/
{
  /*   name:*/ "hrSystemUptime",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrSystemDate 
*/
{
  /*   name:*/ "hrSystemDate",
  /*min,max:*/ 8, 11,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 96,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrSystemDate
},

/********* hrSystemInitialLoadDevice 
*/
{
  /*   name:*/ "hrSystemInitialLoadDevice",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrSystemInitialLoadParameters 
*/
{
  /*   name:*/ "hrSystemInitialLoadParameters",
  /*min,max:*/ 0, 128,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 1032,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrSystemInitialLoadParameters
},

/********* hrSystemNumUsers 
*/
{
  /*   name:*/ "hrSystemNumUsers",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrSystemProcesses 
*/
{
  /*   name:*/ "hrSystemProcesses",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrSystemMaxProcesses 
*/
{
  /*   name:*/ "hrSystemMaxProcesses",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrMemorySize 
*/
{
  /*   name:*/ "hrMemorySize",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrStorageIndex 
*/
{
  /*   name:*/ "hrStorageIndex",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrStorageIndex,
  /*vol val:*/ (void*)_vva_hrStorageIndex
},

/********* hrStorageType 
*/
{
  /*   name:*/ "hrStorageType",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrStorageType
},

/********* hrStorageDescr 
*/
{
  /*   name:*/ "hrStorageDescr",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrStorageDescr
},

/********* hrStorageAllocationUnits 
*/
{
  /*   name:*/ "hrStorageAllocationUnits",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrStorageAllocationUnits,
  /*vol val:*/ (void*)_vva_hrStorageAllocationUnits
},

/********* hrStorageSize 
*/
{
  /*   name:*/ "hrStorageSize",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrStorageSize,
  /*vol val:*/ (void*)_vva_hrStorageSize
},

/********* hrStorageUsed 
*/
{
  /*   name:*/ "hrStorageUsed",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrStorageUsed,
  /*vol val:*/ (void*)_vva_hrStorageUsed
},

/********* hrStorageAllocationFailures 
*/
{
  /*   name:*/ "hrStorageAllocationFailures",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrStorageAllocationFailures,
  /*vol val:*/ (void*)_vva_hrStorageAllocationFailures
},

/********* hrDeviceIndex 
*/
{
  /*   name:*/ "hrDeviceIndex",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrDeviceIndex,
  /*vol val:*/ (void*)_vva_hrDeviceIndex
},

/********* hrDeviceType 
*/
{
  /*   name:*/ "hrDeviceType",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrDeviceType
},

/********* hrDeviceDescr 
*/
{
  /*   name:*/ "hrDeviceDescr",
  /*min,max:*/ 0, 64,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 520,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrDeviceDescr
},

/********* hrDeviceID 
*/
{
  /*   name:*/ "hrDeviceID",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrDeviceID
},

/********* hrDeviceStatus 
*/
{
  /*   name:*/ "hrDeviceStatus",
  /*min,max:*/ 1, 5,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrDeviceStatus,
  /*vol val:*/ (void*)_vva_hrDeviceStatus
},

/********* hrDeviceErrors 
*/
{
  /*   name:*/ "hrDeviceErrors",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrDeviceErrors,
  /*vol val:*/ (void*)_vva_hrDeviceErrors
},

/********* hrProcessorFrwID 
*/
{
  /*   name:*/ "hrProcessorFrwID",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrProcessorFrwID
},

/********* hrProcessorLoad 
*/
{
  /*   name:*/ "hrProcessorLoad",
  /*min,max:*/ 0, 100,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 7,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrProcessorLoad,
  /*vol val:*/ (void*)_vva_hrProcessorLoad
},

/********* hrNetworkIfIndex 
*/
{
  /*   name:*/ "hrNetworkIfIndex",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrNetworkIfIndex,
  /*vol val:*/ (void*)_vva_hrNetworkIfIndex
},

/********* hrPrinterStatus 
*/
{
  /*   name:*/ "hrPrinterStatus",
  /*min,max:*/ 1, 5,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrPrinterStatus,
  /*vol val:*/ (void*)_vva_hrPrinterStatus
},

/********* hrPrinterDetectedErrorState 
*/
{
  /*   name:*/ "hrPrinterDetectedErrorState",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrPrinterDetectedErrorState
},

/********* hrDiskStorageAccess 
*/
{
  /*   name:*/ "hrDiskStorageAccess",
  /*min,max:*/ 1, 2,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrDiskStorageAccess,
  /*vol val:*/ (void*)_vva_hrDiskStorageAccess
},

/********* hrDiskStorageMedia 
*/
{
  /*   name:*/ "hrDiskStorageMedia",
  /*min,max:*/ 1, 8,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrDiskStorageMedia,
  /*vol val:*/ (void*)_vva_hrDiskStorageMedia
},

/********* hrDiskStorageRemoveble 
*/
{
  /*   name:*/ "hrDiskStorageRemoveble",
  /*min,max:*/ 1, 2,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrDiskStorageRemoveble,
  /*vol val:*/ (void*)_vva_hrDiskStorageRemoveble
},

/********* hrDiskStorageCapacity 
*/
{
  /*   name:*/ "hrDiskStorageCapacity",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrDiskStorageCapacity,
  /*vol val:*/ (void*)_vva_hrDiskStorageCapacity
},

/********* hrPartitionIndex 
*/
{
  /*   name:*/ "hrPartitionIndex",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrPartitionIndex,
  /*vol val:*/ (void*)_vva_hrPartitionIndex
},

/********* hrPartitionLabel 
*/
{
  /*   name:*/ "hrPartitionLabel",
  /*min,max:*/ 0, 128,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1032,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrPartitionLabel
},

/********* hrPartitionID 
*/
{
  /*   name:*/ "hrPartitionID",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrPartitionID
},

/********* hrPartitionSize 
*/
{
  /*   name:*/ "hrPartitionSize",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrPartitionSize,
  /*vol val:*/ (void*)_vva_hrPartitionSize
},

/********* hrPartitionFSIndex 
*/
{
  /*   name:*/ "hrPartitionFSIndex",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrPartitionFSIndex,
  /*vol val:*/ (void*)_vva_hrPartitionFSIndex
},

/********* hrFSIndex 
*/
{
  /*   name:*/ "hrFSIndex",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrFSIndex,
  /*vol val:*/ (void*)_vva_hrFSIndex
},

/********* hrFSMountPoint 
*/
{
  /*   name:*/ "hrFSMountPoint",
  /*min,max:*/ 0, 128,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1032,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrFSMountPoint
},

/********* hrFSRemoteMountPoint 
*/
{
  /*   name:*/ "hrFSRemoteMountPoint",
  /*min,max:*/ 0, 128,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1032,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrFSRemoteMountPoint
},

/********* hrFSType 
*/
{
  /*   name:*/ "hrFSType",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrFSType
},

/********* hrFSAccess 
*/
{
  /*   name:*/ "hrFSAccess",
  /*min,max:*/ 1, 2,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrFSAccess,
  /*vol val:*/ (void*)_vva_hrFSAccess
},

/********* hrFSBootable 
*/
{
  /*   name:*/ "hrFSBootable",
  /*min,max:*/ 1, 2,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrFSBootable,
  /*vol val:*/ (void*)_vva_hrFSBootable
},

/********* hrFSStorageIndex 
*/
{
  /*   name:*/ "hrFSStorageIndex",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrFSStorageIndex,
  /*vol val:*/ (void*)_vva_hrFSStorageIndex
},

/********* hrFSLastFullBackupDate 
*/
{
  /*   name:*/ "hrFSLastFullBackupDate",
  /*min,max:*/ 8, 11,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 96,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrFSLastFullBackupDate
},

/********* hrFSLastPartialBackupDate 
*/
{
  /*   name:*/ "hrFSLastPartialBackupDate",
  /*min,max:*/ 8, 11,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 96,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrFSLastPartialBackupDate
},

/********* hrSWOSIndex 
*/
{
  /*   name:*/ "hrSWOSIndex",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrSWRunIndex 
*/
{
  /*   name:*/ "hrSWRunIndex",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrSWRunIndex,
  /*vol val:*/ (void*)_vva_hrSWRunIndex
},

/********* hrSWRunName 
*/
{
  /*   name:*/ "hrSWRunName",
  /*min,max:*/ 0, 64,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 520,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrSWRunName
},

/********* hrSWRunID 
*/
{
  /*   name:*/ "hrSWRunID",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrSWRunID
},

/********* hrSWRunPath 
*/
{
  /*   name:*/ "hrSWRunPath",
  /*min,max:*/ 0, 128,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1032,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrSWRunPath
},

/********* hrSWRunParameters 
*/
{
  /*   name:*/ "hrSWRunParameters",
  /*min,max:*/ 0, 128,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1032,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrSWRunParameters
},

/********* hrSWRunType 
*/
{
  /*   name:*/ "hrSWRunType",
  /*min,max:*/ 1, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrSWRunType,
  /*vol val:*/ (void*)_vva_hrSWRunType
},

/********* hrSWRunStatus 
*/
{
  /*   name:*/ "hrSWRunStatus",
  /*min,max:*/ 1, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrSWRunStatus,
  /*vol val:*/ (void*)_vva_hrSWRunStatus
},

/********* hrSWRunPerfCPU 
*/
{
  /*   name:*/ "hrSWRunPerfCPU",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrSWRunPerfMem 
*/
{
  /*   name:*/ "hrSWRunPerfMem",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrSWInstalledLastChange 
*/
{
  /*   name:*/ "hrSWInstalledLastChange",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrSWInstalledLastUpdateTime 
*/
{
  /*   name:*/ "hrSWInstalledLastUpdateTime",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* hrSWInstalledIndex 
*/
{
  /*   name:*/ "hrSWInstalledIndex",
  /*min,max:*/ 1, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrSWInstalledIndex,
  /*vol val:*/ (void*)_vva_hrSWInstalledIndex
},

/********* hrSWInstalledName 
*/
{
  /*   name:*/ "hrSWInstalledName",
  /*min,max:*/ 0, 64,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 520,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrSWInstalledName
},

/********* hrSWInstalledID 
*/
{
  /*   name:*/ "hrSWInstalledID",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrSWInstalledID
},

/********* hrSWInstalledType 
*/
{
  /*   name:*/ "hrSWInstalledType",
  /*min,max:*/ 1, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_hrSWInstalledType,
  /*vol val:*/ (void*)_vva_hrSWInstalledType
},

/********* hrSWInstalledDate 
*/
{
  /*   name:*/ "hrSWInstalledDate",
  /*min,max:*/ 8, 11,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 96,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_hrSWInstalledDate
},

/********* ifName 
*/
{
  /*   name:*/ "ifName",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_ifName
},

/********* ifInMulticastPkts 
*/
{
  /*   name:*/ "ifInMulticastPkts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifInBroadcastPkts 
*/
{
  /*   name:*/ "ifInBroadcastPkts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifOutMulticastPkts 
*/
{
  /*   name:*/ "ifOutMulticastPkts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifOutBroadcastPkts 
*/
{
  /*   name:*/ "ifOutBroadcastPkts",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifHCInOctets 
*/
{
  /*   name:*/ "ifHCInOctets",
  /*min,max:*/ 0UL, 0UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 64,
  /*   type:*/ BMIBC_UINT64,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vva_ifHCInOctets
},

/********* ifHCInUcastPkts 
*/
{
  /*   name:*/ "ifHCInUcastPkts",
  /*min,max:*/ 0UL, 0UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 64,
  /*   type:*/ BMIBC_UINT64,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vva_ifHCInUcastPkts
},

/********* ifHCInMulticastPkts 
*/
{
  /*   name:*/ "ifHCInMulticastPkts",
  /*min,max:*/ 0UL, 0UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 64,
  /*   type:*/ BMIBC_UINT64,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vva_ifHCInMulticastPkts
},

/********* ifHCInBroadcastPkts 
*/
{
  /*   name:*/ "ifHCInBroadcastPkts",
  /*min,max:*/ 0UL, 0UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 64,
  /*   type:*/ BMIBC_UINT64,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vva_ifHCInBroadcastPkts
},

/********* ifHCOutOctets 
*/
{
  /*   name:*/ "ifHCOutOctets",
  /*min,max:*/ 0UL, 0UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 64,
  /*   type:*/ BMIBC_UINT64,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vva_ifHCOutOctets
},

/********* ifHCOutUcastPkts 
*/
{
  /*   name:*/ "ifHCOutUcastPkts",
  /*min,max:*/ 0UL, 0UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 64,
  /*   type:*/ BMIBC_UINT64,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vva_ifHCOutUcastPkts
},

/********* ifHCOutMulticastPkts 
*/
{
  /*   name:*/ "ifHCOutMulticastPkts",
  /*min,max:*/ 0UL, 0UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 64,
  /*   type:*/ BMIBC_UINT64,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vva_ifHCOutMulticastPkts
},

/********* ifHCOutBroadcastPkts 
*/
{
  /*   name:*/ "ifHCOutBroadcastPkts",
  /*min,max:*/ 0UL, 0UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 64,
  /*   type:*/ BMIBC_UINT64,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vva_ifHCOutBroadcastPkts
},

/********* ifLinkUpDownTrapEnable 
*/
{
  /*   name:*/ "ifLinkUpDownTrapEnable",
  /*min,max:*/ 1, 2,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifHighSpeed 
*/
{
  /*   name:*/ "ifHighSpeed",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifPromiscuousMode 
*/
{
  /*   name:*/ "ifPromiscuousMode",
  /*min,max:*/ 1, 2,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifConnectorPresent 
*/
{
  /*   name:*/ "ifConnectorPresent",
  /*min,max:*/ 1, 2,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifAlias 
*/
{
  /*   name:*/ "ifAlias",
  /*min,max:*/ 0, 64,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 520,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_ifAlias
},

/********* ifCounterDiscontinuityTime 
*/
{
  /*   name:*/ "ifCounterDiscontinuityTime",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifStackHigherLayer 
*/
{
  /*   name:*/ "ifStackHigherLayer",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifStackHigherLayer,
  /*vol val:*/ (void*)_vva_ifStackHigherLayer
},

/********* ifStackLowerLayer 
*/
{
  /*   name:*/ "ifStackLowerLayer",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifStackLowerLayer,
  /*vol val:*/ (void*)_vva_ifStackLowerLayer
},

/********* ifStackStatus 
*/
{
  /*   name:*/ "ifStackStatus",
  /*min,max:*/ 1, 6,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_ifStackStatus,
  /*vol val:*/ (void*)_vva_ifStackStatus
},

/********* ifTableLastChange 
*/
{
  /*   name:*/ "ifTableLastChange",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* ifStackLastChange 
*/
{
  /*   name:*/ "ifStackLastChange",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
},

/********* prtGeneralConfigChanges 
*/
{
  /*   name:*/ "prtGeneralConfigChanges",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtGeneralConfigChanges,
  /*vol val:*/ (void*)_vva_prtGeneralConfigChanges
},

/********* prtGeneralCurrentLocalization 
*/
{
  /*   name:*/ "prtGeneralCurrentLocalization",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtGeneralCurrentLocalization,
  /*vol val:*/ (void*)_vva_prtGeneralCurrentLocalization
},

/********* prtGeneralReset 
*/
{
  /*   name:*/ "prtGeneralReset",
  /*min,max:*/ 3, 6,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtGeneralReset,
  /*vol val:*/ (void*)_vva_prtGeneralReset
},

/********* prtGeneralCurrentOperator 
*/
{
  /*   name:*/ "prtGeneralCurrentOperator",
  /*min,max:*/ 0, 127,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1024,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtGeneralCurrentOperator
},

/********* prtGeneralServicePerson 
*/
{
  /*   name:*/ "prtGeneralServicePerson",
  /*min,max:*/ 0, 127,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1024,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtGeneralServicePerson
},

/********* prtInputDefaultIndex 
*/
{
  /*   name:*/ "prtInputDefaultIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputDefaultIndex,
  /*vol val:*/ (void*)_vva_prtInputDefaultIndex
},

/********* prtOutputDefaultIndex 
*/
{
  /*   name:*/ "prtOutputDefaultIndex",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputDefaultIndex,
  /*vol val:*/ (void*)_vva_prtOutputDefaultIndex
},

/********* prtMarkerDefaultIndex 
*/
{
  /*   name:*/ "prtMarkerDefaultIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerDefaultIndex,
  /*vol val:*/ (void*)_vva_prtMarkerDefaultIndex
},

/********* prtMediaPathDefaultIndex 
*/
{
  /*   name:*/ "prtMediaPathDefaultIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathDefaultIndex,
  /*vol val:*/ (void*)_vva_prtMediaPathDefaultIndex
},

/********* prtConsoleLocalization 
*/
{
  /*   name:*/ "prtConsoleLocalization",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtConsoleLocalization,
  /*vol val:*/ (void*)_vva_prtConsoleLocalization
},

/********* prtConsoleNumberOfDisplayLines 
*/
{
  /*   name:*/ "prtConsoleNumberOfDisplayLines",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtConsoleNumberOfDisplayLines,
  /*vol val:*/ (void*)_vva_prtConsoleNumberOfDisplayLines
},

/********* prtConsoleNumberOfDisplayChars 
*/
{
  /*   name:*/ "prtConsoleNumberOfDisplayChars",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtConsoleNumberOfDisplayChars,
  /*vol val:*/ (void*)_vva_prtConsoleNumberOfDisplayChars
},

/********* prtConsoleDisable 
*/
{
  /*   name:*/ "prtConsoleDisable",
  /*min,max:*/ 3, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtConsoleDisable,
  /*vol val:*/ (void*)_vva_prtConsoleDisable
},

/********* prtStorageRefSeqNumber 
*/
{
  /*   name:*/ "prtStorageRefSeqNumber",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtStorageRefSeqNumber,
  /*vol val:*/ (void*)_vva_prtStorageRefSeqNumber
},

/********* prtStorageRefIndex 
*/
{
  /*   name:*/ "prtStorageRefIndex",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtStorageRefIndex,
  /*vol val:*/ (void*)_vva_prtStorageRefIndex
},

/********* prtDeviceRefSeqNumber 
*/
{
  /*   name:*/ "prtDeviceRefSeqNumber",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtDeviceRefSeqNumber,
  /*vol val:*/ (void*)_vva_prtDeviceRefSeqNumber
},

/********* prtDeviceRefIndex 
*/
{
  /*   name:*/ "prtDeviceRefIndex",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtDeviceRefIndex,
  /*vol val:*/ (void*)_vva_prtDeviceRefIndex
},

/********* prtCoverIndex 
*/
{
  /*   name:*/ "prtCoverIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtCoverIndex,
  /*vol val:*/ (void*)_vva_prtCoverIndex
},

/********* prtCoverDescription 
*/
{
  /*   name:*/ "prtCoverDescription",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtCoverDescription
},

/********* prtCoverStatus 
*/
{
  /*   name:*/ "prtCoverStatus",
  /*min,max:*/ 1, 6,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtCoverStatus,
  /*vol val:*/ (void*)_vva_prtCoverStatus
},

/********* prtLocalizationIndex 
*/
{
  /*   name:*/ "prtLocalizationIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtLocalizationIndex,
  /*vol val:*/ (void*)_vva_prtLocalizationIndex
},

/********* prtLocalizationLanguage 
*/
{
  /*   name:*/ "prtLocalizationLanguage",
  /*min,max:*/ 0, 2,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 24,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtLocalizationLanguage
},

/********* prtLocalizationCountry 
*/
{
  /*   name:*/ "prtLocalizationCountry",
  /*min,max:*/ 0, 2,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 24,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtLocalizationCountry
},

/********* prtLocalizationCharacterSet 
*/
{
  /*   name:*/ "prtLocalizationCharacterSet",
  /*min,max:*/ 1, 1,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtLocalizationCharacterSet,
  /*vol val:*/ (void*)_vva_prtLocalizationCharacterSet
},

/********* prtInputIndex 
*/
{
  /*   name:*/ "prtInputIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputIndex,
  /*vol val:*/ (void*)_vva_prtInputIndex
},

/********* prtInputType 
*/
{
  /*   name:*/ "prtInputType",
  /*min,max:*/ 1, 7,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputType,
  /*vol val:*/ (void*)_vva_prtInputType
},

/********* prtInputDimUnit 
*/
{
  /*   name:*/ "prtInputDimUnit",
  /*min,max:*/ 3, 4,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputDimUnit,
  /*vol val:*/ (void*)_vva_prtInputDimUnit
},

/********* prtInputMediaDimFeedDirDeclared 
*/
{
  /*   name:*/ "prtInputMediaDimFeedDirDeclared",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputMediaDimFeedDirDeclared,
  /*vol val:*/ (void*)_vva_prtInputMediaDimFeedDirDeclared
},

/********* prtInputMediaDimXFeedDirDeclared 
*/
{
  /*   name:*/ "prtInputMediaDimXFeedDirDeclared",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputMediaDimXFeedDirDeclared,
  /*vol val:*/ (void*)_vva_prtInputMediaDimXFeedDirDeclared
},

/********* prtInputMediaDimFeedDirChosen 
*/
{
  /*   name:*/ "prtInputMediaDimFeedDirChosen",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputMediaDimFeedDirChosen,
  /*vol val:*/ (void*)_vva_prtInputMediaDimFeedDirChosen
},

/********* prtInputMediaDimXFeedDirChosen 
*/
{
  /*   name:*/ "prtInputMediaDimXFeedDirChosen",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputMediaDimXFeedDirChosen,
  /*vol val:*/ (void*)_vva_prtInputMediaDimXFeedDirChosen
},

/********* prtInputCapacityUnit 
*/
{
  /*   name:*/ "prtInputCapacityUnit",
  /*min,max:*/ 3, 17,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 4,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputCapacityUnit,
  /*vol val:*/ (void*)_vva_prtInputCapacityUnit
},

/********* prtInputMaxCapacity 
*/
{
  /*   name:*/ "prtInputMaxCapacity",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputMaxCapacity,
  /*vol val:*/ (void*)_vva_prtInputMaxCapacity
},

/********* prtInputCurrentLevel 
*/
{
  /*   name:*/ "prtInputCurrentLevel",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputCurrentLevel,
  /*vol val:*/ (void*)_vva_prtInputCurrentLevel
},

/********* prtInputStatus 
*/
{
  /*   name:*/ "prtInputStatus",
  /*min,max:*/ 0, 126,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 7,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputStatus,
  /*vol val:*/ (void*)_vva_prtInputStatus
},

/********* prtInputMediaName 
*/
{
  /*   name:*/ "prtInputMediaName",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fvs_prtInputMediaName,
  /*vol val:*/ (void*)_vvs_prtInputMediaName
},

/********* prtInputName 
*/
{
  /*   name:*/ "prtInputName",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fvs_prtInputName,
  /*vol val:*/ (void*)_vvs_prtInputName
},

/********* prtInputVendorName 
*/
{
  /*   name:*/ "prtInputVendorName",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInputVendorName
},

/********* prtInputModel 
*/
{
  /*   name:*/ "prtInputModel",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInputModel
},

/********* prtInputVersion 
*/
{
  /*   name:*/ "prtInputVersion",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInputVersion
},

/********* prtInputSerialNumber 
*/
{
  /*   name:*/ "prtInputSerialNumber",
  /*min,max:*/ 0, 32,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 264,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInputSerialNumber
},

/********* prtInputDescription 
*/
{
  /*   name:*/ "prtInputDescription",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInputDescription
},

/********* prtInputSecurity 
*/
{
  /*   name:*/ "prtInputSecurity",
  /*min,max:*/ 1, 5,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputSecurity,
  /*vol val:*/ (void*)_vva_prtInputSecurity
},

/********* prtInputMediaWeight 
*/
{
  /*   name:*/ "prtInputMediaWeight",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputMediaWeight,
  /*vol val:*/ (void*)_vva_prtInputMediaWeight
},

/********* prtInputMediaType 
*/
{
  /*   name:*/ "prtInputMediaType",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInputMediaType
},

/********* prtInputMediaColor 
*/
{
  /*   name:*/ "prtInputMediaColor",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInputMediaColor
},

/********* prtInputMediaFormParts 
*/
{
  /*   name:*/ "prtInputMediaFormParts",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInputMediaFormParts,
  /*vol val:*/ (void*)_vva_prtInputMediaFormParts
},

/********* prtOutputIndex 
*/
{
  /*   name:*/ "prtOutputIndex",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputIndex,
  /*vol val:*/ (void*)_vva_prtOutputIndex
},

/********* prtOutputType 
*/
{
  /*   name:*/ "prtOutputType",
  /*min,max:*/ 1, 7,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputType,
  /*vol val:*/ (void*)_vva_prtOutputType
},

/********* prtOutputCapacityUnit 
*/
{
  /*   name:*/ "prtOutputCapacityUnit",
  /*min,max:*/ 3, 17,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 4,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputCapacityUnit,
  /*vol val:*/ (void*)_vva_prtOutputCapacityUnit
},

/********* prtOutputMaxCapacity 
*/
{
  /*   name:*/ "prtOutputMaxCapacity",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputMaxCapacity,
  /*vol val:*/ (void*)_vva_prtOutputMaxCapacity
},

/********* prtOutputRemainingCapacity 
*/
{
  /*   name:*/ "prtOutputRemainingCapacity",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputRemainingCapacity,
  /*vol val:*/ (void*)_vva_prtOutputRemainingCapacity
},

/********* prtOutputStatus 
*/
{
  /*   name:*/ "prtOutputStatus",
  /*min,max:*/ 0, 126,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 7,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputStatus,
  /*vol val:*/ (void*)_vva_prtOutputStatus
},

/********* prtOutputName 
*/
{
  /*   name:*/ "prtOutputName",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtOutputName
},

/********* prtOutputVendorName 
*/
{
  /*   name:*/ "prtOutputVendorName",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtOutputVendorName
},

/********* prtOutputModel 
*/
{
  /*   name:*/ "prtOutputModel",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtOutputModel
},

/********* prtOutputVersion 
*/
{
  /*   name:*/ "prtOutputVersion",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtOutputVersion
},

/********* prtOutputSerialNumber 
*/
{
  /*   name:*/ "prtOutputSerialNumber",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtOutputSerialNumber
},

/********* prtOutputDescription 
*/
{
  /*   name:*/ "prtOutputDescription",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtOutputDescription
},

/********* prtOutputSecurity 
*/
{
  /*   name:*/ "prtOutputSecurity",
  /*min,max:*/ 1, 5,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputSecurity,
  /*vol val:*/ (void*)_vva_prtOutputSecurity
},

/********* prtOutputDimUnit 
*/
{
  /*   name:*/ "prtOutputDimUnit",
  /*min,max:*/ 3, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputDimUnit,
  /*vol val:*/ (void*)_vva_prtOutputDimUnit
},

/********* prtOutputMaxDimFeedDir 
*/
{
  /*   name:*/ "prtOutputMaxDimFeedDir",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputMaxDimFeedDir,
  /*vol val:*/ (void*)_vva_prtOutputMaxDimFeedDir
},

/********* prtOutputMaxDimXFeedDir 
*/
{
  /*   name:*/ "prtOutputMaxDimXFeedDir",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputMaxDimXFeedDir,
  /*vol val:*/ (void*)_vva_prtOutputMaxDimXFeedDir
},

/********* prtOutputMinDimFeedDir 
*/
{
  /*   name:*/ "prtOutputMinDimFeedDir",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputMinDimFeedDir,
  /*vol val:*/ (void*)_vva_prtOutputMinDimFeedDir
},

/********* prtOutputMinDimXFeedDir 
*/
{
  /*   name:*/ "prtOutputMinDimXFeedDir",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputMinDimXFeedDir,
  /*vol val:*/ (void*)_vva_prtOutputMinDimXFeedDir
},

/********* prtOutputStackingOrder 
*/
{
  /*   name:*/ "prtOutputStackingOrder",
  /*min,max:*/ 2, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputStackingOrder,
  /*vol val:*/ (void*)_vva_prtOutputStackingOrder
},

/********* prtOutputPageDeliveryOrientation 
*/
{
  /*   name:*/ "prtOutputPageDeliveryOrientation",
  /*min,max:*/ 3, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputPageDeliveryOrientation,
  /*vol val:*/ (void*)_vva_prtOutputPageDeliveryOrientation
},

/********* prtOutputBursting 
*/
{
  /*   name:*/ "prtOutputBursting",
  /*min,max:*/ 1, 5,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputBursting,
  /*vol val:*/ (void*)_vva_prtOutputBursting
},

/********* prtOutputDecollating 
*/
{
  /*   name:*/ "prtOutputDecollating",
  /*min,max:*/ 1, 5,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputDecollating,
  /*vol val:*/ (void*)_vva_prtOutputDecollating
},

/********* prtOutputPageCollated 
*/
{
  /*   name:*/ "prtOutputPageCollated",
  /*min,max:*/ 1, 5,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputPageCollated,
  /*vol val:*/ (void*)_vva_prtOutputPageCollated
},

/********* prtOutputOffsetStacking 
*/
{
  /*   name:*/ "prtOutputOffsetStacking",
  /*min,max:*/ 1, 5,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtOutputOffsetStacking,
  /*vol val:*/ (void*)_vva_prtOutputOffsetStacking
},

/********* prtMarkerIndex 
*/
{
  /*   name:*/ "prtMarkerIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerIndex,
  /*vol val:*/ (void*)_vva_prtMarkerIndex
},

/********* prtMarkerMarkTech 
*/
{
  /*   name:*/ "prtMarkerMarkTech",
  /*min,max:*/ 1, 27,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 5,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerMarkTech,
  /*vol val:*/ (void*)_vva_prtMarkerMarkTech
},

/********* prtMarkerCounterUnit 
*/
{
  /*   name:*/ "prtMarkerCounterUnit",
  /*min,max:*/ 3, 17,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 4,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerCounterUnit,
  /*vol val:*/ (void*)_vva_prtMarkerCounterUnit
},

/********* prtMarkerLifeCount 
*/
{
  /*   name:*/ "prtMarkerLifeCount",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerLifeCount,
  /*vol val:*/ (void*)_vva_prtMarkerLifeCount
},

/********* prtMarkerPowerOnCount 
*/
{
  /*   name:*/ "prtMarkerPowerOnCount",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerPowerOnCount,
  /*vol val:*/ (void*)_vva_prtMarkerPowerOnCount
},

/********* prtMarkerProcessColorants 
*/
{
  /*   name:*/ "prtMarkerProcessColorants",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerProcessColorants,
  /*vol val:*/ (void*)_vva_prtMarkerProcessColorants
},

/********* prtMarkerSpotColorants 
*/
{
  /*   name:*/ "prtMarkerSpotColorants",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerSpotColorants,
  /*vol val:*/ (void*)_vva_prtMarkerSpotColorants
},

/********* prtMarkerAddressabilityUnit 
*/
{
  /*   name:*/ "prtMarkerAddressabilityUnit",
  /*min,max:*/ 3, 4,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerAddressabilityUnit,
  /*vol val:*/ (void*)_vva_prtMarkerAddressabilityUnit
},

/********* prtMarkerAddressabilityFeedDir 
*/
{
  /*   name:*/ "prtMarkerAddressabilityFeedDir",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerAddressabilityFeedDir,
  /*vol val:*/ (void*)_vva_prtMarkerAddressabilityFeedDir
},

/********* prtMarkerAddressabilityXFeedDir 
*/
{
  /*   name:*/ "prtMarkerAddressabilityXFeedDir",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerAddressabilityXFeedDir,
  /*vol val:*/ (void*)_vva_prtMarkerAddressabilityXFeedDir
},

/********* prtMarkerNorthMargin 
*/
{
  /*   name:*/ "prtMarkerNorthMargin",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerNorthMargin,
  /*vol val:*/ (void*)_vva_prtMarkerNorthMargin
},

/********* prtMarkerSouthMargin 
*/
{
  /*   name:*/ "prtMarkerSouthMargin",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerSouthMargin,
  /*vol val:*/ (void*)_vva_prtMarkerSouthMargin
},

/********* prtMarkerWestMargin 
*/
{
  /*   name:*/ "prtMarkerWestMargin",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerWestMargin,
  /*vol val:*/ (void*)_vva_prtMarkerWestMargin
},

/********* prtMarkerEastMargin 
*/
{
  /*   name:*/ "prtMarkerEastMargin",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerEastMargin,
  /*vol val:*/ (void*)_vva_prtMarkerEastMargin
},

/********* prtMarkerStatus 
*/
{
  /*   name:*/ "prtMarkerStatus",
  /*min,max:*/ 0, 126,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 7,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerStatus,
  /*vol val:*/ (void*)_vva_prtMarkerStatus
},

/********* prtMarkerSuppliesIndex 
*/
{
  /*   name:*/ "prtMarkerSuppliesIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerSuppliesIndex,
  /*vol val:*/ (void*)_vva_prtMarkerSuppliesIndex
},

/********* prtMarkerSuppliesMarkerIndex 
*/
{
  /*   name:*/ "prtMarkerSuppliesMarkerIndex",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerSuppliesMarkerIndex,
  /*vol val:*/ (void*)_vva_prtMarkerSuppliesMarkerIndex
},

/********* prtMarkerSuppliesColorantIndex 
*/
{
  /*   name:*/ "prtMarkerSuppliesColorantIndex",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerSuppliesColorantIndex,
  /*vol val:*/ (void*)_vva_prtMarkerSuppliesColorantIndex
},

/********* prtMarkerSuppliesClass 
*/
{
  /*   name:*/ "prtMarkerSuppliesClass",
  /*min,max:*/ 1, 4,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerSuppliesClass,
  /*vol val:*/ (void*)_vva_prtMarkerSuppliesClass
},

/********* prtMarkerSuppliesType 
*/
{
  /*   name:*/ "prtMarkerSuppliesType",
  /*min,max:*/ 1, 14,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 4,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerSuppliesType,
  /*vol val:*/ (void*)_vva_prtMarkerSuppliesType
},

/********* prtMarkerSuppliesDescription 
*/
{
  /*   name:*/ "prtMarkerSuppliesDescription",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtMarkerSuppliesDescription
},

/********* prtMarkerSuppliesSupplyUnit 
*/
{
  /*   name:*/ "prtMarkerSuppliesSupplyUnit",
  /*min,max:*/ 3, 15,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 4,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerSuppliesSupplyUnit,
  /*vol val:*/ (void*)_vva_prtMarkerSuppliesSupplyUnit
},

/********* prtMarkerSuppliesMaxCapacity 
*/
{
  /*   name:*/ "prtMarkerSuppliesMaxCapacity",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerSuppliesMaxCapacity,
  /*vol val:*/ (void*)_vva_prtMarkerSuppliesMaxCapacity
},

/********* prtMarkerSuppliesLevel 
*/
{
  /*   name:*/ "prtMarkerSuppliesLevel",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerSuppliesLevel,
  /*vol val:*/ (void*)_vva_prtMarkerSuppliesLevel
},

/********* prtMarkerColorantIndex 
*/
{
  /*   name:*/ "prtMarkerColorantIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerColorantIndex,
  /*vol val:*/ (void*)_vva_prtMarkerColorantIndex
},

/********* prtMarkerColorantMarkerIndex 
*/
{
  /*   name:*/ "prtMarkerColorantMarkerIndex",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerColorantMarkerIndex,
  /*vol val:*/ (void*)_vva_prtMarkerColorantMarkerIndex
},

/********* prtMarkerColorantRole 
*/
{
  /*   name:*/ "prtMarkerColorantRole",
  /*min,max:*/ 1, 4,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerColorantRole,
  /*vol val:*/ (void*)_vva_prtMarkerColorantRole
},

/********* prtMarkerColorantValue 
*/
{
  /*   name:*/ "prtMarkerColorantValue",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fvs_prtMarkerColorantValue,
  /*vol val:*/ (void*)_vvs_prtMarkerColorantValue
},

/********* prtMarkerColorantTonality 
*/
{
  /*   name:*/ "prtMarkerColorantTonality",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 4,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMarkerColorantTonality,
  /*vol val:*/ (void*)_vva_prtMarkerColorantTonality
},

/********* prtMediaPathIndex 
*/
{
  /*   name:*/ "prtMediaPathIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathIndex,
  /*vol val:*/ (void*)_vva_prtMediaPathIndex
},

/********* prtMediaPathMaxSpeedPrintUnit 
*/
{
  /*   name:*/ "prtMediaPathMaxSpeedPrintUnit",
  /*min,max:*/ 3, 17,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 4,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathMaxSpeedPrintUnit,
  /*vol val:*/ (void*)_vva_prtMediaPathMaxSpeedPrintUnit
},

/********* prtMediaPathMediaSizeUnit 
*/
{
  /*   name:*/ "prtMediaPathMediaSizeUnit",
  /*min,max:*/ 3, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathMediaSizeUnit,
  /*vol val:*/ (void*)_vva_prtMediaPathMediaSizeUnit
},

/********* prtMediaPathMaxSpeed 
*/
{
  /*   name:*/ "prtMediaPathMaxSpeed",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathMaxSpeed,
  /*vol val:*/ (void*)_vva_prtMediaPathMaxSpeed
},

/********* prtMediaPathMaxMediaFeedDir 
*/
{
  /*   name:*/ "prtMediaPathMaxMediaFeedDir",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathMaxMediaFeedDir,
  /*vol val:*/ (void*)_vva_prtMediaPathMaxMediaFeedDir
},

/********* prtMediaPathMaxMediaXFeedDir 
*/
{
  /*   name:*/ "prtMediaPathMaxMediaXFeedDir",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathMaxMediaXFeedDir,
  /*vol val:*/ (void*)_vva_prtMediaPathMaxMediaXFeedDir
},

/********* prtMediaPathMinMediaFeedDir 
*/
{
  /*   name:*/ "prtMediaPathMinMediaFeedDir",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathMinMediaFeedDir,
  /*vol val:*/ (void*)_vva_prtMediaPathMinMediaFeedDir
},

/********* prtMediaPathMinMediaXFeedDir 
*/
{
  /*   name:*/ "prtMediaPathMinMediaXFeedDir",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathMinMediaXFeedDir,
  /*vol val:*/ (void*)_vva_prtMediaPathMinMediaXFeedDir
},

/********* prtMediaPathType 
*/
{
  /*   name:*/ "prtMediaPathType",
  /*min,max:*/ 1, 5,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathType,
  /*vol val:*/ (void*)_vva_prtMediaPathType
},

/********* prtMediaPathDescription 
*/
{
  /*   name:*/ "prtMediaPathDescription",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtMediaPathDescription
},

/********* prtMediaPathStatus 
*/
{
  /*   name:*/ "prtMediaPathStatus",
  /*min,max:*/ 0, 126,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 7,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtMediaPathStatus,
  /*vol val:*/ (void*)_vva_prtMediaPathStatus
},

/********* prtChannelIndex 
*/
{
  /*   name:*/ "prtChannelIndex",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtChannelIndex,
  /*vol val:*/ (void*)_vva_prtChannelIndex
},

/********* prtChannelType 
*/
{
  /*   name:*/ "prtChannelType",
  /*min,max:*/ 1, 33,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 6,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtChannelType,
  /*vol val:*/ (void*)_vva_prtChannelType
},

/********* prtChannelProtocolVersion 
*/
{
  /*   name:*/ "prtChannelProtocolVersion",
  /*min,max:*/ 0, 63,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 512,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtChannelProtocolVersion
},

/********* prtChannelCurrentJobCntlLangIndex 
*/
{
  /*   name:*/ "prtChannelCurrentJobCntlLangIndex",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtChannelCurrentJobCntlLangIndex,
  /*vol val:*/ (void*)_vva_prtChannelCurrentJobCntlLangIndex
},

/********* prtChannelDefaultPageDescLangIndex 
*/
{
  /*   name:*/ "prtChannelDefaultPageDescLangIndex",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtChannelDefaultPageDescLangIndex,
  /*vol val:*/ (void*)_vva_prtChannelDefaultPageDescLangIndex
},

/********* prtChannelState 
*/
{
  /*   name:*/ "prtChannelState",
  /*min,max:*/ 1, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtChannelState,
  /*vol val:*/ (void*)_vva_prtChannelState
},

/********* prtChannelIfIndex 
*/
{
  /*   name:*/ "prtChannelIfIndex",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtChannelIfIndex,
  /*vol val:*/ (void*)_vva_prtChannelIfIndex
},

/********* prtChannelStatus 
*/
{
  /*   name:*/ "prtChannelStatus",
  /*min,max:*/ 0, 126,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 7,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtChannelStatus,
  /*vol val:*/ (void*)_vva_prtChannelStatus
},

/********* prtInterpreterIndex 
*/
{
  /*   name:*/ "prtInterpreterIndex",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInterpreterIndex,
  /*vol val:*/ (void*)_vva_prtInterpreterIndex
},

/********* prtInterpreterLangFamily 
*/
{
  /*   name:*/ "prtInterpreterLangFamily",
  /*min,max:*/ 1, 46,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 6,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInterpreterLangFamily,
  /*vol val:*/ (void*)_vva_prtInterpreterLangFamily
},

/********* prtInterpreterLangLevel 
*/
{
  /*   name:*/ "prtInterpreterLangLevel",
  /*min,max:*/ 0, 31,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 256,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInterpreterLangLevel
},

/********* prtInterpreterLangVersion 
*/
{
  /*   name:*/ "prtInterpreterLangVersion",
  /*min,max:*/ 0, 31,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 256,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInterpreterLangVersion
},

/********* prtInterpreterDescription 
*/
{
  /*   name:*/ "prtInterpreterDescription",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInterpreterDescription
},

/********* prtInterpreterVersion 
*/
{
  /*   name:*/ "prtInterpreterVersion",
  /*min,max:*/ 0, 31,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 256,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtInterpreterVersion
},

/********* prtInterpreterDefaultOrientation 
*/
{
  /*   name:*/ "prtInterpreterDefaultOrientation",
  /*min,max:*/ 1, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInterpreterDefaultOrientation,
  /*vol val:*/ (void*)_vva_prtInterpreterDefaultOrientation
},

/********* prtInterpreterFeedAddressability 
*/
{
  /*   name:*/ "prtInterpreterFeedAddressability",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInterpreterFeedAddressability,
  /*vol val:*/ (void*)_vva_prtInterpreterFeedAddressability
},

/********* prtInterpreterXFeedAddressability 
*/
{
  /*   name:*/ "prtInterpreterXFeedAddressability",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInterpreterXFeedAddressability,
  /*vol val:*/ (void*)_vva_prtInterpreterXFeedAddressability
},

/********* prtInterpreterDefaultCharSetIn 
*/
{
  /*   name:*/ "prtInterpreterDefaultCharSetIn",
  /*min,max:*/ 1, 1,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInterpreterDefaultCharSetIn,
  /*vol val:*/ (void*)_vva_prtInterpreterDefaultCharSetIn
},

/********* prtInterpreterDefaultCharSetOut 
*/
{
  /*   name:*/ "prtInterpreterDefaultCharSetOut",
  /*min,max:*/ 1, 1,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInterpreterDefaultCharSetOut,
  /*vol val:*/ (void*)_vva_prtInterpreterDefaultCharSetOut
},

/********* prtInterpreterTwoWay 
*/
{
  /*   name:*/ "prtInterpreterTwoWay",
  /*min,max:*/ 3, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 1,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtInterpreterTwoWay,
  /*vol val:*/ (void*)_vva_prtInterpreterTwoWay
},

/********* prtConsoleDisplayBufferIndex 
*/
{
  /*   name:*/ "prtConsoleDisplayBufferIndex",
  /*min,max:*/ 1, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtConsoleDisplayBufferIndex,
  /*vol val:*/ (void*)_vva_prtConsoleDisplayBufferIndex
},

/********* prtConsoleDisplayBufferText 
*/
{
  /*   name:*/ "prtConsoleDisplayBufferText",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtConsoleDisplayBufferText
},

/********* prtConsoleLightIndex 
*/
{
  /*   name:*/ "prtConsoleLightIndex",
  /*min,max:*/ 0, 65535,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 16,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtConsoleLightIndex,
  /*vol val:*/ (void*)_vva_prtConsoleLightIndex
},

/********* prtConsoleOnTime 
*/
{
  /*   name:*/ "prtConsoleOnTime",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtConsoleOnTime,
  /*vol val:*/ (void*)_vva_prtConsoleOnTime
},

/********* prtConsoleOffTime 
*/
{
  /*   name:*/ "prtConsoleOffTime",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtConsoleOffTime,
  /*vol val:*/ (void*)_vva_prtConsoleOffTime
},

/********* prtConsoleColor 
*/
{
  /*   name:*/ "prtConsoleColor",
  /*min,max:*/ 1, 9,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 4,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtConsoleColor,
  /*vol val:*/ (void*)_vva_prtConsoleColor
},

/********* prtConsoleDescription 
*/
{
  /*   name:*/ "prtConsoleDescription",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtConsoleDescription
},

/********* prtAlertIndex 
*/
{
  /*   name:*/ "prtAlertIndex",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtAlertIndex,
  /*vol val:*/ (void*)_vva_prtAlertIndex
},

/********* prtAlertSeverityLevel 
*/
{
  /*   name:*/ "prtAlertSeverityLevel",
  /*min,max:*/ 1, 4,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtAlertSeverityLevel,
  /*vol val:*/ (void*)_vva_prtAlertSeverityLevel
},

/********* prtAlertTrainingLevel 
*/
{
  /*   name:*/ "prtAlertTrainingLevel",
  /*min,max:*/ 1, 6,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 3,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtAlertTrainingLevel,
  /*vol val:*/ (void*)_vva_prtAlertTrainingLevel
},

/********* prtAlertGroup 
*/
{
  /*   name:*/ "prtAlertGroup",
  /*min,max:*/ 1, 17,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 5,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtAlertGroup,
  /*vol val:*/ (void*)_vva_prtAlertGroup
},

/********* prtAlertGroupIndex 
*/
{
  /*   name:*/ "prtAlertGroupIndex",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtAlertGroupIndex,
  /*vol val:*/ (void*)_vva_prtAlertGroupIndex
},

/********* prtAlertLocation 
*/
{
  /*   name:*/ "prtAlertLocation",
  /*min,max:*/ 0x80000000UL /* min-int */, 2147483647,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtAlertLocation,
  /*vol val:*/ (void*)_vva_prtAlertLocation
},

/********* prtAlertCode 
*/
{
  /*   name:*/ "prtAlertCode",
  /*min,max:*/ 1, 1507,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 11,
  /*   type:*/ BMIBC_ENUM,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtAlertCode,
  /*vol val:*/ (void*)_vva_prtAlertCode
},

/********* prtAlertDescription 
*/
{
  /*   name:*/ "prtAlertDescription",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_prtAlertDescription
},

/********* prtAlertTime 
*/
{
  /*   name:*/ "prtAlertTime",
  /*min,max:*/ 0UL, 4294967295UL,
  /*    dim:*/ 1,
  /* offset:*/ -1,
  /*encbits:*/ 32,
  /*   type:*/ BMIBC_UINT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READ,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)_fva_prtAlertTime,
  /*vol val:*/ (void*)_vva_prtAlertTime
},

/********* snmpTrapOID 
*/
{
  /*   name:*/ "snmpTrapOID",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE /* TRAP ONLY */,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_snmpTrapOID
},

/********* snmpTrapEnterprise 
*/
{
  /*   name:*/ "snmpTrapEnterprise",
  /*min,max:*/ 0, 255,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 2064,
  /*   type:*/ BMIBC_OCTET,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_NONE /* TRAP ONLY */,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)_vvs_snmpTrapEnterprise
},

/********* snmpSetSerialNo 
*/
{
  /*   name:*/ "snmpSetSerialNo",
  /*min,max:*/ 0, 2147483647,
  /*    dim:*/ 0,
  /* offset:*/ -1,
  /*encbits:*/ 31,
  /*   type:*/ BMIBC_INT32,
  /* method:*/ BMIBC_DIRECT,
  /* access:*/ BMIBC_READWRITE,
  /*subsrib:*/ NULL,
  /*fac val:*/ (void*)NULL,
  /*vol val:*/ (void*)NULL
} 

};

/* lists of values for enumerations
*/
/* enumerated values for IANAifType ------------------------
*/
int32_t ve_IANAifType[197] = {
			1,		/* IANAifType_other	*/
			2,		/* IANAifType_regular1822	*/
			3,		/* IANAifType_hdh1822	*/
			4,		/* IANAifType_ddnX25	*/
			5,		/* IANAifType_rfc877x25	*/
			6,		/* IANAifType_ethernetCsmacd	*/
			7,		/* IANAifType_iso88023Csmacd	*/
			8,		/* IANAifType_iso88024TokenBus	*/
			9,		/* IANAifType_iso88025TokenRing	*/
			10,		/* IANAifType_iso88026Man	*/
			11,		/* IANAifType_starLan	*/
			12,		/* IANAifType_proteon10Mbit	*/
			13,		/* IANAifType_proteon80Mbit	*/
			14,		/* IANAifType_hyperchannel	*/
			15,		/* IANAifType_fddi	*/
			16,		/* IANAifType_lapb	*/
			17,		/* IANAifType_sdlc	*/
			18,		/* IANAifType_ds1	*/
			19,		/* IANAifType_e1	*/
			20,		/* IANAifType_basicISDN	*/
			21,		/* IANAifType_primaryISDN	*/
			22,		/* IANAifType_propPointToPointSerial	*/
			23,		/* IANAifType_ppp	*/
			24,		/* IANAifType_softwareLoopback	*/
			25,		/* IANAifType_eon	*/
			26,		/* IANAifType_ethernet3Mbit	*/
			27,		/* IANAifType_nsip	*/
			28,		/* IANAifType_slip	*/
			29,		/* IANAifType_ultra	*/
			30,		/* IANAifType_ds3	*/
			31,		/* IANAifType_sip	*/
			32,		/* IANAifType_frameRelay	*/
			33,		/* IANAifType_rs232	*/
			34,		/* IANAifType_para	*/
			35,		/* IANAifType_arcnet	*/
			36,		/* IANAifType_arcnetPlus	*/
			37,		/* IANAifType_atm	*/
			38,		/* IANAifType_miox25	*/
			39,		/* IANAifType_sonet	*/
			40,		/* IANAifType_x25ple	*/
			41,		/* IANAifType_iso88022llc	*/
			42,		/* IANAifType_localTalk	*/
			43,		/* IANAifType_smdsDxi	*/
			44,		/* IANAifType_frameRelayService	*/
			45,		/* IANAifType_v35	*/
			46,		/* IANAifType_hssi	*/
			47,		/* IANAifType_hippi	*/
			48,		/* IANAifType_modem	*/
			49,		/* IANAifType_aal5	*/
			50,		/* IANAifType_sonetPath	*/
			51,		/* IANAifType_sonetVT	*/
			52,		/* IANAifType_smdsIcip	*/
			53,		/* IANAifType_propVirtual	*/
			54,		/* IANAifType_propMultiplexor	*/
			55,		/* IANAifType_ieee80212	*/
			56,		/* IANAifType_fibreChannel	*/
			57,		/* IANAifType_hippiInterface	*/
			58,		/* IANAifType_frameRelayInterconnect	*/
			59,		/* IANAifType_aflane8023	*/
			60,		/* IANAifType_aflane8025	*/
			61,		/* IANAifType_cctEmul	*/
			62,		/* IANAifType_fastEther	*/
			63,		/* IANAifType_isdn	*/
			64,		/* IANAifType_v11	*/
			65,		/* IANAifType_v36	*/
			66,		/* IANAifType_g703at64k	*/
			67,		/* IANAifType_g703at2mb	*/
			68,		/* IANAifType_qllc	*/
			69,		/* IANAifType_fastEtherFX	*/
			70,		/* IANAifType_channel	*/
			71,		/* IANAifType_ieee80211	*/
			72,		/* IANAifType_ibm370parChan	*/
			73,		/* IANAifType_escon	*/
			74,		/* IANAifType_dlsw	*/
			75,		/* IANAifType_isdns	*/
			76,		/* IANAifType_isdnu	*/
			77,		/* IANAifType_lapd	*/
			78,		/* IANAifType_ipSwitch	*/
			79,		/* IANAifType_rsrb	*/
			80,		/* IANAifType_atmLogical	*/
			81,		/* IANAifType_ds0	*/
			82,		/* IANAifType_ds0Bundle	*/
			83,		/* IANAifType_bsc	*/
			84,		/* IANAifType_async	*/
			85,		/* IANAifType_cnr	*/
			86,		/* IANAifType_iso88025Dtr	*/
			87,		/* IANAifType_eplrs	*/
			88,		/* IANAifType_arap	*/
			89,		/* IANAifType_propCnls	*/
			90,		/* IANAifType_hostPad	*/
			91,		/* IANAifType_termPad	*/
			92,		/* IANAifType_frameRelayMPI	*/
			93,		/* IANAifType_x213	*/
			94,		/* IANAifType_adsl	*/
			95,		/* IANAifType_radsl	*/
			96,		/* IANAifType_sdsl	*/
			97,		/* IANAifType_vdsl	*/
			98,		/* IANAifType_iso88025CRFPInt	*/
			99,		/* IANAifType_myrinet	*/
			100,		/* IANAifType_voiceEM	*/
			101,		/* IANAifType_voiceFXO	*/
			102,		/* IANAifType_voiceFXS	*/
			103,		/* IANAifType_voiceEncap	*/
			104,		/* IANAifType_voiceOverIp	*/
			105,		/* IANAifType_atmDxi	*/
			106,		/* IANAifType_atmFuni	*/
			107,		/* IANAifType_atmIma	*/
			108,		/* IANAifType_pppMultilinkBundle	*/
			109,		/* IANAifType_ipOverCdlc	*/
			110,		/* IANAifType_ipOverClaw	*/
			111,		/* IANAifType_stackToStack	*/
			112,		/* IANAifType_virtualIpAddress	*/
			113,		/* IANAifType_mpc	*/
			114,		/* IANAifType_ipOverAtm	*/
			115,		/* IANAifType_iso88025Fiber	*/
			116,		/* IANAifType_tdlc	*/
			117,		/* IANAifType_gigabitEthernet	*/
			118,		/* IANAifType_hdlc	*/
			119,		/* IANAifType_lapf	*/
			120,		/* IANAifType_v37	*/
			121,		/* IANAifType_x25mlp	*/
			122,		/* IANAifType_x25huntGroup	*/
			123,		/* IANAifType_trasnpHdlc	*/
			124,		/* IANAifType_interleave	*/
			125,		/* IANAifType_fast	*/
			126,		/* IANAifType_ip	*/
			127,		/* IANAifType_docsCableMaclayer	*/
			128,		/* IANAifType_docsCableDownstream	*/
			129,		/* IANAifType_docsCableUpstream	*/
			130,		/* IANAifType_a12MppSwitch	*/
			131,		/* IANAifType_tunnel	*/
			132,		/* IANAifType_coffee	*/
			133,		/* IANAifType_ces	*/
			134,		/* IANAifType_atmSubInterface	*/
			135,		/* IANAifType_l2vlan	*/
			136,		/* IANAifType_l3ipvlan	*/
			137,		/* IANAifType_l3ipxvlan	*/
			138,		/* IANAifType_digitalPowerline	*/
			139,		/* IANAifType_mediaMailOverIp	*/
			140,		/* IANAifType_dtm	*/
			141,		/* IANAifType_dcn	*/
			142,		/* IANAifType_ipForward	*/
			143,		/* IANAifType_msdsl	*/
			144,		/* IANAifType_ieee1394	*/
			145,		/* IANAifType_if_gsn	*/
			146,		/* IANAifType_dvbRccMacLayer	*/
			147,		/* IANAifType_dvbRccDownstream	*/
			148,		/* IANAifType_dvbRccUpstream	*/
			149,		/* IANAifType_atmVirtual	*/
			150,		/* IANAifType_mplsTunnel	*/
			151,		/* IANAifType_srp	*/
			152,		/* IANAifType_voiceOverAtm	*/
			153,		/* IANAifType_voiceOverFrameRelay	*/
			154,		/* IANAifType_idsl	*/
			155,		/* IANAifType_compositeLink	*/
			156,		/* IANAifType_ss7SigLink	*/
			157,		/* IANAifType_propWirelessP2P	*/
			158,		/* IANAifType_frForward	*/
			159,		/* IANAifType_rfc1483	*/
			160,		/* IANAifType_usb	*/
			161,		/* IANAifType_ieee8023adLag	*/
			162,		/* IANAifType_bgppolicyaccounting	*/
			163,		/* IANAifType_frf16MfrBundle	*/
			164,		/* IANAifType_h323Gatekeeper	*/
			165,		/* IANAifType_h323Proxy	*/
			166,		/* IANAifType_mpls	*/
			167,		/* IANAifType_mfSigLink	*/
			168,		/* IANAifType_hdsl2	*/
			169,		/* IANAifType_shdsl	*/
			170,		/* IANAifType_ds1FDL	*/
			171,		/* IANAifType_pos	*/
			172,		/* IANAifType_dvbAsiIn	*/
			173,		/* IANAifType_dvbAsiOut	*/
			174,		/* IANAifType_plc	*/
			175,		/* IANAifType_nfas	*/
			176,		/* IANAifType_tr008	*/
			177,		/* IANAifType_gr303RDT	*/
			178,		/* IANAifType_gr303IDT	*/
			179,		/* IANAifType_isup	*/
			180,		/* IANAifType_propDocsWirelessMaclayer	*/
			181,		/* IANAifType_propDocsWirelessDownstream	*/
			182,		/* IANAifType_propDocsWirelessUpstream	*/
			183,		/* IANAifType_hiperlan2	*/
			184,		/* IANAifType_propBWAp2Mp	*/
			185,		/* IANAifType_sonetOverheadChannel	*/
			186,		/* IANAifType_digitalWrapperOverheadChannel	*/
			187,		/* IANAifType_aal2	*/
			188,		/* IANAifType_radioMAC	*/
			189,		/* IANAifType_atmRadio	*/
			190,		/* IANAifType_imt	*/
			191,		/* IANAifType_mvl	*/
			192,		/* IANAifType_reachDSL	*/
			193,		/* IANAifType_frDlciEndPt	*/
			194,		/* IANAifType_atmVciEndPt	*/
			195,		/* IANAifType_opticalChannel	*/
			196,		/* IANAifType_opticalTransport	*/
			197 		/* IANAifType_propAtm	*/
};

/* enumerated values for ifAdminStatus ---------------------
*/
int32_t ve_ifAdminStatus[3] = {
			1,		/* ifAdminStatus_up	*/
			2,		/* ifAdminStatus_down	*/
			3 		/* ifAdminStatus_testing	*/
};

/* enumerated values for ifOperStatus ----------------------
*/
int32_t ve_ifOperStatus[7] = {
			1,		/* ifOperStatus_up	*/
			2,		/* ifOperStatus_down	*/
			3,		/* ifOperStatus_testing	*/
			4,		/* ifOperStatus_unknown	*/
			5,		/* ifOperStatus_dormant	*/
			6,		/* ifOperStatus_notPresent	*/
			7 		/* ifOperStatus_lowerLayerDown	*/
};

/* enumerated values for snmpEnableAuthenTraps -------------
*/
int32_t ve_snmpEnableAuthenTraps[2] = {
			1,		/* snmpEnableAuthenTraps_enabled	*/
			2 		/* snmpEnableAuthenTraps_disabled	*/
};

/* enumerated values for hrDeviceStatus --------------------
*/
int32_t ve_hrDeviceStatus[5] = {
			1,		/* hrDeviceStatus_unknown	*/
			2,		/* hrDeviceStatus_running	*/
			3,		/* hrDeviceStatus_warning	*/
			4,		/* hrDeviceStatus_testing	*/
			5 		/* hrDeviceStatus_down	*/
};

/* enumerated values for hrPrinterStatus -------------------
*/
int32_t ve_hrPrinterStatus[5] = {
			1,		/* hrPrinterStatus_other	*/
			2,		/* hrPrinterStatus_unknown	*/
			3,		/* hrPrinterStatus_idle	*/
			4,		/* hrPrinterStatus_printing	*/
			5 		/* hrPrinterStatus_warmup	*/
};

/* enumerated values for hrDiskStorageAccess ---------------
*/
int32_t ve_hrDiskStorageAccess[2] = {
			1,		/* hrDiskStorageAccess_readWrite	*/
			2 		/* hrDiskStorageAccess_readOnly	*/
};

/* enumerated values for hrDiskStorageMedia ----------------
*/
int32_t ve_hrDiskStorageMedia[8] = {
			1,		/* hrDiskStorageMedia_other	*/
			2,		/* hrDiskStorageMedia_unknown	*/
			3,		/* hrDiskStorageMedia_hardDisk	*/
			4,		/* hrDiskStorageMedia_floppyDisk	*/
			5,		/* hrDiskStorageMedia_opticalDiskROM	*/
			6,		/* hrDiskStorageMedia_opticalDiskWORM	*/
			7,		/* hrDiskStorageMedia_opticalDiskRW	*/
			8 		/* hrDiskStorageMedia_ramDisk	*/
};

/* enumerated values for TruthValue ------------------------
*/
int32_t ve_TruthValue[2] = {
			1,		/* TruthValue_true	*/
			2 		/* TruthValue_false	*/
};

/* enumerated values for hrFSAccess ------------------------
*/
int32_t ve_hrFSAccess[2] = {
			1,		/* hrFSAccess_readWrite	*/
			2 		/* hrFSAccess_readOnly	*/
};

/* enumerated values for hrSWRunType -----------------------
*/
int32_t ve_hrSWRunType[4] = {
			1,		/* hrSWRunType_unknown	*/
			2,		/* hrSWRunType_operatingSystem	*/
			3,		/* hrSWRunType_deviceDriver	*/
			4 		/* hrSWRunType_application	*/
};

/* enumerated values for hrSWRunStatus ---------------------
*/
int32_t ve_hrSWRunStatus[4] = {
			1,		/* hrSWRunStatus_running	*/
			2,		/* hrSWRunStatus_runnable	*/
			3,		/* hrSWRunStatus_notRunnable	*/
			4 		/* hrSWRunStatus_invalid	*/
};

/* enumerated values for hrSWInstalledType -----------------
*/
int32_t ve_hrSWInstalledType[4] = {
			1,		/* hrSWInstalledType_unknown	*/
			2,		/* hrSWInstalledType_operatingSystem	*/
			3,		/* hrSWInstalledType_deviceDriver	*/
			4 		/* hrSWInstalledType_application	*/
};

/* enumerated values for ifLinkUpDownTrapEnable ------------
*/
int32_t ve_ifLinkUpDownTrapEnable[2] = {
			1,		/* ifLinkUpDownTrapEnable_enabled	*/
			2 		/* ifLinkUpDownTrapEnable_disabled	*/
};

/* enumerated values for RowStatus -------------------------
*/
int32_t ve_RowStatus[6] = {
			1,		/* RowStatus_active	*/
			2,		/* RowStatus_notInService	*/
			3,		/* RowStatus_notReady	*/
			4,		/* RowStatus_createAndGo	*/
			5,		/* RowStatus_createAndWait	*/
			6 		/* RowStatus_destroy	*/
};

/* enumerated values for prtGeneralReset -------------------
*/
int32_t ve_prtGeneralReset[4] = {
			3,		/* prtGeneralReset_notResetting	*/
			4,		/* prtGeneralReset_powerCycleReset	*/
			5,		/* prtGeneralReset_resetToNVRAM	*/
			6 		/* prtGeneralReset_resetToFactoryDefaults	*/
};

/* enumerated values for prtConsoleDisable -----------------
*/
int32_t ve_prtConsoleDisable[2] = {
			3,		/* prtConsoleDisable_enabled	*/
			4 		/* prtConsoleDisable_disabled	*/
};

/* enumerated values for prtCoverStatus --------------------
*/
int32_t ve_prtCoverStatus[5] = {
			1,		/* prtCoverStatus_other	*/
			3,		/* prtCoverStatus_doorOpen	*/
			4,		/* prtCoverStatus_doorClosed	*/
			5,		/* prtCoverStatus_interlockOpen	*/
			6 		/* prtCoverStatus_interlockClosed	*/
};

/* enumerated values for CodedCharSet ----------------------
*/
int32_t ve_CodedCharSet[1] = {
			1 		/* CodedCharSet_other	*/
};

/* enumerated values for prtInputType ----------------------
*/
int32_t ve_prtInputType[7] = {
			1,		/* prtInputType_other	*/
			2,		/* prtInputType_unknown	*/
			3,		/* prtInputType_sheetFeedAutoRemovableTray	*/
			4,		/* prtInputType_sheetFeedAutoNonRemovableTray	*/
			5,		/* prtInputType_sheetFeedManual	*/
			6,		/* prtInputType_continuousRoll	*/
			7 		/* prtInputType_continuousFanFold	*/
};

/* enumerated values for MediaUnit -------------------------
*/
int32_t ve_MediaUnit[2] = {
			3,		/* MediaUnit_tenThousandthsOfInches	*/
			4 		/* MediaUnit_micrometers	*/
};

/* enumerated values for CapacityUnit ----------------------
*/
int32_t ve_CapacityUnit[5] = {
			3,		/* CapacityUnit_tenThousandthsOfInches	*/
			4,		/* CapacityUnit_micrometers	*/
			8,		/* CapacityUnit_sheets	*/
			16,		/* CapacityUnit_feet	*/
			17 		/* CapacityUnit_meters	*/
};

/* enumerated values for PresentOnOff ----------------------
*/
int32_t ve_PresentOnOff[4] = {
			1,		/* PresentOnOff_other	*/
			3,		/* PresentOnOff_on	*/
			4,		/* PresentOnOff_off	*/
			5 		/* PresentOnOff_notPresent	*/
};

/* enumerated values for prtOutputType ---------------------
*/
int32_t ve_prtOutputType[7] = {
			1,		/* prtOutputType_other	*/
			2,		/* prtOutputType_unknown	*/
			3,		/* prtOutputType_removableBin	*/
			4,		/* prtOutputType_unRemovableBin	*/
			5,		/* prtOutputType_continuousRollDevice	*/
			6,		/* prtOutputType_mailBox	*/
			7 		/* prtOutputType_continuousFanFold	*/
};

/* enumerated values for prtOutputStackingOrder ------------
*/
int32_t ve_prtOutputStackingOrder[3] = {
			2,		/* prtOutputStackingOrder_unknown	*/
			3,		/* prtOutputStackingOrder_firstToLast	*/
			4 		/* prtOutputStackingOrder_lastToFirst	*/
};

/* enumerated values for prtOutputPageDeliveryOrientation --
*/
int32_t ve_prtOutputPageDeliveryOrientation[2] = {
			3,		/* prtOutputPageDeliveryOrientation_faceUp	*/
			4 		/* prtOutputPageDeliveryOrientation_faceDown	*/
};

/* enumerated values for prtMarkerMarkTech -----------------
*/
int32_t ve_prtMarkerMarkTech[27] = {
			1,		/* prtMarkerMarkTech_other	*/
			2,		/* prtMarkerMarkTech_unknown	*/
			3,		/* prtMarkerMarkTech_electrophotographicLED	*/
			4,		/* prtMarkerMarkTech_electrophotographicLaser	*/
			5,		/* prtMarkerMarkTech_electrophotographicOther	*/
			6,		/* prtMarkerMarkTech_impactMovingHeadDotMatrix9pin	*/
			7,		/* prtMarkerMarkTech_impactMovingHeadDotMatrix24pin	*/
			8,		/* prtMarkerMarkTech_impactMovingHeadDotMatrixOther	*/
			9,		/* prtMarkerMarkTech_impactMovingHeadFullyFormed	*/
			10,		/* prtMarkerMarkTech_impactBand	*/
			11,		/* prtMarkerMarkTech_impactOther	*/
			12,		/* prtMarkerMarkTech_inkjetAqueous	*/
			13,		/* prtMarkerMarkTech_inkjetSolid	*/
			14,		/* prtMarkerMarkTech_inkjetOther	*/
			15,		/* prtMarkerMarkTech_pen	*/
			16,		/* prtMarkerMarkTech_thermalTransfer	*/
			17,		/* prtMarkerMarkTech_thermalSensitive	*/
			18,		/* prtMarkerMarkTech_thermalDiffusion	*/
			19,		/* prtMarkerMarkTech_thermalOther	*/
			20,		/* prtMarkerMarkTech_electroerosion	*/
			21,		/* prtMarkerMarkTech_electrostatic	*/
			22,		/* prtMarkerMarkTech_photographicMicrofiche	*/
			23,		/* prtMarkerMarkTech_photographicImagesetter	*/
			24,		/* prtMarkerMarkTech_photographicOther	*/
			25,		/* prtMarkerMarkTech_ionDeposition	*/
			26,		/* prtMarkerMarkTech_eBeam	*/
			27 		/* prtMarkerMarkTech_typesetter	*/
};

/* enumerated values for prtMarkerCounterUnit --------------
*/
int32_t ve_prtMarkerCounterUnit[10] = {
			3,		/* prtMarkerCounterUnit_tenThousandthsOfInches	*/
			4,		/* prtMarkerCounterUnit_micrometers	*/
			5,		/* prtMarkerCounterUnit_characters	*/
			6,		/* prtMarkerCounterUnit_lines	*/
			7,		/* prtMarkerCounterUnit_impressions	*/
			8,		/* prtMarkerCounterUnit_sheets	*/
			9,		/* prtMarkerCounterUnit_dotRow	*/
			11,		/* prtMarkerCounterUnit_hours	*/
			16,		/* prtMarkerCounterUnit_feet	*/
			17 		/* prtMarkerCounterUnit_meters	*/
};

/* enumerated values for prtMarkerAddressabilityUnit -------
*/
int32_t ve_prtMarkerAddressabilityUnit[2] = {
			3,		/* prtMarkerAddressabilityUnit_tenThousandthsOfInches	*/
			4 		/* prtMarkerAddressabilityUnit_micrometers	*/
};

/* enumerated values for prtMarkerSuppliesClass ------------
*/
int32_t ve_prtMarkerSuppliesClass[3] = {
			1,		/* prtMarkerSuppliesClass_other	*/
			3,		/* prtMarkerSuppliesClass_supplyThatIsConsumed	*/
			4 		/* prtMarkerSuppliesClass_receptacleThatIsFilled	*/
};

/* enumerated values for prtMarkerSuppliesType -------------
*/
int32_t ve_prtMarkerSuppliesType[14] = {
			1,		/* prtMarkerSuppliesType_other	*/
			2,		/* prtMarkerSuppliesType_unknown	*/
			3,		/* prtMarkerSuppliesType_toner	*/
			4,		/* prtMarkerSuppliesType_wasteToner	*/
			5,		/* prtMarkerSuppliesType_ink	*/
			6,		/* prtMarkerSuppliesType_inkCartridge	*/
			7,		/* prtMarkerSuppliesType_inkRibbon	*/
			8,		/* prtMarkerSuppliesType_wasteInk	*/
			9,		/* prtMarkerSuppliesType_opc	*/
			10,		/* prtMarkerSuppliesType_developer	*/
			11,		/* prtMarkerSuppliesType_fuserOil	*/
			12,		/* prtMarkerSuppliesType_solidWax	*/
			13,		/* prtMarkerSuppliesType_ribbonWax	*/
			14 		/* prtMarkerSuppliesType_wasteWax	*/
};

/* enumerated values for prtMarkerSuppliesSupplyUnit -------
*/
int32_t ve_prtMarkerSuppliesSupplyUnit[6] = {
			3,		/* prtMarkerSuppliesSupplyUnit_tenThousandthsOfInches	*/
			4,		/* prtMarkerSuppliesSupplyUnit_micrometers	*/
			12,		/* prtMarkerSuppliesSupplyUnit_thousandthsOfOunces	*/
			13,		/* prtMarkerSuppliesSupplyUnit_tenthsOfGrams	*/
			14,		/* prtMarkerSuppliesSupplyUnit_hundrethsOfFluidOunces	*/
			15 		/* prtMarkerSuppliesSupplyUnit_tenthsOfMilliliters	*/
};

/* enumerated values for prtMarkerColorantRole -------------
*/
int32_t ve_prtMarkerColorantRole[3] = {
			1,		/* prtMarkerColorantRole_other	*/
			3,		/* prtMarkerColorantRole_process	*/
			4 		/* prtMarkerColorantRole_spot	*/
};

/* enumerated values for prtMediaPathMaxSpeedPrintUnit -----
*/
int32_t ve_prtMediaPathMaxSpeedPrintUnit[9] = {
			3,		/* prtMediaPathMaxSpeedPrintUnit_tenThousandthsOfInchesPerHour	*/
			4,		/* prtMediaPathMaxSpeedPrintUnit_micrometersPerHour	*/
			5,		/* prtMediaPathMaxSpeedPrintUnit_charactersPerHour	*/
			6,		/* prtMediaPathMaxSpeedPrintUnit_linesPerHour	*/
			7,		/* prtMediaPathMaxSpeedPrintUnit_impressionsPerHour	*/
			8,		/* prtMediaPathMaxSpeedPrintUnit_sheetsPerHour	*/
			9,		/* prtMediaPathMaxSpeedPrintUnit_dotRowPerHour	*/
			16,		/* prtMediaPathMaxSpeedPrintUnit_feetPerHour	*/
			17 		/* prtMediaPathMaxSpeedPrintUnit_metersPerHour	*/
};

/* enumerated values for prtMediaPathType ------------------
*/
int32_t ve_prtMediaPathType[5] = {
			1,		/* prtMediaPathType_other	*/
			2,		/* prtMediaPathType_unknown	*/
			3,		/* prtMediaPathType_longEdgeBindingDuplex	*/
			4,		/* prtMediaPathType_shortEdgeBindingDuplex	*/
			5 		/* prtMediaPathType_simplex	*/
};

/* enumerated values for prtChannelType --------------------
*/
int32_t ve_prtChannelType[32] = {
			1,		/* prtChannelType_other	*/
			3,		/* prtChannelType_chSerialPort	*/
			4,		/* prtChannelType_chParallelPort	*/
			5,		/* prtChannelType_chIEEE1284Port	*/
			6,		/* prtChannelType_chSCSIPort	*/
			7,		/* prtChannelType_chAppleTalkPAP	*/
			8,		/* prtChannelType_chLPDServer	*/
			9,		/* prtChannelType_chNetwareRPrinter	*/
			10,		/* prtChannelType_chNetwarePServer	*/
			11,		/* prtChannelType_chPort9100	*/
			12,		/* prtChannelType_chAppSocket	*/
			13,		/* prtChannelType_chFTP	*/
			14,		/* prtChannelType_chTFTP	*/
			15,		/* prtChannelType_chDLCLLCPort	*/
			16,		/* prtChannelType_chIBM3270	*/
			17,		/* prtChannelType_chIBM5250	*/
			18,		/* prtChannelType_chFax	*/
			19,		/* prtChannelType_chIEEE1394	*/
			20,		/* prtChannelType_chTransport1	*/
			21,		/* prtChannelType_chCPAP	*/
			22,		/* prtChannelType_chDCERemoteProcCall	*/
			23,		/* prtChannelType_chONCRemoteProcCall	*/
			24,		/* prtChannelType_chOLE	*/
			25,		/* prtChannelType_chNamedPipe	*/
			26,		/* prtChannelType_chPCPrint	*/
			27,		/* prtChannelType_chServerMessageBlock	*/
			28,		/* prtChannelType_chDPMF	*/
			29,		/* prtChannelType_chDLLAPI	*/
			30,		/* prtChannelType_chVxDAPI	*/
			31,		/* prtChannelType_chSystemObjectManager	*/
			32,		/* prtChannelType_chDECLAT	*/
			33 		/* prtChannelType_chNPAP	*/
};

/* enumerated values for prtChannelState -------------------
*/
int32_t ve_prtChannelState[3] = {
			1,		/* prtChannelState_other	*/
			3,		/* prtChannelState_printDataAccepted	*/
			4 		/* prtChannelState_noDataAccepted	*/
};

/* enumerated values for prtInterpreterLangFamily ----------
*/
int32_t ve_prtInterpreterLangFamily[45] = {
			1,		/* prtInterpreterLangFamily_other	*/
			3,		/* prtInterpreterLangFamily_langPCL	*/
			4,		/* prtInterpreterLangFamily_langHPGL	*/
			5,		/* prtInterpreterLangFamily_langPJL	*/
			6,		/* prtInterpreterLangFamily_langPS	*/
			42,		/* prtInterpreterLangFamily_langPSPrinter	*/
			7,		/* prtInterpreterLangFamily_langIPDS	*/
			8,		/* prtInterpreterLangFamily_langPPDS	*/
			9,		/* prtInterpreterLangFamily_langEscapeP	*/
			10,		/* prtInterpreterLangFamily_langEpson	*/
			11,		/* prtInterpreterLangFamily_langDDIF	*/
			12,		/* prtInterpreterLangFamily_langInterpress	*/
			13,		/* prtInterpreterLangFamily_langISO6429	*/
			14,		/* prtInterpreterLangFamily_langLineData	*/
			15,		/* prtInterpreterLangFamily_langMODCA	*/
			16,		/* prtInterpreterLangFamily_langREGIS	*/
			17,		/* prtInterpreterLangFamily_langSCS	*/
			18,		/* prtInterpreterLangFamily_langSPDL	*/
			19,		/* prtInterpreterLangFamily_langTEK4014	*/
			20,		/* prtInterpreterLangFamily_langPDS	*/
			21,		/* prtInterpreterLangFamily_langIGP	*/
			22,		/* prtInterpreterLangFamily_langCodeV	*/
			23,		/* prtInterpreterLangFamily_langDSCDSE	*/
			24,		/* prtInterpreterLangFamily_langWPS	*/
			25,		/* prtInterpreterLangFamily_langLN03	*/
			26,		/* prtInterpreterLangFamily_langCCITT	*/
			27,		/* prtInterpreterLangFamily_langQUIC	*/
			28,		/* prtInterpreterLangFamily_langCPAP	*/
			29,		/* prtInterpreterLangFamily_langDecPPL	*/
			30,		/* prtInterpreterLangFamily_langSimpleText	*/
			31,		/* prtInterpreterLangFamily_langNPAP	*/
			32,		/* prtInterpreterLangFamily_langDOC	*/
			33,		/* prtInterpreterLangFamily_langimPress	*/
			34,		/* prtInterpreterLangFamily_langPinwriter	*/
			35,		/* prtInterpreterLangFamily_langNPDL	*/
			36,		/* prtInterpreterLangFamily_langNEC201PL	*/
			37,		/* prtInterpreterLangFamily_langAutomatic	*/
			38,		/* prtInterpreterLangFamily_langPages	*/
			39,		/* prtInterpreterLangFamily_langLIPS	*/
			40,		/* prtInterpreterLangFamily_langTIFF	*/
			41,		/* prtInterpreterLangFamily_langDiagnostic	*/
			43,		/* prtInterpreterLangFamily_langCaPSL	*/
			44,		/* prtInterpreterLangFamily_langEXCL	*/
			45,		/* prtInterpreterLangFamily_langLCDS	*/
			46 		/* prtInterpreterLangFamily_langXES	*/
};

/* enumerated values for prtInterpreterDefaultOrientation --
*/
int32_t ve_prtInterpreterDefaultOrientation[3] = {
			1,		/* prtInterpreterDefaultOrientation_other	*/
			3,		/* prtInterpreterDefaultOrientation_portrait	*/
			4 		/* prtInterpreterDefaultOrientation_landscape	*/
};

/* enumerated values for prtInterpreterTwoWay --------------
*/
int32_t ve_prtInterpreterTwoWay[2] = {
			3,		/* prtInterpreterTwoWay_yes	*/
			4 		/* prtInterpreterTwoWay_no	*/
};

/* enumerated values for prtConsoleColor -------------------
*/
int32_t ve_prtConsoleColor[9] = {
			1,		/* prtConsoleColor_other	*/
			2,		/* prtConsoleColor_unknown	*/
			3,		/* prtConsoleColor_white	*/
			4,		/* prtConsoleColor_red	*/
			5,		/* prtConsoleColor_green	*/
			6,		/* prtConsoleColor_blue	*/
			7,		/* prtConsoleColor_cyan	*/
			8,		/* prtConsoleColor_magenta	*/
			9 		/* prtConsoleColor_yellow	*/
};

/* enumerated values for prtAlertSeverityLevel -------------
*/
int32_t ve_prtAlertSeverityLevel[3] = {
			1,		/* prtAlertSeverityLevel_other	*/
			3,		/* prtAlertSeverityLevel_critical	*/
			4 		/* prtAlertSeverityLevel_warning	*/
};

/* enumerated values for prtAlertTrainingLevel -------------
*/
int32_t ve_prtAlertTrainingLevel[6] = {
			1,		/* prtAlertTrainingLevel_other	*/
			2,		/* prtAlertTrainingLevel_unknown	*/
			3,		/* prtAlertTrainingLevel_untrained	*/
			4,		/* prtAlertTrainingLevel_trained	*/
			5,		/* prtAlertTrainingLevel_fieldService	*/
			6 		/* prtAlertTrainingLevel_management	*/
};

/* enumerated values for prtAlertGroup ---------------------
*/
int32_t ve_prtAlertGroup[16] = {
			1,		/* prtAlertGroup_other	*/
			3,		/* prtAlertGroup_hostResourcesMIBStorageTable	*/
			4,		/* prtAlertGroup_hostResourcesMIBDeviceTable	*/
			5,		/* prtAlertGroup_generalPrinter	*/
			6,		/* prtAlertGroup_cover	*/
			7,		/* prtAlertGroup_localization	*/
			8,		/* prtAlertGroup_input	*/
			9,		/* prtAlertGroup_output	*/
			10,		/* prtAlertGroup_marker	*/
			11,		/* prtAlertGroup_markerSupplies	*/
			12,		/* prtAlertGroup_markerColorant	*/
			13,		/* prtAlertGroup_mediaPath	*/
			14,		/* prtAlertGroup_channel	*/
			15,		/* prtAlertGroup_interpreter	*/
			16,		/* prtAlertGroup_consoleDisplayBuffer	*/
			17 		/* prtAlertGroup_consoleLights	*/
};

/* enumerated values for prtAlertCode ----------------------
*/
int32_t ve_prtAlertCode[49] = {
			1,		/* prtAlertCode_other	*/
			2,		/* prtAlertCode_unknown	*/
			3,		/* prtAlertCode_coverOpen	*/
			4,		/* prtAlertCode_coverClosed	*/
			5,		/* prtAlertCode_interlockOpen	*/
			6,		/* prtAlertCode_interlockClosed	*/
			7,		/* prtAlertCode_configurationChange	*/
			8,		/* prtAlertCode_jam	*/
			501,		/* prtAlertCode_doorOpen	*/
			502,		/* prtAlertCode_doorClosed	*/
			503,		/* prtAlertCode_powerUp	*/
			504,		/* prtAlertCode_powerDown	*/
			801,		/* prtAlertCode_inputMediaTrayMissing	*/
			802,		/* prtAlertCode_inputMediaSizeChange	*/
			803,		/* prtAlertCode_inputMediaWeightChange	*/
			804,		/* prtAlertCode_inputMediaTypeChange	*/
			805,		/* prtAlertCode_inputMediaColorChange	*/
			806,		/* prtAlertCode_inputMediaFormPartsChange	*/
			807,		/* prtAlertCode_inputMediaSupplyLow	*/
			808,		/* prtAlertCode_inputMediaSupplyEmpty	*/
			901,		/* prtAlertCode_outputMediaTrayMissing	*/
			902,		/* prtAlertCode_outputMediaTrayAlmostFull	*/
			903,		/* prtAlertCode_outputMediaTrayFull	*/
			1001,		/* prtAlertCode_markerFuserUnderTemperature	*/
			1002,		/* prtAlertCode_markerFuserOverTemperature	*/
			1101,		/* prtAlertCode_markerTonerEmpty	*/
			1102,		/* prtAlertCode_markerInkEmpty	*/
			1103,		/* prtAlertCode_markerPrintRibbonEmpty	*/
			1104,		/* prtAlertCode_markerTonerAlmostEmpty	*/
			1105,		/* prtAlertCode_markerInkAlmostEmpty	*/
			1106,		/* prtAlertCode_markerPrintRibbonAlmostEmpty	*/
			1107,		/* prtAlertCode_markerWasteTonerReceptacleAlmostFull	*/
			1108,		/* prtAlertCode_markerWasteInkReceptacleAlmostFull	*/
			1109,		/* prtAlertCode_markerWasteTonerReceptacleFull	*/
			1110,		/* prtAlertCode_markerWasteInkReceptacleFull	*/
			1111,		/* prtAlertCode_markerOpcLifeAlmostOver	*/
			1112,		/* prtAlertCode_markerOpcLifeOver	*/
			1113,		/* prtAlertCode_markerDeveloperAlmostEmpty	*/
			1114,		/* prtAlertCode_markerDeveloperEmpty	*/
			1301,		/* prtAlertCode_mediaPathMediaTrayMissing	*/
			1302,		/* prtAlertCode_mediaPathMediaTrayAlmostFull	*/
			1303,		/* prtAlertCode_mediaPathMediaTrayFull	*/
			1501,		/* prtAlertCode_interpreterMemoryIncrease	*/
			1502,		/* prtAlertCode_interpreterMemoryDecrease	*/
			1503,		/* prtAlertCode_interpreterCartridgeAdded	*/
			1504,		/* prtAlertCode_interpreterCartridgeDeleted	*/
			1505,		/* prtAlertCode_interpreterResourceAdded	*/
			1506,		/* prtAlertCode_interpreterResourceDeleted	*/
			1507 		/* prtAlertCode_interpreterResourceUnavailable	*/
};



/* pointers to enumeration values array
*/
const bmib_enumvals_t g_mib_enums[BMIBC_NUM_RECORDS] = {
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_IANAifType, ve_IANAifType },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_ifAdminStatus, ve_ifAdminStatus },
			{ ne_ifOperStatus, ve_ifOperStatus },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_snmpEnableAuthenTraps, ve_snmpEnableAuthenTraps },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_hrDeviceStatus, ve_hrDeviceStatus },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_hrPrinterStatus, ve_hrPrinterStatus },
			{ 0, NULL },
			{ ne_hrDiskStorageAccess, ve_hrDiskStorageAccess },
			{ ne_hrDiskStorageMedia, ve_hrDiskStorageMedia },
			{ ne_TruthValue, ve_TruthValue },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_hrFSAccess, ve_hrFSAccess },
			{ ne_TruthValue, ve_TruthValue },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_hrSWRunType, ve_hrSWRunType },
			{ ne_hrSWRunStatus, ve_hrSWRunStatus },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_hrSWInstalledType, ve_hrSWInstalledType },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_ifLinkUpDownTrapEnable, ve_ifLinkUpDownTrapEnable },
			{ 0, NULL },
			{ ne_TruthValue, ve_TruthValue },
			{ ne_TruthValue, ve_TruthValue },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_RowStatus, ve_RowStatus },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtGeneralReset, ve_prtGeneralReset },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtConsoleDisable, ve_prtConsoleDisable },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtCoverStatus, ve_prtCoverStatus },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_CodedCharSet, ve_CodedCharSet },
			{ 0, NULL },
			{ ne_prtInputType, ve_prtInputType },
			{ ne_MediaUnit, ve_MediaUnit },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_CapacityUnit, ve_CapacityUnit },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_PresentOnOff, ve_PresentOnOff },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtOutputType, ve_prtOutputType },
			{ ne_CapacityUnit, ve_CapacityUnit },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_PresentOnOff, ve_PresentOnOff },
			{ ne_MediaUnit, ve_MediaUnit },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtOutputStackingOrder, ve_prtOutputStackingOrder },
			{ ne_prtOutputPageDeliveryOrientation, ve_prtOutputPageDeliveryOrientation },
			{ ne_PresentOnOff, ve_PresentOnOff },
			{ ne_PresentOnOff, ve_PresentOnOff },
			{ ne_PresentOnOff, ve_PresentOnOff },
			{ ne_PresentOnOff, ve_PresentOnOff },
			{ 0, NULL },
			{ ne_prtMarkerMarkTech, ve_prtMarkerMarkTech },
			{ ne_prtMarkerCounterUnit, ve_prtMarkerCounterUnit },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtMarkerAddressabilityUnit, ve_prtMarkerAddressabilityUnit },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtMarkerSuppliesClass, ve_prtMarkerSuppliesClass },
			{ ne_prtMarkerSuppliesType, ve_prtMarkerSuppliesType },
			{ 0, NULL },
			{ ne_prtMarkerSuppliesSupplyUnit, ve_prtMarkerSuppliesSupplyUnit },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtMarkerColorantRole, ve_prtMarkerColorantRole },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtMediaPathMaxSpeedPrintUnit, ve_prtMediaPathMaxSpeedPrintUnit },
			{ ne_MediaUnit, ve_MediaUnit },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtMediaPathType, ve_prtMediaPathType },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtChannelType, ve_prtChannelType },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtChannelState, ve_prtChannelState },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtInterpreterLangFamily, ve_prtInterpreterLangFamily },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtInterpreterDefaultOrientation, ve_prtInterpreterDefaultOrientation },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_CodedCharSet, ve_CodedCharSet },
			{ ne_CodedCharSet, ve_CodedCharSet },
			{ ne_prtInterpreterTwoWay, ve_prtInterpreterTwoWay },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtConsoleColor, ve_prtConsoleColor },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtAlertSeverityLevel, ve_prtAlertSeverityLevel },
			{ ne_prtAlertTrainingLevel, ve_prtAlertTrainingLevel },
			{ ne_prtAlertGroup, ve_prtAlertGroup },
			{ 0, NULL },
			{ 0, NULL },
			{ ne_prtAlertCode, ve_prtAlertCode },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL },
			{ 0, NULL } 
};


/* Storage for multi-dim properties or properties which need more than 32 bits
*/
/* storage for string value(s) for NVRAMSIGNATURE
*/
uint8_t _vvs_NVRAMSIGNATURE[33];
const uint8_t _fvs_NVRAMSIGNATURE[/*max of 33*/] = {
        "\x20NV-Thu Mar 14 17:44:44 2019     "
};

/* storage for string value(s) for sysDescr
*/
uint8_t _vvs_sysDescr[256];


/* storage for string value(s) for sysObjectID
*/
uint8_t _vvs_sysObjectID[256];


/* storage for string value(s) for sysContact
*/
uint8_t _vvs_sysContact[256];


/* storage for string value(s) for sysName
*/
uint8_t _vvs_sysName[256];


/* storage for string value(s) for sysLocation
*/
uint8_t _vvs_sysLocation[256];


/* storage for multi-dimensional property sysORIndex
*/
      int32_t _vva_sysORIndex[1];
const int32_t _fva_sysORIndex[1] = { 1 };

/* storage for string value(s) for sysORID
*/
uint8_t _vvs_sysORID[256];


/* storage for string value(s) for sysORDescr
*/
uint8_t _vvs_sysORDescr[256];


/* storage for multi-dimensional property sysORUpTime
*/
      uint32_t _vva_sysORUpTime[1];
const uint32_t _fva_sysORUpTime[1] = { };

/* storage for multi-dimensional property ifIndex
*/
      int32_t _vva_ifIndex[1];
const int32_t _fva_ifIndex[1] = { 1 };

/* storage for string value(s) for ifDescr
*/
uint8_t _vvs_ifDescr[256];


/* storage for multi-dimensional property ifType
*/
      int32_t _vva_ifType[1];
const int32_t _fva_ifType[1] = { };

/* storage for multi-dimensional property ifMtu
*/
      int32_t _vva_ifMtu[1];
const int32_t _fva_ifMtu[1] = { };

/* storage for multi-dimensional property ifSpeed
*/
      uint32_t _vva_ifSpeed[1];
const uint32_t _fva_ifSpeed[1] = { };

/* storage for string value(s) for ifPhysAddress
*/
uint8_t _vvs_ifPhysAddress[256];


/* storage for multi-dimensional property ifAdminStatus
*/
      int32_t _vva_ifAdminStatus[1];
const int32_t _fva_ifAdminStatus[1] = { };

/* storage for multi-dimensional property ifOperStatus
*/
      int32_t _vva_ifOperStatus[1];
const int32_t _fva_ifOperStatus[1] = { };

/* storage for multi-dimensional property ifLastChange
*/
      uint32_t _vva_ifLastChange[1];
const uint32_t _fva_ifLastChange[1] = { };

/* storage for multi-dimensional property ifInOctets
*/
      uint32_t _vva_ifInOctets[1];
const uint32_t _fva_ifInOctets[1] = { };

/* storage for multi-dimensional property ifInUcastPkts
*/
      uint32_t _vva_ifInUcastPkts[1];
const uint32_t _fva_ifInUcastPkts[1] = { };

/* storage for multi-dimensional property ifInNUcastPkts
*/
      uint32_t _vva_ifInNUcastPkts[1];
const uint32_t _fva_ifInNUcastPkts[1] = { };

/* storage for multi-dimensional property ifInDiscards
*/
      uint32_t _vva_ifInDiscards[1];
const uint32_t _fva_ifInDiscards[1] = { };

/* storage for multi-dimensional property ifInErrors
*/
      uint32_t _vva_ifInErrors[1];
const uint32_t _fva_ifInErrors[1] = { };

/* storage for multi-dimensional property ifInUnknownProtos
*/
      uint32_t _vva_ifInUnknownProtos[1];
const uint32_t _fva_ifInUnknownProtos[1] = { };

/* storage for multi-dimensional property ifOutOctets
*/
      uint32_t _vva_ifOutOctets[1];
const uint32_t _fva_ifOutOctets[1] = { };

/* storage for multi-dimensional property ifOutUcastPkts
*/
      uint32_t _vva_ifOutUcastPkts[1];
const uint32_t _fva_ifOutUcastPkts[1] = { };

/* storage for multi-dimensional property ifOutNUcastPkts
*/
      uint32_t _vva_ifOutNUcastPkts[1];
const uint32_t _fva_ifOutNUcastPkts[1] = { };

/* storage for multi-dimensional property ifOutDiscards
*/
      uint32_t _vva_ifOutDiscards[1];
const uint32_t _fva_ifOutDiscards[1] = { };

/* storage for multi-dimensional property ifOutErrors
*/
      uint32_t _vva_ifOutErrors[1];
const uint32_t _fva_ifOutErrors[1] = { };

/* storage for multi-dimensional property ifOutQLen
*/
      uint32_t _vva_ifOutQLen[1];
const uint32_t _fva_ifOutQLen[1] = { };

/* storage for string value(s) for ifSpecific
*/
uint8_t _vvs_ifSpecific[256];


/* storage for string value(s) for hrSystemDate
*/
uint8_t _vvs_hrSystemDate[12];


/* storage for string value(s) for hrSystemInitialLoadParameters
*/
uint8_t _vvs_hrSystemInitialLoadParameters[129];


/* storage for multi-dimensional property hrStorageIndex
*/
      int32_t _vva_hrStorageIndex[1];
const int32_t _fva_hrStorageIndex[1] = { 1 };

/* storage for string value(s) for hrStorageType
*/
uint8_t _vvs_hrStorageType[256];


/* storage for string value(s) for hrStorageDescr
*/
uint8_t _vvs_hrStorageDescr[256];


/* storage for multi-dimensional property hrStorageAllocationUnits
*/
      int32_t _vva_hrStorageAllocationUnits[1];
const int32_t _fva_hrStorageAllocationUnits[1] = { };

/* storage for multi-dimensional property hrStorageSize
*/
      int32_t _vva_hrStorageSize[1];
const int32_t _fva_hrStorageSize[1] = { };

/* storage for multi-dimensional property hrStorageUsed
*/
      int32_t _vva_hrStorageUsed[1];
const int32_t _fva_hrStorageUsed[1] = { };

/* storage for multi-dimensional property hrStorageAllocationFailures
*/
      uint32_t _vva_hrStorageAllocationFailures[1];
const uint32_t _fva_hrStorageAllocationFailures[1] = { };

/* storage for multi-dimensional property hrDeviceIndex
*/
      int32_t _vva_hrDeviceIndex[1];
const int32_t _fva_hrDeviceIndex[1] = { 1 };

/* storage for string value(s) for hrDeviceType
*/
uint8_t _vvs_hrDeviceType[256];


/* storage for string value(s) for hrDeviceDescr
*/
uint8_t _vvs_hrDeviceDescr[65];


/* storage for string value(s) for hrDeviceID
*/
uint8_t _vvs_hrDeviceID[256];


/* storage for multi-dimensional property hrDeviceStatus
*/
      int32_t _vva_hrDeviceStatus[1];
const int32_t _fva_hrDeviceStatus[1] = { };

/* storage for multi-dimensional property hrDeviceErrors
*/
      uint32_t _vva_hrDeviceErrors[1];
const uint32_t _fva_hrDeviceErrors[1] = { };

/* storage for string value(s) for hrProcessorFrwID
*/
uint8_t _vvs_hrProcessorFrwID[256];


/* storage for multi-dimensional property hrProcessorLoad
*/
      int32_t _vva_hrProcessorLoad[1];
const int32_t _fva_hrProcessorLoad[1] = { };

/* storage for multi-dimensional property hrNetworkIfIndex
*/
      int32_t _vva_hrNetworkIfIndex[1];
const int32_t _fva_hrNetworkIfIndex[1] = { };

/* storage for multi-dimensional property hrPrinterStatus
*/
      int32_t _vva_hrPrinterStatus[1];
const int32_t _fva_hrPrinterStatus[1] = { };

/* storage for string value(s) for hrPrinterDetectedErrorState
*/
uint8_t _vvs_hrPrinterDetectedErrorState[256];


/* storage for multi-dimensional property hrDiskStorageAccess
*/
      int32_t _vva_hrDiskStorageAccess[1];
const int32_t _fva_hrDiskStorageAccess[1] = { };

/* storage for multi-dimensional property hrDiskStorageMedia
*/
      int32_t _vva_hrDiskStorageMedia[1];
const int32_t _fva_hrDiskStorageMedia[1] = { };

/* storage for multi-dimensional property hrDiskStorageRemoveble
*/
      int32_t _vva_hrDiskStorageRemoveble[1];
const int32_t _fva_hrDiskStorageRemoveble[1] = { };

/* storage for multi-dimensional property hrDiskStorageCapacity
*/
      int32_t _vva_hrDiskStorageCapacity[1];
const int32_t _fva_hrDiskStorageCapacity[1] = { };

/* storage for multi-dimensional property hrPartitionIndex
*/
      int32_t _vva_hrPartitionIndex[1];
const int32_t _fva_hrPartitionIndex[1] = { 1 };

/* storage for string value(s) for hrPartitionLabel
*/
uint8_t _vvs_hrPartitionLabel[129];


/* storage for string value(s) for hrPartitionID
*/
uint8_t _vvs_hrPartitionID[256];


/* storage for multi-dimensional property hrPartitionSize
*/
      int32_t _vva_hrPartitionSize[1];
const int32_t _fva_hrPartitionSize[1] = { };

/* storage for multi-dimensional property hrPartitionFSIndex
*/
      int32_t _vva_hrPartitionFSIndex[1];
const int32_t _fva_hrPartitionFSIndex[1] = { };

/* storage for multi-dimensional property hrFSIndex
*/
      int32_t _vva_hrFSIndex[1];
const int32_t _fva_hrFSIndex[1] = { 1 };

/* storage for string value(s) for hrFSMountPoint
*/
uint8_t _vvs_hrFSMountPoint[129];


/* storage for string value(s) for hrFSRemoteMountPoint
*/
uint8_t _vvs_hrFSRemoteMountPoint[129];


/* storage for string value(s) for hrFSType
*/
uint8_t _vvs_hrFSType[256];


/* storage for multi-dimensional property hrFSAccess
*/
      int32_t _vva_hrFSAccess[1];
const int32_t _fva_hrFSAccess[1] = { };

/* storage for multi-dimensional property hrFSBootable
*/
      int32_t _vva_hrFSBootable[1];
const int32_t _fva_hrFSBootable[1] = { };

/* storage for multi-dimensional property hrFSStorageIndex
*/
      int32_t _vva_hrFSStorageIndex[1];
const int32_t _fva_hrFSStorageIndex[1] = { };

/* storage for string value(s) for hrFSLastFullBackupDate
*/
uint8_t _vvs_hrFSLastFullBackupDate[12];


/* storage for string value(s) for hrFSLastPartialBackupDate
*/
uint8_t _vvs_hrFSLastPartialBackupDate[12];


/* storage for multi-dimensional property hrSWRunIndex
*/
      int32_t _vva_hrSWRunIndex[1];
const int32_t _fva_hrSWRunIndex[1] = { 1 };

/* storage for string value(s) for hrSWRunName
*/
uint8_t _vvs_hrSWRunName[65];


/* storage for string value(s) for hrSWRunID
*/
uint8_t _vvs_hrSWRunID[256];


/* storage for string value(s) for hrSWRunPath
*/
uint8_t _vvs_hrSWRunPath[129];


/* storage for string value(s) for hrSWRunParameters
*/
uint8_t _vvs_hrSWRunParameters[129];


/* storage for multi-dimensional property hrSWRunType
*/
      int32_t _vva_hrSWRunType[1];
const int32_t _fva_hrSWRunType[1] = { };

/* storage for multi-dimensional property hrSWRunStatus
*/
      int32_t _vva_hrSWRunStatus[1];
const int32_t _fva_hrSWRunStatus[1] = { };

/* storage for multi-dimensional property hrSWInstalledIndex
*/
      int32_t _vva_hrSWInstalledIndex[1];
const int32_t _fva_hrSWInstalledIndex[1] = { 1 };

/* storage for string value(s) for hrSWInstalledName
*/
uint8_t _vvs_hrSWInstalledName[65];


/* storage for string value(s) for hrSWInstalledID
*/
uint8_t _vvs_hrSWInstalledID[256];


/* storage for multi-dimensional property hrSWInstalledType
*/
      int32_t _vva_hrSWInstalledType[1];
const int32_t _fva_hrSWInstalledType[1] = { };

/* storage for string value(s) for hrSWInstalledDate
*/
uint8_t _vvs_hrSWInstalledDate[12];


/* storage for string value(s) for ifName
*/
uint8_t _vvs_ifName[256];


/* storage for property ifHCInOctets which has encoding more than 32 bits
*/
uint8_t _vva_ifHCInOctets[9];
const uint8_t _fva_ifHCInOctets[9] = {
};

/* storage for property ifHCInUcastPkts which has encoding more than 32 bits
*/
uint8_t _vva_ifHCInUcastPkts[9];
const uint8_t _fva_ifHCInUcastPkts[9] = {
};

/* storage for property ifHCInMulticastPkts which has encoding more than 32 bits
*/
uint8_t _vva_ifHCInMulticastPkts[9];
const uint8_t _fva_ifHCInMulticastPkts[9] = {
};

/* storage for property ifHCInBroadcastPkts which has encoding more than 32 bits
*/
uint8_t _vva_ifHCInBroadcastPkts[9];
const uint8_t _fva_ifHCInBroadcastPkts[9] = {
};

/* storage for property ifHCOutOctets which has encoding more than 32 bits
*/
uint8_t _vva_ifHCOutOctets[9];
const uint8_t _fva_ifHCOutOctets[9] = {
};

/* storage for property ifHCOutUcastPkts which has encoding more than 32 bits
*/
uint8_t _vva_ifHCOutUcastPkts[9];
const uint8_t _fva_ifHCOutUcastPkts[9] = {
};

/* storage for property ifHCOutMulticastPkts which has encoding more than 32 bits
*/
uint8_t _vva_ifHCOutMulticastPkts[9];
const uint8_t _fva_ifHCOutMulticastPkts[9] = {
};

/* storage for property ifHCOutBroadcastPkts which has encoding more than 32 bits
*/
uint8_t _vva_ifHCOutBroadcastPkts[9];
const uint8_t _fva_ifHCOutBroadcastPkts[9] = {
};

/* storage for string value(s) for ifAlias
*/
uint8_t _vvs_ifAlias[65];


/* storage for multi-dimensional property ifStackHigherLayer
*/
      int32_t _vva_ifStackHigherLayer[1];
const int32_t _fva_ifStackHigherLayer[1] = { 1 };

/* storage for multi-dimensional property ifStackLowerLayer
*/
      int32_t _vva_ifStackLowerLayer[1];
const int32_t _fva_ifStackLowerLayer[1] = { 1 };

/* storage for multi-dimensional property ifStackStatus
*/
      int32_t _vva_ifStackStatus[1];
const int32_t _fva_ifStackStatus[1] = { };

/* storage for multi-dimensional property prtGeneralConfigChanges
*/
      uint32_t _vva_prtGeneralConfigChanges[1];
const uint32_t _fva_prtGeneralConfigChanges[1] = { };

/* storage for multi-dimensional property prtGeneralCurrentLocalization
*/
      int32_t _vva_prtGeneralCurrentLocalization[1];
const int32_t _fva_prtGeneralCurrentLocalization[1] = { };

/* storage for multi-dimensional property prtGeneralReset
*/
      int32_t _vva_prtGeneralReset[1];
const int32_t _fva_prtGeneralReset[1] = { };

/* storage for string value(s) for prtGeneralCurrentOperator
*/
uint8_t _vvs_prtGeneralCurrentOperator[128];


/* storage for string value(s) for prtGeneralServicePerson
*/
uint8_t _vvs_prtGeneralServicePerson[128];


/* storage for multi-dimensional property prtInputDefaultIndex
*/
      int32_t _vva_prtInputDefaultIndex[1];
const int32_t _fva_prtInputDefaultIndex[1] = { };

/* storage for multi-dimensional property prtOutputDefaultIndex
*/
      int32_t _vva_prtOutputDefaultIndex[1];
const int32_t _fva_prtOutputDefaultIndex[1] = { };

/* storage for multi-dimensional property prtMarkerDefaultIndex
*/
      int32_t _vva_prtMarkerDefaultIndex[1];
const int32_t _fva_prtMarkerDefaultIndex[1] = { };

/* storage for multi-dimensional property prtMediaPathDefaultIndex
*/
      int32_t _vva_prtMediaPathDefaultIndex[1];
const int32_t _fva_prtMediaPathDefaultIndex[1] = { };

/* storage for multi-dimensional property prtConsoleLocalization
*/
      int32_t _vva_prtConsoleLocalization[1];
const int32_t _fva_prtConsoleLocalization[1] = { };

/* storage for multi-dimensional property prtConsoleNumberOfDisplayLines
*/
      int32_t _vva_prtConsoleNumberOfDisplayLines[1];
const int32_t _fva_prtConsoleNumberOfDisplayLines[1] = { };

/* storage for multi-dimensional property prtConsoleNumberOfDisplayChars
*/
      int32_t _vva_prtConsoleNumberOfDisplayChars[1];
const int32_t _fva_prtConsoleNumberOfDisplayChars[1] = { };

/* storage for multi-dimensional property prtConsoleDisable
*/
      int32_t _vva_prtConsoleDisable[1];
const int32_t _fva_prtConsoleDisable[1] = { };

/* storage for multi-dimensional property prtStorageRefSeqNumber
*/
      int32_t _vva_prtStorageRefSeqNumber[1];
const int32_t _fva_prtStorageRefSeqNumber[1] = { 1 };

/* storage for multi-dimensional property prtStorageRefIndex
*/
      int32_t _vva_prtStorageRefIndex[1];
const int32_t _fva_prtStorageRefIndex[1] = { };

/* storage for multi-dimensional property prtDeviceRefSeqNumber
*/
      int32_t _vva_prtDeviceRefSeqNumber[1];
const int32_t _fva_prtDeviceRefSeqNumber[1] = { 1 };

/* storage for multi-dimensional property prtDeviceRefIndex
*/
      int32_t _vva_prtDeviceRefIndex[1];
const int32_t _fva_prtDeviceRefIndex[1] = { };

/* storage for multi-dimensional property prtCoverIndex
*/
      int32_t _vva_prtCoverIndex[1];
const int32_t _fva_prtCoverIndex[1] = { 1 };

/* storage for string value(s) for prtCoverDescription
*/
uint8_t _vvs_prtCoverDescription[256];


/* storage for multi-dimensional property prtCoverStatus
*/
      int32_t _vva_prtCoverStatus[1];
const int32_t _fva_prtCoverStatus[1] = { };

/* storage for multi-dimensional property prtLocalizationIndex
*/
      int32_t _vva_prtLocalizationIndex[1];
const int32_t _fva_prtLocalizationIndex[1] = { 1 };

/* storage for string value(s) for prtLocalizationLanguage
*/
uint8_t _vvs_prtLocalizationLanguage[3];


/* storage for string value(s) for prtLocalizationCountry
*/
uint8_t _vvs_prtLocalizationCountry[3];


/* storage for multi-dimensional property prtLocalizationCharacterSet
*/
      int32_t _vva_prtLocalizationCharacterSet[1];
const int32_t _fva_prtLocalizationCharacterSet[1] = { };

/* storage for multi-dimensional property prtInputIndex
*/
      int32_t _vva_prtInputIndex[4];
const int32_t _fva_prtInputIndex[4] = { 1, 2, 3, 4 };

/* storage for multi-dimensional property prtInputType
*/
      int32_t _vva_prtInputType[4];
const int32_t _fva_prtInputType[4] = { 3, 3, 3, 4 };

/* storage for multi-dimensional property prtInputDimUnit
*/
      int32_t _vva_prtInputDimUnit[4];
const int32_t _fva_prtInputDimUnit[4] = { 3, 3, 3, 3 };

/* storage for multi-dimensional property prtInputMediaDimFeedDirDeclared
*/
      int32_t _vva_prtInputMediaDimFeedDirDeclared[4];
const int32_t _fva_prtInputMediaDimFeedDirDeclared[4] = { };

/* storage for multi-dimensional property prtInputMediaDimXFeedDirDeclared
*/
      int32_t _vva_prtInputMediaDimXFeedDirDeclared[4];
const int32_t _fva_prtInputMediaDimXFeedDirDeclared[4] = { };

/* storage for multi-dimensional property prtInputMediaDimFeedDirChosen
*/
      int32_t _vva_prtInputMediaDimFeedDirChosen[4];
const int32_t _fva_prtInputMediaDimFeedDirChosen[4] = { };

/* storage for multi-dimensional property prtInputMediaDimXFeedDirChosen
*/
      int32_t _vva_prtInputMediaDimXFeedDirChosen[4];
const int32_t _fva_prtInputMediaDimXFeedDirChosen[4] = { };

/* storage for multi-dimensional property prtInputCapacityUnit
*/
      int32_t _vva_prtInputCapacityUnit[4];
const int32_t _fva_prtInputCapacityUnit[4] = { };

/* storage for multi-dimensional property prtInputMaxCapacity
*/
      int32_t _vva_prtInputMaxCapacity[4];
const int32_t _fva_prtInputMaxCapacity[4] = { };

/* storage for multi-dimensional property prtInputCurrentLevel
*/
      int32_t _vva_prtInputCurrentLevel[4];
const int32_t _fva_prtInputCurrentLevel[4] = { };

/* storage for multi-dimensional property prtInputStatus
*/
      int32_t _vva_prtInputStatus[4];
const int32_t _fva_prtInputStatus[4] = { };

/* storage for string value(s) for prtInputMediaName
*/
uint8_t _vvs_prtInputMediaName[256];
const uint8_t _fvs_prtInputMediaName[/*max of 256*/] = {
        "\x08\x41\x34 plain"
        "\x0CLetter plain"
        "\x09\x35x7 Photo"
        "\x07Unknown"
};

/* storage for string value(s) for prtInputName
*/
uint8_t _vvs_prtInputName[256];
const uint8_t _fvs_prtInputName[/*max of 256*/] = {
        "\x0ALower Tray"
        "\x0BMiddle Tray"
        "\x0APhoto Tray"
        "\x0BManual Feed"
};

/* storage for string value(s) for prtInputVendorName
*/
uint8_t _vvs_prtInputVendorName[256];


/* storage for string value(s) for prtInputModel
*/
uint8_t _vvs_prtInputModel[256];


/* storage for string value(s) for prtInputVersion
*/
uint8_t _vvs_prtInputVersion[256];


/* storage for string value(s) for prtInputSerialNumber
*/
uint8_t _vvs_prtInputSerialNumber[132];


/* storage for string value(s) for prtInputDescription
*/
uint8_t _vvs_prtInputDescription[1024];


/* storage for multi-dimensional property prtInputSecurity
*/
      int32_t _vva_prtInputSecurity[4];
const int32_t _fva_prtInputSecurity[4] = { };

/* storage for multi-dimensional property prtInputMediaWeight
*/
      int32_t _vva_prtInputMediaWeight[4];
const int32_t _fva_prtInputMediaWeight[4] = { };

/* storage for string value(s) for prtInputMediaType
*/
uint8_t _vvs_prtInputMediaType[256];


/* storage for string value(s) for prtInputMediaColor
*/
uint8_t _vvs_prtInputMediaColor[256];


/* storage for multi-dimensional property prtInputMediaFormParts
*/
      int32_t _vva_prtInputMediaFormParts[4];
const int32_t _fva_prtInputMediaFormParts[4] = { };

/* storage for multi-dimensional property prtOutputIndex
*/
      int32_t _vva_prtOutputIndex[1];
const int32_t _fva_prtOutputIndex[1] = { 1 };

/* storage for multi-dimensional property prtOutputType
*/
      int32_t _vva_prtOutputType[1];
const int32_t _fva_prtOutputType[1] = { };

/* storage for multi-dimensional property prtOutputCapacityUnit
*/
      int32_t _vva_prtOutputCapacityUnit[1];
const int32_t _fva_prtOutputCapacityUnit[1] = { };

/* storage for multi-dimensional property prtOutputMaxCapacity
*/
      int32_t _vva_prtOutputMaxCapacity[1];
const int32_t _fva_prtOutputMaxCapacity[1] = { };

/* storage for multi-dimensional property prtOutputRemainingCapacity
*/
      int32_t _vva_prtOutputRemainingCapacity[1];
const int32_t _fva_prtOutputRemainingCapacity[1] = { };

/* storage for multi-dimensional property prtOutputStatus
*/
      int32_t _vva_prtOutputStatus[1];
const int32_t _fva_prtOutputStatus[1] = { };

/* storage for string value(s) for prtOutputName
*/
uint8_t _vvs_prtOutputName[64];


/* storage for string value(s) for prtOutputVendorName
*/
uint8_t _vvs_prtOutputVendorName[64];


/* storage for string value(s) for prtOutputModel
*/
uint8_t _vvs_prtOutputModel[64];


/* storage for string value(s) for prtOutputVersion
*/
uint8_t _vvs_prtOutputVersion[64];


/* storage for string value(s) for prtOutputSerialNumber
*/
uint8_t _vvs_prtOutputSerialNumber[64];


/* storage for string value(s) for prtOutputDescription
*/
uint8_t _vvs_prtOutputDescription[256];


/* storage for multi-dimensional property prtOutputSecurity
*/
      int32_t _vva_prtOutputSecurity[1];
const int32_t _fva_prtOutputSecurity[1] = { };

/* storage for multi-dimensional property prtOutputDimUnit
*/
      int32_t _vva_prtOutputDimUnit[1];
const int32_t _fva_prtOutputDimUnit[1] = { };

/* storage for multi-dimensional property prtOutputMaxDimFeedDir
*/
      int32_t _vva_prtOutputMaxDimFeedDir[1];
const int32_t _fva_prtOutputMaxDimFeedDir[1] = { };

/* storage for multi-dimensional property prtOutputMaxDimXFeedDir
*/
      int32_t _vva_prtOutputMaxDimXFeedDir[1];
const int32_t _fva_prtOutputMaxDimXFeedDir[1] = { };

/* storage for multi-dimensional property prtOutputMinDimFeedDir
*/
      int32_t _vva_prtOutputMinDimFeedDir[1];
const int32_t _fva_prtOutputMinDimFeedDir[1] = { };

/* storage for multi-dimensional property prtOutputMinDimXFeedDir
*/
      int32_t _vva_prtOutputMinDimXFeedDir[1];
const int32_t _fva_prtOutputMinDimXFeedDir[1] = { };

/* storage for multi-dimensional property prtOutputStackingOrder
*/
      int32_t _vva_prtOutputStackingOrder[1];
const int32_t _fva_prtOutputStackingOrder[1] = { };

/* storage for multi-dimensional property prtOutputPageDeliveryOrientation
*/
      int32_t _vva_prtOutputPageDeliveryOrientation[1];
const int32_t _fva_prtOutputPageDeliveryOrientation[1] = { };

/* storage for multi-dimensional property prtOutputBursting
*/
      int32_t _vva_prtOutputBursting[1];
const int32_t _fva_prtOutputBursting[1] = { };

/* storage for multi-dimensional property prtOutputDecollating
*/
      int32_t _vva_prtOutputDecollating[1];
const int32_t _fva_prtOutputDecollating[1] = { };

/* storage for multi-dimensional property prtOutputPageCollated
*/
      int32_t _vva_prtOutputPageCollated[1];
const int32_t _fva_prtOutputPageCollated[1] = { };

/* storage for multi-dimensional property prtOutputOffsetStacking
*/
      int32_t _vva_prtOutputOffsetStacking[1];
const int32_t _fva_prtOutputOffsetStacking[1] = { };

/* storage for multi-dimensional property prtMarkerIndex
*/
      int32_t _vva_prtMarkerIndex[4];
const int32_t _fva_prtMarkerIndex[4] = { 1, 2, 3, 4 };

/* storage for multi-dimensional property prtMarkerMarkTech
*/
      int32_t _vva_prtMarkerMarkTech[4];
const int32_t _fva_prtMarkerMarkTech[4] = { 4, 4, 4, 4 };

/* storage for multi-dimensional property prtMarkerCounterUnit
*/
      int32_t _vva_prtMarkerCounterUnit[4];
const int32_t _fva_prtMarkerCounterUnit[4] = { };

/* storage for multi-dimensional property prtMarkerLifeCount
*/
      uint32_t _vva_prtMarkerLifeCount[4];
const uint32_t _fva_prtMarkerLifeCount[4] = { };

/* storage for multi-dimensional property prtMarkerPowerOnCount
*/
      uint32_t _vva_prtMarkerPowerOnCount[4];
const uint32_t _fva_prtMarkerPowerOnCount[4] = { };

/* storage for multi-dimensional property prtMarkerProcessColorants
*/
      int32_t _vva_prtMarkerProcessColorants[4];
const int32_t _fva_prtMarkerProcessColorants[4] = { };

/* storage for multi-dimensional property prtMarkerSpotColorants
*/
      int32_t _vva_prtMarkerSpotColorants[4];
const int32_t _fva_prtMarkerSpotColorants[4] = { };

/* storage for multi-dimensional property prtMarkerAddressabilityUnit
*/
      int32_t _vva_prtMarkerAddressabilityUnit[4];
const int32_t _fva_prtMarkerAddressabilityUnit[4] = { };

/* storage for multi-dimensional property prtMarkerAddressabilityFeedDir
*/
      int32_t _vva_prtMarkerAddressabilityFeedDir[4];
const int32_t _fva_prtMarkerAddressabilityFeedDir[4] = { };

/* storage for multi-dimensional property prtMarkerAddressabilityXFeedDir
*/
      int32_t _vva_prtMarkerAddressabilityXFeedDir[4];
const int32_t _fva_prtMarkerAddressabilityXFeedDir[4] = { };

/* storage for multi-dimensional property prtMarkerNorthMargin
*/
      int32_t _vva_prtMarkerNorthMargin[4];
const int32_t _fva_prtMarkerNorthMargin[4] = { };

/* storage for multi-dimensional property prtMarkerSouthMargin
*/
      int32_t _vva_prtMarkerSouthMargin[4];
const int32_t _fva_prtMarkerSouthMargin[4] = { };

/* storage for multi-dimensional property prtMarkerWestMargin
*/
      int32_t _vva_prtMarkerWestMargin[4];
const int32_t _fva_prtMarkerWestMargin[4] = { };

/* storage for multi-dimensional property prtMarkerEastMargin
*/
      int32_t _vva_prtMarkerEastMargin[4];
const int32_t _fva_prtMarkerEastMargin[4] = { };

/* storage for multi-dimensional property prtMarkerStatus
*/
      int32_t _vva_prtMarkerStatus[4];
const int32_t _fva_prtMarkerStatus[4] = { };

/* storage for multi-dimensional property prtMarkerSuppliesIndex
*/
      int32_t _vva_prtMarkerSuppliesIndex[4];
const int32_t _fva_prtMarkerSuppliesIndex[4] = { 1, 2, 3, 4 };

/* storage for multi-dimensional property prtMarkerSuppliesMarkerIndex
*/
      int32_t _vva_prtMarkerSuppliesMarkerIndex[4];
const int32_t _fva_prtMarkerSuppliesMarkerIndex[4] = { };

/* storage for multi-dimensional property prtMarkerSuppliesColorantIndex
*/
      int32_t _vva_prtMarkerSuppliesColorantIndex[4];
const int32_t _fva_prtMarkerSuppliesColorantIndex[4] = { };

/* storage for multi-dimensional property prtMarkerSuppliesClass
*/
      int32_t _vva_prtMarkerSuppliesClass[4];
const int32_t _fva_prtMarkerSuppliesClass[4] = { };

/* storage for multi-dimensional property prtMarkerSuppliesType
*/
      int32_t _vva_prtMarkerSuppliesType[4];
const int32_t _fva_prtMarkerSuppliesType[4] = { 3, 3, 3, 3 };

/* storage for string value(s) for prtMarkerSuppliesDescription
*/
uint8_t _vvs_prtMarkerSuppliesDescription[1024];


/* storage for multi-dimensional property prtMarkerSuppliesSupplyUnit
*/
      int32_t _vva_prtMarkerSuppliesSupplyUnit[4];
const int32_t _fva_prtMarkerSuppliesSupplyUnit[4] = { };

/* storage for multi-dimensional property prtMarkerSuppliesMaxCapacity
*/
      int32_t _vva_prtMarkerSuppliesMaxCapacity[4];
const int32_t _fva_prtMarkerSuppliesMaxCapacity[4] = { 100, 100, 100, 100 };

/* storage for multi-dimensional property prtMarkerSuppliesLevel
*/
      int32_t _vva_prtMarkerSuppliesLevel[4];
const int32_t _fva_prtMarkerSuppliesLevel[4] = { 10, 20, 30, 40 };

/* storage for multi-dimensional property prtMarkerColorantIndex
*/
      int32_t _vva_prtMarkerColorantIndex[4];
const int32_t _fva_prtMarkerColorantIndex[4] = { 1, 2, 3, 4 };

/* storage for multi-dimensional property prtMarkerColorantMarkerIndex
*/
      int32_t _vva_prtMarkerColorantMarkerIndex[4];
const int32_t _fva_prtMarkerColorantMarkerIndex[4] = { };

/* storage for multi-dimensional property prtMarkerColorantRole
*/
      int32_t _vva_prtMarkerColorantRole[4];
const int32_t _fva_prtMarkerColorantRole[4] = { };

/* storage for string value(s) for prtMarkerColorantValue
*/
uint8_t _vvs_prtMarkerColorantValue[256];
const uint8_t _fvs_prtMarkerColorantValue[/*max of 256*/] = {
        "\x05\x62lack"
        "\x04\x63yan"
        "\x07magenta"
        "\x06yellow"
};

/* storage for multi-dimensional property prtMarkerColorantTonality
*/
      int32_t _vva_prtMarkerColorantTonality[4];
const int32_t _fva_prtMarkerColorantTonality[4] = { };

/* storage for multi-dimensional property prtMediaPathIndex
*/
      int32_t _vva_prtMediaPathIndex[1];
const int32_t _fva_prtMediaPathIndex[1] = { 1 };

/* storage for multi-dimensional property prtMediaPathMaxSpeedPrintUnit
*/
      int32_t _vva_prtMediaPathMaxSpeedPrintUnit[1];
const int32_t _fva_prtMediaPathMaxSpeedPrintUnit[1] = { };

/* storage for multi-dimensional property prtMediaPathMediaSizeUnit
*/
      int32_t _vva_prtMediaPathMediaSizeUnit[1];
const int32_t _fva_prtMediaPathMediaSizeUnit[1] = { };

/* storage for multi-dimensional property prtMediaPathMaxSpeed
*/
      int32_t _vva_prtMediaPathMaxSpeed[1];
const int32_t _fva_prtMediaPathMaxSpeed[1] = { };

/* storage for multi-dimensional property prtMediaPathMaxMediaFeedDir
*/
      int32_t _vva_prtMediaPathMaxMediaFeedDir[1];
const int32_t _fva_prtMediaPathMaxMediaFeedDir[1] = { };

/* storage for multi-dimensional property prtMediaPathMaxMediaXFeedDir
*/
      int32_t _vva_prtMediaPathMaxMediaXFeedDir[1];
const int32_t _fva_prtMediaPathMaxMediaXFeedDir[1] = { };

/* storage for multi-dimensional property prtMediaPathMinMediaFeedDir
*/
      int32_t _vva_prtMediaPathMinMediaFeedDir[1];
const int32_t _fva_prtMediaPathMinMediaFeedDir[1] = { };

/* storage for multi-dimensional property prtMediaPathMinMediaXFeedDir
*/
      int32_t _vva_prtMediaPathMinMediaXFeedDir[1];
const int32_t _fva_prtMediaPathMinMediaXFeedDir[1] = { };

/* storage for multi-dimensional property prtMediaPathType
*/
      int32_t _vva_prtMediaPathType[1];
const int32_t _fva_prtMediaPathType[1] = { };

/* storage for string value(s) for prtMediaPathDescription
*/
uint8_t _vvs_prtMediaPathDescription[256];


/* storage for multi-dimensional property prtMediaPathStatus
*/
      int32_t _vva_prtMediaPathStatus[1];
const int32_t _fva_prtMediaPathStatus[1] = { };

/* storage for multi-dimensional property prtChannelIndex
*/
      int32_t _vva_prtChannelIndex[1];
const int32_t _fva_prtChannelIndex[1] = { 1 };

/* storage for multi-dimensional property prtChannelType
*/
      int32_t _vva_prtChannelType[1];
const int32_t _fva_prtChannelType[1] = { };

/* storage for string value(s) for prtChannelProtocolVersion
*/
uint8_t _vvs_prtChannelProtocolVersion[64];


/* storage for multi-dimensional property prtChannelCurrentJobCntlLangIndex
*/
      int32_t _vva_prtChannelCurrentJobCntlLangIndex[1];
const int32_t _fva_prtChannelCurrentJobCntlLangIndex[1] = { };

/* storage for multi-dimensional property prtChannelDefaultPageDescLangIndex
*/
      int32_t _vva_prtChannelDefaultPageDescLangIndex[1];
const int32_t _fva_prtChannelDefaultPageDescLangIndex[1] = { };

/* storage for multi-dimensional property prtChannelState
*/
      int32_t _vva_prtChannelState[1];
const int32_t _fva_prtChannelState[1] = { };

/* storage for multi-dimensional property prtChannelIfIndex
*/
      int32_t _vva_prtChannelIfIndex[1];
const int32_t _fva_prtChannelIfIndex[1] = { };

/* storage for multi-dimensional property prtChannelStatus
*/
      int32_t _vva_prtChannelStatus[1];
const int32_t _fva_prtChannelStatus[1] = { };

/* storage for multi-dimensional property prtInterpreterIndex
*/
      int32_t _vva_prtInterpreterIndex[1];
const int32_t _fva_prtInterpreterIndex[1] = { 1 };

/* storage for multi-dimensional property prtInterpreterLangFamily
*/
      int32_t _vva_prtInterpreterLangFamily[1];
const int32_t _fva_prtInterpreterLangFamily[1] = { };

/* storage for string value(s) for prtInterpreterLangLevel
*/
uint8_t _vvs_prtInterpreterLangLevel[32];


/* storage for string value(s) for prtInterpreterLangVersion
*/
uint8_t _vvs_prtInterpreterLangVersion[32];


/* storage for string value(s) for prtInterpreterDescription
*/
uint8_t _vvs_prtInterpreterDescription[256];


/* storage for string value(s) for prtInterpreterVersion
*/
uint8_t _vvs_prtInterpreterVersion[32];


/* storage for multi-dimensional property prtInterpreterDefaultOrientation
*/
      int32_t _vva_prtInterpreterDefaultOrientation[1];
const int32_t _fva_prtInterpreterDefaultOrientation[1] = { };

/* storage for multi-dimensional property prtInterpreterFeedAddressability
*/
      int32_t _vva_prtInterpreterFeedAddressability[1];
const int32_t _fva_prtInterpreterFeedAddressability[1] = { };

/* storage for multi-dimensional property prtInterpreterXFeedAddressability
*/
      int32_t _vva_prtInterpreterXFeedAddressability[1];
const int32_t _fva_prtInterpreterXFeedAddressability[1] = { };

/* storage for multi-dimensional property prtInterpreterDefaultCharSetIn
*/
      int32_t _vva_prtInterpreterDefaultCharSetIn[1];
const int32_t _fva_prtInterpreterDefaultCharSetIn[1] = { };

/* storage for multi-dimensional property prtInterpreterDefaultCharSetOut
*/
      int32_t _vva_prtInterpreterDefaultCharSetOut[1];
const int32_t _fva_prtInterpreterDefaultCharSetOut[1] = { };

/* storage for multi-dimensional property prtInterpreterTwoWay
*/
      int32_t _vva_prtInterpreterTwoWay[1];
const int32_t _fva_prtInterpreterTwoWay[1] = { };

/* storage for multi-dimensional property prtConsoleDisplayBufferIndex
*/
      int32_t _vva_prtConsoleDisplayBufferIndex[1];
const int32_t _fva_prtConsoleDisplayBufferIndex[1] = { 1 };

/* storage for string value(s) for prtConsoleDisplayBufferText
*/
uint8_t _vvs_prtConsoleDisplayBufferText[256];


/* storage for multi-dimensional property prtConsoleLightIndex
*/
      int32_t _vva_prtConsoleLightIndex[1];
const int32_t _fva_prtConsoleLightIndex[1] = { 1 };

/* storage for multi-dimensional property prtConsoleOnTime
*/
      int32_t _vva_prtConsoleOnTime[1];
const int32_t _fva_prtConsoleOnTime[1] = { };

/* storage for multi-dimensional property prtConsoleOffTime
*/
      int32_t _vva_prtConsoleOffTime[1];
const int32_t _fva_prtConsoleOffTime[1] = { };

/* storage for multi-dimensional property prtConsoleColor
*/
      int32_t _vva_prtConsoleColor[1];
const int32_t _fva_prtConsoleColor[1] = { };

/* storage for string value(s) for prtConsoleDescription
*/
uint8_t _vvs_prtConsoleDescription[256];


/* storage for multi-dimensional property prtAlertIndex
*/
      int32_t _vva_prtAlertIndex[1];
const int32_t _fva_prtAlertIndex[1] = { 1 };

/* storage for multi-dimensional property prtAlertSeverityLevel
*/
      int32_t _vva_prtAlertSeverityLevel[1];
const int32_t _fva_prtAlertSeverityLevel[1] = { };

/* storage for multi-dimensional property prtAlertTrainingLevel
*/
      int32_t _vva_prtAlertTrainingLevel[1];
const int32_t _fva_prtAlertTrainingLevel[1] = { };

/* storage for multi-dimensional property prtAlertGroup
*/
      int32_t _vva_prtAlertGroup[1];
const int32_t _fva_prtAlertGroup[1] = { };

/* storage for multi-dimensional property prtAlertGroupIndex
*/
      int32_t _vva_prtAlertGroupIndex[1];
const int32_t _fva_prtAlertGroupIndex[1] = { };

/* storage for multi-dimensional property prtAlertLocation
*/
      int32_t _vva_prtAlertLocation[1];
const int32_t _fva_prtAlertLocation[1] = { };

/* storage for multi-dimensional property prtAlertCode
*/
      int32_t _vva_prtAlertCode[1];
const int32_t _fva_prtAlertCode[1] = { };

/* storage for string value(s) for prtAlertDescription
*/
uint8_t _vvs_prtAlertDescription[256];


/* storage for multi-dimensional property prtAlertTime
*/
      uint32_t _vva_prtAlertTime[1];
const uint32_t _fva_prtAlertTime[1] = { };

/* storage for string value(s) for snmpTrapOID
*/
uint8_t _vvs_snmpTrapOID[256];


/* storage for string value(s) for snmpTrapEnterprise
*/
uint8_t _vvs_snmpTrapEnterprise[256];




/* OID to Index cross reference table
*/
const bmibc_oid_xref_t g_oidxreftab[BMIBC_NUM_RECORDS] = {

	{ "1", NVRAMSIGNATURE, 132, { 0, 0, 0 } },    /* 1 */
	{ "1.3.6.1.2.1.1.1", sysDescr, 132, { 0, 0, 0 } },    /* 1.3.6.1.2.1.1.1 */
	{ "1.3.6.1.2.1.1.2", sysObjectID, 134, { 0, 0, 0 } },    /* 1.3.6.1.2.1.1.2 */
	{ "1.3.6.1.2.1.1.3", sysUpTime, 67, { 0, 0, 0 } },    /* 1.3.6.1.2.1.1.3 */
	{ "1.3.6.1.2.1.1.4", sysContact, 132, { 0, 0, 0 } },    /* 1.3.6.1.2.1.1.4 */
	{ "1.3.6.1.2.1.1.5", sysName, 132, { 0, 0, 0 } },    /* 1.3.6.1.2.1.1.5 */
	{ "1.3.6.1.2.1.1.6", sysLocation, 132, { 0, 0, 0 } },    /* 1.3.6.1.2.1.1.6 */
	{ "1.3.6.1.2.1.1.7", sysServices, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.1.7 */
	{ "1.3.6.1.2.1.1.8", sysORLastChange, 67, { 0, 0, 0 } },    /* 1.3.6.1.2.1.1.8 */
	{ "1.3.6.1.2.1.1.9.1.1", sysORIndex, 2, { 9, 0, 0 } },    /* 1.3.6.1.2.1.1.9.1.1 */
	{ "1.3.6.1.2.1.1.9.1.2", sysORID, 134, { 9, 0, 0 } },    /* 1.3.6.1.2.1.1.9.1.2 */
	{ "1.3.6.1.2.1.1.9.1.3", sysORDescr, 132, { 9, 0, 0 } },    /* 1.3.6.1.2.1.1.9.1.3 */
	{ "1.3.6.1.2.1.1.9.1.4", sysORUpTime, 67, { 9, 0, 0 } },    /* 1.3.6.1.2.1.1.9.1.4 */
	{ "1.3.6.1.2.1.2.1", ifNumber, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.2.1 */
	{ "1.3.6.1.2.1.2.2.1.1", ifIndex, 2, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.1 */
	{ "1.3.6.1.2.1.2.2.1.2", ifDescr, 132, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.2 */
	{ "1.3.6.1.2.1.2.2.1.3", ifType, 2, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.3 */
	{ "1.3.6.1.2.1.2.2.1.4", ifMtu, 2, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.4 */
	{ "1.3.6.1.2.1.2.2.1.5", ifSpeed, 66, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.5 */
	{ "1.3.6.1.2.1.2.2.1.6", ifPhysAddress, 132, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.6 */
	{ "1.3.6.1.2.1.2.2.1.7", ifAdminStatus, 2, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.7 */
	{ "1.3.6.1.2.1.2.2.1.8", ifOperStatus, 2, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.8 */
	{ "1.3.6.1.2.1.2.2.1.9", ifLastChange, 67, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.9 */
	{ "1.3.6.1.2.1.2.2.1.10", ifInOctets, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.10 */
	{ "1.3.6.1.2.1.2.2.1.11", ifInUcastPkts, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.11 */
	{ "1.3.6.1.2.1.2.2.1.12", ifInNUcastPkts, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.12 */
	{ "1.3.6.1.2.1.2.2.1.13", ifInDiscards, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.13 */
	{ "1.3.6.1.2.1.2.2.1.14", ifInErrors, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.14 */
	{ "1.3.6.1.2.1.2.2.1.15", ifInUnknownProtos, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.15 */
	{ "1.3.6.1.2.1.2.2.1.16", ifOutOctets, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.16 */
	{ "1.3.6.1.2.1.2.2.1.17", ifOutUcastPkts, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.17 */
	{ "1.3.6.1.2.1.2.2.1.18", ifOutNUcastPkts, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.18 */
	{ "1.3.6.1.2.1.2.2.1.19", ifOutDiscards, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.19 */
	{ "1.3.6.1.2.1.2.2.1.20", ifOutErrors, 65, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.20 */
	{ "1.3.6.1.2.1.2.2.1.21", ifOutQLen, 66, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.21 */
	{ "1.3.6.1.2.1.2.2.1.22", ifSpecific, 134, { 14, 0, 0 } },    /* 1.3.6.1.2.1.2.2.1.22 */
	{ "1.3.6.1.2.1.11.1", snmpInPkts, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.1 */
	{ "1.3.6.1.2.1.11.2", snmpOutPkts, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.2 */
	{ "1.3.6.1.2.1.11.3", snmpInBadVersions, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.3 */
	{ "1.3.6.1.2.1.11.4", snmpInBadCommunityNames, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.4 */
	{ "1.3.6.1.2.1.11.5", snmpInBadCommunityUses, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.5 */
	{ "1.3.6.1.2.1.11.6", snmpInASNParseErrs, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.6 */
	{ "1.3.6.1.2.1.11.8", snmpInTooBigs, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.8 */
	{ "1.3.6.1.2.1.11.9", snmpInNoSuchNames, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.9 */
	{ "1.3.6.1.2.1.11.10", snmpInBadValues, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.10 */
	{ "1.3.6.1.2.1.11.11", snmpInReadOnlys, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.11 */
	{ "1.3.6.1.2.1.11.12", snmpInGenErrs, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.12 */
	{ "1.3.6.1.2.1.11.13", snmpInTotalReqVars, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.13 */
	{ "1.3.6.1.2.1.11.14", snmpInTotalSetVars, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.14 */
	{ "1.3.6.1.2.1.11.15", snmpInGetRequests, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.15 */
	{ "1.3.6.1.2.1.11.16", snmpInGetNexts, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.16 */
	{ "1.3.6.1.2.1.11.17", snmpInSetRequests, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.17 */
	{ "1.3.6.1.2.1.11.18", snmpInGetResponses, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.18 */
	{ "1.3.6.1.2.1.11.19", snmpInTraps, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.19 */
	{ "1.3.6.1.2.1.11.20", snmpOutTooBigs, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.20 */
	{ "1.3.6.1.2.1.11.21", snmpOutNoSuchNames, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.21 */
	{ "1.3.6.1.2.1.11.22", snmpOutBadValues, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.22 */
	{ "1.3.6.1.2.1.11.24", snmpOutGenErrs, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.24 */
	{ "1.3.6.1.2.1.11.25", snmpOutGetRequests, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.25 */
	{ "1.3.6.1.2.1.11.26", snmpOutGetNexts, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.26 */
	{ "1.3.6.1.2.1.11.27", snmpOutSetRequests, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.27 */
	{ "1.3.6.1.2.1.11.28", snmpOutGetResponses, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.28 */
	{ "1.3.6.1.2.1.11.29", snmpOutTraps, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.29 */
	{ "1.3.6.1.2.1.11.30", snmpEnableAuthenTraps, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.30 */
	{ "1.3.6.1.2.1.11.31", snmpSilentDrops, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.31 */
	{ "1.3.6.1.2.1.11.32", snmpProxyDrops, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.11.32 */
	{ "1.3.6.1.2.1.25.1.1", hrSystemUptime, 67, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.1.1 */
	{ "1.3.6.1.2.1.25.1.2", hrSystemDate, 132, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.1.2 */
	{ "1.3.6.1.2.1.25.1.3", hrSystemInitialLoadDevice, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.1.3 */
	{ "1.3.6.1.2.1.25.1.4", hrSystemInitialLoadParameters, 132, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.1.4 */
	{ "1.3.6.1.2.1.25.1.5", hrSystemNumUsers, 66, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.1.5 */
	{ "1.3.6.1.2.1.25.1.6", hrSystemProcesses, 66, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.1.6 */
	{ "1.3.6.1.2.1.25.1.7", hrSystemMaxProcesses, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.1.7 */
	{ "1.3.6.1.2.1.25.2.2", hrMemorySize, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.2.2 */
	{ "1.3.6.1.2.1.25.2.3.1.1", hrStorageIndex, 2, { 74, 0, 0 } },    /* 1.3.6.1.2.1.25.2.3.1.1 */
	{ "1.3.6.1.2.1.25.2.3.1.2", hrStorageType, 134, { 74, 0, 0 } },    /* 1.3.6.1.2.1.25.2.3.1.2 */
	{ "1.3.6.1.2.1.25.2.3.1.3", hrStorageDescr, 132, { 74, 0, 0 } },    /* 1.3.6.1.2.1.25.2.3.1.3 */
	{ "1.3.6.1.2.1.25.2.3.1.4", hrStorageAllocationUnits, 2, { 74, 0, 0 } },    /* 1.3.6.1.2.1.25.2.3.1.4 */
	{ "1.3.6.1.2.1.25.2.3.1.5", hrStorageSize, 2, { 74, 0, 0 } },    /* 1.3.6.1.2.1.25.2.3.1.5 */
	{ "1.3.6.1.2.1.25.2.3.1.6", hrStorageUsed, 2, { 74, 0, 0 } },    /* 1.3.6.1.2.1.25.2.3.1.6 */
	{ "1.3.6.1.2.1.25.2.3.1.7", hrStorageAllocationFailures, 65, { 74, 0, 0 } },    /* 1.3.6.1.2.1.25.2.3.1.7 */
	{ "1.3.6.1.2.1.25.3.2.1.1", hrDeviceIndex, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.2.1.1 */
	{ "1.3.6.1.2.1.25.3.2.1.2", hrDeviceType, 134, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.2.1.2 */
	{ "1.3.6.1.2.1.25.3.2.1.3", hrDeviceDescr, 132, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.2.1.3 */
	{ "1.3.6.1.2.1.25.3.2.1.4", hrDeviceID, 134, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.2.1.4 */
	{ "1.3.6.1.2.1.25.3.2.1.5", hrDeviceStatus, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.2.1.5 */
	{ "1.3.6.1.2.1.25.3.2.1.6", hrDeviceErrors, 65, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.2.1.6 */
	{ "1.3.6.1.2.1.25.3.3.1.1", hrProcessorFrwID, 134, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.3.1.1 */
	{ "1.3.6.1.2.1.25.3.3.1.2", hrProcessorLoad, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.3.1.2 */
	{ "1.3.6.1.2.1.25.3.4.1.1", hrNetworkIfIndex, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.4.1.1 */
	{ "1.3.6.1.2.1.25.3.5.1.1", hrPrinterStatus, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.5.1.1 */
	{ "1.3.6.1.2.1.25.3.5.1.2", hrPrinterDetectedErrorState, 132, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.5.1.2 */
	{ "1.3.6.1.2.1.25.3.6.1.1", hrDiskStorageAccess, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.6.1.1 */
	{ "1.3.6.1.2.1.25.3.6.1.2", hrDiskStorageMedia, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.6.1.2 */
	{ "1.3.6.1.2.1.25.3.6.1.3", hrDiskStorageRemoveble, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.6.1.3 */
	{ "1.3.6.1.2.1.25.3.6.1.4", hrDiskStorageCapacity, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.25.3.6.1.4 */
	{ "1.3.6.1.2.1.25.3.7.1.1", hrPartitionIndex, 2, { 81, 96, 0 } },    /* 1.3.6.1.2.1.25.3.7.1.1 */
	{ "1.3.6.1.2.1.25.3.7.1.2", hrPartitionLabel, 132, { 81, 96, 0 } },    /* 1.3.6.1.2.1.25.3.7.1.2 */
	{ "1.3.6.1.2.1.25.3.7.1.3", hrPartitionID, 132, { 81, 96, 0 } },    /* 1.3.6.1.2.1.25.3.7.1.3 */
	{ "1.3.6.1.2.1.25.3.7.1.4", hrPartitionSize, 2, { 81, 96, 0 } },    /* 1.3.6.1.2.1.25.3.7.1.4 */
	{ "1.3.6.1.2.1.25.3.7.1.5", hrPartitionFSIndex, 2, { 81, 96, 0 } },    /* 1.3.6.1.2.1.25.3.7.1.5 */
	{ "1.3.6.1.2.1.25.3.8.1.1", hrFSIndex, 2, { 101, 0, 0 } },    /* 1.3.6.1.2.1.25.3.8.1.1 */
	{ "1.3.6.1.2.1.25.3.8.1.2", hrFSMountPoint, 132, { 101, 0, 0 } },    /* 1.3.6.1.2.1.25.3.8.1.2 */
	{ "1.3.6.1.2.1.25.3.8.1.3", hrFSRemoteMountPoint, 132, { 101, 0, 0 } },    /* 1.3.6.1.2.1.25.3.8.1.3 */
	{ "1.3.6.1.2.1.25.3.8.1.4", hrFSType, 134, { 101, 0, 0 } },    /* 1.3.6.1.2.1.25.3.8.1.4 */
	{ "1.3.6.1.2.1.25.3.8.1.5", hrFSAccess, 2, { 101, 0, 0 } },    /* 1.3.6.1.2.1.25.3.8.1.5 */
	{ "1.3.6.1.2.1.25.3.8.1.6", hrFSBootable, 2, { 101, 0, 0 } },    /* 1.3.6.1.2.1.25.3.8.1.6 */
	{ "1.3.6.1.2.1.25.3.8.1.7", hrFSStorageIndex, 2, { 101, 0, 0 } },    /* 1.3.6.1.2.1.25.3.8.1.7 */
	{ "1.3.6.1.2.1.25.3.8.1.8", hrFSLastFullBackupDate, 132, { 101, 0, 0 } },    /* 1.3.6.1.2.1.25.3.8.1.8 */
	{ "1.3.6.1.2.1.25.3.8.1.9", hrFSLastPartialBackupDate, 132, { 101, 0, 0 } },    /* 1.3.6.1.2.1.25.3.8.1.9 */
	{ "1.3.6.1.2.1.25.4.1", hrSWOSIndex, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.4.1 */
	{ "1.3.6.1.2.1.25.4.2.1.1", hrSWRunIndex, 2, { 111, 0, 0 } },    /* 1.3.6.1.2.1.25.4.2.1.1 */
	{ "1.3.6.1.2.1.25.4.2.1.2", hrSWRunName, 132, { 111, 0, 0 } },    /* 1.3.6.1.2.1.25.4.2.1.2 */
	{ "1.3.6.1.2.1.25.4.2.1.3", hrSWRunID, 134, { 111, 0, 0 } },    /* 1.3.6.1.2.1.25.4.2.1.3 */
	{ "1.3.6.1.2.1.25.4.2.1.4", hrSWRunPath, 132, { 111, 0, 0 } },    /* 1.3.6.1.2.1.25.4.2.1.4 */
	{ "1.3.6.1.2.1.25.4.2.1.5", hrSWRunParameters, 132, { 111, 0, 0 } },    /* 1.3.6.1.2.1.25.4.2.1.5 */
	{ "1.3.6.1.2.1.25.4.2.1.6", hrSWRunType, 2, { 111, 0, 0 } },    /* 1.3.6.1.2.1.25.4.2.1.6 */
	{ "1.3.6.1.2.1.25.4.2.1.7", hrSWRunStatus, 2, { 111, 0, 0 } },    /* 1.3.6.1.2.1.25.4.2.1.7 */
	{ "1.3.6.1.2.1.25.5.1.1.1", hrSWRunPerfCPU, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.5.1.1.1 */
	{ "1.3.6.1.2.1.25.5.1.1.2", hrSWRunPerfMem, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.5.1.1.2 */
	{ "1.3.6.1.2.1.25.6.1", hrSWInstalledLastChange, 67, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.6.1 */
	{ "1.3.6.1.2.1.25.6.2", hrSWInstalledLastUpdateTime, 67, { 0, 0, 0 } },    /* 1.3.6.1.2.1.25.6.2 */
	{ "1.3.6.1.2.1.25.6.3.1.1", hrSWInstalledIndex, 2, { 122, 0, 0 } },    /* 1.3.6.1.2.1.25.6.3.1.1 */
	{ "1.3.6.1.2.1.25.6.3.1.2", hrSWInstalledName, 132, { 122, 0, 0 } },    /* 1.3.6.1.2.1.25.6.3.1.2 */
	{ "1.3.6.1.2.1.25.6.3.1.3", hrSWInstalledID, 134, { 122, 0, 0 } },    /* 1.3.6.1.2.1.25.6.3.1.3 */
	{ "1.3.6.1.2.1.25.6.3.1.4", hrSWInstalledType, 2, { 122, 0, 0 } },    /* 1.3.6.1.2.1.25.6.3.1.4 */
	{ "1.3.6.1.2.1.25.6.3.1.5", hrSWInstalledDate, 132, { 122, 0, 0 } },    /* 1.3.6.1.2.1.25.6.3.1.5 */
	{ "1.3.6.1.2.1.31.1.1.1.1", ifName, 132, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.1 */
	{ "1.3.6.1.2.1.31.1.1.1.2", ifInMulticastPkts, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.2 */
	{ "1.3.6.1.2.1.31.1.1.1.3", ifInBroadcastPkts, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.3 */
	{ "1.3.6.1.2.1.31.1.1.1.4", ifOutMulticastPkts, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.4 */
	{ "1.3.6.1.2.1.31.1.1.1.5", ifOutBroadcastPkts, 65, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.5 */
	{ "1.3.6.1.2.1.31.1.1.1.6", ifHCInOctets, 70, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.6 */
	{ "1.3.6.1.2.1.31.1.1.1.7", ifHCInUcastPkts, 70, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.7 */
	{ "1.3.6.1.2.1.31.1.1.1.8", ifHCInMulticastPkts, 70, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.8 */
	{ "1.3.6.1.2.1.31.1.1.1.9", ifHCInBroadcastPkts, 70, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.9 */
	{ "1.3.6.1.2.1.31.1.1.1.10", ifHCOutOctets, 70, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.10 */
	{ "1.3.6.1.2.1.31.1.1.1.11", ifHCOutUcastPkts, 70, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.11 */
	{ "1.3.6.1.2.1.31.1.1.1.12", ifHCOutMulticastPkts, 70, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.12 */
	{ "1.3.6.1.2.1.31.1.1.1.13", ifHCOutBroadcastPkts, 70, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.13 */
	{ "1.3.6.1.2.1.31.1.1.1.14", ifLinkUpDownTrapEnable, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.14 */
	{ "1.3.6.1.2.1.31.1.1.1.15", ifHighSpeed, 66, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.15 */
	{ "1.3.6.1.2.1.31.1.1.1.16", ifPromiscuousMode, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.16 */
	{ "1.3.6.1.2.1.31.1.1.1.17", ifConnectorPresent, 2, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.17 */
	{ "1.3.6.1.2.1.31.1.1.1.18", ifAlias, 132, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.18 */
	{ "1.3.6.1.2.1.31.1.1.1.19", ifCounterDiscontinuityTime, 67, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.1.1.19 */
	{ "1.3.6.1.2.1.31.1.2.1.1", ifStackHigherLayer, 2, { 146, 147, 0 } },    /* 1.3.6.1.2.1.31.1.2.1.1 */
	{ "1.3.6.1.2.1.31.1.2.1.2", ifStackLowerLayer, 2, { 146, 147, 0 } },    /* 1.3.6.1.2.1.31.1.2.1.2 */
	{ "1.3.6.1.2.1.31.1.2.1.3", ifStackStatus, 2, { 146, 147, 0 } },    /* 1.3.6.1.2.1.31.1.2.1.3 */
	{ "1.3.6.1.2.1.31.1.5", ifTableLastChange, 67, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.5 */
	{ "1.3.6.1.2.1.31.1.6", ifStackLastChange, 67, { 0, 0, 0 } },    /* 1.3.6.1.2.1.31.1.6 */
	{ "1.3.6.1.2.1.43.5.1.1.1", prtGeneralConfigChanges, 65, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.1 */
	{ "1.3.6.1.2.1.43.5.1.1.2", prtGeneralCurrentLocalization, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.2 */
	{ "1.3.6.1.2.1.43.5.1.1.3", prtGeneralReset, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.3 */
	{ "1.3.6.1.2.1.43.5.1.1.4", prtGeneralCurrentOperator, 132, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.4 */
	{ "1.3.6.1.2.1.43.5.1.1.5", prtGeneralServicePerson, 132, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.5 */
	{ "1.3.6.1.2.1.43.5.1.1.6", prtInputDefaultIndex, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.6 */
	{ "1.3.6.1.2.1.43.5.1.1.7", prtOutputDefaultIndex, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.7 */
	{ "1.3.6.1.2.1.43.5.1.1.8", prtMarkerDefaultIndex, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.8 */
	{ "1.3.6.1.2.1.43.5.1.1.9", prtMediaPathDefaultIndex, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.9 */
	{ "1.3.6.1.2.1.43.5.1.1.10", prtConsoleLocalization, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.10 */
	{ "1.3.6.1.2.1.43.5.1.1.11", prtConsoleNumberOfDisplayLines, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.11 */
	{ "1.3.6.1.2.1.43.5.1.1.12", prtConsoleNumberOfDisplayChars, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.12 */
	{ "1.3.6.1.2.1.43.5.1.1.13", prtConsoleDisable, 2, { 81, 0, 0 } },    /* 1.3.6.1.2.1.43.5.1.1.13 */
	{ "1.3.6.1.2.1.43.5.2.1.1", prtStorageRefSeqNumber, 2, { 74, 164, 0 } },    /* 1.3.6.1.2.1.43.5.2.1.1 */
	{ "1.3.6.1.2.1.43.5.2.1.2", prtStorageRefIndex, 2, { 74, 164, 0 } },    /* 1.3.6.1.2.1.43.5.2.1.2 */
	{ "1.3.6.1.2.1.43.5.3.1.1", prtDeviceRefSeqNumber, 2, { 81, 166, 0 } },    /* 1.3.6.1.2.1.43.5.3.1.1 */
	{ "1.3.6.1.2.1.43.5.3.1.2", prtDeviceRefIndex, 2, { 81, 166, 0 } },    /* 1.3.6.1.2.1.43.5.3.1.2 */
	{ "1.3.6.1.2.1.43.6.1.1.1", prtCoverIndex, 2, { 81, 168, 0 } },    /* 1.3.6.1.2.1.43.6.1.1.1 */
	{ "1.3.6.1.2.1.43.6.1.1.2", prtCoverDescription, 132, { 81, 168, 0 } },    /* 1.3.6.1.2.1.43.6.1.1.2 */
	{ "1.3.6.1.2.1.43.6.1.1.3", prtCoverStatus, 2, { 81, 168, 0 } },    /* 1.3.6.1.2.1.43.6.1.1.3 */
	{ "1.3.6.1.2.1.43.7.1.1.1", prtLocalizationIndex, 2, { 81, 171, 0 } },    /* 1.3.6.1.2.1.43.7.1.1.1 */
	{ "1.3.6.1.2.1.43.7.1.1.2", prtLocalizationLanguage, 132, { 81, 171, 0 } },    /* 1.3.6.1.2.1.43.7.1.1.2 */
	{ "1.3.6.1.2.1.43.7.1.1.3", prtLocalizationCountry, 132, { 81, 171, 0 } },    /* 1.3.6.1.2.1.43.7.1.1.3 */
	{ "1.3.6.1.2.1.43.7.1.1.4", prtLocalizationCharacterSet, 2, { 81, 171, 0 } },    /* 1.3.6.1.2.1.43.7.1.1.4 */
	{ "1.3.6.1.2.1.43.8.2.1.1", prtInputIndex, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.1 */
	{ "1.3.6.1.2.1.43.8.2.1.2", prtInputType, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.2 */
	{ "1.3.6.1.2.1.43.8.2.1.3", prtInputDimUnit, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.3 */
	{ "1.3.6.1.2.1.43.8.2.1.4", prtInputMediaDimFeedDirDeclared, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.4 */
	{ "1.3.6.1.2.1.43.8.2.1.5", prtInputMediaDimXFeedDirDeclared, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.5 */
	{ "1.3.6.1.2.1.43.8.2.1.6", prtInputMediaDimFeedDirChosen, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.6 */
	{ "1.3.6.1.2.1.43.8.2.1.7", prtInputMediaDimXFeedDirChosen, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.7 */
	{ "1.3.6.1.2.1.43.8.2.1.8", prtInputCapacityUnit, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.8 */
	{ "1.3.6.1.2.1.43.8.2.1.9", prtInputMaxCapacity, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.9 */
	{ "1.3.6.1.2.1.43.8.2.1.10", prtInputCurrentLevel, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.10 */
	{ "1.3.6.1.2.1.43.8.2.1.11", prtInputStatus, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.11 */
	{ "1.3.6.1.2.1.43.8.2.1.12", prtInputMediaName, 132, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.12 */
	{ "1.3.6.1.2.1.43.8.2.1.13", prtInputName, 132, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.13 */
	{ "1.3.6.1.2.1.43.8.2.1.14", prtInputVendorName, 132, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.14 */
	{ "1.3.6.1.2.1.43.8.2.1.15", prtInputModel, 132, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.15 */
	{ "1.3.6.1.2.1.43.8.2.1.16", prtInputVersion, 132, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.16 */
	{ "1.3.6.1.2.1.43.8.2.1.17", prtInputSerialNumber, 132, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.17 */
	{ "1.3.6.1.2.1.43.8.2.1.18", prtInputDescription, 132, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.18 */
	{ "1.3.6.1.2.1.43.8.2.1.19", prtInputSecurity, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.19 */
	{ "1.3.6.1.2.1.43.8.2.1.20", prtInputMediaWeight, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.20 */
	{ "1.3.6.1.2.1.43.8.2.1.21", prtInputMediaType, 132, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.21 */
	{ "1.3.6.1.2.1.43.8.2.1.22", prtInputMediaColor, 132, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.22 */
	{ "1.3.6.1.2.1.43.8.2.1.23", prtInputMediaFormParts, 2, { 81, 175, 0 } },    /* 1.3.6.1.2.1.43.8.2.1.23 */
	{ "1.3.6.1.2.1.43.9.2.1.1", prtOutputIndex, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.1 */
	{ "1.3.6.1.2.1.43.9.2.1.2", prtOutputType, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.2 */
	{ "1.3.6.1.2.1.43.9.2.1.3", prtOutputCapacityUnit, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.3 */
	{ "1.3.6.1.2.1.43.9.2.1.4", prtOutputMaxCapacity, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.4 */
	{ "1.3.6.1.2.1.43.9.2.1.5", prtOutputRemainingCapacity, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.5 */
	{ "1.3.6.1.2.1.43.9.2.1.6", prtOutputStatus, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.6 */
	{ "1.3.6.1.2.1.43.9.2.1.7", prtOutputName, 132, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.7 */
	{ "1.3.6.1.2.1.43.9.2.1.8", prtOutputVendorName, 132, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.8 */
	{ "1.3.6.1.2.1.43.9.2.1.9", prtOutputModel, 132, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.9 */
	{ "1.3.6.1.2.1.43.9.2.1.10", prtOutputVersion, 132, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.10 */
	{ "1.3.6.1.2.1.43.9.2.1.11", prtOutputSerialNumber, 132, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.11 */
	{ "1.3.6.1.2.1.43.9.2.1.12", prtOutputDescription, 132, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.12 */
	{ "1.3.6.1.2.1.43.9.2.1.13", prtOutputSecurity, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.13 */
	{ "1.3.6.1.2.1.43.9.2.1.14", prtOutputDimUnit, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.14 */
	{ "1.3.6.1.2.1.43.9.2.1.15", prtOutputMaxDimFeedDir, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.15 */
	{ "1.3.6.1.2.1.43.9.2.1.16", prtOutputMaxDimXFeedDir, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.16 */
	{ "1.3.6.1.2.1.43.9.2.1.17", prtOutputMinDimFeedDir, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.17 */
	{ "1.3.6.1.2.1.43.9.2.1.18", prtOutputMinDimXFeedDir, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.18 */
	{ "1.3.6.1.2.1.43.9.2.1.19", prtOutputStackingOrder, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.19 */
	{ "1.3.6.1.2.1.43.9.2.1.20", prtOutputPageDeliveryOrientation, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.20 */
	{ "1.3.6.1.2.1.43.9.2.1.21", prtOutputBursting, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.21 */
	{ "1.3.6.1.2.1.43.9.2.1.22", prtOutputDecollating, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.22 */
	{ "1.3.6.1.2.1.43.9.2.1.23", prtOutputPageCollated, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.23 */
	{ "1.3.6.1.2.1.43.9.2.1.24", prtOutputOffsetStacking, 2, { 81, 198, 0 } },    /* 1.3.6.1.2.1.43.9.2.1.24 */
	{ "1.3.6.1.2.1.43.10.2.1.1", prtMarkerIndex, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.1 */
	{ "1.3.6.1.2.1.43.10.2.1.2", prtMarkerMarkTech, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.2 */
	{ "1.3.6.1.2.1.43.10.2.1.3", prtMarkerCounterUnit, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.3 */
	{ "1.3.6.1.2.1.43.10.2.1.4", prtMarkerLifeCount, 65, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.4 */
	{ "1.3.6.1.2.1.43.10.2.1.5", prtMarkerPowerOnCount, 65, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.5 */
	{ "1.3.6.1.2.1.43.10.2.1.6", prtMarkerProcessColorants, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.6 */
	{ "1.3.6.1.2.1.43.10.2.1.7", prtMarkerSpotColorants, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.7 */
	{ "1.3.6.1.2.1.43.10.2.1.8", prtMarkerAddressabilityUnit, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.8 */
	{ "1.3.6.1.2.1.43.10.2.1.9", prtMarkerAddressabilityFeedDir, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.9 */
	{ "1.3.6.1.2.1.43.10.2.1.10", prtMarkerAddressabilityXFeedDir, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.10 */
	{ "1.3.6.1.2.1.43.10.2.1.11", prtMarkerNorthMargin, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.11 */
	{ "1.3.6.1.2.1.43.10.2.1.12", prtMarkerSouthMargin, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.12 */
	{ "1.3.6.1.2.1.43.10.2.1.13", prtMarkerWestMargin, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.13 */
	{ "1.3.6.1.2.1.43.10.2.1.14", prtMarkerEastMargin, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.14 */
	{ "1.3.6.1.2.1.43.10.2.1.15", prtMarkerStatus, 2, { 81, 222, 0 } },    /* 1.3.6.1.2.1.43.10.2.1.15 */
	{ "1.3.6.1.2.1.43.11.1.1.1", prtMarkerSuppliesIndex, 2, { 81, 237, 0 } },    /* 1.3.6.1.2.1.43.11.1.1.1 */
	{ "1.3.6.1.2.1.43.11.1.1.2", prtMarkerSuppliesMarkerIndex, 2, { 81, 237, 0 } },    /* 1.3.6.1.2.1.43.11.1.1.2 */
	{ "1.3.6.1.2.1.43.11.1.1.3", prtMarkerSuppliesColorantIndex, 2, { 81, 237, 0 } },    /* 1.3.6.1.2.1.43.11.1.1.3 */
	{ "1.3.6.1.2.1.43.11.1.1.4", prtMarkerSuppliesClass, 2, { 81, 237, 0 } },    /* 1.3.6.1.2.1.43.11.1.1.4 */
	{ "1.3.6.1.2.1.43.11.1.1.5", prtMarkerSuppliesType, 2, { 81, 237, 0 } },    /* 1.3.6.1.2.1.43.11.1.1.5 */
	{ "1.3.6.1.2.1.43.11.1.1.6", prtMarkerSuppliesDescription, 132, { 81, 237, 0 } },    /* 1.3.6.1.2.1.43.11.1.1.6 */
	{ "1.3.6.1.2.1.43.11.1.1.7", prtMarkerSuppliesSupplyUnit, 2, { 81, 237, 0 } },    /* 1.3.6.1.2.1.43.11.1.1.7 */
	{ "1.3.6.1.2.1.43.11.1.1.8", prtMarkerSuppliesMaxCapacity, 2, { 81, 237, 0 } },    /* 1.3.6.1.2.1.43.11.1.1.8 */
	{ "1.3.6.1.2.1.43.11.1.1.9", prtMarkerSuppliesLevel, 2, { 81, 237, 0 } },    /* 1.3.6.1.2.1.43.11.1.1.9 */
	{ "1.3.6.1.2.1.43.12.1.1.1", prtMarkerColorantIndex, 2, { 81, 246, 0 } },    /* 1.3.6.1.2.1.43.12.1.1.1 */
	{ "1.3.6.1.2.1.43.12.1.1.2", prtMarkerColorantMarkerIndex, 2, { 81, 246, 0 } },    /* 1.3.6.1.2.1.43.12.1.1.2 */
	{ "1.3.6.1.2.1.43.12.1.1.3", prtMarkerColorantRole, 2, { 81, 246, 0 } },    /* 1.3.6.1.2.1.43.12.1.1.3 */
	{ "1.3.6.1.2.1.43.12.1.1.4", prtMarkerColorantValue, 132, { 81, 246, 0 } },    /* 1.3.6.1.2.1.43.12.1.1.4 */
	{ "1.3.6.1.2.1.43.12.1.1.5", prtMarkerColorantTonality, 2, { 81, 246, 0 } },    /* 1.3.6.1.2.1.43.12.1.1.5 */
	{ "1.3.6.1.2.1.43.13.4.1.1", prtMediaPathIndex, 2, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.1 */
	{ "1.3.6.1.2.1.43.13.4.1.2", prtMediaPathMaxSpeedPrintUnit, 2, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.2 */
	{ "1.3.6.1.2.1.43.13.4.1.3", prtMediaPathMediaSizeUnit, 2, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.3 */
	{ "1.3.6.1.2.1.43.13.4.1.4", prtMediaPathMaxSpeed, 2, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.4 */
	{ "1.3.6.1.2.1.43.13.4.1.5", prtMediaPathMaxMediaFeedDir, 2, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.5 */
	{ "1.3.6.1.2.1.43.13.4.1.6", prtMediaPathMaxMediaXFeedDir, 2, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.6 */
	{ "1.3.6.1.2.1.43.13.4.1.7", prtMediaPathMinMediaFeedDir, 2, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.7 */
	{ "1.3.6.1.2.1.43.13.4.1.8", prtMediaPathMinMediaXFeedDir, 2, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.8 */
	{ "1.3.6.1.2.1.43.13.4.1.9", prtMediaPathType, 2, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.9 */
	{ "1.3.6.1.2.1.43.13.4.1.10", prtMediaPathDescription, 132, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.10 */
	{ "1.3.6.1.2.1.43.13.4.1.11", prtMediaPathStatus, 2, { 81, 251, 0 } },    /* 1.3.6.1.2.1.43.13.4.1.11 */
	{ "1.3.6.1.2.1.43.14.1.1.1", prtChannelIndex, 2, { 81, 262, 0 } },    /* 1.3.6.1.2.1.43.14.1.1.1 */
	{ "1.3.6.1.2.1.43.14.1.1.2", prtChannelType, 2, { 81, 262, 0 } },    /* 1.3.6.1.2.1.43.14.1.1.2 */
	{ "1.3.6.1.2.1.43.14.1.1.3", prtChannelProtocolVersion, 132, { 81, 262, 0 } },    /* 1.3.6.1.2.1.43.14.1.1.3 */
	{ "1.3.6.1.2.1.43.14.1.1.4", prtChannelCurrentJobCntlLangIndex, 2, { 81, 262, 0 } },    /* 1.3.6.1.2.1.43.14.1.1.4 */
	{ "1.3.6.1.2.1.43.14.1.1.5", prtChannelDefaultPageDescLangIndex, 2, { 81, 262, 0 } },    /* 1.3.6.1.2.1.43.14.1.1.5 */
	{ "1.3.6.1.2.1.43.14.1.1.6", prtChannelState, 2, { 81, 262, 0 } },    /* 1.3.6.1.2.1.43.14.1.1.6 */
	{ "1.3.6.1.2.1.43.14.1.1.7", prtChannelIfIndex, 2, { 81, 262, 0 } },    /* 1.3.6.1.2.1.43.14.1.1.7 */
	{ "1.3.6.1.2.1.43.14.1.1.8", prtChannelStatus, 2, { 81, 262, 0 } },    /* 1.3.6.1.2.1.43.14.1.1.8 */
	{ "1.3.6.1.2.1.43.15.1.1.1", prtInterpreterIndex, 2, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.1 */
	{ "1.3.6.1.2.1.43.15.1.1.2", prtInterpreterLangFamily, 2, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.2 */
	{ "1.3.6.1.2.1.43.15.1.1.3", prtInterpreterLangLevel, 132, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.3 */
	{ "1.3.6.1.2.1.43.15.1.1.4", prtInterpreterLangVersion, 132, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.4 */
	{ "1.3.6.1.2.1.43.15.1.1.5", prtInterpreterDescription, 132, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.5 */
	{ "1.3.6.1.2.1.43.15.1.1.6", prtInterpreterVersion, 132, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.6 */
	{ "1.3.6.1.2.1.43.15.1.1.7", prtInterpreterDefaultOrientation, 2, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.7 */
	{ "1.3.6.1.2.1.43.15.1.1.8", prtInterpreterFeedAddressability, 2, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.8 */
	{ "1.3.6.1.2.1.43.15.1.1.9", prtInterpreterXFeedAddressability, 2, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.9 */
	{ "1.3.6.1.2.1.43.15.1.1.10", prtInterpreterDefaultCharSetIn, 2, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.10 */
	{ "1.3.6.1.2.1.43.15.1.1.11", prtInterpreterDefaultCharSetOut, 2, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.11 */
	{ "1.3.6.1.2.1.43.15.1.1.12", prtInterpreterTwoWay, 2, { 81, 270, 0 } },    /* 1.3.6.1.2.1.43.15.1.1.12 */
	{ "1.3.6.1.2.1.43.16.5.1.1", prtConsoleDisplayBufferIndex, 2, { 81, 282, 0 } },    /* 1.3.6.1.2.1.43.16.5.1.1 */
	{ "1.3.6.1.2.1.43.16.5.1.2", prtConsoleDisplayBufferText, 132, { 81, 282, 0 } },    /* 1.3.6.1.2.1.43.16.5.1.2 */
	{ "1.3.6.1.2.1.43.17.6.1.1", prtConsoleLightIndex, 2, { 81, 284, 0 } },    /* 1.3.6.1.2.1.43.17.6.1.1 */
	{ "1.3.6.1.2.1.43.17.6.1.2", prtConsoleOnTime, 2, { 81, 284, 0 } },    /* 1.3.6.1.2.1.43.17.6.1.2 */
	{ "1.3.6.1.2.1.43.17.6.1.3", prtConsoleOffTime, 2, { 81, 284, 0 } },    /* 1.3.6.1.2.1.43.17.6.1.3 */
	{ "1.3.6.1.2.1.43.17.6.1.4", prtConsoleColor, 2, { 81, 284, 0 } },    /* 1.3.6.1.2.1.43.17.6.1.4 */
	{ "1.3.6.1.2.1.43.17.6.1.5", prtConsoleDescription, 132, { 81, 284, 0 } },    /* 1.3.6.1.2.1.43.17.6.1.5 */
	{ "1.3.6.1.2.1.43.18.1.1.1", prtAlertIndex, 2, { 81, 289, 0 } },    /* 1.3.6.1.2.1.43.18.1.1.1 */
	{ "1.3.6.1.2.1.43.18.1.1.2", prtAlertSeverityLevel, 2, { 81, 289, 0 } },    /* 1.3.6.1.2.1.43.18.1.1.2 */
	{ "1.3.6.1.2.1.43.18.1.1.3", prtAlertTrainingLevel, 2, { 81, 289, 0 } },    /* 1.3.6.1.2.1.43.18.1.1.3 */
	{ "1.3.6.1.2.1.43.18.1.1.4", prtAlertGroup, 2, { 81, 289, 0 } },    /* 1.3.6.1.2.1.43.18.1.1.4 */
	{ "1.3.6.1.2.1.43.18.1.1.5", prtAlertGroupIndex, 2, { 81, 289, 0 } },    /* 1.3.6.1.2.1.43.18.1.1.5 */
	{ "1.3.6.1.2.1.43.18.1.1.6", prtAlertLocation, 2, { 81, 289, 0 } },    /* 1.3.6.1.2.1.43.18.1.1.6 */
	{ "1.3.6.1.2.1.43.18.1.1.7", prtAlertCode, 2, { 81, 289, 0 } },    /* 1.3.6.1.2.1.43.18.1.1.7 */
	{ "1.3.6.1.2.1.43.18.1.1.8", prtAlertDescription, 132, { 81, 289, 0 } },    /* 1.3.6.1.2.1.43.18.1.1.8 */
	{ "1.3.6.1.2.1.43.18.1.1.9", prtAlertTime, 67, { 81, 289, 0 } },    /* 1.3.6.1.2.1.43.18.1.1.9 */
	{ "1.3.6.1.6.3.1.1.4.1", snmpTrapOID, 134, { 0, 0, 0 } },    /* 1.3.6.1.6.3.1.1.4.1 */
	{ "1.3.6.1.6.3.1.1.4.3", snmpTrapEnterprise, 134, { 0, 0, 0 } },    /* 1.3.6.1.6.3.1.1.4.3 */
	{ "1.3.6.1.6.3.1.1.6.1", snmpSetSerialNo, 2, { 0, 0, 0 } }     /* 1.3.6.1.6.3.1.1.6.1 */
};


#endif /* _BMIBC_GENERATED_C */
