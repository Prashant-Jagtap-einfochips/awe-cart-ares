/*
 * File: MedusaExpandTailComps.h
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

#ifndef RTW_HEADER_MedusaExpandTailComps_h_
#define RTW_HEADER_MedusaExpandTailComps_h_
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

/* Block signals for system '<S1430>/Expand Tail Components' */
typedef struct
{
    real32_T Reshape[32];              /* '<S1453>/Reshape' */
    real32_T y[32];                    /* '<S1457>/FirEvenProcess' */
}
B_ExpandTailComponents_Model_Target_T;

/* Block states (default storage) for system '<S1430>/Expand Tail Components' */
typedef struct
{
    real32_T states[80];               /* '<S1453>/Data Store Memory' */
    real32_T States[1848];             /* '<S1457>/FirEvenProcess' */
}
DW_ExpandTailComponents_Model_Target_T;

/* Extern declarations of internal data for system '<S1430>/Expand Tail Components' */
extern B_ExpandTailComponents_Model_Target_T Model_Target_ExpandTailComponents_B;
extern DW_ExpandTailComponents_Model_Target_T
    Model_Target_ExpandTailComponents_DW;
extern void InitTrigger_Model_141_347_65(void);
extern void Model_Target_ExpandTailComponents_Init(void);
extern void Model_Target_ExpandTailComponents(void);

#endif                                 /* RTW_HEADER_MedusaExpandTailComps_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
