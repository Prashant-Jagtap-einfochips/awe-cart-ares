%% tlc_translate_trigger.c : hand-written C code included by TranslateTrigger.tlc

// -----------------------------------------------------
// Local storage
static unsigned int theTranslationErrorState = 0;
static char translateErrorBuffer[1024];
static int translateErrorCharsUsed = 0;

// -----------------------------------------------------
// Translation engine entry points:

// forward declare local functions
static void SetTranslationErrorState(unsigned int errorState);
static void ErrorVPrintf(const char* formatString, va_list args);
static void ErrorPrintf(const char* formatString, ...);

// -----------------------------------------------------
// Top-level model root array to access engine tables:
int numTranslationRoots = 0;
TranslationRoot* translationRoots[%<ModelPrefix()>ModelCount] = {0};
    
void RegisterTranslationRoot(TranslationRoot* aRoot)
{
    int i;
    const int arraySize = %<ModelPrefix()>ModelCount;
    for (i=0; i < arraySize; i++)
    {
        if (aRoot == translationRoots[i])
        {
            // already here!
            return;
        }
        if (0 == translationRoots[i])
        {
            // found a slot at the end
            translationRoots[i] = aRoot;
            numTranslationRoots = i+1;
            return;
        }
    }
    
    // ERROR! Array size was too small! This should not happen.
    while(1);
}

TranslateEngineIterator INVALID_TRANSLATE_ITERATOR = {-1,-1};
        
int IsValidTranslateIterator(TranslateEngineIterator anIterator)
{
    return (anIterator.model >= 0) && (anIterator.index >= 0);
}

static TranslationRecord_type* GetTranslationRecord(TranslateEngineIterator translationNumIn)
{
    // Validate input parameters
    if ((translationNumIn.model < 0) || (translationNumIn.model >= numTranslationRoots))
    {
        return 0; // model out of range
    }
    TranslationRoot* theRoot = translationRoots[translationNumIn.model];
    if ((translationNumIn.index < 0) || (translationNumIn.index >= *theRoot->translation_count))
    {
        return 0; // index out of range for specified model
    }
    TranslationRecord_type* theTable = *(TranslationRecord_type**)theRoot->translation_table;
    return &theTable[translationNumIn.index];
}


// Return the translation record that outputs specified TOP (or INVALID_TRANSLATE_ITERATOR if not found)
static TranslateEngineIterator FindTranslationForTop(const char* topNameIn)
{
    TranslateEngineIterator retVal = INVALID_TRANSLATE_ITERATOR;
    TranslateEngineIterator theOne;
    int m;
    int foundIt = 0;
    TranslationRecord_type* aRecord;
    // Loop through models
    for (m = 0; (0 == foundIt) && (m < numTranslationRoots); m++)
    {
        theOne.model = m;
        theOne.index = 0;
        // Loop through translations of this model
        for (aRecord = GetTranslationRecord(theOne); (0 == foundIt) && aRecord;
             aRecord = GetTranslationRecord(theOne))
        {
            // Loop through TOPs of this translation
            int topIndex;
            for (topIndex = 0; (0 == foundIt) && (topIndex < aRecord->top_count); topIndex++)
            {
                TranslateEngineIterator aTop;
                aTop.model = m;
                aTop.index = aRecord->tops[topIndex];
                const char* aTopName = GetTopName(aTop);
                if (aTopName && (0 == strcmp(aTopName, topNameIn)))
                {
                    // Found it!
                    foundIt = 1;
                    retVal = theOne;
                    break;
                }
            }
            // move on to the next translation
            theOne.index++;
        }
    }
    return retVal;
}

// Retrieve space-delimited list of TuneVars needed for specified translation
// Returns char count placed in buffer
static int ListTuneVarsForTranslation(TranslateEngineIterator translationNumIn,
     char* bufferInOut, int bufferSizeIn)
{
    char* buf = bufferInOut;
    int bufSize = bufferSizeIn;
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (aRecord)
    {
        // TOPs can only be fed by one translation - which was provided
        // Loop through and dump TuneVar names
        int i;
        for (i = 0; i < aRecord->tunevar_count; i++)
        {
            TranslateEngineIterator aTuneVar;
            aTuneVar.model = translationNumIn.model;
            aTuneVar.index = aRecord->tunevars[i];
            int used = 0;
            if (i > 0)
            {
                used = snprintf(buf, bufSize, " ");
                buf += used;
                bufSize -= used;
            }

            used = snprintf(buf, bufSize, "%s", GetTuneVarName(aTuneVar));
            buf += used;
            bufSize -= used;
        }
    }

    return (bufferSizeIn - bufSize);
}

// Returns space-delimited list of TuneVars (in bufferInOut) that feed specified top
// Returns char count placed in buffer
int ListTuneVarsForTop(const char* topNameIn, char* bufferInOut, int bufferSizeIn)
{
    TranslateEngineIterator theTranslation = FindTranslationForTop(topNameIn);
    return ListTuneVarsForTranslation(theTranslation,bufferInOut,bufferSizeIn);
}

// Touch translation object to indicate new data is available and
// must be translated.
void TouchTranslation(TranslateEngineIterator translationNumIn)
{
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (aRecord)
    {
        aRecord->updated_tune_vars++;
    }
}

// Get a translation's dirty flag in the form of updated TuneVar count
int GetTranslationUpdatedTuneVarCount(TranslateEngineIterator translationNumIn)
{
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (aRecord)
    {
        return aRecord->updated_tune_vars;
    }
    return 0; // bad iterator
}

// Clear all flags that would indicate a translation must be run
void ClearAllTranslationRequiredFlags( void )
{
    TranslationRecord_type* aRecord;
    TranslateEngineIterator translationNum;
    int m;
    for (m = 0; m < numTranslationRoots; m++)
    {
        translationNum.model = m;
        translationNum.index = 0;
        for(aRecord = GetTranslationRecord(translationNum); aRecord;
            aRecord = GetTranslationRecord(translationNum))
        {
            aRecord->updated_tune_vars = 0;
            translationNum.index++;
        }
    }
}
        
// Return the first translation record that has been touched to run (or INVALID_TRANSLATE_ITERATOR if none)
TranslateEngineIterator FindFirstNeededTranslation( void )
{
    TranslateEngineIterator retVal = INVALID_TRANSLATE_ITERATOR;
    TranslateEngineIterator theFirst;
    int m;
    TranslationRecord_type* aRecord;
    for (m = 0; m < numTranslationRoots; m++)
    {
        theFirst.model = m;
        theFirst.index = 0;
        for (aRecord = GetTranslationRecord(theFirst); aRecord;
             aRecord = GetTranslationRecord(theFirst))
        {
            if (aRecord->updated_tune_vars != 0)
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

// Return the next translation record that has been touched to run (or INVALID_TRANSLATE_ITERATOR if no more)
TranslateEngineIterator FindNextNeededTranslation(TranslateEngineIterator afterThisOne)
{
    TranslateEngineIterator retVal = INVALID_TRANSLATE_ITERATOR;
    TranslateEngineIterator nextOne;
    nextOne.index = afterThisOne.index + 1;
    int m;
    int foundIt = 0;
    TranslationRecord_type* aRecord;
    for (m = afterThisOne.model; (0 == foundIt) && (m < numTranslationRoots); m++)
    {
        nextOne.model = m;
        for (aRecord = GetTranslationRecord(nextOne); aRecord;
             aRecord = GetTranslationRecord(nextOne))
        {
            if (aRecord->updated_tune_vars != 0)
            {
                // Found it!
                foundIt = 1;
                retVal = nextOne;
                break;
            }
            // move on to the next translation
            nextOne.index++;
        }
        // Reset the index in preparation for the next model search
        nextOne.index = 0;
    }
    return retVal;
}

// Verify that all TuneVars read by the specified translation have been seen
// Returns 0 if OK; nonzero if error and sets translation error state
int CheckTranslationSourceSightingsOK(TranslateEngineIterator translationNumIn)
{
    int retVal = 0; // assume signtings are OK
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (0 == aRecord) return 0; // if translation not found just bail ok

    // Loop through and check that all source TuneVars have been seen
    int i;
    for (i = 0; i < aRecord->tunevar_count; i++)
    {
        TranslateEngineIterator aTuneVar;
        aTuneVar.model = translationNumIn.model;
        aTuneVar.index = aRecord->tunevars[i];
        int sightings = GetTuneVarSightings(aTuneVar);
        if (sightings == 0)
        {
            // Error - have not seen this one yet!
            retVal = 1;
            ErrorPrintf("Missing TuneVar %s required for translation#%d:%d\n",
                GetTuneVarName(aTuneVar), translationNumIn.model, translationNumIn.index);

            // Missing TuneVar. Set the error state
            SetTranslationErrorState(1);
        }
    }

    return retVal;
}

// Checks source sightings & calls translation function
// Reset dirty flags only if requested and no error in translation
// returns error state (0 means OK);
int CallTranslation(TranslateEngineIterator translationNumIn, int cleanFlagIfNoError)
{
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (0 == aRecord) return 0; // if translation not found just bail ok

    // remember the error state and then clear it so we can check *this*
    // translation's encounters with error
    unsigned int errorStateBefore = GetTranslationErrorState();
    SetTranslationErrorState(0);
    
    // Get the translation record and run the translation
    int err = CheckTranslationSourceSightingsOK(translationNumIn);
    if (0 == err)
    {
        aRecord->translation_func();
    }
    unsigned int errorStateAfter = GetTranslationErrorState();
    if (0 != errorStateAfter)
    {
        ErrorPrintf("... while processing TuneVars { ");
        int i;
        for (i = 0; i < aRecord->tunevar_count; i++)
        {
            TranslateEngineIterator tuneVar;
            tuneVar.model = translationNumIn.model;
            tuneVar.index = aRecord->tunevars[i];
            ErrorPrintf("%s ", GetTuneVarName(tuneVar));
        }
        ErrorPrintf("}\n");
    }

    // If requested to clean the flag and no errors happened
    if (cleanFlagIfNoError && 0 == errorStateAfter)
    {
        // Clear the dirty flag after a successful translation
        aRecord->updated_tune_vars = 0;
    }
    // If no new errors happened, bring back previous errors
    if (0 == errorStateAfter)
    {
        SetTranslationErrorState(errorStateBefore);
    }

    return GetTranslationErrorState();
}

// Returns characters dumped, or 0 if nothing to dump (i.e. top index too high), negative on buffer too small
int DumpTranslatedTOP(TranslateEngineIterator translationNumIn, int topIndexIn, char* bufferInOut, int bufferSizeIn) 
{ 
    int charsDumped = 0; 
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (aRecord) 
    { 
        if (topIndexIn >= 0 && topIndexIn < aRecord->top_count) 
        { 
            TranslateEngineIterator aTop;
            aTop.model = translationNumIn.model;
            aTop.index = aRecord->tops[topIndexIn];
            charsDumped = DumpTranslatedValue(aTop,bufferInOut,bufferSizeIn);
            if (charsDumped < 0)
            {
                ErrorPrintf("Error: Translation engine return buffer too small for TOP\n");
                SetTranslationErrorState(1);
            }
        } 
    } 
    return charsDumped; 
} 

static void SetTranslationErrorState(unsigned int errorState)
{
    theTranslationErrorState = errorState;
}

// Retrieve translation error state
unsigned int GetTranslationErrorState( void )
{
    return theTranslationErrorState;
}

// Internal use print to error buffer
static void ErrorVPrintf(const char* formatString, va_list args)
{
    char* buffer = &translateErrorBuffer[translateErrorCharsUsed];
    int bufferSize = sizeof(translateErrorBuffer) - translateErrorCharsUsed;
    if (bufferSize)
    {
        int charsPrinted = vsnprintf(
            buffer, bufferSize, formatString, args);
        translateErrorCharsUsed += charsPrinted;
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

// Report a translation error (works like printf)
// Called from the model code
void TranslateError(const char* formatString, ...)
{
    va_list args;
    va_start(args, formatString);
    ErrorVPrintf(formatString, args);
    va_end(args);

    // Add newline and set error state
    ErrorPrintf("\n");
    SetTranslationErrorState(1);
}

// Report a translation warning (works like printf)
// Called from the model code
void TranslateWarning(const char* formatString, ...)
{
    va_list args;
    va_start(args, formatString);
    ErrorVPrintf(formatString, args);
    va_end(args);

    // Add newline and set error state
    ErrorPrintf("\n");
}

// Retrieve reported error strings
const char* GetTranslationErrorStrings( void )
{
    return &translateErrorBuffer[0];
}

// Clear error state
void ClearTranslationErrorState( void )
{
    translateErrorBuffer[0] = 0;
    translateErrorCharsUsed = 0;
    SetTranslationErrorState(0);
}

