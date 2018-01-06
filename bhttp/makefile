
MBEDTLS_PATH=../mbedtls
MBEDTLS_LIBS= mbedtls mbedx509 mbedcrypto

CFLAGS += -I../

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

SOURCES=$(SRCDIR)/bhttp.c $(SRCDIR)/bhttpres.c $(SRCDIR)/bhttpdav.c $(SRCDIR)/bhttpws.c $(SRCDIR)/bhttpsip.c \
				$(SRCDIR)/bhttputil.c $(SRCDIR)/bhttpio.c $(SRCDIR)/bhttptls.c
HEADERS=$(SOURCES:%.c=%.h) bhttpconfig.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

HTTPLIB=bhttplib.a

PROGSOURCES=$(SRCDIR)/bhttpmain.c
PROGOBJECTS=$(OBJDIR)/bhttpmain.o

CC=gcc
CFLAGS+=-g -fno-diagnostics-color

AR=ar
ARFLAGS=-r

http: $(PROGOBJECTS) $(HTTPLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(HTTPLIB) $(TLSLIBS) $(LIBS)

$(HTTPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $?

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/bhttpmain.o: $(SRCDIR)/bhttpmain.c $(HEADERS)
$(OBJDIR)/bhttp.o: $(SRCDIR)/bhttp.c $(HEADERS)
$(OBJDIR)/bhttpres.o: $(SRCDIR)/bhttpres.c $(HEADERS)
$(OBJDIR)/bhttpdav.o: $(SRCDIR)/bhttpdav.c $(HEADERS)
$(OBJDIR)/bhttpws.o: $(SRCDIR)/bhttpws.c $(HEADERS)
$(OBJDIR)/bhttpsip.o: $(SRCDIR)/bhttpsip.c $(HEADERS)
$(OBJDIR)/bhttpio.o: $(SRCDIR)/bhttpio.c $(HEADERS)
$(OBJDIR)/bhttptls.o: $(SRCDIR)/bhttptls.c $(HEADERS) $(TLSDEPS) ../certs/ht_privkey.h ../certs/ht_ourcrt.h
$(OBJDIR)/bhttputil.o: $(SRCDIR)/bhttputil.c $(HEADERS)

$(MBEDTLS_PATH)/library/%.a:
	make -C $(MBEDTLS_PATH) lib CC="$(CC)" CFLAGS="$(CFLAGS)"


