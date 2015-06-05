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

#include "leds-arch.h"

void leds_arch_init( void )
{
	const uint32_t _LOW = 0;
	//const uint32_t _HIGH = 1;
	const uint32_t _DISABLE = 0;
	const uint32_t _ENABLE = 1;
	
	//printf("leds_arch_init:\r\n");
	
	pio_set_output(PIOB, (PIO_PER_P0 | PIO_PER_P1), _LOW, _DISABLE, _ENABLE);
	
	pio_set_output(PIOA, (PIO_PER_P18 | PIO_PER_P19), _LOW, _DISABLE, _ENABLE);
	
	// set leds off
	leds_arch_set(0);
}

unsigned char leds_arch_get(void)
{
	//printf("leds_arch_get:\r\n");
	
	uint8_t mask = 0;
	
	if (!pio_get(ARCH_LED_0_PIO, PIO_OUTPUT_0, ARCH_LED_0))
	{
		mask |= 1;
	}
	
	if (!pio_get(ARCH_LED_1_PIO, PIO_OUTPUT_0, ARCH_LED_1))
	{
		mask |= 2;
	}
	
	if (!pio_get(ARCH_LED_2_PIO, PIO_OUTPUT_0, ARCH_LED_2))
	{
		mask |= 4;
	}
	
	return mask;
}

void leds_arch_set(unsigned char l)
{
	//printf("leds_arch_set: %u\r\n", l);
	
	if (l & 1)
	{
		pio_clear(ARCH_LED_0_PIO, ARCH_LED_0);
	}
	else
	{
		pio_set(ARCH_LED_0_PIO, ARCH_LED_0);
	}
	
	if (l & 2)
	{
		pio_clear(ARCH_LED_1_PIO, ARCH_LED_1);
	}
	else
	{
		pio_set(ARCH_LED_1_PIO, ARCH_LED_1);
	}
	
	if (l & 4)
	{
		pio_clear(ARCH_LED_2_PIO, ARCH_LED_2);
	}
	else
	{
		pio_set(ARCH_LED_2_PIO, ARCH_LED_2);
	}
}
