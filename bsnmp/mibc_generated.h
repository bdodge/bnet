
/*
 * This is a generated file - Do Not Edit!
 */

#ifndef _BMIBC_GENERATED_H
#define _BMIBC_GENERATED_H 1

#ifndef BSNMPOBJECT_H
typedef enum {
    BMIBC_VOID,
    BMIBC_ENUM,
    BMIBC_INT,
    BMIBC_INT32,
    BMIBC_UINT32,
    BMIBC_INT64,
    BMIBC_UINT64,
    BMIBC_OCTET,
    BMIBC_OBJECT
} bmibc_value_type_t;

typedef enum {
    BMIBC_DIRECT,
    BMIBC_FUNCTION,
    BMIBC_GLOBAL,
} bmibc_access_method_t;

typedef enum {
    BMIBC_NONE = 0,
    BMIBC_READ = 1,
    BMIBC_WRITE = 2,
    BMIBC_READWRITE = 3,
    BMIBC_PASSWORD = 4
} bmibc_access_perm_t;

typedef struct {
    const char *name;
    int minv;
    int maxv;
    int dim;
    size_t offset;
    size_t bits;
    bmibc_value_type_t type;
    bmibc_access_method_t method;
    bmibc_access_perm_t access;
    void *subs;
    void *factory_value;
    void *value;
} bmibc_record_t;

#endif
/* Number of precompiled object records
*/
#define BMIBC_NUM_RECORDS 301

extern bmibc_record_t g_mib_objects[BMIBC_NUM_RECORDS];


/* Definitions for property tags
*/
#define NVRAMSIGNATURE                  0
#define sysDescr                        1
#define sysObjectID                     2
#define sysUpTime                       3
#define sysContact                      4
#define sysName                         5
#define sysLocation                     6
#define sysServices                     7
#define sysORLastChange                 8
#define sysORIndex                      9
#define sysORID                         10
#define sysORDescr                      11
#define sysORUpTime                     12
#define ifNumber                        13
#define ifIndex                         14
#define ifDescr                         15
#define ifType                          16
#define ifMtu                           17
#define ifSpeed                         18
#define ifPhysAddress                   19
#define ifAdminStatus                   20
#define ifOperStatus                    21
#define ifLastChange                    22
#define ifInOctets                      23
#define ifInUcastPkts                   24
#define ifInNUcastPkts                  25
#define ifInDiscards                    26
#define ifInErrors                      27
#define ifInUnknownProtos               28
#define ifOutOctets                     29
#define ifOutUcastPkts                  30
#define ifOutNUcastPkts                 31
#define ifOutDiscards                   32
#define ifOutErrors                     33
#define ifOutQLen                       34
#define ifSpecific                      35
#define snmpInPkts                      36
#define snmpOutPkts                     37
#define snmpInBadVersions               38
#define snmpInBadCommunityNames         39
#define snmpInBadCommunityUses          40
#define snmpInASNParseErrs              41
#define snmpInTooBigs                   42
#define snmpInNoSuchNames               43
#define snmpInBadValues                 44
#define snmpInReadOnlys                 45
#define snmpInGenErrs                   46
#define snmpInTotalReqVars              47
#define snmpInTotalSetVars              48
#define snmpInGetRequests               49
#define snmpInGetNexts                  50
#define snmpInSetRequests               51
#define snmpInGetResponses              52
#define snmpInTraps                     53
#define snmpOutTooBigs                  54
#define snmpOutNoSuchNames              55
#define snmpOutBadValues                56
#define snmpOutGenErrs                  57
#define snmpOutGetRequests              58
#define snmpOutGetNexts                 59
#define snmpOutSetRequests              60
#define snmpOutGetResponses             61
#define snmpOutTraps                    62
#define snmpEnableAuthenTraps           63
#define snmpSilentDrops                 64
#define snmpProxyDrops                  65
#define hrSystemUptime                  66
#define hrSystemDate                    67
#define hrSystemInitialLoadDevice       68
#define hrSystemInitialLoadParameters   69
#define hrSystemNumUsers                70
#define hrSystemProcesses               71
#define hrSystemMaxProcesses            72
#define hrMemorySize                    73
#define hrStorageIndex                  74
#define hrStorageType                   75
#define hrStorageDescr                  76
#define hrStorageAllocationUnits        77
#define hrStorageSize                   78
#define hrStorageUsed                   79
#define hrStorageAllocationFailures     80
#define hrDeviceIndex                   81
#define hrDeviceType                    82
#define hrDeviceDescr                   83
#define hrDeviceID                      84
#define hrDeviceStatus                  85
#define hrDeviceErrors                  86
#define hrProcessorFrwID                87
#define hrProcessorLoad                 88
#define hrNetworkIfIndex                89
#define hrPrinterStatus                 90
#define hrPrinterDetectedErrorState     91
#define hrDiskStorageAccess             92
#define hrDiskStorageMedia              93
#define hrDiskStorageRemoveble          94
#define hrDiskStorageCapacity           95
#define hrPartitionIndex                96
#define hrPartitionLabel                97
#define hrPartitionID                   98
#define hrPartitionSize                 99
#define hrPartitionFSIndex              100
#define hrFSIndex                       101
#define hrFSMountPoint                  102
#define hrFSRemoteMountPoint            103
#define hrFSType                        104
#define hrFSAccess                      105
#define hrFSBootable                    106
#define hrFSStorageIndex                107
#define hrFSLastFullBackupDate          108
#define hrFSLastPartialBackupDate       109
#define hrSWOSIndex                     110
#define hrSWRunIndex                    111
#define hrSWRunName                     112
#define hrSWRunID                       113
#define hrSWRunPath                     114
#define hrSWRunParameters               115
#define hrSWRunType                     116
#define hrSWRunStatus                   117
#define hrSWRunPerfCPU                  118
#define hrSWRunPerfMem                  119
#define hrSWInstalledLastChange         120
#define hrSWInstalledLastUpdateTime     121
#define hrSWInstalledIndex              122
#define hrSWInstalledName               123
#define hrSWInstalledID                 124
#define hrSWInstalledType               125
#define hrSWInstalledDate               126
#define ifName                          127
#define ifInMulticastPkts               128
#define ifInBroadcastPkts               129
#define ifOutMulticastPkts              130
#define ifOutBroadcastPkts              131
#define ifHCInOctets                    132
#define ifHCInUcastPkts                 133
#define ifHCInMulticastPkts             134
#define ifHCInBroadcastPkts             135
#define ifHCOutOctets                   136
#define ifHCOutUcastPkts                137
#define ifHCOutMulticastPkts            138
#define ifHCOutBroadcastPkts            139
#define ifLinkUpDownTrapEnable          140
#define ifHighSpeed                     141
#define ifPromiscuousMode               142
#define ifConnectorPresent              143
#define ifAlias                         144
#define ifCounterDiscontinuityTime      145
#define ifStackHigherLayer              146
#define ifStackLowerLayer               147
#define ifStackStatus                   148
#define ifTableLastChange               149
#define ifStackLastChange               150
#define prtGeneralConfigChanges         151
#define prtGeneralCurrentLocalization   152
#define prtGeneralReset                 153
#define prtGeneralCurrentOperator       154
#define prtGeneralServicePerson         155
#define prtInputDefaultIndex            156
#define prtOutputDefaultIndex           157
#define prtMarkerDefaultIndex           158
#define prtMediaPathDefaultIndex        159
#define prtConsoleLocalization          160
#define prtConsoleNumberOfDisplayLines  161
#define prtConsoleNumberOfDisplayChars  162
#define prtConsoleDisable               163
#define prtStorageRefSeqNumber          164
#define prtStorageRefIndex              165
#define prtDeviceRefSeqNumber           166
#define prtDeviceRefIndex               167
#define prtCoverIndex                   168
#define prtCoverDescription             169
#define prtCoverStatus                  170
#define prtLocalizationIndex            171
#define prtLocalizationLanguage         172
#define prtLocalizationCountry          173
#define prtLocalizationCharacterSet     174
#define prtInputIndex                   175
#define prtInputType                    176
#define prtInputDimUnit                 177
#define prtInputMediaDimFeedDirDeclared 178
#define prtInputMediaDimXFeedDirDeclared 179
#define prtInputMediaDimFeedDirChosen   180
#define prtInputMediaDimXFeedDirChosen  181
#define prtInputCapacityUnit            182
#define prtInputMaxCapacity             183
#define prtInputCurrentLevel            184
#define prtInputStatus                  185
#define prtInputMediaName               186
#define prtInputName                    187
#define prtInputVendorName              188
#define prtInputModel                   189
#define prtInputVersion                 190
#define prtInputSerialNumber            191
#define prtInputDescription             192
#define prtInputSecurity                193
#define prtInputMediaWeight             194
#define prtInputMediaType               195
#define prtInputMediaColor              196
#define prtInputMediaFormParts          197
#define prtOutputIndex                  198
#define prtOutputType                   199
#define prtOutputCapacityUnit           200
#define prtOutputMaxCapacity            201
#define prtOutputRemainingCapacity      202
#define prtOutputStatus                 203
#define prtOutputName                   204
#define prtOutputVendorName             205
#define prtOutputModel                  206
#define prtOutputVersion                207
#define prtOutputSerialNumber           208
#define prtOutputDescription            209
#define prtOutputSecurity               210
#define prtOutputDimUnit                211
#define prtOutputMaxDimFeedDir          212
#define prtOutputMaxDimXFeedDir         213
#define prtOutputMinDimFeedDir          214
#define prtOutputMinDimXFeedDir         215
#define prtOutputStackingOrder          216
#define prtOutputPageDeliveryOrientation 217
#define prtOutputBursting               218
#define prtOutputDecollating            219
#define prtOutputPageCollated           220
#define prtOutputOffsetStacking         221
#define prtMarkerIndex                  222
#define prtMarkerMarkTech               223
#define prtMarkerCounterUnit            224
#define prtMarkerLifeCount              225
#define prtMarkerPowerOnCount           226
#define prtMarkerProcessColorants       227
#define prtMarkerSpotColorants          228
#define prtMarkerAddressabilityUnit     229
#define prtMarkerAddressabilityFeedDir  230
#define prtMarkerAddressabilityXFeedDir 231
#define prtMarkerNorthMargin            232
#define prtMarkerSouthMargin            233
#define prtMarkerWestMargin             234
#define prtMarkerEastMargin             235
#define prtMarkerStatus                 236
#define prtMarkerSuppliesIndex          237
#define prtMarkerSuppliesMarkerIndex    238
#define prtMarkerSuppliesColorantIndex  239
#define prtMarkerSuppliesClass          240
#define prtMarkerSuppliesType           241
#define prtMarkerSuppliesDescription    242
#define prtMarkerSuppliesSupplyUnit     243
#define prtMarkerSuppliesMaxCapacity    244
#define prtMarkerSuppliesLevel          245
#define prtMarkerColorantIndex          246
#define prtMarkerColorantMarkerIndex    247
#define prtMarkerColorantRole           248
#define prtMarkerColorantValue          249
#define prtMarkerColorantTonality       250
#define prtMediaPathIndex               251
#define prtMediaPathMaxSpeedPrintUnit   252
#define prtMediaPathMediaSizeUnit       253
#define prtMediaPathMaxSpeed            254
#define prtMediaPathMaxMediaFeedDir     255
#define prtMediaPathMaxMediaXFeedDir    256
#define prtMediaPathMinMediaFeedDir     257
#define prtMediaPathMinMediaXFeedDir    258
#define prtMediaPathType                259
#define prtMediaPathDescription         260
#define prtMediaPathStatus              261
#define prtChannelIndex                 262
#define prtChannelType                  263
#define prtChannelProtocolVersion       264
#define prtChannelCurrentJobCntlLangIndex 265
#define prtChannelDefaultPageDescLangIndex 266
#define prtChannelState                 267
#define prtChannelIfIndex               268
#define prtChannelStatus                269
#define prtInterpreterIndex             270
#define prtInterpreterLangFamily        271
#define prtInterpreterLangLevel         272
#define prtInterpreterLangVersion       273
#define prtInterpreterDescription       274
#define prtInterpreterVersion           275
#define prtInterpreterDefaultOrientation 276
#define prtInterpreterFeedAddressability 277
#define prtInterpreterXFeedAddressability 278
#define prtInterpreterDefaultCharSetIn  279
#define prtInterpreterDefaultCharSetOut 280
#define prtInterpreterTwoWay            281
#define prtConsoleDisplayBufferIndex    282
#define prtConsoleDisplayBufferText     283
#define prtConsoleLightIndex            284
#define prtConsoleOnTime                285
#define prtConsoleOffTime               286
#define prtConsoleColor                 287
#define prtConsoleDescription           288
#define prtAlertIndex                   289
#define prtAlertSeverityLevel           290
#define prtAlertTrainingLevel           291
#define prtAlertGroup                   292
#define prtAlertGroupIndex              293
#define prtAlertLocation                294
#define prtAlertCode                    295
#define prtAlertDescription             296
#define prtAlertTime                    297
#define snmpTrapOID                     298
#define snmpTrapEnterprise              299
#define snmpSetSerialNo                 300

/* Enumerations for properties with enumerated type
*/
/* enumeration for IANAifType ------------------------------
*/
typedef enum {
			IANAifType_other = 1,
			IANAifType_regular1822 = 2,
			IANAifType_hdh1822 = 3,
			IANAifType_ddnX25 = 4,
			IANAifType_rfc877x25 = 5,
			IANAifType_ethernetCsmacd = 6,
			IANAifType_iso88023Csmacd = 7,
			IANAifType_iso88024TokenBus = 8,
			IANAifType_iso88025TokenRing = 9,
			IANAifType_iso88026Man = 10,
			IANAifType_starLan = 11,
			IANAifType_proteon10Mbit = 12,
			IANAifType_proteon80Mbit = 13,
			IANAifType_hyperchannel = 14,
			IANAifType_fddi = 15,
			IANAifType_lapb = 16,
			IANAifType_sdlc = 17,
			IANAifType_ds1 = 18,
			IANAifType_e1 = 19,
			IANAifType_basicISDN = 20,
			IANAifType_primaryISDN = 21,
			IANAifType_propPointToPointSerial = 22,
			IANAifType_ppp = 23,
			IANAifType_softwareLoopback = 24,
			IANAifType_eon = 25,
			IANAifType_ethernet3Mbit = 26,
			IANAifType_nsip = 27,
			IANAifType_slip = 28,
			IANAifType_ultra = 29,
			IANAifType_ds3 = 30,
			IANAifType_sip = 31,
			IANAifType_frameRelay = 32,
			IANAifType_rs232 = 33,
			IANAifType_para = 34,
			IANAifType_arcnet = 35,
			IANAifType_arcnetPlus = 36,
			IANAifType_atm = 37,
			IANAifType_miox25 = 38,
			IANAifType_sonet = 39,
			IANAifType_x25ple = 40,
			IANAifType_iso88022llc = 41,
			IANAifType_localTalk = 42,
			IANAifType_smdsDxi = 43,
			IANAifType_frameRelayService = 44,
			IANAifType_v35 = 45,
			IANAifType_hssi = 46,
			IANAifType_hippi = 47,
			IANAifType_modem = 48,
			IANAifType_aal5 = 49,
			IANAifType_sonetPath = 50,
			IANAifType_sonetVT = 51,
			IANAifType_smdsIcip = 52,
			IANAifType_propVirtual = 53,
			IANAifType_propMultiplexor = 54,
			IANAifType_ieee80212 = 55,
			IANAifType_fibreChannel = 56,
			IANAifType_hippiInterface = 57,
			IANAifType_frameRelayInterconnect = 58,
			IANAifType_aflane8023 = 59,
			IANAifType_aflane8025 = 60,
			IANAifType_cctEmul = 61,
			IANAifType_fastEther = 62,
			IANAifType_isdn = 63,
			IANAifType_v11 = 64,
			IANAifType_v36 = 65,
			IANAifType_g703at64k = 66,
			IANAifType_g703at2mb = 67,
			IANAifType_qllc = 68,
			IANAifType_fastEtherFX = 69,
			IANAifType_channel = 70,
			IANAifType_ieee80211 = 71,
			IANAifType_ibm370parChan = 72,
			IANAifType_escon = 73,
			IANAifType_dlsw = 74,
			IANAifType_isdns = 75,
			IANAifType_isdnu = 76,
			IANAifType_lapd = 77,
			IANAifType_ipSwitch = 78,
			IANAifType_rsrb = 79,
			IANAifType_atmLogical = 80,
			IANAifType_ds0 = 81,
			IANAifType_ds0Bundle = 82,
			IANAifType_bsc = 83,
			IANAifType_async = 84,
			IANAifType_cnr = 85,
			IANAifType_iso88025Dtr = 86,
			IANAifType_eplrs = 87,
			IANAifType_arap = 88,
			IANAifType_propCnls = 89,
			IANAifType_hostPad = 90,
			IANAifType_termPad = 91,
			IANAifType_frameRelayMPI = 92,
			IANAifType_x213 = 93,
			IANAifType_adsl = 94,
			IANAifType_radsl = 95,
			IANAifType_sdsl = 96,
			IANAifType_vdsl = 97,
			IANAifType_iso88025CRFPInt = 98,
			IANAifType_myrinet = 99,
			IANAifType_voiceEM = 100,
			IANAifType_voiceFXO = 101,
			IANAifType_voiceFXS = 102,
			IANAifType_voiceEncap = 103,
			IANAifType_voiceOverIp = 104,
			IANAifType_atmDxi = 105,
			IANAifType_atmFuni = 106,
			IANAifType_atmIma = 107,
			IANAifType_pppMultilinkBundle = 108,
			IANAifType_ipOverCdlc = 109,
			IANAifType_ipOverClaw = 110,
			IANAifType_stackToStack = 111,
			IANAifType_virtualIpAddress = 112,
			IANAifType_mpc = 113,
			IANAifType_ipOverAtm = 114,
			IANAifType_iso88025Fiber = 115,
			IANAifType_tdlc = 116,
			IANAifType_gigabitEthernet = 117,
			IANAifType_hdlc = 118,
			IANAifType_lapf = 119,
			IANAifType_v37 = 120,
			IANAifType_x25mlp = 121,
			IANAifType_x25huntGroup = 122,
			IANAifType_trasnpHdlc = 123,
			IANAifType_interleave = 124,
			IANAifType_fast = 125,
			IANAifType_ip = 126,
			IANAifType_docsCableMaclayer = 127,
			IANAifType_docsCableDownstream = 128,
			IANAifType_docsCableUpstream = 129,
			IANAifType_a12MppSwitch = 130,
			IANAifType_tunnel = 131,
			IANAifType_coffee = 132,
			IANAifType_ces = 133,
			IANAifType_atmSubInterface = 134,
			IANAifType_l2vlan = 135,
			IANAifType_l3ipvlan = 136,
			IANAifType_l3ipxvlan = 137,
			IANAifType_digitalPowerline = 138,
			IANAifType_mediaMailOverIp = 139,
			IANAifType_dtm = 140,
			IANAifType_dcn = 141,
			IANAifType_ipForward = 142,
			IANAifType_msdsl = 143,
			IANAifType_ieee1394 = 144,
			IANAifType_if_gsn = 145,
			IANAifType_dvbRccMacLayer = 146,
			IANAifType_dvbRccDownstream = 147,
			IANAifType_dvbRccUpstream = 148,
			IANAifType_atmVirtual = 149,
			IANAifType_mplsTunnel = 150,
			IANAifType_srp = 151,
			IANAifType_voiceOverAtm = 152,
			IANAifType_voiceOverFrameRelay = 153,
			IANAifType_idsl = 154,
			IANAifType_compositeLink = 155,
			IANAifType_ss7SigLink = 156,
			IANAifType_propWirelessP2P = 157,
			IANAifType_frForward = 158,
			IANAifType_rfc1483 = 159,
			IANAifType_usb = 160,
			IANAifType_ieee8023adLag = 161,
			IANAifType_bgppolicyaccounting = 162,
			IANAifType_frf16MfrBundle = 163,
			IANAifType_h323Gatekeeper = 164,
			IANAifType_h323Proxy = 165,
			IANAifType_mpls = 166,
			IANAifType_mfSigLink = 167,
			IANAifType_hdsl2 = 168,
			IANAifType_shdsl = 169,
			IANAifType_ds1FDL = 170,
			IANAifType_pos = 171,
			IANAifType_dvbAsiIn = 172,
			IANAifType_dvbAsiOut = 173,
			IANAifType_plc = 174,
			IANAifType_nfas = 175,
			IANAifType_tr008 = 176,
			IANAifType_gr303RDT = 177,
			IANAifType_gr303IDT = 178,
			IANAifType_isup = 179,
			IANAifType_propDocsWirelessMaclayer = 180,
			IANAifType_propDocsWirelessDownstream = 181,
			IANAifType_propDocsWirelessUpstream = 182,
			IANAifType_hiperlan2 = 183,
			IANAifType_propBWAp2Mp = 184,
			IANAifType_sonetOverheadChannel = 185,
			IANAifType_digitalWrapperOverheadChannel = 186,
			IANAifType_aal2 = 187,
			IANAifType_radioMAC = 188,
			IANAifType_atmRadio = 189,
			IANAifType_imt = 190,
			IANAifType_mvl = 191,
			IANAifType_reachDSL = 192,
			IANAifType_frDlciEndPt = 193,
			IANAifType_atmVciEndPt = 194,
			IANAifType_opticalChannel = 195,
			IANAifType_opticalTransport = 196,
			IANAifType_propAtm = 197 
}
e_IANAifType;



/**** for use in ifdefs in your C code ****/
#define have_IANAifType_other           1
#define have_IANAifType_regular1822     2
#define have_IANAifType_hdh1822         3
#define have_IANAifType_ddnX25          4
#define have_IANAifType_rfc877x25       5
#define have_IANAifType_ethernetCsmacd  6
#define have_IANAifType_iso88023Csmacd  7
#define have_IANAifType_iso88024TokenBus 8
#define have_IANAifType_iso88025TokenRing 9
#define have_IANAifType_iso88026Man     10
#define have_IANAifType_starLan         11
#define have_IANAifType_proteon10Mbit   12
#define have_IANAifType_proteon80Mbit   13
#define have_IANAifType_hyperchannel    14
#define have_IANAifType_fddi            15
#define have_IANAifType_lapb            16
#define have_IANAifType_sdlc            17
#define have_IANAifType_ds1             18
#define have_IANAifType_e1              19
#define have_IANAifType_basicISDN       20
#define have_IANAifType_primaryISDN     21
#define have_IANAifType_propPointToPointSerial 22
#define have_IANAifType_ppp             23
#define have_IANAifType_softwareLoopback 24
#define have_IANAifType_eon             25
#define have_IANAifType_ethernet3Mbit   26
#define have_IANAifType_nsip            27
#define have_IANAifType_slip            28
#define have_IANAifType_ultra           29
#define have_IANAifType_ds3             30
#define have_IANAifType_sip             31
#define have_IANAifType_frameRelay      32
#define have_IANAifType_rs232           33
#define have_IANAifType_para            34
#define have_IANAifType_arcnet          35
#define have_IANAifType_arcnetPlus      36
#define have_IANAifType_atm             37
#define have_IANAifType_miox25          38
#define have_IANAifType_sonet           39
#define have_IANAifType_x25ple          40
#define have_IANAifType_iso88022llc     41
#define have_IANAifType_localTalk       42
#define have_IANAifType_smdsDxi         43
#define have_IANAifType_frameRelayService 44
#define have_IANAifType_v35             45
#define have_IANAifType_hssi            46
#define have_IANAifType_hippi           47
#define have_IANAifType_modem           48
#define have_IANAifType_aal5            49
#define have_IANAifType_sonetPath       50
#define have_IANAifType_sonetVT         51
#define have_IANAifType_smdsIcip        52
#define have_IANAifType_propVirtual     53
#define have_IANAifType_propMultiplexor 54
#define have_IANAifType_ieee80212       55
#define have_IANAifType_fibreChannel    56
#define have_IANAifType_hippiInterface  57
#define have_IANAifType_frameRelayInterconnect 58
#define have_IANAifType_aflane8023      59
#define have_IANAifType_aflane8025      60
#define have_IANAifType_cctEmul         61
#define have_IANAifType_fastEther       62
#define have_IANAifType_isdn            63
#define have_IANAifType_v11             64
#define have_IANAifType_v36             65
#define have_IANAifType_g703at64k       66
#define have_IANAifType_g703at2mb       67
#define have_IANAifType_qllc            68
#define have_IANAifType_fastEtherFX     69
#define have_IANAifType_channel         70
#define have_IANAifType_ieee80211       71
#define have_IANAifType_ibm370parChan   72
#define have_IANAifType_escon           73
#define have_IANAifType_dlsw            74
#define have_IANAifType_isdns           75
#define have_IANAifType_isdnu           76
#define have_IANAifType_lapd            77
#define have_IANAifType_ipSwitch        78
#define have_IANAifType_rsrb            79
#define have_IANAifType_atmLogical      80
#define have_IANAifType_ds0             81
#define have_IANAifType_ds0Bundle       82
#define have_IANAifType_bsc             83
#define have_IANAifType_async           84
#define have_IANAifType_cnr             85
#define have_IANAifType_iso88025Dtr     86
#define have_IANAifType_eplrs           87
#define have_IANAifType_arap            88
#define have_IANAifType_propCnls        89
#define have_IANAifType_hostPad         90
#define have_IANAifType_termPad         91
#define have_IANAifType_frameRelayMPI   92
#define have_IANAifType_x213            93
#define have_IANAifType_adsl            94
#define have_IANAifType_radsl           95
#define have_IANAifType_sdsl            96
#define have_IANAifType_vdsl            97
#define have_IANAifType_iso88025CRFPInt 98
#define have_IANAifType_myrinet         99
#define have_IANAifType_voiceEM         100
#define have_IANAifType_voiceFXO        101
#define have_IANAifType_voiceFXS        102
#define have_IANAifType_voiceEncap      103
#define have_IANAifType_voiceOverIp     104
#define have_IANAifType_atmDxi          105
#define have_IANAifType_atmFuni         106
#define have_IANAifType_atmIma          107
#define have_IANAifType_pppMultilinkBundle 108
#define have_IANAifType_ipOverCdlc      109
#define have_IANAifType_ipOverClaw      110
#define have_IANAifType_stackToStack    111
#define have_IANAifType_virtualIpAddress 112
#define have_IANAifType_mpc             113
#define have_IANAifType_ipOverAtm       114
#define have_IANAifType_iso88025Fiber   115
#define have_IANAifType_tdlc            116
#define have_IANAifType_gigabitEthernet 117
#define have_IANAifType_hdlc            118
#define have_IANAifType_lapf            119
#define have_IANAifType_v37             120
#define have_IANAifType_x25mlp          121
#define have_IANAifType_x25huntGroup    122
#define have_IANAifType_trasnpHdlc      123
#define have_IANAifType_interleave      124
#define have_IANAifType_fast            125
#define have_IANAifType_ip              126
#define have_IANAifType_docsCableMaclayer 127
#define have_IANAifType_docsCableDownstream 128
#define have_IANAifType_docsCableUpstream 129
#define have_IANAifType_a12MppSwitch    130
#define have_IANAifType_tunnel          131
#define have_IANAifType_coffee          132
#define have_IANAifType_ces             133
#define have_IANAifType_atmSubInterface 134
#define have_IANAifType_l2vlan          135
#define have_IANAifType_l3ipvlan        136
#define have_IANAifType_l3ipxvlan       137
#define have_IANAifType_digitalPowerline 138
#define have_IANAifType_mediaMailOverIp 139
#define have_IANAifType_dtm             140
#define have_IANAifType_dcn             141
#define have_IANAifType_ipForward       142
#define have_IANAifType_msdsl           143
#define have_IANAifType_ieee1394        144
#define have_IANAifType_if_gsn          145
#define have_IANAifType_dvbRccMacLayer  146
#define have_IANAifType_dvbRccDownstream 147
#define have_IANAifType_dvbRccUpstream  148
#define have_IANAifType_atmVirtual      149
#define have_IANAifType_mplsTunnel      150
#define have_IANAifType_srp             151
#define have_IANAifType_voiceOverAtm    152
#define have_IANAifType_voiceOverFrameRelay 153
#define have_IANAifType_idsl            154
#define have_IANAifType_compositeLink   155
#define have_IANAifType_ss7SigLink      156
#define have_IANAifType_propWirelessP2P 157
#define have_IANAifType_frForward       158
#define have_IANAifType_rfc1483         159
#define have_IANAifType_usb             160
#define have_IANAifType_ieee8023adLag   161
#define have_IANAifType_bgppolicyaccounting 162
#define have_IANAifType_frf16MfrBundle  163
#define have_IANAifType_h323Gatekeeper  164
#define have_IANAifType_h323Proxy       165
#define have_IANAifType_mpls            166
#define have_IANAifType_mfSigLink       167
#define have_IANAifType_hdsl2           168
#define have_IANAifType_shdsl           169
#define have_IANAifType_ds1FDL          170
#define have_IANAifType_pos             171
#define have_IANAifType_dvbAsiIn        172
#define have_IANAifType_dvbAsiOut       173
#define have_IANAifType_plc             174
#define have_IANAifType_nfas            175
#define have_IANAifType_tr008           176
#define have_IANAifType_gr303RDT        177
#define have_IANAifType_gr303IDT        178
#define have_IANAifType_isup            179
#define have_IANAifType_propDocsWirelessMaclayer 180
#define have_IANAifType_propDocsWirelessDownstream 181
#define have_IANAifType_propDocsWirelessUpstream 182
#define have_IANAifType_hiperlan2       183
#define have_IANAifType_propBWAp2Mp     184
#define have_IANAifType_sonetOverheadChannel 185
#define have_IANAifType_digitalWrapperOverheadChannel 186
#define have_IANAifType_aal2            187
#define have_IANAifType_radioMAC        188
#define have_IANAifType_atmRadio        189
#define have_IANAifType_imt             190
#define have_IANAifType_mvl             191
#define have_IANAifType_reachDSL        192
#define have_IANAifType_frDlciEndPt     193
#define have_IANAifType_atmVciEndPt     194
#define have_IANAifType_opticalChannel  195
#define have_IANAifType_opticalTransport 196
#define have_IANAifType_propAtm         197


#define ne_IANAifType	197
extern int ve_IANAifType[197];

/* enumeration for ifAdminStatus ---------------------------
*/
typedef enum {
			ifAdminStatus_up = 1,
			ifAdminStatus_down = 2,
			ifAdminStatus_testing = 3 
}
e_ifAdminStatus;



/**** for use in ifdefs in your C code ****/
#define have_ifAdminStatus_up        1
#define have_ifAdminStatus_down      2
#define have_ifAdminStatus_testing   3


#define ne_ifAdminStatus	3
extern int ve_ifAdminStatus[3];

/* enumeration for ifOperStatus ----------------------------
*/
typedef enum {
			ifOperStatus_up = 1,
			ifOperStatus_down = 2,
			ifOperStatus_testing = 3,
			ifOperStatus_unknown = 4,
			ifOperStatus_dormant = 5,
			ifOperStatus_notPresent = 6,
			ifOperStatus_lowerLayerDown = 7 
}
e_ifOperStatus;



/**** for use in ifdefs in your C code ****/
#define have_ifOperStatus_up          1
#define have_ifOperStatus_down        2
#define have_ifOperStatus_testing     3
#define have_ifOperStatus_unknown     4
#define have_ifOperStatus_dormant     5
#define have_ifOperStatus_notPresent  6
#define have_ifOperStatus_lowerLayerDown 7


#define ne_ifOperStatus	7
extern int ve_ifOperStatus[7];

/* enumeration for snmpEnableAuthenTraps -------------------
*/
typedef enum {
			snmpEnableAuthenTraps_enabled = 1,
			snmpEnableAuthenTraps_disabled = 2 
}
e_snmpEnableAuthenTraps;



/**** for use in ifdefs in your C code ****/
#define have_snmpEnableAuthenTraps_enabled 1
#define have_snmpEnableAuthenTraps_disabled 2


#define ne_snmpEnableAuthenTraps	2
extern int ve_snmpEnableAuthenTraps[2];

/* enumeration for hrDeviceStatus --------------------------
*/
typedef enum {
			hrDeviceStatus_unknown = 1,
			hrDeviceStatus_running = 2,
			hrDeviceStatus_warning = 3,
			hrDeviceStatus_testing = 4,
			hrDeviceStatus_down = 5 
}
e_hrDeviceStatus;



/**** for use in ifdefs in your C code ****/
#define have_hrDeviceStatus_unknown 1
#define have_hrDeviceStatus_running 2
#define have_hrDeviceStatus_warning 3
#define have_hrDeviceStatus_testing 4
#define have_hrDeviceStatus_down    5


#define ne_hrDeviceStatus	5
extern int ve_hrDeviceStatus[5];

/* enumeration for hrPrinterStatus -------------------------
*/
typedef enum {
			hrPrinterStatus_other = 1,
			hrPrinterStatus_unknown = 2,
			hrPrinterStatus_idle = 3,
			hrPrinterStatus_printing = 4,
			hrPrinterStatus_warmup = 5 
}
e_hrPrinterStatus;



/**** for use in ifdefs in your C code ****/
#define have_hrPrinterStatus_other 1
#define have_hrPrinterStatus_unknown 2
#define have_hrPrinterStatus_idle  3
#define have_hrPrinterStatus_printing 4
#define have_hrPrinterStatus_warmup 5


#define ne_hrPrinterStatus	5
extern int ve_hrPrinterStatus[5];

/* enumeration for hrDiskStorageAccess ---------------------
*/
typedef enum {
			hrDiskStorageAccess_readWrite = 1,
			hrDiskStorageAccess_readOnly = 2 
}
e_hrDiskStorageAccess;



/**** for use in ifdefs in your C code ****/
#define have_hrDiskStorageAccess_readWrite 1
#define have_hrDiskStorageAccess_readOnly 2


#define ne_hrDiskStorageAccess	2
extern int ve_hrDiskStorageAccess[2];

/* enumeration for hrDiskStorageMedia ----------------------
*/
typedef enum {
			hrDiskStorageMedia_other = 1,
			hrDiskStorageMedia_unknown = 2,
			hrDiskStorageMedia_hardDisk = 3,
			hrDiskStorageMedia_floppyDisk = 4,
			hrDiskStorageMedia_opticalDiskROM = 5,
			hrDiskStorageMedia_opticalDiskWORM = 6,
			hrDiskStorageMedia_opticalDiskRW = 7,
			hrDiskStorageMedia_ramDisk = 8 
}
e_hrDiskStorageMedia;



/**** for use in ifdefs in your C code ****/
#define have_hrDiskStorageMedia_other 1
#define have_hrDiskStorageMedia_unknown 2
#define have_hrDiskStorageMedia_hardDisk 3
#define have_hrDiskStorageMedia_floppyDisk 4
#define have_hrDiskStorageMedia_opticalDiskROM 5
#define have_hrDiskStorageMedia_opticalDiskWORM 6
#define have_hrDiskStorageMedia_opticalDiskRW 7
#define have_hrDiskStorageMedia_ramDisk 8


#define ne_hrDiskStorageMedia	8
extern int ve_hrDiskStorageMedia[8];

/* enumeration for TruthValue ------------------------------
*/
typedef enum {
			TruthValue_true = 1,
			TruthValue_false = 2 
}
e_TruthValue;



/**** for use in ifdefs in your C code ****/
#define have_TruthValue_true            1
#define have_TruthValue_false           2


#define ne_TruthValue	2
extern int ve_TruthValue[2];

/* enumeration for hrFSAccess ------------------------------
*/
typedef enum {
			hrFSAccess_readWrite = 1,
			hrFSAccess_readOnly = 2 
}
e_hrFSAccess;



/**** for use in ifdefs in your C code ****/
#define have_hrFSAccess_readWrite       1
#define have_hrFSAccess_readOnly        2


#define ne_hrFSAccess	2
extern int ve_hrFSAccess[2];

/* enumeration for hrSWRunType -----------------------------
*/
typedef enum {
			hrSWRunType_unknown = 1,
			hrSWRunType_operatingSystem = 2,
			hrSWRunType_deviceDriver = 3,
			hrSWRunType_application = 4 
}
e_hrSWRunType;



/**** for use in ifdefs in your C code ****/
#define have_hrSWRunType_unknown       1
#define have_hrSWRunType_operatingSystem 2
#define have_hrSWRunType_deviceDriver  3
#define have_hrSWRunType_application   4


#define ne_hrSWRunType	4
extern int ve_hrSWRunType[4];

/* enumeration for hrSWRunStatus ---------------------------
*/
typedef enum {
			hrSWRunStatus_running = 1,
			hrSWRunStatus_runnable = 2,
			hrSWRunStatus_notRunnable = 3,
			hrSWRunStatus_invalid = 4 
}
e_hrSWRunStatus;



/**** for use in ifdefs in your C code ****/
#define have_hrSWRunStatus_running   1
#define have_hrSWRunStatus_runnable  2
#define have_hrSWRunStatus_notRunnable 3
#define have_hrSWRunStatus_invalid   4


#define ne_hrSWRunStatus	4
extern int ve_hrSWRunStatus[4];

/* enumeration for hrSWInstalledType -----------------------
*/
typedef enum {
			hrSWInstalledType_unknown = 1,
			hrSWInstalledType_operatingSystem = 2,
			hrSWInstalledType_deviceDriver = 3,
			hrSWInstalledType_application = 4 
}
e_hrSWInstalledType;



/**** for use in ifdefs in your C code ****/
#define have_hrSWInstalledType_unknown 1
#define have_hrSWInstalledType_operatingSystem 2
#define have_hrSWInstalledType_deviceDriver 3
#define have_hrSWInstalledType_application 4


#define ne_hrSWInstalledType	4
extern int ve_hrSWInstalledType[4];

/* enumeration for ifLinkUpDownTrapEnable ------------------
*/
typedef enum {
			ifLinkUpDownTrapEnable_enabled = 1,
			ifLinkUpDownTrapEnable_disabled = 2 
}
e_ifLinkUpDownTrapEnable;



/**** for use in ifdefs in your C code ****/
#define have_ifLinkUpDownTrapEnable_enabled 1
#define have_ifLinkUpDownTrapEnable_disabled 2


#define ne_ifLinkUpDownTrapEnable	2
extern int ve_ifLinkUpDownTrapEnable[2];

/* enumeration for RowStatus -------------------------------
*/
typedef enum {
			RowStatus_active = 1,
			RowStatus_notInService = 2,
			RowStatus_notReady = 3,
			RowStatus_createAndGo = 4,
			RowStatus_createAndWait = 5,
			RowStatus_destroy = 6 
}
e_RowStatus;



/**** for use in ifdefs in your C code ****/
#define have_RowStatus_active            1
#define have_RowStatus_notInService      2
#define have_RowStatus_notReady          3
#define have_RowStatus_createAndGo       4
#define have_RowStatus_createAndWait     5
#define have_RowStatus_destroy           6


#define ne_RowStatus	6
extern int ve_RowStatus[6];

/* enumeration for prtGeneralReset -------------------------
*/
typedef enum {
			prtGeneralReset_notResetting = 3,
			prtGeneralReset_powerCycleReset = 4,
			prtGeneralReset_resetToNVRAM = 5,
			prtGeneralReset_resetToFactoryDefaults = 6 
}
e_prtGeneralReset;



/**** for use in ifdefs in your C code ****/
#define have_prtGeneralReset_notResetting 3
#define have_prtGeneralReset_powerCycleReset 4
#define have_prtGeneralReset_resetToNVRAM 5
#define have_prtGeneralReset_resetToFactoryDefaults 6


#define ne_prtGeneralReset	4
extern int ve_prtGeneralReset[4];

/* enumeration for prtConsoleDisable -----------------------
*/
typedef enum {
			prtConsoleDisable_enabled = 3,
			prtConsoleDisable_disabled = 4 
}
e_prtConsoleDisable;



/**** for use in ifdefs in your C code ****/
#define have_prtConsoleDisable_enabled 3
#define have_prtConsoleDisable_disabled 4


#define ne_prtConsoleDisable	2
extern int ve_prtConsoleDisable[2];

/* enumeration for prtCoverStatus --------------------------
*/
typedef enum {
			prtCoverStatus_other = 1,
			prtCoverStatus_doorOpen = 3,
			prtCoverStatus_doorClosed = 4,
			prtCoverStatus_interlockOpen = 5,
			prtCoverStatus_interlockClosed = 6 
}
e_prtCoverStatus;



/**** for use in ifdefs in your C code ****/
#define have_prtCoverStatus_other   1
#define have_prtCoverStatus_doorOpen 3
#define have_prtCoverStatus_doorClosed 4
#define have_prtCoverStatus_interlockOpen 5
#define have_prtCoverStatus_interlockClosed 6


#define ne_prtCoverStatus	5
extern int ve_prtCoverStatus[5];

/* enumeration for CodedCharSet ----------------------------
*/
typedef enum {
			CodedCharSet_other = 1 
}
e_CodedCharSet;



/**** for use in ifdefs in your C code ****/
#define have_CodedCharSet_other       1


#define ne_CodedCharSet	1
extern int ve_CodedCharSet[1];

/* enumeration for prtInputType ----------------------------
*/
typedef enum {
			prtInputType_other = 1,
			prtInputType_unknown = 2,
			prtInputType_sheetFeedAutoRemovableTray = 3,
			prtInputType_sheetFeedAutoNonRemovableTray = 4,
			prtInputType_sheetFeedManual = 5,
			prtInputType_continuousRoll = 6,
			prtInputType_continuousFanFold = 7 
}
e_prtInputType;



/**** for use in ifdefs in your C code ****/
#define have_prtInputType_other       1
#define have_prtInputType_unknown     2
#define have_prtInputType_sheetFeedAutoRemovableTray 3
#define have_prtInputType_sheetFeedAutoNonRemovableTray 4
#define have_prtInputType_sheetFeedManual 5
#define have_prtInputType_continuousRoll 6
#define have_prtInputType_continuousFanFold 7


#define ne_prtInputType	7
extern int ve_prtInputType[7];

/* enumeration for MediaUnit -------------------------------
*/
typedef enum {
			MediaUnit_tenThousandthsOfInches = 3,
			MediaUnit_micrometers = 4 
}
e_MediaUnit;



/**** for use in ifdefs in your C code ****/
#define have_MediaUnit_tenThousandthsOfInches 3
#define have_MediaUnit_micrometers       4


#define ne_MediaUnit	2
extern int ve_MediaUnit[2];

/* enumeration for CapacityUnit ----------------------------
*/
typedef enum {
			CapacityUnit_tenThousandthsOfInches = 3,
			CapacityUnit_micrometers = 4,
			CapacityUnit_sheets = 8,
			CapacityUnit_feet = 16,
			CapacityUnit_meters = 17 
}
e_CapacityUnit;



/**** for use in ifdefs in your C code ****/
#define have_CapacityUnit_tenThousandthsOfInches 3
#define have_CapacityUnit_micrometers 4
#define have_CapacityUnit_sheets      8
#define have_CapacityUnit_feet        16
#define have_CapacityUnit_meters      17


#define ne_CapacityUnit	5
extern int ve_CapacityUnit[5];

/* enumeration for PresentOnOff ----------------------------
*/
typedef enum {
			PresentOnOff_other = 1,
			PresentOnOff_on = 3,
			PresentOnOff_off = 4,
			PresentOnOff_notPresent = 5 
}
e_PresentOnOff;



/**** for use in ifdefs in your C code ****/
#define have_PresentOnOff_other       1
#define have_PresentOnOff_on          3
#define have_PresentOnOff_off         4
#define have_PresentOnOff_notPresent  5


#define ne_PresentOnOff	4
extern int ve_PresentOnOff[4];

/* enumeration for prtOutputType ---------------------------
*/
typedef enum {
			prtOutputType_other = 1,
			prtOutputType_unknown = 2,
			prtOutputType_removableBin = 3,
			prtOutputType_unRemovableBin = 4,
			prtOutputType_continuousRollDevice = 5,
			prtOutputType_mailBox = 6,
			prtOutputType_continuousFanFold = 7 
}
e_prtOutputType;



/**** for use in ifdefs in your C code ****/
#define have_prtOutputType_other     1
#define have_prtOutputType_unknown   2
#define have_prtOutputType_removableBin 3
#define have_prtOutputType_unRemovableBin 4
#define have_prtOutputType_continuousRollDevice 5
#define have_prtOutputType_mailBox   6
#define have_prtOutputType_continuousFanFold 7


#define ne_prtOutputType	7
extern int ve_prtOutputType[7];

/* enumeration for prtOutputStackingOrder ------------------
*/
typedef enum {
			prtOutputStackingOrder_unknown = 2,
			prtOutputStackingOrder_firstToLast = 3,
			prtOutputStackingOrder_lastToFirst = 4 
}
e_prtOutputStackingOrder;



/**** for use in ifdefs in your C code ****/
#define have_prtOutputStackingOrder_unknown 2
#define have_prtOutputStackingOrder_firstToLast 3
#define have_prtOutputStackingOrder_lastToFirst 4


#define ne_prtOutputStackingOrder	3
extern int ve_prtOutputStackingOrder[3];

/* enumeration for prtOutputPageDeliveryOrientation --------
*/
typedef enum {
			prtOutputPageDeliveryOrientation_faceUp = 3,
			prtOutputPageDeliveryOrientation_faceDown = 4 
}
e_prtOutputPageDeliveryOrientation;



/**** for use in ifdefs in your C code ****/
#define have_prtOutputPageDeliveryOrientation_faceUp 3
#define have_prtOutputPageDeliveryOrientation_faceDown 4


#define ne_prtOutputPageDeliveryOrientation	2
extern int ve_prtOutputPageDeliveryOrientation[2];

/* enumeration for prtMarkerMarkTech -----------------------
*/
typedef enum {
			prtMarkerMarkTech_other = 1,
			prtMarkerMarkTech_unknown = 2,
			prtMarkerMarkTech_electrophotographicLED = 3,
			prtMarkerMarkTech_electrophotographicLaser = 4,
			prtMarkerMarkTech_electrophotographicOther = 5,
			prtMarkerMarkTech_impactMovingHeadDotMatrix9pin = 6,
			prtMarkerMarkTech_impactMovingHeadDotMatrix24pin = 7,
			prtMarkerMarkTech_impactMovingHeadDotMatrixOther = 8,
			prtMarkerMarkTech_impactMovingHeadFullyFormed = 9,
			prtMarkerMarkTech_impactBand = 10,
			prtMarkerMarkTech_impactOther = 11,
			prtMarkerMarkTech_inkjetAqueous = 12,
			prtMarkerMarkTech_inkjetSolid = 13,
			prtMarkerMarkTech_inkjetOther = 14,
			prtMarkerMarkTech_pen = 15,
			prtMarkerMarkTech_thermalTransfer = 16,
			prtMarkerMarkTech_thermalSensitive = 17,
			prtMarkerMarkTech_thermalDiffusion = 18,
			prtMarkerMarkTech_thermalOther = 19,
			prtMarkerMarkTech_electroerosion = 20,
			prtMarkerMarkTech_electrostatic = 21,
			prtMarkerMarkTech_photographicMicrofiche = 22,
			prtMarkerMarkTech_photographicImagesetter = 23,
			prtMarkerMarkTech_photographicOther = 24,
			prtMarkerMarkTech_ionDeposition = 25,
			prtMarkerMarkTech_eBeam = 26,
			prtMarkerMarkTech_typesetter = 27 
}
e_prtMarkerMarkTech;



/**** for use in ifdefs in your C code ****/
#define have_prtMarkerMarkTech_other 1
#define have_prtMarkerMarkTech_unknown 2
#define have_prtMarkerMarkTech_electrophotographicLED 3
#define have_prtMarkerMarkTech_electrophotographicLaser 4
#define have_prtMarkerMarkTech_electrophotographicOther 5
#define have_prtMarkerMarkTech_impactMovingHeadDotMatrix9pin 6
#define have_prtMarkerMarkTech_impactMovingHeadDotMatrix24pin 7
#define have_prtMarkerMarkTech_impactMovingHeadDotMatrixOther 8
#define have_prtMarkerMarkTech_impactMovingHeadFullyFormed 9
#define have_prtMarkerMarkTech_impactBand 10
#define have_prtMarkerMarkTech_impactOther 11
#define have_prtMarkerMarkTech_inkjetAqueous 12
#define have_prtMarkerMarkTech_inkjetSolid 13
#define have_prtMarkerMarkTech_inkjetOther 14
#define have_prtMarkerMarkTech_pen 15
#define have_prtMarkerMarkTech_thermalTransfer 16
#define have_prtMarkerMarkTech_thermalSensitive 17
#define have_prtMarkerMarkTech_thermalDiffusion 18
#define have_prtMarkerMarkTech_thermalOther 19
#define have_prtMarkerMarkTech_electroerosion 20
#define have_prtMarkerMarkTech_electrostatic 21
#define have_prtMarkerMarkTech_photographicMicrofiche 22
#define have_prtMarkerMarkTech_photographicImagesetter 23
#define have_prtMarkerMarkTech_photographicOther 24
#define have_prtMarkerMarkTech_ionDeposition 25
#define have_prtMarkerMarkTech_eBeam 26
#define have_prtMarkerMarkTech_typesetter 27


#define ne_prtMarkerMarkTech	27
extern int ve_prtMarkerMarkTech[27];

/* enumeration for prtMarkerCounterUnit --------------------
*/
typedef enum {
			prtMarkerCounterUnit_tenThousandthsOfInches = 3,
			prtMarkerCounterUnit_micrometers = 4,
			prtMarkerCounterUnit_characters = 5,
			prtMarkerCounterUnit_lines = 6,
			prtMarkerCounterUnit_impressions = 7,
			prtMarkerCounterUnit_sheets = 8,
			prtMarkerCounterUnit_dotRow = 9,
			prtMarkerCounterUnit_hours = 11,
			prtMarkerCounterUnit_feet = 16,
			prtMarkerCounterUnit_meters = 17 
}
e_prtMarkerCounterUnit;



/**** for use in ifdefs in your C code ****/
#define have_prtMarkerCounterUnit_tenThousandthsOfInches 3
#define have_prtMarkerCounterUnit_micrometers 4
#define have_prtMarkerCounterUnit_characters 5
#define have_prtMarkerCounterUnit_lines 6
#define have_prtMarkerCounterUnit_impressions 7
#define have_prtMarkerCounterUnit_sheets 8
#define have_prtMarkerCounterUnit_dotRow 9
#define have_prtMarkerCounterUnit_hours 11
#define have_prtMarkerCounterUnit_feet 16
#define have_prtMarkerCounterUnit_meters 17


#define ne_prtMarkerCounterUnit	10
extern int ve_prtMarkerCounterUnit[10];

/* enumeration for prtMarkerAddressabilityUnit -------------
*/
typedef enum {
			prtMarkerAddressabilityUnit_tenThousandthsOfInches = 3,
			prtMarkerAddressabilityUnit_micrometers = 4 
}
e_prtMarkerAddressabilityUnit;



/**** for use in ifdefs in your C code ****/
#define have_prtMarkerAddressabilityUnit_tenThousandthsOfInches 3
#define have_prtMarkerAddressabilityUnit_micrometers 4


#define ne_prtMarkerAddressabilityUnit	2
extern int ve_prtMarkerAddressabilityUnit[2];

/* enumeration for prtMarkerSuppliesClass ------------------
*/
typedef enum {
			prtMarkerSuppliesClass_other = 1,
			prtMarkerSuppliesClass_supplyThatIsConsumed = 3,
			prtMarkerSuppliesClass_receptacleThatIsFilled = 4 
}
e_prtMarkerSuppliesClass;



/**** for use in ifdefs in your C code ****/
#define have_prtMarkerSuppliesClass_other 1
#define have_prtMarkerSuppliesClass_supplyThatIsConsumed 3
#define have_prtMarkerSuppliesClass_receptacleThatIsFilled 4


#define ne_prtMarkerSuppliesClass	3
extern int ve_prtMarkerSuppliesClass[3];

/* enumeration for prtMarkerSuppliesType -------------------
*/
typedef enum {
			prtMarkerSuppliesType_other = 1,
			prtMarkerSuppliesType_unknown = 2,
			prtMarkerSuppliesType_toner = 3,
			prtMarkerSuppliesType_wasteToner = 4,
			prtMarkerSuppliesType_ink = 5,
			prtMarkerSuppliesType_inkCartridge = 6,
			prtMarkerSuppliesType_inkRibbon = 7,
			prtMarkerSuppliesType_wasteInk = 8,
			prtMarkerSuppliesType_opc = 9,
			prtMarkerSuppliesType_developer = 10,
			prtMarkerSuppliesType_fuserOil = 11,
			prtMarkerSuppliesType_solidWax = 12,
			prtMarkerSuppliesType_ribbonWax = 13,
			prtMarkerSuppliesType_wasteWax = 14 
}
e_prtMarkerSuppliesType;



/**** for use in ifdefs in your C code ****/
#define have_prtMarkerSuppliesType_other 1
#define have_prtMarkerSuppliesType_unknown 2
#define have_prtMarkerSuppliesType_toner 3
#define have_prtMarkerSuppliesType_wasteToner 4
#define have_prtMarkerSuppliesType_ink 5
#define have_prtMarkerSuppliesType_inkCartridge 6
#define have_prtMarkerSuppliesType_inkRibbon 7
#define have_prtMarkerSuppliesType_wasteInk 8
#define have_prtMarkerSuppliesType_opc 9
#define have_prtMarkerSuppliesType_developer 10
#define have_prtMarkerSuppliesType_fuserOil 11
#define have_prtMarkerSuppliesType_solidWax 12
#define have_prtMarkerSuppliesType_ribbonWax 13
#define have_prtMarkerSuppliesType_wasteWax 14


#define ne_prtMarkerSuppliesType	14
extern int ve_prtMarkerSuppliesType[14];

/* enumeration for prtMarkerSuppliesSupplyUnit -------------
*/
typedef enum {
			prtMarkerSuppliesSupplyUnit_tenThousandthsOfInches = 3,
			prtMarkerSuppliesSupplyUnit_micrometers = 4,
			prtMarkerSuppliesSupplyUnit_thousandthsOfOunces = 12,
			prtMarkerSuppliesSupplyUnit_tenthsOfGrams = 13,
			prtMarkerSuppliesSupplyUnit_hundrethsOfFluidOunces = 14,
			prtMarkerSuppliesSupplyUnit_tenthsOfMilliliters = 15 
}
e_prtMarkerSuppliesSupplyUnit;



/**** for use in ifdefs in your C code ****/
#define have_prtMarkerSuppliesSupplyUnit_tenThousandthsOfInches 3
#define have_prtMarkerSuppliesSupplyUnit_micrometers 4
#define have_prtMarkerSuppliesSupplyUnit_thousandthsOfOunces 12
#define have_prtMarkerSuppliesSupplyUnit_tenthsOfGrams 13
#define have_prtMarkerSuppliesSupplyUnit_hundrethsOfFluidOunces 14
#define have_prtMarkerSuppliesSupplyUnit_tenthsOfMilliliters 15


#define ne_prtMarkerSuppliesSupplyUnit	6
extern int ve_prtMarkerSuppliesSupplyUnit[6];

/* enumeration for prtMarkerColorantRole -------------------
*/
typedef enum {
			prtMarkerColorantRole_other = 1,
			prtMarkerColorantRole_process = 3,
			prtMarkerColorantRole_spot = 4 
}
e_prtMarkerColorantRole;



/**** for use in ifdefs in your C code ****/
#define have_prtMarkerColorantRole_other 1
#define have_prtMarkerColorantRole_process 3
#define have_prtMarkerColorantRole_spot 4


#define ne_prtMarkerColorantRole	3
extern int ve_prtMarkerColorantRole[3];

/* enumeration for prtMediaPathMaxSpeedPrintUnit -----------
*/
typedef enum {
			prtMediaPathMaxSpeedPrintUnit_tenThousandthsOfInchesPerHour = 3,
			prtMediaPathMaxSpeedPrintUnit_micrometersPerHour = 4,
			prtMediaPathMaxSpeedPrintUnit_charactersPerHour = 5,
			prtMediaPathMaxSpeedPrintUnit_linesPerHour = 6,
			prtMediaPathMaxSpeedPrintUnit_impressionsPerHour = 7,
			prtMediaPathMaxSpeedPrintUnit_sheetsPerHour = 8,
			prtMediaPathMaxSpeedPrintUnit_dotRowPerHour = 9,
			prtMediaPathMaxSpeedPrintUnit_feetPerHour = 16,
			prtMediaPathMaxSpeedPrintUnit_metersPerHour = 17 
}
e_prtMediaPathMaxSpeedPrintUnit;



/**** for use in ifdefs in your C code ****/
#define have_prtMediaPathMaxSpeedPrintUnit_tenThousandthsOfInchesPerHour 3
#define have_prtMediaPathMaxSpeedPrintUnit_micrometersPerHour 4
#define have_prtMediaPathMaxSpeedPrintUnit_charactersPerHour 5
#define have_prtMediaPathMaxSpeedPrintUnit_linesPerHour 6
#define have_prtMediaPathMaxSpeedPrintUnit_impressionsPerHour 7
#define have_prtMediaPathMaxSpeedPrintUnit_sheetsPerHour 8
#define have_prtMediaPathMaxSpeedPrintUnit_dotRowPerHour 9
#define have_prtMediaPathMaxSpeedPrintUnit_feetPerHour 16
#define have_prtMediaPathMaxSpeedPrintUnit_metersPerHour 17


#define ne_prtMediaPathMaxSpeedPrintUnit	9
extern int ve_prtMediaPathMaxSpeedPrintUnit[9];

/* enumeration for prtMediaPathType ------------------------
*/
typedef enum {
			prtMediaPathType_other = 1,
			prtMediaPathType_unknown = 2,
			prtMediaPathType_longEdgeBindingDuplex = 3,
			prtMediaPathType_shortEdgeBindingDuplex = 4,
			prtMediaPathType_simplex = 5 
}
e_prtMediaPathType;



/**** for use in ifdefs in your C code ****/
#define have_prtMediaPathType_other 1
#define have_prtMediaPathType_unknown 2
#define have_prtMediaPathType_longEdgeBindingDuplex 3
#define have_prtMediaPathType_shortEdgeBindingDuplex 4
#define have_prtMediaPathType_simplex 5


#define ne_prtMediaPathType	5
extern int ve_prtMediaPathType[5];

/* enumeration for prtChannelType --------------------------
*/
typedef enum {
			prtChannelType_other = 1,
			prtChannelType_chSerialPort = 3,
			prtChannelType_chParallelPort = 4,
			prtChannelType_chIEEE1284Port = 5,
			prtChannelType_chSCSIPort = 6,
			prtChannelType_chAppleTalkPAP = 7,
			prtChannelType_chLPDServer = 8,
			prtChannelType_chNetwareRPrinter = 9,
			prtChannelType_chNetwarePServer = 10,
			prtChannelType_chPort9100 = 11,
			prtChannelType_chAppSocket = 12,
			prtChannelType_chFTP = 13,
			prtChannelType_chTFTP = 14,
			prtChannelType_chDLCLLCPort = 15,
			prtChannelType_chIBM3270 = 16,
			prtChannelType_chIBM5250 = 17,
			prtChannelType_chFax = 18,
			prtChannelType_chIEEE1394 = 19,
			prtChannelType_chTransport1 = 20,
			prtChannelType_chCPAP = 21,
			prtChannelType_chDCERemoteProcCall = 22,
			prtChannelType_chONCRemoteProcCall = 23,
			prtChannelType_chOLE = 24,
			prtChannelType_chNamedPipe = 25,
			prtChannelType_chPCPrint = 26,
			prtChannelType_chServerMessageBlock = 27,
			prtChannelType_chDPMF = 28,
			prtChannelType_chDLLAPI = 29,
			prtChannelType_chVxDAPI = 30,
			prtChannelType_chSystemObjectManager = 31,
			prtChannelType_chDECLAT = 32,
			prtChannelType_chNPAP = 33 
}
e_prtChannelType;



/**** for use in ifdefs in your C code ****/
#define have_prtChannelType_other   1
#define have_prtChannelType_chSerialPort 3
#define have_prtChannelType_chParallelPort 4
#define have_prtChannelType_chIEEE1284Port 5
#define have_prtChannelType_chSCSIPort 6
#define have_prtChannelType_chAppleTalkPAP 7
#define have_prtChannelType_chLPDServer 8
#define have_prtChannelType_chNetwareRPrinter 9
#define have_prtChannelType_chNetwarePServer 10
#define have_prtChannelType_chPort9100 11
#define have_prtChannelType_chAppSocket 12
#define have_prtChannelType_chFTP   13
#define have_prtChannelType_chTFTP  14
#define have_prtChannelType_chDLCLLCPort 15
#define have_prtChannelType_chIBM3270 16
#define have_prtChannelType_chIBM5250 17
#define have_prtChannelType_chFax   18
#define have_prtChannelType_chIEEE1394 19
#define have_prtChannelType_chTransport1 20
#define have_prtChannelType_chCPAP  21
#define have_prtChannelType_chDCERemoteProcCall 22
#define have_prtChannelType_chONCRemoteProcCall 23
#define have_prtChannelType_chOLE   24
#define have_prtChannelType_chNamedPipe 25
#define have_prtChannelType_chPCPrint 26
#define have_prtChannelType_chServerMessageBlock 27
#define have_prtChannelType_chDPMF  28
#define have_prtChannelType_chDLLAPI 29
#define have_prtChannelType_chVxDAPI 30
#define have_prtChannelType_chSystemObjectManager 31
#define have_prtChannelType_chDECLAT 32
#define have_prtChannelType_chNPAP  33


#define ne_prtChannelType	32
extern int ve_prtChannelType[32];

/* enumeration for prtChannelState -------------------------
*/
typedef enum {
			prtChannelState_other = 1,
			prtChannelState_printDataAccepted = 3,
			prtChannelState_noDataAccepted = 4 
}
e_prtChannelState;



/**** for use in ifdefs in your C code ****/
#define have_prtChannelState_other 1
#define have_prtChannelState_printDataAccepted 3
#define have_prtChannelState_noDataAccepted 4


#define ne_prtChannelState	3
extern int ve_prtChannelState[3];

/* enumeration for prtInterpreterLangFamily ----------------
*/
typedef enum {
			prtInterpreterLangFamily_other = 1,
			prtInterpreterLangFamily_langPCL = 3,
			prtInterpreterLangFamily_langHPGL = 4,
			prtInterpreterLangFamily_langPJL = 5,
			prtInterpreterLangFamily_langPS = 6,
			prtInterpreterLangFamily_langPSPrinter = 42,
			prtInterpreterLangFamily_langIPDS = 7,
			prtInterpreterLangFamily_langPPDS = 8,
			prtInterpreterLangFamily_langEscapeP = 9,
			prtInterpreterLangFamily_langEpson = 10,
			prtInterpreterLangFamily_langDDIF = 11,
			prtInterpreterLangFamily_langInterpress = 12,
			prtInterpreterLangFamily_langISO6429 = 13,
			prtInterpreterLangFamily_langLineData = 14,
			prtInterpreterLangFamily_langMODCA = 15,
			prtInterpreterLangFamily_langREGIS = 16,
			prtInterpreterLangFamily_langSCS = 17,
			prtInterpreterLangFamily_langSPDL = 18,
			prtInterpreterLangFamily_langTEK4014 = 19,
			prtInterpreterLangFamily_langPDS = 20,
			prtInterpreterLangFamily_langIGP = 21,
			prtInterpreterLangFamily_langCodeV = 22,
			prtInterpreterLangFamily_langDSCDSE = 23,
			prtInterpreterLangFamily_langWPS = 24,
			prtInterpreterLangFamily_langLN03 = 25,
			prtInterpreterLangFamily_langCCITT = 26,
			prtInterpreterLangFamily_langQUIC = 27,
			prtInterpreterLangFamily_langCPAP = 28,
			prtInterpreterLangFamily_langDecPPL = 29,
			prtInterpreterLangFamily_langSimpleText = 30,
			prtInterpreterLangFamily_langNPAP = 31,
			prtInterpreterLangFamily_langDOC = 32,
			prtInterpreterLangFamily_langimPress = 33,
			prtInterpreterLangFamily_langPinwriter = 34,
			prtInterpreterLangFamily_langNPDL = 35,
			prtInterpreterLangFamily_langNEC201PL = 36,
			prtInterpreterLangFamily_langAutomatic = 37,
			prtInterpreterLangFamily_langPages = 38,
			prtInterpreterLangFamily_langLIPS = 39,
			prtInterpreterLangFamily_langTIFF = 40,
			prtInterpreterLangFamily_langDiagnostic = 41,
			prtInterpreterLangFamily_langCaPSL = 43,
			prtInterpreterLangFamily_langEXCL = 44,
			prtInterpreterLangFamily_langLCDS = 45,
			prtInterpreterLangFamily_langXES = 46 
}
e_prtInterpreterLangFamily;



/**** for use in ifdefs in your C code ****/
#define have_prtInterpreterLangFamily_other 1
#define have_prtInterpreterLangFamily_langPCL 3
#define have_prtInterpreterLangFamily_langHPGL 4
#define have_prtInterpreterLangFamily_langPJL 5
#define have_prtInterpreterLangFamily_langPS 6
#define have_prtInterpreterLangFamily_langPSPrinter 42
#define have_prtInterpreterLangFamily_langIPDS 7
#define have_prtInterpreterLangFamily_langPPDS 8
#define have_prtInterpreterLangFamily_langEscapeP 9
#define have_prtInterpreterLangFamily_langEpson 10
#define have_prtInterpreterLangFamily_langDDIF 11
#define have_prtInterpreterLangFamily_langInterpress 12
#define have_prtInterpreterLangFamily_langISO6429 13
#define have_prtInterpreterLangFamily_langLineData 14
#define have_prtInterpreterLangFamily_langMODCA 15
#define have_prtInterpreterLangFamily_langREGIS 16
#define have_prtInterpreterLangFamily_langSCS 17
#define have_prtInterpreterLangFamily_langSPDL 18
#define have_prtInterpreterLangFamily_langTEK4014 19
#define have_prtInterpreterLangFamily_langPDS 20
#define have_prtInterpreterLangFamily_langIGP 21
#define have_prtInterpreterLangFamily_langCodeV 22
#define have_prtInterpreterLangFamily_langDSCDSE 23
#define have_prtInterpreterLangFamily_langWPS 24
#define have_prtInterpreterLangFamily_langLN03 25
#define have_prtInterpreterLangFamily_langCCITT 26
#define have_prtInterpreterLangFamily_langQUIC 27
#define have_prtInterpreterLangFamily_langCPAP 28
#define have_prtInterpreterLangFamily_langDecPPL 29
#define have_prtInterpreterLangFamily_langSimpleText 30
#define have_prtInterpreterLangFamily_langNPAP 31
#define have_prtInterpreterLangFamily_langDOC 32
#define have_prtInterpreterLangFamily_langimPress 33
#define have_prtInterpreterLangFamily_langPinwriter 34
#define have_prtInterpreterLangFamily_langNPDL 35
#define have_prtInterpreterLangFamily_langNEC201PL 36
#define have_prtInterpreterLangFamily_langAutomatic 37
#define have_prtInterpreterLangFamily_langPages 38
#define have_prtInterpreterLangFamily_langLIPS 39
#define have_prtInterpreterLangFamily_langTIFF 40
#define have_prtInterpreterLangFamily_langDiagnostic 41
#define have_prtInterpreterLangFamily_langCaPSL 43
#define have_prtInterpreterLangFamily_langEXCL 44
#define have_prtInterpreterLangFamily_langLCDS 45
#define have_prtInterpreterLangFamily_langXES 46


#define ne_prtInterpreterLangFamily	45
extern int ve_prtInterpreterLangFamily[45];

/* enumeration for prtInterpreterDefaultOrientation --------
*/
typedef enum {
			prtInterpreterDefaultOrientation_other = 1,
			prtInterpreterDefaultOrientation_portrait = 3,
			prtInterpreterDefaultOrientation_landscape = 4 
}
e_prtInterpreterDefaultOrientation;



/**** for use in ifdefs in your C code ****/
#define have_prtInterpreterDefaultOrientation_other 1
#define have_prtInterpreterDefaultOrientation_portrait 3
#define have_prtInterpreterDefaultOrientation_landscape 4


#define ne_prtInterpreterDefaultOrientation	3
extern int ve_prtInterpreterDefaultOrientation[3];

/* enumeration for prtInterpreterTwoWay --------------------
*/
typedef enum {
			prtInterpreterTwoWay_yes = 3,
			prtInterpreterTwoWay_no = 4 
}
e_prtInterpreterTwoWay;



/**** for use in ifdefs in your C code ****/
#define have_prtInterpreterTwoWay_yes 3
#define have_prtInterpreterTwoWay_no 4


#define ne_prtInterpreterTwoWay	2
extern int ve_prtInterpreterTwoWay[2];

/* enumeration for prtConsoleColor -------------------------
*/
typedef enum {
			prtConsoleColor_other = 1,
			prtConsoleColor_unknown = 2,
			prtConsoleColor_white = 3,
			prtConsoleColor_red = 4,
			prtConsoleColor_green = 5,
			prtConsoleColor_blue = 6,
			prtConsoleColor_cyan = 7,
			prtConsoleColor_magenta = 8,
			prtConsoleColor_yellow = 9 
}
e_prtConsoleColor;



/**** for use in ifdefs in your C code ****/
#define have_prtConsoleColor_other 1
#define have_prtConsoleColor_unknown 2
#define have_prtConsoleColor_white 3
#define have_prtConsoleColor_red   4
#define have_prtConsoleColor_green 5
#define have_prtConsoleColor_blue  6
#define have_prtConsoleColor_cyan  7
#define have_prtConsoleColor_magenta 8
#define have_prtConsoleColor_yellow 9


#define ne_prtConsoleColor	9
extern int ve_prtConsoleColor[9];

/* enumeration for prtAlertSeverityLevel -------------------
*/
typedef enum {
			prtAlertSeverityLevel_other = 1,
			prtAlertSeverityLevel_critical = 3,
			prtAlertSeverityLevel_warning = 4 
}
e_prtAlertSeverityLevel;



/**** for use in ifdefs in your C code ****/
#define have_prtAlertSeverityLevel_other 1
#define have_prtAlertSeverityLevel_critical 3
#define have_prtAlertSeverityLevel_warning 4


#define ne_prtAlertSeverityLevel	3
extern int ve_prtAlertSeverityLevel[3];

/* enumeration for prtAlertTrainingLevel -------------------
*/
typedef enum {
			prtAlertTrainingLevel_other = 1,
			prtAlertTrainingLevel_unknown = 2,
			prtAlertTrainingLevel_untrained = 3,
			prtAlertTrainingLevel_trained = 4,
			prtAlertTrainingLevel_fieldService = 5,
			prtAlertTrainingLevel_management = 6 
}
e_prtAlertTrainingLevel;



/**** for use in ifdefs in your C code ****/
#define have_prtAlertTrainingLevel_other 1
#define have_prtAlertTrainingLevel_unknown 2
#define have_prtAlertTrainingLevel_untrained 3
#define have_prtAlertTrainingLevel_trained 4
#define have_prtAlertTrainingLevel_fieldService 5
#define have_prtAlertTrainingLevel_management 6


#define ne_prtAlertTrainingLevel	6
extern int ve_prtAlertTrainingLevel[6];

/* enumeration for prtAlertGroup ---------------------------
*/
typedef enum {
			prtAlertGroup_other = 1,
			prtAlertGroup_hostResourcesMIBStorageTable = 3,
			prtAlertGroup_hostResourcesMIBDeviceTable = 4,
			prtAlertGroup_generalPrinter = 5,
			prtAlertGroup_cover = 6,
			prtAlertGroup_localization = 7,
			prtAlertGroup_input = 8,
			prtAlertGroup_output = 9,
			prtAlertGroup_marker = 10,
			prtAlertGroup_markerSupplies = 11,
			prtAlertGroup_markerColorant = 12,
			prtAlertGroup_mediaPath = 13,
			prtAlertGroup_channel = 14,
			prtAlertGroup_interpreter = 15,
			prtAlertGroup_consoleDisplayBuffer = 16,
			prtAlertGroup_consoleLights = 17 
}
e_prtAlertGroup;



/**** for use in ifdefs in your C code ****/
#define have_prtAlertGroup_other     1
#define have_prtAlertGroup_hostResourcesMIBStorageTable 3
#define have_prtAlertGroup_hostResourcesMIBDeviceTable 4
#define have_prtAlertGroup_generalPrinter 5
#define have_prtAlertGroup_cover     6
#define have_prtAlertGroup_localization 7
#define have_prtAlertGroup_input     8
#define have_prtAlertGroup_output    9
#define have_prtAlertGroup_marker    10
#define have_prtAlertGroup_markerSupplies 11
#define have_prtAlertGroup_markerColorant 12
#define have_prtAlertGroup_mediaPath 13
#define have_prtAlertGroup_channel   14
#define have_prtAlertGroup_interpreter 15
#define have_prtAlertGroup_consoleDisplayBuffer 16
#define have_prtAlertGroup_consoleLights 17


#define ne_prtAlertGroup	16
extern int ve_prtAlertGroup[16];

/* enumeration for prtAlertCode ----------------------------
*/
typedef enum {
			prtAlertCode_other = 1,
			prtAlertCode_unknown = 2,
			prtAlertCode_coverOpen = 3,
			prtAlertCode_coverClosed = 4,
			prtAlertCode_interlockOpen = 5,
			prtAlertCode_interlockClosed = 6,
			prtAlertCode_configurationChange = 7,
			prtAlertCode_jam = 8,
			prtAlertCode_doorOpen = 501,
			prtAlertCode_doorClosed = 502,
			prtAlertCode_powerUp = 503,
			prtAlertCode_powerDown = 504,
			prtAlertCode_inputMediaTrayMissing = 801,
			prtAlertCode_inputMediaSizeChange = 802,
			prtAlertCode_inputMediaWeightChange = 803,
			prtAlertCode_inputMediaTypeChange = 804,
			prtAlertCode_inputMediaColorChange = 805,
			prtAlertCode_inputMediaFormPartsChange = 806,
			prtAlertCode_inputMediaSupplyLow = 807,
			prtAlertCode_inputMediaSupplyEmpty = 808,
			prtAlertCode_outputMediaTrayMissing = 901,
			prtAlertCode_outputMediaTrayAlmostFull = 902,
			prtAlertCode_outputMediaTrayFull = 903,
			prtAlertCode_markerFuserUnderTemperature = 1001,
			prtAlertCode_markerFuserOverTemperature = 1002,
			prtAlertCode_markerTonerEmpty = 1101,
			prtAlertCode_markerInkEmpty = 1102,
			prtAlertCode_markerPrintRibbonEmpty = 1103,
			prtAlertCode_markerTonerAlmostEmpty = 1104,
			prtAlertCode_markerInkAlmostEmpty = 1105,
			prtAlertCode_markerPrintRibbonAlmostEmpty = 1106,
			prtAlertCode_markerWasteTonerReceptacleAlmostFull = 1107,
			prtAlertCode_markerWasteInkReceptacleAlmostFull = 1108,
			prtAlertCode_markerWasteTonerReceptacleFull = 1109,
			prtAlertCode_markerWasteInkReceptacleFull = 1110,
			prtAlertCode_markerOpcLifeAlmostOver = 1111,
			prtAlertCode_markerOpcLifeOver = 1112,
			prtAlertCode_markerDeveloperAlmostEmpty = 1113,
			prtAlertCode_markerDeveloperEmpty = 1114,
			prtAlertCode_mediaPathMediaTrayMissing = 1301,
			prtAlertCode_mediaPathMediaTrayAlmostFull = 1302,
			prtAlertCode_mediaPathMediaTrayFull = 1303,
			prtAlertCode_interpreterMemoryIncrease = 1501,
			prtAlertCode_interpreterMemoryDecrease = 1502,
			prtAlertCode_interpreterCartridgeAdded = 1503,
			prtAlertCode_interpreterCartridgeDeleted = 1504,
			prtAlertCode_interpreterResourceAdded = 1505,
			prtAlertCode_interpreterResourceDeleted = 1506,
			prtAlertCode_interpreterResourceUnavailable = 1507 
}
e_prtAlertCode;



/**** for use in ifdefs in your C code ****/
#define have_prtAlertCode_other       1
#define have_prtAlertCode_unknown     2
#define have_prtAlertCode_coverOpen   3
#define have_prtAlertCode_coverClosed 4
#define have_prtAlertCode_interlockOpen 5
#define have_prtAlertCode_interlockClosed 6
#define have_prtAlertCode_configurationChange 7
#define have_prtAlertCode_jam         8
#define have_prtAlertCode_doorOpen    501
#define have_prtAlertCode_doorClosed  502
#define have_prtAlertCode_powerUp     503
#define have_prtAlertCode_powerDown   504
#define have_prtAlertCode_inputMediaTrayMissing 801
#define have_prtAlertCode_inputMediaSizeChange 802
#define have_prtAlertCode_inputMediaWeightChange 803
#define have_prtAlertCode_inputMediaTypeChange 804
#define have_prtAlertCode_inputMediaColorChange 805
#define have_prtAlertCode_inputMediaFormPartsChange 806
#define have_prtAlertCode_inputMediaSupplyLow 807
#define have_prtAlertCode_inputMediaSupplyEmpty 808
#define have_prtAlertCode_outputMediaTrayMissing 901
#define have_prtAlertCode_outputMediaTrayAlmostFull 902
#define have_prtAlertCode_outputMediaTrayFull 903
#define have_prtAlertCode_markerFuserUnderTemperature 1001
#define have_prtAlertCode_markerFuserOverTemperature 1002
#define have_prtAlertCode_markerTonerEmpty 1101
#define have_prtAlertCode_markerInkEmpty 1102
#define have_prtAlertCode_markerPrintRibbonEmpty 1103
#define have_prtAlertCode_markerTonerAlmostEmpty 1104
#define have_prtAlertCode_markerInkAlmostEmpty 1105
#define have_prtAlertCode_markerPrintRibbonAlmostEmpty 1106
#define have_prtAlertCode_markerWasteTonerReceptacleAlmostFull 1107
#define have_prtAlertCode_markerWasteInkReceptacleAlmostFull 1108
#define have_prtAlertCode_markerWasteTonerReceptacleFull 1109
#define have_prtAlertCode_markerWasteInkReceptacleFull 1110
#define have_prtAlertCode_markerOpcLifeAlmostOver 1111
#define have_prtAlertCode_markerOpcLifeOver 1112
#define have_prtAlertCode_markerDeveloperAlmostEmpty 1113
#define have_prtAlertCode_markerDeveloperEmpty 1114
#define have_prtAlertCode_mediaPathMediaTrayMissing 1301
#define have_prtAlertCode_mediaPathMediaTrayAlmostFull 1302
#define have_prtAlertCode_mediaPathMediaTrayFull 1303
#define have_prtAlertCode_interpreterMemoryIncrease 1501
#define have_prtAlertCode_interpreterMemoryDecrease 1502
#define have_prtAlertCode_interpreterCartridgeAdded 1503
#define have_prtAlertCode_interpreterCartridgeDeleted 1504
#define have_prtAlertCode_interpreterResourceAdded 1505
#define have_prtAlertCode_interpreterResourceDeleted 1506
#define have_prtAlertCode_interpreterResourceUnavailable 1507


#define ne_prtAlertCode	49
extern int ve_prtAlertCode[49];


/* Declarations for storage for multi-dim properties
*/
/* storage for string value(s) for NVRAMSIGNATURE
*/
#define MAX_NVRAMSIGNATURE_LEN		32
extern       uint8_t _vvs_NVRAMSIGNATURE[33];
extern const uint8_t _fvs_NVRAMSIGNATURE[/*max of 33*/];


/* storage for string value(s) for sysDescr
*/
#define MAX_sysDescr_LEN		255
extern       uint8_t _vvs_sysDescr[256];
extern const uint8_t _fvs_sysDescr[/*max of 256*/];


/* storage for string value(s) for sysObjectID
*/
#define MAX_sysObjectID_LEN		255
extern       uint8_t _vvs_sysObjectID[256];
extern const uint8_t _fvs_sysObjectID[/*max of 256*/];


/* storage for string value(s) for sysContact
*/
#define MAX_sysContact_LEN		255
extern       uint8_t _vvs_sysContact[256];
extern const uint8_t _fvs_sysContact[/*max of 256*/];


/* storage for string value(s) for sysName
*/
#define MAX_sysName_LEN		255
extern       uint8_t _vvs_sysName[256];
extern const uint8_t _fvs_sysName[/*max of 256*/];


/* storage for string value(s) for sysLocation
*/
#define MAX_sysLocation_LEN		255
extern       uint8_t _vvs_sysLocation[256];
extern const uint8_t _fvs_sysLocation[/*max of 256*/];


/* storage for multi-dimensional property sysORIndex
*/
extern       int _vva_sysORIndex[1];
extern const int _fva_sysORIndex[1];


/* storage for string value(s) for sysORID
*/
#define MAX_sysORID_LEN		255
extern       uint8_t _vvs_sysORID[256];
extern const uint8_t _fvs_sysORID[/*max of 256*/];


/* storage for string value(s) for sysORDescr
*/
#define MAX_sysORDescr_LEN		255
extern       uint8_t _vvs_sysORDescr[256];
extern const uint8_t _fvs_sysORDescr[/*max of 256*/];


/* storage for multi-dimensional property sysORUpTime
*/
extern       uint32_t _vva_sysORUpTime[1];
extern const uint32_t _fva_sysORUpTime[1];


/* storage for multi-dimensional property ifIndex
*/
extern       int _vva_ifIndex[1];
extern const int _fva_ifIndex[1];


/* storage for string value(s) for ifDescr
*/
#define MAX_ifDescr_LEN		255
extern       uint8_t _vvs_ifDescr[256];
extern const uint8_t _fvs_ifDescr[/*max of 256*/];


/* storage for multi-dimensional property ifType
*/
extern       int _vva_ifType[1];
extern const int _fva_ifType[1];


/* storage for multi-dimensional property ifMtu
*/
extern       int _vva_ifMtu[1];
extern const int _fva_ifMtu[1];


/* storage for multi-dimensional property ifSpeed
*/
extern       uint32_t _vva_ifSpeed[1];
extern const uint32_t _fva_ifSpeed[1];


/* storage for string value(s) for ifPhysAddress
*/
#define MAX_ifPhysAddress_LEN		255
extern       uint8_t _vvs_ifPhysAddress[256];
extern const uint8_t _fvs_ifPhysAddress[/*max of 256*/];


/* storage for multi-dimensional property ifAdminStatus
*/
extern       int _vva_ifAdminStatus[1];
extern const int _fva_ifAdminStatus[1];


/* storage for multi-dimensional property ifOperStatus
*/
extern       int _vva_ifOperStatus[1];
extern const int _fva_ifOperStatus[1];


/* storage for multi-dimensional property ifLastChange
*/
extern       uint32_t _vva_ifLastChange[1];
extern const uint32_t _fva_ifLastChange[1];


/* storage for multi-dimensional property ifInOctets
*/
extern       uint32_t _vva_ifInOctets[1];
extern const uint32_t _fva_ifInOctets[1];


/* storage for multi-dimensional property ifInUcastPkts
*/
extern       uint32_t _vva_ifInUcastPkts[1];
extern const uint32_t _fva_ifInUcastPkts[1];


/* storage for multi-dimensional property ifInNUcastPkts
*/
extern       uint32_t _vva_ifInNUcastPkts[1];
extern const uint32_t _fva_ifInNUcastPkts[1];


/* storage for multi-dimensional property ifInDiscards
*/
extern       uint32_t _vva_ifInDiscards[1];
extern const uint32_t _fva_ifInDiscards[1];


/* storage for multi-dimensional property ifInErrors
*/
extern       uint32_t _vva_ifInErrors[1];
extern const uint32_t _fva_ifInErrors[1];


/* storage for multi-dimensional property ifInUnknownProtos
*/
extern       uint32_t _vva_ifInUnknownProtos[1];
extern const uint32_t _fva_ifInUnknownProtos[1];


/* storage for multi-dimensional property ifOutOctets
*/
extern       uint32_t _vva_ifOutOctets[1];
extern const uint32_t _fva_ifOutOctets[1];


/* storage for multi-dimensional property ifOutUcastPkts
*/
extern       uint32_t _vva_ifOutUcastPkts[1];
extern const uint32_t _fva_ifOutUcastPkts[1];


/* storage for multi-dimensional property ifOutNUcastPkts
*/
extern       uint32_t _vva_ifOutNUcastPkts[1];
extern const uint32_t _fva_ifOutNUcastPkts[1];


/* storage for multi-dimensional property ifOutDiscards
*/
extern       uint32_t _vva_ifOutDiscards[1];
extern const uint32_t _fva_ifOutDiscards[1];


/* storage for multi-dimensional property ifOutErrors
*/
extern       uint32_t _vva_ifOutErrors[1];
extern const uint32_t _fva_ifOutErrors[1];


/* storage for multi-dimensional property ifOutQLen
*/
extern       uint32_t _vva_ifOutQLen[1];
extern const uint32_t _fva_ifOutQLen[1];


/* storage for string value(s) for ifSpecific
*/
#define MAX_ifSpecific_LEN		255
extern       uint8_t _vvs_ifSpecific[256];
extern const uint8_t _fvs_ifSpecific[/*max of 256*/];


/* storage for string value(s) for hrSystemDate
*/
#define MAX_hrSystemDate_LEN		11
extern       uint8_t _vvs_hrSystemDate[12];
extern const uint8_t _fvs_hrSystemDate[/*max of 12*/];


/* storage for string value(s) for hrSystemInitialLoadParameters
*/
#define MAX_hrSystemInitialLoadParameters_LEN		128
extern       uint8_t _vvs_hrSystemInitialLoadParameters[129];
extern const uint8_t _fvs_hrSystemInitialLoadParameters[/*max of 129*/];


/* storage for multi-dimensional property hrStorageIndex
*/
extern       int _vva_hrStorageIndex[1];
extern const int _fva_hrStorageIndex[1];


/* storage for string value(s) for hrStorageType
*/
#define MAX_hrStorageType_LEN		255
extern       uint8_t _vvs_hrStorageType[256];
extern const uint8_t _fvs_hrStorageType[/*max of 256*/];


/* storage for string value(s) for hrStorageDescr
*/
#define MAX_hrStorageDescr_LEN		255
extern       uint8_t _vvs_hrStorageDescr[256];
extern const uint8_t _fvs_hrStorageDescr[/*max of 256*/];


/* storage for multi-dimensional property hrStorageAllocationUnits
*/
extern       int _vva_hrStorageAllocationUnits[1];
extern const int _fva_hrStorageAllocationUnits[1];


/* storage for multi-dimensional property hrStorageSize
*/
extern       int _vva_hrStorageSize[1];
extern const int _fva_hrStorageSize[1];


/* storage for multi-dimensional property hrStorageUsed
*/
extern       int _vva_hrStorageUsed[1];
extern const int _fva_hrStorageUsed[1];


/* storage for multi-dimensional property hrStorageAllocationFailures
*/
extern       uint32_t _vva_hrStorageAllocationFailures[1];
extern const uint32_t _fva_hrStorageAllocationFailures[1];


/* storage for multi-dimensional property hrDeviceIndex
*/
extern       int _vva_hrDeviceIndex[1];
extern const int _fva_hrDeviceIndex[1];


/* storage for string value(s) for hrDeviceType
*/
#define MAX_hrDeviceType_LEN		255
extern       uint8_t _vvs_hrDeviceType[256];
extern const uint8_t _fvs_hrDeviceType[/*max of 256*/];


/* storage for string value(s) for hrDeviceDescr
*/
#define MAX_hrDeviceDescr_LEN		64
extern       uint8_t _vvs_hrDeviceDescr[65];
extern const uint8_t _fvs_hrDeviceDescr[/*max of 65*/];


/* storage for string value(s) for hrDeviceID
*/
#define MAX_hrDeviceID_LEN		255
extern       uint8_t _vvs_hrDeviceID[256];
extern const uint8_t _fvs_hrDeviceID[/*max of 256*/];


/* storage for multi-dimensional property hrDeviceStatus
*/
extern       int _vva_hrDeviceStatus[1];
extern const int _fva_hrDeviceStatus[1];


/* storage for multi-dimensional property hrDeviceErrors
*/
extern       uint32_t _vva_hrDeviceErrors[1];
extern const uint32_t _fva_hrDeviceErrors[1];


/* storage for string value(s) for hrProcessorFrwID
*/
#define MAX_hrProcessorFrwID_LEN		255
extern       uint8_t _vvs_hrProcessorFrwID[256];
extern const uint8_t _fvs_hrProcessorFrwID[/*max of 256*/];


/* storage for multi-dimensional property hrProcessorLoad
*/
extern       int _vva_hrProcessorLoad[1];
extern const int _fva_hrProcessorLoad[1];


/* storage for multi-dimensional property hrNetworkIfIndex
*/
extern       int _vva_hrNetworkIfIndex[1];
extern const int _fva_hrNetworkIfIndex[1];


/* storage for multi-dimensional property hrPrinterStatus
*/
extern       int _vva_hrPrinterStatus[1];
extern const int _fva_hrPrinterStatus[1];


/* storage for string value(s) for hrPrinterDetectedErrorState
*/
#define MAX_hrPrinterDetectedErrorState_LEN		255
extern       uint8_t _vvs_hrPrinterDetectedErrorState[256];
extern const uint8_t _fvs_hrPrinterDetectedErrorState[/*max of 256*/];


/* storage for multi-dimensional property hrDiskStorageAccess
*/
extern       int _vva_hrDiskStorageAccess[1];
extern const int _fva_hrDiskStorageAccess[1];


/* storage for multi-dimensional property hrDiskStorageMedia
*/
extern       int _vva_hrDiskStorageMedia[1];
extern const int _fva_hrDiskStorageMedia[1];


/* storage for multi-dimensional property hrDiskStorageRemoveble
*/
extern       int _vva_hrDiskStorageRemoveble[1];
extern const int _fva_hrDiskStorageRemoveble[1];


/* storage for multi-dimensional property hrDiskStorageCapacity
*/
extern       int _vva_hrDiskStorageCapacity[1];
extern const int _fva_hrDiskStorageCapacity[1];


/* storage for multi-dimensional property hrPartitionIndex
*/
extern       int _vva_hrPartitionIndex[1];
extern const int _fva_hrPartitionIndex[1];


/* storage for string value(s) for hrPartitionLabel
*/
#define MAX_hrPartitionLabel_LEN		128
extern       uint8_t _vvs_hrPartitionLabel[129];
extern const uint8_t _fvs_hrPartitionLabel[/*max of 129*/];


/* storage for string value(s) for hrPartitionID
*/
#define MAX_hrPartitionID_LEN		255
extern       uint8_t _vvs_hrPartitionID[256];
extern const uint8_t _fvs_hrPartitionID[/*max of 256*/];


/* storage for multi-dimensional property hrPartitionSize
*/
extern       int _vva_hrPartitionSize[1];
extern const int _fva_hrPartitionSize[1];


/* storage for multi-dimensional property hrPartitionFSIndex
*/
extern       int _vva_hrPartitionFSIndex[1];
extern const int _fva_hrPartitionFSIndex[1];


/* storage for multi-dimensional property hrFSIndex
*/
extern       int _vva_hrFSIndex[1];
extern const int _fva_hrFSIndex[1];


/* storage for string value(s) for hrFSMountPoint
*/
#define MAX_hrFSMountPoint_LEN		128
extern       uint8_t _vvs_hrFSMountPoint[129];
extern const uint8_t _fvs_hrFSMountPoint[/*max of 129*/];


/* storage for string value(s) for hrFSRemoteMountPoint
*/
#define MAX_hrFSRemoteMountPoint_LEN		128
extern       uint8_t _vvs_hrFSRemoteMountPoint[129];
extern const uint8_t _fvs_hrFSRemoteMountPoint[/*max of 129*/];


/* storage for string value(s) for hrFSType
*/
#define MAX_hrFSType_LEN		255
extern       uint8_t _vvs_hrFSType[256];
extern const uint8_t _fvs_hrFSType[/*max of 256*/];


/* storage for multi-dimensional property hrFSAccess
*/
extern       int _vva_hrFSAccess[1];
extern const int _fva_hrFSAccess[1];


/* storage for multi-dimensional property hrFSBootable
*/
extern       int _vva_hrFSBootable[1];
extern const int _fva_hrFSBootable[1];


/* storage for multi-dimensional property hrFSStorageIndex
*/
extern       int _vva_hrFSStorageIndex[1];
extern const int _fva_hrFSStorageIndex[1];


/* storage for string value(s) for hrFSLastFullBackupDate
*/
#define MAX_hrFSLastFullBackupDate_LEN		11
extern       uint8_t _vvs_hrFSLastFullBackupDate[12];
extern const uint8_t _fvs_hrFSLastFullBackupDate[/*max of 12*/];


/* storage for string value(s) for hrFSLastPartialBackupDate
*/
#define MAX_hrFSLastPartialBackupDate_LEN		11
extern       uint8_t _vvs_hrFSLastPartialBackupDate[12];
extern const uint8_t _fvs_hrFSLastPartialBackupDate[/*max of 12*/];


/* storage for multi-dimensional property hrSWRunIndex
*/
extern       int _vva_hrSWRunIndex[1];
extern const int _fva_hrSWRunIndex[1];


/* storage for string value(s) for hrSWRunName
*/
#define MAX_hrSWRunName_LEN		64
extern       uint8_t _vvs_hrSWRunName[65];
extern const uint8_t _fvs_hrSWRunName[/*max of 65*/];


/* storage for string value(s) for hrSWRunID
*/
#define MAX_hrSWRunID_LEN		255
extern       uint8_t _vvs_hrSWRunID[256];
extern const uint8_t _fvs_hrSWRunID[/*max of 256*/];


/* storage for string value(s) for hrSWRunPath
*/
#define MAX_hrSWRunPath_LEN		128
extern       uint8_t _vvs_hrSWRunPath[129];
extern const uint8_t _fvs_hrSWRunPath[/*max of 129*/];


/* storage for string value(s) for hrSWRunParameters
*/
#define MAX_hrSWRunParameters_LEN		128
extern       uint8_t _vvs_hrSWRunParameters[129];
extern const uint8_t _fvs_hrSWRunParameters[/*max of 129*/];


/* storage for multi-dimensional property hrSWRunType
*/
extern       int _vva_hrSWRunType[1];
extern const int _fva_hrSWRunType[1];


/* storage for multi-dimensional property hrSWRunStatus
*/
extern       int _vva_hrSWRunStatus[1];
extern const int _fva_hrSWRunStatus[1];


/* storage for multi-dimensional property hrSWInstalledIndex
*/
extern       int _vva_hrSWInstalledIndex[1];
extern const int _fva_hrSWInstalledIndex[1];


/* storage for string value(s) for hrSWInstalledName
*/
#define MAX_hrSWInstalledName_LEN		64
extern       uint8_t _vvs_hrSWInstalledName[65];
extern const uint8_t _fvs_hrSWInstalledName[/*max of 65*/];


/* storage for string value(s) for hrSWInstalledID
*/
#define MAX_hrSWInstalledID_LEN		255
extern       uint8_t _vvs_hrSWInstalledID[256];
extern const uint8_t _fvs_hrSWInstalledID[/*max of 256*/];


/* storage for multi-dimensional property hrSWInstalledType
*/
extern       int _vva_hrSWInstalledType[1];
extern const int _fva_hrSWInstalledType[1];


/* storage for string value(s) for hrSWInstalledDate
*/
#define MAX_hrSWInstalledDate_LEN		11
extern       uint8_t _vvs_hrSWInstalledDate[12];
extern const uint8_t _fvs_hrSWInstalledDate[/*max of 12*/];


/* storage for string value(s) for ifName
*/
#define MAX_ifName_LEN		255
extern       uint8_t _vvs_ifName[256];
extern const uint8_t _fvs_ifName[/*max of 256*/];


/* storage for property ifHCInOctets which has encoding more than 32 bits
*/
extern       uint8_t _vva_ifHCInOctets[9];
extern const uint8_t _fva_ifHCInOctets[9];


/* storage for property ifHCInUcastPkts which has encoding more than 32 bits
*/
extern       uint8_t _vva_ifHCInUcastPkts[9];
extern const uint8_t _fva_ifHCInUcastPkts[9];


/* storage for property ifHCInMulticastPkts which has encoding more than 32 bits
*/
extern       uint8_t _vva_ifHCInMulticastPkts[9];
extern const uint8_t _fva_ifHCInMulticastPkts[9];


/* storage for property ifHCInBroadcastPkts which has encoding more than 32 bits
*/
extern       uint8_t _vva_ifHCInBroadcastPkts[9];
extern const uint8_t _fva_ifHCInBroadcastPkts[9];


/* storage for property ifHCOutOctets which has encoding more than 32 bits
*/
extern       uint8_t _vva_ifHCOutOctets[9];
extern const uint8_t _fva_ifHCOutOctets[9];


/* storage for property ifHCOutUcastPkts which has encoding more than 32 bits
*/
extern       uint8_t _vva_ifHCOutUcastPkts[9];
extern const uint8_t _fva_ifHCOutUcastPkts[9];


/* storage for property ifHCOutMulticastPkts which has encoding more than 32 bits
*/
extern       uint8_t _vva_ifHCOutMulticastPkts[9];
extern const uint8_t _fva_ifHCOutMulticastPkts[9];


/* storage for property ifHCOutBroadcastPkts which has encoding more than 32 bits
*/
extern       uint8_t _vva_ifHCOutBroadcastPkts[9];
extern const uint8_t _fva_ifHCOutBroadcastPkts[9];


/* storage for string value(s) for ifAlias
*/
#define MAX_ifAlias_LEN		64
extern       uint8_t _vvs_ifAlias[65];
extern const uint8_t _fvs_ifAlias[/*max of 65*/];


/* storage for multi-dimensional property ifStackHigherLayer
*/
extern       int _vva_ifStackHigherLayer[1];
extern const int _fva_ifStackHigherLayer[1];


/* storage for multi-dimensional property ifStackLowerLayer
*/
extern       int _vva_ifStackLowerLayer[1];
extern const int _fva_ifStackLowerLayer[1];


/* storage for multi-dimensional property ifStackStatus
*/
extern       int _vva_ifStackStatus[1];
extern const int _fva_ifStackStatus[1];


/* storage for multi-dimensional property prtGeneralConfigChanges
*/
extern       uint32_t _vva_prtGeneralConfigChanges[1];
extern const uint32_t _fva_prtGeneralConfigChanges[1];


/* storage for multi-dimensional property prtGeneralCurrentLocalization
*/
extern       int _vva_prtGeneralCurrentLocalization[1];
extern const int _fva_prtGeneralCurrentLocalization[1];


/* storage for multi-dimensional property prtGeneralReset
*/
extern       int _vva_prtGeneralReset[1];
extern const int _fva_prtGeneralReset[1];


/* storage for string value(s) for prtGeneralCurrentOperator
*/
#define MAX_prtGeneralCurrentOperator_LEN		127
extern       uint8_t _vvs_prtGeneralCurrentOperator[128];
extern const uint8_t _fvs_prtGeneralCurrentOperator[/*max of 128*/];


/* storage for string value(s) for prtGeneralServicePerson
*/
#define MAX_prtGeneralServicePerson_LEN		127
extern       uint8_t _vvs_prtGeneralServicePerson[128];
extern const uint8_t _fvs_prtGeneralServicePerson[/*max of 128*/];


/* storage for multi-dimensional property prtInputDefaultIndex
*/
extern       int _vva_prtInputDefaultIndex[1];
extern const int _fva_prtInputDefaultIndex[1];


/* storage for multi-dimensional property prtOutputDefaultIndex
*/
extern       int _vva_prtOutputDefaultIndex[1];
extern const int _fva_prtOutputDefaultIndex[1];


/* storage for multi-dimensional property prtMarkerDefaultIndex
*/
extern       int _vva_prtMarkerDefaultIndex[1];
extern const int _fva_prtMarkerDefaultIndex[1];


/* storage for multi-dimensional property prtMediaPathDefaultIndex
*/
extern       int _vva_prtMediaPathDefaultIndex[1];
extern const int _fva_prtMediaPathDefaultIndex[1];


/* storage for multi-dimensional property prtConsoleLocalization
*/
extern       int _vva_prtConsoleLocalization[1];
extern const int _fva_prtConsoleLocalization[1];


/* storage for multi-dimensional property prtConsoleNumberOfDisplayLines
*/
extern       int _vva_prtConsoleNumberOfDisplayLines[1];
extern const int _fva_prtConsoleNumberOfDisplayLines[1];


/* storage for multi-dimensional property prtConsoleNumberOfDisplayChars
*/
extern       int _vva_prtConsoleNumberOfDisplayChars[1];
extern const int _fva_prtConsoleNumberOfDisplayChars[1];


/* storage for multi-dimensional property prtConsoleDisable
*/
extern       int _vva_prtConsoleDisable[1];
extern const int _fva_prtConsoleDisable[1];


/* storage for multi-dimensional property prtStorageRefSeqNumber
*/
extern       int _vva_prtStorageRefSeqNumber[1];
extern const int _fva_prtStorageRefSeqNumber[1];


/* storage for multi-dimensional property prtStorageRefIndex
*/
extern       int _vva_prtStorageRefIndex[1];
extern const int _fva_prtStorageRefIndex[1];


/* storage for multi-dimensional property prtDeviceRefSeqNumber
*/
extern       int _vva_prtDeviceRefSeqNumber[1];
extern const int _fva_prtDeviceRefSeqNumber[1];


/* storage for multi-dimensional property prtDeviceRefIndex
*/
extern       int _vva_prtDeviceRefIndex[1];
extern const int _fva_prtDeviceRefIndex[1];


/* storage for multi-dimensional property prtCoverIndex
*/
extern       int _vva_prtCoverIndex[1];
extern const int _fva_prtCoverIndex[1];


/* storage for string value(s) for prtCoverDescription
*/
#define MAX_prtCoverDescription_LEN		255
extern       uint8_t _vvs_prtCoverDescription[256];
extern const uint8_t _fvs_prtCoverDescription[/*max of 256*/];


/* storage for multi-dimensional property prtCoverStatus
*/
extern       int _vva_prtCoverStatus[1];
extern const int _fva_prtCoverStatus[1];


/* storage for multi-dimensional property prtLocalizationIndex
*/
extern       int _vva_prtLocalizationIndex[1];
extern const int _fva_prtLocalizationIndex[1];


/* storage for string value(s) for prtLocalizationLanguage
*/
#define MAX_prtLocalizationLanguage_LEN		2
extern       uint8_t _vvs_prtLocalizationLanguage[3];
extern const uint8_t _fvs_prtLocalizationLanguage[/*max of 3*/];


/* storage for string value(s) for prtLocalizationCountry
*/
#define MAX_prtLocalizationCountry_LEN		2
extern       uint8_t _vvs_prtLocalizationCountry[3];
extern const uint8_t _fvs_prtLocalizationCountry[/*max of 3*/];


/* storage for multi-dimensional property prtLocalizationCharacterSet
*/
extern       int _vva_prtLocalizationCharacterSet[1];
extern const int _fva_prtLocalizationCharacterSet[1];


/* storage for multi-dimensional property prtInputIndex
*/
extern       int _vva_prtInputIndex[1];
extern const int _fva_prtInputIndex[1];


/* storage for multi-dimensional property prtInputType
*/
extern       int _vva_prtInputType[1];
extern const int _fva_prtInputType[1];


/* storage for multi-dimensional property prtInputDimUnit
*/
extern       int _vva_prtInputDimUnit[1];
extern const int _fva_prtInputDimUnit[1];


/* storage for multi-dimensional property prtInputMediaDimFeedDirDeclared
*/
extern       int _vva_prtInputMediaDimFeedDirDeclared[1];
extern const int _fva_prtInputMediaDimFeedDirDeclared[1];


/* storage for multi-dimensional property prtInputMediaDimXFeedDirDeclared
*/
extern       int _vva_prtInputMediaDimXFeedDirDeclared[1];
extern const int _fva_prtInputMediaDimXFeedDirDeclared[1];


/* storage for multi-dimensional property prtInputMediaDimFeedDirChosen
*/
extern       int _vva_prtInputMediaDimFeedDirChosen[1];
extern const int _fva_prtInputMediaDimFeedDirChosen[1];


/* storage for multi-dimensional property prtInputMediaDimXFeedDirChosen
*/
extern       int _vva_prtInputMediaDimXFeedDirChosen[1];
extern const int _fva_prtInputMediaDimXFeedDirChosen[1];


/* storage for multi-dimensional property prtInputCapacityUnit
*/
extern       int _vva_prtInputCapacityUnit[1];
extern const int _fva_prtInputCapacityUnit[1];


/* storage for multi-dimensional property prtInputMaxCapacity
*/
extern       int _vva_prtInputMaxCapacity[1];
extern const int _fva_prtInputMaxCapacity[1];


/* storage for multi-dimensional property prtInputCurrentLevel
*/
extern       int _vva_prtInputCurrentLevel[1];
extern const int _fva_prtInputCurrentLevel[1];


/* storage for multi-dimensional property prtInputStatus
*/
extern       int _vva_prtInputStatus[1];
extern const int _fva_prtInputStatus[1];


/* storage for string value(s) for prtInputMediaName
*/
#define MAX_prtInputMediaName_LEN		63
extern       uint8_t _vvs_prtInputMediaName[64];
extern const uint8_t _fvs_prtInputMediaName[/*max of 64*/];


/* storage for string value(s) for prtInputName
*/
#define MAX_prtInputName_LEN		63
extern       uint8_t _vvs_prtInputName[64];
extern const uint8_t _fvs_prtInputName[/*max of 64*/];


/* storage for string value(s) for prtInputVendorName
*/
#define MAX_prtInputVendorName_LEN		63
extern       uint8_t _vvs_prtInputVendorName[64];
extern const uint8_t _fvs_prtInputVendorName[/*max of 64*/];


/* storage for string value(s) for prtInputModel
*/
#define MAX_prtInputModel_LEN		63
extern       uint8_t _vvs_prtInputModel[64];
extern const uint8_t _fvs_prtInputModel[/*max of 64*/];


/* storage for string value(s) for prtInputVersion
*/
#define MAX_prtInputVersion_LEN		63
extern       uint8_t _vvs_prtInputVersion[64];
extern const uint8_t _fvs_prtInputVersion[/*max of 64*/];


/* storage for string value(s) for prtInputSerialNumber
*/
#define MAX_prtInputSerialNumber_LEN		32
extern       uint8_t _vvs_prtInputSerialNumber[33];
extern const uint8_t _fvs_prtInputSerialNumber[/*max of 33*/];


/* storage for string value(s) for prtInputDescription
*/
#define MAX_prtInputDescription_LEN		255
extern       uint8_t _vvs_prtInputDescription[256];
extern const uint8_t _fvs_prtInputDescription[/*max of 256*/];


/* storage for multi-dimensional property prtInputSecurity
*/
extern       int _vva_prtInputSecurity[1];
extern const int _fva_prtInputSecurity[1];


/* storage for multi-dimensional property prtInputMediaWeight
*/
extern       int _vva_prtInputMediaWeight[1];
extern const int _fva_prtInputMediaWeight[1];


/* storage for string value(s) for prtInputMediaType
*/
#define MAX_prtInputMediaType_LEN		63
extern       uint8_t _vvs_prtInputMediaType[64];
extern const uint8_t _fvs_prtInputMediaType[/*max of 64*/];


/* storage for string value(s) for prtInputMediaColor
*/
#define MAX_prtInputMediaColor_LEN		63
extern       uint8_t _vvs_prtInputMediaColor[64];
extern const uint8_t _fvs_prtInputMediaColor[/*max of 64*/];


/* storage for multi-dimensional property prtInputMediaFormParts
*/
extern       int _vva_prtInputMediaFormParts[1];
extern const int _fva_prtInputMediaFormParts[1];


/* storage for multi-dimensional property prtOutputIndex
*/
extern       int _vva_prtOutputIndex[1];
extern const int _fva_prtOutputIndex[1];


/* storage for multi-dimensional property prtOutputType
*/
extern       int _vva_prtOutputType[1];
extern const int _fva_prtOutputType[1];


/* storage for multi-dimensional property prtOutputCapacityUnit
*/
extern       int _vva_prtOutputCapacityUnit[1];
extern const int _fva_prtOutputCapacityUnit[1];


/* storage for multi-dimensional property prtOutputMaxCapacity
*/
extern       int _vva_prtOutputMaxCapacity[1];
extern const int _fva_prtOutputMaxCapacity[1];


/* storage for multi-dimensional property prtOutputRemainingCapacity
*/
extern       int _vva_prtOutputRemainingCapacity[1];
extern const int _fva_prtOutputRemainingCapacity[1];


/* storage for multi-dimensional property prtOutputStatus
*/
extern       int _vva_prtOutputStatus[1];
extern const int _fva_prtOutputStatus[1];


/* storage for string value(s) for prtOutputName
*/
#define MAX_prtOutputName_LEN		63
extern       uint8_t _vvs_prtOutputName[64];
extern const uint8_t _fvs_prtOutputName[/*max of 64*/];


/* storage for string value(s) for prtOutputVendorName
*/
#define MAX_prtOutputVendorName_LEN		63
extern       uint8_t _vvs_prtOutputVendorName[64];
extern const uint8_t _fvs_prtOutputVendorName[/*max of 64*/];


/* storage for string value(s) for prtOutputModel
*/
#define MAX_prtOutputModel_LEN		63
extern       uint8_t _vvs_prtOutputModel[64];
extern const uint8_t _fvs_prtOutputModel[/*max of 64*/];


/* storage for string value(s) for prtOutputVersion
*/
#define MAX_prtOutputVersion_LEN		63
extern       uint8_t _vvs_prtOutputVersion[64];
extern const uint8_t _fvs_prtOutputVersion[/*max of 64*/];


/* storage for string value(s) for prtOutputSerialNumber
*/
#define MAX_prtOutputSerialNumber_LEN		63
extern       uint8_t _vvs_prtOutputSerialNumber[64];
extern const uint8_t _fvs_prtOutputSerialNumber[/*max of 64*/];


/* storage for string value(s) for prtOutputDescription
*/
#define MAX_prtOutputDescription_LEN		255
extern       uint8_t _vvs_prtOutputDescription[256];
extern const uint8_t _fvs_prtOutputDescription[/*max of 256*/];


/* storage for multi-dimensional property prtOutputSecurity
*/
extern       int _vva_prtOutputSecurity[1];
extern const int _fva_prtOutputSecurity[1];


/* storage for multi-dimensional property prtOutputDimUnit
*/
extern       int _vva_prtOutputDimUnit[1];
extern const int _fva_prtOutputDimUnit[1];


/* storage for multi-dimensional property prtOutputMaxDimFeedDir
*/
extern       int _vva_prtOutputMaxDimFeedDir[1];
extern const int _fva_prtOutputMaxDimFeedDir[1];


/* storage for multi-dimensional property prtOutputMaxDimXFeedDir
*/
extern       int _vva_prtOutputMaxDimXFeedDir[1];
extern const int _fva_prtOutputMaxDimXFeedDir[1];


/* storage for multi-dimensional property prtOutputMinDimFeedDir
*/
extern       int _vva_prtOutputMinDimFeedDir[1];
extern const int _fva_prtOutputMinDimFeedDir[1];


/* storage for multi-dimensional property prtOutputMinDimXFeedDir
*/
extern       int _vva_prtOutputMinDimXFeedDir[1];
extern const int _fva_prtOutputMinDimXFeedDir[1];


/* storage for multi-dimensional property prtOutputStackingOrder
*/
extern       int _vva_prtOutputStackingOrder[1];
extern const int _fva_prtOutputStackingOrder[1];


/* storage for multi-dimensional property prtOutputPageDeliveryOrientation
*/
extern       int _vva_prtOutputPageDeliveryOrientation[1];
extern const int _fva_prtOutputPageDeliveryOrientation[1];


/* storage for multi-dimensional property prtOutputBursting
*/
extern       int _vva_prtOutputBursting[1];
extern const int _fva_prtOutputBursting[1];


/* storage for multi-dimensional property prtOutputDecollating
*/
extern       int _vva_prtOutputDecollating[1];
extern const int _fva_prtOutputDecollating[1];


/* storage for multi-dimensional property prtOutputPageCollated
*/
extern       int _vva_prtOutputPageCollated[1];
extern const int _fva_prtOutputPageCollated[1];


/* storage for multi-dimensional property prtOutputOffsetStacking
*/
extern       int _vva_prtOutputOffsetStacking[1];
extern const int _fva_prtOutputOffsetStacking[1];


/* storage for multi-dimensional property prtMarkerIndex
*/
extern       int _vva_prtMarkerIndex[1];
extern const int _fva_prtMarkerIndex[1];


/* storage for multi-dimensional property prtMarkerMarkTech
*/
extern       int _vva_prtMarkerMarkTech[1];
extern const int _fva_prtMarkerMarkTech[1];


/* storage for multi-dimensional property prtMarkerCounterUnit
*/
extern       int _vva_prtMarkerCounterUnit[1];
extern const int _fva_prtMarkerCounterUnit[1];


/* storage for multi-dimensional property prtMarkerLifeCount
*/
extern       uint32_t _vva_prtMarkerLifeCount[1];
extern const uint32_t _fva_prtMarkerLifeCount[1];


/* storage for multi-dimensional property prtMarkerPowerOnCount
*/
extern       uint32_t _vva_prtMarkerPowerOnCount[1];
extern const uint32_t _fva_prtMarkerPowerOnCount[1];


/* storage for multi-dimensional property prtMarkerProcessColorants
*/
extern       int _vva_prtMarkerProcessColorants[1];
extern const int _fva_prtMarkerProcessColorants[1];


/* storage for multi-dimensional property prtMarkerSpotColorants
*/
extern       int _vva_prtMarkerSpotColorants[1];
extern const int _fva_prtMarkerSpotColorants[1];


/* storage for multi-dimensional property prtMarkerAddressabilityUnit
*/
extern       int _vva_prtMarkerAddressabilityUnit[1];
extern const int _fva_prtMarkerAddressabilityUnit[1];


/* storage for multi-dimensional property prtMarkerAddressabilityFeedDir
*/
extern       int _vva_prtMarkerAddressabilityFeedDir[1];
extern const int _fva_prtMarkerAddressabilityFeedDir[1];


/* storage for multi-dimensional property prtMarkerAddressabilityXFeedDir
*/
extern       int _vva_prtMarkerAddressabilityXFeedDir[1];
extern const int _fva_prtMarkerAddressabilityXFeedDir[1];


/* storage for multi-dimensional property prtMarkerNorthMargin
*/
extern       int _vva_prtMarkerNorthMargin[1];
extern const int _fva_prtMarkerNorthMargin[1];


/* storage for multi-dimensional property prtMarkerSouthMargin
*/
extern       int _vva_prtMarkerSouthMargin[1];
extern const int _fva_prtMarkerSouthMargin[1];


/* storage for multi-dimensional property prtMarkerWestMargin
*/
extern       int _vva_prtMarkerWestMargin[1];
extern const int _fva_prtMarkerWestMargin[1];


/* storage for multi-dimensional property prtMarkerEastMargin
*/
extern       int _vva_prtMarkerEastMargin[1];
extern const int _fva_prtMarkerEastMargin[1];


/* storage for multi-dimensional property prtMarkerStatus
*/
extern       int _vva_prtMarkerStatus[1];
extern const int _fva_prtMarkerStatus[1];


/* storage for multi-dimensional property prtMarkerSuppliesIndex
*/
extern       int _vva_prtMarkerSuppliesIndex[1];
extern const int _fva_prtMarkerSuppliesIndex[1];


/* storage for multi-dimensional property prtMarkerSuppliesMarkerIndex
*/
extern       int _vva_prtMarkerSuppliesMarkerIndex[1];
extern const int _fva_prtMarkerSuppliesMarkerIndex[1];


/* storage for multi-dimensional property prtMarkerSuppliesColorantIndex
*/
extern       int _vva_prtMarkerSuppliesColorantIndex[1];
extern const int _fva_prtMarkerSuppliesColorantIndex[1];


/* storage for multi-dimensional property prtMarkerSuppliesClass
*/
extern       int _vva_prtMarkerSuppliesClass[1];
extern const int _fva_prtMarkerSuppliesClass[1];


/* storage for multi-dimensional property prtMarkerSuppliesType
*/
extern       int _vva_prtMarkerSuppliesType[1];
extern const int _fva_prtMarkerSuppliesType[1];


/* storage for string value(s) for prtMarkerSuppliesDescription
*/
#define MAX_prtMarkerSuppliesDescription_LEN		255
extern       uint8_t _vvs_prtMarkerSuppliesDescription[256];
extern const uint8_t _fvs_prtMarkerSuppliesDescription[/*max of 256*/];


/* storage for multi-dimensional property prtMarkerSuppliesSupplyUnit
*/
extern       int _vva_prtMarkerSuppliesSupplyUnit[1];
extern const int _fva_prtMarkerSuppliesSupplyUnit[1];


/* storage for multi-dimensional property prtMarkerSuppliesMaxCapacity
*/
extern       int _vva_prtMarkerSuppliesMaxCapacity[1];
extern const int _fva_prtMarkerSuppliesMaxCapacity[1];


/* storage for multi-dimensional property prtMarkerSuppliesLevel
*/
extern       int _vva_prtMarkerSuppliesLevel[1];
extern const int _fva_prtMarkerSuppliesLevel[1];


/* storage for multi-dimensional property prtMarkerColorantIndex
*/
extern       int _vva_prtMarkerColorantIndex[1];
extern const int _fva_prtMarkerColorantIndex[1];


/* storage for multi-dimensional property prtMarkerColorantMarkerIndex
*/
extern       int _vva_prtMarkerColorantMarkerIndex[1];
extern const int _fva_prtMarkerColorantMarkerIndex[1];


/* storage for multi-dimensional property prtMarkerColorantRole
*/
extern       int _vva_prtMarkerColorantRole[1];
extern const int _fva_prtMarkerColorantRole[1];


/* storage for string value(s) for prtMarkerColorantValue
*/
#define MAX_prtMarkerColorantValue_LEN		63
extern       uint8_t _vvs_prtMarkerColorantValue[64];
extern const uint8_t _fvs_prtMarkerColorantValue[/*max of 64*/];


/* storage for multi-dimensional property prtMarkerColorantTonality
*/
extern       int _vva_prtMarkerColorantTonality[1];
extern const int _fva_prtMarkerColorantTonality[1];


/* storage for multi-dimensional property prtMediaPathIndex
*/
extern       int _vva_prtMediaPathIndex[1];
extern const int _fva_prtMediaPathIndex[1];


/* storage for multi-dimensional property prtMediaPathMaxSpeedPrintUnit
*/
extern       int _vva_prtMediaPathMaxSpeedPrintUnit[1];
extern const int _fva_prtMediaPathMaxSpeedPrintUnit[1];


/* storage for multi-dimensional property prtMediaPathMediaSizeUnit
*/
extern       int _vva_prtMediaPathMediaSizeUnit[1];
extern const int _fva_prtMediaPathMediaSizeUnit[1];


/* storage for multi-dimensional property prtMediaPathMaxSpeed
*/
extern       int _vva_prtMediaPathMaxSpeed[1];
extern const int _fva_prtMediaPathMaxSpeed[1];


/* storage for multi-dimensional property prtMediaPathMaxMediaFeedDir
*/
extern       int _vva_prtMediaPathMaxMediaFeedDir[1];
extern const int _fva_prtMediaPathMaxMediaFeedDir[1];


/* storage for multi-dimensional property prtMediaPathMaxMediaXFeedDir
*/
extern       int _vva_prtMediaPathMaxMediaXFeedDir[1];
extern const int _fva_prtMediaPathMaxMediaXFeedDir[1];


/* storage for multi-dimensional property prtMediaPathMinMediaFeedDir
*/
extern       int _vva_prtMediaPathMinMediaFeedDir[1];
extern const int _fva_prtMediaPathMinMediaFeedDir[1];


/* storage for multi-dimensional property prtMediaPathMinMediaXFeedDir
*/
extern       int _vva_prtMediaPathMinMediaXFeedDir[1];
extern const int _fva_prtMediaPathMinMediaXFeedDir[1];


/* storage for multi-dimensional property prtMediaPathType
*/
extern       int _vva_prtMediaPathType[1];
extern const int _fva_prtMediaPathType[1];


/* storage for string value(s) for prtMediaPathDescription
*/
#define MAX_prtMediaPathDescription_LEN		255
extern       uint8_t _vvs_prtMediaPathDescription[256];
extern const uint8_t _fvs_prtMediaPathDescription[/*max of 256*/];


/* storage for multi-dimensional property prtMediaPathStatus
*/
extern       int _vva_prtMediaPathStatus[1];
extern const int _fva_prtMediaPathStatus[1];


/* storage for multi-dimensional property prtChannelIndex
*/
extern       int _vva_prtChannelIndex[1];
extern const int _fva_prtChannelIndex[1];


/* storage for multi-dimensional property prtChannelType
*/
extern       int _vva_prtChannelType[1];
extern const int _fva_prtChannelType[1];


/* storage for string value(s) for prtChannelProtocolVersion
*/
#define MAX_prtChannelProtocolVersion_LEN		63
extern       uint8_t _vvs_prtChannelProtocolVersion[64];
extern const uint8_t _fvs_prtChannelProtocolVersion[/*max of 64*/];


/* storage for multi-dimensional property prtChannelCurrentJobCntlLangIndex
*/
extern       int _vva_prtChannelCurrentJobCntlLangIndex[1];
extern const int _fva_prtChannelCurrentJobCntlLangIndex[1];


/* storage for multi-dimensional property prtChannelDefaultPageDescLangIndex
*/
extern       int _vva_prtChannelDefaultPageDescLangIndex[1];
extern const int _fva_prtChannelDefaultPageDescLangIndex[1];


/* storage for multi-dimensional property prtChannelState
*/
extern       int _vva_prtChannelState[1];
extern const int _fva_prtChannelState[1];


/* storage for multi-dimensional property prtChannelIfIndex
*/
extern       int _vva_prtChannelIfIndex[1];
extern const int _fva_prtChannelIfIndex[1];


/* storage for multi-dimensional property prtChannelStatus
*/
extern       int _vva_prtChannelStatus[1];
extern const int _fva_prtChannelStatus[1];


/* storage for multi-dimensional property prtInterpreterIndex
*/
extern       int _vva_prtInterpreterIndex[1];
extern const int _fva_prtInterpreterIndex[1];


/* storage for multi-dimensional property prtInterpreterLangFamily
*/
extern       int _vva_prtInterpreterLangFamily[1];
extern const int _fva_prtInterpreterLangFamily[1];


/* storage for string value(s) for prtInterpreterLangLevel
*/
#define MAX_prtInterpreterLangLevel_LEN		31
extern       uint8_t _vvs_prtInterpreterLangLevel[32];
extern const uint8_t _fvs_prtInterpreterLangLevel[/*max of 32*/];


/* storage for string value(s) for prtInterpreterLangVersion
*/
#define MAX_prtInterpreterLangVersion_LEN		31
extern       uint8_t _vvs_prtInterpreterLangVersion[32];
extern const uint8_t _fvs_prtInterpreterLangVersion[/*max of 32*/];


/* storage for string value(s) for prtInterpreterDescription
*/
#define MAX_prtInterpreterDescription_LEN		255
extern       uint8_t _vvs_prtInterpreterDescription[256];
extern const uint8_t _fvs_prtInterpreterDescription[/*max of 256*/];


/* storage for string value(s) for prtInterpreterVersion
*/
#define MAX_prtInterpreterVersion_LEN		31
extern       uint8_t _vvs_prtInterpreterVersion[32];
extern const uint8_t _fvs_prtInterpreterVersion[/*max of 32*/];


/* storage for multi-dimensional property prtInterpreterDefaultOrientation
*/
extern       int _vva_prtInterpreterDefaultOrientation[1];
extern const int _fva_prtInterpreterDefaultOrientation[1];


/* storage for multi-dimensional property prtInterpreterFeedAddressability
*/
extern       int _vva_prtInterpreterFeedAddressability[1];
extern const int _fva_prtInterpreterFeedAddressability[1];


/* storage for multi-dimensional property prtInterpreterXFeedAddressability
*/
extern       int _vva_prtInterpreterXFeedAddressability[1];
extern const int _fva_prtInterpreterXFeedAddressability[1];


/* storage for multi-dimensional property prtInterpreterDefaultCharSetIn
*/
extern       int _vva_prtInterpreterDefaultCharSetIn[1];
extern const int _fva_prtInterpreterDefaultCharSetIn[1];


/* storage for multi-dimensional property prtInterpreterDefaultCharSetOut
*/
extern       int _vva_prtInterpreterDefaultCharSetOut[1];
extern const int _fva_prtInterpreterDefaultCharSetOut[1];


/* storage for multi-dimensional property prtInterpreterTwoWay
*/
extern       int _vva_prtInterpreterTwoWay[1];
extern const int _fva_prtInterpreterTwoWay[1];


/* storage for multi-dimensional property prtConsoleDisplayBufferIndex
*/
extern       int _vva_prtConsoleDisplayBufferIndex[1];
extern const int _fva_prtConsoleDisplayBufferIndex[1];


/* storage for string value(s) for prtConsoleDisplayBufferText
*/
#define MAX_prtConsoleDisplayBufferText_LEN		255
extern       uint8_t _vvs_prtConsoleDisplayBufferText[256];
extern const uint8_t _fvs_prtConsoleDisplayBufferText[/*max of 256*/];


/* storage for multi-dimensional property prtConsoleLightIndex
*/
extern       int _vva_prtConsoleLightIndex[1];
extern const int _fva_prtConsoleLightIndex[1];


/* storage for multi-dimensional property prtConsoleOnTime
*/
extern       int _vva_prtConsoleOnTime[1];
extern const int _fva_prtConsoleOnTime[1];


/* storage for multi-dimensional property prtConsoleOffTime
*/
extern       int _vva_prtConsoleOffTime[1];
extern const int _fva_prtConsoleOffTime[1];


/* storage for multi-dimensional property prtConsoleColor
*/
extern       int _vva_prtConsoleColor[1];
extern const int _fva_prtConsoleColor[1];


/* storage for string value(s) for prtConsoleDescription
*/
#define MAX_prtConsoleDescription_LEN		255
extern       uint8_t _vvs_prtConsoleDescription[256];
extern const uint8_t _fvs_prtConsoleDescription[/*max of 256*/];


/* storage for multi-dimensional property prtAlertIndex
*/
extern       int _vva_prtAlertIndex[1];
extern const int _fva_prtAlertIndex[1];


/* storage for multi-dimensional property prtAlertSeverityLevel
*/
extern       int _vva_prtAlertSeverityLevel[1];
extern const int _fva_prtAlertSeverityLevel[1];


/* storage for multi-dimensional property prtAlertTrainingLevel
*/
extern       int _vva_prtAlertTrainingLevel[1];
extern const int _fva_prtAlertTrainingLevel[1];


/* storage for multi-dimensional property prtAlertGroup
*/
extern       int _vva_prtAlertGroup[1];
extern const int _fva_prtAlertGroup[1];


/* storage for multi-dimensional property prtAlertGroupIndex
*/
extern       int _vva_prtAlertGroupIndex[1];
extern const int _fva_prtAlertGroupIndex[1];


/* storage for multi-dimensional property prtAlertLocation
*/
extern       int _vva_prtAlertLocation[1];
extern const int _fva_prtAlertLocation[1];


/* storage for multi-dimensional property prtAlertCode
*/
extern       int _vva_prtAlertCode[1];
extern const int _fva_prtAlertCode[1];


/* storage for string value(s) for prtAlertDescription
*/
#define MAX_prtAlertDescription_LEN		255
extern       uint8_t _vvs_prtAlertDescription[256];
extern const uint8_t _fvs_prtAlertDescription[/*max of 256*/];


/* storage for multi-dimensional property prtAlertTime
*/
extern       uint32_t _vva_prtAlertTime[1];
extern const uint32_t _fva_prtAlertTime[1];


/* storage for string value(s) for snmpTrapOID
*/
#define MAX_snmpTrapOID_LEN		255
extern       uint8_t _vvs_snmpTrapOID[256];
extern const uint8_t _fvs_snmpTrapOID[/*max of 256*/];


/* storage for string value(s) for snmpTrapEnterprise
*/
#define MAX_snmpTrapEnterprise_LEN		255
extern       uint8_t _vvs_snmpTrapEnterprise[256];
extern const uint8_t _fvs_snmpTrapEnterprise[/*max of 256*/];




/* pointer to enumeration values
*/
typedef struct { int count; int* values; } bmib_enumvals_t;

extern const bmib_enumvals_t g_mib_enums[BMIBC_NUM_RECORDS];

/* Total size of allocated non-volatile bits
*/
#define BMIBC_MAX_NV_OFFSET 255
#define BMIBC_MAX_NV_BYTES  32

/* NV-RAM size, if not defined externally
*/
#ifndef BMIBC_NVRAM_SIZE
#define BMIBC_NVRAM_SIZE 32
#endif



/* OID to obejct record index cross reference table
*/
#define BMIBC_MAX_NODE_INDEX 3 /* number of indices supported */
typedef struct {
    const char *oidstr;
    size_t record_index;
    uint8_t asntype;
    size_t indices[BMIBC_MAX_NODE_INDEX];
} bmibc_oid_xref_t;


extern const bmibc_oid_xref_t g_oidxreftab[BMIBC_NUM_RECORDS];

#endif /* _BMIBC_GENERATED_H */
