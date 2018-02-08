
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bxml.c $(SRCDIR)/bxmlutil.c
HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bxmltest.c
PROGOBJECTS=$(OBJDIR)/bxmltest.o

bxmltest: $(PROGOBJECTS) $(XMLLIB)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(XMLLIB) $(LIBS)

library: $(XMLLIB)

$(XMLLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) bxmllib.a $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(XMLLIB)

$(OBJDIR)/bxmltest.o: $(SRCDIR)/bxmltest.c $(HEADERS)
$(OBJDIR)/bxml.o: $(SRCDIR)/bxml.c $(HEADERS)


