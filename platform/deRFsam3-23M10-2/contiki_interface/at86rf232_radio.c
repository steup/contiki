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

#include "at86rf232_radio.h"

uint8_t is_rx_enabled;
uint8_t is_rx_buffer_empty;
uint8_t is_receiving_packet;
uint8_t rx_receive_len;

static uint8_t rx_buffer[LARGE_BUFFER_SIZE];
static uint8_t tx_buffer[LARGE_BUFFER_SIZE];

const struct radio_driver at86rf232_driver =
{
	at86rf232_init,
	prepare,
	transmit,
	send,
	radio_read,
	channel_clear,
	receiving_packet,
	pending_packet,
	on,
	off,
	get_value,
	set_value,
	get_object,
	set_object
};

PROCESS(at86rf232_driver_process, "AT86RF232 Driver");

PROCESS_THREAD(at86rf232_driver_process, ev, data)
{	
//	printf("at86rf232_driver_process is running\r\n");
	
	PROCESS_BEGIN();
		switch (ev)
		{
			case PROCESS_EVENT_POLL:
//				printf("at86rf232_driver_process PROCESS_EVENT_POLL\r\n");
			
				packetbuf_clear();
					
				if (radio_read(packetbuf_dataptr(), rx_receive_len) != RADIO_RESULT_ERROR )
				{
					packetbuf_set_datalen(rx_receive_len);
					NETSTACK_RDC.input();
				}

				if (!is_rx_buffer_empty)
				{
					process_poll(&at86rf232_driver_process);
					break;
				}	
			
			default:
				process_post(&at86rf232_driver_process, PROCESS_EVENT_CONTINUE, NULL);

				pal_task();
				tal_task();
#ifdef ENABLE_RTB 				
				rtb_task();
#endif /* ENABLE_RTB */								
				PT_RESTART(process_pt);
		}
	PROCESS_END();
}

static int at86rf232_init( void )
{
//	printf("[at86rf232_radio] init\r\n");
	
	//set_phy_mac_addr( 0x00212effff0021e6 );
	
	/* Initialize TAL */
    if (tal_init() != MAC_SUCCESS)
    {
        printf("[at86rf232_radio] EMERGENCY, tal_init\r\n");
		
		return RADIO_RESULT_ERROR;
    }
	
#ifdef ENABLE_RTB
    /* Initialize RTB */
    if (rtb_init() != RTB_SUCCESS)
    {
        printf("[at86rf232_radio] EMERGENCY, rtb_init\r\n");
		
		return RADIO_RESULT_ERROR;
    }
#endif  /* ENABLE_RTB */

    /* Calibrate MCU's RC oscillator */
    if (!pal_calibrate_rc_osc())
    {
        printf("[at86rf232_radio] EMERGENCY, pal_calibrate_rc_osc\r\n");
		
		return RADIO_RESULT_ERROR;
    }

    /*
     * The stack is initialized above, hence the global interrupts are enabled
     * here.
     */
    pal_global_irq_enable();
	
	is_rx_enabled = 1;
	is_rx_buffer_empty = 1;
	is_receiving_packet = 0;

	at86rf232_conf();
	
	uint64_t pib_value_64;
	uint8_t *ptr_big  = (uint8_t*)&pib_value_64;
		
	/* converting from little-endian to big-endian for the node address*/
	uint8_t i = 8;
	while ( i-- )
	{
		ptr_big[i] = ((uint8_t*)&tal_pib.IeeeAddress)[7 - i];
	}

	/* set the internal mac address to the contiki node address */
#if LINKADDR_SIZE == 2 || LINKADDR_SIZE == 8
	linkaddr_set_node_addr((linkaddr_t *)&ptr_big[8 - LINKADDR_SIZE]);
#endif

//	print_debug();

	process_start(&at86rf232_driver_process, NULL);
	
	return RADIO_RESULT_OK;
}

static void at86rf232_conf( void )
{
	uint8_t pib_value_8;
	uint16_t pib_value_16;
	
	pib_value_8 = DEFAULT_CHANNEL;
	tal_pib_set(phyCurrentChannel, (pib_value_t *)&pib_value_8);
	//set_value(RADIO_PARAM_CHANNEL, (radio_value_t)pib_value_8);
	
	pib_value_16 = IEEE802154_CONF_PANID;	
	tal_pib_set(macPANId, (pib_value_t *)&pib_value_16);
	//set_value(RADIO_PARAM_PAN_ID, (radio_value_t)pib_value_16);
	
	
	pib_value_16 = (uint16_t)tal_pib.IeeeAddress;
	tal_pib_set(macShortAddress, (pib_value_t *)&pib_value_16);
}
/*
static void print_debug( void )
{
	printf("%-21s: %u\r\n", 	"phyCurrentPage", 		tal_pib.CurrentPage);
	printf("%-21s: %u\r\n", 	"phyCurrentChannel", 	tal_pib.CurrentChannel);
	printf("%-21s: 0x%x\r\n", 	"macPANId", 			tal_pib.PANId);
	printf("%-21s: 0x%x\r\n", 	"macShortAddress", 		tal_pib.ShortAddress);
	printf("%-21s: %lu\r\n", 	"phyChannelsSupported",	tal_pib.SupportedChannels);
	
	uint8_t *p_pib_value_64 = (uint8_t*)&tal_pib.IeeeAddress;		
	printf("%-21s: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\r\n", 
		"macIeeeAddress", p_pib_value_64[7], p_pib_value_64[6],
		p_pib_value_64[5], p_pib_value_64[4], p_pib_value_64[3],
		p_pib_value_64[2], p_pib_value_64[1], *p_pib_value_64);
}
*/
/* Prepare the radio with a packet to be sent. */
static int prepare(const void *payload, unsigned short payload_len)
{	
	frame_info_t *tx_frame_info;
	uint8_t *frame_ptr;
/*	uint8_t i;
	
	printf("[at86rf232_radio] prepare\r\n");
	
	if (payload_len > aMaxPHYPacketSize)
	{
		printf("[at86rf232_radio] LARGE_BUFFER_SIZE\r\n");
		return RADIO_TX_ERR;
	}
	
	printf("payload (%u bytes):", payload_len);
	for (i = 0; i < payload_len; ++i)
	{
		printf(" %x", ((uint8_t*)payload)[i]);
	}
	printf("\r\n");
*/	
	tx_frame_info = (frame_info_t*)tx_buffer;
	
	frame_ptr = (uint8_t *)tx_frame_info + 
		LARGE_BUFFER_SIZE - payload_len - FCS_LENGTH - FRAME_OVERHEAD;

	*frame_ptr = payload_len + FCS_LENGTH; /* FCS (Frame-Check-Sequence) size */
	tx_frame_info->mpdu = frame_ptr;
	memcpy(++frame_ptr, payload, payload_len);
	
	return RADIO_TX_OK;
}

/* Send the packet that has previously been prepared. */
static int transmit(unsigned short transmit_len)
{
	uint8_t err;
	
//	printf("[at86rf232_radio] transmit\r\n");
	
	/* transmits the data */
	for (err = 0; err < 3; ++err)
	{
		if (tal_tx_frame((frame_info_t *)tx_buffer, CSMA_UNSLOTTED, false) == MAC_SUCCESS)
		{
			return RADIO_TX_OK;
		}
		
		pal_timer_delay(1000);
	}
	
//	printf("[at86rf232_radio] transmit :-(\r\n");
		
	return RADIO_TX_ERR;
}

/* Prepare & transmit a packet. */
static int send(const void *payload, unsigned short payload_len)
{
//	printf("[at86rf232_radio] send\r\n");
	
	prepare(payload, payload_len);
	return transmit(payload_len);
}

/* Read a received packet into a buffer. */
static int radio_read(void *buf, unsigned short buf_len)
{
//	printf("[at86rf232_radio] radio_read\r\n");
	
	if (is_rx_buffer_empty)
	{
		return RADIO_RESULT_ERROR;
	}
	
	memcpy(buf, rx_buffer, buf_len);
	
	is_rx_buffer_empty = 1;
	
	return RADIO_RESULT_OK;
}

/* Perform a Clear-Channel Assessment (CCA) to find out if there is a packet in the air or not. */
static int channel_clear( void )
{
//	printf("[at86rf232_radio] channel_clear\r\n");
	
	return 1;
}

/* Check if the radio driver is currently receiving a packet. */
static int receiving_packet( void )
{
//	printf("[at86rf232_radio] receiving_packet\r\n");
	
	return is_receiving_packet;
}

/* Check if the radio driver has just received a packet. */
static int pending_packet( void )
{
//	printf("[at86rf232_radio] pending_packet\r\n");
	
	return !is_rx_buffer_empty;
}

/* Turn the radio on */
static int on( void )
{
//	printf("[at86rf232_radio] on\r\n");
	
	is_rx_enabled = 1;
	is_receiving_packet = 0;
	
	/* Switch receiver on */
    tal_rx_enable(PHY_RX_ON);
	
	return RADIO_RESULT_OK;
}

/* Turn the radio off */
static int off( void )
{
//	printf("[at86rf232_radio] off\r\n");
	
	is_rx_enabled = 0;
	is_receiving_packet = 0;
	
	/* Switch receiver off */
    tal_rx_enable(PHY_TRX_OFF);
	
	return RADIO_RESULT_OK;
}

static radio_result_t get_value(radio_param_t param, radio_value_t *value)
{
	
//	printf("[at86rf232_radio] get_value\r\n");
	
	switch (param)
	{
		case RADIO_PARAM_POWER_MODE:
			
			return is_rx_enabled ? RADIO_POWER_MODE_ON : RADIO_POWER_MODE_OFF;
			
		case RADIO_PARAM_CHANNEL:
		
			*(uint8_t*)value = tal_pib.CurrentChannel;
			//printf("phyCurrentChannel: %u\r\n", value);
				
			return RADIO_RESULT_OK;
			
		case RADIO_PARAM_PAN_ID:

			*(uint16_t*)value = tal_pib.PANId;
			//printf("macPANId: 0x%x\r\n", value);
			
			return RADIO_RESULT_OK;
				
		case RADIO_PARAM_16BIT_ADDR:
		
			*(uint16_t*)value = tal_pib.ShortAddress;
			//printf("macShortAddress: 0x%x\r\n", value);
			
			return RADIO_RESULT_OK;
			
		case RADIO_PARAM_64BIT_ADDR:
		
			*(uint64_t*)value = tal_pib.IeeeAddress;	
			//printf("macIeeeAddress: %llu\r\n", value);
			
			return RADIO_RESULT_OK;
		
		case RADIO_PARAM_TXPOWER:
			
			*(uint8_t*)value = CONV_phyTransmitPower_TO_DBM(tal_pib.TransmitPower);
			//printf("phyTransmitPower: %llu\r\n", *(uint8_t*)value);
			
			return RADIO_RESULT_OK;
		
		/* The minimum transmission power in dBm. */
		case RADIO_CONST_TXPOWER_MIN:
			
			/* value is already in dBm (tal.pib, tx_pwr_table) */
			*(int8_t*)value = -17;
			
			return RADIO_RESULT_OK;
		
		/* The maximum transmission power in dBm. */
		case RADIO_CONST_TXPOWER_MAX:
		
			/* value is already in dBm (tal.pib, tx_pwr_table) */			
			*(int8_t*)value = 3;
			
			return RADIO_RESULT_OK;
		
		default:
			return RADIO_RESULT_NOT_SUPPORTED;
	}
}

static radio_result_t set_value(radio_param_t param, radio_value_t value)
{
//	printf("[at86rf232_radio] set_value\r\n");
	
	switch (param)
	{
		case RADIO_PARAM_POWER_MODE:
			
			if (value == RADIO_POWER_MODE_ON)
			{
				return on();
			}
			
			if (value == RADIO_POWER_MODE_OFF)
			{
				return off();
			}
			
			return RADIO_RESULT_ERROR;
			
		case RADIO_PARAM_CHANNEL:
			
			if (tal_pib_set(phyCurrentChannel, (pib_value_t*)&value) == MAC_SUCCESS)
			{		
				//printf("phyCurrentChannel: %u\r\n", (uint8_t)value);
				
				return RADIO_RESULT_OK;
			}
			
			return RADIO_RESULT_ERROR;
	
		case RADIO_PARAM_PAN_ID:
		
			if (tal_pib_set(macPANId, (pib_value_t*)&value) == MAC_SUCCESS)
			{
				//printf("macPANId: 0x%x\r\n", (uint16_t)value);
				
				return RADIO_RESULT_OK;
			}
			
			return RADIO_RESULT_ERROR;
			
		case RADIO_PARAM_16BIT_ADDR:
		
			if (tal_pib_set(macShortAddress, (pib_value_t*)&value) == MAC_SUCCESS)
			{
				//printf("macShortAddress: 0x%x\r\n", (uint16_t)value);
				
				return RADIO_RESULT_OK;
			}
			
			return RADIO_RESULT_ERROR;
			
		case RADIO_PARAM_64BIT_ADDR:
		
			if (tal_pib_set(macIeeeAddress, (pib_value_t*)&value) == MAC_SUCCESS)
			{
				//printf("macIeeeAddress: %llu\r\n", (uint64_t)value);
				
				return RADIO_RESULT_OK;
			}
		
			return RADIO_RESULT_ERROR;
		
		case RADIO_PARAM_TXPOWER:
			
			if (tal_pib_set(phyTransmitPower, (pib_value_t*)&value) == MAC_SUCCESS)
			{
				//printf("phyTransmitPower: %llu\r\n", *(uint8_t)value);
				
				return RADIO_RESULT_OK;
			}
			
			return RADIO_RESULT_ERROR;
		
		default:
			return RADIO_RESULT_NOT_SUPPORTED;
	}
	
	return RADIO_RESULT_NOT_SUPPORTED;
}

static radio_result_t get_object(radio_param_t param, void *dest, size_t size)
{
//	printf("[at86rf232_radio] get_object\r\n");
	
	return RADIO_RESULT_NOT_SUPPORTED;
}

static radio_result_t set_object(radio_param_t param, const void *src, size_t size)
{
//	printf("[at86rf232_radio] set_object\r\n");
	
	return RADIO_RESULT_NOT_SUPPORTED;
}

/**
 * @brief Callback that is called if data has been received by trx.
 *
 * @param mac_frame_info    Pointer to received data structure
 */
void tal_rx_frame_cb(frame_info_t *frame)
{
//	uint16_t i;
	
//	printf("[at86rf232_radio] tal_rx_frame_cb\r\n");
	
	is_receiving_packet = 1;
	rx_receive_len = frame->mpdu[0] - FCS_LENGTH; 
		
/*	printf("\treceived (%u bytes):", frame->mpdu[0]);

	for (i = 1; i <= rx_receive_len + FCS_LENGTH; ++i)
	{
		printf("%02x ", frame->mpdu[i]);
	}
	
	printf("\r\n");
*/	
	/* frame->mpdu[0] empfangende bytes ohne size */
	memcpy(rx_buffer, frame->mpdu + 1, rx_receive_len);
	
	is_rx_buffer_empty = 0;
    
    /* free buffer that was used for frame reception */
    bmm_buffer_free((buffer_t *)(frame->buffer_header));
	
	is_receiving_packet = 0;
	
	process_poll(&at86rf232_driver_process);
}

/**
 * @brief Callback that is called once tx is done.
 *
 * @param status    Status of the transmission procedure
 * @param frame     Pointer to the transmitted frame structure
 */
void tal_tx_frame_done_cb(retval_t status, frame_info_t *frame)
{
/*	uint16_t i;
	
	printf("[at86rf232_radio] tal_tx_frame_done_cb(%u)\r\n", (uint8_t)status);
	
	printf("\tsend (%u bytes):" , frame->mpdu[0]);

	for (i = 0; i <= frame->mpdu[0]; ++i)
	{
		printf("%02x ", frame->mpdu[i]);
	}
	
	printf("\r\n");
*/	
    if (status == MAC_SUCCESS)
    {
        pal_task();
    }
    else if (status == MAC_CHANNEL_ACCESS_FAILURE)
        /*
         * Channel access failure is the only transmission failure that is handled
         * within this application.
         * If other status codes shall be handled, such as MAC_NO_ACK, this is probably
         * the wrong application on the wrong layer.
         *
         * For absolutely reliable transmission, please use a MAC or TAL based
         * application. The Tiny-TAL is not designed for such a purpose.
         *
         * In case of channel access failure the frame is retried.
         */
    {
        printf("[at86rf232_radio] tal_tx_frame_done_cb: MAC_CHANNEL_ACCESS_FAILURE\r\n");
        /* Transmission was not successful, initiate retry. */
    }
    else
    {
        /*
         * Other transmission status codes are not handled
         * within this application.
         * The transmission is considered as beeing completed for this frame.
         */
    }
}

retval_t set_phy_mac_addr(uint64_t mac)
{
	return pal_ps_set( EE_IEEE_ADDR, 8 , (void*)&mac );
}