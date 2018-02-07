
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bxml.c $(SRCDIR)/bxmlutil.c
HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bxmltest.c
PROGOBJECTS=$(OBJDIR)/bxmltest.o

bxmltest: $(PROGOBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(OBJECTS) $(LIBS)

library: $(OBJECTS)
	$(AR) $(ARFLAGS) bxmllib.a $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS)

$(OBJDIR)/bxmltest.o: $(SRCDIR)/bxmltest.c $(HEADERS)
$(OBJDIR)/bxml.o: $(SRCDIR)/bxml.c $(HEADERS)


