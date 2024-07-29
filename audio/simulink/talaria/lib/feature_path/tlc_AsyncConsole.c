%% TLC code generation file - hand written source
%% INCLUDED BY: AsyncSIL_SFunc.tlc

// -----------------------------------------------------
// Local function prototypes
static char* ReadFromConsoleInputString( void );
static int mystrincmp(const char* a, const char* b, int count);
static const char* GetNameAndAdvance(AsyncConsoleIterator* iterator);

// -----------------------------------------------------
// Top-level model root array to access console metadata tables:
int numAsyncConsoleRoots = 0;
AsyncConsoleRoot* asyncConsoleRoots[%<ModelPrefix()>ModelCount] = {0};

void RegisterAsyncConsoleRoot(AsyncConsoleRoot* aRoot)
{
    int i;
    const int arraySize = %<ModelPrefix()>ModelCount;
    for (i=0; i < arraySize; i++)
    {
        if (aRoot == asyncConsoleRoots[i])
        {
            // already here!
            return;
        }
        if (0 == asyncConsoleRoots[i])
        {
            // found a slot at the end
            asyncConsoleRoots[i] = aRoot;
            numAsyncConsoleRoots = i+1;
            return;
        }
    }

    // ERROR! Array size was too small! This should not happen.
    while(1);
}

AsyncConsoleIterator INVALID_ASYNC_CONSOLE_ITERATOR = {-1,-1};

int IsValidAsyncConsoleIterator(AsyncConsoleIterator anIterator)
{
    return (anIterator.model >= 0) && (anIterator.index >= 0);
}
    
//-----------------------------------------------------------
// Shared prototypes, data, and functions for console workers
//-----------------------------------------------------------
#define INPUT_BUFFER_SIZE 256
static char input_buffer[INPUT_BUFFER_SIZE];

//-----------------------------------------------------------
// String input override
//-----------------------------------------------------------
// When set to a non-null value by %<ModelPrefix()>ConsoleSetStringIn,
// this string takes over input, blocking/ignoring any console input
static const char* consoleInputString = 0;
        
//-----------------------------------------------------------
/** mystrincmp is a bounded case insensitive compare
 * @returns 1 if lowercase strings are equal, 0 otherwise.
 * Presence of any NULL pointers will return 0 always
 */
static int mystrincmp(const char* a, const char* b, int count)
{
    int i = 0;
    if (!a || !b)
        return 0;
    while (*a && *b && i < count)
    {
        if (tolower(*a++) != tolower(*b++))
        {
            return 0;
        }

        i++;
    }

    // strings are the same if our comparison count expired or they both terminated
    return (i >= count) || (*a == *b);
}

//-----------------------------------------------------------
// Datatype strings
static const char* dtypeStr[] =
{
   "DOUBLE", "SINGLE", "INT8", "UINT8", "INT16", "UINT16", "INT32", "UINT32",
   "BOOLEAN"
};
const char* %<ModelPrefix()>GetDTypeStr(int dtypeId)
{
    const char* retVal = "";
    if (dtypeId < (sizeof(dtypeStr)/sizeof(dtypeStr[0])))
    {
        retVal = dtypeStr[dtypeId];
    }
    return retVal;
}

//-----------------------------------------------------------
// Datatype sizes
static const int dtypeSize[] =
{ sizeof(double),
  sizeof(float),
  sizeof(int8_T),
  sizeof(uint8_T),
  sizeof(int16_T),
  sizeof(uint16_T),
  sizeof(int32_T),
  sizeof(uint32_T),
  sizeof(int8_T)
};
int %<ModelPrefix()>GetDTypeSize(SimulinkDatatypeId dtypeId)
{
    return dtypeSize[dtypeId];
}

//-----------------------------------------------------------
void %<ModelPrefix()>PrintTypedArray(SimulinkDatatypeId dTypeId, void* valPtr, uint32_T
    width, FILE* streamOut)
{
    uint32_t index;
    for (index = 0; index < width; index++)
    {
        switch (dTypeId)
        {
        case TSS_DOUBLE:
        {
            double *typedPtr = (double *)valPtr;
            fprintf(streamOut, " %lf", typedPtr[index]);
            break;
        }

        case TSS_SINGLE:
        {
            float *typedPtr = (float *)valPtr;
            fprintf(streamOut, " %f", typedPtr[index]);
            break;
        }

        case TSS_INT8:
        {
            int8_t *typedPtr = (int8_t *)valPtr;
            fprintf(streamOut, " %i", typedPtr[index]);
            break;
        }

        case TSS_UINT8:
        {
            uint8_t *typedPtr = (uint8_t *)valPtr;
            fprintf(streamOut, " %u", typedPtr[index]);
            break;
        }

        case TSS_INT16:
        {
            int16_t *typedPtr = (int16_t *)valPtr;
            fprintf(streamOut, " %i", typedPtr[index]);
            break;
        }

        case TSS_UINT16:
        {
            uint16_t *typedPtr = (uint16_t*)valPtr;
            fprintf(streamOut, " %u", typedPtr[index]);
            break;
        }

        case TSS_INT32:
        {
            int32_t *typedPtr = (int32_t *)valPtr;
            fprintf(streamOut, " %i", typedPtr[index]);
            break;
        }

        case TSS_UINT32:
        {
            uint32_t *typedPtr = (uint32_t*)valPtr;
            fprintf(streamOut, " %u", typedPtr[index]);
            break;
        }

        case TSS_BOOLEAN:
        {
            int8_t *typedPtr = (int8_t *)valPtr;
            fprintf(streamOut, " %i", typedPtr[index]);
            break;
        }
        }
    }
}

//-----------------------------------------------------------
void %<ModelPrefix()>StringToNumericArrayElement(void* valPtr, uint32_t indexIn, SimulinkDatatypeId
    dTypeId, const char* stringIn)
{
    switch (dTypeId)
    {
    case TSS_DOUBLE:
        sscanf(stringIn, "%lf", &((double*)valPtr)[indexIn]);
        break;

    case TSS_SINGLE:
        sscanf(stringIn, "%f", &((float*)valPtr)[indexIn]);
        break;

    case TSS_INT8:
        sscanf(stringIn, "%hhi", &((int8_t*)valPtr)[indexIn]);
        break;

    case TSS_UINT8:
        sscanf(stringIn, "%hhu", &((uint8_t*)valPtr)[indexIn]);
        break;

    case TSS_INT16:
        sscanf(stringIn, "%hi", &((int16_t*)valPtr)[indexIn]);
        break;

    case TSS_UINT16:
        sscanf(stringIn, "%hu", &((uint16_t*)valPtr)[indexIn]);
        break;

    case TSS_INT32:
        sscanf(stringIn, "%i", &((int32_t*)valPtr)[indexIn]);
        break;

    case TSS_UINT32:
        sscanf(stringIn, "%u", &((uint32_t*)valPtr)[indexIn]);
        break;

    case TSS_BOOLEAN:
        sscanf(stringIn, "%hhi", &((int8_t*)valPtr)[indexIn]);
        break;
    }
}

//-----------------------------------------------------------
void %<ModelPrefix()>ConsoleSetStringIn(const char* stringIn)
{
    consoleInputString = stringIn;
}

//-----------------------------------------------------------
static char* ReadFromConsoleInputString()
{
    // Copy from consoleInputString to input_buffer

    // Use the shared input buffer to capture the token
    char* s = input_buffer;

    if (consoleInputString)
    {
    // Phase 1: skip delimiters
    while (*consoleInputString == ' ') consoleInputString++;

    // Phase 2: copy until delimiter or end
    while (*consoleInputString && *consoleInputString != ' ')
    {
        *s++ = *consoleInputString++;
        }
    }
    
    // Phase 3: terminate
    *s = 0;
    return input_buffer;
}

//-----------------------------------------------------------
static const char* GetNameAndAdvance(AsyncConsoleIterator* iterator)
{
    const char* theName = 0;
    
    if (currentHandler)
    {
        // Get the name currently addressed by iterator
        theName = currentHandler->GetRecordName(*iterator);
    
        // If the index has run off the end - try the next model
        if (!theName)
        {
            iterator->index = 0;
            while ((0 == theName) && (++(iterator->model) < numAsyncConsoleRoots))
            {
                theName = currentHandler->GetRecordName(*iterator);
            }
        }
    
        if (theName)
        {
            // Got one- advance to next index
            iterator->index++;
        }
    }    
    return theName;
}


//-----------------------------------------------------------
char* %<ModelPrefix()>AcquireWordFromConsole(int getMsg)
{
%if ::INCLUDE_ASYNC_CONSOLE == ::ASYNC_CONSOLE_PLAYBACK
    return ReadFromConsoleInputString();
%else
    int isEOF = 0;
    int done = 0;
    int c;
    int len = 0;
    AsyncConsoleIterator hintIndex = {0};
    int typedLen = 0;
    const char* delimiters = " \t\n\r\v\f,;";
    char* s = input_buffer;
    if (consoleInputString)
    {
        return ReadFromConsoleInputString();
    }
    else
    {
        if (getMsg)
        {
            const char* handler = currentHandler ? currentHandler->console_name : "???";
            printf("\n%s>",handler);
        }
        do
        {
            c = getch();
            isEOF = (c == EOF);
            if (c == 0) continue;

            // handle backspace
            if (c == '\b')
            {
                if (len > 0)
                {
                    _cputs("\b \b");
                    fflush(stdout);
                    len--;
                    s--;
                    typedLen = len;
                }
            }

            // handle tab "auto-complete"
            else if (getMsg && c == '\t')
            {
                // If there is no handler, ignore tabs
                if (0 == currentHandler)
                {
                    continue;
                }
                // tab completion hints
                AsyncConsoleIterator startIndex = hintIndex;
                int foundMatch = 0;
                int looped = 0;
                do
                {
                    const char* messageName = GetNameAndAdvance(&hintIndex);
                    if (!messageName && !looped)
                    {
                        looped = 1;
                        hintIndex.model = 0;
                        hintIndex.index = 0;
                        messageName = GetNameAndAdvance(&hintIndex);
                    }
                    // Give up if there's no name
                    if (!messageName) break;
                    
                    int msgLen = (int)strlen(messageName);
                    if (mystrincmp(messageName, input_buffer, typedLen) && msgLen <=
                        INPUT_BUFFER_SIZE)
                    {
                        // found a match: erase the old text
                        int e;
                        for (e = 0; e < len; e++)
                        {
                            _cputs("\b \b");
                        }

                        // and print the new stuff
                        _cputs(messageName);
                        strcpy(input_buffer, messageName);
                        len = msgLen;
                        s = &input_buffer[msgLen];
                        foundMatch = 1;
                    }
                }
                while (((startIndex.model != hintIndex.model) || (startIndex.index != hintIndex.index)) && false == foundMatch);
            }

            // check for word delimiters
            else if (strchr(delimiters, c) || isEOF)
            {
                // Delimter is found... but we're not done until there
                // is something gathered in the buffer or we've hit EOF
                if (s != input_buffer || isEOF)
                {
                    done = 1;
                }
            }
            else
            {
                // If a token is too long for the buffer, throw away extra characters
                if (s - input_buffer < INPUT_BUFFER_SIZE)
                {
                    *s++ = (char)c;
                    putch(c);
                    len++;
                    typedLen++;
                }
            }
        }
        while (done == 0);
    }
    // ASCIIZ terminatation of the token:
    *s = 0;

    // Convert hexidecimal strings to decimal:
    if (input_buffer[0] == '0' && input_buffer[1] == 'x')
    {
        int32_t value;
        sscanf(&input_buffer[2], "%x", &value);
        sprintf(input_buffer,"%d", value);
    }
    return input_buffer;
%endif
}


//-----------------------------------------------------------
// Public / external functions:
//-----------------------------------------------------------
/** ConsolePrintMessage Parses and prints (via stdout) details of a
 * given binary asynchronous message. Iterates through handlers and find 
 * one that prints the buffer.
 * @param isRequest : boolean value set to true to inform the parser that
 * the message is known to be a request (i.e. to be sent into the model).
 * Set to false for any traffic received back from the model (i.e. responses
 * and notifications)
 * @param msgBuffer is a pointer to the binary message
 * @param bufferSize the size in char of the msg buffer (not currently used)
 * @param streamOut file stream to receive message printout
 * @ingroup rtc
 **/
void %<ModelPrefix()>ConsolePrintMessage(int isRequest, void* msgBuffer, uint32_t bufferSize, FILE* streamOut)
{
    int i;
    for (i=0; i<AsyncConsoleHandlerTableCount; i++)
    {
        %<ModelPrefix()>AsyncConsoleHandler* aHandler = asyncHandlerTable[i];
        if (aHandler->PrintMessage(isRequest,msgBuffer,bufferSize,streamOut))
        {
            return;
        }
    }
    // None of the handlers above stepped up to parse this message!
    AsdIdStatus_type* statMsg = (AsdIdStatus_type*)msgBuffer;
    int dataBytes;
    uint8_t* dumpPtr;
    if (isRequest)
    {
        fprintf(streamOut,"*** No parser for request available...\n"
                "Message Id:0x%08x\n", statMsg->msgId);
        dumpPtr = (uint8_t*)&statMsg->callerHandle;
        dataBytes = 12;
        fprintf(streamOut, "Unknown request size... dumping first %d bytes:",dataBytes);
    }
    else
    {
        fprintf(streamOut,"*** No parser for response available...\nMessage Id:0x%08x\nCaller Handle:0x%08x\nStatus Code:0x%08x\n",
            statMsg->msgId, statMsg->callerHandle, statMsg->statusCode);
        dumpPtr = (uint8_t*)&statMsg->data;
        dataBytes = bufferSize - sizeof(AsdIdStatus_type);
        fprintf(streamOut, "%d response data bytes:",dataBytes);
    }
    for (i = 0; i < dataBytes; i++)
    {
        if ((i % 4) == 0) fprintf(streamOut,"\n");
        fprintf(streamOut, " 0x%02x", dumpPtr[i]);
    }
    fprintf(streamOut,"\n");
    fflush(streamOut);
}

/** ConsoleBlockingMessageInput Reads from stdin to generate a packed
 * binary request message.
 * @param messageBuffer pointer to buffer that receives the binary message
 * @param bufferSize size in char of the message buffer
 * @param streamOut file stream to receive prompts
 * @returns size in char of the created message- "0" if message input
 * aborted by !QUIT
 **/
int %<ModelPrefix()>ConsoleBlockingMessageInput(void* messageBuffer, uint32_t bufferSize, FILE* streamOut)
{
    if (0 == AsyncConsoleHandlerTableCount)
    {
        return 0; // no handlers registered!
    }
    // If no handler is active yet, just use the first one
    if (0 == currentHandler)
    {
        currentHandler = asyncHandlerTable[0];
    }
    int retVal = -1;
    while (retVal == -1)
    {
        retVal = currentHandler->BlockingMessageInput(messageBuffer,bufferSize,streamOut);
        // check for ! command
        if (retVal < 0)
        {
            if (0 == strcmp(messageBuffer,"!QUIT"))
            {
                return 0;
            }
            if (0 == strcmp(messageBuffer,"!HELP"))
            {
                if (streamOut)
                {
                    fprintf(streamOut,"\n---- Async Console Help ----\n");
                    fprintf(streamOut,"  <tab key> : autocomplete the current message identifier\n");
                    fprintf(streamOut,"  !QUIT : Exits console\n");
                    fprintf(streamOut,"  !HELP : Prints this message\n");
                    int i;
                    for (i=0; i<AsyncConsoleHandlerTableCount; i++)
                    {
                        %<ModelPrefix()>AsyncConsoleHandler* aHandler = asyncHandlerTable[i];
                        fprintf(streamOut,"  !%s : Enters the %s console\n",
                                aHandler->console_name, aHandler->console_name);
                    }
                }
            }
            else
            {
                // switch to a different console parser
                int i;
                %<ModelPrefix()>AsyncConsoleHandler* newHandler = 0;
                const char* cmd = &((char*)messageBuffer)[1];// skip the "!"
                for (i=0; i<AsyncConsoleHandlerTableCount; i++)
                {
                    %<ModelPrefix()>AsyncConsoleHandler* aHandler = asyncHandlerTable[i];
                    if (0 == strcmp(aHandler->console_name, cmd))
                    {
                        newHandler = aHandler;
                        break;
                    }
                }
                if (newHandler)
                {
                    if (streamOut) fprintf(streamOut,"\n---- Entering console: %s ----\n",newHandler->console_name);
                    currentHandler = newHandler;
                }
                else
                {
                    if (streamOut) fprintf(streamOut,"\nCommand not recognized. Try !HELP or !QUIT\n");
                }
            }
        }
    }
    return retVal;
}

