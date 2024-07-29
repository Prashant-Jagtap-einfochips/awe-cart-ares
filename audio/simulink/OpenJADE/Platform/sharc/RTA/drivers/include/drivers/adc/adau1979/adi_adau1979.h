/*********************************************************************************

Copyright(c) 2013-2015 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

/**
 * @file:    adi_adau1979.h
 * @brief:   ADAU1979 Quad ADC driver definitions and API
 * @version: $Revision: 25389 $
 * @date:    $Date: 2016-02-26 12:32:28 -0500 (Fri, 26 Feb 2016) $
 *
 * @details
 *            This is the primary header file for ADAU1979 Audio codec driver.
 *            The file contains the API declarations, data structure and constant
 *            definitions used in the APIs.
 */

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_2_4:"Allows doxygen addtogroup block")
#endif

/** @addtogroup ADI_ADAU1979_Driver ADAU1979 Multichannel Audio codec driver
 *  @{
 */

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif

#ifndef __ADI_ADAU1979_H__
#define __ADI_ADAU1979_H__

/*=============  I N C L U D E S   =============*/

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rules_all:"Disable all rules")
#endif

/* Interrupt Manager includes */
#include <services/int/adi_int.h>
/* SPI driver includes */
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
#endif

/*==============  D E F I N E S  ===============*/

#if !defined(_LANGUAGE_ASM)

/* IF (ADSP-SC5xx Processor family) */
#if defined(__ADSPSC589_FAMILY__) || defined(__ADSPSC573_FAMILY__)
/*! Memory size required to handle an ADAU1979 Audio codec instance */
#define ADI_ADAU1979_MEMORY_SIZE                  (100u)
/* ELSE (Other Processor family) */
#else
#error "Processor not supported"
#endif /* __ADSPSC5XX_FAMILY__ */

/*! ADAU1979 Audio codec Handle. */
typedef void* ADI_ADAU1979_HANDLE;

/*! Enumeration of ADAU1979 driver specific result codes. */
typedef enum
{
    ADI_ADAU1979_SUCCESS = (0u),                /*!< 0x00 - Success */
    ADI_ADAU1979_FAILURE,                       /*!< 0x01 - Generic Failure */
    ADI_ADAU1979_IN_USE,                        /*!< 0x02 - Device Number already in use */
    ADI_ADAU1979_BAD_HANDLE,                    /*!< 0x03 - Supplied device handle is invalid */
    ADI_ADAU1979_NULL_POINTER,                  /*!< 0x04 - Given pointer parameters are valid  */
    ADI_ADAU1979_ALREADY_RUNNING,               /*!< 0x05 - Can't perform the requested operation when the dataflow is enabled */
    ADI_ADAU1979_INSUFFICIENT_MEMORY,           /*!< 0x06 - Supplied memory is insufficient to handle an ADAU1979 device instance */
    ADI_ADAU1979_GPIO_ERROR,                    /*!< 0x07 - Failed to configure GPIO service */
    ADI_ADAU1979_TWI_NOT_CONFIGURED,            /*!< 0x08 - A valid TWI device number  */
    ADI_ADAU1979_TWI_ERROR,                     /*!< 0x09 - TWI driver error */
    ADI_ADAU1979_SPORT_NOT_CONFIGURED,          /*!< 0x0A - A valid SPORT device number is not provided yet */
    ADI_ADAU1979_SPORT_ERROR,                   /*!< 0x0B - SPORT driver error */
    ADI_ADAU1979_SPORT_HW_ERROR,                /*!< 0x0C - SPORT hardware error */
    ADI_ADAU1979_SPORT_CFG_ERROR,               /*!< 0x0D - Failed to configure SPORT register. Dataflow for SPORT Tx and Rx channels should be disabled to perform this operation */
    ADI_ADAU1979_FN_NOT_SUPPORTED,              /*!< 0x0E - Function not supported */
    ADI_ADAU1979_FMT_NOT_SUPPORTED,             /*!< 0x0F - Selected ADAU1979 Serial mode format is not supported */
    ADI_ADAU1979_MCLK_INVALID,                  /*!< 0x10 - MCLK input value is invalid */
    ADI_ADAU1979_MCLK_SAMPLE_RATE_MISMATCH,     /*!< 0x11 - Selected sampling rate is not supported by the current MCLK input */
    ADI_ADAU1979_FMT_CHNL_NUM_MISMATCH,         /*!< 0x12 - Supplied number of channels not supported for the selected serial format */
    ADI_ADAU1979_FMT_SAMPLE_RATE_MISMATCH,      /*!< 0x13 - Supplied sampling rate is not supported by the selected serial format */
    ADI_ADAU1979_BUF_SUBMIT_FAILED,             /*!< 0x14 - Failed to submit an audio buffer */
    ADI_ADAU1979_BUF_Q_FULL,                    /*!< 0x15 - Buffer queue is full and can't accept anymore buffers */
    ADI_ADAU1979_SAMPLE_RATE_NOT_SUPPORTED,     /*!< 0x16 - Supplied sampling rate is not supported by this device */
    ADI_ADAU1979_AUDIO_CHANNEL_NOT_VALID,                /*!< 0x17 - Supplied ADAU1979 audio channel ID is invalid for the selected operation */
    ADI_ADAU1979_INVALID_FN_CALL,               /*!< 0x18 - Invalid function call for the current serial mode configuration */
    ADI_ADAU1979_AUDIO_CHANNEL_ID_DIRECTION_MISMATCH     /*!< 0x19 - Audio channel ids mapped to a SPORT device should have the same data direction */
} ADI_ADAU1979_RESULT;

/*! Enumeration for the ADAU1979 driver specific callback events */
typedef enum
{
    ADI_ADAU1979_EVENT_DMA_ERROR,               /*!< SPORT DMA Error (pArg = None) */
    ADI_ADAU1979_EVENT_BUFFER_PROCESSED         /*!< Processed an Audio buffer (pArg = Start address of the processed buffer) */
} ADI_ADAU1979_EVENT;

/*!
 * List of few industry standard Master Input Clock (MCLK) frequencies supported by ADAU1979
 */
typedef enum _ADI_ADAU1979_MCLK_FREQ_
{
    ADI_ADAU1979_MCLK_IN_FREQ_4096000HZ  = (0u),  /*!<  4.096  MHz MCLK */    
    ADI_ADAU1979_MCLK_IN_FREQ_5644800HZ  = (1u),  /*!<  5.6448 MHz MCLK */        
    ADI_ADAU1979_MCLK_IN_FREQ_6144000HZ  = (2u),  /*!<  6.144  MHz MCLK */    
    ADI_ADAU1979_MCLK_IN_FREQ_8192000HZ  = (3u),  /*!<  8.192  MHz MCLK */    
    ADI_ADAU1979_MCLK_IN_FREQ_11289600HZ = (4u),  /*!< 11.2896 MHz MCLK */    
    ADI_ADAU1979_MCLK_IN_FREQ_12288000HZ = (5u),  /*!< 12.288  MHz MCLK */        
    ADI_ADAU1979_MCLK_IN_FREQ_16384000HZ = (6u),  /*!< 16.384  MHz MCLK */            
    ADI_ADAU1979_MCLK_IN_FREQ_16934400HZ = (7u),  /*!< 16.9344 MHz MCLK */        
    ADI_ADAU1979_MCLK_IN_FREQ_18432000HZ = (8u),  /*!< 18.432  MHz MCLK */    
    ADI_ADAU1979_MCLK_IN_FREQ_22579200HZ = (9u),  /*!< 22.5792 MHz MCLK */    
    ADI_ADAU1979_MCLK_IN_FREQ_24576000HZ = (10u), /*!< 24.576  MHz MCLK */
    ADI_ADAU1979_MCLK_IN_FREQ_33868800HZ = (11u), /*!< 33.8688 MHz MCLK */
    ADI_ADAU1979_MCLK_IN_FREQ_36864000HZ = (12u), /*!< 36.864  MHz MCLK */
    ADI_ADAU1979_MCLK_IN_FREQ_NONE       = (13u)  /*!< MCLK end         */
} ADI_ADAU1979_MCLK_FREQ;

typedef enum _ADI_ADAU1979_PLL_SOURCE_
{
    ADI_ADAU1979_PLL_SOURCE_MCLK,   /*!<  PLL clock source is the MCLK  */
    ADI_ADAU1979_PLL_SOURCE_LRCLK   /*!<  PLL clock source is the LRCLK */
} ADI_ADAU1979_PLL_SOURCE;

typedef enum _ADI_ADAU1979_SAMPLE_RATE_
{
    ADI_ADAU1979_SAMPLE_RATE_32000HZ  = (0u), /*!< Sample rate 32   kHz */
    ADI_ADAU1979_SAMPLE_RATE_44100HZ  = (1u), /*!< Sample rate 44.1 kHz */    
    ADI_ADAU1979_SAMPLE_RATE_48000HZ  = (2u), /*!< Sample rate 48   kHz */        
    ADI_ADAU1979_SAMPLE_RATE_96000HZ  = (3u), /*!< Sample rate 96   kHz */
    ADI_ADAU1979_SAMPLE_RATE_192000HZ = (4u), /*!< Sample rate 192  kHz */    
    ADI_ADAU1979_SAMPLE_RATE_NONE     = (5u) /*!< Sample rate none     */        
} ADI_ADAU1979_SAMPLE_RATE;

typedef enum _ADI_ADAU1979_POWER_ENABLE_
{
    ADI_ADAU1979_POWER_LDO      = (0x20u),  /*!< Power enable for low drop out (LDO) */
    ADI_ADAU1979_POWER_VREF     = (0x10u), /*!< Power enable for voltage reference (VREF) */
    ADI_ADAU1979_POWER_CHNL4    = (0x08u), /*!< Power enable for ADC channel 4 */
    ADI_ADAU1979_POWER_CHNL3    = (0x04u), /*!< Power enable for ADC channel 3 */
    ADI_ADAU1979_POWER_CHNL2    = (0x02u), /*!< Power enable for ADC channel 2 */
    ADI_ADAU1979_POWER_CHNL1    = (0x01u), /*!< Power enable for ADC channel 1 */
    ADI_ADAU1979_POWER_MASTER   = (0xFFu)  /*!< Master power up control */
} ADI_ADAU1979_POWER_ENABLE;

/*!
 * TDM timeslot mapping
 */
typedef enum _ADI_ADAU1979_TDM_TIMESLOT_
{
    ADI_ADAU1979_TDM_TIMESLOT_01 = (0x0u), /*!< TDM timeslot 01 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_02 = (0x1u), /*!< TDM timeslot 02 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_03 = (0x2u), /*!< TDM timeslot 03 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_04 = (0x3u), /*!< TDM timeslot 04 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_05 = (0x4u), /*!< TDM timeslot 05 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_06 = (0x5u), /*!< TDM timeslot 06 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_07 = (0x6u), /*!< TDM timeslot 07 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_08 = (0x7u), /*!< TDM timeslot 08 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_09 = (0x8u), /*!< TDM timeslot 09 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_10 = (0x9u), /*!< TDM timeslot 10 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_11 = (0xAu), /*!< TDM timeslot 11 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_12 = (0xBu), /*!< TDM timeslot 12 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_13 = (0xCu), /*!< TDM timeslot 13 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_14 = (0xDu), /*!< TDM timeslot 14 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_15 = (0xEu), /*!< TDM timeslot 15 assignment */
    ADI_ADAU1979_TDM_TIMESLOT_16 = (0xFu)  /*!< TDM timeslot 16 assignment */
} ADI_ADAU1979_TDM_TIMESLOT;

/*! Serial Mode configuration supported the driver */
typedef enum
{
    ADI_ADAU1979_SERIAL_MODE_STEREO,           /*!< Configure the device and SPORT for Stereo, I2S transfer mode (default) */
    ADI_ADAU1979_SERIAL_MODE_TDM2,             /*!< Configure the device and SPORT for TDM2 mode */                                                                                                      
    ADI_ADAU1979_SERIAL_MODE_TDM4,             /*!< Configure the device and SPORT for TDM4 mode */                                                                                                       
    ADI_ADAU1979_SERIAL_MODE_TDM8,             /*!< Configure the device and SPORT for TDM8 mode */                                                                                                       
    ADI_ADAU1979_SERIAL_MODE_TDM16             /*!< Configure the device and SPORT for TDM16 mode */  
 } ADI_ADAU1979_SERIAL_MODE;

/*! Audio data word width configuration supported the driver */
typedef enum
{
    ADI_ADAU1979_WORD_WIDTH_24 = (0x00u),   /*!< 24-bit audio (default) */
    ADI_ADAU1979_WORD_WIDTH_16 = (0x10u),   /*!< 16-bit audio  */
    ADI_ADAU1979_WORD_WIDTH_NONE =(0xFFu)   /*!< None  */
} ADI_ADAU1979_WORD_WIDTH;

/*! Enumeration of ADAU1979 Audio channels */
typedef enum
{                                        
    ADI_ADAU1979_AUDIO_CHANNEL1,             /*!< Audio channel 1        */
    ADI_ADAU1979_AUDIO_CHANNEL2,             /*!< Audio channel 2        */
    ADI_ADAU1979_AUDIO_CHANNEL3,             /*!< Audio channel 3        */
    ADI_ADAU1979_AUDIO_CHANNEL4,             /*!< Audio channel 4        */
    ADI_ADAU1979_AUDIO_CHANNEL_NONE,         /*!< No audio channels      */
    ADI_ADAU1979_AUDIO_CHANNEL_ALL           /*!< All audio channels     */
} ADI_ADAU1979_AUDIO_CHANNEL;

/*! Enumeration of SPORT channels that can be mapped to ADAU1962A serial data input port */
typedef enum _ADI_ADAU1979_SPORT_CHANNEL_
{
    ADI_ADAU1979_SPORT_A = (ADI_HALF_SPORT_A),    	/*!< Sport A - First half sport */
    ADI_ADAU1979_SPORT_B = (ADI_HALF_SPORT_B)    	/*!< Sport B - Second half sport */
} ADI_ADAU1979_SPORT_CHANNEL;

/*! Enumeration of ADAU1962A Serial data input ports */
typedef enum _ADI_ADAU1979_SERIAL_PORT_
{
    ADI_ADAU1979_SERIAL_PORT_DSDATA1 = (0u),   /*!< ADC  1 and ADC  2 Serial Data Input port */
    ADI_ADAU1979_SERIAL_PORT_DSDATA2,          /*!< ADC  3 and ADC  4 Serial Data Input port */
    ADI_ADAU1979_SERIAL_PORT_NONE              /*!< No Serial Data Input port selected */
} ADI_ADAU1979_SERIAL_PORT;

/*! Enumeration of ADAU1979 TWI address */
typedef enum
{                                        
    ADI_ADAU1979_TWI_ADDR_11 = (0x11u),     /*!< TWI address 0x11 */
    ADI_ADAU1979_TWI_ADDR_31 = (0x31u),     /*!< TWI address 0x31 */
    ADI_ADAU1979_TWI_ADDR_51 = (0x41u),     /*!< TWI address 0x51 */
    ADI_ADAU1979_TWI_ADDR_71 = (0x51u)      /*!< TWI address 0x71 */        
} ADI_ADAU1979_TWI_ADDR;

/*! ADAU1979 Device Registers */
typedef enum
{
    ADI_ADAU1979_REG_M_PWR          = (0x00000000u),  /*!< Master power and soft reset */
    ADI_ADAU1979_REG_PLL_CTL        = (0x01u),  /*!< PLL control */
    ADI_ADAU1979_REG_BLK_PWR_SAI    = (0x04u),  /*!< Power block control and serial port control */
    ADI_ADAU1979_REG_SAI_CTL1       = (0x05u),  /*!< Serial port control 1 */
    ADI_ADAU1979_REG_SAI_CTL2       = (0x06u),  /*!< Serial port control 2 */
    ADI_ADAU1979_REG_SAI_CMAP12     = (0x07u),  /*!< Channel 1&2 serial port mapping */
    ADI_ADAU1979_REG_SAI_CMAP34     = (0x08u),  /*!< Channel 3&4 serial port mapping */
    ADI_ADAU1979_REG_SAI_OVRTEMP    = (0x09u),  /*!< Serial output drive control and over temperature protection status */
    ADI_ADAU1979_REG_POSTADC_GAIN1  = (0x0Au),  /*!< Gain control for channel 1 */     
    ADI_ADAU1979_REG_POSTADC_GAIN2  = (0x0Bu),  /*!< Gain control for channel 2 */     
    ADI_ADAU1979_REG_POSTADC_GAIN3  = (0x0Cu),  /*!< Gain control for channel 3 */     
    ADI_ADAU1979_REG_POSTADC_GAIN4  = (0x0Du),  /*!< Gain control for channel 4 */         
    ADI_ADAU1979_REG_MISC_CTL       = (0x0Eu),  /*!< High pass filter, DC offset, master mute */   
    ADI_ADAU1979_REG_ASC_CLIP       = (0x19u),  /*!< ADC clipping status */   
    ADI_ADAU1979_REG_DC_HPF_CAL     = (0x1Au)   /*!< DC high pass filter and calibration */                              
} ADI_ADAU1979_REG;

/*! Master clock output (MCLKO/XO pin) settings for ADAU1979 device */
typedef enum
{
    ADI_ADAU1979_MCLK_OUT_XTAL_OSC    = (0x00u),  /*!< XTAL oscillator enabled */
    ADI_ADAU1979_MCLK_OUT_256FS_VCO   = (0x08u),  /*!< 256 x Fs VCO output */
    ADI_ADAU1979_MCLK_OUT_512FS_VCO   = (0x10u),  /*!< 512 x Fs VCO output */
    ADI_ADAU1979_MCLK_OUT_OFF         = (0x18u)   /*!< Master clock output off (default) */
} ADI_ADAU1979_MCLK_OUT;


/*! Structure to hold TWI device information to access ADAU1979 registers */
typedef struct
{
    uint16_t                TwiDevNum;          /*!< TWI Device number allocated to this device */
    void                    *pTwiDevMem;        /*!< Pointer to TWI buffer/memory to handle the TWI device */
    uint16_t                TwiDevMemSize;      /*!< Memory size of the buffer "pTwiDevMem" points to */
    ADI_ADAU1979_TWI_ADDR   eTwiAddr;           /*!< TWI address  */
} ADI_ADAU1979_TWI_CONFIG;


/**
 * Structure to hold the information regarding the SPORT device configuration.
 */
typedef struct _ADI_ADAU1979_SPORT_CONFIG_
{
    uint8_t                    SportDevNum;        /*!< SPORT Device number to be used for stereo mode */
    ADI_ADAU1979_SPORT_CHANNEL  eSportChnl;         /*!< SPORT channel to be mapped */
    ADI_ADAU1979_SERIAL_PORT    eSportPri;          /*!< ADAU1962A Serial Port mapped to SPORT half primary channel */
    ADI_ADAU1979_SERIAL_PORT    eSportSec;          /*!< ADAU1962A Serial Port mapped to SPORT half secondary channel */
    void                        *pSportDevMem;      /*!< Pointer to #ADI_ADAU1979_SPORT_DMA_MEMORY_SIZE sized buffer/memory to handle the SPORT device */
    uint32_t                    SportDevMemSize;    /*!< Memory size (#ADI_ADAU1979_SPORT_DMA_MEMORY_SIZE) of the buffer "pSportDevMem" points to */
    bool                        bLsbFirst;          /*!< SPORT endianness, LSB first */
} ADI_ADAU1979_SPORT_CONFIG;

/*=======  P U B L I C   P R O T O T Y P E S  ========*/
/*            (globally-scoped functions)             */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * APIs to open/close device instance and hook it to Blackfin peripherals
 */

/* Opens an ADAU1979 instance for use */
ADI_ADAU1979_RESULT adi_adau1979_Open(
    uint32_t                            DeviceNum,
    ADI_ADAU1979_SERIAL_MODE            eSerialMode,
    void                         *const pDeviceMemory,
    uint32_t                            MemorySize,
    ADI_ADAU1979_HANDLE          *const phDevice);

/* Closes an ADAU1979 instance in use */
ADI_ADAU1979_RESULT adi_adau1979_Close(
    ADI_ADAU1979_HANDLE           const hDevice);

/* Configure the TWI device */
ADI_ADAU1979_RESULT adi_adau1979_ConfigTwi(
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_TWI_CONFIG             *pConfig);

/* Configure the SPORT device */
ADI_ADAU1979_RESULT   adi_adau1979_ConfigSport (
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_SPORT_CONFIG           *pConfig);

/* Enable/Disable data flow audio from ADAU1979 */
ADI_ADAU1979_RESULT  adi_adau1979_Enable(
    ADI_ADAU1979_HANDLE           const hDevice,
    bool                                bEnable);

/* Submits a buffer to ADAU1979 instance to fill with (ADC) audio data */
ADI_ADAU1979_RESULT  adi_adau1979_SubmitBuffer(
    ADI_ADAU1979_HANDLE           const hDevice,
    void                                *pBuffer,
    uint32_t                            BufferSize);

/* Peek function for non-OS application to check if a
   processed (ADC) audio buffer is available or not */
ADI_ADAU1979_RESULT  adi_adau1979_IsBufAvailable(
    ADI_ADAU1979_HANDLE           const hDevice,
    bool                                *pbIsBufAvailable);

/* Gets address of a processed (ADC) audio buffer */
ADI_ADAU1979_RESULT  adi_adau1979_GetBuffer(
    ADI_ADAU1979_HANDLE           const hDevice,
    void                                **ppBuffer);

/* Register a callback for reporting ADAU1979 audio receive (ADC) events */
ADI_ADAU1979_RESULT  adi_adau1979_RegisterCallback(
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_CALLBACK                        pfCallback,
    void                                *pCBParam);
    
/* Assigns ADC channel to a TDM timeslot */
ADI_ADAU1979_RESULT   adi_adau1979_SetTdmTimeslotChannelMap (
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL          eChannel,
    ADI_ADAU1979_TDM_TIMESLOT           eTimeslot);    

/* Configure ADAU1979 PLL clock settings */
ADI_ADAU1979_RESULT adi_adau1979_ConfigPllClk(
    ADI_ADAU1979_HANDLE           const hDevice,
    bool                                bClkMaster,
    ADI_ADAU1979_MCLK_FREQ              eMClkFreq,
    ADI_ADAU1979_PLL_SOURCE             ePllSrc);
    
/* Configure ADAU1979 (ADC) Serial Port Clock settings */    
ADI_ADAU1979_RESULT adi_adau1979_ConfigSerialClk(
    ADI_ADAU1979_HANDLE           const hDevice,
    bool                                bBClkRisingEdgeDriveData,
    bool                                bLRClkHighLow);


/* Sets ADAU1979 (ADC) channel sampling rate */
ADI_ADAU1979_RESULT adi_adau1979_SetSampleRate(
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_SAMPLE_RATE            eSampleRate);

/* Power-up/Power-down ADAU1979 (ADC) module */
ADI_ADAU1979_RESULT  adi_adau1979_PowerUp(    
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_POWER_ENABLE           ePowerEnable,
    bool                                bPowerUp);

/* Sets ADAU1979 (ADC) channel data word width */
ADI_ADAU1979_RESULT adi_adau1979_SetWordWidth(
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_WORD_WIDTH             eWordWidth);



/* Sets ADAU1979 channel volume */
ADI_ADAU1979_RESULT adi_adau1979_SetVolume(
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL          eChannel,
    uint8_t                            Volume);

/* Enables/disables ADAU1979 channel mute */
ADI_ADAU1979_RESULT adi_adau1979_MuteChannel (
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL          eChannel,
    bool                                bMute);

/* Enables/disables ADAU1979 channel clipping */
ADI_ADAU1979_RESULT adi_adau1979_ClipChannel (
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL          eChannel,
    bool                                bClip);

/* Enables/disables ADAU1979 channel calibration */
ADI_ADAU1979_RESULT adi_adau1979_CalibrateChannel (
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL          eChannel,
    bool                                bCalibrate);
    
/* Enables/disables ADAU1979 channel high pass filter */
ADI_ADAU1979_RESULT adi_adau1979_HighPassChannel (
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_AUDIO_CHANNEL          eChannel,
    bool                                bHighPass);


/* Read ADAU1979 Audio Codec register */
ADI_ADAU1979_RESULT adi_adau1979_ReadRegister (
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_REG                    RegAddr,
    uint8_t                             *pRegValue);

/* Write to ADAU1979 Audio Codec register */
ADI_ADAU1979_RESULT adi_adau1979_WriteRegister (
    ADI_ADAU1979_HANDLE           const hDevice,
    ADI_ADAU1979_REG                    RegAddr,
    uint8_t                             RegValue);

#ifdef __cplusplus
}
#endif

#endif

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */

#endif  /* __ADI_ADAU1979_H__ */

/*****/

/*@}*/

