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

#ifndef CONTIKI_CONF_H_DERFSAM3_23M10_2__
#define CONTIKI_CONF_H_DERFSAM3_23M10_2__

#include <stdint.h>

#define CCIF
#define CLIF

typedef long unsigned int clock_time_t;

#define CLOCK_CONF_SECOND 		1000


typedef unsigned int uip_stats_t;

#define NETSTACK_CONF_MAC		nullmac_driver
#define NETSTACK_CONF_RDC		nullrdc_driver
#define NETSTACK_CONF_FRAMER	framer_802154
#define NETSTACK_CONF_RADIO		at86rf232_driver

#define DEFAULT_CHANNEL         (21)
#define IEEE802154_CONF_PANID 	0xCAFE

#define LINKADDR_CONF_SIZE 		2 // 2 (default) or 8

#define UIP_CONF_BROADCAST 1

#endif /* CONTIKI_CONF_H_DERFSAM3_23M10_2__ */
