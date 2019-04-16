
# meant to be used from makecommon.mk with OS, etc. set
#
$(BLDZLIB_PATH)/%.a:
	$(LIBZ_PATH)/configure
	make \ OS=$(OS) CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) ARFLAGS=$(ARFLAGS) LD=$(LD)

