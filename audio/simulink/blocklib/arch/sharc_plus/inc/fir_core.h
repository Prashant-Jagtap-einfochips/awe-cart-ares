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

#ifndef __FIR_CORE_H__
#define __FIR_CORE_H__

#include <stdbool.h>    /* Required for boolean data type */

/**
 *  fir_core_init
 *
 *  @brief Initializes data used with ADI SHARC+ assembly
 *
 *  @param[out]    scratchMemory Artifact of code generation, not intended for use here
 *  @param[in]     input         Input audio buffer pointer
 *  @param[in]     coeffs        Coefficient bufffer pointer
 *  @param[in,out] states        State buffer pointer
 *  @param[in]     filterLengths Pointer to array of filter lengths
 *  @param[in]     inputMapping  Pointer to array of input maps
 *  @param[in]     filterMapping Pointer to array of filter maps
 *  @param[in]     frameSize     Number of samples per frame
 *  @param[in]     numCoeffSet   Number of coefficient sets
 *  @param[in]     numIn         Number of input channels
 *  @param[in]     numOut        Number of output channels
 *  @param[in,out] workMemory    Pointer to memory buffer allocated by model, used by this
 *                               accelerator wraper
 *
 *  @warning It is not recommended to use scratchMemory. When passed in, this will actually
 *           point to the first element of workMemory. The reason for this has to do with
 *           making sure Simulink does not optimize out the call to this function. So it is
 *           not recommended to use, as it's value may change outside of these functions.
 */
void fir_core_init(unsigned int *scratchMemory,
                   const float *input,
                   const float *coeffs,
                   float *states,
                   const unsigned int *filterLengths,
                   const unsigned int *inputMapping,
                   const unsigned int *filterMapping,
                   unsigned int frameSize,
                   unsigned int numCoeffSet,
                   unsigned int numIn,
                   unsigned int numOut,
                   unsigned int *workMemory);


/**
 *  fir_core_process
 *
 *  @brief Run-time processing that calls into ADI assembly routine 
 *
 *  @param [out] output Output audio buffer pointer.
 *  All additional parameters described in init function.
 *
 */
void fir_core_process(float *output,
                      const float *input,
                      unsigned int *workMemory,
                      unsigned int numOut);


typedef struct workMemory_s
{
    // Scratch area that code gen uses, not intended for usage
    // by accelerator functionality
    unsigned int scratchMemoryLocation;
    
    // Pointer to input pointer array
    const float** inputPtrArray;
    
    // Pointer to coeff pointer array
    const float** coeffPtrArray;
    
    // Pointer to state pointer array
    float** statePtrArray;
    
    // Pointer to state sharing enabled per output array
    int* stateSharingEnabled;
    
    // Pointer to tap length per output array
    int* taps;

    // Pointer to mapped audio input index for each input channel
    int* mappedInputInd;

    // Audio frame size
    int frameSize;

    // Current audio input buffer pointer for optimization
    const float *pAudioInput; 

    // Flag to indicate initialization has occured
    bool initComplete;

    // Pointer to start of variable size sections
    #pragma align 4
    char data[];

} workMemory_t;


/**
 *  fir_vec_mc
 *
 *  @brief Function prototype for ADI Multichannel FIR assembly
 *
 *  @param[in] inputPtrArray       Input buffer address locations per output
 *  @param[in] outputPtr           Output buffer address location
 *  @param[in] coeffPtrArray       Coefficient buffer address locations per output
 *  @param[in] statePtrArray       State buffer address locations per output
 *  @param[in] stateSharingEnabled Pointer to array of state sharing enabled per output
 *  @param[in] taps                Pointer to array of filter lengths per output
 *  @param[in] frameSize           Number of samples per frame
 *  @param[in] numOut              Number of output channels
 *
 *  @note 
 *  1. stateSharingEnabled is to 1 if the corresponding filter shares the 
 *  same state buffer with any other output channel with lesser array 
 *  index, else set to 0. This information is used by the function to 
 *  perform state update only for the first time and skip it for the 
 *  subsequent channels sharing the same state buffer. 
 *
 *  2. The number of taps for the channels sharing the same state buffer 
 *  MUST be the same.
 *
 */
void fir_vec_mc(const float*  inputPtrArray[],
                float*  outputPtr,
                const float*  coeffPtrArray[],
                float*  statePtrArray[],
                int*    stateSharingEnabled,
                int*    taps,
                int     samples,
                int     numOutput );

#endif
	
