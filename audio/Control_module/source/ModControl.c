/****************************************************************************
*
*               Audio Framework
*               ---------------
*
****************************************************************************
*     ModControl.c
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
#include "ModControl.h"


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

CREATE_MODULE_CLASS(Class_awe_modControl, (0 + 1))

AWE_MOD_SLOW_ANY_CONST
const Class_awe_modControl awe_modControlClass =
{
    {
        { awe_modControlConstructor, CLASSID_CONTROL, },
        awe_modControlProcess,                // Processing function
        IOMatchUpModule_Bypass,                 // Bypass function
        awe_modControlSet,                    // Set function
        awe_modControlGet,                    // Get function
        0,				                      // Unused field
        ClassModule_PackArgCounts(0, 1),    // (Public words, private words)
        {0x00000001, 0x00000000}, // Specifies which variables are floating-point
    },
#ifdef BUILD64
    {
        offsetof(awe_modControlInstance, coeff),
    }
#endif
};


/* ----------------------------------------------------------------------
** Memory allocation function.  This is required because the module
** requires additional memory outside of its instance structure.
** ------------------------------------------------------------------- */

AWE_MOD_SLOW_CODE
ModInstanceDescriptor *awe_modControlConstructor(INT32 * FW_RESTRICT retVal, UINT32 nIO, WireInstance ** FW_RESTRICT pWires, size_t argCount, const Sample * FW_RESTRICT args)
{   
	
	
    awe_modControlInstance *S = (awe_modControlInstance *) BaseClassModule_Constructor((ModClassModule *) &awe_modControlClass, retVal, nIO, pWires, argCount, args);            
    awe_modTestAudioInstance* pAudio = (awe_modTestAudioInstance*)S->pTestAudio;            
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
                
    if ((pAudio->coeff = (FLOAT32 *) awe_fwMalloc(sizeof(FLOAT32) * 11, AWE_HEAP_FAST2SLOW, retVal)) == 0)                        
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
void awe_modControlProcess(void *pInstance)
{
    awe_modControlInstance *S = (awe_modControlInstance *)pInstance;
    WireInstance **pWires = ClassModule_GetWires(S);
    UINT32 numSamples = ClassWire_GetNumSamples(pWires[0]);
    FLOAT *pSrc = (FLOAT *)(pWires[0] ->buffer);
    FLOAT *pDst = (FLOAT *)(pWires[1] ->buffer);
    UINT32 i;
    for (i = 0; i < numSamples; i++)
    {
        *pDst++ = *pSrc++;
    }
}

/* ----------------------------------------------------------------------
** Set function which updates derived parameters based on the
** module's interface variables.
** ------------------------------------------------------------------- */

AWE_MOD_SLOW_CODE
UINT32 awe_modControlSet(void *pInstance, UINT32 mask)
{
    awe_modControlInstance *S = (awe_modControlInstance *)pInstance;
    awe_modTestAudioInstance* pAudio = (awe_modTestAudioInstance*)S->pTestAudio;
    UINT32 i;
    for (i = 0; i < 11; i++) {
    	pAudio->coeff[i] = S->coeff[i];
    }
    return 0;
}

/* ----------------------------------------------------------------------
** Get function which updates high-level variables based on the module's
** internal variables.
** ------------------------------------------------------------------- */

AWE_MOD_SLOW_CODE
UINT32 awe_modControlGet(void *pInstance, UINT32 mask)
{
    awe_modControlInstance *S = (awe_modControlInstance *)pInstance;
    awe_modTestAudioInstance* pAudio = (awe_modTestAudioInstance*)S->pTestAudio;
    UINT32 i;
    for (i = 0; i < 11; i++) {
    	S->coeff[i] = pAudio->coeff[i];
    }
    return 0;
}


#ifdef __cplusplus
}
#endif

/**
 * @}
 *
 * End of file.
 */
