/*
 * File: TranslateError.h
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

#ifndef RTW_HEADER_TranslateError_h_
#define RTW_HEADER_TranslateError_h_

// MATLAB's loadlibrary does not support variadic functions (...)
// and gives the warning that looks something like:
//   Warning: The data type 'error' used by function TranslateError does not exist.
// So to eliminate these warnings, the following variadic functions
// have been removed from BasicAudio_TranslationEngine64_TranslateTrigger.h which is used
// to load translation DLLs into MATLAB. Instead, these functions
// are declared here out of the way.

// Report a translation error (works like printf)
// Called from the model code
void TranslateError(const char* formatString, ...);
void TranslateWarning(const char* formatString, ...);

#endif                                 /* RTW_HEADER_TranslateError_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
