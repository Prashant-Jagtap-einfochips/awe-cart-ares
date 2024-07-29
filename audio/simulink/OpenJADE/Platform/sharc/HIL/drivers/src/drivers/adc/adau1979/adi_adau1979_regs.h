/*********************************************************************************

Copyright(c) 2013-2015 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

/*!
 * @file:    adi_adau1979_regs.h
 * @brief:   Include file for ADAU1979 driver with register definitions.
 * @version: $Revision: 25249 $
 * @date:    $Date: 2016-02-12 12:09:59 -0500 (Fri, 12 Feb 2016) $
 *
 * @details
 *           This is a private header file for ADAU1979 ADC driver that
 *           contains register field definitions used in ADAU1979 driver implementation.
 */

#ifndef __ADI_ADAU1979_REGS_H__
#define __ADI_ADAU1979_REGS_H__

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_5_1:"Allow identifiers to be more than 31 characters")
#endif

/*==============  D E F I N E S  ===============*/

/*
 * ADAU1979 Register Field definitions
 */

/* Master power and soft reset Register 0x00 field definitions (ADI_ADAU1979_REG_MPWR_RST) */
#define BITM_ADAU1979_00_SW_RESET                 (0x80u)     /* Software reset */
#define ENUM_ADAU1979_00_SW_RESET_EN              (0x80u)     /* Enable: Software reset */

#define BITM_ADAU1979_00_MST_PWR                  (0x01u)     /* Master power up */
#define ENUM_ADAU1979_00_MST_PWR_EN               (0x01u)     /* Enable: Master power up */

/* PLL control Register 0x01 field definitions (ADI_ADAU1979_REG_PLL_CTL) */
#define BITM_ADAU1979_01_PLL_STAT                 (0x80u)     /* PLL lock status */
#define ENUM_ADAU1979_01_PLL_STAT_LOCKED          (0x80u)     /* PLL is locked */

#define BITM_ADAU1979_01_PLL_MUTE                 (0x40u)     /* PLL unlock automatic mute */
#define ENUM_ADAU1979_01_PLL_MUTE_LOCKED          (0x40u)     /* PLL is automatic mute with PLL unlocked */

#define BITM_ADAU1979_01_PLL_CLK_SRC              (0x10u)     /* PLL clock source */
#define ENUM_ADAU1979_01_PLL_CLK_MCLK             (0x00u)     /* Clock source is MCLK */
#define ENUM_ADAU1979_01_PLL_CLK_LRCLK            (0x10u)     /* Clock source is LRCLK */

#define BITM_ADAU1979_01_MCS                     (0x07u)     /* Master clock select */
#define ENUM_ADAU1979_01_MCS_768                 (0x04u)     /* INPUT 768 (x fs for 32kHz to 48 kHz) */
#define ENUM_ADAU1979_01_MCS_512                 (0x03u)     /* INPUT 512 (x fs for 32kHz to 48 kHz) */
#define ENUM_ADAU1979_01_MCS_384                 (0x02u)     /* INPUT 384 (x fs for 32kHz to 48 kHz) */
#define ENUM_ADAU1979_01_MCS_256                 (0x01u)     /* INPUT 256 (x fs for 32kHz to 48 kHz) */
#define ENUM_ADAU1979_01_MCS_128                 (0x00u)     /* INPUT 128 (x fs for 32kHz to 48 kHz) */

/* Power block control and serial port control Register 0x04 field definitions (ADI_ADAU1979_REG_PWR_SP_CTL) */
#define BITM_ADAU1979_04_LRCLK_POL                (0x80u)     /* LRCLK Polarity */            
#define ENUM_ADAU1979_04_LRCLK_POL_HIGH_LOW       (0x80u)     /* LRCLK polarity high then low */

#define BITM_ADAU1979_04_BCLK_POL                 (0x40u)     /* BCLK Polarity */                           
#define ENUM_ADAU1979_04_BCLK_POL_RAISE           (0x40u)     /* BCLK Polarity - Data changes on rising edge */

#define BITM_ADAU1979_04_LDO                      (0x20u)     /* Low drop out regulator */                           
#define ENUM_ADAU1979_04_LDO_EN                   (0x20u)     /* Enable: Low drop out regulator */

#define BITM_ADAU1979_04_VREF                     (0x10u)     /* Voltage reference */                           
#define ENUM_ADAU1979_04_VREF_EN                  (0x10u)     /* Enable: Voltage reference */

#define BITM_ADAU1979_04_ADC4                     (0x08u)     /* ADC4 enable */                           
#define ENUM_ADAU1979_04_ADC4_EN                  (0x08u)     /* Enable: Power up ADC4 */

#define BITM_ADAU1979_04_ADC3                     (0x04u)     /* ADC3 enable */                           
#define ENUM_ADAU1979_04_ADC3_EN                  (0x04u)     /* Enable: Power up ADC3 */

#define BITM_ADAU1979_04_ADC2                     (0x02u)     /* ADC2 enable */                           
#define ENUM_ADAU1979_04_ADC2_EN                  (0x02u)     /* Enable: Power up ADC2 */

#define BITM_ADAU1979_04_ADC1                     (0x01u)     /* ADC1 enable */                           
#define ENUM_ADAU1979_04_ADC1_EN                  (0x01u)     /* Enable: Power up ADC1 */

/* Serial port control 1 Register 0x05 field definitions (ADI_ADAU1979_REG_SP_CTL1) */
#define BITM_ADAU1979_05_SERIAL_FMT               (0xC0u)     /* Serial Format */
#define ENUM_ADAU1979_05_FMT_RIGHT_16BIT          (0xC0u)     /* Right justified 16 bit */
#define ENUM_ADAU1979_05_FMT_RIGHT_24BIT          (0x80u)     /* Right justified 24 bit */
#define ENUM_ADAU1979_05_FMT_LEFT                 (0x40u)     /* Left justified */
#define ENUM_ADAU1979_05_FMT_STEREO               (0x00u)     /* Stereo/I2S mode (normal) */

#define BITM_ADAU1979_05_MODE                     (0x38u)     /* Serial Mode */
#define ENUM_ADAU1979_05_MODE_TDM16               (0x20u)     /* TDM16 mode */
#define ENUM_ADAU1979_05_MODE_TDM8                (0x18u)     /* TDM8 mode */
#define ENUM_ADAU1979_05_MODE_TDM4                (0x10u)     /* TDM4 mode */
#define ENUM_ADAU1979_05_MODE_TDM2                (0x08u)     /* TDM2 mode */
#define ENUM_ADAU1979_05_MODE_STEREO              (0x00u)     /* Stereo mode */

#define BITM_ADAU1979_05_SR                       (0x07u)     /* Sample Rate */
#define ENUM_ADAU1979_05_SR_128_192KHZ            (0x04u)     /* 128 to 192 kHz */
#define ENUM_ADAU1979_05_SR_64_96KHZ              (0x03u)     /*  64 to  96 kHz */
#define ENUM_ADAU1979_05_SR_32_48KHZ              (0x02u)     /*  32 to  48 kHz */
#define ENUM_ADAU1979_05_SR_16_24KHZ              (0x01u)     /*  16 to  24 kHz */
#define ENUM_ADAU1979_05_SR_8_12KHZ               (0x00u)     /*   8 to  12 kHz */

/* Serial port control 2 Register 0x06 field definitions (ADI_ADAU1979_REG_SP_CTL2) */
#define BITM_ADAU1979_06_SDATA_SEL                (0x80u)     /* SData select in TDM4 mode or greater */
#define ENUM_ADAU1979_06_SDATA1                   (0x00u)     /* Select SDATAOUT1 */
                       
#define BITM_ADAU1979_06_SLOT_WIDTH               (0x60u)     /* Slot width, number of BCLK per slot in TDM mode */
#define ENUM_ADAU1979_06_SLOT_16                  (0x40u)     /* 16 BCLKs per slot */
#define ENUM_ADAU1979_06_SLOT_24                  (0x20u)     /* 24 BCLKs per slot */
#define ENUM_ADAU1979_06_SLOT_32                  (0x00u)     /* 32 BCLKs per slot */

#define BITM_ADAU1979_06_WORD_WIDTH               (0x10u)     /* Data word width, output data width */
#define ENUM_ADAU1979_06_WORD_16                  (0x10u)     /* 16 bit */
#define ENUM_ADAU1979_06_WORD_24                  (0x00u)     /* 24 bit */

#define BITM_ADAU1979_06_LRCLK_MODE               (0x08u)     /* LRCLK Mode */
#define ENUM_ADAU1979_06_LRCLK_MODE_PULSE         (0x08u)     /* LRCLK Mode - Pulse (32 BCLKs per channel) */
#define ENUM_ADAU1979_06_LRCLK_MODE_50            (0x00u)     /* LRCLK Mode - 50% duty cycle */
                      
#define BITM_ADAU1979_06_MSB_LSB                  (0x04u)     /* MSB/LSB first */
#define ENUM_ADAU1979_06_MSB_FIRST                (0x04u)     /* MSB first */
#define ENUM_ADAU1979_06_LSB_FIRST                (0x00u)     /* LSB first */

#define BITM_ADAU1979_06_BCLK_RATE                (0x02u)     /* Number of BCLK per cycle when in master mode  */
#define ENUM_ADAU1979_06_BCLK_32                  (0x00u)     /* 32 BCLKS */
#define ENUM_ADAU1979_06_BCLK_16                  (0x02u)     /* 16 BCLKS */
                       
#define BITM_ADAU1979_06_CLK_MS                   (0x01u)     /* BCLK/LRCLK Master/Slave */
#define ENUM_ADAU1979_06_CLK_MASTER               (0x01u)     /* BCLK/LRCLK as Master */

/* Channel 1&2 serial port mapping Register 0x07 field definitions (ADI_ADAU1979_REG_CH1_2_MAP) */
#define BITM_ADAU1979_07_CH1_MAP                  (0x0Fu)     /* Channel 1 serial port mapping */
#define BITP_ADAU1979_07_CH1_MAP                  (0u)        /* Channel 1 serial port mapping position */
#define BITM_ADAU1979_07_CH2_MAP                  (0xF0u)     /* Channel 2 serial port mapping */
#define BITP_ADAU1979_07_CH2_MAP                  (4u)        /* Channel 2 serial port mapping position */

/* Channel 3&4 serial port mapping Register 0x08 field definitions (ADI_ADAU1979_REG_CH3_4_MAP) */
#define BITM_ADAU1979_08_CH3_MAP                  (0x0Fu)     /* Channel 3 serial port mapping */
#define BITP_ADAU1979_08_CH3_MAP                  (0u)        /* Channel 3 serial port mapping position */
#define BITM_ADAU1979_08_CH4_MAP                  (0xF0u)     /* Channel 4 serial port mapping */
#define BITP_ADAU1979_08_CH4_MAP                  (4u)        /* Channel 4 serial port mapping position */

/* Serial output drive control and over temperature protection status Register 0x09 field definitions (ADI_ADAU1979_REG_OUT_TEMP) */
#define BITM_ADAU1979_09_CH4_OP                   (0x80u)     /* Channel 4 output driver */                 
#define ENUM_ADAU1979_09_CH4_OP_EN                (0x80u)     /* Enabled: Channel 4 output */

#define BITM_ADAU1979_09_CH3_OP                   (0x40u)     /* Channel 3 output driver */                 
#define ENUM_ADAU1979_09_CH3_OP_EN                (0x40u)     /* Enabled: Channel 3 output */

#define BITM_ADAU1979_09_CH2_OP                   (0x20u)     /* Channel 2 output driver */                 
#define ENUM_ADAU1979_09_CH2_OP_EN                (0x20u)     /* Enabled: Channel 2 output */

#define BITM_ADAU1979_09_CH1_OP                   (0x10u)     /* Channel 1 output driver */                 
#define ENUM_ADAU1979_09_CH1_OP_EN                (0x10u)     /* Enabled: Channel 1 output */

#define BITM_ADAU1979_09_TRI_OP                   (0x08u)     /* Tristate unused channels */                 
#define ENUM_ADAU1979_09_TRI_OP_EN                (0x08u)     /* Enabled: Tristated unused channels */

#define BITM_ADAU1979_09_OVER_TEMP                (0x01u)     /* Over temperature status */                 
#define ENUM_ADAU1979_09_TEMP_FAULT               (0x01u)     /* Over temperature fault */

/* High pass filter, DC offset, master mute Register 0x0E field definitions (ADI_ADAU1979_REG_HPF_DC) */
#define BITM_ADAU1979_0E_SUM_MODE                 (0xC0u)     /* Summing mode */
#define ENUM_ADAU1979_0E_SUM_1CHANNEL             (0x80u)     /* 1 channel summing */
#define ENUM_ADAU1979_0E_SUM_2CHANNEL             (0x40u)     /* 2 channel summing */
#define ENUM_ADAU1979_0E_SUM_NONE                 (0x00u)     /* normal 4 channel mode */

#define BITM_ADAU1979_0E_MUTE                     (0x10u)     /* Master mute mode */
#define ENUM_ADAU1979_0E_MUTE_ALL                 (0x10u)     /* Mute all channels */
#define ENUM_ADAU1979_0E_UNMUTE_ALL               (0x00u)     /* UnMute all channels */

#define BITM_ADAU1979_0E_CALIBRATE                (0x01u)     /* DC calibrate */
#define ENUM_ADAU1979_0E_CALIBRATE_EN             (0x01u)     /* Enabled: DC calibrate */

/* ADC clipping status Register 0x19 field definitions (ADI_ADAU1979_REG_CLIP) */
#define BITM_ADAU1979_19_CH4_CLIP                 (0x08u)     /* Channel 4 ADC clipping */
#define ENUM_ADAU1979_19_CH4_CLIP_EN              (0x08u)     /* Enabled: channel 4 ADC clipping */

#define BITM_ADAU1979_19_CH3_CLIP                 (0x08u)     /* Channel 3 ADC clipping */
#define ENUM_ADAU1979_19_CH3_CLIP_EN              (0x08u)     /* Enabled: channel 3 ADC clipping */

#define BITM_ADAU1979_19_CH2_CLIP                 (0x08u)     /* Channel 2 ADC clipping */
#define ENUM_ADAU1979_19_CH2_CLIP_EN              (0x08u)     /* Enabled: channel 2 ADC clipping */

#define BITM_ADAU1979_19_CH1_CLIP                 (0x08u)     /* Channel 1 ADC clipping */
#define ENUM_ADAU1979_19_CH1_CLIP_EN              (0x08u)     /* Enabled: channel 1 ADC clipping */

/* DC high pass filter and calibration Register 0x1A field definitions (ADI_ADAU1979_DCHPF_CAL) */
#define BITM_ADAU1979_1A_CH4_CAL                  (0x80u)     /* Channel 4 DC calibration */
#define ENUM_ADAU1979_1A_CH4_CAL_EN               (0x80u)     /* Enabled: channel 4 DC calibration is subtracted */

#define BITM_ADAU1979_1A_CH3_CAL                  (0x40u)     /* Channel 3 DC calibration */
#define ENUM_ADAU1979_1A_CH3_CAL_EN               (0x40u)     /* Enabled: channel 3 DC calibration is subtracted */

#define BITM_ADAU1979_1A_CH2_CAL                  (0x20u)     /* Channel 2 DC calibration */
#define ENUM_ADAU1979_1A_CH2_CAL_EN               (0x20u)     /* Enabled: channel 2 DC calibration is subtracted */

#define BITM_ADAU1979_1A_CH1_CAL                  (0x10u)     /* Channel 1 DC calibration */
#define ENUM_ADAU1979_1A_CH1_CAL_EN               (0x10u)     /* Enabled: channel 1 DC calibration is subtracted */

#define BITM_ADAU1979_1A_CH4_DCHPF                (0x08u)     /* Channel 4 DC high pass filter */
#define ENUM_ADAU1979_1A_CH4_DCHPF_EN             (0x08u)     /* Enabled: channel 4 DC high pass filter */

#define BITM_ADAU1979_1A_CH3_DCHPF                (0x04u)     /* Channel 3 DC high pass filter */
#define ENUM_ADAU1979_1A_CH3_DCHPF_EN             (0x04u)     /* Enabled: channel 3 DC high pass filter */

#define BITM_ADAU1979_1A_CH2_DCHPF                (0x02u)     /* Channel 2 DC high pass filter */
#define ENUM_ADAU1979_1A_CH2_DCHPF_EN             (0x02u)     /* Enabled: channel 2 DC high pass filter */

#define BITM_ADAU1979_1A_CH1_DCHPF                (0x01u)     /* Channel 1 DC high pass filter */
#define ENUM_ADAU1979_1A_CH1_DCHPF_EN             (0x01u)     /* Enabled: channel 1 DC high pass filter */

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */

#endif  /* __ADI_ADAU1979_REGS_H__ */

/*****/
