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
BNET_ZLIB=1
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bgcp.c		\
		$(SRCDIR)/bgcpio.c		\
		$(SRCDIR)/bgcpnv.c		\
		$(SRCDIR)/bgcpcdd.c		\
		$(SRCDIR)/bgcpxmpp.c	\
		$(SRCDIR)/bgcpprivet.c	\
		$(SRCDIR)/bgcplocalprt.c

HEADERS=$(SOURCES:%.c=%.h) bgcpconfig.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBS= $(HTTPLIB) $(MDNSLIB) $(XMPPLIB) $(SASLLIB) $(JSONLIB) $(XMLLIB) $(IOLIB) $(MEMLIB) $(UTILLIB)
LIBDIRS= $(MDNS_PATH) $(IO_PATH) $(MEM_PATH) $(UTIL_PATH)
LIBINCLS= $(LIBDIRS:%=-I%)
CFLAGS += $(LIBINCLS)

ifdef BNET_TLS
	CFLAGS += -DHTTP_SUPPORT_TLS=$(BNET_TLS)
endif
ifdef BNET_ZLIB
	CFLAGS += -DHTTP_SUPPORT_COMPRESSION=$(BNET_ZLIB)
endif

CFLAGS += $(EXTRA_DEFINES:%=-D%)

PROGSOURCES=$(SRCDIR)/bgcptest.c
PROGOBJECTS=$(OBJDIR)/bgcptest.o

CFLAGS+=-I$(HTTP_PATH) -I$(XMPP_PATH) -I$(SASL_PATH) -I$(JSON_PATH) -I$(XML_PATH) -I$(UTIL_PATH) -I$(IO_PATH)

bgcp: $(PROGOBJECTS) $(GCPLIB) $(LIBS) $(TLSDEPS) $(ZLIBDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(GCPLIB)

$(GCPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	make -C $(HTTP_PATH) clean
	rm -f $(PROGOBJECTS) $(OBJECTS) $(GCPLIB) $(HTTPLIB)

$(OBJDIR)/bgcptest.o: $(SRCDIR)/bgcptest.c $(HEADERS)

$(OBJDIR)/bgcp.o: $(SRCDIR)/bgcp.c $(HEADERS)
$(OBJDIR)/bgcpio.o: $(SRCDIR)/bgcpio.c $(HEADERS)
$(OBJDIR)/bgcpnv.o: $(SRCDIR)/bgcpnv.c $(HEADERS)
$(OBJDIR)/bgcpcdd.o: $(SRCDIR)/bgcpcdd.c $(HEADERS)
$(OBJDIR)/bgcpcmpp.o: $(SRCDIR)/bgcpxmpp.c $(HEADERS)
$(OBJDIR)/bgcpprivet.o: $(SRCDIR)/bgcpprivet.c $(HEADERS)
$(OBJDIR)/bgcplocalprt.o: $(SRCDIR)/bgcplocalprt.c $(HEADERS)

