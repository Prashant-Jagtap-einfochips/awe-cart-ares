/*
 * File: BasicAudio_InterpretationEngine64_InterpretTrigger.h
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

#ifndef RTW_HEADER_BasicAudio_InterpretationEngine64_InterpretTrigger_h_
#define RTW_HEADER_BasicAudio_InterpretationEngine64_InterpretTrigger_h_

// ------------------------------------------------
// Structure to access engine parts from each model
typedef struct InterpretationRoot_tag
{
    void* statevar_table;
    void* interpretation_table;
    void* tsp_interpret_table;
    int* statevar_count;
    int* interpretation_count;
    int* tsp_interpret_table_count;
}
InterpretationRoot;

// ------------------------------------------------
// Iterator for use with StateVars and Interpretation
typedef struct
{
    int model;
    int index;
}

InterpretEngineIterator;

extern InterpretEngineIterator INVALID_INTERPRET_ITERATOR;
int IsValidInterpretIterator(InterpretEngineIterator anIterator);

// Share the interpretation roots with the rest of our engine
extern InterpretationRoot* interpretationRoots[];
extern int numInterpretationRoots;

// Register engine
void RegisterInterpretationRoot(InterpretationRoot* aRoot);

#ifdef INCLUDE_INTERPRETATION_DLL_API

// Retrieve space-delimited list of TSPs needed for specified interpretation
// Returns char count placed in buffer
int ListTSPsForInterpretation(InterpretEngineIterator interpretationIn, char
    * bufferIn, int bufferSizeIn);

// Return the first interpretation record that has been touched to run (or -1 if none)
InterpretEngineIterator FindFirstNeededInterpretation( void );

// Return the next interpretation record that has been touched to run (or -1 if no more)
InterpretEngineIterator FindNextNeededInterpretation(InterpretEngineIterator
    afterThisOne);

#endif

// Verify that all TSPs read by the specified interpretation have been seen
// returns 0 for OK, nonzero is error
int CheckInterpretationSourceSightingsOK(InterpretEngineIterator
    interpretationNumIn);

// Checks source sightings & calls interpretation function, but does *not* reset dirty flags
// returns error state (0 means OK);
int CallInterpretation(InterpretEngineIterator interpretationNumIn);

// Touch a interpretation object to indicate source tuning variables
// ... have changed. This indicates the interpretation must be run.
void TouchInterpretation(InterpretEngineIterator interpretationNumIn);

// Clear all flags that would indicate a interpretation must be run
void ClearAllInterpretationRequiredFlags( void );

// Find all touched interpretations and list their source TSPs
const char* GetTspsForStateVar(const char* stateVarName);

// Retrieve interpretation error state
unsigned int GetInterpretationErrorState( void );

// Retrieve reported error strings
const char* GetInterpretationErrorStrings( void );

// Clear errors and error strings
void ClearInterpretationErrorState( void );

#endif    /* RTW_HEADER_BasicAudio_InterpretationEngine64_InterpretTrigger_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
