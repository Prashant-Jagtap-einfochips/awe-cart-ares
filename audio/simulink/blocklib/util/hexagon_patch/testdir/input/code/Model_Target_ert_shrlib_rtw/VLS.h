/*
 * File: VLS.h
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

#ifndef RTW_HEADER_VLS_h_
#define RTW_HEADER_VLS_h_
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

/* Block signals for system '<S1961>/FirGenericProcessFcn' */
typedef struct
{
    real32_T y[160];                   /* '<S1961>/FirGenericProcessFcn' */
}
B_FirGenericProcessFcn_Model_Target_T;

/* Block signals for system '<S663>/VLS' */
typedef struct
{
    real32_T Add[96];                  /* '<S1941>/Add' */
    real32_T Add_b[96];                /* '<S1942>/Add' */
    real32_T Selector[128];            /* '<S1943>/Selector' */
    B_FirGenericProcessFcn_Model_Target_T sf_FirGenericProcessFcn_p;/* '<S1969>/FirGenericProcessFcn' */
    B_FirGenericProcessFcn_Model_Target_T sf_FirGenericProcessFcn;/* '<S1961>/FirGenericProcessFcn' */
}
B_VLS_Model_Target_T;

/* Block states (default storage) for system '<S663>/VLS' */
typedef struct
{
    real32_T delayBuffer[600];         /* '<S1944>/Data Store Memory' */
    real32_T States[6060];             /* '<S1961>/States' */
    real32_T StatesSimulation[5920];   /* '<S1961>/StatesSimulation' */
    real32_T States_k[6060];           /* '<S1969>/States' */
    real32_T StatesSimulation_a[5920]; /* '<S1969>/StatesSimulation' */
    int32_T delayIndex;                /* '<S1944>/MATLAB Function' */
    uint32_T CodeWorkMemory[629];      /* '<S1961>/CodeWorkMemory' */
    uint32_T CodeWorkMemory_a[629];    /* '<S1969>/CodeWorkMemory' */
}
DW_VLS_Model_Target_T;

/* Extern declarations of internal data for system '<S663>/VLS' */
extern B_VLS_Model_Target_T Model_Target_VLS_B;
extern DW_VLS_Model_Target_T Model_Target_VLS_DW;
extern void Model_Target_FirGenericProcessFcn(const real32_T rtu_u[96], uint32_T
    rtd_CodeWorkMemory[629], B_FirGenericProcessFcn_Model_Target_T *localB);
extern void Model_Target_FirGenericInitFcn(const real32_T rtu_u[96], const
    real32_T rtu_FilterCoeffs[5760], const uint32_T rtu_FilterLengths[20], const
    uint32_T rtu_InputMapping[20], const uint32_T rtu_FilterMapping[20],
    uint32_T rtd_CodeWorkMemory[629], real32_T rtd_States[6060]);
extern void InitTrigger_Model_232_8_34_49_172_156(void);
extern void InitTrigger_Model_232_11_238_99(void);
extern void InitTrigger_Model_232_12_238_99(void);
extern void Model_Target_VLS_Init(void);
extern void Model_Target_VLS(void);

#endif                                 /* RTW_HEADER_VLS_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
