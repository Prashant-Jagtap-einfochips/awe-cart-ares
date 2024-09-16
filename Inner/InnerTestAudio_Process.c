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