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
 *  @file  rfft_armv7v8.h
 *  @brief Connects model FFT code generation with ARMv7v8 NEON
 *
 *  @ingroup dsp
 *
 */

#ifndef __RFFT_PROCESS_ARMV7V8_H__
#define __RFFT_PROCESS_ARMV7V8_H__
#include "NE10_dsp.h"
#include "NE10_macros.h"





/**
 *  rfft_init
 *
 *  @brief Initialization code for FFT. Generates twiddle factors to use in FFT run-time function calls.
 *
 *  @param[in]   fftSize            FFT size, must be power of 2  
 *  @param[out]  workMemory         Memory for FFT configuration
 *
 */
void rfft_init( unsigned int *scratchMemory,
                unsigned int fftSize,
                unsigned int *workMemory);


/**
 *  rfft_process
 *
 *  @brief Run-time processing of that calls into FFT wrapper for every 
 *         real input channel
 *
 *  @param[out]  output             Complex-valued output audio spectrum buffer pointer
 *  @param[in]   input              Real-valued input audio buffer pointer
 *  @param[in]   fftSize            FFT size, must be power of 2  
 *  @param[in]  workMemory          Memory for FFT configuration
 *  @param[in]  numChannels         Number of Input Channels
 *
 */
void rfft_process(ne10_fft_cpx_float32_t *output, 
                  ne10_float32_t *input, 
                  unsigned int fftSize,
                  unsigned int* workMemory,
                  unsigned int numChannels);





/**
 *  rifft_process
 *
 *  @brief Run-time processing of that calls into IFFT wrapper for every 
 *         real input channel
 *
 *  @param[out]  output             Complex-valued input audio buffer pointer
 *  @param[in]   input              Complex-valued output audio spectrum buffer pointer
 *  @param[in]   fftSize            FFT size, must be power of 2  
 *  @param[out]  workMemory         Memory for FFT configuration
 *
 */
void rifft_process(float *output,
                   ne10_fft_cpx_float32_t *input,
                   unsigned int num_ch,
                   unsigned int fftSize,
                   unsigned int winOverlapSize,
                   const float* win,
                   float* lastFrame,
                   unsigned int* workMemory);



#endif /* __RFFT_PROCESS_ARMV7V8_H__ */