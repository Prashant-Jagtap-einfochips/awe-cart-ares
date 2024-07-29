/*********************************************************************************

Copyright(c) 2011-2015 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      adi_adau1979.c
 * @brief     ADAU1979 Audio codec driver implementation.
 * @version:  $Revision: 31528 $
 * @date:     $Date: 2017-11-22 02:18:08 -0500 (Wed, 22 Nov 2017) $
 *
 * @details
 *            This is the primary source file for ADAU1979 Audio codec driver.
 *
 */

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_2_4:"Allows doxygen addtogroup block")
#pragma diag(suppress:misra_rule_5_1:"Identifiers (internal and external) shall not rely on the significance of more than 31 characters.")
#pragma diag(suppress:misra_rule_5_6:"Allow driver API function parameters and device struct fields to use matching names")
#pragma diag(suppress:misra_rule_8_7:"Objects shall be defined at block scope if they are only accessed from within a single function.")
#pragma diag(suppress:misra_rule_11_4:"Allow driver APIs to cast pointer types")
#pragma diag(suppress:misra_rule_13_7:"Allow 'assert' function to validate memory size")
#pragma diag(suppress:misra_rule_14_7:"Allows multiple exit points")
#pragma diag(suppress:misra_rule_16_7:"Driver API functions sometimes have pointer parameters that are not const qualified when they could be")
#endif

/** @addtogroup ADI_ADAU1979_Driver    ADAU1979 Audio Codec driver
 *  @{

    <b>ADAU1979 Audio Codec driver</b>

    The ADAU1979 is a analog-to-digital converters (ADCs)  The ADCs has both I2C and SPI control ports however this
    driver supports only I2C. The device's PLL can derive the master clock from an external clock input or
frame clock (sample rate clock). The ADC supports both I2S and multi-channel TDM modes.
 */

/*=============  I N C L U D E S   =============*/

/* Include header file with definitions specific to ADAU1979 Audio Codec driver implementation */
#include "adi_adau1979_def.h"

/*==============  D E F I N E S  ===============*/
#if 0
#define TEST_REGISTER
#endif
/*=============  D A T A  =============*/


/* Pointer to ADAU1979 device instance(s) that are open and in use */
static ADI_ADAU1979_DEV      *pADAU1979LastDev = NULL;

/*=============  L O C A L    F U N C T I O N    P R O T O T Y P E S =============*/

/* Opens and configures TWI device to access ADAU1979 register */
static ADI_ADAU1979_RESULT OpenTwi (ADI_ADAU1979_TWI *pTwiDev);

/* Reads an ADAU1979 register */
static ADI_ADAU1979_RESULT ReadAdau1979 (
    ADI_ADAU1979_DEV    *pDevice,
    ADI_ADAU1979_REG    RegAddr,
    uint8_t             *pRegValue);

/* Configures an ADAU1979 register */
static ADI_ADAU1979_RESULT WriteAdau1979 (
    ADI_ADAU1979_DEV    *pDevice,
    ADI_ADAU1979_REG    RegAddr,
    uint8_t             RegValue);


static uint8_t TwiBuffer[32];

#if defined TEST_REGISTER
uint8_t Readback[15];
#endif
/* Initializes ADAU1979 ADC/Rx registers */
static ADI_ADAU1979_RESULT InitAdcRegs (ADI_ADAU1979_DEV *pDevice);

/* Configures serial port for stereo mode */
static ADI_ADAU1979_RESULT StereoConfig(ADI_ADAU1979_DEV *pDevice);

/* Configures serial port for TDM mode */
static ADI_ADAU1979_RESULT TDMConfig   (ADI_ADAU1979_DEV *pDevice);

/* Closes SPORT device mapped to ADAU1979 ADC */
static ADI_ADAU1979_RESULT CloseSportDevice(ADI_ADAU1979_DEV *pDevice);

/*=============  D E B U G    F U N C T I O N    P R O T O T Y P E S  =============*/

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

/* Validates ADAU1979 device handle */
static ADI_ADAU1979_RESULT ValidateHandle (ADI_ADAU1979_HANDLE hDevice);

/* Checks if the supplied ADAU1979 device number is already open */
static ADI_ADAU1979_RESULT ValidateDevNumber (uint32_t    DeviceNum);

#endif /* ADI_DEBUG */

/*=============  D R I V E R    I M P L E M E N T A T I O N    S O U R C E    F I L E S  =============*/


/*=============  C O D E  =============*/

/*=============  L O C A L    F U N C T I O N S  =============*/


/*
 * Opens and configures TWI device to access ADAU1979 register
 *
 * Parameters
 *
 * - [in]  pTwiDev      Address of TWI device instance allocated to access ADAU1979 registers.
 *
 * Returns
 *  - ADI_ADAU1979_SUCCESS: Successfully opened TWI device.
 *  - ADI_ADAU1979_TWI_ERROR: TWI Driver error.
 *
 */
static ADI_ADAU1979_RESULT OpenTwi(ADI_ADAU1979_TWI *pTwiDev)
{
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

    /* IF (Successfully opened TWI Device) */
    if (adi_twi_Open ((uint32_t)pTwiDev->Info.TwiDevNum,
                      ADI_TWI_MASTER,
                      pTwiDev->Info.pTwiDevMem,
                      (uint32_t)pTwiDev->Info.TwiDevMemSize,
                      &pTwiDev->hTwiDev) == ADI_TWI_SUCCESS)
    {

        /* IF (Set TWI hardware address) */
        if (adi_twi_SetHardwareAddress(pTwiDev->hTwiDev, (uint16_t)pTwiDev->Info.eTwiAddr)!= ADI_TWI_SUCCESS)
        {
            eResult = ADI_ADAU1979_TWI_ERROR;
        }


        /* IF (Set TWI scaling factor) */
        if (adi_twi_SetPrescale(pTwiDev->hTwiDev, ADAU1979_TWI_PRESCALE) != ADI_TWI_SUCCESS)
        {
            eResult = ADI_ADAU1979_TWI_ERROR;
        }

        /* IF (Set TWI bit rate) */
        if (adi_twi_SetBitRate(pTwiDev->hTwiDev, ADAU1979_TWI_BITRATE) != ADI_TWI_SUCCESS)
        {
            eResult = ADI_ADAU1979_TWI_ERROR;
        }

        /* IF (Set TWI duty cycle) */
        if (adi_twi_SetDutyCycle(pTwiDev->hTwiDev,  ADAU1979_TWI_DUTYCYCLE) != ADI_TWI_SUCCESS)
        {
            eResult = ADI_ADAU1979_TWI_ERROR;
        }

        /* IF (TWI configuration failed) */
        if (eResult != ADI_ADAU1979_SUCCESS)
        {
            /* Close TWI device */
            adi_twi_Close (pTwiDev->hTwiDev);
        }
    }
    /* ELSE (Failed to open TWI device) */
    else
    {
        eResult = ADI_ADAU1979_TWI_ERROR;
    }

    /* return */
    return (eResult);
}

/*
 * Reads an ADAU1979 register
 *
 * Parameters
 *
 * - [in]  pDevice      ADAU1979 instance to work on.
 * - [in]  RegAddr      Register address to read.
 * - [out] pRegValue    Pointer to location to store register value.
 *
 * Returns
 *  - ADI_ADAU1979_SUCCESS: Successfully updated ADAU1979 settings.
 *  - ADI_ADAU1979_BAD_HANDLE: Supplied Device handle is invalid.
 *
 */
static ADI_ADAU1979_RESULT ReadAdau1979 (
    ADI_ADAU1979_DEV   *pDevice,
    ADI_ADAU1979_REG    RegAddr,
    uint8_t            *pRegValue)
{
    /* Return code */
    ADI_ADAU1979_RESULT eResult;

    /* write register address */
    TwiBuffer[0] = (uint8_t)RegAddr;

    /* Open TWI device */
    eResult = OpenTwi (&pDevice->Twi);

    /* IF (Failed to open TWI) */
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Failed to open TWI, Return error */
        return (eResult);
    }

    /* Write register address to device */
    if (adi_twi_Write(pDevice->Twi.hTwiDev, TwiBuffer, 1u, true) != ADI_TWI_SUCCESS)
    {
        /* Failed to access ADAU1979 register, return error */
        return (ADI_ADAU1979_TWI_ERROR);
    }

    /* Read TWI register */
    if (adi_twi_Read(pDevice->Twi.hTwiDev, pRegValue, 1u, false) != ADI_TWI_SUCCESS)
    {
        /* Failed to access ADAU1979 register, return error */
        return (ADI_ADAU1979_TWI_ERROR);
    }

    /* Close TWI device */
    adi_twi_Close (pDevice->Twi.hTwiDev);

    /* Return */
    return (eResult);
}

/*
 * Configures an ADAU1979 register
 *
 * Parameters
 *
 * - [in]  pDevice      ADAU1979 instance to work on.
 * - [in]  RegAddr      Register address to configure.
 * - [out] RegValue     Register value to write.
 *
 * Returns
 *  - ADI_ADAU1979_SUCCESS: Successfully updated ADAU1979 settings.
 *  - ADI_ADAU1979_BAD_HANDLE: Supplied Device handle is invalid.
 *
 */
static ADI_ADAU1979_RESULT WriteAdau1979 (
    ADI_ADAU1979_DEV   *pDevice,
    ADI_ADAU1979_REG    RegAddr,
    uint8_t             RegValue)
{
    /* Return code */
    ADI_ADAU1979_RESULT eResult;

    /* write register value */
    TwiBuffer[0] = (uint8_t)RegAddr;
    TwiBuffer[1] = RegValue;  /* high byte */

    /* Open TWI device */
    eResult = OpenTwi (&pDevice->Twi);

    /* IF (Failed to open TWI) */
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Failed to open TWI, Return error */
        return (eResult);
    }

    /* write to TWI */
    if (adi_twi_Write(pDevice->Twi.hTwiDev, TwiBuffer, (uint32_t)2u, false) != ADI_TWI_SUCCESS)
    {
        /* Failed to access ADAU1979 register, return error */
        return (ADI_ADAU1979_TWI_ERROR);
    }

    /* Close TWI device */
    adi_twi_Close (pDevice->Twi.hTwiDev);

    /* Return */
    return (eResult);
}


/*
 * Initializes ADAU1979 registers.
 *
 * Parameters
 *
 * - [in] pDevice   ADAU1979 instance to work on.
 * - [in] eAdcMode  ADC (Rx) Serial data mode.
 *
 * Returns
 *  - ADI_ADAU1979_SUCCESS: Successfully initialized ADAU1979 registers.
 *
 */
static ADI_ADAU1979_RESULT InitAdcRegs (ADI_ADAU1979_DEV *pDevice)
{
    /* Serial port mode */
    uint8_t SerialPortMode;
    /* LRClkMode */
    uint8_t LRClkMode;

    /* Return Code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

    /* determine the TDM mode  */
    switch (pDevice->Adc.eSerialMode)
    {
        case ADI_ADAU1979_SERIAL_MODE_TDM2:
            SerialPortMode = (uint8_t)ENUM_ADAU1979_05_MODE_TDM2;
            LRClkMode      = (uint8_t)ENUM_ADAU1979_06_LRCLK_MODE_PULSE;
            break;
        case ADI_ADAU1979_SERIAL_MODE_TDM4:
            SerialPortMode = (uint8_t)ENUM_ADAU1979_05_MODE_TDM4;
            LRClkMode      = (uint8_t)ENUM_ADAU1979_06_LRCLK_MODE_PULSE;
            break;
        case ADI_ADAU1979_SERIAL_MODE_TDM8:
            SerialPortMode = (uint8_t)ENUM_ADAU1979_05_MODE_TDM8;
            LRClkMode      = (uint8_t)ENUM_ADAU1979_06_LRCLK_MODE_PULSE;
            break;
        case ADI_ADAU1979_SERIAL_MODE_TDM16:
            SerialPortMode = (uint8_t)ENUM_ADAU1979_05_MODE_TDM16;
            LRClkMode      = (uint8_t)ENUM_ADAU1979_06_LRCLK_MODE_PULSE;
            break;
        case ADI_ADAU1979_SERIAL_MODE_STEREO:
            SerialPortMode = (uint8_t)ENUM_ADAU1979_05_MODE_STEREO;
            LRClkMode      = (uint8_t)ENUM_ADAU1979_06_LRCLK_MODE_50;
            break;
        default:
            eResult = ADI_ADAU1979_SPORT_CFG_ERROR;
            break;
    }

    if (eResult == ADI_ADAU1979_SUCCESS)
    {
        /* Loop variable */
        uint32_t i;

        /* Update field bits for TDM settings and LR Clk mode */
        RegConfig[3].RegData &= (uint8_t)~(BITM_ADAU1979_05_MODE);
        RegConfig[3].RegData |= SerialPortMode;
        RegConfig[4].RegData &= (uint8_t)~(BITM_ADAU1979_06_LRCLK_MODE);
        RegConfig[4].RegData |= LRClkMode;

        /* Initialize ADAU1979 with default settings */

        /* FOR (All entries in the configuration table) */
        for (i = 0u; i < (sizeof (RegConfig) / sizeof (RegConfig[0])); i++)
        {
            /* Configure ADAU1979 register */
            eResult = WriteAdau1979 (pDevice, (ADI_ADAU1979_REG)RegConfig[i].RegAddr, RegConfig[i].RegData);

            /* IF (Error) */
            if (eResult != ADI_ADAU1979_SUCCESS)
            {
                break;
            }
        }
    }

    /* Return */
    return (eResult);
}

/*
 * Configure the serial port of the stereo mode.
 *
 * Parameters
 *
 * - [in]  pDevice      ADAU1979 instance to work on.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully configured ADAU1979.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
static ADI_ADAU1979_RESULT StereoConfig(ADI_ADAU1979_DEV *pDevice)
{
    /* Return Code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

    /* Serial port word length */
    uint8_t WordLength;

    /* Determine the serial port channel  */
    switch (pDevice->Adc.eWordLength)
    {
        case ADI_ADAU1979_WORD_WIDTH_24:
            WordLength = 23u;
            break;
        case ADI_ADAU1979_WORD_WIDTH_16:
            WordLength = 15u;
            break;
        default:
            eResult = ADI_ADAU1979_SPORT_ERROR;
            break;
    }

    /* IF (Success) */
    if (eResult == ADI_ADAU1979_SUCCESS)
    {
        /* Configure the serial port for data length, LSB first etc. */
        if(adi_sport_ConfigData(pDevice->Sport.hSportDev,
                                ADI_SPORT_DTYPE_SIGN_FILL,
                                WordLength,
                                false,
                                false,
                                false) != ADI_SPORT_SUCCESS)
        {
            return(ADI_ADAU1979_SPORT_ERROR);
        }

        /* Configure the clock for the serial port. */
        if( adi_sport_ConfigClock(pDevice->Sport.hSportDev,
                                  1u,
                                  false,
                                  !pDevice->Adc.bBClkRisingEdgeLatch,
                                  false) != ADI_SPORT_SUCCESS)
        {
            return(ADI_ADAU1979_SPORT_ERROR);
        }

        /* Do the Frame Sync(FS) configuration for the serial port. */
        if( adi_sport_ConfigFrameSync(pDevice->Sport.hSportDev,
                                      100u,
                                      true,
                                      false,
                                      false,
                                      pDevice->Adc.bLRClkHighLow,
                                      false,
                                      false) != ADI_SPORT_SUCCESS)
        {
            return(ADI_ADAU1979_SPORT_ERROR);
        }
    }

    return eResult;
}


/*
 * Configure the serial port of the TDM mode.
 *
 * Parameters
 *
 * - [in]  pDevice      ADAU1979 instance to work on.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully configured ADAU1979.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
static ADI_ADAU1979_RESULT TDMConfig(ADI_ADAU1979_DEV *pDevice)
{
    /* Return Code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

    /* variable for the number of timeslots */
    uint8_t NumTimeslots;

    /* variable for the sport word length */
    uint8_t WordLength;

    /* determine the TDM mode  */
    switch (pDevice->Adc.eSerialMode)
    {
        case ADI_ADAU1979_SERIAL_MODE_TDM2:
            NumTimeslots = 1u;
            break;
        case ADI_ADAU1979_SERIAL_MODE_TDM4:
            NumTimeslots = 3u;
            break;
        case ADI_ADAU1979_SERIAL_MODE_TDM8:
            NumTimeslots = 7u;
            break;
        case ADI_ADAU1979_SERIAL_MODE_TDM16:
            NumTimeslots = 15u;
            break;
        default:
            eResult = ADI_ADAU1979_SPORT_ERROR;
            break;
    }

    /* SPORT DMA word length always 32 in TDM mode */
    /* IF (Error) */
    if (eResult == ADI_ADAU1979_SUCCESS)
    {
        /* determine the Sport channel  */
        switch (pDevice->Adc.eWordLength)
        {
            case ADI_ADAU1979_WORD_WIDTH_24:
                WordLength = 31u;
                break;
            case ADI_ADAU1979_WORD_WIDTH_16:
                WordLength = 31u;
                break;
            default:
                eResult = ADI_ADAU1979_SPORT_ERROR;
                break;
        }
    }

    /* IF (Success) */
    if (eResult == ADI_ADAU1979_SUCCESS)
    {

        /* configure the sport for data length, LSB first etc */
        if(adi_sport_ConfigData(pDevice->Sport.hSportDev,
                                ADI_SPORT_DTYPE_SIGN_FILL,
                                WordLength,
                                false,
                                false,
                                false) != ADI_SPORT_SUCCESS)
        {
            return(ADI_ADAU1979_SPORT_ERROR);
        }

        /* Configure the clock for the SPORT. This API set the whether use the internal clock, SPORT clock etc */
        if( adi_sport_ConfigClock(pDevice->Sport.hSportDev,
                                  1u,
                                  false,
                                  !pDevice->Adc.bBClkRisingEdgeLatch,
                                  false) != ADI_SPORT_SUCCESS)
        {
            return(ADI_ADAU1979_SPORT_ERROR);
        }

        /* Configure the frame sync. This API configure the SPORT whether to use frame sync or not , external or internal framesync etc */
        if( adi_sport_ConfigFrameSync(pDevice->Sport.hSportDev,
                                      100u,
                                      true,
                                      false,
                                      true,
                                      pDevice->Adc.bLRClkHighLow,
                                      false,
                                      false) != ADI_SPORT_SUCCESS)
        {
            return(ADI_ADAU1979_SPORT_ERROR);
        }

        /* Configure the frame sync. This API configure the SPORT whether to use frame sync or not , external or internal framesync etc */
        if(adi_sport_ConfigMC(pDevice->Sport.hSportDev,
                              0u,
                              NumTimeslots,
                              0u,
                              false) != ADI_SPORT_SUCCESS)
        {
            return(ADI_ADAU1979_SPORT_ERROR);
        }
    }

    return eResult;
}


/*
 * Closes all SPORT devices mapped to ADAU1979 ADC/DAC channels.
 *
 * Parameters
 *
 * - [in]  pModeInfo    Pointer to the instance created to handle serial data mode.
 * - [in]  eMode        ADC/DAC serial data mode.
 *
 * Returns
 *  - ADI_ADAU1979_SUCCESS: Successfully closed SPORT device(s).
 *  - ADI_ADAU1979_SPORT_ERROR: SPORT driver error, failed to close SPORT device.
 *
 */
static ADI_ADAU1979_RESULT CloseSportDevice(ADI_ADAU1979_DEV *pDevice)
{
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

    /* IF (A valid SPORT handle is available) */
    if (pDevice->Sport.hSportDev != NULL)
    {
        /* SPORT driver result */
        ADI_SPORT_RESULT eSportResult = ADI_SPORT_SUCCESS;

        /* Close the SPORT device */
        eSportResult = adi_sport_Close (pDevice->Sport.hSportDev);

        /* IF (Success) */
        if (eSportResult == ADI_SPORT_SUCCESS)
        {
            /* Clear the SPORT device handle */
            pDevice->Sport.hSportDev = NULL;
        }
        else
        {
            eResult = ADI_ADAU1979_SPORT_ERROR;
        }
    }

    return eResult;
}


/*=============  D E B U G   F U N C T I O N S =============*/

/* IF (Debug mode enabled) */
#if defined (ADI_DEBUG)

/*
 * Validates ADAU1979 device handle.
 *
 * Parameters
 *  - [in]  hDevice    ADAU1979 handle to validate.
 *
 * Returns:  Status
 *  - ADI_ADAU1979_SUCCESS: Successfully validated ADAU1979 device handle.
 *  - ADI_ADAU1979_BAD_HANDLE: Invalid Device Handle.
 *
 */
static ADI_ADAU1979_RESULT ValidateHandle (ADI_ADAU1979_HANDLE hDevice)
{
    /* Pointer to the device instance to validate */
    ADI_ADAU1979_DEV *pDeviceToValidate = (ADI_ADAU1979_DEV *) hDevice;
    /* Last open ADAU1979 instance in chain */
    ADI_ADAU1979_DEV *pDeviceInChain;

    /* FOR (ADAU1979 instances in chain) */
    for (pDeviceInChain = pADAU1979LastDev; pDeviceInChain != NULL; pDeviceInChain = pDeviceInChain->pPrevious)
    {
        /* IF (Supplied handle is valid) */
        if (pDeviceToValidate == pDeviceInChain)
        {
            /* This is a valid device */
            return (ADI_ADAU1979_SUCCESS);
        }
    }

    /* This device handle must be invalid */
    return (ADI_ADAU1979_BAD_HANDLE);
}

/*
 * Checks if the supplied ADAU1979 device number is already open.
 *
 * Parameters
 *  - [in]  DeviceNum    ADAU1979 device number to validate.
 *
 * Returns:  Status
 *  - ADI_ADAU1979_SUCCESS: Successfully validated ADAU1979 device number.
 *  - ADI_ADAU1979_IN_USE: Device number is already in use.
 *
 */
static ADI_ADAU1979_RESULT ValidateDevNumber (uint32_t DeviceNum)
{
    /* ADAU1979 instance in chain */
    ADI_ADAU1979_DEV     *pDeviceInChain;

    /* FOR (All Open ADAU1979 instances in chain) */
    for (pDeviceInChain = pADAU1979LastDev; pDeviceInChain != NULL; pDeviceInChain = pDeviceInChain->pPrevious)
    {
        /* IF (Supplied device number is already in use) */
        if (pDeviceInChain->DevNum == DeviceNum)
        {
            /* This device number is already in use*/
            return (ADI_ADAU1979_IN_USE);
        }
    }

    /* This device number is not in use */
    return (ADI_ADAU1979_SUCCESS);
}

/* End of debug functions */
#endif /* ADI_DEBUG */

/*=============  P U B L I C   F U N C T I O N S =============*/

/**
 * @brief Opens ADAU1979 device instance for use.
 *
 * @details     Opens the given ADAU1979 device instance and returns
 *              the handle to the opened device. The returned handle should
 *              be used in all the other APIs.
 *
 * @param [in]  DeviceNum       ADAU1979 instance number to open.
 * @param [in]  eSerialMode     Serial mode of the SPORT, I2S(stereo), TDM2, TDM4, TDM8 or TDM16
 * @param [in]  pDeviceMemory   Pointer to #ADI_ADAU1979_MEMORY_SIZE sized memory
 *                              location to handle the device instance.
 * @param [in]  MemorySize      Size of the buffer to which "pDeviceMemory" points.
 * @param [out] phDevice        Address where the ADAU1979 Device handle will be stored.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully opened ADAU1979 instance.
 *          - #ADI_ADAU1979_NULL_POINTER: Supplied memory address is invalid.
 *          - #ADI_ADAU1979_INSUFFICIENT_MEMORY: Supplied memory is not sufficient to manage the device.
 *          - #ADI_ADAU1979_IN_USE: Device number is already in use.
 *
 * @sa  adi_adau1979_Close()
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_Open(
    uint32_t                        DeviceNum,
    ADI_ADAU1979_SERIAL_MODE        eSerialMode,
    void                    *const  pDeviceMemory,
    uint32_t                        MemorySize,
    ADI_ADAU1979_HANDLE     *const  phDevice)
{
    /* Pointer to the device instance memory to work on */
    ADI_ADAU1979_DEV *pDevice = pDeviceMemory;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (This Device Number is already open) */
    if (ValidateDevNumber (DeviceNum) != ADI_ADAU1979_SUCCESS)
    {
        /* Report failure as device in use */
        return (ADI_ADAU1979_IN_USE);
    }

    /* Check if the given pointer parameters are valid */
    if((pDeviceMemory == NULL) || (phDevice == NULL))
    {
        return (ADI_ADAU1979_NULL_POINTER);
    }

    /* Check if the given memory is insufficient */
    if(MemorySize < ADI_ADAU1979_MEMORY_SIZE)
    {
        return (ADI_ADAU1979_INSUFFICIENT_MEMORY);
    }

    /* Asserts to validate ADAU1979 device memory size */
    assert(ADI_ADAU1979_MEMORY_SIZE >= sizeof(ADI_ADAU1979_DEV));

#endif /* ADI_DEBUG */

    /* Clear the given memory */
    memset(pDeviceMemory, 0, MemorySize);

    /* Initialize the device instance memory */
    pDevice->DevNum                     = DeviceNum;
    pDevice->Adc.eSampleRate            = ADI_ADAU1979_SAMPLE_RATE_NONE;
    pDevice->Adc.eWordLength            = ADI_ADAU1979_WORD_WIDTH_NONE;
    pDevice->Adc.eMClkFreq              = ADI_ADAU1979_MCLK_IN_FREQ_NONE;
    pDevice->Adc.eSerialMode            = eSerialMode;
    pDevice->Adc.bBClkRisingEdgeLatch   = true;
    pDevice->Adc.bLRClkHighLow          = false;

    /* Protect this section of code - entering a critical region */
    adi_osal_EnterCriticalRegion();

    /* Add this instance to ADAU1979 device chain */
    pDevice->pPrevious = pADAU1979LastDev;

    /* IF (This is not the first instance to be opened) */
    if (pADAU1979LastDev != NULL)
    {
        /* Chain this device to the last instance in chain */
        pADAU1979LastDev->pNext = pDevice;
    }
    /* Make this device as last instance in chain */
    pADAU1979LastDev = pDevice;

    /* Exit the critical region */
    adi_osal_ExitCriticalRegion();

    /* Pass the physical device handle to client */
    *phDevice = (ADI_ADAU1979_HANDLE *)pDevice;

    /* Return */
    return(ADI_ADAU1979_SUCCESS);
}

/**
 * @brief Closes ADAU1979 instance.
 *
 * @details     Closes ADAU1979 driver instance and disables the data flow.
 *              Once the drive is closed, it is not allowed access any of the other
 *              APIs as the device handle will not be valid anymore.
 *
 * @param [in]  hDevice     ADAU1979 instance to close.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully closed ADAU1979 instance.
 *          - #ADI_ADAU1979_BAD_HANDLE: Supplied Device handle is invalid.
 *          - #ADI_ADAU1979_SPORT_ERROR: SPORT Driver Error.
 *
 * @sa  adi_adau1979_Open()
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_Close(ADI_ADAU1979_HANDLE const hDevice)
{
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif

    /* Disconnect SPORT device from ADAU1979 */

    /* Close all ADC SPORT devices */
    eResult = CloseSportDevice (pDevice);

    /* IF (Failed to disconnect SPORT) */
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        return (eResult);
    }

    /*-- Remove this device from ADAU1979 device in use chain --*/

    /* Protect this section of code - entering a critical region    */
    adi_osal_EnterCriticalRegion();

    /* IF (This is the last device instance in chain) */
    if (pADAU1979LastDev == pDevice)
    {
        /* Make the previous instance linked to this device as last in chain */
        pADAU1979LastDev = pDevice->pPrevious;
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
    return(ADI_ADAU1979_SUCCESS);
}

/**
 * @brief Configure the TWI device used to access ADAU1979 registers.
 *
 * @details     Configures the TWI device number and TWI slave select to use while
 *              accessing ADAU1979 hardware registers. When a valid TWI configuration
 *              is provided, this function configures the corresponding ADAU1979 device
 *              registers with Stereo (I2S) mode default values.
 *
 * @param [in]  hDevice     ADAU1979 instance to work on.
 * @param [in]  *pConfig    Pointer to information specific to TWI
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully configured TWI for ADAU1979.
 *          - #ADI_ADAU1979_BAD_HANDLE: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_ConfigTwi(
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_TWI_CONFIG             *pConfig)
{
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

    /* IF (ADAU1979 dataflow enabled) */
    if (pDevice->Adc.bIsEnabled)
    {
        /* Report failure as ADAU1979 is already running */
        return (ADI_ADAU1979_ALREADY_RUNNING);
    }

#endif /* ADI_DEBUG */

    /* Save the TWI device number and it configuration for addressing ADAU1979 device */
    pDevice->Twi.Info.TwiDevNum      = pConfig->TwiDevNum;
    pDevice->Twi.Info.pTwiDevMem     = pConfig->pTwiDevMem;
    pDevice->Twi.Info.TwiDevMemSize  = pConfig->TwiDevMemSize;
    pDevice->Twi.Info.eTwiAddr       = pConfig->eTwiAddr;

    /* configure ADC with defaults */
    if(InitAdcRegs(pDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_TWI_ERROR);
    }

    return (ADI_ADAU1979_SUCCESS);
}


/**
 * @brief  Configure the serial port of the ADAU1979.
 *
 * @details     This function can be used to serial port configurations for both the
 *              ADAU1979 register configuration and the SPORT configuration of the DSP.
 *              There is numerous dependencies between the ADAU1979 and the associated
 *              SPORT interface of the DSP.
 *
 * @param [in]  hDevice     ADAU1979 instance to work on.
 * @param [in]  pConfig     Pointer to an instance that holds Stereo mode specific
 *                          configuration settings.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully configured ADAU1979.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_ConfigSport (
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_SPORT_CONFIG           *pConfig)
{
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;
    /* Return Code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */


/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 dataflow enabled) */
    if (pDevice->Adc.bIsEnabled)
    {
        /* Report failure as ADAU1979 is already running */
        return (ADI_ADAU1979_ALREADY_RUNNING);
    }

#endif /* ADI_DEBUG */

    /* Close any the previously mapped SPORT device */
    CloseSportDevice (pDevice);

    /* Save the SPORT device number and its configuration  */
    pDevice->Sport.Info.SportDevNum      = pConfig->SportDevNum;
    pDevice->Sport.Info.pSportDevMem     = pConfig->pSportDevMem;
    pDevice->Sport.Info.SportDevMemSize  = pConfig->SportDevMemSize;
    pDevice->Sport.Info.eSportChnl       = pConfig->eSportChnl;
    pDevice->Sport.Info.eSportPri        = pConfig->eSportPri;
    pDevice->Sport.Info.eSportSec        = pConfig->eSportSec;
    pDevice->Sport.Info.bLsbFirst        = pConfig->bLsbFirst;


    /* IF (SPORT primary is mapped to an invalid Serial data port) */
    if ((pConfig->eSportPri != ADI_ADAU1979_SERIAL_PORT_DSDATA1) &&\
        (pConfig->eSportPri != ADI_ADAU1979_SERIAL_PORT_DSDATA2))
    {
        /* Primary SPORT channel should be mapped to a serial data port */
        return (ADI_ADAU1979_SPORT_CFG_ERROR);
    }

    /* IF (SPORT secondary is mapped to an invalid Serial data port) */
    if (pConfig->eSportSec == pConfig->eSportPri)
    {
        /* Secondary SPORT channel mapping invalid */
        return (ADI_ADAU1979_SPORT_CFG_ERROR);
    }

    /* IF ( success) */
    if (eResult == ADI_ADAU1979_SUCCESS)
    {
        /* determine the SPORT mode  */
        if(pDevice->Adc.eSerialMode == ADI_ADAU1979_SERIAL_MODE_STEREO)
        {
            /* Stereo mode */
            if( adi_sport_Open ((uint32_t)pDevice->Sport.Info.SportDevNum,
                                (ADI_SPORT_CHANNEL)pDevice->Sport.Info.eSportChnl,
                                ADI_SPORT_DIR_RX,
                                ADI_SPORT_I2S_MODE,
                                pDevice->Sport.Info.pSportDevMem,
                                pDevice->Sport.Info.SportDevMemSize,
                                &pDevice->Sport.hSportDev) != ADI_SPORT_SUCCESS)
            {
                return (ADI_ADAU1979_SPORT_NOT_CONFIGURED);
            }

        }
        else if((pDevice->Adc.eSerialMode == ADI_ADAU1979_SERIAL_MODE_TDM2) ||
                (pDevice->Adc.eSerialMode  == ADI_ADAU1979_SERIAL_MODE_TDM4) ||
                (pDevice->Adc.eSerialMode  == ADI_ADAU1979_SERIAL_MODE_TDM8) ||
                (pDevice->Adc.eSerialMode  == ADI_ADAU1979_SERIAL_MODE_TDM16))
        {
            /* TDM mode */
            if( adi_sport_Open ((uint32_t)pDevice->Sport.Info.SportDevNum,
                                (ADI_SPORT_CHANNEL)pDevice->Sport.Info.eSportChnl,
                                ADI_SPORT_DIR_RX,
                                ADI_SPORT_MC_MODE,
                                pDevice->Sport.Info.pSportDevMem,
                                pDevice->Sport.Info.SportDevMemSize,
                                &pDevice->Sport.hSportDev) != ADI_SPORT_SUCCESS)
            {
                return (ADI_ADAU1979_SPORT_NOT_CONFIGURED);
            }
        }
        else
        {
            return (ADI_ADAU1979_SPORT_NOT_CONFIGURED);
        }

        /* enable SPORT DMA */
        if(adi_sport_EnableDMAMode(pDevice->Sport.hSportDev, true) != ADI_SPORT_SUCCESS)
        {
            return (ADI_ADAU1979_SPORT_NOT_CONFIGURED);
        }


        /* enable SPORT streaming */
        if(adi_sport_StreamingEnable(pDevice->Sport.hSportDev, true) != ADI_SPORT_SUCCESS)
        {
            return (ADI_ADAU1979_SPORT_NOT_CONFIGURED);
        }

        /* Set DMA Transfer Size */
        if(adi_sport_SetDmaTransferSize(pDevice->Sport.hSportDev, ADI_SPORT_DMA_TRANSFER_4BYTES) != ADI_SPORT_SUCCESS)
        {
            return (ADI_ADAU1979_SPORT_NOT_CONFIGURED);
        }
    }
    else
    {
        return (ADI_ADAU1979_SPORT_NOT_CONFIGURED);
    }

    return (eResult);
}




/**
 * @brief Submits a buffer to ADAU1979 instance to fill with received audio data.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  pBuffer         Pointer to audio buffer to fill.
 * @param [in]  BufferSize      Audio buffer size in bytes.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully submitted audio buffer.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1979_BUF_INVALID [D]: Supplied audio buffer address and/or size invalid.
 *          - #ADI_ADAU1979_BUF_SUBMIT_FAILED: Failed to submit audio buffer.
 *          - #ADI_ADAU1979_SPORT_NOT_CONFIGURED: SPORT Device number is not yet set.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_SubmitBuffer(
    ADI_ADAU1979_HANDLE     const hDevice,
    void                          *pBuffer,
    uint32_t                      BufferSize)
{
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

    ADI_SPORT_RESULT eSportResult;

    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV     *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

    /* IF (Buffer address is invalid) */
    if ((pBuffer == NULL) || (BufferSize == 0u))
    {
        return (ADI_ADAU1979_NULL_POINTER);
    }

#endif /* ADI_DEBUG */

    eSportResult = adi_sport_SubmitBuffer(pDevice->Sport.hSportDev, pBuffer, BufferSize);
    if(eSportResult != ADI_SPORT_SUCCESS)
    {
        eResult = ADI_ADAU1979_SPORT_ERROR;
    }

    return (eResult);
}


/**
 * @brief Peek function for non-OS application to check if a processed audio buffer is available or not.
 *
 * @param [in]  hDevice             ADAU1979 instance to query.
 * @param [out] pbIsBufAvailable    Pointer to a location to store if a processed buffer is
 *                                  available (true) or not (false).
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully queried processed audio buffer availability.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1979_SPORT_ERROR: SPORT Driver Error.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_IsBufAvailable(
    ADI_ADAU1979_HANDLE     const hDevice,
    bool                          *pbIsBufAvailable)
{

    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

    ADI_SPORT_RESULT eSportResult;

    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV     *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    eSportResult = adi_sport_IsBufferAvailable(pDevice->Sport.hSportDev, pbIsBufAvailable);
    if(eSportResult != ADI_SPORT_SUCCESS)
    {
        eResult = ADI_ADAU1979_SPORT_ERROR;
    }

    /* Call Stereo/TDM mode specific function */
    return (eResult);
}

/**
 * @brief Gets address of a processed audio buffer.
 *
 * @param [in]  hDevice     ADAU1979 instance to query.
 * @param [out] ppBuffer    Location to store address of a processed audio receive buffer
 *                          filled with valid audio data.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully acquired a processed audio receive buffer address.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1979_SPORT_ERROR: SPORT Driver Error.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_GetBuffer(
    ADI_ADAU1979_HANDLE     const hDevice,
    void                          **ppBuffer)
{
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

    ADI_SPORT_RESULT eSportResult;

    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV     *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    eSportResult = adi_sport_GetBuffer(pDevice->Sport.hSportDev, ppBuffer);
    if(eSportResult != ADI_SPORT_SUCCESS)
    {
        eResult = ADI_ADAU1979_SPORT_ERROR;
    }

    /* Call Stereo/TDM mode specific function */
    return (eResult);
}


/**
 * @brief Enable/Disable receiving audio from ADAU1979.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  bEnable         'true' to enable dataflow, 'false' to disable.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully enabled/disabled ADAU1979 receive dataflow.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_Enable(
    ADI_ADAU1979_HANDLE     const hDevice,
    bool                          bEnable)
{
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

    /*
     * UPDATE: Validate configuration
     */
    if(pDevice->Adc.eSampleRate == ADI_ADAU1979_SAMPLE_RATE_NONE)
    {
        return (ADI_ADAU1979_SAMPLE_RATE_NOT_SUPPORTED);
    }

    if(pDevice->Adc.eWordLength == ADI_ADAU1979_WORD_WIDTH_NONE)
    {
        return (ADI_ADAU1979_FMT_NOT_SUPPORTED);
    }

#endif /* ADI_DEBUG */

    /* IF (No change dataflow status) */
    if (bEnable == pDevice->Adc.bIsEnabled)
    {
        /* Return success */
        return (ADI_ADAU1979_SUCCESS);
    }

    /* IF (Enable ADC dataflow) */
    if (bEnable)
    {
        /* Power-up ADC */
        eResult = adi_adau1979_PowerUp (pDevice, ADI_ADAU1979_POWER_MASTER, bEnable);

        /* IF (Failed to configure ADAU1979) */
        if (eResult != ADI_ADAU1979_SUCCESS)
        {
            /* Return error */
            return (eResult);
        }
        /* configure for stereo mode */
        if(pDevice->Adc.eSerialMode == ADI_ADAU1979_SERIAL_MODE_STEREO)
        {
            if(StereoConfig(pDevice) != ADI_ADAU1979_SUCCESS)
            {
                return (ADI_ADAU1979_SPORT_ERROR);
            }
        }
        /* configure for TDM mode */
        else
        {
            if(TDMConfig(pDevice) != ADI_ADAU1979_SUCCESS)
            {
                return (ADI_ADAU1979_SPORT_ERROR);
            }
        }

#if defined TEST_REGISTER
        uint32_t i;
        for (i = 0u; i < 15u; i++)
        {
            eResult = ReadAdau1979 (pDevice, (uint8_t)RegConfig[i].RegAddr, &Readback[i]);
        }
#endif

        if(pDevice->Sport.Info.eSportSec != ADI_ADAU1979_SERIAL_PORT_NONE)
        {
            /* Stereo mode requires secondary to be enabled so that all 4 ADCs are accessible */
            if( adi_sport_EnableSecondary(pDevice->Sport.hSportDev, true) != ADI_SPORT_SUCCESS)
            {
                return(ADI_ADAU1979_SPORT_ERROR);
            }
        }

        if(adi_sport_Enable(pDevice->Sport.hSportDev, bEnable) != ADI_SPORT_SUCCESS)
        {
            return (ADI_ADAU1979_SPORT_ERROR);
        }
    }
    else
    {
        if(adi_sport_Enable(pDevice->Sport.hSportDev, bEnable) != ADI_SPORT_SUCCESS)
        {
            return (ADI_ADAU1979_SPORT_ERROR);
        }
        if(pDevice->Sport.Info.eSportSec != ADI_ADAU1979_SERIAL_PORT_NONE)
        {
            /* disable secondary */
            if( adi_sport_EnableSecondary(pDevice->Sport.hSportDev, false) != ADI_SPORT_SUCCESS)
            {
                return(ADI_ADAU1979_SPORT_ERROR);
            }
        }
    }

    /* IF (Success) */
    if (eResult == ADI_ADAU1979_SUCCESS)
    {
        /* Update Rx dataflow flag */
        pDevice->Adc.bIsEnabled = bEnable;
    }

    return (eResult);
}

/**
 * @brief Set callback function to report ADAU1979 audio receive (ADC) specific events.
 *
 * @param [in]  hDevice     ADAU1979 instance to work on.
 * @param [in]  pfCallback  Address of application callback function.
 * @param [in]  pCBParam    Parameter passed back to application callback.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully updated ADAU1979 callback function.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1979_ALREADY_RUNNING: Cannot perform this operation when dataflow is enabled.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_RegisterCallback(
    ADI_ADAU1979_HANDLE     const hDevice,
    ADI_CALLBACK                  pfCallback,
    void                          *pCBParam)
{

    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;

    ADI_SPORT_RESULT eSportResult;

    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV  *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* IF (ADAU1979 Rx dataflow enabled) */
    if (pDevice->Adc.bIsEnabled)
    {
        /* This operations should not be carried out when Rx dataflow is enabled, report error */
        return (ADI_ADAU1979_ALREADY_RUNNING);
    }

    eSportResult = adi_sport_RegisterCallback (pDevice->Sport.hSportDev, pfCallback, pCBParam);
    if(eSportResult != ADI_SPORT_SUCCESS)
    {
        eResult = ADI_ADAU1979_SPORT_ERROR;
    }
    /* Save the callback information */
    pDevice->Sport.pfCallback = pfCallback;
    pDevice->Sport.pCBParam = pCBParam;

    /* Return success */
    return (eResult);
}


/**
 * @brief Assigns an ADC channel to a TDM timeslot.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  eChannel        Enumeration of the ADC channels for TDM mode.
 * @param [in]  eTimeslot       Enumeration of the timeslot assignment of a ADC channel.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully configured TDM timeslot assignment.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_SetTdmTimeslotChannelMap (
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL          eChannel,
    ADI_ADAU1979_TDM_TIMESLOT           eTimeslot)
{

    /* Field mask  */
    uint8_t FieldMask;
    /* Shift operator */
    uint8_t Shift;
    /* Location to hold register value */
    uint8_t RegValue;
    /* ADAU1979 Register address to access */
    ADI_ADAU1979_REG RegAddr;

    /* Return Code */
    ADI_ADAU1979_RESULT       eResult = ADI_ADAU1979_SUCCESS;
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV          *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

    /* IF (ADC serial data mode is not TDM) */
    if (pDevice->Adc.eSerialMode <= ADI_ADAU1979_SERIAL_MODE_TDM4)
    {
        /* Return error */
        return (ADI_ADAU1979_SPORT_CFG_ERROR);
    }

    /* IF (ADAU1979 ADC (Rx) dataflow enabled) */
    if (pDevice->Adc.bIsEnabled)
    {
        /* Report failure as ADAU1979 is already running */
        return (ADI_ADAU1979_ALREADY_RUNNING);
    }

#endif /* ADI_DEBUG */

    /* determine which channel to configure  */
    switch (eChannel)
    {
        case ADI_ADAU1979_AUDIO_CHANNEL1:
            RegAddr      = ADI_ADAU1979_REG_SAI_CMAP12;
            FieldMask    = BITM_ADAU1979_07_CH1_MAP;
            Shift        = BITP_ADAU1979_07_CH1_MAP;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL2:
            RegAddr      = ADI_ADAU1979_REG_SAI_CMAP34;
            FieldMask    = BITM_ADAU1979_07_CH2_MAP;
            Shift        = BITP_ADAU1979_07_CH2_MAP;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL3:
            RegAddr      = ADI_ADAU1979_REG_SAI_CMAP34;
            FieldMask    = BITM_ADAU1979_08_CH3_MAP;
            Shift        = BITP_ADAU1979_08_CH3_MAP;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL4:
            RegAddr      = ADI_ADAU1979_REG_SAI_CMAP34;
            FieldMask    = BITM_ADAU1979_08_CH4_MAP;
            Shift        = BITP_ADAU1979_08_CH4_MAP;
            break;
        default:
            eResult = ADI_ADAU1979_FN_NOT_SUPPORTED;
            break;
    }

    if (eResult == ADI_ADAU1979_SUCCESS)
    {

        /* Read Register */
        eResult = ReadAdau1979 (pDevice, RegAddr, &RegValue);

        if (eResult == ADI_ADAU1979_SUCCESS)
        {

            /* Clear register value fields */
            RegValue &= ~FieldMask;

            /* Update register value with timeslot */
            RegValue |= ((uint8_t)eTimeslot << Shift);

            /* Configure timeslot assignment */
            eResult = WriteAdau1979 (pDevice, RegAddr, RegValue);
        }
    }

    return (eResult);
}

/**
 * @brief Configure ADAU1979 Phase Locked Loop (PLL) settings.
 *
 * @details     This API can be used to configure the MCLK frequency of ADAU1979 and the on-chip
 *              PLL settings.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  bClkMaster      'true'  to set ADAU1979 as LRCLK/BCLK Clock master,
 *                              'false' to use external LRCLK/BCLK master and set ADAU1979 BCLK as slave (default). *
 * @param [in]  eMClkFreq       Enumeration for the MCK frequency ranges.
 * @param [in]  ePLLSource      Enumeration for the sources of the input clock to the PLL.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully configured MCLK and PLL settings for ADAU1979.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1979_ALREADY_RUNNING [D]: This function should not be called when dataflow is enabled.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_ConfigPllClk(
    ADI_ADAU1979_HANDLE           const hDevice,
    bool                                bClkMaster,
    ADI_ADAU1979_MCLK_FREQ              eMClkFreq,
    ADI_ADAU1979_PLL_SOURCE             ePllSrc)
{
    /* Location to hold ADAU1979 register config value */
    uint8_t RegValue;
    /* Return Code */
    ADI_ADAU1979_RESULT eResult;
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

    /* IF (ADAU1979 dataflow enabled) */
    if (pDevice->Adc.bIsEnabled)
    {
        /* Report failure as ADAU1979 is already running */
        return (ADI_ADAU1979_ALREADY_RUNNING);
    }

    /* IF (MCLK is out of range) */
    if (eMClkFreq > ADI_ADAU1979_MCLK_MAX)
    {
        return (ADI_ADAU1979_MCLK_INVALID);
    }

#endif /* ADI_DEBUG */

    /* Save the MCLK value, can't use this until the sampling rate is set */
    pDevice->Adc.eMClkFreq = eMClkFreq;

    /* Read PLL Control Register 0 */
    eResult = ReadAdau1979 (pDevice, ADI_ADAU1979_REG_PLL_CTL, &RegValue);
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Return on error */
        return (eResult);
    }

    /* Clear PLL clock source field */
    RegValue &= (uint8_t) ~(BITM_ADAU1979_01_PLL_CLK_SRC);

    /* IF PLL clock source is LRCLK) */
    if (ePllSrc == ADI_ADAU1979_PLL_SOURCE_LRCLK)
    {
        RegValue |= (uint8_t) (ENUM_ADAU1979_01_PLL_CLK_LRCLK);
    }

    /* Configure PLL Control Register 0 */
    eResult = WriteAdau1979 (pDevice, ADI_ADAU1979_REG_PLL_CTL, RegValue);
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Return on error */
        return (eResult);
    }

    /* Read Serial Port Control Register 2 0x06 */
    eResult = ReadAdau1979 (pDevice, ADI_ADAU1979_REG_SAI_CTL2, &RegValue);
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Return on error */
        return (eResult);
    }

    /* Clear Serial Port Control Register 2 BCLK/LRCLK master and LRCLK pulse fields */
    RegValue &= (uint8_t) ~(BITM_ADAU1979_06_CLK_MS);

    /* IF (BCLK/LRCLK as Master) */
    if (bClkMaster)
    {
        RegValue |= (uint8_t) (ENUM_ADAU1979_06_CLK_MASTER);
    }

    /* Update Serial Port Control Register 2 0x06 */
    eResult = WriteAdau1979 (pDevice, ADI_ADAU1979_REG_SAI_CTL2, RegValue);
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Return on error */
        return (eResult);
    }

    /* Return */
    return (eResult);
}

/**
 * @brief Configure ADAU1979 Rx (ADC) Clock settings.
 *
 * @details     This API configures the ADAU1979 Clock settings specific to Rx (ADC) module.
 *
 * @param [in]  hDevice                     ADAU1979 instance to work on.
 * @param [in]  bBClkRisingEdgeDriveData    'true'  to drive audio data port on raising edge of BCLK (default),
 *                                          'false' to drive audio data port on falling edge of BCLK.
 * @param [in]  bLRClkHighLow               'true'  to set LRCLK to go high then low,
 *                                          'false' to set LRCLK to go low then high, (default).


 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully configured ADAU1979 Rx (ADC) Clock settings.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1979_ALREADY_RUNNING [D]: This function should not be called when dataflow is enabled.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_ConfigSerialClk(
    ADI_ADAU1979_HANDLE     const hDevice,
    bool                          bBClkRisingEdgeDriveData,
    bool                          bLRClkHighLow)
{
    /* Location to hold ADAU1979 register config value */
    uint8_t               RegValue;
    /* Return Code */
    ADI_ADAU1979_RESULT eResult;
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

    /* IF (ADAU1979 Rx dataflow enabled) */
    if (pDevice->Adc.bIsEnabled)
    {
        /* This operations should not be carried out when Rx dataflow is enabled, report error */
        return (ADI_ADAU1979_ALREADY_RUNNING);
    }

#endif /* ADI_DEBUG */

    /* Read Power/serial port register 0x04 */
    eResult = ReadAdau1979 (pDevice, ADI_ADAU1979_REG_BLK_PWR_SAI, &RegValue);
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Return on error */
        return (eResult);
    }

    /* Clear Power/serial port register LRCLK polarity and BCLK polarity edge fields */
    RegValue &= (uint8_t) (~(BITM_ADAU1979_04_LRCLK_POL | BITM_ADAU1979_04_BCLK_POL));

    /* IF (LRCLK polarity - High then low) */
    if (bLRClkHighLow)
    {
        RegValue |= (uint8_t) (ENUM_ADAU1979_04_LRCLK_POL_HIGH_LOW);
    }

    /* IF (BCLK Polarity - Drive out on rising edge) */
    if (bBClkRisingEdgeDriveData)
    {
        RegValue |= (uint8_t) (ENUM_ADAU1979_04_BCLK_POL_RAISE);
    }

    /* Configure Power/serial port register 0x04 */
    eResult = WriteAdau1979 (pDevice, ADI_ADAU1979_REG_BLK_PWR_SAI, RegValue);
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Return on error */
        return (eResult);
    }

    /* store these parameters so that they can be used to configure the SPORT
     * just prior to enabling dataflow
     */
    pDevice->Adc.bBClkRisingEdgeLatch = !bBClkRisingEdgeDriveData;
    pDevice->Adc.bLRClkHighLow        = bLRClkHighLow;

    /* Return */
    return (eResult);
}


/**
 * @brief   Sets ADAU1979 sampling rate.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  eSampleRate     Enumeration for the ADC's sampling rates.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully updated ADC sample rate.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1979_ALREADY_RUNNING [D]: This function should not be called when dataflow is enabled.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_SetSampleRate(
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_SAMPLE_RATE            eSampleRate)
{
    /* Variables to manipulate ADAU1979 register values */
    uint8_t MasterClockSetting;

    /* Location to hold ADAU1979 register config value */
    uint8_t RegValue;

    /* Return Code */
    ADI_ADAU1979_RESULT eResult;

    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

    /* IF (ADAU1979 ADC dataflow enabled) */
    if (pDevice->Adc.bIsEnabled)
    {
        /* This operations should not be carried out when Rx dataflow is enabled, report error */
        return (ADI_ADAU1979_ALREADY_RUNNING);
    }

    /* IF (ADAU1979 ADC has its MCLK frequency set) */
    if (pDevice->Adc.eMClkFreq  == ADI_ADAU1979_MCLK_IN_FREQ_NONE)
    {
        /* cannot set sampling rate without an MCLK value */
        return (ADI_ADAU1979_MCLK_INVALID);
    }

#endif /* ADI_DEBUG */

    /* retrieve the master clock setting value from the lookup table */
    MasterClockSetting= aMasterClockSelectSetting[pDevice->Adc.eMClkFreq][eSampleRate];

    /* check if the MCLK value and the sample rate combination is valid */
    if(MasterClockSetting == MCS_NONE)
    {
        /* Report error */
        return (ADI_ADAU1979_MCLK_INVALID);
    }

    /* Save ADC Sample rate */
    pDevice->Adc.eSampleRate = eSampleRate;

    /* Read PLL Control Register 0x01 */
    eResult = ReadAdau1979 (pDevice, ADI_ADAU1979_REG_PLL_CTL, &RegValue);
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Return on error */
        return (eResult);
    }

    /* Clear the Master clock select fields */
    RegValue &= ~BITM_ADAU1979_01_MCS;

    /* Update the Master clock select fields */
    RegValue |= MasterClockSetting;

    /* Update PLL Control Register 0x01 */
    eResult = WriteAdau1979 (pDevice, ADI_ADAU1979_REG_PLL_CTL, RegValue);

    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Return on error */
        return (eResult);
    }

    /* Read Serial Port Control Register1 0x05 */
    eResult = ReadAdau1979 (pDevice, ADI_ADAU1979_REG_SAI_CTL1, &RegValue);
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Return on error */
        return (eResult);
    }

    /* Clear the sampling rate fields */
    RegValue &= ~BITM_ADAU1979_05_SR;

    /* determine the sample rate range and update sample rate fields  */
    switch (eSampleRate)
    {
        case ADI_ADAU1979_SAMPLE_RATE_32000HZ:
        case ADI_ADAU1979_SAMPLE_RATE_44100HZ:
        case ADI_ADAU1979_SAMPLE_RATE_48000HZ:
            RegValue |= ENUM_ADAU1979_05_SR_32_48KHZ;
            break;

        case ADI_ADAU1979_SAMPLE_RATE_96000HZ:
            RegValue |= ENUM_ADAU1979_05_SR_64_96KHZ;
            break;

        case ADI_ADAU1979_SAMPLE_RATE_192000HZ:
            RegValue |= ENUM_ADAU1979_05_SR_128_192KHZ;
            break;

        default:
            eResult = ADI_ADAU1979_FN_NOT_SUPPORTED;
            break;
    }

    if (eResult == ADI_ADAU1979_SUCCESS)
    {

        /* Update Serial Port Control Register1 0x05 */
        eResult = WriteAdau1979 (pDevice, ADI_ADAU1979_REG_SAI_CTL1, RegValue);

    }

    /* Return */
    return (eResult);
}

/**
 * @brief Power-up/Power-down ADAU1979 ADC module.
 *
 * @details     This API can be used to power-up or power-down the ADAU1979 ADC modules.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  ePowerEnable    Enumeration for each of the ADC's power up modules.
 * @param [in]  bPowerUp        'true' to power-up (ADC) module, 'false' to power-down.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully Power-up/Power-down ADAU1979 Rx (ADC) module.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1979_ALREADY_RUNNING [D]: This function should not be called when dataflow is enabled.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_PowerUp(
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_POWER_ENABLE           ePowerEnable,
    bool                                bPowerUp)
{
    /* Location to hold ADAU1979 register config value */
    uint8_t RegValue;

    /* Storage for the power register */
    ADI_ADAU1979_REG PowerRegister;
    /* Storage for the power mask */
    uint8_t PowerMask;
    /* Return Code */
    ADI_ADAU1979_RESULT eResult;
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

    /* IF (ADAU1979 Rx dataflow enabled) */
    if (pDevice->Adc.bIsEnabled)
    {
        /* Report failure as ADAU1979 is already running */
        return (ADI_ADAU1979_ALREADY_RUNNING);
    }

#endif /* ADI_DEBUG */

    /* If its a master power control, modify Register (0x00) */
    if(ePowerEnable == ADI_ADAU1979_POWER_MASTER)
    {
        PowerRegister = ADI_ADAU1979_REG_M_PWR;
        PowerMask     = (uint8_t)ENUM_ADAU1979_00_MST_PWR_EN;
    }
    /* If its an ADC component level power control, modify Register (0x04) */
    else
    {
        PowerRegister = ADI_ADAU1979_REG_BLK_PWR_SAI;
        PowerMask     = (uint8_t)ePowerEnable;
    }

    /* Read register */
    eResult = ReadAdau1979 (pDevice, PowerRegister, &RegValue);
    if (eResult != ADI_ADAU1979_SUCCESS)
    {
        /* Return on error */
        return (eResult);
    }

    /* Clear the power enable field */
    RegValue &= ~(uint8_t)PowerMask;

    /* IF (Power-up update field ) */
    if (bPowerUp)
    {
        RegValue |= (uint8_t)PowerMask;
    }

    /* Configure register */
    eResult = WriteAdau1979 (pDevice, PowerRegister, RegValue);

    /* Return */
    return (eResult);
}


/**
 * @brief Sets ADAU1979 (ADC) channel data word width.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  eWordWidth      ADC data word width.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully updated ADC word width.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *          - #ADI_ADAU1979_ALREADY_RUNNING [D]: This function should not be called when dataflow is enabled.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_SetWordWidth(
    ADI_ADAU1979_HANDLE         const hDevice,
    ADI_ADAU1979_WORD_WIDTH           eWordWidth)
{
    /* Location to hold register configuration value */
    uint8_t RegValue;

    /* Configuration value */
    uint8_t Value;
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

    /* IF (ADAU1979 ADC dataflow enabled) */
    if (pDevice->Adc.bIsEnabled)
    {
        /* This operations should not be carried out when Rx dataflow is enabled, report error */
        return (ADI_ADAU1979_ALREADY_RUNNING);
    }

#endif /* ADI_DEBUG */

    /* determine the sample rate range and update sample rate fields  */
    switch (eWordWidth)
    {
        case ADI_ADAU1979_WORD_WIDTH_16: /* 16-bit audio */
            Value = (  ENUM_ADAU1979_06_SLOT_16 
                     | ENUM_ADAU1979_06_WORD_16
                     | ENUM_ADAU1979_06_BCLK_16); /* 0x52u */
            break;
        case ADI_ADAU1979_WORD_WIDTH_24: /* 24-bit audio */
            Value = (  ENUM_ADAU1979_06_SLOT_32
                     | ENUM_ADAU1979_06_WORD_24
                     | ENUM_ADAU1979_06_BCLK_32); /* 0x0u */
            break;
        default:
            eResult = ADI_ADAU1979_FN_NOT_SUPPORTED;
            break;
    }

    if (eResult == ADI_ADAU1979_SUCCESS)
    {

        /* Read Serial Port Control Register 2 0x06 */
        eResult = ReadAdau1979 (pDevice, ADI_ADAU1979_REG_SAI_CTL2, &RegValue);

        if (eResult == ADI_ADAU1979_SUCCESS)
        {

            /* Clear ADC Word width field */
            RegValue &= (uint8_t) ~(BITM_ADAU1979_06_WORD_WIDTH | BITM_ADAU1979_06_SLOT_WIDTH | BITM_ADAU1979_06_BCLK_RATE);

            /* Update ADC Word width */
            RegValue |= (uint8_t)Value;

            /* Update driver cache */
            pDevice->Adc.eWordLength = eWordWidth;

            /* Configure Serial Port Control Register 2 0x06 */
            eResult = WriteAdau1979 (pDevice, ADI_ADAU1979_REG_SAI_CTL2, RegValue);
        }
    }

    /* Return */
    return (eResult);
}


/**
 * @brief Sets ADAU1979 Channel volume.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  eChannel        Enumeration for the channel volume configuration.
 * @param [in]  Volume          Volume level (range 0x00 to 0xFE, 0xFF Mute)
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully set DAC channel volume.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_SetVolume(
    ADI_ADAU1979_HANDLE     const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL    eChannel,
    uint8_t                      Volume)
{
    /* Register address to access */
    uint8_t RegAddr;
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *)hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

    /* Check volume range, 0x00 to 0xFE, throw error on 0xFF as it is the mute level */
    if (Volume > 0xFEu)
    {
        /* Return error as function not supported for this channel */
        return (ADI_ADAU1979_FN_NOT_SUPPORTED);
    }

#endif /* ADI_DEBUG */

    /* determine the sample rate range and update sample rate fields  */
    switch (eChannel)
    {
        case ADI_ADAU1979_AUDIO_CHANNEL1:
            RegAddr  = (uint8_t)ADI_ADAU1979_REG_POSTADC_GAIN1;
            pDevice->Adc.Chnl1Volume = Volume;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL2:
            RegAddr  = (uint8_t)ADI_ADAU1979_REG_POSTADC_GAIN2;
            pDevice->Adc.Chnl2Volume = Volume;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL3:
            RegAddr  = (uint8_t)ADI_ADAU1979_REG_POSTADC_GAIN3;
            pDevice->Adc.Chnl3Volume = Volume;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL4:
            RegAddr  = (uint8_t)ADI_ADAU1979_REG_POSTADC_GAIN4;
            pDevice->Adc.Chnl4Volume = Volume;
            break;
        default:
            /* Return error as channel is not supported */
            eResult = ADI_ADAU1979_FN_NOT_SUPPORTED;
            break;
    }

    if (eResult == ADI_ADAU1979_SUCCESS)
    {
        /* Configure Volume Register */
        eResult = WriteAdau1979 (pDevice, (ADI_ADAU1979_REG) RegAddr, Volume);
    }

    /* Return success */
    return (eResult);
}

/**
 * @brief Mute/Un-mute ADAU1979 Channel.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  eChannel        Audio Channel to configure.
 * @param [in]  bMute           'true' to Mute channel, 'false' to Un-mute.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully updated audio channel mute settings.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_MuteChannel (
    ADI_ADAU1979_HANDLE     const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL    eChannel,
    bool                          bMute)
{
    /* ADAU1979 register field mask to configure */
    uint8_t FieldMask;
    /* Location to hold register value */
    uint8_t RegValue;
    /* ADAU1979 Register address to access */
    ADI_ADAU1979_REG RegAddr;
    /* Location to hold Mute */
    uint8_t Mute;
    /* Location to hold UnMute */
    uint8_t UnMuteVolume;
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* IF (No Audio channel specified) */
    if (eChannel == ADI_ADAU1979_AUDIO_CHANNEL_NONE)
    {
        return (ADI_ADAU1979_FN_NOT_SUPPORTED);
    }

    /* determine which channel to configure  */
    switch (eChannel)
    {
        case ADI_ADAU1979_AUDIO_CHANNEL1:
            RegAddr      = ADI_ADAU1979_REG_POSTADC_GAIN1;
            UnMuteVolume = pDevice->Adc.Chnl1Volume;
            Mute         = 0xFFu;
            FieldMask    = 0xFFu;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL2:
            RegAddr      = ADI_ADAU1979_REG_POSTADC_GAIN2;
            UnMuteVolume = pDevice->Adc.Chnl2Volume;
            Mute         = 0xFFu;
            FieldMask    = 0xFFu;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL3:
            RegAddr      = ADI_ADAU1979_REG_POSTADC_GAIN3;
            UnMuteVolume = pDevice->Adc.Chnl3Volume;
            Mute         = 0xFFu;
            FieldMask    = 0xFFu;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL4:
            RegAddr      = ADI_ADAU1979_REG_POSTADC_GAIN4;
            UnMuteVolume = pDevice->Adc.Chnl4Volume;
            Mute         = 0xFFu;
            FieldMask    = 0xFFu;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL_ALL:
            RegAddr      = ADI_ADAU1979_REG_MISC_CTL;
            UnMuteVolume = (uint8_t)ENUM_ADAU1979_0E_UNMUTE_ALL;
            Mute         = (uint8_t)ENUM_ADAU1979_0E_MUTE_ALL;
            FieldMask    = (uint8_t)BITM_ADAU1979_0E_MUTE;
            break;
        default:
            eResult = ADI_ADAU1979_FN_NOT_SUPPORTED;
            break;
    }

    if (eResult == ADI_ADAU1979_SUCCESS)
    {

        /* Read Register */
        eResult = ReadAdau1979 (pDevice, RegAddr, &RegValue);

        if (eResult == ADI_ADAU1979_SUCCESS)
        {

            /* Clear register value */
            RegValue &= ~FieldMask;

            if (bMute)
            {
                RegValue |= Mute;
            }
            else
            {
                RegValue |= UnMuteVolume;
            }

            /* Configure Mute */
            eResult = WriteAdau1979 (pDevice, RegAddr, RegValue);
        }
    }

    return eResult;

}

/**
 * @brief Enables/disables ADAU1979 channel clipping.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  eChannel        Audio Channel to configure.
 * @param [in]  bClip           'true' to clip channel, 'false' do not clip channel.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully updated audio channel mute settings.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_ClipChannel (
    ADI_ADAU1979_HANDLE     const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL    eChannel,
    bool                          bClip)
{
    /* ADAU1979 register field mask to configure */
    uint8_t FieldMask;
    /* Location to hold register value */
    uint8_t RegValue;
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* determine the sample rate range and update sample rate fields  */
    switch (eChannel)
    {
        case ADI_ADAU1979_AUDIO_CHANNEL1:
            FieldMask    = (uint8_t)BITM_ADAU1979_19_CH1_CLIP;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL2:
            FieldMask    = (uint8_t)BITM_ADAU1979_19_CH2_CLIP;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL3:
            FieldMask    = (uint8_t)BITM_ADAU1979_19_CH3_CLIP;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL4:
            FieldMask    = (uint8_t)BITM_ADAU1979_19_CH4_CLIP;
            break;
        default:
            eResult = ADI_ADAU1979_FN_NOT_SUPPORTED;
            break;
    }

    if (eResult == ADI_ADAU1979_SUCCESS)
    {

        /* Read Register */
        eResult = ReadAdau1979 (pDevice, ADI_ADAU1979_REG_ASC_CLIP, &RegValue);
        if (eResult == ADI_ADAU1979_SUCCESS)
        {

            /* Clear register value */
            RegValue &= ~FieldMask;

            if (bClip)
            {
                RegValue |= (uint8_t)FieldMask;
            }

            /* Configure clipping */
            eResult = WriteAdau1979 (pDevice, ADI_ADAU1979_REG_ASC_CLIP, RegValue);
        }
    }

    return eResult;
}

/**
 * @brief Enables/disables ADAU1979 channel calibration.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  eChannel        Audio Channel to configure.
 * @param [in]  bCalibrate      'true' to calibrate channel, 'false' for no calibration.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully updated audio channel mute settings.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_CalibrateChannel (
    ADI_ADAU1979_HANDLE const  hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL eChannel,
    bool                       bCalibrate)
{
    /* ADAU1979 register field mask to configure */
    uint8_t FieldMask;
    /* Location to hold register value */
    uint8_t RegValue;
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* determine the sample rate range and update sample rate fields  */
    switch (eChannel)
    {
        case ADI_ADAU1979_AUDIO_CHANNEL1:
            FieldMask    = (uint8_t)BITM_ADAU1979_1A_CH1_CAL;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL2:
            FieldMask    = (uint8_t)BITM_ADAU1979_1A_CH2_CAL;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL3:
            FieldMask    = (uint8_t)BITM_ADAU1979_1A_CH3_CAL;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL4:
            FieldMask    = (uint8_t)BITM_ADAU1979_1A_CH4_CAL;
            break;
        default:
            eResult = ADI_ADAU1979_FN_NOT_SUPPORTED;
            break;
    }

    if (eResult == ADI_ADAU1979_SUCCESS)
    {

        /* Read Register */
        eResult = ReadAdau1979 (pDevice, ADI_ADAU1979_REG_DC_HPF_CAL, &RegValue);

        if (eResult == ADI_ADAU1979_SUCCESS)
        {

            /* Clear register value */
            RegValue &= ~(FieldMask);

            if (bCalibrate)
            {
                RegValue |= (uint8_t)FieldMask;
            }

            /* Configure calibration */
            eResult = WriteAdau1979 (pDevice, ADI_ADAU1979_REG_DC_HPF_CAL, RegValue);

        }
    }

    return eResult;

}

/**
 * @brief Enables/disables ADAU1979 channel high pass filtering.
 *
 * @param [in]  hDevice         ADAU1979 instance to work on.
 * @param [in]  eChannel        Audio Channel to configure.
 * @param [in]  bHighPass       'true' to high pass filter channel, 'false' to no high pass filter.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully updated audio channel mute settings.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_HighPassChannel (
    ADI_ADAU1979_HANDLE     const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL    eChannel,
    bool                          bHighPass)
{
    /* ADAU1979 register field mask to configure */
    uint8_t FieldMask;
    /* Location to hold register value */
    uint8_t RegValue;
    /* Return code */
    ADI_ADAU1979_RESULT eResult = ADI_ADAU1979_SUCCESS;
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* determine the sample rate range and update sample rate fields  */
    switch (eChannel)
    {
        case ADI_ADAU1979_AUDIO_CHANNEL1:
            FieldMask    = (uint8_t)BITM_ADAU1979_1A_CH1_DCHPF;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL2:
            FieldMask    = (uint8_t)BITM_ADAU1979_1A_CH2_DCHPF;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL3:
            FieldMask    = (uint8_t)BITM_ADAU1979_1A_CH3_DCHPF;
            break;
        case ADI_ADAU1979_AUDIO_CHANNEL4:
            FieldMask    = (uint8_t)BITM_ADAU1979_1A_CH4_DCHPF;
            break;
        default:
            eResult = ADI_ADAU1979_FN_NOT_SUPPORTED;
            break;
    }

    if (eResult == ADI_ADAU1979_SUCCESS)
    {

        /* Read Register */
        eResult = ReadAdau1979 (pDevice, ADI_ADAU1979_REG_DC_HPF_CAL, &RegValue);

        if (eResult == ADI_ADAU1979_SUCCESS)
        {

            /* Clear register value */
            RegValue &= ~FieldMask;

            if (bHighPass)
            {
                RegValue |= (uint8_t)FieldMask;
            }

            /* Configure high pass filter */
            eResult = WriteAdau1979 (pDevice, ADI_ADAU1979_REG_DC_HPF_CAL, RegValue);
        }
    }

    return eResult;
}


/**
 * @brief Read ADAU1979 Audio Codec register.
 *
 * @param [in]  hDevice     ADAU1979 instance to work on.
 * @param [in]  RegAddr     Register address to access
 * @param [in]  pRegValue   Pointer to location to store register value.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully read ADAU1979 register.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_ReadRegister (
    ADI_ADAU1979_HANDLE const hDevice,
    ADI_ADAU1979_REG          RegAddr,
    uint8_t                   *pRegValue)
{
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* Read ADAU1979 register */
    return (ReadAdau1979 (pDevice, RegAddr, pRegValue));
}


/**
 * @brief Configure ADAU1979 Audio Codec register.
 *
 * @param [in]  hDevice     ADAU1979 instance to work on.
 * @param [in]  RegAddr     Register address to access.
 * @param [in]  RegValue    Register value to write.
 *
 * @return  Status
 *          - #ADI_ADAU1979_SUCCESS: Successfully configured ADAU1979 register.
 *          - #ADI_ADAU1979_BAD_HANDLE [D]: Supplied Device handle is invalid.
 *
 */
ADI_ADAU1979_RESULT adi_adau1979_WriteRegister (
    ADI_ADAU1979_HANDLE const hDevice,
    ADI_ADAU1979_REG          RegAddr,
    uint8_t                   RegValue)
{
    /* Pointer to ADAU1979 device instance to work on */
    ADI_ADAU1979_DEV      *pDevice = (ADI_ADAU1979_DEV *) hDevice;

/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

    /* IF (ADAU1979 Handle is invalid) */
    if (ValidateHandle (hDevice) != ADI_ADAU1979_SUCCESS)
    {
        return (ADI_ADAU1979_BAD_HANDLE);
    }

#endif /* ADI_DEBUG */

    /* Configure ADAU1979 register */
    return (WriteAdau1979 (pDevice, RegAddr, RegValue));
}



/*****/

/*@}*/

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */

