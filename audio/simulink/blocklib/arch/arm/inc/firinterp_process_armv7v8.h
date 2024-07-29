/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: firinterp_process_armv7v8.h
 *
 */

#ifndef __FIRINTERP_PROCESS_ARMV7V8_H_
#define __FIRINTERP_PROCESS_ARMV7V8_H_

#include <stdlib.h>
#include <string.h>


#define FIRINTERP_FOR_ARM





void firinterp_process_armv7v8_init(unsigned int *scratchMemory,
                              const float *input,
                              float *coeffs,
                              unsigned int frameSize,
                              unsigned int factorD,
                              unsigned int filterLength,
                              unsigned int num_of_ch,
                              unsigned int *workMemory);





void firinterp_process_armv7v8(float* y, 
                               float* x, 
                               float* delay_line, 
                               unsigned int* workMemory);




// FIR Even model work memory structure used for processing
typedef struct FirInterp_s
{
    unsigned int frame_size;
    unsigned int num_of_ch;
    unsigned int factorD;
    unsigned int filterLength;
    unsigned int stateOffset;
    float* Coeff;

} FirInterp_t;





#endif

