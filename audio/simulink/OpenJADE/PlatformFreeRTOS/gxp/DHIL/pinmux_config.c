/*
 **
 ** Source file generated on August 18, 2021 at 14:07:08.	
 **
 ** Copyright (C) 2011-2021 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in 
 ** the Pin Multiplexing configuration editor. Changes to the Pin Multiplexing
 ** configuration should be made by changing the appropriate options rather
 ** than editing this file.
 **
 ** Selected Peripherals
 ** --------------------
 ** TWI2 (SCL, SDA)
 ** UART0 (CTS, RTS, RX, TX)
 **
 ** GPIO (unavailable)
 ** ------------------
 ** PA06, PA07, PA08, PA09, PA14, PA15
 */

#include <sys/platform.h>
#include <stdint.h>

#define TWI2_SCL_PORTA_MUX  ((uint32_t) ((uint32_t) 0<<28))
#define TWI2_SDA_PORTA_MUX  ((uint32_t) ((uint32_t) 0<<30))
#define UART0_CTS_PORTA_MUX  ((uint32_t) ((uint32_t) 1<<18))
#define UART0_RTS_PORTA_MUX  ((uint32_t) ((uint32_t) 1<<16))
#define UART0_RX_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<14))
#define UART0_TX_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<12))

#define TWI2_SCL_PORTA_FER  ((uint32_t) ((uint32_t) 1<<14))
#define TWI2_SDA_PORTA_FER  ((uint32_t) ((uint32_t) 1<<15))
#define UART0_CTS_PORTA_FER  ((uint32_t) ((uint32_t) 1<<9))
#define UART0_RTS_PORTA_FER  ((uint32_t) ((uint32_t) 1<<8))
#define UART0_RX_PORTA_FER  ((uint16_t) ((uint16_t) 1<<7))
#define UART0_TX_PORTA_FER  ((uint16_t) ((uint16_t) 1<<6))

int32_t adi_initpinmux(void);

/*
 * Initialize the Port Control MUX and FER Registers
 */
int32_t adi_initpinmux(void) {
    /* PORTx_MUX registers */
    *pREG_PORTA_MUX = TWI2_SCL_PORTA_MUX | TWI2_SDA_PORTA_MUX
     | UART0_CTS_PORTA_MUX | UART0_RTS_PORTA_MUX | UART0_RX_PORTA_MUX
     | UART0_TX_PORTA_MUX;

    /* PORTx_FER registers */
    *pREG_PORTA_FER = TWI2_SCL_PORTA_FER | TWI2_SDA_PORTA_FER
     | UART0_CTS_PORTA_FER | UART0_RTS_PORTA_FER | UART0_RX_PORTA_FER
     | UART0_TX_PORTA_FER;
    return 0;
}
