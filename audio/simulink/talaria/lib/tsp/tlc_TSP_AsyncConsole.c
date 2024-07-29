%% Hand written C code for the TSP asynchronous console
%% included by TSP_Capture.tlc


// TSP console element counts for all payload types
#define TspConNtfCount_TspListAdd      0
#define TspConReqCount_TspListAdd      3
#define TspConRspCount_TspListAdd      2
#define TspConNtfCount_TspListSub      0
#define TspConReqCount_TspListSub      1
#define TspConRspCount_TspListSub      2
#define TspConNtfCount_TspListClear    0
#define TspConReqCount_TspListClear    1
#define TspConRspCount_TspListClear    2
#define TspConNtfCount_TspRetrieve     0
#define TspConReqCount_TspRetrieve     4
#define TspConRspCount_TspRetrieve     3
#define TspConNtfCount_TspBlindRead    0
#define TspConReqCount_TspBlindRead    3
#define TspConRspCount_TspBlindRead    3
#define TspConNtfCount_TspCapture      3
#define TspConReqCount_TspCapture      1
#define TspConRspCount_TspCapture      2

// struct to hold message payload element definitions
typedef struct
{
    const char* name;
    SimulinkDatatypeId dtypeId;
    uint32_t width;
    uint32_t charOffset;
} TspConsolePayloadElement;

// struct to hold control message definitions
typedef struct
{
    const char* messageType;
    int32_t dumpBytesAfterSize;
    int groupCommand;
    TspConsolePayloadElement* reqElems;
    TspConsolePayloadElement* rspElems;
    TspConsolePayloadElement* ntfElems;
    uint32_t reqCount;
    uint32_t rspCount;
    uint32_t ntfCount;
} TspConsoleMessage;

static TspConsolePayloadElement TspConNtfElem_TspListAdd[] = { 0 };

static TspConsolePayloadElement TspConReqElem_TspListAdd[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspListAddReqType, callerHandle) },
  { "captureCount", TSS_UINT32, 1, offsetof(TspListAddReqType, captureCount) },
  { "skipInterval", TSS_UINT32, 1, offsetof(TspListAddReqType, skipInterval) },
};

static TspConsolePayloadElement TspConRspElem_TspListAdd[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspListAddRspType, callerHandle) },
  { "statusCode", TSS_INT32, 1, offsetof(TspListAddRspType, statusCode) },
};

static TspConsolePayloadElement TspConNtfElem_TspListSub[] = { 0 };

static TspConsolePayloadElement TspConReqElem_TspListSub[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspListSubReqType, callerHandle) },
};

static TspConsolePayloadElement TspConRspElem_TspListSub[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspListSubRspType, callerHandle) },
  { "statusCode", TSS_INT32, 1, offsetof(TspListSubRspType, statusCode) },
};

static TspConsolePayloadElement TspConNtfElem_TspListClear[] = { 0 };

static TspConsolePayloadElement TspConReqElem_TspListClear[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspListClearReqType, callerHandle) },
};

static TspConsolePayloadElement TspConRspElem_TspListClear[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspListClearRspType, callerHandle) },
  { "statusCode", TSS_INT32, 1, offsetof(TspListClearRspType, statusCode) },
};

static TspConsolePayloadElement TspConNtfElem_TspRetrieve[] = { 0 };

static TspConsolePayloadElement TspConReqElem_TspRetrieve[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspRetrieveReqType, callerHandle) },
  { "captureIndex", TSS_UINT32, 1, offsetof(TspRetrieveReqType, captureIndex) },
  { "charOffset", TSS_UINT32, 1, offsetof(TspRetrieveReqType, charOffset) },
  { "charCount", TSS_UINT32, 1, offsetof(TspRetrieveReqType, charCount) },
};

static TspConsolePayloadElement TspConRspElem_TspRetrieve[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspRetrieveRspType, callerHandle) },
  { "statusCode", TSS_INT32, 1, offsetof(TspRetrieveRspType, statusCode) },
  { "charCount", TSS_UINT32, 1, offsetof(TspRetrieveRspType, charCount) },
};

static TspConsolePayloadElement TspConNtfElem_TspBlindRead[] = { 0 };

static TspConsolePayloadElement TspConReqElem_TspBlindRead[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspBlindReadReqType, callerHandle) },
  { "charOffset", TSS_UINT32, 1, offsetof(TspBlindReadReqType, charOffset) },
  { "charCount", TSS_UINT32, 1, offsetof(TspBlindReadReqType, charCount) },
};

static TspConsolePayloadElement TspConRspElem_TspBlindRead[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspBlindReadRspType, callerHandle) },
  { "statusCode", TSS_INT32, 1, offsetof(TspBlindReadRspType, statusCode) },
  { "charCount", TSS_UINT32, 1, offsetof(TspBlindReadRspType, charCount) },
};

static TspConsolePayloadElement TspConNtfElem_TspCapture[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspCaptureNtfType, callerHandle) },
  { "statusCode", TSS_INT32, 1, offsetof(TspCaptureNtfType, statusCode) },
  { "totalCaptureCount", TSS_UINT32, 1, offsetof(TspCaptureNtfType, totalCaptureCount) },
};

static TspConsolePayloadElement TspConReqElem_TspCapture[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspCaptureReqType, callerHandle) },
};

static TspConsolePayloadElement TspConRspElem_TspCapture[] =
{
  { "callerHandle", TSS_UINT32, 1, offsetof(TspCaptureRspType, callerHandle) },
  { "statusCode", TSS_INT32, 1, offsetof(TspCaptureRspType, statusCode) },
};

// TSP Console message payload information structures
static TspConsoleMessage tspConsoleMessageTable[] =
{
    {
      "TSP List Add", -1, 0,
      TspConReqElem_TspListAdd,
      TspConRspElem_TspListAdd,
      TspConNtfElem_TspListAdd,
      TspConReqCount_TspListAdd,
      TspConRspCount_TspListAdd,
      TspConNtfCount_TspListAdd,
    },
    {
      "TSP List Subtract", -1, 0,
      TspConReqElem_TspListSub,
      TspConRspElem_TspListSub,
      TspConNtfElem_TspListSub,
      TspConReqCount_TspListSub,
      TspConRspCount_TspListSub,
      TspConNtfCount_TspListSub,
    },
    {
      "TSP Retrieve Captured Value", sizeof(TspRetrieveRspType), 0,
      TspConReqElem_TspRetrieve,
      TspConRspElem_TspRetrieve,
      TspConNtfElem_TspRetrieve,
      TspConReqCount_TspRetrieve,
      TspConRspCount_TspRetrieve,
      TspConNtfCount_TspRetrieve,
    },
    {
      "TSP Blind Read", sizeof(TspBlindReadRspType), 0,
      TspConReqElem_TspBlindRead,
      TspConRspElem_TspBlindRead,
      TspConNtfElem_TspBlindRead,
      TspConReqCount_TspBlindRead,
      TspConRspCount_TspBlindRead,
      TspConNtfCount_TspBlindRead,
    },
    {
      "TSP List Clear", -1, 1,
      TspConReqElem_TspListClear,
      TspConRspElem_TspListClear,
      TspConNtfElem_TspListClear,
      TspConReqCount_TspListClear,
      TspConRspCount_TspListClear,
      TspConNtfCount_TspListClear,
    },
    {
      "TSP Trigger Capture", -1, 1,
      TspConReqElem_TspCapture,
      TspConRspElem_TspCapture,
      TspConNtfElem_TspCapture,
      TspConReqCount_TspCapture,
      TspConRspCount_TspCapture,
      TspConNtfCount_TspCapture,
    }
};
static const int tspConsoleMessageTableSize = sizeof(tspConsoleMessageTable) / sizeof(tspConsoleMessageTable[0]);

// local prototypes
static TspConsoleAccessType* GetConsoleRecord(AsyncConsoleIterator iteratorIn);
static const char* GetName(AsyncConsoleIterator iterator);
static TspConsoleAccessType* GetTspAccessById(uint32_t messageId);
static TspConsoleAccessType* GetTspAccessByName(const char* name);
static int TspConsolePrintMessage(int isRequest, void* msgBuffer, uint32_t bufferSize,
    FILE* streamOut);
static int TspConsoleBlockingInputMessage(void* messageBuffer, uint32_t bufferSize, FILE* streamOut);

//-----------------------------------------------------------
// Define TSP Console handler
//-----------------------------------------------------------

%<ModelPrefix()>AsyncConsoleHandler %<ModelPrefix()>TspConsoleHandler =
{
  "TSP",
  &GetName,
  &TspConsolePrintMessage,
  &TspConsoleBlockingInputMessage
};


//-----------------------------------------------------------
static TspConsoleAccessType* GetConsoleRecord(AsyncConsoleIterator iteratorIn)
{
    if ((iteratorIn.model < 0) || (iteratorIn.model >= numAsyncConsoleRoots))
    {
        return 0; // model out of range
    }
    AsyncConsoleRoot* theRoot = asyncConsoleRoots[iteratorIn.model];
    if ((0 == theRoot->tsp_table) || (iteratorIn.index < 0) || (iteratorIn.index >= *theRoot->tsp_count))
    {
        return 0; // index out of range for specified model
    }
    TspConsoleAccessType* theTable = *(TspConsoleAccessType**)theRoot->tsp_table;
    return &theTable[iteratorIn.index];
}

//-----------------------------------------------------------
static const char* GetName(AsyncConsoleIterator iterator)
{
    const char* theName = 0;
    TspConsoleAccessType* aRecord = GetConsoleRecord(iterator);
    if (aRecord)
    {
        theName = aRecord->name;
    }
    return theName;
}

//-----------------------------------------------------------
static TspConsoleAccessType* GetTspAccessById(uint32_t messageId)
{
    const int numCommands = sizeof(TspConsoleAccessTable[0].msgId) / 
        sizeof(TspConsoleAccessTable[0].msgId[0]);
    AsyncConsoleIterator iterator = {0};
    TspConsoleAccessType* aRecord;

    // Loop through all records
    int m,j;
    for (m = 0; m < numAsyncConsoleRoots; m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for(aRecord = GetConsoleRecord(iterator); aRecord;
            aRecord = GetConsoleRecord(iterator))
        {
            for (j = 0; j < numCommands; j++)
            {
                if (messageId == aRecord->msgId[j])
                {
                    return aRecord;
                }
            }
            
            // move on to next tunevar
            iterator.index++;
        }
    }
    return 0; // not found
}

//-----------------------------------------------------------
static TspConsoleAccessType* GetTspAccessByName(const char* name)
{
    AsyncConsoleIterator iterator = {0};
    TspConsoleAccessType* aRecord;

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


/** TspConsolePrintMessage Parses and prints (via stdout) details of a
 * given binary asynchronous message that represents TSP traffic.
 * @param isRequest : boolean value set to 1 to inform the parser that
 * the message is known to be a request (i.e. to be sent into the model).
 * Set to 0 for any traffic received back from the model (i.e. responses
 * and notifications)
 * @param msgBuffer is a pointer to the binary message
 * @param bufferSize the size in char of the msg buffer (not currently used)
 * @returns 1 (1) if message is TSP and so printed or 0 if not an TSP
 * @ingroup TSP
 **/
static int TspConsolePrintMessage(int isRequest, void* msgBuffer, uint32_t bufferSize,
    FILE* streamOut)
{
    AsdIdStatus_type* statMsg = (AsdIdStatus_type*)msgBuffer;
    if (0 == ASDID_IS_TSP(statMsg->msgId))
    {
        return 0;
    }

    uint32_t tspCmd = RTCID_INFO(statMsg->msgId);

    // Assume the command IDs are in the same order as our carefully crafted table
    // (Add, sub, clear, capture, retrieve, blindread)
    int messageTableIndex = tspCmd - TSP_COMMAND_LIST_ADD;
    if (messageTableIndex >= tspConsoleMessageTableSize)
    {
        printf("*** Message Id 0x%08x is TSP, but with unrecognized command\n", statMsg->msgId);
        return 0;
    }

    TspConsoleMessage* theMsg = &tspConsoleMessageTable[messageTableIndex];
    TspConsoleAccessType* tspAccess = GetTspAccessById(statMsg->msgId);
    if (0 == tspAccess && 0 == theMsg->groupCommand)
    {
        printf("*** Message Id 0x%08x is TSP, but with unrecognized TSP index\n", statMsg->msgId);
        return 0;
    }

    const char* payloadTypeStr;
    TspConsolePayloadElement* elements;
    uint32_t elementCount;
    int isResponse = 0;
    if (isRequest)
    {
        payloadTypeStr = "REQ";
        elements = theMsg->reqElems;
        elementCount = theMsg->reqCount;
    }
    else if (statMsg->statusCode == ASDSTATUS_RTC_NOTIFICATION)
    {
        payloadTypeStr = "NTF";
        elements = theMsg->ntfElems;
        elementCount = theMsg->ntfCount;
    }
    else
    {
        payloadTypeStr = "RSP";
        elements = theMsg->rspElems;
        elementCount = theMsg->rspCount;
        isResponse = 1;
    }

    fprintf(streamOut, "%s: %s \n   (for %s)\n", payloadTypeStr, theMsg->messageType,
        (theMsg->groupCommand) ? "the TSP list" : tspAccess->name);
    uint32_t i;
    for (i = 0; i < elementCount; i++)
    {
        TspConsolePayloadElement* anElem = &elements[i];
        void* valPtr = &((char*)msgBuffer)[anElem->charOffset];
        fprintf(streamOut, "   %s:", anElem->name);
        %<ModelPrefix()>PrintTypedArray(anElem->dtypeId, valPtr, anElem->width, streamOut);
        fprintf(streamOut, "\n");
    }

    // Dump out bytes if a response that does that kind of thing
    if (isResponse && theMsg->dumpBytesAfterSize > 0 &&
        bufferSize > (uint32_T)theMsg->dumpBytesAfterSize)
    {
        // We know how many bytes were returned and the datatype of the
        // elements in the array. Based on this, we can determine the
        // number of elements returned.
        // In "normal" operation, TOP data is not externally parsed like
        // this. TOP data exists only to be used by the signal processing
        // logic and it is created from the translation engine and database
        // build components.
        uint32_t dataBytes = bufferSize - theMsg->dumpBytesAfterSize;
        uint32_t typeSize = %<ModelPrefix()>GetDTypeSize(tspAccess->dtypeId);
        uint32_t rspWidth = dataBytes / typeSize;
        fprintf(streamOut, "   (%d bytes of %s data, %d elements)\n",
            dataBytes, %<ModelPrefix()>GetDTypeStr(tspAccess->dtypeId), rspWidth);
        void* dataPtr = &((char*)msgBuffer)[theMsg->dumpBytesAfterSize];
        %<ModelPrefix()>PrintTypedArray(tspAccess->dtypeId, dataPtr, rspWidth, streamOut);
        fprintf(streamOut, "\n<end of data>\n");
    }

    fflush(streamOut);
    return 1;                            // message printed
}

/** TspConsoleBlockingInputMessage Reads from stdin to generate a packed
 * binary request message.
 * @param messageBuffer pointer to buffer that receives the binary message
 * @param bufferSize size in char of the message buffer
 * @param streamOut file stream to receive prompts
 * @returns size in char of the created message, or "-1" if instead the
 * message buffer contains a ! command.
 **/
static int TspConsoleBlockingInputMessage(void* messageBuffer, uint32_t bufferSize, FILE* streamOut)
{
    const char* symbols = "+-?";
    const int cmdOffset = TSP_COMMAND_LIST_ADD;
    TspConsoleAccessType* tspAccess = 0;
    TspConsoleMessage* currentMessage = 0;
    int currentElementIndex = 0;
    int signalItemNum = -1;
    int active = 1;
    int sizeOut = 0;
    int tokenLen = 0;
    while (active)
    {
        char* token = %<ModelPrefix()>AcquireWordFromConsole(currentMessage == 0);
        if (*token == '!')
        {
            strncpy(messageBuffer, token, bufferSize);
            return -1;
        }
        tokenLen = (int)strlen(token);

        if (0 == currentMessage)
        {
            // This is the "first" token to tell us what type of payload to build
            // Break apart the token to separate possible terminating symbols
            uint32_t msgId;
            int tspAccessCommandTableIndex = -1;
            int endSymbolsIndex = (int)strcspn(token, symbols);
            if (*token == '+')
            {
                // Starting with + indicates capture trigger
                currentMessage = &tspConsoleMessageTable[TSP_COMMAND_CAPTURE - cmdOffset];
                msgId = %<ModelPrefix()>TSPID_CAPTURE;
            }
            else if (*token == '-')
            {
                // Starting with - indicates clear list
                currentMessage = &tspConsoleMessageTable[TSP_COMMAND_LIST_CLEAR - cmdOffset];
                msgId = %<ModelPrefix()>TSPID_CLEAR;
            }
            else if (endSymbolsIndex < tokenLen)
            {
                if (token[endSymbolsIndex] == '+')
                {
                    // A + after the name indicates a list add
                    // setup logic below to search for TSP for this command
                    tspAccessCommandTableIndex = TSP_COMMAND_LIST_ADD - cmdOffset;
                }
                else if (token[endSymbolsIndex] == '-')
                {
                    // A - after the name indicates a list subtract
                    // setup logic below to search for TSP for this command
                    tspAccessCommandTableIndex = TSP_COMMAND_LIST_SUB - cmdOffset;
                }
                else if (token[endSymbolsIndex] == '?')
                {
                    // A ? after the name indicates a capture retrieval
                    // setup logic below to search for TSP for this command
                    tspAccessCommandTableIndex = TSP_COMMAND_RETRIEVE - cmdOffset;
                }
            }
            else
            {
                // No symbols - this should be a blind read
                // setup logic below to search for TSP for this command
                tspAccessCommandTableIndex = TSP_COMMAND_BLIND_READ - cmdOffset;
            }
            if (tspAccessCommandTableIndex >= 0)
            {
                token[endSymbolsIndex] = 0; // terminate name at the symbol
                tspAccess = GetTspAccessByName(token); // search for TSP
                if (0 == tspAccess)
                {
                    // not found, return negative length
                    return -tokenLen;
                }
                // found it - we're creating a TSP-access message:
                currentMessage = &tspConsoleMessageTable[tspAccessCommandTableIndex];
                msgId = tspAccess->msgId[tspAccessCommandTableIndex];
            }

            // If we made it here - we have a current message
            if (streamOut) fprintf(streamOut,"\n   [Building %s]",currentMessage->messageType);
            AsdIdStatus_type* statMsg = (AsdIdStatus_type*)messageBuffer;
            statMsg->msgId = msgId;

            // start with the first payload element
            currentElementIndex = 0;
            signalItemNum = -1;
        }

        // check if still entering a payload element
        if (currentMessage && signalItemNum != -1)
        {
            TspConsolePayloadElement* anElem = &currentMessage->
                reqElems[currentElementIndex];

            void* valPtr = &((char*)messageBuffer)[anElem->charOffset];
            %<ModelPrefix()>StringToNumericArrayElement(valPtr, signalItemNum, anElem->dtypeId, token);

            // check if done with payload element
            ++signalItemNum;
            if (signalItemNum >= (int)anElem->width)
            {
                // done with element, check if there are more
                ++currentElementIndex;
                if (currentElementIndex >= (int)currentMessage->reqCount)
                {
                    // No more... message is done!
                    currentMessage = 0;
                    tspAccess = 0;
                    sizeOut = 1;                 // TODO tspAccess->reqSize
                    active = 0;
                }
                else
                {
                    // Still more signals to go, start the next one
                    signalItemNum = -1;
                }
            }
            else
            {
                if (streamOut) fprintf(streamOut," ");
            }
        }

        // Check if a new payload element has just started
        if (currentMessage && signalItemNum == -1)
        {
            signalItemNum = 0;
            if (currentElementIndex < (int)currentMessage->reqCount)
            {
                // prompt for payload element value(s)
                TspConsolePayloadElement* anElem = &currentMessage->
                    reqElems[currentElementIndex];
                if (streamOut) fprintf(streamOut, "\n   %s (%d %s values): ", anElem->name, anElem->width,
                    %<ModelPrefix()>GetDTypeStr(anElem->dtypeId));
            }
        }

        // if current message is 0, then this must be a message name
    }

    return sizeOut;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
