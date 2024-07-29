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
 *  @file sharedDelay.c
 *  @brief  Shared delay CRL implementation for DMA based shared delay. Delay
 *          buffer resides in L2 (or L1) memory. 
 *
 *  @note Optimization settings for speed should include setting compiler
 *        flags -Ov100 and -ipa (if possible) for the top level build.
 *        Also, it is important to note that this module was NOT designed 
 *        to support the usage of cached data.
 *     
 *  @ingroup dsp
 *
 */

/***  Include Files ***********************************************************/

#include <builtins.h>
#include <stddef.h>
#include <string.h>
#include <GulTranslateAddr.h>
#include "gMDMA.h"
#include <sharedDelay.h>


/***  Debug Support  **********************************************************/

// Shared Delay debugging support. Note that for profiling and production
// use, DEBUG_SHARED_DELAY should not be defined, ADI_DEBUG should NOT be 
// defined for the EMDMA drivers, and compiler optimization flags should
// be SET as stated above. 
#define DEBUG_SHARED_DELAY (1)

#if (DEBUG_SHARED_DELAY)
   #define DEBUG_PD(expr)    (expr)
   #define DEBUG_PTR(ptr)    (ptr)
#else /* DEBUG_SHARED_DELAY */
   #define DEBUG_PD(expr)
   #define DEBUG_PTR(expr)   NULL

    // Turn off asserts
    #if !defined(NDEBUG)
        #define NDEBUG
    #endif /* NDEBUG */
#endif /* DEBUG_SHARED_DELAY */
#include "assert.h"


/***  External Variable References  *******************************************/


/***  Module Macros  **********************************************************/

// Verifies requested memory size does not exceed actual memory size
#define VERIFY_ITEM_SIZE(reqMemSize, memSize) assert((memSize) >= (reqMemSize))

// Defines descriptor offset for EMDMA delay line descriptor (TCB).
// Note that this is the DMA hardware TCB size and it does not include 
// the next pointer included in the ADI driver descriptor.
#define EMDMA_CIRC_TCB_OFFSET ((sizeof(ADI_EMDMA_DESC_DELAYLINE) - \
                                sizeof(void*)))

// Defines EMDMA stream to use for delay models
#define SHARED_DELAY_EMDMA_STREAM  GMDMA_STREAM_1                                


/***  Module Types  ***********************************************************/


/***  Module Variables  *******************************************************/


/***  Local Function Declarations  ********************************************/

static void emdma_callback( void *pCBParam, unsigned int Event, void *pArg );
static void emdma_config( sharedDelayWorkMemory_t *pWork, 
                          const int *delayChain );
static void emdma_create_tcb_list( sharedDelayWorkMemory_t *pWork, 
                                   const int *delayChain );
static inline void emdma_start_transfer( sharedDelayWorkMemory_t *pWork );
static void emdma_cancel( sharedDelayWorkMemory_t *pWork );
static inline void* translate_addr( float *memoryAddr );
static void setup_variable_work_memory( sharedDelayWorkMemory_t *pWork );


/***  Module Functions  *******************************************************/

/**
 *  shared_delay_mdma_init()
 *
 *  @brief Initializes data used for DMA based implementation of shared delay
 *
 *  @param[out]    reserved             Artifact of code generation, not intended for use here
 *  @param[in]     input                Input audio buffer pointer
 *  @param[in]     delayBuffer          Pointer to delay buffer allocated by model 
 *  @param[in]     delayBufferSize      Size of delay buffer
 *  @param[in]     fastDmaBuffer        Pointer to fast DMA output buffer allocated by model 
 *  @param[in]     fastDmaBufferSize    Size of fast DMA buffer
 *  @param[in]     delayChain           Pointer to vector of delays for all channels
 *  @param[in]     inputMapping         Pointer to array of input mappings
 *  @param[in]     numInputs            Number of input channels
 *  @param[in]     numOutputs           Number of output channels
 *  @param[in]     frameSize            Number of samples per frame
 *  @param[in,out] workMemory           Pointer to memory buffer allocated by model and used
 *                                      by shared delay
 *  @param[in]     workMemorySize       Size of work memory allocated by model
 *
 *  @warning It is not recommended to use reserved memory. When passed in, this will actually
 *           point to the first element of workMemory. The reason for this has to do with
 *           making sure Simulink does not optimize out the call to this function. So it is
 *           not recommended to use, as it's value may change outside of these functions.
 */
void shared_delay_mdma_init(float *reserved,
                            const float *input,
                            float *delayBuffer,
                            const int delayBufferSize,
                            float *fastDmaBuffer,
                            const int fastDmaBufferSize,                                   
                            const int *delayChain,
                            const int *inputMapping,
                            const int numInputs,
                            const int numOutputs,
                            const int frameSize,
                            unsigned int *workMemory,
                            const int workMemorySize)
{
    // Get pointer to the work memory
    // Note that the work memory will always be initialized to zero
    // by the model initialize function prior to calling this function.
    sharedDelayWorkMemory_t *pWork = (sharedDelayWorkMemory_t *)workMemory;

    // Verify work memory is large enough
    int requiredWorkMemSize = sizeof(*pWork) + 
                                     DMA_VARIABLE_DATA_SECTION_SIZE(numInputs, 
                                     numOutputs);
    requiredWorkMemSize /= sizeof(workMemory[0]);
    assert(workMemorySize >= requiredWorkMemSize);

    // Initialize the work memory and get pointers to variable data section
    // members
    pWork->frameSize = frameSize;
    pWork->numInputs = numInputs;
    pWork->numOutputs = numOutputs;

    // Setup the variable data work memory space
    setup_variable_work_memory(pWork);

    // Verify delay chain elements are valid and zero based input mappings
    // are valid
    int minMap = numInputs;
    int maxMap = 0;
    for (int i = 0; i < numOutputs; i++)
    {
        assert(delayChain[i] >= 0);
        
        int inputCh = inputMapping[i];
        minMap = min(minMap, inputCh);
        maxMap = max(maxMap, inputCh);
    }
    assert((minMap == 0) && (maxMap < numInputs));

    // Determine the EMDMA output tap count for each input
    int *pEmdmaTapCount = pWork->pEmdmaTapCount;
    for (int i = 0; i < numOutputs; i++)
    {
        int inputCh = inputMapping[i];
        ++pEmdmaTapCount[inputCh];
    }

    // Create output mapping that contains output channel/delay index for
    // each input channel's output taps in sequential order
    int *pOutputMapping = pWork->pOutputMapping;
    int tapIndex[numInputs];
    int startIndex[numInputs];
    int offset = 0;
    for (int i = 0; i < numInputs; i++)
    {
        tapIndex[i] = 0;
        startIndex[i] = offset;
        offset += pEmdmaTapCount[i];
    }
    for (int i = 0; i < numOutputs; i++)
    {
        int inputCh = inputMapping[i];
        int outIndex = startIndex[inputCh] + tapIndex[inputCh]++;
        pOutputMapping[outIndex] = i;
    }

    // Determine the total delay buffer length for all channel delays,
    // actual delay buffer length required, actual fast DMA buffer
    // length required, and delay buffer lengths for each input.
    int delayIndex = 0;
    int totalDelayBufLen = 0;
    for (int i = 0; i < numInputs; i++ )
    {
        int maxOutputDelay = 0;
        int outputCount = pEmdmaTapCount[i];

        for (int j = 0; j < outputCount; j++)
        {
            int mappedIndex = pOutputMapping[delayIndex++];
            maxOutputDelay = max(maxOutputDelay, delayChain[mappedIndex]);
        }

        // Delay buffer length is the maximum output tap delay for this
        // input, plus the frame size to account for the DMA write before
        // read delay line implementation
        int bufLength = (maxOutputDelay + frameSize);
        pWork->pDelayBufferLengthInput[i] = bufLength;

        totalDelayBufLen += bufLength; 
    }
    pWork->delayBufferLength = totalDelayBufLen;
    pWork->fastDmaBufferLength = numOutputs * frameSize;

    // Verify delay buffers are large enough
    assert( pWork->delayBufferLength <= delayBufferSize );
    assert( pWork->fastDmaBufferLength <= fastDmaBufferSize );

    pWork->delayBuffer = delayBuffer;
    pWork->delayBufferSize = delayBufferSize;

    pWork->fastDmaBuffer = fastDmaBuffer;
    pWork->fastDmaBufferSize = fastDmaBufferSize;

    // Initialize delay and fast DMA buffer pointers for each input
    int bufOffset = 0;
    int dmaIndex = 0;
    for (int i = 0; i < numInputs; i++)
    {   
        pWork->pDelayBufferInput[i] = &delayBuffer[bufOffset]; 
        pWork->pFastDmaBufferInput[i] = &fastDmaBuffer[dmaIndex * frameSize];

        // Increment index by the buffer length
        bufOffset += pWork->pDelayBufferLengthInput[i];

        // Increment DMA index by number of output taps for this input
        dmaIndex += pEmdmaTapCount[i];
    }

    // Initialize EMDMA output tap list pointers for each input
    delayIndex = 0;
    int* pEmdmaTranslatedTapList = pWork->pEmdmaTranslatedTapList;
    for (int i = 0; i < numInputs; i++)
    {
        pWork->pEmdmaTapList[i] = &pEmdmaTranslatedTapList[delayIndex];
        delayIndex += pEmdmaTapCount[i];
    }

    // Current audio input pointer for EMDMA optimization
    pWork->pAudioInput = input;

    // EMDMA ISR callback counter for debugging
    pWork->emdmaCallbackCount = 0;

    // Create initial EMDMA TCB list and setup EMDMA engine
    emdma_config(pWork, delayChain );

    // Set flag indicating initialization has completed successfully
    pWork->initComplete = true;

} /* shared_delay_mdma_init() */


/**
 *  shared_delay_mdma_process()
 *
 *  @brief DMA based run-time processing of pool of channel delays. Channel
 *         states are stored in slow delay buffers, with slow delay buffer
 *         only accessed by DMA. Outputs are populated by the core from 
 *         a fast DMA buffer filled with DMA.
 *
 *  @param[out]    output           Output audio buffer pointer
 *  @param[in]     input            Input audio buffer pointer
 *  @param[in]     delayChain       Pointer to vector of delays for all channels
 *  @param[in,out] workMemory       Pointer to memory buffer allocated by model and used
 *                                  by shared delay
 */
void shared_delay_mdma_process(float *output,
                               const float *input,
                               const int *delayChain,
                               unsigned int *workMemory)
{
    // Grab data objects from work memory
    sharedDelayWorkMemory_t *pWork = (sharedDelayWorkMemory_t *)workMemory;

    // Exit if shared delay initialization has not completed or RMDL
    // process has started
    if (pWork->initComplete == false)
    {
        return;
    }

    // Pointer to TCB list
    ADI_EMDMA_DESC_DELAYLINE *pTcbList = pWork->emdmaTcbList;

    // Pointers to channel input and output buffers
    const float *pInput;
    float *pOutput;

    // Number of input channels, output tap count pointers, output mapping
    // pointer, and frame size
    const int numInputs = pWork->numInputs;
    const int *pOutputTaps = pWork->pEmdmaTapCount;
    const int *pOutputMapping = pWork->pOutputMapping;
    const int frameSize = pWork->frameSize;

    // Pointer to fast DMA buffer
    float pm *pFastBufferBase = (float pm *) pWork->fastDmaBuffer;

    // Fast DMA buffer (not a circular buffer) and delay indices
    int fastBufferIndex = 0;
    int delayIndex = 0;

    // Loop counters
    int sampleIndex;
    int inputCh;
    int outputCh;

    //---------------------------------------------------------------
    // 1st Channel Loop: DMA Output Buffer Read to Audio Output 
    //---------------------------------------------------------------
    for (inputCh = 0; inputCh < numInputs; inputCh++)
    {
        // Input pointer for current input channel
        pInput = &input[inputCh * frameSize];

        // Output delay taps for this input channel
        const int numOutputTaps = pOutputTaps[inputCh];

        for (outputCh = 0; outputCh < numOutputTaps; outputCh++)
        {
            // Get the mapped output and delay index
            int mappedIndex = pOutputMapping[delayIndex++];

            // Audio output pointer for current mapped output channel
            pOutput = &output[mappedIndex * frameSize];

            // Delay for current output channel
            int delay = delayChain[mappedIndex];

            // Number of samples into the delay buffer
            int partialDelay = min(frameSize, delay);

            // Initial offset into fast DMA buffer
            int fastSrcIndex = fastBufferIndex;

            // Fill first part of output buffer from the fast DMA buffer
            for (sampleIndex = 0; sampleIndex < partialDelay; sampleIndex++, fastSrcIndex++)
            {
                pOutput[sampleIndex] = pFastBufferBase[fastSrcIndex];
            }

            // Fill the rest from the input buffer
            int srcIndex = 0;
            for (sampleIndex = partialDelay; sampleIndex < frameSize; sampleIndex++, srcIndex++)
            {
                pOutput[sampleIndex] = pInput[srcIndex];
            }

            // Advance fast buffer index to next output channel
            fastBufferIndex += frameSize;
        }

    } // end of 1st channel loop

    // Only update TCBs if audio input pointer changes
    // Compiler optimization for false condition 
    if (expected_false(pWork->pAudioInput != input))
    {
        pWork->pAudioInput = input;

        //-----------------------------------------------------------------
        // 2nd Channel Loop: Delay Buffer Write from Audio Input
        //-----------------------------------------------------------------
        for (inputCh = 0; inputCh < numInputs; inputCh++)
        {
            // Input pointer for current input channel
            pInput = &input[inputCh * frameSize];

            pTcbList[inputCh].pIntWriteStartAddress = translate_addr((float*) pInput);
        } // end of 2nd channel loop

    }

    // Start the DMA transfers to update delay lines in preparation for the
    // next frame
    emdma_start_transfer(pWork);

} /* shared_delay_mdma_process() */


/**
 *  shared_delay_mdma_cancel()
 *
 *  @brief DMA based cancel of further audio frame step shared delay processing 
 *         and cancellation of the DMA transfer if it is currently in progress.
 *
 *  @param[in,out] workMemory   Pointer to memory buffer allocated by model and used
 *                              by pool delay
 * 
 *  @returns Status of 0 for success and 1 for failure
 */
unsigned int shared_delay_mdma_cancel(unsigned int *workMemory)
{
    // Grab data objects from work memory
    sharedDelayWorkMemory_t *pWork = (sharedDelayWorkMemory_t *)workMemory;

    // Exit if RMDL process has already started and cleared the initialization
    // complete flag to avoid the overhead of cancelling the fast copy at each
    // step
    if (pWork->initComplete == false)
    {
        return 0;
    }

    // Set the initialization flag to false to stop further step processing
    // on audio frame boundaries to prepare for the RMDL process
    pWork->initComplete = false;

    // Cancel any DMA transfers currently in progress
    gMdmaResult_t eResult = gMdmaCancelFastCopy( pWork->emdmaFastReuseCopyReq );
    assert(eResult == GMDMA_SUCCESS);

    return (unsigned int) eResult;

} /* shared_delay_mdma_cancel() */


/**
 *  setup_variable_work_memory()
 *
 *  @brief Allocates data within work memory variable data space for accelerator 
 *
 *  @param[in,out] pWork   Pointer to work memory buffer allocated by model and
 *                         containing data defining this model instance
 *
 */
static void setup_variable_work_memory(sharedDelayWorkMemory_t *pWork)
{
    assert(pWork != NULL);

    int numIn = pWork->numInputs;
    int numOut = pWork->numOutputs;
    int varMemSize = DMA_VARIABLE_DATA_SECTION_SIZE(numIn, numOut);
   
    char *pVarBase = pWork->data;
    char *pVarMem = pVarBase;
    int size;

    size = (sizeof(*pWork->emdmaTcbList) * numIn);
    VERIFY_ITEM_SIZE((pVarMem - pVarBase + size), varMemSize);
    pWork->emdmaTcbList = (ADI_EMDMA_DESC_DELAYLINE*) pVarMem;
    pVarMem += size;

    size = (sizeof(*pWork->pDelayBufferInput) * numIn);
    VERIFY_ITEM_SIZE((pVarMem - pVarBase + size), varMemSize);
    pWork->pDelayBufferInput = (float**) pVarMem;
    pVarMem += size;

    size = (sizeof(*pWork->pDelayBufferLengthInput) * numIn);
    VERIFY_ITEM_SIZE((pVarMem - pVarBase + size), varMemSize);
    pWork->pDelayBufferLengthInput = (int*) pVarMem;
    pVarMem += size;    

    size = (sizeof(*pWork->pFastDmaBufferInput) * numIn);
    VERIFY_ITEM_SIZE((pVarMem - pVarBase + size), varMemSize);
    pWork->pFastDmaBufferInput = (float**) pVarMem;
    pVarMem += size;    

    size = (sizeof(*pWork->pEmdmaTapList) * numIn);
    VERIFY_ITEM_SIZE((pVarMem - pVarBase + size), varMemSize);
    pWork->pEmdmaTapList = (int**) pVarMem;
    pVarMem += size;

    size = (sizeof(*pWork->pEmdmaTapCount) * numIn);
    VERIFY_ITEM_SIZE((pVarMem - pVarBase + size), varMemSize);
    pWork->pEmdmaTapCount = (int*) pVarMem;
    pVarMem += size;

    size = (sizeof(*pWork->pOutputMapping) * numOut);
    VERIFY_ITEM_SIZE((pVarMem - pVarBase + size), varMemSize);
    pWork->pOutputMapping = (int*) pVarMem;
    pVarMem += size;

    size = (sizeof(*pWork->pEmdmaTranslatedTapList) * numOut);
    VERIFY_ITEM_SIZE((pVarMem - pVarBase + size), varMemSize);
    pWork->pEmdmaTranslatedTapList = (int*) pVarMem;

} /* setup_variable_work_memory() */


/**
 *  emdma_config()
 *
 *  @brief Initializes TCB list and opens the Enhanced Memory DMA (EMDMA) driver
 *
 *  @param[in,out] pWork        Pointer to work memory buffer allocated by model
 *                              and containing data defining this model instance
 *  @param[in]     delayChain   Pointer to output tap delays for all input channels
 *
 */
static void emdma_config( sharedDelayWorkMemory_t *pWork, 
                          const int *delayChain )
{
    gMdmaResult_t eResult = GMDMA_SUCCESS;
    unsigned int emdmaChainPtrRegDir = 0;

    // Verify pointers are valid
    assert(pWork != NULL);
    assert(delayChain != NULL);

    // Note that the EMDMA driver stream must have already been opened for
    // fast reuse mode of operation by the platform software using the 
    // Memory DMA (MDMA) manager.

    // Create initial EMDMA TCB list
    emdma_create_tcb_list(pWork, delayChain);

    // Create an EMDMA fast reuse copy request for a delay line mode copy. 
    // Enable circular buffering for external buffer and circular buffer 
    // ExtStartAddress write back on transfer completion.  Note that write
    // back is always enabled for delay line mode. Provide callback function
    // and parameter specific to this fast copy request.
    eResult = gMdmaCreateFastCopy(SHARED_DELAY_EMDMA_STREAM,
                                  ADI_EMDMA_DELAYLINE_MODE,
                                  0,             // OTF Direction is NA to delay mode
                                  (void*) pWork->emdmaTcbList,
                                  true,          // Enable external circular buffer
                                  true,          // Enable TCB ext start addr write back
                                  DEBUG_PTR(emdma_callback),
                                  DEBUG_PTR(pWork),
                                  &pWork->emdmaFastReuseCopyReq);
    assert(eResult == GMDMA_SUCCESS);

} /* emdma_config() */


/**
 *  emdma_create_tcb_list()
 *
 *  @brief Creates the initial TCB list
 *
 *  @param[in,out] pWork        Pointer to work memory buffer allocated by model
 *                              and containing data defining this model instance
 *  @param[in]     delayChain   Pointer to output tap delays for all input channels
 *
 */
static void emdma_create_tcb_list( sharedDelayWorkMemory_t *pWork, 
                                   const int *delayChain )
{
    ADI_EMDMA_DESC_DELAYLINE *pTcbList = pWork->emdmaTcbList;
    const int frameSize = pWork->frameSize;
    const int numInputs = pWork->numInputs;
    const int numOutputs = pWork->numOutputs;
    float **pBufferBase = pWork->pDelayBufferInput;
    int *pDelayBufferLength = pWork->pDelayBufferLengthInput;
    float pm **pFastBufferBase = (float pm **) pWork->pFastDmaBufferInput;
    int **pEmdmaTapList = pWork->pEmdmaTapList;
    int *pEmdmaTapCount = pWork->pEmdmaTapCount;
    int *pOutputMapping = pWork->pOutputMapping;
    int *pEmdmaTranslatedTapList = pWork->pEmdmaTranslatedTapList;
    const float *pInput;
    unsigned int chainPtr;

    // ------------------------------------------------------------------------
    // Circular TCBs are used for all channel transfers and the external TCB
    // items refer to the circular buffer. EMDMA delay line transfers always
    // have write back enabled so that the external start address is updated
    // after each transfer to point to the location of the next transfer.
    // This way external start addresses never have to be updated by software
    // after TCB creation. 
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    // Initialize EMDMA delay line tap list with sequentially mapped and 
    // translated delays that correspond to EMDMA delay line offsets.
    // ------------------------------------------------------------------------
    for (int i = 0; i < numOutputs; i++)
    {
        // Set tap list offset to retrieve delayed frame
        pEmdmaTranslatedTapList[i] = -delayChain[pOutputMapping[i]];
    }

    // ------------------------------------------------------------------------
    // Initialize delay line TCB fields for each input channel. Note that the
    // work memory will always be initialized to zero by the model initialize
    // function prior to calling this function. 
    // ------------------------------------------------------------------------ 
    for (int i = 0; i < numInputs; i++)
    {
        pInput = &pWork->pAudioInput[i * frameSize];

        // Setup to point to next TCB in the list or terminate the list
        if (i == (numInputs - 1))
        {
            pTcbList[i].pNext = NULL;
            chainPtr = 0;
        }
        else
        {
            pTcbList[i].pNext = &pTcbList[i+1];

            // Set TCB chain pointer to address of last field of next TCB
            chainPtr = (unsigned int) pTcbList[i].pNext + EMDMA_CIRC_TCB_OFFSET;
        }

        // Set TCB chain pointer to translated address
        pTcbList[i].Chnptr = TranslateAddr(chainPtr);
        pTcbList[i].pTaplistPointer = translate_addr((float*) pEmdmaTapList[i]);
        pTcbList[i].TapCount = pEmdmaTapCount[i];
        pTcbList[i].ExtReadModify = 1;          // Number of 32 bit words
        pTcbList[i].ReadCount = frameSize;
        pTcbList[i].pIntReadStartAddress = translate_addr(pFastBufferBase[i]);
        pTcbList[i].ExtLength = pDelayBufferLength[i];
        pTcbList[i].pExtBaseAddress = translate_addr(pBufferBase[i]);
        pTcbList[i].ExtWriteModify = 1;         // Number of 32 bit words
        pTcbList[i].pExtWriteStartAddress = translate_addr(pBufferBase[i]);
        pTcbList[i].WriteCount = frameSize;
        pTcbList[i].IntModify = 1;              // Number of 32 bit words
        pTcbList[i].pIntWriteStartAddress = translate_addr((float*) pInput);
    }

    // Generate interrupt on completion of last TCB in chain 
    pTcbList[numInputs - 1].Chnptr = BITM_EMDMA_CHNPTR_PCI;

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
static inline void emdma_start_transfer(sharedDelayWorkMemory_t *pWork)
{
    gMdmaResult_t eResult = GMDMA_SUCCESS;

    // Submit a fast reuse mode EMDMA copy request to initiate the copy
    // immediately if no existing copy is in progress or place on the
    // wait list for processing after all copies on the list have completed.
    // Note that this allows multiple instances of the shared Delay module
    // to run together. 
    eResult = gMdmaSubmitFastCopy(pWork->emdmaFastReuseCopyReq);
    assert(eResult == GMDMA_SUCCESS);

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
static void emdma_cancel(sharedDelayWorkMemory_t *pWork)
{
    gMdmaResult_t eResult = GMDMA_SUCCESS;

    // Verify pointer is valid
    assert(pWork != NULL);

    /* Cancel the copy request for this instance of the model */
    eResult = gMdmaCancelFastCopy(pWork->emdmaFastReuseCopyReq);
    assert(eResult == GMDMA_SUCCESS);

} /* emdma_cancel() */


/**
 * translate_addr()
 * 
 * Translates the supplied GUL memory byte address for DMA operation
 *
 * Parameters
 *  - memoryAddr   Memory address to translate
 *
 * Returns
 *  - Translated memory address for DMA usage
 */
static inline void* translate_addr( float *memoryAddr )
{
    unsigned int memAddr = (unsigned int) memoryAddr;

    /* Translate the address */
    return (void*)(TranslateAddr( memAddr ));

} /* translate_addr() */


/**
 *  emdma_callback()
 * 
 *  @brief EMDMA manager interrupt callback used for debugging
 *
 *  @param[in]  pCBParam    Callback parameter supplied by application
 *  @param[in]  Event       Callback event
 *  @param[in]  pArg        Callback argument
 *
 */
static void emdma_callback( void *pCBParam, unsigned int Event, void *pArg )
{
    sharedDelayWorkMemory_t *pWork = pCBParam; 

    /* CASEOF (Event) */
    switch ((ADI_EMDMA_EVENT)Event)
    {
        /* CASE (Processed an EMDMA TCB list) */
        case (ADI_EMDMA_EVENT_DELAY_LINE_DESC_PROCESSED):
        {
            DEBUG_PD(pWork->emdmaCallbackCount += 1);
            break;
        }

        default:
            break;
    }

} /* emdma_callback() */

