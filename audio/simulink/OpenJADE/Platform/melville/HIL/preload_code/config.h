/******************************************************************************

Copyright(c) 2018 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*******************************************************************************/

#ifndef ADSP2156X_INIT_CONFIG_H
#define ADSP2156X_INIT_CONFIG_H

#include "mem_types.h"

/* Define any non-default configuration macros here
 * and then delete the #error.
 */
/*Common macros*/
#define CLKIN   (25000000)               /*!< SYS_CLKIN value*/
#define CONFIG_DMC0 (ISSI_8Gb_DDR3_533MHZ) /*!< Define as 0, if there is no DDR on the board or the chip*/
#define CONFIG_SPIFLASH (1)              /*!< Define as 0, if there is no SPI Flash on the board or the chip*/
#define EXECUTE_SAFE_STATE (1)           /*!< Define as 1, if the program must go in to a safe state up on errors in the API calls*/
/*Boot related macros*/
#define CONFIG_BOOT_UART_BAUD_RATE (0)   /*!< Define as 1, if the UART Baud Rate need to be recalculated and reinitialized*/
#define CONFIG_BOOT_SPI_CLOCK_RATE (0)   /*!< Define as 1, if the SPI Clock Rate need to be recalculated and reinitialized*/
#define CONFIG_BOOT_OSPI_CLOCK_RATE (0)  /*!< Define as 1, if the OSPI Clock Rate need to be recalculated and reinitialized*/
#define DO_PROCESS_BOOTSTRUCT (0)        /*!< Define as 1, if there is need to process the boot struct passed by ROM*/

#endif /* ADSP2156X_INIT_CONFIG_H */
