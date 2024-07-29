/*
Copyright 2011 Bose Corporation
Contact: Chris Cheng
$Revision: 14285 $ $Date: 2016-07-19 16:39:52 -0400 (Tue, 19 Jul 2016) $
*/

#define S_FUNCTION_NAME  SampleRateConverter_sfunc
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include <math.h>


/*=======================*
 * S-function parameters *
 *=======================*/

// Number of block parameters
#define NUM_PARAMS                    4

// Input parameters
#define UP_FACTOR      ((uint_T) mxGetPr(ssGetSFcnParam(S, 0))[0])
#define DOWN_FACTOR    ((uint_T) mxGetPr(ssGetSFcnParam(S, 1))[0])
#define NFRAME_OUT     ((uint_T) mxGetPr(ssGetSFcnParam(S, 2))[0])
#define NFILTERBLOCK   ((uint_T) mxGetPr(ssGetSFcnParam(S, 3))[0])

// Number of work vectors
#define NUM_IWORK                    1
#define NUM_PWORK                    10

/*====================*
 * S-function methods *
 *====================*/

/* mdlInitializeSizes */
static void mdlInitializeSizes(SimStruct *S)
{
    int k;
    //float upRatio; // Number of output frames per input frame
    
    /* Set number of parameters, and compare with block parameters */
    ssSetNumSFcnParams(S, NUM_PARAMS);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) return;
    
    /* Parameters are not tunable */
    for (k = 0; k < NUM_PARAMS; k++)
    {
        ssSetSFcnParamNotTunable(S, k);
    }
    
    // Check filter
    if (NFILTERBLOCK < 1) {
        ssSetErrorStatus(S, "Sample rate conversion filter must not be empty.");
        return;
    }        
    
    /* Number of sample times */
    ssSetNumSampleTimes(S, -1);

    /*
    /* Specify port dimensions/type
     */
    
    // Specify the number of ports
    if (!ssSetNumInputPorts(S, 3)) return;
    if (!ssSetNumOutputPorts(S, 1)) return;

    // Input audio
    ssSetInputPortMatrixDimensions(S, 0, DYNAMICALLY_SIZED, DYNAMICALLY_SIZED);
    ssSetInputPortFrameData(S, 0, 1);
    ssSetInputPortDataType(S, 0, SS_SINGLE);
    ssSetInputPortSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetInputPortOffsetTime(S, 0, 0.0);
    ssSetInputPortRequiredContiguous(S, 0, 1);
    
    // Filter coefficients
    ssSetInputPortMatrixDimensions(S, 1, DYNAMICALLY_SIZED, DYNAMICALLY_SIZED);
    ssSetInputPortFrameData(S, 1, 0);
    ssSetInputPortDataType(S, 1, SS_SINGLE);
    ssSetInputPortSampleTime(S, 1, INHERITED_SAMPLE_TIME);
    ssSetInputPortOffsetTime(S, 1, 0.0);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortRequiredContiguous(S, 1, 1);
    
    // Advance samples array
    ssSetInputPortMatrixDimensions(S, 2, DYNAMICALLY_SIZED, DYNAMICALLY_SIZED);
    ssSetInputPortFrameData(S, 2, 0);
    ssSetInputPortDataType(S, 2, SS_INT32);
    ssSetInputPortSampleTime(S, 2, INHERITED_SAMPLE_TIME);
    ssSetInputPortOffsetTime(S, 2, 0.0);
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    ssSetInputPortRequiredContiguous(S, 2, 1);
    
    // Output audio
    ssSetOutputPortMatrixDimensions(S, 0, DYNAMICALLY_SIZED, DYNAMICALLY_SIZED);
    ssSetOutputPortFrameData(S, 0, 1);
    ssSetOutputPortDataType(S, 0, SS_SINGLE);
    ssSetOutputPortSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOutputPortOffsetTime(S, 0, 0.0);

    /* Number of continuous and discrete states */
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 1);

    /* Number of modes */
    ssSetNumModes(S, 0);
    
    /* Number of non-sampled zero-crossings */
    ssSetNumNonsampledZCs(S, 0);
    
    /* Options (advanced stuff) */
    ssSetOptions(S, SS_OPTION_ALLOW_CONSTANT_PORT_SAMPLE_TIME);
}

#define MDL_SET_WORK_WIDTHS
void mdlSetWorkWidths(SimStruct *S)
{
    int nBuffer, numChan, nFrameIn, nFrameOut;
	/*
     * Set up miscellaneous
     */
	nFrameIn = (((int*)ssGetInputPortDimensions(S, 0))[0]);
	numChan = (((int*)ssGetInputPortDimensions(S, 0))[1]);
	if (NFRAME_OUT == -1)	nFrameOut = nFrameIn;
	else					nFrameOut = NFRAME_OUT;

    // Number of output frames per input frame
    //upRatio = (float)(UP_FACTOR * NFRAME_IN) / (float)(DOWN_FACTOR * NFRAME_OUT);
    
    // Calculate the number of input samples needed to produce output per input frame
    //nBuffer = (NFRAME_OUT * DOWN_FACTOR * (int)ceil(upRatio) - DOWN_FACTOR + NFILTERBLOCK * UP_FACTOR) / UP_FACTOR;
	nBuffer = 2 * ceil((nFrameOut) * DOWN_FACTOR / UP_FACTOR) + NFILTERBLOCK;
	
    // Quantize to input frame size
    nBuffer = (int)(ceil((float)nBuffer / (float)(nFrameIn))) ;	//Number of frames
	if (nBuffer<2) nBuffer = 2;										//Min num of frames = 2
	nBuffer = nBuffer * nFrameIn;

    /* Work vectors */
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, NUM_IWORK);
    ssSetNumPWork(S, NUM_PWORK);

    ssSetNumDWork(S, 5);
	ssSetDWorkWidth(S, 0, numChan * sizeof(float*));
	ssSetDWorkName(S, 0, "pInputBuffer");
	ssSetDWorkDataType(S, 0, SS_INT8);
	ssSetDWorkWidth(S, 1, numChan * sizeof(float*));
	ssSetDWorkName(S, 1, "pInputBufferEnd");
	ssSetDWorkDataType(S, 1, SS_INT8);
	ssSetDWorkWidth(S, 2, numChan * sizeof(float*));
	ssSetDWorkName(S, 2, "pInputWrite");
	ssSetDWorkDataType(S, 2, SS_INT8);
	ssSetDWorkWidth(S, 3, numChan * sizeof(float*));
	ssSetDWorkName(S, 3, "pInputRead");
	ssSetDWorkDataType(S, 3, SS_INT8);
	ssSetDWorkWidth(S, 4, nBuffer * numChan);
	ssSetDWorkName(S, 4, "InputBuffer");
	ssSetDWorkDataType(S, 4, SS_SINGLE);
}

#if defined(MATLAB_MEX_FILE)

//added for R2010B support
#define MDL_INITIALIZE_CONDITIONS
static void mdlInitializeConditions(SimStruct *S)
{
}

#define MDL_INITIALIZE_SAMPLE_TIMES
static void mdlInitializeSampleTimes(SimStruct *S)
{
    // Port-based sample-times so no need to specify anything here
    ssSetModelReferenceSampleTimeDisallowInheritance(S);
}

#define MDL_SET_INPUT_PORT_SAMPLE_TIME
static void mdlSetInputPortSampleTime(SimStruct *S, int_T port, real_T sampleTime, real_T offsetTime)
{
	int nFrameIn, nFrameOut;
    if (port == 0) {
		nFrameIn = (((int*)ssGetInputPortDimensions(S, 0))[0]);
		if (NFRAME_OUT == -1)	nFrameOut = nFrameIn;
		else					nFrameOut = NFRAME_OUT;
        ssSetInputPortSampleTime(S, port, sampleTime);
        ssSetInputPortOffsetTime(S, port, offsetTime);
        ssSetOutputPortSampleTime(S, 0, (double)sampleTime * ((double)(DOWN_FACTOR * nFrameOut) / (double)(UP_FACTOR * nFrameIn)));
        ssSetOutputPortOffsetTime(S, 0, offsetTime);
    } else {
        ssSetInputPortSampleTime(S, port, sampleTime);
        ssSetInputPortOffsetTime(S, port, offsetTime);
    }
}

#define MDL_SET_OUTPUT_PORT_SAMPLE_TIME
static void mdlSetOutputPortSampleTime(SimStruct *S, int_T port, real_T sampleTime, real_T offsetTime)
{
	int nFrameIn, nFrameOut;
	nFrameIn = (((int*)ssGetInputPortDimensions(S, 0))[0]);
	if (NFRAME_OUT == -1)	nFrameOut = nFrameIn;
	else					nFrameOut = NFRAME_OUT;
    ssSetOutputPortSampleTime(S, port, sampleTime);
    ssSetOutputPortOffsetTime(S, port, offsetTime);
    ssSetInputPortSampleTime(S, 0, (double)sampleTime * ((double)(UP_FACTOR * nFrameIn) / (double)(DOWN_FACTOR * nFrameOut)));
    ssSetInputPortOffsetTime(S, 0, offsetTime);
}

#define MDL_SET_INPUT_PORT_DIMENSION_INFO
static void mdlSetInputPortDimensionInfo(SimStruct *S, int_T port, const DimsInfo_T *dimsInfo)
{
    int_T  *uDims   = dimsInfo->dims;
    int myDims[2];
    DECL_AND_INIT_DIMSINFO(myDimsInfo);

    // Set input port dimensions
    if (!ssSetInputPortDimensionInfo(S, port, dimsInfo)) return;
 
    if (port == 0) {
        /*if (uDims[0] != NFRAME_IN) {
            ssSetErrorStatus(S, "Sample rate conversion filter input frame size does not match block specifications.");
        }*/
		int nFrameOut;
		if (NFRAME_OUT == -1)	nFrameOut = (((int*)ssGetInputPortDimensions(S, 0))[0]);
		else					nFrameOut = NFRAME_OUT;
        
        // Set output port dimensions
        myDimsInfo.dims = myDims;
        myDimsInfo.numDims = 2;
        myDimsInfo.width = nFrameOut * uDims[1];
        myDims[0] = nFrameOut;
        myDims[1] = uDims[1];
        if (!ssSetOutputPortDimensionInfo(S, 0, &myDimsInfo)) return;
    }
}


# define MDL_SET_OUTPUT_PORT_DIMENSION_INFO
static void mdlSetOutputPortDimensionInfo(SimStruct        *S, 
                                          int_T            port,
                                          const DimsInfo_T *dimsInfo)
{
    int_T  *uDims   = dimsInfo->dims;
    int myDims[2];
	int nFrameIn, nFrameOut;
    DECL_AND_INIT_DIMSINFO(myDimsInfo);

	nFrameIn = (((int*)ssGetInputPortDimensions(S, 0))[0]);
	if (NFRAME_OUT == -1)	nFrameOut = nFrameIn;
	else					nFrameOut = NFRAME_OUT;

    // Set output port dimensions
    if (!ssSetOutputPortDimensionInfo(S, port, dimsInfo)) return;

    if (uDims[0] != nFrameOut) {
        ssSetErrorStatus(S, "Sample rate conversion filter output frame size does not match block specifications.");
    }
    
    // Set input port dimensions
    myDimsInfo.dims = myDims;
    myDimsInfo.numDims = 2;
    myDimsInfo.width = nFrameIn * uDims[1];
    myDims[0] = nFrameIn;
    myDims[1] = uDims[1];
    if (!ssSetInputPortDimensionInfo(S, 0, &myDimsInfo)) return;
}


# define MDL_SET_DEFAULT_PORT_DIMENSION_INFO
static void mdlSetDefaultPortDimensionInfo(SimStruct *S)
{
    if (ssGetInputPortWidth(S, 0) == DYNAMICALLY_SIZED) {
        if (!ssSetInputPortMatrixDimensions(S, 0, 1, 1)) return;
    }
    if (ssGetInputPortWidth(S, 1) == DYNAMICALLY_SIZED) {
        if (!ssSetInputPortMatrixDimensions(S, 1, 1, 1)) return;
    }
    if (ssGetInputPortWidth(S, 2) == DYNAMICALLY_SIZED) {
        if (!ssSetInputPortMatrixDimensions(S, 2, 1, 1)) return;
    }
    if (ssGetOutputPortWidth(S, 0) == DYNAMICALLY_SIZED) {
        if (!ssSetOutputPortMatrixDimensions(S, 0, 1, 1)) return;
    }
}

#endif //#if defined(MATLAB_MEX_FILE)

#define MDL_START
void mdlStart(SimStruct *S)
{
    // Temporary values/pointers
    int idx;
    
    // Pointer to filter coefficient blocks
    float *pFilter, *pFilterEnd, *pFilterCurr;
    int *pAdvance, *pAdvanceEnd, *pAdvanceCurr;
    
    int nBuffer, numChan;

    // Pointer to input circular buffers and read/write pointers
    float **pInputBuffer, **pInputBufferEnd, **pInputWrite, **pInputRead; 

	numChan = ssGetDWorkWidth(S, 0)/sizeof(float*);
	nBuffer = ssGetDWorkWidth(S, 4)/numChan;

    /*
     * Save filter and advance sample pointers
     */
    
    pFilter = (float*)ssGetInputPortSignal(S, 1);
    pFilterEnd = pFilter + ssGetInputPortWidth(S, 1);
    pFilterCurr = pFilter;
    pAdvance = (int*)ssGetInputPortSignal(S, 2);
    pAdvanceEnd = pAdvance + ssGetInputPortWidth(S, 2);
    pAdvanceCurr = pAdvance;
    
    /*
     * Allocate input buffers and pointers
     */
    
    // Allocate the circular buffers along with end/read/write pointers
	pInputBuffer = (float**)ssGetDWork(S, 0);
	pInputBufferEnd = (float**)ssGetDWork(S, 1);
	pInputWrite = (float**)ssGetDWork(S, 2);
	pInputRead = (float**)ssGetDWork(S, 3);
    for (idx = 0; idx < numChan; idx++) {
        // Allocate twice what is needed ("double buffer")
		pInputBuffer[idx] = (float*)ssGetDWork(S, 4) + (idx * nBuffer);
        pInputBufferEnd[idx] = pInputBuffer[idx] + nBuffer;
        pInputWrite[idx] = pInputBuffer[idx] ;
        pInputRead[idx] = pInputBuffer[idx] + (nBuffer>>1) - 1;
    }
    ssSetPWorkValue(S, 0, pInputBuffer);
    ssSetPWorkValue(S, 1, pInputBufferEnd);
    ssSetPWorkValue(S, 2, pInputWrite);
    ssSetPWorkValue(S, 3, pInputRead);
    ssSetPWorkValue(S, 4, pFilter);
    ssSetPWorkValue(S, 5, pFilterEnd);
    ssSetPWorkValue(S, 6, pFilterCurr);
    ssSetPWorkValue(S, 7, pAdvance);
    ssSetPWorkValue(S, 8, pAdvanceEnd);
    ssSetPWorkValue(S, 9, pAdvanceCurr);
    ssSetIWorkValue(S, 0, nBuffer);
}

/* mdlUpdate */
#define MDL_UPDATE
static void mdlUpdate(SimStruct *S, int_T tid)
{
    int ch;
	int nFrameIn;
	nFrameIn = (((int*)ssGetInputPortDimensions(S, 0))[0]);
    if (ssIsSampleHit(S, ssGetInputPortSampleTimeIndex(S, 0), tid)) {
        // Copy input samples into the circular buffer
        float *in = (float*)ssGetInputPortSignal(S, 0);
        float **pInputBuffer = (float**)ssGetPWorkValue(S, 0);
        float **pInputBufferEnd = (float**)ssGetPWorkValue(S, 1);
        float **pInputWrite = (float**)ssGetPWorkValue(S, 2);
        
        for (ch = 0; ch < ((int*)ssGetInputPortDimensions(S, 0))[1]; ch++) {
            memcpy(pInputWrite[ch], in, nFrameIn * sizeof(float));
            in += nFrameIn;
            pInputWrite[ch] += nFrameIn;
            if (pInputWrite[ch] >= pInputBufferEnd[ch]) {
                pInputWrite[ch] = pInputBuffer[ch];
            }
        }
    }
}

/* mdlOutputs */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    int ch, idx, samp;
	int nFrameOut;
	nFrameOut = (((int*)ssGetOutputPortDimensions(S, 0))[0]);
    if (ssIsSampleHit(S, ssGetOutputPortSampleTimeIndex(S, 0), tid)) {
        float *out = (float*)ssGetOutputPortSignal(S, 0);
        float **pInputBuffer = (float**)ssGetPWorkValue(S, 0);
        float **pInputBufferEnd = (float**)ssGetPWorkValue(S, 1);
        float **pInputWrite = (float**)ssGetPWorkValue(S, 2);
        float **pInputRead = (float**)ssGetPWorkValue(S, 3);
        float *pFilter = (float*)ssGetPWorkValue(S, 4);
        float *pFilterEnd = (float*)ssGetPWorkValue(S, 5);
        float *pFilterCurr = (float*)ssGetPWorkValue(S, 6);
        int *pAdvance = (int*)ssGetPWorkValue(S, 7);
        int *pAdvanceEnd = (int*)ssGetPWorkValue(S, 8);
        int *pAdvanceCurr = (int*)ssGetPWorkValue(S, 9);
        int nBuffer = ssGetIWorkValue(S, 0);
        int numChan = ((int*)ssGetInputPortDimensions(S, 0))[1];

        for (idx = 0; idx < nFrameOut; idx++) {
            float *pOut = out + idx;
            float *pFiltBase = pFilterCurr;
            
            // Do the polyphase convolution for each channel
            for (ch = 0; ch < numChan; ch++) {
                float *pIn = pInputRead[ch];
                float outval = 0.0;
                pFilterCurr = pFiltBase;
				for (samp = 0; samp < NFILTERBLOCK; samp++) {
					outval += *(pFilterCurr++) * *(pIn++);
					if (pIn > pInputBufferEnd[ch] - 1) {
						pIn = pInputBuffer[ch];
					}
				}
                *pOut = outval;
                pOut += nFrameOut;
            }
            
            // Move back to the first filter block if we're at the end
            if (pFilterCurr >= pFilterEnd) {
                pFilterCurr = pFilter;
            }
            
            // Increment input read as needed
            for (ch = 0; ch < numChan; ch++) {
                pInputRead[ch] += *pAdvanceCurr;
                if (pInputRead[ch] >= pInputBufferEnd[ch]) {
                    pInputRead[ch] -= nBuffer;
                }
            }
            pAdvanceCurr++;
            if (pAdvanceCurr >= pAdvanceEnd) {
                pAdvanceCurr = pAdvance;
            }
        }
        ssSetPWorkValue(S, 6, pFilterCurr);
        ssSetPWorkValue(S, 9, pAdvanceCurr);
    }
}


/* mdlTerminate (REQUIRED!!) */
static void mdlTerminate(SimStruct *S)
{
}


/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
