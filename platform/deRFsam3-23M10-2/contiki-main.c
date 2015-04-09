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

#include "contiki-main.h"

void HardFault_Handler( void )
{
	leds_arch_set(2);
}

int main()
{
	cpu_init();

	uart_write_msg(CONTIKI_VERSION_STRING " started\r\n");
	
	clock_init();	

	leds_init();
	
	NVIC_EnableIRQ(HardFault_IRQn);

	process_init();
	
	process_start(&etimer_process, NULL);
	
	ctimer_init();
	serial_line_init();
	
	autostart_start(autostart_processes);
	
	while (1)
	{
		uint8_t c;
		
		process_run();	
		
		if (!uart_read((Uart*)p_uart_stdio, &c))
		{
			unsigned char led_mask = leds_arch_get();
			
			led_mask |= 1;
			
			/* ohne sonderzeichen */
			if ((c >= 48 && c <=  57) 
			  ||(c >= 65 && c <=  90)
			  ||(c >= 97 && c <= 122)
			  ||(c == 13 || c ==  32))
			{
				uart_write((Uart*)p_uart_stdio, c);
				c = c == 13 ? 0x0a : c;
				serial_line_input_byte((char)c);
			}
			
			leds_arch_set(led_mask);
		}
		else
		{
			unsigned char led_mask = leds_arch_get();
			
			led_mask &= 0xFE;
			
			leds_arch_set(led_mask);
		}

		etimer_request_poll();
	}
	
	return 0;
}
