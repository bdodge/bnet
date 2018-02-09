
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/butil.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/butiltest.c
PROGOBJECTS=$(OBJDIR)/butiltest.o

butiltest: $(PROGOBJECTS) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(UTILLIB) $(LIBS)

library: $(UTILLIB)

$(UTILLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(OBJECTS) $(UTILLIB)

$(OBJDIR)/butil.o: $(SRCDIR)/butil.c $(HEADERS)
$(OBJDIR)/butiltest.o: $(SRCDIR)/butiltest.c $(HEADERS)

