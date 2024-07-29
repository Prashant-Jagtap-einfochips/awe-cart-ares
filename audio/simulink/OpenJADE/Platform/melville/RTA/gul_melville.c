/*****************************************************************************
 * Audio_Loopback_TDM.c
 *****************************************************************************/
#include <sys/platform.h>
#include <sys/adi_core.h>
#include <services/int/adi_int.h>
#include <services/int/adi_sec.h>
#include <drivers/sport/adi_sport.h>
#include <drivers/linkport/adi_linkport.h>
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


#define DMA_NUM_DESC          2u

#define SUCCESS   0
#define FAILED   -1

#define CHECK_RESULT(eResult) \
        if(eResult != 0)\
      {\
         return (1);\
        }

#define REPORT_ERROR           printf

#define LINKPORT_MEM_SIZE                ADI_LP_MEMORY_SIZE
#define LINKPORT_RECV                    1
#define LINKPORT_SEND                    2
#define LINKPORT_IDLE                    3
#define LP0_CLK_DIV 					 4

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

int linkport_state;
int linkport_launch;


extern void deploy_target_step( void     *pAppHandle,
                                uint32_t  nEvent,
                                void     *pArg);
extern void resp_fill_state(uint8_t* pBuf);
extern void resp_extract_state(uint8_t* pBuf);
#if ((NUM_OF_SOC == DUALSOC) && (DUALSOC_TYPE == SOC_MASTER))
void linkport0_target_step( void     *pAppHandle,
                         	 	uint32_t  nEvent,
								void     *pArg);
#endif


#if ((NUM_OF_SOC == SINGLESOC) || ((NUM_OF_SOC == DUALSOC) && (DUALSOC_TYPE == SOC_MASTER)))
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
#endif

/* Linkport 0 handle */
static uint8_t linkport_mem0[LINKPORT_MEM_SIZE];
static ADI_LINKPORT_HANDLE hDevice0;
static ADI_PDMA_DESC_LIST  LP0_Desc;

/* SPU handle */
ADI_SPU_HANDLE hSpu;

/* Memory required for the SPU operation */
static uint8_t SpuMemory[ADI_SPU_MEMORY_SIZE];

/* sport buffers for ADC/DAC data */
_Pragma("align 64") int32_t sport_TxBuffer1[FRAME_SIZE_OUT + FRAM_SIZE_CMD];
_Pragma("align 64") int32_t sport_TxBuffer2[FRAME_SIZE_OUT + FRAM_SIZE_CMD];
_Pragma("align 64") int32_t sport_RxBuffer1[FRAME_SIZE_IN + FRAM_SIZE_CMD];
_Pragma("align 64") int32_t sport_RxBuffer2[FRAME_SIZE_IN + FRAM_SIZE_CMD];
#if ((NUM_OF_SOC == DUALSOC) && (DUALSOC_TYPE == SOC_MASTER))
_Pragma("align 64") int32_t middle_Buffer1[FRAME_SIZE_OUT + FRAM_SIZE_CMD];
_Pragma("align 64") int32_t middle_Buffer2[FRAME_SIZE_OUT + FRAM_SIZE_CMD];
#endif


#if ((NUM_OF_SOC == SINGLESOC) || ((NUM_OF_SOC == DUALSOC) && (DUALSOC_TYPE == SOC_MASTER)))
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
#endif

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

   return SUCCESS;
}

#if (NUM_OF_SOC == DUALSOC)
#if (DUALSOC_TYPE == SOC_MASTER)
void LINKPORTCallback0(int32_t* pBuf)
{
	int elpResult;

	//printf("Core 0: done\r\n");
	if (linkport_state == LINKPORT_SEND) {
		//printf("Core 0: to receive\r\n");
		// Switch Linkport to receive data back from Slave SHARC
		LP0_Desc.pStartAddr            = pBuf;
		LP0_Desc.XCount                = FRAME_SIZE_OUT + FRAM_SIZE_CMD;
		LP0_Desc.XModify               = 4u;
		LP0_Desc.Config                = ENUM_DMA_CFG_XCNT_INT;
		elpResult = adi_linkport_Reconfigure(hDevice0);
		if(elpResult != ADI_LINKPORT_SUCCESS) {
			printf("Linkport Reconfigure Error.\r\n");
		}
		elpResult = adi_linkport_DMATransfer(hDevice0,&LP0_Desc,1,ADI_PDMA_STOP_MODE);
		if(elpResult != ADI_LINKPORT_SUCCESS) {
			printf("Linkport DMA transfer after Reconfigure Error.\r\n");
		}
		linkport_state = LINKPORT_RECV;
	}
	else if (linkport_state == LINKPORT_RECV) {
		//printf("Core 0: to idle\r\n");
      resp_extract_state((uint8_t*)pBuf);
		linkport_state = LINKPORT_IDLE;
	}
}
#else
static void LINKPORTCallback0(void *pCBParam, uint32_t Event, void *pArg)
{
	int elpResult;

	switch (Event)
	{
		case ADI_LINKPORT_EVENT_DMA_PROCESSED:
			if (linkport_state == LINKPORT_RECV) {
				deploy_target_step(pCBParam, Event, pArg);
				// Fill Response from DSP2
            resp_fill_state((uint8_t*)sport_TxBuffer1);
            // Switch Linkport to send data back to Master SHARC
				LP0_Desc.pStartAddr            = sport_TxBuffer1;
				LP0_Desc.XCount                = FRAME_SIZE_OUT + FRAM_SIZE_CMD;
				LP0_Desc.XModify               = 4u;
				LP0_Desc.Config                = ENUM_DMA_CFG_XCNT_INT;
				elpResult = adi_linkport_Reconfigure(hDevice0);
				if(elpResult != ADI_LINKPORT_SUCCESS) {
					printf("Linkport Reconfigure Error.\r\n");
				}
				elpResult = adi_linkport_DMATransfer(hDevice0,&LP0_Desc,1,ADI_PDMA_STOP_MODE);
				if(elpResult != ADI_LINKPORT_SUCCESS) {
					printf("Linkport DMA transfer after Reconfigure Error.\r\n");
				}
				linkport_state = LINKPORT_SEND;
			}
			else {
				// Switch Linkport to receive data from Master SHARC
				LP0_Desc.pStartAddr            = sport_RxBuffer1;
				LP0_Desc.XCount                = FRAME_SIZE_IN + FRAM_SIZE_CMD;
				LP0_Desc.XModify               = 4u;
				LP0_Desc.Config                = ENUM_DMA_CFG_XCNT_INT;
				elpResult = adi_linkport_Reconfigure(hDevice0);
				if(elpResult != ADI_LINKPORT_SUCCESS) {
					printf("Linkport Reconfigure Error.\r\n");
				}
				elpResult = adi_linkport_DMATransfer(hDevice0,&LP0_Desc,1,ADI_PDMA_STOP_MODE);
				if(elpResult != ADI_LINKPORT_SUCCESS) {
					printf("Linkport DMA transfer after Reconfigure Error.\r\n");
				}
				linkport_state = LINKPORT_RECV;
			}
			break;
		default:
			break;
	}

}
#endif

/*
 * Linkport 0 Initialization
 *
 * Parameters
 *  None
 *
 * Returns
 *  None
 *
 */
int Linkport0_Init(void)
{
	unsigned int  elpResult;
#if (DUALSOC_TYPE == SOC_MASTER)
	Platform_Handle_t hPlatform = platform_get_handle();

	linkport_launch = 0;
	linkport_state = LINKPORT_IDLE;
	//Disable the Fault functionality to use the pin PC_07 as Link port1 Clock
	*pREG_PADS0_PCFG0 |= BITM_PADS_PCFG0_FAULT_DIS;
	//Configure the SPU correspond to Linkport and Linkport DMAs
	adi_spu_EnableMasterSlaveSecure(3,true,true);
	adi_spu_EnableMasterSlaveSecure(5,true,true);

	elpResult = adi_linkport_Open(0,ADI_LINKPORT_DIR_TX,linkport_mem0,LINKPORT_MEM_SIZE,&hDevice0);
	if(elpResult != ADI_LINKPORT_SUCCESS) {
		printf("Linkeport 0 open failed\n");
	}

	elpResult = adi_linkport_ConfigClock(hDevice0,LP0_CLK_DIV);
	if(elpResult != ADI_LINKPORT_SUCCESS) {
		printf("Device 0 clock configuration failed\n");
	}

	elpResult = adi_linkport_RegisterCallback(hDevice0,linkport0_target_step,(void *)hPlatform);
	if(elpResult != ADI_LINKPORT_SUCCESS) {
		printf("LP0 Callback registering failed\n");
	}
#else
	//Disable the Fault functionality to use the pin PC_07 as Link port1 Clock
	*pREG_PADS0_PCFG0 |= BITM_PADS_PCFG0_FAULT_DIS;
	//Configure the SPU correspond to Linkport and Linkport DMAs
	adi_spu_EnableMasterSlaveSecure(3,true,true);
	adi_spu_EnableMasterSlaveSecure(5,true,true);

	elpResult = adi_linkport_Open(0,ADI_LINKPORT_DIR_RX,linkport_mem0,LINKPORT_MEM_SIZE,&hDevice0);
	if(elpResult != ADI_LINKPORT_SUCCESS) {
		printf("Linkeport 0 open failed\n");
	}
#endif

	return (int)elpResult;
}

#if (DUALSOC_TYPE == SOC_MASTER)
int Linkport0_Send(int32_t* txBuff, int32_t size)
{
	int elpResult;

	LP0_Desc.pStartAddr            = txBuff;
	LP0_Desc.XCount                = size;
	LP0_Desc.XModify               = 4u;
	LP0_Desc.Config                = ENUM_DMA_CFG_XCNT_INT;

	if (linkport_state == LINKPORT_IDLE) {
		if (linkport_launch) {
			elpResult = adi_linkport_Reconfigure(hDevice0);
			if(elpResult != ADI_LINKPORT_SUCCESS) {
				printf("Device transmitter reconfiguration to receiver failed\n");
			}
		}
		elpResult = adi_linkport_DMATransfer(hDevice0,&LP0_Desc,1,ADI_PDMA_STOP_MODE);
		if(elpResult != ADI_LINKPORT_SUCCESS) {
			printf("Device receiver configuration failed\n");
		}
		linkport_launch = 1;
		linkport_state = LINKPORT_SEND;
	}

	return elpResult;
}
#else
int Linkport0_Recv(void)
{
	int elpResult;

	Platform_Handle_t hPlatform = platform_get_handle();

	LP0_Desc.pStartAddr            = sport_RxBuffer1;
	LP0_Desc.XCount                = FRAME_SIZE_IN + FRAM_SIZE_CMD;
	LP0_Desc.XModify               = 4u;
	LP0_Desc.Config                = ENUM_DMA_CFG_XCNT_INT;

	elpResult = adi_linkport_RegisterCallback(hDevice0,LINKPORTCallback0,(void *)hPlatform);
	if(elpResult != ADI_LINKPORT_SUCCESS) {
			printf("LP0 Callback registering failed\n");
	}
	elpResult = adi_linkport_DMATransfer(hDevice0,&LP0_Desc,1,ADI_PDMA_STOP_MODE);
	if(elpResult != ADI_LINKPORT_SUCCESS) {
		printf("Device receiver configuration failed\n");
	}
	linkport_state = LINKPORT_RECV;

	return elpResult;
}
#endif

int Linkport0_Close(void)
{
	int elpResult;

	elpResult = adi_linkport_Close(hDevice0);
	if(elpResult != ADI_LINKPORT_SUCCESS) {
		printf("Linkport 0 close failed\n");
	}

	return elpResult;
}
#endif




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
   uint8_t ch = (uint8_t)*pREG_UART1_RBR;

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

void gul_uartio_stop(void)
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

#define SEG_L2_UNCACHED _Pragma ("section(\"seg_l2_uncached\", DOUBLEANY)" )
SEG_L2_UNCACHED char stdout_buf[256];
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
   *pREG_UART1_STAT     = (uint32_t)(DEFAULT_STATUS_VALUE);  /* Clear it to default */
   *pREG_UART1_SCR      = 0u;                                /* Clear it to default */
   *pREG_UART1_IMSK_CLR = 0x1FFu;                            /* Disable all the interrupts by default */
   
   /* Reseting Control and Clock register values to default */
   *pREG_UART1_CTL      = 0x0u;                              /* Clear it by default */
   *pREG_UART1_CLK      = 0x0u;                              /* Set to default UART CLK value as (SCLK/16) */

   if(adi_int_InstallHandler(INTR_UART1_RXDMA, UART_Rx_Handler, NULL, true) != ADI_INT_SUCCESS)
   {
      printf("Error: Unable to install UART Rx Handler\n");
      exit(-1);
   }

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

   *pREG_UART1_IMSK_SET = 0x001;

   /* Create new device ID.  The device ID is arbitrary and used globally in the application.*/
   int deviceID = 0; 

   UARTIO_DevEntry.DeviceID = UARTIO_DEVICEID;

   /* Insert this new device into the device table.   */
   deviceID = add_devtab_entry ( &UARTIO_DevEntry );

   if ( UARTIO_DEVICEID != deviceID )
   {
      printf("Error: Unable to register new STDIO Device Driver. Try another DeviceID\n");
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
  while (!(*pREG_UART1_STAT & BITM_UART_STAT_TEMT))
  {
     
  }; /* wait*/
}

/*transmit character by polling the THRE bit in the LSR register.*/
void UART_putc(char c)
{
  while (!(*pREG_UART1_STAT & BITM_UART_STAT_THRE))
  {
      
   }; /*wait*/
   *pREG_UART1_THR = c;
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
void gul_uartio_start(void)
{
   return;
}
void gul_uartio_stop(void)
{
   return;
}
#endif /* GUL_UARTIO */

void gul_melville_init(void)
{
   uint32_t i, Result=0;
   
   /* Initialize Init code. We don't process boot struct in preload case*/
   Result = (uint32_t)adi_pwr_Init((uint8_t)ADI_PWR_CGU0, (uint32_t)CLKIN);

   adi_initComponents();

   gul_uartio_start();

   /* SPU initialization */
   if (Result==0u)
   {
      Result=SPU_init();
   }
#if ((NUM_OF_SOC == SINGLESOC) || ((NUM_OF_SOC == DUALSOC) && (DUALSOC_TYPE == SOC_MASTER)))
   // CLocks will be generated
   Init_PCG();

   /* SRU Configuration */
   Init_SRU();
#endif

#if (NUM_OF_SOC == DUALSOC)
   /* As SHARC Master in Melville, it does initialization as Linkport master */
   Linkport0_Init();
#endif

}

void gul_melville_audio_start(void)
{
   uint32_t Result=0;

#if ((NUM_OF_SOC == SINGLESOC) || ((NUM_OF_SOC == DUALSOC) && (DUALSOC_TYPE == SOC_MASTER)))
	/* SPORT Initialization */
	if (Result==0u)
	{
	   Result=Sport_Init();
	}
#else
	/* Linkport Open as salve device */
	Linkport0_Recv();
#endif

}

