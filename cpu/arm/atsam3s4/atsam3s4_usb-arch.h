#ifndef ATSAM3S4_USB_ARCH_H_
#define ATSAM3S4_USB_ARCH_H_

#include <usb-arch.h>
#include <stdio.h>
#include "gpio.h"
#include "nvic.h"
#include "debug-uart.h"
#include "usb-atsam3s4.h"

/* #define DEBUG     */
#ifdef DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

typedef struct
{
  vu32 EPR[8];
  u32  RESERVED[8];
  vu32 CNTR;
  vu32 ISTR;
  vu32 FNR;
  vu32 DADDR;
  vu32 BTABLE;
} USB_TypeDef;

#define USB_BASE             (APB1PERIPH_BASE + 0x5c00)
#define USB_MEM_BASE	     (APB1PERIPH_BASE + 0x6000)
#define USB_MEM_SIZE	     (512)
#define USB ((volatile USB_TypeDef *) USB_BASE)

typedef struct
{
  vu32 ADDR_TX;
  vu32 COUNT_TX;
  vu32 ADDR_RX;
  vu32 COUNT_RX;
} USB_HW_Buffer;

#define USB_EP0_BUF_SIZE (2*CTRL_EP_SIZE)

#define USB_EP1_BUF_SIZE (2*USB_EP1_SIZE)

#define USB_EP2_BUF_SIZE (2*USB_EP2_SIZE)

#define USB_EP3_BUF_SIZE (2*USB_EP3_SIZE)

#define USB_EP4_BUF_SIZE (2*USB_EP4_SIZE)

#define USB_EP5_BUF_SIZE (2*USB_EP5_SIZE)

#define USB_EP6_BUF_SIZE (2*USB_EP6_SIZE)

#define USB_EP7_BUF_SIZE (2*USB_EP7_SIZE)



#ifndef MAX_CTRL_DATA
#define MAX_CTRL_DATA 128
#endif
/* Double buffered IN endpoint */
#define ADDR_TX_0 ADDR_TX
#define ADDR_TX_1 ADDR_RX
#define COUNT_TX_0 COUNT_TX
#define COUNT_TX_1 COUNT_RX

/* Double buffered OUT endpoint */
#define ADDR_RX_0 ADDR_TX
#define ADDR_RX_1 ADDR_RX
#define COUNT_RX_0 COUNT_TX
#define COUNT_RX_1 COUNT_RX

#define USB_EPxR_EP_TYPE_BULK 0
#define USB_EPxR_EP_TYPE_CONTROL USB_EP0R_EP_TYPE_0
#define USB_EPxR_EP_TYPE_ISO USB_EP0R_EP_TYPE_1
#define USB_EPxR_EP_TYPE_INTERRUPT (USB_EP0R_EP_TYPE_1|USB_EP0R_EP_TYPE_0)

#define USB_EPxR_EP_DBL_BUF USB_EP0R_EP_KIND
#define USB_EPxR_EP_STATUS_OUT USB_EP0R_EP_KIND

#define USB_EPxR_STAT_RX_DISABLED 0
#define USB_EPxR_STAT_RX_STALL USB_EP0R_STAT_RX_0
#define USB_EPxR_STAT_RX_NAK USB_EP0R_STAT_RX_1
#define USB_EPxR_STAT_RX_VALID (USB_EP0R_STAT_RX_1|USB_EP0R_STAT_RX_0)

#define USB_EPxR_STAT_TX_DISABLED 0
#define USB_EPxR_STAT_TX_STALL USB_EP0R_STAT_TX_0
#define USB_EPxR_STAT_TX_NAK USB_EP0R_STAT_TX_1
#define USB_EPxR_STAT_TX_VALID (USB_EP0R_STAT_TX_1|USB_EP0R_STAT_TX_0)

#define USB_EPxR_SW_BUF_TX USB_EP0R_DTOG_RX
#define USB_EPxR_SW_BUF_RX USB_EP0R_DTOG_TX

static const uint16_t ep_buffer_size[8] =
	{
		USB_EP0_BUF_SIZE,
		USB_EP1_BUF_SIZE,
		USB_EP2_BUF_SIZE,
		USB_EP3_BUF_SIZE,
		USB_EP4_BUF_SIZE,
		USB_EP5_BUF_SIZE,
		USB_EP6_BUF_SIZE,
		USB_EP7_BUF_SIZE
	};

#define USB_EP_BUF_SIZE(ep) ep_buffer_size[ep]
#define USB_EP_BUF_OFFSET(ep) ep_buffer_offset[ep]
#define USB_EP_BUF_ADDR(ep) (u32*)(USB_MEM_BASE + ep_buffer_offset[ep]*2);
#define USB_EP_BUF_DESC(ep) ((USB_HW_Buffer*)(USB_MEM_BASE + 16 * (ep)))

#define USB_EP0_OFFSET (8*USB_MAX_ENDPOINTS)
#define USB_EP1_OFFSET (USB_EP0_OFFSET + USB_EP0_BUF_SIZE)
#define USB_EP2_OFFSET (USB_EP1_OFFSET + USB_EP1_BUF_SIZE)
#define USB_EP3_OFFSET (USB_EP2_OFFSET + USB_EP2_BUF_SIZE)
#define USB_EP4_OFFSET (USB_EP3_OFFSET + USB_EP3_BUF_SIZE)
#define USB_EP5_OFFSET (USB_EP4_OFFSET + USB_EP4_BUF_SIZE)
#define USB_EP6_OFFSET (USB_EP5_OFFSET + USB_EP5_BUF_SIZE)
#define USB_EP7_OFFSET (USB_EP6_OFFSET + USB_EP6_BUF_SIZE)

#if (USB_EP7_OFFSET+USB_EP7_BUF_SIZE) > USB_MEM_SIZE
#error USB endpoints buffers does not fit in USB memory
#endif
static const uint16_t ep_buffer_offset[8] =
	{
		USB_EP0_OFFSET,
		USB_EP1_OFFSET,
		USB_EP2_OFFSET,
		USB_EP3_OFFSET,
		USB_EP4_OFFSET,
		USB_EP5_OFFSET,
		USB_EP6_OFFSET,
		USB_EP7_OFFSET
	};

#define USB_EP_BUF_CAPACITY(s) ((((s) <64)?((s)/2):(0x20 | ((s)/64)))<<10)

typedef struct _USBEndpoint USBEndpoint;

struct _USBEndpoint
{
	uint16_t status;
	uint8_t addr;
	uint8_t flags;
	USBBuffer *buffer;	/* NULL if no current buffer */
	struct process *event_process;
	uint16_t events;
	uint16_t xfer_size;
};

#define USB_EP_FLAGS_TYPE_MASK 0x03
#define USB_EP_FLAGS_TYPE_BULK 0x00
#define USB_EP_FLAGS_TYPE_CONTROL 0x01
#define USB_EP_FLAGS_TYPE_ISO 0x02
#define USB_EP_FLAGS_TYPE_INTERRUPT 0x03

#define IS_EP_TYPE(ep, type) (((ep)->flags & USB_EP_FLAGS_TYPE_MASK) == (type))
#define IS_CONTROL_EP(ep) IS_EP_TYPE(ep, USB_EP_FLAGS_TYPE_CONTROL)
#define IS_BULK_EP(ep) IS_EP_TYPE(ep, USB_EP_FLAGS_TYPE_BULK)
#define IS_INTERRUPT_EP(ep) IS_EP_TYPE(ep, USB_EP_FLAGS_TYPE_INTERRUPT)

#define USB_EP_FLAGS_ENABLED 0x04

/* A packet has been received but the data is still in hardware buffer */
#define USB_EP_FLAGS_RECV_PENDING 0x08
/* The pending packet is a SETUP packet */
#define USB_EP_FLAGS_SETUP_PENDING 0x10

/* The data in the hardware buffer is being transmitted */
#define USB_EP_FLAGS_TRANSMITTING 0x20

/* The  receiver is waiting for a packet */
#define USB_EP_FLAGS_RECEIVING 0x40

/* For bulk endpoints. Both buffers are busy are in use, either by
   hardware or software. */
#define USB_EP_FLAGS_DOUBLE 0x80

/* States for double buffered reception:

Packets being received	0	1	2	1	0	0
Packets pending		0	0	0	1	2	1

RECVING			0	1	1	1	0	0
RECV_PENDING		0	0	0	1	1	1
DOUBLE			0	0	1	0	1	0
*/

/* States for double buffered transmission:
   
Packets being transmitted	0	1	2

TRANSMITTING			0	1	1
DOUBLE				0	0	1
*/

/* Index in endpoint array */
#define EP_INDEX(addr) ((addr) & 0x7f)

/* Get address of endpoint struct */
#define EP_STRUCT(addr) (&usb_endpoints[EP_INDEX(addr)])

/* Number of hardware endpoint */
#define EP_HW_NUM(addr) ((addr) & 0x7f)

#define USB_DISABLE_INT \
  NVIC_DISABLE_INT(USB_LP_CAN_RX0_IRQChannel);\
  NVIC_DISABLE_INT(USB_HP_CAN_TX_IRQChannel)

#define USB_ENABLE_INT \
  NVIC_ENABLE_INT(USB_LP_CAN_RX0_IRQChannel);\
  NVIC_ENABLE_INT(USB_HP_CAN_TX_IRQChannel)

#define EPR_RW (USB_EP0R_EP_TYPE|USB_EP0R_EP_KIND|USB_EP0R_EA)
#define EPR_W0 (USB_EP0R_CTR_RX|USB_EP0R_CTR_TX)
#define EPR_TOGGLE (USB_EP0R_DTOG_RX | USB_EP0R_STAT_RX \
		    | USB_EP0R_DTOG_TX | USB_EP0R_STAT_TX)

#define EPR_INVARIANT(epr) ((epr & (EPR_RW)) | EPR_W0)

#define EPR_TOGGLE_SET(epr, mask, set) ((((epr) & (EPR_RW | (mask))) | EPR_W0) ^ (set))

#define USB_READ_BLOCK 0x01	/* The currently submitted buffers
				   can't hold the received data, wait
				   for more buffers. No data was read
				   from the hardware buffer */
#define USB_READ_NOTIFY 0x02	/* Some buffers that had the
				   USB_BUFFER_NOTIFY flags set were
				   released */
#define USB_READ_FAIL 0x04	/* The received data doesn't match the
				   submitted buffers. The hardware
				   buffer is discarded. */

#define USB_WRITE_BLOCK 0x01
#define USB_WRITE_NOTIFY 0x02
				   
static inline uint32_t usb_save_disable_int()
{
  uint32_t v = NVIC->ISER[0];
  NVIC->ICER[0] = (1<<USB_HP_CAN_TX_IRQChannel | 1<<USB_LP_CAN_RX0_IRQChannel);
  return v;
}

static inline void usb_restore_int(uint32_t v)
{
  NVIC->ISER[0] =
    v & (1<<USB_HP_CAN_TX_IRQChannel | 1<<USB_LP_CAN_RX0_IRQChannel);
}

static USBEndpoint usb_endpoints[USB_MAX_ENDPOINTS];
struct process *event_process = 0;
volatile unsigned int events = 0;

static void notify_process(unsigned int e);

static void notify_ep_process(USBEndpoint *ep, unsigned int e);

static void usb_arch_reset(void);

static void usb_arch_setup_endpoint(unsigned char addr);

inline void stall_bulk_in(unsigned int hw_ep)
{
	volatile uint32_t *eprp = &USB->EPR[hw_ep];
	*eprp = (*eprp & (EPR_RW | USB_EP0R_STAT_TX_1)) | EPR_W0;
	PRINTF("HALT IN\n");
}

inline void stall_bulk_out(unsigned int hw_ep)
{
	volatile uint32_t *eprp = &USB->EPR[hw_ep];
	*eprp = (*eprp & ((EPR_RW | USB_EP0R_STAT_RX_1) & ~USB_EP0R_CTR_RX)) |EPR_W0;
	PRINTF("HALT OUT\n");
}

inline unsigned int ep_capacity(unsigned int count)
{
	return (((count & USB_COUNT0_RX_NUM_BLOCK)>>10) * ((count & USB_COUNT0_RX_BLSIZE) ? 32 : 2));
}

/* Skip buffers until mask and flags matches*/
static USBBuffer *skip_buffers_until(USBBuffer *buffer, unsigned int mask, unsigned int flags, unsigned int *resp);

static void read_hw_buffer(USBBuffer *buffer, unsigned int offset, unsigned int len);

void write_hw_buffer(USBBuffer *buffer,unsigned int offset, unsigned int len);

static unsigned int get_receive_capacity(USBBuffer *buffer);

static int handle_pending_receive(USBEndpoint *ep);

static void start_receive(USBEndpoint *ep);

static unsigned int get_transmit_length(USBBuffer *buffer);

static int start_transmit(USBEndpoint *ep);

static void start_transfer(USBEndpoint *ep);

static void transfer_complete(unsigned int hw_ep);

void usb_set_ep_event_process(unsigned char addr, struct process *p);

unsigned int usb_get_ep_events(unsigned char addr);

void usb_submit_recv_buffer(unsigned char ep_addr, USBBuffer *buffer);

void usb_submit_xmit_buffer(unsigned char ep_addr, USBBuffer *buffer);

void USB_HP_CAN_TX_handler(void) __attribute__((interrupt));

void USB_HP_CAN_TX_handler(void);

void USB_LP_CAN_RX0_handler(void) __attribute__((interrupt));

void USB_LP_CAN_RX0_handler(void);

void usb_arch_toggle_SW_BUF_RX();

#endif /* ATSAM3S4_USB_ARCH_H_ */