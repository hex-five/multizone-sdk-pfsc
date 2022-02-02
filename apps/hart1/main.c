/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <string.h>	    // strxxx()
#include <stdio.h>      // mss_printf() s_printf()
#include <stdlib.h>     // strtoul()

#include "mpfs_hal/mss_hal.h"
#include "drivers/mss/mss_mmuart/mss_uart.h"
#include "mss_multizone.h"

#define mss_printf(format, args...) { \
            snprintf(mss_uart_tx_buf, sizeof mss_uart_tx_buf, format, ## args); \
            MSS_UART_polled_tx_string (uart, (uint8_t *)mss_uart_tx_buf); \
        }

typedef enum {zone1=1, zone2, zone3, zone4} Zone;

char mss_uart_tx_buf [80];
mss_uart_instance_t *uart = &g_mss_uart1_lo;

static char inputline[32+1]="";

static volatile char inbox[4][16] = { {'\0'}, {'\0'}, {'\0'}, {'\0'} };
int inbox_empty(void){
	return (inbox[0][0]=='\0' && inbox[1][0]=='\0' && inbox[2][0]=='\0' && inbox[3][0]=='\0');
}

// ------------------------------------------------------------------------
void handle_m_trap_h1(uintptr_t * regs, uintptr_t mcause, uintptr_t mepc){

    uint64_t mtval = read_csr(mtval);

    switch(mcause){

    case CAUSE_LOAD_ACCESS :
        mss_printf("Load access fault : 0x%08x 0x%08x 0x%08x \n\r", (unsigned)mcause, (unsigned)mepc, (unsigned)mtval);
        write_csr(mepc, mepc + (((*(char *)mepc) & 0b11) == 0b11 ? 4 : 2)); // skip faulty instruction
        return;

    case CAUSE_STORE_ACCESS :
        mss_printf("Store access fault : 0x%08x 0x%08x 0x%08x \n\r", (unsigned)mcause, (unsigned)mepc, (unsigned)mtval);
        write_csr(mepc, mepc + (((*(char *)mepc) & 0b11) == 0b11 ? 4 : 2)); // skip faulty instruction
        return;

    case CAUSE_FETCH_ACCESS :
        mss_printf("Instr access fault : 0x%08x 0x%08x 0x%08x \n\r", (unsigned)mcause, (unsigned)mepc, (unsigned)mtval);
        break;

    default : mss_printf("Exception : 0x%08x 0x%08x 0x%08x \n\r", (unsigned)mcause, (unsigned)mepc, (unsigned)mtval);

    }

    mss_printf("Press any key to continue \n\r");
    char c='\0'; while( MSS_UART_get_rx(uart, (uint8_t *)&c, 1) == 0 ){;}
    inputline[0]='\0';
    asm ("la t0, main; csrw mepc, t0" : : : "t0"); // TBD: asm("j _start");

}

// ------------------------------------------------------------------------
void Software_h1_IRQHandler(void){

    for (Zone zone = zone1; zone <= zone4; zone++) {
        char msg[16];
        if (MZONE_RECV(zone, msg))
            memcpy((char*) &inbox[zone-1][0], msg, sizeof inbox[0]);
    }

    clear_soft_interrupt();

}

// ------------------------------------------------------------------------
void print_pmp(void){
// ------------------------------------------------------------------------

	#define TOR   0b00001000
	#define NA4   0b00010000
	#define NAPOT 0b00011000

//  #define PMP_R   0x01
//  #define PMP_W   0x02
//  #define PMP_X   0x04
//  #define PMP_A   0x18
//  #define PMP_L   0x80

    unsigned long pmpcfg0; asm ( "csrr %0, pmpcfg0" : "=r"(pmpcfg0) );
    unsigned long pmpcfg2; asm ( "csrr %0, pmpcfg2" : "=r"(pmpcfg2) );

	unsigned long pmpaddr[16];
	asm ( "csrr %0, pmpaddr0" : "=r"(pmpaddr[0]) );
	asm ( "csrr %0, pmpaddr1" : "=r"(pmpaddr[1]) );
	asm ( "csrr %0, pmpaddr2" : "=r"(pmpaddr[2]) );
	asm ( "csrr %0, pmpaddr3" : "=r"(pmpaddr[3]) );
	asm ( "csrr %0, pmpaddr4" : "=r"(pmpaddr[4]) );
	asm ( "csrr %0, pmpaddr5" : "=r"(pmpaddr[5]) );
	asm ( "csrr %0, pmpaddr6" : "=r"(pmpaddr[6]) );
	asm ( "csrr %0, pmpaddr7" : "=r"(pmpaddr[7]) );
    asm ( "csrr %0, pmpaddr8" : "=r"(pmpaddr[8]) );
    asm ( "csrr %0, pmpaddr9" : "=r"(pmpaddr[9]) );
    asm ( "csrr %0, pmpaddr10" : "=r"(pmpaddr[10]) );
    asm ( "csrr %0, pmpaddr11" : "=r"(pmpaddr[11]) );
    asm ( "csrr %0, pmpaddr12" : "=r"(pmpaddr[12]) );
    asm ( "csrr %0, pmpaddr13" : "=r"(pmpaddr[13]) );
    asm ( "csrr %0, pmpaddr14" : "=r"(pmpaddr[14]) );
    asm ( "csrr %0, pmpaddr15" : "=r"(pmpaddr[15]) );

	for (int i=0; i<16; i++){

		const uint8_t cfg = (i<8 ? pmpcfg0 : pmpcfg2) >> (8*(i%8));

		char rwx[3+1] = {cfg & PMP_R ? 'r':'-', cfg & PMP_W ? 'w':'-', cfg & PMP_X ? 'x':'-', '\0'};

		unsigned long start=0, end=0;

		char type[5+1]="";

		switch (cfg & (TOR | NA4 | NAPOT)){

		case TOR :
			start = pmpaddr[i-1]<<2;
			end =  (pmpaddr[i]<<2) -1;
			strcpy(type, "TOR");
			break;

		case NA4 :
			start = pmpaddr[i]<<2;
			end =  start+4 -1;
			strcpy(type, "NA4");
			break;

		case NAPOT:
			for (int j=0; j<__riscv_xlen; j++){
				if ( ((pmpaddr[i] >> j) & 1UL) == 0){
					const unsigned long size = 1UL << (3+j);
					start = (pmpaddr[i] >>j) <<(j+2);
					end = start + size -1;
					strcpy(type, "NAPOT");
					break;
				}
			}
		    break;

		default : continue;

		}

		mss_printf("0x%02x%08x 0x%02x%08x %s %s \n\r", (unsigned)(start>>32), (unsigned)start, (unsigned)(end>>32), (unsigned)end, rwx, type);

	}

}

// ------------------------------------------------------------------------
void msg_handler(void) {

    //CSRC(mie, 1 << 3);

    for (Zone zone = zone1; zone <= zone4; zone++) {

        char * const msg = (char *)inbox[zone-1];

        if (*msg != '\0') {

            if (strcmp("ping", msg) == 0) {
                MZONE_SEND(zone, (char[16]){"pong"});

            } else {
                mss_printf("\e7\e[2K");   // save curs pos & clear entire line
                mss_printf("\rZ%d > %.16s\n\r", zone, msg);
                mss_printf("\nH1 > ");
                mss_printf("%s", inputline);
                mss_printf("\e8\e[2B");   // restore curs pos & curs down 2x
            }

            *msg = '\0';

        }

    }

    //CSRS(mie, 1 << 3);

}

// ------------------------------------------------------------------------
void cmd_handler(void){

	char * tk[9]; tk[0] = strtok(inputline, " "); for (int i=1; i<9; i++) tk[i] = strtok(NULL, " ");

	if (tk[0] == NULL) tk[0] = "help";

	// --------------------------------------------------------------------
	if (strcmp(tk[0], "load")==0){
	// --------------------------------------------------------------------
		if (tk[1] != NULL){
			uint8_t data = 0x00;
			const uint64_t addr = strtoul(tk[1], NULL, 16);
			asm ("lbu %0, (%1)" : "+r"(data) : "r"(addr));
			mss_printf("0x%08x : 0x%02x \n\r", (unsigned int)addr, data);
		} else
		    mss_printf("Syntax: load address \n\r");

	// --------------------------------------------------------------------
	} else if (strcmp(tk[0], "store")==0){
	// --------------------------------------------------------------------
		if (tk[1] != NULL && tk[2] != NULL){
			const uint32_t data = (uint32_t)strtoul(tk[2], NULL, 16);
			const uint64_t addr = strtoul(tk[1], NULL, 16);

			if ( strlen(tk[2]) <=2 )
				asm ( "sb %0, (%1)" : : "r"(data), "r"(addr));
			else if ( strlen(tk[2]) <=4 )
				asm ( "sh %0, (%1)" : : "r"(data), "r"(addr));
			else
				asm ( "sw %0, (%1)" : : "r"(data), "r"(addr));

			mss_printf("0x%08x : 0x%02x \n\r", (unsigned int)addr, (unsigned int)data);
		} else
		    mss_printf("Syntax: store address data \n\r");

	// --------------------------------------------------------------------
	} else if (strcmp(tk[0], "exec")==0){
	// --------------------------------------------------------------------
		if (tk[1] != NULL){
			const uint64_t addr = strtoul(tk[1], NULL, 16);
			asm ( "jr (%0)" : : "r"(addr));
		} else
		    mss_printf("Syntax: exec address \n\r");

	// --------------------------------------------------------------------
	} else if (strcmp(tk[0], "send")==0){
	// --------------------------------------------------------------------
		if (tk[1] != NULL && tk[1][0]>='1' && tk[1][0]<='4' && tk[2] != NULL){
			char msg[16]; strncpy(msg, tk[2], (sizeof msg)-1);
			if (!MZONE_SEND( tk[1][0]-'0', msg) ){
				mss_printf("Error: Inbox full.\n\r");
			}
		} else mss_printf("Syntax: send {1|2|3|4} message \n\r");

	// --------------------------------------------------------------------
	} else if (strcmp(tk[0], "recv")==0){
	// --------------------------------------------------------------------
		if (tk[1] != NULL && tk[1][0]>='1' && tk[1][0]<='4'){
			char msg[16];
			if (MZONE_RECV(tk[1][0]-'0', msg)){
				mss_printf("msg : %.16s\n\r", msg);
			} else
				mss_printf("Error: Inbox empty.\n\r");
		} else
		    mss_printf("Syntax: recv {1|2|3|4} \n\r");

	// --------------------------------------------------------------------
	} else if (strcmp(tk[0], "pmp")==0) {
	    print_pmp();

	// --------------------------------------------------------------------
	} else mss_printf("Commands: load store exec send recv pmp \n\r");

}

// ------------------------------------------------------------------------
int readline(const char c) {
// ------------------------------------------------------------------------

	static size_t p=0;
	static int esc=0;
	static char history[8][sizeof(inputline)]={"","","","","","","",""}; static int h=-1;

	int eol = 0; // end of line
	
		if (c=='\e'){
			esc=1;

		} else if (esc==1 && c=='['){
			esc=2;

		} else if (esc==2 && c=='3'){
			esc=3;

		} else if (esc==3 && c=='~'){ // del key
			for (size_t i=p; i<strlen(inputline); i++) inputline[i]=inputline[i+1];
			mss_printf("\e7"); // save curs pos
			mss_printf("\e[K"); // clear line from curs pos
			mss_printf(&inputline[p]);
			mss_printf("\e8"); // restore curs pos
			esc=0;

		} else if (esc==2 && c=='C'){ // right arrow
			esc=0;
			if (p < strlen(inputline)){
				p++;
				mss_printf("\e[C");
			}

		} else if (esc==2 && c=='D'){ // left arrow
			esc=0;
			if (p>0){
				p--;
				mss_printf("\e[D");
			}

		} else if (esc==2 && c=='A'){ // up arrow (history)
			esc=0;
			if (h<8-1 && strlen(history[h+1])>0){
				h++;
				strcpy(inputline, history[h]);
				mss_printf("\e[2K"); // 2K clear entire line - cur pos dosn't change
				mss_printf("\rH1 > ");
				mss_printf(inputline);
				p=strlen(inputline);

			}

		} else if (esc==2 && c=='B'){ // down arrow (history)
			esc=0;
			if (h>0 && strlen(history[h-1])>0){
				h--;
				strcpy(inputline, history[h]);
				mss_printf("\e[2K"); // 2K clear entire line - cur pos dosn't change
				mss_printf("\rH1 > ");
				mss_printf(inputline);
				p=strlen(inputline);
			}

		} else if ((c=='\b' || c=='\x7f') && p>0 && esc==0){ // backspace
			p--;
			for (size_t i=p; i<strlen(inputline); i++) inputline[i]=inputline[i+1];
			mss_printf("\e[D");
			mss_printf("\e7");
			mss_printf("\e[K");
			mss_printf(&inputline[p]);
			mss_printf("\e8");

		} else if (c>=' ' && c<='~' && p < sizeof(inputline)-1 && esc==0){
			for (size_t i = sizeof(inputline)-1-1; i > p; i--) inputline[i]=inputline[i-1]; // make room for 1 ch
			inputline[p]=c;
			mss_printf("\e7"); // save curs pos
			mss_printf("\e[K"); // clear line from curs pos
			mss_printf(&inputline[p]); p++;
			mss_printf("\e8"); // restore curs pos
			mss_printf("\e[C"); // move curs right 1 pos

		} else if (c=='\r') {
			p=0; esc=0; eol = 1;
			// trim
			while (inputline[strlen(inputline)-1]==' ') inputline[strlen(inputline)-1]='\0';
			while (inputline[0]==' ') for (size_t i = 0; i < strlen(inputline); i++) inputline[i]=inputline[i+1];
			// save line to history
			if (strlen(inputline)>0 && strcmp(inputline, history[0])!=0){
				for (int i = 8-1; i > 0; i--) strcpy(history[i], history[i-1]);
				strcpy(history[0], inputline);
			} h = -1;
			mss_printf("\n\r");

		} else
			esc=0;

	return eol;

}

// ------------------------------------------------------------------------
int main (void) {

    MSS_UART_init(uart, MSS_UART_115200_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

	mss_printf("\e[2J\e[H"); // clear terminal screen

	mss_printf("=====================================================================\n\r");
	mss_printf("      	             Hex Five MultiZone® Security                    \n\r");
	mss_printf("    Copyright© 2020 Hex Five Security, Inc. - All Rights Reserved    \n\r");
	mss_printf("=====================================================================\n\r");
	mss_printf("This version of MultiZone® Security is meant for evaluation purposes \n\r");
	mss_printf("only. As such, use of this software is governed by the Evaluation    \n\r");
	mss_printf("License. There may be other functional limitations as described in   \n\r");
	mss_printf("the evaluation SDK documentation. The commercial version of the      \n\r");
	mss_printf("software does not have these restrictions.                           \n\r");
	mss_printf("=====================================================================\n\r");

	mss_printf("\n\rH1 > ");

	char c = '\0';

    while(1){

    	// UART RX event handler
		if (MSS_UART_get_rx(uart, (uint8_t *)&c, 1) && readline(c)){
			cmd_handler();
			mss_printf("\n\rH1 > ");
			inputline[0]='\0';
		}

		// MultiZone IPC message handler
		if (!inbox_empty()){
		    msg_handler();
		}

	}

}
