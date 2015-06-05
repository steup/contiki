/**
 * @file pal_config.h
 *
 * @brief PAL configuration for SAM3_RFEK01
 *
 * This header file contains configuration parameters for SAM3_RFEK01.
 *
 * $Id: pal_config.h 29480 2011-11-23 12:04:21Z mahendran.p $
 *
 */
/**
 * @author
 * Atmel Corporation: http://www.atmel.com
 * Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* Prevent double inclusion */


#ifndef PAL_CONFIG_H
#define PAL_CONFIG_H

/* === Includes ==============================================================*/
#include "sam3s4.h"
#include "pal_types.h"
#include "pal_boardtypes.h"
#include "vendor_boardtypes.h"
#include "armtypes.h"
#include "pal_internal.h"
#include "return_val.h"

/*
 * If ranging is enabled define the distance offset for this board in cm.
 * The valid range is 0...255 (cm).
 */
#ifdef ENABLE_RTB
#define DISTANCE_OFFSET                 (100)
#if ((DISTANCE_OFFSET < 0) || (DISTANCE_OFFSET > 255))
#   error "Invalid Distance Offset"
#endif

/* Default Ranging Transmit Power is set to -17dBm. */
#define RTB_TRANSMIT_POWER_DEFAULT      (0xAF)
#endif  /* ENABLE_RTB */

#if (BOARD_TYPE == deRFsam3_13M10)
/* === Types =================================================================*/

/*
 * Indentifiers for PIO's in AT91SAM3S4B
 */
typedef enum pio_type_tag
{
    PIO_A,
    PIO_B,
    PIO_C
} pio_type_t_t;

/* Enumerations used to identify LEDs */
typedef enum led_id_tag
{
    LED_0,
    LED_1,
    LED_2
} led_id_t;

#define NO_OF_LEDS                      (3)

/* Enumerations used to identify buttons */
typedef enum button_id_tag
{
    BUTTON_0
} button_id_t;

#define NO_OF_BUTTONS                   (0)

/* === Externals =============================================================*/


/* === Macros ================================================================*/

/* The CPU clock */
/** Frequency of the board main oscillator */
#define BOARD_MAINOSC                  (18432000)

#define F_CPU                          (32000000)

/**
 * This board does not support antenna diversity.
 */
#define ANTENNA_DIVERSITY               (0)

#define TRX_INT_PIN   {1 << 1, PIOA, ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_IT_RISE_EDGE}
#define TRX_RXTS_PIN  {1 << 15, PIOA, ID_PIOA, PIO_INPUT, PIO_DEGLITCH | PIO_IT_RISE_EDGE}

/** Base address of SPI peripheral connected to the RF TRX. */
#define BOARD_TRX_SPI_BASE         SPI
/** Identifier of SPI peripheral connected to the transceiver. */
#define BOARD_TRX_SPI_ID           ID_SPI
/** Pins of the SPI peripheral connected to the transceiver. */
#define BOARD_TRX_SPI_PINS         PINS_SPI
/** Chip select connected to the transceiver. */
#define BOARD_TRX_NPCS             2
/** Chip select pin connected to the touchscreen controller. */
#define BOARD_TRX_NPCS_PIN         PIN_SPI_NPCS0_PA11

/** SPI MISO pin definition. */
#define PIN_SPI_MISO  {1 << 12, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI MOSI pin definition. */
#define PIN_SPI_MOSI  {1 << 13, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI SPCK pin definition. */
#define PIN_SPI_SPCK  {1 << 14, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** SPI chip select pin definition. */
#define PIN_SPI_NPCS0_PA11  {1 << 11, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** List of SPI pin definitions (MISO, MOSI & SPCK). */
//#define PINS_SPI      PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SPCK

/** USB attributes configuration descriptor (bus or self powered, remote wakeup) */
#define BOARD_USB_BMATTRIBUTES USBConfigurationDescriptor_SELFPOWERED_NORWAKEUP

/** Number of USB endpoints */
#define CHIP_USB_NUMENDPOINTS 8

#define CHIP_USB_ENDPOINTS_MAXPACKETSIZE(i) \
    ((i == 0) ? 64 : \
     ((i == 1) ? 64 : \
      ((i == 2) ? 64 : \
       ((i == 3) ? 64 : \
        ((i == 4) ? 512 : \
         ((i == 5) ? 512 : \
          ((i == 6) ? 64 : \
           ((i == 7) ? 64 : 0 ))))))))

/** Endpoints Number of Bank */
#define CHIP_USB_ENDPOINTS_BANKS(i) \
    ((i == 0) ? 1 : \
     ((i == 1) ? 2 : \
      ((i == 2) ? 2 : \
       ((i == 3) ? 1 : \
        ((i == 4) ? 2 : \
         ((i == 5) ? 2 : \
          ((i == 6) ? 2 : \
           ((i == 7) ? 2 : 0 ))))))))

#if (F_CPU == 32000000UL)

/* SPI baud rate divider to generate 4MHz SPI clock, when F_CPU is 32MHz. */
#define SPI_BR_DIVIDER                  (8)

#elif (F_CPU == 48000000UL)

/* SPI baud rate divider to generate 4MHz SPI clock, when F_CPU is 48MHz. */
#define SPI_BR_DIVIDER                  (12)

#else
#error "unsupported CPU clock"
#endif

/*
 * This board does NOT have an external eeprom available.
 */
#ifndef EXTERN_EEPROM_AVAILABLE
#define EXTERN_EEPROM_AVAILABLE            (0)
#endif

/*
 * Macros dealing with the PMC of AT91SAM3S4B
 */

/*
 * Enables the clock to the given peripheral id
 */
#define PERIPHERAL_CLOCK_ENABLE(id)     (pal_peripheral_clock_enable(id))

/*
 * Disables the clock to the given peripheral id
 */
#define PERIPHERAL_CLOCK_DISABLE(id)    (pal_peripheral_clock_disable(id))



/*
 * SPI Base Register
 */
#define SPI_USED             (SPI)


/*
 * Macros dealing with the PIO of AT91SAM3S4B
 */

/*
 * Enables the PIO control for the requested pin and sets it as output
 */

#define PIN_SET_AS_PIO_OUTPUT(pin, pio) do {                            \
        Pio *pio_ptr;                                                       \
        if (PIO_A == pio)                                                   \
        {                                                                   \
            pio_ptr = PIOA;                                                 \
        }                                                                   \
        else                                                               \
        {                                                                   \
            pio_ptr = PIOB;                                    			  \
        }                                                                  \
        pio_ptr->PIO_OER = pin;                                             \
        pio_ptr->PIO_PER = pin;                                             \
    } while (0);

/*
 * Enables the PIO control for the requested pin and sets it as input
 */
#define PIN_SET_AS_PIO_INPUT(pin, pio) do {                             \
        Pio *pio_ptr;                                                       \
        if (PIO_A == pio)                                                   \
        {                                                                   \
            pio_ptr = PIOA;                                                 \
        }                                                                   \
        else				                                               \
        {                                                                   \
            pio_ptr = PIOB;                                                 \
        }                                                                   \
        pio_ptr->PIO_ODR = pin;                                             \
        pio_ptr->PIO_PER = pin;                                             \
    } while (0);

/*
 * Sets the pin of requested PIO
 */
#define PIN_SET(pin, pio)   do {    \
        Pio *pio_ptr;                   \
        if (PIO_A == pio)               \
        {                               \
            pio_ptr = PIOA;             \
        }                               \
        else				          \
        {                               \
            pio_ptr = PIOB;             \
        }                               \
        pio_ptr->PIO_SODR = pin;        \
    } while (0);

/*
 * Clears the pin of requested PIO
 */
#define PIN_CLEAR(pin, pio) do {    	\
        Pio *pio_ptr;                   \
        if (PIO_A == pio)               \
        {                               \
            pio_ptr = PIOA;             \
        }                               \
        else                            \
        {                               \
            pio_ptr = PIOB;  			\
        }                               \
        pio_ptr->PIO_CODR = pin;        \
    } while (0);

/*
 * Gets the status of the requested pin of PIO
 */
#define PIN_GET_STATUS(status, pin, pio) do {               \
        Pio *pio_ptr;                                           \
        if (PIO_A == pio)                                       \
        {                                                       \
            pio_ptr = PIOA;                                     \
        }                                                       \
        else				                                   \
        {                                                       \
            pio_ptr = PIOB;                                     \
        }                                                       \
        status = (pio_ptr->PIO_PDSR & pin ? true : false);      \
    } while (0);

/* Enables the transceiver interrupts. */
#define ENABLE_TRX_IRQ() do    {                             	\
        (pal_pio_enable_interrupt(&(const pin_t)TRX_INT_PIN));	\
    } while(0);

/* Disables the transceiver interrupts. */
#define DISABLE_TRX_IRQ() do {                             		\
        (pal_pio_disable_interrupt(&(const pin_t)TRX_INT_PIN));	\
    }while(0);

/* Clears the transceiver interrupts. */
#define    CLEAR_TRX_IRQ()    do{\
        (NVIC_ClearPendingIRQ((IRQn_Type)ID_PIOA));       \
    }while(0);

/* Enables the transceiver interrupts. */
#define ENABLE_TRX_IRQ_TSTAMP() do{                        \
        (pal_pio_enable_interrupt(&(const pin_t)TRX_RXTS_PIN));          \
    } while(0);

#define DISABLE_TRX_IRQ_TSTAMP() do {                      \
        pal_pio_disable_interrupt(&(const pin_t)TRX_RXTS_PIN);           \
    } while (0);


#define CLEAR_TRX_IRQ_TSTAMP() do{                         \
        (NVIC_ClearPendingIRQ((IRQn_Type)ID_PIOA));        \
    } while (0);


/*
 * Macros dealing with the global interrupt (IRQ bit) of AT91SAM7S256.
 */

/* Enables the global interrupt. */
#define ENABLE_GLOBAL_IRQ()             sei()

/* Disables the global interrupt. */
#define DISABLE_GLOBAL_IRQ()            cli()

/*
 * This macro is used to mark the start of a critical region. It saves the
 * current status register and then disables the interrupt.
 */
#define ENTER_CRITICAL_REGION()         \
    {uint32_t sreg; GET_CPSR(sreg); DISABLE_GLOBAL_IRQ()

/*
 * This macro is used to mark the end of a critical region. It restores the
 * current status register.
 */
#define LEAVE_CRITICAL_REGION()         SET_CPSR(sreg);}

/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION() {uint32_t irq_mask = PIOA->PIO_IMR; \
        DISABLE_TRX_IRQ() DISABLE_TRX_IRQ_TSTAMP()

/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION()  PIOA->PIO_IER = irq_mask; }


/*
 * GPIO macros for AT91SAM3S4B
 */

/*
 * This board uses an SPI-attached transceiver.
 */
#define PAL_USE_SPI_TRX                 (1)

/* RESET pin is pin 24 of PIOA. */
#define RST                             (PIO_PA16)

/* Sleep Transceiver pin is pin 21 of PIOA. */
#define SLP_TR                          (PIO_PA17)

/*
 * Slave select pin is pin 11 PA11
 */
#define SEL                             (PIO_PA11A_NPCS0)

/*
 * SPI Bus Master Output/Slave Input pin is pin 22 is PA13
 */
#define MOSI                            (PIO_PA13A_MOSI)

/*
 * SPI Bus Master Input/Slave Output pin is pin 27 is PA12
 */
#define MISO                            (PIO_PA12A_MISO)

/*
 * SPI serial clock pin is pin 21 is PA14
 */
#define SCK                             (PIO_PA14A_SPCK)

#define IRQ_PINGET()                             (PIO_PA1)

/*
 * Set TRX GPIO pins.
 */
#define RST_HIGH()                      {PIN_SET(RST, PIO_A)}
#define RST_LOW()                       {PIN_CLEAR(RST, PIO_A)}
#define SLP_TR_HIGH()                   {PIN_SET(SLP_TR, PIO_A)}
#define SLP_TR_LOW()                    {PIN_CLEAR(SLP_TR, PIO_A)}


/*
 * Timer macros for AT91SAM3S4
 */

/*
 * These macros are placeholders for delay functions for high speed processors.
 *
 * The following delay are not reasonbly implemented via delay functions,
 * but rather via a certain number of NOP operations.
 * The actual number of NOPs for each delay is fully MCU and frequency
 * dependent, so it needs to be updated for each board configuration.
 *
 * AT91SAM7S256 @ FPU
 */
#if (F_CPU == 32000000UL)
/* 32 MHz */
/*
 * Wait for 65 ns.
 * time t7: SLP_TR time (see data sheet or SWPM).
 * In case the system clock is 32 MHz we need to have this value filled,
 * otherwise frame transmission may not be initiated properly.
 */
#define PAL_WAIT_65_NS()                {nop(); nop();}

/* Wait for 500 ns. */
#define PAL_WAIT_500_NS()               {nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop();}
/* Wait for 1 us. */
#define PAL_WAIT_1_US()                 {nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop();}
#elif (F_CPU == 48000000UL)
/* 48 MHz */
/*
 * Wait for 65 ns.
 * time t7: SLP_TR time (see data sheet or SWPM).
 * In case the system clock is 48 MHz we need to have this value filled,
 * otherwise frame transmission may not be initiated properly.
 */
#define PAL_WAIT_65_NS()                {nop(); nop();nop(); nop();}

/* Wait for 500 ns. */
#define PAL_WAIT_500_NS()               {nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop();}
/* Wait for 1 us. */
#define PAL_WAIT_1_US()                 {nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop(); \
        nop(); nop(); nop(); nop();}
#endif

/*
 * The smallest timeout in microseconds
 */
#define MIN_TIMEOUT                     (0x80)

/*
 * The largest timeout in microseconds
 */
#define MAX_TIMEOUT                     (0x7FFFFFFF)

/*
 * Settings to give clocking to timer when radio is awake
 *
 */
#define TIMER_SRC_DURING_TRX_AWAKE()

/*
 * Settings to give clocking to timer when radio is sleeping
 *
 */
#define TIMER_SRC_DURING_TRX_SLEEP()

/*
 * Maximum numbers of software timers running at a time
 */
#define MAX_NO_OF_TIMERS                (25)
#if (MAX_NO_OF_TIMERS > 255)
#error "MAX_NO_OF_TIMERS must not be larger than 255"
#endif

/* If this macro is defined then the upper 16-bit of the timestamp value
is taken from sys_time which is a SW count incremented whenever lower 16bit
overflow happens */
#define TIMESTAMP_UPPER_16BIT_SW     (1)
/*
 * Hardware register that holds Rx timestamp
 */
#define TIME_STAMP_LOW_REGISTER      (TC0->TC_CHANNEL[0].TC_CV)
#define TIMER_LOW_REGISTER           (TC0->TC_CHANNEL[0].TC_CV)

/* Macros for PAL Normal Timer Register */
#define PAL_TIMER_CH                 (TC0->TC_CHANNEL[0])
#define PAL_TIMER_IRQ_ID             (TC0_IRQn)
#define PAL_TIMER_PERIPH_ID          (ID_TC0)
#define PAL_TIMER_CCR_EN             (TC_CCR_CLKEN)
#define PAL_TIMER_IER_FLAG           (TC_IER_CPCS)
#define PAL_TIMER_IDR_FLAG           (TC_IDR_CPCS)
#define PAL_TIMER_SR_FLAG            (TC_SR_CPCS)
#define PAL_TIMER_SR_OVERFLOW_FLAG   (TC_SR_COVFS)
#define PAL_TIMER_STATUS_REG         (PAL_TIMER_CH.TC_SR)
#define PAL_TIMER_IER                (PAL_TIMER_CH.TC_IER)
#define PAL_TIMER_IDR                (PAL_TIMER_CH.TC_IDR)
#define PAL_TIMER_COMP_REG           (PAL_TIMER_CH.TC_RC)
#define PAL_TIMER_CCR                (PAL_TIMER_CH.TC_CCR)
//#define PAL_TIMER_IRQ                 void TC0_IrqHandler(void)
#define PAL_TIMER_IRQ                 void TC0_Handler(void)

/* Macros for PAL High Priority Timer Register */
#define PAL_HIGHPRI_TMR_CH            (TC0->TC_CHANNEL[2])
#define PAL_HIGHPRIO_TIMER_IRQ_ID     (TC2_IRQn)
#define PAL_HIGHPRIO_TIMER_PERIPH_ID  (ID_TC2)
#define PAL_HIGHPRIO_TIMER_CCR_EN     (TC_CCR2_CLKEN)
#define PAL_HIGHPRIO_TIMER_IER_FLAG   (TC_IER2_CPCS)
#define PAL_HIGHPRIO_TIMER_IDR_FLAG   (TC_IDR2_CPCS)
#define PAL_HIGHPRIO_TIMER_STATUS_REG (PAL_HIGHPRI_TMR_CH.TC_SR)
#define PAL_HIGHPRIO_TIMER_IER        (PAL_HIGHPRI_TMR_CH.TC_IER)
#define PAL_HIGHPRIO_TIMER_IDR        (PAL_HIGHPRI_TMR_CH.TC_IDR)
#define PAL_HIGHPRIO_TIMER_COMP_REG   (PAL_HIGHPRI_TMR_CH.TC_RC)
#define PAL_HIGHPRIO_TIMER_CCR        (PAL_HIGHPRI_TMR_CH.TC_CCR)
#define PAL_HIGHPRIO_TIMER_IRQ         void TC2_IrqHandler(void)

/*
 * TRX Access macros for AT91SAM3S4
 */

/*
 * Position of the PCS (peripheral chip select) field in the SPI_MR register.
 */
#define PCS_FIELD_IN_MR                 (16)

/*
 * Value that needs to be written to in the PCS field of the SPI_MR to
 * activate the line to which the trx select line is connected.
 */
#define PCS_FIELD_VALUE                 (3)

/*
 * Value of PCS field in SPI_MR (mode register) which indicates the contoller
 * about the line to which the slave is connected.
 */
#define SS_ENABLE                       (PCS_FIELD_VALUE << PCS_FIELD_IN_MR)

/*
 * Slave select made low
 */
#define SS_LOW()                        PIN_CLEAR(SEL, PIO_A)

/*
 * Slave select made high
 */
#define SS_HIGH()                       PIN_SET(SEL, PIO_A)

/*
 * Dummy value to be written in SPI transmit register to retrieve data form it
 */
#define SPI_DUMMY_VALUE                 (0x00)

/* Reads the data from the SPI receive register. */
#define SPI_READ(register_value)    do {                        \
        while ((SPI->SPI_SR & SPI_SR_RDRF) == 0);    \
        register_value = (SPI->SPI_RDR & 0xFFFF);       \
    } while (0);

/* Writes the data into the SPI transmit register. */
#define SPI_WRITE(data)     do {                                    \
        while ((SPI->SPI_SR & SPI_SR_TDRE) == 0);        \
        SPI->SPI_TDR = data & 0xFFFF;                       \
        while ((SPI->SPI_SR & SPI_SR_TXEMPTY) == 0);     \
    } while (0);


/*
 * LED pins on the AT91SAM3S4A board
 */
#define LED_PINS                (PIO_PB1 | PIO_PA19 | PIO_PA18)


/**
 * This board use the timestamp interrupt, since the DIG2 pin from the
 * the transceiver is connected to the MCU.
 */

#ifndef DISABLE_TSTAMP_IRQ
#define DISABLE_TSTAMP_IRQ              (0)
#endif


/*
 * Value of an external PA gain
 * If no external PA is available, value is 0.
 */
#define EXTERN_PA_GAIN                  (0)

/*
 * Alert initialization
 */
#define ALERT_INIT()        {PIN_SET_AS_PIO_OUTPUT((LED_PINS), PIO_A);}

/*
 * Alert indication
 */
#define ALERT_INDICATE()    do                      \
    {                                                   \
        bool pin_status;                                \
        PIN_GET_STATUS(pin_status, LED_PINS, PIO_A);    \
        if (pin_status)                                 \
        {                                               \
            PIN_CLEAR(LED_PINS, PIO_A);                 \
        }                                               \
        else                                            \
        {                                               \
            PIN_SET(LED_PINS, PIO_A);                   \
        }                                               \
    } while (0);


/*
 * ADC Initialization values
 */
/*
 * Number of ADC conversions to be done during generation of random number.
 * Since a 16-bit value is needed and 4 ADC channels are used
 * to get 1 single bit (always the LSB), 4 subsequent conversions are required.
 */
#define BOARD_ADC_FREQ                  (6000000)

#define NO_OF_CONVERSIONS               (16)

/* Value of the ADC clock in Hz */
#define USED_ADC_CLOCK_FREQ             (6000000)
/* Value of MCK in Hz */
#define USED_MCK_CLOCK_FREQ             (F_CPU)

#define ADC_STARTUP_TIME_NS             (10)        /* In us */
#define ADC_SAMPLE_AND_HOLD_TIME_US     (1200)      /* In ns */

/* Defines for the ADC Mode register */
#define ADC_PRESCAL     ((USED_MCK_CLOCK_FREQ / (2 * USED_ADC_CLOCK_FREQ)) - 1)
#define ADC_STARTUP     (((USED_ADC_CLOCK_FREQ / 1000000) * ADC_STARTUP_TIME_NS / 8) - 1)
#define ADC_SHTIM       ((((USED_ADC_CLOCK_FREQ / 1000000) * ADC_SAMPLE_AND_HOLD_TIME_US) / 1000) - 1)

#define ADC_NUM_1                       (4)

/** Internal SRAM address */
#define AT91C_ISRAM                   AT91C_IRAM
/** Internal SRAM size */
#define AT91C_ISRAM_SIZE              0x00008000

/** Internal Flash size */
//#define AT91C_IFLASH_SIZE               (0x40000)
/** Internal Flash page size */
//#define AT91C_IFLASH_PAGE_SIZE              (256) /* Internal FLASH 0 Page Size: 256 bytes */
/** Internal Flash number of pages */
#define AT91C_IFLASH_NB_OF_PAGES           (1024) /* Internal FLASH 0 Number of Pages: 512 */
/** Internal Flash lock region size */
#define AT91C_IFLASH_LOCK_REGION_SIZE     (16384) /* Internal FLASH 0 Lock Region Size: 16 Kbytes */
/** Internal Flash number of lock bits */
#define AT91C_IFLASH_NB_OF_LOCK_BITS         (16) /* Internal FLASH 0 Number of Lock Bits: 16 */


#define FLASH_PAL_ADDRESS   IFLASH_ADDR + IFLASH_SIZE - IFLASH_PAGE_SIZE

#define FLASH_PAL_PAGENUM    1023

/* === Prototypes ===========================================================*/
void LowLevelInit(void);
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

#endif  /* PAL_CONFIG_H */
/* EOF */
