ModuleTestAudio 0xBEEFF2CE, BaseModule

{

    validCtrlRsp            int        // valid control response
    algoState               int        // algorithm state active/bypass
    gain                    int        // Linear gain
    STATE                   int        // Toggles AudioPP Active/Bypass
    logCTRL                 int        // Debug Start/Stop
    CONTROL_REQUEST         *uint      // Control Message Request
    CONTROL_RESPONSE        *uint      // Control Message Response
    coeff                   *float     // Coefficent Data
    pHandle                 int        // Instance handle
}

