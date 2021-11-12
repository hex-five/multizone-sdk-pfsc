/* Copyright(C) 2021 Hex Five Security, Inc. - All Rights Reserved */

#include "mpfs_hal/mss_hal.h"

void e51(void) {

	/* Enable UART0 */
	SYSREG->SUBBLK_CLOCK_CR |=  SUBBLK_CLOCK_CR_MMUART0_MASK;
	SYSREG->SOFT_RESET_CR   &= ~SUBBLK_CLOCK_CR_MMUART0_MASK;

	/* Enable GPIO2 */
	SYSREG->SUBBLK_CLOCK_CR |=  SUBBLK_CLOCK_CR_GPIO2_MASK;
	SYSREG->SOFT_RESET_CR   &= ~SUBBLK_CLOCK_CR_GPIO2_MASK;

	/* GPIO interrupt mutex */
	SYSREG->GPIO_INTERRUPT_FAB_CR |= (1 << 30); // SW2
	SYSREG->GPIO_INTERRUPT_FAB_CR |= (1 << 31); // SW3

	/* Enable PLIC */
	PLIC_SetPriority_Threshold(0);

	/* Return to MultiZone boot */
	asm("csrr t0, mscratch; csrw mtvec, t0; ecall");

}
