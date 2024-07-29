/*
 * File: BasicAudio_Target_StructStaticInit.h
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

#ifndef RTW_HEADER_BasicAudio_Target_StructStaticInit_h_
#define RTW_HEADER_BasicAudio_Target_StructStaticInit_h_
#include "rtwtypes.h"
#include "BasicAudio_Target_types.h"

/* StructStaticInit type definitions */
typedef struct BasicAudio_TargetBasicAudioBalanceSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S31>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S31>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S31>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S31>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S31>/UnpackREQ' */
}
BasicAudio_TargetBasicAudioBalanceSetRtcOut_type;

typedef struct BasicAudio_TargetBasicAudioBassSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S57>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S57>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S57>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S57>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S57>/UnpackREQ' */
}
BasicAudio_TargetBasicAudioBassSetRtcOut_type;

typedef struct BasicAudio_TargetBasicAudioMuteSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S77>/UnpackREQ' */
    uint32_T REQ_mute_switch;          /* '<S77>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S77>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S77>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S77>/UnpackREQ' */
}
BasicAudio_TargetBasicAudioMuteSetRtcOut_type;

typedef struct BasicAudio_TargetBasicAudioTrebleSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S109>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S109>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S109>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S109>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S109>/UnpackREQ' */
}
BasicAudio_TargetBasicAudioTrebleSetRtcOut_type;

typedef struct BasicAudio_TargetBasicAudioVolumeSetRtcOut_tag
{
    int32_T REQ_gain_offset;           /* '<S132>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S132>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S132>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S132>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S132>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S132>/UnpackREQ' */
}
BasicAudio_TargetBasicAudioVolumeSetRtcOut_type;

typedef struct BasicAudio_TargetControlDsms_tag
{
    rgain_bus BasicAudioBalanceRampers[4];/* '<S11>/ramp control' */
    rgain_bus BasicAudioBassRampers[2];/* '<S40>/ramp control' */
    rgain_bus BasicAudioTrebleRampers[2];/* '<S92>/ramp control' */
    rgain_bus BasicAudioVolumeRampers; /* '<S121>/ramp control' */
    real32_T BasicAudioMuteCurrentGain;/* '<S68>/currentGain' */
    real32_T BasicAudioMuteNframes;    /* '<S68>/nFrames' */
    real32_T BasicAudioMuteTargetLinearGain;/* '<S68>/targetLinearGain' */
    uint32_T BasicAudioBalanceFreeze;  /* '<S11>/freeze control' */
    uint32_T BasicAudioBassFreeze;     /* '<S40>/freeze control' */
    uint32_T BasicAudioBassPhase;      /* '<S40>/phase control' */
    uint32_T BasicAudioTrebleFreeze;   /* '<S92>/freeze control' */
    uint32_T BasicAudioTreblePhase;    /* '<S92>/phase control' */
    uint32_T BasicAudioVolumeFreeze;   /* '<S121>/freeze control' */
}
BasicAudio_TargetControlDsms_type;

/* StructStaticInit instance declarations */
extern BasicAudio_TargetBasicAudioBalanceSetRtcOut_type
    BasicAudio_TargetBasicAudioBalanceSetRtcOut;
extern BasicAudio_TargetBasicAudioBassSetRtcOut_type
    BasicAudio_TargetBasicAudioBassSetRtcOut;
extern BasicAudio_TargetBasicAudioMuteSetRtcOut_type
    BasicAudio_TargetBasicAudioMuteSetRtcOut;
extern BasicAudio_TargetBasicAudioTrebleSetRtcOut_type
    BasicAudio_TargetBasicAudioTrebleSetRtcOut;
extern BasicAudio_TargetBasicAudioVolumeSetRtcOut_type
    BasicAudio_TargetBasicAudioVolumeSetRtcOut;
extern BasicAudio_TargetControlDsms_type BasicAudio_TargetControlDsms;

#endif                    /* RTW_HEADER_BasicAudio_Target_StructStaticInit_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
