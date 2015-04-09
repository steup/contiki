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

#include "uart_stdio.h"

volatile Uart *p_uart_stdio = UART0;

void uart_stdio_init(Uart *p_uart)
{
	p_uart_stdio = p_uart;
		
	sam_uart_opt_t uart_conf = 
		{
			.ul_baudrate = 9600,
			.ul_mode = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL,
			.ul_mck = sysclk_get_main_hz()
		};
	
	if (p_uart == UART0)
	{
		pio_configure_pin_group(PINS_UART0_PIO, PINS_UART0, PINS_UART0_FLAGS);
	}
	else
	{
		pio_configure_pin_group(PINS_UART1_PIO, PINS_UART1, PINS_UART1_FLAGS);
	}
	
	pmc_enable_periph_clk(uart_stdio_get_IRQ_type(p_uart));
	
	uart_init(p_uart, &uart_conf);
	
	uart_disable_interrupt(p_uart, ~UART_IER_RXRDY);
	
	uart_enable_interrupt(p_uart, UART_IER_RXRDY);
	
	//NVIC_EnableIRQ(uart_stdio_get_IRQ_type(p_uart));
}

void uart_write_msg(char *msg)
{
	size_t i;
				
	for (i = 0; i < strlen(msg); ++i)
	{
		while(uart_write(UART0, (uint8_t)msg[i]));
	}
}

void uart_write_msg_int(char *msg, int value)
{
	uart_write_msg(msg);
	
	char msg_int[255];
	
	memset(msg_int, 0, 255);
	
	itoa(value, msg_int, 10);
	
	uart_write_msg(msg_int);
	
	uart_write_msg("\r\n");
}

void printDebug( void )
{
	printf("%-18s: 0x%08lx\r\n", "CHIPID_CIDR", CHIPID->CHIPID_CIDR);
	
	printf("%-18s: 0x%08lx\r\n\n", "CHIPID_EXID", CHIPID->CHIPID_EXID);
	
	printf("\n%-18s: 0x%08lx\r\n\n", "addr_uart_stdout", (uint32_t)&p_uart_stdio);
	
	printf("%-18s: 0x%08lx\r\n", "ref - p_uart_stdio", (uint32_t)p_uart_stdio);
	printf("%-18s: 0x%08lx\r\n", "ref - UART0", (uint32_t)UART0);
	
	//printf("%-14s: 0x%08lx\r\n", "UART0_SR" , uart_get_status(p_uart_stdio));
	
	//printf("%-14s: 0x%08lx\r\n", "UART0_MR", p_uart_stdio->UART_MR);
}

void uart_stdio_enable( void )
{
	NVIC_EnableIRQ(uart_stdio_get_IRQ_type(UART0));
}

void uart_stdio_disable( void )
{
	NVIC_EnableIRQ(uart_stdio_get_IRQ_type(UART0));
}

void uart_stdio_clear_pending( void )
{
	NVIC_ClearPendingIRQ(uart_stdio_get_IRQ_type(UART0));
}

extern int _read(int file, char *ptr, int len)
{
	int i;
	
	if (file != 0 || (p_uart_stdio != UART0 && p_uart_stdio != UART1))
	{
		uart_write_msg("_read error invalid file or no stdin\r\n");
		uart_write_msg_int("_read: value file           : ", file);
		uart_write_msg_int("_read: value p_uart_stdin: ", (uint32_t)p_uart_stdio);
		return -1;
	}

	for (i = 0; i < len; ++i)
	{
		while (uart_read((Uart*)p_uart_stdio, (uint8_t*)&(ptr[i])));
		while (uart_write((Uart*)p_uart_stdio, (uint8_t)ptr[i]));
		
		if (ptr[i] == ' ')
		{
			ptr[i] = '_';
		}		
	}
		
	return len;
}

extern int _write(int file, char *ptr, int len)
{
	int i;
		
	if ( file != 1 && (p_uart_stdio != UART0 && p_uart_stdio != UART1))
	{
		uart_write_msg("_write error invalid file or no stdout\r\n");
		uart_write_msg_int("_write: value file           : ", file);
		uart_write_msg_int("_write: value p_uart_stdout: ", (uint32_t)p_uart_stdio);
		return -1;
	}

	for (i = 0; i < len; i++) 
	{
		uint8_t err_cnt = 255;
	
		while (--err_cnt > 0 && uart_write((Uart*)p_uart_stdio, (uint8_t)ptr[i]));
	
		if (err_cnt == 0)
		{
			uart_write_msg("_write error uart_write\r\n");
			return --i;
		}
	}
	
	return len;
}
