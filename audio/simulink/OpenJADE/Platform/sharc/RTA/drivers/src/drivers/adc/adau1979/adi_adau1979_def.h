/*********************************************************************************

Copyright(c) 2013-2015 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

/*!
 * @file:    adi_adau1979_def.h
 * @brief:   Private header file for ADAU1979 driver implementation.
 * @version: $Revision: 25249 $
 * @date:    $Date: 2016-02-12 12:09:59 -0500 (Fri, 12 Feb 2016) $
 *
 * @details
 *           This is a private header file for ADAU1979 Audio codec driver
 *           that contains definitions used in ADAU1979 driver implementation.
 */

#ifndef __ADI_ADAU1979_DEF_H__
#define __ADI_ADAU1979_DEF_H__

/*=============  I N C L U D E S   =============*/

/* ADI ADAU1979 Audio codec driver includes */
#include <drivers/adc/adau1979/adi_adau1979.h>
/* ADI ADAU1979 Audio codec register field definition includes */
#include "adi_adau1979_regs.h"
/* Memory size check */
#include <assert.h>
/* NULL definition includes */
#include <stddef.h>

/* memset include */
#include <string.h>

/* standard types includes */
#include <stdint.h>
#include <stdbool.h>

#include <adi_osal.h>

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_5_1:"Allow identifiers to be more than 31 characters")
#pragma diag(suppress:misra_rule_5_3:"Structure name had to defined earlier for function prototype")
#pragma diag(suppress:misra_rule_5_4:"Structure name had to defined earlier for function prototype")
#pragma diag(suppress:misra_rule_8_5:"There shall be no definitions of objects or functions in a header file.")

#endif

/*==============  D E F I N E S  ===============*/



/***** ADAU1979 Register default configurations (I2S) *****/

#define ADI_ADAU1979_REG_DFLT_M_PWR             (0x00u)
                                                
#define ADI_ADAU1979_REG_DFLT_PLL_CTL           (ENUM_ADAU1979_01_PLL_MUTE_LOCKED|\
                                                 ENUM_ADAU1979_01_MCS_256)
                                                 
#define ADI_ADAU1979_REG_DFLT_BLK_PWR_SAI       (ENUM_ADAU1979_04_LDO_EN|\
                                                 ENUM_ADAU1979_04_VREF_EN|\
                                                 ENUM_ADAU1979_04_ADC4_EN|\
                                                 ENUM_ADAU1979_04_ADC3_EN|\
                                                 ENUM_ADAU1979_04_ADC2_EN|\
                                                 ENUM_ADAU1979_04_ADC1_EN)
                                                
#define ADI_ADAU1979_REG_DFLT_SAI_CTL1          (ENUM_ADAU1979_05_SR_32_48KHZ)
                                                
#define ADI_ADAU1979_REG_DFLT_SAI_CTL2          (0x00u)
                                                
#define ADI_ADAU1979_REG_DFLT_SAI_CMAP12        (0x10u)
                                                
#define ADI_ADAU1979_REG_DFLT_SAI_CMAP34        (0x32u)
                                                
#define ADI_ADAU1979_REG_DFLT_SAI_OVRTEMP       (ENUM_ADAU1979_09_CH4_OP_EN|\
                                                 ENUM_ADAU1979_09_CH3_OP_EN|\
                                                 ENUM_ADAU1979_09_CH2_OP_EN|\
                                                 ENUM_ADAU1979_09_CH1_OP_EN)
                                                 
#define ADI_ADAU1979_REG_DFLT_GAIN1             (0xA0u)
#define ADI_ADAU1979_REG_DFLT_GAIN2             (0xA0u)
#define ADI_ADAU1979_REG_DFLT_GAIN3             (0xA0u)
#define ADI_ADAU1979_REG_DFLT_GAIN4             (0xA0u)
                                                
#define ADI_ADAU1979_REG_DFLT_MISC_CTL          (0x02u)
                                                
#define ADI_ADAU1979_REG_DFLT_ASC_CLIP          (0x00u)
                                                
#define ADI_ADAU1979_REG_DFLT_DC_HPF_CAL        (0x00u)

/* ADAU1979 SPI data packet size in bytes */
#define ADI_ADAU1979_SPI_PKT_SIZE               (3u)
/* ADAU1979 Global address for register read */
#define ADI_ADAU1979_SPI_GADDR_READ             (0x09u)
/* ADAU1979 Global address for register write */
#define ADI_ADAU1979_SPI_GADDR_WRITE            (0x08u)

/* SPORT configuration for Stereo mode */
/* Control Register 1 - External Clock, External FS, Requires TFS for every data word */
#define ADI_ADAU1979_STEREO_SPORT_CTRL          (0x0400u)

/* SPORT Multi-channel control register configuration for 8 channels */
#define ADI_ADAU1979_TDM_SPORT_MC_CTRL_8        (NCH7 | MFD1)
/* SPORT Multi-channel control register configuration for 4 channels */
#define ADI_ADAU1979_TDM_SPORT_MC_CTRL_4        (NCH3 | MFD1)
/* SPORT Multi-channel control register configuration for 2 channels */
#define ADI_ADAU1979_TDM_SPORT_MC_CTRL_2        (NCH1 | MFD1)

/* SPORT Multi-channel select register configuration for 8 channels */
#define ADI_ADAU1979_TDM_SPORT_MC_SEL_8         (0x000000FF)
/* SPORT Multi-channel select register configuration for 4 channels */
#define ADI_ADAU1979_TDM_SPORT_MC_SEL_4         (0x0000000F)
/* SPORT Multi-channel select register configuration for 2 channels */
#define ADI_ADAU1979_TDM_SPORT_MC_SEL_2         (0x00000003)

/* Number of DAC volume control registers in ADAU1979 device */
#define ADI_AD9138_NUM_DAC_PAIR_VOLUME_CTRL     (2u)

/* Number of DAC volume control registers in ADAU1979 device */
#define ADI_AD9138_NUM_DAC_ALL_VOLUME_CTRL      (8u)

/* Number of DAC channel pairs in an ADAU1979 device */
#define ADI_AD9138_NUM_DAC_PAIRS                (4u)

/* Number of ADC channel pairs in an ADAU1979 device */
#define ADI_AD9138_NUM_ADC_PAIRS                (2u)

/* Macros used to configure ADAU1979 Sampling Rate */

/* Minimum MCLK limit is 6.9MHz */
#define ADI_ADAU1979_MCLK_MIN                   (ADI_ADAU1979_MCLK_IN_FREQ_4096000HZ)
/* Maximum MCLK limit is 27.6MHz */
#define ADI_ADAU1979_MCLK_MAX                   (ADI_ADAU1979_MCLK_IN_FREQ_36864000HZ)


/* Master clock select */
#define MCS_NONE (0xFFu)

#define ADAU1979_TWI_PRESCALE   (1u)
#define ADAU1979_TWI_DUTYCYCLE  (50u)
#define ADAU1979_TWI_BITRATE    (300u)

/* Structure to hold ADAU1979 register address and configuration value */
typedef struct
{
    uint8_t                 RegAddr;
    uint8_t                 RegData;
} ADAU1979_REG_TABLE;

#pragma pack (4)
/* Structure to handle TWI device used to access ADAU1979 registers */
typedef struct
{
    ADI_ADAU1979_TWI_CONFIG     Info;                   /* TWI device specific information */
    ADI_TWI_HANDLE              hTwiDev;                /* Handle to TWI device */
} ADI_ADAU1979_TWI;
#pragma pack ()

#pragma pack (4)
/* Structure to handle SPORT device */
typedef struct
{
    ADI_ADAU1979_SPORT_CONFIG   Info;                   /* SPORT device specific information */
    ADI_SPORT_HANDLE            hSportDev;              /* Handle to SPORT device */
    ADI_CALLBACK                pfCallback;             /* Address of application callback function */
    void                        *pCBParam;              /* Parameter passed back to application callback */
} ADI_ADAU1979_SPORT;
#pragma pack ()

/* Structure to handle ADC channels */
typedef struct
{
    bool                        bIsEnabled;             /* 'true' if  dataflow is enabled */
    ADI_ADAU1979_MCLK_FREQ      eMClkFreq;              /* MClk frequency */
    ADI_ADAU1979_SAMPLE_RATE    eSampleRate;            /* Sampling rate */
    ADI_ADAU1979_WORD_WIDTH     eWordLength;            /* Word length */
    ADI_ADAU1979_SERIAL_MODE    eSerialMode;            /* Serial mode */
    bool                        bBClkRisingEdgeLatch;   /* Data is latched on rising edge of BClk */
    bool                        bLRClkPulse;            /* LR clock is pulse mode as opposed to 50% duty cycle mode */
    bool                        bLRClkHighLow;          /* LR clock goes high then low */
    uint8_t                     Chnl1Volume;            /* Channel 1 volume level */       
    uint8_t                     Chnl2Volume;            /* Channel 2 volume level */
    uint8_t                     Chnl3Volume;            /* Channel 3 volume level */
    uint8_t                     Chnl4Volume;            /* Channel 4 volume level */
} ADI_ADAU1979_ADC;

#pragma pack (4)
/* Structure to handle ADAU1979 device instance */
typedef struct ADI_ADAU1979_DEV
{
    uint32_t                    DevNum;                 /* Device instance number */
    ADI_ADAU1979_TWI            Twi;                    /* TWI instance to access ADAU1979 registers */
    ADI_ADAU1979_SPORT          Sport;                  /* SPORT instance */    
    ADI_ADAU1979_ADC            Adc;                    /* Instance to handle ADC channels */
    struct ADI_ADAU1979_DEV     *pPrevious;             /* Pointer to previous device instance in chain */
    struct ADI_ADAU1979_DEV     *pNext;                 /* Pointer to next device instance in chain */
} ADI_ADAU1979_DEV;
#pragma pack ()

/* ADAU1979 register configuration table */
static ADAU1979_REG_TABLE  RegConfig[] =
{
    { (uint8_t)ADI_ADAU1979_REG_M_PWR        , ADI_ADAU1979_REG_DFLT_M_PWR      },
    { (uint8_t)ADI_ADAU1979_REG_PLL_CTL      , ADI_ADAU1979_REG_DFLT_PLL_CTL    },
    { (uint8_t)ADI_ADAU1979_REG_BLK_PWR_SAI  , ADI_ADAU1979_REG_DFLT_BLK_PWR_SAI},
    { (uint8_t)ADI_ADAU1979_REG_SAI_CTL1     , ADI_ADAU1979_REG_DFLT_SAI_CTL1   },
    { (uint8_t)ADI_ADAU1979_REG_SAI_CTL2     , ADI_ADAU1979_REG_DFLT_SAI_CTL2   },
    { (uint8_t)ADI_ADAU1979_REG_SAI_CMAP12   , ADI_ADAU1979_REG_DFLT_SAI_CMAP12 },
    { (uint8_t)ADI_ADAU1979_REG_SAI_CMAP34   , ADI_ADAU1979_REG_DFLT_SAI_CMAP34 },
    { (uint8_t)ADI_ADAU1979_REG_SAI_OVRTEMP  , ADI_ADAU1979_REG_DFLT_SAI_OVRTEMP},
    { (uint8_t)ADI_ADAU1979_REG_POSTADC_GAIN1, ADI_ADAU1979_REG_DFLT_GAIN1      },
    { (uint8_t)ADI_ADAU1979_REG_POSTADC_GAIN2, ADI_ADAU1979_REG_DFLT_GAIN2      },
    { (uint8_t)ADI_ADAU1979_REG_POSTADC_GAIN3, ADI_ADAU1979_REG_DFLT_GAIN3      },
    { (uint8_t)ADI_ADAU1979_REG_POSTADC_GAIN4, ADI_ADAU1979_REG_DFLT_GAIN4      },
    { (uint8_t)ADI_ADAU1979_REG_MISC_CTL     , ADI_ADAU1979_REG_DFLT_MISC_CTL   },
    { (uint8_t)ADI_ADAU1979_REG_ASC_CLIP     , ADI_ADAU1979_REG_DFLT_ASC_CLIP   },
    { (uint8_t)ADI_ADAU1979_REG_DC_HPF_CAL   , ADI_ADAU1979_REG_DFLT_DC_HPF_CAL }
};


/* Master clock select setting for PLL control register, ADI_ADAU1979_REG_PLL_CTL,  MCS[2:0] */
static uint8_t aMasterClockSelectSetting[ADI_ADAU1979_MCLK_IN_FREQ_NONE][ADI_ADAU1979_SAMPLE_RATE_NONE] =
{
    /* 32kHz               44.1kHz,          48kHz,             96kHz,             192kHz             */
    {  (uint8_t)0x00u,    (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE},    /*!<  4.096  MHz MCLK */
    {  (uint8_t)MCS_NONE, (uint8_t)0x00u,    (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE},    /*!<  5.6448 MHz MCLK */
    {  (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)0x00u,    (uint8_t)0x00u,    (uint8_t)0x00u   },    /*!<  6.144  MHz MCLK */
    {  (uint8_t)0x01,     (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE},    /*!<  8.192  MHz MCLK */
    {  (uint8_t)MCS_NONE, (uint8_t)0x01u,    (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE},    /*!< 11.2896 MHz MCLK */
    {  (uint8_t)0x02u,    (uint8_t)MCS_NONE, (uint8_t)0x01u,    (uint8_t)0x01u,    (uint8_t)0x01u   },    /*!< 12.288  MHz MCLK */
    {  (uint8_t)0x03u,    (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE},    /*!< 16.384  MHz MCLK */
    {  (uint8_t)MCS_NONE, (uint8_t)0x02u,    (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE},    /*!< 16.9344 MHz MCLK */
    {  (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)0x02u,    (uint8_t)0x02u,    (uint8_t)0x02u   },    /*!< 18.432  MHz MCLK */
    {  (uint8_t)MCS_NONE, (uint8_t)0x03u,    (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE},    /*!< 22.5792 MHz MCLK */
    {  (uint8_t)0x04u,    (uint8_t)MCS_NONE, (uint8_t)0x03u,    (uint8_t)0x03u,    (uint8_t)0x03u   },    /*!< 24.576  MHz MCLK */
    {  (uint8_t)MCS_NONE, (uint8_t)0x04u,    (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)MCS_NONE},    /*!< 33.8688 MHz MCLK */
    {  (uint8_t)MCS_NONE, (uint8_t)MCS_NONE, (uint8_t)0x04u,    (uint8_t)0x04u,    (uint8_t)0x04u   }     /*!< 36.864  MHz MCLK */
};

/*=============  L O C A L    F U N C T I O N    P R O T O T Y P E S =============*/

/*=============  C A L L B A C K    F U N C T I O N    P R O T O T Y P E S  =============*/

/*=============  D E B U G    F U N C T I O N    P R O T O T Y P E S  =============*/
/*
 * Debug Functions
 */
/* IF (Debug information enabled) */
#if defined (ADI_DEBUG)

/* End of debug functions */
#endif /* ADI_DEBUG */

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */

#endif  /* __ADI_ADAU1979_DEF_H__ */

/*****/
