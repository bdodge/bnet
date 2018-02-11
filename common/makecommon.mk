
SRCROOT ?= ..

MBEDTLS_PATH=$(SRCROOT)/mbedtls
MBEDTLS_LIBS= mbedtls mbedx509 mbedcrypto

UTIL_PATH=$(SRCROOT)/butil
IO_PATH=$(SRCROOT)/bstreamio
XML_PATH=$(SRCROOT)/bxml
SASL_PATH=$(SRCROOT)/bsasl
JSON_PATH=$(SRCROOT)/bjson
OSAL_PATH=$(SRCROOT)/bosal
HTTP_PATH=$(SRCROOT)/bhttp
FTP_PATH=$(SRCROOT)/bftp
XMPP_PATH=$(SRCROOT)/bxmpp
MQTT_PATH=$(SRCROOT)/bmqtt

UTILLIB=$(UTIL_PATH)/butillib.a
IOLIB=$(IO_PATH)/bstreamio.a
XMLLIB=$(XML_PATH)/bxmllib.a
SASLLIB=$(SASL_PATH)/bsasllib.a
OSALLIB=$(OSAL_PATH)/bosallib.a
JSONLIB=$(IO_PATH)/bjsonlib.a
HTTPLIB=$(HTTP_PATH)/bhttplib.a
FTPLIB=$(FTP_PATH)/bftplib.a
XMPPLIB=$(XMPP_PATH)/bxmpplib.a
MQTTLIB=$(MQTT_PATH)/bmqttlib.a

CFLAGS += -I$(SRCROOT) -I$(SRCROOT)/common

ifeq ($(OS),Windows_NT)
    CFLAGS += -D WIN32
 	CFLAGS += -Wl,--subsystem,console
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        CFLAGS += -D AMD64 -m32
    endif
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        CFLAGS += -D IA32
    endif
	LIBS=-lws2_32
else
	LIBS=
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFLAGS += -D Linux -m32
    endif
    ifeq ($(UNAME_S),Darwin)
        CFLAGS += -D OSX -m32
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CFLAGS += -D AMD64
    else
    ifeq ($(UNAME_P),i386)
        CFLAGS += -D AMD64
    else
    ifneq ($(filter %86,$(UNAME_P)),)
        CFLAGS += -D IA32
    else
    ifneq ($(filter arm%,$(UNAME_P)),)
        CFLAGS += -D ARM
    endif
    endif
    endif
    endif
	ifndef NO_MBED_TLS
		TLSLIBS=-L$(MBEDTLS_PATH)/library $(MBEDTLS_LIBS:%=-l%)
		TLSDEPS=$(MBEDTLS_LIBS:%=$(MBEDTLS_PATH)/library/lib%.a$)
		CFLAGS+=-I$(MBEDTLS_PATH)/include
	else
		TLSLIBS=
		TLSDEPS=
	endif
endif

SRCDIR=.
OBJDIR=.obj

CC=gcc
CFLAGS+=-g -fno-diagnostics-color

AR=ar
ARFLAGS=-r

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(UTIL_PATH)/%.a:
	make -C $(UTIL_PATH) library

$(IO_PATH)/%.a:
	make -C $(IO_PATH) library

$(XML_PATH)/%.a:
	make -C $(XML_PATH) library

$(SASL_PATH)/%.a:
	make -C $(SASL_PATH) library

$(JSON_PATH)/%.a:
	make -C $(JSON_PATH) library

$(HTTP_PATH)/%.a:
	make -C $(HTTP_PATH) library

$(XMPP_PATH)/%.a:
	make -C $(XMPP_PATH) library

$(MQTT_PATH)/%.a:
	make -C $(MQTT_PATH) library

$(MBEDTLS_PATH)/library/%.a:
	make -C $(MBEDTLS_PATH) lib CC="$(CC)" CFLAGS="$(CFLAGS)"


