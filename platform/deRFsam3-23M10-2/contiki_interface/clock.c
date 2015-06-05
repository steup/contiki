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

#include <stdio.h>
#include <sys/clock.h>
#include <sys/etimer.h>
#include <services/sys_tick.h>
#include <services/sysclk.h>

volatile unsigned long current_seconds = 1;
unsigned int second_countdown = CLOCK_SECOND;

void SysTick_Handler( void )
{
	++sys_ticks;
	
	if(etimer_pending()) 
	{
		etimer_request_poll();
	}

	if(--second_countdown == 0)
	{
		++current_seconds;
		second_countdown = CLOCK_SECOND;
	}
}

void clock_init()
{
	current_seconds = 0;
	
	if (systick_init(sysclk_get_main_hz() / (8 * CLOCK_SECOND)))
	{
		printf("clock_init: systick_init failed\r\n");
	}
	
}

CCIF clock_time_t clock_time(void)
{
	return (clock_time_t)sys_ticks;
}

void clock_delay(unsigned int t)
{
	my_delay_nop( t );
}

unsigned long clock_seconds(void)
{
	return current_seconds;
}
