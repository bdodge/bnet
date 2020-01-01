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

SOURCES=$(SRCDIR)/bupnp.c $(SRCDIR)/bupnpservice.c

HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBS= $(HTTPLIB) $(IOLIB) $(UTILLIB)
LIBDIRS= $(HTTP_PATH) $(IO_PATH) $(UTIL_PATH)
LIBINCLS= $(LIBDIRS:%=-I%)
CFLAGS += $(LIBINCLS)

ifdef BNET_TLS
	CFLAGS += -DHTTP_SUPPORT_TLS=$(BNET_TLS)
endif
CFLAGS += $(EXTRA_DEFINES:%=-D%) -Wno-format-truncation

PROGSOURCES=$(SRCDIR)/bupnpmain.c
PROGOBJECTS=$(OBJDIR)/bupnpmain.o

bupnp: $(PROGOBJECTS) $(UPNPLIB) $(LIBS) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(UPNPLIB)

$(UPNPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB)

$(OBJDIR)/bupnpmain.o: $(SRCDIR)/bupnpmain.c $(HEADERS)

$(OBJDIR)/bupnp.o: $(SRCDIR)/bupnp.c $(HEADERS)

$(OBJDIR)/bupnpservice.o: $(SRCDIR)/bupnpservice.c $(HEADERS)
