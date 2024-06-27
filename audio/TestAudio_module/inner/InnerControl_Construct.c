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