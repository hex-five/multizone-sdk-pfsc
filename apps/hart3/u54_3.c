/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <stdio.h>
#include <string.h>

#include "mpfs_hal/mss_hal.h"
#include "drivers/mss_mmuart/mss_uart.h"

void u54_3(void) {

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

	mss_uart_instance_t *uart = &g_mss_uart3_lo;

	MSS_UART_init(uart, MSS_UART_115200_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

	MSS_UART_polled_tx_string (uart, "H3> ");

	while (1) {

		uint8_t rx_buff[1];

		uint8_t rx_size = MSS_UART_get_rx(uart, rx_buff, sizeof(rx_buff));

		if (rx_size > 0) {
			MSS_UART_polled_tx_string(uart, rx_buff);
		}

	}

} // u54_3()


/* HART3 Software interrupt handler */
static volatile uint32_t count_sw_ints_h3 = 0U;
void Software_h3_IRQHandler(void){
	uint64_t hart_id = read_csr(mhartid);
	count_sw_ints_h3++;
}
