# Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved

#############################################################
# Platform definitions
#############################################################

BOARD ?= PFSC-ENVM
ifeq ($(BOARD), PFSC-ENVM)
    ARCH := rv64
    RISCV_ARCH := $(ARCH)imac
    RISCV_ABI := lp64
else ifeq ($(BOARD), PFSC-LIM)
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
# RISC-V Toolchain definitions
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
export LD      := $(CROSS_COMPILE)ld
export STRIP   := $(CROSS_COMPILE)strip
export SIZE    := $(CROSS_COMPILE)size

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
	rm -f multizone.elf multizone.hex multizone.bin bootmode0 bootmode1

#############################################################
# Load to LIM (debug - boot mode 0)
#############################################################

ifeq ($(BOARD), PFSC-LIM)

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
    GDB_LOAD_CMDS += -ex "monitor reset init"
    GDB_LOAD_CMDS += -ex "load"
    GDB_LOAD_CMDS += -ex "monitor resume"
    GDB_LOAD_CMDS += -ex "monitor shutdown"
    GDB_LOAD_CMDS += -ex "quit"
    
    .PHONY: load
    
    load:
	$(OPENOCD_BIN) $(OPENOCDARGS) & \
	$(GDB) multizone.hex $(GDB_LOAD_ARGS) $(GDB_LOAD_CMDS)

endif

#############################################################
# Load to eNVM (production - boot mode 1)
#############################################################

ifeq ($(BOARD), PFSC-ENVM)

    ifndef FPGENPROG
       $(error FPGENPROG not set)
    endif

    ifndef SC_INSTALL_DIR
       $(error SC_INSTALL_DIR not set)
    endif
    
    MPFS_BOOT_MODE_PROG := $(abspath $(SC_INSTALL_DIR))/extras/mpfs/mpfsBootmodeProgrammer.jar
    
    .PHONY: load
    
    load:
    # Convert multizone.hex to multizone.elf as required by mpfsBootmodeProgrammer.jar
	$(OBJCOPY) -S -I ihex -O binary multizone.hex multizone.bin && \
	$(LD) -b binary -r -o multizone.tmp multizone.bin && \
	$(OBJCOPY) --rename-section .data=.text --set-section-flags .data=alloc,code,load multizone.tmp && \
	$(LD) multizone.tmp -T bsp/PFSC-ENVM/hex2elf.ld -o multizone.elf && \
	$(STRIP) -s multizone.elf && \
	java -jar $(MPFS_BOOT_MODE_PROG) --bootmode 1 --die MPFS250T_ES --package FCVG484 multizone.elf && \
	rm -rf bootmode1 multizone.tmp multizone.bin 

endif