/*********************************************************************************

Copyright(c) 2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

/*!
 * @file:    adi_adau1962a_def.h
 * @brief:   Private header file for ADAU1962A driver implementation.
 * @version: $Revision: 25249 $
 * @date:    $Date: 2016-02-12 12:09:59 -0500 (Fri, 12 Feb 2016) $
 *
 * @details
 *           This is a private header file for ADAU1962A DAC driver
 *           that contains definitions used in ADAU1962A driver implementation.
 */

#ifndef __ADI_ADAU1962A_DEF_H__
#define __ADI_ADAU1962A_DEF_H__

/*=============  I N C L U D E S   =============*/

/* ADI ADAU1962A DAC driver includes */
#include <drivers/dac/adau1962a/adi_adau1962a.h>
/* ADI ADAU1962A DAC register field definition includes */
#include "adi_adau1962a_regs.h"
/* Memory size check */
#include <assert.h>
/* NULL definition includes */
#include <string.h>
#include <adi_osal.h>

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_5_1:"Allow identifiers to be more than 31 characters")
#pragma diag(suppress:misra_rule_5_3:"Structure name had to defined earlier for function prototype")
#pragma diag(suppress:misra_rule_5_4:"Structure name had to defined earlier for function prototype")
#pragma diag(suppress:misra_rule_18_4:"Allow using unions")
#endif

/*==============  D E F I N E S  ===============*/

/*
 * TWI settings
 */

/* TWI Prescale */
#define ADI_ADAU1962A_TWI_PRESCALE                  (8u)
/*/ 400kHz TWI Clock */
#define ADI_ADAU1962A_TWI_BITRATE                   (400u)
/* 50% TWI Duty cycle */
#define ADI_ADAU1962A_TWI_DUTYCYCLE                 (50u)
/* Value to identify invalid TWI device number */
#define ADI_ADAU1962A_TWI_NUM_INVALID               (0xFFu)
/* TWI Register field write */
#define ADI_ADAU1962A_TWI_FIELD_WRITE               (0u)
/* TWI Register field read */
#define ADI_ADAU1962A_TWI_FIELD_READ                (1u)

/*
 * MCLK Input range
 */
/* Minimum MCLK Input - 6.9MHz */
#define ADI_ADAU1962A_MIN_MCLK_IN                   (6900000u)
/* Maximum MCLK Input - 40.5MHz */
#define ADI_ADAU1962A_MAX_MCLK_IN                   (40500000u)

/* Number of serial data ports in ADAU1962A */
#define NUM_SERIAL_PORTS                            (6u)

/* Macros used to configure ADAU1962A Sampling Rate */
/* 48kHz Sample Rate */
#define ADI_ADAU1962A_SR_48000HZ                    (48000u)
/* 96kHz Sample Rate */
#define ADI_ADAU1962A_SR_96000HZ                    (96000u)
/* MCLK Freq of 12.28MHz */
#define ADI_ADAU1962A_MCLK_12288000HZ               (12288000u)
/* Sampling rate (fs) multiplier max limit */
#define ADI_ADAU1962A_FS_MULTIPLIER_MAX             (768u)
/* Sampling rate (fs) multiplier base value for MCLK less than 12.288MHz */
#define ADI_ADAU1962A_FS_MULTIPLIER_BASE_1          (256u)
/* Sampling rate (fs) multiplier base value for MCLK more than 12.288MHz */
#define ADI_ADAU1962A_FS_MULTIPLIER_BASE_2          (512u)
/* Sampling rate (fs) multiplier values supported (256 x) */
#define ADI_ADAU1962A_FS_MULTIPLIER_256X            (256u)
/* Sampling rate (fs) multiplier values supported (384 x) */
#define ADI_ADAU1962A_FS_MULTIPLIER_384X            (384u)
/* Sampling rate (fs) multiplier values supported (512 x) */
#define ADI_ADAU1962A_FS_MULTIPLIER_512X            (512u)
/* Sampling rate (fs) multiplier values supported (768 x) */
#define ADI_ADAU1962A_FS_MULTIPLIER_768X            (768u)
/* Maximum value for sampling rate field */
#define ADI_ADAU1962A_SR_FIELD_MAX                  (3u)

#pragma pack (4)
/* Structure to handle TWI device used to access ADAU1962A registers */
typedef struct
{
    ADI_ADAU1962A_TWI_CONFIG    Info;           /* TWI device specific information */
    ADI_TWI_HANDLE              hDev;           /* Handle to TWI device */
} ADI_ADAU1962A_TWI;
#pragma pack ()

#pragma pack (4)
/* Structure to handle ADAU1962A device instance */
typedef struct ADI_ADAU1962A_DEV
{
    bool                            bIsEnabled;     /* 'true' if  dataflow is enabled */
    bool                            bLsbFirst;      /* 'true' if LSB is first on DSDATA, 'false' for MSB first */
    bool                            bEnableSec;     /* 'true' to enable secondary SPORT channel */
    bool                            bLeftChnlClkHi; /* 'true' for Left/Odd channels when LRCLK is High (inverted) */
    bool                            bBClkFallLatch; /* 'true' to latch data in DBCLK falling edge */
    uint8_t                         DevNum;         /* Device instance number */
    uint8_t                         WordWidth;      /* Word width in bits */
    uint32_t                        SampleRate;     /* Sample Rate */
    uint32_t     			        MClkInFreq;     /* Master Clock input frequency */
    ADI_SPORT_HANDLE                hSportDev;      /* SPORT Device handle allocated to the DAC Serial data port */
    ADI_ADAU1962A_SERIAL_MODE       eSerialMode;    /* Serial data mode */
    ADI_ADAU1962A_TWI               Twi;            /* TWI instance to access ADAU1962A registers */
    ADI_CALLBACK                    pfCallback;     /* Address of application callback function */
    void                            *pCBParam;      /* Parameter passed back to application callback */
    struct ADI_ADAU1962A_DEV        *pPrevious;     /* Pointer to previous device instance in chain */
    struct ADI_ADAU1962A_DEV        *pNext;         /* Pointer to next device instance in chain */
} ADI_ADAU1962A_DEV;
#pragma pack ()

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

#endif  /* __ADI_ADAU1962A_DEF_H__ */

/*****/

