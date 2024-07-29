/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: firup.c
 *          source code for firup (reconstruction filter) shared between hexagon, 
 *          arm neon and generic variant
 *
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hexagon_sim_timer.h>
#include "firup.h"
#include "hexagon_protos.h"
#include "qhdsp.h"
#include "math.h"

#include "HAP_perf.h"
#define FARF_HIGH 1
#include <HAP_farf.h>

#define COEFF_LENGTH_LARGER_THAN_FRAME_SIZE
#define MAX_FRAME_SIZE                                (32)
#define STACK_SIZE 1024

//Threshold for starting multithreading operation (filter elements* upsampling factor * number of channels)
#define MULTITHREADING_ELEMENT_THRESHOLD  1250

// Local functions prototypes
static void firup_process_thread(void *arg);
static void firup_process_single(float* y,
                   float* x,
                   float* delay_line,
                   unsigned int* workMemory,
                   unsigned int UpFactor);

static void firup_process_parallel(float* y,
                   float* x,
                   float* delay_line,
                   unsigned int* workMemory,
                   unsigned int UpFactor);

#ifdef FIRUP_FOR_HEXAGON
#elif defined FIRUP_FOR_ARM_NEON
#else

int dly_wr_idx = 0;
int fir_generic(float *in_samples, float *delay_line, float *coefs, unsigned int taps, unsigned int length, float *out_samples)
{
    float sum1, sum2;
    int i, j, dly_rd1_idx, dly_rd2_idx;
    for(i = 0; i < length; i+=2)
    {
        sum2 = 0;
        delay_line[dly_wr_idx] = in_samples[i];
        dly_rd1_idx = dly_wr_idx;

        // update delay_line write index (++)
        dly_wr_idx = (dly_wr_idx + 1) < taps ? dly_wr_idx + 1 : 0;
        sum1 = coefs[taps-1] * delay_line[dly_wr_idx];
        dly_rd2_idx = dly_wr_idx;
        delay_line[dly_wr_idx] = in_samples[i+1];

        // update delay_line write index (++)
        dly_wr_idx = (dly_wr_idx + 1) < taps ? dly_wr_idx + 1 : 0;

        for(j = 0; j < taps-1; j++)
        {
            sum1 += coefs[j] * delay_line[dly_rd1_idx];
            sum2 += coefs[j] * delay_line[dly_rd2_idx];

            // update delay_line read index (--)
            dly_rd1_idx = (dly_rd1_idx - 1) < 0 ? taps - 1 : dly_rd1_idx - 1;
            // update delay_line read index (--)
            dly_rd2_idx = (dly_rd2_idx - 1) < 0 ? taps - 1 : dly_rd2_idx - 1;
        }

        sum2 += coefs[taps-1] * delay_line[dly_rd2_idx];

        out_samples[i] = sum1;
        out_samples[i+1] = sum2;
    }

    return 0;
}
#endif

void firup_init(  unsigned int *scratchMemory,
                  float *coeffs,
                  unsigned int frameSize,
                  unsigned int factorD,
                  unsigned int SubFilterLength,
                  unsigned int num_of_ch,
                  unsigned int *workMemory)
{
    FirUp_t* pFir;

    // Create output struct in the work memory
    pFir = (FirUp_t*)workMemory;
    pFir->frame_size = frameSize;
    pFir->num_of_ch = num_of_ch;
    pFir->factorD = factorD;
    pFir->subFilterLength =  SubFilterLength; 
    pFir->stateOffset = 0;
    pFir->outframeisze = frameSize * factorD;
    pFir->Coeff = (float*)coeffs;

    // Calculate optimal number of threads for given workload
    pFir->numThreadsToRun = 1;
    pFir->thread_arg[0].numChannels=num_of_ch;
    pFir->thread_arg[0].state_offset = 0;
    pFir->thread_arg[0].startChannel=0;

    if (num_of_ch * SubFilterLength * factorD > MULTITHREADING_ELEMENT_THRESHOLD) {
       pFir->numThreadsToRun = MAX_THREAD_NUM;
       unsigned int tLoad=ceilf((float)num_of_ch/(float)pFir->numThreadsToRun);

       for(int i=0;i<pFir->numThreadsToRun-1;i++){
          pFir->thread_arg[i].numChannels=tLoad;
          pFir->thread_arg[i].state_offset = 0;
          pFir->thread_arg[i].startChannel=i*tLoad;
       }

       //sum of number of channels processed by previous threads
       unsigned int lastTLoad = pFir->thread_arg[pFir->numThreadsToRun-2].startChannel + pFir->thread_arg[pFir->numThreadsToRun-2].numChannels;

       pFir->thread_arg[pFir->numThreadsToRun-1].numChannels= num_of_ch - lastTLoad;
       pFir->thread_arg[pFir->numThreadsToRun-1].state_offset = 0;
       pFir->thread_arg[pFir->numThreadsToRun-1].startChannel= lastTLoad;

    } else {
       pFir->numThreadsToRun = 1;
       pFir->thread_arg[0].numChannels=num_of_ch;
       pFir->thread_arg[0].state_offset = 0;
       pFir->thread_arg[0].startChannel=0;
    }
}

void firup_process(float* y,
                   float* x,
                   float* delay_line,
                   unsigned int* workMemory,
                   unsigned int UpFactor)
{
   FirUp_t* pFir = (FirUp_t*)workMemory;
   if  (pFir->numThreadsToRun == 1) {
      firup_process_single(y,x,delay_line,workMemory,UpFactor);
   } else {
      firup_process_parallel(y,x,delay_line,workMemory,UpFactor);
   }
}

static void firup_process_single(float* y,
                   float* x,
                   float* delay_line,
                   unsigned int* workMemory,
                   unsigned int UpFactor)
{
    /* Polyphase implentation of upsampling and Fir filtering.
    Inputs:
         y: Output 
         x: Input
         delay_line: Buffer to store intermediate states during filtering
         workMemory: firup_init stores struct with input arguments such as the upsampling factor,
                 framesize, and polyphase subfilter coefficients. 
         UpFactor: The Up-Sampling Factor
    */
    int tmp;
    unsigned int i, j, coeffLen, framesize, factorD, circu_left;
    float* xptr;
    float* yptr;
    float* cptr;
    float* optr;
    float* dptr;
    FirUp_t* pFir;

    pFir = (FirUp_t*)workMemory;
    coeffLen = pFir->subFilterLength;
    framesize = pFir->frame_size;
    factorD = pFir->factorD;
    xptr = x;
    yptr = y;
    dptr = delay_line;
    circu_left = (pFir->stateOffset + framesize < coeffLen) ? (framesize) : (coeffLen - pFir->stateOffset);

    float output_backup[framesize*factorD];
    float circu_backup[framesize*factorD];

    for (i = 0; i < pFir->num_of_ch; i ++)
    {
        cptr = pFir->Coeff;
        optr = output_backup;
        memcpy(circu_backup, dptr + pFir->stateOffset, circu_left * sizeof(float));
        if (circu_left != framesize) {
            memcpy(circu_backup + circu_left, dptr, (framesize - circu_left) * sizeof(float));
        }
        for (j = 0; j < factorD - 1; j ++)
        {

#ifdef FIRUP_FOR_HEXAGON
            // Call Fir Generic for each subfilter
            tmp = (framesize | (pFir->stateOffset) << 16);
            qhdsp_fir_af(xptr, dptr, cptr, coeffLen, tmp, optr);
#elif defined FIRUP_FOR_ARM_NEON
            fir_arm_neon(output_backup + j * framesize, xptr, cptr, framesize, coeffLen);
#else
            dly_wr_idx = pFir->stateOffset;
            fir_generic(xptr, dptr, cptr, coeffLen, framesize, optr);
#endif
            // Assign output of subfilter routine to firup output buffer
            for (int fr = 0; fr < framesize; fr ++){
                yptr[j + fr*factorD] = optr[fr];
            }
            cptr += coeffLen;
            optr += framesize;
            memcpy(dptr + pFir->stateOffset, circu_backup, circu_left * sizeof(float));
            if (circu_left != framesize) {
                memcpy(dptr, circu_backup + circu_left, (framesize - circu_left) * sizeof(float));
            }
        }

        // Additional Frame of Processing
#ifdef FIRUP_FOR_HEXAGON
        // Call Fir Generic for each subfilter
        tmp = (framesize | (pFir->stateOffset) << 16);
        qhdsp_fir_af(xptr, dptr, cptr, coeffLen, tmp, optr);
#elif defined FIRUP_FOR_ARM_NEON
        fir_arm_neon(output_backup + j * framesize, xptr, cptr, framesize, coeffLen);
#else
        dly_wr_idx = pFir->stateOffset;
        fir_generic(xptr, dptr, cptr, coeffLen, framesize, optr);
#endif

        for (int fr = 0; fr < framesize; fr ++){
            yptr[j + fr*factorD] = optr[fr];
        }

        xptr += framesize;
        yptr += pFir->outframeisze;
        dptr += coeffLen;
    }
    // Update offset in circular buffer
    pFir->stateOffset = (pFir->stateOffset + framesize < coeffLen) ? (pFir->stateOffset + framesize) : (pFir->stateOffset + framesize - coeffLen);
}


void firup_process_thread(void *arg){
   firup_thread_args_t *firup_args_local = (firup_thread_args_t*) arg;
   do{

#ifdef PTHREAD
      sem_wait(&firup_args_local->semaWork);
#endif

#ifdef QURTTHREAD
      qurt_sem_down(&firup_args_local->semaWork);
#endif
      //int startPacket = HAP_perf_get_packets();
      int tmp;
      unsigned int i, j, coeffLen, framesize, factorD, circu_left;
      float* xptr;
      float* yptr;
      float* cptr;
      float* optr;
      float* dptr;
      FirUp_t* pFir;

      pFir = (FirUp_t*)firup_args_local->workMemory;
      coeffLen = pFir->subFilterLength;
      framesize = pFir->frame_size;
      factorD = pFir->factorD;
      xptr = firup_args_local->x;
      yptr = firup_args_local->y;
      dptr = firup_args_local->delay_line;
      circu_left = (pFir->stateOffset + framesize < coeffLen) ? (framesize) : (coeffLen - pFir->stateOffset);

      float output_backup[framesize*factorD];
      float circu_backup[framesize*factorD];

      for (i = 0; i < pFir->num_of_ch; i ++)
      {
          cptr = pFir->Coeff;
          optr = output_backup;
          memcpy(circu_backup, dptr + pFir->stateOffset, circu_left * sizeof(float));
          if (circu_left != framesize) {
              memcpy(circu_backup + circu_left, dptr, (framesize - circu_left) * sizeof(float));
          }
          for (j = 0; j < factorD - 1; j ++)
          {

  #ifdef FIRUP_FOR_HEXAGON
              // Call Fir Generic for each subfilter
              tmp = (framesize | (pFir->stateOffset) << 16);
              qhdsp_fir_af(xptr, dptr, cptr, coeffLen, tmp, optr);
  #elif defined FIRUP_FOR_ARM_NEON
              fir_arm_neon(output_backup + j * framesize, xptr, cptr, framesize, coeffLen);
  #else
              dly_wr_idx = pFir->stateOffset;
              fir_generic(xptr, dptr, cptr, coeffLen, framesize, optr);
  #endif
              // Assign output of subfilter routine to firup output buffer
              for (int fr = 0; fr < framesize; fr ++){
                  yptr[j + fr*factorD] = optr[fr];
              }
              cptr += coeffLen;
              optr += framesize;
              memcpy(dptr + pFir->stateOffset, circu_backup, circu_left * sizeof(float));
              if (circu_left != framesize) {
                  memcpy(dptr, circu_backup + circu_left, (framesize - circu_left) * sizeof(float));
              }
          }

          // Additional Frame of Processing
  #ifdef FIRUP_FOR_HEXAGON
          // Call Fir Generic for each subfilter
          tmp = (framesize | (pFir->stateOffset) << 16);
          qhdsp_fir_af(xptr, dptr, cptr, coeffLen, tmp, optr);
  #elif defined FIRUP_FOR_ARM_NEON
          fir_arm_neon(output_backup + j * framesize, xptr, cptr, framesize, coeffLen);
  #else
          dly_wr_idx = pFir->stateOffset;
          fir_generic(xptr, dptr, cptr, coeffLen, framesize, optr);
  #endif

          for (int fr = 0; fr < framesize; fr ++){
              yptr[j + fr*factorD] = optr[fr];
          }

          xptr += framesize;
          yptr += pFir->outframeisze;
          dptr += coeffLen;
      }
      // Update offset in circular buffer
      pFir->stateOffset = (pFir->stateOffset + framesize < coeffLen) ? (pFir->stateOffset + framesize) : (pFir->stateOffset + framesize - coeffLen);


#ifdef PTHREAD
      sem_post(&firup_args_local->semaDone);
#endif

#ifdef QURTTHREAD
      qurt_sem_up(&firup_args_local->semaDone);
#endif
      //FARF(HIGH,"FIR parallel packets: %d \n" ,(int)(HAP_perf_get_packets()-startPacket));
   }while(1);
}



static void firup_process_parallel(float* y,
                   float* x,
                   float* delay_line,
                   unsigned int* workMemory,
                   unsigned int UpFactor)
{
   int start = HAP_perf_get_pcycles();

   FirUp_t* pFir = (FirUp_t*)workMemory;

   for(int i=0;i<pFir->numThreadsToRun;i++){
      pFir->thread_arg[i].x=x;
      pFir->thread_arg[i].y=y;
      pFir->thread_arg[i].delay_line=delay_line;
      pFir->thread_arg[i].workMemory=workMemory;
   }

#ifdef PTHREAD
   for(int i=0;i<pFir->numThreadsToRun;i++){
      sem_post(&pFir->thread_arg[i].semaWork);
   }

   for(int i=0;i<pFir->numThreadsToRun;i++){
      sem_wait(&pFir->thread_arg[i].semaDone);
   }
#endif

#ifdef QURTTHREAD

   for(int i=0;i<pFir->numThreadsToRun;i++){
      qurt_sem_up(&pFir->thread_arg[i].semaWork);
   }

   for(int i=0;i<pFir->numThreadsToRun;i++){
      qurt_sem_down(&pFir->thread_arg[i].semaDone);
   }
#endif
   /*

   for(int i=0;i<pFir->numThreadsToRun;i++){
      qurt_thread_create(&tid[i], &attr[i], pFir->funcPtr, (void *) &pFir->thread_arg[i]);
   }

    for(int i=0;i<pFir->numThreadsToRun;i++){
      qurt_thread_join(tid[i], &thread_exit_status);
   }

*/
    FARF(HIGH,"FIR parallel cycles : %d \n" ,(int)HAP_perf_get_pcycles()-start);

}

