/**
 * \file
 *
 * \brief Power Management Controller (PMC) driver for SAM.
 *
 * Copyright (c) 2011 - 2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
 /**
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "pmc.h"

#define MAX_PERIPH_ID 34

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 * \defgroup sam_drivers_pmc_group Power Management Controller (PMC)
 *
 * \par Purpose
 *
 * The Power Management Controller (PMC) optimizes power consumption by
 * controlling all system and user peripheral clocks. The PMC enables/disables
 * the clock inputs to many of the peripherals and the Cortex-M Processor.
 *
 * @{
 */

/**
 * \brief Set the prescaler of the MCK.
 *
 * \param ul_pres Prescaler value.
 */
void pmc_mck_set_prescaler(uint32_t ul_pres)
{
	PMC->PMC_MCKR =
			(PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}

/**
 * \brief Set the source of the MCK.
 *
 * \param ul_source Source selection value.
 */
void pmc_mck_set_source(uint32_t ul_source)
{
	PMC->PMC_MCKR =
			(PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) | ul_source;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}

/**
 * \brief Switch master clock source selection to slow clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_sclk(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) |
			PMC_MCKR_CSS_SLOW_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch master clock source selection to main clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_mainck(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) |
			PMC_MCKR_CSS_MAIN_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch master clock source selection to PLLA clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_pllack(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) |
			PMC_MCKR_CSS_PLLA_CLK;

	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch master clock source selection to PLLB clock.
 *
 * \param ul_pres Processor clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_mck_to_pllbck(uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | ul_pres;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) |
			PMC_MCKR_CSS_PLLB_CLK;
	for (ul_timeout = PMC_TIMEOUT; !(PMC->PMC_SR & PMC_SR_MCKRDY);
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch slow clock source selection to external 32k (Xtal or Bypass).
 *
 * \note Switching SCLK back to 32krc is only possible by shutting down the
 *       VDDIO power supply.
 *
 * \param ul_bypass 0 for Xtal, 1 for bypass.
 */
void pmc_switch_sclk_to_32kxtal(uint32_t ul_bypass)
{
	/* Set Bypass mode if required */
	if (ul_bypass == 1) {
		SUPC->SUPC_MR |= SUPC_MR_KEY_PASSWD |
			SUPC_MR_OSCBYPASS;
	}

	SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_XTALSEL;
}

/**
 * \brief Check if the external 32k Xtal is ready.
 *
 * \retval 1 External 32k Xtal is ready.
 * \retval 0 External 32k Xtal is not ready.
 */
uint32_t pmc_osc_is_ready_32kxtal(void)
{
	return ((SUPC->SUPC_SR & SUPC_SR_OSCSEL)
			&& (PMC->PMC_SR & PMC_SR_OSCSELS));
}

/**
 * \brief Switch main clock source selection to internal fast RC.
 *
 * \param ul_moscrcf Fast RC oscillator(4/8/12Mhz).
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 * \retval 2 Invalid frequency.
 */
void pmc_switch_mainck_to_fastrc(uint32_t ul_moscrcf)
{
	/* Enable Fast RC oscillator but DO NOT switch to RC now */
	PMC->CKGR_MOR |= (CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN);

	/* Wait the Fast RC to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

	/* Change Fast RC oscillator frequency */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCF_Msk) |
			CKGR_MOR_KEY_PASSWD | ul_moscrcf;

	/* Wait the Fast RC to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

	/* Switch to Fast RC */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCSEL) |
			CKGR_MOR_KEY_PASSWD;
}

/**
 * \brief Enable fast RC oscillator.
 *
 * \param ul_rc Fast RC oscillator(4/8/12Mhz).
 */
void pmc_osc_enable_fastrc(uint32_t ul_rc)
{
	/* Enable Fast RC oscillator but DO NOT switch to RC */
	PMC->CKGR_MOR |= (CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN);
	/* Wait the Fast RC to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));

	/* Change Fast RC oscillator frequency */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCF_Msk) |
			CKGR_MOR_KEY_PASSWD | ul_rc;
	/* Wait the Fast RC to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));
}

/**
 * \brief Disable the internal fast RC.
 */
void pmc_osc_disable_fastrc(void)
{
	/* Disable Fast RC oscillator */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCEN &
					~CKGR_MOR_MOSCRCF_Msk)
				| CKGR_MOR_KEY_PASSWD;
}

/**
 * \brief Check if the main fastrc is ready.
 *
 * \retval 0 Xtal is not ready, otherwise ready.
 */
uint32_t pmc_osc_is_ready_fastrc(void)
{
	return (PMC->PMC_SR & PMC_SR_MOSCRCS);
}

/**
 * \brief Enable main XTAL oscillator.
 *
 * \param ul_xtal_startup_time Xtal start-up time, in number of slow clocks.
 */
void pmc_osc_enable_main_xtal(uint32_t ul_xtal_startup_time)
{
	uint32_t mor = PMC->CKGR_MOR;
	mor &= ~(CKGR_MOR_MOSCXTBY|CKGR_MOR_MOSCXTEN);
	mor |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN |
			CKGR_MOR_MOSCXTST(ul_xtal_startup_time);
	PMC->CKGR_MOR = mor;
	/* Wait the main Xtal to stabilize */
	while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));
}

/**
 * \brief Bypass main XTAL.
 */
void pmc_osc_bypass_main_xtal(void)
{
	uint32_t mor = PMC->CKGR_MOR;
	mor &= ~(CKGR_MOR_MOSCXTBY|CKGR_MOR_MOSCXTEN);
	mor |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTBY;
	/* Enable Crystal oscillator but DO NOT switch now. Keep MOSCSEL to 0 */
	PMC->CKGR_MOR = mor;
	/* The MOSCXTS in PMC_SR is automatically set */
}

/**
 * \brief Disable the main Xtal.
 */
void pmc_osc_disable_main_xtal(void)
{
	uint32_t mor = PMC->CKGR_MOR;
	mor &= ~(CKGR_MOR_MOSCXTBY|CKGR_MOR_MOSCXTEN);
	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | mor;
}

/**
 * \brief Check if the main crystal is bypassed.
 *
 * \retval 0 Xtal is bypassed, otherwise not.
 */
uint32_t pmc_osc_is_bypassed_main_xtal(void)
{
	return (PMC->CKGR_MOR & CKGR_MOR_MOSCXTBY);
}

/**
 * \brief Check if the main crystal is ready.
 *
 * \note If main crystal is bypassed, it's always ready.
 *
 * \retval 0 main crystal is not ready, otherwise ready.
 */
uint32_t pmc_osc_is_ready_main_xtal(void)
{
	return (PMC->PMC_SR & PMC_SR_MOSCXTS);
}

/**
 * \brief Switch main clock source selection to external Xtal/Bypass.
 *
 * \note The function may switch MCK to SCLK if MCK source is MAINCK to avoid
 *       any system crash.
 *
 * \note If used in Xtal mode, the Xtal is automatically enabled.
 *
 * \param ul_bypass 0 for Xtal, 1 for bypass.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
void pmc_switch_mainck_to_xtal(uint32_t ul_bypass,
		uint32_t ul_xtal_startup_time)
{
	/* Enable Main Xtal oscillator */
	if (ul_bypass) {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTEN) |
				CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTBY |
				CKGR_MOR_MOSCSEL;
	} else {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTBY) |
				CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN |
				CKGR_MOR_MOSCXTST(ul_xtal_startup_time);
		/* Wait the Xtal to stabilize */
		while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));

		PMC->CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL;
	}
}

/**
 * \brief Disable the external Xtal.
 *
 * \param ul_bypass 0 for Xtal, 1 for bypass.
 */
void pmc_osc_disable_xtal(uint32_t ul_bypass)
{
	/* Disable xtal oscillator */
	if (ul_bypass) {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTBY) |
				CKGR_MOR_KEY_PASSWD;
	} else {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCXTEN) |
				CKGR_MOR_KEY_PASSWD;
	}
}

/**
 * \brief Check if the MAINCK is ready. Depending on MOSCEL, MAINCK can be one
 * of Xtal, bypass or internal RC.
 *
 * \retval 1 Xtal is ready.
 * \retval 0 Xtal is not ready.
 */
uint32_t pmc_osc_is_ready_mainck(void)
{
	return PMC->PMC_SR & PMC_SR_MOSCSELS;
}

/**
 * \brief Select Main Crystal or internal RC as main clock source.
 *
 * \note This function will not enable/disable RC or Main Crystal.
 *
 * \param ul_xtal_rc 0 internal RC is selected, otherwise Main Crystal.
 */
void pmc_mainck_osc_select(uint32_t ul_xtal_rc)
{
	uint32_t mor = PMC->CKGR_MOR;
	if (ul_xtal_rc) {
		mor |=  CKGR_MOR_MOSCSEL;
	} else {
		mor &= ~CKGR_MOR_MOSCSEL;
	}
	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | mor;
}

/**
 * \brief Enable PLLA clock.
 *
 * \param mula PLLA multiplier.
 * \param pllacount PLLA counter.
 * \param diva Divider.
 */
void pmc_enable_pllack(uint32_t mula, uint32_t pllacount, uint32_t diva)
{
	/* first disable the PLL to unlock the lock */
	pmc_disable_pllack();

	PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_DIVA(diva) |
			CKGR_PLLAR_PLLACOUNT(pllacount) | CKGR_PLLAR_MULA(mula);

	while ((PMC->PMC_SR & PMC_SR_LOCKA) == 0);
}

/**
 * \brief Disable PLLA clock.
 */
void pmc_disable_pllack(void)
{
	PMC->CKGR_PLLAR = CKGR_PLLAR_MULA(0);

	PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0);
}

/**
 * \brief Is PLLA locked?
 *
 * \retval 0 Not locked.
 * \retval 1 Locked.
 */
uint32_t pmc_is_locked_pllack(void)
{
	return (PMC->PMC_SR & PMC_SR_LOCKA);
}

/**
 * \brief Enable PLLB clock.
 *
 * \param mulb PLLB multiplier.
 * \param pllbcount PLLB counter.
 * \param divb Divider.
 */
void pmc_enable_pllbck(uint32_t mulb, uint32_t pllbcount, uint32_t divb)
{
	/* first disable the PLL to unlock the lock */
	pmc_disable_pllbck();

	PMC->CKGR_PLLBR =
			CKGR_PLLBR_DIVB(divb) | CKGR_PLLBR_PLLBCOUNT(pllbcount)
			| CKGR_PLLBR_MULB(mulb);
	while ((PMC->PMC_SR & PMC_SR_LOCKB) == 0);
}

/**
 * \brief Disable PLLB clock.
 */
void pmc_disable_pllbck(void)
{
	PMC->CKGR_PLLBR = CKGR_PLLBR_MULB(0);
}

/**
 * \brief Is PLLB locked?
 *
 * \retval 0 Not locked.
 * \retval 1 Locked.
 */
uint32_t pmc_is_locked_pllbck(void)
{
	return (PMC->PMC_SR & PMC_SR_LOCKB);
}

/**
 * \brief Enable the specified peripheral clock.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_enable_periph_clk(uint32_t ul_id)
{
	if (ul_id > MAX_PERIPH_ID) {
		return 1;
	}

	if (ul_id < 32) {
		if ((PMC->PMC_PCSR0 & (1u << ul_id)) != (1u << ul_id)) {
			PMC->PMC_PCER0 = 1 << ul_id;
		}
	} else {
		ul_id -= 32;
		if ((PMC->PMC_PCSR1 & (1u << ul_id)) != (1u << ul_id)) {
			PMC->PMC_PCER1 = 1 << ul_id;
		}
	}

	return 0;
}

/**
 * \brief Disable the specified peripheral clock.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Success.
 * \retval 1 Invalid parameter.
 */
uint32_t pmc_disable_periph_clk(uint32_t ul_id)
{
	if (ul_id > MAX_PERIPH_ID) {
		return 1;
	}

	if (ul_id < 32) {
		if ((PMC->PMC_PCSR0 & (1u << ul_id)) == (1u << ul_id)) {
			PMC->PMC_PCDR0 = 1 << ul_id;
		}
	} else {
		ul_id -= 32;
		if ((PMC->PMC_PCSR1 & (1u << ul_id)) == (1u << ul_id)) {
			PMC->PMC_PCDR1 = 1 << ul_id;
		}
	}
	return 0;
}

/**
 * \brief Enable all peripheral clocks.
 */
void pmc_enable_all_periph_clk(void)
{
	PMC->PMC_PCER0 = PMC_MASK_STATUS0;
	while ((PMC->PMC_PCSR0 & PMC_MASK_STATUS0) != PMC_MASK_STATUS0);

	PMC->PMC_PCER1 = PMC_MASK_STATUS1;
	while ((PMC->PMC_PCSR1 & PMC_MASK_STATUS1) != PMC_MASK_STATUS1);
}

/**
 * \brief Disable all peripheral clocks.
 */
void pmc_disable_all_periph_clk(void)
{
	PMC->PMC_PCDR0 = PMC_MASK_STATUS0;
	while ((PMC->PMC_PCSR0 & PMC_MASK_STATUS0) != 0);

	PMC->PMC_PCDR1 = PMC_MASK_STATUS1;
	while ((PMC->PMC_PCSR1 & PMC_MASK_STATUS1) != 0);
}

/**
 * \brief Check if the specified peripheral clock is enabled.
 *
 * \note The ID must NOT be shifted (i.e., 1 << ID_xxx).
 *
 * \param ul_id Peripheral ID (ID_xxx).
 *
 * \retval 0 Peripheral clock is disabled or unknown.
 * \retval 1 Peripheral clock is enabled.
 */
uint32_t pmc_is_periph_clk_enabled(uint32_t ul_id)
{
	if (ul_id > MAX_PERIPH_ID) {
		return 0;
	}

	if (ul_id < 32) {
		if ((PMC->PMC_PCSR0 & (1u << ul_id))) {
			return 1;
		} else {
			return 0;
		}
	} else {
		ul_id -= 32;
		if ((PMC->PMC_PCSR1 & (1u << ul_id))) {
			return 1;
		} else {
			return 0;
		}
	}
}

/**
 * \brief Set the prescaler for the specified programmable clock.
 *
 * \param ul_id Peripheral ID.
 * \param ul_pres Prescaler value.
 */
void pmc_pck_set_prescaler(uint32_t ul_id, uint32_t ul_pres)
{
	PMC->PMC_PCK[ul_id] =
			(PMC->PMC_PCK[ul_id] & ~PMC_PCK_PRES_Msk) | ul_pres;
	while ((PMC->PMC_SCER & (PMC_SCER_PCK0 << ul_id))
			&& !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)));
}

/**
 * \brief Set the source oscillator for the specified programmable clock.
 *
 * \param ul_id Peripheral ID.
 * \param ul_source Source selection value.
 */
void pmc_pck_set_source(uint32_t ul_id, uint32_t ul_source)
{
	PMC->PMC_PCK[ul_id] =
			(PMC->PMC_PCK[ul_id] & ~PMC_PCK_CSS_Msk) | ul_source;
	while ((PMC->PMC_SCER & (PMC_SCER_PCK0 << ul_id))
			&& !(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)));
}

/**
 * \brief Switch programmable clock source selection to slow clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_sclk(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_SLOW_CLK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
	!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to main clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_mainck(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_MAIN_CLK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
	!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to PLLA clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_pllack(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_PLLA_CLK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
	!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to PLLB clock.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_pllbck(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_PLLB_CLK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
			!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id));
			--ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Switch programmable clock source selection to mck.
 *
 * \param ul_id Id of the programmable clock.
 * \param ul_pres Programmable clock prescaler.
 *
 * \retval 0 Success.
 * \retval 1 Timeout error.
 */
uint32_t pmc_switch_pck_to_mck(uint32_t ul_id, uint32_t ul_pres)
{
	uint32_t ul_timeout;

	PMC->PMC_PCK[ul_id] = PMC_PCK_CSS_MCK | ul_pres;
	for (ul_timeout = PMC_TIMEOUT;
	!(PMC->PMC_SR & (PMC_SR_PCKRDY0 << ul_id)); --ul_timeout) {
		if (ul_timeout == 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief Enable the specified programmable clock.
 *
 * \param ul_id Id of the programmable clock.
 */
void pmc_enable_pck(uint32_t ul_id)
{
	PMC->PMC_SCER = PMC_SCER_PCK0 << ul_id;
}

/**
 * \brief Disable the specified programmable clock.
 *
 * \param ul_id Id of the programmable clock.
 */
void pmc_disable_pck(uint32_t ul_id)
{
	PMC->PMC_SCDR = PMC_SCER_PCK0 << ul_id;
}

/**
 * \brief Enable all programmable clocks.
 */
void pmc_enable_all_pck(void)
{
	PMC->PMC_SCER = PMC_SCER_PCK0 | PMC_SCER_PCK1 | PMC_SCER_PCK2;
}

/**
 * \brief Disable all programmable clocks.
 */
void pmc_disable_all_pck(void)
{
	PMC->PMC_SCDR = PMC_SCDR_PCK0 | PMC_SCDR_PCK1 | PMC_SCDR_PCK2;
}

/**
 * \brief Check if the specified programmable clock is enabled.
 *
 * \param ul_id Id of the programmable clock.
 *
 * \retval 0 Programmable clock is disabled or unknown.
 * \retval 1 Programmable clock is enabled.
 */
uint32_t pmc_is_pck_enabled(uint32_t ul_id)
{
	if (ul_id > 2) {
		return 0;
	}

	return (PMC->PMC_SCSR & (PMC_SCSR_PCK0 << ul_id));
}

/**
 * \brief Switch UDP (USB) clock source selection to PLLA clock.
 *
 * \param ul_usbdiv Clock divisor.
 */
void pmc_switch_udpck_to_pllack(uint32_t ul_usbdiv)
{
	PMC->PMC_USB = PMC_USB_USBDIV(ul_usbdiv);
}

/**
 * \brief Switch UDP (USB) clock source selection to PLLB clock.
 *
 * \param ul_usbdiv Clock divisor.
 */
void pmc_switch_udpck_to_pllbck(uint32_t ul_usbdiv)
{
	PMC->PMC_USB = PMC_USB_USBDIV(ul_usbdiv) | PMC_USB_USBS;
}

/**
 * \brief Enable UDP (USB) clock.
 */
void pmc_enable_udpck(void)
{
	PMC->PMC_SCER = PMC_SCER_UDP;
}

/**
 * \brief Disable UDP (USB) clock.
 */
void pmc_disable_udpck(void)
{
	PMC->PMC_SCDR = PMC_SCDR_UDP;
}

/**
 * \brief Enable PMC interrupts.
 *
 * \param ul_sources Interrupt sources bit map.
 */
void pmc_enable_interrupt(uint32_t ul_sources)
{
	PMC->PMC_IER = ul_sources;
}

/**
 * \brief Disable PMC interrupts.
 *
 * \param ul_sources Interrupt sources bit map.
 */
void pmc_disable_interrupt(uint32_t ul_sources)
{
	PMC->PMC_IDR = ul_sources;
}

/**
 * \brief Get PMC interrupt mask.
 *
 * \return The interrupt mask value.
 */
uint32_t pmc_get_interrupt_mask(void)
{
	return PMC->PMC_IMR;
}

/**
 * \brief Get current status.
 *
 * \return The current PMC status.
 */
uint32_t pmc_get_status(void)
{
	return PMC->PMC_SR;
}

/**
 * \brief Set the wake-up inputs for fast startup mode registers
 *        (event generation).
 *
 * \param ul_inputs Wake up inputs to enable.
 */
void pmc_set_fast_startup_input(uint32_t ul_inputs)
{
	ul_inputs &= PMC_FAST_STARTUP_Msk;
	PMC->PMC_FSMR |= ul_inputs;
}

/**
 * \brief Clear the wake-up inputs for fast startup mode registers
 *        (remove event generation).
 *
 * \param ul_inputs Wake up inputs to disable.
 */
void pmc_clr_fast_startup_input(uint32_t ul_inputs)
{
	ul_inputs &= PMC_FAST_STARTUP_Msk;
	PMC->PMC_FSMR &= ~ul_inputs;
}

/**
 * \brief Enable Sleep Mode.
 * Enter condition: (WFE or WFI) + (SLEEPDEEP bit = 0) + (LPM bit = 0)
 *
 * \param uc_type 0 for wait for interrupt, 1 for wait for event.
 * \note For SAM4S, SAM4C, SAM4CM, SAM4CP and SAM4E series,
 * since only WFI is effective, uc_type = 1 will be treated as uc_type = 0.
 */
void pmc_enable_sleepmode(uint8_t uc_type)
{
	PMC->PMC_FSMR &= (uint32_t) ~ PMC_FSMR_LPM; // Enter Sleep mode

	SCB->SCR &= (uint32_t) ~ SCB_SCR_SLEEPDEEP_Msk; // Deep sleep

	if (uc_type == 0) {
		__WFI();
	} else {
		__WFE();
	}
}

/**
 * \brief Enable Backup Mode. Enter condition: WFE/(VROFF bit = 1) +
 * (SLEEPDEEP bit = 1)
 */
void pmc_enable_backupmode(void)
{
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	
	__WFE();
}

/**
 * \brief Enable Clock Failure Detector.
 */
void pmc_enable_clock_failure_detector(void)
{
	uint32_t ul_reg = PMC->CKGR_MOR;

	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_CFDEN | ul_reg;
}

/**
 * \brief Disable Clock Failure Detector.
 */
void pmc_disable_clock_failure_detector(void)
{
	uint32_t ul_reg = PMC->CKGR_MOR & (~CKGR_MOR_CFDEN);

	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | ul_reg;
}

/**
 * \brief Enable or disable write protect of PMC registers.
 *
 * \param ul_enable 1 to enable, 0 to disable.
 */
void pmc_set_writeprotect(uint32_t ul_enable)
{
	if (ul_enable) {
		PMC->PMC_WPMR = PMC_WPMR_WPKEY_PASSWD | PMC_WPMR_WPEN;
	} else {
		PMC->PMC_WPMR = PMC_WPMR_WPKEY_PASSWD;
	}
}

/**
 * \brief Return write protect status.
 *
 * \return Return write protect status.
 */
uint32_t pmc_get_writeprotect_status(void)
{
	return PMC->PMC_WPSR;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
