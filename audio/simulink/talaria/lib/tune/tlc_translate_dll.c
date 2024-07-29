%% TLC code generation file - hand written source
%% INCLUDED BY: TranslateTrigger.tlc

/*******************************************************************
 *
 *  Translation engine interface via DLL
 *
 *  Copyright (C) Bose Corporation. All rights reserved.
 *******************************************************************/

#define INCLUDE_TRANSLATION_DLL_API
        
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcmp
#include <ctype.h>	// for isdigit

#include "rtwtypes.h"
#include "%<LibGetMdlPubHdrBaseName()>.h" // for TOP_COMPAT
#include "translate_dll.h"

#define DEFAULT_RETURN_BUFFER_SIZE (1 << 20)
char* theReturnBuffer = 0;
int theReturnBufferBytes = 0;

// Allocates buffer for strings returned by the engine.
// Any previous buffer is destroyed
// return 0 for success, 1 for error
int SetReturnBufferSize(int numBytes)
{
    // If the size has not changed, we're all good here
    if (numBytes == theReturnBufferBytes)
    {
        return 0;
    }
    // Otherwise, remove old return buffer
    if (theReturnBuffer)
    {
        free(theReturnBuffer);
        theReturnBuffer = 0;
        theReturnBufferBytes = 0;
    }
    // ... and allocate a new one
    if (numBytes)
    {
        theReturnBuffer = (char*)malloc(numBytes);
        if (theReturnBuffer)
        {
            // Allocation success
            theReturnBufferBytes = numBytes;
        }
        else
        {
            // Allocation failure!
            return 1;
        }        
    }

    return 0;
}

// return 0 on success
int EnsureReturnBuffer(void)
{
    int retVal = 0;
    if (0 == theReturnBufferBytes)
    {
        retVal = SetReturnBufferSize(DEFAULT_RETURN_BUFFER_SIZE);
    }
    return retVal;
}

%if EXISTS(::TALARIA_ENGINE) && (TLC_FALSE == LibIsModelReferenceTarget())
    //-----------------------------------------------------------
    // DLL "EASIER TO FIND" ENTRYPOINT TO INITIALIZE THE MODEL
    //-----------------------------------------------------------
    void InitializeTranslationEngine()
    {
        // Initialize the Simulink generated code
        %<LibCallModelInitialize()>
    }
%endif

//-----------------------------------------------------------
// Retrieve List of TuneVars
//-----------------------------------------------------------
static TranslateEngineIterator listCursor;
// Return TuneVar name or NULL if no more
const char* NextTuneVar()
{
%if EXISTS(::TALARIA_ENGINE) && ::TALARIA_ENGINE == "EMPTY"
    return 0;
%else
    return GetNextTuneVarName(&listCursor);
%endif
}
// Return TuneVar name or NULL if no TuneVars
const char* FirstTuneVar()
{
%if EXISTS(::TALARIA_ENGINE) && ::TALARIA_ENGINE == "EMPTY"
    return 0;
%else
    return GetFirstTuneVarName(&listCursor);
%endif
}

const char* GetTuneVarsForTop( const char* topNameIn)
{
%if EXISTS(::TALARIA_ENGINE) && ::TALARIA_ENGINE == "EMPTY"
    return 0;
%else
    // Fail if unable to obtain a return buffer
    if (EnsureReturnBuffer()) return 0;
    theReturnBuffer[0] = 0;
    int retVal = ListTuneVarsForTop(topNameIn, theReturnBuffer, theReturnBufferBytes);
    if (retVal > theReturnBufferBytes)
    {
        // Error strings were likely set in the call above
    }
    return theReturnBuffer;
%endif
}

//-----------------------------------------------------------
// input value parsing
//-----------------------------------------------------------
char tte_input_buffer[1024];
const char* tteGetToken(const char* inStr, int* iterator)
{
    int isEOF = 0;
    int done = 0;
    char c;
    const char* delimiters = " \t\n\r\v\f,;";
    char* s = tte_input_buffer;
    do
    {
        c = inStr[(*iterator)];
        isEOF = (c == 0);
        if (!isEOF) ++(*iterator);
        if (strchr(delimiters, c) || isEOF)
        {
            // Delimter is found... but we're not done until there
            // is something gathered in the buffer or we've hit EOF
            if (s != tte_input_buffer || isEOF)
            {
                done = 1;
            }
        }
        else
        {
            // If the buffer has room, copy over the character. (otherwise throw away extra characters)
            if ((s - tte_input_buffer) < sizeof(tte_input_buffer))
            {
                *s++ = c;
            }
        }
    } while (done == 0);

    // ASCIIZ terminatation of the token:
    *s = 0;
    return tte_input_buffer;
}

//-----------------------------------------------------------
// Set TuneVar values
//-----------------------------------------------------------
// returns 0 for OK, 1 for error
int SetTuneVar(char* name, char* spaceDelimitedValues)
{
    int retVal = 0;
    TranslateEngineIterator currentTuneVar;
    ClearTuneVarErrorStrings();
    currentTuneVar = RecognizeTuneVariable(name);
    if (false == IsValidTranslateIterator(currentTuneVar))
    {
        retVal = 1;
    }
    else
    {
        const char* token;
        int elementIndex = 0;
        int iterator = 0;
        int tokenCount = 0;
        // Count total number of elements first. This quantity is needed
        // to determine total rows or columns in variable size situations.
        for (token = tteGetToken(spaceDelimitedValues, &iterator);
            *token;
             token = tteGetToken(spaceDelimitedValues, &iterator))
        {
            tokenCount++;
        }
        // Pass on the size to the TuneVar
        retVal = SetTuneVariableSize(currentTuneVar, tokenCount);
        
        iterator = 0;
        for (token = tteGetToken(spaceDelimitedValues, &iterator);
            *token;
             token = tteGetToken(spaceDelimitedValues, &iterator))
        {
            int err = SetTuneVariableElement(currentTuneVar, elementIndex, token);
            elementIndex++;
            if (err)
            {
                retVal = err;
                // Stop processing further TuneVar elements (ignore the rest)
                break;
            }
        }
    }
    return retVal;
}

// returns space-delimited elements or emptyness on error
// use TuneVar error strings to retrieve any errors.
char* GetTuneVar(const char* name)
{
    TranslateEngineIterator currentTuneVar;

    // Fail if unable to obtain a return buffer
    if (EnsureReturnBuffer()) return 0;

    theReturnBuffer[0] = 0;

    ClearTuneVarErrorStrings();
    currentTuneVar = GetTuneVarIteratorByName(name);
    if (false == IsValidTranslateIterator(currentTuneVar))
    {
        return 0;
    }
    else
    {
        int retVal = PrintTuneVarToBuffer(currentTuneVar, theReturnBuffer, theReturnBufferBytes);
        if (retVal > theReturnBufferBytes)
        {
            // Error strings were likely set in the call above
        }
    }
    return theReturnBuffer;
}
    

//-----------------------------------------------------------
// Perform Translations
//-----------------------------------------------------------
// In this design, the DLL offers the ability to pull out translated TOPs.
// Each "dirty" translation is run immediately before returning its TOPs.
TranslateEngineIterator currentTranslation;
int currentTopIndex;

// Return string with TopName TopDataType TopValues, or NULL if no more
const char* GetNextTranslatedTOP()
{
    int err = GetTranslationErrorState();
    int charsDumped = 0;
    // Fail if unable to obtain a return buffer
    if (EnsureReturnBuffer()) return 0;
    theReturnBuffer[0] = 0;
    // the end of the translations is first reached, search from the beginning
    // again to find any translations that were made dirty during TOP retrieval
    int sweptAgain = 0;

    // Keep searching until we find a TOP to dump or run out
    // currentTopIndex counts TOPs *within* a translation. A value of "-1"
    // at the top of the loop indicates that the translation needs to run.
    // At the bottom of the loop, "-1" means it's time for the next translation
    while (IsValidTranslateIterator(currentTranslation) && charsDumped == 0 && err == 0)
    {
        // Run translation if it has not run yet
        if (currentTopIndex == -1)
        {
            // call the translation and clean its dirty flag (if no error)
            err = CallTranslation(currentTranslation, 1);

            // If the translation is OK, start pulling out TOPs
            if (0 == err)
            {
                currentTopIndex = 0;
            }
        }

        // Get the next TOP... but if there are no errors and the
        // translation that brought us this TOP is dirty again,
        // then stop dumping its TOPs and move on. We'll wrap around at
        // the end to get fresh TOPs from this one.
        if ((0 == err) && GetTranslationUpdatedTuneVarCount(currentTranslation))
        {
            // Re-dirtied! Stop dumping TOPs for this translation:
            currentTopIndex = -1;
        }
        if (currentTopIndex >= 0)
        {
            charsDumped = DumpTranslatedTOP(currentTranslation, currentTopIndex,
                theReturnBuffer, theReturnBufferBytes);
            if (charsDumped > 0)
            {
                // We got something. Move on to the next TOP
                currentTopIndex++;
            }
            else if (charsDumped < 0)
            {
                // ran out of buffer space
                break;
            }
            else
            {
                // No more here... move to next translation
                currentTopIndex = -1;
            }
        }

        // If there's no TOP to grab, move on to next translation
        if (currentTopIndex == -1 && err == 0)
        {
            currentTranslation = FindNextNeededTranslation(currentTranslation);
            if ((false == IsValidTranslateIterator(currentTranslation)) && (0 == sweptAgain))
            {
                // When the iteration reaches the end, start again from the
                // beginning again in case TuneVars were received during
                // retrieval of TOPs (i.e. translations we checked earlier
                // have become dirty since). This restart will clear errors
                // but there should be none anyway because we checked above.
                sweptAgain = 1;
                currentTranslation = FindFirstNeededTranslation();
            }
        }
    }

    if (theReturnBuffer[0] == 0) return 0;
    return theReturnBuffer;
}

// Return string with TopName TopDataType TopValues, or NULL if no TOPs
const char* GetFirstTranslatedTOP()
{
    // Clear out the errors to start a new round of retrievals
    ClearTranslationErrorState();
    currentTranslation = FindFirstNeededTranslation();
    currentTopIndex = -1;
    return GetNextTranslatedTOP();
}

//-----------------------------------------------------------
// Retrieval of compatibility hashes
//-----------------------------------------------------------
// For source model builds and other cases where the partition tool has not run
// ... define empty COMPAT values
#ifndef TOP_COMPAT
#define TOP_COMPAT {0}
#endif
#ifndef TOP_COMPAT_NAME_HASH
#define TOP_COMPAT_NAME_HASH {0}
#endif
#ifndef TOP_COMPAT_NAME
#define TOP_COMPAT_NAME {""}
#endif
unsigned int TopCompatNumbers[] = TOP_COMPAT;
unsigned int TopCompatNameHashes[] = TOP_COMPAT_NAME_HASH;
const int NumTopCompatNumbers = sizeof(TopCompatNumbers) / sizeof(TopCompatNumbers[0]);
const char* TopCompatNames[] = TOP_COMPAT_NAME;
// Return compatibility hash number given name hash
unsigned int GetTopCompat(unsigned int nameHash)
{
    unsigned int theCompat = 0;
    int i;
    for (i=0; i < NumTopCompatNumbers; i++)
    {
        if (nameHash == TopCompatNameHashes[i])
        {
            theCompat = TopCompatNumbers[i];
            break;
        }
    }
    return theCompat;
}
// Return compatibility hash number
unsigned int GetTopCompatByName(const char* featureName)
{
    unsigned int theCompat = 0;
    int i;
    for (i=0; i < NumTopCompatNumbers; i++)
    {
        if (0 == strcmp(featureName,TopCompatNames[i]))
        {
            theCompat = TopCompatNumbers[i];
            break;
        }
    }
    return theCompat;
}
