/*
 * File: Model_Target_types.h
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

#ifndef RTW_HEADER_Model_Target_types_h_
#define RTW_HEADER_Model_Target_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_MonoDetectorTunable_
#define DEFINED_TYPEDEF_FOR_MonoDetectorTunable_

typedef struct
{
    real32_T levelPole;
    real32_T levelGain;
    real32_T lsSmoothPole;
    real32_T lsSmoothGain;
    real32_T lsSmoothResetPole;
    real32_T lsSmoothResetGain;
    real32_T leftRightMatchTolerance;
    real32_T thresholdZeroLevel;
    real32_T thresholdMono;
    real32_T thresholdStereo;
}
MonoDetectorTunable;

#endif

#ifndef DEFINED_TYPEDEF_FOR_decayEstTunable_
#define DEFINED_TYPEDEF_FOR_decayEstTunable_

typedef struct
{
    real32_T onsetThreshold;
    real32_T noiseFloor;
    real32_T fastAttack;
    real32_T slowAttack;
    real32_T directPathFramesFs;
}
decayEstTunable;

#endif

#ifndef DEFINED_TYPEDEF_FOR_decaySlewTunable_
#define DEFINED_TYPEDEF_FOR_decaySlewTunable_

typedef struct
{
    real32_T slewThreshold;
    real32_T slewIncrement;
    real32_T slewDecrement;
}
decaySlewTunable;

#endif

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

#ifndef DEFINED_TYPEDEF_FOR_struct_XRJXGIsqwVExSKkghyNPsB_
#define DEFINED_TYPEDEF_FOR_struct_XRJXGIsqwVExSKkghyNPsB_

typedef struct
{
    boolean_T ok;
    boolean_T isIndirect;
}
struct_XRJXGIsqwVExSKkghyNPsB;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_B4Uj7hfl7xoXuY6EKqbQzE_
#define DEFINED_TYPEDEF_FOR_struct_B4Uj7hfl7xoXuY6EKqbQzE_

typedef struct
{
    real_T defaultSurroundIndex;
    real_T defaultRampRate;
    real_T defaultRampTime;
}
struct_B4Uj7hfl7xoXuY6EKqbQzE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_GvRmpueHHywrqEBgHMxfHG_
#define DEFINED_TYPEDEF_FOR_struct_GvRmpueHHywrqEBgHMxfHG_

typedef struct
{
    real_T LR;
    real_T CS;
}
struct_GvRmpueHHywrqEBgHMxfHG;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_QTcciMeJFBa9P5bUVDnkTD_
#define DEFINED_TYPEDEF_FOR_struct_QTcciMeJFBa9P5bUVDnkTD_

typedef struct
{
    real_T sample_rate_in_hertz;
    real_T frame_size;
    real32_T rgain_SILENT_GAIN;
    real_T granularity[3];
    real_T LR_in;
    real_T LR_out;
    real_T CS_in;
    real_T CS_out;
    struct_GvRmpueHHywrqEBgHMxfHG NumTailComp;
    real_T includesSurroundInput;
    real_T includeCAE;
}
struct_QTcciMeJFBa9P5bUVDnkTD;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_vN6YUHTUe8GSjCLdMtNEsF_
#define DEFINED_TYPEDEF_FOR_struct_vN6YUHTUe8GSjCLdMtNEsF_

typedef struct
{
    real_T BackwardDownmixMax;
    real_T BackwardDownmixThreshold;
    real_T ForwardDownmixMax;
    real_T ForwardDownmixThreshold;
    real_T bitGranularity[3];
    real_T detentGain_CS[12];
    real_T detentGain_Left[30];
    real_T detentGain_Right[30];
    real_T downmix_wts_CS[12];
    real_T downmix_wts_Left[30];
    real_T downmix_wts_Right[30];
    real_T interpolationMethod;
    real_T maxGain_CS[12];
    real_T maxGain_Left[30];
    real_T maxGain_Right[30];
    real_T minGain_CS[12];
    real_T minGain_Left[30];
    real_T minGain_Right[30];
}
struct_vN6YUHTUe8GSjCLdMtNEsF;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_S37gCiXDpPQ11yRVjr982B_
#define DEFINED_TYPEDEF_FOR_struct_S37gCiXDpPQ11yRVjr982B_

typedef struct
{
    real_T sample_rate_in_hertz;
    real_T frame_size;
    real32_T rgain_SILENT_GAIN;
    real_T granularity[3];
    real_T LR_in;
    real_T LR_out;
    real_T CS_in;
    real_T CS_out;
    real_T NumCenterOut;
    real_T NumSurroundOut;
    struct_GvRmpueHHywrqEBgHMxfHG NumTailComp;
    real_T NumPremixOut;
    real_T numOverheadComps;
    real_T includesSurroundInput;
    real_T includeCAE;
    real_T initLeftGain[77];
    real_T initCsGain[20];
    real_T initRightGain[77];
}
struct_S37gCiXDpPQ11yRVjr982B;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_aTKP0jLMDeR127sb7s7mZH_
#define DEFINED_TYPEDEF_FOR_struct_aTKP0jLMDeR127sb7s7mZH_

typedef struct
{
    real_T BackwardDownmixMax;
    real_T BackwardDownmixThreshold;
    real_T ForwardDownmixMax;
    real_T ForwardDownmixThreshold;
    real_T bitGranularity[3];
    real_T detentGain_CS[20];
    real_T detentGain_Left[77];
    real_T detentGain_Right[77];
    real_T downmix_wts_CS[20];
    real_T downmix_wts_Left[77];
    real_T downmix_wts_Right[77];
    real_T interpolationMethod;
    real_T maxGain_CS[20];
    real_T maxGain_Left[77];
    real_T maxGain_Right[77];
    real_T minGain_CS[20];
    real_T minGain_Left[77];
    real_T minGain_Right[77];
}
struct_aTKP0jLMDeR127sb7s7mZH;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_ilb4ZOOw6Bci8S37FHnj3F_
#define DEFINED_TYPEDEF_FOR_struct_ilb4ZOOw6Bci8S37FHnj3F_

typedef struct
{
    real_T sample_rate_in_hertz;
    real_T frame_size;
    real32_T rgain_SILENT_GAIN;
    real_T granularity[3];
    uint32_T numGains;
}
struct_ilb4ZOOw6Bci8S37FHnj3F;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_uF3SqGWtyJHSkvF0WoUfJ_
#define DEFINED_TYPEDEF_FOR_struct_uF3SqGWtyJHSkvF0WoUfJ_

typedef struct
{
    real_T BackwardDownmixMax;
    real_T BackwardDownmixThreshold;
    real_T ForwardDownmixMax;
    real_T ForwardDownmixThreshold;
    real_T bitGranularity[3];
    real_T detentGain[12];
    real_T downmix_wts[12];
    real_T interpolationMethod;
    real_T maxGain[12];
    real_T minGain[12];
}
struct_uF3SqGWtyJHSkvF0WoUfJ;

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

/* Custom Type definition for MATLAB Function: '<S1974>/tone control' */
#ifndef struct_tag_suFnE4DIYISSjYbtQHPXOwD
#define struct_tag_suFnE4DIYISSjYbtQHPXOwD

struct tag_suFnE4DIYISSjYbtQHPXOwD
{
    real32_T frames;
    real32_T target_linear;
    real32_T current_linear;
};

#endif                                 /* struct_tag_suFnE4DIYISSjYbtQHPXOwD */

#ifndef typedef_suFnE4DIYISSjYbtQHPXOwD_Model_Target_T
#define typedef_suFnE4DIYISSjYbtQHPXOwD_Model_Target_T

typedef struct tag_suFnE4DIYISSjYbtQHPXOwD
    suFnE4DIYISSjYbtQHPXOwD_Model_Target_T;

#endif                      /* typedef_suFnE4DIYISSjYbtQHPXOwD_Model_Target_T */

/* Forward declaration for rtModel */
typedef struct tag_RTM_Model_Target_T RT_MODEL_Model_Target_T;

#endif                                 /* RTW_HEADER_Model_Target_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
