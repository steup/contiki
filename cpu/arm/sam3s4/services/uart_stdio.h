/*
 *  Copyright (c) 2015, Otto-von-Guericke-Universität Magdeburg (OVGU)
 *
 *  Authors:	Andé Keuns		<andre.keuns@st.ovgu.de>
 *				Marcus Viererbe	<marcus.viererbe@st.ovgu.de>
 *
 *  All rights reserved
 *
 *  This script free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  The GNU General Public License can be found at
 *  http://www.gnu.org/copyleft/gpl.html.
 *
 *  This script is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  This copyright notice MUST APPEAR in all copies of the script!
 *  Thanks for all inspirations.
 *
 */

#ifndef UART_STDIO_H_
#define UART_STDIO_H_

#include <sam3s4.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sysclk.h"

#include <drv_uart.h>
#include <drv_pio.h>
#include <drv_pmc.h>

#define PINS_UART0_PIO		PIOA
#define PINS_UART0			(PIO_PA9A_URXD0 | PIO_PA10A_UTXD0)
#define PINS_UART0_FLAGS	(PIO_PERIPH_A | PIO_DEFAULT)

#define PINS_UART1_PIO		PIOB
#define PINS_UART1			(PIO_PB2A_URXD1 | PIO_PB3A_UTXD1)
#define PINS_UART1_FLAGS	(PIO_PERIPH_B | PIO_DEFAULT)

//extern volatile uint32_t uart_stdout;
extern volatile Uart *p_uart_stdio;

// [uart_std functions]
static inline IRQn_Type uart_stdio_get_IRQ_type(Uart *p_uart)
{
	return p_uart == UART0 ? UART0_IRQn : UART1_IRQn;
}

void uart_stdio_init(Uart *p_uart);
void uart_write_msg(char *msg);
void uart_write_msg_int(char *msg, int value);
void printDebug( void );
void uart_stdio_enable( void );
void uart_stdio_disable( void );
void uart_stdio_clear_pending( void );

#endif /* UART_STDIO_H_ */
