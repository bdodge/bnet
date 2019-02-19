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

SOURCES=$(SRCDIR)/bhttp.c \
		$(SRCDIR)/bhttpres.c \
		$(SRCDIR)/bhttpdav.c \
		$(SRCDIR)/bhttpws.c  \
		$(SRCDIR)/bhttputil.c

HEADERS=$(SOURCES:%.c=%.h) bhttpconfig.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBS= $(IOLIB) $(UTILLIB)
LIBDIRS= $(IO_PATH) $(UTIL_PATH)
LIBINCLS= $(LIBDIRS:%=-I%)
CFLAGS += $(LIBINCLS)

ifdef BNET_TLS
	CFLAGS += -DHTTP_SUPPORT_TLS=$(BNET_TLS)
endif
ifdef NO_WEB_SOCKETS
	CFLAGS += -DHTTP_SUPPORT_WEBSOCKET=$(NO_WEB_SOCKETS)
endif
CFLAGS += $(EXTRA_DEFINES:%=-D%)

PROGSOURCES=$(SRCDIR)/bhttpmain.c
PROGOBJECTS=$(OBJDIR)/bhttpmain.o

bhttp: $(PROGOBJECTS) $(HTTPLIB) $(LIBS) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^

library: $(HTTPLIB)

$(HTTPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(HTTPLIB) $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB)

$(OBJDIR)/bhttpmain.o: $(SRCDIR)/bhttpmain.c $(HEADERS)

$(OBJDIR)/bhttp.o: $(SRCDIR)/bhttp.c $(HEADERS)
$(OBJDIR)/bhttpres.o: $(SRCDIR)/bhttpres.c $(HEADERS)
$(OBJDIR)/bhttpdav.o: $(SRCDIR)/bhttpdav.c $(HEADERS)
$(OBJDIR)/bhttpws.o: $(SRCDIR)/bhttpws.c $(HEADERS)
$(OBJDIR)/bhttputil.o: $(SRCDIR)/bhttputil.c $(HEADERS)

