/*
 **
 ** Source file generated on October 9, 2018 at 18:42:14.	
 **
 ** Copyright (C) 2011-2018 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in 
 ** the Pin Multiplexing configuration editor. Changes to the Pin Multiplexing
 ** configuration should be made by changing the appropriate options rather
 ** than editing this file.
 **
 ** Selected Peripherals
 ** --------------------
 ** TWI2 (SCL, SDA)
 **
 ** GPIO (unavailable)
 ** ------------------
 ** PA14, PA15
 */

#include <sys/platform.h>
#include <stdint.h>
#include "gul_melville.h"

#define TWI1_SCL_PORTA_MUX  ((uint32_t) ((uint32_t) 1<<00))
#define TWI1_SDA_PORTA_MUX  ((uint32_t) ((uint32_t) 1<<02))

#define TWI1_SCL_PORTA_FER  ((uint32_t) ((uint32_t) 1<<0))
#define TWI1_SDA_PORTA_FER  ((uint32_t) ((uint32_t) 1<<1))

#define TWI2_SCL_PORTA_MUX  ((uint32_t) ((uint32_t) 0<<28))
#define TWI2_SDA_PORTA_MUX  ((uint32_t) ((uint32_t) 0<<30))

#define TWI2_SCL_PORTA_FER  ((uint32_t) ((uint32_t) 1<<14))
#define TWI2_SDA_PORTA_FER  ((uint32_t) ((uint32_t) 1<<15))

#define UART0_RX_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<14))
#define UART0_TX_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<12))
#define UART1_RX_PORTA_MUX  ((uint32_t) ((uint32_t) 2<<28))
#define UART1_TX_PORTA_MUX  ((uint32_t) ((uint32_t) 2<<30))

#define UART0_RX_PORTA_FER  ((uint16_t) ((uint16_t) 1<<7))
#define UART0_TX_PORTA_FER  ((uint16_t) ((uint16_t) 1<<6))
#define UART1_RX_PORTA_FER  ((uint32_t) ((uint32_t) 1<<14))
#define UART1_TX_PORTA_FER  ((uint32_t) ((uint32_t) 1<<15))


extern int32_t adi_initpinmux(void);

/*
 * Initialize the Port Control MUX and FER Registers
 */
int32_t adi_initpinmux(void)
{
#if TWIDEVNUM == 1
    /* PORTx_MUX registers */
    *pREG_PORTB_MUX = TWI1_SCL_PORTA_MUX | TWI1_SDA_PORTA_MUX;

    /* PORTx_FER registers */
    *pREG_PORTB_FER = TWI1_SCL_PORTA_FER | TWI1_SDA_PORTA_FER;
#elif TWIDEVNUM == 2
    /* PORTx_MUX registers */
    *pREG_PORTA_MUX = TWI2_SCL_PORTA_MUX | TWI2_SDA_PORTA_MUX;

    /* PORTx_FER registers */
    *pREG_PORTA_FER = TWI2_SCL_PORTA_FER | TWI2_SDA_PORTA_FER;
#endif

    /* PORTx_MUX registers */
    *pREG_PORTA_MUX = UART0_RX_PORTA_MUX | UART0_TX_PORTA_MUX | UART1_RX_PORTA_MUX | UART1_TX_PORTA_MUX;

    /* PORTx_FER registers */
    *pREG_PORTA_FER = UART0_RX_PORTA_FER | UART0_TX_PORTA_FER | UART1_RX_PORTA_FER | UART1_TX_PORTA_FER;

    return 0;
}

