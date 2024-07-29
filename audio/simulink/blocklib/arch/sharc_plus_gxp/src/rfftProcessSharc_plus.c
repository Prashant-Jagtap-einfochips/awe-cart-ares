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
 *  @file  rfftProcessSharc_plus.c
 *  @brief Connects model FFT code generation with ADI SHARC+ fast 
 *         complex FFT routine (cfftf), using one complex FFT to
 *         compute two real FFTs.
 *
 *  @ingroup dsp
 *
 */

#include <filter.h>
#include <assert.h>
#include "rfft.h"
#include "twiddle.h"

/**
 *  rfft_process
 *
 *  @brief Run-time processing of that calls into FFT wrapper for every 
 *         real input channel pair
 *
 *  @param[out]  output        Output audio spectrum buffer pointer
 *  @param[in]   input         Input audio buffer pointer
 *  @param[in]   data_r        Temporary real FFT input/output buffer pointer
 *  @param[in]   data_i        Temporary imaginary FFT input/output buffer pointer 
 *  @param[in]   temp_r        Temporary real FFT working buffer pointer 
 *  @param[in]   temp_i        Temporary imaginary FFT working buffer pointer
 *  @param[in]   twiddleLoc_r  Real twiddle array pointer 
 *  @param[in]   twiddleLoc_i  Imaginary twiddle array pointer
 *  @param[in]   fftSize       FFT size 
 *  @param[in]   numChannels   Number of channels
 *
 */
void rfft_process(creal32_T *output,  
                  const float *input, 
                  float *data_r,
                  float *data_i,
                  float *temp_r,
                  float *temp_i,
                  unsigned int twiddleLoc_r,
                  unsigned int twiddleLoc_i,
                  unsigned int fftSize, 
                  unsigned int numChannels)
{  
    // Check for even numbered input channels
    assert(numChannels % 2 == 0); 
    
    // Typecast to resolve datatype incompatability 
    complex_float *output_c = (complex_float*) output;
    float* twiddle_r = (float*) twiddleLoc_r;
    float* twiddle_i = (float*) twiddleLoc_i;
    
    int spectrumSize = fftSize/2 + 1;

    for (int ch = 0; ch < numChannels; ch+=2)
    {   
        // Copy input over to working memory 
        for (int i = 0; i < fftSize; i++)
        {
            data_r[i] = input[(ch+0)*fftSize + i];
            data_i[i] = input[(ch+1)*fftSize + i];
        }

        // Perform FFT for each input pair
        cfftf_wrapper(&output_c[(ch+0)*spectrumSize],
                      &output_c[(ch+1)*spectrumSize],
                      data_r,
                      data_i,
                      temp_r, 
                      temp_i,
                      twiddle_r, 
                      twiddle_i,
                      fftSize);
    }   
}


/**
 *  rfft_process_inplace
 *
 *  @brief Run-time in-place processing of that calls into FFT wrapper for 
 *         real 2-channel input 
 *
 *  @param[out]  X1            First output spectrum buffer pointer
 *  @param[out]  X2            Second output spectrum buffer pointer
 *  @param[in]   x1            First input audio buffer pointer
 *  @param[in]   x2            Second input audio buffer pointer
 *  @param[in]   temp_r        Temporary real FFT working buffer pointer 
 *  @param[in]   temp_i        Temporary imaginary FFT working buffer pointer
 *  @param[in]   twiddleLoc_r  Real twiddle array pointer 
 *  @param[in]   twiddleLoc_i  Imaginary twiddle array pointer
 *  @param[in]   fftSize       FFT size 
 *
 */
void rfft_process_inplace(creal32_T *X1,  
                          creal32_T *X2, 
                          float *x1,
                          float *x2,
                          float *temp_r,
                          float *temp_i,
                          unsigned int twiddleLoc_r,
                          unsigned int twiddleLoc_i,
                          unsigned int fftSize)
{
    // Cast creal32_T to complex_float to resolve datatype incompatability 
    complex_float *X1_c = (complex_float*) X1;
    complex_float *X2_c = (complex_float*) X2;
    float* twiddle_r = (float*) twiddleLoc_r;
    float* twiddle_i = (float*) twiddleLoc_i;

    // Perform FFT for the input pair
    cfftf_wrapper(X1_c,
                  X2_c,
                  x1,
                  x2,
                  temp_r, 
                  temp_i,
                  twiddle_r, 
                  twiddle_i,
                  fftSize);
}


/**
 *  cfftf_wrapper
 *
 *  @brief Performs two forward FFTs of real input data and returns two 
 *         complex half spectra through ADI SHARC+ optimized FFT routine.
 *
 *  @param[out]  X1            First FFT result buffer pointer
 *  @param[out]  X2            Second FFT result buffer pointer
 *  @param[in]   data_r        First time-domain input buffer pointer, also 
 *                             real in-place working buffer 
 *  @param[in]   data_i        Second time-domain input buffer pointer, also
 *                             imaginary in-place working buffer 
 *  @param[in]   temp_r        Real scratch buffer pointer
 *  @param[in]   temp_i        Imaginary scratch buffer pointer
 *  @param[in]   twiddle_r     Real twiddle table pointer 
 *  @param[in]   twiddle_i     Imaginary twiddle table pointer 
 *  @param[in]   fftSize       FFT size 
 *
 */
void cfftf_wrapper(complex_float *X1,
                   complex_float *X2,
                   float *data_r,
                   float *data_i,
                   float *temp_r,
                   float *temp_i,
                   float *twiddle_r,
                   float *twiddle_i,
                   unsigned fftSize)
{ 
    float *rp_ptr;
    float *ip_ptr;
    float *rpEnd_ptr;
    float *ipEnd_ptr;
    float rp;
    float ip;
    float rpEnd;
    float ipEnd;
    float rpEven;
    float ipEven;
    float rpOdd;
    float ipOdd; 
    
    // Call into ADI FFT routine
    cfftf(data_r, 
          data_i,
          temp_r, 
          temp_i,
          twiddle_r, 
          twiddle_i,
          fftSize);
    
    /* -------------------------------------------------------------------
    ** For x = x1 + jx2, we have X1[K] = (X[k] + X*[N-k])/2, 
    ** and X2[K] = -j(X[k] - X*[N-k])/2.
    ** Dissect into even and odd symmetric components to output buffers
    ** ---------------------------------------------------------------- */
    
    // These point to the start of the FFT result
    rp_ptr = &(data_r[0]);
    ip_ptr = &(data_i[0]);

    // These point to the end of the FFT result
    rpEnd_ptr = &(data_r[fftSize-1]);
    ipEnd_ptr = &(data_i[fftSize-1]);
    
    // The first samples are purely real
    (*X1).re = *rp_ptr++;
    (*X1++).im = 0;

    (*X2).re = *ip_ptr++;
    (*X2++).im = 0;

    // There are N-2 complex values in the middle
    for (int i = 0; i < ((fftSize/2) - 1); i++)
    {
        // Fetch samples k and N-K
        rp = *rp_ptr++;
        ip = *ip_ptr++;
        rpEnd = *rpEnd_ptr--;
        ipEnd = *ipEnd_ptr--;

        rpEven = (rp + rpEnd) * 0.5f;
        ipEven = (ip - ipEnd) * 0.5f;
        rpOdd = (rp - rpEnd) * 0.5f;
        ipOdd = (ip + ipEnd) * 0.5f;

        (*X1).re = rpEven;
        (*X1++).im = ipEven;
        (*X2).re = ipOdd;
        (*X2++).im = -rpOdd;
    }

    // The last samples are purely real
    (*X1).re = *rp_ptr++;
    (*X1++).im = 0;

    (*X2).re = *ip_ptr++;
    (*X2++).im = 0;
}
