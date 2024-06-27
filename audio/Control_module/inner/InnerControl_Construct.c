awe_modControlInstance *S = (awe_modControlInstance *) BaseClassModule_Constructor((ModClassModule *) &awe_modControlClass, retVal, nIO, pWires, argCount, args);            
            
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

if ((S->pTestAudio = (awe_modTestAudioInstance*)awe_fwMalloc(sizeof(awe_modTestAudioInstance), AWE_HEAP_FAST2SLOW, retVal)) == 0)
{
    // Error code is in *retVal                        
    return 0;
}

awe_modTestAudioInstance* pAudio = (awe_modTestAudioInstance*)S->pTestAudio;
            
if ((pAudio->coeff = (FLOAT32 *) awe_fwMalloc(sizeof(FLOAT32) * 11, AWE_HEAP_FAST2SLOW, retVal)) == 0)                        
{                        
   	// Error code is in *retVal                        
   	return 0;                        
}            
              
return ((ModInstanceDescriptor *) S);