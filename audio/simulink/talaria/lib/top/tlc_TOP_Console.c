%% TLC code generation file - hand written source
%% INCLUDED BY: TOP_MEX.tlc


//-----------------------------------------------------------
// local prototypes
static TopConsoleElement* GetConsoleRecord(AsyncConsoleIterator iteratorIn);
static const char* GetName(AsyncConsoleIterator iterator);
static TopConsoleElement* GetTopElementById(uint32_t topToken);
static TopConsoleElement* GetTopElementByName(const char* name);
static int TopConsoleBlockingInputWriteMessage(void* messageBuffer, uint32_t bufferSize, FILE* streamOut);
static int TopConsoleBlockingInputReadMessage(void* messageBuffer, uint32_t bufferSize, FILE* streamOut);
static int TopConsoleBlockingInputMessage(void* messageBuffer, uint32_t bufferSize, FILE* streamOut, int isWrite);


//-----------------------------------------------------------
// Define TOP Console handlers
//-----------------------------------------------------------
static int TopConsolePrintMessage(int isRequest, void* msgBuffer, uint32_t bufferSize, FILE* streamOut);
%<ModelPrefix()>AsyncConsoleHandler %<ModelPrefix()>TopWriteConsoleHandler =
{
    "TOPWRITE",
    &GetName,
    &TopConsolePrintMessage,
    &TopConsoleBlockingInputWriteMessage
};

%<ModelPrefix()>AsyncConsoleHandler %<ModelPrefix()>TopReadConsoleHandler =
{
    "TOPREAD",
    &GetName,
    &TopConsolePrintMessage,
    &TopConsoleBlockingInputReadMessage
};

//-----------------------------------------------------------
static TopConsoleElement* GetConsoleRecord(AsyncConsoleIterator iteratorIn)
{
    if ((iteratorIn.model < 0) || (iteratorIn.model >= numAsyncConsoleRoots))
    {
        return 0; // model out of range
    }
    AsyncConsoleRoot* theRoot = asyncConsoleRoots[iteratorIn.model];
    if ((0 == theRoot->top_table) || (iteratorIn.index < 0) || (iteratorIn.index >= *theRoot->top_count))
    {
        return 0; // index out of range for specified model
    }
    TopConsoleElement* theTable = *(TopConsoleElement**)theRoot->top_table;
    return &theTable[iteratorIn.index];
}

//-----------------------------------------------------------
static const char* GetName(AsyncConsoleIterator iterator)
{
    const char* theName = 0;
    TopConsoleElement* aRecord = GetConsoleRecord(iterator);
    if (aRecord)
    {
        theName = aRecord->name;
    }
    return theName;
}

//-----------------------------------------------------------
static TopConsoleElement* GetTopElementById(uint32_t topToken)
{
    AsyncConsoleIterator iterator = {0};
    TopConsoleElement* aRecord;

    // Loop through all records
    int m;
    for (m = 0; m < numAsyncConsoleRoots; m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for(aRecord = GetConsoleRecord(iterator); aRecord;
            aRecord = GetConsoleRecord(iterator))
        {
            if (topToken == aRecord->readId || topToken == aRecord->writeId)
            {
                return aRecord;
            }
            
            // move on to next tunevar
            iterator.index++;
        }
    }
    return 0; // not found
}

//-----------------------------------------------------------
static TopConsoleElement* GetTopElementByName(const char* name)
{
    AsyncConsoleIterator iterator = {0};
    TopConsoleElement* aRecord;

    // Loop through all records
    int m;
    for (m = 0; m < numAsyncConsoleRoots; m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for(aRecord = GetConsoleRecord(iterator); aRecord;
            aRecord = GetConsoleRecord(iterator))
        {
            if (0 == strcmp(name, aRecord->name))
            {
                return aRecord;
            }
            
            // move on to next tunevar
            iterator.index++;
        }
    }
    return 0; // not found
}

/** TopConsolePrintMessage Parses and prints (via stdout) details of a
 * given binary asynchronous message.
 * @param isRequest : boolean value set to true to inform the parser that
 * the message is known to be a request (i.e. to be sent into the model).
 * Set to false for any traffic received back from the model (i.e. responses
 * and notifications)
 * @param msgBuffer is a pointer to the binary message
 * @param bufferSize the size in char of the msg buffer (not currently used)
 * @returns true (1) if message is RTC and so printed or 0 if not an RTC
 * @ingroup rtc
 **/
static int TopConsolePrintMessage(int isRequest, void* msgBuffer, uint32_t bufferSize, FILE* streamOut)
{
    AsdIdStatus_type* statMsg = (AsdIdStatus_type*)msgBuffer;
    uint32_t msgId = statMsg->msgId;
    if (0 == ASDID_IS_TOP(msgId))
    {
        return 0;
    }
    TopConsoleElement* theElem = GetTopElementById(msgId);
    if (!theElem)
    {
        fprintf(streamOut,"*** TOP 0x%08x not recognized\n", msgId);
    }

    // There are four types of TOP messages- read vs write and req vs rsp:
    int isWrite = TOPID_IS_WRITE(msgId);
    const char* readWriteStr = isWrite ? "Write" : "Read";
    if (isRequest)
    {
        // cast message buffer to top request format and dump it
        TOP_Request_type* topReqMsg = (TOP_Request_type*)msgBuffer;
        fprintf(streamOut,"TOP %s REQ\n   %s (0x%08x)\n",
                readWriteStr, theElem->name, topReqMsg->topId);
        fprintf(streamOut,"   callerHandle: %d\n", topReqMsg->callerHandle);
        fprintf(streamOut,"   offset: %d\n", topReqMsg->offset);
        fprintf(streamOut,"   byteCount: %d\n", topReqMsg->byteCount);
        // write requests contain data values
        if (isWrite)
        {
            uint32_t typeSize = %<ModelPrefix()>GetDTypeSize(theElem->dtypeId);
            uint32_t reqWidth = topReqMsg->byteCount / typeSize;
            fprintf(streamOut,"   (%d bytes of %s data, %d elements)\n",
                    topReqMsg->byteCount, %<ModelPrefix()>GetDTypeStr(theElem->dtypeId), reqWidth);
            %<ModelPrefix()>PrintTypedArray(theElem->dtypeId, topReqMsg->data, reqWidth, streamOut);
            fprintf(streamOut,"\n<end of data>\n");
        }
    }
    else // otherwise, it's a response
    {
        // cast message buffer to status message (already done) and dump it
        fprintf(streamOut,"TOP %s RSP\n   %s (0x%08x)\n",
                readWriteStr, theElem->name, statMsg->msgId);
        fprintf(streamOut,"   callerHandle: %d\n", statMsg->callerHandle);
        fprintf(streamOut,"   statusCode: %d\n", statMsg->statusCode);
        // read responses contain data values
        if (false == isWrite)
        {
            // ... and TOP read responses contain byteCount for our convenience
            TOP_ReadResponse_type* topRSP = (TOP_ReadResponse_type*)msgBuffer;
            // In "normal" operation, TOP data read operations are rare other
            // than as a passthrough request from external tuning tools.
            
            uint32_t dataBytes = topRSP->byteCount;
            fprintf(streamOut,"   byteCount: %d\n", dataBytes);
            uint32_t actualDataBytes = bufferSize - sizeof(TOP_ReadResponse_type);
            if (dataBytes != actualDataBytes)
            {
                fprintf(streamOut,"   **ALERT!** payload byte count %d does not match actual data bytes %d\n",
                        dataBytes, actualDataBytes);
            }
            uint32_t typeSize = %<ModelPrefix()>GetDTypeSize(theElem->dtypeId);
            uint32_t rspWidth = dataBytes / typeSize;
            fprintf(streamOut,"   (%d bytes of %s data, %d elements)\n",
                    dataBytes, %<ModelPrefix()>GetDTypeStr(theElem->dtypeId), rspWidth);
            %<ModelPrefix()>PrintTypedArray(theElem->dtypeId, topRSP->data, rspWidth, streamOut);
            fprintf(streamOut,"\n<end of data>\n");
        }
    }

    fflush(streamOut);
    return 1; // message printed
}

static int TopConsoleBlockingInputWriteMessage(void* messageBuffer, uint32_t bufferSize,FILE* streamOut)
{
    return TopConsoleBlockingInputMessage(messageBuffer,bufferSize,streamOut,1);
}
static int TopConsoleBlockingInputReadMessage(void* messageBuffer, uint32_t bufferSize, FILE* streamOut)
{
    return TopConsoleBlockingInputMessage(messageBuffer,bufferSize,streamOut,0);
}

/** TopConsoleBlockingInputMessage Reads from stdin to generate a packed
 * binary request message of either TOP READ or TOP WRITE.
 * @param messageBuffer pointer to buffer that receives the binary message
 * @param bufferSize size in char of the message buffer
 * @param streamOut file stream to receive prompts
 * @param isWrite boolean true if write, otherwise operation is read
 * @returns size in char of the created message, or "-1" if instead the
 * message buffer contains a ! command.
 **/
static int TopConsoleBlockingInputMessage(void* messageBuffer, uint32_t bufferSize, FILE* streamOut, int isWrite)
{
    enum TOP_PAY_PART { TPP_TOP, TPP_CALLER_HANDLE, TPP_OFFSET, TPP_BYTE_COUNT, TPP_DATA } part = TPP_TOP;
    TopConsoleElement* currentElement = 0;
    int currentDataIndex = 0;
    int dataElementsToInput = 0;
    uint32_t typeSize = 0;
    int active = 1;
    int sizeOut = 0;
    TOP_Request_type* topRequest = (TOP_Request_type*)messageBuffer;
    while (active)
    {
        const char* word = %<ModelPrefix()>AcquireWordFromConsole(currentElement == 0);
        if (*word == '!')
        {
            strncpy(messageBuffer, word, bufferSize);
            return -1;
        }
        if (0 == currentElement)
        {
            // need a message. Search for it.
            currentElement = GetTopElementByName(word);
            if (0 == currentElement)
            {
                // unrecognized message- return negative length to caller
                int len = (int)strlen(word);
                return -len;
                //active = 0;
            }
            else
            {
                // setup the msgid
                if (isWrite)
                {
                    topRequest->topId = currentElement->writeId;
                }
                else
                {
                    topRequest->topId = currentElement->readId;
                }
                // start with the first payload element
                part = TPP_CALLER_HANDLE;
                currentDataIndex = 0;
            }
        }
        else
        { // step through the rest of the pieces of the payload
            switch (part)
            {
                case TPP_CALLER_HANDLE:
                    topRequest->callerHandle = atoi(word);
                    part = TPP_OFFSET;
                    break;
                case TPP_OFFSET:
                    topRequest->offset = atoi(word);
                    part = TPP_BYTE_COUNT;
                    break;
                case TPP_BYTE_COUNT:
                dataElementsToInput = atoi(word);
                if (dataElementsToInput == 0)
                {
                    if (streamOut) fprintf(streamOut,"\n** Zero elements? Try again or request !HELP");
                }
                else
                {
                    typeSize = %<ModelPrefix()>GetDTypeSize(currentElement->dtypeId);
                    topRequest->byteCount = typeSize * dataElementsToInput;
                    if (isWrite)
                    {
                        if (streamOut) fprintf(streamOut,"\nPacking %d bytes of data:", topRequest->byteCount);
                    }
                    else
                    {
                        if (streamOut) fprintf(streamOut,"\nRequesting %d bytes of data.\n",
                            topRequest->byteCount);
                    }

                    currentDataIndex = 0;
                    part = TPP_DATA;
                }
                break;
                case TPP_DATA:
                    %<ModelPrefix()>StringToNumericArrayElement(topRequest->data, currentDataIndex, currentElement->dtypeId, word);
                    currentDataIndex++;
                    break;
            }
                    
        }
        // prompt for the next part
        switch (part)
        {
            case TPP_CALLER_HANDLE:
                if (streamOut) fprintf(streamOut,"\n   callerHandle:");
                if (streamOut) fflush(stdout);
                break;
            case TPP_OFFSET:
                if (streamOut) fprintf(streamOut,"\n   offset:");
                if (streamOut) fflush(stdout);
                break;
            case TPP_BYTE_COUNT:
                if (streamOut) fprintf(streamOut,"\n   Number of values (count of values instead of bytes):");
                if (streamOut) fflush(stdout);
                break;
            case TPP_DATA:
                if (false == isWrite || currentDataIndex >= dataElementsToInput)
                {
                    // done entering data!
                    currentElement = 0;
                    sizeOut = sizeof(TopConsoleElement) + isWrite ?topRequest->byteCount : 0;
                    active = 0;
                }
                else
                {
                    if (streamOut) fprintf(streamOut,"\n   Data value %d of %d: ",currentDataIndex+1,dataElementsToInput);
                    if (streamOut) fprintf(streamOut," ");
                }
                    
        }
    }

    return sizeOut;
}
