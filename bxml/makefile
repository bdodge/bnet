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

SOURCES=$(SRCDIR)/bxml.c $(SRCDIR)/bxmlutil.c
HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bxmltest.c
PROGOBJECTS=$(OBJDIR)/bxmltest.o

bxmltest: $(PROGOBJECTS) $(XMLLIB)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(XMLLIB)

$(XMLLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(XMLLIB)

$(OBJDIR)/bxmltest.o: $(SRCDIR)/bxmltest.c $(HEADERS)
$(OBJDIR)/bxml.o: $(SRCDIR)/bxml.c $(HEADERS)


