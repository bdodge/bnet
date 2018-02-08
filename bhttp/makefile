
SRCROOT=..

include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bhttp.c $(SRCDIR)/bhttpres.c $(SRCDIR)/bhttpdav.c \
		$(SRCDIR)/bhttpws.c $(SRCDIR)/bhttpsip.c \
		$(SRCDIR)/bhttputil.c
HEADERS=$(SOURCES:%.c=%.h) bhttpconfig.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bhttpmain.c
PROGOBJECTS=$(OBJDIR)/bhttpmain.o

bhttp: $(PROGOBJECTS) $(HTTPLIB) $(IOLIB) $(UTILLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

library: $(HTTPLIB)

$(HTTPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(HTTPLIB) $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB)

$(OBJDIR)/bhttpmain.o: $(SRCDIR)/bhttpmain.c $(HEADERS)
$(OBJDIR)/bhttp.o: $(SRCDIR)/bhttp.c $(HEADERS)
$(OBJDIR)/bhttpres.o: $(SRCDIR)/bhttpres.c $(HEADERS)
$(OBJDIR)/bhttpdav.o: $(SRCDIR)/bhttpdav.c $(HEADERS)
$(OBJDIR)/bhttpws.o: $(SRCDIR)/bhttpws.c $(HEADERS)
$(OBJDIR)/bhttpsip.o: $(SRCDIR)/bhttpsip.c $(HEADERS)
$(OBJDIR)/bhttputil.o: $(SRCDIR)/bhttputil.c $(HEADERS)


