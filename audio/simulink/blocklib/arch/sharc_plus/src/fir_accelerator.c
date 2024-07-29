/*****************************************************************************************
 *
 * BOSE CORPORATION
 * COPYRIGHT 2014 BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:

 *     BOSE CORPORATION
 *     The Mountain
 *     Framingham, MA 01701-9168

*****************************************************************************************/

/**
 *  @file fir_accelerator.c
 *  @brief Connects model FIR code generation with hardware accelerator manager
 *
 *  @note Optimization settings for speed should include setting compiler
 *        flags -Ov100 and -ipa (if possible) for the top level build. NDEBUG 
 *        should also be defined to turn off asserts. Also, it is important
 *        to note that this module was NOT designed to support the usage of 
 *        cached data.
 * 
 *  @ingroup dsp
 *
 */

/***  Include Files ***********************************************************/
#include <stddef.h>
#include <string.h>
#include "fir_accelerator.h"
#include "gMDMA.h"
#include "GulTranslateAddr.h"


/***  Debug Support  **********************************************************/

// FIR accelerator debugging support. Note that for profiling and production
// use, DEBUG_FIR_ACCEL should be set to zero, ADI_DEBUG should NOT be 
// defined for the EMDMA drivers, and compiler optimization flags should
// be SET as stated above. 
#define DEBUG_FIR_ACCEL (1)

#if (DEBUG_FIR_ACCEL)
   #define DEBUG_ACCEL(expr) (expr)
#else /* DEBUG_FIR_ACCEL */
   #define DEBUG_ACCEL(expr)

    // Turn off asserts
    #if !defined(NDEBUG)
        #define NDEBUG
    #endif /* NDEBUG */
#endif /* DEBUG_FIR_ACCEL */
#include "assert.h"


/***  External Variable References  *******************************************/


/***  Module Macros  **********************************************************/

// Verifies requested memory size does not exceed actual memory size
#define VERIFY_ITEM_SIZE( reqMemSize, memSize )  assert( (memSize) >= (reqMemSize) )


/***  Module Types  ***********************************************************/

// Defines parameters used to create task.  Data residing in work memory
// persists for the life of the accelerator task.  
typedef struct
{
    firAccelWorkMemory_t *pWork;            // Pointer to work memory
    const int *filterLengths;               // Pointer to filter lengths
    const int *filterMapping;               // Pointer to filter mappings
    const float **firCoeffBuff;             // Pointer to FIR coefficients
    const unsigned int taskMemorySize;      // Task memory size

} firTaskParams_t;


/***  Module Variables  *******************************************************/

// Defines EMDMA stream to use for accelerator models
#define FIR_ACCEL_EMDMA_STREAM  GMDMA_STREAM_0


/***  Local Function Declarations  ********************************************/

static bool create_fir_accel_task( firTaskParams_t *params );
static void fir_accel_queue_task( firAccelWorkMemory_t *pWork  );
static void setup_variable_work_memory( firAccelWorkMemory_t *pWork, unsigned int *taskMemSize );
static void emdma_config( firAccelWorkMemory_t *pWork );
static void emdma_create_tcb_list( firAccelWorkMemory_t *pWork );
static inline void emdma_start_transfer( firAccelWorkMemory_t *pWork );
static void emdma_cancel( firAccelWorkMemory_t *pWork );
static void emdma_callback( void *pCBParam, unsigned int Event, void *pArg );


/***  Module Functions  *******************************************************/

/**
 *  fir_accelerator_init()
 *
 *  @brief Initializes data used by FIR accelerator and creates task list
 *
 *  @param[out]    reserved       Artifact of code generation, not intended for use here
 *  @param[in]     input          Input audio buffer pointer
 *  @param[in]     coeffs         Coefficient buffer pointer
 *  @param[in,out] states         State buffer pointer
 *  @param[in]     statesSize     Size of states buffer
 *  @param[in]     filterLengths  Pointer to array of filter lengths
 *  @param[in]     inputMapping   Pointer to array of input maps
 *  @param[in]     filterMapping  Pointer to array of filter maps
 *  @param[in]     frameSize      Number of samples per frame
 *  @param[in]     numIn          Number of input channels
 *  @param[in]     numOut         Number of output channels
 *  @param[in,out] workMemory     Pointer to memory buffer allocated by model, used by this
 *                                accelerator wrapper
 *  @param[in]     workMemorySize Size of work memory allocated by model
 *
 *  @warning It is not recommended to use reserved memory. When passed in, this will actually
 *           point to the first element of workMemory. The reason for this has to do with
 *           making sure Simulink does not optimize out the call to this function. So it is
 *           not recommended to use, as it's value may change outside of these functions.
 */
void fir_accelerator_init( unsigned int *reserved,
                           const float *input,
                           const float *coeffs,
                           float *states,
                           unsigned int statesSize,
                           const unsigned int *filterLengths,
                           const unsigned int *inputMapping,
                           const unsigned int *filterMapping,
                           unsigned int frameSize,
                           unsigned int numFilters,
                           unsigned int numIn,
                           unsigned int numOut,
                           unsigned int *workMemory,
                           unsigned int workMemorySize )
{
    // Get pointer to the work memory
    // Note that the work memory will always be initialized to zero 
    // by the model initialize function prior to calling this function.
    firAccelWorkMemory_t *pWork = (firAccelWorkMemory_t *) workMemory;

    // Verify work memory is large enough
    unsigned int requiredWorkMemSize = sizeof(*pWork) + 
                                       FIR_VARIABLE_DATA_SECTION_SIZE( numIn, numOut, numOut, frameSize );
    requiredWorkMemSize /= sizeof(workMemory[0]);                                       
    assert( workMemorySize >= requiredWorkMemSize );
    
    // Number of outputs is the number of channels
    int numChan = numOut;

    // Temporary storage of pointers to filter coefficients.  There can be fewer filter 
    // coefficient sets than actual filters when some filters share the same coefficient
    // sets. This buffer stores those pointers before they are mapped to the number of
    // filters/channels buffer of pointers for TCB initialization.
    // Buffer located within variable data memory section.
    const float *firCoeffSetsPtrs[numChan];

    // FIR accelerator maximum coefficient size for all channels sharing a single input.
    // Buffer located within variable data memory section.
    int firMaxCoeffSizePerInput[numIn];

    // FIR accelerator filter coefficient buffer of pointers to Simulink generated
    // coefficient buffers.
    // Buffer located within variable data memory section.
    const float *firCoeffBuff[numChan];

    // Initialize the work memory and get pointers to variable data section
    // members
    pWork->frameSize = frameSize;
    pWork->numInputs = numIn;
    pWork->numOutputs = numOut;
    pWork->numChannels = numChan; 

    // Setup the variable data work memory space and get task memory size
    unsigned int taskMemorySize;
    setup_variable_work_memory( pWork, &taskMemorySize );

    // Initialize work memory input channel map for future reference
    int *pFirInputChannelMap = pWork->firInputChannelMap;
    int *inputMap = (int*) inputMapping;
    for( int i = 0; i < numChan; i++ )
    {
        pFirInputChannelMap[i] = inputMap[i];
    }

    // ------------------------------------------------------------------------
    // Generate all data required to create the FIR accelerator task
    // ------------------------------------------------------------------------
    const int count = numFilters;
    for( int i = 0, j = 0; i < count; i++ )
    {
        firCoeffSetsPtrs[i] = &coeffs[j];
        j +=  filterLengths[i];
    }

    const int numInputs = numIn;
    for( int i = 0; i < numInputs; i++ )
    {
        firMaxCoeffSizePerInput[i] = 0;
    }
    for( int i = 0; i < numChan; i++ )
    {
        int chan = pFirInputChannelMap[i];
        int coeffLen =  filterLengths[filterMapping[i]];

        // Get the maximum coefficient length of channel filters sharing a common
        // input for each input. Note, coefficients passed in are already padded
        // for input sharing and the lengths are the padded lengths. It is necessary
        // to go through all channels though to insure the maximum size for each
        // input is obtained.
        if ( firMaxCoeffSizePerInput[chan] != 0 )
        {
            // Verify filter lengths are padded for shared inputs
            assert( firMaxCoeffSizePerInput[chan] == coeffLen );
        }
        else
        {
            firMaxCoeffSizePerInput[chan] = coeffLen;
        }
    }

    unsigned int requiredStatesSize = 0;
    float **pCircInputBuffs = pWork->firCircInputDmaBuffs;
    int *pCircInputBuffsSize = pWork->firCircInputDmaBuffsSize;
    for( int i = 0, j = 0; i < numInputs; i++ )
    {
        pCircInputBuffs[i] = &states[j];
        pCircInputBuffsSize[i] = firMaxCoeffSizePerInput[i] + frameSize - 1;
        requiredStatesSize += pCircInputBuffsSize[i];
        j += pCircInputBuffsSize[i];
    }

    // Verify states memory is large enough
    assert( statesSize >= requiredStatesSize );

    float **pFirInputBuff = pWork->firInputBuff;
    int *pFirInputBuffSize = pWork->firInputBuffSize;
    float **pFirOutputBuff = pWork->firOutputBuff;
    float *pFirCircOutputDmaBuffs = pWork->firCircOutputDmaBuffs;
    for( int i = 0; i < numChan; i++ )
    {
        pFirInputBuff[i] = pCircInputBuffs[pFirInputChannelMap[i]];
        pFirInputBuffSize[i] = pCircInputBuffsSize[pFirInputChannelMap[i]];
        pFirOutputBuff[i] = &pFirCircOutputDmaBuffs[i * frameSize];
        firCoeffBuff[i] = firCoeffSetsPtrs[filterMapping[i]];
    }

    // Current audio input pointer for EMDMA optimization
    pWork->pAudioInput = input;

    // Set flag to indicate input/output TCB indexes should not be updated
    pWork->startUpdates = false;

    // EMDMA ISR callback counter for debugging
    pWork->emdmaCallbackCount = 0;

    // Create initial EMDMA TCB list and setup EMDMA engine
    emdma_config( pWork );    

    // Create the FIR accelerator task
    firTaskParams_t params =
    {
        pWork,                          // Pointer to work memory
        (const int*) filterLengths,     // Pointer to filter lengths
        (const int*) filterMapping,     // Pointer to filter mappings
        firCoeffBuff,                   // Pointer to FIR coefficients
        taskMemorySize                  // Task memory size
    };
    bool result = create_fir_accel_task( &params );

    // Set flag indicating initialization has completed successfully. 
    if ( result == true )
    {
        pWork->initComplete = true;
    }

} /* fir_accelerator_init() */


/**
 *  fir_accelerator_process()
 *
 *  @brief Run-time processing of FIR accelerator, retrieving accelerator outputs
 *         from previous frame and initiating accelerator processing of current
 *         frame.
 *
 *  @param[out]    output       Output audio buffer pointer.
 *  @param[in]     input        Input audio buffer pointer
 *  @param[in,out] workMemory   Pointer to memory buffer allocated by model, used by this
 *                              accelerator wrapper
 *  @param[in]     numOut       Unused here: Number of outputs used to size dummy output
 *                              properly for generic code generation  
 *
 */
void fir_accelerator_process( float *output,
                              const float *input, 
                              unsigned int *workMemory,
                              unsigned int numOut )
{
    // Grab data objects from work memory
    firAccelWorkMemory_t *pWork = (firAccelWorkMemory_t *) workMemory;
    
    // Exit if accelerator initialization has not completed or RMDL
    // process has started
    if ( pWork->initComplete == false )
    {
        return;
    }

    // Copy FIR accelerator output buffers from previous frame to Simulink model output
    // buffers for further processing
    float *pFirOut = &pWork->firOutputBuff[0][0];
    const int frameSize = pWork->frameSize;
    const int count = pWork->numChannels * frameSize;
    for ( int i = 0; i < count; i++ )
    {
        output[i] = pFirOut[i];
    }

    // Calculate FIR accelerator circular input buffer frame index for each input
    const int numInputs = pWork->numInputs;
    ADI_EMDMA_DESC_STANDARD_CIRCULAR *pEmdmaTcbList = pWork->emdmaTcbList;
    int *pFirFrameStartIndex = pWork->firFrameStartIndex;
    int *pFirCircInputDmaBuffsSize = pWork->firCircInputDmaBuffsSize;
    for ( int i = 0; i < numInputs; i++ )
    {
        pFirFrameStartIndex[i] = circindex( pFirFrameStartIndex[i], 
                                            frameSize, 
                                            pFirCircInputDmaBuffsSize[i] );
    }

    // Only update EMDMA TCB list if audio input pointer changes
    // Compiler optimization for false condition 
    if ( expected_false(pWork->pAudioInput != input) )
    {
        pWork->pAudioInput = input;

        const float *pInput;
        for ( int i = 0; i < numInputs; i++ )
        {
            // Input pointer for current channel
            pInput = &input[i * frameSize];

            // Update audio input buffer pointer for source (internal) of copy
            // to circular input buffer
            pEmdmaTcbList->pIntStartAddress = (void*) TranslateAddr( (unsigned int) &pInput[0] );
        }
    }

    // Start EMDMA transfer for next frame to copy audio inputs into
    // accelerator circular input buffers
    emdma_start_transfer( pWork );

} /* fir_accelerator_process() */


/**
 *  fir_accelerator_cancel()
 *
 *  @brief FIR accelerator cancel of further audio frame step processing and of
 *         any EMDMA or accelerator DMA transfer currently in progress.
 *
 *  @param[in,out] workMemory   Pointer to memory buffer allocated by model and used
 *                              by FIR Accelerator
 * 
 *  @returns Status of 0 for success and 1 for failure
 */
unsigned int fir_accelerator_cancel( unsigned int *workMemory )
{
    // Grab data objects from work memory
    firAccelWorkMemory_t *pWork = (firAccelWorkMemory_t *)workMemory;

    // Exit if RMDL process has already started and cleared the initialization
    // complete flag to avoid the overhead of cancelling the fast copy at each
    // step
    if ( pWork->initComplete == false )
    {
        return 0;
    }

    // Set the initialization flag to false to stop further step processing
    // on audio frame boundaries to prepare for the RMDL process
    pWork->initComplete = false;

    // Cancel any EMDMA transfer currently in progress
    gMdmaResult_t eResult = gMdmaCancelFastCopy( pWork->emdmaFastReuseCopyReq );
    assert(eResult == GMDMA_SUCCESS);

    // Cancel any FIR accelerator task processing currently in progress
    gHamResult_t aResult = gHamCancelTask( FIR_ACCEL, pWork->hFirTaskHandle );
    assert( aResult == GHAM_SUCCESS );

    return (unsigned int) (eResult | aResult);

} /* fir_accelerator_cancel() */


/**
 *  create_fir_accel_task()
 *
 *  @brief Creates the FIR accelerator task from data defining model instance
 *
 *  @param[in]   params    Parameter data required to create task
 *
 */
static bool create_fir_accel_task( firTaskParams_t *params )
{
    // Initialize FIR task ACM channel information.  Only the last channel of the task's
    // callback has interrupts enabled.
    firAccelWorkMemory_t *pWork = params->pWork;
    gHamFirTcb_t *ChannelTcb = pWork->firTaskMemory->pChannelList;

    // Setup loop variables for loop optimization
    const int frameSize         = pWork->frameSize;
    const int numChannels       = pWork->numChannels;
    const int *filterLengths    = params->filterLengths;
    const int *filterMap        = params->filterMapping;
    float **pInput              = pWork->firInputBuff;
    float **pOutput             = pWork->firOutputBuff;
    const int *pInputSize       = pWork->firInputBuffSize;
    const int *pChannelMap      = pWork->firInputChannelMap;
    const float** pCoeff        = params->firCoeffBuff;

    FirAccelSecondaryGlobalCtl_t FirSgCtlParams = 
    {
        ADI_FIR_FLOAT_ROUNDING_MODE_IEEE_ROUND_TO_NEAREST_EVEN,     // Rounding Mode
        false,                                                      // Fixed Point enable
        ADI_FIR_FIXED_INPUT_FORMAT_UNSIGNED_INTEGER                 // Fixed Point format
    };
    FirAccelChannelCtl_t FirChCtlParams =  
    {
        frameSize,                      // Window Size
        0,                              // Tap Length
        0,                              // Priority
        ADI_FIR_SAMPLING_SINGLE_RATE,   // Sampling Type 
        1,                              // Sampling Ratio 
        false,                          // Generate Trigger on Completion 
        false,                          // Wait for Trigger to Start 
        0                               // Callback Enable 
    };

    uint32_t FirSgCtl = gHamCreateAccelFirSgCtlRegValue( &FirSgCtlParams );

    for( int i = 0; i < numChannels; i++ )
    {
        uint32_t FirCtl2;

        // Set channel filter tap length and callback enable parameters.  Callback is only
        // called on task completion.
        const int filterLen = filterLengths[filterMap[i]];
        FirChCtlParams.tapLength      = filterLen;                      // Tap Length 
        FirChCtlParams.callbackEnable = (i == (numChannels - 1));       // Callback enabled only
                                                                        // on last channel 
        FirCtl2 = gHamCreateAccelFirCtl2RegValue( &FirChCtlParams );

        // Note: GHAM links TCB blocks (pNextTCB field) together to create task 
        // Note: FIR Software control register 1 (nFirSoftwareControl1 field) is reserved for 
        //       driver usage 

        // Rounding Mode, Fixed Point Enable and Formatting 
        ChannelTcb[i].nFirSecondaryControl1 = FirSgCtl;

        // Coefficient, Output, and Input Buffers 
        ChannelTcb[i].nCoeffCount    = filterLen;                       // Coefficient Count  
        ChannelTcb[i].nCoeffModify   = 1;                               // Coefficient Modify  
        ChannelTcb[i].pCoeffIndex    = (void *)&pCoeff[i][0];           // Coefficient Index  
        ChannelTcb[i].pOutputBase    = (void *)&pOutput[i][0];          // Output Base  
        ChannelTcb[i].nOutputCount   = frameSize;                       // Output Count  
        ChannelTcb[i].nOutputModify  = 1;                               // Output Modify  
        ChannelTcb[i].pOutputIndex   = (void *)&pOutput[i][0];          // Output Index  
        ChannelTcb[i].pInputBase     = (void *)&pInput[i][0];           // Input Base  
        ChannelTcb[i].nInputCount    = pInputSize[i];                   // Input Count  
        ChannelTcb[i].nInputModify   = 1;                               // Input Modify  
        ChannelTcb[i].pInputIndex    = (void *)&pInput[i][0];           // Input Index 

        // Window Size, Tap Length, Priority, Sampling Type, Sampling Ratio, Generate Trigger, 
        // Wait for Trigger, and Callback Enable 
        ChannelTcb[i].nFirControl2 = FirCtl2;
    }

    // Create Accelerator Task 
    gHamAccelTask_t taskParams = 
    {
        FIR_ACCEL,                          // FIR task 
        {                                   // Pointer to task memory with unlinked TCB channel list 
            .pTaskMemoryFir = (gHamFirTask_t*) pWork->firTaskMemory 
        },
        numChannels,                        // Number TCB channels in task list 
        params->taskMemorySize,             // Overall task memory size 
        &(pWork->hFirTaskHandle)    // Location to store handle of created task 
    };

    gHamResult_t result = gHamCreateAccelTask( &taskParams );
    assert( result == GHAM_SUCCESS );

    return (result == GHAM_SUCCESS);

} /* create_fir_accel_task() */


/**
 *  fir_accel_queue_task()
 *
 *  @brief Queues FIR accelerator task for execution
 *
 *  @param[in,out] pWork   Pointer to work memory buffer allocated by model and
 *                         containing data defining this model instance
 *
 */
static void fir_accel_queue_task( firAccelWorkMemory_t *pWork  )
{
    gHamResult_t result = GHAM_SUCCESS;

    /* Verify pointer is valid */
    assert( pWork != NULL );

    // Update circular input/output buffer pointers starting only after the first
    // time the task has been queued
    if ( pWork->startUpdates ) 
    {
        // Setup loop variables for loop optimization
        gHamFirTcb_t *pList = pWork->firTaskMemory->pChannelList;
        float **pInput            = pWork->firInputBuff;
        float **pOutput           = pWork->firOutputBuff;
        const int *pStartIndex    = pWork->firFrameStartIndex;
        const int *pChannelMap    = pWork->firInputChannelMap;
        const int numChannels     = pWork->numChannels;

        // Update circular buffer pointer indexes
        for ( int i = 0; i < numChannels; i++ )
        {
            // Map input circular buffer index to channel and get input and
            // output buffer addresses
            int index = pStartIndex[pChannelMap[i]];
            unsigned int inputAddr = (unsigned int) &pInput[i][index];
            unsigned int outputAddr = (unsigned int) &pOutput[i][0]; 

            pList[i].pOutputIndex  = (void *)TranslateAddr( outputAddr );  // Output Index
            pList[i].pInputIndex   = (void *)TranslateAddr( inputAddr );   // Input Index
        }
    }
    else 
    { 
        pWork->startUpdates = true; 
    }

    // Queue up the task to the FIR accelerator and start processing
    result = gHamQueueAccelTask( FIR_ACCEL, pWork->hFirTaskHandle, NULL );
    assert( result == GHAM_SUCCESS );

} /* fir_accel_queue_task() */


/**
 *  emdma_config()
 *
 *  @brief Initializes TCB list and opens the Enhanced Memory DMA (EMDMA) driver
 *
 *  @param[in,out] pWork        Pointer to work memory buffer allocated by model
 *                              and containing data defining this model instance
 *
 */
static void emdma_config( firAccelWorkMemory_t *pWork )
{
    gMdmaResult_t eResult = GMDMA_SUCCESS;

    // Verify pointer is valid
    assert( pWork != NULL );   

    // Note that the EMDMA driver stream must have already been opened for
    // fast reuse mode of operation by the platform software using the 
    // Memory DMA (MDMA) manager.

    // Create initial EMDMA TCB list
    emdma_create_tcb_list( pWork );

    // Create an EMDMA fast reuse copy request for a standard write copy. 
    // Enable circular buffering for external buffer and circular buffer
    // ExtStartAddress write back on transfer completion.  Provide callback
    // function and parameter specific to this fast copy request.
    eResult = gMdmaCreateFastCopy( FIR_ACCEL_EMDMA_STREAM,
                                   ADI_EMDMA_STANDARD_WRITE_MODE,
                                   ADI_EMDMA_OTFDIR_WRITE,
                                   (void*) pWork->emdmaTcbList,
                                   true,           // Enable external circular buffer
                                   true,           // Enable TCB ext start addr write back
                                   emdma_callback,
                                   pWork,
                                   &pWork->emdmaFastReuseCopyReq );
    assert( eResult == GMDMA_SUCCESS );

} /* emdma_config() */


/**
 *  emdma_create_tcb_list()
 *
 *  @brief Creates the initial TCB list
 *
 *  @param[in,out] pWork        Pointer to work memory buffer allocated by model
 *                              and containing data defining this model instance
 *
 */ 
static void emdma_create_tcb_list( firAccelWorkMemory_t *pWork )
{
    // Verify pointers are valid
    assert( pWork != NULL );
    
    ADI_EMDMA_DESC_STANDARD_CIRCULAR *pTcbList = pWork->emdmaTcbList;
    const int frameSize = pWork->frameSize;
    const int numInputs = pWork->numInputs;
    float **pCircInputBuf = pWork->firCircInputDmaBuffs; 
    int *pCircBufSize = pWork->firCircInputDmaBuffsSize;
    const float *pInput = pWork->pAudioInput;
    unsigned int rdChainPtr;

    // ------------------------------------------------------------------------
    // Circular TCBs are used for all input transfers and the external TCB
    // items refer to the circular buffer. EMDMA transfers are setup with
    // write back enabled so that the external start address is updated
    // after each transfer to point to the location of the next transfer.
    // This way external start addresses never have to be updated by software
    // after TCB creation. 
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    // Initialize all fields of all TCBs in the list and next pointers.
    // Note that the work memory will always be initialized to zero by the
    // model initialize function prior to calling this function. 
    // ------------------------------------------------------------------------ 
    for ( int i = 0; i < numInputs; i++ )
    {
        // Setup to point to next TCB in the list or terminate the list
        if ( i == (numInputs - 1) )
        {
            // Terminate list and generate interrupt on completion of last TCB
            // in list             
            pTcbList[i].pNext = NULL;
            rdChainPtr = BITM_EMDMA_CHNPTR_PCI;
        }
        else
        {
            // Set pointer to next TCB and TCB chain pointer to translated 
            // address of last field of next TCB, and to write transaction
            pTcbList[i].pNext = &pTcbList[i+1];
            rdChainPtr = (unsigned int) pTcbList[i].pNext + 
                         (sizeof(ADI_EMDMA_DESC_STANDARD_CIRCULAR) - sizeof(void*));
            rdChainPtr = (unsigned int) TranslateAddr( rdChainPtr );
            rdChainPtr |= ADI_EMDMA_OTFDIR_WRITE;                        
        }

        // Input pointer for current input channel
        unsigned int input = (unsigned int) &pInput[i * frameSize];

        // Translated address of circular input buffer
        void *pCircInputBufAddr = (void*) TranslateAddr( (unsigned int) &pCircInputBuf[i][0] );

        // Write frame of data to circular input buffer (external) from audio
        // input buffer (internal)
        pTcbList[i].Chnptr = rdChainPtr;
        pTcbList[i].ExtLength = pCircBufSize[i];
        pTcbList[i].pExtBaseAddress = pCircInputBufAddr;
        pTcbList[i].ExtModify = 1;      // 32 bit word increment
        pTcbList[i].pExtStartAddress = pCircInputBufAddr;
        pTcbList[i].Count = frameSize;
        pTcbList[i].IntModify = 1;      // 32 bit word increment
        pTcbList[i].pIntStartAddress = (void*) TranslateAddr( input );
    }

} /* emdma_create_tcb_list() */


/**
 *  emdma_start_transfer()
 *
 *  @brief Starts the Enhanced Memory DMA transfer for model instance
 *         TCB list
 *
 *  @param[in,out] pWork   Pointer to work memory buffer allocated by model and
 *                         containing data defining this model instance
 * 
 *  @pre TCB list is initialized for DMA transfer
 *
 */
static inline void emdma_start_transfer( firAccelWorkMemory_t *pWork )
{
    gMdmaResult_t eResult = GMDMA_SUCCESS;

    // Submit a fast reuse mode EMDMA copy request to initiate the copy
    // immediately if no existing copy is in progress or place on the
    // wait list for processing after all copies on the list have completed.
    // Note that this allows multiple instances of the accelerator module
    // to run together. 
    eResult = gMdmaSubmitFastCopy( pWork->emdmaFastReuseCopyReq );
    assert( eResult == GMDMA_SUCCESS );

} /* emdma_start_transfer() */


/**
 *  emdma_cancel()
 *
 *  @brief Cancels the Enhanced Memory DMA transfer for the model instance
 *         EMDMA stream, if in progress or on the wait list
 *
 *  @param[in,out] pWork   Pointer to work memory buffer allocated by model and
 *                         containing data defining this model instance
 * 
 */
static void emdma_cancel( firAccelWorkMemory_t *pWork )
{
    gMdmaResult_t eResult = GMDMA_SUCCESS;

    // Verify pointer is valid
    assert(pWork != NULL);

    /* Cancel the copy request for this instance of the model */
    eResult = gMdmaCancelFastCopy(pWork->emdmaFastReuseCopyReq);
    assert(eResult == GMDMA_SUCCESS);    

} /* emdma_cancel() */


/**
 *  emdma_callback()
 * 
 *  @brief EMDMA manager interrupt callback used for queuing up next
 *         accelerator task and debug interrupt count information
 *
 *  @param[in]  pCBParam    Callback parameter supplied by application
 *  @param[in]  Event       Callback event
 *  @param[in]  pArg        Callback argument
 *
 */
static void emdma_callback( void *pCBParam, unsigned int Event, void *pArg )
{
    firAccelWorkMemory_t *pWork = pCBParam; 

    /* CASEOF (Event) */
    switch ( (ADI_EMDMA_EVENT)Event )
    {
        /* CASE (Processed an EMDMA TCB list) */
        case (ADI_EMDMA_EVENT_STANDARD_WRITE_DESC_PROCESSED):
        {
            DEBUG_ACCEL( pWork->emdmaCallbackCount += 1 );

            // Queue up the next FIR accelerator processing task
            fir_accel_queue_task( pWork );

            break;
        }

        default:
            break;
    }

} /* emdma_callback() */


/**
 *  setup_variable_work_memory()
 *
 *  @brief Allocates data within work memory variable data space for accelerator 
 *
 *  @param[in,out] pWork            Pointer to work memory buffer allocated by model and
 *                                  containing data defining this model instance
 *  @param[out]    taskMemorySize   Pointer to location to store size of task memory allocated 
 *
 */
static void setup_variable_work_memory( firAccelWorkMemory_t *pWork, unsigned int *taskMemSize )
{
    assert( pWork != NULL );
    assert( taskMemSize != NULL );

    unsigned int numIn = pWork->numInputs;
    unsigned int numOut = pWork->numOutputs;
    unsigned int numCh = pWork->numChannels;
    unsigned int varMemSize = FIR_VARIABLE_DATA_SECTION_SIZE( numIn, numOut, 
                                                              numCh, pWork->frameSize );
    
    char *pVarBase = pWork->data;
    char *pVarMem = pVarBase;
    unsigned int size;

    size = (numIn * sizeof(pWork->emdmaTcbList[0]));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->emdmaTcbList = (ADI_EMDMA_DESC_STANDARD_CIRCULAR*) pVarMem; 
    pVarMem += size; 

    size = (sizeof(*pWork->firTaskMemory) + numOut * sizeof(gHamFirTcb_t));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->firTaskMemory = (gHamFirTask_t*) pVarMem; 
    pVarMem += size; 
    *taskMemSize = size;

    size = (numOut * pWork->frameSize * sizeof(pWork->firCircOutputDmaBuffs[0]));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->firCircOutputDmaBuffs = (float*) pVarMem; 
    pVarMem += size; 

    size = (numCh * sizeof(pWork->firInputChannelMap[0]));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->firInputChannelMap = (int*) pVarMem;
    pVarMem += size; 

    size = (numIn * sizeof(pWork->firCircInputDmaBuffs[0]));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->firCircInputDmaBuffs = (float**) pVarMem;  
    pVarMem += size; 

    size = (numIn * sizeof(pWork->firCircInputDmaBuffsSize[0]));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->firCircInputDmaBuffsSize = (int*) pVarMem;  
    pVarMem += size; 

    size = (numCh * sizeof(pWork->firInputBuff[0]));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->firInputBuff = (float**) pVarMem;  
    pVarMem += size; 

    size = (numCh * sizeof(pWork->firInputBuffSize[0]));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->firInputBuffSize = (int*) pVarMem;  
    pVarMem += size; 

    size = (numOut * sizeof(pWork->firOutputBuff[0]));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->firOutputBuff = (float**) pVarMem;  
    pVarMem += size; 

    size = (numIn * sizeof(pWork->firFrameStartIndex[0]));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->firFrameStartIndex = (int*) pVarMem;  
    pVarMem += size; 

} /* setup_variable_work_memory() */
