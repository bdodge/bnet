
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bmem.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bmemtest.c
PROGOBJECTS=$(OBJDIR)/bmemtest.o

CFLAGS+=-I$(HTTP_PATH) -I$(OSAL_PATH) -I$(UTIL_PATH)

bmemtest: $(PROGOBJECTS) $(MEMLIB) $(OSALLIB) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

library: $(MEMIB)

$(MEMLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(OBJECTS) $(MEMLIB)

$(OBJDIR)/bmem.o: $(SRCDIR)/bmem.c $(HEADERS)
$(OBJDIR)/bmemtest.o: $(SRCDIR)/bmemtest.c $(HEADERS)

