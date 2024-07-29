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
 *  @file fireven_hexagon.h
 *  @brief Connects model FIR Even code generation 
 *
 *  @ingroup dsp
 *
 */

#ifndef __FIREVEN_HEXAGON_H__
#define __FIREVEN_HEXAGON_H__

#ifndef MAX_THREAD_NUM_FIREVEN
#define MAX_THREAD_NUM_FIREVEN 6
#endif

//#define FIREVEN_MULTITHREADING

/*#if (MAX_THREAD_NUM_FIREVEN != 4) && (MAX_THREAD_NUM_FIREVEN != 6)
#error "Support only 4 or 6 threads!"
#endif*/

#ifndef THREAD_NAME_LEN
#define THREAD_NAME_LEN 20
#endif

#ifndef STACK_SIZE
#define STACK_SIZE 256
#endif

#ifdef FIREVEN_MULTITHREADING
#include "qurt.h"
#include "qurt_sem.h"
#endif

/**
 *  fireven_init
 *
 *  @brief Initialization of FIR Even.
 *
 *  @param[in]     frame_size      Number of samples per frame
 *  @param[in]     num_of_ch       Number of channels
 *  @param[in]     num_of_filters  Number of filters
 *  @param[in]     filter_length   Length of filter       
 *  @param[in]     coeffs          Coefficient buffer pointer
 *
 */
void fireven_init(unsigned int *scratchMemory, 
                  unsigned int frame_size, 
                  unsigned int num_of_ch, 
                  unsigned int num_of_filters, 
                  unsigned int filter_length, 
                  const float* coeffs,
                  unsigned int* workMemory,
				  unsigned int maxNumberOfThreads);


/**
 *  fireven_process_mt
 *
 *  @brief Run-time processing of FIR Even in case of coeff length larger than frame size
 *
 *  @param[out]    y               Audio Data Out
 *  @param[in]     x               Audio Data In
 *  @param[in,out] newstate        State buffer pointer
 *  @param[in]     activeCoeffSet  
 *
 */
void fireven_process_mt(float* __attribute__ ((__aligned__(8))) y, 
                        float* x, 
                        unsigned int *activeCoeffSet, 
                        float *newstate,
                        unsigned int* workMemory);


/**
 *  fireven_process_lt
 *
 *  @brief Run-time processing of FIR Even in case of coeff length less than frame size
 *
 *  @param[out]    y               Audio Data Out
 *  @param[in]     x               Audio Data In
 *  @param[in,out] newstate        State buffer pointer
 *  @param[in]     activeCoeffSet  
 *
 */
void fireven_process_lt(float* y, 
                        float* x, 
                        unsigned int const *activeCoeffSet, 
                        float *newstate,
                        unsigned int* workMemory);


// FIR Even model work memory structure used for processing
#ifdef FIREVEN_MULTITHREADING
typedef struct
{
	float* y;
	float* x;
	const unsigned int *activeCoeffSet;
	float *newstate;
	unsigned int* workMemory;
	unsigned int startChannel;
	unsigned int numChannels;
	qurt_sem_t semaWork;
	qurt_sem_t semaDone;
	unsigned int state_spread;
}FirEven_thread_args;
#endif

typedef struct FirEven_s
{
    unsigned int frame_size;
    unsigned int num_of_ch;
    unsigned int numFilters;
    unsigned int filterLength;
    float*  flippedCoeff;
#ifdef FIREVEN_MULTITHREADING
    unsigned int numThreadsToRun;
    void *funcPtr;
    FirEven_thread_args thread_arg [MAX_THREAD_NUM_FIREVEN];
	void* thread_stack_addr[MAX_THREAD_NUM_FIREVEN];
	qurt_thread_t threads[MAX_THREAD_NUM_FIREVEN];
	qurt_thread_attr_t attr[MAX_THREAD_NUM_FIREVEN];
	unsigned int stack[MAX_THREAD_NUM_FIREVEN*STACK_SIZE];
#endif
} FirEven_t;


#endif /* __FIREVEN_HEXAGON_H__ */
