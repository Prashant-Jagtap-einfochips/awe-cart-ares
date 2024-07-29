/*****************************************************************************
 * Audio_Loopback_TDM.c
 *****************************************************************************/
#include <sys/platform.h>
#include <sys/adi_core.h>
#include <services/int/adi_int.h>
#include <services/int/adi_sec.h>
#include <drivers/sport/adi_sport.h>
#include <services/spu/adi_spu.h>
#include <drivers/twi/adi_twi_2156x.h>
#include <services/pwr/adi_pwr.h>
#include <cdef21569.h>
#include <interrupt.h>   
#include "adi_initialize.h"
#include "ADAU_1962Common.h"
#include "ADAU_1979Common.h"
#include "sru21569.h"

#include "gul_ezkit.h"
#include "oj_platform.h"

#define SPORT_DEVICE_4A       4u       /* SPORT device number */
#define SPORT_DEVICE_4B       4u       /* SPORT device number */
#define TWIDEVNUM             2u         /* TWI device number */

#define BITRATE               (100u)      /* kHz */
#define DUTYCYCLE             (50u)       /* percent */
#define PRESCALEVALUE         (12u)       /* fSCLK/10MHz (112.5 sclk0_0) */
#define BUFFER_SIZE           (8u)

#define TARGETADDR            (0x38u)     /* hardware address */
#define TARGETADDR_1962       (0x04u)     /* hardware address of adau1962 DAC */
#define TARGETADDR_1979       (0x11u)     /* hardware address of adau1979 ADC */

#define SPORT_4A_SPU          (57u)
#define SPORT_4B_SPU          (58u)

#define DMA_NUM_DESC           (2u)

#define SUCCESS   0
#define FAILED   -1

#define MHZTOHZ               (1000000u)
#define EZKIT_CLKIN           (25u  * MHZTOHZ)

#define REPORT_ERROR           platform_printf

#define CHECK_RESULT(eResult) \
        if(eResult != 0)\
      {\
         return (1);\
        }


#define MAX_INTERRUPT_THREAD                     8
#define SOFT_INTERRUPT_PRIORITY_BASE             80

typedef enum InterruptId__ {
	SOFT_INTERRUPT_ID0 = INTR_SOFT0,
	SOFT_INTERRUPT_ID1,
	SOFT_INTERRUPT_ID2,
	SOFT_INTERRUPT_ID3,
	SOFT_INTERRUPT_ID4,
	SOFT_INTERRUPT_ID5,
	SOFT_INTERRUPT_ID6,
	SOFT_INTERRUPT_ID7
} InterruptId;



extern void ConfigSoftSwitches_ADC_DAC(void);
extern void ConfigSoftSwitches_ADAU_Reset(void);
extern void deploy_target_step( void     *pAppHandle,
                                uint32_t  nEvent,
                                void     *pArg);

static int ADAU_1962_Pllinit(ADI_TWI_HANDLE hTwiDevice);
static int ADAU_1979_Pllinit(ADI_TWI_HANDLE hTwiDevice);
static void Switch_Configurator(void);
static int ADAU_1962_init(ADI_TWI_HANDLE hTwiDevice);
static int ADAU_1979_init(ADI_TWI_HANDLE hTwiDevice);
static void SRU_Init(void);
static int Sport_Init(void);
static int SPU_init(void);
static void PrepareDescriptors (void);
static ADI_TWI_HANDLE Init_TWI(uint32_t *result);
static int Stop_TWI(ADI_TWI_HANDLE);

struct Config_Table
{
   short Reg_Add;
   char  Value;
};

struct Config_Table Config_array_DAC[28] = {
   {     ADAU1962_PDN_CTRL_1,  0x00},
   {     ADAU1962_PDN_CTRL_2,  0xff},
   {     ADAU1962_PDN_CTRL_3,  0x0f},
   {     ADAU1962_DAC_CTRL0,   0x01},
   {     ADAU1962_DAC_CTRL1,   0x01},
   {     ADAU1962_DAC_CTRL2,   0x00},
   {     ADAU1962_DAC_MUTE1,   0x0},
   {     ADAU1962_DAC_MUTE2,   0x00},
   {     ADAU1962_MSTR_VOL,    0x00},
   {     ADAU1962_DAC1_VOL,    0x00},
   {     ADAU1962_DAC2_VOL,    0x00},
   {     ADAU1962_DAC3_VOL,    0x00},
   {     ADAU1962_DAC4_VOL,    0x00},
   {     ADAU1962_DAC5_VOL,    0x00},
   {     ADAU1962_DAC6_VOL,    0x00},
   {     ADAU1962_DAC7_VOL,    0x00},
   {     ADAU1962_DAC8_VOL,    0x00},
   {     ADAU1962_DAC9_VOL,    0x00},
   {     ADAU1962_DAC10_VOL,   0x00},
   {     ADAU1962_DAC11_VOL,   0x00},
   {     ADAU1962_DAC12_VOL,   0x00},
   {     ADAU1962_PAD_STRGTH,  0x00},
   {     ADAU1962_DAC_PWR1,    0xaa},
   {     ADAU1962_DAC_PWR2,    0xaa},
   {     ADAU1962_DAC_PWR3,    0xaa},
   {     ADAU1962_PDN_CTRL_2,  0x00},
   {     ADAU1962_PDN_CTRL_3,  0x00},
   {     ADAU1962_DAC_CTRL0,   0x18}
};

struct Config_Table Config_array_ADC[16] = {
   {ADAU1979_REG_BOOST,           0x00},
   {ADAU1979_REG_MICBIAS,         0x00},
   {ADAU1979_REG_BLOCK_POWER_SAI, 0x30},
   {ADAU1979_REG_SAI_CTRL0,       0x1B},/*I2S 48kHz*/
   {ADAU1979_REG_SAI_CTRL1,       0x08},
   {ADAU1979_REG_CMAP12,          0x01},
   {ADAU1979_REG_CMAP34,          0x23},
   {ADAU1979_REG_SAI_OVERTEMP,    0xf0},
   {ADAU1979_REG_POST_ADC_GAIN1,  0xA0},
   {ADAU1979_REG_POST_ADC_GAIN2,  0xA0},
   {ADAU1979_REG_POST_ADC_GAIN3,  0xA0},
   {ADAU1979_REG_POST_ADC_GAIN4,  0xA0},
   {ADAU1979_REG_ADC_CLIP,        0x00},
   {ADAU1979_REG_DC_HPF_CAL,      0x00},
   {ADAU1979_REG_BLOCK_POWER_SAI, 0x3f},
   {ADAU1979_REG_MISC_CONTROL,    0x00}
};

/* Peripheral DMA descriptor list required for SPORT */
ADI_PDMA_DESC_LIST iDESC_LIST_1_SP4A;
ADI_PDMA_DESC_LIST iDESC_LIST_2_SP4A;
ADI_PDMA_DESC_LIST iSRC_LIST_1_SP4B;
ADI_PDMA_DESC_LIST iSRC_LIST_2_SP4B;

/* Memory required for SPORT */
static uint8_t SPORTMemory4A[ADI_SPORT_MEMORY_SIZE];
static uint8_t SPORTMemory4B[ADI_SPORT_MEMORY_SIZE];

/* Memory required for TWI */
static  uint8_t TwideviceMemory[ADI_TWI_MEMORY_SIZE];

/* Memory required for the SPU operation */
static uint8_t SpuMemory[ADI_SPU_MEMORY_SIZE];

/* sport buffers for ADC/DAC data */
int32_t int_SP0ABuffer1[FRAME_SIZE_OUT];
int32_t int_SP0ABuffer2[FRAME_SIZE_OUT];
int32_t int_SP0ABuffer4[FRAME_SIZE_IN];
int32_t int_SP0ABuffer5[FRAME_SIZE_IN];

/* SPU handle */
ADI_SPU_HANDLE hSpu;

/*
 * Prepares descriptors for DMA transfers
 *
 * Parameters
 *  None
 *
 * Returns
 *  None
 *
 */
static void PrepareDescriptors (void)
{

   iDESC_LIST_1_SP4A.pStartAddr  = (int *)int_SP0ABuffer1;
   iDESC_LIST_1_SP4A.Config      = ENUM_DMA_CFG_XCNT_INT ;
   iDESC_LIST_1_SP4A.XCount      = FRAME_SIZE_OUT;
   iDESC_LIST_1_SP4A.XModify     = 4;
   iDESC_LIST_1_SP4A.YCount      = 0;
   iDESC_LIST_1_SP4A.YModify     = 0;
   iDESC_LIST_1_SP4A.pNxtDscp    = &iDESC_LIST_2_SP4A;

   iDESC_LIST_2_SP4A.pStartAddr  = (int *)int_SP0ABuffer2;
   iDESC_LIST_2_SP4A.Config      = ENUM_DMA_CFG_XCNT_INT ;
   iDESC_LIST_2_SP4A.XCount      = FRAME_SIZE_OUT;
   iDESC_LIST_2_SP4A.XModify     = 4;
   iDESC_LIST_2_SP4A.YCount      = 0;
   iDESC_LIST_2_SP4A.YModify     = 0;
   iDESC_LIST_2_SP4A.pNxtDscp    = &iDESC_LIST_1_SP4A;

   iSRC_LIST_1_SP4B.pStartAddr   = (int *)int_SP0ABuffer4;
   iSRC_LIST_1_SP4B.Config       = ENUM_DMA_CFG_XCNT_INT ;
   iSRC_LIST_1_SP4B.XCount       = FRAME_SIZE_IN;
   iSRC_LIST_1_SP4B.XModify      = 4;
   iSRC_LIST_1_SP4B.YCount       = 0;
   iSRC_LIST_1_SP4B.YModify      = 0;
   iSRC_LIST_1_SP4B.pNxtDscp     = &iSRC_LIST_2_SP4B;

   iSRC_LIST_2_SP4B.pStartAddr   = (int *)int_SP0ABuffer5;
   iSRC_LIST_2_SP4B.Config       = ENUM_DMA_CFG_XCNT_INT;
   iSRC_LIST_2_SP4B.XCount       = FRAME_SIZE_IN;
   iSRC_LIST_2_SP4B.XModify      = 4;
   iSRC_LIST_2_SP4B.YCount       = 0;
   iSRC_LIST_2_SP4B.YModify      = 0;
   iSRC_LIST_2_SP4B.pNxtDscp     = &iSRC_LIST_1_SP4B;
}

#define CCES_w1907312_BUILD
static int Sport_Init(void)
{
   /* SPORT return code */
   ADI_SPORT_RESULT    eResult;

   /* SPORT Handle */
   ADI_SPORT_HANDLE hSPORTDev4ATx;//TX
   ADI_SPORT_HANDLE hSPORTDev4BRx;//RX

   Platform_Handle_t hPlatform = platform_get_handle();

   /* Open the SPORT Device 4A */
   eResult = adi_sport_Open(SPORT_DEVICE_4A,ADI_HALF_SPORT_A,ADI_SPORT_DIR_TX, ADI_SPORT_MC_MODE, SPORTMemory4A,ADI_SPORT_MEMORY_SIZE,&hSPORTDev4ATx);
   CHECK_RESULT(eResult);
   /* Open the SPORT Device 4B*/
   eResult = adi_sport_Open(SPORT_DEVICE_4B,ADI_HALF_SPORT_B,ADI_SPORT_DIR_RX, ADI_SPORT_MC_MODE, SPORTMemory4B,ADI_SPORT_MEMORY_SIZE,&hSPORTDev4BRx);
   CHECK_RESULT(eResult);

   /* Configure the data,clock,frame sync and MCTL of SPORT Device 4A*/
   eResult = adi_sport_ConfigData(hSPORTDev4ATx,ADI_SPORT_DTYPE_SIGN_FILL,31,false,false,false);
   CHECK_RESULT(eResult);
   eResult = adi_sport_ConfigClock(hSPORTDev4ATx,32,false,false,false);
   CHECK_RESULT(eResult);

   #if defined(CCES_w1907312_BUILD)
   eResult = adi_sport_ConfigFrameSync(hSPORTDev4ATx,31,false,false,false,true,false,false);
   #else
	eResult = adi_sport_ConfigFrameSync(hSPORTDev4ATx,31,false,false,false,false,false,false);
   #endif
   CHECK_RESULT(eResult);
   eResult = adi_sport_ConfigMC(hSPORTDev4ATx,1u,7u,0u,true);
   CHECK_RESULT(eResult);
   eResult = adi_sport_SelectChannel(hSPORTDev4ATx,0u,7u);
   CHECK_RESULT(eResult);

   /* Configure the data,clock,frame sync and MCTL of SPORT Device 4B*/
   eResult = adi_sport_ConfigData(hSPORTDev4BRx,ADI_SPORT_DTYPE_SIGN_FILL,31,false,false,false);
   CHECK_RESULT(eResult);
   eResult = adi_sport_ConfigClock(hSPORTDev4BRx,32,false,false,false);
   CHECK_RESULT(eResult);
   #if defined(CCES_w1907312_BUILD)
   eResult = adi_sport_ConfigFrameSync(hSPORTDev4BRx,31,false,false,false,true,false,false);
   #else
	eResult = adi_sport_ConfigFrameSync(hSPORTDev4BRx,31,false,false,false,false,false,false);
   #endif
   CHECK_RESULT(eResult);
   eResult = adi_sport_ConfigMC(hSPORTDev4BRx,1u,7u,0u,true);
   CHECK_RESULT(eResult);
   eResult = adi_sport_SelectChannel(hSPORTDev4BRx,0u,3u);
   CHECK_RESULT(eResult);


   /* Register SPORT Callback function */
   eResult = adi_sport_RegisterCallback(hSPORTDev4ATx,/*SPORTCallback*/deploy_target_step,(void *)hPlatform);
   CHECK_RESULT(eResult);
#if 0
   /* Register SPORT Callback function */
   eResult = adi_sport_RegisterCallback(hSPORTDev4BRx,/*SPORTCallback*/deploy_target_step,(void *)hPlatform);
   CHECK_RESULT(eResult);
#endif

   /* Prepare descriptors */
   PrepareDescriptors();

   /* Submit the first buffer for Rx.  */
   eResult = adi_sport_DMATransfer(hSPORTDev4BRx,&iSRC_LIST_1_SP4B,(DMA_NUM_DESC),ADI_PDMA_DESCRIPTOR_LIST, ADI_SPORT_CHANNEL_PRIM);
   CHECK_RESULT(eResult);
   /* Submit the first buffer for Tx.  */
   eResult = adi_sport_DMATransfer(hSPORTDev4ATx,&iDESC_LIST_1_SP4A,(DMA_NUM_DESC),ADI_PDMA_DESCRIPTOR_LIST, ADI_SPORT_CHANNEL_PRIM);
   CHECK_RESULT(eResult);

   /*Enable the Sport Device 4B */
   eResult = adi_sport_Enable(hSPORTDev4BRx,true);
   CHECK_RESULT(eResult);
   /*Enable the Sport Device 4A */
   eResult = adi_sport_Enable(hSPORTDev4ATx,true);
   CHECK_RESULT(eResult);

   return eResult;
}

/*
 * Prepares Switch configuration.
 *
 * Parameters
 *  None
 *
 * Returns
 *  None
 *
 */
static void Switch_Configurator(void)
{
   int delay11=0xffff;

   /* Software Switch Configuration for Enabling ADC-DAC */
   ConfigSoftSwitches_ADC_DAC();

   while (delay11--)
   {
      asm("nop;");
   }

   /* Software Switch Configuration for Re-Setting ADC-DAC  */
   ConfigSoftSwitches_ADAU_Reset();


   /* wait for Codec to up */
   delay11=0xffff;
   while (delay11--)
   {
      asm("nop;");
   }
}



/*
 * Prepares SRU configuration.
 *
 * Parameters
 *  None
 *
 * Returns
 *  None
 *
 */
static void SRU_Init(void)
{
   *pREG_PADS0_DAI0_IE=0x1ffffe;
   *pREG_PADS0_DAI1_IE=0x1ffffe;

   SRU2(LOW,DAI1_PBEN05_I);

   SRU2(DAI1_PB05_O,SPT4_ACLK_I); /*DAC clock to SPORT 4A*/
   SRU2(DAI1_PB05_O,SPT4_BCLK_I); /*DAC clock to SPORT 4B*/

   SRU2(DAI1_PB04_O,SPT4_AFS_I);  /*DAC FS to SPORT 4A*/
   SRU2(DAI1_PB04_O,SPT4_BFS_I);  /*DAC FS to SPORT 4B*/
   SRU2(LOW,DAI1_PBEN04_I);

   SRU2(SPT4_AD0_O,DAI1_PB01_I); /* SPORT 4A to DAC*/
   SRU2(HIGH,DAI1_PBEN01_I);

   SRU2(DAI1_PB05_O,DAI1_PB12_I);  /*DAC clock to ADC */
   SRU2(HIGH,DAI1_PBEN12_I);

   SRU2(DAI1_PB04_O,DAI1_PB20_I);  /*DAC FS to ADC */
   SRU2(HIGH,DAI1_PBEN20_I);

   SRU2(DAI1_PB06_O,SPT4_BD0_I);
   SRU2(LOW,DAI1_PBEN06_I);

}

/*
 * Prepares SPU configuration.
 *
 * Parameters
 *  None
 *
 * Returns
 *  None
 *
 */
int SPU_init(void)
{
   if (adi_spu_Init(0, SpuMemory, NULL, NULL, &hSpu) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Failed to initialize SPU service\n");
      return FAILED;
   }

   /* Make SPORT 0A to generate secure transactions */
   if (adi_spu_EnableMasterSecure(hSpu, SPORT_4A_SPU, true) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Failed to enable Master secure for SPORT0A\n");
      return FAILED;
   }

   /* Make SPORT 0B to generate secure transactions */
   if (adi_spu_EnableMasterSecure(hSpu, SPORT_4B_SPU, true) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Failed to enable Master secure for SPORT0B\n");
      return FAILED;
   }

   return SUCCESS;
}

void Write_TWI_8bit_Reg(ADI_TWI_HANDLE hTwiDevice, unsigned char Reg_ID, unsigned char Tx_Data)
{
   /* Dev buffer for configuring ADC-DAC through TWI*/
   uint8_t devBuffer[BUFFER_SIZE];

   devBuffer[0] = Reg_ID;
   devBuffer[1] = Tx_Data;
   adi_twi_Write(hTwiDevice, devBuffer, 2u, false);
}

unsigned char Read_TWI_8bit_Reg(ADI_TWI_HANDLE hTwiDevice, unsigned char Reg_ID)
{
   ADI_TWI_RESULT eResult;
   unsigned char Rx_Data;

   /* Dev buffer for configuring ADC-DAC through TWI*/
   uint8_t devBuffer[BUFFER_SIZE];

   /* write register address */
   devBuffer[0] = Reg_ID;
   eResult = adi_twi_Write(hTwiDevice, devBuffer, 1u, true);
   if (eResult!=ADI_TWI_SUCCESS)
   {
      REPORT_ERROR("TWI write failed 0x%08X\n", eResult);
   }

   /* read register value */
   eResult = adi_twi_Read(hTwiDevice, &Rx_Data, 1u, false);
   if (eResult!=ADI_TWI_SUCCESS)
   {
      REPORT_ERROR("TWI Read failed 0x%08X\n", eResult);
   }

   return Rx_Data;
}

static ADI_TWI_HANDLE Init_TWI(uint32_t *result)
{

   ADI_TWI_RESULT eResult;
   ADI_TWI_HANDLE hTwiDevice;

   eResult = adi_twi_Open(TWIDEVNUM, ADI_TWI_MASTER, &TwideviceMemory[0],
                          ADI_TWI_MEMORY_SIZE, &hTwiDevice);
   if (eResult!=ADI_TWI_SUCCESS)
   {
      REPORT_ERROR("TWI Open failed 0x%08X\n", eResult);
   }

   eResult = adi_twi_SetPrescale(hTwiDevice, PRESCALEVALUE);
   if (eResult!=ADI_TWI_SUCCESS)
   {
      REPORT_ERROR("TWI Set Prescale failed 0x%08X\n", eResult);
   }

   eResult = adi_twi_SetBitRate(hTwiDevice, BITRATE);
   if (eResult!=ADI_TWI_SUCCESS)
   {
      REPORT_ERROR("TWI Set Bitrate failed 0x%08X\n", eResult);
   }


   eResult = adi_twi_SetDutyCycle(hTwiDevice, DUTYCYCLE);
   if (eResult!=ADI_TWI_SUCCESS)
   {
      REPORT_ERROR("TWI Set Duty cycle failed 0x%08X\n", eResult);
   }

   eResult = adi_twi_SetHardwareAddress(hTwiDevice, TARGETADDR);
   if (eResult!=ADI_TWI_SUCCESS)
   {
      REPORT_ERROR("TWI Set Hw address failed 0x%08X\n", eResult);
   }

   *result = (uint32_t)eResult;

   return hTwiDevice;
}

static int Stop_TWI(ADI_TWI_HANDLE hTwiDevice)
{
   ADI_TWI_RESULT eResult;

   eResult = adi_twi_Close(hTwiDevice);
   CHECK_RESULT(eResult);

   return eResult;
}

static int ADAU_1962_init(ADI_TWI_HANDLE hTwiDevice)
{
   int i;
   char Config_read_DAC[28];
   ADAU_1962_Pllinit(hTwiDevice);
   for (i=0;i<28;i++)
   {
      /* write value */
      Write_TWI_8bit_Reg(hTwiDevice, Config_array_DAC[i].Reg_Add,Config_array_DAC[i].Value);
      Config_read_DAC[i]=Read_TWI_8bit_Reg(hTwiDevice, Config_array_DAC[i].Reg_Add);
      if (Config_array_DAC[i].Value!= Config_read_DAC[i])
      {
         REPORT_ERROR("Configuring ADAU_1962 failed\n");
         return FAILED;
      }
   }

   return SUCCESS;
}

static int ADAU_1962_Pllinit(ADI_TWI_HANDLE hTwiDevice)
{
   int status,delay1=0xffff;

   ADI_TWI_RESULT eResult;

   eResult = adi_twi_SetHardwareAddress(hTwiDevice, TARGETADDR_1962);
   if (eResult!=ADI_TWI_SUCCESS)
   {
      REPORT_ERROR("TWI Set Hw address failed 0x%08X\n", eResult);
   }

   Write_TWI_8bit_Reg(hTwiDevice, ADAU1962_PLL_CTL_CTRL0,0x01);
   while (delay1--)
   {
      asm("nop;");
   }

   Write_TWI_8bit_Reg(hTwiDevice, ADAU1962_PLL_CTL_CTRL0,0x05);

   delay1=0xffff;
   while (delay1--)
   {
      asm("nop;");
   }

   Write_TWI_8bit_Reg(hTwiDevice, ADAU1962_PLL_CTL_CTRL1,0x22);
   delay1=0xffff;
   while (delay1--)
   {
      asm("nop;");
   }
   status=Read_TWI_8bit_Reg(hTwiDevice, ADAU1962_PLL_CTL_CTRL1);
   while (!((status & 0x4)>>2)  )
   {
      status=Read_TWI_8bit_Reg(hTwiDevice, ADAU1962_PLL_CTL_CTRL1);
   }

   return eResult;
}

static int ADAU_1979_init(ADI_TWI_HANDLE hTwiDevice)
{
   int i;
   char Config_read_ADC[16];

   ADAU_1979_Pllinit(hTwiDevice);

   for (i=0;i<16;i++)
   {
      Write_TWI_8bit_Reg(hTwiDevice, Config_array_ADC[i].Reg_Add,Config_array_ADC[i].Value);
      Config_read_ADC[i]=Read_TWI_8bit_Reg(hTwiDevice, Config_array_ADC[i].Reg_Add);
      if (Config_array_ADC[i].Value!= Config_read_ADC[i])
      {
         REPORT_ERROR("Configuring ADAU_1979 failed\n");
         return FAILED;
      }
   }

   return SUCCESS;
}

static int ADAU_1979_Pllinit(ADI_TWI_HANDLE hTwiDevice)
{

   int status,delay1=0xffff;

   ADI_TWI_RESULT eResult;

   eResult = adi_twi_SetHardwareAddress(hTwiDevice, TARGETADDR_1979);
   if (eResult!=ADI_TWI_SUCCESS)
   {
      REPORT_ERROR("TWI Set Hw address failed 0x%08X\n", eResult);
   }

   Write_TWI_8bit_Reg(hTwiDevice, ADAU1979_REG_POWER,0x01);
   Write_TWI_8bit_Reg(hTwiDevice, ADAU1979_REG_PLL,0x03);
   status=Read_TWI_8bit_Reg(hTwiDevice, ADAU1979_REG_PLL);
   while (delay1--)
   {
      asm("nop;");
   }
   while (!((status & 0x80)>>7)  )
   {
      status=Read_TWI_8bit_Reg(hTwiDevice, ADAU1979_REG_PLL);
      asm("nop;");
   }

   return eResult;

}

#define PFB_DPORT 1
void enable_prefetch(void)
{
#if defined(PFB_DPORT_IPORT_BOTH) // code and data
   *pREG_CMMR0_SYSCTL |= BITM_CMMR_SYSCTL_IPORT_PFB_EN | BITM_CMMR_SYSCTL_DPORT_PFB_EN;
#elif defined(PFB_DPORT)  // data only
   *pREG_CMMR0_SYSCTL |= BITM_CMMR_SYSCTL_DPORT_PFB_EN;
#elif defined(PFB_IPORT)  // code only
   *pREG_CMMR0_SYSCTL |= BITM_CMMR_SYSCTL_IPORT_PFB_EN;
#else
   asm("nop;");
#endif

}

#define BAUD_RATE     115200u
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

int UART_getc(void) {
    if (UARTRX_ring_head != UARTRX_ring_tail) {
        int c = UARTRX_ring_data[UARTRX_ring_tail];
        UARTRX_ring_tail = (UARTRX_ring_tail + 1) & UARTRX_BUF_MASK;
        return c;
    } else {
        return -1;
    }
}

static void UART_Rx_Handler(uint32_t SID, void *pCBParam)
{
   uint8_t ch = (uint8_t)*pREG_UART0_RBR;

   uint32_t next_head = (UARTRX_ring_head + 1) & UARTRX_BUF_MASK;
   if (next_head != UARTRX_ring_tail) {
      /* there is room */
      UARTRX_ring_data[UARTRX_ring_head] = ch;
      UARTRX_ring_head = next_head;
   } else {
      /* no room left in the buffer */
      UARTRX_ringdata_err++;
   }
   return;
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

void gul_uartio_start(void)
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

   if(adi_int_InstallHandler(INTR_UART0_RXDMA, UART_Rx_Handler, NULL, true) != ADI_INT_SUCCESS)
   {
      REPORT_ERROR("Unable to install UART Rx Handler\n");
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

   return;
}

/*poll the TEMT bit in LSR reg and wait until all data shifted out.*/
static void UART_waitForTransferCompletion(void)
{
  while (!(*pREG_UART0_STAT & BITM_UART_STAT_TEMT))
  {

  }; /* wait*/
}

/*transmit character by polling the THRE bit in the LSR register.*/
void UART_putc(char c)
{
  while (!(*pREG_UART0_STAT & BITM_UART_STAT_THRE))
  {

   }; /*wait*/
   *pREG_UART0_THR = c;
}

int   UARTIO_close (void)
{
   /* driver API result code */
   UART_waitForTransferCompletion();
   *pREG_UART0_CTL = 0;
   return 0;
}

void UARTIO_write (char * buffer, int size)
{
   int i;
   for (i = 0; i < size; ++i)  UART_putc( *buffer++ );
}

void oj_ezkit_init(void)
{
   uint32_t Result=0;
   ADI_PWR_RESULT ePwrResult;

   /* Initialize the power services */
   ePwrResult = adi_pwr_Init(0u, EZKIT_CLKIN);
   if(ePwrResult != ADI_PWR_SUCCESS)
   {
      REPORT_ERROR("Power initialization failed 0x%08X\n", ePwrResult);
      return;
   }

   enable_prefetch();

   adi_initComponents();

   gul_uartio_start();

   /* SPU initialization */
   if (Result==0u)
   {
      Result=SPU_init();
   }
}

void oj_audio_start(void)
{
   uint32_t Result=0;

   /* Switch Configuration */
   Switch_Configurator();

   /* SRU Configuration */
   SRU_Init();

   ADI_TWI_HANDLE hTwiDevice;

   /* TWI Initialization */
   hTwiDevice=Init_TWI(&Result);

   /* ADAU1962 Initialization */
   if (Result==0u)
   {
      Result=ADAU_1962_init(hTwiDevice);
   }

   /* ADAU1979 Initialization */
   if (Result==0u)
   {
      Result=ADAU_1979_init(hTwiDevice);
   }

   /* Close TWI */
   if (Result==0u)
   {
      Result=Stop_TWI(hTwiDevice);
   }

   /* SPORT Initialization */
   if (Result==0u)
   {
      Result=Sport_Init();
   }
}

