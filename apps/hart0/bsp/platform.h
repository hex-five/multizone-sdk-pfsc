/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#ifndef HEXFIVE_PLATFORM_H
#define HEXFIVE_PLATFORM_H


#define CPU_FREQ	 600062500 // 600MHz
#define RTC_FREQ	   1000000 //   1MHz
#define APB_AHB_FREQ 150012500 // 150MHz (UART)

#define PMP 16
#define IPC_RECV    0x01000A00 /* MultiZone TEE < AMP */
#define IPC_SEND    0x01000A80 /* MultiZone TEE > AMP */

// -----------------------------------------------------------------------------
// RTC (CLIC)
// -----------------------------------------------------------------------------
#define CLINT_BASE	0x02000000UL

#define CLINT_MSIP	    0x0000
#define CLINT_MTIMECMP  0x4000
#define CLINT_MTIME	    0xBFF8

// -----------------------------------------------------------------------------
// UART 0
// -----------------------------------------------------------------------------
#define UART0_BASE 0x20000000UL

#define UART_RBR 		  	0x00 // Receiver buffer register
#define UART_THR 			0x00 // Transmit holding register
#define UART_LCR 			0x0C // Line control register
#define UART_LCR_DLAB_MSK 	1<<7 // Divisor latch access bit
#define UART_LCR_EPS_MSK  	1<<4 // Even parity select 0: Odd parity (def) 1: Even parity
#define UART_LCR_PEN_MSK  	1<<3 // Parity enable 0: Disabled 1: Enabled
#define UART_LCR_STB_MSK  	1<<2 // Number of stop bits 0: 1 stop bit (def) 1: 11/2 stop bits ...
#define UART_LCR_WLS_MSK  	3<<0 // Word length 0b00: 5 bits (def) 0b01: 6 bits 0b10: 7 bits 0b11: 8 bits
#define UART_DLR			0x00 // Divisor latch (LSB)
#define UART_DRM			0x04 // Divisor latch (MSB)
#define UART_FCR 			0x08 // FIFO control register
#define UART_LSR 			0x14 // Line status register
#define UART_LSR_THRE_MSK	1<<5 // Transmitter holding register empty
#define UART_IER 			0x04 // Interrupt enable register
#define UART_IER_ERBFI_MSK	1<<0 // Enables received data available interrupt

#define UART_IRQ			27

// -----------------------------------------------------------------------------
// GPIO 2
// ------------------------------------------------------------------------------
#define GPIO_BASE 0x20122000UL

#define GPIO_CONFIG_0 	    0x00
#define GPIO_CONFIG_1 	    0x04
#define GPIO_CONFIG_2 	    0x08
#define GPIO_CONFIG_3 	    0x0C
#define GPIO_CONFIG_4 	    0x10
#define GPIO_CONFIG_5 	    0x14
#define GPIO_CONFIG_6 	    0x18
#define GPIO_CONFIG_7 	    0x1C
#define GPIO_CONFIG_8 	    0x20
#define GPIO_CONFIG_9 	    0x24
#define GPIO_CONFIG_10 	    0x28
#define GPIO_CONFIG_11 	    0x2C
#define GPIO_CONFIG_12 	    0x30
#define GPIO_CONFIG_13 	    0x34
#define GPIO_CONFIG_14 	    0x38
#define GPIO_CONFIG_15 	    0x3C
#define GPIO_CONFIG_16 	    0x40 // LED1
#define GPIO_CONFIG_17 	    0x44 // LED2
#define GPIO_CONFIG_18 	    0x48 // LED3
#define GPIO_CONFIG_19 	    0x4C // LED4
#define GPIO_CONFIG_20 	    0x50
#define GPIO_CONFIG_21 	    0x54
#define GPIO_CONFIG_22 	    0x58
#define GPIO_CONFIG_23 	    0x5C
#define GPIO_CONFIG_24 	    0x60
#define GPIO_CONFIG_25 	    0x64
#define GPIO_CONFIG_26 	    0x68
#define GPIO_CONFIG_27 	    0x6C
#define GPIO_CONFIG_28 	    0x70
#define GPIO_CONFIG_29 	    0x74
#define GPIO_CONFIG_30 	    0x78 // SW2
#define GPIO_CONFIG_31 	    0x7C // SW3
#define GPIO_INTR 		    0x80
#define GPIO_GPIN 		    0x84
#define GPIO_GPOUT 		    0x88
#define GPIO_CONFIG_ALL 	0x8C
#define GPIO_CONFIG_BYTE0 	0x90
#define GPIO_CONFIG_BYTE1 	0x94
#define GPIO_CONFIG_BYTE2 	0x98
#define GPIO_CONFIG_BYTE3 	0x9C
#define GPIO_CLEAR_BITS 	0xA0
#define GPIO_SET_BITS		0xA4

// -----------------------------------------------------------------------------
// LEDs & SWs (GPIO 2)
// ------------------------------------------------------------------------------
#define LED1 1<<16
#define LED2 1<<17
#define LED3 1<<18
#define LED4 1<<19

#define LED1_CFG GPIO_CONFIG_16
#define LED2_CFG GPIO_CONFIG_17
#define LED3_CFG GPIO_CONFIG_18
#define LED4_CFG GPIO_CONFIG_19

#define SW2 1<<30
#define SW3 1<<31

#define SW2_CFG GPIO_CONFIG_30
#define SW3_CFG GPIO_CONFIG_31

// -----------------------------------------------------------------------------
// PLIC (shared)
// ------------------------------------------------------------------------------
#define PLIC_BASE 	0x0C000000UL

#define PLIC_PRI_OFFSET 			0x0
#define PLIC_PRI_SHIFT_PER_SOURCE 	2
#define PLIC_EN_OFFSET				0x2000
#define PLIC_EN_SHIFT_PER_TARGET	1
#define PLIC_THRES_OFFSET			0x200000
#define PLIC_CLAIM_OFFSET			0x200004

#define PLIC_SOURCES 			187
#define PLIC_UART0_RX_SOURCE	 90
#define PLIC_SW1_SOURCE			118
#define PLIC_SW2_SOURCE			 43 // GPIO2.30
#define PLIC_SW3_SOURCE			 44 // GPIO2.31

// -----------------------------------------------------------------------------
// DMA (single channel mockup)
// ------------------------------------------------------------------------------
#define DMA_BASE 0x20009000UL

#define DMA_VER_OFF			0x00
#define DMA_CFG_OFF			0x10
#define DMA_CTRL_OFF		0x20
#define DMA_CH_STATUS_OFF	0x30 /* TC: 1<<ch+16, AB: 1<<ch+8,  ERR: 1<<ch+0 */
#define DMA_CH_ENABLE_OFF	0x34 /* 1<<ch */
#define DMA_CH_ABORT_OFF	0x40 /* 1<<ch */
#define DMA_CH_CTRL_OFF		0x44 /* +ch*0x14 */
#define DMA_TR_SRC_OFF		0x48 /* +ch*0x14 */
#define DMA_TR_DEST_OFF		0x4C /* +ch*0x14 */
#define DMA_TR_SIZE_OFF		0x50 /* +ch*0x14 */

#define DMA_IRQ 63 /* Mockup (U51 local 16+47 = H2_FABRIC_F2H_31_U54_INT) */


// -----------------------------------------------------------------------------
// C Helper functions
// -----------------------------------------------------------------------------

#define _REG64(base, offset) (*(volatile uint64_t *)((base) + (offset)))
#define _REG32(base, offset) (*(volatile uint32_t *)((base) + (offset)))

#define CLINT_REG(offset) _REG64(CLINT_BASE, offset)
#define GPIO_REG(offset)  _REG32(GPIO_BASE, offset)
#define PWM_REG(offset)   _REG32(PWM_BASE, offset)
#define UART_REG(offset)  _REG32(UART0_BASE, offset)
#define PLIC_REG(offset)  _REG32(PLIC_BASE, offset)
#define DMA_REG(offset)   _REG32(DMA_BASE, offset)


#endif /* HEXFIVE_PLATFORM_H */
