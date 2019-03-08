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
#include "bmibckeywords.h"
#include "bnetheaders.h"

bmib_builtin_macro_t g_mib_macros[] =
{
    { "OBJECT-TYPE"                 },
    { "OBJECT-GROUP"                },
    { "OBJECT-IDENTITY"             },
    { "TEXTUAL-CONVENTION"          },
    { "MODULE-IDENTITY"             },
    { "MODULE-COMPLIANCE"           },
    { "NOTIFICATION-TYPE"           },
    { NULL }
};

bmib_builtin_type_t g_mib_types[] =
{
    { "OBJECT",                 kwOBJECT                    },
    { "OCTET",                  kwOCTET                     },
    { "SEQUENCE",               kwSEQUENCE                  },
    { "CHOICE",                 kwCHOICE                    },
    { "NULL",                   kwNULL                      },
    { "INTEGER",                kwINTEGER                   },
#ifndef NO_PREDEF_TYPES
    { "Integer32",              kwINTEGER32                 },
    { "UInteger32",             kwUINTEGER32                },
    { "Unsigned32",             kwUNSIGNED32                },
    { "Counter",                kwCOUNTER                   },
    { "Counter32",              kwCOUNTER32                 },
    { "Gauge",                  kwGAUGE                     },
    { "Gauge32",                kwGAUGE32                   },
    { "Counter64",              kwCOUNTER64                 },
    { "TimeTicks",              kwTIMETICKS                 },
    { "IpAddress",              kwIPADDRESS                 },
    { "NetworkAddress",         kwNETWORKADDRESS            },
    { "NsapAddress",            kwNSAPADDRESS               },
#endif
    { NULL }
};


bmib_keyword_map_t g_mib_keywords[] =
{

    { "MODULE",                 kwMODULE                    },
    { "BEGIN",                  kwBEGIN                     },
    { "IMPORTS",                kwIMPORTS                   },
    { "EXPORTS",                kwEXPORTS                   },
    { "INCLUDES",               kwINCLUDES                  },
    { "AUGMENTS",               kwAUGMENTS                  },
    { "REVISION",               kwREVISION                  },
    { "OBJECTS",                kwOBJECTS                   },
    { "LAST-UPDATED",           kwLAST_UPDATED              },
    { "ORGANIZATION",           kwORGANIZATION              },
    { "CONTACT-INFO",           kwCONTACT_INFO              },
    { "MODULE-IDENTITY",        kwMODULE_IDENTITY           },
    { "MODULE-COMPLIANCE",      kwMODULE_COMPLIANCE         },
    { "DEFINITIONS",            kwDEFINITIONS               },
    { "END",                    kwEND                       },
    { "NOTIFICATIONS",          kwNOTIFICATIONS             },
    { "ACCESS",                 kwACCESS                    },
    { "MIN-ACCESS",             kwMIN_ACCESS                },
    { "MAX-ACCESS",             kwMAX_ACCESS                },
    { "PRODUCT-RELEASE",        kwPRODUCT_RELEASE           },
    { "TEXTUAL-CONVENTION",     kwTEXTUAL_CONVENTION        },
    { "NOTIFICATION-GROUP",     kwNOTIFICATION_GROUP        },
    { "DISPLAY-HINT",           kwDISPLAY_HINT              },
    { "FROM",                   kwFROM                      },
    { "AGENT-CAPABILITIES",     kwAGENT_CAPABILITIES        },
    { "MACRO",                  kwMACRO                     },
    { "IMPLIED",                kwIMPLIED                   },
    { "SUPPORTS",               kwSUPPORTS                  },
    { "VARIATION",              kwVARIATION                 },
    { "WRITE-SYNTAX",           kwWRITE_SYNTAX              },
    { "CREATION-REQUIRES",      kwCREATION_REQUIRES         },
    { "GROUP",                  kwGROUP                     },
    { "MANDATORY-GROUPS",       kwMANDATORY_GROUPS          },
    { "STATUS",                 kwSTATUS                    },
    { "SYNTAX",                 kwSYNTAX                    },
    { "OBJECT-TYPE",            kwOBJECT_TYPE               },
    { "TRAP-TYPE",              kwTRAP_TYPE                 },
    { "ENTERPRISE",             kwENTERPRISE                },
    { "CHOICE",                 kwCHOICE                    },
    { "IMPLICIT",               kwIMPLICIT                  },
    { "VARIABLES",              kwVARIABLES                 },
    { "UNITS",                  kwUNITS                     },
    { "REFERENCE",              kwREFERENCE                 },
    { "NUM-ENTRIES",            kwNUM_ENTRIES               },
    { "BITSTRING",              kwBITSTRING                 },
    { "BIT",                    kwBIT                       },
    { "BITS",                   kwBITS                      },
    { "NOTIFICATION-TYPE",      kwNOTIFICATION_TYPE         },
    { "OBJECT-GROUP",           kwOBJECT_GROUP              },
    { "OBJECT-IDENTITY",        kwOBJECT_IDENTITY           },
    { "IDENTIFIER",             kwIDENTIFIER                },
    { "OBJECT",                 kwOBJECT                    },
    { "STRING",                 kwSTRING                    },
    { "OCTET",                  kwOCTET                     },
    { "OF",                     kwOF                        },
    { "SEQUENCE",               kwSEQUENCE                  },
    { "NULL",                   kwNULL                      },
    { "INTEGER",                kwINTEGER                   },
    { "DESCRIPTION",            kwDESCRIPTION               },
    { "INDEX",                  kwINDEX                     },
    { "DEFVAL",                 kwDEFVAL                    },
    { "SIZE",                   kwSIZE                      },
    { "not-accessible",         kwNOT_ACCESSIBLE            },
    { "read-only",              kwREAD_ONLY                 },
    { "read-write",             kwREAD_WRITE                },
    { "read-create",            kwREAD_CREATE               },
    { "write-only",             kwWRITE_ONLY                },
#ifndef NO_PREDEF_TYPE
    { "Integer32",              kwINTEGER32                 },
    { "UInteger32",             kwUINTEGER32                },
    { "Unsigned32",             kwUNSIGNED32                },
    { "Counter",                kwCOUNTER                   },
    { "Counter32",              kwCOUNTER32                 },
    { "Gauge",                  kwGAUGE                     },
    { "Gauge32",                kwGAUGE32                   },
    { "Counter64",              kwCOUNTER64                 },
    { "TimeTicks",              kwTIMETICKS                 },
    { "IpAddress",              kwIPADDRESS                 },
    { "NetworkAddress",         kwNETWORKADDRESS            },
    { "NsapAddress",            kwNSAPADDRESS               },
#endif
    { "current",                kwCURRENT                   },
    { "mandatory",              kwMANDATORY                 },
    { "accessible-for-notify",  kwACCESSIBLE_FOR_NOTIFY     },
    { "not-implemented",        kwNOT_IMPLEMENTED           },
    { "ObjectSyntax",           kwOBJECTSYNTAX              },
    { "SimpleSyntax",           kwSIMPLESYNTAX              },
    { "ApplicationSyntax",      kwAPPLICATIONSYNTAX         },
    { "ObjectName",             kwOBJECTNAME                },
    { "NotificationName",       kwNOTIFICATIONNAME          },
    { "deprecated",             kwDEPRECATED                },
    { "obsolete",               kwOBSOLETE                  },
    { "Opaque",                 kwOPAQUE                    },
    { "optional",               kwOPTIONAL                  },
#ifndef NO_EXTENSIONS
    { "OFFSET",                 kwOFFSET                    },
    { "VOLATILE",               kwVOLATILE                  },
#endif
    { NULL,                     kwNONE                      }
};

