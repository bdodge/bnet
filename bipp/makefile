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

SOURCES=$(SRCDIR)/bipplib.c		\
		$(SRCDIR)/bipp.c		\
		$(SRCDIR)/bippreq.c		\
		$(SRCDIR)/bipperror.c

HEADERS=$(SOURCES:%.c=%.h) bippconfig.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBS= $(HTTPLIB) $(IOLIB) $(UTILLIB)
LIBDIRS= $(IO_PATH) $(UTIL_PATH)
LIBINCLS= $(LIBDIRS:%=-I%)
CFLAGS += $(LIBINCLS)

ifdef BNET_TLS
	CFLAGS += -DHTTP_SUPPORT_TLS=$(BNET_TLS)
endif

CFLAGS += $(EXTRA_DEFINES:%=-D%)

PROGSOURCES=$(SRCDIR)/bipptest.c
PROGOBJECTS=$(OBJDIR)/bipptest.o

CFLAGS+=-I$(HTTP_PATH) -I$(UTIL_PATH) -I$(IO_PATH)

bipp: $(PROGOBJECTS) $(IPPLIB) $(LIBS) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(IPPLIB)

$(IPPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB)

$(OBJDIR)/bipptest.o: $(SRCDIR)/bipptest.c $(HEADERS)

$(OBJDIR)/bipplib.o: $(SRCDIR)/bipplib.c $(HEADERS)
$(OBJDIR)/bippp.o: $(SRCDIR)/bipp.c $(HEADERS)
