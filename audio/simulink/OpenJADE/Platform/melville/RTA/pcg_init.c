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
 *  @file pcg_init.c
 *
 *  @brief Initializes Precision clock generators according to board hardware
 *
 *  @ingroup platform
 */


/***  Include Files ***********************************************************/

#include "sys/platform.h"

/***  External Variable References  *******************************************/

/***  Module Macros  **********************************************************/

/***  Module Types  ***********************************************************/

/***  Module Variables  *******************************************************/

/***  Module Function Prototypes  *********************************************/

/***  Module Functions  *******************************************************/


/**
 *  @brief  Configures PCG peripherals to generate all necessary clocks.
 *
 *  @pre    None
 *
 *  @param  None
 *
 *  @post   Modifies PCG settins
 *
 *  @return None
 *
 */
void Init_PCG ( void )
{
   /*
   ** PCG Programming
   */

   /*
   ** IMPORTANT: The order of the register writing matters here. The PCG should be enabled only after
   ** all other PCG configuration registers have been written.
   */

   /* PCG A
    * PCG A will be used to generate the ADC clock for TDM8.
    * Clock = 12.2880 MHz;
    * FS    = 48 kHz */
   *pREG_PCG0_CTLA1 =
      (0 << BITP_PCG_CTLA1_CLKSRC)  | // CLKSRC    [31   ]: SYS_CLKIN pin selected for clock
      (0 << BITP_PCG_CTLA1_FSSRC)   | // FSSRC     [30   ]: SYS_CLKIN pin selected for frame sync
      0x00000000UL                  | // FSPHASELO [29-20]: 0x000
      0x00000002UL                  ; // CLKDIV    [19- 0]: (SYS_CLKIN0 = 24.576(MHz)) / 12.2880(MHz) = 2 = 0x00002

   *pREG_PCG0_CTLA0 =
      (1UL << BITP_PCG_CTLA0_CLKEN) | // CLKEN     [31   ]: clock generation enabled
      (1UL << BITP_PCG_CTLA0_FSEN)  | // FSEN      [30   ]: frame sync generation enabled
      0x00000000UL                  | // FSPHASEHI [29-20]: 0x000
      0x00000200UL                  ; // FSDIV     [19- 0]: (SYS_CLKIN0 = 24.576(MHz)) / 48(KHz) = 512 = 0x00200

//   /* PCG B */
//
//   *pREG_PCG0_CTLB1 =
//      PCG_CFG_CLK_SRC_SYS_CLKIN | // CLKSRC    [31   ]: SYS_CLKIN pin selected for clock
//      PCG_CFG_FS_SRC_SYS_CLKIN  | // FSSRC     [30   ]: SYS_CLKIN pin selected for frame sync
//      0x00000000UL              | // FSPHASELO [29-20]: 0x000
//      0x00000040UL              ; // CLKDIV    [19- 0]: (SCLK0 = 124.1856(MHz)) / 1.94040(MHz) = 64 = 0x00040
//
//   *pREG_PCG0_CTLB0 =
//      PCG_CFG_CLK_EN            | // CLKEN     [31   ]: clock generation enabled
//      PCG_CFG_FS_EN             | // FSEN      [30   ]: frame sync generation disabled
//      0x00000000UL              | // FSPHASEHI [29-20]: 0x000
//      0x00000160UL              ; // FSDIV     [19- 0]: (SCLK0 = 124.1856(MHz)) / 352.8(KHz) = 352 = 0x00160
//
//   *pREG_PCG0_SYNC1 =
//      PCG_B_CFG_FS_SRC_SCLK0    | // FSBSRC    [19   ]: SCLK0 pin selected for frame sync
//      PCG_B_CFG_CLK_SRC_SCLK0   ; // CLKBSRC   [18   ]: SCLK0 pin selected for clock
//
   /* PCG C
    * PCG C will be used to generate the ADC clock for TDM8.
    * Clock = 12.2880 MHz;
    * FS    = 48 kHz */
   *pREG_PCG0_CTLC1 =
      (0 << BITP_PCG_CTLC1_CLKSRC)  | // CLKSRC    [31   ]: SYS_CLKIN pin selected for clock
      (0 << BITP_PCG_CTLC1_FSSRC)   | // FSSRC     [30   ]: SYS_CLKIN pin selected for frame sync
      0x00000000UL                  | // FSPHASELO [29-20]: 0x000
      0x00000002UL                  ; // CLKDIV    [19- 0]: (SYS_CLKIN0 = 24.576(MHz)) / 12.2880(MHz) = 2 = 0x00002

   *pREG_PCG0_CTLC0 =
      (1UL << BITP_PCG_CTLC0_CLKEN) | // CLKEN     [31   ]: clock generation enabled
      (1UL << BITP_PCG_CTLC0_FSEN)  | // FSEN      [30   ]: frame sync generation enabled
      0x00000000UL                  | // FSPHASEHI [29-20]: 0x000
      0x00000200UL                  ; // FSDIV     [19- 0]: (SYS_CLKIN0 = 24.576(MHz)) / 48(KHz) = 512 = 0x00200

   /* PCG D
    * PCG D will be used to generate the Regulator clocks
    * Clock = 384 KHz;
    * FS    = Not needed*/
   *pREG_PCG0_CTLD1 =
      (0 << BITP_PCG_CTLC1_CLKSRC)  | // CLKSRC    [31   ]: SYS_CLKIN pin selected for clock
      0x00000000UL                  | // FSSRC     [30   ]: SYS_CLKIN pin selected for frame sync
      0x00000000UL                  | // FSPHASELO [29-20]: 0x000
      0x00000040UL                  ; // CLKDIV    [19- 0]: (SYS_CLKIN0 = 24.576(MHz)) / 0.384(MHz) = 64 = 0x0040

   *pREG_PCG0_CTLD0 =
      (1UL << BITP_PCG_CTLC0_CLKEN) | // CLKEN     [31   ]: clock generation enabled
      0x00000000UL                  | // FSEN      [30   ]: frame sync generation enabled
      0x00000000UL                  | // FSPHASEHI [29-20]: 0x000
      0x00000000UL                  ; // FSDIV     [19- 0]: (SYS_CLKIN0 = 24.576(MHz)) / 48(KHz) = 512 = 0x00200

}

