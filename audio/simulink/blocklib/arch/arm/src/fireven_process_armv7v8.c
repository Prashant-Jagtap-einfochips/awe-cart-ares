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
 *  @file fireven_core.c
 *  @brief Connects model FIR code generation with hardware accelerator manager
 *
 *  @note Optimization settings for speed should include setting compiler
 *        flags -Ov100 and -ipa (if possible) for the top level build. NDEBUG 
 *        should also be defined to turn off asserts. Also, it is important
 *        to note that this module was NOT designed to support the usage of 
 *        cached data.
 * 
 *  @ingroup dsp
 *
 */

/***  Include Files ***********************************************************/
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "fireven_process_armv7v8.h"
#ifdef FIREVEN_FOR_ARM
#include <arm_neon.h>
#endif

/***  Debug Support  **********************************************************/

// FIR accelerator debugging support. Note that for profiling and production
// use, DEBUG_FIR_ACCEL should be set to zero, ADI_DEBUG should NOT be 
// defined for the EMDMA drivers, and compiler optimization flags should
// be SET as stated above. 


#define MAX_FRAME_SIZE                                (32)





/***  External Variable References  *******************************************/


/***  Module Macros  **********************************************************/
//#define COEFF_LENGTH_LARGER_THAN_FRAME_SIZE


/***  Module Types  ***********************************************************/



/***  Module Variables  *******************************************************/


/***  Local Function Declarations  ********************************************/



/***  Module Functions  *******************************************************/

#ifdef FIREVEN_FOR_ARM
static void fireven_neon_floating(float* input, float *coeffs, int filterLength, int length, float *output)
{
    float acc0, acc1, acc2, acc3;     // accumulator for MACs
    float *coeffp;                    // pointer to coefficients
    float *inputp0;                    // pointer to input samples
    int n, k, coeff_dec, coeff_rem;
 
    float32x4_t c, b, tmp;
    float32x2_t tsum1, tsum2;
    float32x4_t acc[4] = {0.0, 0.0, 0.0, 0.0};

    coeff_dec = filterLength >> 2;
    coeff_rem = filterLength & 0x3;
    // apply the filter to each input sample
    for (n = 0; n < length; n += 4) {
        // calculate output n
        coeffp = coeffs;
        inputp0 = input + n;
        acc0 = acc1 = acc2 = acc3 = 0;
        acc[0] = vdupq_n_f32(0.0f);
        acc[1] = vdupq_n_f32(0.0f);
        acc[2] = vdupq_n_f32(0.0f);
        acc[3] = vdupq_n_f32(0.0f);
        for (k = 0; k < coeff_dec; k ++) {
            c = vld1q_f32(coeffp);
            b = vld1q_f32(inputp0);
            tmp = vmulq_f32(c, b);
            acc[0] = vaddq_f32(tmp, acc[0]);

            b = vld1q_f32(inputp0 + 1);
            tmp = vmulq_f32(c, b);
            acc[1] = vaddq_f32(tmp, acc[1]);

            b = vld1q_f32(inputp0 + 2);
            tmp = vmulq_f32(c, b);
            acc[2] = vaddq_f32(tmp, acc[2]);

            b = vld1q_f32(inputp0 + 3);
            tmp = vmulq_f32(c, b);
            acc[3] = vaddq_f32(tmp, acc[3]);

            coeffp += 4;
            inputp0 += 4;
        }
#if (ARM_INSTRUCTION_VERSION == ARMV7)
        tsum1 = vadd_f32(vget_low_f32(acc[0]), vget_high_f32(acc[0]));
        tsum2 = vpadd_f32(tsum1, tsum1);
        acc0 = vget_lane_f32(tsum2, 0);
        tsum1 = vadd_f32(vget_low_f32(acc[1]), vget_high_f32(acc[1]));
        tsum2 = vpadd_f32(tsum1, tsum1);
        acc1 = vget_lane_f32(tsum2, 0);
        tsum1 = vadd_f32(vget_low_f32(acc[2]), vget_high_f32(acc[2]));
        tsum2 = vpadd_f32(tsum1, tsum1);
        acc2 = vget_lane_f32(tsum2, 0);
        tsum1 = vadd_f32(vget_low_f32(acc[3]), vget_high_f32(acc[3]));
        tsum2 = vpadd_f32(tsum1, tsum1);
        acc3 = vget_lane_f32(tsum2, 0);
#else
        acc0 = vaddvq_f32(acc[0]);
        acc1 = vaddvq_f32(acc[1]);
        acc2 = vaddvq_f32(acc[2]);
        acc3 = vaddvq_f32(acc[3]);
#endif
        for (k = 0; k < coeff_rem; k ++) {
            acc0 += coeffp[k] * inputp0[k];
            acc1 += coeffp[k] * inputp0[k + 1];
            acc2 += coeffp[k] * inputp0[k + 2];
            acc3 += coeffp[k] * inputp0[k + 3];
        }

        output[n] = acc0;
        output[n + 1] = acc1;
        output[n + 2] = acc2;
        output[n + 3] = acc3;
    }
    // shift input samples back in time for next time
    memmove(&input[0], &input[length], (filterLength - 1) * sizeof(float));
}
#endif


#ifdef FIREVEN_FOR_GENERIC
// This is C Version of FIR for ARM
static void fireven_generic_for_arm(float* insamp, float *coeffs, int filterLength, int length, float *output)
{
    float acc0;                       // accumulator for MACs
    float *coeffp;                    // pointer to coefficients
    float *inputp;                    // pointer to input samples
    int n, k;
 
    // apply the filter to each input sample
    for (n = 0; n < length; n ++) {
        // calculate output n
        coeffp = coeffs;
        inputp = insamp + n;
        acc0 = 0;
        for (k = 0; k < filterLength; k ++) {
            acc0 += (*coeffp++) * (*inputp++);
        }

        output[n] = acc0;
    }
    // shift input samples back in time for next time
    memmove(&insamp[0], &insamp[length], (filterLength - 1) * sizeof(float));
}
#endif



void fireven_process_armv7v8_init(unsigned int *scratchMemory, 
                                  unsigned int frame_size, 
                                  unsigned int num_of_ch, 
                                  unsigned int num_of_filters, 
                                  unsigned int filter_length, 
                                  float const* coeffs,
                                  unsigned int* workMemory)
{
    int j, k, coeff_size, xtmp_tmp, b_tmp;
    float xtmp;
    FirEven_t* pFir;

    coeff_size = num_of_filters * filter_length;
    pFir = (FirEven_t*)workMemory;
    pFir->frame_size = frame_size;
    pFir->num_of_ch = num_of_ch;
    pFir->numFilters = num_of_filters;
    pFir->filterLength = filter_length;
    pFir->flippedCoeff = (float*)coeffs;
    pFir->state_offset = 0;
    pFir->rem = frame_size % filter_length;
    // Reverse Coefficients
    // No need to flip coeffs for fireven_generic_for_arm() and ARM NEON
    //for (j = 0; j < num_of_filters; j++) {
    //  for (k = 0; k < (filter_length >> 1); k ++) {
    //    xtmp_tmp = filter_length * j + k;
    //    xtmp = pFir->flippedCoeff[xtmp_tmp];
    //    b_tmp = (filter_length * j - k) + (filter_length - 1);
    //    pFir->flippedCoeff[xtmp_tmp] = pFir->flippedCoeff[b_tmp];
    //    pFir->flippedCoeff[b_tmp] = xtmp;
    //  }
    //}
}

void fireven_process_armv7v8(float* y, 
                             float* x, 
                             unsigned int *activeCoeffSet, 
                             float *newstate,
                             unsigned int* workMemory)
{
  int i, filterLen, framesize, tmp;
  float __attribute__ ((__aligned__(8))) output_b[MAX_FRAME_SIZE];
  float* coof;
  float* xptr;
  float* yptr;
  float* sptr;
  FirEven_t* pFir;

  pFir = (FirEven_t*)workMemory;
  framesize = pFir->frame_size;
  filterLen = pFir->filterLength;
  xptr = x;
  yptr = y;
  sptr = &newstate[0];
  i = 0;
  do {
    coof = pFir->flippedCoeff + activeCoeffSet[i] * filterLen;
    memcpy(sptr + filterLen - 1, xptr, framesize * sizeof(float));
#ifdef FIREVEN_FOR_GENERIC
    fireven_generic_for_arm(sptr, coof, filterLen, framesize, yptr);
#endif
#ifdef FIREVEN_FOR_ARM
    fireven_neon_floating(sptr, coof, filterLen, framesize, yptr);
#endif
    xptr += framesize;
    yptr += framesize;
    sptr += filterLen + framesize - 1;
  } while (++ i < pFir->num_of_ch);
}