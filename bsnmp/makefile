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

SOURCES=$(SRCDIR)/bsnmp.c \
		$(SRCDIR)/bsnmpobject.c \
		$(SRCDIR)/bsnmpio.c \
		$(SRCDIR)/bsnmpvar.c \
		$(SRCDIR)/bsnmpber.c \
		$(SRCDIR)/bsnmputils.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bsnmptest.c
PROGOBJECTS=$(OBJDIR)/bsnmptest.o

CFLAGS+=-I$(IO_PATH)  -I$(UTIL_PATH)

bsnmp: $(PROGOBJECTS) $(SNMPLIB) $(IOLIB) $(UTILLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(SNMPLIB)

$(SNMPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(SNMPLIB) $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(SNMPLIB)

$(OBJDIR)/bsnmptest.o: $(SRCDIR)/bsnmptest.c $(HEADERS)
$(OBJDIR)/bsnmp.o: $(SRCDIR)/bsnmp.c $(HEADERS)
$(OBJDIR)/bsnmp.o: $(SRCDIR)/bsnmpobject.c $(HEADERS)
$(OBJDIR)/bsnmpvar.o: $(SRCDIR)/bsnmpvar.c $(HEADERS)
$(OBJDIR)/bsnmpio.o: $(SRCDIR)/bsnmpio.c $(HEADERS)
$(OBJDIR)/bsnmpber.o: $(SRCDIR)/bsnmpber.c $(HEADERS)
$(OBJDIR)/bsnmputils.o: $(SRCDIR)/bsnmputils.c $(HEADERS)


