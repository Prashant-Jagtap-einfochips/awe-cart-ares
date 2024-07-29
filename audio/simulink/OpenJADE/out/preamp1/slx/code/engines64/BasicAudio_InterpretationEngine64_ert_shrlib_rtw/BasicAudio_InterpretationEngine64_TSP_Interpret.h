/*
 * File: BasicAudio_InterpretationEngine64_TSP_Interpret.h
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

#ifndef RTW_HEADER_BasicAudio_InterpretationEngine64_TSP_Interpret_h_
#define RTW_HEADER_BasicAudio_InterpretationEngine64_TSP_Interpret_h_
#include "BasicAudio_InterpretationEngine64_InterpretTrigger.h" // for InterpretEngineIterator
#ifdef INCLUDE_INTERPRETATION_DLL_API

// Returns a TSP iterator if name is recognized, INVALID_INTERPRET_ITERATOR otherwise
InterpretEngineIterator RecognizeTSP(const char* tspName);

#endif

// Reset all TSP sighting counts to zero
void ClearAllTSPSightings( void );

// Return count of how many times the specified TSP has been seen
int GetTSPSightings(InterpretEngineIterator tspNumberIn);

// Return TSP name (or NULL if bad number)
const char* GetTSPName(InterpretEngineIterator tspNumberIn);

// Set a single value of the TSP (index traverses by column first)
// For an N row by M column matrix using zero-based row and col, indexIn = row + (col * N);
// Returns 0 for OK, nonzero for error (does not set error state)
int SetTSPElement(InterpretEngineIterator tspNumberIn, int indexIn, const char
                  * stringElementValueIn);

// Retrieve reported error strings
const char* GetTSPErrorStrings( void );

// Clear error strings (call when starting new operation)
void ClearTSPErrorStrings( void );

// Local model StateVar table
extern void* BasicAudio_InterpretationEngine64_tsp_interpret_table;
extern int BasicAudio_InterpretationEngine64_tsp_interpret_table_count;

#endif       /* RTW_HEADER_BasicAudio_InterpretationEngine64_TSP_Interpret_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
