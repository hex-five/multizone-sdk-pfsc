/* Copyright(C) 2021 Hex Five Security, Inc. - All Rights Reserved */

#include "mpfs_hal/mss_hal.h"

/* Overrides weak symbol in ext/pfsc-platform/mpfs_hal/startup_gcc/system_startup.c */
void e51(void) {

	/* Enable UARTs */
	SYSREG->SUBBLK_CLOCK_CR |=  SUBBLK_CLOCK_CR_MMUART0_MASK |
                                SUBBLK_CLOCK_CR_MMUART1_MASK |
                                SUBBLK_CLOCK_CR_MMUART2_MASK |
                                SUBBLK_CLOCK_CR_MMUART3_MASK |
                                SUBBLK_CLOCK_CR_MMUART4_MASK ;
	SYSREG->SOFT_RESET_CR   &=~(SUBBLK_CLOCK_CR_MMUART0_MASK |
                                SUBBLK_CLOCK_CR_MMUART1_MASK |
                                SUBBLK_CLOCK_CR_MMUART2_MASK |
                                SUBBLK_CLOCK_CR_MMUART3_MASK |
                                SUBBLK_CLOCK_CR_MMUART4_MASK);

    /* Enable GPIO2 */
    SYSREG->SUBBLK_CLOCK_CR |= SUBBLK_CLOCK_CR_GPIO2_MASK;
    SYSREG->SOFT_RESET_CR &= ~SUBBLK_CLOCK_CR_GPIO2_MASK;

    /* GPIO interrupt mutex */
    SYSREG->GPIO_INTERRUPT_FAB_CR |= (1 << 30); // SW2
    SYSREG->GPIO_INTERRUPT_FAB_CR |= (1 << 31); // SW3

    /* Enable PLIC */
    PLIC_SetPriority_Threshold(0);

    /* Release U54 hart1,2,3,4 */
    raise_soft_interrupt(1);
    raise_soft_interrupt(2);
    raise_soft_interrupt(3);
    raise_soft_interrupt(4);

    /* Boot MultiZone TEE */
    asm("csrr t0, mscratch; csrw mtvec, t0; ecall");

}

/* Overrides weak symbol in ext/pfsc-platform/mpfs_hal/startup_gcc/system_startup.c */
uint8_t init_pmp(uint8_t hart_id) {
  //pmp_configure(hart_id);
    return (0U);
}
