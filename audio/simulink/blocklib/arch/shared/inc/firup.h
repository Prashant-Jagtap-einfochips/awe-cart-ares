/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: firup.h
 *
 */

#include <stdlib.h>
#include <string.h>

#define MAX_THREAD_NUM 4
//#define PTHREAD
#define QURTTHREAD

#ifdef PTHREAD
   #include <pthread.h>
   #include <semaphore.h>
#endif

#ifdef QURTTHREAD
   #include "qurt.h"
   #include "qurt_sem.h"
#endif


void firup_init(  unsigned int *scratchMemory,
                  float *coeffs,
                  unsigned int frameSize,
                  unsigned int factorD,
                  unsigned int filterLength,
                  unsigned int num_of_ch,
                  unsigned int *workMemory);

void firup_process(float* y,
                   float* x,
                   float* delay_line,
                   unsigned int* workMemory,
                   unsigned int UpFactor);


// FIR Even model work memory structure used for processing

typedef struct
{
   float* y;
   float* x;
   float* delay_line;
   unsigned int* workMemory;
   unsigned int numChannels;
   unsigned int state_offset;
   unsigned int startChannel;
   unsigned int UpFactor;
#ifdef PTHREAD
   sem_t semaWork;
   sem_t semaDone;
#endif
#ifdef QURTTHREAD
   qurt_sem_t semaWork;
   qurt_sem_t semaDone;
#endif
}firup_thread_args_t;

typedef struct FirUp_t
{
    unsigned int frame_size;
    unsigned int num_of_ch;
    unsigned int factorD;
    unsigned int subFilterLength;
    unsigned int stateOffset;
    unsigned int outframeisze;
    float* Coeff;
    unsigned int numThreadsToRun;
    firup_thread_args_t thread_arg [MAX_THREAD_NUM];
    void* thread_stack_addr[MAX_THREAD_NUM];
#ifdef PTHREAD
   pthread_t threads[MAX_THREAD_NUM];
#endif
#ifdef QURTTHREAD
   qurt_thread_t threads[MAX_THREAD_NUM];
   qurt_thread_attr_t attr[MAX_THREAD_NUM];
#endif
} FirUp_t;
