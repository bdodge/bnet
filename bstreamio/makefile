
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

CERT_PATH=../certs
CERTS=$(CERT_PATH)/ht_privkey.h $(CERT_PATH)/ht_ourcrt.h

SOURCES=$(SRCDIR)/bstreamio.c $(SRCDIR)/bstreamtls.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

ifndef NO_MBED_TLS
	CFLAGS+=-DIOSTREAM_SUPPORT_TLS
endif

IOLIB=bstreamio.a

library: $(IOLIB)

$(IOLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $?

clean:
	rm $(OBJECTS) $(IOLIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/bstreamio.o: $(SRCDIR)/bstreamio.c $(HEADERS)
$(OBJDIR)/bstreamtls.o: $(SRCDIR)/bstreamtls.c $(HEADERS) $(CERTS)

