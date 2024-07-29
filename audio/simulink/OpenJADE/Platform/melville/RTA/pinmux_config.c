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



#define UART0_RX_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<14))
#define UART0_TX_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<12))
#define UART1_RX_PORTA_MUX  ((uint32_t) ((uint32_t) 2<<28))
#define UART1_TX_PORTA_MUX  ((uint32_t) ((uint32_t) 2<<30))

#define UART0_RX_PORTA_FER  ((uint16_t) ((uint16_t) 1<<7))
#define UART0_TX_PORTA_FER  ((uint16_t) ((uint16_t) 1<<6))
#define UART1_RX_PORTA_FER  ((uint32_t) ((uint32_t) 1<<14))
#define UART1_TX_PORTA_FER  ((uint32_t) ((uint32_t) 1<<15))


#define LP0_ACK_PORTB_MUX  ((uint16_t) ((uint16_t) 2<<8))
#define LP0_CLK_PORTB_MUX  ((uint16_t) ((uint16_t) 0<<12))
#define LP0_D0_PORTB_MUX  ((uint16_t) ((uint16_t) 0<<14))
#define LP0_D1_PORTB_MUX  ((uint32_t) ((uint32_t) 0<<16))
#define LP0_D2_PORTB_MUX  ((uint32_t) ((uint32_t) 0<<18))
#define LP0_D3_PORTB_MUX  ((uint32_t) ((uint32_t) 0<<20))
#define LP0_D4_PORTB_MUX  ((uint32_t) ((uint32_t) 0<<22))
#define LP0_D5_PORTB_MUX  ((uint32_t) ((uint32_t) 0<<24))
#define LP0_D6_PORTB_MUX  ((uint32_t) ((uint32_t) 0<<26))
#define LP0_D7_PORTB_MUX  ((uint32_t) ((uint32_t) 0<<28))

#define LP0_ACK_PORTB_FER  ((uint16_t) ((uint16_t) 1<<4))
#define LP0_CLK_PORTB_FER  ((uint16_t) ((uint16_t) 1<<6))
#define LP0_D0_PORTB_FER  ((uint16_t) ((uint16_t) 1<<7))
#define LP0_D1_PORTB_FER  ((uint32_t) ((uint32_t) 1<<8))
#define LP0_D2_PORTB_FER  ((uint32_t) ((uint32_t) 1<<9))
#define LP0_D3_PORTB_FER  ((uint32_t) ((uint32_t) 1<<10))
#define LP0_D4_PORTB_FER  ((uint32_t) ((uint32_t) 1<<11))
#define LP0_D5_PORTB_FER  ((uint32_t) ((uint32_t) 1<<12))
#define LP0_D6_PORTB_FER  ((uint32_t) ((uint32_t) 1<<13))
#define LP0_D7_PORTB_FER  ((uint32_t) ((uint32_t) 1<<14))


/*
 * Initialize the Port Control MUX and FER Registers
 */
int32_t adi_initpinmux(void)
{
    /* PORTx_MUX registers */
    *pREG_PORTA_MUX = UART0_RX_PORTA_MUX | UART0_TX_PORTA_MUX | UART1_RX_PORTA_MUX | UART1_TX_PORTA_MUX;

    /* PORTx_FER registers */
    *pREG_PORTA_FER = UART0_RX_PORTA_FER | UART0_TX_PORTA_FER | UART1_RX_PORTA_FER | UART1_TX_PORTA_FER;

    /* Linkport MUX registers */
    *pREG_PORTB_MUX = LP0_ACK_PORTB_MUX | LP0_CLK_PORTB_MUX
     | LP0_D0_PORTB_MUX | LP0_D1_PORTB_MUX | LP0_D2_PORTB_MUX
     | LP0_D3_PORTB_MUX | LP0_D4_PORTB_MUX | LP0_D5_PORTB_MUX
     | LP0_D6_PORTB_MUX | LP0_D7_PORTB_MUX;

    *pREG_PORTB_FER = LP0_ACK_PORTB_FER | LP0_CLK_PORTB_FER
     | LP0_D0_PORTB_FER | LP0_D1_PORTB_FER | LP0_D2_PORTB_FER
     | LP0_D3_PORTB_FER | LP0_D4_PORTB_FER | LP0_D5_PORTB_FER
     | LP0_D6_PORTB_FER | LP0_D7_PORTB_FER;


    return 0;
}

