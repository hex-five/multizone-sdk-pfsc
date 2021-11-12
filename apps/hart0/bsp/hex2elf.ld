/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

OUTPUT_ARCH( "riscv" )

MEMORY { eNVM (rx) : ORIGIN = 0x20220100, LENGTH = 128K-256 }

SECTIONS { .text : { *(.text) } > eNVM }
