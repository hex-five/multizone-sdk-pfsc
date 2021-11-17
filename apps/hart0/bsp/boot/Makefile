# Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved

TARGET = boot.elf

EXT_DIR := ../../../../ext
PLATFORM_DIR      := $(EXT_DIR)/bare-metal-lib/src/platform
ICICLE_KIT_ES_DIR := $(EXT_DIR)/icicle-kit-es
SOC_CONF_DIR      := $(EXT_DIR)/icicle-kit-es/soc_config
SOC_CONF_GEN_DIR  := $(EXT_DIR)/bare-metal-lib/src/platform/soc_config_generator
SOC_CONF_XML_DIR  := $(EXT_DIR)/icicle-ref-design/XML
MULTIZONE_DIR     := $(EXT_DIR)/multizone

INCLUDES += -I $(ICICLE_KIT_ES_DIR)
INCLUDES += -I $(ICICLE_KIT_ES_DIR)/platform_config
INCLUDES += -I $(PLATFORM_DIR)
INCLUDES += -I $(PLATFORM_DIR)/platform_config_reference
INCLUDES += -I $(MULTIZONE_DIR)

LINKER_SCRIPT = $(ICICLE_KIT_ES_DIR)/platform_config/linker/mpfs-$(MEM).ld

C_SRCS += ../../../hart0/e51.c
C_SRCS += ../../../hart1/u54_1.c ../../../hart1/main.c
C_SRCS += ../../../hart2/u54_2.c
C_SRCS += ../../../hart3/u54_3.c
C_SRCS += ../../../hart4/u54_4.c

ASM_SRCS+= $(PLATFORM_DIR)/mpfs_hal/startup_gcc/mss_entry.S
#ASM_SRCS+= $(PLATFORM_DIR)/mpfs_hal/startup_gcc/mss_mutex.S
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/startup_gcc/system_startup.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/startup_gcc/newlib_stubs.c
#C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_axiswitch.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_clint.c
#C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_h2f.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_irq_handler_stubs.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_l2_cache.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_mpu.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_mtrap.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_plic.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_pmp.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/mss_util.c
#C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_cfm.c
#C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_ddr.c
#C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_ddr_debug.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_io.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_nwc_init.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_pll.c
C_SRCS  += $(PLATFORM_DIR)/mpfs_hal/common/nwc/mss_sgmii.c
C_SRCS  += $(PLATFORM_DIR)/drivers/mss_mmuart/mss_uart.c

CFLAGS += -march=rv64gc -mabi=lp64d # g = imafd
CFLAGS += -mcmodel=medany -msmall-data-limit=8
CFLAGS += -mstrict-align -mno-save-restore
CFLAGS += -fmessage-length=0 -fsigned-char
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Og -g # MCHP Bug: anything but -O0 => GDB msg "The riscv_frame_cache's start_addr is ..."
CFLAGS += -Wextra -DNDEBUG
CFLAGS += -std=gnu11 -Wstrict-prototypes -Wbad-function-cast

LDFLAGS += -T $(LINKER_SCRIPT)
LDFLAGS += --specs=nano.specs --specs=nosys.specs
LDFLAGS += -nostartfiles
LDFLAGS += -Xlinker --gc-sections

# =================================================================================

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

$(ASM_OBJS): %.o: %.S $(HEADERS) $(SOC_CONF_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(C_OBJS): %.o: %.c $(HEADERS) $(SOC_CONF_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(SOC_CONF_DIR):
	@python3 $(SOC_CONF_GEN_DIR)/mpfs_configuration_generator.py \
    $(SOC_CONF_XML_DIR)/ICICLE_MSS_cfg.xml \
    $(ICICLE_KIT_ES_DIR)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(LINK_OBJS) $(HEX) $(BIN) $(LST) $(MAP) $(SIZ) $(SOC_CONF_DIR)
