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
 *  @file  rifft.h
 *  @brief Connects model IFFT, window and overlap-add code generation 
 *         with ADI SHARC+ fast complex IFFT routine (ifftf), using one 
 *         complex IFFT to compute two real-signal IFFTs. 
 *
 *  @ingroup dsp
 *
 */

#ifndef __RIFFT_H__
#define __RIFFT_H__

#include <complex.h>
#include "rtwtypes.h"

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
 *  @param[in]   twiddleLoc_r   Real twiddle array pointer 
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
                   unsigned int numOverlap);


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
 *  @note 
 *  1. The inputs X1 and X2 are both half spectra, and contain fftSize/2+1 
 *     complex words each. The complex data is stored in interleaved form.
 *     The function generates two real output sequences of length fftSize.
 * 
 *  2. See BDSP rfft_dual for the original implementation. This version
 *     has no option to specify input buffer and only allows in-place 
 *     processing. 
 *
 *  3. To operate in SIMD mode, ADI recommends that all real bufers are 
 *     allocated separately from the imaginary ones, and aligned on an 
 *     address boundary that is a multiple of the FFT size.
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
                   unsigned fftSize);
        
#endif /* __RIFFT_H__ */
