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

LIBS+=-lpthread

SOURCES=$(SRCDIR)/bosal.c $(SRCDIR)/bosal_sem.c $(SRCDIR)/bosal_msgq.c
HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bosaltest.c
PROGOBJECTS=$(OBJDIR)/bosaltest.o

bosaltest: $(PROGOBJECTS) $(OSALLIB)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(OSALLIB) $(LIBS)

library: $(OSALLIB)

$(OSALLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(OSALLIB)

$(OBJDIR)/bosal.o: $(SRCDIR)/bosal.c $(HEADERS)
$(OBJDIR)/bosal_sem.o: $(SRCDIR)/bosal_sem.c $(HEADERS)
$(OBJDIR)/bosal_msgq.o: $(SRCDIR)/bosal_msgq.c $(HEADERS)
$(OBJDIR)/bosaltest.o: $(SRCDIR)/bosaltest.c $(HEADERS)


