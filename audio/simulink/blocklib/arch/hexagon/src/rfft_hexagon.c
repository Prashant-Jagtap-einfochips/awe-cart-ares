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
 *  @file  rfft_hexagon.c
 *  @brief Connects model FFT and IFFT code generation with Qualcomm Hexagon Library (QHL)
 *         supplied qhdsp_acf_r1dfft_af() and qhdsp_acf_r1difft_af() function, which computes
 *         the FFT and IFFT of an input.
 *
 *  @ingroup dsp
 *
 */

#if __hexagon__
#include <stdlib.h>
#include "qhcomplex.h"
#else
#include <math.h>
#include <complex.h>
#include <malloc.h>
#endif

#include "rtwtypes.h"
#include "qhdsp.h"
#include "rfft_hexagon.h"
#include "blkutil.h"


/**
 *  rfft_init
 *
 *  @brief Initialization code for FFT. Generates twiddle factors to use in FFT run-time function calls.
 *
 *  @param[in]   Twiddle1           Twiddle factors for N/2-point complex FFT, must be aligned by 8 bytes
 *  @param[in]   Twiddle2           Twiddle factors fot last stage, must be aligned by 8 bytes
 *  @param[in]   fftSize            FFT size, must be power of 2 
 *
 */

void rfft_init( creal32_T *Twiddle1,
                creal32_T *Twiddle2,
                const int fftSize)
{
    qhdsp_fft_gen_twiddles_real_acf((float complex *) Twiddle1,
                                    (float complex *) Twiddle2, 
                                    fftSize, 
                                    qhmath_log2_f(fftSize));
}

/**
 *  rfft_process
 *
 *  @brief Run-time processing of that calls into FFT wrapper for every 
 *         real input channel
 *
 *  @param[out]  output             Complex-valued output audio spectrum buffer pointer
 *  @param[in]   input              Real-valued input audio buffer pointer, must be aligned by 8*fftSize bytes
 *  @param[in]   Twiddle1           Twiddle factors for N/2-point complex FFT, must be aligned by 8 bytes
 *  @param[in]   Twiddle2           Twiddle factors fot last stage, must be aligned by 8 bytes
 *  @param[in]   RfftOutputTmp      Working buffer for RFFT calculation, must be aligned by 8 bytes
 *  @param[in]   fftSize            FFT size, must be power of 2 
 *  @param[in]   numChannels        Number of channels
 *
 */
void rfft_process(creal32_T *output, 
                  const float *input, 
                  const creal32_T *Twiddle1,
                  const creal32_T *Twiddle2,
                  creal32_T *RfftOutputTmp,
                  const int fftSize,
                  const int numChannels) 
{   
    // Initialize constants and pointers
    const int OutputChannelSize = (fftSize/2) + 1;
    const float *ChannelInput;
    creal32_T *ChannelOutput;
    
    // Loop through each channel
    for (int ch = 0; ch < numChannels; ch ++){

        // Set channel pointers
        ChannelInput = &input[ch*fftSize];
        ChannelOutput = &output[ch*OutputChannelSize];

        // Call QHL RFFT function
        qhdsp_acf_r1dfft_af(ChannelInput,
                            fftSize, 
                            (float complex*) Twiddle1, 
                            (float complex*) Twiddle2, 
                            (float complex*) RfftOutputTmp);

        // Post process output
        for (int i = 0; i < OutputChannelSize; i ++){
            ChannelOutput[i].re = (float) RfftOutputTmp[i].re;
            ChannelOutput[i].im = (float) RfftOutputTmp[i].im;
        }
    }
}

/**
 *  rifft_process
 *
 *  @brief Run-time processing for IFFT
 *
 *  @param[out]  output             Real-valued output audio buffer pointer, must be aligned by 8 bytes
 *  @param[in]   input              Complex-valued input audio spectrum buffer
 *  @param[in]   Twiddle1           Twiddle factors for N/2-point complex FFT, must be aligned by 8 bytes
 *  @param[in]   Twiddle2           Twiddle factors fot last stage, must be aligned by 8 bytes
 *  @param[in]   RifftOutputTmp     Complex-valued working buffer for RIFFT calculation, must be aligned by 8 bytes
 *  @param[in]   window             Real-valued window buffer  
 *  @param[in]   outputOverlap      Buffer pointer to hold overlapping samples between frames
 *  @param[in]   fftSize            FFT size, must be power of 2 
 *  @param[in]   numChannels        Number of audio channels on input and output side
 *  @param[in]   numOverlap         Number of overlapping samples per channel between frames
 *
 */
void rifft_process( float *output, 
                    const creal32_T *input,
                    const creal32_T *Twiddle1,
                    const creal32_T *Twiddle2,
                    creal32_T *RifftOutputTmp,
                    const float *window,
                    float *outputOverlap,
                    const int fftSize,
                    const int numChannels,
                    const int numOverlap) 
{   
    // Initialize constants
    const int half_spectrum_size = fftSize/2 + 1;
    const int outputSize = fftSize - numOverlap;

    // Initialize buffer for full spectrum input
    __attribute((aligned(8))) float complex input_full_spectrum[fftSize];
    float complex *input_c = (float complex*) input;  

    // Initialize pointers
    float *overlapSource;
    float complex *RifftOutputTmpComplexFloat = (float complex*) RifftOutputTmp;
    float *RifftOutputTmpFloat;

    // For each channel
    for (int ch = 0; ch < numChannels; ch ++ ){

        ////////////////// Prepare full spectrum input //////////////////
        
        // Set first and middle values
        input_full_spectrum[0] = input_c[(ch*half_spectrum_size) + 0 ];
        input_full_spectrum[fftSize/2 ] = input_c[(ch* half_spectrum_size ) + fftSize/2 ];
        
        // Set all other values
        for (int idx = 1; idx < fftSize/2; idx ++ ){
            input_full_spectrum[idx] = input_c[(ch*half_spectrum_size) + idx ];
            input_full_spectrum[fftSize-idx] = qhcomplex_conj_f(input_full_spectrum[idx]);
        }

        //////////////////     Call IFFT Function     //////////////////
        qhdsp_af_r1difft_acf(   input_full_spectrum,
                                fftSize, 
                                (float complex*) Twiddle1, 
                                (float complex*) Twiddle2, 
                                RifftOutputTmpComplexFloat);
        // Recast output buffer from complex to real
        RifftOutputTmpFloat = (float*) RifftOutputTmpComplexFloat;

        //////////////////       Apply Windowing      //////////////////
        blkprod(RifftOutputTmpFloat, window, RifftOutputTmpFloat, fftSize);

        //////////////////  Add prev output overlap   ////////////////// 
        overlapSource = &(outputOverlap[ch*numOverlap]);
        blkadd(RifftOutputTmpFloat, overlapSource, RifftOutputTmpFloat, numOverlap);

        ////////////////// Write to the output buffer //////////////////  
        blkcopy(RifftOutputTmpFloat, &output[ch*outputSize], outputSize);

        //////////  Save 2nd half of FFT to overlap buffer  ////////////  
        blkcopy(&RifftOutputTmpFloat[fftSize - numOverlap], overlapSource, numOverlap );

    }
}
