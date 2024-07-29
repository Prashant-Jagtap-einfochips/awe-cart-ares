/*
 * File: PostProcess.h
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

#ifndef RTW_HEADER_PostProcess_h_
#define RTW_HEADER_PostProcess_h_
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

#include "rt_sys_Model_Target_100.h"

/* Block signals for system '<S6>/PostProcess' */
typedef struct
{
    real32_T Magsq[768];               /* '<S573>/Magsq' */
    uint32_T In1;                      /* '<S605>/In1' */
    uint32_T DataTypeConversion;       /* '<S577>/Data Type Conversion' */
    real32_T DataStoreRead;            /* '<S603>/Data Store Read' */
    real32_T DataTypeConversion1[24];  /* '<S572>/Data Type Conversion1' */
    real32_T CurrentGain;              /* '<S572>/Delay1' */
    __attribute__ ((__aligned__(8))) real32_T y[768];                   /* '<S653>/MATLAB Function' */
    real32_T ImpAsg_InsertedFor_Out1_at_inport_0[768];/* '<S625>/Multiply' */
    real32_T ramping_coeff;            /* '<S603>/get_ramp_coeffs' */
    real32_T y_f[768];                 /* '<S599>/MATLAB Function' */
    real32_T transition;               /* '<S572>/EBL Transition' */
    int16_T DataTypeConversion5;       /* '<S572>/Data Type Conversion5' */
    int16_T DataTypeConversion6;       /* '<S572>/Data Type Conversion6' */
    boolean_T done;                    /* '<S603>/TriggeredFunction' */
    boolean_T DataTypeConversion_m;    /* '<S576>/Data Type Conversion' */
    B_calculate_ramp_rate_Model_Target_T sf_calculate_ramp_rate_g;/* '<S602>/calculate_ramp_rate' */
}
B_PostProcess_Model_Target_T;

/* Block states (default storage) for system '<S6>/PostProcess' */
typedef struct
{
    real_T Delay2_DSTATE[24];          /* '<S579>/Delay2' */
    real32_T Delay1_DSTATE;            /* '<S572>/Delay1' */
    real32_T Delay2_DSTATE_n;          /* '<S572>/Delay2' */
    real32_T Delay_DSTATE;             /* '<S572>/Delay' */
    real32_T Delay3_DSTATE;            /* '<S572>/Delay3' */
    real32_T Delay_DSTATE_l;           /* '<S622>/Delay' */
    real32_T ramping_filter_states;    /* '<S620>/ramping_filter' */
    real32_T CurrentGainValue;         /* '<S571>/CurrentGain' */
    real32_T state[24];                /* '<S599>/MATLAB Function' */
    real32_T prevGain;                 /* '<S571>/RamperProcess' */
    int16_T EBL_EnableCounter;         /* '<S572>/Data Store Memory' */
    int16_T EBL_EnableCounter_o;       /* '<S572>/EBL Transition' */
    uint8_T ramping_filter_icLoad;     /* '<S620>/ramping_filter' */
    boolean_T icLoad;                  /* '<S572>/Delay1' */
    boolean_T prevGain_not_empty;      /* '<S571>/RamperProcess' */
}
DW_PostProcess_Model_Target_T;

/* Invariant block signals for system '<S6>/PostProcess' */
typedef struct
{
    const real_T Add1;                 /* '<S572>/Add1' */
    const real_T Product;              /* '<S578>/Product' */
    const real_T Divide;               /* '<S578>/Divide' */
    const real_T MathFunction;         /* '<S578>/Math Function' */
    const real_T Subtract1;            /* '<S579>/Subtract1' */
    const real_T Divide1;              /* '<S582>/Divide1' */
    const real_T Gain;                 /* '<S582>/Gain' */
    const real_T MathFunction1;        /* '<S582>/Math Function1' */
    const real_T MathFunction2;        /* '<S582>/Math Function2' */
    const real_T Product5;             /* '<S583>/Product5' */
    const real_T Product6;             /* '<S583>/Product6' */
    const real_T RoundingFunction;     /* '<S583>/Rounding Function' */
    const real_T RoundingFunction1;    /* '<S583>/Rounding Function1' */
    const real_T RoundingFunction_i;   /* '<S584>/Rounding Function' */
    const real_T Gain1;                /* '<S585>/Gain1' */
    const real_T Divide4;              /* '<S585>/Divide4' */
    const real_T MathFunction4;        /* '<S585>/Math Function4' */
    const real_T DEBUG_EBL_THRESHOLD_DBSPL;/* '<S592>/Add1' */
    const real32_T DataTypeConversion3;/* '<S572>/Data Type Conversion3' */
}
ConstB_PostProcess_Model_Target_T;

/* Extern declarations of internal data for system '<S6>/PostProcess' */
extern B_PostProcess_Model_Target_T Model_Target_PostProcess_B;
extern DW_PostProcess_Model_Target_T Model_Target_PostProcess_DW;
extern const ConstB_PostProcess_Model_Target_T Model_Target_PostProcess_ConstB;
extern void HandleReqPostProcessMuteCfgSet(void);
extern void InitTrigger_Model_76_172_156(void);
extern void Model_Target_PostProcess_Init(void);
extern void Model_Target_PostProcess(void);

#endif                                 /* RTW_HEADER_PostProcess_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
