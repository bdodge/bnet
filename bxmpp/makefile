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

SOURCES=$(SRCDIR)/bxmpp.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bxmpptest.c
PROGOBJECTS=$(OBJDIR)/bxmpptest.o

CFLAGS+=-I$(IO_PATH) -I$(XML_PATH) -I$(SASL_PATH) -I$(UTIL_PATH)

bxmpp: $(PROGOBJECTS) $(OBJECTS) $(XMLLIB) $(SASLLIB) $(IOLIB) $(UTILLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(XMPPLIB)

$(XMPPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(XMPPLIB) $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(XMPPLIB)

$(OBJDIR)/bxmpptest.o: $(SRCDIR)/bxmpptest.c $(HEADERS)
$(OBJDIR)/bxmpp.o: $(SRCDIR)/bxmpp.c $(HEADERS)


