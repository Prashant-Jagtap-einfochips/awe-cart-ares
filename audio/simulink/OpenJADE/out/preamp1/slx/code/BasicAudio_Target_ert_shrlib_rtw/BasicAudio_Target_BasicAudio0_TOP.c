/*
 * File: BasicAudio_Target_BasicAudio0_TOP.c
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

#include "BasicAudio_Target_TOP.h"
#if defined __SPECIFY_TOP_MEMORY_SECTION__

#pragma section("BASICAUDIO_TARGET_BASICAUDIO0_MEMORY_SECTION")

#endif

BasicAudio_Target_BasicAudio0_type BasicAudio_Target_BasicAudio0 =
{
    /* BalanceBoostDisable */
    0.0F,

    /* BalanceChanRampMap */
    {
        1.0F, 2.0F
    },

    /* BalanceEnableSilentExtreme */
    0.0F,

    /* BalanceOffset */
    127.0F,

    /* BalanceRampTime */
    30.0F,

    /* BalanceSleepingBeautyMutesBass */
    1.0F,

    /* BalanceTableDb */
    {
        -30.0F, -27.0F, -14.0F, -7.0F, -5.0F, -3.0F, -1.0F, 0.0F, -1.0F, -3.0F,
        -5.0F, -7.0F, -14.0F, -27.0F, -30.0F, -30.0F, -30.0F, -30.0F, -30.0F,
        -30.0F, -30.0F, -30.0F, -30.0F, -30.0F, -30.0F, -30.0F, -30.0F, -30.0F,
        -30.0F, -30.0F
    },

    /* BalanceTableIdx */
    {
        0.0F, 10.0F, 31.0F, 52.0F, 74.0F, 95.0F, 116.0F, 128.0F, 138.0F, 159.0F,
        180.0F, 202.0F, 223.0F, 244.0F, 255.0F, 255.0F, 255.0F, 255.0F, 255.0F,
        255.0F, 255.0F, 255.0F, 255.0F, 255.0F, 255.0F, 255.0F, 255.0F, 255.0F,
        255.0F, 255.0F
    },

    /* BassBoostCoeffs */
    {
        0.0129215717F, 0.0129215717F, 0.974156857F
    },

    /* BassBoostMaximum_dB */
    20.0F,

    /* BassCutCoeffs */
    {
        0.0129215717F, 0.0129215717F, 0.974156857F
    },

    /* BassCutMaximum_dB */
    20.0F,

    /* BassOffset */
    128.0F,

    /* BassScale */
    127.0F,

    /* Bassramptime */
    30.0F,

    /* MuteRampTime */
    100.0F,

    /* TrebleBoostCoeffs */
    {
        0.987078428F, -0.987078428F, 0.974156857F
    },

    /* TrebleBoostMaximum_dB */
    20.0F,

    /* TrebleCutCoeffs */
    {
        0.987078428F, -0.987078428F, 0.974156857F
    },

    /* TrebleCutMaximum_dB */
    20.0F,

    /* TrebleOffset */
    128.0F,

    /* TrebleScale */
    127.0F,

    /* Trebleramptime */
    30.0F,

    /* VolumeRampTime */
    100.0F,

    /* VolumeTable_dB */
    {
        -100.0F, -30.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F
    },

    /* Volumemaxgain */
    0.0F,

    /* VolumeTable_Idx */
    {
        0U, 128U, 255U, 255U, 255U, 255U, 255U, 255U
    }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
