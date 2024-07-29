/*
 **
 ** Source file generated on August 27, 2018 at 12:04:48.
 **
 ** Copyright (C) 2018 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in 
 ** the DMC Initialization configuration dialog. Changes to this configuration should be made by
 ** changing the appropriate options rather than editing this file.
 **
 */

/*!
* @file      adi_dmc.h
*
* @brief     DMC Configuration header file
*
* @details
*            DMC Configuration header file
*/

#ifndef _ADI_DMC_H_
#define _ADI_DMC_H_

#include <sys/platform.h>
#include <stdbool.h>
#if defined (__ADSP21569__) || defined(__ADSP21567__)
#define ConfigDMC 1
#else
#define ConfigDMC 0
#endif

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_5_1:"Identifiers (internal and external) shall not rely on the significance of more than 31 characters")
#endif

/* including missing declarations*/
#define BITM_DMC_DDR_LANE0_CTL0_CB_RSTDLL    (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  Reset the Lane DLL */
#define BITM_DMC_DDR_LANE1_CTL0_CB_RSTDLL    (_ADI_MSK_3(0x00000100,0x00000100UL, uint32_t  ))    /*  Reset the Lane DLL */
#define BITM_DMC_DDR_LANE0_CTL1_COMP_DCYCLE  (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Compute Datacycle */
#define BITM_DMC_DDR_LANE1_CTL1_COMP_DCYCLE  (_ADI_MSK_3(0x00000002,0x00000002UL, uint32_t  ))    /*  Compute Datacycle */
#define BITM_DMC_DDR_LANE0_CTL0_CB_RSTDAT    (_ADI_MSK_3(0x08000000,0x08000000UL, uint32_t  ))    /*  Reset the Data Pads */
#define BITM_DMC_DDR_LANE1_CTL0_CB_RSTDAT    (_ADI_MSK_3(0x08000000,0x08000000UL, uint32_t  ))    /*  Reset the Data Pads */
#define BITP_DMC_CTL_RDTOWR                   9            /*  Read-to-Write Cycle */
#define BITP_DMC_TR2_TWR                     12            /*  Timing Write Recovery */


typedef enum
{
  ADI_DMC_SUCCESS=0u,
  ADI_DMC_FAILURE
}ADI_DMC_RESULT;

/* structure which holds DMC register values */
typedef struct
{

  uint32_t ulDDR_DLLCTLCFG;               /*!< Content of DDR DLLCTL and DMC_CFG register     */
  uint32_t ulDDR_EMR2EMR3;                /*!< Content of the DDR EMR2 and EMR3 Register      */
  uint32_t ulDDR_CTL;                     /*!< Content of the DDR Control                   */
  uint32_t ulDDR_MREMR1;                  /*!< Content of the DDR MR and EMR1 Register      */
  uint32_t ulDDR_TR0;                     /*!< Content of the DDR Timing Register      */
  uint32_t ulDDR_TR1;                     /*!< Content of the DDR Timing Register      */
  uint32_t ulDDR_TR2;                     /*!< Content of the DDR Timing Register      */
  uint32_t ulDDR_ZQCTL0;                  /*!< Content of ZQCTL0 register */
  uint32_t ulDDR_ZQCTL1;                  /*!< Content of ZQCTL1 register */
  uint32_t ulDDR_ZQCTL2;                  /*!< Content of ZQCTL2 register */

}ADI_DMC_CONFIG;

#ifdef __cplusplus
extern "C" {
#endif

/* reset dmc lanes */
void adi_dmc_lane_reset(bool reset);
/* enter/exit self refresh mode */
void adi_dmc_self_refresh(bool enter);
/* program dmc controller timing registers */
ADI_DMC_RESULT adi_dmc_ctrl_init(ADI_DMC_CONFIG *pConfig,uint32_t csel_dsel_r);
/* trigger Zq calibration*/
void adi_dmc_phy_calibration(ADI_DMC_CONFIG *pConfig,uint32_t csel_dsel_r);

void adi_dmc_lane_reset(bool reset);

#ifdef __cplusplus
}
#endif

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif

#endif /* ADI_DMC_H_*/


