awe_modControlInstance *S = (awe_modControlInstance *)pInstance;
awe_modTestAudioInstance* pAudio = (awe_modTestAudioInstance*)S->pTestAudio;
UINT32 i;
for (i = 0; i < 11; i++) {
	S->coeff[i] = pAudio->coeff[i];
}
return 0;