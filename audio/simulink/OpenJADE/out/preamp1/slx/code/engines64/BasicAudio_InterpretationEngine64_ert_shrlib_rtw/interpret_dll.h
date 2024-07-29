/*
 * File: interpret_dll.h
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

#ifndef RTW_HEADER_interpret_dll_h_
#define RTW_HEADER_interpret_dll_h_
#include "BasicAudio_InterpretationEngine64_InterpretTrigger.h"
#include "BasicAudio_InterpretationEngine64_TSP_Interpret.h"
#include "BasicAudio_InterpretationEngine64_StateVar.h"

// DLL Interface prototypes:
// ---- Initialize the engine at startup
void InitializeInterpretationEngine( void );

// ---- Retrieve list of StateVar names
const char* FirstStateVar( void );
const char* NextStateVar( void );

// ---- Get get names of required tsps
const char* GetTspsForStateVar(const char* stateVarName);

// ---- Set tsp value
int SetTsp(const char* name, char* spaceDelimitedValues);
const char* CalculateStateVar( void );

// ---- Retrieve TSP COMPAT numbers
unsigned int GetTspCompat(unsigned int nameHash);
unsigned int GetTspCompatByName(const char* featureName);

// ---- Error state and strings
unsigned int GetInterpretationErrorState( void );
const char* GetInterpretationErrorStrings( void );
void ClearInterpretationErrorState( void );
const char* GetTSPErrorStrings( void );
void ClearTSPErrorStrings( void );

// ---- Clear various flags
void ClearAllTSPSightings( void );
void ClearAllStateRequests( void );
void ClearAllInterpretationRequiredFlags( void );

#endif                                 /* RTW_HEADER_interpret_dll_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
