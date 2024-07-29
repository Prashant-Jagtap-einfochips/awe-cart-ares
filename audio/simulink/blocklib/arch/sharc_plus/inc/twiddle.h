#ifndef TWIDDLE_H
#define TWIDDLE_H
#include <complex.h>

//defines the max FFT size supported
#define MAX_FFTSIZE 256 

// The minimum size of the twiddle table must be FFTSIZE/2
// Here a larger twiddle table is being used to derive the twiddle table of 
// less FFTSIZE by setting twiddle stride = MAX_FFTSIZE/FFTSIZE
extern complex_float pm twiddle[MAX_FFTSIZE/2];

// twiddle array declarations, the definitions are in
// twiddle_arrays_[fftsize].c files
#pragma section("seg_l1_dmda")
extern float dm twiddle256_r[256];
#pragma section("seg_l1_pmda")
extern float pm twiddle256_i[256];

#pragma section("seg_l1_dmda")
extern float dm twiddle128_r[128];
#pragma section("seg_l1_pmda")
extern float pm twiddle128_i[128];

// set twiddle array function declarations
void set_twiddle_256(unsigned int *twiddle_r, unsigned int *twiddle_i);
void set_twiddle_128(unsigned int *twiddle_r, unsigned int *twiddle_i);

#endif
