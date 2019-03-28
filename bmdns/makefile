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

SOURCES=$(SRCDIR)/bmdns.c
HEADERS=$(SOURCES:%.c=%.h) bmdnsconfig.h $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bmdnstest.c
PROGOBJECTS=$(OBJDIR)/bmdnstest.o

CFLAGS+=-I$(UTIL_PATH) -I$(IO_PATH) -I$(MEM_PATH)

mdnsr: $(PROGOBJECTS) $(MDNSLIB) $(MEMLIB) $(IOLIB) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(MDNSLIB)

$(MDNSLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(MDNSLIB) $^

clean:
	rm $(PROGOBJECTS) $(MDNSLIB) $(OBJECTS)

$(OBJDIR)/bmdnstest.o: $(SRCDIR)/bmdnstest.c $(HEADERS)
$(OBJDIR)/bmdns.o: $(SRCDIR)/bmdns.c $(HEADERS)


