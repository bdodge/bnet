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

SOURCES=$(SRCDIR)/bmem.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bmemtest.c
PROGOBJECTS=$(OBJDIR)/bmemtest.o

CFLAGS+=-I$(HTTP_PATH) -I$(OSAL_PATH) -I$(UTIL_PATH)

bmemtest: $(PROGOBJECTS) $(MEMLIB) $(OSALLIB) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(MEMLIB)

$(MEMLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(OBJECTS) $(MEMLIB)

$(OBJDIR)/bmem.o: $(SRCDIR)/bmem.c $(HEADERS)
$(OBJDIR)/bmemtest.o: $(SRCDIR)/bmemtest.c $(HEADERS)

