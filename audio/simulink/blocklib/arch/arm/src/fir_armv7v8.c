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
 *  @file fir_armv7v8.c
 *  @brief Connects model FIR code generation with ARM Neon v7/v8
 *
 *  @ingroup dsp
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <arm_neon.h>
#include "fir_armv7v8.h"

#define ARM_INSTRUCTION_VERSION                   ARMV8

#define INPUT_MAPPING_TBL_OFFSET                  (20)


/***  Module Functions  *******************************************************/
static void fir_neon_floating(float* input, float *coeffs, int filterLength, int length, float *output)
{
    float acc0, acc1, acc2, acc3;      // accumulator for MACs
    float *coeffp;                     // pointer to coefficients
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
            acc[0] = vmlaq_f32(acc[0], c, b);
            b = vld1q_f32(inputp0 + 1);
            acc[1] = vmlaq_f32(acc[1], c, b);
            b = vld1q_f32(inputp0 + 2);
            acc[2] = vmlaq_f32(acc[2], c, b);
            b = vld1q_f32(inputp0 + 3);
            acc[3] = vmlaq_f32(acc[3], c, b);

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

/**
 *  fir_init
 *
 *  @brief Initializes data used for ARMv7v8
 *
 */
void fir_init(unsigned int *scratchMemory,
              const float *input,
              float *coeffs,
              float *states,
              unsigned int *filterLengths,
              unsigned int *inputMapping,
              const unsigned int *filterMapping,
              unsigned int frameSize,
              unsigned int numCoeffSet,
              unsigned int numIn,
              unsigned int numOut,
              unsigned int *workMemory)
{
    int loop, idx, accu, totallen, currIndex, firstChannelIdx;
    int firstCoeffIdx, prevStateBufferLength;

    FirGenericWorkMemory_t *pFirGeneric;
    pFirGeneric = (FirGenericWorkMemory_t*) workMemory;

    // Initialize the FirGeneric Data Structure
    pFirGeneric->frame_size = frameSize;
    pFirGeneric->numIn = numIn;
    pFirGeneric->numOut = numOut;
    pFirGeneric->numFilters = numCoeffSet;
    pFirGeneric->state_offset = 0;
    pFirGeneric->filterLengths = (unsigned int*)filterLengths;
    pFirGeneric->inputMapping = (unsigned int*)inputMapping;
    pFirGeneric->filterMapping = (unsigned int*)filterMapping;
    pFirGeneric->coeffs = (float*)coeffs;
    pFirGeneric->states = states;
    pFirGeneric->workMemory = workMemory;
    pFirGeneric->workMemorySize = sizeof(workMemory);

    // Create Input Mapping Table to count the number of inputs mapped to all output channels
    pFirGeneric->inputMappingTbl = pFirGeneric->workMemory + INPUT_MAPPING_TBL_OFFSET;
    memset(pFirGeneric->inputMappingTbl, 0, numIn * sizeof(unsigned int));
    for (loop = 0; loop < numOut; loop ++) {
        // Here, in Matlab, the index of inputMapping starts with 0.
        ++ pFirGeneric->inputMappingTbl[(pFirGeneric->inputMapping[loop])];
    }

    // Initialize filterOffsets
    pFirGeneric->filterOffsets = pFirGeneric->inputMappingTbl + pFirGeneric->numIn;
    memset(pFirGeneric->filterOffsets, 0x1, pFirGeneric->numFilters);
    // In Matlab the index start with 1, in C, the index is from 0.
    currIndex = 0;
    pFirGeneric->filterOffsets[0] = currIndex;
    for (loop = 1; loop < numCoeffSet; loop ++) {
        currIndex = pFirGeneric->filterLengths[loop - 1] + currIndex;
        pFirGeneric->filterOffsets[loop] = currIndex;
    }

    // Initialize stateOffsets
    pFirGeneric->stateOffsets = pFirGeneric->filterOffsets + pFirGeneric->numFilters;
    memset(pFirGeneric->stateOffsets, 0x0, pFirGeneric->numIn);
    // In Matlab the index start with 1; in C, the index is from 0.
    currIndex = 0;
    pFirGeneric->stateOffsets[0] = currIndex;
    for (loop = 1; loop < numOut; loop ++) {
        currIndex += pFirGeneric->filterLengths[loop - 1] + pFirGeneric->frame_size;
        pFirGeneric->stateOffsets[loop] = currIndex;
    }

}


/**
 *  fir_process
 *
 *  @brief Run-time processing with ARMv7v8 NEON
 *
 */
void fir_process(float *output,
                 const float *input,
                 unsigned int *workMemory,
                 unsigned int numOut)
{
    int loop, inputIdx, coeffIdx, coeffOffset, coeffLength;
    int stateOffset, tmp;
    float* input_f;
    float* output_f = output;
    float* coeff_addr;
    float* state_addr;
    
    // Cast workMemory into FirGenericWorkMemoy to extract parameters
    FirGenericWorkMemory_t *pFirGeneric = (FirGenericWorkMemory_t*) workMemory;
    
    // Loop through each output channel
    for (int loop = 0; loop < pFirGeneric->numOut; loop++){
        
        // Get Input that maps to the output channel
        inputIdx = pFirGeneric->inputMapping[loop];
        input_f = (float*)input + inputIdx * pFirGeneric->frame_size;

        // Get filter coefficients
        coeffIdx = pFirGeneric->filterMapping[loop];
        coeffOffset = pFirGeneric->filterOffsets[coeffIdx];
        coeffLength = pFirGeneric->filterLengths[coeffIdx];

        // Get state pointer
        stateOffset = pFirGeneric->stateOffsets[loop];
        
        // ARM Neon Fir fcn call
        memcpy(pFirGeneric->states + stateOffset + coeffLength - 1, input_f, pFirGeneric->frame_size* sizeof(float));
        fir_neon_floating((pFirGeneric->states + stateOffset), (pFirGeneric->coeffs + coeffOffset), coeffLength, pFirGeneric->frame_size, output_f);
        
        // move output pointer
        output_f += pFirGeneric->frame_size;
    }
}
