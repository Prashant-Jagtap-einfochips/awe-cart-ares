/*
 * File: BasicAudio_TranslationEngine64_TranslateTrigger.h
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

#ifndef RTW_HEADER_BasicAudio_TranslationEngine64_TranslateTrigger_h_
#define RTW_HEADER_BasicAudio_TranslationEngine64_TranslateTrigger_h_

// ------------------------------------------------
// Structure to access engine parts from each model
typedef struct TranslationRoot_tag
{
    void* tunevar_table;
    void* translation_table;
    void* top_table;
    int* tunevar_count;
    int* translation_count;
    int* top_count;
}
TranslationRoot;

// ------------------------------------------------
// Iterator for use with TuneVars and Translations
typedef struct
{
    int model;
    int index;
}

TranslateEngineIterator;

extern TranslateEngineIterator INVALID_TRANSLATE_ITERATOR;
int IsValidTranslateIterator(TranslateEngineIterator anIterator);

// Share the translation roots with the rest of our engine
extern TranslationRoot* translationRoots[];
extern int numTranslationRoots;

// Register engine
void RegisterTranslationRoot(TranslationRoot* aRoot);

#ifdef INCLUDE_TRANSLATION_DLL_API

// Return the first translation record that has been touched to run (or INVALID_TRANSLATE_ITERATOR if none)
TranslateEngineIterator FindFirstNeededTranslation( void );

// Return the next translation record that has been touched to run (or INVALID_TRANSLATE_ITERATOR if no more)
TranslateEngineIterator FindNextNeededTranslation(TranslateEngineIterator
    afterThisOne);

#endif

// Verify that all TuneVars read by the specified translation have been seen
// Returns 0 if OK; nonzero if error and sets translation error state
int CheckTranslationSourceSightingsOK(TranslateEngineIterator translationNumIn);

// Returns space-delimited list of TuneVars (in bufferInOut) that feed specified top
// Returns char count placed in buffer
int ListTuneVarsForTop(const char* topNameIn, char* bufferInOut, int
                       bufferSizeIn);

// Touch a translation object to indicate source tuning variables
// ... have changed. This indicates the translation must be run.
void TouchTranslation(TranslateEngineIterator translationNumIn);

// Get a translation's dirty flag in the form of updated TuneVar count
int GetTranslationUpdatedTuneVarCount(TranslateEngineIterator translationNumIn);

// Clear all flags that would indicate a translation must be run
void ClearAllTranslationRequiredFlags( void );

// Checks source sightings & calls translation function
// Reset dirty flags only if requested and no error in translation
// returns error state (0 means OK);
int CallTranslation(TranslateEngineIterator translationNumIn,int
                    cleanFlagIfNoError);

// Returns characters dumped, or 0 if nothing to dump (i.e. top index too high)
int DumpTranslatedTOP(TranslateEngineIterator translationNumIn, int topIndexIn,
                      char* bufferInOut, int bufferSizeIn);

// Retrieve translation error state
unsigned int GetTranslationErrorState( void );

// Retrieve reported error strings
const char* GetTranslationErrorStrings( void );

// Clear translation errors and error strings
void ClearTranslationErrorState( void );

#endif       /* RTW_HEADER_BasicAudio_TranslationEngine64_TranslateTrigger_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
