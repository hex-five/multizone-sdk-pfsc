/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <string.h>
#include <stdlib.h> // itoa()

#include "platform.h"
#include "multizone.h"

typedef enum {zone1=1, zone2, zone3, zone4, zone5, zone6, zone7, zone8} Zone;

volatile char const inbox[8][16] = { "", "", "", "", "", "", "", "" };
int inbox_is_empty(void){

    CSRC(mie, 1<<3);

    const int empty =  (inbox[0][0]=='\0' &&
                        inbox[1][0]=='\0' &&
                        inbox[2][0]=='\0' &&
                        inbox[3][0]=='\0' &&
                        inbox[4][0]=='\0' &&
                        inbox[5][0]=='\0' &&
                        inbox[6][0]=='\0' &&
                        inbox[7][0]=='\0'  );

    CSRS(mie, 1<<3);

    return empty;
}

// ------------------------------------------------------------------------
static void (*trap_vect[__riscv_xlen])(void) = {};
__attribute__((interrupt())) void trp_handler(void)	 { // trap handler (0)

	const unsigned long mcause = MZONE_CSRR(CSR_MCAUSE);

	switch (mcause) {
	case 0:	break; // Instruction address misaligned
	case 1:	break; // Instruction access fault
	case 3:	break; // Breakpoint
	case 4:	break; // Load address misaligned
	case 5:	break; // Load access fault
	case 6:	break; // Store/AMO address misaligned
	case 7:	break; // Store access fault
	case 8:	break; // Environment call from U-mode
	}

	for(;;);

}
__attribute__((interrupt())) void msi_handler(void)  { // machine software interrupt (3)

    for (Zone zone = zone1; zone <= zone8; zone++) {
        char msg[16];
        if (MZONE_RECV(zone, msg))
            memcpy((char*) &inbox[zone-1][0], msg, sizeof inbox[0]);
    }

}
__attribute__((interrupt())) void tmr_handler(void)  { // machine timer interrupt (7)

	// togle LED2 - thread safe
    GPIO_REG(GPIO_REG(GPIO_GPOUT) & LED2 ? GPIO_CLEAR_BITS : GPIO_SET_BITS) = LED2;

	// reset free running tstimer (clear irq7 mip)
	MZONE_ADTIMECMP((uint64_t)500*RTC_FREQ/1000);

}
__attribute__((interrupt())) void plic_handler(void) { // plic interrupt (11)

	const uint32_t plic_int = PLIC_REG(PLIC_CLAIM); // PLIC claim

	const uint64_t T = MZONE_RDTIME();

	switch (plic_int) {

	case PLIC_SRC_SW2: {

	    static uint64_t debounce_sw2 = 0;
        if (T > debounce_sw2){
            debounce_sw2 = T + 200*RTC_FREQ/1000;
            static unsigned count_sw2 = 0;
            char msg[16]="IRQ SW2 ("; utoa(count_sw2++, msg+9, 10); strcat(msg, ")");
            for (Zone zone = zone1; zone <= zone8; zone++) {
                MZONE_SEND(zone, msg);
            }
            GPIO_REG(GPIO_REG(GPIO_GPOUT) & LED3 ? GPIO_CLEAR_BITS : GPIO_SET_BITS) = LED3;
        }

		GPIO_REG(GPIO_INTR) |= SW2; // clear irq
	    } break;

	case PLIC_SRC_SW3: {

        static uint64_t debounce_sw3 = 0;
		if (T > debounce_sw3){
		    debounce_sw3 = T + 200*RTC_FREQ/1000;
		    static unsigned count_sw3 = 0;
            char msg[16]="IRQ SW3 ("; utoa(count_sw3++, msg+9, 10); strcat(msg, ")");
            for (Zone zone = zone1; zone <= zone8; zone++) {
                MZONE_SEND(zone, msg);
            }
            GPIO_REG(GPIO_REG(GPIO_GPOUT) & LED4 ? GPIO_CLEAR_BITS : GPIO_SET_BITS) = LED4;
		}

		GPIO_REG(GPIO_INTR) |= SW3; // clear irq
	    } break;

	}

	PLIC_REG(PLIC_CLAIM) = plic_int; // PLIC complete

}

// ------------------------------------------------------------------------
int main (void){

	//while(1) MZONE_WFI();
    //while(1) MZONE_YIELD();
	//while(1);

 	// vectored trap handler
	trap_vect [0] = trp_handler;
	trap_vect [3] = msi_handler;
	trap_vect [7] = tmr_handler;
	trap_vect[11] = plic_handler;
	CSRW(mtvec, trap_vect);	CSRS(mtvec, 0x1);

	// Enable GPIO inputs
	GPIO_REG(GPIO_INTR) = 0xFFFFFFFF; // reset interrupts
 	GPIO_REG(SW2_CFG) = 0b000<<5 | 1<<3 | 1<<1; // SW2 gpio2.30 input, 000 = irq lev h
	GPIO_REG(SW3_CFG) = 0b000<<5 | 1<<3 | 1<<1; // SW3 gpio2.31 input, 000 = irq lev h

	// Enable GPIO outputs
	GPIO_REG(LED2_CFG) = 1<<0; // LED2 (red) gpio2.17 output
	GPIO_REG(LED3_CFG) = 1<<0; // LED3 (yellow) gpio2.18 output
	GPIO_REG(LED4_CFG) = 1<<0; // LED4 (yellow) gpio2.19 output

	// Enable PLIC sources: SW2 priority 1, SW3 priority 2
	PLIC_REG(PLIC_PRI + (PLIC_SRC_SW2 << PLIC_SHIFT_PER_SRC)) = 0x1; // P1 (1 = lowest)
	PLIC_REG(PLIC_EN + PLIC_SRC_SW2/32*4) |= 1 << (PLIC_SRC_SW2 % 32);
	PLIC_REG(PLIC_PRI + (PLIC_SRC_SW3 << PLIC_SHIFT_PER_SRC)) = 0x1; // P1
	PLIC_REG(PLIC_EN + PLIC_SRC_SW3/32*4) |= 1 << (PLIC_SRC_SW3 % 32);
	CSRS(mie, 1<<11);

    // set & enable timer
	MZONE_ADTIMECMP((uint64_t)500*RTC_FREQ/1000);
    CSRS(mie, 1<<7);
    
    // enable msip/inbox interrupt
	CSRS(mie, 1<<3);    

	// enable global interrupts
    CSRS(mstatus, 1<<3);

    // turn on hartbit LED2 (red)
    GPIO_REG(GPIO_GPOUT) |= LED2;

    while (1) {

        // Asynchronous message handling example
        CSRC(mie, 1 << 3);

        for (Zone zone = zone1; zone <= zone8; zone++) {

            char * const msg = (char *)inbox[zone-1];

            if (msg[0] != '\0') {

                if (strcmp("ping", (char*) msg)==0) {
                    MZONE_SEND(zone, (char[16]){"pong"});

                } else if (strcmp("restart", msg)==0){
                    asm ("j _start");

                /* test: wfi resume with global irq disabled - irqs not taken */
                } else if (strcmp("mstatus.mie=0", (char*) msg)==0) {
                    CSRC(mstatus, 1 << 3);

                /* test: wfi resume with global irq enabled - irqs taken */
                } else if (strcmp("mstatus.mie=1", (char*) msg)==0) {
                    CSRS(mstatus, 1 << 3);

                /* test: preemptive scheduler - block for good */
                } else if (strcmp("block", (char*) msg)==0) {
                    CSRC(mstatus, 1 << 3); for (;;);

                } else {
                    // MZONE_SEND(zone, msg);

                }

                msg[0] = '\0';

            }

        }

        CSRS(mie, 1 << 3);

        // Wait For Interrupt - irqs taken if mstatus.mie=1
        MZONE_WFI();

        // test: wfi resume with global irq disabled - poll inbox
        if ( (CSRR(mstatus) & 1<<3) ==0){
            for (Zone zone = zone1; zone <= zone8; zone++) {
                char msg[16];
                if (MZONE_RECV(zone, msg))
                    memcpy((char*) &inbox[zone-1][0], msg, sizeof inbox[0]);
            }
        }

    }

}
