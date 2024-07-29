/*
 * File: BasicAudio_Target_types.h
 *
 * Code generated for Simulink model 'BasicAudio_Target'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:46:55 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_BasicAudio_Target_types_h_
#define RTW_HEADER_BasicAudio_Target_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_rgain_bus_
#define DEFINED_TYPEDEF_FOR_rgain_bus_

/* This structure contains a the complete set of control parameters for the rgain block's "SetGain" service. */
typedef struct
{
    /* This count indicates how many frames remain until the targetGain is reached.
       When frameCount reaches "0", the current gain should be explicitly set to target gain. */
    int32_T frameCount;

    /* Multiplicative coefficient of gain. Each sample, current gain will be multiplied by this value.
       In steady state (i.e. no ramping) the coefficient value will be "1.0". */
    real32_T rampCoeff;

    /* This is the target gain value the ramper is trying to achieve. */
    real32_T targetGain;

    /* This is the current applied gain value. */
    real32_T currentGain;
}
rgain_bus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_Sjz1k0ulTvVYBfShQFzB4F_
#define DEFINED_TYPEDEF_FOR_struct_Sjz1k0ulTvVYBfShQFzB4F_

typedef struct
{
    real_T frameCount;
    real_T rampCoeff;
    real_T targetGain;
    real_T currentGain;
}
struct_Sjz1k0ulTvVYBfShQFzB4F;

#endif

/* Custom Type definition for MATLAB Function: '<S40>/tone control' */
#ifndef struct_tag_suFnE4DIYISSjYbtQHPXOwD
#define struct_tag_suFnE4DIYISSjYbtQHPXOwD

struct tag_suFnE4DIYISSjYbtQHPXOwD
{
    real32_T frames;
    real32_T target_linear;
    real32_T current_linear;
};

#endif                                 /* struct_tag_suFnE4DIYISSjYbtQHPXOwD */

#ifndef typedef_suFnE4DIYISSjYbtQHPXOwD_BasicAudio_Target_T
#define typedef_suFnE4DIYISSjYbtQHPXOwD_BasicAudio_Target_T

typedef struct tag_suFnE4DIYISSjYbtQHPXOwD
    suFnE4DIYISSjYbtQHPXOwD_BasicAudio_Target_T;

#endif                 /* typedef_suFnE4DIYISSjYbtQHPXOwD_BasicAudio_Target_T */

/* Forward declaration for rtModel */
typedef struct tag_RTM_BasicAudio_Target_T RT_MODEL_BasicAudio_Target_T;

#endif                               /* RTW_HEADER_BasicAudio_Target_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
