/*
 **
 ** Source file generated on February 2, 2018 at 20:06:04.
 **
 ** Copyright (C) 2018 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in
 ** the CGU Initialization configuration dialog. Changes to this configuration should be made by
 ** changing the appropriate options rather than editing this file.
 **
 */
#include <adi_dmc.h>
#include <sys/platform.h>
#include <stdint.h>
#include <services/pwr/adi_pwr.h>

/**********************************************************************************************
 *						CGU	Configuration Number 0
 **********************************************************************************************/
/*
Configuration Number	: 0
SDRAM Mode				: DDR3
SYS_CLKIN0 (MHz)		: 25
Use CGU1 ?				: Yes
Reset DMC DLL ? 		: Yes

//CDU Initialization Options
SPI(0-2) CLOCK SOURCE (CLKO0)  :   SCLK0_0		:	125 MHz
DDR CLOCK	  		  (CLKO1)  :   DCLK0_1		:	800 MHz

//CGU0 Initialization Options
fVCO (Desired)			:	2000 MHz
fVCO (Actual)			: 	2000 MHz
CCLK					:	1000 MHz
SYSCLK					:	500  MHz
SCLK0					:	125  MHz
SCLK1(SPDIF RX)			:	250	 MHz
DCLK					:	666.66 MHz
OCLK (LP CLOCK)			:	125  MHz

MSEL					: 	80
Use DF?					: 	No
DF						: 	0
CSEL					: 	2
CCLK to SYSCLK Ratio	: 	2:1
S0SEL					: 	4
S1SEL					: 	2
DSEL					: 	3
OSEL					: 	16
Use S1SELEX?			: 	No
S1SELEX					: 	0

//CGU1 Initialization Options
fVCO (Desired)			:	1600 MHz
fVCO (Actual)			: 	1600 MHz
DCLK0_1					:	800  MHz

MSEL					: 	64
Use DF?					: 	No
DF						: 	0
DSEL					: 	2
*/


/**********************************************************************************************
 *						CGU	Configuration Number 0
 **********************************************************************************************/

/* CLKIN */
#define CFG0_BIT_CGU0_CLKIN									25000000

/* SPI(0-2) CLOCK SOURCE */
#define CDU0_CFG0_SEL_VALUE									0

/* DDR CLOCK */
#define CDU0_CFG1_SEL_VALUE									2

/**********************************************************************************************
 *						CGU	Configuration Number 0 Register Values
 **********************************************************************************************/

/*****************************************CGU0_CTL**********************************************/

#define CFG0_BIT_CGU0_CTL_DF								0
#define CFG0_BIT_CGU0_CTL_MSEL								80


/*****************************************CGU0_DIV**********************************************/

#define CFG0_BIT_CGU0_DIV_CSEL								2
#define CFG0_BIT_CGU0_DIV_SYSSEL							4
#define CFG0_BIT_CGU0_DIV_S0SEL								4
#define CFG0_BIT_CGU0_DIV_S1SEL								2
#define CFG0_BIT_CGU0_DIV_DSEL								4
#define CFG0_BIT_CGU0_DIV_OSEL								16


/*****************************************CGU0_DIVEX**********************************************/

#define CFG0_BIT_CGU0_DIV_S1SELEX							6

/*****************************************CGU1_CTL**********************************************/

#define CFG0_BIT_CGU1_CTL_DF								0
#define CFG0_BIT_CGU1_CTL_MSEL								64

/*****************************************CGU1_DIV**********************************************/

#define CFG0_BIT_CGU1_DIV_DSEL								3

/*****************************************DMC SELF REFESH***************************************/

#define DDR_SELF_REFRESH    0

/**************************************************************************************************/


void adi_pwr_cfg0_init(void) ;

/* Structure pointer for CGU0 and CGU1 parameters*/
ADI_PWR_CGU_PARAM_LIST pADI_CGU_Param_List;

/* Structure pointer for CDU parameters*/
ADI_PWR_CDU_PARAM_LIST pADI_CDU_Param_List;

void adi_pwr_cfg0_init(void)
{
	/* CDU */
	pADI_CDU_Param_List.cdu_settings[0].cfg_SEL 					= 		(ADI_PWR_CDU_CLKIN)CDU0_CFG0_SEL_VALUE;
	pADI_CDU_Param_List.cdu_settings[0].cfg_EN  					= 		true;

	pADI_CDU_Param_List.cdu_settings[1].cfg_SEL 					= 		(ADI_PWR_CDU_CLKIN)CDU0_CFG1_SEL_VALUE;
	pADI_CDU_Param_List.cdu_settings[1].cfg_EN  					= 		true;

	/* CGU0 */
	pADI_CGU_Param_List.cgu0_settings.clocksettings.ctl_MSEL		=		(uint32_t)CFG0_BIT_CGU0_CTL_MSEL;
	pADI_CGU_Param_List.cgu0_settings.clocksettings.ctl_DF			=		(uint32_t)CFG0_BIT_CGU0_CTL_DF;
	pADI_CGU_Param_List.cgu0_settings.clocksettings.div_CSEL		=		(uint32_t)CFG0_BIT_CGU0_DIV_CSEL;
	pADI_CGU_Param_List.cgu0_settings.clocksettings.div_SYSSEL		=		(uint32_t)CFG0_BIT_CGU0_DIV_SYSSEL;
	pADI_CGU_Param_List.cgu0_settings.clocksettings.div_S0SEL		=		(uint32_t)CFG0_BIT_CGU0_DIV_S0SEL;
	pADI_CGU_Param_List.cgu0_settings.clocksettings.div_S1SEL		=		(uint32_t)CFG0_BIT_CGU0_DIV_S1SEL;
	pADI_CGU_Param_List.cgu0_settings.clocksettings.divex_S1SELEX	=		(uint32_t)CFG0_BIT_CGU0_DIV_S1SELEX;
	pADI_CGU_Param_List.cgu0_settings.clocksettings.div_DSEL		=		(uint32_t)CFG0_BIT_CGU0_DIV_DSEL;
	pADI_CGU_Param_List.cgu0_settings.clocksettings.div_OSEL		=		(uint32_t)CFG0_BIT_CGU0_DIV_OSEL;
	pADI_CGU_Param_List.cgu0_settings.clkin 						= 		(uint32_t)CFG0_BIT_CGU0_CLKIN;
	pADI_CGU_Param_List.cgu0_settings.enable_IDLE					= 		false;
	pADI_CGU_Param_List.cgu0_settings.enable_SCLK1ExDiv				= 		false;

	/* CGU1 */
	pADI_CGU_Param_List.cgu1_settings.clkin 						= 		(uint32_t)CFG0_BIT_CGU0_CLKIN;
	pADI_CGU_Param_List.cgu1_settings.clocksettings.ctl_MSEL		=		(uint32_t)CFG0_BIT_CGU1_CTL_MSEL;
	pADI_CGU_Param_List.cgu1_settings.clocksettings.ctl_DF			=		(uint32_t)CFG0_BIT_CGU1_CTL_DF;
	pADI_CGU_Param_List.cgu1_settings.clocksettings.div_DSEL		=		(uint32_t)CFG0_BIT_CGU1_DIV_DSEL;
	pADI_CGU_Param_List.cgu1_settings.enable_IDLE					= 		false;

	/* Set DDR DLL Reset*/
	int delay=9000;
	adi_dmc_lane_reset(true);
	while(delay--);

	/* DDR Self Refresh*/
#if DDR_SELF_REFRESH
	adi_pwr_ddr_Self_Refresh(0, true);
	adi_pwr_ddr_Self_Refresh(1, true);
#endif

	/* Initialize all the clocks*/
	adi_pwr_ClockInit(&pADI_CGU_Param_List, &pADI_CDU_Param_List);

	/* Clear DDR DLL Reset*/
	adi_dmc_lane_reset(false);
	delay=9000;
	while(delay--);

	/* DDR Self Refresh*/
#if DDR_SELF_REFRESH
	adi_pwr_ddr_Self_Refresh(0, false);
	adi_pwr_ddr_Self_Refresh(1, false);
#endif
}



