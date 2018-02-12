
SRCROOT=..
include $(SRCROOT)/common/makecommon.mk

SOURCES=$(SRCDIR)/bftp.c
HEADERS=$(SOURCES:%.c=%.h) $(SRCROOT)/common/bnetheaders.h
OBJECTS=$(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROGSOURCES=$(SRCDIR)/bftptest.c
PROGOBJECTS=$(OBJDIR)/bftptest.o

CFLAGS+=-I$(UTIL_PATH) -I$(IO_PATH)

bftptest: $(PROGOBJECTS) $(FTPLIB) $(IOLIB) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

library: $(FTPLIB)

$(FTPLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm $(PROGOBJECTS) $(FTPLIB) $(OBJECTS)

$(OBJDIR)/bftptest.o: $(SRCDIR)/bftptest.c $(HEADERS)
$(OBJDIR)/bftp.o: $(SRCDIR)/bftp.c $(HEADERS)

