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

#include "temp.h"

/* 
 * Test if ADC channel interrupt Status is Set
 * p_adc - pointer to an ADC instance
 * adc_ch - channel to be tested
 * return 1 if interrupt status is set, otherwise 0
 */
uint8_t adc_is_channel_interrupt_status_set(Adc *p_adc, const enum adc_channel_num_t adc_ch)
{
	return (adc_get_status(p_adc) & (1 << adc_ch)) == (1 << adc_ch);
}

int temp_init( void )
{
	//uint32_t mck = sysclk_get_cpu_hz();
	
	//printf("sysclk_get_cpu_hz = %lu Hz\r\n", mck);
	
	/* TC TIOA configuration */
	pio_configure_pin(PIN_TC0_TIOA0, PIN_TC0_TIOA0_FLAGS);

	/* ADC Trigger configuration */
	pio_configure_pin(PINS_ADC_TRIG, PINS_ADC_TRIG_FLAG);

	/* PWMH0 configuration */
	//pio_configure_pin(PIN_PWMC_PWMH0_TRIG, PIN_PWMC_PWMH0_TRIG_FLAG);
	
	/* Enable peripheral clock. */
	pmc_enable_periph_clk(ID_ADC);
	
	/* Initialize ADC. */
	/*
	 * Formula: ADCClock = MCK / ( (PRESCAL+1) * 2 )
	 * For example, MCK = 64MHZ, PRESCAL = 4, then:
	 * ADCClock = 64 / ((4+1) * 2) = 6.4MHz;
	 */
	/* Formula:
	 *     Startup  Time = startup value / ADCClock
	 *     Startup time = 64 / 6.4MHz = 10 us
	 */
	adc_init(ADC, sysclk_get_cpu_hz(), 1000000, STARTUP_TIME);

	adc_check(ADC, sysclk_get_cpu_hz());

	get_raw();
	
	return 0;
}

uint32_t get_raw( void )
{
	uint32_t adc_out;
	
	/* Enable channel for potentiometer. */
	adc_enable_channel(ADC, ADC_TEMPERATURE_SENSOR);

	/* Enable the temperature sensor. */
	adc_enable_ts(ADC);

	/* Start conversion. */
	adc_start(ADC);
	
	printf("wait for data\r\n");
	
	// wait for conversion termination
	while (!adc_is_channel_interrupt_status_set(ADC, ADC_TEMPERATURE_SENSOR));
	
	// perform measurement
	adc_out = adc_get_channel_value(ADC, ADC_TEMPERATURE_SENSOR);
	
	printf("adc_out: %lu\r\n", adc_out);
	
	/* Stop conversion. */
	adc_stop(ADC);
	
	/* deactivate the ADC for power saving */
	adc_disable_channel(ADC, ADC_TEMPERATURE_SENSOR);
	
	return adc_out;
}
