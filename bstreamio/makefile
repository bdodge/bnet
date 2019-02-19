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

CERT_PATH=../certs
CERTS=$(CERT_PATH)/ht_privkey.h $(CERT_PATH)/ht_ourcrt.h

SOURCES=$(SRCDIR)/bstreamio.c $(SRCDIR)/bstreamtls.c
HEADERS=$(SOURCES:%.c=%.h)
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

ifndef NO_MBED_TLS
	CFLAGS+=-DIOSTREAM_SUPPORT_TLS
endif

PROGSOURCES=$(SRCDIR)/bstreamiotest.c
PROGOBJECTS=$(OBJDIR)/bstreamiotest.o

bstreamiotest: $(PROGOBJECTS) $(IOLIB)
	$(CC) $(CFLAGS) -o $@ $(PROGOBJECTS) $(IOLIB) $(LIBS)

library: $(IOLIB)

$(IOLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $?

clean:
	rm $(OBJECTS) $(IOLIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/bstreamio.o: $(SRCDIR)/bstreamio.c $(HEADERS)
$(OBJDIR)/bstreamtls.o: $(SRCDIR)/bstreamtls.c $(HEADERS) $(CERTS)

$(OBJDIR)/bstreamiotest.o: $(SRCDIR)/bstreamiotest.c $(HEADERS)

