#
# Copyright 2019 Brian Dodge
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

SRCROOT ?= ..

# Our libraries
UTIL_PATH=$(SRCROOT)/butil
IO_PATH=$(SRCROOT)/bstreamio
IMG_PATH=$(SRCROOT)/bimageio
MEM_PATH=$(SRCROOT)/bmem
XML_PATH=$(SRCROOT)/bxml
SASL_PATH=$(SRCROOT)/bsasl
JSON_PATH=$(SRCROOT)/bjson
OSAL_PATH=$(SRCROOT)/bosal
HTTP_PATH=$(SRCROOT)/bhttp
UPNP_PATH=$(SRCROOT)/bupnp
MEDIA_PATH=$(SRCROOT)/bmediatree
FTP_PATH=$(SRCROOT)/bftp
XMPP_PATH=$(SRCROOT)/bxmpp
MQTT_PATH=$(SRCROOT)/bmqtt
MDNS_PATH=$(SRCROOT)/bmdns
SMTP_PATH=$(SRCROOT)/bsmtp
SNMP_PATH=$(SRCROOT)/bsnmp
SIP_PATH=$(SRCROOT)/bsip
MPCL_PATH=$(SRCROOT)/bmpcl
IPP_PATH=$(SRCROOT)/bipp
GCP_PATH=$(SRCROOT)/bgcloudprt

# Third party libaries
MBEDTLS_PATH=$(SRCROOT)/mbedtls
MBEDTLS_LIBS=mbedtls mbedx509 mbedcrypto

LIBJPEGT_PATH=$(SRCROOT)/libjpeg-turbo
LIBJPEGT_LIBS=turbojpeg

LIBPNG_PATH=$(SRCROOT)/libpng-1.6.37
LIBPNG_LIBS=png

LIBZ_PATH=$(SRCROOT)/zlib-1.2.11
LIBZ_LIBS=z

CC=gcc
CFLAGS+=-g -fno-diagnostics-color

AR=ar
ARFLAGS=-r

LD=ld

CFLAGS += -I$(SRCROOT) -I$(SRCROOT)/common

SYLIBS=

ifeq ($(OS),Windows_NT)
    CFLAGS += -DWIN32
 	CFLAGS += -Wl,--subsystem,console
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        CFLAGS += -DAMD64 -m32
		ARCH=x86_64
    endif
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        CFLAGS += -DIA32
		ARCH=x86_32
    endif
	SYSLIBS +=-lws2_32
else
ifeq ($(OS),iOS)
	CC=clang
	AR=libtool
	ARFLAGS=-static -o
    CFLAGS += -DIOS -arch armv7 -arch arm64	\
                -fmessage-length=0 -fdiagnostics-show-note-include-stack \
                -fmacro-backtrace-limit=0 -Wno-trigraphs \
                -fpascal-strings -fno-common -Wno-missing-field-initializers \
                -fstrict-aliasing -Wprotocol -Wdeprecated-declarations -miphoneos-version-min=9.0 \
				-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS12.1.sdk \
				-Wno-sign-conversion -fembed-bitcode-marker
	ARCH=arm
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFLAGS += -DLinux
		OS=Linux
		ifdef FORCE32
			CFLAGS += -m32
		endif
    endif
    ifeq ($(UNAME_S),Darwin)
        CFLAGS += -DOSX
		OS=OSX
    endif
    UNAME_M := $(shell uname -m)
    ifeq ($(UNAME_M),x86_64)
        CFLAGS += -DAMD64
		ARCH=x86_64
    else
    ifeq ($(UNAME_M),i386)
        CFLAGS += -DAMD64
		ARCH=x86_32
    else
    ifneq ($(filter %86,$(UNAME_M)),)
        CFLAGS += -DIA32
		ARCH=IA32
    else
    ifneq ($(filter arm%,$(UNAME_M)),)
        CFLAGS += -DARM
		ARCH=arm
	else
		ARCH=unkown
    endif
    endif
    endif
    endif
endif
endif

SRCDIR=.
OBJDIR=.obj$(OS)$(ARCH)

UTILLIB=$(UTIL_PATH)/$(OBJDIR)/libbutil.a
IOLIB=$(IO_PATH)/$(OBJDIR)/libbstreamio.a
IMGLIB=$(IMG_PATH)/$(OBJDIR)/libbimageio.a
MEMLIB=$(MEM_PATH)/$(OBJDIR)/libbmem.a
XMLLIB=$(XML_PATH)/$(OBJDIR)/libbxml.a
SASLLIB=$(SASL_PATH)/$(OBJDIR)/libbsasl.a
OSALLIB=$(OSAL_PATH)/$(OBJDIR)/libbosal.a
JSONLIB=$(JSON_PATH)/$(OBJDIR)/libbjson.a
HTTPLIB=$(HTTP_PATH)/$(OBJDIR)/libbhttp.a
UPNPLIB=$(UPNP_PATH)/$(OBJDIR)/libupnp.a
MEDIALIB=$(MEDIA_PATH)/$(OBJDIR)/libmedia.a
FTPLIB=$(FTP_PATH)/$(OBJDIR)/libbftp.a
XMPPLIB=$(XMPP_PATH)/$(OBJDIR)/libbxmpp.a
MQTTLIB=$(MQTT_PATH)/$(OBJDIR)/libbmqtt.a
MDNSLIB=$(MDNS_PATH)/$(OBJDIR)/libbmdns.a
SMTPLIB=$(SMTP_PATH)/$(OBJDIR)/libbsmtp.a
SNMPLIB=$(SNMP_PATH)/$(OBJDIR)/libbsnmp.a
SIPLIB=$(SIP_PATH)/$(OBJDIR)/libbsip.a
MPCLLIB=$(MPCL_PATH)/$(OBJDIR)/libbmpcl.a
IPPLIB=$(IPP_PATH)/$(OBJDIR)/libbipp.a
GCPLIB=$(GCP_PATH)/$(OBJDIR)/libbgcp.a

ifndef BNET_TLS
	BNET_TLS=1
endif
ifneq ($(BNET_TLS),0)
	TLSLIBS=-L$(MBEDTLS_PATH)/library $(MBEDTLS_LIBS:%=-l%)
	TLSDEPS=$(MBEDTLS_LIBS:%=$(MBEDTLS_PATH)/library/lib%.a)
	CFLAGS+=-I$(MBEDTLS_PATH)/include -DBNET_TLS=1
else
	TLSLIBS=
	TLSDEPS=
	CFLAGS+=-DBNET_TLS=0
endif
ifndef BNET_JPEG
	BNET_JPEG=0
endif
ifneq ($(BNET_JPEG),0)
	JPEGLIBS=$(LIBJPEGT_LIBS:%=-l%)
	JPEGDEPS=$(LIBJPEGT_LIBS:%=$(OBJDIR)/lib%.a)
	CFLAGS+=-I$(LIBJPEGT_PATH) -DBNET_JPEG=1
else
	JPEGLIBS=
	JPEGDEPS=
endif
ifneq ($(BNET_PNG),0)
	PNGLIBS=$(LIBPNG_LIBS:%=-l%)
	PNGDEPS=$(LIBPNG_LIBS:%=$(OBJDIR)/lib%.a)
	CFLAGS+=-I$(LIBPNG_PATH) -DBNET_PNG=1
else
	PNGLIBS=
	PNGDEPS=
endif
ifndef BNET_ZLIB
	BNET_ZLIB=0
endif
ifneq ($(BNET_ZLIB),0)
	ZLIBLIBS=$(LIBZ_LIBS:%=-l%)
	ZLIBDEPS=$(LIBZ_LIBS:%=$(OBJDIR)/lib%.a$)
	CFLAGS+=-I$(LIBZ_PATH) -DBNET_ZLIB=1
else
	ZLIBLIBS=
	ZLIBDEPS=
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(UTIL_PATH)/%.a:
	make -C $(UTIL_PATH) library

$(IO_PATH)/%.a:
	make -C $(IO_PATH) BNET_TLS=$(BNET_TLS) library

$(IMG_PATH)/%.a:
	make -C $(IMG_PATH) library

$(MEM_PATH)/%.a:
	make -C $(MEM_PATH) library

$(XML_PATH)/%.a:
	make -C $(XML_PATH) library

$(SASL_PATH)/%.a:
	make -C $(SASL_PATH) library

$(JSON_PATH)/%.a:
	make -C $(JSON_PATH) library

$(HTTP_PATH)/%.a:
	make -C $(HTTP_PATH) BNET_TLS=$(BNET_TLS) BNET_ZLIB=$(BNET_ZLIB) EXTRA_DEFINES=$(EXTRA_DEFINES) library

$(UPNP_PATH)/%.a:
	make -C $(UPNP_PATH) BNET_TLS=$(BNET_TLS) EXTRA_DEFINES=$(EXTRA_DEFINES) library

$(MEDIA_PATH)/%.a:
	make -C $(MEDIA_PATH) BNET_TLS=$(BNET_TLS) EXTRA_DEFINES=$(EXTRA_DEFINES) library

$(XMPP_PATH)/%.a:
	make -C $(XMPP_PATH) library

$(MQTT_PATH)/%.a:
	make -C $(MQTT_PATH) library

$(MDNS_PATH)/%.a:
	make -C $(MDNS_PATH) library

$(SNMP_PATH)/%.a:
	make -C $(SNMP_PATH) BNET_TLS=$(BNET_TLS) library

$(SIP_PATH)/%.a:
	make -C $(SIP_PATH) BNET_TLS=$(BNET_TLS) library

$(MPCL_PATH)/%.a:
	make -C $(MPCL_PATH) library

$(IPP_PATH)/%.a:
	make -C $(IPP_PATH) BNET_TLS=$(BNET_TLS) library

$(GCP_PATH)/%.a:
	make -C $(GCP_PATH) BNET_TLS=$(BNET_TLS) library

$(MBEDTLS_PATH)/library/%.a:
	make -C $(MBEDTLS_PATH) lib OS=$(OS) BNET_TLS=$(BNET_TLS) CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) ARFLAGS=$(ARFLAGS) LD=$(LD)

$(OBJDIR)/%turbojpeg.a:
	mkdir -p $(OBJDIR)
	make -C $(OBJDIR) -f ../$(SRCROOT)/common/makejpegturbo.mk $@ BLDJPEGT_PATH=$(OBJDIR) LIBJPEGT_PATH=../$(LIBJPEGT_PATH) OS=$(OS) BNET_JPEG=$(BNET_JPEG) CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) ARFLAGS=$(ARFLAGS) LD=$(LD)

$(OBJDIR)/%png.a:
	mkdir -p $(OBJDIR)
	make -C $(OBJDIR) -f ../$(SRCROOT)/common/makepng.mk $@ BLDPNG_PATH=$(OBJDIR) LIBPNG_PATH=../$(LIBPNG_PATH) OS=$(OS) CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) ARFLAGS=$(ARFLAGS) LD=$(LD)

$(OBJDIR)/%z.a:
	mkdir -p $(OBJDIR)
	make -C $(OBJDIR) -f ../$(SRCROOT)/common/makezlib.mk $@ BLDZLIB_PATH=$(OBJDIR) LIBZ_PATH=../$(LIBZ_PATH) OS=$(OS) CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) ARFLAGS=$(ARFLAGS) LD=$(LD)

