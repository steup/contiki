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

#include "sys_tick.h"

void my_delay_nop(uint32_t n)
{
	__asm (
		"loop: DMB	\n"
		"SUBS R0, R0, #1  \n"
		"BNE.N loop         "
	);
}

volatile uint32_t sys_ticks = 1;
/*
void SysTick_Handler( void )
{
	sys_ticks += SysTick->VAL;
}
*/
uint32_t systick_init(uint32_t ticks)
{
	if (SysTick_Config(ticks))
	{
		return 1;
	}

	sys_ticks = 0;
	
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	NVIC_EnableIRQ(SysTick_IRQn);
		
	return 0;
}

void systick_enable( void )
{
	NVIC_EnableIRQ(SysTick_IRQn);
}

void systick_disable( void )
{
	NVIC_DisableIRQ(SysTick_IRQn);
}

void systick_reset( void )
{
	if (sys_ticks > 0)
	{
		NVIC_DisableIRQ(SysTick_IRQn);
		
		sys_ticks = 0;
		
		SysTick->VAL = 0;
		
		NVIC_EnableIRQ(SysTick_IRQn);
	}
}
