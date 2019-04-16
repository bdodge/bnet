
# meant to be used from makecommon.mk with OS, etc. set
#
$(BLDJPEGT_PATH)/%.a:
	cmake -G"Unix Makefiles" $(LIBJPEGT_PATH)
	make  OS=$(OS) BNET_JPEG=$(BNET_JPEG) CC=$(CC) CFLAGS="$(CFLAGS)" AR=$(AR) ARFLAGS=$(ARFLAGS) LD=$(LD)

