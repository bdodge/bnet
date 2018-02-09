
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

PROGSOURCES=$(SRCDIR)/bstreamiotest.c
PROGOBJECTS=$(OBJDIR)/bstreamiotest.o

bstreamiotest: $(PROGOBJECTS) $(IOLIB)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(IOLIB) $(LIBS)

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

$(OBJDIR)/bstreamiotest.o: $(SRCDIR)/bstreamiotest.c $(HEADERS)

