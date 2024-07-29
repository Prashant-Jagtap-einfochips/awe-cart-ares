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


#include <adi_dmc.h>
#include <stdio.h>
#include <stdbool.h>

#if defined(ADI_UCOS3_PRODUCT)
   #include <os.h>
#endif

#if ConfigDMC
#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_14_7:"Allows multiple exit points")
#endif

#define TrigCalib 0ul
#define DelayTrim 0ul
#define DqsTrim  0ul
#define ClkTrim 0ul
#define CLKDIR 0ul
#define Bypasscode 0ul
#define Dqscode 0ul
#define Clkcode 0ul
#define OfstdCycle 2ul



/* delay function in microseconds comes in microseconds/10  */
#pragma inline
void ddr3_dmcdelay(uint32_t delay)
{
#if defined(ADI_UCOS3_PRODUCT)
   OS_ERR err;
   uint32_t delay_us = delay/10u;

   OSTimeDlyHMSM(
         (CPU_INT16U) 0,
         (CPU_INT16U) 0,
         (CPU_INT16U) 0,
         (CPU_INT32U) delay_us,  // us
         (OS_OPT) OS_OPT_TIME_HMSM_STRICT,
         (OS_ERR *) &err
         );

#else
  uint32_t i;
  for(i=delay; i>0ul; i--){
	  NOP();

  }
#endif
}



/* DMC phy ZQ calibration routine
 * The first step in DMC initialization
 */
void adi_dmc_phy_calibration(ADI_DMC_CONFIG *pConfig,uint32_t csel_dsel_r)
{

  /* Program the ODT and drive strength values */

  *pREG_DMC0_DDR_ZQ_CTL0 = pConfig->ulDDR_ZQCTL0;
  *pREG_DMC0_DDR_ZQ_CTL1 = pConfig->ulDDR_ZQCTL1;
  *pREG_DMC0_DDR_ZQ_CTL2 = pConfig->ulDDR_ZQCTL2;

    /* Generate the trigger */
  *pREG_DMC0_DDR_CA_CTL = 0x00000000ul ;
  *pREG_DMC0_DDR_ROOT_CTL = 0x00000000ul;
  *pREG_DMC0_DDR_ROOT_CTL = 0x00010000ul;

  ddr3_dmcdelay(csel_dsel_r*8000ul);

     /* The [31:26] bits may change if pad ring changes */
  *pREG_DMC0_DDR_CA_CTL  = 0x0C000001ul|TrigCalib;
  ddr3_dmcdelay(csel_dsel_r*8000ul);
  *pREG_DMC0_DDR_CA_CTL  = 0x00000000ul ;
  *pREG_DMC0_DDR_ROOT_CTL = 0x00000000ul ;


  /* DQS delay trim*/
  if (DelayTrim)
  {
    *pREG_DMC0_DDR_LANE0_CTL1|= (((Bypasscode)<<BITP_DMC_DDR_LANE0_CTL1_BYPCODE) & BITM_DMC_DDR_LANE0_CTL1_BYPCODE)|BITM_DMC_DDR_LANE0_CTL1_BYPDELCHAINEN;
    *pREG_DMC0_DDR_LANE1_CTL1|=(((Bypasscode)<<BITP_DMC_DDR_LANE1_CTL1_BYPCODE) & BITM_DMC_DDR_LANE1_CTL1_BYPCODE)|BITM_DMC_DDR_LANE1_CTL1_BYPDELCHAINEN;
  }

  /* DQS duty trim */
   if(DqsTrim)
   {
    *pREG_DMC0_DDR_LANE0_CTL0|= ((Dqscode)<<BITP_DMC_DDR_LANE0_CTL0_BYPENB) & (BITM_DMC_DDR_LANE1_CTL0_BYPENB|BITM_DMC_DDR_LANE0_CTL0_BYPSELP|BITM_DMC_DDR_LANE0_CTL0_BYPCODE);
    *pREG_DMC0_DDR_LANE1_CTL0|=((Dqscode)<<BITP_DMC_DDR_LANE1_CTL0_BYPENB) & (BITM_DMC_DDR_LANE1_CTL1_BYPCODE|BITM_DMC_DDR_LANE1_CTL0_BYPSELP|BITM_DMC_DDR_LANE1_CTL0_BYPCODE);
   }

   /* Clock duty trim */
   if(ClkTrim)
   {
     *pREG_DMC0_DDR_CA_CTL|=(((Clkcode <<BITP_DMC_DDR_CA_CTL_BYPCODE1)&BITM_DMC_DDR_CA_CTL_BYPCODE1)|BITM_DMC_DDR_CA_CTL_BYPENB|((CLKDIR<<BITP_DMC_DDR_CA_CTL_BYPSELP)&BITM_DMC_DDR_CA_CTL_BYPSELP));
   }



}

/*  DMC lane reset routine
 *  DMC lane reset should be performed when DMC clock is changed
 *  Sequence: Before changing the DDR clock frequency reset the lane , call cgu routine to change the DMC clock frequency
 *        Then take DMC lanes out of reset
 */
void adi_dmc_lane_reset(bool reset)
{

  if (reset) {
    *pREG_DMC0_DDR_LANE0_CTL0 |= BITM_DMC_DDR_LANE0_CTL0_CB_RSTDLL;
    *pREG_DMC0_DDR_LANE1_CTL0 |= BITM_DMC_DDR_LANE1_CTL0_CB_RSTDLL;


  } else {
    *pREG_DMC0_DDR_LANE0_CTL0 &= ~BITM_DMC_DDR_LANE0_CTL0_CB_RSTDLL;
    *pREG_DMC0_DDR_LANE1_CTL0 &= ~BITM_DMC_DDR_LANE1_CTL0_CB_RSTDLL;

  }
}


/* dmc_ctrl_init() configures DMC controller
 * It returns ADI_DMC_SUCCESS on success and ADI_DMC_FAILURE on failure
 */

ADI_DMC_RESULT adi_dmc_ctrl_init(ADI_DMC_CONFIG *pConfig,uint32_t csel_dsel_r)
{

  uint32_t phyphase,rd_cnt,t_EMR1,t_EMR3, t_CTL,data_cyc;

  /* program timing registers*/
  *pREG_DMC0_CFG = (pConfig->ulDDR_DLLCTLCFG) & 0xFFFFul;
  *pREG_DMC0_TR0 = pConfig->ulDDR_TR0;
  *pREG_DMC0_TR1 = pConfig->ulDDR_TR1;
  *pREG_DMC0_TR2 = pConfig->ulDDR_TR2;

  /* program shadow registers */
  *pREG_DMC0_MR =  ((pConfig->ulDDR_MREMR1) >> 16ul) & 0xFFFFul;
  *pREG_DMC0_MR1 = (pConfig->ulDDR_MREMR1) & 0xFFFFul;
  *pREG_DMC0_MR2 = (pConfig->ulDDR_EMR2EMR3)>>16ul & 0xFFFFul;
  *pREG_DMC0_EMR3 =(pConfig->ulDDR_EMR2EMR3) & 0xFFFFul;

  /* program Dll timing register */
  *pREG_DMC0_DLLCTL = ((pConfig->ulDDR_DLLCTLCFG) >> 16ul) & 0xFFFFul;

  ddr3_dmcdelay(csel_dsel_r*2000ul);

  *pREG_DMC0_DDR_CA_CTL |=BITM_DMC_DDR_CA_CTL_SW_REFRESH;

  ddr3_dmcdelay(csel_dsel_r*5ul);

  *pREG_DMC0_DDR_ROOT_CTL |= BITM_DMC_DDR_ROOT_CTL_SW_REFRESH | (OfstdCycle << BITP_DMC_DDR_ROOT_CTL_PIPE_OFSTDCYCLE);

  /* Start DMC initialization */
  *pREG_DMC0_CTL       = pConfig->ulDDR_CTL;

  ddr3_dmcdelay(csel_dsel_r*12500ul);

  /* Add necessary delay depending on the configuration */
  t_EMR1=(pConfig->ulDDR_MREMR1 & BITM_DMC_MR1_WL)>>BITP_DMC_MR1_WL;
  if(t_EMR1)
  {
     ddr3_dmcdelay(csel_dsel_r*600ul);
     while(((*pREG_DMC0_MR1 & BITM_DMC_MR1_WL)>>BITP_DMC_MR1_WL) != 0ul)
     {

     }
  }


  t_EMR3=(pConfig->ulDDR_EMR2EMR3 & BITM_DMC_EMR3_MPR)>>BITP_DMC_EMR3_MPR;
  if(t_EMR3)
  {
     ddr3_dmcdelay(csel_dsel_r*2000ul);
     while(((*pREG_DMC0_EMR3 & BITM_DMC_EMR3_MPR)>>BITP_DMC_EMR3_MPR) != 0ul)
     {
     }
  }


  t_CTL=(pConfig->ulDDR_CTL & BITM_DMC_CTL_RL_DQS)>>BITP_DMC_CTL_RL_DQS;
  if(t_CTL)
  {
     ddr3_dmcdelay(csel_dsel_r*600ul);
     while(((*pREG_DMC0_CTL & BITM_DMC_CTL_RL_DQS)>>BITP_DMC_CTL_RL_DQS) != 0ul)
     {

     }
  }

  /* check if DMC initialization finished, if not return error */
  while((*pREG_DMC0_STAT & BITM_DMC_STAT_INITDONE) !=BITM_DMC_STAT_INITDONE);


  /* End of DMC controller configuration, Start of Phy control registers */

  /* toggle DCYCLE */

  *pREG_DMC0_DDR_LANE0_CTL1 |= BITM_DMC_DDR_LANE0_CTL1_COMP_DCYCLE;
  *pREG_DMC0_DDR_LANE1_CTL1 |= BITM_DMC_DDR_LANE1_CTL1_COMP_DCYCLE;

  ddr3_dmcdelay(csel_dsel_r*10ul);

  *pREG_DMC0_DDR_LANE0_CTL1 &= (~BITM_DMC_DDR_LANE0_CTL1_COMP_DCYCLE);
  *pREG_DMC0_DDR_LANE1_CTL1 &= (~BITM_DMC_DDR_LANE1_CTL1_COMP_DCYCLE);

  /* toggle RSTDAT */
  *pREG_DMC0_DDR_LANE0_CTL0 |= BITM_DMC_DDR_LANE0_CTL0_CB_RSTDAT;
  *pREG_DMC0_DDR_LANE0_CTL0 &= (~BITM_DMC_DDR_LANE0_CTL0_CB_RSTDAT);

  *pREG_DMC0_DDR_LANE1_CTL0 |= BITM_DMC_DDR_LANE1_CTL0_CB_RSTDAT;
  *pREG_DMC0_DDR_LANE1_CTL0 &= (~BITM_DMC_DDR_LANE1_CTL0_CB_RSTDAT);

  ddr3_dmcdelay(csel_dsel_r*2500ul);

  /* Program phyphase*/

  phyphase = (*pREG_DMC0_STAT & BITM_DMC_STAT_PHYRDPHASE)>>BITP_DMC_STAT_PHYRDPHASE;
  data_cyc= (phyphase << BITP_DMC_DLLCTL_DATACYC) & BITM_DMC_DLLCTL_DATACYC;
  rd_cnt = ((pConfig->ulDDR_DLLCTLCFG) >> 16);
  rd_cnt <<= BITP_DMC_DLLCTL_DLLCALRDCNT;
  rd_cnt &= BITM_DMC_DLLCTL_DLLCALRDCNT;

  *pREG_DMC0_DLLCTL =rd_cnt|data_cyc;

  *pREG_DMC0_CTL = (pConfig->ulDDR_CTL & (~BITM_DMC_CTL_INIT) & (~BITM_DMC_CTL_RL_DQS));

  return ADI_DMC_SUCCESS;

}

/* In order to preserve ddr content while changing DMC clock, put memory in self refresh mode
 *
 * sequence: program DMC controller to enter self refresh mode. No further access should be issued to ddr memory
 *       call cgu routines to change DMC clock
 *       exit self-refresh mode
 *
 */
void adi_dmc_self_refresh(bool enter)
{



  if (enter){
    /*Set the SRREQ bit to enter self refresh mode*/
	  while((*pREG_DMC0_STAT & BITM_DMC_STAT_IDLE)==0ul)
	  {

	  }
    *pREG_DMC0_CTL|= BITM_DMC_CTL_SRREQ;

    /*Wait for self refresh acknowledge from the controller*/
    while((*pREG_DMC0_STAT & BITM_DMC_STAT_SRACK)==0ul)
    {

    }

  }else{
    /*clear the SRREQ bit to exit self refresh mode*/
    *pREG_DMC0_CTL &= ~BITM_DMC_CTL_SRREQ;

    /*Wait for self refresh acknowledge from the controller*/
    while((*pREG_DMC0_STAT & BITM_DMC_STAT_SRACK)==1ul)
    {

    }
    /*Wait for the DMC to be in IDLE state*/
    while((*pREG_DMC0_STAT & BITM_DMC_STAT_IDLE)==0ul)
    {

    }

    *pREG_DMC0_CTL |=BITM_DMC_CTL_DLLCAL;
    /*Wait for DLL calib*/
    while((*pREG_DMC0_STAT & BITM_DMC_STAT_DLLCALDONE)==0ul)
    {

    }

  }
}

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif
#endif
