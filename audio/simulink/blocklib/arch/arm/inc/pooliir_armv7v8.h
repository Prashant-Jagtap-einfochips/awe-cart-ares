/*****************************************************************************************
 *
 * BOSE CORPORATION
 * COPYRIGHT 2022 BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:

 *     BOSE CORPORATION
 *     The Mountain
 *     Framingham, MA 01701-9168

*****************************************************************************************/

/**
 *  @file pooliir_armv7v8.h
 *  @brief Connects Simulink model IIR code generation with ARM Neon V7/V8
 *         supplied qhdsp_iir_af() function, which has been optimized for the target.
 *
 *  @ingroup dsp
 *
 */

#ifndef __POOL_IIR_PROCESS_ARMV7V8_H__
#define __POOL_IIR_PROCESS_ARMV7V8_H__




#define POOLIIR_FOR_ARM



/***  Module Function Prototypes  *********************************************/

/**
 *  pooliir_armv7v8_init()
 *
 *  @brief Initializes data used by Hexagon assebly code
 *
 *  @param[out]    reserved       Artifact of code generation, not intended for use here
 *  @param[in]     input          Input audio buffer pointer
 *  @param[in]     frameSize      Number of samples per frame
 *  @param[in]     numChannels    Number of channels
 *  @param[in]     numStages      Pointer to array of number of filter SOSs
 *  @param[in]     coeffs         Coefficient buffer pointer for all SOSs
 *  @param[in,out] workMemory     Pointer to memory buffer allocated by QHL assembly code
 *  @param[in]     workMemorySize Size of work memory allocated by model
 *
 *  @warning It is not recommended to use reserved memory. When passed in, this will actually
 *           point to the first element of workMemory. The reason for this has to do with
 *           making sure Simulink does not optimize out the call to this function. So it is
 *           not recommended to use, as it's value may change outside of these functions.
 */
void pooliir_init( float               *reserved,
                   float               *input,
                   unsigned int        frameSize,
                   unsigned int        numChannels,
                   unsigned int        *numStages,
                   float               *coeffs,
                   float               *workMemory,
                   unsigned int        workMemorySize );


/**
 *  pooliir_process()
 *
 *  @brief Run-time interface to process audio stream using optimized IIR filtering routine.
 *
 *
 *  @param[out]    out_samples    Output audio sample buffer pointer
 *  @param[in]     in_samples     Input audio buffer pointer
 *  @param[in,out] workMemory     Pointer to memory buffer allocated by QHL assembly code
 *
 */
void pooliir_process(float            *out_samples,
                     float            *in_samples, 
                     float            *workMemory  );



// Pooliir model work memory structure used for processing
typedef struct Pooliir_s
{
    unsigned int frame_size;
    unsigned int num_of_ch;
    unsigned int* num_of_stage;
    float* flippedCoeff;
} Pooliir_t;



#endif /* __POOL_IIR_PROCESS_ARMV7V8_H__ */