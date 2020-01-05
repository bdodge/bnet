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

SOURCES=$(SRCDIR)/bmediatree.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bmediatest.c
PROGOBJECTS=$(OBJDIR)/bmediatest.o

CFLAGS+=-I$(UTIL_PATH)

bmediatest: $(PROGOBJECTS) $(MEDIALIB) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(MEDIALIB) $(UTILLIB) $(SYSLIBS)

library: $(MEDIALIB)

$(MEDIALIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(OBJECTS) $(MEDIALIB)

$(OBJDIR)/bmediatree.o: $(SRCDIR)/bmediatree.c $(HEADERS)
$(OBJDIR)/bmediatest.o: $(SRCDIR)/bmediatest.c $(HEADERS)

