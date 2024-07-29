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
 *  @file fir_accelerator.h
 *  @brief Connects model FIR accelerator code generation with Griffin Ultra Lite (GUL)
 *         Hardware Accelerator Manager (HAM)
 *
 *  @ingroup dsp
 *
 */

#ifndef __FIR_ACCELERATOR_H__
#define __FIR_ACCELERATOR_H__

/***  Include Files ***********************************************************/

#include "gHAM.h"
#include "GulEmdmaDriver.h"


/***  External Variable References  *******************************************/
 

/***  Module Macros  **********************************************************/

// Calculates total memory size in bytes needed for firAccelWorkMemory_t 
// variable data section
#define FIR_VARIABLE_DATA_SECTION_SIZE( numIn, numOut, numCh, frameSize ) ( \
                ((numIn) * sizeof(((firAccelWorkMemory_t*)0)->emdmaTcbList[0])) + \
                (sizeof(*((firAccelWorkMemory_t*)0)->firTaskMemory) + (numCh) * sizeof(gHamFirTcb_t)) + \
                ((numOut) * (frameSize) * sizeof(((firAccelWorkMemory_t*)0)->firCircOutputDmaBuffs[0])) + \
                ((numCh) * sizeof(((firAccelWorkMemory_t*)0)->firInputChannelMap[0])) + \
                ((numIn) * sizeof(((firAccelWorkMemory_t*)0)->firCircInputDmaBuffs[0])) + \
                ((numIn) * sizeof(((firAccelWorkMemory_t*)0)->firCircInputDmaBuffsSize[0])) + \
                ((numCh) * sizeof(((firAccelWorkMemory_t*)0)->firInputBuff[0])) + \
                ((numCh) * sizeof(((firAccelWorkMemory_t*)0)->firInputBuffSize[0])) + \
                ((numCh) * sizeof(((firAccelWorkMemory_t*)0)->firOutputBuff[0])) + \
                ((numIn) * sizeof(((firAccelWorkMemory_t*)0)->firFrameStartIndex[0])) )


/***  Module Types  ***********************************************************/

// FIR accelerator model work memory structure used for each instance of the
// model.
typedef struct firAccelWorkMemory_s
{
    // Scratch area that code gen uses. Reserved and not intended for usage
    // by accelerator functionality
    unsigned int scratchMemoryLocation;
    
    // Accelerator frame size, number inputs, number outputs, and number channels.
    int frameSize;
    int numInputs;
    int numOutputs;
    int numChannels;

    // FIR accelerator model block mapping of FIR input channels from model inputs. 
    // Note, Simulink data is provided as 0 index based for compatibility with "C" 
    // 0 based indexing.
    // Buffer located within variable data memory section.
    int *firInputChannelMap;                        // numChannels elements
    
    // FIR accelerator circular input buffer pointer to Simulink generated buffers and buffer
    // calculated sizes. Note that there can be less inputs than outputs.
    // Buffers located within variable data memory section.
    float **firCircInputDmaBuffs;                   // numInputs elements
    int *firCircInputDmaBuffsSize;                  // numInputs elements

    // FIR accelerator pointer to circular output buffer used as DMA buffer for 
    // accelerator outputs 
    // Buffers located within variable data memory section.
    float *firCircOutputDmaBuffs;                   // numOutputs elements

    // FIR accelerator circular input and output buffers of pointers to Simulink generated
    // buffers, and input buffer sizes. Inputs may be less than the number of output channels
    // and multiple channel buffers may point to the same circular input buffer, depending on 
    // how channels are mapped to inputs in the Simulink FIR accelerator model.
    // Buffers located within variable data memory section.
    float **firInputBuff;                           // numChannels elements
    int *firInputBuffSize;                          // numChannels elements
    float **firOutputBuff;                          // numChannels elements

    // FIR accelerator channel input/output buffer data used for updating channel
    // circular input/output buffer indexes after each step
    int *firFrameStartIndex;                        // numInputs elements

    // Flag to indicate when to start updating cicular input/output buffer pointers
    // after each step
    bool startUpdates;

    // gHam FIR task memory 
    // Buffer located within variable data memory section.
    gHamFirTask_t *firTaskMemory;

    // gHam FIR task handle
    gHamTaskHandle_t hFirTaskHandle;

    // EMDMA TCB list for audio input to accelerator circular input buffer transfers
    ADI_EMDMA_DESC_STANDARD_CIRCULAR *emdmaTcbList;

    // Memory required to create fast reuse mode copy request
    unsigned char emdmaFastReuseCopyReq[ADI_EMDMA_COPY_REQ_MEMORY];

    // EMDMA callback counter for debugging
    volatile unsigned int emdmaCallbackCount;

    // Current audio input buffer pointer for EMDMA optimization
    const float *pAudioInput; 

    // Flag to indicate initialization has occurred
    bool initComplete;

    // Pointer to start of variable size sections.  Note that the size of this data
    // is specified as a mask parameter to the Generic FIR model and must be of 
    // required size.  Note that it is necessary to place the EMDMA TCB list memory, 
    // accelerator task memory, and accelerator DMA output buffers first, and that 4 
    // byte alignment is required (32 bit) for them.
    #pragma align 4
    char data[];

    // -----------------------------------------------------------------------------------
    //                           Variable Data Section Sizing
    //        Name :                                        Size in Bytes
    // -----------------------------------------------------------------------------------
    // EMDMA TCB memory :                       (numInputs * sizeof(emdmaTcbList[0]))
    // gHam task memory :                       (sizeof(firTaskMemory) + numChannels * sizeof(gHamFirTcb_t))
    // Accel circular output buffers :          (numOutputs * frameSize * sizeof(firCircOutputDmaBuffs[0]))
    // Input channel map :                      (numChannels * sizeof(firInputChannelMap[0]))
    // Accel circular input pointers :          (numInputs * sizeof(firCircInputDmaBuffs[0]))
    // Accel circular input sizes :             (numInputs * sizeof(firCircInputDmaBuffsSize[0]))
    // Accel channel input pointers :           (numChannels * sizeof(firInputBuff[0]))
    // Accel channel input sizes :              (numChannels * sizeof(firInputBuffSize[0]))
    // Accel channel output pointers :          (numChannels * sizeof(firOutputBuff[0]))
    // Circular input channel frame indexes :   (numInputs * sizeof(firFrameStartIndex[0]))

} firAccelWorkMemory_t;


/***  Module Function Prototypes  *********************************************/

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
extern void fir_accelerator_init( unsigned int *reserved,
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
                                  unsigned int workMemorySize );


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
 */
extern void fir_accelerator_process( float *output,
                                     const float *input, 
                                     unsigned int *workMemory,
                                     unsigned int numOut );


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
extern unsigned int fir_accelerator_cancel( unsigned int *workMemory );


#endif /* __FIR_ACCELERATOR_H__ */
