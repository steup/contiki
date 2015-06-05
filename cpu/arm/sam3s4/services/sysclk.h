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

#ifndef SYS_CLOCK_H_
#define SYS_CLOCK_H_

#include "../core/sam3s4.h"
#include "../drivers/pmc.h"

static inline uint32_t sysclk_get_main_hz( void )
{
	// CSS: Master Clock Source Selection
	uint8_t mcss = PMC->PMC_MCKR & 0x3;
	uint8_t freqSel;
	
	switch (mcss)
	{
		case PMC_MCKR_CSS_SLOW_CLK:
		
			// CHIP_FREQ_SLCK_RC_MIN < CHIP_FREQ_SLCK_RC < CHIP_FREQ_SLCK_RC_MAX
			return CHIP_FREQ_SLCK_RC;
			
		case PMC_MCKR_CSS_MAIN_CLK:
		
			freqSel = PMC->CKGR_MOR & (7 << CKGR_MOR_MOSCRCF_Pos);
			
			if (freqSel == CKGR_MOR_MOSCRCF_4_MHz)
			{
				return CHIP_FREQ_MAINCK_RC_4MHZ;
			}
			
			if (freqSel == CKGR_MOR_MOSCRCF_8_MHz)
			{
				return CHIP_FREQ_MAINCK_RC_8MHZ;
			}
			
			//CKGR_MOR_MOSCRCF_12_MHz
			return CHIP_FREQ_MAINCK_RC_12MHZ;
		
		case PMC_MCKR_CSS_PLLA_CLK:
		case PMC_MCKR_CSS_PLLB_CLK:

			/*#define CHIP_FREQ_CPU_MAX               (64000000UL)
			#define CHIP_FREQ_XTAL_32K              (32768UL)
			#define CHIP_FREQ_XTAL_12M              (12000000UL)*/
		
		default:
			return CHIP_FREQ_CPU_MAX;
	}
}

static inline uint32_t sysclk_get_cpu_hz( void )
{
	uint8_t pres = PMC->PMC_MCKR & (7 << PMC_MCKR_PRES_Pos);
	
	switch (pres)
	{
		case PMC_MCKR_PRES_CLK_1: // 0
			return sysclk_get_main_hz();
				
		case PMC_MCKR_PRES_CLK_3:
			return sysclk_get_main_hz() / 3;
			
		default:
			return sysclk_get_main_hz() / (1 << (pres >> PMC_MCKR_PRES_Pos));
	}
}

static inline uint32_t sysclk_get_peripheral_hz( void )
{
	return sysclk_get_cpu_hz();
}

/*
 * enable peripheral clock (clk_id)
 * clk_id => id number of the peripheral clock
 */
static inline void sysclk_enable_peripheral_clock(uint32_t clk_id)
{
	pmc_enable_periph_clk(clk_id);
}

/*
 * disable peripheral clock (clk_id)
 * clk_id => id number of the peripheral clock
 */
static inline void sysclk_disable_peripheral_clock(uint32_t clk_id)
{
	pmc_disable_periph_clk(clk_id);
}

// sysclk_init replace the SystemInit() function from atsam3s4_system.c
void sysclk_init( void );

#endif /* SYS_CLOCK_H_ */
