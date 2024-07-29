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
 *  @file poolDelay.c
 *  @brief Pool delay CRL implementations for core and DMA versions. Delay
 *         buffer resides in L1 memory for core version and L2 memory, with
 *         frame buffer in L1 for DMA version. 
 *
 *  @note Optimization settings for speed should include setting compiler
 *        flags -Ov100 and -ipa (if possible) for the top level build. 
 *        NDEBUG should also be defined to turn off asserts. Also, it is
 *        important to note that this module was NOT designed to support
 *        the usage of cached data.
 *     
 *  @ingroup dsp
 *
 */

/***  Include Files ***********************************************************/

#include "builtins.h"
#include <stddef.h>
#include <string.h>
#include "GulTranslateAddr.h"
#include "gMDMA.h"
#include "poolDelay.h"


/***  Debug Support  **********************************************************/

// Pool Delay debugging support. Note that for profiling and production
// use, DEBUG_POOL_DELAY should not be defined, ADI_DEBUG should NOT be 
// defined for the EMDMA drivers, and compiler optimization flags should
// be SET as stated above. 
#define DEBUG_POOL_DELAY (1)

#if (DEBUG_POOL_DELAY)
   #define DEBUG_PD(expr)    (expr)
   #define DEBUG_PTR(ptr)    (ptr)
#else /* DEBUG_POOL_DELAY */
   #define DEBUG_PD(expr)
   #define DEBUG_PTR(expr)   NULL

    // Turn off asserts
    #if !defined(NDEBUG)
        #define NDEBUG
    #endif /* NDEBUG */
#endif /* DEBUG_POOL_DELAY */
#include "assert.h"


/***  External Variable References  *******************************************/


/***  Module Macros  **********************************************************/

// Verifies requested memory size does not exceed actual memory size
#define VERIFY_ITEM_SIZE(reqMemSize, memSize) assert((memSize) >= (reqMemSize))

// Defines number of EMDMA TCBs (memory copies) required to handle the
// different channel delay cases of: 
//    (delay = 0), (delay <= frame), (frame < delay < 2 frames), and
//    (delay >= 2 frames)
#define NUM_DMA_READ_DESC_NO_DELAY              0u
#define NUM_DMA_WRITE_DESC_NO_DELAY             0u
#define NUM_DMA_READ_DESC_FRAME_OR_LESS         0u
#define NUM_DMA_WRITE_DESC_FRAME_OR_LESS        1u
#define NUM_DMA_READ_DESC_GREATER_FRAME         1u
#define NUM_DMA_WRITE_DESC_GREATER_FRAME        2u
#define NUM_DMA_READ_DESC_GREATER_TWO_FRAMES    1u
#define NUM_DMA_WRITE_DESC_GREATER_TWO_FRAMES   1u

// Defines descriptor offset for EMDMA circular descriptor (TCB).
// Note that this is the DMA hardware TCB size and it does not include 
// the next pointer included in the ADI driver descriptor.
#define EMDMA_CIRC_TCB_OFFSET ((sizeof(ADI_EMDMA_DESC_STANDARD_CIRCULAR) - \
                                sizeof(void*)))

// Defines EMDMA stream to use for delay models
#define POOL_DELAY_EMDMA_STREAM  GMDMA_STREAM_1

/***  Module Types  ***********************************************************/

// Structure contains number of DMA TCBs required, based on channel delays
typedef struct
{
    int numDmaReadTcb;     // Number of DMA read TCBs required
    int numDmaWriteTcb;    // Number of DMA write TCBs required
    int numDmaTcb;         // Total number of DMA TCBs required

} dmaTcbListSize_t;


/***  Module Variables  *******************************************************/


/***  Local Function Declarations  ********************************************/

static void emdma_callback( void *pCBParam, unsigned int Event, void *pArg );
static void emdma_config( dmaWorkMemory_t *pWork, const unsigned int *delayChain );
static void emdma_size_tcb_list( volatile dmaTcbListSize_t *pSize,
                                 int numChannels, 
                                 int frameSize, 
                                 const unsigned int *delayChain );
static void emdma_create_tcb_list( dmaWorkMemory_t *pWork, const unsigned int *delayChain );
static inline void emdma_start_transfer( dmaWorkMemory_t *pWork );
static void emdma_cancel( dmaWorkMemory_t *pWork );
static inline void* translate_addr( float *memoryAddr );
static void setup_variable_work_memory( dmaWorkMemory_t *pWork );


/***  Module Functions  *******************************************************/

/**
 *  pool_delay_core_init()
 *
 *  @brief Initializes data used for core based implementation of pool delay
 *
 *  @param[out]    reserved             Artifact of code generation, not intended for use here
 *  @param[in]     input                Input audio buffer pointer
 *  @param[in]     fastDelayBuffer      Pointer to fast delay buffer allocated by model 
 *  @param[in]     fastDelayBufferSize  Size of fast delay buffer
 *  @param[in]     delayChain           Pointer to array of delays for each channel
 *  @param[in]     numChannels          Number of channels
 *  @param[in]     frameSize            Number of samples per frame
 *  @param[in,out] workMemory           Pointer to memory buffer allocated by model and used
 *                                      by pool delay
 *  @param[in]     workMemorySize       Size of work memory allocated by model
 *
 *  @warning It is not recommended to use reserved memory. When passed in, this will actually
 *           point to the first element of workMemory. The reason for this has to do with
 *           making sure Simulink does not optimize out the call to this function. So it is
 *           not recommended to use, as it's value may change outside of these functions.
 */
void pool_delay_core_init(float *reserved,
                          const float *input,
                          float *fastDelayBuffer,
                          const unsigned int fastDelayBufferSize,
                          const unsigned int *delayChain,
                          const unsigned int numChannels,
                          const unsigned int frameSize,
                          unsigned int *workMemory,
                          const unsigned int workMemorySize)
{
    // Get pointer to the work memory
    // Note that the work memory will always be initialized to zero
    // by the model initialize function prior to calling this function.
    coreWorkMemory_t *pWork = (coreWorkMemory_t *)workMemory;

    // Verify work memory is large enough. Note that no variable sized
    // data is used by the pool delay core implementation.
    unsigned int requiredWorkMemSize = sizeof(*pWork) / sizeof(workMemory[0]);
    assert(workMemorySize >= requiredWorkMemSize);

    // Initialize the work memory and get pointers to variable data section
    // members
    pWork->frameSize = frameSize;
    pWork->numChannels = numChannels;

    // Determine the total delay chain length for all channel delays
    for (int i = 0; i < numChannels; i++)
    {
        pWork->totalDelayChainLength += delayChain[i];
    }
    
    // Make sure the circular delay buffer length is at least as large
    // as the frame size. This is REQUIRED to ensure the SHARC built-in
    // function cirindex() will function correctly. The increment size
    // must not exceed the buffer length.
    pWork->totalDelayChainLength = max(frameSize, pWork->totalDelayChainLength);

    // Verify delay buffer is large enough
    assert( pWork->totalDelayChainLength <= fastDelayBufferSize );

    pWork->fastDelayBuffer = fastDelayBuffer;
    pWork->fastDelayBufferSize = fastDelayBufferSize;
    pWork->delayIndex = 0;

    // Set flag indicating initialization has completed successfully.
    pWork->initComplete = true;

} /* pool_delay_core_init() */


/**
 *  @brief pool_delay_core_processs()
 *
 *  @brief Core based run-time processing of pool of channel delays. Channel
 *         states are stored in a fast delay buffer by core. Outputs are 
 *         populated from fast delay buffer by core.  
 *                                   
 *  @param[out]    output       Output audio buffer pointer
 *  @param[in]     input        Input audio buffer pointer
 *  @param[in]     delayChain   Pointer to array of delays for each channel
 *  @param[in,out] workMemory   Pointer to memory buffer allocated by model and used
 *                              by pool delay
 */
void pool_delay_core_process(float *output,
                             const float *input,
                             const unsigned int *delayChain,
                             unsigned int *workMemory)
{
    // Grab data objects from work memory
    coreWorkMemory_t *pWork = (coreWorkMemory_t *)workMemory;

    // Exit if pool delay initialization has not completed
    if (pWork->initComplete == false)
    {
        return;
    }

    // Pointers to channel input and output buffers
    const float *pInput;
    float *pOutput;

    // Number of channels and frame size
    const int numChannels = pWork->numChannels;
    const int frameSize = pWork->frameSize;

    // Pointer to fast delay buffer
    float pm *pFastBufferBase = (pm float *) pWork->fastDelayBuffer;

    // Delay buffer length and current index
    int bufferLength = pWork->totalDelayChainLength;
    int bufferIndex = pWork->delayIndex;

    // Loop counters
    int sampleIndex;
    int channel;

    //-----------------------------------------------------
    // 1st Channel Loop: Delay Buffer Read to Audio Output
    //-----------------------------------------------------
    for (channel = 0; channel < numChannels; channel++)
    {
        // Input & output pointers for current channel
        pInput = &input[channel * frameSize];
        pOutput = &output[channel * frameSize];

        // Delay for current channel
        int delay = (int) delayChain[channel];

        // Number of samples into the delay buffer
        int partialDelay = min(frameSize, delay);

        // Initial offset into delay buffer
        int srcIndex = bufferIndex;

        // Fill first part of output buffer from delay buffer
        for (sampleIndex = 0; sampleIndex < partialDelay; sampleIndex++)
        {
            pOutput[sampleIndex] = pFastBufferBase[srcIndex];
            srcIndex = circindex(srcIndex, 1, bufferLength);
        }

        // Fill the rest from the input buffer
        srcIndex = 0;
        for (sampleIndex = partialDelay; sampleIndex < frameSize; sampleIndex++, srcIndex++)
        {
            pOutput[sampleIndex] = pInput[srcIndex];
        }

        // Advance buffer index for next channel
        bufferIndex = circindex(bufferIndex, delay, bufferLength);

    } // end of 1st channel loop

    //-----------------------------------------------------
    // Reset & Advance indexes before 2nd loop
    //-----------------------------------------------------
    bufferIndex = circindex(pWork->delayIndex, frameSize, bufferLength);
    pWork->delayIndex = bufferIndex;

    //-------------------------------------------------------
    // 2nd Channel Loop: Delay Buffer Write from Audio Input
    //-------------------------------------------------------
    for (channel = 0; channel < numChannels; channel++)
    {
        // Input pointer for current channel
        pInput = &input[channel * frameSize];

        // Delay for current channel
        int delay = (int) delayChain[channel];

        // Number of samples into the delay buffer
        int partialDelay = min(frameSize, delay);

        // Calculate index with offset
        int destIndex = circindex(bufferIndex, (delay - partialDelay), bufferLength);

        // Fill delay buffer with remainder of audio input
        for (sampleIndex = (frameSize - partialDelay); sampleIndex < frameSize; sampleIndex++)
        {
            pFastBufferBase[destIndex] = pInput[sampleIndex];
            destIndex = circindex(destIndex, 1, bufferLength);
        }

        // Advance buffer index for next channel
        bufferIndex = circindex(bufferIndex, delay, bufferLength);

    } // end of 2nd channel loop

} // pool_delay_core_process()


/**
 *  pool_delay_mdma_init()
 *
 *  @brief Initializes data used for DMA based implementation of pool delay
 *
 *  @param[out]    reserved             Artifact of code generation, not intended for use here
 *  @param[in]     input                Input audio buffer pointer
 *  @param[in]     delayBuffer          Pointer to delay buffer allocated by model 
 *  @param[in]     delayBufferSize      Size of delay buffer
 *  @param[in]     fastDelayBuffer      Pointer to fast delay buffer allocated by model 
 *  @param[in]     fastDelayBufferSize  Size of fast delay buffer
 *  @param[in]     delayChain           Pointer to array of delays for each channel
 *  @param[in]     numChannels          Number of channels
 *  @param[in]     frameSize            Number of samples per frame
 *  @param[in,out] workMemory           Pointer to memory buffer allocated by model and used
 *                                      by pool delay
 *  @param[in]     workMemorySize       Size of work memory allocated by model
 *
 *  @warning It is not recommended to use reserved memory. When passed in, this will actually
 *           point to the first element of workMemory. The reason for this has to do with
 *           making sure Simulink does not optimize out the call to this function. So it is
 *           not recommended to use, as it's value may change outside of these functions.
 */
void pool_delay_mdma_init(float *reserved,
                          const float *input,
                          float *delayBuffer,
                          const unsigned int delayBufferSize,
                          float *fastDelayBuffer,
                          const unsigned int fastDelayBufferSize,                          
                          const unsigned int *delayChain,
                          const unsigned int numChannels,
                          const unsigned int frameSize,
                          unsigned int *workMemory,
                          const unsigned int workMemorySize)
{
    // Get pointer to the work memory
    // Note that the work memory will always be initialized to zero
    // by the model initialize function prior to calling this function.
    dmaWorkMemory_t *pWork = (dmaWorkMemory_t *)workMemory;

    // Determine size of variable memory TCB list required based on 
    // channel delays.  Note: This MUST be done before verifying the 
    // variable memory size needed and setting up the variable work 
    // space memory.
    volatile dmaTcbListSize_t numTcb;
    emdma_size_tcb_list(&numTcb, numChannels, frameSize, delayChain);

    // Verify work memory is large enough
    unsigned int requiredWorkMemSize = sizeof(*pWork) + 
                                       DMA_VARIABLE_DATA_SECTION_SIZE(numChannels, 
                                                                      numTcb.numDmaTcb);
    requiredWorkMemSize /= sizeof(workMemory[0]);
    assert(workMemorySize >= requiredWorkMemSize);

    // Initialize the work memory and get pointers to variable data section
    // members
    pWork->frameSize = frameSize;
    pWork->numChannels = numChannels;

    // Determine the total delay chain length for all channel delays,
    // actual delay buffer length required, and actual fast delay buffer
    // length required. Note that the total delay buffer size required 
    // is dependent on the delay chain lengths for each channel and is
    // split between the delay buffer and the fast delay buffer.
    for (int i = 0; i < numChannels; i++)
    {
        unsigned int delay = delayChain[i];
        unsigned int minDelay = min( frameSize, delay );
        pWork->delayBufferLength += (delay - minDelay);
        pWork->fastDelayBufferLength += minDelay;       
    }

    // Make sure the circular delay buffer length is at least as large
    // as the frame size. This is REQUIRED to ensure the SHARC built-in
    // function cirindex() will function correctly. The increment size
    // must not exceed the buffer length.
    pWork->delayBufferLength = max(frameSize, pWork->delayBufferLength);

    // Verify delay buffers are large enough
    assert( pWork->delayBufferLength <= delayBufferSize );
    assert( pWork->fastDelayBufferLength <= fastDelayBufferSize );

    pWork->delayBuffer = delayBuffer;
    pWork->delayBufferSize = delayBufferSize;
    pWork->delayIndex = 0;

    pWork->fastDelayBuffer = fastDelayBuffer;
    pWork->fastDelayBufferSize = fastDelayBufferSize;

    // Initialize EMDMA variables
    pWork->numDmaReadTcb = numTcb.numDmaReadTcb;
    pWork->numDmaWriteTcb = numTcb.numDmaWriteTcb;
    pWork->numDmaTcb = numTcb.numDmaTcb;

    // Current audio input pointer for EMDMA optimization
    pWork->pAudioInput = 0;

    // EMDMA ISR callback counter for debugging
    pWork->emdmaCallbackCount = 0;

    // Setup the variable data work memory space
    setup_variable_work_memory(pWork);

    // Setup the pointers to variable work memory read and write 
    // DMA channel maps
    pWork->emdmaReadChanTcbMap = &pWork->emdmaTcbMap[0];
    pWork->emdmaWriteChanTcbMap = &pWork->emdmaTcbMap[numChannels];

    // Setup EMDMA transfers only if EMDMA transfers are required,
    // meaning there is at least one non-zero channel delay.
    if ( pWork->numDmaTcb != 0 )
    {
        // Create initial EMDMA TCB list and setup EMDMA engine
        emdma_config(pWork, delayChain);
    }

    // Set flag indicating initialization has completed successfully.
    pWork->initComplete = true;

} /* pool_delay_mdma_init() */


/**
 *  pool_delay_mdma_process()
 *
 *  @brief DMA based run-time processing of pool of channel delays. Channel
 *         states are stored in fast and slow delay buffers, with slow delay 
 *         buffer only accessed with DMA. Outputs are populated from fast
 *         delay buffer by core.
 *
 *  @param[out]    output       Output audio buffer pointer
 *  @param[in]     input        Input audio buffer pointer
 *  @param[in]     delayChain   Pointer to array of delays for each channel
 *  @param[in,out] workMemory   Pointer to memory buffer allocated by model and used
 *                              by pool delay
 */
void pool_delay_mdma_process(float *output,
                             const float *input,
                             const unsigned int *delayChain,
                             unsigned int *workMemory)
{
    // Grab data objects from work memory
    dmaWorkMemory_t *pWork = (dmaWorkMemory_t *)workMemory;

    // Exit if pool delay initialization has not completed or RMDL
    // process has started
    if (pWork->initComplete == false)
    {
        return;
    }

    // Pointers to channel input and output buffers
    const float *pInput;
    float *pOutput;

    // Pointers for channel map information and DMA TCBs
    ADI_EMDMA_DESC_STANDARD_CIRCULAR **wrTcbMap = pWork->emdmaWriteChanTcbMap;

    // Number of channels and frame size
    const int numChannels = pWork->numChannels;
    const int frameSize = pWork->frameSize;

    // Pointer to fast delay buffer
    float pm *pFastBufferBase = (float pm *) pWork->fastDelayBuffer;

    // Fast delay buffer index (not a circular buffer)
    int fastBufferIndex = 0;

    // Loop counters
    int sampleIndex;
    int channel;

    //-----------------------------------------------------
    // 1st Channel Loop: Delay Buffer Read to Audio Output 
    //-----------------------------------------------------
    for (channel = 0; channel < numChannels; channel++)
    {
        // Input & output pointers for current channel
        pInput = &input[channel * frameSize];
        pOutput = &output[channel * frameSize];

        // Delay for current channel
        int delay = (int) delayChain[channel];

        // Number of samples into the delay buffer
        int partialDelay = min(frameSize, delay);

        // Initial offset into delay buffer
        int fastSrcIndex = fastBufferIndex;

        // Fill first part of output buffer from the fast delay buffer
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

        // Advance fast buffer index for next channel
        fastBufferIndex += partialDelay;

    } // end of 1st channel loop

    //------------------------------------------------
    // Reset index before 2nd loop
    //------------------------------------------------
    fastBufferIndex = 0;

    // Only update TCBs if audio input pointer changes
    // Compiler optimization for false condition 
    if (expected_false(pWork->pAudioInput != input))
    {
        pWork->pAudioInput = input;

        //-------------------------------------------------------
        // 2nd Channel Loop: Delay Buffer Write from Audio Input
        //-------------------------------------------------------
        for (channel = 0; channel < numChannels; channel++)
        {
            // Input pointer for current channel
            pInput = &input[channel * frameSize];

            // Pointer to channel write TCB
            ADI_EMDMA_DESC_STANDARD_CIRCULAR *pWrTcb = wrTcbMap[channel];

            // Delay for current channel
            int delay = (int) delayChain[channel];

            // Number of samples into the delay buffer
            int partialDelay = min(frameSize, delay);

            // Update channel's DMA TCB for DMA write for next frame
            int chBufSize = delay - partialDelay;

            if (delay >= (2 * frameSize))
            {
                // Write frame into circular delay buffer from audio input buffer 
                pWrTcb->pIntStartAddress = translate_addr((float*) &pInput[0]);
            }
            else if ((delay > frameSize) && (delay < (2 * frameSize)))
            {
                // Write partial frame from audio input buffer into fast delay buffer
                pWrTcb->pIntStartAddress = translate_addr((float*) &pInput[0]);

                // Write remainder of frame into circular delay buffer from audio 
                // input buffer
                pWrTcb = pWrTcb->pNext;
                pWrTcb->pIntStartAddress = translate_addr((float*) &pInput[frameSize - chBufSize]);
            }
            else if (delay <= frameSize)
            {
                // Write partial frame into fast delay buffer from audio input buffer
                pWrTcb->pIntStartAddress = translate_addr((float*) &pInput[frameSize - delay]);
            }

            // Advance fast buffer index for next channel
            fastBufferIndex += partialDelay;

        } // end of 2nd channel loop
    }

    // Start EMDMA transfers only if EMDMA transfers are required,
    // meaning there is at least one non-zero channel delay.
    if ( pWork->numDmaTcb != 0 )
    {
        // Start the DMA transfers to update delay lines in preparation for the
        // next frame
        emdma_start_transfer(pWork);
    }

} /* pool_delay_mdma_process() */


/**
 *  pool_delay_mdma_cancel()
 *
 *  @brief DMA based cancel of further audio frame step pool delay processing 
 *         and cancellation of the DMA transfer if it is currently in progress.
 *
 *  @param[in,out] workMemory   Pointer to memory buffer allocated by model and used
 *                              by pool delay
 * 
 *  @returns Status of 0 for success and 1 for failure
 */
unsigned int pool_delay_mdma_cancel(unsigned int *workMemory)
{
    // Grab data objects from work memory
    dmaWorkMemory_t *pWork = (dmaWorkMemory_t *)workMemory;

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

} /* pool_delay_mdma_cancel() */


/**
 *  setup_variable_work_memory()
 *
 *  @brief Allocates data within work memory variable data space for accelerator 
 *
 *  @param[in,out] pWork            Pointer to work memory buffer allocated by model and
 *                                  containing data defining this model instance
 *
 */
static void setup_variable_work_memory(dmaWorkMemory_t *pWork)
{
    assert(pWork != NULL);

    int numCh = pWork->numChannels;
    int numDmaTcb = pWork->numDmaTcb;
    int varMemSize = DMA_VARIABLE_DATA_SECTION_SIZE(numCh, numDmaTcb);
   
    char *pVarBase = pWork->data;
    char *pVarMem = pVarBase;
    int size;

    size = (sizeof(*pWork->emdmaTcbList) * numDmaTcb);
    VERIFY_ITEM_SIZE((pVarMem - pVarBase + size), varMemSize);
    pWork->emdmaTcbList = (ADI_EMDMA_DESC_STANDARD_CIRCULAR*) pVarMem;
    pVarMem += size;

    size = (sizeof(*pWork->emdmaTcbMap) * 2 * numCh);
    VERIFY_ITEM_SIZE((pVarMem - pVarBase + size), varMemSize);
    pWork->emdmaTcbMap = (ADI_EMDMA_DESC_STANDARD_CIRCULAR**) pVarMem;

} /* setup_variable_work_memory() */


/**
 *  emdma_size_tcb_list()
 *
 *  @brief Determines number of DMA TCBs required, based on channel delays
 *
 *  @param[in,out] pWork        Pointer to DMA TCB size structure, containing
 *                              number of read, write, and total TCBs required
 *  @param[in]     numChannels  Number of channels
 *  @param[in]     frameSize    Frame size
 *  @param[in]     delayChain   Pointer to delays for all channels
 *
 */
static void emdma_size_tcb_list( volatile dmaTcbListSize_t *pSize,
                                 int numChannels, 
                                 int frameSize, 
                                 const unsigned int *delayChain )
{
    // Verify pointer is valid
    assert(pSize != NULL);   

    // Initialize TCB counts
    pSize->numDmaReadTcb = 0;
    pSize->numDmaTcb = 0;
    pSize->numDmaWriteTcb = 0;

    // Create channel TCB map and determine the total number of DMA
    // channel required for channel delays.
    int i;
    for (i = 0; i < numChannels; i++)
    {
        unsigned int delay = delayChain[i];
        
        if (delay == 0)
        {
            pSize->numDmaReadTcb += NUM_DMA_READ_DESC_NO_DELAY;
            pSize->numDmaWriteTcb += NUM_DMA_WRITE_DESC_NO_DELAY;            
        }
        else if ((delay > 0) && (delay <= frameSize))
        {
            pSize->numDmaReadTcb += NUM_DMA_READ_DESC_FRAME_OR_LESS;
            pSize->numDmaWriteTcb += NUM_DMA_WRITE_DESC_FRAME_OR_LESS;
        }
        else if ((delay > frameSize) && (delay < (2 * frameSize)))
        {
            pSize->numDmaReadTcb += NUM_DMA_READ_DESC_GREATER_FRAME;
            pSize->numDmaWriteTcb += NUM_DMA_WRITE_DESC_GREATER_FRAME; 
        }
        else
        {
            pSize->numDmaReadTcb += NUM_DMA_READ_DESC_GREATER_TWO_FRAMES;
            pSize->numDmaWriteTcb += NUM_DMA_WRITE_DESC_GREATER_TWO_FRAMES;
        }
    }
    pSize->numDmaTcb = pSize->numDmaReadTcb + pSize->numDmaWriteTcb; 

} /* emdma_size_tcb_list() */


/**
 *  emdma_config()
 *
 *  @brief Initializes TCB list and opens the Enhanced Memory DMA (EMDMA) driver
 *
 *  @param[in,out] pWork        Pointer to work memory buffer allocated by model
 *                              and containing data defining this model instance
 *  @param[in]     delayChain   Pointer to delays for all channels
 *
 */
static void emdma_config( dmaWorkMemory_t *pWork, const unsigned int *delayChain )
{
    gMdmaResult_t eResult = GMDMA_SUCCESS;
    unsigned int emdmaChainPtrRegDir = 0;

    // Verify pointer is valid
    assert(pWork != NULL);   

    // Note that the EMDMA driver stream must have already been opened for
    // fast reuse mode of operation by the platform software using the 
    // Memory DMA (MDMA) manager.

    // Create initial EMDMA TCB list
    emdma_create_tcb_list(pWork, delayChain);

    if ( pWork->numDmaReadTcb > 0 )
    {
        // Setup EMDMA chain pointer register for read for first TCB in list
        emdmaChainPtrRegDir = ADI_EMDMA_OTFDIR_READ;
    }
    else
    {
        // Setup EMDMA chain pointer register for write for first TCB in list
        emdmaChainPtrRegDir = ADI_EMDMA_OTFDIR_WRITE;
    }

    // Create an EMDMA fast reuse copy request for an On-The-Fly direction copy. 
    // Copy direction is based on direction (read or write) specified in TCBs 
    // (chain pointer register for chain TCB 0). Enable circular buffering for
    // external buffer and circular buffer ExtStartAddress write back on transfer
    // completion.  Provide callback function and parameter specific to this fast 
    // copy request.
    eResult = gMdmaCreateFastCopy(POOL_DELAY_EMDMA_STREAM,
                                  ADI_EMDMA_STANDARD_OTFDIR_MODE,
                                  emdmaChainPtrRegDir,
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
 *  @param[in]     delayChain   Pointer to delays for all channels
 *
 */
static void emdma_create_tcb_list( dmaWorkMemory_t *pWork, const unsigned int *delayChain )
{
    // Verify pointers are valid
    assert(pWork != NULL);
    assert(delayChain != NULL);   
    
    ADI_EMDMA_DESC_STANDARD_CIRCULAR *pTcbList = pWork->emdmaTcbList;
    ADI_EMDMA_DESC_STANDARD_CIRCULAR **pTcbMap;
    const int frameSize = pWork->frameSize;
    const int numChannels = pWork->numChannels;
    const int numDmaTcb = pWork->numDmaTcb;
    unsigned int rdChainPtr;

    // ------------------------------------------------------------------------
    // Initialize common fields of all TCBs in the list and next pointers.
    // Note that the work memory will always be initialized to zero by the
    // model initialize function prior to calling this function. 
    // ------------------------------------------------------------------------ 
    for (int i = 0; i < numDmaTcb; i++)
    {
        // Setup to point to next TCB in the list or terminate the list
        if (i == (numDmaTcb - 1))
        {
            pTcbList[i].pNext = NULL;
            rdChainPtr = 0;
        }
        else
        {
            pTcbList[i].pNext = &pTcbList[i+1];

            // Set TCB chain pointer to address of last field of next TCB
            rdChainPtr = (unsigned int) pTcbList[i].pNext + EMDMA_CIRC_TCB_OFFSET;
        }

        // Set TCB chain pointer to translated address
        pTcbList[i].Chnptr = (unsigned int) translate_addr((float*) rdChainPtr);

        // Increment external buffer access by 32 bit words 
        pTcbList[i].ExtModify = 1;

        // Increment internal buffer access by 32 bit words 
        pTcbList[i].IntModify = 1;
    }

    // ------------------------------------------------------------------------
    // Circular TCBs are used for all channel transfers and the external TCB
    // items refer to the circular buffer. EMDMA transfers are setup with
    // write back enabled so that the external start address is updated
    // after each transfer to point to the location of the next transfer.
    // This way external start addresses never have to be updated by software
    // after TCB creation. 
    // ------------------------------------------------------------------------

    // Pointers to slow and fast delay buffers
    float *pBufferBase = pWork->delayBuffer;
    float pm *pFastBufferBase = (float pm *) pWork->fastDelayBuffer;

    // Delay buffer channel base index (circular buffer)
    int chBaseIndex = 0;

    // Fast delay buffer channel base index (not a circular buffer)
    int chBaseFastIndex = 0;

    // ------------------------------------------------------------------------
    // Initialize channel read TCB map and TCB fields that will not change
    // ------------------------------------------------------------------------
    pTcbMap = pWork->emdmaReadChanTcbMap;
    for (int i = 0, channel = 0; channel < numChannels; channel++)
    {
        int delay = (int) delayChain[channel];
        
        // Number of samples into the delay buffer
        int partialDelay = min(frameSize, delay);

        // Update channel's DMA TCB(s) for DMA read(s) for next frame
        int chBufSize = delay - partialDelay;

        if (delay == 0)
        {
            // No reads, so set channel pointer to indicate no TCB
            pTcbMap[channel] = NULL;        
        }
        else if (delay <= frameSize)
        {
            // No reads, so set channel pointer to indicate no TCB
            pTcbMap[channel] = NULL;            
        }
        else if ((delay > frameSize) && (delay < (2 * frameSize)))
        {
            // One read TCB: Read from delay buffer
            pTcbMap[channel] = &pTcbList[i];

            // Read partial frame from circular delay buffer into fast delay buffer
            pTcbList[i].Chnptr |= ADI_EMDMA_OTFDIR_READ;
            pTcbList[i].ExtLength = chBufSize;
            pTcbList[i].pExtBaseAddress = translate_addr(&pBufferBase[chBaseIndex]);
            pTcbList[i].pExtStartAddress = translate_addr(&pBufferBase[chBaseIndex]);
            pTcbList[i].Count = chBufSize;
            pTcbList[i].pIntStartAddress = translate_addr(&pFastBufferBase[chBaseFastIndex]);

            i++;      
        }
        else
        {
            // Multiple frame delay of two or more frames

            // One read TCB: Read from delay buffer to fast delay buffer
            pTcbMap[channel] = &pTcbList[i];

            // Read frame from circular delay buffer into fast delay buffer
            pTcbList[i].Chnptr |= ADI_EMDMA_OTFDIR_READ; 
            pTcbList[i].ExtLength = chBufSize;
            pTcbList[i].pExtBaseAddress = translate_addr(&pBufferBase[chBaseIndex]);
            pTcbList[i].pExtStartAddress = translate_addr(&pBufferBase[chBaseIndex]);
            pTcbList[i].Count = frameSize;
            pTcbList[i].pIntStartAddress = translate_addr(&pFastBufferBase[chBaseFastIndex]);

            ++i;
        }

        // Advance buffer indexes for next channel, accounting for one frame 
        // of the delay residing in the fast delay buffer
        chBaseIndex += chBufSize;
        chBaseFastIndex += partialDelay;      
    }

    // Update last read TCB in list chain pointer to set the NEXT TCB's
    // (first write TCB) DMA direction to write and to generate interrupt
    // on completion of last TCB in chain.  NOTE: Chain pointer direction 
    // applies to the NEXT TCB.  
    if ( pWork->numDmaReadTcb > 0 )
    {
        // Only update if there are read TCBs.  Note that read delays 
        // only exist for delays > frame.
        pWork->emdmaTcbList[pWork->numDmaReadTcb - 1].Chnptr |= ADI_EMDMA_OTFDIR_WRITE;
    }

    // Reset & Advance indexes before 2nd loop
    chBaseFastIndex = 0;
    chBaseIndex = 0;

    // ------------------------------------------------------------------------
    // Initialize channel write TCB map and TCB fields that will not change
    // ------------------------------------------------------------------------    
    pTcbMap = pWork->emdmaWriteChanTcbMap;
    for (int i = pWork->numDmaReadTcb, channel = 0; channel < numChannels; channel++)
    {
        int delay = (int) delayChain[channel];

        // Number of samples into the delay buffer
        int partialDelay = min(frameSize, delay);

        // Update channel's DMA TCB(s) for DMA write(s) for next frame
        int chBufSize = delay - partialDelay;

        if (delay == 0)
        {
            // No writes, so set channel pointer to indicate no TCB
            pTcbMap[channel] = NULL;
        }
        else if (delay <= frameSize)
        {
            // One write TCB: Write to fast delay buffer from audio input buffer
            pTcbMap[channel] = &pTcbList[i];

            // Write partial frame into fast delay buffer from audio input buffer
            pTcbList[i].Chnptr |= ADI_EMDMA_OTFDIR_WRITE;
            pTcbList[i].ExtLength = partialDelay;
            pTcbList[i].pExtBaseAddress = translate_addr(&pFastBufferBase[chBaseFastIndex]);
            pTcbList[i].pExtStartAddress = translate_addr(&pFastBufferBase[chBaseFastIndex]);
            pTcbList[i].Count = delay;

            ++i;
        }
        else if ((delay > frameSize) && (delay < (2 * frameSize)))
        {
            // Two write TCBs: Write into fast delay from audio input buffer and 
            // to delay buffer from audio input buffer 
            pTcbMap[channel] = &pTcbList[i];

            // Write partial frame from audio input buffer into fast delay buffer
            pTcbList[i].Chnptr |= ADI_EMDMA_OTFDIR_WRITE;
            pTcbList[i].ExtLength = (frameSize - chBufSize);
            pTcbList[i].pExtBaseAddress = translate_addr(&pFastBufferBase[chBaseFastIndex + chBufSize]);
            pTcbList[i].pExtStartAddress = translate_addr(&pFastBufferBase[chBaseFastIndex + chBufSize]);
            pTcbList[i].Count = (frameSize - chBufSize);

            // Write remainder of frame into circular delay buffer from audio input buffer
            pTcbList[i+1].Chnptr |= ADI_EMDMA_OTFDIR_WRITE;
            pTcbList[i+1].ExtLength = chBufSize;
            pTcbList[i+1].pExtBaseAddress = translate_addr(&pBufferBase[chBaseIndex]);
            pTcbList[i+1].pExtStartAddress = translate_addr(&pBufferBase[chBaseIndex]);
            pTcbList[i+1].Count = chBufSize;

            i += 2;
        }
        else
        {
            // Multiple frame delay of two or more frames

            // One write TCB: Write to delay buffer from audio input buffer
            pTcbMap[channel] = &pTcbList[i];

            // Calculate index with offset, accounting for one frame of the delay
            // residing in the fast delay buffer
            int chDestIndex = circindex(chBaseIndex, (delay - frameSize), chBufSize);

            // One write TCB: Write to delay buffer from audio input buffer
            pTcbList[i].Chnptr |= ADI_EMDMA_OTFDIR_WRITE;
            pTcbList[i].ExtLength = chBufSize;
            pTcbList[i].pExtBaseAddress = translate_addr(&pBufferBase[chBaseIndex]);
            pTcbList[i].pExtStartAddress = translate_addr(&pBufferBase[chDestIndex]);
            pTcbList[i].Count = frameSize;

            ++i;
        }

        // Advance buffer indexes for next channel, accounting for one frame 
        // of the delay residing in the fast delay buffer
        chBaseFastIndex += partialDelay;
        chBaseIndex += chBufSize;         
    }

    // Update last TCB in list chain pointer to indicate the end of the
    // TCB list and to generate interrupt on completion of last TCB 
    // in chain 
    pWork->emdmaTcbList[numDmaTcb - 1].Chnptr = BITM_EMDMA_CHNPTR_PCI; 

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
static inline void emdma_start_transfer(dmaWorkMemory_t *pWork)
{
    gMdmaResult_t eResult = GMDMA_SUCCESS;

    // Submit a fast reuse mode EMDMA copy request to initiate the copy
    // immediately if no existing copy is in progress or place on the
    // wait list for processing after all copies on the list have completed.
    // Note that this allows multiple instances of the pool Delay module
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
static void emdma_cancel(dmaWorkMemory_t *pWork)
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
    dmaWorkMemory_t *pWork = pCBParam; 

    /* CASEOF (Event) */
    switch ((ADI_EMDMA_EVENT)Event)
    {
        /* CASE (Processed an EMDMA TCB list) */
        case (ADI_EMDMA_EVENT_STANDARD_WRITE_DESC_PROCESSED):
        case (ADI_EMDMA_EVENT_STANDARD_READ_DESC_PROCESSED):
        case (ADI_EMDMA_EVENT_STANDARD_OTFDIR_DESC_PROCESSED):
        {
            DEBUG_PD(pWork->emdmaCallbackCount += 1);
            break;
        }

        default:
            break;
    }

} /* emdma_callback() */

