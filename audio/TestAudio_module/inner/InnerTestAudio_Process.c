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
