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
#ifndef BMIBCKEYWORDS_H
#define BMIBCKEYWORDS_H 1

typedef enum
{
    kwNONE,                     // 0
    kwMODULE,
    kwBEGIN,
    kwIMPORTS,
    kwEXPORTS,
    kwINCLUDES,
    kwAUGMENTS,
    kwREVISION,
    kwOBJECTS,
    kwLAST_UPDATED,

    kwORGANIZATION,             // 10
    kwCONTACT_INFO,
    kwMODULE_IDENTITY,
    kwMODULE_COMPLIANCE,
    kwDEFINITIONS,
    kwEND,
    kwNOTIFICATIONS,
    kwACCESS,
    kwMIN_ACCESS,
    kwMAX_ACCESS,

    kwPRODUCT_RELEASE,          // 20
    kwTEXTUAL_CONVENTION,
    kwNOTIFICATION_GROUP,
    kwDISPLAY_HINT,
    kwFROM,
    kwAGENT_CAPABILITIES,
    kwMACRO,
    kwIMPLIED,
    kwSUPPORTS,
    kwVARIATION,

    kwWRITE_SYNTAX,             // 30
    kwCREATION_REQUIRES,
    kwGROUP,
    kwMANDATORY_GROUPS,
    kwSTATUS,
    kwSYNTAX,
    kwOBJECT_TYPE,
    kwTRAP_TYPE,
    kwENTERPRISE,
    kwCHOICE,

    kwIMPLICIT,                 // 40
    kwVARIABLES,
    kwUNITS,
    kwREFERENCE,
    kwNUM_ENTRIES,
    kwBITSTRING,
    kwBIT,
    kwBITS,
    kwNOTIFICATION_TYPE,
    kwOBJECT_GROUP,

    kwOBJECT_IDENTITY,          // 50
    kwIDENTIFIER,
    kwOBJECT,
    kwSTRING,
    kwOCTET,
    kwOF,
    kwSEQUENCE,
    kwNULL,
    kwINTEGER,
    kwDESCRIPTION,

    kwINDEX,                    // 60
    kwDEFVAL,
    kwSIZE,
    kwNOT_ACCESSIBLE,
    kwREAD_ONLY,
    kwREAD_WRITE,
    kwREAD_CREATE,
    kwWRITE_ONLY,
#ifndef NO_PREDEF_TYPES
    kwINTEGER32,
    kwUINTEGER32,

    kwUNSIGNED32,               // 70
    kwCOUNTER,
    kwCOUNTER32,
    kwGAUGE,
    kwGAUGE32,
    kwCOUNTER64,
    kwTIMETICKS,
    kwIPADDRESS,
    kwNETWORKADDRESS,
    kwNSAPADDRESS,
#endif
    kwCURRENT,                  // 80
    kwMANDATORY,
    kwACCESSIBLE_FOR_NOTIFY,
    kwNOT_IMPLEMENTED,
    kwOBJECTSYNTAX,
    kwSIMPLESYNTAX,
    kwAPPLICATIONSYNTAX,
    kwOBJECTNAME,
    kwNOTIFICATIONNAME,

    kwDEPRECATED,               // 90
    kwOBSOLETE,
    kwOPAQUE,
    kwOPTIONAL
#ifndef NO_EXTENSIONS
    ,
    kwOFFSET,
    kwVOLATILE
#endif
}
bmib_keyword_t;

typedef struct tag_builtin_macro
{
    const char *f_name;
}
bmib_builtin_macro_t;

typedef struct tag_builtin_type
{
    const char     *f_name;
    bmib_keyword_t  f_kw;

}
bmib_builtin_type_t;

typedef struct tag_mib_keyword
{
    const char     *f_name;
    bmib_keyword_t  f_value;
}
bmib_keyword_map_t;

extern bmib_builtin_macro_t g_mib_macros[];
extern bmib_builtin_type_t  g_mib_types[];
extern bmib_keyword_map_t   g_mib_keywords[];

#endif

