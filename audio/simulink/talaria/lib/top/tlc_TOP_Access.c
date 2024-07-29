%% TLC code generation file - hand written source
%% INCLUDED BY: TOP_Direct.tlc and TOP_Indirect.tlc 
        
%% Shared routines used by both direct and indirect addressing modes

// -----------------------------------------------------------------
static int SetTOP(const uint32_t token, const uint32_t offset, const uint32_t byteCount,
    const void* data)
{
    int errCode;
    void* theAddress = ValidateArgsForTOP(token, offset, byteCount, &errCode);
    if (theAddress && errCode == 0)
    {
        // Set the TOP data range
        memcpy(theAddress, data, byteCount);
    }

    return errCode;
}

// -----------------------------------------------------------------
static int GetTOP(const uint32_t token, const uint32_t offset, const uint32_t byteCount,
    void* data)
{
    int errCode;
    void* theAddress;

    theAddress = ValidateArgsForTOP(token, offset, byteCount, &errCode);
    if (theAddress && errCode == 0)
    {
        // Retrieve data from the TOP data range
        memcpy(data, theAddress, byteCount);
    }

    return errCode;
}

// -----------------------------------------------------------------
/** Handle request to read or write Target Overlay Parameter (TOP).
 *  @param pReqBuf Request buffer containing @ref TOP_WriteRequest_type
 *  @param reqBufSize number of bytes used in the request buffer, which can
 *         be described by the structure appropriate to the request:
 *         either @ref TOP_WriteResponse_type or @ref TOP_ReadResponse_type.
 *  @param pRspBuf Response buffer to receive response of either TOP_ReadResponse_type
 *         or TOP_WriteResponse_type, depending on the type of request
 *  @param rspBufSize Size of available bytes provided for the response to fill
 *  @return Returns the number of bytes utilized in the response buffer, or
 *          ASDSTATUS_RSP_BUFFER_SMALL if the response buffer is too small to
 *          contain a AsdIdStatus_type payload (which could in turn hold
 *          an error). 
 **/
int %<ModelPrefix()>HandleTopRequest(void* pReqBuf, uint32_t reqBufSize,
                     void* pRspBuf, uint32_t rspBufSize)
{
    // returns number of bytes
    int retVal = 0;
    int status = ASDSTATUS_OK;
    uint32_t reqRequiredBytes;
    uint32_t rspRequiredBytes;

    // parse out the message id and caller handle
    if (reqBufSize < (sizeof(uint32_T)*2) || 
        pReqBuf == 0)
    {
        return ASDSTATUS_REQ_BUFFER_SMALL;
    }
    uint32_t msgId = ((uint32_T*)pReqBuf)[0];
    uint32_t callerHandle = ((uint32_T*)pReqBuf)[1];
    
    if (TOPID_IS_WRITE(msgId))
    {
        // WRITE data from request buffer into memory. Respond with just the topId
        TOP_WriteRequest_type* topREQ = (TOP_WriteRequest_type*)pReqBuf;
        TOP_WriteResponse_type* topRSP = (TOP_WriteResponse_type*)pRspBuf;
        reqRequiredBytes = sizeof(TOP_WriteRequest_type) + topREQ->byteCount;
        rspRequiredBytes = sizeof(TOP_WriteResponse_type);
        uint8_t reqBufOK = (reqBufSize >= reqRequiredBytes) && pReqBuf;
        uint8_t rspBufOK = (rspBufSize >= rspRequiredBytes) && pRspBuf;
        if (reqBufOK && rspBufOK)
        {
            // Write the data into memory
            status = SetTOP(topREQ->topId, topREQ->offset, topREQ->byteCount, topREQ->data);

            // Prepare response payload
            topRSP->topId = msgId;
            topRSP->callerHandle = callerHandle;
            topRSP->statusCode = status;
            retVal = (int)rspRequiredBytes;
        }
        else
        {
            // one of the buffers was too small
            status = reqBufOK ? ASDSTATUS_RSP_BUFFER_SMALL : ASDSTATUS_REQ_BUFFER_SMALL;
        }
    }
    else // (checking read vs. write)
    {
        // READ data from memory into the response buffer
        TOP_ReadRequest_type* topREQ = (TOP_ReadRequest_type*)pReqBuf;
        TOP_ReadResponse_type* topRSP = (TOP_ReadResponse_type*)pRspBuf;
        reqRequiredBytes = sizeof(TOP_ReadRequest_type);
        rspRequiredBytes = sizeof(TOP_ReadResponse_type) + topREQ->byteCount;
        uint8_t reqBufOK = (reqBufSize >= reqRequiredBytes) && pReqBuf;
        uint8_t rspBufOK = (rspBufSize >= rspRequiredBytes) && pRspBuf;
        if (reqBufOK && rspBufOK)
        {
            // Save the byte count
            uint32_t byteCount = topREQ->byteCount;
            
            // Copy the data from memory into the response buffer
            status = GetTOP(
                    topREQ->topId, topREQ->offset, topREQ->byteCount, // from REQ
                    topRSP->data);                                   // to RSP
            
            // Prepare the rest of the response payload
            topRSP->topId = msgId;
            topRSP->callerHandle = callerHandle;
            topRSP->statusCode = status;
            topRSP->byteCount = byteCount;
            retVal = (int)rspRequiredBytes;
        }
        else
        {
            // one of the buffers was too small
            status = reqBufOK ? ASDSTATUS_RSP_BUFFER_SMALL : ASDSTATUS_REQ_BUFFER_SMALL;
        }
    }

    // If a problem has arisen, the response is only a status payload
    if (status < 0)
    {
        rspRequiredBytes = sizeof(AsdIdStatus_type);
        if ((rspBufSize >= rspRequiredBytes) && pRspBuf)
        {
            // Fill in the response with information from the request
            AsdIdStatus_type* topRSP = (AsdIdStatus_type*)pReqBuf;
            topRSP->msgId = msgId;
            topRSP->callerHandle = callerHandle;
            topRSP->statusCode = status;
            retVal = (int)rspRequiredBytes;
        }
        else
        {
            // otherwise, we cannot return a response buffer!
            retVal = ASDSTATUS_RSP_BUFFER_SMALL;
        }
    }

    return retVal;
}