#
# Copyright 2019 Brian Dodge
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bmqtt.c
HEADERS=$(SOURCES:%.c=%.h) bmqttconfig.h $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bmqttmain.c
PROGOBJECTS=$(OBJDIR)/bmqttmain.o

CFLAGS+=-I$(HTTP_PATH) -I$(UTIL_PATH) -I$(IO_PATH)

mqtt: $(PROGOBJECTS) $(MQTTLIB) $(HTTPLIB) $(IOLIB) $(UTILLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(TLSLIBS) $(LIBS)

library: $(MQTTLIB)

$(MQTTLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(MQTTLIB) $^

clean:
	rm $(PROGOBJECTS) $(MQTTLIB) $(OBJECTS)

$(OBJDIR)/bmqttmain.o: $(SRCDIR)/bmqttmain.c $(HEADERS)
$(OBJDIR)/bmqtt.o: $(SRCDIR)/bmqtt.c $(HEADERS)

$(MBEDTLS_PATH)/library/%.a:
	make -C $(MBEDTLS_PATH) lib CC="$(CC)" CFLAGS="$(CFLAGS)"


