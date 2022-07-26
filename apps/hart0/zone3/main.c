/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <string.h>
#include "multizone.h"

typedef enum {zone1=1, zone2, zone3, zone4, zone5, zone6, zone7, zone8} Zone;

__attribute__(( interrupt())) void trap_handler(void){

    for( ;; );

}

int main (void){

    //while(1) MZONE_WFI();
    //while(1) MZONE_YIELD();
    //while(1);

    CSRW(mtvec, trap_handler); // register trap handler
    CSRS(mie, 1<<3);           // wake up on msip/inbox
	CSRC(mstatus, 1<<3);	   // disable global interrupts - no irq taken

    while (1) {

	    // Synchronous message handling example
        for (Zone zone = zone1; zone <= zone8; zone++) {

            char msg[16]; if (MZONE_RECV(zone, msg)) {

                if (strcmp("ping", msg) == 0)
                    MZONE_SEND(zone, (char[16]){"pong"});

                else if (strcmp("restart", msg)==0)
                    asm ("j _start");

				/* test: wfi resume with global irq disabled - no irq taken */
                else if (strcmp("mstatus.mie=0", msg)==0)
                    CSRC(mstatus, 1<<3);

				/* test: wfi resume with global irq enabled - irqs taken */
                else if (strcmp("mstatus.mie=1", msg)==0)
                    CSRS(mstatus, 1<<3);
                    
                else if (strcmp("block", msg) == 0)
                    for( ;; );                    

                //else MZONE_SEND(zone, msg);
            }

        }

        // Suspend waiting for incoming msg
        MZONE_WFI();

    }

}
