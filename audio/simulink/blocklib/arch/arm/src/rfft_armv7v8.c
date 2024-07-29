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
 *  @file rfft_armv7v8.c
 *  @brief Connects Simulink model IIR code generation with ARMv7v8
 *  @ingroup dsp
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arm_neon.h>
#include "rfft_armv7v8.h"





void rfft_init( unsigned int *scratchMemory,
                unsigned int fftSize,
                unsigned int *workMemory)
{
    ne10_fft_r2c_cfg_float32_t fftcfg;

    fftcfg = ne10_fft_alloc_r2c_float32_for_crl(fftSize, (ne10_fft_r2c_cfg_float32_t)workMemory);
}


void rfft_process(ne10_fft_cpx_float32_t *output, 
                  ne10_float32_t *input, 
                  unsigned int fftSize,
                  unsigned int* workMemory,
                  unsigned int numChannels)
{
    
    ne10_fft_r2c_cfg_float32_t fftcfg;
    fftcfg = (ne10_fft_r2c_cfg_float32_t)workMemory;
    ne10_float32_t* EachInput;
    ne10_fft_cpx_float32_t* EachOutput;

    for (int ch = 0; ch < numChannels; ch ++){
        EachInput = input + fftSize*ch;
        EachOutput = output + (fftSize/2 +1)*ch;
        ne10_fft_r2c_1d_float32_neon(EachOutput, EachInput, fftcfg);
    }
}


void rifft_process(    float *output,
                       ne10_fft_cpx_float32_t *input,
                       unsigned int num_ch,
                       unsigned int fftSize,
                       unsigned int OverlapSize,
                       const float* win,
                       float* lastFrame,
                       unsigned int* workMemory)
{
    int ch, i, halfPt = (fftSize >> 1) + 1;
    float *each_output, *each_lf;
    float tmpBuf[fftSize];
    ne10_fft_r2c_cfg_float32_t fftcfg;
    ne10_fft_cpx_float32_t *each_input;
    ne10_fft_cpx_float32_t fullData[fftSize];


    fftcfg = (ne10_fft_r2c_cfg_float32_t)workMemory;
    for (ch = 0; ch < num_ch; ch ++) {
        each_input = input + ch * halfPt;
        each_output = output + ch * (fftSize >> 1);
        each_lf = lastFrame + ch * (fftSize >> 1);
        // Conjugation
        memcpy(&fullData[0], each_input, halfPt * sizeof(ne10_fft_cpx_float32_t));
        for (i = 0; i < (halfPt - 2); i ++ ) {
            fullData[halfPt + i].r = each_input[halfPt - 1 - i].r;
            fullData[halfPt + i].i = -each_input[halfPt - 1 - i].i;
        }
        // IFFT Complex to Real
        ne10_fft_c2r_1d_float32_neon(tmpBuf, fullData, fftcfg);
        // Windowing
        float32x4_t nw, nl, nt, vlo, vll;
        for (i = 0; i < OverlapSize; i += 4) {
            nw = vld1q_f32(win + i);
            nl = vld1q_f32(each_lf + i);
            nt = vld1q_f32(tmpBuf + i);
            vlo = vmlaq_f32(nl, nt, nw);
            nt = vld1q_f32(tmpBuf + i + OverlapSize);
            nw = vld1q_f32(win + i + OverlapSize);
            vll = vmulq_f32(nt, nw);
            vst1q_f32((each_output + i), vlo);
            vst1q_f32((each_lf + i), vll);
        }
    }
}