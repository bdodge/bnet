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

SOURCES=$(SRCDIR)/bupnp.c	\
		$(SRCDIR)/bupnpservice.c \
		$(SRCDIR)/bupnpsoap.c	\
		$(SRCDIR)/bupnpevent.c	\
		$(SRCDIR)/bupnpvar.c

HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBS= $(HTTPLIB) $(XMLLIB) $(MEDIALIB) $(IOLIB) $(UTILLIB)
LIBDIRS= $(HTTP_PATH) $(XML_PATH) $(MEDIA_PATH) $(IO_PATH) $(UTIL_PATH)
LIBINCLS= $(LIBDIRS:%=-I%)
CFLAGS += $(LIBINCLS)

ifdef BNET_TLS
	CFLAGS += -DHTTP_SUPPORT_TLS=$(BNET_TLS)
endif
ifdef BNET_ZLIB
	CFLAGS += -DHTTP_SUPPORT_COMPRESSION=$(BNET_ZLIB)
endif
CFLAGS += $(EXTRA_DEFINES:%=-D%) -Wno-format-truncation

PROGSOURCES=$(SRCDIR)/bupnpmediaserver.c
PROGOBJECTS=$(OBJDIR)/bupnpmediaserver.o

bupnpms: $(PROGOBJECTS) $(UPNPLIB) $(LIBS) $(TLSDEPS) $(ZLIBDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(UPNPLIB)

$(UPNPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB)

$(OBJDIR)/bupnpmain.o: $(SRCDIR)/bupnpmain.c $(HEADERS)

$(OBJDIR)/bupnp.o: $(SRCDIR)/bupnp.c $(HEADERS)

$(OBJDIR)/bupnpservice.o: $(SRCDIR)/bupnpservice.c $(HEADERS)

$(OBJDIR)/bupnpsoap.o: $(SRCDIR)/bupnpsoap.c $(HEADERS)

$(OBJDIR)/bupnpevent.o: $(SRCDIR)/bupnpevent.c $(HEADERS)

$(OBJDIR)/bupnpvar.o: $(SRCDIR)/bupnpvar.c $(HEADERS)

