
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bxmpp.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bxmpptest.c
PROGOBJECTS=$(OBJDIR)/bxmpptest.o

IOLIB_PATH=../bstreamio
IOLIB=$(IOLIB_PATH)/bstreamio.a

XMLLIB_PATH=../bxml
XMLLIB=$(XMLLIB_PATH)/bxmllib.a

CFLAGS+=-I$(IOLIB_PATH) -I$(XMLLIB_PATH)

bxmpp: $(PROGOBJECTS) $(OBJECTS) $(XMLLIB) $(IOLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB)

$(OBJDIR)/bxmpptest.o: $(SRCDIR)/bxmpptest.c $(HEADERS)
$(OBJDIR)/bxmpp.o: $(SRCDIR)/bxmpp.c $(HEADERS)

$(IOLIB):
	make -C $(IOLIB_PATH)

$(XMLLIB):
	make -C $(XMLLIB_PATH) library

$(MBEDTLS_PATH)/library/%.a:
	make -C $(MBEDTLS_PATH) lib CC="$(CC)" CFLAGS="$(CFLAGS)"


