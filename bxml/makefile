
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bxml.c $(SRCDIR)/bxmlutil.c
HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bxmltest.c
PROGOBJECTS=$(OBJDIR)/bxmltest.o

CFLAGS+=-I$(HTTP_PATH) -I$(IO_PATH)

bxmltest: $(PROGOBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(OBJECTS) $(LIBS)

clean:
	rm $(PROGOBJECTS) $(OBJECTS)

$(OBJDIR)/bxmltest.o: $(SRCDIR)/bxmltest.c $(HEADERS)
$(OBJDIR)/bxml.o: $(SRCDIR)/bxml.c $(HEADERS)


