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

SOURCES=$(SRCDIR)/buiwindows.c	\
		$(SRCDIR)/buidialogs.c	\
		$(SRCDIR)/buigdi.c

HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBS= $(UPNPLIB) $(HTTPLIB) $(XMLLIB) $(IOLIB) $(UTILLIB)
LIBDIRS= $(HTTP_PATH) $(UPNP_PATH) $(XML_PATH) $(IO_PATH) $(UTIL_PATH)
LIBINCLS= $(LIBDIRS:%=-I%)
CFLAGS += $(LIBINCLS)

CFLAGS += $(EXTRA_DEFINES:%=-D%) -Wno-format-truncation

PROGSOURCES=$(SRCDIR)/buitest.c
PROGOBJECTS=$(OBJDIR)/buitest.o

buitest: $(PROGOBJECTS) $(UILIB) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS) -lX11

library: $(UILIB)

$(UILIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(PROGOBJECTS) $(OBJECTS) $(UILIB) buitest

$(OBJDIR)/buitest.o: $(SRCDIR)/buitest.c $(HEADERS)

$(OBJDIR)/buimain.o: $(SRCDIR)/buimain.c $(HEADERS)

$(OBJDIR)/buidial.o: $(SRCDIR)/buidial.c $(HEADERS)

