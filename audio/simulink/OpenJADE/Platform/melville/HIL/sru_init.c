 /********************************************************************************
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 * BOSE CORPORATION
 * The Mountain
 * Framingham, MA 01701-9168
 *
 ****************************************************************************/
/**
 *  @file sru_init.c
 *
 *  @brief Contains SRU connections linked to the board hardware
 *
 *  @ingroup platform
 */
 

/***  Include Files ***********************************************************/

#include "sru.h"

/***  External Variable References  *******************************************/

/***  Module Macros  **********************************************************/

#define DAI0(a,b)    SRU (a,b)
#define DAI1(a,b)    SRU2(a,b)

/***  Module Types  ***********************************************************/

/***  Module Variables  *******************************************************/

/***  Module Function Prototypes  *********************************************/
static void Clear_DAI_Pins( void );

/***  Module Functions  *******************************************************/


/**
 *  @brief  Configures PCG and DAI peripherals to generate all necessary
 *          clocks and data connections. SPORTs are configured by the SHARCs.
 *
 *  @pre    None
 *
 *  @param  None
 *
 *  @post   Modifies PCG and DAI pin settings.
 *
 *  @return None
 *
 */
void Init_SRU ( void )
{
   /*
    * Initialize unused DAI0 & DAI1 pins
    */
   Clear_DAI_Pins ( ) ;

#if defined(__EZKIT_21569__)
   DAI1(LOW,DAI1_PBEN05_I);

   DAI1(DAI1_PB05_O,SPT4_ACLK_I); /*DAC clock to SPORT 4A*/
   DAI1(DAI1_PB05_O,SPT4_BCLK_I); /*DAC clock to SPORT 4B*/

   DAI1(DAI1_PB04_O,SPT4_AFS_I);  /*DAC FS to SPORT 4A*/
   DAI1(DAI1_PB04_O,SPT4_BFS_I);  /*DAC FS to SPORT 4B*/
   DAI1(LOW,DAI1_PBEN04_I);

   DAI1(SPT4_AD0_O,DAI1_PB01_I); /* SPORT 4A to DAC*/
   DAI1(HIGH,DAI1_PBEN01_I);

   DAI1(DAI1_PB05_O,DAI1_PB12_I);  /*DAC clock to ADC */
   DAI1(HIGH,DAI1_PBEN12_I);

   DAI1(DAI1_PB04_O,DAI1_PB20_I);  /*DAC FS to ADC */
   DAI1(HIGH,DAI1_PBEN20_I);

   DAI1(DAI1_PB06_O,SPT4_BD0_I);
   DAI1(LOW,DAI1_PBEN06_I);
#else // Bose Hardware Variants

   /* ****************************************
    *  Internal Connections                  *
    * ***************************************/
   /* SPORT 4A */
   DAI1(PCG0_CLKC_O,SPT4_ACLK_I);      // PCGC clock to SPORT 4A
   DAI1(PCG0_FSC_O,SPT4_AFS_I);        // PCGC FS to SPORT 4A

   /* SPORT 0A */
   DAI0(PCG0_CLKA_O,SPT0_ACLK_I);      // PCGA clock to SPORT 0A
   DAI0(PCG0_FSA_O,SPT0_AFS_I);        // PCGA FS to SPORT 0A

   /* ****************************************
    * * External Connections                 *
    * ***************************************/
   #if defined(__HW_DP1__)
   /* ****************************************
    * * Audio Outputs                        *
    * ***************************************/
   /* Bit Clocks */
   /* MCLK_AMP1_AMP2 12.2880 Mhz*/
   DAI1(HIGH, DAI1_PBEN01_I);          // set as Output
   DAI1(PCG0_CLKC_O, DAI1_PB01_I);     // PCG C Clock to MCLK_AMP1_AMP2 (DAI1_PB01)

   /* MCLK_AMP3_AMP4 12.2880 Mhz*/
   DAI1(HIGH, DAI1_PBEN04_I);          // set as Output
   DAI1(PCG0_CLKC_O, DAI1_PB04_I);     // PCG C Clock to MCLK_AMP3_AMP4 (DAI1_PB04)

   /* Frame Select */
   /* FS_AMP1_AMP2 & FS_AMP3_AMP4 48.0 khz*/
   DAI1(HIGH, DAI1_PBEN08_I);          // set as Output
   DAI1(PCG0_FSC_O, DAI1_PB08_I);      // PCG C FS to FS_AMP1_AMP2 (DAI1_PB08)
                                       //         and FS_AMP3_AMP4 (DAI1_PB08)

   /* Data Out */
   DAI1(HIGH,DAI1_PBEN03_I);           // set as Output
   DAI1(SPT4_AD0_O,DAI1_PB03_I);       // SPORT4A to SD_DSP_AMP1_AMP2 (DAI1_PB03)
   DAI1(HIGH,DAI1_PBEN06_I);           // set as Output
   DAI1(SPT4_AD0_O,DAI1_PB06_I);       // SPORT4A to SD_DSP_AMP3_AMP4 (DAI1_PB06)

   /* ****************************************
    * * Audio Inputs                         *
    * ***************************************/
   /* ADAU1977 Connections */
   /* Bit clock */
   DAI0(HIGH, DAI0_PBEN20_I);          // set as Output
   DAI0(PCG0_CLKA_O, DAI0_PB20_I);     // PCG A Clock to MCLK_ADAU1977 (DAI0_PB20)

   /* Frame Select */
   DAI0(HIGH, DAI0_PBEN09_I);          // set as Output
   DAI0(PCG0_FSA_O, DAI0_PB09_I);      // PCG A FS to FS_ADAU1977 (DAI0_PB09)

   /* Data In */
   DAI0(LOW,DAI0_PBEN06_I);            // set as Input
   DAI0(DAI0_PB06_O, SPT0_AD0_I);      // SD_ADAU_DSP_TDM8 to SPORT0A (DAI0_PB06)
   #elif defined(__HW_DP2__)
   /* ****************************************
    * * Audio Outputs                        *
    * ***************************************/
   /* Bit Clocks */
   /* MCLK_AMP1_AMP2 12.2880 Mhz*/
   DAI1(HIGH, DAI1_PBEN01_I);          // set as Output
   DAI1(PCG0_CLKC_O, DAI1_PB01_I);     // PCG C Clock to MCLK_AMP1_AMP2 (DAI1_PB01)

   /* MCLK_AMP3_AMP4 12.2880 Mhz*/
   DAI1(HIGH, DAI1_PBEN04_I);          // set as Output
   DAI1(PCG0_CLKC_O, DAI1_PB04_I);     // PCG C Clock to MCLK_AMP3_AMP4 (DAI1_PB04)

   /* Frame Select */
   /* FS_AMP1_AMP2 & FS_AMP3_AMP4 48.0 khz*/
   DAI1(HIGH, DAI1_PBEN08_I);          // set as Output
   DAI1(PCG0_FSC_O, DAI1_PB08_I);      // PCG C FS to FS_AMP1_AMP2 (DAI1_PB08)
                                       //         and FS_AMP3_AMP4 (DAI1_PB08)

   /* Data Out */
   DAI1(HIGH,DAI1_PBEN03_I);           // set as Output
   DAI1(SPT4_AD0_O,DAI1_PB03_I);       // SPORT4A to SD_DSP_AMP1_AMP2 (DAI1_PB03)
   DAI1(HIGH,DAI1_PBEN06_I);           // set as Output
   DAI1(SPT4_AD0_O,DAI1_PB06_I);       // SPORT4A to SD_DSP_AMP3_AMP4 (DAI1_PB06)

   /* ****************************************
    * * Audio Inputs                         *
    * ***************************************/
   /* ADAU1977 Connections */
   /* Bit clock */
   DAI0(HIGH, DAI0_PBEN20_I);          // set as Output
   DAI0(PCG0_CLKA_O, DAI0_PB20_I);     // PCG A Clock to MCLK_ADAU1977 (DAI0_PB20)

   /* Frame Select */
   DAI0(HIGH, DAI0_PBEN19_I);          // set as Output
   DAI0(PCG0_FSA_O, DAI0_PB19_I);      // PCG A FS to FS_ADAU1977 (DAI0_PB19)

   /* Data In */
   DAI0(LOW,DAI0_PBEN12_I);            // set as Input
   DAI0(DAI0_PB12_O, SPT0_AD0_I);      // SD_ADAU_DSP_TDM8 to SPORT0A (DAI0_PB12)

   /* ****************************************
    * * SYNC Clock Outputs                   *
    * ***************************************/
   /* SYNC Regulator Connections */
   DAI1(HIGH, DAI1_PBEN20_I);          // set as Output
   DAI1(PCG0_CLKD_O, DAI1_PB20_I);     // PCG D Clock to SYNC_384KHZ_DSP_CLK (DAI1_PB20)
   #else
      #error "Hardware variant is not defined."
   #endif
#endif
}


/**
 *  @brief  Set DAI pins to known startup state.
 *
 *  @pre    None
 *
 *  @param  None
 *
 *  @post   DAI configuration will go to input and low.
 *
 *  @return None
 *
 */
static void Clear_DAI_Pins( void )
{
   /*
    * Tie to GROUND
    */
   // DAI0
   DAI0(LOW, DAI0_PB01_I);
   DAI0(LOW, DAI0_PB02_I);
   DAI0(LOW, DAI0_PB03_I);
   DAI0(LOW, DAI0_PB04_I);
   DAI0(LOW, DAI0_PB05_I);
   DAI0(LOW, DAI0_PB06_I);
   DAI0(LOW, DAI0_PB07_I);
   DAI0(LOW, DAI0_PB08_I);
   DAI0(LOW, DAI0_PB09_I);
   DAI0(LOW, DAI0_PB10_I);
   DAI0(LOW, DAI0_PB11_I);
   DAI0(LOW, DAI0_PB12_I);
/* 21569 does not have access to these pins */
#if !defined(__ADSP21569__)
   DAI0(LOW, DAI0_PB13_I);
   DAI0(LOW, DAI0_PB14_I);
   DAI0(LOW, DAI0_PB15_I);
   DAI0(LOW, DAI0_PB16_I);
   DAI0(LOW, DAI0_PB17_I);
   DAI0(LOW, DAI0_PB18_I);
#endif
   DAI0(LOW, DAI0_PB19_I);
   DAI0(LOW, DAI0_PB20_I);

   // DAI1
   DAI1(LOW, DAI1_PB01_I);
   DAI1(LOW, DAI1_PB02_I);
   DAI1(LOW, DAI1_PB03_I);
   DAI1(LOW, DAI1_PB04_I);
   DAI1(LOW, DAI1_PB05_I);
   DAI1(LOW, DAI1_PB06_I);
   DAI1(LOW, DAI1_PB07_I);
   DAI1(LOW, DAI1_PB08_I);
   DAI1(LOW, DAI1_PB09_I);
   DAI1(LOW, DAI1_PB10_I);
   DAI1(LOW, DAI1_PB11_I);
   DAI1(LOW, DAI1_PB12_I);
/* 21569 does not have access to these pins */
#if !defined(__ADSP21569__)
   DAI1(LOW, DAI1_PB13_I);
   DAI1(LOW, DAI1_PB14_I);
   DAI1(LOW, DAI1_PB15_I);
   DAI1(LOW, DAI1_PB16_I);
   DAI1(LOW, DAI1_PB17_I);
   DAI1(LOW, DAI1_PB18_I);
#endif
   DAI1(LOW, DAI1_PB19_I);
   DAI1(LOW, DAI1_PB20_I);

   /*
    * Configure as Inputs
    */
   // DAI0
   DAI0(LOW, DAI0_PBEN01_I);
   DAI0(LOW, DAI0_PBEN02_I);
   DAI0(LOW, DAI0_PBEN03_I);
   DAI0(LOW, DAI0_PBEN04_I);
   DAI0(LOW, DAI0_PBEN05_I);
   DAI0(LOW, DAI0_PBEN06_I);
   DAI0(LOW, DAI0_PBEN07_I);
   DAI0(LOW, DAI0_PBEN08_I);
   DAI0(LOW, DAI0_PBEN09_I);
   DAI0(LOW, DAI0_PBEN10_I);
   DAI0(LOW, DAI0_PBEN11_I);
   DAI0(LOW, DAI0_PBEN12_I);
   DAI0(LOW, DAI0_PBEN13_I);
   DAI0(LOW, DAI0_PBEN14_I);
   DAI0(LOW, DAI0_PBEN15_I);
   DAI0(LOW, DAI0_PBEN16_I);
   DAI0(LOW, DAI0_PBEN17_I);
   DAI0(LOW, DAI0_PBEN18_I);
   DAI0(LOW, DAI0_PBEN19_I);
   DAI0(LOW, DAI0_PBEN20_I);

   // DAI1
   DAI1(LOW, DAI1_PBEN01_I);
   DAI1(LOW, DAI1_PBEN02_I);
   DAI1(LOW, DAI1_PBEN03_I);
   DAI1(LOW, DAI1_PBEN04_I);
   DAI1(LOW, DAI1_PBEN05_I);
   DAI1(LOW, DAI1_PBEN06_I);
   DAI1(LOW, DAI1_PBEN07_I);
   DAI1(LOW, DAI1_PBEN08_I);
   DAI1(LOW, DAI1_PBEN09_I);
   DAI1(LOW, DAI1_PBEN10_I);
   DAI1(LOW, DAI1_PBEN11_I);
   DAI1(LOW, DAI1_PBEN12_I);
   DAI1(LOW, DAI1_PBEN13_I);
   DAI1(LOW, DAI1_PBEN14_I);
   DAI1(LOW, DAI1_PBEN15_I);
   DAI1(LOW, DAI1_PBEN16_I);
   DAI1(LOW, DAI1_PBEN17_I);
   DAI1(LOW, DAI1_PBEN18_I);
   DAI1(LOW, DAI1_PBEN19_I);
   DAI1(LOW, DAI1_PBEN20_I);

   /* Enable all Input buffers, if any pin needs to be
    * output, it will be part of the configuration. */
   /* PADS0 DAI0 Port Input Enable Control Register */
   *pREG_PADS0_DAI0_IE = (unsigned int) 0x001FFFFF;
   /* PADS0 DAI1 Port Input Enable Control Register */
   *pREG_PADS0_DAI1_IE = (unsigned int) 0x001FFFFF;
}
