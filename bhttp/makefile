
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bhttp.c $(SRCDIR)/bhttpres.c $(SRCDIR)/bhttpdav.c \
		$(SRCDIR)/bhttpws.c $(SRCDIR)/bhttpsip.c \
		$(SRCDIR)/bhttputil.c
HEADERS=$(SOURCES:%.c=%.h) bhttpconfig.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

HTTPLIB=bhttplib.a

PROGSOURCES=$(SRCDIR)/bhttpmain.c
PROGOBJECTS=$(OBJDIR)/bhttpmain.o

CC=gcc
CFLAGS+=-g -fno-diagnostics-color

AR=ar
ARFLAGS=-r

IOLIB_PATH=../bstreamio
IOLIB=$(IOLIB_PATH)/bstreamio.a

bhttp: $(PROGOBJECTS) $(HTTPLIB) $(IOLIB) $(TLSDEPS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(HTTPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(PROGOBJECTS) $(OBJECTS) $(HTTPLIB)

$(OBJDIR)/bhttpmain.o: $(SRCDIR)/bhttpmain.c $(HEADERS)
$(OBJDIR)/bhttp.o: $(SRCDIR)/bhttp.c $(HEADERS)
$(OBJDIR)/bhttpres.o: $(SRCDIR)/bhttpres.c $(HEADERS)
$(OBJDIR)/bhttpdav.o: $(SRCDIR)/bhttpdav.c $(HEADERS)
$(OBJDIR)/bhttpws.o: $(SRCDIR)/bhttpws.c $(HEADERS)
$(OBJDIR)/bhttpsip.o: $(SRCDIR)/bhttpsip.c $(HEADERS)
$(OBJDIR)/bhttputil.o: $(SRCDIR)/bhttputil.c $(HEADERS)

$(IOLIB):
	make -C $(IOLIB_PATH)

$(MBEDTLS_PATH)/library/%.a:
	make -C $(MBEDTLS_PATH) lib CC="$(CC)" CFLAGS="$(CFLAGS)"


