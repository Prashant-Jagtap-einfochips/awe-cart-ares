/*******************************************************************************

Copyright(c) 2017-2018 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you
agree to the terms of the associated Analog Devices License Agreement.

*******************************************************************************/

/*
 *  Software Switch Configuration for the ADSP-21569 EZ-BOARD
 *
 *  Please see the SoftConfig documentation in CCES Help for detailed
 *  information on SoftConfig.
 *
 */

#include <drivers\twi\adi_twi_2156x.h>

/* TWI settings */
#define TWI_PRESCALE  (12u)
#define TWI_BITRATE   (100u)
#define TWI_DUTYCYCLE (50u)

#define BUFFER_SIZE (32u)

/* TWI device memory */
static uint8_t deviceMemory[ADI_TWI_MEMORY_SIZE];

/* switch register structure */
typedef struct {
	uint8_t Register;
	uint8_t Value;
} SWITCH_CONFIG;

/* switch parameter structure */
typedef struct {
	uint32_t TWIDevice;
	uint16_t HardwareAddress;
	uint32_t NumConfigSettings;
	SWITCH_CONFIG *ConfigSettings;
} SOFT_SWITCH;

/* prototype */
void ConfigSoftSwitches_ADC_DAC(void);
void ConfigSoftSwitches_ADAU(void);
void ConfigSoftSwitches_EV_SOM(void);

/* disable misra diagnostics as necessary */
#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_8_7:"Objects shall be defined at block scope")
#pragma diag(suppress:misra_rule_17_4:"Array indexing shall be the only allowed form of pointer arithmetic")
#endif /* _MISRA_RULES */

#ifdef __ADSPGCC__
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

/************************SoftConfig Information********************************

    ~ means the signal is active low

    Please see the ADSP-21569 EZ-BOARD manual for more information on using
    Software-Controlled Switches(SoftConfig)

********************************************************************************/

/* switch 0 register settings */
static SWITCH_CONFIG ADC_DAC_SwitchConfig0[] =
{
 /*
       U6 Port A                                  U6 Port B
  7--------------- ~ADAU1979_EN       |       7--------------- ~GIGe_RESET
  | 6------------- ~ADAU1962_EN       |       | 6------------- ~ETH1_RESET
  | | 5-----------  ADAU_RESET        |       | | 5----------- ~ETH1_EN
  | | | 4---------  Not Used          |       | | | 4--------- ~MLB_EN
  | | | | 3-------  Not Used          |       | | | | 3-------  AUDIO_JACK_SEL
  | | | | | 2----- ~MicroSD_SPI       |       | | | | | 2----- ~SPDIF_OPTICAL_EN
  | | | | | | 1---  PUSHBUTTON_EN     |       | | | | | | 1--- ~SPDIF_DIGITAL_EN
  | | | | | | | 0-  EEPROM_EN         |       | | | | | | | 0-  OCTAL_SPI_CS_EN
  | | | | | | | |                     |       | | | | | | | |
  Y Y N X X N N Y                     |       N N N N Y N Y Y     ( Active Y or N )
  0 0 0 0 0 1 0 1                     |       1 1 1 1 1 1 0 1     ( value being set )
*/
  { 0x12u, 0x05u },                               { 0x13u, 0xFDu },

 /*
  * specify inputs/outputs
  */

  { 0x0u, 0x18u },   /* Set IODIRA direction  */
  { 0x1u, 0x00u },   /* Set IODIRB direction  */
};

/* switch configuration */
static SOFT_SWITCH ADC_DAC_SoftSwitch[] =
{
  {
    2u,
    0x22u,
    sizeof(ADC_DAC_SwitchConfig0)/sizeof(SWITCH_CONFIG),
    ADC_DAC_SwitchConfig0
  }
};

/* switch 0 register settings */
static SWITCH_CONFIG ADAU_SwitchConfig0[] =
{
 /*
       U6 Port A                                  U6 Port B
  7--------------- ~ADAU1979_EN       |       7--------------- ~GIGe_RESET
  | 6------------- ~ADAU1962_EN       |       | 6------------- ~ETH1_RESET
  | | 5-----------  ADAU_RESET        |       | | 5----------- ~ETH1_EN
  | | | 4---------  Not Used          |       | | | 4--------- ~MLB_EN
  | | | | 3-------  Not Used          |       | | | | 3-------  AUDIO_JACK_SEL
  | | | | | 2----- ~MicroSD_SPI       |       | | | | | 2----- ~SPDIF_OPTICAL_EN
  | | | | | | 1---  PUSHBUTTON_EN     |       | | | | | | 1--- ~SPDIF_DIGITAL_EN
  | | | | | | | 0-  EEPROM_EN         |       | | | | | | | 0-  OCTAL_SPI_CS_EN
  | | | | | | | |                     |       | | | | | | | |
  Y Y Y X X N N Y                     |       N N N N Y N Y Y     ( Active Y or N )
  0 0 1 0 0 1 0 1                     |       1 1 1 1 1 1 0 1     ( value being set )
*/
  { 0x12u, 0x25u },                               { 0x13u, 0xFDu },

 /*
  * specify inputs/outputs
  */

  { 0x0u, 0x18u },   /* Set IODIRA direction  */
  { 0x1u, 0x00u },   /* Set IODIRB direction  */
};

/* switch configuration */
static SOFT_SWITCH ADAU_SoftSwitch[] =
{
  {
    2u,
    0x22u,
    sizeof(ADAU_SwitchConfig0)/sizeof(SWITCH_CONFIG),
    ADAU_SwitchConfig0
  }
};

/* switch 0 register settings */
static SWITCH_CONFIG UART_SOM_SwitchConfig0[] =
{

 /*
       U16 Port A                                  U16 Port B
  7---------------  Not Used          |       7---------------  Not Used
  | 6------------- ~UART0_FLOW_EN     |       | 6-------------  Not Used
  | | 5----------- ~UART0_EN          |       | | 5-----------  Not Used
  | | | 4--------- ~SPI2D2_D3_EN      |       | | | 4---------  Not Used
  | | | | 3------- ~SP2IFLASH_CS_EN   |       | | | | 3-------  Not Used
  | | | | | 2-----  LED2              |       | | | | | 2-----  Not Used
  | | | | | | 1---  LED5              |       | | | | | | 1---  Not Used
  | | | | | | | 0-  LED4              |       | | | | | | | 0-  Not Used
  | | | | | | | |                     |       | | | | | | | |
  X Y Y Y Y Y Y Y                     |       X X X X X X X X     ( Active Y or N )
  0 0 0 0 0 0 0 0                     |       0 0 0 0 0 0 0 0     ( value being set )
*/
  { 0x12u, 0x00u },                               { 0x13u, 0x00u },

 /*
  * specify inputs/outputs
  */

  { 0x0u, 0x80u },   /* Set IODIRA direction  */
  { 0x1u, 0xFFu },   /* Set IODIRB direction  */
};


/* switch configuration */
static SOFT_SWITCH UART_SOM_SoftSwitch[] =
{
   {
      2u,
      0x21u,
      sizeof(UART_SOM_SwitchConfig0)/sizeof(SWITCH_CONFIG),
      UART_SOM_SwitchConfig0
   }
};

#if defined(ADI_DEBUG)
#include <stdio.h>
#define CHECK_RESULT(result, message) \
	do { \
		if((result) != ADI_TWI_SUCCESS) \
		{ \
			printf((message)); \
			printf("\n"); \
		} \
	} while (0)  /* do-while-zero needed for Misra Rule 19.4 */
#else
#define CHECK_RESULT(result, message)
#endif

void ConfigSoftSwitches(SOFT_SWITCH SoftSwitch[], uint32_t switches)
{
	uint32_t switchNum;
	uint32_t configNum;

	SOFT_SWITCH *ss;
	SWITCH_CONFIG *configReg;
	ADI_TWI_RESULT result;

   /* TWI hDevice handle */
   ADI_TWI_HANDLE hDevice;

   /* TWI data buffer */
   uint8_t twiBuffer[BUFFER_SIZE];

	for (switchNum=0u; switchNum<switches; switchNum++)
	{
		ss = &SoftSwitch[switchNum];

		result = adi_twi_Open(ss->TWIDevice, ADI_TWI_MASTER,
    		deviceMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
		CHECK_RESULT(result, "adi_twi_Open failed");

		result = adi_twi_SetHardwareAddress(hDevice, ss->HardwareAddress);
		CHECK_RESULT(result, "adi_twi_SetHardwareAddress failed");

		result = adi_twi_SetPrescale(hDevice, TWI_PRESCALE);
		CHECK_RESULT(result, "adi_twi_Prescale failed");

		result = adi_twi_SetBitRate(hDevice, TWI_BITRATE);
		CHECK_RESULT(result, "adi_twi_SetBitRate failed");

		result = adi_twi_SetDutyCycle(hDevice, TWI_DUTYCYCLE);
		CHECK_RESULT(result, "adi_twi_SetDutyCycle failed");
		
		/* switch register settings */
		for (configNum=0u; configNum<ss->NumConfigSettings; configNum++)
		{
			configReg = &ss->ConfigSettings[configNum];

			/* write register value */
			twiBuffer[0] = configReg->Register;
			twiBuffer[1] = configReg->Value;
			result = adi_twi_Write(hDevice, twiBuffer, (uint32_t)2, false);
			CHECK_RESULT(result, "adi_twi_Write failed");
		}

		result = adi_twi_Close(hDevice);
		CHECK_RESULT(result, "adi_twi_Close failed");
	}
}

void ConfigSoftSwitches_ADC_DAC(void)
{
   ConfigSoftSwitches(ADC_DAC_SoftSwitch, (uint32_t)(sizeof(ADC_DAC_SoftSwitch)/sizeof(SOFT_SWITCH)));
}

void ConfigSoftSwitches_ADAU_Reset(void)
{
   ConfigSoftSwitches(ADAU_SoftSwitch, (uint32_t)(sizeof(ADAU_SoftSwitch)/sizeof(SOFT_SWITCH)));
}

void ConfigSoftSwitches_EV_SOM(void)
{
   ConfigSoftSwitches(UART_SOM_SoftSwitch, (uint32_t)(sizeof(UART_SOM_SoftSwitch)/sizeof(SOFT_SWITCH)));
}

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */

