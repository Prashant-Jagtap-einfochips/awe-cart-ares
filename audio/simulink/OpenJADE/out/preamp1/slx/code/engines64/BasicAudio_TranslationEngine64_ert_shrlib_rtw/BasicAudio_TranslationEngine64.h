/*
 * File: BasicAudio_TranslationEngine64.h
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

#ifndef RTW_HEADER_BasicAudio_TranslationEngine64_h_
#define RTW_HEADER_BasicAudio_TranslationEngine64_h_
#ifndef BasicAudio_TranslationEngine64_COMMON_INCLUDES_
#define BasicAudio_TranslationEngine64_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "BasicAudio_TranslationEngine64_TuneVar.h"
#include "TranslateError.h"
#include "BasicAudio_TranslationEngine64_TalariaAPI.h"
#endif                     /* BasicAudio_TranslationEngine64_COMMON_INCLUDES_ */

#include "BasicAudio_TranslationEngine64_types.h"
#include "rtGetInf.h"
#include <stddef.h>
#include <string.h>
#include "rt_nonfinite.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* user code (top of header file) */
// Auto-generated code for model BasicAudio
/**** TOP TOKEN COMPATIBILITY HASH VALUES ****/
#define TOP_COMPAT                     {0x6E521A79UL}
#define TOP_COMPAT_NAME                {"BasicAudio"}
#define TOP_COMPAT_NAME_HASH           {0x9FD22631UL}

/**** COMPAT HASH VALUES BY MODULE ****/
#define BASICAUDIO_TOP_COMPAT          0x6E521A79UL

/**** NAME HASH VALUES BY MODULE ****/
#define BASICAUDIO_TOP_NAME_HASH       0x9FD22631UL

/* Block signals for system '<S43>/tone filter tune translate3' */
typedef struct
{
    real32_T maxOut;                   /* '<S43>/tone filter tune translate3' */
}
B_tonefiltertunetranslate3_BasicAudio_TranslationEngine64_T;

/* Block signals for system '<S43>/tone filter tune translate1' */
typedef struct
{
    real32_T FilterCoeffs[3];          /* '<S43>/tone filter tune translate1' */
}
B_tonefiltertunetranslate1_BasicAudio_TranslationEngine64_T;

/* Block signals for system '<S80>/tone filter tune translate1' */
typedef struct
{
    real32_T FilterCoeffs[3];          /* '<S80>/tone filter tune translate1' */
}
B_tonefiltertunetranslate1_BasicAudio_TranslationEngine64_i_T;

/* Block signals (default storage) */
typedef struct
{
    bus_double_30__2 Table;            /* '<S16>/TuneVarBalanceTable' */
    bus_double_8_2 Table_i;            /* '<S110>/TuneVar_Table' */
    bus_double_1_2 ChanRampMap;        /* '<S9>/TuneVarChanRampMap' */
    bus_double_1_1 maxgain;            /* '<S113>/Tune Variable' */
    bus_double_1_1 RampTime;           /* '<S111>/Tune Variable' */
    bus_double_1_1 BoostFrequencyHz;   /* '<S80>/BoostHz' */
    bus_double_1_1 CutFrequencyHz;     /* '<S80>/CutHz' */
    bus_double_1_1 BoostMaximum_dB;    /* '<S80>/Boost_dB' */
    bus_double_1_1 CutMaximum_dB;      /* '<S80>/Cut_dB' */
    bus_double_1_1 Offset;             /* '<S85>/Tune Variable' */
    bus_double_1_1 ramptime;           /* '<S86>/Tune Variable' */
    bus_double_1_1 Scale;              /* '<S87>/Tune Variable' */
    bus_double_1_1 RampTime_c;         /* '<S74>/Tune Variable' */
    bus_double_1_1 BoostFrequencyHz_o; /* '<S43>/BoostHz' */
    bus_double_1_1 CutFrequencyHz_o;   /* '<S43>/CutHz' */
    bus_double_1_1 BoostMaximum_dB_i;  /* '<S43>/Boost_dB' */
    bus_double_1_1 CutMaximum_dB_a;    /* '<S43>/Cut_dB' */
    bus_double_1_1 Offset_l;           /* '<S48>/Tune Variable' */
    bus_double_1_1 ramptime_l;         /* '<S49>/Tune Variable' */
    bus_double_1_1 Scale_o;            /* '<S50>/Tune Variable' */
    bus_double_1_1 BoostDisable;       /* '<S11>/Tune Variable' */
    bus_double_1_1 Offset_k;           /* '<S12>/Tune Variable' */
    bus_double_1_1 RampTime_d;         /* '<S13>/Tune Variable' */
    bus_double_1_1 SleepingBeautyMutesBass;/* '<S14>/Tune Variable' */
    bus_double_1_1 EnableSilentExtreme;/* '<S15>/Tune Variable' */
    real_T Value;
    real_T Value_i;
    real_T Value_j;
    real_T Value_l;
    real_T Value_e;
    real_T Value_b;
    real_T Value_h;
    real_T Value_lm;
    real_T Value_p;
    real_T Value_g;
    real_T Value_ih;
    real_T Value_d;
    real_T Value_iw;
    real_T Value_l0;
    uint32_T Table_Idx[8];             /* '<S110>/table translation' */
    uint32_T tableIdx[30];             /* '<S16>/Balance_Table_Translation' */
    real32_T Table_dB[8];              /* '<S110>/table translation' */
    real32_T tableDb[30];              /* '<S16>/Balance_Table_Translation' */
    int32_T ChanRampMap_i[2];          /* '<S9>/channel_ramper_translation' */
    B_tonefiltertunetranslate3_BasicAudio_TranslationEngine64_T
        sf_tonefiltertunetranslate4_lk;/* '<S80>/tone filter tune translate4' */
    B_tonefiltertunetranslate1_BasicAudio_TranslationEngine64_i_T
        sf_tonefiltertunetranslate2_fq;/* '<S80>/tone filter tune translate2' */
    B_tonefiltertunetranslate1_BasicAudio_TranslationEngine64_i_T
        sf_tonefiltertunetranslate1_d; /* '<S80>/tone filter tune translate1' */
    B_tonefiltertunetranslate3_BasicAudio_TranslationEngine64_T
        sf_tonefiltertunetranslate3_fg;/* '<S80>/tone filter tune translate3' */
    B_tonefiltertunetranslate3_BasicAudio_TranslationEngine64_T
        sf_tonefiltertunetranslate4_i; /* '<S43>/tone filter tune translate4' */
    B_tonefiltertunetranslate1_BasicAudio_TranslationEngine64_T
        sf_tonefiltertunetranslate2_e; /* '<S43>/tone filter tune translate2' */
    B_tonefiltertunetranslate1_BasicAudio_TranslationEngine64_T
        sf_tonefiltertunetranslate1;   /* '<S43>/tone filter tune translate1' */
    B_tonefiltertunetranslate3_BasicAudio_TranslationEngine64_T
        sf_tonefiltertunetranslate3;   /* '<S43>/tone filter tune translate3' */
}
B_BasicAudio_TranslationEngine64_T;

/* Invariant block signals (default storage) */
typedef struct
{
    const real32_T CapturedTuneConst;  /* '<S80>/Captured Tune Const' */
    const real32_T CapturedTuneConst_j;/* '<S43>/Captured Tune Const' */
    const real32_T CapturedTuneConst_i;/* '<S9>/Captured Tune Const' */
}
ConstB_BasicAudio_TranslationEngine64_T;

/* Real-time Model Data Structure */
struct tag_RTM_BasicAudio_TranslationEngine64_T
{
    const char_T * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_BasicAudio_TranslationEngine64_T BasicAudio_TranslationEngine64_B;
extern const ConstB_BasicAudio_TranslationEngine64_T
    BasicAudio_TranslationEngine64_ConstB;/* constant block i/o */

/* Model entry point functions */
extern void BasicAudio_TranslationEngine64_initialize(void);
extern void BasicAudio_TranslationEngine64_step(void);
extern void BasicAudio_TranslationEngine64_terminate(void);
extern void BasicAudio_TranslationEngine64_InitializeTranslationRoot( void );

/* Real-time Model object */
extern RT_MODEL_BasicAudio_TranslationEngine64_T *const
    BasicAudio_TranslationEngine64_M;

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
 * '<Root>' : 'BasicAudio_TranslationEngine64'
 * '<S1>'   : 'BasicAudio_TranslationEngine64/BasicAudio'
 * '<S2>'   : 'BasicAudio_TranslationEngine64/CoreIndex'
 * '<S3>'   : 'BasicAudio_TranslationEngine64/BasicAudio/Balance'
 * '<S4>'   : 'BasicAudio_TranslationEngine64/BasicAudio/Bass'
 * '<S5>'   : 'BasicAudio_TranslationEngine64/BasicAudio/Mute'
 * '<S6>'   : 'BasicAudio_TranslationEngine64/BasicAudio/Treble'
 * '<S7>'   : 'BasicAudio_TranslationEngine64/BasicAudio/Volume'
 * '<S8>'   : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance'
 * '<S9>'   : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink'
 * '<S10>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TranslateTrigger'
 * '<S11>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarBoostDisable'
 * '<S12>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarOffset'
 * '<S13>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarRampTime'
 * '<S14>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarSilentBass'
 * '<S15>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarSilentExtreme'
 * '<S16>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarTable'
 * '<S17>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/channel_ramper_translation'
 * '<S18>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TranslateTrigger/tfunc'
 * '<S19>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarBoostDisable/TrivialTranslation'
 * '<S20>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarBoostDisable/TrivialTranslation/Subsystem'
 * '<S21>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarBoostDisable/TrivialTranslation/TranslateTrigger1'
 * '<S22>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarBoostDisable/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S23>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarOffset/TrivialTranslation'
 * '<S24>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarOffset/TrivialTranslation/Subsystem'
 * '<S25>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarOffset/TrivialTranslation/TranslateTrigger1'
 * '<S26>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarOffset/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S27>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarRampTime/TrivialTranslation'
 * '<S28>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarRampTime/TrivialTranslation/Subsystem'
 * '<S29>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarRampTime/TrivialTranslation/TranslateTrigger1'
 * '<S30>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarRampTime/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S31>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarSilentBass/TrivialTranslation'
 * '<S32>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarSilentBass/TrivialTranslation/Subsystem'
 * '<S33>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarSilentBass/TrivialTranslation/TranslateTrigger1'
 * '<S34>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarSilentBass/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S35>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarSilentExtreme/TrivialTranslation'
 * '<S36>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarSilentExtreme/TrivialTranslation/Subsystem'
 * '<S37>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarSilentExtreme/TrivialTranslation/TranslateTrigger1'
 * '<S38>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarSilentExtreme/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S39>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarTable/Balance_Table_Translation'
 * '<S40>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarTable/TranslateTrigger'
 * '<S41>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Balance/balance/InnerLink/TuneVarTable/TranslateTrigger/tfunc'
 * '<S42>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone'
 * '<S43>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink'
 * '<S44>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TranslateTrigger1'
 * '<S45>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TranslateTrigger2'
 * '<S46>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TranslateTrigger3'
 * '<S47>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TranslateTrigger4'
 * '<S48>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarOffset'
 * '<S49>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarRampTime'
 * '<S50>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarScale'
 * '<S51>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/tone filter tune translate1'
 * '<S52>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/tone filter tune translate2'
 * '<S53>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/tone filter tune translate3'
 * '<S54>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/tone filter tune translate4'
 * '<S55>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TranslateTrigger1/tfunc'
 * '<S56>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TranslateTrigger2/tfunc'
 * '<S57>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TranslateTrigger3/tfunc'
 * '<S58>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TranslateTrigger4/tfunc'
 * '<S59>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarOffset/TrivialTranslation'
 * '<S60>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarOffset/TrivialTranslation/Subsystem'
 * '<S61>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarOffset/TrivialTranslation/TranslateTrigger1'
 * '<S62>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarOffset/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S63>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarRampTime/TrivialTranslation'
 * '<S64>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarRampTime/TrivialTranslation/Subsystem'
 * '<S65>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarRampTime/TrivialTranslation/TranslateTrigger1'
 * '<S66>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarRampTime/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S67>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarScale/TrivialTranslation'
 * '<S68>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarScale/TrivialTranslation/Subsystem'
 * '<S69>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarScale/TrivialTranslation/TranslateTrigger1'
 * '<S70>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Bass/Tone/InnerLink/TuneVarScale/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S71>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Mute/mute'
 * '<S72>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Mute/mute/InnerLink'
 * '<S73>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Mute/mute/InnerLink/MuteControl'
 * '<S74>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Mute/mute/InnerLink/MuteControl/TuneVarRampTime'
 * '<S75>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Mute/mute/InnerLink/MuteControl/TuneVarRampTime/TrivialTranslation'
 * '<S76>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Mute/mute/InnerLink/MuteControl/TuneVarRampTime/TrivialTranslation/Subsystem'
 * '<S77>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Mute/mute/InnerLink/MuteControl/TuneVarRampTime/TrivialTranslation/TranslateTrigger1'
 * '<S78>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Mute/mute/InnerLink/MuteControl/TuneVarRampTime/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S79>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone'
 * '<S80>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink'
 * '<S81>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TranslateTrigger1'
 * '<S82>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TranslateTrigger2'
 * '<S83>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TranslateTrigger3'
 * '<S84>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TranslateTrigger4'
 * '<S85>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarOffset'
 * '<S86>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarRampTime'
 * '<S87>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarScale'
 * '<S88>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/tone filter tune translate1'
 * '<S89>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/tone filter tune translate2'
 * '<S90>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/tone filter tune translate3'
 * '<S91>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/tone filter tune translate4'
 * '<S92>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TranslateTrigger1/tfunc'
 * '<S93>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TranslateTrigger2/tfunc'
 * '<S94>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TranslateTrigger3/tfunc'
 * '<S95>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TranslateTrigger4/tfunc'
 * '<S96>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarOffset/TrivialTranslation'
 * '<S97>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarOffset/TrivialTranslation/Subsystem'
 * '<S98>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarOffset/TrivialTranslation/TranslateTrigger1'
 * '<S99>'  : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarOffset/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S100>' : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarRampTime/TrivialTranslation'
 * '<S101>' : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarRampTime/TrivialTranslation/Subsystem'
 * '<S102>' : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarRampTime/TrivialTranslation/TranslateTrigger1'
 * '<S103>' : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarRampTime/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S104>' : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarScale/TrivialTranslation'
 * '<S105>' : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarScale/TrivialTranslation/Subsystem'
 * '<S106>' : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarScale/TrivialTranslation/TranslateTrigger1'
 * '<S107>' : 'BasicAudio_TranslationEngine64/BasicAudio/Treble/Tone/InnerLink/TuneVarScale/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S108>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume'
 * '<S109>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink'
 * '<S110>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/Index2DbTable'
 * '<S111>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/TuneVarRampTimeMs'
 * '<S112>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/Index2DbTable/TranslateTrigger1'
 * '<S113>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/Index2DbTable/TuneTOP'
 * '<S114>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/Index2DbTable/table translation'
 * '<S115>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/Index2DbTable/TranslateTrigger1/tfunc'
 * '<S116>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/Index2DbTable/TuneTOP/TrivialTranslation'
 * '<S117>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/Index2DbTable/TuneTOP/TrivialTranslation/Subsystem'
 * '<S118>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/Index2DbTable/TuneTOP/TrivialTranslation/TranslateTrigger1'
 * '<S119>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/Index2DbTable/TuneTOP/TrivialTranslation/TranslateTrigger1/tfunc'
 * '<S120>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/TuneVarRampTimeMs/TrivialTranslation'
 * '<S121>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/TuneVarRampTimeMs/TrivialTranslation/Subsystem'
 * '<S122>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/TuneVarRampTimeMs/TrivialTranslation/TranslateTrigger1'
 * '<S123>' : 'BasicAudio_TranslationEngine64/BasicAudio/Volume/volume/InnerLink/TuneVarRampTimeMs/TrivialTranslation/TranslateTrigger1/tfunc'
 */
#endif                        /* RTW_HEADER_BasicAudio_TranslationEngine64_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
