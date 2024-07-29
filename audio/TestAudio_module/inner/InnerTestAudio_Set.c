awe_modTestAudioInstance* S = (awe_modTestAudioInstance*)pInstance;

BAF *baf = (BAF*)(S->pHandle);

if (S == NULL)
{
    return 0;
}

if (mask == 0)
{
    // Close()function if any
    
    return 0;
}


if (mask & MASK_TestAudio_CONTROL_REQUEST)
{
    BAF_Message baf_message_req;
    BAF_Message baf_message_rsp;
    
    baf_message_req.id = S->CONTROL_REQUEST[0];
    baf_message_req.size = S->CONTROL_REQUEST[1];
    baf_message_req.data = &(S->CONTROL_REQUEST[2]);
    //BOSE_LOG_printf("baf_message_req id 0x%8.8x size %d\n", baf_message_req.id, baf_message_req.size);
    
    baf_message_rsp.size = CONTROL_MAX_PAYLOAD_SIZE-2;
    baf_message_rsp.data = &(S->CONTROL_RESPONSE[2]);
    BAF_command(baf, &baf_message_req, &baf_message_rsp);
    
    //BOSE_LOG_printf("baf_message_rsp id 0x%8.8x size %d\n", baf_message_rsp.id, baf_message_rsp.size);
    
    S->CONTROL_RESPONSE[0] = baf_message_rsp.id;
    S->CONTROL_RESPONSE[1] = baf_message_rsp.size;
    S->validCtrlRsp = CONTROL_RESPONSE_VALID;
}

if (mask & MASK_TestAudio_STATE)
{
    if (S->STATE == AUDIOPP_STATE_BYPASS)
    {
       S->algoState = AUDIOPP_STATE_BYPASS;
    }
    else if (S->STATE == AUDIOPP_STATE_ACTIVE)
    {
       S->algoState = AUDIOPP_STATE_ACTIVE;
    }
}

if (S->coeff[0] == 0)
    S->coeff[0] = 0.1;
else
    S->coeff[0] = S->coeff[0];

if (S->gain != S->gain)
    S->gain = S->gain;

return 0;