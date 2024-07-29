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
 *  @file  rfft.h
 *  @brief Connects model FFT code generation with ADI SHARC+ fast 
 *         complex FFT routine (cfftf), using one complex FFT to
 *         compute two real FFTs.
 *
 *  @ingroup dsp
 *
 */

#ifndef __RFFT_H__
#define __RFFT_H__

#include <complex.h>
#include "rtwtypes.h"

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
                  unsigned int numChannels);


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
 *  @note The two input channels have to be allocated in separate memory blocks
 *        to operate in SIMD mode, and these buffers will be overwritten with 
 *        intermediate FFT results during processing.
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
                          unsigned int fftSize);


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
 *  @note 
 *  1. The inputs each contain fftSize values. The outputs X1 and X2 contain
 *     interleaved complex values. A total of fftSize/2+1 complex values will 
 *     be written to each output buffer.
 *
 *  2. See BDSP rfft_dual for the original implementation. This version
 *     has no option to specify input buffer and only allows in-place 
 *     processing. If input is to be preserved, it must first be copied 
 *     to a safe location before calling this function.
 *
 *  3. To operate in SIMD mode, ADI recommends that all real bufers are 
 *     allocated separately from the imaginary ones, and aligned on an 
 *     address boundary that is a multiple of the FFT size.
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
                   unsigned fftSize);

#endif /* __RFFT_H__ */
