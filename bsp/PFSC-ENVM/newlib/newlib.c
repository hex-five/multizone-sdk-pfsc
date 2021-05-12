/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "../platform.h"

// ----------------------------------------------------------------------------
int _close(int file) {
// ----------------------------------------------------------------------------

	return -1;
}

// ----------------------------------------------------------------------------
int _fstat(int file, struct stat *st) {
// ----------------------------------------------------------------------------

	st->st_mode = S_IFCHR;
	return 0;
}

// ----------------------------------------------------------------------------
void * _sbrk(int incr) {
// ----------------------------------------------------------------------------

	extern char _end[];
	extern char _heap_end[];
	static char *_heap_ptr = _end;

	if ((_heap_ptr + incr < _end) || (_heap_ptr + incr > _heap_end))
		return  (void *) -1;

	_heap_ptr += incr;
	return _heap_ptr - incr;
}

// ----------------------------------------------------------------------------
int _isatty(int file) {
// ----------------------------------------------------------------------------

	return (file == STDIN_FILENO || file == STDOUT_FILENO || file == STDERR_FILENO) ? 1 : 0;

}

// ----------------------------------------------------------------------------
int _lseek(int file, off_t ptr, int dir) {
// ----------------------------------------------------------------------------

	return 0;
}

// ----------------------------------------------------------------------------
int _open(const char* name, int flags, int mode) {
// ----------------------------------------------------------------------------

	if (strcmp(name, "UART")==0){

		UART_REG(UART_LCR) = UART_LCR_DLAB_MSK;  // Enable div latch access
		UART_REG(UART_DLR) = APB_AHB_FREQ / (115200*16);  // PCLK_Frequency / (baud_rate * 16)
		UART_REG(UART_LCR) = UART_LCR_WLS_MSK;   // Disable div latch access & set Word length=8bit

		UART_REG(UART_FCR) = 0b00000111; // Clear TX/RX FIFO

		UART_REG(UART_IER) = UART_IER_ERBFI_MSK; // Enables received data available interrupt

		return 0;

	}

	return -1;
}

// ----------------------------------------------------------------------------
int _read(int file, char *ptr, size_t len) {
// ----------------------------------------------------------------------------

	if (isatty(file)) {

		ssize_t count = 0;

		while( count<len && ( UART_REG(UART_LSR) & 1<<0 ) ){ // DR Data Ready
			*ptr++ = (char)UART_REG(UART_RBR);
			count++;
		}

		return count;
	}

	return -1;
}

// ----------------------------------------------------------------------------
size_t _write(int file, const void *ptr, size_t len) {
// ----------------------------------------------------------------------------

	if (isatty(file)) {

		const uint8_t * buff = (uint8_t *)ptr;

		for (size_t i = 0; i < len; i++) {

			while ( (UART_REG(UART_LSR) & UART_LSR_THRE_MSK) == 0){;} // THRE Transmitter register empty

			UART_REG(UART_THR) = buff[i];

			if (buff[i] == '\n') {
				while ( (UART_REG(UART_LSR) & UART_LSR_THRE_MSK) == 0){;} // THRE
				UART_REG(UART_THR) = '\r';
			}


		}

		return len;

	}

	return -1;
}

// open("UART", 0, 0); write(1, "Ok >\n", 5); char c='\0'; while(1) if ( read(0, &c, 1) >0 ) write(1, &c, 1);
