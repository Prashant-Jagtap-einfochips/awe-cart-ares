/*
 * File: rt_sys_Model_Target_100.h
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

#ifndef RTW_HEADER_rt_sys_Model_Target_100_h_
#define RTW_HEADER_rt_sys_Model_Target_100_h_
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

/* Block signals for system '<S267>/calculate_ramp_rate' */
typedef struct
{
    real32_T rampRate;                 /* '<S267>/calculate_ramp_rate' */
    real32_T ramptime;                 /* '<S267>/calculate_ramp_rate' */
}
B_calculate_ramp_rate_Model_Target_T;

extern void Model_Target_calculate_ramp_rate(uint32_T rtu_rampRateRTC, uint32_T
    rtu_rampTimeRTC, real32_T rtu_rampTimeTune,
    B_calculate_ramp_rate_Model_Target_T *localB);

#endif                               /* RTW_HEADER_rt_sys_Model_Target_100_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
