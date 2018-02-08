
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bmqtt.c
HEADERS=$(SOURCES:%.c=%.h) bmqttconfig.h $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bmqttmain.c
PROGOBJECTS=$(OBJDIR)/bmqttmain.o

CFLAGS+=-I$(HTTP_PATH) -I$(UTIL_PATH) -I$(IO_PATH)

mqtt: $(PROGOBJECTS) $(MQTTLIB) $(HTTPLIB) $(IOLIB) $(UTILLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(MQTTLIB) $(HTTPLIB) $(IOLIB) $(UTILLIB) $(TLSLIBS) $(LIBS)

library: $(MQTTLIB)

$(MQTTLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(MQTTLIB) $^

clean:
	rm $(PROGOBJECTS) $(MQTTLIB) $(OBJECTS)

$(OBJDIR)/bmqttmain.o: $(SRCDIR)/bmqttmain.c $(HEADERS)
$(OBJDIR)/bmqtt.o: $(SRCDIR)/bmqtt.c $(HEADERS)

$(MBEDTLS_PATH)/library/%.a:
	make -C $(MBEDTLS_PATH) lib CC="$(CC)" CFLAGS="$(CFLAGS)"


