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
 *  @file sharedDelay.h
 *  @brief  Shared delay CRL implementation for DMA based shared delay. Delay
 *          buffer resides in L2 (or L1) memory. 
 *
 *  @ingroup dsp
 *
 */

#ifndef __SHARED_DELAY_H__
#define __SHARED_DELAY_H__


/***  Include Files ***********************************************************/

#include <GulEmdmaDriver.h>


/***  External Variable References  *******************************************/
 

/***  Module Macros  **********************************************************/

// Calculates total memory size in bytes needed for sharedDelayWorkMemory_t 
// variable data section
#define DMA_VARIABLE_DATA_SECTION_SIZE( numIn, numOut ) ( \
            (sizeof(*((sharedDelayWorkMemory_t*)0)->emdmaTcbList) * (numIn)) + \
            (sizeof(*((sharedDelayWorkMemory_t*)0)->pDelayBufferInput) * (numIn)) + \
            (sizeof(*((sharedDelayWorkMemory_t*)0)->pDelayBufferLengthInput) * (numIn)) + \
            (sizeof(*((sharedDelayWorkMemory_t*)0)->pFastDmaBufferInput) * (numIn)) + \
            (sizeof(*((sharedDelayWorkMemory_t*)0)->pEmdmaTapList) * (numIn)) + \
            (sizeof(*((sharedDelayWorkMemory_t*)0)->pEmdmaTapCount) * (numIn)) + \
            (sizeof(*((sharedDelayWorkMemory_t*)0)->pOutputMapping) * (numOut)) + \
            (sizeof(*((sharedDelayWorkMemory_t*)0)->pEmdmaTranslatedTapList) * (numOut)) )


/***  Module Types  ***********************************************************/

// DMA based shared delay model work memory structure used for each instance of the
// model.
typedef struct sharedDelayWorkMemory_s
{
    // Scratch area that code gen uses. Reserved and not intended for usage
    // by shared delay functionality
    unsigned int scratchMemoryLocation;
    
    // Delay chain frame size and number of inputs, outputs, channels
    int frameSize;
    int numInputs;
    int numOutputs;
    
    // Pointer to Simulink generated circular delay buffer, buffer size, and index
    float *delayBuffer;
    int delayBufferSize;

    // Pointer to Simulink generated fast DMA output buffer and buffer size
    float *fastDmaBuffer;
    int fastDmaBufferSize;

    // Actual buffer lengths required based on number of channels and delays
    int delayBufferLength;
    int fastDmaBufferLength;    

    // Pointer to delay buffer pointers, fast DMA buffer pointers, and delay buffer
    // lengths for each input channel
    float **pDelayBufferInput;
    int *pDelayBufferLengthInput;
    float **pFastDmaBufferInput;

    // EMDMA TCB list for delay channels
    ADI_EMDMA_DESC_DELAYLINE *emdmaTcbList;

    // EMDMA Delay Line Tap List pointers to tap delays and tap counts
    // for each input
    int **pEmdmaTapList;
    int *pEmdmaTapCount;

    // Output map that maps outputs and delays to sequentially ordered list with
    // respect to input channels
    int *pOutputMapping;

    // Translated EMDMA Delay Line Tap List for all output taps
    int *pEmdmaTranslatedTapList;

    // Memory required to create fast reuse mode copy request
    unsigned char emdmaFastReuseCopyReq[ADI_EMDMA_COPY_REQ_MEMORY];

    // Current audio input buffer pointer for EMDMA optimization
    const float *pAudioInput; 

    // EMDMA callback counter for debugging
    volatile unsigned int emdmaCallbackCount;

    // Flag to indicate initialization has occurred
    bool initComplete;

    // Pointer to start of variable size sections.  Note that the size of this data is
    // specified as a mask parameter to the delay model and must be of 
    // required size.  Note that it is necessary to place the EMDMA TCB list
    // first and that 4 byte alignment is required (32 bit) for them.
    #pragma align 4
    char data[];

    // -----------------------------------------------------------------------------------
    //                           Variable Data Section Sizing
    //        Name :                                        Size in Bytes
    // -----------------------------------------------------------------------------------
    // DMA TCB memory :             (sizeof(ADI_EMDMA_DESC_DELAYLINE) * numInputs)
    // Delay Buffer Pointers :      (sizeof(float*) * numInputs)
    // Delay Buffer Lengths :       (sizeof(int)) * numInputs)
    // Fast Dma Buffer Pointers :   (sizeof(float*) * numInputs)
    // Tap Pointers :               (sizeof(int*) * numInputs)
    // Tap Counts :                 (sizeof(int) * numInputs)
    // Output Mapping :             (sizeof(int) * numOutputs)
    // Translated Tap List :        (sizeof(int) * numOutputs)

} sharedDelayWorkMemory_t;


/***  Module Function Prototypes  *********************************************/

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
extern void shared_delay_mdma_init(float *reserved,
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
                                   const int workMemorySize);

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
extern void shared_delay_mdma_process(float *output,
                                      const float *input,
                                      const int *delayChain,
                                      unsigned int *workMemory);

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
extern unsigned int shared_delay_mdma_cancel(unsigned int *workMemory);                                      

#endif /* __SHARED_DELAY_H__ */

