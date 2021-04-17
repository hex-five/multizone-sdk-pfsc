# Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved

#############################################################
# Platform definitions
#############################################################

BOARD = PFSC
ifeq ($(BOARD), PFSC)
	ARCH := rv64
	RISCV_ARCH := $(ARCH)imac
	RISCV_ABI := lp64		
else
	$(error Unsupported board $(BOARD))
endif

#############################################################
# Arguments/variables available to all submakes
#############################################################

export BOARD
export RISCV_ARCH
export RISCV_ABI

#############################################################
# Toolchain definitions
#############################################################

ifndef RISCV
$(error RISCV not set)
endif

export CROSS_COMPILE := $(abspath $(RISCV))/bin/riscv64-unknown-elf-
export CC      := $(CROSS_COMPILE)gcc
export OBJDUMP := $(CROSS_COMPILE)objdump
export OBJCOPY := $(CROSS_COMPILE)objcopy
export GDB     := $(CROSS_COMPILE)gdb
export AR      := $(CROSS_COMPILE)ar

#############################################################
# Rules for building multizone
#############################################################

.PHONY: all 
all: clean
	$(MAKE) -C zone1
	$(MAKE) -C zone2
	$(MAKE) -C zone3
	$(MAKE) -C zone4
	$(MAKE) -C bsp/$(BOARD)/boot
	java -jar multizone.jar \
		--arch $(BOARD) \
		--config bsp/$(BOARD)/multizone.cfg \
		--boot bsp/$(BOARD)/boot/boot.hex \
		zone1/zone1.hex \
		zone2/zone2.hex \
		zone3/zone3.hex \
		zone4/zone4.hex

.PHONY: clean
clean: 
	$(MAKE) -C zone1 clean
	$(MAKE) -C zone2 clean
	$(MAKE) -C zone3 clean
	$(MAKE) -C zone4 clean
	$(MAKE) -C bsp/$(BOARD)/boot clean
	rm -f multizone.hex

#############################################################
# Load and debug variables and rules
#############################################################

ifndef OPENOCD
	$(error OPENOCD not set)
endif

OPENOCD_BIN := $(abspath $(OPENOCD))/bin/openocd

OPENOCDARGS += --command "set DEVICE MPFS"
OPENOCDARGS += --file $(abspath $(OPENOCD))/share/openocd/scripts/board/microsemi-riscv.cfg

GDB_PORT ?= 3333
GDB_LOAD_ARGS ?= --batch
GDB_LOAD_CMDS += -ex "set $target_riscv=1"
GDB_LOAD_CMDS += -ex "set arch riscv:$(ARCH)"
GDB_LOAD_CMDS += -ex "set mem inaccessible-by-default off"
GDB_LOAD_CMDS += -ex "target extended-remote localhost:$(GDB_PORT)"
GDB_LOAD_CMDS += -ex "monitor reset halt"
GDB_LOAD_CMDS += -ex "load"
GDB_LOAD_CMDS += -ex "monitor resume"
GDB_LOAD_CMDS += -ex "monitor shutdown"
GDB_LOAD_CMDS += -ex "quit"

.PHONY: load

load:
	$(OPENOCD_BIN) $(OPENOCDARGS) & \
	$(GDB) multizone.hex $(GDB_LOAD_ARGS) $(GDB_LOAD_CMDS)
	
