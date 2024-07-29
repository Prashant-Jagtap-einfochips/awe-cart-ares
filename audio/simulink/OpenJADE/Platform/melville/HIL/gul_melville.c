/*****************************************************************************
 * Audio_Loopback_TDM.c
 *****************************************************************************/
#include <sys/platform.h>
#include <sys/adi_core.h>
#include <services/int/adi_int.h>
#include <services/int/adi_sec.h>
#include <drivers/sport/adi_sport.h>
#include <services/spu/adi_spu.h>
#include <services/pwr/adi_pwr.h>
#include <cdef21569.h>
#include "adi_initialize.h"
#include "sru_init.h"
#include "pcg_init.h"
#include "gul_melville.h"
#include "oj_platform.h"
#include "2156x_InitPreload_code.h"

#define SPORT_DEVICE_4A       4u       /* SPORT device number */
#define SPORT_DEVICE_0A       0u       /* SPORT device number */

#define SPORT_0A_SPU          49
#define SPORT_4A_SPU          57

#define UART1_SPU_PID         (30u)
#define UART1_TxDMA_SPU_PID   (68u)


#define DMA_NUM_DESC          2u

#define SUCCESS   0
#define FAILED   -1

#define BAUD_RATE             (115200u)

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
extern void deploy_target_step( void     *pAppHandle,
                                uint32_t  nEvent,
                                void     *pArg);
static int Sport_Init(void);
static int Sport_Stop(void);
static int SPU_init(void);
static void PrepareDescriptors (void);

/* Peripheral DMA descriptor list required for SPORT */
ADI_PDMA_DESC_LIST iDESC_LIST_1_SP4A;
ADI_PDMA_DESC_LIST iDESC_LIST_2_SP4A;
ADI_PDMA_DESC_LIST iSRC_LIST_1_SP0A;
ADI_PDMA_DESC_LIST iSRC_LIST_2_SP0A;

/* Memory required for SPORT */
static uint8_t SPORTMemory4A[ADI_SPORT_MEMORY_SIZE];
static uint8_t SPORTMemory0A[ADI_SPORT_MEMORY_SIZE];

/* SPORT Handle */
static ADI_SPORT_HANDLE hSPORTDev4ATx;//TX
static ADI_SPORT_HANDLE hSPORTDev0ARx;//RX

/* SPU handle */
ADI_SPU_HANDLE hSpu;

/* Memory required for the SPU operation */
static uint8_t SpuMemory[ADI_SPU_MEMORY_SIZE];

/* sport buffers for ADC/DAC data */
int32_t sport_TxBuffer1[FRAME_SIZE_OUT];
int32_t sport_TxBuffer2[FRAME_SIZE_OUT];
int32_t sport_RxBuffer1[FRAME_SIZE_IN];
int32_t sport_RxBuffer2[FRAME_SIZE_IN];

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

   iDESC_LIST_1_SP4A.pStartAddr  = (int *)sport_TxBuffer1;
   iDESC_LIST_1_SP4A.Config      = ENUM_DMA_CFG_XCNT_INT ;
   iDESC_LIST_1_SP4A.XCount      = FRAME_SIZE_OUT;
   iDESC_LIST_1_SP4A.XModify     = 4;
   iDESC_LIST_1_SP4A.YCount      = 0;
   iDESC_LIST_1_SP4A.YModify     = 0;
   iDESC_LIST_1_SP4A.pNxtDscp    = &iDESC_LIST_2_SP4A;

   iDESC_LIST_2_SP4A.pStartAddr  = (int *)sport_TxBuffer2;
   iDESC_LIST_2_SP4A.Config      = ENUM_DMA_CFG_XCNT_INT ;
   iDESC_LIST_2_SP4A.XCount      = FRAME_SIZE_OUT;
   iDESC_LIST_2_SP4A.XModify     = 4;
   iDESC_LIST_2_SP4A.YCount      = 0;
   iDESC_LIST_2_SP4A.YModify     = 0;
   iDESC_LIST_2_SP4A.pNxtDscp    = &iDESC_LIST_1_SP4A;

   iSRC_LIST_1_SP0A.pStartAddr   = (int *)sport_RxBuffer1;
   iSRC_LIST_1_SP0A.Config       = ENUM_DMA_CFG_XCNT_INT ;
   iSRC_LIST_1_SP0A.XCount       = FRAME_SIZE_IN;
   iSRC_LIST_1_SP0A.XModify      = 4;
   iSRC_LIST_1_SP0A.YCount       = 0;
   iSRC_LIST_1_SP0A.YModify      = 0;
   iSRC_LIST_1_SP0A.pNxtDscp     = &iSRC_LIST_2_SP0A;

   iSRC_LIST_2_SP0A.pStartAddr   = (int *)sport_RxBuffer2;
   iSRC_LIST_2_SP0A.Config       = ENUM_DMA_CFG_XCNT_INT;
   iSRC_LIST_2_SP0A.XCount       = FRAME_SIZE_IN;
   iSRC_LIST_2_SP0A.XModify      = 4;
   iSRC_LIST_2_SP0A.YCount       = 0;
   iSRC_LIST_2_SP0A.YModify      = 0;
   iSRC_LIST_2_SP0A.pNxtDscp     = &iSRC_LIST_1_SP0A;
}

static int Sport_Init()
{
   /* SPORT return code */
   ADI_SPORT_RESULT    eResult;

   Platform_Handle_t hPlatform = platform_get_handle();

   /* Open the SPORT Device 4A */
   eResult = adi_sport_Open(SPORT_DEVICE_4A,ADI_HALF_SPORT_A,ADI_SPORT_DIR_TX, ADI_SPORT_MC_MODE, SPORTMemory4A,ADI_SPORT_MEMORY_SIZE,&hSPORTDev4ATx);
   CHECK_RESULT(eResult);
   /* Open the SPORT Device 4B*/
   eResult = adi_sport_Open(SPORT_DEVICE_0A,ADI_HALF_SPORT_A,ADI_SPORT_DIR_RX, ADI_SPORT_MC_MODE, SPORTMemory0A,ADI_SPORT_MEMORY_SIZE,&hSPORTDev0ARx);
   CHECK_RESULT(eResult);

   /* Configure the data,clock,frame sync and MCTL of SPORT Device 4A*/
   eResult = adi_sport_ConfigData(hSPORTDev4ATx,ADI_SPORT_DTYPE_SIGN_FILL,31,false,false,false);
   CHECK_RESULT(eResult);
   eResult = adi_sport_ConfigClock(hSPORTDev4ATx,32,false,false,false);
   CHECK_RESULT(eResult);

   eResult = adi_sport_ConfigFrameSync(hSPORTDev4ATx,31,false,false,false,true,false,false);
   CHECK_RESULT(eResult);
   eResult = adi_sport_ConfigMC(hSPORTDev4ATx,1u,7u,0u,true);
   CHECK_RESULT(eResult);
   eResult = adi_sport_SelectChannel(hSPORTDev4ATx,0u,7u);
   CHECK_RESULT(eResult);

   /* Configure the data,clock,frame sync and MCTL of SPORT Device 4B*/
   eResult = adi_sport_ConfigData(hSPORTDev0ARx,ADI_SPORT_DTYPE_SIGN_FILL,31,false,false,false);
   CHECK_RESULT(eResult);
   eResult = adi_sport_ConfigClock(hSPORTDev0ARx,32,false,false,false);
   CHECK_RESULT(eResult);
   eResult = adi_sport_ConfigFrameSync(hSPORTDev0ARx,31,false,false,false,false,false,false);
   CHECK_RESULT(eResult);
   eResult = adi_sport_ConfigMC(hSPORTDev0ARx,1u,7u,0u,true);
   CHECK_RESULT(eResult);
   eResult = adi_sport_SelectChannel(hSPORTDev0ARx,0u,7u);
   CHECK_RESULT(eResult);


   /* Register SPORT Callback function */
   eResult = adi_sport_RegisterCallback(hSPORTDev4ATx,/*SPORTCallback*/deploy_target_step,(void *)hPlatform);
   CHECK_RESULT(eResult);
#if 0
   /* Register SPORT Callback function */
   eResult = adi_sport_RegisterCallback(hSPORTDev0ARx,/*SPORTCallback*/deploy_target_step,(void*)hPlatform);
   CHECK_RESULT(eResult);
#endif

   /* Prepare descriptors */
   PrepareDescriptors();

   /* Submit the first buffer for Rx.  */
   eResult = adi_sport_DMATransfer(hSPORTDev0ARx,&iSRC_LIST_1_SP0A,(DMA_NUM_DESC),ADI_PDMA_DESCRIPTOR_LIST, ADI_SPORT_CHANNEL_PRIM);
   CHECK_RESULT(eResult);
   /* Submit the first buffer for Tx.  */
   eResult = adi_sport_DMATransfer(hSPORTDev4ATx,&iDESC_LIST_1_SP4A,(DMA_NUM_DESC),ADI_PDMA_DESCRIPTOR_LIST, ADI_SPORT_CHANNEL_PRIM);
   CHECK_RESULT(eResult);

   /*Enable the Sport Device 4B */
   eResult = adi_sport_Enable(hSPORTDev0ARx,true);
   CHECK_RESULT(eResult);
   /*Enable the Sport Device 4A */
   eResult = adi_sport_Enable(hSPORTDev4ATx,true);
   CHECK_RESULT(eResult);

   return eResult;

}

static int Sport_Stop(void)
{
   /* SPORT return code */
   ADI_SPORT_RESULT    eResult;

   /*Stop the DMA transfer of  Sport Device 0A */
   eResult = adi_sport_StopDMATransfer(hSPORTDev0ARx);
   CHECK_RESULT(eResult);
   /*Stop the DMA transfer of  Sport Device 4A */
   eResult = adi_sport_StopDMATransfer(hSPORTDev4ATx);
   CHECK_RESULT(eResult);
   /*Close Sport Device 0A */
   eResult = adi_sport_Close(hSPORTDev0ARx);
   CHECK_RESULT(eResult);
   /*Close Sport Device 4A */
   eResult = adi_sport_Close(hSPORTDev4ATx);
   CHECK_RESULT(eResult);

   return eResult;
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

   /* Make SPORT 4A to generate secure transactions */
   if (adi_spu_EnableMasterSecure(hSpu, SPORT_4A_SPU, true) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Failed to enable Master secure for SPORT0A\n");
      return FAILED;
   }

   /* Make SPORT 0A to generate secure transactions */
   if (adi_spu_EnableMasterSecure(hSpu, SPORT_0A_SPU, true) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Failed to enable Master secure for SPORT0B\n");
      return FAILED;
   }

   /* Make UART1 to generate secure transactions */
   if (adi_spu_EnableMasterSecure(hSpu, UART1_SPU_PID, true) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Failed to enable Master secure for UART1\n");
      return (FAILED);
   }

   /* Make UART1 Tx DMA to generate secure transactions */
   if (adi_spu_EnableMasterSecure(hSpu, UART1_TxDMA_SPU_PID, true) != ADI_SPU_SUCCESS)
   {
      REPORT_ERROR("Failed to enable Master secure for UART1 Tx DMA\n");
      return (FAILED);
   }

   return SUCCESS;
}


static volatile int oj_uart_txdma_done;

static void UART_Tx_Handler(uint32_t SID, void *pCBParam)
{
   *pREG_DMA34_STAT     |= ENUM_DMA_STAT_IRQDONE;
   *pREG_UART1_IMSK_CLR = ENUM_UART_IMSK_ETBEI_HI;
   oj_uart_txdma_done = 1;
   return;
}

int oj_uart_txdma_complete(void)
{
   if (oj_uart_txdma_done == 1)
      return 1;
   else
      return 0;
}

void oj_uart_txdma_start(unsigned char *src_addr, int cnt)
{
   oj_uart_txdma_done   = 0;
   *pREG_DMA34_ADDRSTART = (void *)adi_rtl_internal_to_system_addr((uint32_t)(src_addr), 1);
   *pREG_DMA34_XCNT      = (uint32_t)cnt;
   *pREG_DMA34_XMOD      = 1;
   *pREG_DMA34_CFG       = ENUM_DMA_CFG_XCNT_INT | ENUM_DMA_CFG_SYNC | ENUM_DMA_CFG_EN;
   *pREG_UART1_IMSK_SET  = ENUM_UART_IMSK_ETBEI_HI;
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

void oj_uart_txdma_init(void)
{
   uint32_t sclk0_freq;
   uint32_t sclk1_freq;
   uint32_t ssysclk_freq;

   /* Reseting the UART registers */
   *pREG_UART1_STAT     = (uint32_t)(DEFAULT_STATUS_VALUE);  /* Clear it to default */
   *pREG_UART1_SCR      = 0u;                                /* Clear it to default */
   *pREG_UART1_IMSK_CLR = 0x1FFu;                            /* Disable all the interrupts by default */

   /* Reseting Control and Clock register values to default */
   *pREG_UART1_CTL      = 0x0u;                              /* Clear it by default */
   *pREG_UART1_CLK      = 0x0u;                              /* Set to default UART CLK value as (SCLK/16) */

   /* Reset UART1 TxDMA registers */
   *pREG_DMA34_CFG         = 0x0;
   *pREG_DMA34_DSCPTR_NXT  = 0x0;
   *pREG_DMA34_ADDRSTART   = 0x0;
   *pREG_DMA34_XCNT        = 0x0;
   *pREG_DMA34_XMOD        = 0x0;
   *pREG_DMA34_YCNT        = 0x0;
   *pREG_DMA34_YMOD        = 0x0;
   *pREG_DMA34_STAT        = (ENUM_DMA_STAT_PIRQ | ENUM_DMA_STAT_IRQERR | ENUM_DMA_STAT_IRQDONE);
   *pREG_DMA34_DSCPTR_CUR  = 0x0;

   adi_pwr_GetSystemFreq(0u, &ssysclk_freq, &sclk0_freq, &sclk1_freq);

   /*****************************************************************************
    *
    *  Enable UART clock, put in UART mode, word length 8.
    *
    ****************************************************************************/
   *pREG_UART1_CTL = 0x0301;

   /* Bit Rate =        SCLK0
               -------------------
               [16^(1-EDBO)] * Div
     if EDBO is a 1, then
     Div = (SCLK0/BAUD_RATE)
   */
   *pREG_UART1_CLK = (0x80000000) | (sclk0_freq/BAUD_RATE);

   if(adi_int_InstallHandler(INTR_UART1_TXDMA, UART_Tx_Handler, NULL, true) != ADI_INT_SUCCESS)
   {
      REPORT_ERROR("Unable to install UART Tx Handler\n");
      exit(-1);
   }
}

void oj_ezkit_init(void)
{
   uint32_t i, Result=0;

   /* Initialize Init code. We don't process boot struct in preload case*/
   Result = (uint32_t)adi_pwr_Init((uint8_t)ADI_PWR_CGU0, (uint32_t)CLKIN);

   adi_initComponents();

   oj_uart_txdma_init();

   /* SPU initialization */
   if (Result==0u)
   {
      Result=SPU_init();
   }

   // CLocks will be generated
   Init_PCG();

   /* SRU Configuration */
   Init_SRU();

}

void oj_audio_start(void)
{
   uint32_t Result=0;

   /* SPORT Initialization */
   if (Result==0u)
   {
      Result=Sport_Init();
   }

}

