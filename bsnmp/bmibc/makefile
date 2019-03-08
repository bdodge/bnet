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

CFLAGS+=-I$(SRCROOT)/bsnmp -I$(SRCROOT)/bmem -I$(SRCROOT)/bosal

SOURCES=$(SRCDIR)/bmibcparse.c		\
		$(SRCDIR)/bmibcsym.c		\
		$(SRCDIR)/bmibcutils.c		\
		$(SRCDIR)/bmibctoken.c		\
		$(SRCDIR)/bmibckeywords.c	\
		$(SRCDIR)/bmibcerr.c
HEADERS=$(SOURCES:%.c=%.h) bmibc.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bmibc.c
PROGOBJECTS=$(OBJDIR)/bmibc.o

CFLAGS+=-I$(IO_PATH)  -I$(UTIL_PATH)

bmibc: $(PROGOBJECTS) $(OBJECTS) $(SNMPLIB) $(MEMLIB) $(IOLIB) $(UTILLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(SYSLIBS)

clean:
	rm $(PROGOBJECTS) $(OBJECTS) bmibc

$(OBJDIR)/bmibc.o: $(SRCDIR)/bmibc.c $(HEADERS)
$(OBJDIR)/bmibcerr.o: $(SRCDIR)/bmibcerr.c $(HEADERS)
$(OBJDIR)/bmibcsym.o: $(SRCDIR)/bmibcsym.c $(HEADERS)
$(OBJDIR)/bmibcutils.o: $(SRCDIR)/bmibcutils.c $(HEADERS)
$(OBJDIR)/bmibctoken.o: $(SRCDIR)/bmibctoken.c $(HEADERS)
$(OBJDIR)/bmibckeywords.o: $(SRCDIR)/bmibckeywords.c $(HEADERS)

