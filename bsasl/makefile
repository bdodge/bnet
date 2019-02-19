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

SOURCES=$(SRCDIR)/bsasl.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bsasltest.c
PROGOBJECTS=$(OBJDIR)/bsasltest.o

CFLAGS+=-I$(UTIL_PATH) -I$(IO_PATH)

bsasltest: $(PROGOBJECTS) $(OBJECTS) $(SASLLIB) $(IOLIB) $(UTILLIB) $(TLSDEPS) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

library: $(SASLLIB)

$(SASLLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $(SASLLIB) $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(SASLLIB)

$(OBJDIR)/bsasltest.o: $(SRCDIR)/bsasltest.c $(HEADERS)
$(OBJDIR)/bsasl.o: $(SRCDIR)/bsasl.c $(HEADERS)

