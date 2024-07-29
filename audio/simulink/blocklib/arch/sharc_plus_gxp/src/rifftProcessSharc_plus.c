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
 *  @file  rifftProcessSharc_plus.c
 *  @brief Connects model IFFT, window and overlap-add code generation 
 *         with ADI SHARC+ fast complex IFFT routine (ifftf), using one 
 *         complex IFFT to compute two real-signal IFFTs. 
 *
 *  @ingroup dsp
 *
 */

#include <filter.h>
#include <assert.h>
#include "rifft.h"
#include "twiddle.h"
#include "blkutil.h"

/**
 *  rifft_process
 *
 *  @brief Run-time processing of that calls into IFFT wrapper for every 
 *         input channel pair, then performs windowing and overlap-add.
 *
 *  @param[out]  output         Output audio buffer pointer
 *  @param[in]   input          Input audio spectrum buffer pointer
 *  @param[in]   data_r         Temporary real FFT input/output buffer pointer
 *  @param[in]   data_i         Temporary imaginary FFT input/output buffer pointer 
 *  @param[in]   temp_r         Temporary real FFT working buffer pointer 
 *  @param[in]   temp_i         Temporary imaginary FFT working buffer pointer
 *  @param[in]   window         Windowing fuction 
 *  @param[in]   outputOverlap  Overlapping buffer pointer
    @param[in]   twiddleLoc_r   Real twiddle array pointer 
 *  @param[in]   twiddleLoc_i   Imaginary twiddle array pointer 
 *  @param[in]   fftSize        FFT size 
 *  @param[in]   numChannels    Number of channels
 *  @param[in]   numOverlap     Number of overlapping samples between frames
 *
 */
void rifft_process(float *output, 
                   creal32_T *input, 
                   float *data_r,
                   float *data_i,
                   float *temp_r,
                   float *temp_i,
                   const float *window,
                   float *outputOverlap,
                   unsigned int twiddleLoc_r,
                   unsigned int twiddleLoc_i,
                   unsigned int fftSize, 
                   unsigned int numChannels, 
                   unsigned int numOverlap)
{   
    // Check for even numbered input channels
    assert(numChannels % 2 == 0); 
    
    // Cast creal32_T to complex_float to resolve datatype incompatability 
    complex_float *input_c = (complex_float*) input;
    float* twiddle_r = (float*) twiddleLoc_r;
    float* twiddle_i = (float*) twiddleLoc_i;
    
    float *overlap_l;
    float *overlap_r;
    float *output_l;
    float *output_r;
    float *fft_real;
    float *fft_imag;
    int inputSize;
    int outputSize;

    fft_real=&(data_r[0]);
    fft_imag=&(data_i[0]);
    inputSize = fftSize/2 + 1;
    outputSize = fftSize - numOverlap;

    for (int ch = 0; ch < numChannels; ch+=2)
    {       
        // IFFT
        ifftf_wrapper(&input_c[(ch+0)*inputSize],
                      &input_c[(ch+1)*inputSize],
                      fft_real,
                      fft_imag,
                      temp_r, 
                      temp_i,
                      twiddle_r, 
                      twiddle_i,
                      fftSize);
        
        // Windowing
        blkprod(fft_real, window, fft_real, fftSize);
        blkprod(fft_imag, window, fft_imag, fftSize);

        // Add previous output overlap with the first half of the buffer 
        overlap_l = &(outputOverlap[(ch+0)*numOverlap]);
        overlap_r = &(outputOverlap[(ch+1)*numOverlap]);

        // Write it to the output buffer
        output_l = &(output[(ch+0)*outputSize]);
        output_r = &(output[(ch+1)*outputSize]);
        
        blkadd(overlap_l, fft_real, output_l, numOverlap);
        blkadd(overlap_r, fft_imag, output_r, numOverlap);

        // Copy the second half of the FFT output buffer into the previous 
        // overlap buffers
        blkcopy(fft_real + numOverlap, overlap_l, numOverlap);
        blkcopy(fft_imag + numOverlap, overlap_r, numOverlap);   
    }
}


/**
 *  ifftf_wrapper
 *
 *  @brief Performs two inverse FFTs of complex half spectra and returns 
 *         real output data through ADI SHARC+ optimized IFFT routine.
 *
 *  @param[in]   X1            First half spectrum input buffer pointer
 *  @param[in]   X2            Second half spectrum input buffer pointer
 *  @param[out]  data_r        First time-domain output buffer pointer, also 
 *                             real in-place working buffer 
 *  @param[out]  data_i        Second time-domain output buffer pointer, also
 *                             imaginary in-place working buffer 
 *  @param[in]   temp_r        Real scratch buffer pointer
 *  @param[in]   temp_i        Imaginary scratch buffer pointer
 *  @param[in]   twiddle_r     Real twiddle table pointer 
 *  @param[in]   twiddle_i     Imaginary twiddle table pointer 
 *  @param[in]   fftSize       FFT size 
 *
 */
void ifftf_wrapper(complex_float *X1,
                   complex_float *X2,
                   float *data_r,
                   float *data_i,
                   float *temp_r,
                   float *temp_i,
                   float *twiddle_r,
                   float *twiddle_i,
                   unsigned fftSize)
{
    int i;
    float *fft_real;
    float *fft_imag;
    float rp1;
    float ip1;
    float rp2;
    float ip2;

    /* -------------------------------------------------------------------
    ** For x = x1 + jx2, we have X1[K] = (X[k] + X*[N-k])/2, 
    ** and X2[K] = -j(X[k] - X*[N-k])/2.
    ** Therefore Re{X} = Re{X1} - Im{X2}, and Im{X} = Im{X1} + Re{X2}
    ** Deinterleave the complex inputs before doing inverse transform
    ** ---------------------------------------------------------------- */

    // Update the first half N+1 samples
    fft_real = &(data_r[0]);
    fft_imag = &(data_i[0]);

    for (i = 0; i < ((fftSize/2) + 1); i++)
    {
        rp1 = (*X1).re;
        ip1 = (*X1++).im;
        rp2 = (*X2).re;
        ip2 = (*X2++).im;

        *fft_real++ = (rp1 - ip2);
        *fft_imag++ = (ip1 + rp2);
    }

    // At this point, X1 and X2 point to sample [fftSize/2 + 1]
    // Decrement this so they point to sample [fftSize/2 - 1] 
    X1 -= 2;
    X2 -= 2;

    // Update the middle N-2 samples
    for (i = 0; i < ((fftSize/2) - 1); i++)
    {
        rp1 = (*X1).re;
        ip1 = (*X1--).im;
        rp2 = (*X2).re;
        ip2 = (*X2--).im;

        *fft_real++ = (rp1 + ip2);
        *fft_imag++ = (rp2 - ip1);
    }

    // Call into ADI IFFT routine
    ifftf(data_r,
          data_i,
          temp_r,
          temp_i,
          twiddle_r,
          twiddle_i,
          fftSize);   
}
