/*
 * File: FirStorage.h
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

#ifndef RTW_HEADER_FirStorage_h_
#define RTW_HEADER_FirStorage_h_
#include "rtwtypes.h"
#include "Model_Target_types.h"

/* Exported data declaration */

/* Declaration for custom storage class: FirStorage */
extern __attribute((aligned(8))) uint32_T MedusaBandSplitImgRejCodeWorkMemory[88];/* '<S1198>/CodeWorkMemory' */
extern __attribute((aligned(8))) uint32_T MedusaExpandTailWiggleCodeWorkMemory
    [88];                              /* '<S1457>/CodeWorkMemory' */
extern __attribute((aligned(8))) uint32_T NearBandSplitCodeWorkMemory[88];/* '<S163>/CodeWorkMemory' */
extern __attribute((aligned(8))) uint32_T NearWiggleCodeWorkMemory[88];/* '<S1810>/CodeWorkMemory' */

#endif                                 /* RTW_HEADER_FirStorage_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
