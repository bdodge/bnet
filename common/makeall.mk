#
# build libs and apps in dependant order
#
all:
	make -C butil
	make -C bxml
	make -C bjson
	make -C bosal
	make -C bmem
	make -C bstreamio
	make -C bimageio
	make -C bmediatree
	make -C bsasl
	make -C bxmpp
	make -C bftp
	make -C bhttp
	make -C bdavserver
	make -C bupnp
	make -C bui
	make -C bmtp
	make -C bsip
	make -C bmdns
	make -C bmqtt
	make -C bipp
	make -C bgcloudprt
	make -C bmqtt
	make -C bmqtt
	make -C bsnmp

