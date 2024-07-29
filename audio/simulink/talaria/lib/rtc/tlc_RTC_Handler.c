%% TLC code generation file - hand written source
%% INCLUDED BY: tlc_SchTokenApi.tlc


/** Handle Run Time Control request (RTC) and generate response payload (if any).
 * NOTE: This function integrates with HandleAsdIdRequest. It is not intended
 * to be called directly if @ref HandleAsdIdRequest is more convenient.
 *  @param pReqBuf Request buffer containing RTC request...
 *  @param reqBufSize number of bytes used in the request buffer
 *  @param pRspBuf Response buffer to receive response of AsdIdStatus_type...
 *  @param rspBufSize Size of available bytes provided for the response to fill
 *  @return Returns the toal number of bytes utilized in the response buffer, or
 *          ASDSTATUS_RSP_BUFFER_SMALL if the response buffer is too small to
 *          contain a AsdIdStatus_type payload. If the returned value is positive,
 *          the caller must check the AsdIdStatus_type's statusCode. If the
 *          status code is less than zero, then any further reponse "data" as
 *          defined by the RTC service will not be present (i.e. there probably
 *          was not space for it anyway).
 *         
 * @ingroup rtc
 */
int %<ModelPrefix()>UseDispatchTable(void* pReqBuf, uint32_t reqBufSize,
                     void* pRspBuf, uint32_t rspBufSize,
                     %<RtcGetReqRspFileName()>_type* theTable, uint32_t tableSize)
{
    // returns number of bytes
    int retVal = 0;
    int status = ASDSTATUS_OK;

    // parse out the message id
    if (reqBufSize < (sizeof(uint32_t)) || 
        pReqBuf == 0)
    {
        return ASDSTATUS_REQ_BUFFER_SMALL;
    }
    uint32_t msgId        = ((uint32_t*)pReqBuf)[0];
    // Assume bogus caller handle until we're sure we've got one
    uint32_t callerHandle = 0xffffffff;

    // setup cast of the response buffer (if any)
    AsdIdStatus_type* rspStatusBuf = (AsdIdStatus_type*)pRspBuf;

    // Lookup the RTC service (by index) in the array
    uint32_t rtcIdx = RTCID_INDEX(msgId);
    if (rtcIdx < tableSize)
    {
        // Index was within array - here is our service entry:
        %<RtcGetReqRspFileName()>_type* pTableEntry = &theTable[rtcIdx];
        uint32_t reqBytes = pTableEntry->request_payload_bytes;
        uint32_t rspBytes = pTableEntry->response_payload_bytes;

        // Validate the sizes for provided request and response buffers
        // (allow either to be bigger than expected - just ignore extra bytes)
        uint8_t reqBufOK = (reqBufSize >= reqBytes) && 
                             (pReqBuf || (reqBytes==0));
        uint8_t rspBufOK = (rspBufSize >= rspBytes) && 
                             (pRspBuf || (rspBytes==0));
        
        // We need both buffers to be OK if we want to call the service handler:
        if (reqBufOK && rspBufOK)
        {
            // Request and response buffers are OK. Proceed with the call.
            pTableEntry->request_response_func(pReqBuf, pRspBuf);
            retVal = pTableEntry->response_payload_bytes;
            if (retVal) // copy in msgid if RSP payload enabled
            {
                // The caller handle and status code are set by the REQ RSP func called above.
                // However, the msgId must be manually copied in here as the REQ RSP func
                // is not aware of its own msgId.
                rspStatusBuf->msgId = msgId;
            }
        }
        else
        {
            // one of the buffers was too small
            status = reqBufOK ? ASDSTATUS_RSP_BUFFER_SMALL : ASDSTATUS_REQ_BUFFER_SMALL;
            
            // if there is supposed to be a response and the request is big enough to
            // contain a caller handle - grab it here
            if (rspBytes && reqBufSize >= sizeof(uint32_t)*2)
            {
                 callerHandle = ((uint32_t*)pReqBuf)[1];
            }
        }
    }
    else // rtc index is *not* within the table
    {
        status = ASDSTATUS_RTC_UNKNOWN_SERVICE;
    }

    // If the request-response was successful, everything is already all set.
    // Otherwise - handle errors by attempting to return an error response:
    if (status < 0)
    {
        if (rspBufSize >= sizeof(AsdIdStatus_type) && pRspBuf)
        {
            // Response buffer looks ready to receive error, so return it
            // Copy caller handle and message id still available from the REQ:
            rspStatusBuf->msgId = msgId;
            rspStatusBuf->callerHandle = callerHandle;
            // fill in the error status code
            rspStatusBuf->statusCode = status;
            retVal = sizeof(AsdIdStatus_type);
        }
        else
        {
            // Cannot even return an error payload so return error code.
            // The RSP buffer is too small, perhaps even to send back a 
            // more specific error code.
            retVal = ASDSTATUS_RSP_BUFFER_SMALL;
        }
    }

    return retVal;
}