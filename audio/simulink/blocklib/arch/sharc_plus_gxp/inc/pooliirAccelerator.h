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
 *  @file pooliirAccelerator.h
 *  @brief Connects model IIR accelerator code generation with Griffin Lite XP (GLXP)
 *         Hardware Accelerator Manager (HAM)
 *
 *  @ingroup dsp
 *
 */

#ifndef __POOLIIR_ACCELERATOR_H__
#define __POOLIIR_ACCELERATOR_H__


/***  Include Files ***********************************************************/

#include "gxpHAM.h"


/***  External Variable References  *******************************************/
 

/***  Module Macros  **********************************************************/

// Calculates total memory size in bytes needed for iirAccelWorkMemory_t 
// variable data section and includes task and output buffer cache alignment
// in the event this is ever placed in cached memory, which is NOT 
// recommended. Note that the driver cache aligns the task memory and
// gxpHamIirTcb_t.
#define IIR_DMA_OUTPUT_BUF_SIZE(numCh, frameSize) \
            ((numCh) * (frameSize) * sizeof(((iirAccelWorkMemory_t*)0)->iirCircOutputDmaBuffs[0]))

#define IIR_VARIABLE_DATA_SECTION_SIZE( numCh, frameSize ) ( \
                (sizeof(*((iirAccelWorkMemory_t*)0)->iirTaskMemory) + (numCh) * sizeof(gxpHamIirTcb_t)) + \
                ADI_CACHE_ROUND_UP_SIZE(IIR_DMA_OUTPUT_BUF_SIZE(numCh, frameSize), char) )


/***  Module Types  ***********************************************************/

// IIR accelerator model work memory structure used for each instance of the
// model.
typedef struct iirAccelWorkMemory_s
{
    // Scratch area that code gen uses. Reserved and not intended for usage
    // by accelerator functionality
    unsigned int scratchMemoryLocation;
    
    // Accelerator frame size and number channels
    int frameSize;
    int numChannels;

    // Current audio input buffer pointer
    const float *pAudioInput;

    // IIR accelerator pointer to circular output buffer used as DMA buffer for 
    // accelerator outputs.
    // Buffers located within variable data memory section.
    float *iirCircOutputDmaBuffs;                   // numOutputs elements

    // Device ID of GXP IIR Accelerator to use
    gxpHamIirDevice_t iirAccelDevId;

    // gxpHam IIR task memory 
    // Buffer located within variable data memory section.
    gxpHamIirTask_t *iirTaskMemory;

    // gxpHam IIR task handle
    gxpHamTaskHandle_t hIirTaskHandle;

    // Flag to indicate initialization has occurred
    bool initComplete;

    // Pointer to start of variable size sections.  Note that the size of this
    // data is calculated from the mask parameters to the IIR model.  Note that
    // it is necessary to place the accelerator task memory and DMA buffers first
    // and that 64 byte alignment (cache line length) is necessary in the event
    // they are ever placed in cached memory, which is NOT recommended.
    // Task memory should be first followed by the output buffers. Alignment is
    // with respect to the start of the structure and in order to guarantee 
    // absolute 64 byte memory alignment each structure object must also have
    // 64 byte alignment.
    #pragma align 64
    char data[];

    // -----------------------------------------------------------------------------------
    //                           Variable Data Section Sizing
    //                        Without Buffer Size Cache Alignment
    //        Name :                                        Size in Bytes
    // -----------------------------------------------------------------------------------
    // gxpHam task memory :                     (sizeof(iirTaskMemory) + numChannels * sizeof(gxpHamIirTcb_t))
    // Accel circular output buffers :          (numChannels * frameSize * sizeof(iirCircOutputDmaBuffs[0]))

} iirAccelWorkMemory_t;


/***  Module Function Prototypes  *********************************************/

/**
 *  iir_accelerator_hw_init()
 *
 *  @brief One-time initialization of hardware used by IIR accelerator
 *
 *  @param[out]    reserved  Artifact of code generation, not intended for use here
 *  @param[in]     deviceId  Device ID of accelerator to use
 *  @param[in]     coreId    DSP Core ID accelerator device assigned to
 *
 *  @warning It is not recommended to use reserved memory. When passed in, this will actually
 *           point to the first element of workMemory. The reason for this has to do with
 *           making sure Simulink does not optimize out the call to this function. So it is
 *           not recommended to use, as it's value may change outside of these functions.
 */
extern void iir_accelerator_hw_init( unsigned int *reserved, 
                                     unsigned int deviceId,
                                     unsigned int coreId );


/**
 *  iir_accelerator_init()
 *
 *  @brief Initializes data used by IIR accelerator and creates task list
 *
 *  @param[out]    reserved       Artifact of code generation, not intended for use here
 *  @param[in]     input          Input audio buffer pointer
 *  @param[in]     frameSize      Number of samples per frame
 *  @param[in]     numChannels    Number of channels
 *  @param[in]     numStages      Pointer to array of number of filter SOSs
 *  @param[in]     coeffs         Coefficient buffer pointer for all SOSs
 *  @param[in]     deviceId       Device ID of accelerator to use
 *  @param[in,out] workMemory     Pointer to memory buffer allocated by model, used by this
 *                                accelerator wrapper
 *  @param[in]     workMemorySize Size of work memory allocated by model
 *
 *  @warning It is not recommended to use reserved memory. When passed in, this will actually
 *           point to the first element of workMemory. The reason for this has to do with
 *           making sure Simulink does not optimize out the call to this function. So it is
 *           not recommended to use, as it's value may change outside of these functions.
 */
extern void iir_accelerator_init( float *reserved,
                                  const float *input,
                                  unsigned int frameSize,
                                  unsigned int numChannels,
                                  const unsigned int *numStages,
                                  const float *coeffs,
                                  unsigned int deviceId,
                                  float *workMemory,
                                  unsigned int workMemorySize );


/**
 *  iir_accelerator_process()
 *
 *  @brief Run-time processing of IIR accelerator, retrieving accelerator outputs
 *         from previous frame and initiating accelerator processing of current
 *         frame.
 *
 *  @param[out]    output       Output audio buffer pointer
 *  @param[in]     input        Input audio buffer pointer
 *  @param[in,out] workMemory   Pointer to memory buffer allocated by model, used by this
 *                              accelerator wrapper
 */
extern void iir_accelerator_process( float *output,
                                     const float *input,
                                     float *workMemory );


/**
 *  iir_accelerator_cancel()
 *
 *  @brief IIR accelerator cancel of further audio frame step processing and of
 *         any accelerator DMA transfer currently in progress.
 *
 *  @param[in,out] workMemory   Pointer to memory buffer allocated by model and used
 *                              by IIR Accelerator
 * 
 *  @returns Status of 0 for success and 1 for failure
 */
extern unsigned int iir_accelerator_cancel( float *workMemory );


#endif /* __POOLIIR_ACCELERATOR_H__ */

