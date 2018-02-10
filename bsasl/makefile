
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bsasl.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bsasltest.c
PROGOBJECTS=$(OBJDIR)/bsasltest.o

CFLAGS+=-I$(UTIL_PATH) -I$(IO_PATH)

bsasltest: $(PROGOBJECTS) $(OBJECTS) $(SASLLIB) $(IOLIB) $(UTILLIB) $(TLSDEPS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

library: $(SASLLIB)

$(SASLLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(SASLLIB) $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(SASLLIB)

$(OBJDIR)/bsasltest.o: $(SRCDIR)/bsasltest.c $(HEADERS)
$(OBJDIR)/bsasl.o: $(SRCDIR)/bsasl.c $(HEADERS)

