/*
 * File: BasicAudio_TranslationEngine64_types.h
 *
 * Code generated for Simulink model 'BasicAudio_TranslationEngine64'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:48:46 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_BasicAudio_TranslationEngine64_types_h_
#define RTW_HEADER_BasicAudio_TranslationEngine64_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_bus_double_1_1_
#define DEFINED_TYPEDEF_FOR_bus_double_1_1_

typedef struct
{
    real_T Size[2];
    real_T Value;
}
bus_double_1_1;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ToneFilterTypeEnum_
#define DEFINED_TYPEDEF_FOR_ToneFilterTypeEnum_

typedef enum
{
    ToneFilterTypeEnum_lowpass = 1,    /* Default value */
    ToneFilterTypeEnum_highpass
}
ToneFilterTypeEnum;

#endif

#ifndef DEFINED_TYPEDEF_FOR_bus_double_1_2_
#define DEFINED_TYPEDEF_FOR_bus_double_1_2_

typedef struct
{
    real_T Size[2];
    real_T Value[2];
}
bus_double_1_2;

#endif

#ifndef DEFINED_TYPEDEF_FOR_bus_double_30__2_
#define DEFINED_TYPEDEF_FOR_bus_double_30__2_

typedef struct
{
    real_T Size[2];
    real_T Value[60];
}
bus_double_30__2;

#endif

#ifndef DEFINED_TYPEDEF_FOR_bus_double_8_2_
#define DEFINED_TYPEDEF_FOR_bus_double_8_2_

typedef struct
{
    real_T Size[2];
    real_T Value[16];
}
bus_double_8_2;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_BasicAudio_TranslationEngine64_T
    RT_MODEL_BasicAudio_TranslationEngine64_T;

#endif                  /* RTW_HEADER_BasicAudio_TranslationEngine64_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
