/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <stdio.h>
#include <string.h>

#include "mpfs_hal/mss_hal.h"
#include "drivers/mss_mmuart/mss_uart.h"

void u54_1(void) {

	/*Clear pending software interrupt in case there was any.
      Enable only the software interrupt so that the E51 core can bring this
      core out of WFI by raising a software interrupt.*/
	clear_soft_interrupt();
	set_csr(mie, MIP_MSIP);

	/*Put this hart into WFI.*/
	do {__asm("wfi");} while(0 == (read_csr(mip) & MIP_MSIP));

	/*The hart is out of WFI, clear the SW interrupt. Hear onwards Application
	 * can enable and use any interrupts as required*/
	clear_soft_interrupt();

	__enable_irq();

	extern void main(void);	main();

} // u54_1()

