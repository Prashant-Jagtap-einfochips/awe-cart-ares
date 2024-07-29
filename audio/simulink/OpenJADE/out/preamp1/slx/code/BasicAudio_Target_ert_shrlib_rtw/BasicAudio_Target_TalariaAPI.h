/*
 * File: BasicAudio_Target_TalariaAPI.h
 *
 * Code generated for Simulink model 'BasicAudio_Target'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:46:55 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_BasicAudio_Target_TalariaAPI_h_
#define RTW_HEADER_BasicAudio_Target_TalariaAPI_h_
#include <stdint.h>

// ------------------------------------------------------------
// MODEL SYNCHRONOUS PROCESSING INTERFACE
// ------------------------------------------------------------
#define MODEL_FUNC(fname)              BasicAudio_Target_ ## fname
#define MODEL_HEADER_FILENAME          "BasicAudio_Target.h"
#define MODEL_SYNC_TASK_COUNT          1
#define MODEL_RUN_FLAGS_UPDATE(flags)
#define MODEL_STEP(TID)                MODEL_FUNC(step)();
#define MODEL_TASK_RATE_FACTORS        1
#define MODEL_TASK_SAMPLE_PERIODS      0.00066666666666666664
#define MODEL_TASK_SAMPLE_OFFSETS      0.0

// ------------------------------------------------------------
// SYSTEM-WIDE BASE RATE (In context of this model's call rates)
// ------------------------------------------------------------
#define SYSTEM_BASE_RATE_TID           0
#define SYSTEM_BASE_RATE_FACTOR        1
#define SYSTEM_BASE_RATE_PERIOD        0.00066666666666666664

// ------------------------------------------------------------
// AUDIO BUFFER STATISTICS
// ------------------------------------------------------------
#define UNITTEST_TARGET_AUDIOIN_SAMPLE_RATE_IN_HERTZ 48000       /* '<S149>/AudioStats' */
#define UNITTEST_TARGET_AUDIOIN_CALL_RATE_IN_HERTZ 1500          /* '<S149>/AudioStats' */
#define UNITTEST_TARGET_AUDIOIN_FRAME_SIZE_IN_SAMPLES 32         /* '<S149>/AudioStats' */
#define UNITTEST_TARGET_AUDIOIN_CHANNEL_COUNT 2                  /* '<S149>/AudioStats' */
#define UNITTEST_TARGET_AUDIOIN_TASK_RATE_FACTOR 1               /* '<S149>/AudioStats' */
#define UNITTEST_TARGET_AUDIOIN_TASK_ID 0                        /* '<S149>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_SAMPLE_RATE_IN_HERTZ 48000      /* '<S152>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_CALL_RATE_IN_HERTZ 1500         /* '<S152>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_FRAME_SIZE_IN_SAMPLES 32        /* '<S152>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_CHANNEL_COUNT 2                 /* '<S152>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_TASK_RATE_FACTOR 1              /* '<S152>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_TASK_ID 0                       /* '<S152>/AudioStats' */

// Relevant function prototypes- some referenced by above macros
// (Officially declared in BasicAudio_Target.h)
extern void BasicAudio_Target_step(void);
extern void BasicAudio_Target_initialize(void);
extern void BasicAudio_Target_terminate(void);

/* API Buffer (Imported with Pointer access )*/
extern float *UnitTest_Target_AudioIn;

/* API Buffer (Imported with Pointer access )*/
extern float *UnitTest_Target_AudioOut;

// ------------------------------------------------------------
// ASYNCHRONOUS NOTIFICATION SERVICE
// ------------------------------------------------------------
int BasicAudio_Target_ServiceNotifications(void* pNtfBuf, uint32_t nftBufSize);

// ------------------------------------------------------------
// ASDID ASYNCHRONOUS MESSAGE HANDLER
// ------------------------------------------------------------
int BasicAudio_Target_HandleAsdIdRequest(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize);

#endif                          /* RTW_HEADER_BasicAudio_Target_TalariaAPI_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
