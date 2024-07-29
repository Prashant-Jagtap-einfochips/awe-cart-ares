/*********************************************************************************

Copyright(c) 2014 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

/*!
 * @file:    adi_adau1962a_regs.h
 * @brief:   Include file for ADAU1962A driver with register definitions.
 * @version: $Revision: 25249 $
 * @date:    $Date: 2016-02-12 12:09:59 -0500 (Fri, 12 Feb 2016) $
 *
 * @details
 *           This is a private header file for ADAU1962A DAC driver that
 *           contains register field definitions used in ADAU1962A driver implementation.
 */

#ifndef __ADI_ADAU1962A_REGS_H__
#define __ADI_ADAU1962A_REGS_H__

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_5_1:"Allow identifiers to be more than 31 characters")
#endif

/*==============  D E F I N E S  ===============*/

/*
 * ADAU1962A Register Field definitions
 */

/* PLL and Clock control 0 Register field definitions (ADI_ADAU1962A_REG_PLL_CLK_CTRL0) */
#define BITM_ADAU1962A_00_PLL_IN                    (0xC0u)     /* PLL Input */
#define ENUM_ADAU1962A_00_PLL_IN_DLRCLK             (0x40u)     /* DLRCLK */
#define ENUM_ADAU1962A_00_PLL_IN_MCLKI              (0x00u)     /* MCLKI/XI */

#define BITM_ADAU1962A_00_XTAL_SET                  (0x30u)     /* XTALO pin status */
#define ENUM_ADAU1962A_00_XTAL_OSC_OFF              (0x30u)     /* XTAL Oscillator Off */
#define ENUM_ADAU1962A_00_XTAL_OSC_ON               (0x00u)     /* XTAL oscillator On */

#define BITM_ADAU1962A_00_SOFT_RST                  (0x08u)     /* Soft-Reset control */
#define ENUM_ADAU1962A_00_SOFT_RST_EN               (0x08u)     /* Enable Soft-Reset */
#define ENUM_ADAU1962A_00_SOFT_RST_CLR              (0x00u)     /* Clear Soft-Reset */

#define BITP_ADAU1962A_00_MCLK_SEL                  (1u)        /* MCLK Select position */
#define BITM_ADAU1962A_00_MCLK_SEL                  (0x06u)     /* MCLK pin functionality (PLL active) Mask */
#define ENUM_ADAU1962A_00_MCLK_SEL_768              (0x06u)     /* INPUT 768 (x 44.1 kHz or 48 kHz) */
#define ENUM_ADAU1962A_00_MCLK_SEL_512              (0x04u)     /* INPUT 512 (x 44.1 kHz or 48 kHz) */
#define ENUM_ADAU1962A_00_MCLK_SEL_384              (0x02u)     /* INPUT 384 (x 44.1 kHz or 48 kHz) */
#define ENUM_ADAU1962A_00_MCLK_SEL_256              (0x00u)     /* INPUT 256 (x 44.1 kHz or 48 kHz) */

#define BITM_ADAU1962A_00_MSTR_PWR                  (0x01u)     /* Master Power-Up Control */
#define ENUM_ADAU1962A_00_MSTR_PWR_UP               (0x01u)     /* Master Power-Up */
#define ENUM_ADAU1962A_00_MSTR_PWR_DN               (0x00u)     /* Master Power-Down */

/* PLL and Clock control 1 Register field definitions (ADI_ADAU1962A_REG_PLL_CLK_CTRL1) */
#define BITM_ADAU1962A_01_LOPWR_MODE                (0xC0u)     /* Global Power/Performance Adjust */
#define ENUM_ADAU1962A_01_LOPWR_MODE_LOWEST         (0xC0u)     /* Lowest Power */
#define ENUM_ADAU1962A_01_LOPWR_MODE_LOWER          (0x80u)     /* Lower Power */
#define ENUM_ADAU1962A_01_LOPWR_MODE_I2C            (0x00u)     /* I2C Register Settings */

#define BITM_ADAU1962A_01_MCLKO_SEL                 (0x30u)     /* MCLK Output Frequency */
#define ENUM_ADAU1962A_01_MCLKO_DIS                 (0x30u)     /* MCLKO Pin Disabled */
#define ENUM_ADAU1962A_01_MCLKO_MCLKI               (0x20u)     /* Buffered MCLKI */
#define ENUM_ADAU1962A_01_MCLKO_8_12MHZ             (0x10u)     /* 8 MHz to 12 MHz scaled by fs */
#define ENUM_ADAU1962A_01_MCLKO_4_6MHZ              (0x00u)     /* 4 MHz to 6 MHz scaled by fs */

#define BITM_ADAU1962A_01_PLL_MUTE                  (0x08u)     /* PLL Automute Enable/Lock */
#define ENUM_ADAU1962A_01_PLL_MUTE_EN               (0x08u)     /* DAC Automute on PLL Unlock */
#define ENUM_ADAU1962A_01_PLL_MUTE_DIS              (0x00u)     /* No DAC Automute */

#define BITM_ADAU1962A_01_PLL_LOCK                  (0x04u)     /* PLL Lock Indicator */
#define ENUM_ADAU1962A_01_PLL_LOCKED                (0x00u)     /* PLL Locked */

#define BITM_ADAU1962A_01_VREF_EN                   (0x02u)     /* Internal Voltage Reference Enable */
#define ENUM_ADAU1962A_01_VREF_EN                   (0x02u)     /* VREF Enabled */
#define ENUM_ADAU1962A_01_VREF_DIS                  (0x00u)     /* VREF Disabled */

#define BITM_ADAU1962A_01_DAC_CLK_SEL               (0x01u)     /* DAC Clock Select */
#define ENUM_ADAU1962A_01_DAC_CLK_MCLKI             (0x01u)     /* MCLK from MCLKI or XTALI as DAC Clock Source */
#define ENUM_ADAU1962A_01_DAC_CLK_PLL               (0x00u)     /* PLL as DAC Clock Source */

/* Block Power-down and Thermal Sensor Control 1 Register field definitions (ADI_ADAU1962A_REG_PDN_THRMSENS_CTRL_1) */
#define BITM_ADAU1962A_02_THRM_RATE                 (0xC0u)     /* Conversion Time Interval */
#define ENUM_ADAU1962A_02_THRM_RATE_2S              (0xC0u)     /* 2 sec/Conversion */
#define ENUM_ADAU1962A_02_THRM_RATE_1S              (0x80u)     /* 1 sec/Conversion */
#define ENUM_ADAU1962A_02_THRM_RATE_0_5_S           (0x40u)     /* One-Shot Mode */
#define ENUM_ADAU1962A_02_THRM_RATE_4S              (0x00u)     /* 4 sec/Conversion */

#define BITM_ADAU1962A_02_THRM_MODE                 (0x20u)     /* One-Shot Conversion Mode */
#define ENUM_ADAU1962A_02_THRM_MODE_ONE_SHOT        (0x20u)     /* One-Shot Mode */
#define ENUM_ADAU1962A_02_THRM_MODE_CONTINUOUS      (0x00u)     /* Continuous Operation */

#define BITM_ADAU1962A_02_THRM_GO                   (0x10u)     /* One-Shot Conversion Mode */
#define ENUM_ADAU1962A_02_THRM_GO_CONVERT           (0x10u)     /* Convert Temperature */
#define ENUM_ADAU1962A_02_THRM_GO_RESET             (0x00u)     /* Reset */

#define BITM_ADAU1962A_02_TS_PDN                    (0x04u)     /* Temperature Sensor Power-Down */
#define ENUM_ADAU1962A_02_TS_PDN_PDN                (0x04u)     /* Temperature Sensor Power-Down */
#define ENUM_ADAU1962A_02_TS_PDN_NORMAL             (0x00u)     /* Temperature Sensor Normal Operation */

#define BITM_ADAU1962A_02_PLL_PDN                   (0x02u)     /* PLL Power-Down */
#define ENUM_ADAU1962A_02_PLL_PDN_PDN               (0x02u)     /* PLL Power-Down */
#define ENUM_ADAU1962A_02_PLL_PDN_NORMAL            (0x00u)     /* PLL Normal Operation */

#define BITM_ADAU1962A_02_VREG_PDN                  (0x01u)     /* Voltage Regulator Power-Down */
#define ENUM_ADAU1962A_02_VREG_PDN_PDN              (0x01u)     /* Voltage Regulator Power-Down */
#define ENUM_ADAU1962A_02_VREG_PDN_NORMAL           (0x00u)     /* Voltage Regulator Normal Operation */

/* Bit mask for power-down control */
#define BITM_ADAU1962A_02_PDN_CTRL                  (BITM_ADAU1962A_02_TS_PDN  |\
                                                     BITM_ADAU1962A_02_PLL_PDN |\
                                                     BITM_ADAU1962A_02_VREG_PDN)

/* Power-Down control 2 Register field definitions (ADI_ADAU1962A_REG_PDN_CTRL2) */
#define BITM_ADAU1962A_03_DAC_PDN                   (0x01u)     /* Power-Down DAC Channel */
#define ENUM_ADAU1962A_03_DAC_PDN_PDN               (0x01u)     /* Power-Down DAC Channel */
#define ENUM_ADAU1962A_03_DAC_PDN_NORMAL            (0x00u)     /* Normal Operation */

/* DAC control 0 Register field definitions (ADI_ADAU1962A_REG_DAC_CTRL0) */
#define BITM_ADAU1962A_06_SDATA_FMT                 (0xC0u)     /* SDATA Format */
#define ENUM_ADAU1962A_06_SDATA_FMT_I2S             (0x00u)     /* I2S, 1 BCLK Cycle Delay */

#define BITM_ADAU1962A_06_SAI                       (0x38u)     /* Serial Audio Interface */
#define ENUM_ADAU1962A_06_SAI_TDM16                 (0x20u)     /* TDM16—Single Line (48 kHz) */
#define ENUM_ADAU1962A_06_SAI_TDM8                  (0x18u)     /* TDM8—Dual Line */
#define ENUM_ADAU1962A_06_SAI_TDM4                  (0x10u)     /* TDM4—Quad Line */
#define ENUM_ADAU1962A_06_SAI_TDM2                  (0x08u)     /* TDM2—Octal Line */
#define ENUM_ADAU1962A_06_SAI_STEREO                (0x00u)     /* Stereo (I2S, LJ, RJ) */

#define BITP_ADAU1962A_06_FS                        (1u)        /* Sample Rate Select */
#define BITM_ADAU1962A_06_FS                        (0x06u)     /* Mask for Sample Rate Select */
#define ENUM_ADAU1962A_06_FS_128_176_192_LOW        (0x06u)     /* 128 kHz/176.4 kHz/192 kHz Low Propagation Delay */
#define ENUM_ADAU1962A_06_FS_128_176_192            (0x04u)     /* 128 kHz/176.4 kHz/192 kHz */
#define ENUM_ADAU1962A_06_FS_64_88_96               (0x02u)     /* 64 kHz/88.2 kHz/96 kHz */
#define ENUM_ADAU1962A_06_FS_32_44_48               (0x00u)     /* 32 kHz/44.1 kHz/48 kHz */

#define BITM_ADAU1962A_06_MMUTE                     (0x01u)     /* Master Mute */
#define ENUM_ADAU1962A_06_MMUTE_EN                  (0x01u)     /* All Channels Muted */
#define ENUM_ADAU1962A_06_MMUTE_DIS                 (0x00u)     /* Normal Operation */

/* DAC control 1 Register field definitions (ADI_ADAU1962A_REG_DAC_CTRL1) */
#define BITM_ADAU1962A_07_BCLK_GEN                  (0x80u)     /* DBCLK Generation */
#define ENUM_ADAU1962A_07_BCLK_GEN_INTERNAL         (0x80u)     /* Internal DBCLK Generation */
#define ENUM_ADAU1962A_07_BCLK_GEN_NORMAL           (0x00u)     /* Normal Operation—DBCLK */

#define BITM_ADAU1962A_07_LRCLK_MODE                (0x40u)     /* DLRCLK Mode Select. Only Valid for TDM modes */
#define ENUM_ADAU1962A_07_LRCLK_PULSE               (0x40u)     /* Pulse mode */
#define ENUM_ADAU1962A_07_LRCLK_50_50               (0x00u)     /* 50% Duty Cycle DLRCLK */

#define BITM_ADAU1962A_07_LRCLK_POL                 (0x20u)     /* DLRCLK Polarity. Allows the swapping of data between channels */
#define ENUM_ADAU1962A_07_LRCLK_POL_INVERT          (0x20u)     /* Left/Odd channels are DLRCLK High (Inverted) */
#define ENUM_ADAU1962A_07_LRCLK_POL_NORMAL          (0x00u)     /* Left/Odd channels are DLRCLK Low (Normal) */

#define BITM_ADAU1962A_07_SAI_MSB                   (0x10u)     /* MSB Position */
#define ENUM_ADAU1962A_07_SAI_LSB_FIRST             (0x10u)     /* LSB First DSDATA */
#define ENUM_ADAU1962A_07_SAI_MSB_FIRST             (0x00u)     /* MSB First DSDATA */

#define BITM_ADAU1962A_07_BCLK_RATE                 (0x04u)     /* DBCLK Rate. Number of DBCLK cycles per DLRCLK frame */
#define ENUM_ADAU1962A_07_BCLK_RATE_16CYCLES        (0x04u)     /* 16 Cycles per Frame */
#define ENUM_ADAU1962A_07_BCLK_RATE_32CYCLES        (0x00u)     /* 32 Cycles per Frame */

#define BITM_ADAU1962A_07_BCLK_EDGE                 (0x02u)     /* DBCLK Active Edge */
#define ENUM_ADAU1962A_07_BCLK_FALLING_EDGE         (0x02u)     /* Latch on Falling Edge */
#define ENUM_ADAU1962A_07_BCLK_RISING_EDGE          (0x00u)     /* Latch on Rising Edge */

#define BITM_ADAU1962A_07_SAI_MS                    (0x01u)     /* Serial Interface Master */
#define ENUM_ADAU1962A_07_SAI_MASTER                (0x01u)     /* DLRCLK/DBCLK Master */
#define ENUM_ADAU1962A_07_SAI_SLAVE                 (0x00u)     /* DLRCLK/DBCLK Slave */

/* DAC control 2 Register field definitions (ADI_ADAU1962A_REG_DAC_CTRL2) */
#define BITM_ADAU1962A_08_BCLK_TDMC                 (0x20u)     /* DBCLK Rate in TDM Mode */
#define ENUM_ADAU1962A_08_BCLK_TDMC_16              (0x20u)     /* 16 BCLK cycles/channel slot */
#define ENUM_ADAU1962A_08_BCLK_TDMC_32              (0x00u)     /* 32 BCLK cycles/channel slot */

#define BITM_ADAU1962A_08_DAC_POL                   (0x10u)     /* DAC Output Polarity. This is a global switch of DAC polarity */
#define ENUM_ADAU1962A_08_DAC_POL_INVERT            (0x10u)     /* Inverted DAC Output */
#define ENUM_ADAU1962A_08_DAC_POL_NON_INVERT        (0x00u)     /* NonInverted DAC Output */

#define BITM_ADAU1962A_08_AUTO_MUTE_EN              (0x04u)     /* Automute Enable */
#define ENUM_ADAU1962A_08_AUTO_MUTE_EN              (0x04u)     /* Auto-Zero Input Mute Enabled */
#define ENUM_ADAU1962A_08_AUTO_MUTE_DIS             (0x00u)     /* Auto-Zero Input Mute Disabled */

#define BITM_ADAU1962A_08_DAC_OSR                   (0x02u)     /* DAC Oversampling Rate. OSR selection */
#define ENUM_ADAU1962A_08_DAC_OSR_128               (0x02u)     /* 128 × fs DAC Oversampling */
#define ENUM_ADAU1962A_08_DAC_OSR_256               (0x00u)     /* 256 × fs DAC Oversampling */

#define BITM_ADAU1962A_08_DE_EMP_EN                 (0x01u)     /* De-Emphasis Enable */
#define ENUM_ADAU1962A_08_DE_EMP_EN                 (0x01u)     /* De-Emphasis Enabled */
#define ENUM_ADAU1962A_08_DE_EMP_DIS                (0x00u)     /* No De-Emphasis/Flat */

/* DAC Individual channel mutes 1 Register field definitions (ADI_ADAU1962A_REG_DAC_MUTE1) */
#define BITM_ADAU1962A_09_DAC_MUTE                  (0x01u)     /* Mute DAC Channel */
#define ENUM_ADAU1962A_09_DAC_MUTE_EN               (0x01u)     /* Mute DAC Channel */
#define ENUM_ADAU1962A_09_DAC_MUTE_DIS              (0x00u)     /* Normal Operation, Mute disabled */

/* Pad strength Register field definitions (ADI_ADAU1962A_REG_PAD_STRGTH) */
#define BITM_ADAU1962A_1C_PAD_DRV                   (0x20u)     /* Output Pad Drive Strength Control */
#define ENUM_ADAU1962A_1C_PAD_DRV_8MA               (0x20u)     /* 8 mA Drive for All Pads */
#define ENUM_ADAU1962A_1C_PAD_DRV_4MA               (0x00u)     /* 4 mA Drive for All Pads */

/* DAC Power adjust 1 Register field definitions (ADI_ADAU1962A_REG_DAC_POWER1) */
#define BITM_ADAU1962A_1D_DAC_POWER                 (0x03u)     /* DAC Power Control */
#define ENUM_ADAU1962A_1D_DAC_GOOD_PERF             (0x03u)     /* Good Performance */
#define ENUM_ADAU1962A_1D_DAC_BEST_PERF             (0x02u)     /* Best Performance */
#define ENUM_ADAU1962A_1D_DAC_LOWEST_POWER          (0x01u)     /* Lowest Power */
#define ENUM_ADAU1962A_1D_DAC_LOW_POWER             (0x00u)     /* Low Power */

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */

#endif  /* __ADI_ADAU1962A_REGS_H__ */

/*****/

