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
 *  @file fir_core.c
 *  @brief Connects model FIR code generation with ADI SHARC+ assembly
 *
 *  @ingroup dsp
 *
 */

#include "fir_core.h"

/**
 *  fir_core_init
 *
 *  @brief Initializes data used with ADI SHARC+ assembly
 *
 */
void fir_core_init(unsigned int *scratchMemory,
                   const float *input,
                   const float *coeffs,
                   float *states,
                   const unsigned int *filterLengths,
                   const unsigned int *inputMapping,
                   const unsigned int *filterMapping,
                   unsigned int frameSize,
                   unsigned int numCoeffSet,
                   unsigned int numIn,
                   unsigned int numOut,
                   unsigned int *workMemory)
{
    
    // Grab data objects from work memory
    workMemory_t *ptr = (workMemory_t *) workMemory;
    
    // Initialize the variable size buffer pointer
    char *variableSizeBuffers = ptr->data;

    // Initialize pointer to input pointer array
    ptr->inputPtrArray = (const float **) variableSizeBuffers;
    variableSizeBuffers += numOut * sizeof(float*);
    
    // Initialize pointer to coefficient pointer array
    ptr->coeffPtrArray = (const float **) variableSizeBuffers;
    variableSizeBuffers += numOut * sizeof(float*);
    
    // Initialize pointer to state pointer array
    ptr->statePtrArray = (float **) variableSizeBuffers;
    variableSizeBuffers += numOut * sizeof(float*);
    
    // Initialize pointer to state sharing enabled array
    ptr->stateSharingEnabled = (int *) variableSizeBuffers;
    variableSizeBuffers += numOut * sizeof(int);
    
    // Initialize pointer to tap length array
    ptr->taps = (int *) variableSizeBuffers;
    variableSizeBuffers += numOut * sizeof(int);  

    // Initialize pointer to mapped audio input indices
    ptr->mappedInputInd = (int *) variableSizeBuffers;
    variableSizeBuffers += numOut * sizeof(int);
    
    // Initialize the frame size
    ptr->frameSize = frameSize;

    // Current audio input pointer for optimization and input pointer
    // change detection
    ptr->pAudioInput = input;

    // Now initialize elements in each array    
    const float** inputPtrArray  = ptr->inputPtrArray;
    const float** coeffPtrArray  = ptr->coeffPtrArray;
    float** statePtrArray  = ptr->statePtrArray;
    int* stateSharingEnabled = ptr->stateSharingEnabled;
    int* taps = ptr->taps;
    int* mappedInputInd = ptr->mappedInputInd;
    
    int filterOffsets[numCoeffSet];
    int stateOffsets[numIn];
    int stateLength[numIn];
    int ch;
    int inMap;
    int filterMap;
    int coeffInd;
    int stateInd;
    int mappedInd;
            
    // Compute the filter offset for each input channel 
    // by adding the previous filter length to the previous offset
    filterOffsets[0] = 0; // initialize first offset to 0
    for (coeffInd = 1; coeffInd < numCoeffSet; coeffInd++)
    {
        filterOffsets[coeffInd] = filterOffsets[coeffInd-1] + filterLengths[coeffInd-1];
    }
	 
    // Steps to compute the stateSharingEnabled
    // 0. Initialize all the stateSharingEnabled to zero
    // 1. Fetch the first element from the inputMapping array 	
    // 2. Fetch Next element in the inputMapping
    // 3. check current inputMapping and the next inputMapping element are same 
    //    if they are not same then set the next position of stateSharingEnabled to 0
    //    if they are same, set stateSharingEnabled to 1 
    // 4. repeat the step 2 to 4 till the numOutput channels
    // 5. repeat the step 1 to 5 till the numOutput channels  
    for (ch = 0; ch < numOut; ch++)
    {  
        inMap = inputMapping[ch];
        filterMap = filterMapping[ch];
        mappedInd = inMap * frameSize;
        mappedInputInd[ch] = mappedInd;
        inputPtrArray[ch] = &(input[mappedInd]);
        coeffPtrArray[ch] = &(coeffs[filterOffsets[filterMap]]);
        taps[ch] = filterLengths[filterMap];
        
        for (stateInd = ch+1; stateInd < numOut; stateInd++)
        {
            if (stateSharingEnabled[stateInd] != 1)
            {
                if (inMap != inputMapping[stateInd]) 
                {
                    stateSharingEnabled[stateInd] = 0;
                }
                else 
                {
                     stateSharingEnabled[stateInd] = 1;
                }
            }
        }		 
    }
 
    // Compute the state length for each input  
    for (ch = 0; ch < numOut; ch++)
    {
        if (stateSharingEnabled[ch] != 1)
        {	
            // Fetch the actual channel mapping and corresponding tap 
            // length to find the state length			
            inMap = inputMapping[ch];
            stateLength[inMap] = taps[ch] + frameSize;
        }
    }	  
    
    // Compute the state offset for each input channel
    // by adding the previous state length to the previous offset
    stateOffsets[0] = 0; // initialize first offset to 0     
    for (ch = 1; ch < numIn; ch++)
    {
        stateOffsets[ch] = stateOffsets[ch-1] + stateLength[ch-1] ;     
    }
    
    // Update the state pointer array   
    for (ch = 0; ch < numOut; ch++)
    {
        inMap = inputMapping[ch];
        statePtrArray[ch] = &(states[stateOffsets[inMap]]);
    }
    
    // Finally set flag indicating initialization has completed
    ptr->initComplete = true;  
}
    

/**
 *  fir_core_process
 *
 *  @brief Run-time processing that calls into ADI assembly routine 
 *
 */
void fir_core_process(float *output,
                      const float *input,
                      unsigned int *workMemory,
                      unsigned int numOut)
{
    // Grab data objects from work memory
    workMemory_t *ptr = (workMemory_t *) workMemory;
    
    // Exit if initialization has not completed
    if (ptr->initComplete == false)
    {
        return;
    }

    // Only update mapped input pointer list if audio input pointer changes
    if ( ptr->pAudioInput != input )
    {
        ptr->pAudioInput = input;

        // Update mapped list of audio input pointers
        int mappedInd;
        int *mappedInputInd = ptr->mappedInputInd;
        const float **inputPtrArray = ptr->inputPtrArray;
        for (int ch = 0; ch < numOut; ch++)
        {  
            mappedInd = mappedInputInd[ch];
            inputPtrArray[ch] = &(input[mappedInd]);
        }
    }

    fir_vec_mc(ptr->inputPtrArray,
               output, 
               ptr->coeffPtrArray,
               ptr->statePtrArray,
               ptr->stateSharingEnabled,
               ptr->taps,
               ptr->frameSize,
               numOut);
}
