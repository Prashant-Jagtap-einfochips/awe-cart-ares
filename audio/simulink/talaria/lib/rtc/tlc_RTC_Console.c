%% TLC code generation file - hand written source
%% INCLUDED BY: RTC_BytePack.tlc

//-----------------------------------------------------------
// local prototypes
static RtcConsoleMessage* GetConsoleRecord(AsyncConsoleIterator iteratorIn);
static const char* GetName(AsyncConsoleIterator iterator);
static RtcConsoleMessage* GetMessageById(uint32_t messageId);
static RtcConsoleMessage* GetMessageByName(const char* name);


//-----------------------------------------------------------
// Define RTC Console handler
//-----------------------------------------------------------
static int RtcConsolePrintMessage(int isRequest, void* msgBuffer, uint32_t bufferSize, FILE* streamOut);
static int RtcConsoleBlockingInputMessage(void* messageBuffer, uint32_t bufferSize, FILE* streamOut);
%<ModelPrefix()>AsyncConsoleHandler %<ModelPrefix()>RtcConsoleHandler =
{
    "RTC",
    &GetName,
    &RtcConsolePrintMessage,
    &RtcConsoleBlockingInputMessage
};


//-----------------------------------------------------------
static RtcConsoleMessage* GetConsoleRecord(AsyncConsoleIterator iteratorIn)
{
    if ((iteratorIn.model < 0) || (iteratorIn.model >= numAsyncConsoleRoots))
    {
        return 0; // model out of range
    }
    AsyncConsoleRoot* theRoot = asyncConsoleRoots[iteratorIn.model];
    if ((0 == theRoot->rtc_table) || (iteratorIn.index < 0) || (iteratorIn.index >= *theRoot->rtc_count))
    {
        return 0; // index out of range for specified model
    }
    RtcConsoleMessage* theTable = *(RtcConsoleMessage**)theRoot->rtc_table;
    return &theTable[iteratorIn.index];
}

//-----------------------------------------------------------
static const char* GetName(AsyncConsoleIterator iterator)
{
    const char* theName = 0;
    RtcConsoleMessage* aRecord = GetConsoleRecord(iterator);
    if (aRecord)
    {
        theName = aRecord->name;
    }
    return theName;
}

//-----------------------------------------------------------
static RtcConsoleMessage* GetMessageById(uint32_t messageId)
{
    AsyncConsoleIterator iterator = {0};
    RtcConsoleMessage* aRecord;

    // Loop through all records
    int m;
    for (m = 0; m < numAsyncConsoleRoots; m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for(aRecord = GetConsoleRecord(iterator); aRecord;
            aRecord = GetConsoleRecord(iterator))
        {
            if (messageId == aRecord->msgId)
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
static RtcConsoleMessage* GetMessageByName(const char* name)
{
    AsyncConsoleIterator iterator = {0};
    RtcConsoleMessage* aRecord;

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

/** RtcConsolePrintMessage Parses and prints (via stdout) details of a
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
static int RtcConsolePrintMessage(int isRequest, void* msgBuffer, uint32_t bufferSize, FILE* streamOut)
{
    // "use" argument to avoid warnings
    (void)bufferSize;
    
    AsdIdStatus_type* statMsg = (AsdIdStatus_type*)msgBuffer;
    if (0 == ASDID_IS_RTC(statMsg->msgId))
    {
        return 0;
    }
    RtcConsoleMessage* theMsg = GetMessageById(statMsg->msgId);
    if (!theMsg)
    {
        fprintf(streamOut,"*** RTC Message Id 0x%08x not recognized\n", statMsg->msgId);
        return 0; // do not claim it
    }

    const char* payloadTypeStr;
    RtcConsolePayloadElement* elements;
    uint32_t elementCount;
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
    }

    fprintf(streamOut,"%s: %s\n", payloadTypeStr, theMsg->name);
    uint32_t i;
    for (i = 0; i < elementCount; i++)
    {
        RtcConsolePayloadElement* anElem = &elements[i];
        void* valPtr = &((char*)msgBuffer)[anElem->charOffset];
        fprintf(streamOut,"   %s:", anElem->name);
        %<ModelPrefix()>PrintTypedArray(anElem->dtypeId, valPtr, anElem->width,streamOut);
        fprintf(streamOut,"\n");
    }
    fflush(streamOut);
    return 1; // message printed
}

/** RtcConsoleBlockingInputMessage Reads from stdin to generate a packed
 * binary request message.
 * @param messageBuffer pointer to buffer that receives the binary message
 * @param bufferSize size in char of the message buffer
 * @param streamOut file stream to receive prompts
 * @returns size in char of the created message, or "-1" if instead the
 * message buffer contains a ! command.
 **/
static int RtcConsoleBlockingInputMessage(void* messageBuffer, uint32_t bufferSize, FILE* streamOut)
{
    RtcConsoleMessage* currentMessage = 0;
    int currentElementIndex = 0;
    int signalItemNum = -1;
    int active = 1;
    int sizeOut = 0;
    while (active)
    {
        const char* token = %<ModelPrefix()>AcquireWordFromConsole(currentMessage == 0);
        if (*token == '!')
        {
            strncpy(messageBuffer, token, bufferSize);
            return -1;
        }
        if (0 == currentMessage)
        {
            // need a message. Search for it.
            currentMessage = GetMessageByName(token);
            if (0 == currentMessage)
            {
                // unrecognized message- return negative length to caller
                int len = (int)strlen(token);
                return -len;
                //active = 0;
            }
            else
            {
                // setup the msgid
                AsdIdStatus_type* statMsg = (AsdIdStatus_type*)messageBuffer;
                statMsg->msgId = currentMessage->msgId;

                // start with the first payload element
                currentElementIndex = 0;
                signalItemNum = -1;
            }
        }

        // check if still entering a payload element
        if (currentMessage && signalItemNum != -1)
        {
            RtcConsolePayloadElement* anElem = &currentMessage->
                reqElems[currentElementIndex];

            // TODO set token is signalItemNum of currentElementIndex
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
                    sizeOut = 1;// TODO currentMessage->reqSize
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
                RtcConsolePayloadElement* anElem = &currentMessage->
                    reqElems[currentElementIndex];
                if (streamOut)
                {
                    fprintf(streamOut,"\n   %s (%d %s values): ", anElem->name, anElem->width,
                        %<ModelPrefix()>GetDTypeStr(anElem->dtypeId));
                }
            }
            else if (0 == (int)currentMessage->reqCount)
            {
                // There are NO REQ payload elements... message is done!
                currentMessage = 0;
                sizeOut = 1;
                active = 0;
            }
        }

        // if current message is 0, then this must be a message name
    }

    return sizeOut;
}
