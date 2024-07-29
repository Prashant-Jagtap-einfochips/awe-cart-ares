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
 *  @file pooliir_armv7v8.c
 *  @brief Connects Simulink model IIR code generation with ARMv7v8
 *
 *  @ingroup dsp
 *
 */

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <malloc.h>
#include "pooliir_armv7v8.h"
#include <arm_neon.h>


int iir_neon_floating(const float *in_samples, const float *coefs, float *states, unsigned int num_biquads, unsigned int length, float *out_samples)
{
    unsigned int i, j;
    float y_n, s1_n, s2_n, x_n;
    float b0, b1, a1, b2, a2;
    float *in_samples_ptr;
    float *out_samples_ptr;


    in_samples_ptr = (float *)in_samples;
    for(j = 0; j < num_biquads; j++)
    {
        s1_n = states[0];
        s2_n = states[1];
        b0 = *coefs++;
        b1 = *coefs++;
        a1 = *coefs++;
        b2 = *coefs++;
        a2 = *coefs++;

        out_samples_ptr = out_samples;

        for(i = 0; i < length; i++)
        {
            x_n = *in_samples_ptr++;
            y_n = b0 * x_n + s1_n;             // y[n] = b0 * x[n] + s1[n-1]
            *out_samples_ptr++ = y_n;

            s1_n = s2_n + b1 * x_n - a1 * y_n; // s1[n] = s2[n-1] + b1 * x[n] - a1 * y[n]
            s2_n = b2 * x_n - a2 * y_n;        // s2[n] = b2 * x[n] - a2 * y[n]
        }

        in_samples_ptr = out_samples;

        states[0] = s1_n;
        states[1] = s2_n;
        states += 2;
    }

    return 0;
}

void pooliir_init( float               *reserved,
                   float               *input,
                   unsigned int        frameSize,
                   unsigned int        numChannels,
                   unsigned int        *numStages,
                   float               *coeffs,
                   float               *workMemory,
                   unsigned int        workMemorySize)
{
   Pooliir_t* pooliir;

   pooliir = (Pooliir_t*)workMemory;
   pooliir->frame_size = frameSize;
   pooliir->num_of_ch = numChannels;
   pooliir->num_of_stage = numStages;
   pooliir->flippedCoeff = coeffs;
}   
									   
								   
								   
void pooliir_process(float            *out_samples,
                     float            *in_samples, 
                     float            *workMemory)
{  
   int i, j, num_ch, framesize;
   float* xptr;
   float* yptr;
   float* cptr;
   float* sptr;
	Pooliir_t* pooliir;
   

   pooliir = (Pooliir_t*)workMemory;
   num_ch = pooliir->num_of_ch;
   framesize = pooliir->frame_size;
   xptr = in_samples;
   yptr = out_samples;
   cptr = pooliir->flippedCoeff;
   sptr = workMemory + (sizeof(Pooliir_t) / sizeof(float));
    for (i = 0; i < num_ch; i ++)
    {        
        // Call iir function
        iir_neon_floating((const float*)xptr, (const float*)cptr, sptr, pooliir->num_of_stage[i], framesize, yptr);

        xptr += framesize;
        yptr += framesize;
        cptr += pooliir->num_of_stage[i] * 5;
        sptr += pooliir->num_of_stage[i] * 2;
    }
}