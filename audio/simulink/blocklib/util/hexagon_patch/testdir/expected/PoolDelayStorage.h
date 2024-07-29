/*
 * File: PoolDelayStorage.h
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

#ifndef RTW_HEADER_PoolDelayStorage_h_
#define RTW_HEADER_PoolDelayStorage_h_
#include "rtwtypes.h"
#include "Model_Target_types.h"

/* Exported data declaration */

/* Declaration for custom storage class: FirGenericStorage */
extern __attribute((aligned(8))) real32_T DrHeadrestFirEqCoeffsTarget[6384];/* '<S1752>/SignalBreakFcn' */
extern __attribute((aligned(8))) real32_T MedusaDrHoligramFirEqCoeffsTarget
    [35112];                           /* '<S1719>/SignalBreakFcn' */

/* Declaration for custom storage class: PooliirAccelStorage */
extern __attribute((aligned(8))) real32_T HoligramFrIir20562states[7761];/* '<S1737>/states' */
extern __attribute((aligned(8))) real32_T MedusaBassMgrHpf18828states[65];/* '<S1178>/states' */
extern __attribute((aligned(8))) real32_T MedusaBassMgrLpf11643states[65];/* '<S1186>/states' */
extern __attribute((aligned(8))) real32_T MedusaDrHoligramIirEq26171states
    [174257];                          /* '<S1726>/states' */
extern __attribute((aligned(8))) real32_T MedusaMonoDetectorHpf32323states[33];/* '<S1401>/states' */
extern __attribute((aligned(8))) real32_T NearFsDrEq47493states[1417];/* '<S46>/states' */
extern __attribute((aligned(8))) real32_T NearFsFrEq10008states[1017];/* '<S90>/states' */
extern __attribute((aligned(8))) real32_T NearHrEqDr25031states[817];/* '<S58>/states' */
extern __attribute((aligned(8))) real32_T NearHrEqFr28288states[817];/* '<S102>/states' */
extern __attribute((aligned(8))) real32_T PostProcPostLimiterEq56836states[6161];/* '<S630>/states' */
extern __attribute((aligned(8))) real32_T SPUMDecayIir57735states[113];/* '<S1423>/states' */
extern __attribute((aligned(8))) real32_T VlsHeadrestEq20140states[5777];/* '<S1759>/states' */
extern __attribute((aligned(8))) real32_T XtalkIIR42849states[1937];/* '<S1953>/states' */
extern __attribute__ ((__aligned__(8))) real32_T Model_Target_Near0Dr_PooliirpooliirCoeffs[407];
extern __attribute((aligned(8))) real32_T Model_Target_Near0Dr_PoolIirTunablepooliirCoeffs[404];
#endif                                 /* RTW_HEADER_PoolDelayStorage_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
