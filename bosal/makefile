
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

LIBS+=-lpthread

SOURCES=$(SRCDIR)/bosal_msgq.c
HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bosaltest.c
PROGOBJECTS=$(OBJDIR)/bosaltest.o

bosaltest: $(PROGOBJECTS) $(OSALLIB)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(OSALLIB) $(LIBS)

library: $(OSALLIB)

$(OSALLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS)

$(OBJDIR)/bosal_msgq.o: $(SRCDIR)/bosal_msgq.c $(HEADERS)
$(OBJDIR)/bosaltest.o: $(SRCDIR)/bosaltest.c $(HEADERS)


