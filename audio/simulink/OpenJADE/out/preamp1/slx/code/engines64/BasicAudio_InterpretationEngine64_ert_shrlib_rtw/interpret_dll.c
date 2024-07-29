/*
 * File: interpret_dll.c
 *
 * Code generated for Simulink model 'BasicAudio_InterpretationEngine64'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:49:20 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

/*******************************************************************
 *
 *  Interpretationengine interface via DLL
 *
 *  Copyright (C) Bose Corporation. All rights reserved.
 *******************************************************************/
#define INCLUDE_INTERPRETATION_DLL_API
#include <stdio.h>
#include <stdlib.h>
#include <string.h>                    // for strcmp
#include "rtwtypes.h"
#include "BasicAudio_InterpretationEngine64.h" // for TSP_COMPAT
#include "interpret_dll.h"

//-----------------------------------------------------------
// DLL "EASIER TO FIND" ENTRYPOINT TO INITIALIZE THE MODEL
//-----------------------------------------------------------
void InitializeInterpretationEngine()
{
    // Initialize the Simulink generated code
    BasicAudio_InterpretationEngine64_initialize();
}

//-----------------------------------------------------------
// DLL interface storage
//-----------------------------------------------------------
// Buffer for returning strings
char outputBuffer[32768];

// Cursor/interator used to retrieve StateVar list
static InterpretEngineIterator listCursor;

// Tracking of currently requested StateVar
InterpretEngineIterator requestedStateVar =
{
    -1, -1
};

//-----------------------------------------------------------
// Retrieve list of StateVars
//-----------------------------------------------------------
const char* NextStateVar()
{
    return 0;
}

// Return StateVar name or NULL if no StateVars
const char* FirstStateVar()
{
    return 0;
}

//-----------------------------------------------------------
// Request a StateVar (buy asking about its required TSPs)
//-----------------------------------------------------------
// Returns space delimited list of TSP value needed to retrieve StateVar
// (or empty if the StateVar is not recognized)
const char* GetTspsForStateVar(const char* stateVarName)
{
    outputBuffer[0] = 0;

    // Clear out all the interpretation flags
    ClearAllInterpretationRequiredFlags();

    // Recognize the StateVar and flag to it be requested
    requestedStateVar = RequestStateVariable(stateVarName);
    if (IsValidInterpretIterator(requestedStateVar))
    {
        // Dig out TSPs from the interpretation that was flagged above
        InterpretEngineIterator theInterpretation =
            FindFirstNeededInterpretation();
        ListTSPsForInterpretation(theInterpretation,outputBuffer, sizeof
            (outputBuffer));
    }

    return outputBuffer;
}

//-----------------------------------------------------------
// input value parsing
//-----------------------------------------------------------
char tie_input_buffer[1024];
const char* tieGetToken(const char* inStr, int* iterator)
{
    int isEOF = 0;
    int done = 0;
    char c;
    const char* delimiters = " \t\n\r\v\f,;";
    char* s = tie_input_buffer;
    do
    {
        c = inStr[(*iterator)];
        isEOF = (c == 0);
        if (!isEOF)
            ++(*iterator);
        if (strchr(delimiters, c) || isEOF)
        {
            // Delimter is found... but we're not done until there
            // is something gathered in the buffer or we've hit EOF
            if (s != tie_input_buffer || isEOF)
            {
                done = 1;
            }
        }
        else
        {
            // If the buffer has room, copy over the character. (otherwise throw away extra characters)
            if ((s - tie_input_buffer) < sizeof(tie_input_buffer))
            {
                *s++ = c;
            }
        }
    }
    while (done == 0);

    // ASCIIZ terminatation of the token:
    *s = 0;
    return tie_input_buffer;
}

//-----------------------------------------------------------
// Set TSP values in preparation for StateVar calculation
//-----------------------------------------------------------
// returns 0 for OK, 1 for error, 2 for warning
int SetTsp(const char* name, char* spaceDelimitedValues)
{
    int retVal = 0;
    InterpretEngineIterator currentTSP;
    ClearTSPErrorStrings();
    currentTSP = RecognizeTSP(name);
    if (false == IsValidInterpretIterator(currentTSP))
    {
        retVal = 1;
    }
    else
    {
        const char* token;
        int elementIndex = 0;
        int iterator = 0;
        for (token = tieGetToken(spaceDelimitedValues, &iterator);
             *token;
             token = tieGetToken(spaceDelimitedValues, &iterator))
        {
            int err = SetTSPElement(currentTSP, elementIndex, token);
            elementIndex++;
            if (err)
            {
                retVal = err;
                break;
            }
        }
    }

    return retVal;
}

//-----------------------------------------------------------
// Calculate and retrieve the StateVar value
//-----------------------------------------------------------
// Returns StateVar previously setup by GetTsps... and SetTsp
const char* CalculateStateVar()
{
    outputBuffer[0] = 0;
    ClearInterpretationErrorState();
    InterpretEngineIterator theInterpretation = FindFirstNeededInterpretation();
    int err = CallInterpretation(theInterpretation);
    if (err == 0)
    {
        ReceiveInterpretation(requestedStateVar,outputBuffer,sizeof(outputBuffer));
    }

    return outputBuffer;
}

//-----------------------------------------------------------
// Retrieval of compatibility hashes
//-----------------------------------------------------------
// For source model builds and other cases where the partition tool has not run
// ... define empty COMPAT values
#ifndef TSP_COMPAT
#define TSP_COMPAT                     {0}
#endif

#ifndef TSP_COMPAT_NAME_HASH
#define TSP_COMPAT_NAME_HASH           {0}
#endif

#ifndef TSP_COMPAT_NAME
#define TSP_COMPAT_NAME                {""}
#endif

unsigned int TspCompatNumbers[] = TSP_COMPAT;
unsigned int TspCompatNameHashes[] = TSP_COMPAT_NAME_HASH;
const int NumTspCompatNumbers = sizeof(TspCompatNumbers) / sizeof
    (TspCompatNumbers[0]);
const char* TspCompatNames[] = TSP_COMPAT_NAME;

// Return compatibility hash number given name hash
unsigned int GetTspCompat(unsigned int nameHash)
{
    unsigned int theCompat = 0;
    int i;
    for (i=0; i < NumTspCompatNumbers; i++)
    {
        if (nameHash == TspCompatNameHashes[i])
        {
            theCompat = TspCompatNumbers[i];
            break;
        }
    }

    return theCompat;
}

// Return compatibility hash number
unsigned int GetTspCompatByName(const char* featureName)
{
    unsigned int theCompat = 0;
    int i;
    for (i=0; i < NumTspCompatNumbers; i++)
    {
        if (0 == strcmp(featureName,TspCompatNames[i]))
        {
            theCompat = TspCompatNumbers[i];
            break;
        }
    }

    return theCompat;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
