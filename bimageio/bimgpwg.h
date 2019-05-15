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
#ifndef BIMGPWG_H
#define BIMGPWG_H 1

#include "bimageio.h"


#define PWG_IO_SIZE 1024

#define PWG_SYNC_WORD (0x52615332)
#define PWG_SYNC_STRING "Ras2"

typedef struct tag_pwg_header
{
    char        /*0-63 CString */           PwgRaster[64];
    char        /*64-127 CString*/          MediaColor[64];
    char        /*128-191 CString*/         MediaType[64];
    char        /*192-255 CString*/         PrintContentOptimize[64];
    uint8_t     /*256-267 Reserved*/        Reserved_1[12];
    uint32_t    /*268-271 WhenEnum*/        CutMedia;
    uint32_t    /*272-275 Boolean*/         Duplex;
    uint32_t    /*276-283 UnsignedInteger x 2 HWResolution*/ xres;
    uint32_t    /*276-283 UnsignedInteger x 2 HWResolution*/ yres;
    uint8_t     /*284-299 Reserved*/        Reserved_2[16];
    uint32_t    /*300-303 Boolean*/         InsertSheet;
    uint32_t    /*304-307 WhenEnum*/        Jog;
    uint32_t    /*308-311 EdgeEnum*/        LeadingEdge;
    uint8_t     /*312-323 Reserved*/        Reserved_3[12];
    uint32_t    /*324-327 MediaPositionEnum*/ MediaPosition;
    uint32_t    /*328-331 UnsignedInteger*/ MediaWeightMetric;
    uint8_t     /*332-339 Reserved*/        Reserved_4[8];
    uint32_t    /*340-343 UnsignedInteger*/ NumCopies;
    uint32_t    /*344-347 OrientationEnum*/ Orientation;
    uint8_t     /*348-351 Reserved*/        Reserved_5[4];
    uint32_t    /*352-359 UnsignedInteger x 2*/ PageSize_X;
    uint32_t    /*352-359 UnsignedInteger x 2*/ PageSize_Y;
    uint8_t     /*360-367 Reserved*/        Reserved_6[8];
    uint32_t    /*368-371 Boolean*/         Tumble;
    uint32_t    /*372-375 UnsignedInteger*/ Width;
    uint32_t    /*376-379 UnsignedInteger*/ Height;
    uint8_t     /*380-383 Reserved*/        Reserved_7[4];
    uint32_t    /*384-387 UnsignedInteger*/ BitsPerColor;
    uint32_t    /*388-391 UnsignedInteger*/ BitsPerPixel;
    uint32_t    /*392-395 UnsignedInteger*/ BytesPerLine;
    uint32_t    /*396-399 ColorOrderEnum*/  ColorOrder;
    uint32_t    /*400-403 ColorSpaceEnum*/  ColorSpace;
    uint8_t     /*404-419 Reserved*/        Reserved[16];
    uint32_t    /*420-423 UnsignedInteger*/ NumColors;
    uint8_t     /*424-451 Reserved*/        Reserved_8[28];
    uint32_t    /*452-455 UnsignedInteger*/ TotalPageCount;
    uint32_t    /*456-459 Integer*/         CrossFeedTransform;
    uint32_t    /*460-463 Integer*/         FeedTransform;
    uint32_t    /*464-467 UnsignedInteger*/ ImageBoxLeft;
    uint32_t    /*468-471 UnsignedInteger*/ ImageBoxTop;
    uint32_t    /*472-475 UnsignedInteger*/ ImageBoxRight;
    uint32_t    /*476-479 UnsignedInteger*/ ImageBoxBottom;
    uint32_t    /*480-483 SrgbColor*/       AlternatePrimary;
    uint32_t    /*484-487 PrintQualityEnum*/PrintQuality;
    uint8_t     /*488-507 Reserved*/        Reserved_9[20];
    uint32_t    /*508-511 UnsignedInteger*/ VendorIdentifier;
    uint32_t    /*512-515 UnsignedInteger*/ VendorLength;
    uint8_t     /*516-1603 VendorData*/     VendorData[1088];
    uint8_t     /*1604-1667 Reserved*/      Reserved_10[64];
    char        /*1668-1731 CString*/       RenderingIntent[64];
    char        /*1732-1795 CString*/       PageSizeName[64];
}
pwg_header_t;

int image_open_pwg (image_stream_t *istream, image_open_intent_t intent);
int image_read_pwg (image_stream_t *istream, uint8_t *line, size_t nline);
int image_write_pwg(image_stream_t *istream, uint8_t *line, size_t nline);
int image_close_pwg(image_stream_t *istream);

#endif

