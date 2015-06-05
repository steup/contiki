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

#include "cpu.h"

void cpu_init( void )
{	
	wdt_disable(WDT);
	
	sysclk_init();
	
	uart_stdio_init(STD_UART);
}

void print_cpu_info( void )
{
	uint32_t cpu_id;
	
	printf("CPU-Info\r\n########\r\n\r\n");
	
	cpu_id = CHIPID->CHIPID_CIDR;
	
	printf("%-7s: 0x%lX", "ID", cpu_id);
	
	switch (cpu_id)
	{
		case ID_ATSAM3S4A:
			printf(" (SAM3S4A)");
			break;
			
		case ID_ATSAM3S4B:
			printf(" (SAM3S4B)");
			break;
			
		case ID_ATSAM3S4C:
			printf(" (SAM3S4C)");
			break;
	}
	
	printf("\r\n%-7s: 0x%02lX", "Version", cpu_id & CHIPID_CIDR_VERSION_Msk);
	
	printf("\r\n%-7s: 0x%02lX", "EPROC", (cpu_id & CHIPID_CIDR_EPROC_Msk) >> CHIPID_CIDR_EPROC_Pos);
	
	switch (cpu_id & CHIPID_CIDR_EPROC_Msk)
	{
		case CHIPID_CIDR_EPROC_ARM946ES:
			printf(" (ARM946ES)");
			break;
			
		case CHIPID_CIDR_EPROC_ARM7TDMI:
			printf(" (ARM7TDMI)");
			break;
			
		case CHIPID_CIDR_EPROC_CM3:
			printf(" (Cortex-M3)");
			break;
			
		case CHIPID_CIDR_EPROC_ARM920T:
			printf(" (ARM920T)");
			break;
			
		case CHIPID_CIDR_EPROC_ARM926EJS:
			printf(" (ARM926EJS)");
			break;
			
		case CHIPID_CIDR_EPROC_CA5:
			printf(" (Cortex-A5)");
			break;
			
		case CHIPID_CIDR_EPROC_CM4:
			printf(" (Cortex-M5)");
			break;
	}
	
	printf("\r\n%-7s: 0x%02lX", "NVPSIZ", (cpu_id & CHIPID_CIDR_NVPSIZ_Msk) >> CHIPID_CIDR_NVPSIZ_Pos);
	
	switch (cpu_id & CHIPID_CIDR_NVPSIZ_Msk)
	{
		case CHIPID_CIDR_NVPSIZ_NONE:
			printf(" (None)");
			break;
			
		case CHIPID_CIDR_NVPSIZ_8K:
			printf(" (8 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ_16K:
			printf(" (16 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ_32K:
			printf(" (32 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ_64K:
			printf(" (64 KByte)");
			break;
		
		case CHIPID_CIDR_NVPSIZ_128K:
			printf(" (128 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ_256K:
			printf(" (256 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ_512K:
			printf(" (512 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ_1024K:
			printf(" (1024 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ_2048K:
			printf(" (2048 KByte)");
			break;
			
		default:
			printf(" (Reserved)");
	}
	
	/* Second Nonvolatile Program Memory Size */
	printf("\r\n%-7s: 0x%02lX", "NVPSIZ2", (cpu_id & CHIPID_CIDR_NVPSIZ2_Msk) >> CHIPID_CIDR_NVPSIZ2_Pos);
	
	switch (cpu_id & CHIPID_CIDR_NVPSIZ2_Msk)
	{
		case CHIPID_CIDR_NVPSIZ2_NONE:
			printf(" (None)");
			break;
			
		case CHIPID_CIDR_NVPSIZ2_8K:
			printf(" (8 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ2_16K:
			printf(" (16 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ2_32K:
			printf(" (32 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ2_64K:
			printf(" (64 KByte)");
			break;
		
		case CHIPID_CIDR_NVPSIZ2_128K:
			printf(" (128 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ2_256K:
			printf(" (256 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ2_512K:
			printf(" (512 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ2_1024K:
			printf(" (1024 KByte)");
			break;
			
		case CHIPID_CIDR_NVPSIZ2_2048K:
			printf(" (2048 KByte)");
			break;
			
		default:
			printf(" (Reserved)");
	}
	
	printf("\r\n%-7s: 0x%02lX", "SRAMSIZ", (cpu_id & CHIPID_CIDR_SRAMSIZ_Msk) >> CHIPID_CIDR_SRAMSIZ_Pos);
	
	switch (cpu_id & CHIPID_CIDR_SRAMSIZ_Msk)
	{
		case CHIPID_CIDR_SRAMSIZ_48K:
			printf(" (48 KByte)");
			break;
			
		case CHIPID_CIDR_SRAMSIZ_192K:
			printf(" (192 KByte)");
			break;
			
		case CHIPID_CIDR_SRAMSIZ_2K:
			printf(" (2 KByte)");
			break;
			
		case CHIPID_CIDR_SRAMSIZ_6K:
			printf(" (6 KByte)");
			break;

		case CHIPID_CIDR_SRAMSIZ_24K:
			printf(" (24 KByte)");
			break;
		
		case CHIPID_CIDR_SRAMSIZ_4K:
			printf(" (4 KByte)");
			break;
			
		case CHIPID_CIDR_SRAMSIZ_80K:
			printf(" (80 KByte)");
			break;
		
		case CHIPID_CIDR_SRAMSIZ_160K:
			printf(" (160 KByte)");
			break;
		
		case CHIPID_CIDR_SRAMSIZ_8K:
			printf(" (8 KByte)");
			break;
			
		case CHIPID_CIDR_SRAMSIZ_16K:
			printf(" (16 KByte)");
			break;
			
		case CHIPID_CIDR_SRAMSIZ_32K:
			printf(" (32 KByte)");
			break;
		
		case CHIPID_CIDR_SRAMSIZ_64K:
			printf(" (64 KByte)");
			break;
			
		case CHIPID_CIDR_SRAMSIZ_128K:
			printf(" (128 KByte)");
			break;
		
		case CHIPID_CIDR_SRAMSIZ_256K:
			printf(" (256 KByte)");
			break;
			
		case CHIPID_CIDR_SRAMSIZ_96K:
			printf(" (96 KByte)");
			break;
			
		case CHIPID_CIDR_SRAMSIZ_512K:
			printf(" (512 KByte)");
			break;
	}
	
	printf("\r\n%-7s: 0x%02lX", "ARCH", (cpu_id & CHIPID_CIDR_ARCH_Msk) >> CHIPID_CIDR_ARCH_Pos);
	
	switch (cpu_id & CHIPID_CIDR_ARCH_Msk)
	{
		case CHIPID_CIDR_ARCH_AT91SAM9xx:
			printf(" (AT91SAM9xx Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91SAM9XExx:
			printf(" (AT91SAM9XExx Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91x34:
			printf(" (AT91x34 Series)");
			break;
			
		case CHIPID_CIDR_ARCH_CAP7:
			printf(" (CAP7 Series)");
			break;
			
		case CHIPID_CIDR_ARCH_CAP9:
			printf(" (CAP9 Series)");
			break;
			
		case CHIPID_CIDR_ARCH_CAP11:
			printf(" (CAP11 Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91x40:
			printf(" (AT91x40 Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91x42:
			printf(" (AT91x42 Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91SAM4SH2:
			printf(" (AT91SAM4SH2 Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91x55:
			printf(" (AT91x55 Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91SAM7Axx:
			printf(" (AT91SAM7Axx Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91SAM7AQxx:
			printf(" (AT91SAM7AQxx Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91x63:
			printf(" (AT91x63 Series)");
			break;
			
		case CHIPID_CIDR_ARCH_SAM4CxxC:
			printf(" (SAM4CxC Series (100-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_AT91SAM7Sxx:
			printf(" (AT91SAM7Sxx Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91SAM7XCxx:
			printf(" (AT91SAM7XCxx Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91SAM7SExx:
			printf(" (AT91SAM7SExx Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91SAM7Lxx:
			printf(" (AT91SAM7Lxx Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91SAM7Xxx:
			printf(" (AT91SAM7Xxx Series)");
			break;
			
		case CHIPID_CIDR_ARCH_AT91SAM7SLxx:
			printf(" (AT91SAM7SLxx Series)");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3UxC:
			printf(" (ATSAM3UxC Series (100-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3UxE:
			printf(" (ATSAM3UxE Series (144-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3AxC:
			printf(" (ATSAM3AxC Series (100-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3XxC:
			printf(" (ATSAM3XxC Series (100-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3XxE:
			printf(" (ATSAM3XxE Series (144-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3XxG:
			printf(" (ATSAM3XxG Series (208/217-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3SxA:
			printf(" (ATSAM3SxA Series (48-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3SxB:
			printf(" (ATSAM3SxB Series (64-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3SxC:
			printf(" (ATSAM3SxC Series (100-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_AT91x92:
			printf(" (AT91x92 Series)");
			break;
			
		case (uint32_t)(0x93 << CHIPID_CIDR_ARCH_Pos):
			printf(" (ATSAM3NxA Series (48-pin version))");
			break;
			
		case (uint32_t)(0x94 << CHIPID_CIDR_ARCH_Pos):
			printf(" (ATSAM3NxB Series (64-pin version))");
			break;
			
		case (uint32_t)(0x95 << CHIPID_CIDR_ARCH_Pos):
			printf(" (ATSAM3NxC Series (100-pin version))");
			break;
			
		case (uint32_t)(0x98 << CHIPID_CIDR_ARCH_Pos):
			printf(" (ATSAM3SDxA Series (48-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3SDxB:
			printf(" (ATSAM3SDxB Series (64-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM3SDxC:
			printf(" (ATSAM3SDxC Series (100-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM5A:
			printf(" (ATSAM5A)");
			break;
			
		case CHIPID_CIDR_ARCH_SAM4LxA:
			printf(" (SAM4LxA Series (48-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM4LxB:
			printf(" (SAM4LxB Series (64-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_SAM4LxC:
			printf(" (SAM4LxC Series (100-pin version))");
			break;
			
		case CHIPID_CIDR_ARCH_AT75Cxx:
			printf(" (AT75Cxx Series)");
			break;
	}
	
	/* Nonvolatile Program Memory Type */
	printf("\r\n%-7s: 0x%02lX", "NVPTYP", (cpu_id & CHIPID_CIDR_NVPTYP_Msk) >> CHIPID_CIDR_NVPTYP_Pos);
	
	switch (cpu_id & CHIPID_CIDR_NVPTYP_Msk)
	{
		case CHIPID_CIDR_NVPTYP_ROM:
			printf(" (ROM)");
			break;
			
		case CHIPID_CIDR_NVPTYP_ROMLESS:
			printf(" (ROMless or on-chip Flash)");
			break;
			
		case CHIPID_CIDR_NVPTYP_FLASH:
			printf(" (Embedded Flash Memory)");
			break;
			
		case CHIPID_CIDR_NVPTYP_ROM_FLASH:
			printf(" (ROM and Embedded Flash Memory / NVPSIZ is ROM size / NVPSIZ2 is Flash size)");
			break;
			
		case CHIPID_CIDR_NVPTYP_SRAM:
			printf(" (SRAM emulating ROM)");
			break;
	}
	
	printf("\r\n%-7s: 0x%02lX\r\n\r\n", "EXT", cpu_id & CHIPID_CIDR_EXT);
}
