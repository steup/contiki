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

#ifndef TEMP_H_
#define TEMP_H_

#include <stdio.h>
#include <string.h>
#include "../core/sam3s4.h"
#include "../drivers/pio.h"
#include "../drivers/pmc.h"
#include "../drivers/adc.h"
#include "../services/sysclk.h"

/* Tracking Time*/
#define TRACKING_TIME    1
/* Transfer Period */
#define TRANSFER_PERIOD  1
/* Startup Time*/

#define STARTUP_TIME ADC_STARTUP_TIME_4

/** Size of the receive buffer and transmit buffer. */
#define BUFFER_SIZE     (100)

/* Reference voltage for ADC,in mv. */
#define VOLT_REF        (3300)

/* The maximal digital value */
#define MAX_DIGITAL     (4095)

#define ADC_CLOCK   6400000

/* PIO pins & flags */
#define PIN_TC0_TIOA0       (PIO_PA0_IDX)
#define PIN_TC0_TIOA0_FLAGS (PIO_PERIPH_B | PIO_DEFAULT)

#define PINS_ADC_TRIG  PIO_PA8_IDX
#define PINS_ADC_TRIG_FLAG  (PIO_PERIPH_B | PIO_DEFAULT)

//#define PIN_PWMC_PWMH0_TRIG   PIO_PC18_IDX
//#define PIN_PWMC_PWMH0_TRIG_FLAG   PIO_PERIPH_B | PIO_DEFAULT

int temp_init( void );
uint32_t get_raw( void );

#endif /* TEMP_H_ */
