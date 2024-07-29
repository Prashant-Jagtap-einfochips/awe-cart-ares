/*
 * File: BasicAudio_Target.h
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

#ifndef RTW_HEADER_BasicAudio_Target_h_
#define RTW_HEADER_BasicAudio_Target_h_
#ifndef BasicAudio_Target_COMMON_INCLUDES_
#define BasicAudio_Target_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "BasicAudio_Target_RTC_IDs.h"
#include "BasicAudio_Target_TalariaAPI.h"
#include "BasicAudio_Target_StructStaticInit.h"
#include "BasicAudio_Target_TOP.h"
#endif                                 /* BasicAudio_Target_COMMON_INCLUDES_ */

#include "BasicAudio_Target_types.h"

/* Custom includes */
#include <string.h>

// Macro used to insert locking directives '<S34>/InsertLockCode'
#ifndef TALARIA_LOCK_ENTER
#define TALARIA_LOCK_ENTER(x)
#endif

#ifndef TALARIA_LOCK_EXIT
#define TALARIA_LOCK_EXIT(x)
#endif

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals for system '<S40>/index to linear' */
typedef struct
{
    real32_T linearGain;               /* '<S40>/index to linear' */
}
B_indextolinear_BasicAudio_Target_T;

/* Block signals for system '<S40>/ramp parameters' */
typedef struct
{
    real32_T ramptime;                 /* '<S40>/ramp parameters' */
    real32_T rampRate;                 /* '<S40>/ramp parameters' */
}
B_rampparameters_BasicAudio_Target_T;

/* Block signals for system '<S40>/tone' */
typedef struct
{
    real32_T audioOut[64];             /* '<S40>/tone' */
}
B_tone_BasicAudio_Target_T;

/* Block states (default storage) for system '<S40>/tone' */
typedef struct
{
    real32_T toneFilterStates[2];      /* '<S40>/tone' */
}
DW_tone_BasicAudio_Target_T;

/* Block signals for system '<S92>/tone' */
typedef struct
{
    real32_T audioOut[64];             /* '<S92>/tone' */
}
B_tone_BasicAudio_Target_h_T;

/* Block states (default storage) for system '<S92>/tone' */
typedef struct
{
    real32_T toneFilterStates[2];      /* '<S92>/tone' */
}
DW_tone_BasicAudio_Target_p_T;

/* Block signals for system '<S119>/lookup db from index' */
typedef struct
{
    real32_T gain_value;               /* '<S119>/lookup db from index' */
}
B_lookupdbfromindex_BasicAudio_Target_T;

/* Block signals for system '<S118>/ramp parameters' */
typedef struct
{
    real32_T rampRate;                 /* '<S118>/ramp parameters' */
    real32_T ramptime;                 /* '<S118>/ramp parameters' */
}
B_rampparameters_BasicAudio_Target_m_T;

/* Block signals for system '<S134>/rgainy process' */
typedef struct
{
    real32_T audioOut[64];             /* '<S134>/rgainy process' */
    real32_T gainOut[2];               /* '<S134>/rgainy process' */
}
B_rgainyprocess_BasicAudio_Target_T;

/* Block signals (default storage) */
typedef struct
{
    uint32_T In1;                      /* '<S122>/In1' */
    uint32_T In1_n;                    /* '<S96>/In1' */
    uint32_T In1_c;                    /* '<S70>/In1' */
    uint32_T In1_h;                    /* '<S44>/In1' */
    uint32_T In1_d;                    /* '<S15>/In1' */
    real32_T NumericalUnity[64];       /* '<S148>/Numerical Unity' */
    real32_T ImpAsg_InsertedFor_Out1_at_inport_0[64];/* '<S90>/Multiply' */
    real32_T ramping_coeff;            /* '<S68>/get_ramp_coeffs' */
    real32_T audioOut[64];             /* '<S11>/balanceProcess' */
    boolean_T done;                    /* '<S136>/rgainx check sequence' */
    boolean_T done_l;                  /* '<S92>/check done' */
    boolean_T done_e;                  /* '<S68>/TriggeredFunction' */
    boolean_T done_m;                  /* '<S40>/check done' */
    boolean_T done_p;                  /* '<S11>/check sequence' */
    B_rgainyprocess_BasicAudio_Target_T sf_rgainyprocess;/* '<S134>/rgainy process' */
    B_rampparameters_BasicAudio_Target_m_T sf_rampparameters_o;/* '<S118>/ramp parameters' */
    B_lookupdbfromindex_BasicAudio_Target_T sf_lookupdbfromindex;/* '<S119>/lookup db from index' */
    B_tone_BasicAudio_Target_h_T sf_tone_e;/* '<S92>/tone' */
    B_rampparameters_BasicAudio_Target_T sf_rampparameters_ho;/* '<S92>/ramp parameters' */
    B_indextolinear_BasicAudio_Target_T sf_indextolinear_ee;/* '<S92>/index to linear' */
    B_tone_BasicAudio_Target_T sf_tone;/* '<S40>/tone' */
    B_rampparameters_BasicAudio_Target_T sf_rampparameters;/* '<S40>/ramp parameters' */
    B_indextolinear_BasicAudio_Target_T sf_indextolinear;/* '<S40>/index to linear' */
}
B_BasicAudio_Target_T;

/* Block states (default storage) for system '<Root>' */
typedef struct
{
    real32_T Delay_DSTATE;             /* '<S87>/Delay' */
    real32_T ramping_filter_states;    /* '<S85>/ramping_filter' */
    uint8_T ramping_filter_icLoad;     /* '<S85>/ramping_filter' */
    DW_tone_BasicAudio_Target_p_T sf_tone_e;/* '<S92>/tone' */
    DW_tone_BasicAudio_Target_T sf_tone;/* '<S40>/tone' */
}
DW_BasicAudio_Target_T;

/* Invariant block signals (default storage) */
typedef struct
{
    const uint8_T AudioStatsFunc[3];   /* '<S134>/Audio Stats Func' */
}
ConstB_BasicAudio_Target_T;

/* Constant parameters (default storage) */
typedef struct
{
    /* Pooled Parameter (Mixed Expressions)
     * Referenced by:
     *   '<S40>/ramp parameters'
     *   '<S92>/ramp parameters'
     */
    real_T pooled1[2];

    /* Pooled Parameter (Mixed Expressions)
     * Referenced by:
     *   '<S40>/ramp parameters'
     *   '<S92>/ramp parameters'
     */
    real_T pooled2[2];
}
ConstP_BasicAudio_Target_T;

/* Real-time Model Data Structure */
struct tag_RTM_BasicAudio_Target_T
{
    const char_T * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_BasicAudio_Target_T BasicAudio_Target_B;

/* Block states (default storage) */
extern DW_BasicAudio_Target_T BasicAudio_Target_DW;
extern const ConstB_BasicAudio_Target_T BasicAudio_Target_ConstB;/* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP_BasicAudio_Target_T BasicAudio_Target_ConstP;

/* Model entry point functions */
extern void BasicAudio_Target_initialize(void);
extern void BasicAudio_Target_step(void);
extern void BasicAudio_Target_terminate(void);

/* Real-time Model object */
extern RT_MODEL_BasicAudio_Target_T *const BasicAudio_Target_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S4>/Add' : Unused code path elimination
 * Block '<S4>/Constant' : Unused code path elimination
 * Block '<S4>/Scope' : Unused code path elimination
 * Block '<S148>/Switch Control' : Unused code path elimination
 * Block '<S3>/Input Wave' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BasicAudio_Target'
 * '<S1>'   : 'BasicAudio_Target/BasicAudio'
 * '<S2>'   : 'BasicAudio_Target/CoreIndex'
 * '<S3>'   : 'BasicAudio_Target/Input'
 * '<S4>'   : 'BasicAudio_Target/Output'
 * '<S5>'   : 'BasicAudio_Target/BasicAudio/Balance'
 * '<S6>'   : 'BasicAudio_Target/BasicAudio/Bass'
 * '<S7>'   : 'BasicAudio_Target/BasicAudio/Mute'
 * '<S8>'   : 'BasicAudio_Target/BasicAudio/Treble'
 * '<S9>'   : 'BasicAudio_Target/BasicAudio/Volume'
 * '<S10>'  : 'BasicAudio_Target/BasicAudio/Balance/balance'
 * '<S11>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink'
 * '<S12>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/RTC Set'
 * '<S13>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/SetDsm'
 * '<S14>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/SetDsm1'
 * '<S15>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/Triggered Subsystem'
 * '<S16>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/TuneVarBoostDisable'
 * '<S17>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/TuneVarOffset'
 * '<S18>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/TuneVarRampTime'
 * '<S19>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/TuneVarSilentBass'
 * '<S20>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/TuneVarSilentExtreme'
 * '<S21>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/TuneVarTable'
 * '<S22>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/balanceProcess'
 * '<S23>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/calculate_balance_gain'
 * '<S24>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/calculate_ramp_parameters'
 * '<S25>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/check sequence'
 * '<S26>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/control'
 * '<S27>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/RTC Set/Notification'
 * '<S28>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/RTC Set/NtfCheck'
 * '<S29>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/RTC Set/NtfPrep'
 * '<S30>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/RTC Set/REQ'
 * '<S31>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/RTC Set/Request'
 * '<S32>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/RTC Set/Response'
 * '<S33>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/SetDsm/DsmWrite'
 * '<S34>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/SetDsm/LockEnter'
 * '<S35>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/SetDsm/LockExit'
 * '<S36>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/SetDsm1/DsmWrite'
 * '<S37>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/SetDsm1/LockEnter'
 * '<S38>'  : 'BasicAudio_Target/BasicAudio/Balance/balance/InnerLink/SetDsm1/LockExit'
 * '<S39>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone'
 * '<S40>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink'
 * '<S41>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/RTC Set'
 * '<S42>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/SetDsm'
 * '<S43>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/SetDsm1'
 * '<S44>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/Triggered Subsystem'
 * '<S45>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/TuneVarOffset'
 * '<S46>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/TuneVarRampTime'
 * '<S47>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/TuneVarScale'
 * '<S48>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/check done'
 * '<S49>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/index to linear'
 * '<S50>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/ramp parameters'
 * '<S51>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/tone'
 * '<S52>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/tone control'
 * '<S53>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/RTC Set/Notification'
 * '<S54>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/RTC Set/NtfCheck'
 * '<S55>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/RTC Set/NtfPrep'
 * '<S56>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/RTC Set/REQ'
 * '<S57>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/RTC Set/Request'
 * '<S58>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/RTC Set/Response'
 * '<S59>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/SetDsm/DsmWrite'
 * '<S60>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/SetDsm/LockEnter'
 * '<S61>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/SetDsm/LockExit'
 * '<S62>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/SetDsm1/DsmWrite'
 * '<S63>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/SetDsm1/LockEnter'
 * '<S64>'  : 'BasicAudio_Target/BasicAudio/Bass/Tone/InnerLink/SetDsm1/LockExit'
 * '<S65>'  : 'BasicAudio_Target/BasicAudio/Mute/mute'
 * '<S66>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink'
 * '<S67>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl'
 * '<S68>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper'
 * '<S69>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl/RTC set'
 * '<S70>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl/Triggered Subsystem'
 * '<S71>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl/TuneVarRampTime'
 * '<S72>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl/calculate_ramp_rate'
 * '<S73>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl/RTC set/Notification'
 * '<S74>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl/RTC set/NtfCheck'
 * '<S75>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl/RTC set/NtfPrep'
 * '<S76>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl/RTC set/REQ'
 * '<S77>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl/RTC set/Request'
 * '<S78>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteControl/RTC set/Response'
 * '<S79>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/Ramping_Subsystem'
 * '<S80>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/TriggeredFunction'
 * '<S81>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/get_ramp_coeffs'
 * '<S82>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/Ramping_Subsystem/RampingGain'
 * '<S83>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/Ramping_Subsystem/applyGain'
 * '<S84>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/Ramping_Subsystem/RampingGain/elseActionSubsystem'
 * '<S85>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/Ramping_Subsystem/RampingGain/ifActionSubsystem'
 * '<S86>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/Ramping_Subsystem/RampingGain/ifActionSubsystem/denCoefficient'
 * '<S87>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/Ramping_Subsystem/RampingGain/ifActionSubsystem/resetFilter'
 * '<S88>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/Ramping_Subsystem/RampingGain/ifActionSubsystem/resetFilter/If Action Subsystem'
 * '<S89>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/Ramping_Subsystem/RampingGain/ifActionSubsystem/resetFilter/If Action Subsystem1'
 * '<S90>'  : 'BasicAudio_Target/BasicAudio/Mute/mute/InnerLink/MuteRamper/Ramping_Subsystem/applyGain/For Each Subsystem'
 * '<S91>'  : 'BasicAudio_Target/BasicAudio/Treble/Tone'
 * '<S92>'  : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink'
 * '<S93>'  : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/RTC Set'
 * '<S94>'  : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/SetDsm'
 * '<S95>'  : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/SetDsm1'
 * '<S96>'  : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/Triggered Subsystem'
 * '<S97>'  : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/TuneVarOffset'
 * '<S98>'  : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/TuneVarRampTime'
 * '<S99>'  : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/TuneVarScale'
 * '<S100>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/check done'
 * '<S101>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/index to linear'
 * '<S102>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/ramp parameters'
 * '<S103>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/tone'
 * '<S104>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/tone control'
 * '<S105>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/RTC Set/Notification'
 * '<S106>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/RTC Set/NtfCheck'
 * '<S107>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/RTC Set/NtfPrep'
 * '<S108>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/RTC Set/REQ'
 * '<S109>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/RTC Set/Request'
 * '<S110>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/RTC Set/Response'
 * '<S111>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/SetDsm/DsmWrite'
 * '<S112>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/SetDsm/LockEnter'
 * '<S113>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/SetDsm/LockExit'
 * '<S114>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/SetDsm1/DsmWrite'
 * '<S115>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/SetDsm1/LockEnter'
 * '<S116>' : 'BasicAudio_Target/BasicAudio/Treble/Tone/InnerLink/SetDsm1/LockExit'
 * '<S117>' : 'BasicAudio_Target/BasicAudio/Volume/volume'
 * '<S118>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink'
 * '<S119>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Index2DbTable'
 * '<S120>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/RTC Set'
 * '<S121>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy'
 * '<S122>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Triggered Subsystem'
 * '<S123>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/TuneVarRampTimeMs'
 * '<S124>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/ramp parameters'
 * '<S125>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/select one channel gain'
 * '<S126>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Index2DbTable/TuneTOP'
 * '<S127>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Index2DbTable/lookup db from index'
 * '<S128>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/RTC Set/Notification'
 * '<S129>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/RTC Set/NtfCheck'
 * '<S130>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/RTC Set/NtfPrep'
 * '<S131>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/RTC Set/REQ'
 * '<S132>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/RTC Set/Request'
 * '<S133>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/RTC Set/Response'
 * '<S134>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/AudioRateSubsystem'
 * '<S135>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/ControlSubsystem'
 * '<S136>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/NtfCheckSubsystem'
 * '<S137>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/AudioRateSubsystem/rgainy process'
 * '<S138>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/ControlSubsystem/SetDsm'
 * '<S139>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/ControlSubsystem/SetDsm1'
 * '<S140>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/ControlSubsystem/rgainy ramper control'
 * '<S141>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/ControlSubsystem/SetDsm/DsmWrite'
 * '<S142>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/ControlSubsystem/SetDsm/LockEnter'
 * '<S143>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/ControlSubsystem/SetDsm/LockExit'
 * '<S144>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/ControlSubsystem/SetDsm1/DsmWrite'
 * '<S145>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/ControlSubsystem/SetDsm1/LockEnter'
 * '<S146>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/ControlSubsystem/SetDsm1/LockExit'
 * '<S147>' : 'BasicAudio_Target/BasicAudio/Volume/volume/InnerLink/Rgainy/NtfCheckSubsystem/rgainx check sequence'
 * '<S148>' : 'BasicAudio_Target/Input/Environment Controller'
 * '<S149>' : 'BasicAudio_Target/Input/ModelRootInport'
 * '<S150>' : 'BasicAudio_Target/Input/ModelRootInport/InportBuildTime'
 * '<S151>' : 'BasicAudio_Target/Input/ModelRootInport/TalariaApiBufferBuildTime'
 * '<S152>' : 'BasicAudio_Target/Output/ModelRootOutport'
 * '<S153>' : 'BasicAudio_Target/Output/ModelRootOutport/TalariaApiBufferBuildTime'
 */
#endif                                 /* RTW_HEADER_BasicAudio_Target_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
