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
 *  @file fir_hexagon.c
 *  @brief Connects model FIR code generation with Qualcomm Hexagon Library (QHL)
 *
 *  @ingroup dsp
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fir_hexagon.h"
//#include "qhdsp.h"




#define FIR_FOR_HEXAGON
#define MAX_FRAME_SIZE                            (32)
#define INPUT_MAPPING_TBL_OFFSET                  (120)




#define L2FETCH_CONFIG    0x0100FF00    // initial L2FETCH config [stride = 256 : width = 255 : height = 0]


typedef struct Multihread_Context_s {
    FirGenericWorkMemory_t* pp;
    unsigned int offset;
    unsigned int start_l;
    unsigned int num_l;
} Multihread_Context_t;


/***  Local Function Declarations  ********************************************/






/***  Module Functions  *******************************************************/
static void FlipCoeffs(float x_data[], const unsigned int *x_size);
void FirGeneric_ChildTask(void *pArgs);
void flt_bkfir(float *xin, float *coefs, int taps, int length, float *yout);




/**
 *  fir_process_hexagon_init
 *
 *  @brief Initializes data used with with Qualcomm Hexagon Library (QHL)
 *
 */
void fir_init(unsigned int *scratchMemory,
              const float *input,
              float *coeffs,
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
    unsigned int coeff_address, state_address;
    int loop, currIndex;
    FirGenericWorkMemory_t* pFirGeneric;


    pFirGeneric = (FirGenericWorkMemory_t*)workMemory;
    // Initialize the FirGeneric Data Structure
    pFirGeneric->frame_size = frameSize;
    pFirGeneric->numIn = numIn;
    pFirGeneric->numOut = numOut;
    pFirGeneric->numFilters = numCoeffSet;
    pFirGeneric->state_offset = 0;
    pFirGeneric->filterLengths = (unsigned int*)filterLengths;
    pFirGeneric->inputMapping = (unsigned int*)inputMapping;
    pFirGeneric->filterMapping = (unsigned int*)filterMapping;
    pFirGeneric->scratchMemory = workMemory;
    pFirGeneric->workMemorySize = sizeof(workMemory);
    pFirGeneric->input_save = NULL;
    pFirGeneric->output_save = NULL;
    // Deal with 8-byte alignment for coefficients
    coeff_address = (unsigned int)coeffs;
    if (coeff_address % 8) {
        memcpy(coeffs, coeffs + 4, filterLengths[0] * numOut * sizeof(float));
        pFirGeneric->coeffs = coeffs + 1;
    }
    else {
        pFirGeneric->coeffs = coeffs;
    }
    // Deal with 8-byte alignment for state buffer
    state_address = (unsigned int)states;
    if (state_address % 8) {
        pFirGeneric->states = states + 1;
    }
    else {
        pFirGeneric->states = states;
    }

    // Initialize filterOffsets
    pFirGeneric->filterOffets = workMemory + INPUT_MAPPING_TBL_OFFSET;
    memset(pFirGeneric->filterOffets, 0x1, pFirGeneric->numFilters);
    // In Matlab the index start with 1, in C, the index is from 0.
    currIndex = 0;
    pFirGeneric->filterOffets[0] = currIndex;
    for (loop = 1; loop < numCoeffSet; loop ++) {
        currIndex = pFirGeneric->filterLengths[loop - 1] + currIndex;
        pFirGeneric->filterOffets[loop] = currIndex;
    }

    // Initialize stateOffsets
    pFirGeneric->stateOffsets = pFirGeneric->filterOffets + pFirGeneric->numFilters;
    memset(pFirGeneric->stateOffsets, 0x0, pFirGeneric->numIn);
    // In Matlab the index start with 1; in C, the index is from 0.
    currIndex = 0;
    pFirGeneric->stateOffsets[0] = currIndex;
    for (loop = 1; loop < numOut; loop ++) {
        currIndex += pFirGeneric->filterLengths[pFirGeneric->filterMapping[loop-1]] + pFirGeneric->frame_size;
        pFirGeneric->stateOffsets[loop] = currIndex;
    }
#if 1
    // Coefficient flipped
    int accu = 0;
    for (loop = 0; loop < pFirGeneric->numFilters; loop ++) {
        FlipCoeffs(coeffs + accu, &(pFirGeneric->filterLengths[loop]));
        accu += pFirGeneric->filterLengths[loop];
    }
#endif


#if (MULTITHREAD_ENABLE == 1)
    int retCode;
    char attrN[32];
    unsigned int num_ch, re_ch;
    Multihread_Context_t mContext[3];

    re_ch = numOut % 4;
    num_ch = numOut / 4;
    mContext[0].pp = pFirGeneric;
    mContext[0].offset = 0;
    mContext[0].start_l = num_ch * 1 + re_ch;
    mContext[0].num_l = num_ch;
    mContext[1].pp = pFirGeneric;
    mContext[1].offset = 1;
    mContext[1].start_l = num_ch * 2 + re_ch;
    mContext[1].num_l = num_ch;
    mContext[2].pp = pFirGeneric;
    mContext[2].offset = 2;
    mContext[2].start_l = num_ch * 3 + re_ch;
    mContext[2].num_l = num_ch;
    pFirGeneric->num_of_loop = num_ch + re_ch;

    // Multithread
    memset(attrN, 0, sizeof(attrN));
    sprintf(attrN, "FirG_%x", (unsigned int)(pFirGeneric->stack[0]));
	qurt_thread_attr_init(&pFirGeneric->attr[0]);
	qurt_thread_attr_set_name(&pFirGeneric->attr[0], attrN);
	qurt_thread_attr_set_stack_addr(&pFirGeneric->attr[0], pFirGeneric->stack[0]);
	qurt_thread_attr_set_stack_size(&pFirGeneric->attr[0], FIR_GENERIC_STACK_SIZE);
	/*   thread priority must be set above QURT_THREAD_ATTR_PRIORITY_DEFAULT */
	qurt_thread_attr_set_priority(&pFirGeneric->attr[0], QURT_THREAD_ATTR_PRIORITY_DEFAULT/2);
    memset(attrN, 0, sizeof(attrN));
    sprintf(attrN, "FirG_%x", (unsigned int)(pFirGeneric->stack[1]));
    qurt_thread_attr_init(&pFirGeneric->attr[1]);
	qurt_thread_attr_set_name(&pFirGeneric->attr[1], attrN);
	qurt_thread_attr_set_stack_addr(&pFirGeneric->attr[1], pFirGeneric->stack[1]);
	qurt_thread_attr_set_stack_size(&pFirGeneric->attr[1], FIR_GENERIC_STACK_SIZE);
	/*   thread priority must be set above QURT_THREAD_ATTR_PRIORITY_DEFAULT */
	qurt_thread_attr_set_priority(&pFirGeneric->attr[1], QURT_THREAD_ATTR_PRIORITY_DEFAULT/2);
    memset(attrN, 0, sizeof(attrN));
    sprintf(attrN, "FirG_%x", (unsigned int)(pFirGeneric->stack[2]));
    qurt_thread_attr_init(&pFirGeneric->attr[2]);
	qurt_thread_attr_set_name(&pFirGeneric->attr[2], attrN);
	qurt_thread_attr_set_stack_addr(&pFirGeneric->attr[2], pFirGeneric->stack[2]);
	qurt_thread_attr_set_stack_size(&pFirGeneric->attr[2], FIR_GENERIC_STACK_SIZE);
	/*   thread priority must be set above QURT_THREAD_ATTR_PRIORITY_DEFAULT */
	qurt_thread_attr_set_priority(&pFirGeneric->attr[2], QURT_THREAD_ATTR_PRIORITY_DEFAULT/2);

	qurt_sem_init(&pFirGeneric->sem[0]);
	qurt_sem_down(&pFirGeneric->sem[0]);
    qurt_sem_init(&pFirGeneric->sem[1]);
	qurt_sem_down(&pFirGeneric->sem[1]);
    qurt_sem_init(&pFirGeneric->sem[2]);
	qurt_sem_down(&pFirGeneric->sem[2]);
    qurt_sem_init(&pFirGeneric->sem[3]);
	qurt_sem_down(&pFirGeneric->sem[3]);
    qurt_sem_init(&pFirGeneric->sem[4]);
	qurt_sem_down(&pFirGeneric->sem[4]);
    qurt_sem_init(&pFirGeneric->sem[5]);
	qurt_sem_down(&pFirGeneric->sem[5]);
    qurt_sem_init(&pFirGeneric->sem[6]);
	qurt_sem_down(&pFirGeneric->sem[6]);
    qurt_sem_init(&pFirGeneric->sem[7]);
	qurt_sem_down(&pFirGeneric->sem[7]);

	retCode = qurt_thread_create(&pFirGeneric->tid[0], &pFirGeneric->attr[0], FirGeneric_ChildTask, (void *)(&mContext[0]));
    retCode = qurt_thread_create(&pFirGeneric->tid[1], &pFirGeneric->attr[1], FirGeneric_ChildTask, (void *)(&mContext[1]));
    retCode = qurt_thread_create(&pFirGeneric->tid[2], &pFirGeneric->attr[2], FirGeneric_ChildTask, (void *)(&mContext[2]));
#endif
}


/**
 *  fir_process_hexagon
 *
 *  @brief Run-time processing that calls into with Qualcomm Hexagon Library (QHL)
 *
 */
void fir_process(float *output,
                 const float *input,
                 unsigned int *workMemory,
                 unsigned int numOut)
{
    int loop, inputIdx, coeffIdx, coeffOffset, coeffLength, framesize, loopCnt;
    int stateOffset, tmp;
    float* input_f;
    float* output_f;
    FirGenericWorkMemory_t* pFirGeneric = (FirGenericWorkMemory_t*)workMemory;
    float __attribute__ ((__aligned__(8))) output_b[MAX_FRAME_SIZE];

#if (MULTITHREAD_ENABLE == 1)
    pFirGeneric->input_save = input;
    pFirGeneric->output_save = output;
    loopCnt = pFirGeneric->num_of_loop;
    qurt_sem_add(&pFirGeneric->sem[0], 1);
    qurt_sem_add(&pFirGeneric->sem[1], 1);
    qurt_sem_add(&pFirGeneric->sem[2], 1);
#else
    loopCnt = pFirGeneric->numOut;
#endif
    framesize = pFirGeneric->frame_size;
    tmp = (framesize * numOut) >> 5;
    tmp = (tmp > 1) ? (tmp) : (1);
    Q6_l2fetch_AR((void *)input, L2FETCH_CONFIG + tmp);
    output_f = output;
    loop = 0;
    do {
        inputIdx = pFirGeneric->inputMapping[loop];
        input_f = (float*)input + inputIdx * framesize;

        // Get filter coefficients
        coeffIdx = pFirGeneric->filterMapping[loop];
        coeffOffset = pFirGeneric->filterOffets[coeffIdx];
        coeffLength = pFirGeneric->filterLengths[coeffIdx];

        // Get state pointer
        stateOffset = pFirGeneric->stateOffsets[loop];
#if 0
        // L2 prefetch next filter 
        if(loop < loopCnt-1){
            int next_filt_idx = pFirGeneric->inputMapping[loop+1];
            int nxt_coeffIdx = pFirGeneric->filterMapping[loop+1];
            int nxt_coeffOffset = pFirGeneric->filterOffets[nxt_coeffIdx];
            float* nxt_coeff_addr = pFirGeneric->coeffs + nxt_coeffOffset;
            float* nxt_state_addr = pFirGeneric->states + pFirGeneric->stateOffsets[loop+1];
            int nxt_filt_cache_lines = pFirGeneric->filterLengths[next_filt_idx] >> 4;
            
            Q6_l2fetch_AR(nxt_coeff_addr, 0xFF+(128U<<16)+(128U<<8)+nxt_filt_cache_lines);
            Q6_l2fetch_AR(nxt_state_addr, 0xFF+(128U<<16)+(128U<<8)+nxt_filt_cache_lines);
        }
#endif        
        // L1 fetch current filter
        float* coeff_addr = pFirGeneric->coeffs + coeffOffset;
        float* state_addr = pFirGeneric->states + stateOffset;
#if 0
        for(int m=0; m<=(coeffLength>>4)+1; m++){
            Q6_dcfetch_A(coeff_addr+(16*m));
            Q6_dcfetch_A(state_addr+(16*m));
        }
#endif
        // L2 prefetch current filter 
        //tmp = (coeffLength > 64) ? (coeffLength >> 6) : (1);
        //Q6_l2fetch_AR(pFirGeneric->states + stateOffset, (L2FETCH_CONFIG + tmp));
        //Q6_dcfetch_A(pFirGeneric->states);
        
        memcpy(pFirGeneric->states + stateOffset + coeffLength - 1, input_f, framesize * sizeof(float));
        flt_bkfir(state_addr, coeff_addr, coeffLength, framesize, output_b); 
        //flt_bkfir(pFirGeneric->states + stateOffset, (pFirGeneric->coeffs + coeffOffset), coeffLength, framesize, output_b);
        memmove((pFirGeneric->states + stateOffset), (pFirGeneric->states + stateOffset + framesize), (coeffLength - 1) * sizeof(float));
        memcpy(output_f, output_b, framesize * sizeof(float));

        output_f += framesize;
    } while (++ loop < loopCnt);

#if (MULTITHREAD_ENABLE == 1)
    // Wait for all FIR Children processing done
    qurt_sem_down(&pFirGeneric->sem[4]);
    qurt_sem_down(&pFirGeneric->sem[5]);
    qurt_sem_down(&pFirGeneric->sem[6]);
#endif
}





static void FlipCoeffs(float x_data[], const unsigned int *x_size)
{
    unsigned int i, tmp, md2;
    float xtmp;

    md2 = *x_size >> 1;
    for (i = 0; i < md2; i++)
    {
        xtmp = x_data[i];
        tmp = (*x_size - i) - 1;
        x_data[i] = x_data[tmp];
        x_data[tmp] = xtmp;
    }
}

#if (MULTITHREAD_ENABLE == 1)
void FirGeneric_ChildTask(void *pArgs)
{
    // Get arguments
    int start_l, num_l;
    int loop, inputIdx, coeffIdx, coeffOffset, coeffLength, framesize;
    int stateOffset;
    float* input_f;
    float* output_f;
    Multihread_Context_t* pContext = (Multihread_Context_t*)pArgs;
    FirGenericWorkMemory_t* pFirGeneric = (pContext->pp);
    float __attribute__ ((__aligned__(8))) output_b[MAX_FRAME_SIZE];
    unsigned int sem_num = pContext->offset;
    qurt_thread_t t_id = qurt_thread_get_id();
    unsigned int stepIndex = qurt_thread_get_priority(t_id);


    start_l = pContext->start_l;
    num_l = pContext->num_l;
    while (1)
    {
        /* Wait to be triggered */
        qurt_sem_down(&pFirGeneric->sem[sem_num]);
	    /* Perform FIR */
	    //printf("FIR Generic Child Task running.... %d, %d\r\n", sem_num, stepIndex);

        framesize = pFirGeneric->frame_size;
        output_f = pFirGeneric->output_save + start_l * framesize;
        loop = start_l;
        do {
            inputIdx = pFirGeneric->inputMapping[loop];
            input_f = (float*)(pFirGeneric->input_save) + inputIdx * framesize;

            // Get filter coefficients
            coeffIdx = pFirGeneric->filterMapping[loop];
            coeffOffset = pFirGeneric->filterOffets[coeffIdx];
            coeffLength = pFirGeneric->filterLengths[coeffIdx];
            // Get state pointer
            stateOffset = pFirGeneric->stateOffsets[loop];
            
            memcpy(pFirGeneric->states + stateOffset + coeffLength - 1, input_f, framesize * sizeof(float));
            flt_bkfir(pFirGeneric->states + stateOffset, pFirGeneric->coeffs + coeffOffset, coeffLength, framesize, output_b);
            memmove((pFirGeneric->states + stateOffset), (pFirGeneric->states + stateOffset + framesize), (coeffLength - 1) * sizeof(float));
            memcpy(output_f, output_b, framesize * sizeof(float));

            output_f += framesize;
        } while (++ loop < start_l + num_l);

        /* Notify Main FIR processing */
        qurt_sem_add(&pFirGeneric->sem[sem_num + 4], 1);
    }
}
#endif  // MULTITHREAD_ENABLE == 1