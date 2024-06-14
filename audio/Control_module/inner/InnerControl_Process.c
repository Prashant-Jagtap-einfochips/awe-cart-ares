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
