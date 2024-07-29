#ifndef CIFFT_H
#define CIFFT_H

#include "rtwtypes.h"
#include <complex.h>
// function prototype for IFFT
void cifft_process(creal32_T*, creal32_T*, creal32_T*, unsigned int, unsigned int, int);

#endif
