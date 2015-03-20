#include "debug-uart.h"

volatile unsigned char dma_running = 0;

static unsigned char * volatile dma_end;

static unsigned char dbg_write_overrun = 0;

void dbg_setup_uart_default()
{
	RCC->APB2ENR |=  (RCC_APB2ENR_AFIOEN
			| RCC_APB2ENR_IOPAEN 
			| RCC_APB2ENR_IOPBEN
			| RCC_APB2ENR_USART1EN );
			
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	AFIO_REMAP( AFIO_MAPR_USART1_REMAP, AFIO_MAPR_USART1_REMAP);
	GPIO_CONF_OUTPUT_PORT(B,6,ALT_PUSH_PULL,50);
	GPIO_CONF_INPUT_PORT(B,7,FLOATING);

	USART1->CR1 = USART_CR1_UE;

	USART1->CR2 = 0;
	USART1->CR3 = USART_CR3_DMAT;
	USART1->CR1 |= USART_CR1_TE;
	USART1->BRR= 0x1a1;
}

static void update_dma(void)
{
	if (xmit_buffer_tail == xmit_buffer_head)
	{
		return;
	}
	
	DBG_DMA_CHANNEL->CCR = (DMA_Priority_Low 
		| DMA_PeripheralDataSize_Byte
		| DMA_MemoryDataSize_Byte
		| DMA_PeripheralInc_Disable
		| DMA_MemoryInc_Enable
		| DMA_Mode_Normal
		| DMA_DIR_PeripheralDST
		| DMA_CCR4_TCIE
		);
		
	DBG_DMA_CHANNEL->CPAR = (u32)&DBG_UART->DR;
	DBG_DMA_CHANNEL->CMAR = (u32)xmit_buffer_head;
	
	if (xmit_buffer_head < xmit_buffer_tail)
	{
		DBG_DMA_CHANNEL->CNDTR = xmit_buffer_tail - xmit_buffer_head;
		dma_end = xmit_buffer_tail;    
	}
	else
	{
		DBG_DMA_CHANNEL->CNDTR = XMIT_BUFFER_END - xmit_buffer_head;
		dma_end = xmit_buffer;
	}
	
	NVIC_ENABLE_INT(DMA1_Channel4_IRQChannel);
	NVIC_SET_PRIORITY(DMA1_Channel4_IRQChannel, 2);
	DBG_DMA_CHANNEL->CCR |= DMA_CCR4_EN;
}

void DMA1_Channel4_handler()
{
	DBG_DMA->IFCR = DBG_DMA_CHANNEL_IFCR_CGIF;
	xmit_buffer_head = dma_end;
	
	if (xmit_buffer_tail == xmit_buffer_head)
	{
		dma_running = 0;
		return;
	}
	
	update_dma();
}

unsigned int dbg_send_bytes(const unsigned char *seq, unsigned int len)
{
	/* Since each of the pointers should be read atomically
	there's no need to disable interrupts */
	
	unsigned char *head = xmit_buffer_head;
	unsigned char *tail = xmit_buffer_tail;
	
	if (tail >= head)
	{
		// Free space wraps
		//unsigned int xfer_len = XMIT_BUFFER_END - tail;
		unsigned int xfer_len = DBG_XMIT_BUFFER_LEN;
		unsigned int free = DBG_XMIT_BUFFER_LEN - (tail - head) - 1;

		if (len > free)
		{
			len = free;
		}
		
		if (xfer_len < len)
		{
			memcpy(tail, seq, xfer_len);
			
			seq += xfer_len;
			xfer_len = len - xfer_len;
			memcpy(xmit_buffer, seq, xfer_len);
			tail = xmit_buffer + xfer_len;
		}
		else
		{
			memcpy(tail, seq, len);
			tail += len;
			
			if (tail == XMIT_BUFFER_END)
			{
				tail = xmit_buffer;
			}
		}
	}
	else
	{
		// Free space continuous
		unsigned int free = (head - tail) - 1;
		
		if (len > free)
		{
			len = free;
		}
		
		memcpy(tail, seq, len);
		tail += len;
	}
	
	xmit_buffer_tail = tail;
	
	if (!dma_running)
	{
		dma_running = 1;
		update_dma();
	}
	
	return len;
}

void dbg_putchar(const char ch)
{
	if (dbg_write_overrun)
	{
		if (dbg_send_bytes((const unsigned char*)"^",1) != 1)
		{
			return;
		}
	}
	
	dbg_write_overrun = 0;
	
	if (dbg_send_bytes((const unsigned char*)&ch,1) != 1)
	{
		dbg_write_overrun = 1;
	}
}

void dbg_blocking_putchar(const char ch)
{
	if (dbg_write_overrun)
	{
		while (dbg_send_bytes((const unsigned char*)"^",1) != 1);
	}
	
	dbg_write_overrun = 0;
	
	while (dbg_send_bytes((const unsigned char*)&ch,1) != 1);
}

void dbg_drain()
{
	while(xmit_buffer_tail != xmit_buffer_head);
}
