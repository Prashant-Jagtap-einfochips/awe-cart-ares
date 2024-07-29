%% tlc_interpret_trigger.c included by InterpretTrigger.tlc

// -----------------------------------------------------
// Local storage
static uint32_t theInterpretationErrorState = 0;
static char interpretErrorBuffer[1024];
static int interpretErrorCharsUsed = 0;

// -----------------------------------------------------
// Interpretation engine entry points:

// forward declare local functions
static void SetInterpretationErrorState(uint32_t errorState);
static void ErrorVPrintf(const char* formatString, va_list args);
static void ErrorPrintf(const char* formatString, ...);

// -----------------------------------------------------
// Top-level model root array to access engine tables:
int numInterpretationRoots = 0;
InterpretationRoot* interpretationRoots[%<ModelPrefix()>ModelCount] = { 0 };

void RegisterInterpretationRoot(InterpretationRoot* aRoot)
{
    int i;
    const int arraySize = %<ModelPrefix()>ModelCount;
    for (i = 0; i < arraySize; i++)
    {
        if (aRoot == interpretationRoots[i])
        {
            // already here!
            return;
        }
        if (0 == interpretationRoots[i])
        {
            // found a slot at the end
            interpretationRoots[i] = aRoot;
            numInterpretationRoots = i + 1;
            return;
        }
    }

    // ERROR! Array size was too small! This should not happen.
    while (1);
}

InterpretEngineIterator INVALID_INTERPRET_ITERATOR = { -1,-1 };

int IsValidInterpretIterator(InterpretEngineIterator anIterator)
{
    return (anIterator.model >= 0) && (anIterator.index >= 0);
}

static InterpretationRecord_type* GetInterpretationRecord(InterpretEngineIterator interpretationNumIn)
{
    // Validate input parameters
    if ((interpretationNumIn.model < 0) || (interpretationNumIn.model >= numInterpretationRoots))
    {
        return 0; // model out of range
    }
    InterpretationRoot* theRoot = interpretationRoots[interpretationNumIn.model];
    if ((interpretationNumIn.index < 0) || (interpretationNumIn.index >= *theRoot->interpretation_count))
    {
        return 0; // index out of range for specified model
    }
    InterpretationRecord_type* theTable = *(InterpretationRecord_type**)theRoot->interpretation_table;
    return &theTable[interpretationNumIn.index];
}
// Touch interpretation object to indicate new data is available and
// must be interpreted.
void TouchInterpretation(InterpretEngineIterator interpretationNumIn)
{
    InterpretationRecord_type* aRecord = GetInterpretationRecord(interpretationNumIn);
    if (aRecord)
    {
        aRecord->requested_state_vars++;
    }
}

// Clear all flags that would indicate a interpretation must be run
void ClearAllInterpretationRequiredFlags( void )
{
    InterpretationRecord_type* aRecord;
    InterpretEngineIterator interpretationNum;
    int m;
    for (m = 0; m < numInterpretationRoots; m++)
    {
        interpretationNum.model = m;
        interpretationNum.index = 0;
        for(aRecord = GetInterpretationRecord(interpretationNum); aRecord;
            aRecord = GetInterpretationRecord(interpretationNum))
        {
            aRecord->requested_state_vars = 0;
            interpretationNum.index++;
        }
    }
}

// Retrieve space-delimited list of TSPs needed for specified interpretation
// Returns char count placed in buffer
int ListTSPsForInterpretation(InterpretEngineIterator interpretationNumIn, char* bufferIn, int bufferSizeIn)
{
    char* buf = bufferIn;
    int bufSize = bufferSizeIn;
    InterpretationRecord_type* aRecord = GetInterpretationRecord(interpretationNumIn);
    if (aRecord)
    {
        // Loop through and dump TSP names
        int i;
        for (i = 0; i < aRecord->tsp_count; i++)
        {
            InterpretEngineIterator aTSP;
            aTSP.model = interpretationNumIn.model;
            aTSP.index = aRecord->tsps[i];
            int used = 0;
            if (i > 0)
            {
                used = snprintf(buf, bufSize, " ");
                buf += used;
                bufSize -= used;
            }
            used = snprintf(buf, bufSize, "%s", GetTSPName(aTSP));
            buf += used;
            bufSize -= used;
        }
    }
    return (bufferSizeIn - bufSize);
}

// Return the first interpretation record that has been touched to run (or INVALID_INTERPRETATION if none)
InterpretEngineIterator FindFirstNeededInterpretation( void )
{
    InterpretEngineIterator retVal = INVALID_INTERPRET_ITERATOR;
    InterpretEngineIterator theFirst;
    int m;
    InterpretationRecord_type* aRecord;
    for (m = 0; m < numInterpretationRoots; m++)
    {
        theFirst.model = m;
        theFirst.index = 0;
        for (aRecord = GetInterpretationRecord(theFirst); aRecord;
            aRecord = GetInterpretationRecord(theFirst))
        {
            if (aRecord->requested_state_vars != 0)
            {
                // Found it!
                retVal = theFirst;
                break;
            }
            theFirst.index++;
        }
    }
    return retVal;
}

// Return the next interpretation record that has been touched to run (or -1 if no more)
InterpretEngineIterator FindNextNeededInterpretation(InterpretEngineIterator afterThisOne)
{
    InterpretEngineIterator retVal = INVALID_INTERPRET_ITERATOR;
    InterpretEngineIterator nextOne;
    nextOne.index = afterThisOne.index + 1;
    int m;
    int foundIt = 0;
    InterpretationRecord_type* aRecord;
    for (m = afterThisOne.model; (0 == foundIt) && (m < numInterpretationRoots); m++)
    {
        nextOne.model = m;
        for (aRecord = GetInterpretationRecord(nextOne); aRecord;
            aRecord = GetInterpretationRecord(nextOne))
        {
            if (aRecord->requested_state_vars != 0)
            {
                // Found it!
                foundIt = 1;
                retVal = nextOne;
                break;
            }
            // move on to the next interpretation
            nextOne.index++;
        }
        // Reset the index in preparation for the next model search
        nextOne.index = 0;
    }
    return retVal;
}

// Verify that all TSPs read by the specified interpretation have been seen
// returns 0 for OK, nonzero is error
int CheckInterpretationSourceSightingsOK(InterpretEngineIterator interpretationNumIn)
{
    int retVal = 0; // assume signtings are OK
    InterpretationRecord_type* aRecord = GetInterpretationRecord(interpretationNumIn);
    if (0 == aRecord) return 0; // if interpretation not found just bail ok


        // Loop through and check that all source TSPs have been seen
    int i;
    for (i = 0; i < aRecord->tsp_count; i++)
    {
        InterpretEngineIterator aTSP;
        aTSP.model = interpretationNumIn.model;
        aTSP.index = aRecord->tsps[i];
        int sightings = GetTSPSightings(aTSP);
        if (sightings == 0)
        {
            // Error - have not seen this one yet!
            retVal = 1;
            ErrorPrintf("Error: Missing TSP %s (needed for interpretation#%d:%d)\n",
                GetTSPName(aTSP), interpretationNumIn.model, interpretationNumIn.index);
            // Missing StateVar. Set the error state
            SetInterpretationErrorState(1);
        }
    }
    return retVal;
}


// Checks source sightings & calls interpretation function, but does *not* reset dirty flags
// returns error state (0 means OK);
int CallInterpretation(InterpretEngineIterator interpretationNumIn)
{
    InterpretationRecord_type* aRecord = GetInterpretationRecord(interpretationNumIn);
    if (0 == aRecord) return 0; // if interpretation not found just bail ok

    int err = CheckInterpretationSourceSightingsOK(interpretationNumIn);

    if (0 == err)
    {
        aRecord->interpretation_func();
    }
    if (0 != GetInterpretationErrorState())
    {
        ErrorPrintf("... while processing TSPs { ");
        int i;
        for (i = 0; i < aRecord->tsp_count; i++)
        {
            InterpretEngineIterator aTSP;
            aTSP.model = interpretationNumIn.model;
            aTSP.index = aRecord->tsps[i];
            ErrorPrintf("%s ", GetTSPName(aTSP));
        }
        ErrorPrintf("}\n");
    }


    return GetInterpretationErrorState();
}

// Retrieve interpretation error state
uint32_t GetInterpretationErrorState( void )
{
    return theInterpretationErrorState;
}

static void SetInterpretationErrorState(uint32_t errorState)
{
    theInterpretationErrorState = errorState;
}

// Internal use print to error buffer
static void ErrorVPrintf(const char* formatString, va_list args)
{
    char* buffer = &interpretErrorBuffer[interpretErrorCharsUsed];
    int bufferSize = sizeof(interpretErrorBuffer) - interpretErrorCharsUsed;
    if (bufferSize)
    {
        int charsPrinted = vsnprintf(
            buffer, bufferSize, formatString, args);
        interpretErrorCharsUsed += charsPrinted;
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

// Report a interpretation error (works like printf)
// Called from the model code
void InterpretError(const char* formatString, ...)
{
    va_list args;
    va_start(args, formatString);
    ErrorVPrintf(formatString, args);
    va_end(args);

    // Add newline and set error state
    ErrorPrintf("\n");
    SetInterpretationErrorState(1);
}

// Retrieve reported error strings
const char* GetInterpretationErrorStrings( void )
{
    return &interpretErrorBuffer[0];
}

// Clear error state
void ClearInterpretationErrorState( void )
{
    interpretErrorBuffer[0] = 0;
    interpretErrorCharsUsed = 0;
    SetInterpretationErrorState(0);
}
