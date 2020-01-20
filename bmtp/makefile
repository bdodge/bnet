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
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bmtp.c	\
		$(SRCDIR)/bptpproto.c	\
		$(SRCDIR)/bptpprops.c	\
		$(SRCDIR)/bptpfiles.c	\
		$(SRCDIR)/bptpio.c

HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBS= $(UPNPLIB) $(HTTPLIB) $(XMLLIB) $(IOLIB) $(UTILLIB)
LIBDIRS= $(HTTP_PATH) $(UPNP_PATH) $(XML_PATH) $(IO_PATH) $(UTIL_PATH)
LIBINCLS= $(LIBDIRS:%=-I%)
CFLAGS += $(LIBINCLS)

ifdef BNET_TLS
	CFLAGS += -DHTTP_SUPPORT_TLS=$(BNET_TLS)
endif
ifdef BNET_ZLIB
	CFLAGS += -DHTTP_SUPPORT_COMPRESSION=$(BNET_ZLIB)
endif
CFLAGS += $(EXTRA_DEFINES:%=-D%) -Wno-format-truncation

PROGSOURCES=$(SRCDIR)/bmtpipserver.c
PROGOBJECTS=$(OBJDIR)/bmtpipserver.o

bmtpipserver: $(PROGOBJECTS) $(MTPLIB) $(LIBS) $(TLSDEPS) $(ZLIBDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(MTPLIB)

$(MTPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(PROGOBJECTS) $(OBJECTS) $(MTPLIB) bmtpipserver

$(OBJDIR)/bmtpipserver.o: $(SRCDIR)/bmtpipserver.c $(HEADERS)

$(OBJDIR)/bmtp.o: $(SRCDIR)/bmtp.c $(HEADERS)

$(OBJDIR)/bptpio.o: $(SRCDIR)/bptpio.c $(HEADERS)

$(OBJDIR)/bptpproto.o: $(SRCDIR)/bptpproto.c $(HEADERS)

$(OBJDIR)/bptpprops.o: $(SRCDIR)/bptpprops.c $(HEADERS)

$(OBJDIR)/bptpfiles.o: $(SRCDIR)/bptpfiles.c $(HEADERS)

