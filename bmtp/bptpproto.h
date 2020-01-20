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
#ifndef BPTPPROTO_H
#define BPTPPROTO_H 1

#define MTPIP_BINARY_PROTOCOL_VERSION   0x00010000

#define PTP_INIT_REQ            			(0x01)
#define PTP_INIT_REQ_ACK        			(0x02)
#define PTP_INIT_EVENT_REQ      			(0x03)
#define PTP_INIT_EVENT_REQ_ACK  			(0x04)
#define PTP_INIT_FAIL           			(0x05)
#define PTP_CMD_REQ             			(0x06)
#define PTP_CMD_RESP            			(0x07)
#define PTP_EVENT_REQ           			(0x08)
#define PTP_START_DATA          			(0x09)
#define PTP_DATA                			(0x0A)
#define PTP_CANCEL              			(0x0B)
#define PTP_END_DATA            			(0x0C)
#define PTP_PING                			(0x0D)

// Vendor IDs
//
#define PTP_VENDOR_EASTMAN_KODAK            (0x1)
#define PTP_VENDOR_SEIKO_EPSON              (0x2)
#define PTP_VENDOR_AGILENT                  (0x3)
#define PTP_VENDOR_POLAROID                 (0x4)
#define PTP_VENDOR_AGFA_GEVAERT             (0x5)
#define PTP_VENDOR_MICROSOFT                (0x6)
#define PTP_VENDOR_EQUINOX                  (0x7)
#define PTP_VENDOR_VIEWQUEST                (0x8)
#define PTP_VENDOR_STMICROELECTRONICS       (0x9)
#define PTP_VENDOR_NIKON                    (0xA)
#define PTP_VENDOR_CANON                    (0xB)

// Operation Codes
//
#define PTP_OPCODE_Undefined                0x1000
#define PTP_OPCODE_GetDeviceInfo            0x1001
#define PTP_OPCODE_OpenSession              0x1002
#define PTP_OPCODE_CloseSession             0x1003
#define PTP_OPCODE_GetStorageIDs            0x1004
#define PTP_OPCODE_GetStorageInfo           0x1005
#define PTP_OPCODE_GetNumObjects            0x1006
#define PTP_OPCODE_GetObjectHandles         0x1007
#define PTP_OPCODE_GetObjectInfo            0x1008
#define PTP_OPCODE_GetObject                0x1009
#define PTP_OPCODE_GetThumb                 0x100A
#define PTP_OPCODE_DeleteObject             0x100B
#define PTP_OPCODE_SendObjectInfo           0x100C
#define PTP_OPCODE_SendObject               0x100D
#define PTP_OPCODE_InitiateCapture          0x100E
#define PTP_OPCODE_FormatStore              0x100F
#define PTP_OPCODE_ResetDevice              0x1010
#define PTP_OPCODE_SelfTest                 0x1011
#define PTP_OPCODE_SetObjectProtection      0x1012
#define PTP_OPCODE_PowerDown                0x1013
#define PTP_OPCODE_GetDevicePropDesc        0x1014
#define PTP_OPCODE_GetDevicePropValue       0x1015
#define PTP_OPCODE_SetDevicePropValue       0x1016
#define PTP_OPCODE_ResetDevicePropValue     0x1017
#define PTP_OPCODE_TerminateOpenCapture     0x1018
#define PTP_OPCODE_MoveObject               0x1019
#define PTP_OPCODE_CopyObject               0x101A
#define PTP_OPCODE_GetPartialObject         0x101B
#define PTP_OPCODE_InitiateOpenCapture      0x101C

#define PTP_OPCODE_GetObjectPropsSupported  0x9801
#define PTP_OPCODE_GetObjectPropDesc        0x9802
#define PTP_OPCODE_GetObjectPropValue       0x9803
#define PTP_OPCODE_SetObjectPropValue       0x9804
#define PTP_OPCODE_GetObjectPropList        0x9805
#define PTP_OPCODE_SetObjectPropList        0x9806
#define PTP_OPCODE_GetObjectReferences      0x9810
#define PTP_OPCODE_SetObjectReferences      0x9811
#define PTP_OPCODE_Skip                     0x9820

// Response Codes
//
#define PTP_RESP_Undefined                  0x2000
#define PTP_RESP_OK                         0x2001
#define PTP_RESP_GeneralError               0x2002
#define PTP_RESP_SessionNotOpen             0x2003
#define PTP_RESP_InvalidTransactionID       0x2004
#define PTP_RESP_OperationNotSupported      0x2005
#define PTP_RESP_ParameterNotSupported      0x2006
#define PTP_RESP_IncompleteTransfer         0x2007
#define PTP_RESP_InvalidStorageId           0x2008
#define PTP_RESP_InvalidObjectHandle        0x2009
#define PTP_RESP_DevicePropNotSupported     0x200A
#define PTP_RESP_InvalidObjectFormatCode    0x200B
#define PTP_RESP_StoreFull                  0x200C
#define PTP_RESP_ObjectWriteProtected       0x200D
#define PTP_RESP_StoreReadOnly              0x200E
#define PTP_RESP_AccessDenied               0x200F
#define PTP_RESP_NoThumbnailPresent         0x2010
#define PTP_RESP_SelfTestFailed             0x2011
#define PTP_RESP_PartialDeletion            0x2012
#define PTP_RESP_StoreNotAvailable          0x2013
#define PTP_RESP_SpecificationByFormatUnsupported 0x2014
#define PTP_RESP_NoValidObjectInfo          0x2015
#define PTP_RESP_InvalidCodeFormat          0x2016
#define PTP_RESP_UnknownVendoRESPode        0x2017
#define PTP_RESP_CaptureAlreadyTerminated   0x2018
#define PTP_RESP_DeviceBusy                 0x2019
#define PTP_RESP_InvalidParentObject        0x201A
#define PTP_RESP_InvalidDevicePropFormat    0x201B
#define PTP_RESP_InvalidDevicePropValue     0x201C
#define PTP_RESP_InvalidParameter           0x201D
#define PTP_RESP_SessionAlreadyOpened       0x201E
#define PTP_RESP_TransactionCanceled        0x201F
#define PTP_RESP_SpecificationOfDestinationUnsupported 0x2020

#define PTP_RESP_Invalid_ObjectPropCode     0xA801
#define PTP_RESP_Invalid_ObjectProp_Format  0xA802
#define PTP_RESP_Invalid_ObjectProp_Value   0xA803
#define PTP_RESP_Invalid_ObjectReference    0xA804

#define PTP_RESP_Invalid_Dataset            0xA806
#define PTP_RESP_Specification_By_Group_Unsupported 0xA807
#define PTP_RESP_Specification_By_Depth_Unsupported 0xA808
#define PTP_RESP_Object_Too_Large           0xA809
#define PTP_RESP_ObjectProp_Not_Supported   0xA80A
#define PTP_RESP_Group_Not_Supported        0xA805

// PTP Event Codes
//
#define PTP_EVENT_Undefined                 0x4000
#define PTP_EVENT_CancelTransaction         0x4001
#define PTP_EVENT_ObjectAdded               0x4002
#define PTP_EVENT_ObjectRemoved             0x4003
#define PTP_EVENT_StoreAdded                0x4004
#define PTP_EVENT_StoreRemoved              0x4005
#define PTP_EVENT_DevicePropChanged         0x4006
#define PTP_EVENT_ObjectInfoChanged         0x4007
#define PTP_EVENT_DeviceInfoChanged         0x4008
#define PTP_EVENT_RequestObjectTransfer     0x4009
#define PTP_EVENT_StoreFull                 0x400A
#define PTP_EVENT_DeviceReset               0x400B
#define PTP_EVENT_StorageInfoChanged        0x400C
#define PTP_EVENT_CaptureObjectComplete     0x400D
#define PTP_EVENT_UnreportedStatus          0x400E

// Property Codes
//
#define PTP_PROP_Undefined                  0x5000
#define PTP_PROP_Battery_Level              0x5001
#define PTP_PROP_Functional_Mode            0x5002
#define PTP_PROP_Image_Size                 0x5003
#define PTP_PROP_Compression_Setting        0x5004
#define PTP_PROP_White_Balance              0x5005
#define PTP_PROP_RGB_Gain                   0x5006
#define PTP_PROP_F_Number                   0x5007
#define PTP_PROP_Focal_Length               0x5008
#define PTP_PROP_Focus_Distance             0x5009
#define PTP_PROP_Focus_Mode                 0x500A
#define PTP_PROP_Exposure_Metering_Mode     0x500B
#define PTP_PROP_Flash_Mode                 0x500C
#define PTP_PROP_Exposure_Time              0x500D
#define PTP_PROP_Exposure_Program_Mode      0x500E
#define PTP_PROP_Exposure_Index             0x500F
#define PTP_PROP_Exposure_Bias_Compensation 0x5010
#define PTP_DROP_DateTime                   0x5011
#define PTP_PROP_Capture_Delay              0x5012
#define PTP_PROP_Still_Capture_Mode         0x5013
#define PTP_PROP_Contrast                   0x5014
#define PTP_PROP_Sharpness                  0x5015
#define PTP_PROP_Digital_Zoom               0x5016
#define PTP_PROP_Effect_Mode                0x5017
#define PTP_PROP_Burst_Number               0x5018
#define PTP_PROP_Burst_Interval             0x5019
#define PTP_PROP_Timelapse_Number           0x501A
#define PTP_PROP_Timelapse_Interval         0x501B
#define PTP_PROP_Focus_Metering_Mode        0x501C
#define PTP_PROP_Upload_URL                 0x501D
#define PTP_PROP_Artist                     0x501E
#define PTP_PROP_Copyright_Info             0x501F
#define PTP_PROP_Synchronization_Partner    0xD401
#define PTP_PROP_Device_Friendly_Name       0xD402
#define PTP_PROP_Volume                     0xD403
#define PTP_PROP_SupportedFormatsOrdered    0xD404
#define PTP_PROP_DeviceIcon                 0xD405
#define PTP_PROP_Playback_Rate              0xD410
#define PTP_PROP_Playback_Object            0xD411
#define PTP_PROP_Playback_Container_Index   0xD412
#define PTP_PROP_Session_Initiator_Version_Info 0xD406
#define PTP_PROP_Perceived_Device_Type      0xD407

#define PTP_PROP_READONLY   0x00
#define PTP_PROP_READWRITE  0x01

#define PTP_PROP_BuyFlag                    0xD901
#define PTP_PROP_StorageID                  0xDC01
#define PTP_PROP_ObjectFormat               0xDC02
#define PTP_PROP_ProtectionStatus           0xDC03
#define PTP_PROP_ObjectSize                 0xDC04
#define PTP_PROP_AssociationType            0xDC05
#define PTP_PROP_AssociationDesc            0xDC06
#define PTP_PROP_ObjectFileName             0xDC07
#define PTP_PROP_DateCreated                0xDC08
#define PTP_PROP_DateModified               0xDC09
#define PTP_PROP_Keywords                   0xDC0A
#define PTP_PROP_ParentObject               0xDC0B
#define PTP_PROP_AllowedFolderContents      0xDC0C
#define PTP_PROP_Hidden                     0xDC0D
#define PTP_PROP_SystemObject               0xDC0E
#define PTP_PROP_PersistantUniqueObjectIdentifier    0xDC41
#define PTP_PROP_SyncID                     0xDC42
#define PTP_PROP_PropertyBag                0xDC43
#define PTP_PROP_Name                       0xDC44
#define PTP_PROP_CreatedBy                  0xDC45
#define PTP_PROP_ObjArtist                  0xDC46
#define PTP_PROP_DateAuthored               0xDC47
#define PTP_PROP_Description                0xDC48
#define PTP_PROP_URLReference               0xDC49
#define PTP_PROP_LanguageLocale             0xDC4A
#define PTP_PROP_CopyrightInformation       0xDC4B
#define PTP_PROP_Source                     0xDC4C
#define PTP_PROP_OriginLocation             0xDC4D
#define PTP_PROP_DateAdded                  0xDC4E
#define PTP_PROP_NonConsumable              0xDC4F
#define PTP_PROP_CorruptOrUnplayable        0xDC50
#define PTP_PROP_ProducerSerialNumber       0xDC51
#define PTP_PROP_RepresentativeSampleFormat 0xDC81
#define PTP_PROP_RepresentativeSampleSize   0xDC82
#define PTP_PROP_RepresentativeSampleHeight 0xDC83
#define PTP_PROP_RepresentativeSampleWidth  0xDC84
#define PTP_PROP_RepresentativeSampleDuration 0xDC85
#define PTP_PROP_RepresentativeSampleData    0xDC86
#define PTP_PROP_Width                      0xDC87
#define PTP_PROP_Height                     0xDC88
#define PTP_PROP_Duration                   0xDC89
#define PTP_PROP_Rating                     0xDC8A
#define PTP_PROP_Track                      0xDC8B
#define PTP_PROP_Genre                      0xDC8C
#define PTP_PROP_Credits                    0xDC8D
#define PTP_PROP_Lyrics                     0xDC8E
#define PTP_PROP_SubscriptionContentID      0xDC8F
#define PTP_PROP_ProducedBy                 0xDC90
#define PTP_PROP_UseCount                   0xDC91
#define PTP_PROP_SkipCount                  0xDC92
#define PTP_PROP_LastAccessed               0xDC93
#define PTP_PROP_ParentalRating             0xDC94
#define PTP_PROP_MetaGenre                  0xDC95
#define PTP_PROP_Composer                   0xDC96
#define PTP_PROP_EffectiveRating            0xDC97
#define PTP_PROP_Subtitle                   0xDC98
#define PTP_PROP_OriginalReleaseDate        0xDC99
#define PTP_PROP_AlbumName                  0xDC9A
#define PTP_PROP_AlbumArtist                0xDC9B
#define PTP_PROP_Mood                       0xDC9C
#define PTP_PROP_DRMStatus                  0xDC9D
#define PTP_PROP_SubDescription             0xDC9E
#define PTP_PROP_IsCropped                  0xDCD1
#define PTP_PROP_IsColorCorrected           0xDCD2
#define PTP_PROP_ImageBitDepth              0xDCD3
#define PTP_PROP_Fnumber                    0xDCD4
#define PTP_PROP_ExposureTime               0xDCD5
#define PTP_PROP_ExposureIndex              0xDCD6
#define PTP_PROP_DisplayName                0xDCE0
#define PTP_PROP_BodyText                   0xDCE1
#define PTP_PROP_Subject                    0xDCE2
#define PTP_PROP_Priority                   0xDCE3
#define PTP_PROP_GivenName                  0xDD00
#define PTP_PROP_MiddleNames                0xDD01
#define PTP_PROP_FamilyName                 0xDD02
#define PTP_PROP_Prefix                     0xDD03
#define PTP_PROP_Suffix                     0xDD04
#define PTP_PROP_PhoneticGivenName          0xDD05
#define PTP_PROP_PhoneticFamilyName         0xDD06
#define PTP_PROP_EmailPrimary               0xDD07
#define PTP_PROP_EmailPersonal1             0xDD08
#define PTP_PROP_EmailPersonal2             0xDD09
#define PTP_PROP_EmailBusiness1             0xDD0A
#define PTP_PROP_EmailBusiness2             0xDD0B
#define PTP_PROP_EmailOthers                0xDD0C
#define PTP_PROP_PhoneNumberPrimary         0xDD0D
#define PTP_PROP_PhoneNumberPersonal        0xDD0E
#define PTP_PROP_PhoneNumberPersonal2       0xDD0F
#define PTP_PROP_PhoneNumberBusiness        0xDD10
#define PTP_PROP_PhoneNumberBusiness2       0xDD11
#define PTP_PROP_PhoneNumberMobile          0xDD12
#define PTP_PROP_PhoneNumberMobile2         0xDD13
#define PTP_PROP_FaxNumberPrimary           0xDD14
#define PTP_PROP_FaxNumberPersonal          0xDD15
#define PTP_PROP_FaxNumberBusiness          0xDD16
#define PTP_PROP_PagerNumber                0xDD17
#define PTP_PROP_PhoneNumberOthers          0xDD18
#define PTP_PROP_PrimaryWebAddress          0xDD19
#define PTP_PROP_PersonalWebAddress         0xDD1A
#define PTP_PROP_BusinessWebAddress         0xDD1B
#define PTP_PROP_InstantMessengerAddress        0xDD1C
#define PTP_PROP_InstantMessengerAddress2       0xDD1D
#define PTP_PROP_InstantMessengerAddress3       0xDD1E
#define PTP_PROP_PostalAddressPersonalFull      0xDD1F
#define PTP_PROP_PostalAddressPersonalFullLine1     0xDD20
#define PTP_PROP_PostalAddressPersonalFullLine2     0xDD21
#define PTP_PROP_PostalAddressPersonalFullCity      0xDD22
#define PTP_PROP_PostalAddressPersonalFullRegion    0xDD23
#define PTP_PROP_PostalAddressPersonalFullPostalCode 0xDD24
#define PTP_PROP_PostalAddressPersonalFullCountry   0xDD25
#define PTP_PROP_PostalAddressBusinessFull      0xDD26
#define PTP_PROP_PostalAddressBusinessLine1     0xDD27
#define PTP_PROP_PostalAddressBusinessLine2     0xDD28
#define PTP_PROP_PostalAddressBusinessCity      0xDD29
#define PTP_PROP_PostalAddressBusinessRegion    0xDD2A
#define PTP_PROP_PostalAddressBusinessPostalCode    0xDD2B
#define PTP_PROP_PostalAddressBusinessCountry       0xDD2C
#define PTP_PROP_PostalAddressOtherFull         0xDD2D
#define PTP_PROP_PostalAddressOtherLine1        0xDD2E
#define PTP_PROP_PostalAddressOtherLine2        0xDD2F
#define PTP_PROP_PostalAddressOtherCity         0xDD30
#define PTP_PROP_PostalAddressOtherRegion       0xDD31
#define PTP_PROP_PostalAddressOtherPostalCode   0xDD32
#define PTP_PROP_PostalAddressOtherCountry      0xDD33
#define PTP_PROP_OrganizationName               0xDD34
#define PTP_PROP_PhoneticOrganizationName       0xDD35
#define PTP_PROP_Role                   0xDD36
#define PTP_PROP_Birthdate              0xDD37
#define PTP_PROP_MessageTo              0xDD40
#define PTP_PROP_MessageCC              0xDD41
#define PTP_PROP_MessageBCC             0xDD42
#define PTP_PROP_MessageRead            0xDD43
#define PTP_PROP_MessageReceivedTime        0xDD44
#define PTP_PROP_MessageSender              0xDD45
#define PTP_PROP_ActivityBeginTime          0xDD50
#define PTP_PROP_ActivityEndTime            0xDD51
#define PTP_PROP_ActivityLocation           0xDD52
#define PTP_PROP_ActivityRequiredAttendees  0xDD54
#define PTP_PROP_ActivityOptionalAttendees  0xDD55
#define PTP_PROP_ActivityResources          0xDD56
#define PTP_PROP_ActivityAccepted           0xDD57
#define PTP_PROP_Owner                      0xDD5D
#define PTP_PROP_Editor                     0xDD5E
#define PTP_PROP_Webmaster                  0xDD5F
#define PTP_PROP_URLSource                  0xDD60
#define PTP_PROP_URLDestination             0xDD61
#define PTP_PROP_TimeBookmark               0xDD62
#define PTP_PROP_ObjectBookmark             0xDD63
#define PTP_PROP_ByteBookmark               0xDD64
#define PTP_PROP_LastBuildDate              0xDD70
#define PTP_PROP_TimetoLive                 0xDD71
#define PTP_PROP_MediaGUID                  0xDD72
#define PTP_PROP_TotalBitRate               0xDE91
#define PTP_PROP_BitRateType                0xDE92
#define PTP_PROP_SampleRate                 0xDE93
#define PTP_PROP_NumberOfChannels           0xDE94
#define PTP_PROP_AudioBitDepth              0xDE95
#define PTP_PROP_ScanDepth                  0xDE97
#define PTP_PROP_AudioWAVECodec             0xDE99
#define PTP_PROP_AudioBitRate               0xDE9A
#define PTP_PROP_VideoFourCCCodec           0xDE9B
#define PTP_PROP_VideoBitRate               0xDE9C
#define PTP_PROP_FramesPerThousandSeconds   0xDE9D
#define PTP_PROP_KeyFrameDistance           0xDE9E
#define PTP_PROP_BufferSize                 0xDE9F
#define PTP_PROP_EncodingQuality            0xDEA0
#define PTP_PROP_EncodingProfile            0xDEA1
#define PTP_PROP_LastProp                   0xDEA1

// File formats
//
#define PTP_OBJFMT_UNDEF            		0x3000
#define PTP_OBJFMT_ASSOC            		0x3001
#define PTP_OBJFMT_SCRIPT           		0x3002
#define PTP_OBJFMT_EXE              		0x3003
#define PTP_OBJFMT_TEXT             		0x3004
#define PTP_OBJFMT_HTML             		0x3005
#define PTP_OBJFMT_DPOF             		0x3006
#define PTP_OBJFMT_AIFF             		0x3007
#define PTP_OBJFMT_WAV              		0x3008
#define PTP_OBJFMT_MP3              		0x3009
#define PTP_OBJFMT_AVI              		0x300A
#define PTP_OBJFMT_MPEG             		0x300B
#define PTP_OBJFMT_ASF              		0x300C
#define PTP_OBJFMT_EXIF_JPEG        		0x3801
#define PTP_OBJFMT_TIFF_EP          		0x3802
#define PTP_OBJFMT_FlashPix         		0x3803
#define PTP_OBJFMT_BMP              		0x3804
#define PTP_OBJFMT_CIFF             		0x3805
#define PTP_OBJFMT_GIF              		0x3807
#define PTP_OBJFMT_JFIF             		0x3808
#define PTP_OBJFMT_PCD              		0x3809
#define PTP_OBJFMT_PICT             		0x380A
#define PTP_OBJFMT_PNG              		0x380B
#define PTP_OBJFMT_TIFF             		0x380D
#define PTP_OBJFMT_TIFF_IT          		0x380E
#define PTP_OBJFMT_JP2              		0x380F
#define PTP_OBJFMT_JPX              		0x3810
#define PTP_OBJFMT_FW               		0xB802

#define PTP_OBJFMT_WINDOWSIMAGEFORMAT   	0xB881
#define PTP_OBJFMT_UNDEFINEDAUDIO       	0xB900
#define PTP_OBJFMT_WMA                  	0xB901
#define PTP_OBJFMT_OGG                  	0xB902
#define PTP_OBJFMT_AAC                  	0xB903
#define PTP_OBJFMT_AUDIBLE              	0xB904
#define PTP_OBJFMT_FLAC                 	0xB906
#define PTP_OBJFMT_UNDEFINEDVIDEO       	0xB980
#define PTP_OBJFMT_WMV                  	0xB981
#define PTP_OBJFMT_MP4                  	0xB982
#define PTP_OBJFMT_MP2                  	0xB983
#define PTP_OBJFMT_UNDEFINEDCOLLECTION  	0xBA00
#define PTP_OBJFMT_ABSTRACTMULTIMEDIAALBUM 	0xBA01
#define PTP_OBJFMT_ABSTRACTIMAGEALBUM   	0xBA02
#define PTP_OBJFMT_ABSTRACTAUDIOALBUM   	0xBA03
#define PTP_OBJFMT_ABSTRACTVIDEOALBUM   	0xBA04
#define PTP_OBJFMT_ABSTRACTAUDIOVIDEOPLAYLIST 0xBA05
#define PTP_OBJFMT_ABSTRACTCONTACTGROUP 	0xBA06
#define PTP_OBJFMT_ABSTRACTMESSAGEFOLDER 	0xBA07
#define PTP_OBJFMT_ABSTRACTCHAPTEREDPRODUCTION 0xBA08
#define PTP_OBJFMT_WPLPLAYLIST          	0xBA10
#define PTP_OBJFMT_M3UPLAYLIST          	0xBA11
#define PTP_OBJFMT_MPLPLAYLIST          	0xBA12
#define PTP_OBJFMT_ASXPLAYLIST          	0xBA13
#define PTP_OBJFMT_PLSPLAYLIST          	0xBA14
#define PTP_OBJFMT_UNDEFINEDDOCUMENT    	0xBA80
#define PTP_OBJFMT_ABSTRACTDOCUMENT     	0xBA81
#define PTP_OBJFMT_XMLDOCUMENT          	0xBA82
#define PTP_OBJFMT_MICROSOFTWORDDOCUMENT 	0xBA83
#define PTP_OBJFMT_MHTCOMPILEDHTMLDOCUMENT 0xBA84
#define PTP_OBJFMT_MICROSOFTEXCELSPREADSHEE 0xBA85
#define PTP_OBJFMT_MICROSOFTPOWERPOINTDOCUMENT 0xBA86
#define PTP_OBJFMT_UNDEFINEDMESSAGE     	0xBB00
#define PTP_OBJFMT_ABSTRACTMESSAGE      	0xBB01
#define PTP_OBJFMT_UNDEFINEDCONTACT     	0xBB80
#define PTP_OBJFMT_ABSTRACTCONTACT      	0xBB81
#define PTP_OBJFMT_VCARD2               	0xBB82
#define PTP_OBJFMT_VCARD3               	0xBB83
#define PTP_OBJFMT_UNDEFINEDCALENDARITEM 	0xBE00
#define PTP_OBJFMT_ABSTRACTCALENDARITEM 	0xBE01
#define PTP_OBJFMT_VCALENDAR1           	0xBE02
#define PTP_OBJFMT_VCALENDAR2           	0xBE03
#define PTP_OBJFMT_UNDEFINEDWINDOWSEXECUTABLE 0xBE80
#define PTP_OBJFMT_UNDEFINEDTASKITEM    	0xBF00
#define PTP_OBJFMT_ABSTRACTTASKITEM     	0xBF01

#define PTP_OBJ_READONLY   0x01
#define PTP_OBJ_READWRITE  0x00

// Data Types
//
#define PTP_TYPE_UNDEF          0x0000
#define PTP_TYPE_INT8           0x0001
#define PTP_TYPE_UINT8          0x0002
#define PTP_TYPE_INT16          0x0003
#define PTP_TYPE_UINT16         0x0004
#define PTP_TYPE_INT32          0x0005
#define PTP_TYPE_UINT32         0x0006
#define PTP_TYPE_INT64          0x0007
#define PTP_TYPE_UINT64         0x0008
#define PTP_TYPE_INT128         0x0009
#define PTP_TYPE_UINT128        0x000A
#define PTP_TYPE_AINT8          0x4001
#define PTP_TYPE_AUINT8         0x4002
#define PTP_TYPE_AINT16         0x4003
#define PTP_TYPE_AUINT16        0x4004
#define PTP_TYPE_AINT32         0x4005
#define PTP_TYPE_AUINT32        0x4006
#define PTP_TYPE_AINT64         0x4007
#define PTP_TYPE_AUINT64        0x4008
#define PTP_TYPE_AINT128        0x4009
#define PTP_TYPE_AUINT128       0x400A
#define PTP_TYPE_STR            0xFFFF

// Transaction Types
//
#define PTP_XA_CMD              0x0001
#define PTP_XA_DATA             0x0002
#define PTP_XA_RESP             0x0003
#define PTP_XA_EVENT            0x0004

int    ptp_dispatch(ptp_connection_t *ptpx, ioring_t *io, size_t count);
int    ptp_cancel(ptp_connection_t *ptpx);
int    ptp_data_start(ptp_connection_t *ptpx, ioring_t *io, size_t total);
int    ptp_rx_data(ptp_connection_t *ptpx, ioring_t *io, size_t count, size_t oftotal);
int    ptp_init(ptp_connection_t *ptpx);
void   ptp_deinit(ptp_connection_t *ptpx);

#endif

