static int init = 0;
awe_modTestAudioInstance* S = (awe_modTestAudioInstance*)pInstance;
if (init == 0)
    S->coeff[0] = 0.1;
return 0;