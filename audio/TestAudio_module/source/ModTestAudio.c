/****************************************************************************
*
*               Audio Framework
*               ---------------
*
****************************************************************************
*     ModTestAudio.c
****************************************************************************
*
*     Description:  Copies input wire to output wire
*
*     Copyright:    2007-2022 DSP Concepts, Inc. All rights reserved.
*                   3235 Kifer Road
*                   Santa Clara, CA 95054
*
***************************************************************************/

/**
 * @addtogroup Modules
 * @{
 */

/**
 * @file
 * @brief Copies input wire to output wire
 */

#define NOREDEF


#include "Framework.h"
#include "Errors.h"
#include "ModTestAudio.h"


#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------------
** THIS IS AN AUTO-GENERATED FILE. Please make all changes in the inner file. 
** Changes made to this file will be overwritten during module generation.
** Audio module class object.  This describes the audio module to the
** framework.  It contains pointers to functions and number of
** variables.
** ------------------------------------------------------------------- */

CREATE_MODULE_CLASS(Class_awe_modTestAudio, (1 + 1))

AWE_MOD_SLOW_ANY_CONST
const Class_awe_modTestAudio awe_modTestAudioClass =
{
    {
        { awe_modTestAudioConstructor, CLASSID_TESTAUDIO, },
        awe_modTestAudioProcess,                // Processing function
        IOMatchUpModule_Bypass,                 // Bypass function
        0,                                    // Set function
        0,                                    // Get function
        0,				                      // Unused field
        ClassModule_PackArgCounts(1, 1),    // (Public words, private words)
        {0x00000002, 0x00000000}, // Specifies which variables are floating-point
    },
#ifdef BUILD64
    {
        offsetof(awe_modTestAudioInstance, gain),
        offsetof(awe_modTestAudioInstance, coeff),
    }
#endif
};


/* ----------------------------------------------------------------------
** Memory allocation function.  This is required because the module
** requires additional memory outside of its instance structure.
** ------------------------------------------------------------------- */

AWE_MOD_SLOW_CODE
ModInstanceDescriptor *awe_modTestAudioConstructor(INT32 * FW_RESTRICT retVal, UINT32 nIO, WireInstance ** FW_RESTRICT pWires, size_t argCount, const Sample * FW_RESTRICT args)
{   
	awe_modTestAudioInstance *S = (awe_modTestAudioInstance *) BaseClassModule_Constructor((ModClassModule *) &awe_modTestAudioClass, retVal, nIO, pWires, argCount, args);
	
	// Check if BaseClassModule_Constructor() finished properly.  If not,
	// the error code is in *retVal
	if (S == NULL)
	{
	    return 0;
	}
	
    if ((S->coeff = (FLOAT32 *) awe_fwMalloc(sizeof(FLOAT32) * 11, AWE_HEAP_FAST2SLOW, retVal)) == 0)            
    {            
        // Error code is in *retVal            
        return 0;            
    }            
                
                
    return ((ModInstanceDescriptor *) S);            

    
}

/* ----------------------------------------------------------------------
** Real-time Processing function.
** ------------------------------------------------------------------- */

AWE_MOD_FAST_CODE
void awe_modTestAudioProcess(void *pInstance)
{
    awe_modTestAudioInstance *S = (awe_modTestAudioInstance *)pInstance;
    WireInstance **pWires = ClassModule_GetWires(S);
    UINT32 numSamples = ClassWire_GetNumSamples(pWires[0]);
    FLOAT *pSrc = (FLOAT *)(pWires[0] ->buffer);
    FLOAT *pDst = (FLOAT *)(pWires[1] ->buffer);
    UINT32 i;
    for (i = 0; i < numSamples; i++)
    {
        switch(S->gain) {
    	case 1:
    	    *pDst++ = *pSrc++ * S->coeff[1];
    		break;
    	case 2:
    		*pDst++ = *pSrc++ * S->coeff[2];
    		break;
    	case 3:
    		*pDst++ = *pSrc++ * S->coeff[3];
    		break;
    	case 4:
    		*pDst++ = *pSrc++ * S->coeff[4];
    		break;
    	case 5:
    		*pDst++ = *pSrc++ * S->coeff[5];
    		break;
    	case 6:
    		*pDst++ = *pSrc++ * S->coeff[6];
    		break;
    	case 7:
    		*pDst++ = *pSrc++ * S->coeff[7];
    		break;
    	case 8:
    		*pDst++ = *pSrc++ * S->coeff[8];
    		break;
    	case 9:
    		*pDst++ = *pSrc++ * S->coeff[9];
    		break;
    	case 10:
    		*pDst++ = *pSrc++ * S->coeff[10];
    		break;
    	default:
    		*pDst++ = *pSrc++ * 0;
    		break;
    	}
    }
}




#ifdef __cplusplus
}
#endif

/**
 * @}
 *
 * End of file.
 */
