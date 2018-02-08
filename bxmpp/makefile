
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bxmpp.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bxmpptest.c
PROGOBJECTS=$(OBJDIR)/bxmpptest.o

CFLAGS+=-I$(IO_PATH) -I$(XML_PATH) -I$(UTIL_PATH)

bxmpp: $(PROGOBJECTS) $(OBJECTS) $(XMLLIB) $(IOLIB) $(UTILLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

library: $(XMPPLIB)

$(XMPPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(XMPPLIB) $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB)

$(OBJDIR)/bxmpptest.o: $(SRCDIR)/bxmpptest.c $(HEADERS)
$(OBJDIR)/bxmpp.o: $(SRCDIR)/bxmpp.c $(HEADERS)


