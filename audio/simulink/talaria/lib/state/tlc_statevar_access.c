%% tlc_statevar_access.c included by StateVarSFunc.tlc

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


static StateVar_type* GetStateVarRecord(InterpretEngineIterator iteratorIn)
{
    if ((iteratorIn.model < 0) || (iteratorIn.model >= numInterpretationRoots))
    {
        return 0; // model out of range
    }
    InterpretationRoot* theRoot = interpretationRoots[iteratorIn.model];
    if ((iteratorIn.index < 0) || (iteratorIn.index >= *theRoot->statevar_count))
    {
        return 0; // index out of range for specified model
    }
    StateVar_type* theTable = *(StateVar_type**)theRoot->statevar_table;
    return &theTable[iteratorIn.index];
}

// Return a state variable iterator if name is recognized, INVALID_INTERPRET_ITERATOR otherwise
InterpretEngineIterator RequestStateVariable(const char* variableName)
{
    InterpretEngineIterator retVal = INVALID_INTERPRET_ITERATOR;
    InterpretEngineIterator iterator;
    StateVar_type* theVar = 0;
    StateVar_type* aRecord;

    // Loop through table searching for a variable name match
    int m;
    for (m = 0; (m < numInterpretationRoots) && (theVar == 0); m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for(aRecord = GetStateVarRecord(iterator); aRecord;
            aRecord = GetStateVarRecord(iterator))
        {
            if (insensitiveStrcmp(variableName, aRecord->name))
            {
                // Found it!
                theVar = aRecord;
                retVal = iterator;
                break;
            }
            iterator.index++;
        }
    }
    // If a variable was found, mark it as requested and flag the interpretation
    if (theVar)
    {
        theVar->requested++;
        InterpretEngineIterator anInterpretation;
        anInterpretation.model = iterator.model;
        anInterpretation.index = theVar->interpretation;
        TouchInterpretation(anInterpretation);
    }
    return retVal;
}

// Reset all StateVar requested flags to zero
void ClearAllStateRequests()
{
    InterpretEngineIterator iterator;
    StateVar_type* aRecord = 0;
    int m;
    for (m = 0; m < numInterpretationRoots; m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for(aRecord = GetStateVarRecord(iterator); aRecord;
            aRecord = GetStateVarRecord(iterator))
        {
            aRecord->requested = 0;
            iterator.index++;
        }
    }
}
// Return whether the specified StateVar has been requested
int GetStateVarRequested(InterpretEngineIterator stateVarNumberIn)
{
    int requested = 0;
    StateVar_type* aRecord = GetStateVarRecord(stateVarNumberIn);
    if (aRecord)
    {
        requested = aRecord->requested;
    }
    return requested;
}

// Return StateVar name (or NULL if bad number)
const char* GetStateVarName(InterpretEngineIterator stateVarNumberIn)
{
    const char* name = 0;
    StateVar_type* aRecord = GetStateVarRecord(stateVarNumberIn);
    if (aRecord)
    {
        name = aRecord->name;
    }
    return name;
}

const char* GetFirstStateVarName(InterpretEngineIterator* stateVarNumberIn)
{
    if (!stateVarNumberIn) return 0;
    stateVarNumberIn->model = 0;
    stateVarNumberIn->index = 0;
    const char* aName = GetStateVarName(*stateVarNumberIn);
    if (aName == 0)
    {
        // First model had none. Try next:
        return GetNextStateVarName(stateVarNumberIn);
    }
    return aName;
}
const char* GetNextStateVarName(InterpretEngineIterator* stateVarNumberIn)
{
    if (!stateVarNumberIn) return 0;

    // Check for next name in current model
    stateVarNumberIn->index++;
    const char* aName = GetStateVarName(*stateVarNumberIn);

    // If no more - try the next model
    if (0 == aName)
    {
        // Name not found - so try the next model(s)
        stateVarNumberIn->index = 0;
        while ((0 == aName) && (++(stateVarNumberIn->model) < numInterpretationRoots))
        {
            // name not found - so try the next model
            aName = GetStateVarName(*stateVarNumberIn);
        }
    }
    return aName;
}

// TYPED_PRINT performs the print
// Assumed:srcArray,numElem,buf,bufSize
#define TYPED_PRINT(STYPE,FMTSTR)\
{int index; \
for (index = 0; index < numElem; index++){\
    STYPE srcVal = ((STYPE *)srcArray)[index];\
    if (bufSize > 1){\
        int used=0;\
        if(srcFix.factor){\
            used = snprintf(buf, bufSize, " %lf", (double)(srcVal)*srcFix.factor);\
        } else {\
            used = snprintf(buf, bufSize, FMTSTR, srcVal);\
        }\
        bufSize -= used;\
        buf += used;\
    }\
}}

%% pull in code for FixedInfo and DecodeFixedTypeName(const char* typeName)
%include "fixdtNameParse_c"

// Receive a new interpreted value, resetting request flag & printing value to buffer
// returns char count placed in buffer
int ReceiveInterpretation(InterpretEngineIterator stateVarNumberIn, char* bufferIn, int bufferSizeIn)
{
    // bail out if the specified StateVar receiver is not found
    StateVar_type* aRecord = GetStateVarRecord(stateVarNumberIn);
    if (!aRecord) return 0;



    // Clear out the request. We have received it!
    aRecord->requested = 0;

    // Receive the input datatype array & print
    SimulinkDatatypeId dTypeId = aRecord->dtype_id;
    void* srcArray = aRecord->value_ptr;
    int numElem = aRecord->width;
    char* buf = bufferIn;
    int bufSize = bufferSizeIn;
    int nameChars = 0;
    FixedInfo srcFix = DecodeFixedTypeName(aRecord->dtype_name);
    if (srcFix.factor)
    {
        // When decoding a fixed point type, set the dTypeId to the
        // integer format of the data. Then... the srcfix.factor will
        // be used to convert to floating point
        dTypeId = srcFix.intDTypeId;
        // output the fixed point type name
        nameChars = snprintf(buf, bufSize, "%s %s", aRecord->name, aRecord->dtype_name);
    }
    else
    {
        // for regular types, use our all-caps type names
        nameChars = snprintf(buf, bufSize, "%s %s", aRecord->name, dtypeStr[aRecord->dtype_id]);
    }
    bufSize -= nameChars;
    buf += nameChars;
    switch (dTypeId)
    {
    case TSS_DOUBLE: {TYPED_PRINT(double, " %lf")}break;
    case TSS_SINGLE: {TYPED_PRINT(float, " %f")}break;
    case TSS_INT8: {TYPED_PRINT(int8_T, " %i")}break;
    case TSS_UINT8: {TYPED_PRINT(uint8_T, " %u")}break;
    case TSS_INT16: {TYPED_PRINT(int16_T, " %i")}break;
    case TSS_UINT16: {TYPED_PRINT(uint16_T, " %u")}break;
    case TSS_INT32: {TYPED_PRINT(int32_T, " %i")}break;
    case TSS_UINT32: {TYPED_PRINT(uint32_T, " %u")}break;
    case TSS_BOOLEAN: {TYPED_PRINT(int8_T, " %i")}break;
    }
    return (bufferSizeIn - bufSize);
}

// Return the first StateVar  that has been requested (or INVALID_INTERPRETATION if none)
InterpretEngineIterator FindFirstRequestedStateVar()
{
    InterpretEngineIterator retVal = INVALID_INTERPRET_ITERATOR;
    InterpretEngineIterator theFirst;
    int m;
    StateVar_type* aRecord;
    for (m = 0; m < numInterpretationRoots; m++)
    {
        theFirst.model = m;
        theFirst.index = 0;
        for (aRecord = GetStateVarRecord(theFirst); aRecord;
            aRecord = GetStateVarRecord(theFirst))
        {
            if (aRecord->requested != 0)
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

// Return the next StateVar that has been requested (or -1 if no more)
InterpretEngineIterator FindNextRequestedStateVar(InterpretEngineIterator afterThisOne)
{
    InterpretEngineIterator retVal = INVALID_INTERPRET_ITERATOR;
    InterpretEngineIterator nextOne;
    nextOne.index = afterThisOne.index + 1;
    int m;
    int foundIt = 0;
    StateVar_type* aRecord;
    for (m = afterThisOne.model; (0 == foundIt) && (m < numInterpretationRoots); m++)
    {
        nextOne.model = m;
        for (aRecord = GetStateVarRecord(nextOne); aRecord;
            aRecord = GetStateVarRecord(nextOne))
        {
            if (aRecord->requested != 0)
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
