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