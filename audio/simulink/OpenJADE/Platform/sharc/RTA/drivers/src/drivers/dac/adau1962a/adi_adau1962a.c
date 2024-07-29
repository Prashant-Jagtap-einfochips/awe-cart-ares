/*********************************************************************************

Copyright(c) 2014-2015 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      adi_adau1962a.c
 * @brief     ADAU1962A DAC driver implementation.
 * @version:  $Revision: 26376 $
 * @date:     $Date: 2016-05-18 10:22:27 -0400 (Wed, 18 May 2016) $
 *
 * @details
 *            This is the primary source file for ADAU1962A DAC driver.
 *
 */

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_2_4:"Allows doxygen addtogroup block")
#pragma diag(suppress:misra_rule_13_7:"Allow 'assert' function to validate memory size")
#pragma diag(suppress:misra_rule_14_6:"Allows multiple break statements")
#pragma diag(suppress:misra_rule_14_7:"Allows multiple exit points")
#endif

/** @addtogroup ADI_ADAU1962A_Driver    ADAU1962A DAC driver
 *  @{

    <b>ADAU1962A DAC driver</b>

    The ADAU1962A is a high performance, that provides 12 digital-to-analog converters (DACs) with differential output using the
    Analog Devices, Inc. patented multibit sigma-delta architecture. The current version of ADAU1962A driver supports
    ADSP-SC58x family of processors when operated in I2S/Stereo mode (#ADI_ADAU1962A_SERIAL_MODE_STEREO).
    Though the driver has APIs for TDM mode, functionality specific to TDM mode hasn't been tested.

 */

/*=============  I N C L U D E S   =============*/

#if 0
#define TEST_REGISTER_1962A
#endif

/* Include header file with definitions specific to ADAU1962A DAC driver implementation */
#include "adi_adau1962a_def.h"

/*==============  D E F I N E S  ===============*/

/*=============  D A T A  =============*/

/* Pointer to ADAU1962A device instance(s) that are open and in use */
static ADI_ADAU1962A_DEV      *pADAU1962ALastDev = NULL;

/*=============  L O C A L    F U N C T I O N    P R O T O T Y P E S =============*/

/* Reads an ADAU1962A register */
static ADI_ADAU1962A_RESULT ReadAdau1962a (
    ADI_ADAU1962A_DEV          	*pDevice,
    uint8_t           			RegAddr,
    uint8_t                     *pRegValue);

/* Configures an ADAU1962A register */
static ADI_ADAU1962A_RESULT WriteAdau1962a (
    ADI_ADAU1962A_DEV      	    *pDevice,
    uint8_t           			RegAddr,
    uint8_t                     RegValue);

/* Opens and configures TWI Device to communicate with ADAU1962A */
static ADI_TWI_RESULT  OpenTwi(ADI_ADAU1962A_DEV    *pDevice);

/* Closes TWI Device opened to communicate with ADAU1962A */
static void CloseTwi (ADI_ADAU1962A_DEV    *pDevice);

/* Updates SPORT configuration */
static ADI_ADAU1962A_RESULT UpdateSportConfig (ADI_ADAU1962A_DEV    *pDevice);

/* Updates master power configuration */
static ADI_ADAU1962A_RESULT  UpdateMasterPower(
    ADI_ADAU1962A_DEV               *pDevice,
    ADI_ADAU1962A_DAC_PWR           eDacPwr,
    bool                            bPwrUp);

/* Initializes ADAU1962A registers with default settings */
static ADI_ADAU1962A_RESULT InitAdau1962a (ADI_ADAU1962A_DEV    *pDevice);

#ifdef TEST_REGISTER_1962A
uint8_t Register[10];
#endif
/*=============  D E B U G    F U N C T I O N    P R O T O T Y P E S  =============*/

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

/* Validates ADAU1962A device handle */
static ADI_ADAU1962A_RESULT ValidateHandle (ADI_ADAU1962A_HANDLE hDevice);

/* Checks if the supplied ADAU1962A device number is already open */
static ADI_ADAU1962A_RESULT ValidateDevNumber (uint32_t    DeviceNum);

#endif /* ADI_DEBUG */

/*=============  D R I V E R    I M P L E M E N T A T I O N    S O U R C E    F I L E S  =============*/

/*=============  C O D E  =============*/

/*=============  L O C A L    F U N C T I O N S  =============*/

/*
 * Reads an ADAU1962A register
 *
 * Parameters
 *
 * - [in]  pDevice      ADAU1962A instance to work on.
 * - [in]  RegAddr      Register address to read.
 * - [out] pRegValue    Pointer to location to store register value.
 *
 * Returns
 *  - ADI_ADAU1962A_SUCCESS: Successfully updated ADAU1962A settings.
 *  - ADI_ADAU1962A_BAD_HANDLE: Supplied Device handle is invalid.
 *
 */
static ADI_ADAU1962A_RESULT ReadAdau1962a (
    ADI_ADAU1962A_DEV      		*pDevice,
    uint8_t                     RegAddr,
    uint8_t                     *pRegValue)
{
	/* Return code */
	ADI_TWI_RESULT  eResult;

	/* Open TWI */
    eResult = OpenTwi (pDevice);

    /* IF (Success) */
    if (eResult == ADI_TWI_SUCCESS)
    {
        /* Write the first register address to access */
        eResult = adi_twi_Write(pDevice->Twi.hDev, &RegAddr, 1u, true);
    }
    /* IF (Success) */
    if (eResult == ADI_TWI_SUCCESS)
    {
        /* Read the register */
        eResult = adi_twi_Read(pDevice->Twi.hDev, pRegValue, 1u, false);
    }

    /* Close TWI */
    CloseTwi(pDevice);

    /* IF (Failed) */
    if (eResult != ADI_TWI_SUCCESS)
    {
        /* Return failure */
        return (ADI_ADAU1962A_TWI_ERROR);
    }
    else
    {
        /* Return success */
        return (ADI_ADAU1962A_SUCCESS);
    }
}

/*
 * Configures an ADAU1962A register
 *
 * Parameters
 *
 * - [in]  pDevice      ADAU1962A instance to work on.
 * - [in]  RegAddr      Register address to configure.
 * - [out] RegValue     Register value to write.
 *
 * Returns
 *  - ADI_ADAU1962A_SUCCESS: Successfully updated ADAU1962A settings.
 *  - ADI_ADAU1962A_BAD_HANDLE: Supplied Device handle is invalid.
 *
 */
static ADI_ADAU1962A_RESULT WriteAdau1962a (
    ADI_ADAU1962A_DEV      		*pDevice,
    uint8_t          	        RegAddr,
    uint8_t                 	RegValue)
{
    /* TWI data buffer */
    uint8_t         TwiBuffer[2];
	/* Return code */
    ADI_TWI_RESULT  eResult;

    /* Populate TWI data buffer */
    TwiBuffer[0] = RegAddr;
    TwiBuffer[1] = RegValue;

    /* Open TWI */
    eResult = OpenTwi (pDevice);

    /* IF (Success) */
    if (eResult == ADI_TWI_SUCCESS)
    {
        /* Write to ADAU1962A */
    	eResult = adi_twi_Write(pDevice->Twi.hDev, &TwiBuffer[0], 2u, false);
    }

    /* Close TWI */
    CloseTwi(pDevice);

    /* IF (Failed) */
    if (eResult != ADI_TWI_SUCCESS)
    {
        /* Return failure */
        return (ADI_ADAU1962A_TWI_ERROR);
    }
    else
    {
        /* Return success */
        return (ADI_ADAU1962A_SUCCESS);
    }
}

/*
 * Opens and configures TWI Device to communicate with ADAU1962A.
 *
 * Parameters
 *  - pDevice       ADAU1962A instance to work on.
 *
 * Return
 *  - ADI_ADAU1962A_SUCCESS: Successfully opened TWI device.
 *  - ADI_ADAU1962A_TWI_BANK_ADDR_INVALID: TWI Bank address invalid.
 *  - ADI_ADAU1962A_TWI_ERROR: TWI driver error.
 *
 */
static ADI_TWI_RESULT  OpenTwi(ADI_ADAU1962A_DEV    *pDevice)
{
    /* Return code */
    ADI_TWI_RESULT  eResult = ADI_TWI_SUCCESS;

    /* IF (TWI Device is not open) */
    if (pDevice->Twi.hDev == NULL)
    {
        /* Open the TWI Device */
        eResult = adi_twi_Open ((uint32_t)pDevice->Twi.Info.TwiDevNum,
                                ADI_TWI_MASTER,
                                pDevice->Twi.Info.pTwiDevMem,
                                pDevice->Twi.Info.TwiDevMemSize,
                                &(pDevice->Twi.hDev));

        /*-- Configure TWI driver --*/

        /* IF (Success) */
        if (eResult == ADI_TWI_SUCCESS)
        {
            /* Set TWI Prescale */
            eResult = adi_twi_SetPrescale(pDevice->Twi.hDev, ADI_ADAU1962A_TWI_PRESCALE);

            /* IF (Success) */
            if (eResult == ADI_TWI_SUCCESS)
            {
                /* Set TWI Duty Cycle */
                eResult = adi_twi_SetDutyCycle(pDevice->Twi.hDev, ADI_ADAU1962A_TWI_DUTYCYCLE);
            }
            /* IF (Success) */
            if (eResult == ADI_TWI_SUCCESS)
            {
                /* Set TWI Bit Rate */
                eResult = adi_twi_SetBitRate(pDevice->Twi.hDev, ADI_ADAU1962A_TWI_BITRATE);
            }

            /* IF (Success) */
            if (eResult == ADI_TWI_SUCCESS)
            {
                /* Set TWI address to access */
                eResult = adi_twi_SetHardwareAddress(pDevice->Twi.hDev, (uint16_t) pDevice->Twi.Info.eTwiAddr);
            }

            /* IF (Error) */
            if (eResult != ADI_TWI_SUCCESS)
            {
                /* Close TWI Device */
                CloseTwi (pDevice);
            }
        }
    }

    /* Return */
    return (eResult);
}

/*
 * Closes TWI Device opened to communicate with ADAU1962A.
 *
 * Parameters
 *  - pDevice    ADAU1962A instance to work on.
 *
 * Returns
 *  - None
 *
 */
static void  CloseTwi(ADI_ADAU1962A_DEV  *pDevice)
{
    /* IF (TWI Device is already open) */
    if (pDevice->Twi.hDev != NULL)
    {
        /* Close TWI Device */
        adi_twi_Close(pDevice->Twi.hDev);
        /* Clear the TWI Device handle */
        pDevice->Twi.hDev = NULL;
    }
}

/*
 * Updates SPORT configuration.
 *
 * Parameters
 *
 * - [in] pDevice       ADAU1962A instance to work on.
 *
 * Returns
 *  - ADI_ADAU1962A_SUCCESS: Successfully initialized ADAU1962A registers.
 *  - ADI_ADAU1962A_SPORT_ERROR: SPORT driver error.
 *
 */
static ADI_ADAU1962A_RESULT UpdateSportConfig (ADI_ADAU1962A_DEV    *pDevice)
{

	uint8_t RegData;
	uint8_t TDM_Mode;

	ADI_ADAU1962A_RESULT    eResult = ADI_ADAU1962A_SUCCESS;

    /* SPORT device mapped to this serial data port */
    if (pDevice->hSportDev != NULL)
    {
        /* Configure SPORT clock - use external clock */
        if(adi_sport_ConfigClock(pDevice->hSportDev,
        		                 1u,
        		                 false,
        		                 pDevice->bBClkFallLatch,
        		                 false) != ADI_SPORT_SUCCESS)
        {
        	/* SPORT driver error */
        	return (ADI_ADAU1962A_SPORT_ERROR);
        }

        /* IF (SPORT configured for Stereo mode) */
        if (pDevice->eSerialMode == ADI_ADAU1962A_SERIAL_MODE_STEREO)
        {
            /* Configure SPORT channel order */
            if(adi_sport_SelectChannelOrder(pDevice->hSportDev,
            		                        pDevice->bLeftChnlClkHi) != ADI_SPORT_SUCCESS)
            {
            	/* SPORT driver error */
            	return (ADI_ADAU1962A_SPORT_ERROR);
            }

            /* Configure SPORT data length, LSB first etc */
            if(adi_sport_ConfigData (pDevice->hSportDev,
                                     ADI_SPORT_DTYPE_SIGN_FILL,
            	                     pDevice->WordWidth,
            	                     pDevice->bLsbFirst,
            	                     false,
            	                     false) != ADI_SPORT_SUCCESS)
            {
                /* SPORT driver error */
            	return (ADI_ADAU1962A_SPORT_ERROR);
            }

    	    /* Configure Frame Sync - Frame Sync required, External Frame sync */
    	    if(adi_sport_ConfigFrameSync(pDevice->hSportDev,
    	    		                     100u,
    	    		                     true,
    	    		                     false,
    	    		                     false,
    	    		                     false,
    	    		                     false,
    	    		                     false) != ADI_SPORT_SUCCESS)
    	    {
    	    	/* SPORT driver error */
    	    	return (ADI_ADAU1962A_SPORT_ERROR);
    	    }
        }
        /* ELSE (Multichannel mode) */
        else
        {
            /* Configure SPORT data length, LSB first etc */
            if(adi_sport_ConfigData (pDevice->hSportDev,
                                     ADI_SPORT_DTYPE_SIGN_FILL,
                                     31u,
            	                     pDevice->bLsbFirst,
            	                     false,
            	                     false) != ADI_SPORT_SUCCESS)
            {
                /* SPORT driver error */
            	return (ADI_ADAU1962A_SPORT_ERROR);
            }

            /* Configure Frame Sync - Frame Sync required, External Frame sync */
    	    if(adi_sport_ConfigFrameSync(pDevice->hSportDev,
    	    		                     100u,
    	    		                     true,
    	    		                     false,
    	    		                     true,
    	    		                     false,
    	    		                     false,
    	    		                     false) != ADI_SPORT_SUCCESS)
    	    {
    	    	/* SPORT driver error */
    	    	return (ADI_ADAU1962A_SPORT_ERROR);
    	    }

    	    /* Configure multi-channel register for TDM mode */
            if (adi_sport_ConfigMC(pDevice->hSportDev,	   
        	 		               0u,
        	  		               (uint8_t) pDevice->eSerialMode,
        	  		               0u,
        	  		               false) != ADI_SPORT_SUCCESS)
        	{
        	    /* SPORT driver error */
    	    	return (ADI_ADAU1962A_SPORT_ERROR);
            }

            /* determine which channel to configure  */
            switch (pDevice->eSerialMode)
            {
                case ADI_ADAU1962A_SERIAL_MODE_TDM2:
                	TDM_Mode     = ENUM_ADAU1962A_06_SAI_TDM2;
                    break;
                case ADI_ADAU1962A_SERIAL_MODE_TDM4:
                	TDM_Mode     = ENUM_ADAU1962A_06_SAI_TDM4;
                    break;
                case ADI_ADAU1962A_SERIAL_MODE_TDM8:
                	TDM_Mode     = ENUM_ADAU1962A_06_SAI_TDM8;
                    break;
                case ADI_ADAU1962A_SERIAL_MODE_TDM16:
                	TDM_Mode     = ENUM_ADAU1962A_06_SAI_TDM16;
                    break;
                default:
                    eResult =  ADI_ADAU1962A_FN_NOT_SUPPORTED;
                    break;
            }

            if(eResult == ADI_ADAU1962A_SUCCESS)
            {
                /* Read DAC CTL0 register */
                eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_DAC_CTRL0, &RegData);

                /* IF (Success) */
                if (eResult == ADI_ADAU1962A_SUCCESS)
                {
                    /* Clear and update register */
                    RegData &= ~BITM_ADAU1962A_06_SAI;
                    /* Update register configuration */
                    RegData |= TDM_Mode;

                    /* Write the updated register configuration */
                    eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_DAC_CTRL0, RegData);
                }
            }
        }
    }


	/* Return */
	return (eResult);
}

/*
 * Updates master power configuration.
 *
 * Parameters
 *
 * - [in] pDevice       ADAU1962A instance to work on.
 * - [in] eDacPwr       Power configuration.
 * - [in] bPwrUp        'true' for Master power-up, 'false' for power-down.
 *
 * Returns
 *  - ADI_ADAU1962A_SUCCESS: Successfully configured ADAU1962A registers.
 *
 */
static ADI_ADAU1962A_RESULT  UpdateMasterPower(
    ADI_ADAU1962A_DEV               *pDevice,
    ADI_ADAU1962A_DAC_PWR           eDacPwr,
    bool                            bPwrUp)
{
    /* Register configuration data */
    uint8_t               	RegData;
    /* Return Code */
    ADI_ADAU1962A_RESULT    eResult;

    /* Read PLL clock control 0 register */
    eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL0, &RegData);

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear Master Power-Up Control */
        RegData &= ~BITM_ADAU1962A_00_MSTR_PWR;

        /* IF (Master Power-Up) */
        if (bPwrUp)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_00_MSTR_PWR_UP;
        }

        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL0, RegData);
    }

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Read PLL clock control 1 register */
        eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL1, &RegData);

        /* IF (Success) */
        if (eResult == ADI_ADAU1962A_SUCCESS)
        {
            /* Clear global power adjust - I2C register settings for individual DAC power control */
            RegData &= ~BITM_ADAU1962A_01_LOPWR_MODE;

            /* IF (Low power mode) */
            if (eDacPwr == ADI_ADAU1962A_DAC_PWR_LOW)
            {
                /* Configure for low power */
                RegData |= ENUM_ADAU1962A_01_LOPWR_MODE_LOWER;
            }

            /* IF (Lowest power mode) */
            if (eDacPwr == ADI_ADAU1962A_DAC_PWR_LOWEST)
            {
                /* Configure for lowest power */
                RegData |= ENUM_ADAU1962A_01_LOPWR_MODE_LOWEST;
            }
        }

        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL1, RegData);
    }

    /* Return */
    return (eResult);
}

/*
 * Initializes ADAU1962A registers with default settings.
 *
 * Parameters
 *
 * - [in] pDevice       ADAU1962A instance to work on.
 *
 * Returns
 *  - ADI_ADAU1962A_SUCCESS: Successfully initialized ADAU1962A registers.
 *
 */
static ADI_ADAU1962A_RESULT InitAdau1962a (ADI_ADAU1962A_DEV    *pDevice)
{
    /* Use power-up/reset defaults */
    return (ADI_ADAU1962A_SUCCESS);
}

/*=============  D E B U G   F U N C T I O N S =============*/

/* IF (Debug mode enabled) */
#if defined (ADI_DEBUG)

/*
 * Validates ADAU1962A device handle.
 *
 * Parameters
 *  - [in]  hDevice    ADAU1962A handle to validate.
 *
 * Returns:  Status
 *  - ADI_ADAU1962A_SUCCESS: Successfully validated ADAU1962A device handle.
 *  - ADI_ADAU1962A_BAD_HANDLE: Invalid Device Handle.
 *
 */
static ADI_ADAU1962A_RESULT ValidateHandle (ADI_ADAU1962A_HANDLE hDevice)
{
    /* Pointer to the device instance to validate */
    ADI_ADAU1962A_DEV     *pDeviceToValidate = (ADI_ADAU1962A_DEV *) hDevice;
    /* Last open ADAU1962A instance in chain */
    ADI_ADAU1962A_DEV     *pDeviceInChain;

    /* FOR (ADAU1962A instances in chain) */
    for (pDeviceInChain = pADAU1962ALastDev; pDeviceInChain != NULL; pDeviceInChain = pDeviceInChain->pPrevious)
    {
        /* IF (Supplied handle is valid) */
        if (pDeviceToValidate == pDeviceInChain)
        {
            /* This is a valid device */
            return (ADI_ADAU1962A_SUCCESS);
        }
    }

    /* This device handle must be invalid */
    return (ADI_ADAU1962A_BAD_HANDLE);
}

/*
 * Checks if the supplied ADAU1962A device number is already open.
 *
 * Parameters
 *  - [in]  DeviceNum    ADAU1962A device number to validate.
 *
 * Returns:  Status
 *  - ADI_ADAU1962A_SUCCESS: Successfully validated ADAU1962A device number.
 *  - ADI_ADAU1962A_IN_USE: Device number is already in use.
 *
 */
static ADI_ADAU1962A_RESULT ValidateDevNumber (uint32_t    DeviceNum)
{
    /* ADAU1962A instance in chain */
    ADI_ADAU1962A_DEV     *pDeviceInChain;

    /* FOR (All Open ADAU1962A instances in chain) */
    for (pDeviceInChain = pADAU1962ALastDev; pDeviceInChain != NULL; pDeviceInChain = pDeviceInChain->pPrevious)
    {
        /* IF (Supplied device number is already in use) */
        if (pDeviceInChain->DevNum == DeviceNum)
        {
            /* This device number is already in use*/
            return (ADI_ADAU1962A_IN_USE);
        }
    }

    /* This device number is not in use */
    return (ADI_ADAU1962A_SUCCESS);
}

/* End of debug functions */
#endif /* ADI_DEBUG */

/*=============  P U B L I C   F U N C T I O N S =============*/

/**
 * @brief Opens ADAU1962A device instance for use.
 *
 * @details     Opens the given ADAU1962A device instance and returns
 *              the handle to the opened device. The returned handle should
 *              be used in all the other APIs.
 *
 * @param [in]  DeviceNum       ADAU1962A instance number to open.
 * @param [in]  eSerialMode     Serial data mode of this ADAU1962A instance.
 * @param [in]  pDeviceMemory   Pointer to #ADI_ADAU1962A_MEMORY_SIZE sized memory
 *                              location to handle the device instance.
 * @param [in]  MemorySize      Size of the buffer to which "pDeviceMemory" points.
 * @param [out] phDevice        Address where the ADAU1962A Device handle will be stored.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully opened ADAU1962A instance.
 *          - #ADI_ADAU1962A_NULL_POINTER: Supplied memory address is invalid.
 *          - #ADI_ADAU1962A_INSUFFICIENT_MEMORY: Supplied memory is not sufficient to manage the device.
 *          - #ADI_ADAU1962A_IN_USE: Device number is already in use.
 *
 * @sa  adi_adau1962a_Close()
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_Open(
    uint8_t                                 DeviceNum,
    ADI_ADAU1962A_SERIAL_MODE               eSerialMode,
    void                            *const  pDeviceMemory,
    uint32_t                                MemorySize,
    ADI_ADAU1962A_HANDLE            *const  phDevice)
{
    /* Pointer to the device instance memory to work on */
    ADI_ADAU1962A_DEV     *pDevice = pDeviceMemory;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (This Device Number is already open) */
    if (ValidateDevNumber ((uint32_t)DeviceNum) != ADI_ADAU1962A_SUCCESS)
    {
        /* Report failure as device in use */
        return (ADI_ADAU1962A_IN_USE);
    }

    /* Check if the given pointer parameters are valid */
    if((pDeviceMemory == NULL) || (phDevice == NULL))
    {
        return (ADI_ADAU1962A_NULL_POINTER);
    }

    /* Check if the given memory is insufficient */
    if(MemorySize < ADI_ADAU1962A_MEMORY_SIZE)
    {
        return (ADI_ADAU1962A_INSUFFICIENT_MEMORY);
    }

    /* Asserts to validate ADAU1962A device memory size */
    assert(ADI_ADAU1962A_MEMORY_SIZE >= sizeof(ADI_ADAU1962A_DEV));

#endif /* ADI_DEBUG */

    /* Clear the given memory */
    memset(pDeviceMemory, 0, MemorySize);

    /* Initialize the device instance memory */
    pDevice->DevNum      = DeviceNum;
    pDevice->WordWidth   = (uint8_t) ADI_ADAU1962A_WORD_WIDTH_24;
    pDevice->eSerialMode = eSerialMode;

    /* Protect this section of code - entering a critical region */
    adi_osal_EnterCriticalRegion();

    /* Add this instance to ADAU1962A device chain */
    pDevice->pPrevious = pADAU1962ALastDev;
    /* IF (This is not the first instance to be opened) */
    if (pADAU1962ALastDev != NULL)
    {
        /* Chain this device to the last instance in chain */
        pADAU1962ALastDev->pNext = pDevice;
    }
    /* Make this device as last instance in chain */
    pADAU1962ALastDev = pDevice;

    /* Exit the critical region */
    adi_osal_ExitCriticalRegion();

    /* Pass the physical device handle to client */
    *phDevice = (ADI_ADAU1962A_HANDLE *)pDevice;

    /* Return */
    return(ADI_ADAU1962A_SUCCESS);
}

/**
 * @brief Closes ADAU1962A instance.
 *
 * @details     Closes ADAU1962A driver instance and disables the data flow.
 *              Once the drive is closed, it is not allowed access any of the other
 *              APIs as the device handle will not be valid anymore.
 *
 * @param [in]  hDevice     ADAU1962A instance to close.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully closed ADAU1962A instance.
 *          - #ADI_ADAU1962A_BAD_HANDLE: Supplied Device handle is invalid.
 *          - #ADI_ADAU1962A_SPORT_ERROR: SPORT Driver Error.
 *
 * @sa  adi_adau1962a_Open()
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_Close(
    ADI_ADAU1962A_HANDLE           const   hDevice)
{
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV     *pDevice = (ADI_ADAU1962A_DEV *) hDevice;
    /* Return code */
    ADI_ADAU1962A_RESULT  eResult = ADI_ADAU1962A_SUCCESS;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif

    /* Disconnect SPORT device from ADAU1962A DAC */

    /* IF (we've a valid SPORT device handle) */
    if (pDevice->hSportDev != NULL)
    {
        /* Close SPORT device */
        adi_sport_Close (pDevice->hSportDev);
        pDevice->hSportDev = NULL;
    }

    /* IF (Failed to disconnect SPORT) */
    if (eResult != ADI_ADAU1962A_SUCCESS)
    {
        return (eResult);
    }

    /*-- Remove this device from ADAU1962A device in use chain --*/

    /* Protect this section of code - entering a critical region    */
    adi_osal_EnterCriticalRegion();

    /* IF (This is the last device instance in chain) */
    if (pADAU1962ALastDev == pDevice)
    {
        /* Make the previous instance linked to this device as last in chain */
        pADAU1962ALastDev = pDevice->pPrevious;
        if (pDevice->pPrevious != NULL)
        {
        	/* Remove the device instance from list */
        	pDevice->pPrevious->pNext = NULL;
        }
    }
    /* ELSE IF (This is the first device in chain) */
    else if (pDevice->pPrevious == NULL)
    {
    	/* IF (we've more than one device in chain) */
    	if (pDevice->pNext != NULL)
    	{
    		/* Remove this instance from the chain */
    	    pDevice->pNext->pPrevious = NULL;
    	}
    }
    /* ELSE (This device is in the middle of chain) */
    else
    {
        /* Remove this instance from the chain */
        pDevice->pPrevious->pNext = pDevice->pNext;
        pDevice->pNext->pPrevious = pDevice->pPrevious;
    }

    /* Exit the critical region */
    adi_osal_ExitCriticalRegion();

    /* Return */
    return(ADI_ADAU1962A_SUCCESS);
}

/**
 * @brief Configure the TWI device used to access ADAU1962A registers.
 *
 * @details     Configures the TWI device number and TWI slave address to use while
 *              accessing ADAU1962A hardware registers. When a valid TWI configuration
 *              is provided, this function configures the corresponding ADAU1962A device
 *              registers with Stereo (I2S) mode default values.
 *
 * @param [in]  hDevice     ADAU1962A instance to work on.
 * @param [in]  *pConfig    Pointer to information specific to TWI.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully configured TWI for ADAU1962A.
 *          - #ADI_ADAU1962A_BAD_HANDLE: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_ConfigTwi(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_TWI_CONFIG                *pConfig)
{
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV     *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

    /* IF (TWI Slave address is invalid) */
    if ((pConfig->eTwiAddr != ADI_ADAU1962A_TWI_ADDR_04) &&\
        (pConfig->eTwiAddr != ADI_ADAU1962A_TWI_ADDR_24) &&\
        (pConfig->eTwiAddr != ADI_ADAU1962A_TWI_ADDR_44) &&\
        (pConfig->eTwiAddr != ADI_ADAU1962A_TWI_ADDR_64))
    {
        return (ADI_ADAU1962A_TWI_NOT_CONFIGURED);
    }

    /* IF (ADAU1962A dataflow enabled) */
    if (pDevice->bIsEnabled)
    {
        /* Report failure as ADAU1962A is already running */
        return (ADI_ADAU1962A_ALREADY_RUNNING);
    }

#endif /* ADI_DEBUG */

    /* Save the TWI device number and slave address to access ADAU1962A device */
    pDevice->Twi.Info.TwiDevNum     = pConfig->TwiDevNum;
    pDevice->Twi.Info.eTwiAddr      = pConfig->eTwiAddr;
    pDevice->Twi.Info.pTwiDevMem    = pConfig->pTwiDevMem;
    pDevice->Twi.Info.TwiDevMemSize = pConfig->TwiDevMemSize;

    /* Configure device with default settings */
    return (InitAdau1962a (pDevice));
}

/**
 * @brief Configure SPORT device to be used to transmit audio data to ADAU1962A.
 *
 * @details     This function can be used to pass SPORT configuration for DAC channels.
 *
 * @param [in]  hDevice     ADAU1962A instance to work on.
 * @param [in]  *pConfig    Pointer to information specific to SPORT.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully configured SPORT for ADAU1962A.
 *          - #ADI_ADAU1962A_BAD_HANDLE: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT   adi_adau1962a_ConfigSport (
    ADI_ADAU1962A_HANDLE           const    hDevice,
    ADI_ADAU1962A_SPORT_CONFIG              *pConfig)
{
    /* SPORT return code */
    ADI_SPORT_RESULT        eSportResult;
    /* SPORT data mode */
    ADI_SPORT_MODE			eSportMode;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* IF (SPORT config instance points to NULL and we've a valid SPORT device handle) */
    if ((pConfig == NULL) && (pDevice->hSportDev != NULL))
    {
        /* Close SPORT device */
        adi_sport_Close (pDevice->hSportDev);
        pDevice->hSportDev = NULL;
        return (ADI_ADAU1962A_SUCCESS);
    }

    /* IF (SPORT already mapped) */
    if (pDevice->hSportDev != NULL)
    {
        return (ADI_ADAU1962A_SERIAL_PORT_CONFIGURED);
    }

    /* IF (SPORT primary is mapped to an invalid Serial data port) */
    if ((pConfig->eSportPri != ADI_ADAU1962A_SERIAL_PORT_DSDATA1) &&\
        (pConfig->eSportPri != ADI_ADAU1962A_SERIAL_PORT_DSDATA2))
    {
        /* Primary SPORT channel should be mapped to a serial data port */
        return (ADI_ADAU1962A_SPORT_CONFIG_INVALID);
    }

    /* IF (SPORT secondary is mapped to an invalid Serial data port) */
    if ((pConfig->eSportSec == pConfig->eSportPri) ||\
        ((pConfig->eSportSec != ADI_ADAU1962A_SERIAL_PORT_DSDATA1) &&\
         (pConfig->eSportSec != ADI_ADAU1962A_SERIAL_PORT_DSDATA2) &&\
         (pConfig->eSportSec != ADI_ADAU1962A_SERIAL_PORT_NONE)))
    {
        /* Secondary SPORT channel mapping invalid */
        return (ADI_ADAU1962A_SPORT_CONFIG_INVALID);
    }

    /* IF (Device opened for Stereo mode) */
    if (pDevice->eSerialMode == ADI_ADAU1962A_SERIAL_MODE_STEREO)
    {
        /* SPORT data mode should be I2S */
        eSportMode = ADI_SPORT_I2S_MODE;
    }
    /* ELSE (Device operated in TDM mode) */
    else
    {
        /* SPORT data mode should be Multi-channel */
        eSportMode = ADI_SPORT_MC_MODE;
    }

    /* Open the SPORT device */
    eSportResult = adi_sport_Open ((uint32_t)pConfig->SportDevNum,
            					   (ADI_SPORT_CHANNEL) pConfig->eSportChnl,
                                   ADI_SPORT_DIR_TX,
                                   eSportMode,
                                   pConfig->pSportDevMem,
                                   pConfig->SportDevMemSize,
                                   &(pDevice->hSportDev));

    /* IF (Successfully opened SPORT) */
    if (eSportResult == ADI_SPORT_SUCCESS)
    {
        /* Enable DMA mode */
        eSportResult = adi_sport_EnableDMAMode(pDevice->hSportDev, true);
    }

    /* IF (Successfully enabled DMA mode) */
    if (eSportResult == ADI_SPORT_SUCCESS)
    {
        /* Enable streaming mode */
        eSportResult = adi_sport_StreamingEnable(pDevice->hSportDev, true);
    }

    /* IF (Successfully enabled DMA mode) */
    if (eSportResult == ADI_SPORT_SUCCESS)
    {
        /* Set DMA Transfer size */
        eSportResult = adi_sport_SetDmaTransferSize(pDevice->hSportDev, ADI_SPORT_DMA_TRANSFER_4BYTES);
    }

    /* IF (Successfully set DMA Transfer size and SPORT secondary is mapped to a valid Serial data port) */
    if ((eSportResult == ADI_SPORT_SUCCESS) && (pConfig->eSportSec != ADI_ADAU1962A_SERIAL_PORT_NONE))
    {
        /* Update secondary channel flag */
        pDevice->bEnableSec = true;
    }
    else
    {
        /* Update secondary channel flag */
        pDevice->bEnableSec = false;
    }

    /* IF (Successfully enabled secondary channel) */
    if ((eSportResult == ADI_SPORT_SUCCESS) && (pDevice->pfCallback != NULL))
    {
        /* Register callback with SPORT */
        eSportResult = adi_sport_RegisterCallback (pDevice->hSportDev, pDevice->pfCallback, pDevice->pCBParam);
    }

    /* IF (Failed to configure SPORT) */
    if (eSportResult != ADI_SPORT_SUCCESS)
    {
        /* SPORT driver error */
        return (ADI_ADAU1962A_SPORT_ERROR);
    }
    else
    {
        return (ADI_ADAU1962A_SUCCESS);
    }
}

/**
 * @brief Configures ADAU1962A PLL and Master clock settings.
 *
 * @param [in]  hDevice         ADAU1962A instance to work on.
 * @param [in]  MClkInFreqHz    Current Master Clock input frequency driving ADAU1962A in Hertz.
 * @param [in]  eMClkSelect     Master clock source select.
 * @param [in]  ePllInput       On-chip phase locked loop (PLL) input selection.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully configured ADAU1962A clock settings.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_ConfigPllClk(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    uint32_t                                MClkInFreqHz,
    ADI_ADAU1962A_MCLK_SEL                  eMClkSelect,
    ADI_ADAU1962A_PLL_IN                    ePllInput)
{
    /* Register configuration data */
    uint8_t                  RegData;
    /* Return Code */
    ADI_ADAU1962A_RESULT    eResult;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

    /* IF (MCLK In frequency is out of range) */
    if ((MClkInFreqHz < ADI_ADAU1962A_MIN_MCLK_IN) || (MClkInFreqHz > ADI_ADAU1962A_MAX_MCLK_IN))
    {
        return (ADI_ADAU1962A_MCLK_INVLAID);
    }

#endif /* ADI_DEBUG */

    /* Save MCLK */
    pDevice->MClkInFreq = MClkInFreqHz;

    /* Read PLL clock control 1 register */
    eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL1, &RegData);

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear DAC Clock select */
        RegData &= ~BITM_ADAU1962A_01_DAC_CLK_SEL;

        /* IF (MCLK from MCLKI or XTALI) */
        if (eMClkSelect == ADI_ADAU1962A_MCLK_SEL_MCLKI_XTALI)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_01_DAC_CLK_MCLKI;
        }

        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL1, RegData);
    }

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Read PLL clock control 0 register */
        eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL0, &RegData);
    }

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear PLL Input select */
        RegData &= ~BITM_ADAU1962A_00_PLL_IN;

        /* IF (DAC LR Clock as PLL source) */
        if (ePllInput == ADI_ADAU1962A_PLL_IN_DLRCLK)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_00_PLL_IN_DLRCLK;
        }

        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL0, RegData);
    }

    /* Return */
    return (eResult);
}

/**
 * @brief Configure ADAU1962A Serial data clock settings.
 *
 * @param [in]  hDevice             ADAU1962A instance to work on.
 * @param [in]  bClkMaster          'true' for DLRCLK/DBCLK as Master,
 *                                  'false' for DLRCLK/DBCLK as Slave.
 * @param [in]  bBClkInternal       'true' to generate DBCLK internally when PLL is clocked by DLRCLK,
 *                                  'false' to use external DBCLK.
 * @param [in]  bBClkFallLatch      'true' to latch data in DBCLK falling edge,
 *                                  'false' to latch data in DBCLK raising edge.
 * @param [in]  bLRClkPulse         'true' to generate LRCLK pulse for TDM,
 *                                  'false' for 50% duty cycle.
 * @param [in]  bLsbFirst           'true' for LSB first on DSDATA,
 *                                  'false' for MSB first.
 * @param [in]  bLeftChnlClkHi      'true' for Left/Odd channels when LRCLK is High (inverted),
 *                                  'false' for Left/Odd channels when LRCLK is Low (Normal).
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully configured Serial data clock settings.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_ConfigSerialClk(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    bClkMaster,
    bool                                    bBClkInternal,
    bool                                    bBClkFallLatch,
    bool                                    bLRClkPulse,
    bool                                    bLsbFirst,
    bool                                    bLeftChnlClkHi)
{
    /* Register configuration data */
    uint8_t                  RegData;
    /* Return Code */
    ADI_ADAU1962A_RESULT    eResult;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* Read DAC Control 1 register */
    eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_DAC_CTRL1, &RegData);

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear all fields but DBCLK Rate */
        RegData &= BITM_ADAU1962A_07_BCLK_RATE;

        /* IF (DLRCLK/DBCLK Master) */
        if (bClkMaster)
        {
            RegData |= ENUM_ADAU1962A_07_SAI_MASTER;
        }
        /* IF (Latch on Falling Edge) */
        if (bBClkFallLatch)
        {
            RegData |= ENUM_ADAU1962A_07_BCLK_FALLING_EDGE;
        }
        /* IF (LSB First DSDATA) */
        if (bLsbFirst)
        {
            RegData |= ENUM_ADAU1962A_07_SAI_LSB_FIRST;
        }
        /* IF (Left/Odd channels are DLRCLK High (Inverted)) */
        if (bLeftChnlClkHi)
        {
            RegData |= ENUM_ADAU1962A_07_LRCLK_POL_INVERT;
        }
        /* IF (LRCLK Pulse mode) */
        if (bLRClkPulse)
        {
            RegData |= ENUM_ADAU1962A_07_LRCLK_PULSE;
        }
        /* IF (Internal DBCLK Generation) */
        if (bBClkInternal)
        {
            RegData |= ENUM_ADAU1962A_07_BCLK_GEN_INTERNAL;
        }

        /* Update driver cache */
        pDevice->bLsbFirst      = bLsbFirst;
        pDevice->bLeftChnlClkHi = bLeftChnlClkHi;
        pDevice->bBClkFallLatch = bBClkFallLatch;

        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_DAC_CTRL1, RegData);
    }

    /* Return */
    return (eResult);
}
/**
 * @brief Configure ADAU1962A Master clock output settings.
 *
 * @param [in]  hDevice         ADAU1962A instance to work on.
 * @param [in]  bEnableXtalOut  'true' to enable crystal inverter output, 'false' to disable
 * @param [in]  eMClkOut        Master clock output configuration.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully configured ADAU1962A Master clock out settings.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_ConfigMClkOut(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    bEnableXtalOut,
    ADI_ADAU1962A_MCLK_OUT                  eMClkOut)
{
    /* Register configuration data */
    uint8_t                  RegData;
    /* Return Code */
    ADI_ADAU1962A_RESULT    eResult;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* Read PLL clock control 1 register */
    eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL1, &RegData);

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear MCLK out select */
        RegData &= ~BITM_ADAU1962A_01_MCLKO_SEL;
        /* Update MCLK out select */
        RegData |= (uint8_t) eMClkOut;

        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL1, RegData);
    }

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Read PLL clock control 0 register */
        eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL0, &RegData);
    }

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear XTALO pin status */
        RegData &= ~BITM_ADAU1962A_00_XTAL_SET;

        /* IF (Disable XTAL Oscillator) */
        if (bEnableXtalOut == false)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_00_XTAL_OSC_OFF;
        }

        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL0, RegData);
    }

    /* Return */
    return (eResult);
}

/**
 * @brief Power-up/Power-down individual blocks of ADAU1962A.
 *
 * @param [in]  hDevice             ADAU1962A instance to work on.
 * @param [in]  bTempSensorPwrUp    'true' to power up temperature sensor, 'false' to power down.
 * @param [in]  bPllPwrUp           'true' to power up PLL, 'false' to power down.
 * @param [in]  bVRegPwrUp          'true' to power up voltage regulator, 'false' to power down voltage regulator.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully configured ADAU1962A registers.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT  adi_adau1962a_ConfigBlockPwr(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    bTempSensorPwrUp,
    bool                                    bPllPwrUp,
    bool                                    bVRegPwrUp)
{
    /* Register configuration data */
    uint8_t                  RegData;
    /* Return Code */
    ADI_ADAU1962A_RESULT    eResult;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* Block Power-down and Thermal sensor control 1 register */
    eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PDN_THRMSENS_CTRL_1, &RegData);

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear power control register fields */
        RegData &= ~BITM_ADAU1962A_02_PDN_CTRL;

        /* IF (Power down temperature sensor) */
        if (bTempSensorPwrUp == false)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_02_TS_PDN_PDN;
        }
        /* IF (Power down PLL) */
        if (bPllPwrUp == false)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_02_PLL_PDN_PDN;
        }
        /* IF (Power-Down Voltage Regulator) */
        if (bVRegPwrUp == false)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_02_VREG_PDN_PDN;
        }

        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PDN_THRMSENS_CTRL_1, RegData);
    }

    /* Return */
    return (eResult);
}

/**
 * @brief Configure ADAU1962A De-Emphasis settings.
 *
 * @param [in]  hDevice             ADAU1962A instance to work on.
 * @param [in]  bInvertOut          'true' for Inverted DAC output.
 * @param [in]  bEnableAutoMute     'true' to enable Auto-Zero input mute.
 * @param [in]  bOverSample128      'true' for 128 x fs DAC oversampling.
 * @param [in]  bEnableDeEmphasis   'true' to enable De-Emphasis.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully configured ADAU1962A register settings.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT  adi_adau1962a_ConfigDeEmphasis(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    bInvertOut,
    bool                                    bEnableAutoMute,
    bool                                    bOverSample128,
    bool                                    bEnableDeEmphasis)
{
    /* Register configuration data */
    uint8_t                  RegData;
    /* Return Code */
    ADI_ADAU1962A_RESULT    eResult;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* DAC Control 2 register */
    eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_DAC_CTRL2, &RegData);

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear Master Power-Up Control */
        RegData &= BITM_ADAU1962A_08_BCLK_TDMC;

        /* IF (De-Emphasis Enabled) */
        if (bEnableDeEmphasis)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_08_DE_EMP_EN;
        }
        /* IF (128 × fs DAC Oversampling) */
        if (bOverSample128)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_08_DAC_OSR_128;
        }
        /* IF (Enable Auto-Zero Input Mute) */
        if (bEnableAutoMute)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_08_AUTO_MUTE_EN;
        }
        /* IF (Enable Inverted DAC Output) */
        if (bInvertOut)
        {
            /* Update register configuration */
            RegData |= ENUM_ADAU1962A_08_DAC_POL_INVERT;
        }

        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_DAC_CTRL2, RegData);
    }

    /* Return */
    return (eResult);
}

/**
 * @brief Configure ADAU1962A DAC power setting
 *
 * @param [in]  hDevice         ADAU1962A instance to work on.
 * @param [in]  eDacChnl        DAC channel to update.
 * @param [in]  eDacPwr         DAC power configuration.
 * @param [in]  bPwrUp          'true' to power-up DAC channel, 'false' to power-down.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully configured MCLK and PLL settings for ADAU1962A.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1962A_ALREADY_RUNNING [D]: This function should not be called when dataflow is enabled.
 *
 */
ADI_ADAU1962A_RESULT  adi_adau1962a_ConfigDacPwr(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_CHNL                      eDacChnl,
    ADI_ADAU1962A_DAC_PWR                   eDacPwr,
    bool                                    bPwrUp)
{
    /* Register address to update */
    uint8_t                  RegAddr;
    /* Register configuration data and shift count for mask */
    uint8_t                  RegData, ShiftCount;
    /* Return Code */
    ADI_ADAU1962A_RESULT    eResult;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* IF (Configure Master power control) */
    if (eDacChnl == ADI_ADAU1962A_CHNL_DAC_MSTR)
    {
        /* Update Master power control */
        return (UpdateMasterPower (pDevice, eDacPwr, bPwrUp));
    }

    /*
     * Update Power configuration for individual DAC channel
     */

    /* IF (Power configuration invalid for individual DAC channel) */
    if (eDacPwr == ADI_ADAU1962A_DAC_PWR_INDIVIDUAL)
    {
        /* revert to default */
        eDacPwr = ADI_ADAU1962A_DAC_PWR_LOW;
    }

    /* IF (DAC 1 to 4) */
    if (eDacChnl <= ADI_ADAU1962A_CHNL_DAC_4)
    {
        /* Should update DAC power 1 register */
        RegAddr = (uint8_t) ADI_ADAU1962A_REG_DAC_POWER1;
        /* Calculate shift count */
        ShiftCount = (uint8_t) eDacChnl;
    }
    /* ELSE IF (DAC 4 to 8) */
    else if (eDacChnl <= ADI_ADAU1962A_CHNL_DAC_8)
    {
        int8_t Diff4_8 = (int8_t) (eDacChnl - ADI_ADAU1962A_CHNL_DAC_5);
        /* Should update DAC power 2 register */
        RegAddr = (uint8_t) ADI_ADAU1962A_REG_DAC_POWER2;
        /* Calculate shift count */
        ShiftCount = (uint8_t) Diff4_8;
    }
    /* ELSE (DAC 9 to 12) */
    else
    {
        int8_t Diff9_12 = (int8_t) (eDacChnl - ADI_ADAU1962A_CHNL_DAC_9);
        /* Should update DAC power 3 register */
        RegAddr = (uint8_t) ADI_ADAU1962A_REG_DAC_POWER3;
        /* Calculate shift count */
        ShiftCount = (uint8_t) Diff9_12;
    }

    /*
     * Double the shift count to get the actual number of
     * left shifts required for power configuration
     */
    ShiftCount <<= 1;

    /* Get the current value of the register to be updated */
    eResult = ReadAdau1962a (pDevice, RegAddr, &RegData);

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear the current power configuration */
        RegData &= ~(uint8_t)(BITM_ADAU1962A_1D_DAC_POWER << ShiftCount);
        /* Update power configuration */
        RegData |= ((uint8_t) eDacPwr << ShiftCount);
        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, RegAddr, RegData);
    }

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /*
         * Update DAC Power-down configuration
         */

        /* IF (DAC 1 to 8) */
        if (eDacChnl <= ADI_ADAU1962A_CHNL_DAC_8)
        {
            /* Should update Power-down control 2 register */
            RegAddr = (uint8_t) ADI_ADAU1962A_REG_PDN_CTRL2;
            /* Calculate shift count */
            ShiftCount = (uint8_t) eDacChnl;
        }
        /* ELSE (DAC 9 to 12) */
        else
        {
            int8_t Diff9_12 = (int8_t) (eDacChnl - ADI_ADAU1962A_CHNL_DAC_9);
            /* Should update Power-down control 3 register */
            RegAddr = (uint8_t) ADI_ADAU1962A_REG_PDN_CTRL3;
            /* Calculate shift count */
            ShiftCount = (uint8_t) Diff9_12;
        }

        /* Get the current value of the register to be updated */
        eResult = ReadAdau1962a (pDevice, RegAddr, &RegData);

        /* IF (Success) */
        if (eResult == ADI_ADAU1962A_SUCCESS)
        {
            /* Clear the current power-down settings */
            RegData &= ~(uint8_t)(BITM_ADAU1962A_03_DAC_PDN << ShiftCount);

            /* IF (Power-down DAC channel) */
            if (bPwrUp == false)
            {
                /* Update power-down configuration */
                RegData |= (ENUM_ADAU1962A_03_DAC_PDN_PDN << ShiftCount);
            }

            /* Write the updated register configuration */
            eResult = WriteAdau1962a (pDevice, RegAddr, RegData);
        }
    }

    /* Return */
    return (eResult);
}

/**
 * @brief Configure ADAU1962A DAC power setting
 *
 * @param [in]  hDevice         ADAU1962A instance to work on.
 * @param [in]  eDacChnl        DAC channel to configure.
 * @param [in]  bMute           'true' to mute DAC channel, 'false' to un-mute.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully configured mute settings for ADAU1962A.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1962A_ALREADY_RUNNING [D]: This function should not be called when dataflow is enabled.
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_MuteChannel (
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_CHNL                      eChannel,
    bool                                    bMute)
{
    /* Register address to update */
    uint8_t                  RegAddr;
    /* Register configuration data and shift count for mask */
    uint8_t                  RegData, ShiftCount;
    /* Return Code */
    ADI_ADAU1962A_RESULT    eResult;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* IF (Master Mute) */
    if (eChannel == ADI_ADAU1962A_CHNL_DAC_MSTR)
    {
    	/* Should update DAC control register 0 */
    	RegAddr = (uint8_t) ADI_ADAU1962A_REG_DAC_CTRL0;
    	/* Calculate shift count */
    	ShiftCount = 0u;
    }
    /* IF (DAC 1 to 8) */
    else if (eChannel <= ADI_ADAU1962A_CHNL_DAC_8)
    {
        /* Should update DAC Individual channel mutes 1 register */
        RegAddr = (uint8_t) ADI_ADAU1962A_REG_DAC_MUTE1;
        /* Calculate shift count */
        ShiftCount = (uint8_t) eChannel;
    }
    /* ELSE (DAC 9 to 12) */
    else
    {
        int8_t Diff9_12 = (int8_t) (eChannel - ADI_ADAU1962A_CHNL_DAC_9);
        /* Should update DAC Individual channel mutes 2 register */
        RegAddr = (uint8_t) ADI_ADAU1962A_REG_DAC_MUTE2;
        /* Calculate shift count */
        ShiftCount = (uint8_t) Diff9_12;
    }

    /* Get the current value of the register to be updated */
    eResult = ReadAdau1962a (pDevice, RegAddr, &RegData);

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear the current mute settings */
        RegData &= ~(uint8_t)(BITM_ADAU1962A_09_DAC_MUTE << ShiftCount);

        /* IF (Mute DAC channel) */
        if (bMute)
        {
            /* Update mute configuration */
            RegData |= (ENUM_ADAU1962A_09_DAC_MUTE_EN << ShiftCount);
        }

        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, RegAddr, RegData);
    }

    /* Return */
    return (eResult);
}

/**
 * @brief Sets ADAU1962A DAC Channel Volume
 *
 * @param [in]  hDevice         ADAU1962A instance to work on.
 * @param [in]  eDacChnl        DAC channel to configure.
 * @param [in]  Volume          DAC volume to configure.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully updated DAC volume.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_SetVolume(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_CHNL                      eChannel,
    uint8_t                                 Volume)
{
    /* Register address to update */
    uint8_t                  RegAddr;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* IF (Master volume control) */
    if (eChannel == ADI_ADAU1962A_CHNL_DAC_MSTR)
    {
        /* Should update Master volume control register */
        RegAddr = (uint8_t) ADI_ADAU1962A_REG_DACMSTR_VOL;
    }
    /* ELSE (DAC 1 to 12) */
    else
    {
        /* Should update DAC Individual volume control register */
        RegAddr = ((uint8_t) ADI_ADAU1962A_REG_DAC1_VOL + (uint8_t) eChannel);
    }

    /* Write the volume register configuration */
    return (WriteAdau1962a (pDevice, RegAddr, Volume));
}

/**
 * @brief Sets ADAU1962A data word width
 *
 * @param [in]  hDevice         ADAU1962A instance to work on.
 * @param [in]  eWordWidth      Audio data word width.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully updated DAC word width.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1962A_ALREADY_RUNNING [D]: Can't change word width when the dataflow is enabled.
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_SetWordWidth(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_WORD_WIDTH                eWordWidth)
{
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

    /* IF (Dataflow enabled) */
    if (pDevice->bIsEnabled)
    {
        /* Can't change word length when dataflow is enabled */
        return (ADI_ADAU1962A_ALREADY_RUNNING);
    }

#endif /* ADI_DEBUG */

    /* Save word width */
    pDevice->WordWidth = (uint8_t) eWordWidth;

    /* Return success */
    return (ADI_ADAU1962A_SUCCESS);
}

/**
 * @brief Sets ADAU1962A sample rate
 *
 * @param [in]  hDevice         ADAU1962A instance to work on.
 * @param [in]  SampleRateHz    Sample Rate in Hertz.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully updated DAC sample rate.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
/* Sets ADAU1962A sample rate */
ADI_ADAU1962A_RESULT adi_adau1962a_SetSampleRate(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    uint32_t                                SampleRateHz)
{
    /* Register configuration data and shift count for mask */
    uint8_t           		RegData;
    /* Variables to manipulate ADAU1962A register values */
    uint8_t             	SampleRateFieldVal, MClkRateFieldVal;
    uint32_t            	FsMultiplier, FsMultiplierBase;
    /* Return Code */
    ADI_ADAU1962A_RESULT   	eResult;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV   	*pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

    /* IF (MCLK In frequency is out of range) */
    if ((pDevice->MClkInFreq < ADI_ADAU1962A_MIN_MCLK_IN) || (pDevice->MClkInFreq > ADI_ADAU1962A_MAX_MCLK_IN))
    {
        return (ADI_ADAU1962A_MCLK_INVLAID);
    }

#endif /* ADI_DEBUG */

    /* IF (MCLK is less than supplied sample rate) */
    if (((uint32_t)pDevice->MClkInFreq) <= SampleRateHz)
    {
        /* Report error */
        return (ADI_ADAU1962A_MCLK_INVLAID);
    }

    /* IF (MCLK can't support supplied sample rate) */
    if (((uint32_t)pDevice->MClkInFreq) % SampleRateHz > 0u)
    {
        /* Report error */
        return (ADI_ADAU1962A_MCLK_SAMPLE_RATE_MISMATCH);
    }

    /* IF (MCLK more than 12.288MHz) */
    if (pDevice->MClkInFreq > ADI_ADAU1962A_MCLK_12288000HZ)
    {
        /* Use sampling rate (fs) multiplier base value 2 */
        FsMultiplierBase = ADI_ADAU1962A_FS_MULTIPLIER_BASE_2;
    }
    /* ELSE (MCLK less than 12.288MHz) */
    else
    {
        /* Use sampling rate (fs) multiplier base value 1 */
        FsMultiplierBase = ADI_ADAU1962A_FS_MULTIPLIER_BASE_1;
    }

    /* Get the Sampling Rate Multiplier for this MCLK */
    FsMultiplier = (pDevice->MClkInFreq / SampleRateHz);

    /* Initialize the Sample Rate Field value */
    SampleRateFieldVal = 0u;

    /* IF (Sampling Rate Multiplier exceeded maximum limit) */
    if (FsMultiplier > ADI_ADAU1962A_FS_MULTIPLIER_MAX)
    {
        /* Return error as sample rate not supported */
        return(ADI_ADAU1962A_SAMPLE_RATE_NOT_SUPPORTED);
    }

    /* IF (Sampling Rate Multiplier less than base value) */
    if (FsMultiplier < FsMultiplierBase)
    {
        do
        {
        	/* Increment Sample Rate field value */
        	SampleRateFieldVal++;
        	/* Double the Sample Rate Multiplier */
        	FsMultiplier <<= 1u;

        	/* IF (Sample Rate field value exceeded maximum limit) */
        	if (SampleRateFieldVal > ADI_ADAU1962A_SR_FIELD_MAX)
        	{
        		/* Return error as sample rate not supported */
        		return (ADI_ADAU1962A_SAMPLE_RATE_NOT_SUPPORTED);
        	}

        } while (FsMultiplier < FsMultiplierBase);
    }

    /* IF (FS Multiplier is 256) */
    if (FsMultiplier == ADI_ADAU1962A_FS_MULTIPLIER_256X)
    {
    	MClkRateFieldVal = ENUM_ADAU1962A_00_MCLK_SEL_256;
    }
    /* ELSE IF (FS Multiplier is 384) */
    else if (FsMultiplier == ADI_ADAU1962A_FS_MULTIPLIER_384X)
    {
    	MClkRateFieldVal = ENUM_ADAU1962A_00_MCLK_SEL_384;
    }
    /* ELSE IF (FS Multiplier is 512) */
    else if (FsMultiplier == ADI_ADAU1962A_FS_MULTIPLIER_512X)
    {
    	MClkRateFieldVal = ENUM_ADAU1962A_00_MCLK_SEL_512;
    }

    /* ELSE IF (FS Multiplier is 768) */
    else if (FsMultiplier == ADI_ADAU1962A_FS_MULTIPLIER_768X)
    {
    	MClkRateFieldVal = ENUM_ADAU1962A_00_MCLK_SEL_768;
    }
    /* ELSE (Other values) */
    else
    {
    	/* Return error as sample rate not supported */
        return (ADI_ADAU1962A_SAMPLE_RATE_NOT_SUPPORTED);
    }

    /* Read PLL Control 0 Register */
    eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL0, &RegData);

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Clear the current MCLK Select settings */
        RegData &= ~BITM_ADAU1962A_00_MCLK_SEL;
        /* Update MCLK select */
        RegData |= MClkRateFieldVal;
        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_PLL_CLK_CTRL0, RegData);
    }

    /* IF (Success) */
    if (eResult == ADI_ADAU1962A_SUCCESS)
    {
        /* Read DAC Control 0 Register */
        eResult = ReadAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_DAC_CTRL0, &RegData);

        /* Clear the current Sample Rate settings */
        RegData &= ~BITM_ADAU1962A_06_FS;
        /* Update Sample Rate select */
        RegData |= (SampleRateFieldVal << BITP_ADAU1962A_06_FS);
        /* Write the updated register configuration */
        eResult = WriteAdau1962a (pDevice, (uint8_t) ADI_ADAU1962A_REG_DAC_CTRL0, RegData);
    }

    /* Return */
    return (eResult);
}

/**
 * @brief Enable/Disable receiving audio from ADAU1962A.
 *
 * @param [in]  hDevice         ADAU1962A instance to work on.
 * @param [in]  bEnable         'true' to enable dataflow, 'false' to disable.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully enabled/disabled ADAU1962A dataflow.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT  adi_adau1962a_Enable(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    bEnable)
{
    /* Return code */
    ADI_ADAU1962A_RESULT    eResult = ADI_ADAU1962A_SUCCESS;
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV       *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

    /*
     * UPDATE: Validate SPORT configuration
     */

#endif /* ADI_DEBUG */

    /* IF (No change in dataflow status) */
    if (bEnable == pDevice->bIsEnabled)
    {
        /* Return success */
        return (ADI_ADAU1962A_SUCCESS);
    }

    /* IF (Valid SPORT handle available) */
    if (pDevice->hSportDev != NULL)
    {
        /* IF (Enable SPORT dataflow) */
        if (bEnable)
        {
            /* Update SPORT configuration */
            eResult = UpdateSportConfig (pDevice);

            if (eResult != ADI_ADAU1962A_SUCCESS)
            {
                return (eResult);
            }
        }

        /* Master mute/un-mute */
        eResult = adi_adau1962a_MuteChannel (hDevice, ADI_ADAU1962A_CHNL_DAC_MSTR, !bEnable);

        /* IF (Failed to configure ADAU1962A) */
        if (eResult != ADI_ADAU1962A_SUCCESS)
        {
        	/* Return error */
            return (eResult);
        }
#if defined TEST_REGISTER_1962A
        adi_adau1962a_ReadRegister(hDevice, (ADI_ADAU1962A_REG)0u, &Register[0]);
        adi_adau1962a_ReadRegister(hDevice, (ADI_ADAU1962A_REG)1u, &Register[1]);
        adi_adau1962a_ReadRegister(hDevice, (ADI_ADAU1962A_REG)2u, &Register[2]);
        adi_adau1962a_ReadRegister(hDevice, (ADI_ADAU1962A_REG)3u, &Register[3]);
        adi_adau1962a_ReadRegister(hDevice, (ADI_ADAU1962A_REG)4u, &Register[4]);
        adi_adau1962a_ReadRegister(hDevice, (ADI_ADAU1962A_REG)5u, &Register[5]);
        adi_adau1962a_ReadRegister(hDevice, (ADI_ADAU1962A_REG)6u, &Register[6]);
        adi_adau1962a_ReadRegister(hDevice, (ADI_ADAU1962A_REG)7u, &Register[7]);
        adi_adau1962a_ReadRegister(hDevice, (ADI_ADAU1962A_REG)8u, &Register[8]);
        adi_adau1962a_ReadRegister(hDevice, (ADI_ADAU1962A_REG)9u, &Register[9]);
#endif

        /* IF (Use SPORT secondary channel and enable dataflow) */
        if ((pDevice->bEnableSec) && bEnable)
        {
            /* Update SPORT secondary dataflow */
            if (adi_sport_EnableSecondary (pDevice->hSportDev, bEnable) != ADI_SPORT_SUCCESS)
            {
                /* Return error */
                return (ADI_ADAU1962A_SPORT_ERROR);
            }
        }

        /* Update SPORT primary dataflow */
        if (adi_sport_Enable (pDevice->hSportDev, bEnable) != ADI_SPORT_SUCCESS)
        {
            /* Return error */
            return (ADI_ADAU1962A_SPORT_ERROR);
        }

        /* IF (Use SPORT secondary channel and disable dataflow) */
        if ((pDevice->bEnableSec) && (bEnable == false))
        {
            /* Update SPORT secondary dataflow */
            if (adi_sport_EnableSecondary (pDevice->hSportDev, bEnable) != ADI_SPORT_SUCCESS)
            {
                /* Return error */
                return (ADI_ADAU1962A_SPORT_ERROR);
            }
        }
    }
    /* ELSE (SPORT handle not available) */
    else
    {
        return (ADI_ADAU1962A_SPORT_CONFIG_INVALID);
    }

    /* Update dataflow flag */
    pDevice->bIsEnabled = bEnable;

    /* Return success */
    return (eResult);
}

/**
 * @brief Submits a buffer to ADAU1962A instance filled with audio data to transmit
 *
 * @param [in]  hDevice         ADAU1962A instance to work on.
 * @param [in]  pBuffer         Pointer to buffer filled with audio data to transmit.
 * @param [in]  BufferSize      Audio buffer size in bytes.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully submitted audio buffer.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1962A_BUF_INVALID [D]: Supplied audio buffer address and/or size invalid.
 *          - #ADI_ADAU1962A_BUF_SUBMIT_FAILED: Failed to submit audio buffer.
 *          - #ADI_ADAU1962A_SPORT_NOT_CONFIGURED: SPORT Device number is not yet set.
 *
 */
ADI_ADAU1962A_RESULT  adi_adau1962a_SubmitBuffer(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    void                                    *pBuffer,
    uint32_t                                BufferSize)
{
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV   *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

    /* IF (Buffer address is invalid) */
    if ((pBuffer == NULL) || (BufferSize == 0u))
    {
        return (ADI_ADAU1962A_NULL_POINTER);
    }

#endif /* ADI_DEBUG */

    /* IF (Valid SPORT handle available) */
    if (pDevice->hSportDev != NULL)
    {
        /* Submit data buffer */
        if (adi_sport_SubmitBuffer (pDevice->hSportDev, pBuffer, BufferSize) != ADI_SPORT_SUCCESS)
        {
            /* Return error */
            return (ADI_ADAU1962A_SPORT_ERROR);
        }
    }
    /* ELSE (SPORT handle not available) */
    else
    {
        return (ADI_ADAU1962A_SPORT_CONFIG_INVALID);
    }

    /* Return success */
    return (ADI_ADAU1962A_SUCCESS);
}

/**
 * @brief Peek function for non-OS application to check if a processed audio buffer is available or not.
 *
 * @param [in]  hDevice             ADAU1962A instance to query.
 * @param [out] pbIsBufAvailable    Pointer to a location to store if a processed buffer is
 *                                  available (true) or not (false).
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully queried processed audio buffer availability.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1962A_SPORT_ERROR: SPORT Driver Error.
 *
 */
ADI_ADAU1962A_RESULT  adi_adau1962a_IsBufAvailable(
    ADI_ADAU1962A_HANDLE           const    hDevice,
    bool                                    *pbIsBufAvailable)
{
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV     *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* IF (Valid SPORT handle available) */
    if (pDevice->hSportDev != NULL)
    {
        /* Call SPORT driver to check for buffer availability */
        if (adi_sport_IsBufferAvailable (pDevice->hSportDev, pbIsBufAvailable) != ADI_SPORT_SUCCESS)
        {
            /* Return error */
            return (ADI_ADAU1962A_SPORT_ERROR);
        }
    }
    /* ELSE (SPORT handle not available) */
    else
    {
        return (ADI_ADAU1962A_SPORT_CONFIG_INVALID);
    }

    /* Return success */
    return (ADI_ADAU1962A_SUCCESS);
}

/**
 * @brief Gets address of a processed audio buffer.
 *
 * @param [in]  hDevice     ADAU1962A instance to query.
 * @param [out] ppBuffer    Location to store address of a processed audio buffer.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully acquired a processed audio buffer address.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1962A_SPORT_ERROR: SPORT Driver Error.
 *
 */
ADI_ADAU1962A_RESULT  adi_adau1962a_GetBuffer(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    void                                    **ppBuffer)
{
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV     *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* IF (Valid SPORT handle available) */
    if (pDevice->hSportDev != NULL)
    {
        /* Call SPORT driver to get processed buffer address */
        if (adi_sport_GetBuffer (pDevice->hSportDev, ppBuffer) != ADI_SPORT_SUCCESS)
        {
            /* Return error */
            return (ADI_ADAU1962A_SPORT_ERROR);
        }
    }
    /* ELSE (SPORT handle not available) */
    else
    {
        return (ADI_ADAU1962A_SPORT_CONFIG_INVALID);
    }

    /* Return success */
    return (ADI_ADAU1962A_SUCCESS);
}

/**
 * @brief Set callback function to report ADAU1962A audio specific events.
 *
 * @param [in]  hDevice     ADAU1962A instance work on.
 * @param [in]  pfCallback  Address of application callback function.
 * @param [in]  pCBParam    Parameter passed back to application callback.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully set ADAU1962A callback function.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1962A_ALREADY_RUNNING: Cannot perform this operation when dataflow is enabled.
 *
 */
ADI_ADAU1962A_RESULT  adi_adau1962a_RegisterCallback(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_CALLBACK                            pfCallback,
    void                                    *pCBParam)
{
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV  *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* IF (ADAU1962A dataflow enabled) */
    if (pDevice->bIsEnabled)
    {
        /* This operations should not be carried out when dataflow is enabled, report error */
        return (ADI_ADAU1962A_ALREADY_RUNNING);
    }

    /* IF (Valid SPORT handle available) */
    if (pDevice->hSportDev != NULL)
    {
        /* Call SPORT driver to register callback */
        if (adi_sport_RegisterCallback (pDevice->hSportDev, pfCallback, pCBParam) != ADI_SPORT_SUCCESS)
        {
            /* Return error */
            return (ADI_ADAU1962A_SPORT_ERROR);
        }
    }
    /* ELSE (SPORT handle not available) */
    else
    {
        /* Save the callback information */
        pDevice->pfCallback = pfCallback;
        pDevice->pCBParam = pCBParam;
    }

    /* Return success */
    return (ADI_ADAU1962A_SUCCESS);
}

/**
 * @brief Read ADAU1962A DAC register.
 *
 * @param [in]  hDevice     ADAU1962A instance work on.
 * @param [in]  RegAddr     Register address to access
 * @param [in]  pRegValue   Pointer to location to store register value.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully read ADAU1962A register.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_ReadRegister (
    ADI_ADAU1962A_HANDLE           const   	hDevice,
    ADI_ADAU1962A_REG                      	RegAddr,
    uint8_t                             	*pRegValue)
{
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV      *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* Read ADAU1962A register */
    return (ReadAdau1962a (pDevice, (uint8_t) RegAddr, pRegValue));
}

/**
 * @brief Configure ADAU1962A DAC register.
 *
 * @param [in]  hDevice     ADAU1962A instance work on.
 * @param [in]  RegAddr     Register address to access.
 * @param [in]  RegValue    Register value to write.
 *
 * @return  Status
 *          - #ADI_ADAU1962A_SUCCESS: Successfully configured ADAU1962A register.
 *          - #ADI_ADAU1962A_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1962A_RESULT adi_adau1962a_WriteRegister (
    ADI_ADAU1962A_HANDLE           const   	hDevice,
    ADI_ADAU1962A_REG                      	RegAddr,
    uint8_t                            		RegValue)
{
    /* Pointer to ADAU1962A device instance to work on */
    ADI_ADAU1962A_DEV      *pDevice = (ADI_ADAU1962A_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1962A Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1962A_SUCCESS)
    {
        return (ADI_ADAU1962A_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* Configure ADAU1962A register */
    return (WriteAdau1962a (pDevice, (uint8_t) RegAddr, RegValue));
}

/*****/

/*@}*/

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */


