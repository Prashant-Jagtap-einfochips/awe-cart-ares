%% TLC code generation file - hand written source
%% API calls supporting TSP inports(capture of state on target core)
%% INCLUDED BY : TSP_Capture.tlc

#if (1 == %<EnableAdvancedDefine>)
// Capture buffer
static int8_t tspCaptureBuffer[TSP_CAPTURE_BUFFER_SIZE];
#endif // (1 == %<EnableAdvancedDefine>)

%if IsIndirect
    // "Quick lookup" flags to determine if a partition is capturing
    static char partitionActiveFlag[256] = {0};
%endif

// File scope function prototypes
static void GetTableByMsgId(uint32_t msgId, %<config.CaptureFileName>_type** theTable, uint32_t* tableSize);
%if IsIndirect
static void GetTableByPartitionIndex(int partitionIndex,%<config.CaptureFileName>_type** theTable, uint32_t* tableSize);
static int GetPartitionIndex(uint32_t msgId);
%endif
#if (1 == %<EnableAdvancedDefine>)
%if IsIndirect
static bool      IsPartitionActive(int partitionIndex);
%endif
static uint32_t  GetRequiredCaptureBufferBytes(void);
static int32_t   TSP_CaptureListAdd(TspListAddReqType* theReq, uint32_t bufferBytes);
static ASDSTATUS TSP_CaptureListSub(TspListSubReqType* subReq);
static void      TSP_CaptureListClear(void);
static uint32_t  TSP_CaptureTrigger(int8_t* captureBuffer, uint32_t bufferBytes);
static bool      TSP_CaptureCheckComplete(void);
static uint32_t  TSP_CaptureRetrievalSize(void);
static ASDSTATUS TSP_CaptureRetrieve(TspRetrieveReqType* theReq,
        uint32_t* charsReturnedOut, int8_t* bufferOut, uint32_t bufferOutSize);
#endif // (1 == %<EnableAdvancedDefine>)
static ASDSTATUS TSP_BlindRead(TspBlindReadReqType* theReq,
        uint32_t* charsReturnedOut, int8_t* bufferOut, uint32_t bufferOutSize);
static ASDSTATUS TSP_BlindWrite(TspBlindWriteReqType* theReq,
    uint32_t charsSentIn, int8_t* bufferIn);


/** Handle Target State Prob request (TSP) and generate response payload (if any).
 * NOTE: This function integrates with HandleAsdIdRequest. It is not intended
 * to be called directly if @ref %<ModelPrefix()>HandleAsdIdRequest is more convenient.
 *  @param pReqBuf Request buffer containing RTC request
 *  @param reqBufSize number of bytes used in the request buffer
 *  @param pRspBuf Response buffer to receive response of AsdIdStatus_type
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
int %<ModelPrefix()>HandleTspRequest(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize)
{
    // returns number of bytes
    int retVal = 0;
    int status = ASDSTATUS_OK;

    // parse out the message id
    if (reqBufSize < sizeof(uint32_T) || pReqBuf == 0)
    {
        return ASDSTATUS_REQ_BUFFER_SMALL;
    }
    if (rspBufSize < sizeof(AsdIdStatus_type) || pRspBuf == 0)
    {
        return ASDSTATUS_RSP_BUFFER_SMALL;
    }

    // setup cast of the response buffer (if any)
    uint32_t msgId = ((uint32_t*)pReqBuf)[0];
    uint32_t callerHandle = ((uint32_t*)pReqBuf)[1];
    AsdIdStatus_type* rspStatusBuf = (AsdIdStatus_type*)pRspBuf;
    rspStatusBuf->msgId = msgId;
    rspStatusBuf->callerHandle = callerHandle;

    // Setup default returned size & override as necessary
    retVal = sizeof(AsdIdStatus_type);

    // Extract TSP information from the msgId
    uint32_t tspCmd = RTCID_INFO(msgId);

    switch (tspCmd)
    {
#if (1 == %<EnableAdvancedDefine>)
    case TSP_COMMAND_LIST_ADD:
    {
        TspListAddReqType* theReq = (TspListAddReqType*)pReqBuf;
        if (reqBufSize < sizeof(TspListAddReqType))
        {
            status = ASDSTATUS_REQ_BUFFER_SMALL;
        }
        else
        {
            status = TSP_CaptureListAdd(theReq, TSP_CAPTURE_BUFFER_SIZE);
        }
    }
    break;
    case TSP_COMMAND_LIST_SUB:
    {
        TspListSubReqType* theReq = (TspListSubReqType*)pReqBuf;
        if (reqBufSize < sizeof(TspListSubReqType))
        {
            status = ASDSTATUS_REQ_BUFFER_SMALL;
        }
        else
        {
            status = TSP_CaptureListSub(theReq);
        }
    }
    break;
    case TSP_COMMAND_LIST_CLEAR:
    {
        if (reqBufSize < sizeof(TspListClearReqType))
        {
            status = ASDSTATUS_REQ_BUFFER_SMALL;
        }
        else
        {
            TSP_CaptureListClear();
        }
    }
    break;
    case TSP_COMMAND_CAPTURE:
    {
        TspCaptureReqType* theReq = (TspCaptureReqType*)pReqBuf;
        if (reqBufSize < sizeof(TspCaptureReqType))
        {
            status = ASDSTATUS_REQ_BUFFER_SMALL;
        }
        else
        {
            uint32_t totalCaptures = TSP_CaptureTrigger(&tspCaptureBuffer[0], TSP_CAPTURE_BUFFER_SIZE);
            // setup state to support capture complete notification
            triggerRequestSequence++;
            triggerRequestTotalCaptureCount = totalCaptures;
            triggerRequestCallerHandle = theReq->callerHandle;
        }
    }
    break;
    case TSP_COMMAND_RETRIEVE:
    {
        TspRetrieveReqType* theReq = (TspRetrieveReqType*)pReqBuf;
        if (reqBufSize < sizeof(TspRetrieveReqType))
        {
            status = ASDSTATUS_REQ_BUFFER_SMALL;
        }
        else
        {
            uint32_t requiredRspSize = sizeof(TspRetrieveRspType) + theReq->charCount;
            if (rspBufSize < requiredRspSize)
            {
                status = ASDSTATUS_RSP_BUFFER_SMALL;
            }
            else
            {
                retVal = requiredRspSize;
                TspRetrieveRspType* theRsp = (TspRetrieveRspType*)pRspBuf;
                status = TSP_CaptureRetrieve(theReq,
                    &(theRsp->charCount), theRsp->data, theReq->charCount);
            }
        }
    }
    break;
#else
    case TSP_COMMAND_LIST_ADD:
    case TSP_COMMAND_LIST_SUB:
    case TSP_COMMAND_LIST_CLEAR:
    case TSP_COMMAND_CAPTURE:
    case TSP_COMMAND_RETRIEVE:
        // All advanced triggered capture code is removed from the image.
        // Return a special error code informing caller that this binary is
        // not able to service triggered capture requests.
        status = ASDSTATUS_TSP_CAPTURE_ALL_OFF;
        break;
#endif // (1 == %<EnableAdvancedDefine>)
    case TSP_COMMAND_BLIND_READ:
    {
        if (TSPID_IS_WRITE(msgId))
        {
            // Blind Write
            TspBlindWriteReqType* theReq = (TspBlindWriteReqType*)pReqBuf;
            uint32_t requiredReqSize = sizeof(TspBlindWriteReqType) + theReq->charCount;
            if (reqBufSize < requiredReqSize)
            {
                status = ASDSTATUS_REQ_BUFFER_SMALL;
            }
            else
            {
                if (rspBufSize < sizeof(TspBlindWriteRspType))
                {
                    status = ASDSTATUS_RSP_BUFFER_SMALL;
                }
                else
                {
                    status = TSP_BlindWrite(theReq, theReq->charCount,
                        theReq->data);
                }
            }
        }
        else
        {
            // Blind Read
            TspBlindReadReqType* theReq = (TspBlindReadReqType*)pReqBuf;
            if (reqBufSize < sizeof(TspBlindReadReqType))
            {
                status = ASDSTATUS_REQ_BUFFER_SMALL;
            }
            else
            {
                uint32_t requiredRspSize = sizeof(TspBlindReadRspType) + theReq->charCount;
                if (rspBufSize < requiredRspSize)
                {
                    status = ASDSTATUS_RSP_BUFFER_SMALL;
                }
                else
                {
                    retVal = requiredRspSize;
                    TspBlindReadRspType* theRsp = (TspBlindReadRspType*)pRspBuf;
                    status = TSP_BlindRead(theReq, &(theRsp->charCount),
                        theRsp->data, theReq->charCount);
                }
            }
        }
    }
    break;
    default:
        status = ASDSTATUS_RTC_UNKNOWN_SERVICE;
    }

    // If the request-response was successful, everything is already all set.
    // Otherwise - handle errors by attempting to return an error response:
    rspStatusBuf->statusCode = status;
    if (status < 0)
    {
        // Response buffer looks ready to receive error, so return it
        // Copy caller handle and message id still available from the REQ:
        retVal = sizeof(AsdIdStatus_type);
    }

    return retVal;
}

static void GetTableByMsgId(uint32_t msgId, %<config.CaptureFileName>_type** theTable, uint32_t* tableSize)
{
    %if IsIndirect
        int partitionIndex = GetPartitionIndex(msgId);
        GetTableByPartitionIndex(partitionIndex, theTable, tableSize);
    %else
        (void)(msgId);
        %% TableName must manually match TSP_Common.tlc
        %assign TableName = ModelPrefix() + FEVAL("TspManager.GetDispatchTableName",0,0)
        // Direct addressing uses just one table:
        *theTable = %<TableName>;
        *tableSize = %<TableName>Count;
    %endif
}
%if IsIndirect
    static void GetTableByPartitionIndex(int partitionIndex,%<config.CaptureFileName>_type** theTable, uint32_t* tableSize)
    {
            PartitionRoot_type* partitionRoot = GetPartitionRootByIndex(partitionIndex);
            if (partitionRoot && partitionRoot->tspTable && partitionRoot->tspCount)
            {
                *theTable = *(%<config.CaptureFileName>_type**)partitionRoot->tspTable;
                *tableSize = *partitionRoot->tspCount;
            }
            else
            {
                *theTable = 0;
                *tableSize = 0;
            }
    }
    static int GetPartitionIndex(uint32_t msgId)
    {
        uint32_t groupNumber = ASDID_GROUP(msgId);
        uint32_t partitionNumber = ASDID_PARTITION(msgId);
        return ((groupNumber & 0xf) * 16U) + (partitionNumber & 0xf);
    }
%endif

//-------------------------------------------------------------------------
// Advanced TSP capture capabilities
#if (1 == %<EnableAdvancedDefine>)

%if IsIndirect
// Returns true if anything in partition desires captures
static bool IsPartitionActive(int partitionIndex)
{
    bool isActive = false;
    PartitionRoot_type* partitionRoot = GetPartitionRootByIndex(partitionIndex);
    %<config.CaptureFileName>_type* theTable = (%<config.CaptureFileName>_type*)partitionRoot->tspTable;
    int tableSize = *partitionRoot->tspCount;
    int i;
    for (i = 0; i < tableSize; i++)
    {
        %<config.CaptureFileName>_type* theEntry = &theTable[i];
        if (theEntry->capturesDesired > 0)
        {
            isActive = true;
            break;
        }
    }
    return isActive;
}
%endif

// Capture TSP during step operations (i.e. AUDIO THREAD!)
void %<ModelPrefix()>TSP_StepCapture(void* tablePtr, uint32_t tableIndex, const void* sourceBuffer)
{
    // Assume the tableIndex is good because it was placed by generated code
    // if a capture is happening (and a safety check for table bounds)
    if (triggerActive)
    {
        // access the table entry and perform the capture
        %<config.CaptureFileName>_type* theEntry = &(((%<config.CaptureFileName>_type*)tablePtr)[tableIndex]);
        // if the desired number of captures have not yet been completed
        // (... and there's an address of where to capture)
        if (theEntry->capturesComplete < theEntry->capturesDesired
            /*[safe]*/&& theEntry->captureAddress)
        {
            // First check that the skip interval has passed
            if (theEntry->skipCounter >= theEntry->captureSkipInterval)
            {
                // Setup a destination header struct inside the capture buffer
                uint32_t requiredBytes = theEntry->sizeInChar;
                int8_t* destination = (int8_t*)
                    (theEntry->captureAddress + (requiredBytes * theEntry->capturesComplete));

                // Copy in data from the TSP source buffer and reset the skip counter
                memcpy(destination, sourceBuffer, theEntry->sizeInChar);
                ++(theEntry->capturesComplete);
                theEntry->skipCounter = 0;
            }
            else
            {
                // Skip this frame
                ++(theEntry->skipCounter);
            }
        }
    }
}

static uint32_t GetRequiredCaptureBufferBytes()
{
    uint32_t i;
    uint32_t totalBytes = 0;
    %<config.CaptureFileName>_type* theTable = 0;
    uint32_t tableSize;
    
    %if IsIndirect
        int p;
        for (p=0; p<256; p++)
        {
            if (partitionActiveFlag[p])
            {
                GetTableByPartitionIndex(p,&theTable,&tableSize);
    %else
                GetTableByMsgId(0, &theTable, &tableSize);
    %endif
    for (i = 0; i < tableSize; i++)
    {
        %<config.CaptureFileName>_type* theEntry = &theTable[i];
        uint32_t unitBytes = theEntry->sizeInChar;
        uint32_t requiredBytes = unitBytes * theEntry->capturesDesired;
        totalBytes += requiredBytes;
    }
    %if IsIndirect
            }
        }
    %endif
    return totalBytes;
}

// Add TSP to capture list, returns ASDID status code (ASDSTATUS_TSP_CAPTURE_OVERFLOW if no room in buffer for this request)
static ASDSTATUS TSP_CaptureListAdd(TspListAddReqType* theReq, uint32_t bufferBytes)
{
    // TODO - FAIL any add requests if a trigger is currently active
    // ... because the desired capture count changes here!

    ASDSTATUS retVal = ASDSTATUS_OK;
    uint32_t tableIndex = RTCID_INDEX(theReq->msgId);
    %<config.CaptureFileName>_type* theTable = 0;
    uint32_t tableSize;
    GetTableByMsgId(theReq->msgId, &theTable, &tableSize);
    if (tableIndex < tableSize)
    {
        %<config.CaptureFileName>_type* theEntry = &theTable[tableIndex];
        if (theEntry->captureSkipInterval == (uint32_t)-1)
        {
            // Ability to perform triggered capture is not available for this TSP
            retVal = ASDSTATUS_TSP_CAPTURE_THIS_OFF;
        }   
        else
        {
            // See if the buffer is big enough
            uint32_t previousWishes = theEntry->capturesDesired;
            theEntry->capturesDesired = theReq->captureCount;
                %if IsIndirect
                    // Mark this partition active to participate in required size
                    int partitionIndex = GetPartitionIndex(theReq->msgId);
                    partitionActiveFlag[partitionIndex] = 1;
                %endif
            uint32_t requiredBytes = GetRequiredCaptureBufferBytes();
            if (requiredBytes > bufferBytes)
            {
                // oops - buffer not big enough. Roll back to previous state
                // ... and return an error status
                retVal = ASDSTATUS_TSP_CAPTURE_OVERFLOW;
                theEntry->capturesDesired = previousWishes;
                %if IsIndirect
                    // Reset partition active state to the way it should be
                    bool isActive = IsPartitionActive(partitionIndex);
                    partitionActiveFlag[partitionIndex] = isActive;
                %endif
            }
            else
            {
                // Looks good. Record the desired skip interval as well
                theEntry->captureSkipInterval = theReq->skipInterval;
            }
        }
    }
    else
    {
        retVal = ASDSTATUS_TSP_BAD_STATE_ID;
    }
    return retVal;
}

static ASDSTATUS TSP_CaptureListSub(TspListSubReqType* theReq )
{
    ASDSTATUS retVal = ASDSTATUS_OK;
    uint32_t tableIndex = RTCID_INDEX(theReq->msgId);
    %<config.CaptureFileName>_type* theTable = 0;
    uint32_t tableSize;
    GetTableByMsgId(theReq->msgId, &theTable, &tableSize);

    if (tableIndex < tableSize)
    {
        %<config.CaptureFileName>_type* theEntry = &theTable[tableIndex];
        theEntry->capturesDesired = 0;

        %if IsIndirect
            // Clear the partition's active flag if this was the only capture
            int partitionIndex = GetPartitionIndex(theReq->msgId);
            bool isActive = IsPartitionActive(partitionIndex);
            partitionActiveFlag[partitionIndex] = isActive;
        %endif
    }    
    else
    {
        retVal = ASDSTATUS_TSP_BAD_STATE_ID;
    }
    return retVal;
}

static void TSP_CaptureListClear()
{
    uint32_t i;
    uint32_t tableSize;
    %<config.CaptureFileName>_type* theTable = 0;
    %if IsIndirect
        // Blindly clear every partition and its active flags
        int p;
        for (p=0; p<256; p++)
        {
            GetTableByPartitionIndex(p,&theTable,&tableSize);
            for (i=0; i < tableSize; i++)
            {
               %<config.CaptureFileName>_type* theEntry = &theTable[i];
               theEntry->capturesDesired = 0;
            }
            partitionActiveFlag[p] = 0;
        }
    %else
        GetTableByMsgId(0,&theTable,&tableSize);
        for (i=0; i < tableSize; i++)
        {
           %<config.CaptureFileName>_type* theEntry = &theTable[i];
           theEntry->capturesDesired = 0;
        }
    %endif
    
}

// Setup and trigger a capture to happen
static uint32_t TSP_CaptureTrigger(int8_t* captureBuffer, uint32_t bufferBytes)
{
    uint32_t i;
    uint32_t totalBytes = 0;
    uint32_t totalCaptures = 0;

    // ensure that no captures are happening
    triggerActive = false;
    
    %<config.CaptureFileName>_type* theTable = 0;
    uint32_t tableSize;
    %if IsIndirect
        int p;
        for (p=0; p<256; p++)
        {
            if (partitionActiveFlag[p])
            {
                GetTableByPartitionIndex(p,&theTable,&tableSize);
    %else
                GetTableByMsgId(0, &theTable, &tableSize);
    %endif
    
    // setup the capture address for each desired TSP
    for (i = 0; i < tableSize; i++)
    {
        %<config.CaptureFileName>_type* theEntry = &theTable[i];
        // Reset the counter - all captures will begin anew just before
        // exiting this function (when triggerActive becomes true)
        theEntry->capturesComplete = 0;
        theEntry->skipCounter = 0;
        
        // If captures are desired, set up where to put them.
        if (theEntry->capturesDesired > 0)
        {
            uint32_t unitBytes = theEntry->sizeInChar;
            uint32_t requiredBytes = unitBytes * theEntry->capturesDesired;
            if ((totalBytes + requiredBytes) <= bufferBytes)
            {
                totalCaptures += theEntry->capturesDesired;
                theEntry->captureAddress = captureBuffer + totalBytes;
                totalBytes += requiredBytes;                
            }
            else
            {
                // We should never run out of space because the ListAdd was
                // supposed to check that size was available. Nevertheless,
                // to handle this unexpected turn of events, kill the capture
                // by clobbering the capture address. No capture will happen.
                theEntry->captureAddress = 0; /*[safe]*/
            }
        }
        else
        {
            theEntry->captureAddress = 0;
        }
    }
    %if IsIndirect
            } // if partition active
        }   // loop partitions
    %endif

    // release the triggers to happen ASAP
    triggerActive = true;

    return totalCaptures;
}

// Notification worker that checks if triggered capture is done
static bool TSP_CaptureCheckComplete()
{
    uint32_t i;
    bool complete = true;
    if (triggerActive)
    {
        %<config.CaptureFileName>_type* theTable = 0;
        uint32_t tableSize;
        %if IsIndirect
            int p;
            for (p=0; (complete==true) && (p<256); p++)
            {
                if (partitionActiveFlag[p])
                {
                    GetTableByPartitionIndex(p,&theTable,&tableSize);
        %else
                    GetTableByMsgId(0, &theTable, &tableSize);
        %endif
        for (i=0; i < tableSize; i++)
        {
            %<config.CaptureFileName>_type* theEntry = &theTable[i];
            if (theEntry->capturesComplete < theEntry->capturesDesired
                /*[safe]*/&& theEntry->captureAddress)
            {
                // found one that still has work to do
                complete = false;
                break;
            }
        }
    %if IsIndirect
            } // if partition active
        }   // loop partitions
    %endif
    } // if trigger active
                
    // if the capture is complete, go ahead and shut the trigger off
    if (complete)
    {
        triggerActive = false;
    }
    return complete;
}

// Size of current capture data. Returns how many chars are required
// to contain the currently specified capture data.
static uint32_t TSP_CaptureRetrievalSize()
{
    uint32_t requiredBytes = GetRequiredCaptureBufferBytes();
    return requiredBytes;
}

/** Retrieve a single state value from the capture buffer filled by the last trigger.
 * @param tableIndex TSP index of the state which was previously
 *        added to the capture list.
 * @param captureIndex Sequential capture instance for this state (i.e. if the TSP
 *        was added to the list with a capture count of 3, then index values
 *        of 0, 1, and 2 are valid here.
 * @param offset Char offset into the captured state data resolved by the
 *        above tsp tableIndex and capture_instance index.
 * @param charsReturnedOut ptr to int to return the chars returned in buffer
 * @param bufferOut ptr to buffer to receive the data
 * @param bufferOutSize size of provided buffer
 * @return ASDSTATUS code: ASDSTATUS_OK,ASDSTATUS_TSP_BAD_STATE_ID,ASDSTATUS_TSP_CAPTURE_OVERFLOW  */
static ASDSTATUS TSP_CaptureRetrieve(TspRetrieveReqType* theReq,
        uint32_t* charsReturnedOut, int8_t* bufferOut, uint32_t bufferOutSize)
{
    ASDSTATUS retVal = ASDSTATUS_OK;
    uint32_t tableIndex = RTCID_INDEX(theReq->msgId);
    uint32_t offset = theReq->charOffset;
    uint32_t captureIndex = theReq->captureIndex;
    %<config.CaptureFileName>_type* theTable = 0;
    uint32_t tableSize;
    GetTableByMsgId(theReq->msgId, &theTable, &tableSize);
    if (tableIndex < tableSize)
    {
        %<config.CaptureFileName>_type* theEntry = &theTable[tableIndex];
        // Ensure the offset does not start off beyond the edge of data
        uint32_t tspSize = theEntry->sizeInChar;
        if ((theEntry->capturesComplete <= captureIndex) || (offset >= tspSize))
        {
            // There is no data that can even partially satisfy the request
            *charsReturnedOut = 0;
            retVal = ASDSTATUS_TSP_CAPTURE_OVERFLOW;
        }
        else
        {
            // seems ok - grab the data
            uint32_t unitBytes = theEntry->sizeInChar;
            uint32_t instanceOffset = unitBytes * captureIndex;
            int8_t* source = (theEntry->captureAddress + instanceOffset + offset);

            // Retrieve the minimum between these two choices: 
            //   max-possible-size vs. output-buffer-size
            uint32_t maxRetrievalSize = theEntry->sizeInChar - offset;
            if (maxRetrievalSize < bufferOutSize)
            {
                // Not enough data to fill buffer, so copy what *is* available
                memcpy(bufferOut, source, maxRetrievalSize);
                // ... and pad the rest with 0
                memset(bufferOut+maxRetrievalSize, 0, bufferOutSize-maxRetrievalSize);
                *charsReturnedOut = maxRetrievalSize;
            }
            else
            {
                // Fill the output buffer with data
                memcpy(bufferOut, source, bufferOutSize);
                *charsReturnedOut = bufferOutSize;
            }
        }
    }
    else
    {
        memset(bufferOut, 0, bufferOutSize);
        *charsReturnedOut = 0;
        retVal = ASDSTATUS_TSP_BAD_STATE_ID;
    }
    return retVal;
}

#endif // (1 == %<EnableAdvancedDefine>)
//-------------------------------------------------------------------------

/** Blindly write a state value directly into its source signal. The
 * integrity of the value is not reliable as audio processing can
 * happen during the middle of this write. (not recommended for really 
 * ANY signals... but requested by the teams).
 * @param tableIndex Numeric index of the state to blindly write
 * @param offset Char offset into the signal source memory
 * @param charsSentIn chars found in input buffer to copy/write into signal
 * @param bufferIn ptr to buffer containing data to copy/write into signal
 * @return ASDSTATUS code: ASDSTATUS_OK,ASDSTATUS_TSP_BAD_STATE_ID,ASDSTATUS_TSP_CAPTURE_OVERFLOW
 */
static ASDSTATUS TSP_BlindWrite(TspBlindWriteReqType* theReq,
    uint32_t charsSentIn, int8_t* bufferIn)
{
    ASDSTATUS retVal = ASDSTATUS_OK;
    uint32_t tableIndex = RTCID_INDEX(theReq->msgId);
    uint32_t offset = theReq->charOffset;
    %<config.CaptureFileName>_type* theTable = 0;
    uint32_t tableSize;
    GetTableByMsgId(theReq->msgId, &theTable, &tableSize);
    if (tableIndex < tableSize)
    {
        %<config.CaptureFileName>_type* theEntry = &theTable[tableIndex];
        
        uint32_t tspSize = theEntry->sizeInChar;
        // Ensure the TSP is not constant (i.e. read only)
        if (theEntry->rateFactor == 0)
        {
            // Impossible to write to this TSP
            retVal = ASDSTATUS_TSP_WRITE_IMPOSSIBLE;
        }
        // Ensure the offset does not start off beyond the edge of data
        else if (offset >= tspSize)
        {
            // There is no data that can even partially satisfy the request
            retVal = ASDSTATUS_TSP_CAPTURE_OVERFLOW;
        }
        else
        {
            // Write the signal *DIRECTLY* while it continues to change
            int8_t* blindWritePtr = (int8_t*)(theEntry->GetBlindReadPtr)();
            uint32_t maxWriteSize = theEntry->sizeInChar - offset;
            if (maxWriteSize < charsSentIn)
            {
                // Buffer not big enough, but copy as much as possible
                memcpy(&blindWritePtr[offset],bufferIn, maxWriteSize);
                // ... but also give an error. This request could not be
                // satisfied, and the caller should know.
                retVal = ASDSTATUS_TSP_CAPTURE_OVERFLOW;
            }
            else
            {
                // Copy/write the buffer with the provided data
                memcpy(&blindWritePtr[offset], bufferIn, charsSentIn);
            }
        }
    }
    else
    {
        // The TSP index was not contained in the table. Error.
        retVal = ASDSTATUS_TSP_BAD_STATE_ID;
    }
    return retVal;
}

/** Blindly read a state value directly from its source signal. The
 * integrity of the value is not reliable as audio processing can
 * happen during the middle of this read. (not recommended for multi-
 * word signals).
 * @param tableIndex Numeric index of the state to blindly read
 * @param offset Char offset into the signal source memory
 * @param charsReturnedOut ptr to int to return the chars returned in buffer
 * @param bufferOut ptr to buffer to receive the data
 * @param bufferOutSize size of provided buffer
 * @return ASDSTATUS code: ASDSTATUS_OK,ASDSTATUS_TSP_BAD_STATE_ID,ASDSTATUS_TSP_CAPTURE_OVERFLOW
 */
static ASDSTATUS TSP_BlindRead(TspBlindReadReqType* theReq,
    uint32_t* charsReturnedOut, int8_t* bufferOut, uint32_t bufferOutSize)
{
    ASDSTATUS retVal = ASDSTATUS_OK;
    uint32_t tableIndex = RTCID_INDEX(theReq->msgId);
    uint32_t offset = theReq->charOffset;
    %<config.CaptureFileName>_type* theTable = 0;
    uint32_t tableSize;
    GetTableByMsgId(theReq->msgId, &theTable, &tableSize);
    if (tableIndex < tableSize)
    {
        %<config.CaptureFileName>_type* theEntry = &theTable[tableIndex];
        
        // Ensure the offset does not start off beyond the edge of data
        uint32_t tspSize = theEntry->sizeInChar;
        if (offset >= tspSize)
        {
            // There is no data that can even partially satisfy the request
            *charsReturnedOut = 0;
            retVal = ASDSTATUS_TSP_CAPTURE_OVERFLOW;
        }
        else
        {
            // Read the signal *DIRECTLY* while it continues to change
            // Retrieve the minimum between these two choices: 
            //   max-possible-size vs. output-buffer-size
            const int8_t* blindReadPtr = (int8_t*)(theEntry->GetBlindReadPtr)();
            uint32_t maxRetrievalSize = theEntry->sizeInChar - offset;
            if (maxRetrievalSize < bufferOutSize)
            {
                // Not enough data to fill buffer, so copy what *is* available
                memcpy(bufferOut, &blindReadPtr[offset], maxRetrievalSize);
                // ... and pad the rest with 0
                memset(bufferOut+maxRetrievalSize, 0, bufferOutSize-maxRetrievalSize);
                *charsReturnedOut = maxRetrievalSize;
            }
            else
            {
                // Fill the output buffer with data
                memcpy(bufferOut, &blindReadPtr[offset], bufferOutSize);
                *charsReturnedOut = bufferOutSize;
            }
        }
    }
    else
    {
        memset(bufferOut, 0, bufferOutSize);
        *charsReturnedOut = 0;
        retVal = ASDSTATUS_TSP_BAD_STATE_ID;
    }
    return retVal;
}

/** Services TSP Notification - polling for request completion as well as
 * creating notification message payloads upon completion.
 * NOTE: the same thread that services RTC requests must be responsible for
 * servicing notifications as well. The two routines must not operate at the
 * same time or race conditions can lead to faulty operation.
 *  @param pNtfBuf Empty buffer read to receive a notification message.
 *  @param ntfBufSize Size of buffer in bytes.
 *  @returns Number of bytes used in the provided notification buffer (pNtfBuf).
 *          Return Value | Meaning
 *           ----------- | :--------
 *               0       | All done. No use in calling this again until audio processing happens.
 *             > 0       | A notification was returned with this many bytes used in buffer.
 *             < 0       | Error: buffer is too small. This is how many bytes are needed for the buffer (in negative form)
 * @ingroup rtc
 * @see AsdIdStatus_type for format of each notification message's header
 */
int %<ModelPrefix()>ServiceTspNotification(void* pNtfBuf, uint32_t ntfBufSize)
{
    // Note assumption of single service thread that handles both
    // RTC message requests and this notification handler.

    int retVal = 0;

#if (1 == %<EnableAdvancedDefine>)
    // Calculate the table size - subtract one for the End Sentinel
    // Poll for completion if the current request is still pending
    if (triggerRequestSequence != triggerCompletedSequence)
    {
        // Not done last we checked. Check again now and
        // if the capture is done, capture status
        if (TSP_CaptureCheckComplete())
        {
            triggerCompletedSequence = triggerRequestSequence;
            triggerCompletedCallerHandle = triggerRequestCallerHandle;
            triggerCompletedTotalCaptureCount = triggerRequestTotalCaptureCount;
        }
    }

    // Attempt to send notification if a completed sequence is waiting to send
    if (triggerLastSentSequence != triggerCompletedSequence)
    {
        uint32_t requiredNtfBytes = sizeof(TspCaptureNtfType);
        bool ntfBufOK = (ntfBufSize >= requiredNtfBytes && pNtfBuf);
        if (ntfBufOK)
        {
            TspCaptureNtfType* statusHdr = (TspCaptureNtfType*)pNtfBuf;
            statusHdr->msgId = %<ModelPrefix()>TSPID_CAPTURE;
            statusHdr->callerHandle = triggerCompletedCallerHandle;
            statusHdr->statusCode = ASDSTATUS_RTC_NOTIFICATION;
            statusHdr->totalCaptureCount = triggerCompletedTotalCaptureCount;
            retVal = requiredNtfBytes;
            triggerLastSentSequence = triggerCompletedSequence;
        }
        else
        {
            // indicate how many bytes should have been given - just make it negative
            retVal = (0 - requiredNtfBytes);
        }
    }
#endif // (1 == %<EnableAdvancedDefine>)

    return retVal;
}
