/*
 * File: PreAmpPart2.h
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

#ifndef RTW_HEADER_PreAmpPart2_h_
#define RTW_HEADER_PreAmpPart2_h_
#ifndef Model_Target_COMMON_INCLUDES_
#define Model_Target_COMMON_INCLUDES_
#include <stdlib.h>
#include <stdio.h>
#include "rtwtypes.h"
#include "Model_Target_RTC_IDs.h"
#include "Model_Target_TalariaAPI.h"
#include "Model_Target_TSP_Capture.h"
#include "Model_Target_StructStaticInit.h"
#include "Model_Target_TOP.h"
#endif                                 /* Model_Target_COMMON_INCLUDES_ */

/* Block signals for system '<S1843>/fade' */
typedef struct
{
    real32_T audioOut[240];            /* '<S1843>/fade' */
}
B_fade_Model_Target_T;

/* Block signals for system '<S1850>/fade' */
typedef struct
{
    real32_T audioOut[704];            /* '<S1850>/fade' */
    real32_T yHpf[704];
    real32_T tgtGains[2];              /* '<S1850>/fade' */
}
B_fade_Model_Target_m_T;

/* Block signals for system '<S1900>/calculate_SB_gains' */
typedef struct
{
    real32_T targetGainsSB[4];         /* '<S1900>/calculate_SB_gains' */
}
B_calculate_SB_gains_Model_Target_T;

/* Block signals for system '<S1900>/calculate_ramp_parameters' */
typedef struct
{
    real32_T ramptime;                 /* '<S1900>/calculate_ramp_parameters' */
    real32_T rampRate;                 /* '<S1900>/calculate_ramp_parameters' */
}
B_calculate_ramp_parameters_Model_Target_T;

extern void Model_Target_SetDsm_l(uint32_T rtp_Value);
extern void Model_Target_fadecontrol(const real32_T rtu_targetGainLinear[4],
    real32_T rtu_dbPerSec, real32_T rtu_rampTimeMs, real32_T rtu_sampleRateHz,
    real32_T rtu_frameSize);
extern void Model_Target_SetDsm_dn(uint32_T rtp_Value);
extern void Model_Target_fadecontrol_b(const real32_T rtu_targetGainLinear[4],
    real32_T rtu_dbPerSec, real32_T rtu_rampTimeMs, real32_T rtu_sampleRateHz,
    real32_T rtu_frameSize);
extern void Model_Target_fade(const real32_T rtu_audioIn[240], const real32_T
    rtu_channelToRamperMap[30], const real32_T rtu_yLpf[240],
    B_fade_Model_Target_T *localB);
extern void Model_Target_fade_l(const real32_T rtu_audioIn[704], const real32_T
    rtu_channelToRamperMap[22], const real32_T rtu_yLpf[704],
    B_fade_Model_Target_m_T *localB);
extern void Model_Target_calculate_SB_gains(uint32_T rtu_gainIdx, const real32_T
    rtu_TableDb[30], const real32_T rtu_TableIdx[30], real32_T rtu_indexOffset,
    real32_T rtu_MaskSilentExtremeMutesBass, B_calculate_SB_gains_Model_Target_T
    *localB);
extern void Model_Target_calculate_ramp_parameters(real32_T rtu_rampTimeTune,
    uint32_T rtu_rampRateRTC, uint32_T rtu_rampTimeRTC,
    B_calculate_ramp_parameters_Model_Target_T *localB);
extern void Model_Target_SetDsm_f(uint32_T rtp_Value);
extern void Model_Target_SetDsm_o(uint32_T rtp_Value);
extern void HandleReqPreAmpFadeControlSet(void);
extern void HandleReqPreAmpMuteControlSet(void);
extern void HandleReqPreAmpSleepingBeautyDRSet(void);
extern void HandleReqPreAmpSleepingBeautyFRSet(void);
extern void Model_Target_PreAmpPart2_Init(void);
extern void Model_Target_PreAmpPart2(void);

#endif                                 /* RTW_HEADER_PreAmpPart2_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
