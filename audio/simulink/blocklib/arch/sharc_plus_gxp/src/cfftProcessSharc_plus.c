/*****************************************************************************
*Function: IFFT - CRL wrapper
*PROTOTYPE: cfft_process(complex_float* output,        - Output buffer
*                        complex_float* input ,        - Input buffer
*                        complex_float* temp,          - Temporary working buffer
*                        unsigned int   fftSize,       - FFT size
*                        unsigned int   numChannels,   - number of output channels 
*                        int            twiddleStride  - A larger twiddle table is being to used to derive 
*                       );                               the twiddle table of less FFT sizes by setting twiddle 
                                                         stride appropriately
*                                                        i.e twiddleStride = MAX_FFTSIZE/fftSize;

*****************************************************************************/
#include <filter.h>
#include "cfft.h"
#include "twiddle.h"

// Function definition for FFT CRL wrapper
void cfft_process(creal32_T output[], creal32_T input[], creal32_T temp[], unsigned int fftSize, unsigned int numChannels, int twiddleStride)
{
   int i;
   
   //cast the creal32_T datatype to complex_float to resolve the datetype incompatability
   complex_float *input_c = (complex_float*) input;
   complex_float *output_c = (complex_float*) output;
   complex_float *temp_c = (complex_float*) temp;
   
   for(i=0; i<numChannels; i++)
   {	   
      cfft(&input_c[i*fftSize],
           &temp_c[i*fftSize],
           &output_c[i*fftSize],
           twiddle,
           twiddleStride,
           fftSize);
   }
}
