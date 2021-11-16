/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <stdio.h>
#include <string.h>

#include "mpfs_hal/mss_hal.h"
#include "drivers/mss_mmuart/mss_uart.h"

void e51(void) {

	/* Turn on clocks */
	SYSREG->SUBBLK_CLOCK_CR |= SUBBLK_CLOCK_CR_MMUART1_MASK;
	SYSREG->SUBBLK_CLOCK_CR |= SUBBLK_CLOCK_CR_MMUART2_MASK;

	/* Remove soft reset */
	SYSREG->SOFT_RESET_CR &= ~SUBBLK_CLOCK_CR_MMUART1_MASK;
	SYSREG->SOFT_RESET_CR &= ~SUBBLK_CLOCK_CR_MMUART2_MASK;

	/* Resume other harts */
	raise_soft_interrupt(1);
	raise_soft_interrupt(2);

	while(1){;}

} // e51()

/* HART0 Software interrupt handler */
static volatile uint32_t count_sw_ints_h0 = 0U;
void Software_h0_IRQHandler(void){
	uint64_t hart_id = read_csr(mhartid);
	count_sw_ints_h0++;
}
