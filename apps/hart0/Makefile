# Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved

#############################################################
# Rules for building hart0
#############################################################

.PHONY: all
all: clean
	$(MAKE) -C bsp/boot
	$(MAKE) -C zone1
	$(MAKE) -C zone2
	$(MAKE) -C zone3
	$(MAKE) -C zone4

	java -jar ../../ext/multizone/multizone.jar \
		--arch $(BOARD) \
		--config bsp/multizone-$(MEM).cfg \
		--boot bsp/boot/boot.hex \
		--output hart0.hex \
		zone1/zone1.hex \
		zone2/zone2.hex \
		zone3/zone3.hex \
		zone4/zone4.hex

.PHONY: clean
clean:
	$(MAKE) -C bsp/boot clean
	$(MAKE) -C zone1 clean
	$(MAKE) -C zone2 clean
	$(MAKE) -C zone3 clean
	$(MAKE) -C zone4 clean
	rm -f hart0.*
