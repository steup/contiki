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

#ifndef CPU_H_
#define CPU_H_

#include "core/sam3s4.h"
#include "services/sysclk.h"
#include "services/uart_stdio.h"
#include "services/sys_tick.h"
#include "drivers/pio.h"
#include "drivers/pmc.h"
#include "drivers/uart.h"
#include "drivers/wdt.h"
#include "drivers/adc.h"
#include "drivers/spi.h"
#include "drivers/twi.h"
#include "utils/temp.h"

#define ID_ATSAM3S4A 0x28800960
#define ID_ATSAM3S4B 0x28900960
#define ID_ATSAM3S4C 0x28A00960

#ifndef STD_UART
	//#warning "STD_UART was not defined, fallback to UART0"
	#define STD_UART UART0
#endif

void cpu_init( void );
void print_cpu_info( void );

#endif /* CPU_H_ */