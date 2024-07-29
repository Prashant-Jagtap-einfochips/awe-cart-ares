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
 *  @file pooliir_hexgon.h
 *  @brief Connects Simulink model IIR code generation with Qualcomm Hexagon Library (QHL)
 *         supplied qhdsp_iir_af() function, which has been optimized for the target.
 *
 *  @ingroup dsp
 *
 */

#ifndef __POOL_IIR_HEXAGON_H__
#define __POOL_IIR_HEXAGON_H__
#define MULTITHREAD_POOLIIR_ENABLE          (0)

#if MULTITHREAD_POOLIIR_ENABLE
#include "qurt.h"
#include "qurt_sem.h"
#endif

#include "math.h"
#define MAX_THREAD_NUM 4

/***  Module Function Prototypes  *********************************************/
void flt_IIR_casBiquad(float *xin, float *coef, float *state, int nsec, int nsamples, float *out);
void flt_IIR_casBiquad_c(float *xin, float *coef, float *state, int nsec, int nsamples, float *out);

/**
 *  pooliir_init()
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
                   const float         *input,
                   unsigned int        frameSize,
                   unsigned int        numChannels,
                   const unsigned int  *numStages,
                   const float         *coeffs,
                   float               *workMemory,
                   unsigned int        workMemorySize,
				   unsigned int        maxNumberOfThreads,
				   unsigned int        maxNumSosCoeff);


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
                     float            *workMemory);


// Arguments for threads
#if MULTITHREAD_POOLIIR_ENABLE   
typedef struct
{
    float* out_samples;
    const float* in_samples;
    float *workMemory;

    unsigned int numChannels;
    unsigned int startChannel;
    qurt_sem_t semaWork;
    qurt_sem_t semaDone;
}Pooliir_thread_args;
#endif

// Pooliir model work memory structure used for processing
typedef struct Pooliir_s
{
    unsigned int frame_size;
    unsigned int num_of_ch;
    const unsigned int* num_of_stage;
    float* flippedCoeff;

#if MULTITHREAD_POOLIIR_ENABLE    
    unsigned int numThreadsToRun;
    qurt_thread_t threads[MAX_THREAD_NUM];
    qurt_thread_attr_t attr[MAX_THREAD_NUM];
    Pooliir_thread_args thread_arg[MAX_THREAD_NUM];
    void* thread_stack_addr[MAX_THREAD_NUM];
#endif

    float *workMemory;
	unsigned int __attribute((aligned(8))) num_sos_coeff;
} Pooliir_t;



#endif /* __POOL_IIR_HEXAGON_H__ */
