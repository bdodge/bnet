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

SRCROOT=../..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/battrparse.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bianaimp.c
PROGOBJECTS=$(OBJDIR)/bianaimp.o

CFLAGS+=-I$(IPP_PATH) -I$(HTTP_PATH) -I$(IO_PATH) -I$(UTIL_PATH)

bianaimp: $(PROGOBJECTS) $(OBJECTS) $(IOLIB) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

clean:
	rm $(PROGOBJECTS) $(OBJECTS) bianaimp

$(OBJDIR)/bianaimp.o: $(SRCDIR)/bianaimp.c $(HEADERS)

$(OBJDIR)/battrparse.o: $(SRCDIR)/battrparse.c $(HEADERS)

