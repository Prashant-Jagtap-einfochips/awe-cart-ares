#ifndef CFFT_H
#define CFFT_H

#include "rtwtypes.h"
#include <complex.h>
// function prototype for FFT
void cfft_process(creal32_T*, creal32_T*, creal32_T*, unsigned int, unsigned int, int);

#endif
