/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

/*
* Hardware setup / OEM low-level boot code
* Link to HEX format and pass to the multizone.jar via --boot option
* Runs in privileged machine mode "M" (after policy integrity check)
* Return execution to MultiZone via trap 0xb (ecall from M mode)
*/

#include "../platform.h"

.section .text.boot
.global boot

boot:

	# -------------------------------------------------------------------------------------------------------
	# mss_nwc_init.s
	# -------------------------------------------------------------------------------------------------------

	#  SYSREG->RTC_CLOCK_CR &= ~(0x01U<<16); /* disable RTC clock */
	li a4, 0x64; li a5, 0x20002000; sw a4,12(a5)

	#  SYSREG->RTC_CLOCK_CR = (LIBERO_SETTING_MSS_EXT_SGMII_REF_CLK /
	li a4, 0x7d; li a5, 0x20002000; sw a4,12(a5)

	#  SYSREG->RTC_CLOCK_CR |= (0x01U<<16); /* enable RTC clock */
	li a4, 0x1007d; li a5, 0x20002000; sw a4,12(a5)

	# SCBCFG_REGS->TIMER.TIMER = MSS_SCB_ACCESS_CONFIG
	li a4, 0xa080; li a5, 0x37080000; sw a4,8(a5)

	# SYSREG->DFIAPB_CR = 0x00000001U;
	li a4, 0x1; li a5, 0x20002000; sw a4,152(a5)

	# CFG_DDR_SGMII_PHY->DDRPHY_STARTUP.DDRPHY_STARTUP = (0x3FU << 16U) | (0x1FU << 8U);
	li a4, 0x3f1f00; li a5, 0x20007000; sw a4,8(a5)

	# CFG_DDR_SGMII_PHY->DYN_CNTL.DYN_CNTL = (0x01U<< 10U) | (0x7FU<<0U);
	li a4, 0x47f; li a5, 0x20008000; sw a4,-996(a5) # 0xc1c

	//# error |= mssio_setup();

	# SCB_REGS->MSSIO_CONTROL_CR.MSSIO_CONTROL_CR = (0x07U<<8U)|(0x01U<<11U)|(0x00U<<12U)|(0x00U<<13U)
	li a4, 0xf00; li a5, 0x20003000; sw a4,444(a5)

	# delay((uint32_t) 10U);
	li a4, 100; 0: addi a4, a4, -1; bnez a4, 0b

	# SCB_REGS->MSSIO_CONTROL_CR.MSSIO_CONTROL_CR = (0x00U<<8U)|(0x01U<<11U)|(0x00U<<12U)|(0x00U<<13U);
	li a4, 0x800; li a5, 0x20003000; sw a4,444(a5)

	# delay((uint32_t) 10U);
	li a4, 100; 0: addi a4, a4, -1; bnez a4, 0b

	# SCB_REGS->MSSIO_CONTROL_CR.MSSIO_CONTROL_CR = (0x00U<<8U)|(0x01U<<11U)|(0x01U<<12U)|(0x00U<<13U)
	li a4, 0x1800; li a5, 0x20003000; sw a4,444(a5)

	# delay((uint32_t) 10U);
	li a4, 100; 0: addi a4, a4, -1; bnez a4, 0b

	# SCB_REGS->MSSIO_CONTROL_CR.MSSIO_CONTROL_CR = (0x00U<<8U)|(0x01U<<11U)|(0x01U<<12U)|(0x01U<<13U);
	li a4, 0x3800; li a5, 0x20003000; sw a4,444(a5)

	# mss_plc.c pre_configure_sgmii_and_ddr_pll_via_scb() sgmii_mux_config_via_scb()

	# MSS_SCB_CFM_SGMII_MUX->SGMII_CLKMUX = LIBERO_SETTING_SGMII_SGMII_CLKMUX;
	li a4,    0x5; li a5, 0x3e200000; sw a4, 8(a5)

	# MSS_SCB_CFM_SGMII_MUX->CLK_XCVR = LIBERO_SETTING_SGMII_CLK_XCVR;
	li a4, 0x2C30; li a5, 0x3e200000; sw a4, 16(a5)

	# MSS_SCB_CFM_SGMII_MUX->RFCKMUX  = LIBERO_SETTING_SGMII_SGMII_CLKMUX;
	li a4,    0x5; li a5, 0x3e200000; sw a4, 4(a5)


	# -------------------------------------------------------------------------------------------------------
	# pfsoc_mss_top_sysreg (see mss_sysreg.h)
	# -------------------------------------------------------------------------------------------------------

	.equ SYSCTRL_BASE, 0x20002000
	la x2, SYSCTRL_BASE

	/* Bring peripheral out of reset */
	.equ SYSCTRL_SOFT_RESET,			0x88
	.equ SYSCTRL_SOFT_RESET_UART0_MSK,	1<<5
	.equ SYSCTRL_SOFT_RESET_GPIO2_MSK,	1<<0x16
	.equ SYSCTRL_SOFT_RESET_ENVM_MSK,	1<<0x0

    li x1, ~(SYSCTRL_SOFT_RESET_ENVM_MSK | SYSCTRL_SOFT_RESET_UART0_MSK | SYSCTRL_SOFT_RESET_GPIO2_MSK)
   #li x1, 0x0
	sw x1, SYSCTRL_SOFT_RESET(x2)

	/* Enable peripheral clock */
	.equ SYSCTRL_SUBBLK_CLOCK,  		 0x84
    .equ SYSCTRL_SUBBLK_CLOCK_UART0_MSK, 1<<5
    .equ SYSCTRL_SUBBLK_CLOCK_GPIO2_MSK, 1<<0x16

    li x1, SYSCTRL_SOFT_RESET_ENVM_MSK | SYSCTRL_SOFT_RESET_UART0_MSK | SYSCTRL_SOFT_RESET_GPIO2_MSK
   #li x1, 0xFFFFFFFF
	sw x1, SYSCTRL_SUBBLK_CLOCK(x2)

	/* SYSCTRL_GPIO_INT_FAB */
	.equ SYSCTRL_GPIO_INT_FAB,	  0x54
	.equ SYSCTRL_GPIO_INT_FAB_30, 1<<30 # GPIO2.30 SW2
	.equ SYSCTRL_GPIO_INT_FAB_31, 1<<31 # GPIO2.31 SW3

	li x1, SYSCTRL_GPIO_INT_FAB_31 | SYSCTRL_GPIO_INT_FAB_30
	sw x1, SYSCTRL_GPIO_INT_FAB(x2)

	# -------------------------------------------------------------------------------------------------------
	# Reset PLIC
	# -------------------------------------------------------------------------------------------------------
	li t0, PLIC_BASE + PLIC_EN_OFFSET + 0x80*0; sd zero, (t0) # Hart0 M
/*	li t0, PLIC_BASE + PLIC_EN_OFFSET + 0x80*1; sd zero, (t0) # Hart1 M
	li t0, PLIC_BASE + PLIC_EN_OFFSET + 0x80*2; sd zero, (t0) # Hart1 S
	li t0, PLIC_BASE + PLIC_EN_OFFSET + 0x80*3; sd zero, (t0) # Hart2 M
	li t0, PLIC_BASE + PLIC_EN_OFFSET + 0x80*4; sd zero, (t0) # Hart2 S
	li t0, PLIC_BASE + PLIC_EN_OFFSET + 0x80*5; sd zero, (t0) # Hart3 M
	li t0, PLIC_BASE + PLIC_EN_OFFSET + 0x80*6; sd zero, (t0) # Hart3 S
	li t0, PLIC_BASE + PLIC_EN_OFFSET + 0x80*7; sd zero, (t0) # Hart4 M
	li t0, PLIC_BASE + PLIC_EN_OFFSET + 0x80*8; sw zero, (t0) # Hart4 S
*/
	li t0, PLIC_BASE + PLIC_THRES_OFFSET + 0x1000*0; sw zero, (t0) # Hart0 M


/*	# -------------------------------------------------------------------------------------------------------
	# Wait 100ms for HW initialization - TBD: really necessary? 100ms?
	# -------------------------------------------------------------------------------------------------------
	la x2, CLINT_BASE+CLINT_MTIME; ld t0, (x2)
	li t1, RTC_FREQ/10; add t1, t0, t1
	1: ld t0, (x2)
	bltu t0, t1, 1b
*/

	# -------------------------------------------------------------------------------------------------------
	# exit to MultiZone
	# -------------------------------------------------------------------------------------------------------
	ecall
