/*
 * File: translate_dll.h
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

#ifndef RTW_HEADER_translate_dll_h_
#define RTW_HEADER_translate_dll_h_
#include "BasicAudio_TranslationEngine64_TranslateTrigger.h"
#include "BasicAudio_TranslationEngine64_TOP_RcvTrans.h"
#include "BasicAudio_TranslationEngine64_TuneVar.h"

// DLL Interface prototypes:
// ---- Initialize the engine at startup
void InitializeTranslationEngine( void );

// ---- Optional override of buffer size for returned strings (return 0 on failure)
int SetReturnBufferSize(int numBytes);

// ---- Retrieve list of TuneVar names
const char* FirstTuneVar( void );
const char* NextTuneVar( void );
const char* GetTuneVarsForTop( const char* topNameIn);

// ---- Set TuneVar values: returns 0 for OK, 1 for error
int SetTuneVar(char* name, char* spaceDelimitedValues);
void LoadAllTuneVarDefaults(void);

// ---- Get TuneVar values:
char* GetTuneVar(const char* name);

// ---- Retrieve translated TOP values
const char* GetFirstTranslatedTOP( void );
const char* GetNextTranslatedTOP( void );

// ---- Error state and strings
unsigned int GetTranslationErrorState( void );
const char* GetTranslationErrorStrings( void );
void ClearTranslationErrorState( void );
const char* GetTuneVarErrorStrings(void);

// ---- Clear various flags
void ClearAllTranslationRequiredFlags( void );
void ClearAllTuneSightings(void);

// ---- Retrieve TOP COMPAT numbers
unsigned int GetTopCompat(unsigned int nameHash);
unsigned int GetTopCompatByName(const char* featureName);

#endif                                 /* RTW_HEADER_translate_dll_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
