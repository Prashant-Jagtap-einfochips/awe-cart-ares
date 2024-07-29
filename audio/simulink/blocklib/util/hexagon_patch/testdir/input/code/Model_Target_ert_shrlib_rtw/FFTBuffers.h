/*
 * File: FFTBuffers.h
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

#ifndef RTW_HEADER_FFTBuffers_h_
#define RTW_HEADER_FFTBuffers_h_
#include "rtwtypes.h"
#include "Model_Target_types.h"

/* Exported data declaration */

/* Declaration for custom storage class: FFTBuffer */
extern __attribute((aligned(1024))) real32_T MedusaFdpDeciRateRfftRfftInput[512];/* '<S1214>/Windowing' */
extern __attribute((aligned(1024))) real32_T MedusaFdpFullRateRfftRfftInput[512];/* '<S1310>/Windowing' */

/* Declaration for custom storage class: FFTBuffer */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpDeciRateRfftRfftBufferRfftOutputTmp[256];/* '<S1302>/RfftOutputTmp' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpDeciRateRfftRfftBufferTwiddle1[96];/* '<S1302>/Twiddle1' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpDeciRateRfftRfftBufferTwiddle2[128];/* '<S1302>/Twiddle2' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpDeciRateRifftRifftBufferRifftOutputTmp[256];/* '<S1294>/RifftOutputTmp' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpDeciRateRifftRifftBufferTwiddle1[96];/* '<S1294>/Twiddle1' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpDeciRateRifftRifftBufferTwiddle2[128];/* '<S1294>/Twiddle2' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpFullRateRfftRfftBufferRfftOutputTmp[256];/* '<S1386>/RfftOutputTmp' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpFullRateRfftRfftBufferTwiddle1[96];/* '<S1386>/Twiddle1' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpFullRateRfftRfftBufferTwiddle2[128];/* '<S1386>/Twiddle2' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpFullRateRifftRifftBufferRifftOutputTmp[256];/* '<S1378>/RifftOutputTmp' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpFullRateRifftRifftBufferTwiddle1[96];/* '<S1378>/Twiddle1' */
extern __attribute((aligned(8))) creal32_T
    MedusaFdpFullRateRifftRifftBufferTwiddle2[128];/* '<S1378>/Twiddle2' */

#endif                                 /* RTW_HEADER_FFTBuffers_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
