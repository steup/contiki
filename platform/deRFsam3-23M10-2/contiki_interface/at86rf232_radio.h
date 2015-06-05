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

#ifndef AT86RF232_RADIO_H_
#define AT86RF232_RADIO_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
 
#include <contiki.h>
#include "dev/radio.h"
#include "net/netstack.h"
#include "net/packetbuf.h"

#include "ieee_const.h"

#include "pal.h"
#include "tal.h"
#include "tfa.h"
#ifdef ENABLE_RTB
#	include "rtb.h"
#endif
//#include "mac_api.h"

#include "app_config.h"

/** @brief This is the first timer identifier of the application.
 *
 *  The value of this identifier is an increment of the largest identifier
 *  value used by the MAC.
 */
#if (NUMBER_OF_TOTAL_STACK_TIMERS == 0)
#define APP_FIRST_TIMER_ID          (0)
#else
#define APP_FIRST_TIMER_ID          (LAST_STACK_TIMER_ID + 1)
#endif

#define APP_TIMER APP_FIRST_TIMER_ID

#define DST_PAN_ID                      (IEEE802154_CONF_PANID)
#define SRC_PAN_ID                      (IEEE802154_CONF_PANID)

/* Frame overhead due to selected address scheme incl. FCS */
#if (DST_PAN_ID == SRC_PAN_ID)
#define FRAME_OVERHEAD          (11)
#else
#define FRAME_OVERHEAD          (13)
#endif

#define FCS_LENGTH              (2)

linkaddr_t linkaddr_node_addr = { { 0, 0 } };

extern const struct radio_driver at86rf232_driver;

static int at86rf232_init( void );
static void at86rf232_conf( void );
//static void print_debug( void );
static int prepare(const void *payload, unsigned short payload_len);
static int transmit(unsigned short transmit_len);
static int send(const void *payload, unsigned short payload_len);
static int radio_read(void *buf, unsigned short buf_len);
static int channel_clear( void );
static int receiving_packet( void );
static int pending_packet( void );
static int on( void );
static int off( void );
static radio_result_t get_value(radio_param_t param, radio_value_t *value);
static radio_result_t set_value(radio_param_t param, radio_value_t value);
static radio_result_t get_object(radio_param_t param, void *dest, size_t size);
static radio_result_t set_object(radio_param_t param, const void *src, size_t size);

retval_t set_phy_mac_addr(uint64_t mac);

#endif /* AT86RF232_RADIO_H_ */