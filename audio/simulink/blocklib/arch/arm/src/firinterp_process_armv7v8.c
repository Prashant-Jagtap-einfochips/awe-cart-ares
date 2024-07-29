/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: firinterp_process_armv7v8.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "firinterp_process_armv7v8.h"
#ifdef FIRINTERP_FOR_ARM
#include <arm_neon.h>
#endif



#define MAX_FRAME_SIZE                                (32)




static void FlipCoeffs(float x_data[], const unsigned int *x_size);




#ifdef FIRINTERP_FOR_ARM
static void firinterp_neon_floating(float* input, float *coeffs, int filterLength, int length, float *output)
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
    //memmove(&input[0], &input[length], (filterLength - 1) * sizeof(float));
}
#endif

#ifdef FIRINTERP_FOR_GENERIC
// This is C Version of FIR for ARM
static void firinterp_generic_for_arm(float* insamp, float *coeffs, int filterLength, int length, float *output)
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
    //memmove(&insamp[0], &insamp[length], (filterLength - 1) * sizeof(float));
}
#endif




void firinterp_process_armv7v8_init(unsigned int *scratchMemory,
                              const float *input,
                              float *coeffs,
                              unsigned int frameSize,
                              unsigned int factorD,
                              unsigned int filterLength,
                              unsigned int num_of_ch,
                              unsigned int *workMemory)
{
    int loop, accu;
    FirInterp_t* pFir;

    pFir = (FirInterp_t*)workMemory;
    pFir->frame_size = frameSize;
    pFir->num_of_ch = num_of_ch;
    pFir->factorD = factorD;
    pFir->filterLength = filterLength;
    pFir->stateOffset = 0;
    pFir->Coeff = (float*)coeffs;

    //Flip coefficient for fir_generic_for_arm and ARM NEON
    accu = 0;
    for (loop = 0; loop < factorD; loop ++) {
        FlipCoeffs(coeffs + accu, &(pFir->filterLength));
        accu += pFir->filterLength;
    }
}




void firinterp_process_armv7v8(float* y, float* x, float* delay_line, unsigned int* workMemory)
{
    unsigned int i, j, outframeSize, delayOffset = 0;
    unsigned int coeffLen, framesize, factorD;
    float output_backup[MAX_FRAME_SIZE];
    float* xptr;
    float* yptr;
    float* cptr;
    FirInterp_t* pFir;


    pFir = (FirInterp_t*)workMemory;
    coeffLen = pFir->filterLength;
    framesize = pFir->frame_size;
    factorD = pFir->factorD;
    outframeSize = framesize * factorD;
    xptr = x;
    yptr = y;

    for (i = 0; i < pFir->num_of_ch; i ++)
    {
        cptr = pFir->Coeff;
        memcpy(delay_line + delayOffset + coeffLen - 1, xptr, framesize * sizeof(float));
        for (j = 0; j < factorD; j ++)
        {
#ifdef FIRINTERP_FOR_ARM
            firinterp_neon_floating(delay_line + delayOffset, cptr, coeffLen, framesize, output_backup + j * framesize);
#endif
#ifdef FIRINTERP_FOR_GENERIC
            firinterp_generic_for_arm(delay_line + delayOffset, cptr, coeffLen, framesize, output_backup + j * framesize);
#endif
            yptr[j + 0] = output_backup[j * framesize + 0];
            yptr[j + 4] = output_backup[j * framesize + 1];
            yptr[j + 8] = output_backup[j * framesize + 2];
            yptr[j + 12] = output_backup[j * framesize + 3];
            yptr[j + 16] = output_backup[j * framesize + 4];
            yptr[j + 20] = output_backup[j * framesize + 5];
            yptr[j + 24] = output_backup[j * framesize + 6];
            yptr[j + 28] = output_backup[j * framesize + 7];
            cptr += coeffLen;
        }
        // shift input samples back in time for next time
        memmove(delay_line + delayOffset, delay_line + delayOffset + framesize, (coeffLen - 1) * sizeof(float));
        xptr += framesize;
        yptr += outframeSize;
        delayOffset += coeffLen + framesize - 1;
    }
}


static void FlipCoeffs(float x_data[], const unsigned int *x_size)
{
    unsigned int i, tmp, md2;
    float xtmp;

    md2 = *x_size >> 1;
    for (i = 0; i < md2; i++)
    {
        xtmp = x_data[i];
        tmp = (*x_size - i) - 1;
        x_data[i] = x_data[tmp];
        x_data[tmp] = xtmp;
    }
}