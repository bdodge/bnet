
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

CERT_PATH=../certs
CERTS=$(CERT_PATH)/ht_privkey.h $(CERT_PATH)/ht_ourcrt.h

SOURCES=$(SRCDIR)/bhttpio.c $(SRCDIR)/bhttptls.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC=gcc
CFLAGS+=-g -fno-diagnostics-color

AR=ar
ARFLAGS=-r

ifndef NO_MBED_TLS
	CFLAGS+=-DIOSTREAM_SUPPORT_TLS
endif

IOLIB=bstreamio.a

$(IOLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $?

clean:
	rm $(OBJECTS) $(IOLIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/bhttpio.o: $(SRCDIR)/bhttpio.c $(HEADERS)
$(OBJDIR)/bhttptls.o: $(SRCDIR)/bhttptls.c $(HEADERS) $(TLSDEPS) $(CERTS)

