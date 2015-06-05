/*
 *  Copyright (c) 2015, Otto-von-Guericke-Universität Magdeburg (OVGU)
 *
 *  Authors:	André Keuns		<andre.keuns@st.ovgu.de>
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

static struct uip_fw_netif meshif = { UIP_FW_NETIF(192,168,0,0, 255,255,0,0, uip_over_mesh_send) };

void HardFault_Handler( void )
{
	leds_arch_set(2);
	
	while (1);
}

void net_init()
{
	uip_ipaddr_t hostaddr, netmask;

	netstack_init();
	
	uip_init();
	uip_fw_init();
	
	process_start(&tcpip_process, NULL);
	process_start(&uip_fw_process, NULL);
		
	/* Construct ip address from four bytes. */
	uip_ipaddr(&hostaddr, 192, 168, linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1]);
	
	/* Construct netmask from four bytes. */
	uip_ipaddr(&netmask, 255, 255, 0, 0);
	
	uip_ipaddr_copy(&meshif.ipaddr, &hostaddr);
	
	/* Set the IP address for this host. */
	uip_sethostaddr(&hostaddr);
	
	/* Set the netmask for this host. */
	uip_setnetmask(&netmask);

	uip_over_mesh_set_net(&hostaddr, &netmask);
	
	/* Register slip interface with forwarding module. */
	//uip_fw_register(&slipif);
	//uip_over_mesh_set_gateway_netif(&slipif);

	/* Set slip interface to be a default forwarding interface . */
	uip_fw_default(&meshif);
	uip_over_mesh_init(UIP_OVER_MESH_CHANNEL);
	
	printf("IP: %d.%d.%d.%d\n", uip_ipaddr_to_quad(&hostaddr));
}

int main()
{
	cpu_init();

	print_cpu_info();
	
	uart_write_msg(CONTIKI_VERSION_STRING " started\r\n");
	
	clock_init();

	leds_init();
	
	NVIC_EnableIRQ(HardFault_IRQn);

	process_init();	
	
	process_start(&etimer_process, NULL);
	
	net_init();
	
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
//			if (c == 13 || (c >= 32 && c <=  122))
			{
				uart_write((Uart*)p_uart_stdio, c);
				c = c == 13 ? 0x0a : c;
				serial_line_input_byte((char)c);
				
				if (c == 0x0a)
				{
					printf("\r\n");
				}
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
