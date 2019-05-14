
# meant to be used from makecommon.mk with OS, etc. set
#
$(BLDPNG_PATH)/%.a:
	$(LIBPNG_PATH)/configure --prefix=$(PWD)/$(BLDPNG_PATH)
	make OS=$(OS) CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) ARFLAGS=$(ARFLAGS) LD=$(LD)
	make OS=$(OS) CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) ARFLAGS=$(ARFLAGS) LD=$(LD) install
	cp lib/libpng.a libpng.a


