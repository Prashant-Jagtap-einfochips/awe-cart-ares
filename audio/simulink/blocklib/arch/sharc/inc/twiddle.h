#ifndef TWIDDLE_H
#define TWIDDLE_H
#include <complex.h>

//defines the max FFT size supported
#define MAX_FFTSIZE 256 

// The minimum size of the twiddle table must be FFTSIZE/2
// Here a larger twiddle table is being used to derive the twiddle table of 
// less FFTSIZE by setting twiddle stride = MAX_FFTSIZE/FFTSIZE
extern complex_float pm twiddle[MAX_FFTSIZE/2];
#endif
