
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

HTTP_PATH=$(SRCROOT)/bhttp
IO_PATH=$(SRCROOT)/bstreamio

SOURCES=$(SRCDIR)/bjson.c
HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

HTTPLIB=$(HTTP_PATH)/bhttplib.a
IOLIB=$(IO_PATH)/bstreamio.a

PROGSOURCES=$(SRCDIR)/bjsontest.c
PROGOBJECTS=$(OBJDIR)/bjsontest.o

CFLAGS+=-I$(HTTP_PATH) -I$(IO_PATH)

bjsontest: $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB) $(IOLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB) $(IOLIB) $(TLSLIBS) $(LIBS)

clean:
	rm $(PROGOBJECTS) $(OBJECTS)

$(OBJDIR)/bjsontest.o: $(SRCDIR)/bjsontest.c $(HEADERS)
$(OBJDIR)/bjson.o: $(SRCDIR)/bjson.c $(HEADERS)

$(MBEDTLS_PATH)/library/%.a:
	make -C $(MBEDTLS_PATH) lib CC="$(CC)" CFLAGS="$(CFLAGS)"


