/*
 * File: BasicAudio_Target_private.h
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

#ifndef RTW_HEADER_BasicAudio_Target_private_h_
#define RTW_HEADER_BasicAudio_Target_private_h_
#include "rtwtypes.h"
#include "BasicAudio_Target.h"
#include "BasicAudio_Target_types.h"
#include "BasicAudio_Target_StructStaticInit.h" // from BoseFlatStructure.tlc

void RtcReqRsp_BasicAudioBalanceSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_BasicAudioBalanceSet( void );
void RtcNtfPack_BasicAudioBalanceSet(void* pNtfBuf);
void RtcReqRsp_BasicAudioBassSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_BasicAudioBassSet( void );
void RtcNtfPack_BasicAudioBassSet(void* pNtfBuf);
void RtcReqRsp_BasicAudioMuteSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_BasicAudioMuteSet( void );
void RtcNtfPack_BasicAudioMuteSet(void* pNtfBuf);
void RtcReqRsp_BasicAudioTrebleSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_BasicAudioTrebleSet( void );
void RtcNtfPack_BasicAudioTrebleSet(void* pNtfBuf);
void RtcReqRsp_BasicAudioVolumeSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_BasicAudioVolumeSet( void );
void RtcNtfPack_BasicAudioVolumeSet(void* pNtfBuf);
extern void BasicAudio_Target_LockEnter(void);
extern void BasicAudio_Target_LockExit(void);
extern void BasicAudio_Target_SetDsm(void);
extern void BasicAudio_Target_control(const real32_T rtu_targetGainLinear[4],
    real32_T rtu_rampdbPerSec, real32_T rtu_rampTimesMs, real32_T
    rtu_sampleRateInHertz, real32_T rtu_frameSize);
extern void BasicAudio_Target_SetDsm1(void);
extern void BasicAudio_Target_indextolinear(real32_T rtu_scale, real32_T
    rtu_offset, uint32_T rtu_index, B_indextolinear_BasicAudio_Target_T *localB);
extern void BasicAudio_Target_rampparameters(real32_T rtu_data, uint32_T
    rtu_rampTimeRTC, uint32_T rtu_rampRateRTC, const real_T rtp_rampRateBounds[2],
    const real_T rtp_rampTimeBounds[2], B_rampparameters_BasicAudio_Target_T
    *localB);
extern void BasicAudio_Target_SetDsm_a(void);
extern void BasicAudio_Target_tonecontrol(real32_T rtu_boost_max_db, real32_T
    rtu_cut_max_db, real32_T rtu_normalized_gain_impact, real32_T rtu_db_per_sec,
    real32_T rtu_ramp_time_ms, real32_T rtu_sample_rate_in_hertz, real32_T
    rtu_frame_size);
extern void BasicAudio_Target_SetDsm1_m(void);
extern void BasicAudio_Target_tone(const real32_T rtu_boost_coeffs[3], const
    real32_T rtu_cut_coeffs[3], const real32_T rtu_audioIn[64],
    B_tone_BasicAudio_Target_T *localB, DW_tone_BasicAudio_Target_T *localDW);
extern void BasicAudio_Target_SetDsm_l(void);
extern void BasicAudio_Target_tonecontrol_o(real32_T rtu_boost_max_db, real32_T
    rtu_cut_max_db, real32_T rtu_normalized_gain_impact, real32_T rtu_db_per_sec,
    real32_T rtu_ramp_time_ms, real32_T rtu_sample_rate_in_hertz, real32_T
    rtu_frame_size);
extern void BasicAudio_Target_SetDsm1_f(void);
extern void BasicAudio_Target_tone_e(const real32_T rtu_boost_coeffs[3], const
    real32_T rtu_cut_coeffs[3], const real32_T rtu_audioIn[64],
    B_tone_BasicAudio_Target_h_T *localB, DW_tone_BasicAudio_Target_p_T *localDW);
extern void BasicAudio_Target_lookupdbfromindex(const uint32_T rtu_volTableIdx[8],
    const real32_T rtu_volTabledB[8], uint32_T rtu_idx, int32_T rtu_db_offset,
    real32_T rtu_maxgain, B_lookupdbfromindex_BasicAudio_Target_T *localB);
extern void BasicAudio_Target_rampparameters_o(uint32_T rtu_rampRateRTC,
    uint32_T rtu_rampTimeRTC, real32_T rtu_rampTimeTune,
    B_rampparameters_BasicAudio_Target_m_T *localB);
extern void BasicAudio_Target_SetDsm_f(void);
extern void BasicAudio_Target_rgainyrampercontrol(real32_T
    rtu_target_gains_linear, real32_T rtu_db_per_sec, real32_T rtu_ramp_times_ms,
    real32_T rtu_sample_rate_in_hertz, real32_T rtu_frame_size);
extern void BasicAudio_Target_SetDsm1_g(void);
extern void BasicAudio_Target_rgainyprocess(const uint8_T rtu_rampToChanMap[3],
    const real32_T rtu_audioIn[64], B_rgainyprocess_BasicAudio_Target_T *localB);
extern void HandleReqBasicAudioBalanceSet(void);
extern void HandleReqBasicAudioBassSet(void);
extern void HandleReqBasicAudioMuteSet(void);
extern void HandleReqBasicAudioTrebleSet(void);
extern void HandleReqBasicAudioVolumeSet(void);

/* Exported data declaration */

/* Declaration for custom storage class: StructStaticInit */
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

#endif                             /* RTW_HEADER_BasicAudio_Target_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
