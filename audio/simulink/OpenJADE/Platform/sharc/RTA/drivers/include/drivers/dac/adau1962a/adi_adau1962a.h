/*********************************************************************************

Copyright(c) 2014-2015 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

/**
 * @file:    adi_adau1962a.h
 * @brief:   ADAU1962A DAC driver definitions and API
 * @version: $Revision: 25389 $
 * @date:    $Date: 2016-02-26 12:32:28 -0500 (Fri, 26 Feb 2016) $
 *
 * @details
 *            This is the primary header file for ADAU1962A DAC driver.
 *            The file contains the API declarations, data structure and constant
 *            definitions used in the APIs.
 */

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_2_4:"Allows doxygen addtogroup block")
#endif

/** @addtogroup ADI_ADAU1962A_Driver ADAU1962A DAC driver
 *  @{
 */

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif

#ifndef __ADI_ADAU1962A_H__
#define __ADI_ADAU1962A_H__

/*=============  I N C L U D E S   =============*/

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rules_all:"Disable all rules")
#endif


/* Interrupt Manager includes */
#include <services/int/adi_int.h>
/* TWI driver includes */
#include <drivers/twi/adi_twi.h>
/* SPORT driver includes */
#include <drivers/sport/adi_sport.h>

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_2_4:"Allows doxygen addtogroup block")
#pragma diag(suppress:misra_rule_5_1:"Allow identifiers to be more than 31 characters")
#pragma diag(suppress:misra_rule_5_7:"Allow identifier names to be reused")
#pragma diag(suppress:misra_rule_9_3:"Allow some unspecified enum values as adi_sport.h header doesn't have all necessary values")
#endif

/*==============  D E F I N E S  ===============*/

#if !defined(_LANGUAGE_ASM)

/* IF (ADSP-SC5xx processor family) */
#if defined(__ADSPSC589_FAMILY__) || defined(__ADSPSC573_FAMILY__)
/*! Memory size required to handle an ADAU1962A DAC instance */
#define ADI_ADAU1962A_MEMORY_SIZE               (60u)
/* ELSE (Other Processor family) */
#else
#error "Processor not supported"
#endif /* __ADSPSC5XX_FAMILY__ */

/*! ADAU1962A DAC Handle */
typedef void*   ADI_ADAU1962A_HANDLE;

/*! Enumeration of ADAU1962A TWI address */
typedef enum _ADI_ADAU1962A_TWI_ADDR_
{
    ADI_ADAU1962A_TWI_ADDR_04 = (0x04u),		/*!< TWI address 0x04 */
    ADI_ADAU1962A_TWI_ADDR_24 = (0x24u),		/*!< TWI address 0x24 */
    ADI_ADAU1962A_TWI_ADDR_44 = (0x44u),		/*!< TWI address 0x44 */
    ADI_ADAU1962A_TWI_ADDR_64 = (0x64u)		    /*!< TWI address 0x64 */
} ADI_ADAU1962A_TWI_ADDR;

/*! Enumeration of ADAU1962A driver specific result codes. */
typedef enum _ADI_ADAU1962A_RESULT_
{
    ADI_ADAU1962A_SUCCESS = (0u),			    /*!< 0x00 - Success */
    ADI_ADAU1962A_FAILURE,					    /*!< 0x01 - Generic Failure */
    ADI_ADAU1962A_IN_USE,						/*!< 0x02 - Device Number already in use */
    ADI_ADAU1962A_BAD_HANDLE,					/*!< 0x03 - Supplied device handle is invalid */
    ADI_ADAU1962A_NULL_POINTER,                 /*!< 0x04 - Given pointer parameters are valid  */
    ADI_ADAU1962A_ALREADY_RUNNING,			    /*!< 0x05 - Can't perform the requested operation when the dataflow is enabled */
    ADI_ADAU1962A_INSUFFICIENT_MEMORY, 	        /*!< 0x06 - Supplied memory is insufficient to handle an ADAU1962A device instance */
    ADI_ADAU1962A_TWI_NOT_CONFIGURED,           /*!< 0x08 - TWI device configuration not provided yet */
    ADI_ADAU1962A_TWI_ERROR,				    /*!< 0x09 - TWI driver error */
    ADI_ADAU1962A_SPORT_CONFIG_INVALID,	        /*!< 0x0A - SPORT configuration invalid */
    ADI_ADAU1962A_SPORT_ERROR,				    /*!< 0x0B - SPORT driver error */
    ADI_ADAU1962A_SPORT_HW_ERROR,               /*!< 0x0C - SPORT hardware error */
    ADI_ADAU1962A_FN_NOT_SUPPORTED,             /*!< 0x0E - Function not supported */
    ADI_ADAU1962A_FMT_NOT_SUPPORTED,            /*!< 0x0F - Selected ADAU1962A Serial mode format is not supported */
    ADI_ADAU1962A_MCLK_INVLAID,                 /*!< 0x10 - MCLK input value is invalid */
    ADI_ADAU1962A_MCLK_SAMPLE_RATE_MISMATCH,    /*!< 0x11 - Selected sampling rate is not supported by the current MCLK input */
    ADI_ADAU1962A_FMT_CHNL_NUM_MISMATCH,        /*!< 0x12 - Supplied number of channels not supported for the selected serial format */
    ADI_ADAU1962A_FMT_SAMPLE_RATE_MISMATCH,     /*!< 0x13 - Supplied sampling rate is not supported by the selected serial format */
    ADI_ADAU1962A_BUF_SUBMIT_FAILED,			/*!< 0x14 - Failed to submit an audio buffer */
    ADI_ADAU1962A_BUF_Q_FULL,                   /*!< 0x15 - Buffer queue is full and can't accept anymore buffers */
    ADI_ADAU1962A_SAMPLE_RATE_NOT_SUPPORTED,    /*!< 0x16 - Supplied sampling rate is not supported by this device */
    ADI_ADAU1962A_CHNL_NOT_VALID,               /*!< 0x17 - Supplied ADAU1962A audio channel ID is invalid for the selected operation */
    ADI_ADAU1962A_INVALID_FN_CALL,              /*!< 0x18 - Invalid function call for the current serial mode configuration */
    ADI_ADAU1962A_CHNL_ID_DIRECTION_MISMATCH,   /*!< 0x19 - Audio channel ids mapped to a SPORT device should have the same data direction */
    ADI_ADAU1962A_SERIAL_PORT_CONFIGURED,       /*!< Serial data port has already been configured with a SPORT device */
    ADI_ADAU1962A_SERIAL_PORT_NOT_USED 	        /*!< Serial data port will not be used for the current serial data mode */
} ADI_ADAU1962A_RESULT;

/*! Enumeration for the ADAU1962A driver specific callback events */
typedef enum _ADI_ADAU1962A_EVENT_
{
    ADI_ADAU1962A_EVENT_DMA_ERROR,              /*!< SPORT DMA Error (pArg = None) */
    ADI_ADAU1962A_EVENT_BUFFER_PROCESSED        /*!< Processed an Audio buffer
                                                     (pArg = Start address of the processed audio buffer) */
} ADI_ADAU1962A_EVENT;

/*! Enumeration of serial mode configurations supported the driver */
typedef enum _ADI_ADAU1962A_SERIAL_MODE_
{
    ADI_ADAU1962A_SERIAL_MODE_STEREO = (0u),     /*!< Configure the device and SPORT for stereo (I2S) transfer mode (default) */
    ADI_ADAU1962A_SERIAL_MODE_TDM2   = (1u),     /*!< Configure the device and SPORT for Octal line TDM mode */
    ADI_ADAU1962A_SERIAL_MODE_TDM4   = (3u),     /*!< Configure the device and SPORT for Quad line TDM mode */
    ADI_ADAU1962A_SERIAL_MODE_TDM8   = (7u),     /*!< Configure the device and SPORT for Dual line TDM mode */
    ADI_ADAU1962A_SERIAL_MODE_TDM16  = (15u)     /*!< Configure the device and SPORT for Single line TDM mode */
 } ADI_ADAU1962A_SERIAL_MODE;

/*! Enumeration of Audio data word width configurations supported by ADAU1962A driver */
typedef enum _ADI_ADAU1962A_WORD_WIDTH_
{
    ADI_ADAU1962A_WORD_WIDTH_24 = (23u),        /*!< Configure the device and SPORT for 24-bit audio data transfer mode (default) */
    ADI_ADAU1962A_WORD_WIDTH_20 = (19u),        /*!< Configure the device and SPORT for 20-bit audio data transfer mode */
    ADI_ADAU1962A_WORD_WIDTH_16 = (15u)         /*!< Configure the device and SPORT for 16-bit audio data transfer mode */
} ADI_ADAU1962A_WORD_WIDTH;

/*! Enumeration of SPORT channels that can be mapped to ADAU1962A serial data input port */
typedef enum _ADI_ADAU1962A_SPORT_CHANNEL_
{
    ADI_ADAU1962A_SPORT_A = (ADI_HALF_SPORT_A),     /*!< Sport A - First half sport */
    ADI_ADAU1962A_SPORT_B = (ADI_HALF_SPORT_B),     /*!< Sport B - Second half sport */
    ADI_ADAU1962A_SPORT_NONE                        /*!< No Sport */
} ADI_ADAU1962A_SPORT_CHANNEL;

/*! Enumeration of ADAU1962A Serial data input ports */
typedef enum _ADI_ADAU1962A_SERIAL_PORT_
{
    ADI_ADAU1962A_SERIAL_PORT_DSDATA1 = (0u),   /*!< DAC  1 and DAC  2 Serial Data Input port */
    ADI_ADAU1962A_SERIAL_PORT_DSDATA2,          /*!< DAC  3 and DAC  4 Serial Data Input port */
    ADI_ADAU1962A_SERIAL_PORT_DSDATA3,          /*!< DAC  5 and DAC  6 Serial Data Input port */
    ADI_ADAU1962A_SERIAL_PORT_DSDATA4,          /*!< DAC  7 and DAC  8 Serial Data Input port */
    ADI_ADAU1962A_SERIAL_PORT_DSDATA5,          /*!< DAC  9 and DAC 10 Serial Data Input port */
    ADI_ADAU1962A_SERIAL_PORT_DSDATA6,          /*!< DAC 11 and DAC 12 Serial Data Input port */
    ADI_ADAU1962A_SERIAL_PORT_NONE              /*!< No Serial Data Input port selected */
} ADI_ADAU1962A_SERIAL_PORT;

/*! Enumeration of ADAU1962A DAC channels */
typedef enum _ADI_ADAU1962A_CHNL_
{
    ADI_ADAU1962A_CHNL_DAC_1 = (0u),            /*!< DAC Channel  1 */
    ADI_ADAU1962A_CHNL_DAC_2,                   /*!< DAC Channel  2 */
    ADI_ADAU1962A_CHNL_DAC_3,                   /*!< DAC Channel  3 */
    ADI_ADAU1962A_CHNL_DAC_4,                   /*!< DAC Channel  4 */
    ADI_ADAU1962A_CHNL_DAC_5,                   /*!< DAC Channel  5 */
    ADI_ADAU1962A_CHNL_DAC_6,                   /*!< DAC Channel  6 */
    ADI_ADAU1962A_CHNL_DAC_7,                   /*!< DAC Channel  7 */
    ADI_ADAU1962A_CHNL_DAC_8,                   /*!< DAC Channel  8 */
    ADI_ADAU1962A_CHNL_DAC_9,                   /*!< DAC Channel  9 */
    ADI_ADAU1962A_CHNL_DAC_10,                  /*!< DAC Channel 10 */
    ADI_ADAU1962A_CHNL_DAC_11,                  /*!< DAC Channel 11 */
    ADI_ADAU1962A_CHNL_DAC_12,                  /*!< DAC Channel 12 */
    ADI_ADAU1962A_CHNL_DAC_MSTR                 /*!< DAC Master control */
} ADI_ADAU1962A_CHNL;

/*! Enumeration of ADAU1962A Device Registers */
typedef enum _ADI_ADAU1962A_REG_
{
    ADI_ADAU1962A_REG_PLL_CLK_CTRL0         = (0x00u),      /*!< PLL and Clock Control 0 register */
    ADI_ADAU1962A_REG_PLL_CLK_CTRL1         = (0x01u),      /*!< PLL and Clock Control 1 register */
    ADI_ADAU1962A_REG_PDN_THRMSENS_CTRL_1   = (0x02u),      /*!< Block Power-down and Thermal sensor control 1 register */
    ADI_ADAU1962A_REG_PDN_CTRL2             = (0x03u),      /*!< Power-down control 2 */
    ADI_ADAU1962A_REG_PDN_CTRL3             = (0x04u),      /*!< Power-down control 3 */
    ADI_ADAU1962A_REG_THRM_TEMP_STAT        = (0x05u),      /*!< Thermal Sensor Temperature readout register */
    ADI_ADAU1962A_REG_DAC_CTRL0             = (0x06u),      /*!< DAC Control 0 register */
    ADI_ADAU1962A_REG_DAC_CTRL1             = (0x07u),      /*!< DAC Control 1 register */
    ADI_ADAU1962A_REG_DAC_CTRL2             = (0x08u),      /*!< DAC Control 2 register */
    ADI_ADAU1962A_REG_DAC_MUTE1             = (0x09u),      /*!< DAC Individual channel mutes 1 register */
    ADI_ADAU1962A_REG_DAC_MUTE2             = (0x0Au),      /*!< DAC Individual channel mutes 2 register */
    ADI_ADAU1962A_REG_DACMSTR_VOL           = (0x0Bu),      /*!< DAC Master Volume control register */
    ADI_ADAU1962A_REG_DAC1_VOL              = (0x0Cu),      /*!< DAC  1 Volume control register */
    ADI_ADAU1962A_REG_DAC2_VOL              = (0x0Du),      /*!< DAC  2 Volume control register */
    ADI_ADAU1962A_REG_DAC3_VOL              = (0x0Eu),      /*!< DAC  3 Volume control register */
    ADI_ADAU1962A_REG_DAC4_VOL              = (0x0Fu),      /*!< DAC  4 Volume control register */
    ADI_ADAU1962A_REG_DAC5_VOL              = (0x10u),      /*!< DAC  5 Volume control register */
    ADI_ADAU1962A_REG_DAC6_VOL              = (0x11u),      /*!< DAC  6 Volume control register */
    ADI_ADAU1962A_REG_DAC7_VOL              = (0x12u),      /*!< DAC  7 Volume control register */
    ADI_ADAU1962A_REG_DAC8_VOL              = (0x13u),      /*!< DAC  8 Volume control register */
    ADI_ADAU1962A_REG_DAC9_VOL              = (0x14u),      /*!< DAC  9 Volume control register */
    ADI_ADAU1962A_REG_DAC10_VOL             = (0x15u),      /*!< DAC 10 Volume control register */
    ADI_ADAU1962A_REG_DAC11_VOL             = (0x16u),      /*!< DAC 11 Volume control register */
    ADI_ADAU1962A_REG_DAC12_VOL             = (0x17u),      /*!< DAC 12 Volume control register */
    ADI_ADAU1962A_REG_PAD_STRGTH            = (0x1Cu),      /*!< Pad strength register */
    ADI_ADAU1962A_REG_DAC_POWER1            = (0x1Du),      /*!< DAC Power adjust 1 register */
    ADI_ADAU1962A_REG_DAC_POWER2            = (0x1Eu),      /*!< DAC Power adjust 2 register */
    ADI_ADAU1962A_REG_DAC_POWER3            = (0x1Fu)       /*!< DAC Power adjust 3 register */
} ADI_ADAU1962A_REG;

/*! Enumerations of ADAU1962A On-chip phase locked loop (PLL) input selection */
typedef enum _ADI_ADAU1962A_PLL_IN_
{
    ADI_ADAU1962A_PLL_IN_MCLKI_XTALI  = (0x00u),    /*!< Master Clock Input as PLL source (default) */
    ADI_ADAU1962A_PLL_IN_DLRCLK       = (0x40u)     /*!< DAC LR Clock as PLL source */
} ADI_ADAU1962A_PLL_IN;

/*! Enumerations of ADAU1962A Master clock source select options */
typedef enum _ADI_ADAU1962A_MCLK_SEL_
{
    ADI_ADAU1962A_MCLK_SEL_PLL = (0u),      /*!< MCLK from PLL */
    ADI_ADAU1962A_MCLK_SEL_MCLKI_XTALI      /*!< MCLK from MCLKI or XTALI */
}ADI_ADAU1962A_MCLK_SEL;

/*! Enumerations of ADAU1962A Master clock output (MCLKO/XO pin) settings */
typedef enum _ADI_ADAU1962A_MCLK_OUT_
{
    ADI_ADAU1962A_MCLK_OUT_4MHZ_TO_6MHZ   = (0x00u),  /*!< 4 MHz to 6 MHz */
    ADI_ADAU1962A_MCLK_OUT_8MHZ_TO_12MHZ  = (0x10u),  /*!< 8 MHz to 12 MHz */
    ADI_ADAU1962A_MCLK_OUT_BUFFERED_MCLKI = (0x20u),  /*!< Buffered MCLKI */
    ADI_ADAU1962A_MCLK_OUT_DISABLED       = (0x30u)   /*!< MCLK Out Pin Disabled */
} ADI_ADAU1962A_MCLK_OUT;

/*! Enumerations of ADAU1962A DAC power configuration options */
typedef enum _ADI_ADAU1962A_DAC_PWR_
{
    ADI_ADAU1962A_DAC_PWR_LOW = (0u),           /*!< Low Power - supported by DAC Master control and individual DAC channels */
    ADI_ADAU1962A_DAC_PWR_LOWEST,               /*!< Lowest Power - supported by DAC Master control and individual DAC channels */
    ADI_ADAU1962A_DAC_PWR_BEST_PERFORMANCE,     /*!< Best performance - supported by individual DAC channels only */
    ADI_ADAU1962A_DAC_PWR_GOOD_PERFORMANCE,     /*!< Good performance - supported by individual DAC channels only */
    ADI_ADAU1962A_DAC_PWR_INDIVIDUAL            /*!< Individual power control for each channel - supported by DAC Master control only */
}ADI_ADAU1962A_DAC_PWR;

/*! Enumerations of time interval between temperature conversion for ADAU1962A */
typedef enum _ADI_ADAU1962A_TEMP_CONVERT_
{
    ADI_ADAU1962A_TEMP_CONVERT_4S = (0u),       /*!< 4 sec/Conversion */
    ADI_ADAU1962A_TEMP_CONVERT_0_5S,            /*!< 0.5 sec/Conversion */
    ADI_ADAU1962A_TEMP_CONVERT_1S,              /*!< 1 sec/Conversion */
    ADI_ADAU1962A_TEMP_CONVERT_2S               /*!< 2 sec/Conversion */
} ADI_ADAU1962A_TEMP_CONVERT;

/*! Structure to hold TWI device information to access ADAU1962A registers */
typedef struct
{
    uint8_t                     TwiDevNum;          /*!< TWI device number allocated to this device */
    ADI_ADAU1962A_TWI_ADDR      eTwiAddr;           /*!< TWI slave address to access the device registers */
    void                        *pTwiDevMem;        /*!< Pointer to #ADI_TWI_MEMORY_SIZE memory to handle the TWI device */
    uint32_t                    TwiDevMemSize;      /*!< Memory size (#ADI_TWI_MEMORY_SIZE) of the buffer "pTwiDevMem" points to */
} ADI_ADAU1962A_TWI_CONFIG;

/*! Structure to hold SPORT device information to transfer audio samples to ADAU1962A DAC channels */
typedef struct
{
    uint8_t                     SportDevNum;        /*!< SPORT Device number to be used for stereo mode */
    ADI_ADAU1962A_SPORT_CHANNEL eSportChnl;         /*!< SPORT channel to be mapped */
    ADI_ADAU1962A_SERIAL_PORT   eSportPri;          /*!< ADAU1962A Serial Port mapped to SPORT half primary channel */
    ADI_ADAU1962A_SERIAL_PORT   eSportSec;          /*!< ADAU1962A Serial Port mapped to SPORT half secondary channel */
    void                        *pSportDevMem;      /*!< Pointer to #ADI_SPORT_DMA_MEMORY_SIZE sized memory to handle SPORT device */
    uint32_t                    SportDevMemSize;    /*!< Memory size (#ADI_SPORT_DMA_MEMORY_SIZE) of the buffer "pSportDevMem" points to */
} ADI_ADAU1962A_SPORT_CONFIG;

/*=======  P U B L I C   P R O T O T Y P E S  ========*/
/*            (globally-scoped functions)             */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * APIs to open/close device instance and hook it to Blackfin peripherals
 */

/* Opens an ADAU1962A instance for use */
ADI_ADAU1962A_RESULT adi_adau1962a_Open(
    uint8_t                                 DeviceNum,
    ADI_ADAU1962A_SERIAL_MODE               eSerialMode,
    void                            *const  pDeviceMemory,
    uint32_t                                MemorySize,
    ADI_ADAU1962A_HANDLE            *const  phDevice);

/* Closes an ADAU1962A instance in use */
ADI_ADAU1962A_RESULT adi_adau1962a_Close(
    ADI_ADAU1962A_HANDLE            const   hDevice);

/* Soft-reset ADAU1962A device */
ADI_ADAU1962A_RESULT  adi_adau1962a_SoftReset(
    ADI_ADAU1962A_HANDLE            const   hDevice);

/* Configure TWI device to be used to access ADAU1962A device registers */
ADI_ADAU1962A_RESULT adi_adau1962a_ConfigTwi(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_TWI_CONFIG                *pConfig);

/* Configure SPORT device to be used to transmit audio data to ADAU1962A */
ADI_ADAU1962A_RESULT   adi_adau1962a_ConfigSport (
    ADI_ADAU1962A_HANDLE           const    hDevice,
    ADI_ADAU1962A_SPORT_CONFIG              *pConfig);

/* Configure ADAU1962A PLL and Master clock settings */
ADI_ADAU1962A_RESULT adi_adau1962a_ConfigPllClk(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    uint32_t                                MClkInFreqHz,
    ADI_ADAU1962A_MCLK_SEL                  eMClkSelect,
    ADI_ADAU1962A_PLL_IN                    ePllInput);

/* Configure ADAU1962A Serial data clock settings */
ADI_ADAU1962A_RESULT adi_adau1962a_ConfigSerialClk(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    bClkMaster,
    bool                                    bBClkInternal,
    bool                                    bBClkFallLatch,
    bool                                    bLRClkPulse,
    bool                                    bLsbFirst,
    bool                                    bLeftChnlClkHi);

/* Configure ADAU1962A Master clock output settings */
ADI_ADAU1962A_RESULT adi_adau1962a_ConfigMClkOut(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    bEnableXtalOut,
    ADI_ADAU1962A_MCLK_OUT                  eMClkOut);

/* Power-up/Power-down individual blocks of ADAU1962A */
ADI_ADAU1962A_RESULT  adi_adau1962a_ConfigBlockPwr(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    bTempSensorPwrUp,
    bool                                    bPllPwrUp,
    bool                                    bVRegPwrUp);

/* Configure Thermal/Temperature Sensor on ADAU1962A for continuous conversion */
ADI_ADAU1962A_RESULT  adi_adau1962a_ContinuousTempSense(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_TEMP_CONVERT              eTimeInterval);

/* Configure Thermal/Temperature Sensor on ADAU1962A for one-shot conversion */
ADI_ADAU1962A_RESULT  adi_adau1962a_OneShotTempSense(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    uint8_t                                 TempInCelcius);

/* Configure ADAU1962A De-Emphasis settings */
ADI_ADAU1962A_RESULT  adi_adau1962a_ConfigDeEmphasis(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    bInvertOut,
    bool                                    bEnableAutoMute,
    bool                                    bOverSample128,
    bool                                    bEnableDeEmphasis);

/* Configure ADAU1962A DAC power settings */
ADI_ADAU1962A_RESULT  adi_adau1962a_ConfigDacPwr(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_CHNL                      eDacChnl,
    ADI_ADAU1962A_DAC_PWR                   eDacPwr,
    bool                                    bPwrUp);

/* Mute/Un-mute ADAU1962A Channel */
ADI_ADAU1962A_RESULT adi_adau1962a_MuteChannel (
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_CHNL                      eChannel,
    bool                                    bMute);

/* Sets ADAU1962A DAC Channel Volume */
ADI_ADAU1962A_RESULT adi_adau1962a_SetVolume(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_CHNL                      eChannel,
    uint8_t                                 Volume);

/* Sets ADAU1962A data word width */
ADI_ADAU1962A_RESULT adi_adau1962a_SetWordWidth(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_WORD_WIDTH                eWordWidth);

/* Sets ADAU1962A sample rate */
ADI_ADAU1962A_RESULT adi_adau1962a_SetSampleRate(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    uint32_t                                SampleRateHz);

/* Enable/Disable ADAU1962A audio playback */
ADI_ADAU1962A_RESULT  adi_adau1962a_Enable(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    bEnable);

/* Submits a buffer filled with audio samples to ADAU1962A for playback */
ADI_ADAU1962A_RESULT  adi_adau1962a_SubmitBuffer(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    void                                    *pBuffer,
    uint32_t                                BufferSize);

/* Peek function for non-OS application to check if a
   processed ADAU1962A audio buffer is available or not */
ADI_ADAU1962A_RESULT  adi_adau1962a_IsBufAvailable(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    bool                                    *pbIsBufAvailable);

/* Gets address of a processed ADAU1962A audio buffer */
ADI_ADAU1962A_RESULT  adi_adau1962a_GetBuffer(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    void                                    **ppBuffer);

/* Register a callback for reporting ADAU1962A related events */
ADI_ADAU1962A_RESULT  adi_adau1962a_RegisterCallback(
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_CALLBACK                            pfCallback,
    void                                    *pCBParam);

/* Read ADAU1962A device register */
ADI_ADAU1962A_RESULT adi_adau1962a_ReadRegister (
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_REG                       RegAddr,
    uint8_t                               	*pRegValue);

/* Write to ADAU1962A device register */
ADI_ADAU1962A_RESULT adi_adau1962a_WriteRegister (
    ADI_ADAU1962A_HANDLE            const   hDevice,
    ADI_ADAU1962A_REG                       RegAddr,
    uint8_t                               	RegValue);

#ifdef __cplusplus
}
#endif

#endif

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */

#endif  /* __ADI_ADAU1962A_H__ */

/*****/

/*@}*/

