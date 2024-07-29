awe_modControlInstance *S = (awe_modControlInstance *)pInstance;

if (S->modPtr == NULL)
{
    return;
}
else
{
	UINT32 i = 0;
	awe_modTestAudioInstance* pAudio = (awe_modTestAudioInstance*)S->modPtr;
    for (i = 0; i < 11; i++) {
    	pAudio->coeff[i] = S->coeff[i];
		pAudio->CONTROL_REQUEST[i] = S->CONTROL_REQUEST[i];
    }
    
    AWE_CLASSMODULE_SETALL(pAudio);
}

return 0;