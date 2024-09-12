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

#include "BAF.h"
#include "BAF_Message_Id.h"
#define CONTROL_RESPONSE_INVALID 0U
#define CONTROL_RESPONSE_VALID   1U
#define AUDIOPP_STATE_BYPASS     0U
#define AUDIOPP_STATE_ACTIVE     1U

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

CREATE_MODULE_CLASS(Class_awe_modTestAudio, (5 + 4))

AWE_MOD_SLOW_ANY_CONST
const Class_awe_modTestAudio awe_modTestAudioClass =
{
    {
        { awe_modTestAudioConstructor, CLASSID_TESTAUDIO, },
        awe_modTestAudioProcess,                // Processing function
        IOMatchUpModule_Bypass,                 // Bypass function
        awe_modTestAudioSet,                    // Set function
        awe_modTestAudioGet,                    // Get function
        0,				                      // Unused field
        ClassModule_PackArgCounts(5, 4),    // (Public words, private words)
        {0x00000080, 0x00000000}, // Specifies which variables are floating-point
    },
#ifdef BUILD64
    {
        offsetof(awe_modTestAudioInstance, validCtrlRsp),
        offsetof(awe_modTestAudioInstance, algoState),
        offsetof(awe_modTestAudioInstance, gain),
        offsetof(awe_modTestAudioInstance, STATE),
        offsetof(awe_modTestAudioInstance, logCTRL),
        offsetof(awe_modTestAudioInstance, CONTROL_REQUEST),
        offsetof(awe_modTestAudioInstance, CONTROL_RESPONSE),
        offsetof(awe_modTestAudioInstance, coeff),
        offsetof(awe_modTestAudioInstance, pHandle),
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
	
	
    awe_modTestAudioInstance* S = (awe_modTestAudioInstance*)BaseClassModule_Constructor((ModClassModule*)&awe_modTestAudioClass, retVal, nIO, pWires, argCount, args);            
                
    BAF* pHandle = NULL;            
    BAF_Resource* pResource = NULL;            
    INT32 ret = 0;            
                
                
    // Check if BaseClassModule_Constructor() finished properly.  If not,            
    // the error code is in *retVal            
    if (S == NULL)            
    {            
    	return 0;            
    }            
                
    pHandle = BAF_create(NULL);            
    if (pHandle == NULL)            
    {            
    	*retVal = E_PARAMETER_ERROR;            
    	return 0;            
    }            
                
    pResource = BAF_resource(pHandle);            
    if (pResource == NULL)            
    {            
    	*retVal = E_PARAMETER_ERROR;            
    	return 0;            
    }            
                
    if (pResource->memory_size > 0)            
    {            
    	if ((pResource->memory_pointer = (void*)awe_fwMalloc(pResource->memory_size, AWE_HEAP_FAST2SLOW, retVal)) == 0)            
    	{            
    		// Error code is in *retVal            
    		return 0;            
    	}            
    	pResource->memory_log_pointer = NULL;            
    	pResource->memory_log_size = 0;            
    	pResource->print_log_size = 0;            
    	pResource->capture_log_size = 0;            
    	pResource->exec_log_size = 0;            
    	pResource->base_thread = BASE_TASKID;            
    	pResource->ext_scheduler = EXT_SCHEDULER;            
    	pResource->mask = 0;            
    }            
                
    ret = BAF_init(pHandle);            
    if (ret != 0)            
    {            
    	*retVal = E_PARAMETER_ERROR;            
    	return 0;            
    }            
                
    S->pHandle = pHandle;            
                
    // CONTROL_REQUEST Message Pointer            
    if ((S->CONTROL_REQUEST = (UINT32*)awe_fwMalloc(CONTROL_MAX_PAYLOAD_SIZE * sizeof(UINT32), AWE_HEAP_FAST2SLOW, retVal)) == 0)            
    {            
    	// Error code is in *retVal            
    	return 0;            
    }            
                
    // CONTROL_RESPONSE Message Pointer            
    if ((S->CONTROL_RESPONSE = (UINT32*)awe_fwMalloc(CONTROL_MAX_PAYLOAD_SIZE * sizeof(UINT32), AWE_HEAP_FAST2SLOW, retVal)) == 0)            
    {            
    	// Error code is in *retVal            
    	return 0;            
    }            
                
    S->validCtrlRsp = CONTROL_RESPONSE_INVALID;            
                
    S->STATE = AUDIOPP_STATE_BYPASS;            
    S->algoState = AUDIOPP_STATE_BYPASS;            
    S->logCTRL = 0;            
                
                
    if ((S->coeff = (FLOAT32 *) awe_fwMalloc(sizeof(FLOAT32) * 11, AWE_HEAP_FAST2SLOW, retVal)) == 0)            
    {            
    	// Error code is in *retVal            
    	return 0;            
    }             
                
    return ((ModInstanceDescriptor*)S);            

    
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
    FLOAT32 *pSrc = (FLOAT32 *)(pWires[0] ->buffer);
    FLOAT32 *pDst = (FLOAT32 *)(pWires[1] ->buffer);
    UINT32 i;
    
    if (S->algoState == AUDIOPP_STATE_ACTIVE)
    {
    #if 0
        BAF *pBAF = (BAF *)S->pHandle;
        BAF_exec(pBAF, BASE_TASKID, (void *)pInstance);
    #else
    	WireInstance **pWires = ClassModule_GetWires(S);
        UINT32 numInPins = ClassModule_GetNInWires(S);
        FLOAT32 *ptrSrc1 = (FLOAT32 *)(pWires[0]->buffer);
        FLOAT32 *ptrDst1 = (FLOAT32 *)(pWires[numInPins]->buffer);
        UINT32 numChannelsIn = ClassWire_GetChannelCount(pWires[0]);
        UINT32 numChannelsOut = ClassWire_GetChannelCount(pWires[numInPins]);
        UINT32 blockSize = ClassWire_GetBlockSize(pWires[0]);
        UINT32 i = 0;
        UINT32 j = 0;
        
        UINT32 channelCnt = (numChannelsIn < numChannelsOut) ? numChannelsIn : numChannelsOut;
        
        for (i = 0; i < channelCnt; i++)
        {
            FLOAT32 *pSrc = (ptrSrc1 + i);
            FLOAT32 *pDst = (ptrDst1 + i);
            for (j = 0; j < blockSize; j++)
            {
         	    *pDst = *pSrc;
         	    pSrc += numChannelsIn;
         	    pDst += numChannelsOut;
            }
        }
    #endif
    }
    else if (S->algoState == AUDIOPP_STATE_BYPASS)
    {
    #if 0
        WireInstance **pWires = ClassModule_GetWires(S);
        UINT32 numInPins = ClassModule_GetNInWires(S);
        FLOAT32 *ptrSrc1 = (FLOAT32 *)(pWires[0]->buffer);
        FLOAT32 *ptrDst1 = (FLOAT32 *)(pWires[numInPins]->buffer);
        UINT32 numChannelsIn = ClassWire_GetChannelCount(pWires[0]);
        UINT32 numChannelsOut = ClassWire_GetChannelCount(pWires[numInPins]);
        UINT32 blockSize = ClassWire_GetBlockSize(pWires[0]);
        UINT32 i = 0;
        UINT32 j = 0;
        
        UINT32 channelCnt = (numChannelsIn < numChannelsOut) ? numChannelsIn : numChannelsOut;
        
        for (i = 0; i < channelCnt; i++)
        {
            FLOAT32 *pSrc = (ptrSrc1 + i);
            FLOAT32 *pDst = (ptrDst1 + i);
            for (j = 0; j < blockSize; j++)
            {
         	    *pDst = *pSrc;
         	    pSrc += numChannelsIn;
         	    pDst += numChannelsOut;
            }
        }
    
    #else
    
    pSrc = (FLOAT32*)(pWires[0]->buffer);
    pDst = (FLOAT32*)(pWires[1]->buffer);
    
    for (i = 0; i < numSamples; i++)
    {
    	switch(S->gain) {
    	case 0:
    		*pDst++ = *pSrc++ * S->coeff[0];
    		break;
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
    #endif
    }
}

/* ----------------------------------------------------------------------
** Set function which updates derived parameters based on the
** module's interface variables.
** ------------------------------------------------------------------- */

AWE_MOD_SLOW_CODE
UINT32 awe_modTestAudioSet(void *pInstance, UINT32 mask)
{
    awe_modTestAudioInstance* S = (awe_modTestAudioInstance*)pInstance;
    
    BAF *baf = (BAF*)(S->pHandle);
    
    if (S == NULL)
    {
        return 0;
    }
    
    if (mask == 0)
    {
        // Close()function if any
        
        return 0;
    }
    
    
    if (mask & MASK_TestAudio_CONTROL_REQUEST)
    {
        BAF_Message baf_message_req;
        BAF_Message baf_message_rsp;
        
        baf_message_req.id = S->CONTROL_REQUEST[0];
        baf_message_req.size = S->CONTROL_REQUEST[1];
        baf_message_req.data = &(S->CONTROL_REQUEST[2]);
        //BOSE_LOG_printf("baf_message_req id 0x%8.8x size %d\n", baf_message_req.id, baf_message_req.size);
        
        baf_message_rsp.size = CONTROL_MAX_PAYLOAD_SIZE-2;
        baf_message_rsp.data = &(S->CONTROL_RESPONSE[2]);
        BAF_command(baf, &baf_message_req, &baf_message_rsp);
        
        //BOSE_LOG_printf("baf_message_rsp id 0x%8.8x size %d\n", baf_message_rsp.id, baf_message_rsp.size);
        
        S->CONTROL_RESPONSE[0] = baf_message_rsp.id;
        S->CONTROL_RESPONSE[1] = baf_message_rsp.size;
        S->validCtrlRsp = CONTROL_RESPONSE_VALID;
    }
    
    if (mask & MASK_TestAudio_STATE)
    {
        if (S->STATE == AUDIOPP_STATE_BYPASS)
        {
           S->algoState = AUDIOPP_STATE_BYPASS;
        }
        else if (S->STATE == AUDIOPP_STATE_ACTIVE)
        {
           S->algoState = AUDIOPP_STATE_ACTIVE;
        }
    }
    
    if (S->coeff[0] == 0)
        S->coeff[0] = 0.1;
    else
        S->coeff[0] = S->coeff[0];
    
    if (S->gain != S->gain)
        S->gain = S->gain;
    
    return 0;
}

/* ----------------------------------------------------------------------
** Get function which updates high-level variables based on the module's
** internal variables.
** ------------------------------------------------------------------- */

AWE_MOD_SLOW_CODE
UINT32 awe_modTestAudioGet(void *pInstance, UINT32 mask)
{
    awe_modTestAudioInstance* S = (awe_modTestAudioInstance*)pInstance;
    
    if (S == NULL)
    {
        return 0;
    }
    
    if (mask == 0)
    {
        // Close()function if any
        
        return 0;
    }
    
    if (mask & MASK_TestAudio_CONTROL_RESPONSE)
    {
        if (S->validCtrlRsp == CONTROL_RESPONSE_VALID)
        {
        }
        else
        {
            /* Indicate that there there is no valid response TBD */
            S-> CONTROL_RESPONSE[0] = -1; /* id */
            S-> CONTROL_RESPONSE[1] = 0; /* size */
        }
        S->validCtrlRsp = CONTROL_RESPONSE_INVALID;
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
