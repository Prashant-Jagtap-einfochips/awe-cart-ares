/*
 * File: Model_Target_TalariaAPI.h
 *
 * Code generated for Simulink model 'Model_Target'.
 *
 * Model version                  : 1.12
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Dec 20 15:13:20 2023
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Qualcomm->Hexagon
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Model_Target_TalariaAPI_h_
#define RTW_HEADER_Model_Target_TalariaAPI_h_
#include <stdint.h>

// ------------------------------------------------------------
// MODEL SYNCHRONOUS PROCESSING INTERFACE
// ------------------------------------------------------------
#define MODEL_FUNC(fname)              Model_Target_ ## fname
#define MODEL_HEADER_FILENAME          "Model_Target.h"
#define MODEL_SYNC_TASK_COUNT          9
#define MODEL_RUN_FLAGS_UPDATE(flags)  MODEL_FUNC(SetEventsForThisBaseStep)(flags);
#define MODEL_STEP(TID)                MODEL_FUNC(step)(TID);
#define MODEL_TASK_RATE_FACTORS        1, 12, 13, 26, 52, 208, 832, 3328, 9984
#define MODEL_TASK_SAMPLE_PERIODS      5.5555555555555551E-5, 0.00066666666666666664, 0.00072222222222222219, 0.0014444444444444444, 0.0028888888888888888, 0.011555555555555555, 0.04622222222222222, 0.18488888888888888, 0.55466666666666664
#define MODEL_TASK_SAMPLE_OFFSETS      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0

// ------------------------------------------------------------
// SYSTEM-WIDE BASE RATE (In context of this model's call rates)
// ------------------------------------------------------------
#define SYSTEM_BASE_RATE_TID           1
#define SYSTEM_BASE_RATE_FACTOR        12
#define SYSTEM_BASE_RATE_PERIOD        0.00066666666666666664

// ------------------------------------------------------------
// AUDIO BUFFER STATISTICS
// ------------------------------------------------------------
#define UNITTEST_TARGET_AUDIOIN_SAMPLE_RATE_IN_HERTZ 48000       /* '<S2>/AudioStats' */
#define UNITTEST_TARGET_AUDIOIN_CALL_RATE_IN_HERTZ 1500          /* '<S2>/AudioStats' */
#define UNITTEST_TARGET_AUDIOIN_FRAME_SIZE_IN_SAMPLES 32         /* '<S2>/AudioStats' */
#define UNITTEST_TARGET_AUDIOIN_CHANNEL_COUNT 5                  /* '<S2>/AudioStats' */
#define UNITTEST_TARGET_AUDIOIN_TASK_RATE_FACTOR 12              /* '<S2>/AudioStats' */
#define UNITTEST_TARGET_AUDIOIN_TASK_ID 1                        /* '<S2>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_SAMPLE_RATE_IN_HERTZ 48000      /* '<S5>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_CALL_RATE_IN_HERTZ 1500         /* '<S5>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_FRAME_SIZE_IN_SAMPLES 32        /* '<S5>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_CHANNEL_COUNT 24                /* '<S5>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_TASK_RATE_FACTOR 12             /* '<S5>/AudioStats' */
#define UNITTEST_TARGET_AUDIOOUT_TASK_ID 1                       /* '<S5>/AudioStats' */

// Relevant function prototypes- some referenced by above macros
// (Officially declared in Model_Target.h)
extern void Model_Target_step(int tid);
extern void Model_Target_SetEventsForThisBaseStep(uint8_t *eventFlags);
extern void Model_Target_initialize(void);
extern void Model_Target_terminate(void);

/* API Buffer (Exported with Direct access )*/
extern float UnitTest_Target_AudioIn[160];

/* API Buffer (Exported with Direct access )*/
extern float UnitTest_Target_AudioOut[768];

// ------------------------------------------------------------
// ASYNCHRONOUS NOTIFICATION SERVICE
// ------------------------------------------------------------
int Model_Target_ServiceNotifications(void* pNtfBuf, uint32_t nftBufSize);

// ------------------------------------------------------------
// ASDID ASYNCHRONOUS MESSAGE HANDLER
// ------------------------------------------------------------
int Model_Target_HandleAsdIdRequest(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize);

#endif                               /* RTW_HEADER_Model_Target_TalariaAPI_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
