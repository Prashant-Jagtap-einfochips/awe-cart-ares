/*****************************************************************************
 * Audio_Loopback_TDM.c
 *****************************************************************************/
#include <sys/platform.h>
#include <sys/adi_core.h>
#include <services/int/adi_int.h>
#include <services/int/adi_sec.h>
#include <services/pwr/adi_pwr.h>
#include <drivers/uart/adi_uart.h>
#include <drivers/twi/adi_twi.h>
#include <services/spu/adi_spu.h>
#include <services/gpio/adi_gpio.h>
#include <drivers/adc/adau1979/adi_adau1979.h>
#include <drivers/dac/adau1962a/adi_adau1962a.h>
#include <SRU.h>
#include <interrupt.h>

#include "adi_initialize.h"

#include "sc584_ezkit.h"
#include "oj_platform.h"

#define TDM4
//#define TDM8

/* Number of callbacks that occur before the program exits */
#define CALLBACK_COUNT        (2000000u)

/* Macro to specify delay count for ADC/DAC reset */
#define DELAY_COUNT             (100000u)

/*
 * ADC settings
 */
/* ADAU1979 SPORT config parameters */
#define LR_B_CLK_MASTER_1979    (true)
#define BCLK_RISING_1979       (true)
#define LRCLK_HI_LO_1979       (false)
/*
 * DAC settings
 */
/* DAC Master clock frequency */
#define ADAU1962A_MCLK_IN       (24576000u)
/* DAC sample rate */
#define SAMPLE_RATE           (48000u)

/* ADAU1962A SPORT config parameters */
#define LR_B_CLK_MASTER_1962    (true)
#define BCLK_RISING_1962       (true)
#define LRCLK_HI_LO_1962        (false)

#if defined(TDM4)
#define NUM_CHANNELS          (4u)
#elif defined(TDM8)
#define NUM_CHANNELS          (8u)
#endif

/* SPU Peripheral ID */
#define  SPORT_4A_SPU_PID         (23u)
#define  SPORT_4B_SPU_PID         (24u)
#define  SPORT_4A_DMA10_SPU_PID   (74u)
#define  SPORT_4B_DMA11_SPU_PID   (75u)

#define MHZTOHZ               (1000000u)
#define EZKIT_CLKIN           (25u  * MHZTOHZ)

#define REPORT_ERROR           platform_printf

extern void deploy_target_step( void     *pAppHandle,
                                uint32_t  nEvent,
                                void     *pArg);

/*=============  D A T A  =============*/

/* Twi  */
uint32_t TwiMemory[ADI_TWI_MEMORY_SIZE];
/* Gpio */
uint32_t GpioMemory[ADI_GPIO_CALLBACK_MEM_SIZE];

/* ADAU1979 ADC */
static ADI_ADAU1979_HANDLE phAdau1979;
uint32_t Adau1979Memory[ADI_ADAU1979_MEMORY_SIZE];
/* ADAU1979 Sport */
uint32_t Adau1979SportMemory[ADI_SPORT_DMA_MEMORY_SIZE];

/* ADAU1962A DAC DATA */
static ADI_ADAU1962A_HANDLE phAdau1962a;
uint32_t Adau1962aMemory[ADI_ADAU1962A_MEMORY_SIZE];
/* ADAU1962A Sport */
uint32_t Adau1962aSportMemory[ADI_SPORT_DMA_MEMORY_SIZE];

/* sport buffers for ADC/DAC data */
int32_t int_DACPingBuffer[FRAME_SIZE_OUT];
int32_t int_DACPongBuffer[FRAME_SIZE_OUT];
int32_t int_ADCPingBuffer[FRAME_SIZE_IN];
int32_t int_ADCPongBuffer[FRAME_SIZE_IN];

/* Memory required for the SPU operation */
static uint8_t  SpuMemory[ADI_SPU_MEMORY_SIZE];

/* SPU handle */
static ADI_SPU_HANDLE hSpu;

/*=============  L O C A L    F U N C T I O N    P R O T O T Y P E S =============*/
/* Initialize GPIO and reset peripherals */
uint32_t    GpioInit(void);
/* Initializes ADC */
uint32_t    Adau1979Init(void);
/* Initializes DAC */
uint32_t    Adau1962aInit(void);
/* Submit buffers to ADC */
uint32_t    Adau1979SubmitBuffers(void);
/* Submit buffers to DAC */
uint32_t    Adau1962aSubmitBuffers(void);
/* Process audio buffers */
uint32_t    ProcessBuffers(void);


/*=============  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S =============*/

/* Configures soft switches */
extern void ConfigSoftSwitches(void);

/*=============  C O D E  =============*/
/*
 * Initializes GPIO service
 * A GPIO line is used to control reset of the ADC and DAC devices
 */
uint32_t GpioInit(void)
{
   uint32_t Result = 0u;
   /* Loop variable */
   volatile uint32_t i;
   uint32_t gpioMaxCallbacks;

   if ((uint32_t)adi_gpio_Init((void*)GpioMemory, ADI_GPIO_CALLBACK_MEM_SIZE, &gpioMaxCallbacks) != 0u)
   {
      /* return error */
      return 1u;
   }

   if ((uint32_t)adi_gpio_SetDirection(ADI_GPIO_PORT_A, ADI_GPIO_PIN_14, ADI_GPIO_DIRECTION_OUTPUT) != 0u)
   {
      /* return error */
      return 1u;
   }
   /* bring reset low */
   if ((uint32_t)adi_gpio_Clear(ADI_GPIO_PORT_A, ADI_GPIO_PIN_14) != 0u)
   {
      /* return error */
      return 1u;
   }

   /* delay */
   for (i = DELAY_COUNT; i ; i --);

   /* bring reset high */
   if ((uint32_t)adi_gpio_Set(ADI_GPIO_PORT_A, ADI_GPIO_PIN_14) != 0u)
   {
      /* return error */
      return 1u;
   }

   /* delay */
   for (i = DELAY_COUNT; i ; i --);

   return Result;
}

/*
 * Opens and initializes ADAU1979 ADC Device.
 *
 * Parameters
 *  None
 *
 * Returns
 *  0 if success, other values for error
 *
 */
uint32_t Adau1979Init(void)
{
   uint32_t Result = 0u;
   /* Instance to submit SPORT configuration */
   ADI_ADAU1979_SPORT_CONFIG   SportConfig;
   /* Instance to submit TWI configuration */
   ADI_ADAU1979_TWI_CONFIG     TwiConfig;

   /* open ADAU1979 instance */
   if ((uint32_t)adi_adau1979_Open(0u,
#if defined(TDM4)
                                   ADI_ADAU1979_SERIAL_MODE_TDM4,
#elif defined(TDM8)
                                   ADI_ADAU1979_SERIAL_MODE_TDM8,
#endif
                                   &Adau1979Memory,
                                   ADI_ADAU1979_MEMORY_SIZE,
                                   &phAdau1979) != 0u)
   {
      REPORT_ERROR ("ADAU1979: adi_adau1979_Open failed\n");
      /* return error */
      return 1u;
   }

   /* TWI parameters required to open/configure TWI */
   TwiConfig.TwiDevNum  = 0u;
   TwiConfig.TwiDevMemSize = ADI_TWI_MEMORY_SIZE;
   TwiConfig.pTwiDevMem    = &TwiMemory;
   TwiConfig.eTwiAddr      = ADI_ADAU1979_TWI_ADDR_11;

   if ((uint32_t)adi_adau1979_ConfigTwi(phAdau1979, &TwiConfig) != 0u)
   {
      REPORT_ERROR ("ADAU1979: adi_adau1979_ConfigTwi failed\n");
      /* return error */
      return 1u;
   }

   /* SPORT parameters required to open/configure TWI */
   SportConfig.SportDevNum    = 4u;
   SportConfig.SportDevMemSize   = ADI_SPORT_DMA_MEMORY_SIZE;
   SportConfig.pSportDevMem   = &Adau1979SportMemory;
   SportConfig.eSportChnl      = ADI_ADAU1979_SPORT_A;
   SportConfig.eSportPri       = ADI_ADAU1979_SERIAL_PORT_DSDATA1;
   SportConfig.eSportSec       = ADI_ADAU1979_SERIAL_PORT_NONE;
   SportConfig.bLsbFirst      = true;

   if ((uint32_t)adi_adau1979_ConfigSport(phAdau1979, &SportConfig) != 0u)
   {
      REPORT_ERROR ("ADAU1979: adi_adau1979_ConfigSport failed\n");
      /* return error */
      return 1u;
   }

   /* ADC is a master source of SPORT clk and FS, MCLK 25.576 MHz and PLL used MCLK */
   if ((uint32_t)adi_adau1979_ConfigPllClk(phAdau1979,
                                           LR_B_CLK_MASTER_1979,
                                           ADI_ADAU1979_MCLK_IN_FREQ_24576000HZ,
                                           ADI_ADAU1979_PLL_SOURCE_MCLK) != 0u)
   {
      REPORT_ERROR ("ADAU1979: adi_adau1979_ConfigPllClk failed\n");
      /* return error */
      return 1u;
   }

   if ((uint32_t)adi_adau1979_ConfigSerialClk(phAdau1979,
                                              BCLK_RISING_1979,
                                              LRCLK_HI_LO_1979) != 0u)
   {
      REPORT_ERROR ("ADAU1979: adi_adau1979_ConfigSerialClk failed\n");
      /* return error */
      return 1u;
   }

   if ((uint32_t)adi_adau1979_SetSampleRate(phAdau1979, ADI_ADAU1979_SAMPLE_RATE_48000HZ) != 0u)
   {
      REPORT_ERROR ("ADAU1979: adi_adau1979_SetSampleRate failed\n");
      /* return error */
      return 1u;
   }
   if ((uint32_t)adi_adau1979_SetWordWidth(phAdau1979, ADI_ADAU1979_WORD_WIDTH_24) != 0u)
   {
      REPORT_ERROR ("ADAU1979: adi_adau1979_SetWordWidth failed\n");
      /* return error */
      return 1u;
   }

   {
      Platform_Handle_t hPlatform = platform_get_handle();
      if ((uint32_t)adi_adau1979_RegisterCallback(phAdau1979,
                                                  deploy_target_step,
                                                  (void *)hPlatform) != 0u)
      {
         REPORT_ERROR ("ADAU1979: adi_adau1979_RegisterCallback failed\n");
         /* return error */
         return 1u;
      }
   }


   return Result;
}

/*
 * Opens and initializes ADAU1962A DAC Device.
 *
 * Parameters
 *  None
 *
 * Returns
 *  0 if success, other values for error
 *
 */
uint32_t Adau1962aInit(void)
{
   ADI_ADAU1962A_RESULT        eResult;
   ADI_ADAU1962A_TWI_CONFIG    TwiConfig;
   ADI_ADAU1962A_SPORT_CONFIG  SportConfig;

   /* Open ADAU1962A device instance */
   if ((eResult = adi_adau1962a_Open(0u,
#if defined(TDM4)
                                     ADI_ADAU1962A_SERIAL_MODE_TDM4,
#elif defined(TDM8)
                                     ADI_ADAU1962A_SERIAL_MODE_TDM8,
#endif
                                     &Adau1962aMemory,
                                     ADI_ADAU1962A_MEMORY_SIZE,
                                     &phAdau1962a)) != ADI_ADAU1962A_SUCCESS)
   {
      REPORT_ERROR ("ADAU1962A: Failed to open ADAU1962A device instance, Error Code: 0x%08X\n", eResult);
      /* return error */
      return 1u;
   }

   /* TWI parameters required to open/configure TWI */
   TwiConfig.TwiDevNum  = 0u;
   TwiConfig.eTwiAddr      = ADI_ADAU1962A_TWI_ADDR_04;
   TwiConfig.TwiDevMemSize = ADI_TWI_MEMORY_SIZE;
   TwiConfig.pTwiDevMem    = &TwiMemory;

   /* Configure TWI */
   if ((eResult = adi_adau1962a_ConfigTwi (phAdau1962a, &TwiConfig)) != ADI_ADAU1962A_SUCCESS)
   {
      REPORT_ERROR ("ADAU1962A: Failed to configure TWI, Error Code: 0x%08X\n", eResult);
      /* return error */
      return 1u;
   }

   /* SPORT parameters required to open/configure SPORT */
   SportConfig.SportDevNum    = 4u;
   SportConfig.eSportChnl      = ADI_ADAU1962A_SPORT_B;
   SportConfig.eSportPri       = ADI_ADAU1962A_SERIAL_PORT_DSDATA1;
   SportConfig.eSportSec       = ADI_ADAU1962A_SERIAL_PORT_NONE;
   SportConfig.SportDevMemSize   = ADI_SPORT_DMA_MEMORY_SIZE;
   SportConfig.pSportDevMem   = &Adau1962aSportMemory;

   /* Configure SPORT */
   if ((eResult = adi_adau1962a_ConfigSport (phAdau1962a, &SportConfig)) != ADI_ADAU1962A_SUCCESS)
   {
      REPORT_ERROR ("ADAU1962A: Failed to configure SPORT, Error Code: 0x%08X\n", eResult);
      /* return error */
      return 1u;
   }

   /* DAC Master Power-up */
   if ((eResult = adi_adau1962a_ConfigDacPwr (phAdau1962a,
                                              ADI_ADAU1962A_CHNL_DAC_MSTR,
                                              ADI_ADAU1962A_DAC_PWR_LOW,
                                              true)) != ADI_ADAU1962A_SUCCESS)
   {
      REPORT_ERROR ("ADAU1962A: Failed to configure DAC power, Error Code: 0x%08X\n", eResult);
      /* return error */
      return 1u;
   }

   /*
    * Configure PLL clock - DAC is clock master and drives SPORT clk and FS
    * MCLK 24.576 MHz and PLL uses MCLK
    */
   if ((eResult = adi_adau1962a_ConfigPllClk (phAdau1962a,
                                              ADAU1962A_MCLK_IN,
                                              ADI_ADAU1962A_MCLK_SEL_PLL,
                                              ADI_ADAU1962A_PLL_IN_MCLKI_XTALI)) != ADI_ADAU1962A_SUCCESS)
   {
      REPORT_ERROR ("ADAU1962A: Failed to configure PLL clock, Error Code: 0x%08X\n", eResult);
      /* return error */
      return 1u;
   }

   /*
    * Configure serial data clock
    * DAC as clock master, External BCLK, Latch on raising edge
    * LRCLK at 50% duty cycle, MSB first, Left channel at LRCLK low
    */
   if ((eResult = adi_adau1962a_ConfigSerialClk (phAdau1962a,
                                                 LR_B_CLK_MASTER_1962,
                                                 false,
                                                 BCLK_RISING_1962,
/* pulse mode - true */
                                                 true,
                                                 false,
                                                 LRCLK_HI_LO_1962)) != ADI_ADAU1962A_SUCCESS)
   {
      REPORT_ERROR ("ADAU1962A: Failed to configure serial data clock, Error Code: 0x%08X\n", eResult);
      /* return error */
      return 1u;
   }

   /* Power-up PLL */
   if ((eResult = adi_adau1962a_ConfigBlockPwr (phAdau1962a,
                                                false,
                                                true,
                                                true)) != ADI_ADAU1962A_SUCCESS)
   {
      REPORT_ERROR ("ADAU1962A: Failed to Power-up PLL, Error Code: 0x%08X\n", eResult);
      /* return error */
      return 1u;
   }

   /* Configure Sample rate */
   if ((eResult = adi_adau1962a_SetSampleRate (phAdau1962a, SAMPLE_RATE)) != ADI_ADAU1962A_SUCCESS)
   {
      REPORT_ERROR ("ADAU1962A: Failed to configure Sample rate, Error Code: 0x%08X\n", eResult);
      /* return error */
      return 1u;
   }

   /* Configure Word width */
   if ((eResult = adi_adau1962a_SetWordWidth (phAdau1962a,
                                              ADI_ADAU1962A_WORD_WIDTH_24)) != ADI_ADAU1962A_SUCCESS)
   {
      REPORT_ERROR ("ADAU1962A: Failed to configure word width, Error Code: 0x%08X\n", eResult);
      /* return error */
      return 1u;
   }

   {
      Platform_Handle_t hPlatform = platform_get_handle();
      /* Register callback */
      if ((eResult = adi_adau1962a_RegisterCallback (phAdau1962a,
                                                     deploy_target_step,
                                                     (void *)hPlatform)) != ADI_ADAU1962A_SUCCESS)
      {
         REPORT_ERROR ("ADAU1962A: Failed to register callback, Error Code: 0x%08X\n", eResult);
         /* return error */
         return 1u;
      }
   }


   return 0u;
}

/*
 * Submits ping-pong buffers to ADC and enables ADC data flow.
 *
 * Parameters
 *  None
 *
 * Returns
 *  0 if success, other values for error
 *
 */
uint32_t Adau1979SubmitBuffers(void)
{
   uint32_t Result = 0u;

   /* submit ping buffer */
   if ((uint32_t)adi_adau1979_SubmitBuffer(phAdau1979, int_ADCPingBuffer, FRAME_SIZE_IN*sizeof(int32_t)) != 0u)
   {
      /* return error */
      return 1u;
   }

   /* submit pong buffer */
   if ((uint32_t)adi_adau1979_SubmitBuffer(phAdau1979, int_ADCPongBuffer, FRAME_SIZE_IN*sizeof(int32_t)) != 0u)
   {
      /* return error */
      return 1u;
   }

   return Result;
}

/*
 * Submits ping-pong buffers to DAC and enables ADC data flow.
 *
 * Parameters
 *  None
 *
 * Returns
 *  0 if success, other values for error
 *
 */
uint32_t Adau1962aSubmitBuffers(void)
{
   uint32_t Result = 0u;

   /* submit ping buffer */
   if ((uint32_t)adi_adau1962a_SubmitBuffer(phAdau1962a, int_DACPingBuffer, FRAME_SIZE_OUT*sizeof(int32_t)) != 0u)
   {
      /* return error */
      return 1u;
   }

   /* submit pong buffer */
   if ((uint32_t)adi_adau1962a_SubmitBuffer(phAdau1962a, int_DACPongBuffer, FRAME_SIZE_OUT*sizeof(int32_t)) != 0u)
   {
      /* return error */
      return 1u;
   }

   return Result;
}

uint32_t resumbitAdcDacBuffers(void *pADC, void *pDAC)
{
  ADI_ADAU1979_RESULT     eResult1;
  ADI_ADAU1962A_RESULT    eResult2;
  uint32_t Result = 0u;

  /* re-submit the ADC buffer */
  eResult1 = adi_adau1979_SubmitBuffer(phAdau1979, pADC, FRAME_SIZE_IN*sizeof(int32_t));
  if(eResult1)
  {
     return 1u;
  }

  /* re-submit the DAC buffer */
  eResult2 = adi_adau1962a_SubmitBuffer(phAdau1962a, pDAC, FRAME_SIZE_OUT*sizeof(int32_t));
  if(eResult2)
  {
     return 1u;
  }
  return 0u;
}

#define GUL_UARTIO
#if defined(GUL_UARTIO)

#include <device.h>
#include <device_int.h>

#define BAUD_RATE     115200u

#define UARTIO_DEVICEID   4

int   UARTIO_init  (struct DevEntry *dev);
int   UARTIO_open  (const char *name, int mode);
int   UARTIO_close (int fd);
int   UARTIO_write (int fd, unsigned char * buf, int size);
int   UARTIO_read  (int fd, unsigned char * buf, int size);
long  UARTIO_seek  (int fd, long offset, int whence);
int UARTIO_remove(const char *filename);
int UARTIO_rename(const char *oldname, const char *newname);
int UARTIO_ioctl(int fd, int request, va_list varg_list);

DevEntry_Extension UARTIO_DevEntry_Extension =
{
   DEVFLAGS_BYTEADDRESSED,
   NULL,
   NULL
};

DevEntry UARTIO_DevEntry =
{
   0,
   NULL,
   UARTIO_init,
   UARTIO_open,
   UARTIO_close,
   UARTIO_write,
   UARTIO_read,
   UARTIO_seek,
   dev_not_claimed,
   dev_not_claimed,
   dev_not_claimed,
   UARTIO_remove,
   UARTIO_rename,
   UARTIO_ioctl,
   &UARTIO_DevEntry_Extension,
};

static FILE *uartiofp = 0;

/*
* https://www.downtowndougbrown.com/2013/01/microcontrollers-interrupt-safe-ring-buffers/
*/
#define UARTRX_LOG2_BUF_SIZE 8
#define UARTRX_BUF_SIZE (1<<(UARTRX_LOG2_BUF_SIZE))
#define UARTRX_BUF_MASK (UARTRX_BUF_SIZE-1)

volatile uint32_t UARTRX_ring_head;
volatile uint32_t UARTRX_ring_tail;
volatile uint8_t UARTRX_ring_data[UARTRX_BUF_SIZE];
int UARTRX_ringdata_err;

int UART_getc(void)
{
   if (UARTRX_ring_head != UARTRX_ring_tail)
   {
      int c = UARTRX_ring_data[UARTRX_ring_tail];
      UARTRX_ring_tail = (UARTRX_ring_tail + 1) & UARTRX_BUF_MASK;
      return c;
   }
   else
   {
      return -1;
   }
}

static void UART_Rx_Handler(uint32_t SID, void *pCBParam)
{
   uint8_t ch = (uint8_t)*pREG_UART0_RBR;

   uint32_t next_head = (UARTRX_ring_head + 1) & UARTRX_BUF_MASK;
   if (next_head != UARTRX_ring_tail)
   {
      /* there is room */
      UARTRX_ring_data[UARTRX_ring_head] = ch;
      UARTRX_ring_head = next_head;
   }
   else
   {
      /* no room left in the buffer */
      UARTRX_ringdata_err++;
   }
   return;
}

void oj_uartio_stop(void)
{
   fflush(uartiofp);
   fclose(uartiofp);
}

/* Default Status Value to clear the status register */
#define DEFAULT_STATUS_VALUE            (ENUM_UART_STAT_OVR_ERR|\
                                         ENUM_UART_STAT_PARITY_ERR|\
                                         ENUM_UART_STAT_FRAMING_ERR|\
                                         ENUM_UART_STAT_BREAK_INT|\
                                         ENUM_UART_STAT_TX_DONE|\
                                         ENUM_UART_STAT_ADDR_HI_STKY|\
                                         ENUM_UART_STAT_ADDR_HI|\
                                         ENUM_UART_STAT_CTS_HI_STKY)

#define SEG_L2_UNCACHED _Pragma ("section(\"seg_l2\", DOUBLEANY)" )
SEG_L2_UNCACHED char stdout_buf[256];
void oj_uartio_start(void)
{
   uint32_t sclk0_freq;
   uint32_t sclk1_freq;
   uint32_t ssysclk_freq;

   UARTRX_ring_head = 0;
   UARTRX_ring_tail = 0;
   UARTRX_ringdata_err = 0;

   adi_pwr_GetSystemFreq(0u, &ssysclk_freq, &sclk0_freq, &sclk1_freq);

   /* Reseting the UART registers */
   *pREG_UART0_STAT     = (uint32_t)(DEFAULT_STATUS_VALUE);  /* Clear it to default */
   *pREG_UART0_SCR      = 0u;                                /* Clear it to default */
   *pREG_UART0_IMSK_CLR = 0x1FFu;                            /* Disable all the interrupts by default */

   /* Reseting Control and Clock register values to default */
   *pREG_UART0_CTL      = 0x0u;                              /* Clear it by default */
   *pREG_UART0_CLK      = 0x0u;                              /* Set to default UART CLK value as (SCLK/16) */

   if (adi_int_InstallHandler(INTR_UART0_RXDMA, UART_Rx_Handler, NULL, true) != ADI_INT_SUCCESS)
   {
      REPORT_ERROR("Error: Unable to install UART Rx Handler\n");
      exit(-1);
   }

   /*****************************************************************************
    *
    *  Enable UART clock, put in UART mode, word length 8.
    *
    ****************************************************************************/
   *pREG_UART0_CTL = 0x0301;

   /* Bit Rate =        SCLK0
               -------------------
               [16^(1-EDBO)] * Div
     if EDBO is a 1, then
     Div = (SCLK0/BAUD_RATE)
   */
   *pREG_UART0_CLK = (0x80000000) | (sclk0_freq/BAUD_RATE);

   *pREG_UART0_IMSK_SET = 0x001;

   /* Create new device ID.  The device ID is arbitrary and used globally in the application.*/
   int deviceID = 0;

   UARTIO_DevEntry.DeviceID = UARTIO_DEVICEID;

   /* Insert this new device into the device table.   */
   deviceID = add_devtab_entry ( &UARTIO_DevEntry );

   if ( UARTIO_DEVICEID != deviceID )
   {
      REPORT_ERROR("Error: Unable to register new STDIO Device Driver. Try another DeviceID\n");
      exit(-1);
   }
   else
      set_default_io_device( deviceID );

   /* Redirect Stdio for printf and scanf using reopen   */
   uartiofp = fopen  ("", "a+");
   uartiofp = freopen("", "a+", stdin);
   uartiofp = freopen("", "a+", stdout);
   setvbuf(uartiofp,stdout_buf,_IOLBF, sizeof(stdout_buf));
   return;
}

/*poll the TEMT bit in LSR reg and wait until all data shifted out.*/
static void UART_waitForTransferCompletion(void)
{
   while (!(*pREG_UART0_STAT & BITM_UART_STAT_TEMT))
   {

   }

; /* wait*/
}

/*transmit character by polling the THRE bit in the LSR register.*/
void UART_putc(char c)
{
   while (!(*pREG_UART0_STAT & BITM_UART_STAT_THRE))
   {

   }

; /*wait*/
   *pREG_UART0_THR = c;
}

int   UARTIO_init  ( struct DevEntry *dev )
{
   return 0;
}

int   UARTIO_open  (const char *name, int mode)
{
   return 0;
}

int   UARTIO_close (int fd)
{
   /* driver API result code */
   UART_waitForTransferCompletion();
   *pREG_UART0_CTL = 0;
   return 0;
}

int   UARTIO_write (int fd, unsigned char * buffer, int size)
{
   /* driver API result code */
   int i = 0;
   for ( ; i < size; ++i)  UART_putc( *buffer++ );
   //UART_waitForTransferCompletion(); //not needed - UART_putc waits already and seems to block things
   return size;
}

int   UARTIO_read  (int fd, unsigned char * buffer, int size)
{
   return 0;
}

long   UARTIO_seek  (int fd, long offset, int whence)
{
   return -1;
}

int UARTIO_remove(const char *filename)
{
   return -1;
}

int UARTIO_rename(const char *oldname, const char *newname)
{
   return -1;
}

int UARTIO_ioctl(int fd, int request, va_list varg_list)
{
   return -1;
}

#else
void oj_uartio_start(void)
{
   return;
}
void oj_uartio_stop(void)
{
   return;
}
#endif /* GUL_UARTIO */

void oj_ezkit_init(void)
{
   uint32_t Result=0;
   ADI_PWR_RESULT ePwrResult;

   /* Initialize the power services */
   ePwrResult = adi_pwr_Init(0u, EZKIT_CLKIN);
   if (ePwrResult != ADI_PWR_SUCCESS)
   {
      REPORT_ERROR("Error: Power initialization failed 0x%08X\n", ePwrResult);
      return;
   }

   /* configure System Event Controller SEC and Signal Routing Unit SRU */
   adi_initComponents();

   /* Software Switch Configuration for the EZ-Board */
   ConfigSoftSwitches();

   oj_uartio_start();

   /* Initialize GPIO for ADC/DAC reset control */
   if (Result == 0u)
   {
      Result = GpioInit();
   }

   /* Initialize SPU Service */
   if (adi_spu_Init(0u, SpuMemory, NULL, NULL, &hSpu) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Error: Failed to initialize SPU service\n");
      return;
   }

   /* Make SPORT 4A to generate secure transactions */
   if (adi_spu_EnableMasterSecure(hSpu, SPORT_4A_SPU_PID, true) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Error: Failed to enable Master secure for SPORT 4A\n");
      return;
   }

   /* Make SPORT 4B to generate secure transactions */
   if (adi_spu_EnableMasterSecure(hSpu, SPORT_4B_SPU_PID, true) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Error: Failed to enable Master secure for SPORT 4B\n");
      return;
   }

   /* Make SPORT 4A DMA to generate secure transactions */
   if (adi_spu_EnableMasterSecure(hSpu, SPORT_4A_DMA10_SPU_PID, true) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Error: Failed to enable Master secure for SPORT 4A DMA\n");
      return;
   }

   /* Make SPORT 4B DMA to generate secure transactions */
   if (adi_spu_EnableMasterSecure(hSpu, SPORT_4B_DMA11_SPU_PID, true) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Error: Failed to enable Master secure for SPORT 4B DMA\n");
      return;
   }

   /* Initialize ADAU1979 */
   if (Result == 0u)
   {
      Result = Adau1979Init();
   }

   /* Initialize ADAU1962A */
   if (Result == 0u)
   {
      Result = Adau1962aInit();
   }

   /* Submit ADC buffers */
   if (Result == 0u)
   {
      Result = Adau1979SubmitBuffers();
   }

   /* Submit DAC buffers */
   if (Result == 0u)
   {
      Result = Adau1962aSubmitBuffers();
   }
}

void oj_audio_start(void)
{
   uint32_t Result=0;

   /* Enable data flow for only the ADC and DAC */
   if (Result==0u)
   {
      Result=adi_adau1962a_Enable(phAdau1962a, true);
   }

   if (Result==0u)
   {
      Result=adi_adau1979_Enable(phAdau1979, true);
   }
}

