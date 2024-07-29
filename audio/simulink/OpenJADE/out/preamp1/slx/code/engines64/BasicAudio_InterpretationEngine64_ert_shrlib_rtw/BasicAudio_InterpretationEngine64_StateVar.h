/*
 * File: BasicAudio_InterpretationEngine64_StateVar.h
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

#ifndef RTW_HEADER_BasicAudio_InterpretationEngine64_StateVar_h_
#define RTW_HEADER_BasicAudio_InterpretationEngine64_StateVar_h_
#include "BasicAudio_InterpretationEngine64_InterpretTrigger.h" // for InterpretEngineIterator
#ifdef INCLUDE_INTERPRETATION_DLL_API

// Returns a state variable record if name is recognized, INVALID_STATEVAR otherwise
// Also "touches" any interpretation that will be impacted by changing the found variable
InterpretEngineIterator RequestStateVariable(const char* variableName);

#endif

// Reset all StateVar requested flags to zero
void ClearAllStateRequests( void );

// Return whether the specified StateVar has been requested
int GetStateVarRequested(InterpretEngineIterator stateVarNumberIn);

// Return StateVar name (or NULL if bad number)
const char* GetStateVarName(InterpretEngineIterator stateVarNumberIn);
const char* GetFirstStateVarName(InterpretEngineIterator* stateVarNumberIn);
const char* GetNextStateVarName(InterpretEngineIterator* stateVarNumberIn);

// Receive a new interpreted value, resetting request flag & printing value to buffer
// returns char count placed in buffer
int ReceiveInterpretation(InterpretEngineIterator rcvStateVarNumIn, char
    * bufferIn, int bufferSizeIn);

// Return the first requested record to run (or INVALID_STATEVAR if none)
InterpretEngineIterator FindFirstRequestedStateVar( void );

// Return the next StateVar record that is requested (or INVALID_INTERPRET_ITERATOR if no more)
InterpretEngineIterator FindNextRequestedStateVar(InterpretEngineIterator
    afterThisOne);

// Local model StateVar table
extern void* BasicAudio_InterpretationEngine64_state_var_table;
extern int BasicAudio_InterpretationEngine64_state_var_table_count;

#endif            /* RTW_HEADER_BasicAudio_InterpretationEngine64_StateVar_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
