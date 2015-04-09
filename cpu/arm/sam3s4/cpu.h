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

#ifndef CPU_H_
#define CPU_H_

#include <sam3s4.h>
#include <drv_pio.h>
#include <drv_pmc.h>
#include <drv_uart.h>
#include <drv_wdt.h>
#include <sysclk.h>
#include <uart_stdio.h>
#include <sys_tick.h>

#ifndef STD_UART
	//#warning "STD_UART was not defined, fallback to UART0"
	#define STD_UART UART0
#endif

void cpu_init( void );

#endif /* CPU_H_ */