
SRCROOT ?= ..

MBEDTLS_PATH=$(SRCROOT)/mbedtls
MBEDTLS_LIBS= mbedtls mbedx509 mbedcrypto

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

