/*
 * File: BasicAudio_TranslationEngine64_TalariaAPI.h
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

#ifndef RTW_HEADER_BasicAudio_TranslationEngine64_TalariaAPI_h_
#define RTW_HEADER_BasicAudio_TranslationEngine64_TalariaAPI_h_
#include <stdint.h>

// ------------------------------------------------------------
// MODEL SYNCHRONOUS PROCESSING INTERFACE
// ------------------------------------------------------------
#define MODEL_FUNC(fname)              BasicAudio_TranslationEngine64_ ## fname
#define MODEL_HEADER_FILENAME          "BasicAudio_TranslationEngine64.h"
#define MODEL_SYNC_TASK_COUNT          1
#define MODEL_RUN_FLAGS_UPDATE(flags)
#define MODEL_STEP(TID)                MODEL_FUNC(step)();
#define MODEL_TASK_RATE_FACTORS        1
#define MODEL_TASK_SAMPLE_PERIODS      0.1
#define MODEL_TASK_SAMPLE_OFFSETS      0.0

// ------------------------------------------------------------
// SYSTEM-WIDE BASE RATE (In context of this model's call rates)
// ------------------------------------------------------------
#define SYSTEM_BASE_RATE_TID           0
#define SYSTEM_BASE_RATE_FACTOR        1
#define SYSTEM_BASE_RATE_PERIOD        0.1

// Relevant function prototypes- some referenced by above macros
// (Officially declared in BasicAudio_TranslationEngine64.h)
extern void BasicAudio_TranslationEngine64_step(void);
extern void BasicAudio_TranslationEngine64_initialize(void);
extern void BasicAudio_TranslationEngine64_terminate(void);

// ------------------------------------------------------------
// ASYNCHRONOUS NOTIFICATION SERVICE
// ------------------------------------------------------------
int BasicAudio_TranslationEngine64_ServiceNotifications(void* pNtfBuf, uint32_t
    nftBufSize);

// ------------------------------------------------------------
// ASDID ASYNCHRONOUS MESSAGE HANDLER
// ------------------------------------------------------------
int BasicAudio_TranslationEngine64_HandleAsdIdRequest(void* pReqBuf, uint32_t
    reqBufSize,
    void* pRspBuf, uint32_t rspBufSize);

#endif             /* RTW_HEADER_BasicAudio_TranslationEngine64_TalariaAPI_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
