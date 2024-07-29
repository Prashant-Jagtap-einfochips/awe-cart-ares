/*
 * File: BasicAudio_TranslationEngine64_TuneVar.h
 *
 * Code generated for Simulink model 'BasicAudio_TranslationEngine64'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:48:46 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_BasicAudio_TranslationEngine64_TuneVar_h_
#define RTW_HEADER_BasicAudio_TranslationEngine64_TuneVar_h_
#include "BasicAudio_TranslationEngine64_TranslateTrigger.h" // for TranslateEngineIterator
#ifdef INCLUDE_TRANSLATION_DLL_API

// Returns a tune variable iterator if name is recognized, -1 otherwise
// Adds to error strings if name is not recognized (see GetTuneVarErrorStrings)
// Also "touches" any translations that will be impacted by changing the found variable
TranslateEngineIterator RecognizeTuneVariable(const char* variableName);

// Returns a tune variable iterator if name is recognized, -1 otherwise
// Instead use "RecognizeTuneVariable" if the TuneVar will be changed
TranslateEngineIterator GetTuneVarIteratorByName(const char* variableName);

#endif

// Reset all TuneVar sighting counts to zero
void ClearAllTuneSightings(void);

// Return count of how many times the specified TuneVar has been seen
int GetTuneVarSightings(TranslateEngineIterator tuneVarNumberIn);

// Return TuneVar name (or NULL if bad number)
const char* GetFirstTuneVarName(TranslateEngineIterator* tuneVarNumberIn);
const char* GetNextTuneVarName(TranslateEngineIterator* tuneVarNumberIn);
const char* GetTuneVarName(TranslateEngineIterator tuneVarNumberIn);

// Set incoming TuneVar size - required for all TuneVars to account
// for variable size TuneVars and conversion from row-major to
// column-major storage indexing.
// Returns 0 for OK, 1 for error (does not set translation error state)
// ... but DOES set TuneVar error messages
int SetTuneVariableSize(TranslateEngineIterator tuneVarNumberIn, int countIn);

// Set a single value of the tune variable. Provide total count of
// values in addition to the index of current value to set. The index
// traverses the TuneVar storage in row-major organization which is
// typical for C language matrix storage (i.e. same as the way a matrix
// is represented in a text file... listing out each row contiguously).
// Returns 0 for OK, 1 for error (does not set translation error state)
int SetTuneVariableElement(TranslateEngineIterator tuneVarNumberIn, int
    rowMajorIndexIn,
    const char* stringElementValueIn);

// returns num chars used, negative if buffer was too small, 0 if TuneVar not recognized
int PrintTuneVarToBuffer(TranslateEngineIterator tuneVarNumberIn, char
    * returnBuffer, int returnBufferSize);

// Retrieve reported error strings
const char* GetTuneVarErrorStrings(void);

// Clear error strings (call when starting new operation)
void ClearTuneVarErrorStrings(void);

// Load default values for all TuneVars
void LoadAllTuneVarDefaults(void);

// Local model TuneVar table
extern void* BasicAudio_TranslationEngine64_tune_var_table;
extern int BasicAudio_TranslationEngine64_tune_var_table_count;

#endif                /* RTW_HEADER_BasicAudio_TranslationEngine64_TuneVar_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
