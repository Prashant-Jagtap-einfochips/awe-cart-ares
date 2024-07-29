/*****************************************************************************************
 *
 * BOSE CORPORATION
 * COPYRIGHT 2014 BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:

 *     BOSE CORPORATION
 *     The Mountain
 *     Framingham, MA 01701-9168

*****************************************************************************************/

/**
 *  @file fireven_process_armv7v8.h
 *  @brief Connects model FIR Even code generation 
 *
 *  @ingroup dsp
 *
 */

#ifndef __FIREVEN_PROCESS_ARMV7V8_H__
#define __FIREVEN_PROCESS_ARMV7V8_H__

/***  Include Files ***********************************************************/



/***  External Variable References  *******************************************/
 

/***  Module Macros  **********************************************************/

#define FIREVEN_FOR_ARM


/***  Module Types  ***********************************************************/

// FIR Generic model work memory structure used for processing
typedef struct FirGenericWorkMemory_s
{
    unsigned int frame_size;
    unsigned int numIn;
    unsigned int numOut;
    unsigned int numFilters;

    float* coeffs;
    float* states;
    unsigned int statesSize;

    unsigned int* filterLengths;
    unsigned int* inputMapping;
    unsigned int* filterMapping;

    unsigned int* inputMappingTbl;
    unsigned int* filterOffets;
    unsigned int* stateOffsets;

    unsigned int* workMemory;
    unsigned int workMemorySize;

} FirGenericWorkMemory_t;


/***  Module Function Prototypes  *********************************************/

/**
 *  FirEven_Process()
 *
 *  @brief Run-time processing of FIR Even.
 *
 *  @param[out]    y               Audio Data Out
 *  @param[in]     x               Audio Data In
 *  @param[in]     frameSize       Number of samples per frame
 *  @param[in]     num_channels    Number of input channels
 *  @param[in,out] states          State buffer pointer
 *  @param[in]     filter_length   Pointer to array of filter lengths
 *  @param[in]     stateIndex
 *  @param[in]     activeCoeffSet  
 *  @param[in]     coeffs          Coefficient buffer pointer
 *
 */
extern void FirEven_Process(float y[], float x[], int frame_size, int num_channels, float *newstate, 
                            int filter_length, int *stateIndex, unsigned int *activeCoeffSet, float *coeffs);



void fireven_process_armv7v8_init(unsigned int *scratchMemory, 
                                  unsigned int frame_size, 
                                  unsigned int num_of_ch, 
                                  unsigned int num_of_filters, 
                                  unsigned int filter_length, 
                                  float const* coeffs,
                                  unsigned int* workMemory);

                                  

void fireven_process_armv7v8(float* y, 
                             float* x, 
                             unsigned int *activeCoeffSet, 
                             float *newstate,
                             unsigned int* workMemory);



typedef struct FirEven_s
{
    unsigned int frame_size;
    unsigned int num_of_ch;
    unsigned int numFilters;
    unsigned int filterLength;
    unsigned int state_offset;
    unsigned int rem;
    float*  flippedCoeff;

} FirEven_t;

#endif /* __FIREVEN_PROCESS_ARMV7V8_H__ */
