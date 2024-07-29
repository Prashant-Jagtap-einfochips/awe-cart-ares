/*
 * File: BasicAudio_InterpretationEngine64.h
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

#ifndef RTW_HEADER_BasicAudio_InterpretationEngine64_h_
#define RTW_HEADER_BasicAudio_InterpretationEngine64_h_
#ifndef BasicAudio_InterpretationEngine64_COMMON_INCLUDES_
#define BasicAudio_InterpretationEngine64_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "BasicAudio_InterpretationEngine64_TalariaAPI.h"
#include "BasicAudio_InterpretationEngine64_InterpretError.h"
#endif                  /* BasicAudio_InterpretationEngine64_COMMON_INCLUDES_ */

#include "BasicAudio_InterpretationEngine64_types.h"
#include <stddef.h>
#include <string.h>

/* Includes for objects with custom storage classes */
#include "tsp_capture_settings.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* user code (top of header file) */
// Auto-generated code for model BasicAudio
/**** TSP TOKEN COMPATIBILITY HASH VALUES ****/
#define TSP_COMPAT                     {0xD202EF8DUL}
#define TSP_COMPAT_NAME                {"BasicAudio"}
#define TSP_COMPAT_NAME_HASH           {0x9FD22631UL}

/**** COMPAT HASH VALUES BY MODULE ****/
#define BASICAUDIO_TSP_COMPAT          0xD202EF8DUL

/**** NAME HASH VALUES BY MODULE ****/
#define BASICAUDIO_TSP_NAME_HASH       0x9FD22631UL

/* Block signals (default storage) */
typedef struct
{
    real32_T TSP;                      /* '<S2>/TSP' */
    real32_T State;                    /* '<S2>/TrivialInterpretation' */
}
B_BasicAudio_InterpretationEngine64_T;

/* Real-time Model Data Structure */
struct tag_RTM_BasicAudio_InterpretationEngine64_T
{
    const char_T * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_BasicAudio_InterpretationEngine64_T BasicAudio_InterpretationEngine64_B;

/* Model entry point functions */
extern void BasicAudio_InterpretationEngine64_initialize(void);
extern void BasicAudio_InterpretationEngine64_step(void);
extern void BasicAudio_InterpretationEngine64_terminate(void);

/* Real-time Model object */
extern RT_MODEL_BasicAudio_InterpretationEngine64_T *const
    BasicAudio_InterpretationEngine64_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BasicAudio_InterpretationEngine64'
 * '<S1>'   : 'BasicAudio_InterpretationEngine64/CoreIndex'
 * '<S2>'   : 'BasicAudio_InterpretationEngine64/Empty'
 * '<S3>'   : 'BasicAudio_InterpretationEngine64/Empty/InterpretTrigger'
 * '<S4>'   : 'BasicAudio_InterpretationEngine64/Empty/TrivialInterpretation'
 * '<S5>'   : 'BasicAudio_InterpretationEngine64/Empty/InterpretTrigger/ifunc'
 */
#endif                     /* RTW_HEADER_BasicAudio_InterpretationEngine64_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
