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
 *  @file poolDelay.h
 *  @brief  Pool delay CRL implementations for core and DMA versions. Delay
 *          buffer resides in L1 memory for core version and L2 memory, with
 *          frame buffer in L1 for DMA version. 
 *
 *  @ingroup dsp
 *
 */

#ifndef __POOL_DELAY_H__
#define __POOL_DELAY_H__


/***  Include Files ***********************************************************/

#include "GulEmdmaDriver.h"


/***  External Variable References  *******************************************/
 

/***  Module Macros  **********************************************************/

// Calculates total memory size in bytes needed for dmaWorkMemory_t 
// variable data section
#define DMA_VARIABLE_DATA_SECTION_SIZE( numCh, numDmaTcb ) ( \
                (sizeof(*((dmaWorkMemory_t*)0)->emdmaTcbList) * (numDmaTcb)) + \
                (sizeof(((dmaWorkMemory_t*)0)->emdmaTcbMap[0]) * 2 * (numCh)) )

// Number of EMDMA initialization TCBs to handle all memory copy types
#define NUM_INITIALIZATION_TCB  3


/***  Module Types  ***********************************************************/

// Core based pool delay model work memory structure used for each instance of the
// model.
typedef struct coreWorkMemory_s
{
    // Scratch area that code gen uses. Reserved and not intended for usage
    // by pool delay functionality
    unsigned int scratchMemoryLocation;
    
    // Delay chain frame size and number of channels
    int frameSize;
    int numChannels;
    
    // Total delay chain length of all channel delays in samples
    int totalDelayChainLength;

    // Pointer to Simulink generated fast delay buffer, buffer size and index
    float *fastDelayBuffer;
    int fastDelayBufferSize;
    int delayIndex;

    // Actual delay buffer length required based on channel delays
    int fastDelayBufferLength;    

    // Flag to indicate initialization has occurred
    bool initComplete;

    // Pointer to start of variable size sections.  Note that the size of this data is
    // specified as a mask parameter to the delay model and must be of 
    // required size. This is not used for the core pool delay implementation.

    #pragma align 4
    char data[];

    // -----------------------------------------------------------------------------------
    //                           Variable Data Section Sizing
    //        Name :                                        Size in Bytes
    // -----------------------------------------------------------------------------------

} coreWorkMemory_t;

// DMA based pool delay model work memory structure used for each instance of the
// model.
typedef struct dmaWorkMemory_s
{
    // Scratch area that code gen uses. Reserved and not intended for usage
    // by pool delay functionality
    unsigned int scratchMemoryLocation;
    
    // Delay chain frame size and number of channels
    int frameSize;
    int numChannels;
    
    // Pointer to Simulink generated circular delay buffer, buffer size, and index
    float *delayBuffer;
    int delayBufferSize;
    int delayIndex;

    // Pointer to Simulink generated fast delay buffer and buffer size
    float *fastDelayBuffer;
    int fastDelayBufferSize;

    // Actual delay buffer lengths required based on channel delays
    int delayBufferLength;
    int fastDelayBufferLength;    

    // EMDMA TCB list for  delay channels
    ADI_EMDMA_DESC_STANDARD_CIRCULAR *emdmaTcbList;

    // Number of EMDMA descriptors (TCBs) required based on delay lengths
    int numDmaReadTcb;
    int numDmaWriteTcb;
    int numDmaTcb;

    // Memory required to create fast reuse mode copy request
    unsigned char emdmaFastReuseCopyReq[ADI_EMDMA_COPY_REQ_MEMORY];

    // EMDMA read and write channel TCB maps.  Contains read/write
    // channel base TCB for each channel. This accounts for handling
    // of special case delays of (delay < frame) and (frame <= delay < 2 frames),
    // where there is not just a single read and write transaction per channel.
    ADI_EMDMA_DESC_STANDARD_CIRCULAR **emdmaReadChanTcbMap;
    ADI_EMDMA_DESC_STANDARD_CIRCULAR **emdmaWriteChanTcbMap;
    ADI_EMDMA_DESC_STANDARD_CIRCULAR **emdmaTcbMap;

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
    // DMA TCB memory :    (sizeof(ADI_EMDMA_DESC_STANDARD_CIRCULAR) * numDmaTcb)
    // DMA chan TCB map:   (sizeof(ADI_EMDMA_DESC_STANDARD_CIRCULAR*) * 2 * numChannels)

} dmaWorkMemory_t;


/***  Module Function Prototypes  *********************************************/

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
extern void pool_delay_core_init(float *reserved,
                                 const float *input,
                                 float *fastDelayBuffer,
                                 const unsigned int fastDelayBufferSize,
                                 const unsigned int *delayChain,
                                 const unsigned int numChannels,
                                 const unsigned int frameSize,
                                 unsigned int *workMemory,
                                 const unsigned int workMemorySize);

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
extern void pool_delay_core_process(float *output,
                                    const float *input,
                                    const unsigned int *delayChain,
                                    unsigned int *workMemory); 

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
extern void pool_delay_mdma_init(float *reserved,
                                 const float *input,
                                 float *delayBuffer,
                                 const unsigned int delayBufferSize,
                                 float *fastDelayBuffer,
                                 const unsigned int fastDelayBufferSize,                          
                                 const unsigned int *delayChain,
                                 const unsigned int numChannels,
                                 const unsigned int frameSize,
                                 unsigned int *workMemory,
                                 const unsigned int workMemorySize);

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
extern void pool_delay_mdma_process(float *output,
                                    const float *input,
                                    const unsigned int *delayChain,
                                    unsigned int *workMemory);

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
extern unsigned int pool_delay_mdma_cancel(unsigned int *workMemory);

#endif /* __POOL_DELAY_H__ */

