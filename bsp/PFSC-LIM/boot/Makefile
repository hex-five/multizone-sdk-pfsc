# Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved

TARGET = boot.elf

BOARDS_DIR   := ../../../ext/bare-metal-lib/src/boards
PLATFORM_DIR := ../../../ext/bare-metal-lib/src/platform

C_SRCS   := ./boot.c

ASM_SRCS+= $(PLATFORM_DIR)/mpfs_hal/startup_gcc/mss_entry.S
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/startup_gcc/system_startup.c
#C_SRCS += $(PLATFORM_DIR)/mpfs_hal/common/mss_axiswitch.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_clint.c
#C_SRCS += $(PLATFORM_DIR)/mpfs_hal/common/mss_h2f.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_irq_handler_stubs.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_l2_cache.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_mpu.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_mtrap.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_plic.c
#C_SRCS += $(PLATFORM_DIR)/mpfs_hal/common/mss_pmp.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_util.c

#C_SRCS += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_cfm.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_ddr.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_ddr_debug.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_io.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_nwc_init.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_pll.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_sgmii.c

INCLUDES += -I $(PLATFORM_DIR)
INCLUDES += -I $(PLATFORM_DIR)/platform_config_reference
INCLUDES += -I $(BOARDS_DIR)/icicle-kit-es

CFLAGS += -march=rv64gc -mabi=lp64d -mcmodel=medany -msmall-data-limit=8 -mstrict-align -mno-save-restore -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections
CFLAGS += -Os -Wextra  -g -DNDEBUG
CFLAGS += -std=gnu11 -Wstrict-prototypes -Wbad-function-cast
#CFLAGS+= -Wa,-adhlns="boot.o.lst"

LINKER_SCRIPT = $(PLATFORM_DIR)/platform_config_reference/linker/mpfs-lim.ld
LDFLAGS += -T $(LINKER_SCRIPT)
LDFLAGS += --specs=nano.specs --specs=nosys.specs
LDFLAGS += -nostartfiles
LDFLAGS += -Xlinker --gc-sections
#LDFLAGS += -Xlinker -print-gc-sections

.PHONY: all
all: $(TARGET)

ASM_OBJS := $(ASM_SRCS:.S=.o)
C_OBJS := $(C_SRCS:.c=.o)

LINK_OBJS += $(ASM_OBJS) $(C_OBJS)
LINK_DEPS += $(LINKER_SCRIPT)

HEX = $(subst .elf,.hex,$(TARGET))
BIN = $(subst .elf,.bin,$(TARGET))
LST = $(subst .elf,.lst,$(TARGET))
SIZ = $(subst .elf,.siz,$(TARGET))

$(TARGET): $(LINK_OBJS) $(LINK_DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LINK_OBJS) -o $@ $(LDFLAGS)
	$(OBJDUMP) --all-headers --demangle --disassemble --file-headers --wide -D $(TARGET) > $(LST)
	$(SIZE) --format=sysv $(TARGET) > $(SIZ)
	$(OBJCOPY) -S -O ihex -j.text -j.ram_code -j.sdata -j.sdata2 -j.data --gap-fill 0x00 $(TARGET) $(HEX)
	$(OBJCOPY) -S -I ihex -O binary $(HEX) $(BIN)

$(ASM_OBJS): %.o: %.S $(BOARDS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(C_OBJS): %.o: %.c $(BOARDS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

XML_DIR := ../../../ext/icicle-ref-design/XML
$(BOARDS_DIR):
	mkdir -p $(BOARDS_DIR)/icicle-kit-es
	python3 $(PLATFORM_DIR)/soc_config_generator/mpfs_configuration_generator.py $(XML_DIR)/ICICLE_MSS_eMMC_cfg.xml $(BOARDS_DIR)/icicle-kit-es

.PHONY: clean
clean:
	rm -rf $(TARGET) $(LINK_OBJS) $(HEX) $(BIN) $(LST) $(SIZ) $(BOARDS_DIR)