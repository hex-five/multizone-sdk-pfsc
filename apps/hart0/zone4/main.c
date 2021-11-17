/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <string.h>
#include "multizone.h"

typedef enum {zone0, zone1, zone2, zone3, zone4} Zone;

__attribute__(( interrupt())) void trap_handler(void){

    for( ;; );

}

int main (void){

    //while(1) MZONE_WFI();
    //while(1) MZONE_YIELD();
    //while(1);

    CSRW(mtvec, trap_handler); // register trap handler
    CSRS(mie, 1<<3);           // wake up on msip/inbox
    CSRC(mstatus, 1<<3);       // disable global interrupts

    while (1) {

        // Message handler
        for (Zone zone = zone0; zone <= zone4; zone++) {

            char msg[16]="";

            if (MZONE_RECV(zone, msg)) {
                if (strcmp("ping", msg) == 0)
                    MZONE_SEND(zone, (char[16]){"pong"});
                else if (strcmp("block", msg) == 0)
                    for( ;; );
                else if (strcmp("mie=0", msg)==0)
                    CSRC(mstatus, 1<<3);
                else if (strcmp("mie=1", msg)==0)
                    CSRS(mstatus, 1<<3);
                else
                    MZONE_SEND(zone, msg);
            }

        }

        // Suspend waiting for incoming msg
        MZONE_WFI();

    }

}
