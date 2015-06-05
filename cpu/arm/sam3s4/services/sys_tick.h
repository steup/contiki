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

#ifndef SYS_TICK_H_
#define SYS_TICK_H_

#include "../core/sam3s4.h"

__attribute__((optimize("O0")))
__attribute__ ((section(".ramfunc")))
void my_delay_nop(uint32_t n);

// __Vendor_SysTickConfig must be 0, defined in sam3s4.h

volatile uint32_t sys_ticks;

uint32_t systick_init(uint32_t ticks);
void systick_enable( void );
void systick_disable( void );
void systick_reset( void );

#endif /* SYS_TICK_H_ */