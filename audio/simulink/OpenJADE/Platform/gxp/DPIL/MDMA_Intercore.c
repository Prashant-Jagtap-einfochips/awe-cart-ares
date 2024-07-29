/*******************************************************************************
 *  BOSE CORPORATION
 *  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 *  This program may not be reproduced, in whole or in part in any
 *  form or any means whatsoever without the written permission of:
 *     BOSE CORPORATION
 *     The Mountain
 *     Framingham, MA 01701-9168
 ******************************************************************************/

/**
 * @file MDMA.c
 * @date
 * @brief
 *
 * @ingroup Software component: DSP
 *
 * @details
 *
 * @see
 *
 */

/***  Include Files ***********************************************************/

#include <adi_types.h>
#include <sys/adi_core.h>
#include <sys/platform.h>
#include <services/int/adi_int.h>
#include <services/int/adi_sec.h>

#include <stdio.h>

#include "TalariaAPI.h"
#include "TalariaCrossCore.h"
#include "MDMA_Intercore.h"

/***  External Variable References  *******************************************/

/***  Macros  **********************************************************/
#define NUM_INCOMING_INTERCORE_TRANSFERS PING_PONG_NUM_LOCAL_DST
#define NUM_OUTGOING_INTERCORE_TRANSFERS PING_PONG_NUM_LOCAL_SRC

#define SYNC_INSTN() asm("SYNC;");

/* GXP L1 memory address translation information */
#define SHARC_L1_START_ADDRESS  (0x00240000u)
#define SHARC_L1_END_ADDRESS    (0x0039FFFFu)
#define SHARC0_L1_SLAVE1_OFFSET (0x28000000u)
#define SHARC1_L1_SLAVE1_OFFSET (0x28800000u)


//#define MY_MDMA_SYNCH_BIT ENUM_DMA_CFG_SYNC   // Use SYNC to keep channel clean (line below does *not* use it)
#define MY_MDMA_SYNCH_BIT 0      // Do not use SYNC (above line *does* use it)

// These defines capture the variance between the two cores of a ASDP-SC58x chip.
#ifdef SPC1
// SPC1 transmits on MDMA0 and receives on MDMA1
#define MDMA_RX_INTR_IID        INTR_SYS_MDMA1_DST
#define MDMA_RX_ERR_INTR_IID    INTR_SYS_MDMA1_DST_ERR
#define MDMA_TX_ERR_INTR_IID    INTR_SYS_MDMA0_SRC_ERR
#define RECEIVE_DMA_REGS        pREG_DMA19_DSCPTR_NXT
#define TRANSMIT_DMA_REGS       pREG_DMA8_DSCPTR_NXT
#else
// SPC2 transmits on MDMA1 and receives on MDMA0
#define MDMA_RX_INTR_IID        INTR_SYS_MDMA0_DST
#define MDMA_RX_ERR_INTR_IID    INTR_SYS_MDMA0_DST_ERR
#define MDMA_TX_ERR_INTR_IID    INTR_SYS_MDMA1_SRC_ERR
#define RECEIVE_DMA_REGS        pREG_DMA9_DSCPTR_NXT
#define TRANSMIT_DMA_REGS       pREG_DMA18_DSCPTR_NXT
#endif

/*
 * MDMA_BASE_CFG_FLAGS - Shared settings present in all our MDMA configurations
 */
#define MDMA_BASE_CFG_FLAGS \
        ENUM_DMA_CFG_PDAT_NOTFWD  /*  PDRF: Peripheral Data Request Not Forwarded   */ | \
        ENUM_DMA_CFG_ADDR1D       /*  TWOD: One-Dimensional Addressing              */ | \
        ENUM_DMA_CFG_NO_COPY      /*  DESCIDCPY: Never Copy                         */ | \
        ENUM_DMA_CFG_TOV_DIS      /*  TOVEN: Ignore Trigger Overrun                 */ | \
        ENUM_DMA_CFG_NO_TRIG      /*  TRIG: Never assert Trigger                    */ | \
                                  /*  INT:                                          */\
        ENUM_DMA_CFG_FETCH04      /*  NDSIZE: Fetch four Descriptor Elements        */ | \
        ENUM_DMA_CFG_NO_TRGWAIT   /*  TWAIT: Begin Work Unit Automatically (No Wait)*/ | \
                                  /*  FLOW: DSCARRAY - Descriptor Array             */\
        ENUM_DMA_CFG_MSIZE04      /*  MSIZE: 4 Bytes                                */ | \
        ENUM_DMA_CFG_PSIZE04      /*  PSIZE: 4 Bytes                                */ | \
        ENUM_DMA_CFG_LD_STARTADDR /*  CADDR: Load Starting Address                  */ | \
        ENUM_DMA_CFG_NO_SYNC      /*  SYNC: No Synchronization                      */ | \
                                  /*  WNR: Read vs. write                           */\
        ENUM_DMA_CFG_EN           /*  EN: Keep channel enabled                      */

/*
 * MDMA_SRC_CONFIG_NO_INT_DSCARRAY_NODE - Source side MDMA node configured with
 * INT: NO_INT - Do NOT generate an Interrupt when data transfer for this Node completes
 * FLOW: DSCARRAY - Descriptor Array mode for MDMA transfer
 * WNR: Transmit (Read from memory)  */
#define MDMA_SRC_CONFIG_NO_INT_DSCARRAY_NODE       MDMA_BASE_CFG_FLAGS | \
    ENUM_DMA_CFG_NO_INT       /* NO_INT: Never Assert Interrupt     */ | \
    ENUM_DMA_CFG_DSCARRAY     /* FLOW: DSCARRAY - Descriptor Array  */ | \
    ENUM_DMA_CFG_READ         /* WNR: Transmit (Read from memory)   */

/*
 * MDMA_SRC_CONFIG_STOP_NODE - Source side MDMA node configured with
 * INT: NO_INT - Do NOT generate an Interrupt when data transfer for this Node completes
 * FLOW: STOP - This is the Last MDMA node to transfer
 * WNR: Transmit (Read from memory)  */
#define MDMA_SRC_CONFIG_STOP_NODE                  MDMA_BASE_CFG_FLAGS | \
    ENUM_DMA_CFG_NO_INT       /* NO_INT: Never Assert Interrupt     */ | \
    ENUM_DMA_CFG_STOP         /* FLOW: STOP - no more descriptors   */ | \
    MY_MDMA_SYNCH_BIT         /* SYNC: (for read) clear FIFO at end */ | \
    ENUM_DMA_CFG_READ         /* WNR: Transmit (Read from memory)   */

/*
 * MDMA_DST_CONFIG_NO_INT_DSCARRAY_NODE - Destination side MDMA node configured with
 * INT: NO_INT - Do NOT generate an Interrupt when data transfer for this Node completes
 * FLOW: DSCARRAY - Descriptor Array mode for MDMA transfer
 * WNR: Receive (Write to memory)  */
#define MDMA_DST_CONFIG_NO_INT_DSCARRAY_NODE       MDMA_BASE_CFG_FLAGS | \
    ENUM_DMA_CFG_NO_INT       /* NO_INT: Never Assert Interrupt     */ | \
    ENUM_DMA_CFG_DSCARRAY     /* FLOW: ARRAY - DSCARRAY             */ | \
    ENUM_DMA_CFG_WRITE        /* WNR: Receive (Write to memory)     */

/*
 * MDMA_DST_CONFIG_STOP_NODE - Destination side MDMA node configured with
 * INT: INT - Generate an Interrupt when data transfer for this node completes
 * FLOW: STOP - This is the Last MDMA node to transfer
 * WNR: Receive (Write to memory)  */
#define MDMA_DST_CONFIG_STOP_NODE                  MDMA_BASE_CFG_FLAGS | \
    ENUM_DMA_CFG_XCNT_INT     /* INT: Interrupt when X Count Expires*/ | \
    ENUM_DMA_CFG_STOP         /* FLOW: STOP - no mode descriptors   */ | \
    ENUM_DMA_CFG_WRITE        /* WNR: Receive (Write to memory)     */

/*
 * MDMA_CLEAR_CHAN_INTERRUPTS - Clears pending MDMA stream channel interrupts   */
#define MDMA_CLEAR_CHAN_INTERRUPTS      (ENUM_DMA_STAT_PIRQ | ENUM_DMA_STAT_IRQERR | ENUM_DMA_STAT_IRQDONE)


/***  Types  ***********************************************************/
typedef struct
{
   unsigned int start_address;     /* start address of work unit */
   unsigned int configuration;     /* DMA channel configuration */
   unsigned int data_word_counter; /* data word down counter */
   unsigned int stride;            /* stride*/
} DMA_TCB_ARRAY; //TCB ARRAY

#pragma pack(4)
typedef struct
{
    volatile uint32_t   NextDescPtr;    /* Pointer to next Initial Descriptor */
    volatile uint32_t   StartAddr;      /* Start address of current buffer */
    volatile uint32_t   Config;         /* Configuration register */
    volatile uint32_t   XCount;         /* XCOUNT - Inner loop count start value in number of MSIZE data transfers */
    volatile int32_t    XModify;        /* XMODIFY - Inner loop address increment, in bytes */
    volatile uint32_t   YCount;         /* YCOUNT - Outer loop count start value (2D only), in number of rows */
    volatile int32_t    YModify;        /* YMODIFY - Outer loop address increment (2D only), in bytes */
    uint32_t            Padding0;
    uint32_t            Padding1;
    volatile uint32_t   CurrDescPtr;    /* Current Descriptor Pointer */
    volatile uint32_t   PrevDescPtr;    /* Previous Initial Descriptor Pointer */
    volatile uint32_t   CurrAddr;       /* Current Address */
    volatile uint32_t   Status;         /* Status Register */
    volatile uint32_t   CurrXCount;     /* Current XCOUNT */
    volatile uint32_t   CurrYCount;     /* Current YCOUNT */
    uint32_t            Padding3;
    volatile uint32_t   BwLimit;        /* Bandwidth Limit Count */
    volatile uint32_t   CurrBwLimit;    /* Bandwidth Limit Count Current */
    volatile uint32_t   BwMonitor;      /* Bandwidth Monitor Count */
    volatile uint32_t   CurrBwMonitor;  /* Bandwidth Monitor Count Current */

} MDMA_DMA_CHAN_REGS;
#pragma pack()


/***  Local Function Prototypes  *********************************************/

void MDMA_Rx_Int_Handler(uint32_t iid, void* handlerArg);
void MDMA_RX_Complete_Err_Handler(uint32_t iid, void* handlerArg);
void MDMA_TX_Complete_Err_Handler(uint32_t iid, void* handlerArg);
static void MDMA_Create_TxArray(void);
static inline void MDMA_Start_SRC_Tx(void);
static void MDMA_Create_RxArray(void);
static inline void MDMA_Start_DST_Rx(void);
static inline void MDMA_HarvestStartAddressTx(void);
static inline void MDMA_HarvestStartAddressRx(void);
static void MDMA_ResetDmaRegs( volatile MDMA_DMA_CHAN_REGS *pBaseReg );


/***  Globals  *******************************************************/

/* MDMA Tx and Rx TCBs */
#if (NUM_OUTGOING_INTERCORE_TRANSFERS > 0)
DMA_TCB_ARRAY MDMA_CORE_TX_TCB[NUM_OUTGOING_INTERCORE_TRANSFERS];
#endif //#if (NUM_OUTGOING_INTERCORE_TRANSFERS > 0)

static unsigned int mdma_rx_cnt    = 0;
static unsigned int mdma_tx_cnt    = 0;

volatile unsigned int err_count_mdma_tx = 0;
volatile unsigned int err_count_mdma_rx = 0;

#if (NUM_INCOMING_INTERCORE_TRANSFERS > 0)
DMA_TCB_ARRAY MDMA_CORE_RX_TCB[NUM_INCOMING_INTERCORE_TRANSFERS];
#endif //#if (NUM_INCOMING_INTERCORE_TRANSFERS > 0)

/***  Functions  *******************************************************/
/**
 * @brief Translates the supplied GXP memory byte address for MDMA operation
 *
 * @param[in] MemAddr   Memory address to translate (byte address)
 *
 * @return Translated memory address for MDMA usage
 */
static inline uint32_t TranslateMdmaAddr( uint32_t MemAddr )
{
    /* Check if the supplied memory address is in L1 */
    if ( (MemAddr >= SHARC_L1_START_ADDRESS) && (MemAddr <= SHARC_L1_END_ADDRESS) )
    {
        /* Adjust L1 address based on core executing on */
        ADI_CORE_ID Core = adi_core_id();
        if ( Core == ADI_CORE_SHARC0 )
        {
            MemAddr += SHARC0_L1_SLAVE1_OFFSET;
        }
        else if ( Core == ADI_CORE_SHARC1 )
        {
            MemAddr += SHARC1_L1_SLAVE1_OFFSET;
        }
    }

    return MemAddr;

} /* TranslateMdmaAddr() */

/**
*  @brief MDMA_Init: MDMA driver Initializer
*
*  @details Registers interrupts related to the MDMA driver, stores pointers
*           to functions it will call at run time to get tx/rx address,
*           initializes the tx/rx DMA TCB chain, and kicks off the MDMA RX
*           to be ready ahead of the TX from the other core.
*
*  @param[in] rx_next_frame_spec_callback pointer to function that will return
*             the transfer spec for the _next_ frame's transfer
*
*  @param[in] tx_this_frame_spec_callback pointer to function that will return
*             the transfer spec for the current frame's transfer
*
*  @return success or failure
*
*******************************************************************************/
mdma_result_t MDMA_Init( void )
{
   ADI_INT_STATUS int_res = ADI_INT_SUCCESS;

   // Reset MDMA stream DMA channel registers and clear pending interrupts used
   // by SHARC core
   MDMA_ResetDmaRegs((volatile MDMA_DMA_CHAN_REGS*) TRANSMIT_DMA_REGS);
   MDMA_ResetDmaRegs((volatile MDMA_DMA_CHAN_REGS*) RECEIVE_DMA_REGS);

   // Install the MDMA Rx ISR
   int_res = adi_int_InstallHandler(MDMA_RX_INTR_IID, MDMA_Rx_Int_Handler, 0, 1);

   // Install the MDMA Error ISRs
   int_res |= adi_int_InstallHandler(MDMA_TX_ERR_INTR_IID, MDMA_TX_Complete_Err_Handler, 0, 1);
   int_res |= adi_int_InstallHandler(MDMA_RX_ERR_INTR_IID, MDMA_RX_Complete_Err_Handler, 0, 1);
   mdma_result_t result = (mdma_result_t) (int_res != ADI_INT_SUCCESS);

   // Initialize the MDMA RX TCB chain and setup for the first reception
#if (NUM_INCOMING_INTERCORE_TRANSFERS > 0)
   MDMA_Create_RxArray();

   MDMA_Start_DST_Rx();
#endif

   // Initialize the MDMA TX TCB chain
#if (NUM_OUTGOING_INTERCORE_TRANSFERS > 0)
   MDMA_Create_TxArray();
#endif

   return result;

} /* MDMA_Init() */

volatile unsigned int mdma_rx_count = 0;
volatile unsigned int g_mdmaRxDone = 0;

/**
 * @brief MDMA_Rx_Int_Handler: MDMA Rx ISR
 *
 * @details
 *
 * @see
 *
 * @pre uCOS-III has started.
 *
 * @param
 *
 * @post
 *
 * @return
 *
 * @warning
 *
 */
void MDMA_Rx_Int_Handler( uint32_t iid, void* handlerArg )
{
    mdma_rx_count++;

    g_mdmaRxDone = 1;

#if defined (SPC1)
    *pREG_DMA19_STAT = ENUM_DMA_STAT_IRQDONE; //For SH1, SH3
#endif
#if defined (SPC2)
    *pREG_DMA9_STAT = ENUM_DMA_STAT_IRQDONE; //For SH2, SH4
#endif

   SYNC_INSTN();

} /* MDMA_Rx_Int_Handler() */


/**
 * @brief MDMA_TX_Complete_Err_Handler: ISR for MDMA Source Channel Error
 *
 * @details
 *
 * @see
 *
 * @pre uCOS-III has started.
 *
 * @param
 *
 * @post
 *
 * @return
 *
 * @warning
 *
 */
void MDMA_TX_Complete_Err_Handler( uint32_t iid, void* handlerArg )
{
    err_count_mdma_tx++;
#if defined (SPC1)
    *pREG_DMA8_STAT = ENUM_DMA_STAT_IRQERR;
#endif //#if defined (SPC1)
#if defined (SPC2)
    *pREG_DMA18_STAT = ENUM_DMA_STAT_IRQERR;
#endif //#if defined (SPC2)

} /* MDMA_TX_Complete_Err_Handler() */


/**
 * @brief MDMA_RX_Complete_Err_Handler: ISR for MDMA Destination Channel Error
 *
 * @details
 *
 * @see
 *
 * @pre uCOS-III has started.
 *
 * @param
 *
 * @post
 *
 * @return
 *
 * @warning
 *
 */
void MDMA_RX_Complete_Err_Handler( uint32_t iid, void* handlerArg )
{

    err_count_mdma_rx++;
#if defined (SPC1)
    *pREG_DMA19_STAT = ENUM_DMA_STAT_IRQERR;
#endif //#if defined (SPC1)
#if defined (SPC2)
    *pREG_DMA9_STAT = ENUM_DMA_STAT_IRQERR;
#endif //#if defined (SPC2)

} /* MDMA_RX_Complete_Err_Handler() */


/**
*  @brief MDMA_ResetDmaRegs: Resets DMA channel registers
*
*  @details Resets DMA channel registers and clears pending interrupts
*           of DMA channel with base register address provided
*
*  @param[in] regBaseAddr pointer to base register address of the DMA
*             channel
*
*  @pre
*
*  @warning
*
*  @return None
*
*******************************************************************************/
static void MDMA_ResetDmaRegs( volatile MDMA_DMA_CHAN_REGS *regBaseAddr )
{
    /* Reset DMA channel registers and clear any pending interrupts */
    regBaseAddr->Config        = 0u;
    regBaseAddr->NextDescPtr   = 0u;
    regBaseAddr->StartAddr     = 0u;
    regBaseAddr->XCount        = 0u;
    regBaseAddr->XModify       = 0u;
    regBaseAddr->YCount        = 0u;
    regBaseAddr->YModify       = 0u;
    regBaseAddr->Status        = MDMA_CLEAR_CHAN_INTERRUPTS;
    regBaseAddr->CurrDescPtr   = 0u;
    regBaseAddr->BwLimit	   = 0u;
    regBaseAddr->BwMonitor	   = 0u;

} /* MDMA_ResetDmaRegs() */


#if (NUM_OUTGOING_INTERCORE_TRANSFERS > 0)
/**
*  @brief MDMA_Create_TxArray: Create MDMA TCB Array to Send data (TX)
*
*  @details This creates an MDMA TCB TX Array for sending MDMA data from this
*           SHARC core to another SHARC core on the same Griffin
*
*  @pre
*
*  @warning
*
*  @return None
*
*******************************************************************************/
static void MDMA_Create_TxArray( void )
{
    int i;
    for (i = 0; i < NUM_OUTGOING_INTERCORE_TRANSFERS; i++)
    {
        transfer_spec_type spec = MODEL_FUNC(GetLocalSrcBuffer)(i);
        uint32_t buffer_address = (uint32_t)spec.buffer;
        uint32_t byte_size = spec.byte_size;

        MDMA_CORE_TX_TCB[i].start_address = TranslateMdmaAddr( buffer_address );
        MDMA_CORE_TX_TCB[i].configuration = MDMA_SRC_CONFIG_NO_INT_DSCARRAY_NODE;
        MDMA_CORE_TX_TCB[i].data_word_counter = byte_size / 4;
        MDMA_CORE_TX_TCB[i].stride = 4;
    }

    /* Configure the Last MDMA node in DMA STOP mode */
    MDMA_CORE_TX_TCB[i - 1].configuration = MDMA_SRC_CONFIG_STOP_NODE;

} /* MDMA_Create_TxArray() */


/**
*  @brief MDMA_Start_SRC_Tx: Start MDMA TX
*
*  @details Start DMA8 MDMA0_SRC - Memory DMA 0 Source Channel for SH1, SH3
*           Start DMA18 MDMA1_SRC - Memory DMA 1 Source Channel for SH2, SH4
*  @pre
*
*  @warning
*
*  @return None
*
*******************************************************************************/
static inline void MDMA_Start_SRC_Tx( void )
{
	/* Start in DSCARRAY mode */
#if defined (SPC1)
    *pREG_DMA8_DSCPTR_CUR = (void *) TranslateMdmaAddr( (uint32_t) &MDMA_CORE_TX_TCB[0] );
    *pREG_DMA8_CFG = MDMA_SRC_CONFIG_NO_INT_DSCARRAY_NODE;
    SYNC_INSTN();
#endif
#if defined (SPC2)
    *pREG_DMA18_DSCPTR_CUR = (void *) TranslateMdmaAddr( (uint32_t) &MDMA_CORE_TX_TCB[0] );
    *pREG_DMA18_CFG = MDMA_SRC_CONFIG_NO_INT_DSCARRAY_NODE;
    SYNC_INSTN();
#endif

} /* MDMA_Start_SRC_Tx() */
#endif //#if (NUM_OUTGOING_INTERCORE_TRANSFERS > 0)


#if (NUM_INCOMING_INTERCORE_TRANSFERS > 0)
/**
*  @brief MDMA_Create_RxArray: Create MDMA TCB RX Array
*
*  @details This creates a MDMA TCB RX Array for Receiving data into this
*           SHARC core from the other SHARC core on same Griffin
*
*  @pre
*
*  @warning
*
*  @return None
*
*******************************************************************************/
static void MDMA_Create_RxArray( void )
{
    int i;
    for (i = 0; i < NUM_INCOMING_INTERCORE_TRANSFERS; i++)
    {
        transfer_spec_type spec = MODEL_FUNC(GetLocalDstBufferForNextFrame)(i);
        uint32_t buffer_address = (uint32_t)spec.buffer;
        uint32_t byte_size = spec.byte_size;

        MDMA_CORE_RX_TCB[i].start_address = TranslateMdmaAddr( buffer_address );
        MDMA_CORE_RX_TCB[i].configuration = MDMA_DST_CONFIG_NO_INT_DSCARRAY_NODE;
        MDMA_CORE_RX_TCB[i].data_word_counter = byte_size / 4;
        MDMA_CORE_RX_TCB[i].stride = 4;
    }

    /* Configure the Last MDMA node in DMA STOP mode */
    MDMA_CORE_RX_TCB[i - 1].configuration = MDMA_DST_CONFIG_STOP_NODE;

} /* MDMA_Create_RxArray() */


/**
*  @brief MDMA_Start_DST_Rx: Start MDMA RX
*
*  @details Start DMA9 MDMA0_DST - Memory DMA 0 Destination Channel for SH2, SH4
*           Start DMA19 MDMA1_DST - Memory DMA 1 Destination Channel for SH1, SH3
*
*  @pre
*
*  @warning
*
*  @return None
*
*******************************************************************************/
static inline void MDMA_Start_DST_Rx( void )
{
	/* Start in DSCARRAY mode */
#if defined (SPC1)
    *pREG_DMA19_DSCPTR_CUR = (void *) TranslateMdmaAddr( (uint32_t) &MDMA_CORE_RX_TCB[0] );
    SYNC_INSTN();
    *pREG_DMA19_CFG = MDMA_DST_CONFIG_NO_INT_DSCARRAY_NODE;
    SYNC_INSTN();
#endif //#if defined (SPC1)
#if defined (SPC2)
    *pREG_DMA9_DSCPTR_CUR = (void *) TranslateMdmaAddr( (uint32_t) &MDMA_CORE_RX_TCB[0] );
    SYNC_INSTN();
    *pREG_DMA9_CFG = MDMA_DST_CONFIG_NO_INT_DSCARRAY_NODE;
    SYNC_INSTN();
#endif //#if defined (SPC2)

} /* MDMA_Start_DST_Rx() */
#endif //#if (NUM_INCOMING_INTERCORE_TRANSFERS > 0)


#if (NUM_OUTGOING_INTERCORE_TRANSFERS > 0)
/**
*  @brief MDMA_HarvestStartAddressTx: Harvest start address for MDMA TX
*
*  @details
*
*  @pre
*
*  @warning
*
*  @return None
*
*******************************************************************************/
static inline void MDMA_HarvestStartAddressTx( void )
{
   int index;

   for (index = 0;
        index < NUM_OUTGOING_INTERCORE_TRANSFERS;
        index++)
    {
        transfer_spec_type spec = MODEL_FUNC(GetLocalSrcBuffer)(index);
        uint32_t buffer_address = (uint32_t)spec.buffer;
        MDMA_CORE_TX_TCB[index].start_address = TranslateMdmaAddr( buffer_address );
    }

} /* MDMA_HarvestStartAddressTx() */
#endif //#if (NUM_OUTGOING_INTERCORE_TRANSFERS > 0)


/**
*  @brief Updates ping pong address and starts MDMA TX
*
*  @return None
*
*******************************************************************************/
void MDMA_launchTx( void )
{
#if (NUM_OUTGOING_INTERCORE_TRANSFERS > 0)
    // Update the start address
    MDMA_HarvestStartAddressTx();

    // Launch the MDMA
    MDMA_Start_SRC_Tx();

    // Update the debug counter
    mdma_tx_cnt++;
#endif //#if (NUM_OUTGOING_INTERCORE_TRANSFERS > 0)

} /* MDMA_launchTx() */


#if (NUM_INCOMING_INTERCORE_TRANSFERS > 0)
/**
*  @brief MDMA_HarvestStartAddressRx: Harvest start address for MDMA RX
*
*  @details
*
*  @pre
*
*  @warning
*
*  @return None
*
*******************************************************************************/
static inline void MDMA_HarvestStartAddressRx( void )
{
   int index;

   for (index = 0;
        index < NUM_INCOMING_INTERCORE_TRANSFERS;
        index++)
    {
        transfer_spec_type spec = MODEL_FUNC(GetLocalDstBufferForNextFrame)(index);
        uint32_t buffer_address = (uint32_t)spec.buffer;
        MDMA_CORE_RX_TCB[index].start_address = TranslateMdmaAddr( buffer_address );
    }

} /* MDMA_HarvestStartAddressRx() */
#endif //#if (NUM_INCOMING_INTERCORE_TRANSFERS > 0)


/**
*  @brief Waits for this frame's RX to complete, then updates ping pong address
*        and starts MDMA RX
*
*  @warning This function block until a previous RX has been received, so it
*           should be called towards the end of a frame
*
*  @return None
*
*******************************************************************************/
void MDMA_blockAndLaunchRx( void )
{
#if (NUM_INCOMING_INTERCORE_TRANSFERS > 0)
    while (!g_mdmaRxDone);
    /* Reset state for next transfer */
    g_mdmaRxDone = 0;

    // Update the debug counter
    mdma_rx_cnt++;

    // Update the start address
    MDMA_HarvestStartAddressRx();

    // Launch the MDMA
    MDMA_Start_DST_Rx();
#endif //#if (NUM_INCOMING_INTERCORE_TRANSFERS > 0)

} /* MDMA_blockAndLaunchRx() */

