
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/butil.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

library: $(UTILLIB)

$(UTILLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(OBJECTS) $(UTILLIB)

$(OBJDIR)/butil.o: $(SRCDIR)/butil.c $(HEADERS)

