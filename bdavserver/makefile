#
# Copyright 2020 Brian Dodge
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

BNET_TLS=1
BNET_ZLIB=1

include $(SRCROOT)/common/makecommon.mk

HEADERS=
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBS= $(IOLIB) $(UTILLIB) $(HTTPLIB) $(XMLLIB)
LIBDIRS= $(IO_PATH) $(UTIL_PATH) $(HTTP_PATH) $(XML_PATH)
LIBINCLS= $(LIBDIRS:%=-I%)
CFLAGS += $(LIBINCLS)

ifdef BNET_TLS
	CFLAGS += -DHTTP_SUPPORT_TLS=$(BNET_TLS)
endif
ifdef BNET_ZLIB
	CFLAGS += -DHTTP_SUPPORT_COMPRESSION=$(BNET_ZLIB)
endif
EXTRA_DEFINES += "HTTP_SUPPORT_WEBSOCKET=0 HTTP_SUPPORT_WEBDAV=1"

PROGSOURCES=$(SRCDIR)/bdavserver.c
PROGOBJECTS=$(OBJDIR)/bdavserver.o

bdavserver: $(PROGOBJECTS) $(HTTPLIB) $(LIBS) $(TLSDEPS) $(ZLIBDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

clean:
	rm -f $(PROGOBJECTS)

$(OBJDIR)/bdavserver.o: $(SRCDIR)/bdavserver.c $(HEADERS)

