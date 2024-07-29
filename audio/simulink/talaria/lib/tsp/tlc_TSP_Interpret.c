%% TLC code generation file - hand written source
%% API calls supporting TSP outports(interpretation engine)
%% INCLUDED BY : TSP_Interpret.tlc

// -----------------------------------------------------
// Local storage
static char tspErrorBuffer[400];
static int tspErrorCharsUsed = 0;

// forward declare local functions
static void ErrorVPrintf(const char* formatString, va_list args);
static void ErrorPrintf(const char* formatString, ...);

// Internal use print to error buffer
static void ErrorVPrintf(const char* formatString, va_list args)
{
    char* buffer = &tspErrorBuffer[tspErrorCharsUsed];
    int bufferSize = sizeof(tspErrorBuffer) - tspErrorCharsUsed;
    if (bufferSize)
    {
        int charsPrinted = vsnprintf(
            buffer, bufferSize, formatString, args);
        tspErrorCharsUsed += charsPrinted;
    }
}

// Internal use print to error buffer
static void ErrorPrintf(const char* formatString, ...)
{
    va_list args;
    va_start(args, formatString);
    ErrorVPrintf(formatString, args);
    va_end(args);
}

// Clear error state
void ClearTSPErrorStrings()
{
    tspErrorBuffer[0] = 0;
    tspErrorCharsUsed = 0;
}

// Retrieve reported error strings
const char* GetTSPErrorStrings()
{
    return &tspErrorBuffer[0];
}

// checks for string equality without regards to case
// returns 1 if lowercase strings are equal, 0 otherwise
static int insensitiveStrcmp(const char* A, const char* B)
{
    int i;
    for (i = 0; *A && *B; i++)
    {
        if (tolower(*A++) != tolower(*B++))
        {
            return 0;
        }
    }

    return (*A == *B);
}

static TSP_InterpretationTable_type* GetTspInterpretationRecord(InterpretEngineIterator iteratorIn)
{
    if ((iteratorIn.model < 0) || (iteratorIn.model >= numInterpretationRoots))
    {
        return 0; // model out of range
    }
    InterpretationRoot* theRoot = interpretationRoots[iteratorIn.model];
    if ((iteratorIn.index < 0) || (iteratorIn.index >= *theRoot->tsp_interpret_table_count))
    {
        return 0; // index out of range for specified model
    }
    TSP_InterpretationTable_type* theTable = *(TSP_InterpretationTable_type**)theRoot->tsp_interpret_table;
    return &theTable[iteratorIn.index];
}

// Return a TSP iterator if name is recognized, INVALID_INTERPRET_ITERATOR otherwise
InterpretEngineIterator RecognizeTSP(const char* tspName)
{
    InterpretEngineIterator retVal = INVALID_INTERPRET_ITERATOR;
    InterpretEngineIterator iterator;
    TSP_InterpretationTable_type* theVar = 0;
    TSP_InterpretationTable_type* aRecord;

    // Loop through table searching for a variable name match
    int m;
    for (m = 0; (m < numInterpretationRoots) && (theVar == 0); m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for(aRecord = GetTspInterpretationRecord(iterator); aRecord;
            aRecord = GetTspInterpretationRecord(iterator))
        {
            if (insensitiveStrcmp(tspName, aRecord->name))
            {
                theVar = aRecord;
                retVal = iterator;
                break;
            }
            iterator.index++;
        }
    }

    // If a variable was found, set all the flags announcing its arrival
    if (theVar)
    {
        theVar->sightings_count++;
    }
    else
    {
        // variable was *not* found. generate error string
        ErrorPrintf("Error: Unrecognized TSP [%s]\n", tspName);
    }

    return retVal;
}

// Reset all TSP sighting counts to zero
void ClearAllTSPSightings()
{
    TSP_InterpretationTable_type* aRecord;
    InterpretEngineIterator iterator;
    int m;
    for (m = 0; m < numInterpretationRoots; m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for(aRecord = GetTspInterpretationRecord(iterator); aRecord;
            aRecord = GetTspInterpretationRecord(iterator))
        {
            aRecord->sightings_count = 0;
            iterator.index++;
        }
    }
}

// Return count of how many times the specified TSP has been seen
int GetTSPSightings(InterpretEngineIterator tspNumberIn)
{
    int sightings = 0;
    TSP_InterpretationTable_type* aRecord = GetTspInterpretationRecord(tspNumberIn);
    if (aRecord)
    {
        sightings = aRecord->sightings_count;
    }
    return sightings;
}

// Return TSP name (or NULL if bad number)
const char* GetTSPName(InterpretEngineIterator tspNumberIn)
{
    const char* name = 0;
    TSP_InterpretationTable_type* aRecord = GetTspInterpretationRecord(tspNumberIn);
    if (aRecord)
    {
        name = aRecord->name;
    }
    return name;
}

uint8_t checkIfHexData(const char* element)
{
    uint8_t isHexData = 0;
    char *hexHeader = "0x";
    if (strncmp(element, hexHeader, 2) == 0)
    {
        isHexData = 1;
    }
    return isHexData;
}

// Set a single value of the TSP
// For an N row by M column matrix using zero-based row and col, indexIn = row + (col * N);
// Returns 0 for OK, 1=error 2=warning (does not set error state)
int SetTSPElement(InterpretEngineIterator tspNumberIn, int indexIn, const char* stringElementValueIn)
{
    // bail if provided number is not within the table
    TSP_InterpretationTable_type* aRecord = GetTspInterpretationRecord(tspNumberIn);
    if (!aRecord) return 1;

    int maxElementIndex = aRecord->width;

    if (indexIn >= maxElementIndex)
    {
        ErrorPrintf("Error: TSP %s accepts only %d elements. Ignoring element #%d with value [%s]\n",
            aRecord->name, maxElementIndex, indexIn + 1, stringElementValueIn);
        fflush(stderr);
        return 2; // bail out... requested index is out of range!
    }

    // parse the value string, placing numeric value into the TSP
    SimulinkDatatypeId dTypeId = aRecord->dtype_id;
    void* valPtr = aRecord->value_ptr;

    uint8_t isHexData = checkIfHexData(stringElementValueIn);
    if (isHexData == 1)
    {
        switch (dTypeId)
        {
        case TSS_DOUBLE:
            sscanf(stringElementValueIn, "%llx", &((double *)valPtr)[indexIn]);
            break;

        case TSS_SINGLE:
            sscanf(stringElementValueIn, "%lx", &((float *)valPtr)[indexIn]);
            break;

        case TSS_INT32:
        case TSS_UINT32:
            sscanf(stringElementValueIn, "%x", &((uint32_t *)valPtr)[indexIn]);
            break;

        case TSS_INT16:
        case TSS_UINT16:
            sscanf(stringElementValueIn, "%hx", &((uint16_t *)valPtr)[indexIn]);
            break;

        case TSS_INT8:
        case TSS_UINT8:
        case TSS_BOOLEAN:
            sscanf(stringElementValueIn, "%hhx", &((uint8_t *)valPtr)[indexIn]);
            break;
        }
    }
    else
    {

        switch (dTypeId)
        {
        case TSS_DOUBLE:
            sscanf(stringElementValueIn, "%lf", &((double*)valPtr)[indexIn]);
            break;
        case TSS_SINGLE:
            sscanf(stringElementValueIn, "%f", &((float*)valPtr)[indexIn]);
            break;
        case TSS_INT8:
            sscanf(stringElementValueIn, "%hhi", &((int8_t *)valPtr)[indexIn]);
            break;
        case TSS_UINT8:
            sscanf(stringElementValueIn, "%hhu", &((uint8_t *)valPtr)[indexIn]);
            break;
        case TSS_INT16:
            sscanf(stringElementValueIn, "%hi", &((int16_t *)valPtr)[indexIn]);
            break;
        case TSS_UINT16:
            sscanf(stringElementValueIn, "%hu", &((uint16_t *)valPtr)[indexIn]);
            break;
        case TSS_INT32:
            sscanf(stringElementValueIn, "%i", &((int32_t *)valPtr)[indexIn]);
            break;
        case TSS_UINT32:
            sscanf(stringElementValueIn, "%u", &((uint32_t *)valPtr)[indexIn]);
            break;
        case TSS_BOOLEAN:
            sscanf(stringElementValueIn, "%hhi", &((int8_t *)valPtr)[indexIn]);
            break;
        }
    }
    return 0; // no errors
}

