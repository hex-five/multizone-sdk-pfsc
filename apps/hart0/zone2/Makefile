# Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved

TARGET = zone2.elf

BSP_DIR := ../bsp
NEWLIB_DIR := $(BSP_DIR)/newlib

LINKER_SCRIPT := linker.lds

C_SRCS += main.c

INCLUDES += -I ../../../ext/multizone

include $(NEWLIB_DIR)/newlib.mk
