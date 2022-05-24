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

SOURCES=$(SRCDIR)/bsipmain.c	\
		$(SRCDIR)/bsip.c

HEADERS=$(SOURCES:%.c=%.h) bsipconfig.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBS= $(HTTPLIB) $(XMLLIB) $(IOLIB) $(UTILLIB)
LIBDIRS= $(IO_PATH) $(UTIL_PATH)
LIBINCLS= $(LIBDIRS:%=-I%)
CFLAGS += $(LIBINCLS)

ifdef BNET_TLS
	CFLAGS += -DHTTP_SUPPORT_TLS=$(BNET_TLS)
endif
ifdef BNET_ZLIB
	CFLAGS += -DHTTP_SUPPORT_COMPRESSION=$(BNET_ZLIB)
endif

EXTRA_DEFINES += HTTP_SUPPORT_UDP=1

CFLAGS += $(EXTRA_DEFINES:%=-D%)

PROGSOURCES=$(SRCDIR)/bsiptest.c
PROGOBJECTS=$(OBJDIR)/bsiptest.o

CFLAGS+=-I$(HTTP_PATH) -I$(UTIL_PATH) -I$(IO_PATH)

bsip: $(PROGOBJECTS) $(SIPLIB) $(LIBS) $(TLSDEPS) $(ZLIBDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(SIPLIB)

$(SIPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	make -C $(HTTP_PATH) EXTRA_DEFINES="$(EXTRA_DEFINES)" clean
	rm -f $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB)

$(OBJDIR)/bsiptest.o: $(SRCDIR)/bsiptest.c $(HEADERS)

$(OBJDIR)/bsipmain.o: $(SRCDIR)/bsipmain.c $(HEADERS)
$(OBJDIR)/bsipp.o: $(SRCDIR)/bsip.c $(HEADERS)

