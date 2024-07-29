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