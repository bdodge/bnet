
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

LIBS=-lpthread

SOURCES=$(SRCDIR)/bosal.c
HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bosaltest.c  $(SRCDIR)/bosalposix.c
PROGOBJECTS=$(OBJDIR)/bosaltest.o $(SRCDIR)/bosalposix.o

bosaltest: $(PROGOBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(OBJECTS) $(LIBS)

clean:
	rm $(PROGOBJECTS) $(OBJECTS)

$(OBJDIR)/bosaltest.o: $(SRCDIR)/bosaltest.c $(HEADERS)
$(OBJDIR)/bosal.o: $(SRCDIR)/bosal.c $(HEADERS)
$(OBJDIR)/bosalposix.o: $(SRCDIR)/bosalposix.c $(HEADERS)


