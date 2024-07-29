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
 *  @file pooliir_hexgon.c
 *  @brief Connects Simulink model IIR code generation with Qualcomm Hexagon Library (QHL)
 *         supplied qhdsp_iir_af() function, which has been optimized for the target.
 *
 *  @ingroup dsp
 *
 */

#if __hexagon__
#include <stdlib.h>
#include "qhmath.h"
#else
#include <math.h>
#include <complex.h>
#include <malloc.h>
#endif

#include "qhdsp.h"
#include "qhdsp_iir.h"
#include "qhmath.h"
#include "qhblas.h"
#include "pooliir_hexagon.h"
#include "hexagon_protos.h"

#define NUM_COEFF_PER_STAGE_FOR_BIQUAD                       (4)
#define NUM_STATE_PER_STAGE_FOR_BIQUAD                       (2)
#define MAX_FRAME_SIZE                                       (32)

#if MULTITHREAD_POOLIIR_ENABLE
void pooliir_process_thread( void *arg );
#endif

void pooliir_init( float               *reserved,
                   const float         *input,
                   unsigned int        frameSize,
                   unsigned int        numChannels,
                   const unsigned int  *numStages,
                   const float         *coeffs,
                   float               *workMemory,
                   unsigned int        workMemorySize,
				   unsigned int        maxNumberOfThreads,
				   unsigned int        maxNumSosCoeff) 
{
	Pooliir_t* pooliir;
	pooliir = (Pooliir_t*) workMemory;

	pooliir->frame_size = frameSize;
	pooliir->num_of_ch = numChannels;
	pooliir->num_of_stage = (unsigned int*) numStages;
	pooliir->flippedCoeff = (float*) coeffs;
	pooliir->num_sos_coeff = maxNumSosCoeff;

#if MULTITHREAD_POOLIIR_ENABLE
    pooliir->numThreadsToRun = maxNumberOfThreads;
	if (maxNumberOfThreads > 1) { // if multithreaded

		unsigned int tLoad = roundf((float) numChannels / (float) maxNumberOfThreads);

		for (int i = 0; i < maxNumberOfThreads - 1; i++) {
			pooliir->thread_arg[i].numChannels = tLoad;
			pooliir->thread_arg[i].startChannel = i * tLoad;
		}
		//sum of number of channels processed by previous threads
		unsigned int lastTLoad = pooliir->thread_arg[maxNumberOfThreads - 2].startChannel + pooliir->thread_arg[maxNumberOfThreads - 2].numChannels;

		pooliir->thread_arg[maxNumberOfThreads - 1].numChannels = numChannels - lastTLoad;
		pooliir->thread_arg[maxNumberOfThreads - 1].startChannel = lastTLoad;

		pooliir->numThreadsToRun = maxNumberOfThreads;

		for (int i = 0; i < pooliir->numThreadsToRun; i++) {

			qurt_sem_init_val(&pooliir->thread_arg[i].semaWork, 0);
			qurt_sem_init_val(&pooliir->thread_arg[i].semaDone, 0);

			pooliir->thread_stack_addr[i] = malloc(STACK_SIZE);
			qurt_thread_attr_init(&pooliir->attr[i]);

			qurt_thread_attr_set_stack_addr(&pooliir->attr[i], pooliir->thread_stack_addr[i]);
			qurt_thread_attr_set_stack_size(&pooliir->attr[i], STACK_SIZE);
			qurt_thread_attr_set_priority(&pooliir->attr[i], 0);

			qurt_thread_create(&pooliir->threads[i], &pooliir->attr[i], pooliir_process_thread, (void *) &pooliir->thread_arg[i]);
		}
	}
#endif	
}

#if MULTITHREAD_POOLIIR_ENABLE
void pooliir_process_thread( void *arg ) {

	Pooliir_thread_args *pooliir_args_local = (Pooliir_thread_args*) arg;

	do {
		qurt_sem_down(&pooliir_args_local->semaWork);

		int i, j, num_ch, framesize;
		float* xptr;
		float* yptr;
		float* cptr;
		float* sptr;
		Pooliir_t* pooliir;

		pooliir = (Pooliir_t*) pooliir_args_local->workMemory;
		num_ch = pooliir->num_of_ch;
		framesize = pooliir->frame_size;
		xptr = (float*) pooliir_args_local->in_samples + framesize * pooliir_args_local->startChannel;
		yptr = pooliir_args_local->out_samples + framesize * pooliir_args_local->startChannel;
		cptr = pooliir->flippedCoeff;
		sptr = pooliir_args_local->workMemory + (sizeof(Pooliir_t) / sizeof(float)) * (1 + pooliir_args_local->startChannel);

		for (i = pooliir_args_local->startChannel; i < pooliir_args_local->startChannel + pooliir_args_local->numChannels; i++) {
			// Call iir function
			qhdsp_iir_af((const float*) xptr, (const float*) cptr, sptr, pooliir->num_of_stage[i], framesize, yptr);
			xptr += framesize;
			yptr += framesize;
			cptr += pooliir->num_of_stage[i] * NUM_COEFF_PER_STAGE_FOR_BIQUAD;
			sptr += pooliir->num_of_stage[i] * NUM_STATE_PER_STAGE_FOR_BIQUAD;
		}

		qurt_sem_up(&pooliir_args_local->semaDone);

	} while (1);
}
#endif

void pooliir_process(float *out_samples,
                     float *in_samples,
                     float *workMemory) 
{
	int i, j, num_ch, framesize;
	float* yptr;
	float* cptr;
	float* sptr;
	float* xptr;
	float_a8_t scaled_input[MAX_FRAME_SIZE];
	Pooliir_t* pooliir;
	pooliir = (Pooliir_t*) workMemory;

#if MULTITHREAD_POOLIIR_ENABLE
	if (pooliir->numThreadsToRun == 1) {
#endif		
		num_ch = pooliir->num_of_ch;
		framesize = pooliir->frame_size;
		yptr = out_samples;
		xptr = in_samples;
		cptr = pooliir->flippedCoeff;
		sptr = workMemory + (sizeof(Pooliir_t) / sizeof(float));

		for (i = 0; i < num_ch; i++) {
            float per_ch_gain = *(pooliir->flippedCoeff + pooliir->num_sos_coeff + i);
            qhblas_vector_scaling_af((float_a8_t *) xptr, 
                                                    per_ch_gain, 
                                                    scaled_input, 
                                      (uint32_t) framesize);
            flt_IIR_casBiquad(scaled_input, cptr, sptr, pooliir->num_of_stage[i], framesize, yptr);
            //flt_IIR_casBiquad_c(scaled_input, cptr, sptr, pooliir->num_of_stage[i], framesize, yptr);
			yptr += framesize;
			xptr += framesize;
			cptr += pooliir->num_of_stage[i] * NUM_COEFF_PER_STAGE_FOR_BIQUAD;
			sptr += pooliir->num_of_stage[i] * NUM_STATE_PER_STAGE_FOR_BIQUAD;
	}

#if MULTITHREAD_POOLIIR_ENABLE
	} else {

		for (int i = 0; i < pooliir->numThreadsToRun; i++) {
			pooliir->thread_arg[i].out_samples = out_samples;
			pooliir->thread_arg[i].in_samples = in_samples;
			pooliir->thread_arg[i].workMemory = workMemory;
		}

		for (int i = 0; i < pooliir->numThreadsToRun; i++) {
			qurt_sem_up(&pooliir->thread_arg[i].semaWork);
		}

		for (int i = 0; i < pooliir->numThreadsToRun; i++) {
			qurt_sem_down(&pooliir->thread_arg[i].semaDone);
		}
	}
#endif	
}

void flt_IIR_casBiquad_c( float *xin, float *pCoef, float *pState, \
                            int nSec, int nsamples, float *out)
{
   float *coef, *state;
   float t, x;
   int i, j;

   for(i = 0; i < nsamples; i++) {
      coef = pCoef;
      state = pState;
      x = xin[i];

      for (j=0; j< nSec; j++) {

          t = x + coef[0]*state[0] + coef[2]*state[1];
          x = t + coef[1]*state[0] + coef[3]*state[1];

          state[1] = state[0];
          state[0] = t;

          coef  += 4;
          state += 2;
      }
      out[i] = x;
   }
}

