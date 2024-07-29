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
 *  @file  rfft_hexagon.h
 *  @brief Connects model FFT code generation with Qualcomm Hexagon Library (QHL)
 *         supplied qhdsp_acf_r1dfft_af() function, which utilizes one 1-D complex 
 *         FFT to compute two real FFTs.
 *
 *  @ingroup dsp
 *
 */

#ifndef __RFFT_HEXAGON_H__
#define __RFFT_HEXAGON_H__


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
                const int fftSize);

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
                  const int numChannels);
						  
/**
 *  rifft_process
 *
 *  @brief Run-time processing for IFFT
 *
 *  @param[out]  output             Real-valued output audio buffer pointer, must be aligned by 8 bytes
 *  @param[in]   input              Complex-valued input audio spectrum buffer, must be aligned by 8 bytes
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
                    const int numOverlap);

#endif /* __RFFT_HEXAGON_H__ */