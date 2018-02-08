
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bjson.c
HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bjsontest.c
PROGOBJECTS=$(OBJDIR)/bjsontest.o

CFLAGS+=-I$(UTIL_PATH)

LIBS+=$(UTILLIB)

bjsontest: $(PROGOBJECTS) $(OBJECTS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^

library: $(JSONLIB)

$(JSONLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(JSONLIB)

$(OBJDIR)/bjsontest.o: $(SRCDIR)/bjsontest.c $(HEADERS)
$(OBJDIR)/bjson.o: $(SRCDIR)/bjson.c $(HEADERS)


