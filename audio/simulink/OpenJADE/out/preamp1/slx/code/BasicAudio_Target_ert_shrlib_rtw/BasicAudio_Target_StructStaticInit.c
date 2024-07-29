/*
 * File: BasicAudio_Target_StructStaticInit.c
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

#include "BasicAudio_Target_StructStaticInit.h"

/* StructStaticInit instance definitions */
BasicAudio_TargetBasicAudioBalanceSetRtcOut_type
    BasicAudio_TargetBasicAudioBalanceSetRtcOut =
{
    /* REQ_caller_handle */
    0U,

    /* REQ_gain_index */
    0U,

    /* REQ_ramp_db_per_second */
    0U,

    /* REQ_ramp_milliseconds */
    0U,

    /* REQ_sequence */
    0U,
};

BasicAudio_TargetBasicAudioBassSetRtcOut_type
    BasicAudio_TargetBasicAudioBassSetRtcOut =
{
    /* REQ_caller_handle */
    0U,

    /* REQ_gain_index */
    1U,

    /* REQ_ramp_db_per_second */
    255U,

    /* REQ_ramp_milliseconds */
    0U,

    /* REQ_sequence */
    0U,
};

BasicAudio_TargetBasicAudioMuteSetRtcOut_type
    BasicAudio_TargetBasicAudioMuteSetRtcOut =
{
    /* REQ_caller_handle */
    0U,

    /* REQ_mute_switch */
    0U,

    /* REQ_ramp_db_per_second */
    0U,

    /* REQ_ramp_milliseconds */
    0U,

    /* REQ_sequence */
    0U,
};

BasicAudio_TargetBasicAudioTrebleSetRtcOut_type
    BasicAudio_TargetBasicAudioTrebleSetRtcOut =
{
    /* REQ_caller_handle */
    0U,

    /* REQ_gain_index */
    1U,

    /* REQ_ramp_db_per_second */
    255U,

    /* REQ_ramp_milliseconds */
    0U,

    /* REQ_sequence */
    0U,
};

BasicAudio_TargetBasicAudioVolumeSetRtcOut_type
    BasicAudio_TargetBasicAudioVolumeSetRtcOut =
{
    /* REQ_gain_offset */
    0,

    /* REQ_caller_handle */
    0U,

    /* REQ_gain_index */
    0U,

    /* REQ_ramp_db_per_second */
    0U,

    /* REQ_ramp_milliseconds */
    0U,

    /* REQ_sequence */
    0U,
};

BasicAudio_TargetControlDsms_type BasicAudio_TargetControlDsms =
{
    /* BasicAudioBalanceRampers */
    {
        {
            0,
            1.0F,
            1.0F,
            1.0F
        },
        {
            0,
            1.0F,
            1.0F,
            1.0F
        },
        {
            0,
            1.0F,
            1.0F,
            1.0F
        },
        {
            0,
            1.0F,
            1.0F,
            1.0F
        }
    },

    /* BasicAudioBassRampers */
    {
        {
            0,
            1.0F,
            1.0F,
            1.0F
        },
        {
            0,
            1.0F,
            1.0F,
            1.0F
        }
    },

    /* BasicAudioTrebleRampers */
    {
        {
            0,
            1.0F,
            1.0F,
            1.0F
        },
        {
            0,
            1.0F,
            1.0F,
            1.0F
        }
    },

    /* BasicAudioVolumeRampers */
    {
        0,
        1.0F,
        0.0F,
        0.0F
    },

    /* BasicAudioMuteCurrentGain */
    1.0F,

    /* BasicAudioMuteNframes */
    0.0F,

    /* BasicAudioMuteTargetLinearGain */
    1.0F,

    /* BasicAudioBalanceFreeze */
    0U,

    /* BasicAudioBassFreeze */
    0U,

    /* BasicAudioBassPhase */
    1U,

    /* BasicAudioTrebleFreeze */
    0U,

    /* BasicAudioTreblePhase */
    1U,

    /* BasicAudioVolumeFreeze */
    0U
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
