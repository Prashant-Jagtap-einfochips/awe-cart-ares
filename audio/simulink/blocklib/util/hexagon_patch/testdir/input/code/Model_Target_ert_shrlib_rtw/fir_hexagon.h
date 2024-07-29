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
 *  @file fir_hexagon.h
 *  @brief Connects model FIR code generation with Qualcomm Hexagon Library (QHL)
 *
 *  @ingroup dsp
 *
 */
 
#ifndef __FIR_HEXAGON_H__
#define __FIR_HEXAGON_H__
#define MULTITHREAD_ENABLE          (0)

#include <stdbool.h>    /* Required for boolean data type */
#include "hexagon_protos.h"

#if MULTITHREAD_ENABLE
#include "qurt.h"
#include "qurt_thread.h"
#endif
//#include "rtwtypes.h"
//#include "qhdsp.h"


#define FIR_FLOATING_POINT          (1)
#define FIR_FIXED_POINT_16          (2)
#define FIR_PROCESS                 FIR_FLOATING_POINT
#define FIR_GENERIC_STACK_SIZE      (1024)


/**
 *  fir_init
 *
 *  @brief Initializes data used with Qualcomm Hexagon Library (QHL)
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
void fir_init(unsigned int *scratchMemory,
              const float *input,
              float *coeffs,
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
 *  fir_process
 *
 *  @brief Run-time processing that calls into  Qualcomm Hexagon Library (QHL) 
 *
 *  @param [out] output Output audio buffer pointer.
 *  All additional parameters described in init function.
 *
 */
void fir_process(float *output,
                 const float *input,
                 unsigned int *workMemory,
                 unsigned int numOut);


typedef struct FirGenericWorkMemory_s
{
    unsigned int frame_size;
    unsigned int numIn;
    unsigned int numOut;
    unsigned int numFilters;
    unsigned int state_offset;

    float* coeffs;
    float* states;
    unsigned int statesSize;

    unsigned int* filterLengths;
    unsigned int* inputMapping;
    unsigned int* filterMapping;

    unsigned int* inputMappingTbl;
    unsigned int* filterOffets;
    unsigned int* stateOffsets;

    unsigned int* workMemory;
    unsigned int workMemorySize;

// Multi-thread entires
    const float* input_save;
    float* output_save;
    unsigned int num_of_loop;
    unsigned int thread_offset[4];
   
#if MULTITHREAD_ENABLE
    unsigned int __attribute__ ((__aligned__(8))) stack[3][FIR_GENERIC_STACK_SIZE];
    qurt_thread_t tid[4];
	qurt_thread_attr_t attr[4];
	qurt_sem_t sem[8];
#endif
} FirGenericWorkMemory_t;



#endif    /* __FIR_HEXAGON_H__ */	
