
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

HTTP_PATH=$(SRCROOT)/bhttp
IO_PATH=$(SRCROOT)/bstreamio

SOURCES=$(SRCDIR)/bmqtt.c
HEADERS=$(SOURCES:%.c=%.h) bmqttconfig.h $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

HTTPLIB=$(HTTP_PATH)/bhttplib.a
IOLIB=$(IO_PATH)/bstreamio.a

PROGSOURCES=$(SRCDIR)/bmqttmain.c
PROGOBJECTS=$(OBJDIR)/bmqttmain.o

CFLAGS+=-I$(HTTP_PATH) -I$(IO_PATH)

mqtt: $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB) $(IOLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB) $(IOLIB) $(TLSLIBS) $(LIBS)

clean:
	rm $(PROGOBJECTS) $(OBJECTS)

$(OBJDIR)/bmqttmain.o: $(SRCDIR)/bmqttmain.c $(HEADERS)
$(OBJDIR)/bmqtt.o: $(SRCDIR)/bmqtt.c $(HEADERS)

$(MBEDTLS_PATH)/library/%.a:
	make -C $(MBEDTLS_PATH) lib CC="$(CC)" CFLAGS="$(CFLAGS)"


