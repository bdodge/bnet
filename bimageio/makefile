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
BNET_ZLIB=1
BNET_PNG=1
BNET_JPEG=1

SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bimageio.c		\
		$(SRCDIR)/bimgpng.c

HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bimgtest.c
PROGOBJECTS=$(OBJDIR)/bimgtest.o

CFLAGS+=-I$(UTIL_PATH) -I$(IO_PATH) -I$(MEM_PATH) -I$(OBJDIR)

bimg: $(PROGOBJECTS) $(IMGLIB) $(MEMLIB) $(IOLIB) $(UTILLIB)  $(ZLIBDEPS) $(PNGDEPS) #(JPEGDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

library: $(IMGLIB)

$(IMGLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -f $(PROGOBJECTS) $(IMGLIB) $(OBJECTS)

$(OBJDIR)/bimgtest.o: $(SRCDIR)/bimgtest.c $(HEADERS)
$(OBJDIR)/bimageio.o: $(SRCDIR)/bimageio.c $(HEADERS)
$(OBJDIR)/bimgpng.o: $(SRCDIR)/bimgpng.c $(HEADERS)

