/*
 * File: BasicAudio_Target_TOP.h
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

#ifndef RTW_HEADER_BasicAudio_Target_TOP_h_
#define RTW_HEADER_BasicAudio_Target_TOP_h_
#include "rtwtypes.h"

typedef struct BasicAudio_Target_BasicAudio0_tag
{
    real32_T BalanceBoostDisable;      /* '<S16>/TOP' */
    real32_T BalanceChanRampMap[2];    /* '<S11>/TOP' */
    real32_T BalanceEnableSilentExtreme;/* '<S20>/TOP' */
    real32_T BalanceOffset;            /* '<S17>/TOP' */
    real32_T BalanceRampTime;          /* '<S18>/TOP' */
    real32_T BalanceSleepingBeautyMutesBass;/* '<S19>/TOP' */
    real32_T BalanceTableDb[30];       /* '<S21>/TOP_TableIDb' */
    real32_T BalanceTableIdx[30];      /* '<S21>/TOP_TableIdx' */
    real32_T BassBoostCoeffs[3];       /* '<S40>/BoostCoeffs' */
    real32_T BassBoostMaximum_dB;      /* '<S40>/Boost_dB TOP' */
    real32_T BassCutCoeffs[3];         /* '<S40>/CutCoeffs' */
    real32_T BassCutMaximum_dB;        /* '<S40>/Cut_dB TOP' */
    real32_T BassOffset;               /* '<S45>/TOP' */
    real32_T BassScale;                /* '<S47>/TOP' */
    real32_T Bassramptime;             /* '<S46>/TOP' */
    real32_T MuteRampTime;             /* '<S71>/TOP' */
    real32_T TrebleBoostCoeffs[3];     /* '<S92>/BoostCoeffs' */
    real32_T TrebleBoostMaximum_dB;    /* '<S92>/Boost_dB TOP' */
    real32_T TrebleCutCoeffs[3];       /* '<S92>/CutCoeffs' */
    real32_T TrebleCutMaximum_dB;      /* '<S92>/Cut_dB TOP' */
    real32_T TrebleOffset;             /* '<S97>/TOP' */
    real32_T TrebleScale;              /* '<S99>/TOP' */
    real32_T Trebleramptime;           /* '<S98>/TOP' */
    real32_T VolumeRampTime;           /* '<S123>/TOP' */
    real32_T VolumeTable_dB[8];        /* '<S119>/TOP_GainDB' */
    real32_T Volumemaxgain;            /* '<S126>/TOP' */
    uint32_T VolumeTable_Idx[8];       /* '<S119>/TOP_Index' */
}
BasicAudio_Target_BasicAudio0_type;

#define TOP_BANK_BasicAudio_Target_BasicAudio0 &BasicAudio_Target_BasicAudio0
#define TOP_BANK_BasicAudio_Target_BasicAudio0_SIZE sizeof(BasicAudio_Target_BasicAudio0_type)

// Unused banks
// ------------
#define TOP_BANK_BasicAudio_Target_BasicAudio1 NULL
#define TOP_BANK_BasicAudio_Target_BasicAudio1_SIZE 0
#define TOP_BANK_BasicAudio_Target_BasicAudio2 NULL
#define TOP_BANK_BasicAudio_Target_BasicAudio2_SIZE 0
#define TOP_BANK_BasicAudio_Target_BasicAudio3 NULL
#define TOP_BANK_BasicAudio_Target_BasicAudio3_SIZE 0
#define TOP_BANK_BasicAudio_Target_BasicAudio4 NULL
#define TOP_BANK_BasicAudio_Target_BasicAudio4_SIZE 0
#define TOP_BANK_BasicAudio_Target_BasicAudio5 NULL
#define TOP_BANK_BasicAudio_Target_BasicAudio5_SIZE 0
#define TOP_BANK_BasicAudio_Target_BasicAudio6 NULL
#define TOP_BANK_BasicAudio_Target_BasicAudio6_SIZE 0

extern BasicAudio_Target_BasicAudio0_type BasicAudio_Target_BasicAudio0;

#endif                                 /* RTW_HEADER_BasicAudio_Target_TOP_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
