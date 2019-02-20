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

SOURCES=$(SRCDIR)/butil.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/butiltest.c
PROGOBJECTS=$(OBJDIR)/butiltest.o

butiltest: $(PROGOBJECTS) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(UTILLIB) $(SYSLIBS)

library: $(UTILLIB)

$(UTILLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(OBJECTS) $(UTILLIB)

$(OBJDIR)/butil.o: $(SRCDIR)/butil.c $(HEADERS)
$(OBJDIR)/butiltest.o: $(SRCDIR)/butiltest.c $(HEADERS)

