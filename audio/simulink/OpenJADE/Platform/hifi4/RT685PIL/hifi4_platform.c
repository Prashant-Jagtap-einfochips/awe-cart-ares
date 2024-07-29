//#include <xtensa/config/core.h>
//#include <xtensa/xos.h>

#include <stdint.h>

#include <fsl_clock.h>
#include <fsl_common.h>
#include <fsl_reset.h>
#include <fsl_gpio.h>
#include <fsl_inputmux.h>
#include <fsl_iopctl.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL_SYS_CLK_HZ 24000000U /*!< Board xtal_sys frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ  32768U    /*!< Board xtal32K frequency in Hz */

/*! @brief The board name */
#define BOARD_NAME "MIMXRT685-EVK"

#define IOPCTL_PIO_ANAMUX_DI 0x00u        /*!<@brief Analog mux is disabled */
#define IOPCTL_PIO_FULLDRIVE_DI 0x00u     /*!<@brief Normal drive */
#define IOPCTL_PIO_FULLDRIVE_EN 0x0100u   /*!<@brief Full drive enable */
#define IOPCTL_PIO_FUNC1 0x01u            /*!<@brief Selects pin function 1 */
#define IOPCTL_PIO_INBUF_EN 0x40u         /*!<@brief Enables input buffer function */
#define IOPCTL_PIO_INV_DI 0x00u           /*!<@brief Input function is not inverted */
#define IOPCTL_PIO_PSEDRAIN_DI 0x00u      /*!<@brief Pseudo Output Drain is disabled */
#define IOPCTL_PIO_PULLDOWN_EN 0x00u      /*!<@brief Enable pull-down function */
#define IOPCTL_PIO_PULLUP_EN 0x20u        /*!<@brief Enable pull-up function */
#define IOPCTL_PIO_PUPD_DI 0x00u          /*!<@brief Disable pull-up / pull-down function */
#define IOPCTL_PIO_PUPD_EN 0x10u          /*!<@brief Enable pull-up / pull-down function */
#define IOPCTL_PIO_SLEW_RATE_NORMAL 0x00u /*!<@brief Normal mode */


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);
void BOARD_InitPins(void);
static void BOARD_InitClock(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint32_t g_xtalFreq = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: R1, peripheral: DMIC0, signal: 'CLK, 0_1', pin_signal: PIO2_16/PDM_CLK01, pupdena: disabled, pupdsel: pullDown, ibena: enabled, slew_rate: normal, drive: normal,
    amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: U1, peripheral: DMIC0, signal: 'CLK, 2_3', pin_signal: PIO2_17/PDM_CLK23/FLEXSPI0B_DATA4, pupdena: disabled, pupdsel: pullDown, ibena: enabled, slew_rate: normal,
    drive: normal, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: R2, peripheral: DMIC0, signal: 'CLK, 4_5', pin_signal: PIO2_18/PDM_CLK45/FLEXSPI0B_DATA5, pupdena: disabled, pupdsel: pullDown, ibena: enabled, slew_rate: normal,
    drive: normal, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: T2, peripheral: DMIC0, signal: 'CLK, 6_7', pin_signal: PIO2_19/PDM_CLK67/FLEXSPI0B_SS0_N, pupdena: disabled, pupdsel: pullDown, ibena: enabled, slew_rate: normal,
    drive: normal, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: U2, peripheral: DMIC0, signal: 'DATA, 0_1', pin_signal: PIO2_20/PDM_DATA01, pupdena: disabled, pupdsel: pullDown, ibena: enabled, slew_rate: normal,
    drive: normal, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: R3, peripheral: DMIC0, signal: 'DATA, 2_3', pin_signal: PIO2_21/PDM_DATA23/CTIMER_INP14/FLEXSPI0B_SS1_N, pupdena: disabled, pupdsel: pullDown, ibena: enabled,
    slew_rate: normal, drive: normal, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: P3, peripheral: DMIC0, signal: 'DATA, 4_5', pin_signal: PIO2_22/PDM_DATA45/FLEXSPI0B_DATA6, pupdena: disabled, pupdsel: pullDown, ibena: enabled, slew_rate: normal,
    drive: normal, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: P5, peripheral: DMIC0, signal: 'DATA, 6_7', pin_signal: PIO2_23/PDM_DATA67/FLEXSPI0B_DATA7, pupdena: disabled, pupdsel: pullDown, ibena: enabled, slew_rate: normal,
    drive: normal, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: J2, peripheral: FLEXCOMM1, signal: SCK, pin_signal: PIO0_7/FC1_SCK/SCT0_GPI4/SCT0_OUT4/CTIMER1_MAT0/I2S_BRIDGE_CLK_OUT/SEC_PIO0_7, pupdena: enabled,
    pupdsel: pullUp, ibena: enabled, slew_rate: normal, drive: full, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: K4, peripheral: FLEXCOMM1, signal: TXD_SCL_MISO_WS, pin_signal: PIO0_8/FC1_TXD_SCL_MISO_WS/SCT0_GPI5/SCT0_OUT5/CTIMER1_MAT1/I2S_BRIDGE_WS_OUT/SEC_PIO0_8,
    pupdena: enabled, pupdsel: pullUp, ibena: enabled, slew_rate: normal, drive: full, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: L3, peripheral: FLEXCOMM1, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_9/FC1_RXD_SDA_MOSI_DATA/SCT0_GPI6/SCT0_OUT6/CTIMER1_MAT2/I2S_BRIDGE_DATA_OUT/SEC_PIO0_9,
    pupdena: enabled, pupdsel: pullUp, ibena: enabled, slew_rate: normal, drive: full, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: B10, peripheral: FLEXCOMM4, signal: TXD_SCL_MISO_WS, pin_signal: PIO0_29/FC4_TXD_SCL_MISO_WS/CTIMER4_MAT1/I2S_BRIDGE_WS_OUT/SEC_PIO0_29, pupdena: enabled,
    pupdsel: pullUp, ibena: enabled, slew_rate: normal, drive: full, amena: disabled, odena: disabled, iiena: disabled}
  - {pin_num: C11, peripheral: FLEXCOMM4, signal: RXD_SDA_MOSI_DATA, pin_signal: PIO0_30/FC4_RXD_SDA_MOSI_DATA/CTIMER4_MAT2/I2S_BRIDGE_DATA_OUT/SEC_PIO0_30, pupdena: enabled,
    pupdsel: pullUp, ibena: enabled, slew_rate: normal, drive: full, amena: disabled, odena: disabled, iiena: disabled}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitPins(void)
{
    const uint32_t port0_pin29_config = (/* Pin is configured as FC4_TXD_SCL_MISO_WS */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Enable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_EN |
                                         /* Enable pull-up function */
                                         IOPCTL_PIO_PULLUP_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Full drive enable */
                                         IOPCTL_PIO_FULLDRIVE_EN |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT0 PIN29 (coords: B10) is configured as FC4_TXD_SCL_MISO_WS */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 29U, port0_pin29_config);

    const uint32_t port0_pin30_config = (/* Pin is configured as FC4_RXD_SDA_MOSI_DATA */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Enable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_EN |
                                         /* Enable pull-up function */
                                         IOPCTL_PIO_PULLUP_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Full drive enable */
                                         IOPCTL_PIO_FULLDRIVE_EN |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT0 PIN30 (coords: C11) is configured as FC4_RXD_SDA_MOSI_DATA */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 30U, port0_pin30_config);

    const uint32_t port0_pin7_config = (/* Pin is configured as FC1_SCK */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
                                        /* Enable pull-up function */
                                        IOPCTL_PIO_PULLUP_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Full drive enable */
                                        IOPCTL_PIO_FULLDRIVE_EN |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT0 PIN7 (coords: J2) is configured as FC1_SCK */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 7U, port0_pin7_config);

    const uint32_t port0_pin8_config = (/* Pin is configured as FC1_TXD_SCL_MISO_WS */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
                                        /* Enable pull-up function */
                                        IOPCTL_PIO_PULLUP_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Full drive enable */
                                        IOPCTL_PIO_FULLDRIVE_EN |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT0 PIN8 (coords: K4) is configured as FC1_TXD_SCL_MISO_WS */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 8U, port0_pin8_config);

    const uint32_t port0_pin9_config = (/* Pin is configured as FC1_RXD_SDA_MOSI_DATA */
                                        IOPCTL_PIO_FUNC1 |
                                        /* Enable pull-up / pull-down function */
                                        IOPCTL_PIO_PUPD_EN |
                                        /* Enable pull-up function */
                                        IOPCTL_PIO_PULLUP_EN |
                                        /* Enables input buffer function */
                                        IOPCTL_PIO_INBUF_EN |
                                        /* Normal mode */
                                        IOPCTL_PIO_SLEW_RATE_NORMAL |
                                        /* Full drive enable */
                                        IOPCTL_PIO_FULLDRIVE_EN |
                                        /* Analog mux is disabled */
                                        IOPCTL_PIO_ANAMUX_DI |
                                        /* Pseudo Output Drain is disabled */
                                        IOPCTL_PIO_PSEDRAIN_DI |
                                        /* Input function is not inverted */
                                        IOPCTL_PIO_INV_DI);
    /* PORT0 PIN9 (coords: L3) is configured as FC1_RXD_SDA_MOSI_DATA */
    IOPCTL_PinMuxSet(IOPCTL, 0U, 9U, port0_pin9_config);

    const uint32_t port2_pin16_config = (/* Pin is configured as PDM_CLK01 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT2 PIN16 (coords: R1) is configured as PDM_CLK01 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 16U, port2_pin16_config);

    const uint32_t port2_pin17_config = (/* Pin is configured as PDM_CLK23 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT2 PIN17 (coords: U1) is configured as PDM_CLK23 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 17U, port2_pin17_config);

    const uint32_t port2_pin18_config = (/* Pin is configured as PDM_CLK45 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT2 PIN18 (coords: R2) is configured as PDM_CLK45 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 18U, port2_pin18_config);

    const uint32_t port2_pin19_config = (/* Pin is configured as PDM_CLK67 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT2 PIN19 (coords: T2) is configured as PDM_CLK67 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 19U, port2_pin19_config);

    const uint32_t port2_pin20_config = (/* Pin is configured as PDM_DATA01 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT2 PIN20 (coords: U2) is configured as PDM_DATA01 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 20U, port2_pin20_config);

    const uint32_t port2_pin21_config = (/* Pin is configured as PDM_DATA23 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT2 PIN21 (coords: R3) is configured as PDM_DATA23 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 21U, port2_pin21_config);

    const uint32_t port2_pin22_config = (/* Pin is configured as PDM_DATA45 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT2 PIN22 (coords: P3) is configured as PDM_DATA45 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 22U, port2_pin22_config);

    const uint32_t port2_pin23_config = (/* Pin is configured as PDM_DATA67 */
                                         IOPCTL_PIO_FUNC1 |
                                         /* Disable pull-up / pull-down function */
                                         IOPCTL_PIO_PUPD_DI |
                                         /* Enable pull-down function */
                                         IOPCTL_PIO_PULLDOWN_EN |
                                         /* Enables input buffer function */
                                         IOPCTL_PIO_INBUF_EN |
                                         /* Normal mode */
                                         IOPCTL_PIO_SLEW_RATE_NORMAL |
                                         /* Normal drive */
                                         IOPCTL_PIO_FULLDRIVE_DI |
                                         /* Analog mux is disabled */
                                         IOPCTL_PIO_ANAMUX_DI |
                                         /* Pseudo Output Drain is disabled */
                                         IOPCTL_PIO_PSEDRAIN_DI |
                                         /* Input function is not inverted */
                                         IOPCTL_PIO_INV_DI);
    /* PORT2 PIN23 (coords: P5) is configured as PDM_DATA67 */
    IOPCTL_PinMuxSet(IOPCTL, 2U, 23U, port2_pin23_config);
}

static void BOARD_InitClock(void)
{
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* sets external XTAL OSC freq */
}

extern int platform_printf( const char *fmt, ... );

void oj_platform_init(void)
{
    BOARD_InitBootPins();
    BOARD_InitClock();
}

