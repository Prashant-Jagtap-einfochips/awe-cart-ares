/*
https://github.com/dawbarton/rtc/tree/master/matlab

The MIT License (MIT)

Copyright (c) 2015 David A.W. Barton (david.barton@bristol.ac.uk)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "mex.h"

/* CRC table */
static unsigned int crc_table[256];
static int initialised = 0;

/* Initial value for the CRC */
#define CRC32_INITIAL 0xFFFFFFFF

/* ************************************************************************ */
void crc32_setup(void)
{
	/* This CRC implementation is the same as is used for PNG images */
	unsigned int c;
	unsigned int n, k;

	for (n = 0; n < 256; n++) {
		c = n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xEDB88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
}

/* ************************************************************************ */
void crc32_update(unsigned int *crc, unsigned int len, unsigned char *data)
{
	unsigned int i;
	for (i = 0; i < len; i++) {
		*crc = crc_table[(*crc ^ data[i]) & 0xFF] ^ (*crc >> 8);
	}
}

/* ************************************************************************ */
void crc32_final(unsigned int *crc)
{
	*crc ^= 0xFFFFFFFF;
}


/* The main mex function */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
    unsigned int crc = CRC32_INITIAL;
    
    /* Check inputs/outputs */
    if (nrhs != 1) 
        mexErrMsgIdAndTxt("crc32:crc32", "Expected one input to crc32 (the data)");
    if (nlhs > 1)
        mexErrMsgIdAndTxt("crc32:crc32", "Expected one output for crc32 (the checksum)");
    if (!mxIsUint8(prhs[0]))
        mexErrMsgIdAndTxt("crc32:crc32", "Expected the input data to be a uint8 array");
    
    /* Make sure the CRC table is initialised */
    if (!initialised) {
        crc32_setup();
        initialised = 1;
    }
    
    /* Calculate the checksum */
    crc32_update(&crc, (unsigned int)mxGetNumberOfElements(prhs[0]), (unsigned char *)mxGetData(prhs[0]));
    crc32_final(&crc);
    
    /* Output the checksum */
    plhs[0] = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS, mxREAL);
    *(unsigned int *)mxGetData(plhs[0]) = crc;
}