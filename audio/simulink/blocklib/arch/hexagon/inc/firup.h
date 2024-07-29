/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: firup_hexagon.h
 *
 */

#define FIRUP_FOR_HEXAGON

#include <stdlib.h>
#include <string.h>


void firup_init(  unsigned int *scratchMemory,
                  float *coeffs,
                  unsigned int frameSize,
                  unsigned int factorD,
                  unsigned int filterLength,
                  unsigned int num_of_ch,
                  unsigned int *workMemory);

void firup_process(float* y,
                   float* x,
                   float* delay_line,
                   unsigned int* workMemory,
                   unsigned int UpFactor);

typedef struct FirUp_t
{
    unsigned int frame_size;
    unsigned int num_of_ch;
    unsigned int factorD;
    unsigned int subFilterLength;
    unsigned int stateOffset;
    unsigned int outframeisze;
    float* Coeff;

} FirUp_t;



