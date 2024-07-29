/*
 * File: Model_Target_private.h
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

#ifndef RTW_HEADER_Model_Target_private_h_
#define RTW_HEADER_Model_Target_private_h_
#include "rtwtypes.h"
#include "Model_Target.h"
#include "Model_Target_types.h"

// Intel signal-processing primitives
#if defined (PCPREQ4)
#include <math.h>

//#include <ipp.h>
#elif defined (__HEXAGON_ARCH__)
#include <math.h>
#elif defined (__HEXAGON_ARCH__) && defined (MULTITHREAD)
#include "srcThread.h"
#include "qurt.h"
#include "qurt_sem.h"
#ifndef MAX_THREAD_NUM
#define MAX_THREAD_NUM                 4
#endif                                 //MAX_THREAD_NUM

#ifndef THREAD_NAME_LEN
#define THREAD_NAME_LEN                20
#endif                                 //THREAD_NAME_LEN

#if (MAX_THREAD_NUM != 4) && (MAX_THREAD_NUM != 6)
#error "Support only 4 or 6 threads!"
#endif                                 //MAX_THREAD_NUM == 4,6
#endif

#include "Model_Target_StructStaticInit.h" // from BoseFlatStructure.tlc

extern void* TspDispatchTable0000;
void RtcReqRsp_InputSelectRouterCfgSet(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_NearFr_RouterSet(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_NearAnn1Ann1VolumeSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_NearAnn1Ann1VolumeSet( void );
void RtcNtfPack_NearAnn1Ann1VolumeSet(void* pNtfBuf);
void RtcReqRsp_NearAnn2Ann2VolumeSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_NearAnn2Ann2VolumeSet( void );
void RtcNtfPack_NearAnn2Ann2VolumeSet(void* pNtfBuf);
void RtcReqRsp_NearAnn3Ann3VolumeSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_NearAnn3Ann3VolumeSet( void );
void RtcNtfPack_NearAnn3Ann3VolumeSet(void* pNtfBuf);
void RtcReqRsp_NearAnn4Ann4VolumeSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_NearAnn4Ann4VolumeSet( void );
void RtcNtfPack_NearAnn4Ann4VolumeSet(void* pNtfBuf);
void RtcReqRsp_NearAnn1Ann1NonEntGainCalcvncEnableRTC(void* pReqBuf, void
    * pRspBuf);
void RtcReqRsp_NearAnn1Ann1MuteSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_NearAnn1Ann1MuteSet( void );
void RtcNtfPack_NearAnn1Ann1MuteSet(void* pNtfBuf);
void RtcReqRsp_NearAnn2Ann2NonEntGainCalcvncEnableRTC(void* pReqBuf, void
    * pRspBuf);
void RtcReqRsp_NearAnn2Ann2MuteSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_NearAnn2Ann2MuteSet( void );
void RtcNtfPack_NearAnn2Ann2MuteSet(void* pNtfBuf);
void RtcReqRsp_NearAnn3Ann3NonEntGainCalcvncEnableRTC(void* pReqBuf, void
    * pRspBuf);
void RtcReqRsp_NearAnn3Ann3MuteSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_NearAnn3Ann3MuteSet( void );
void RtcNtfPack_NearAnn3Ann3MuteSet(void* pNtfBuf);
void RtcReqRsp_NearAnn4Ann4NonEntGainCalcvncEnableRTC(void* pReqBuf, void
    * pRspBuf);
void RtcReqRsp_NearAnn4Ann4MuteSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_NearAnn4Ann4MuteSet( void );
void RtcNtfPack_NearAnn4Ann4MuteSet(void* pNtfBuf);
void RtcReqRsp_NearAnnNoiseLevel(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_NearChannel1_Location(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_NearChannel2_Location(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_NearChannel3_Location(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_NearChannel4_Location(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_NearDr_RouterSet(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_NearEnable(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PostProcessMuteCfgSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_PostProcessMuteCfgSet( void );
void RtcNtfPack_PostProcessMuteCfgSet(void* pNtfBuf);
void RtcReqRsp_PostProcessLimiterSoftclipParamSet(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PostProcessLimiterSoftclipDisable(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PostProcessEnergyLimiterConfigEnableLimiter(void* pReqBuf, void
    * pRspBuf);
void RtcNtfCheck_PostProcessEnergyLimiterConfigEnableLimiter( void );
void RtcNtfPack_PostProcessEnergyLimiterConfigEnableLimiter(void* pNtfBuf);
void RtcReqRsp_PostProcessPostLimiterEqShutdown(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotmuteReference(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetSpeedIn(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetHVACSetting(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetDynEQBoostOverRide(void* pReqBuf, void
    * pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetVNCTrebleBoostOverRide(void* pReqBuf, void
    * pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetLoudSignalOverRide(void* pReqBuf, void
    * pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetVNCSignalOverRide(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetLFNoiseOverRide(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetWBNoiseOverRide(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetHFNoiseOverRide(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetRatioOverRide(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetDyneqOn(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetAudioPilotOn(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetVNCBassBoostOverRide(void* pReqBuf, void
    * pRspBuf);
void RtcReqRsp_PreAmpAudiopilotgetBassBoost(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotsetVNCMidBoostOverRide(void* pReqBuf, void
    * pRspBuf);
void RtcReqRsp_PreAmpAudiopilotgetMidBoost(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotgetTrebleBoost(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpAudiopilotgetAverageAudioPilotBoostLevel(void* pReqBuf,
    void* pRspBuf);
void RtcReqRsp_PreAmpVolumeCfgSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_PreAmpVolumeCfgSet( void );
void RtcNtfPack_PreAmpVolumeCfgSet(void* pNtfBuf);
void RtcReqRsp_PreAmpBalance_CfgSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_PreAmpBalance_CfgSet( void );
void RtcNtfPack_PreAmpBalance_CfgSet(void* pNtfBuf);
void RtcReqRsp_PreAmpMedusaSurroundLevelControlIndex(void* pReqBuf, void
    * pRspBuf);
void RtcNtfCheck_PreAmpMedusaSurroundLevelControlIndex( void );
void RtcNtfPack_PreAmpMedusaSurroundLevelControlIndex(void* pNtfBuf);
void RtcReqRsp_PreAmpMedusaSurroundLevelControlReset(void* pReqBuf, void
    * pRspBuf);
void RtcReqRsp_PreAmpMedusaPostUpmixDisable(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpMedusaHoligramDisable(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpMedusaOverheadHoligramDisable(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpMedusaPostHoligramDisable(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpMuteControlSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_PreAmpMuteControlSet( void );
void RtcNtfPack_PreAmpMuteControlSet(void* pNtfBuf);
void RtcReqRsp_PreAmpFadeControlSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_PreAmpFadeControlSet( void );
void RtcNtfPack_PreAmpFadeControlSet(void* pNtfBuf);
void RtcReqRsp_PreAmpSleepingBeautyDRSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_PreAmpSleepingBeautyDRSet( void );
void RtcNtfPack_PreAmpSleepingBeautyDRSet(void* pNtfBuf);
void RtcReqRsp_PreAmpSleepingBeautyFRSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_PreAmpSleepingBeautyFRSet( void );
void RtcNtfPack_PreAmpSleepingBeautyFRSet(void* pNtfBuf);
void RtcReqRsp_PreAmpResetSleepingBeauty(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_PreAmpBassSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_PreAmpBassSet( void );
void RtcNtfPack_PreAmpBassSet(void* pNtfBuf);
void RtcReqRsp_PreAmpMidrangeCfgSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_PreAmpMidrangeCfgSet( void );
void RtcNtfPack_PreAmpMidrangeCfgSet(void* pNtfBuf);
void RtcReqRsp_PreAmpTrebleSet(void* pReqBuf, void* pRspBuf);
void RtcNtfCheck_PreAmpTrebleSet( void );
void RtcNtfPack_PreAmpTrebleSet(void* pNtfBuf);
void RtcReqRsp_TestRouterASDRouterMainSelect(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_TestRouterASDRouterMainGain(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_TestRouterASDRouterAuxEnable(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_TestRouterASDRouterAuxSelect(void* pReqBuf, void* pRspBuf);
void RtcReqRsp_TestRouterASDRouterAuxGain(void* pReqBuf, void* pRspBuf);
extern int32_T div_nde_s32_floor(int32_T numerator, int32_T denominator);
extern int32_T div_nzp_s32_floor(int32_T numerator, int32_T denominator);
extern void Model_Target_MATLABFunction(const int32_T rtu_tuneTopMap[18], const
    int32_T rtu_rtcMap[18], B_MATLABFunction_Model_Target_T *localB);
extern void Model_Target_MATLABFunction_e_Init
    (DW_MATLABFunction_Model_Target_n_T *localDW);
extern void Model_Target_MATLABFunction_n(real32_T rtu_tgain, real32_T
    rtu_ramp_db_per_samp, real_T rtu_minGain, real32_T *rty_gval,
    DW_MATLABFunction_Model_Target_n_T *localDW);
extern void Model_Target_Reset(real32_T rtd_states_on[4]);
extern void Model_Target_PoolIirProcess(const real32_T rtu_x[64], const uint32_T
    rtu_numStages[2], const real32_T rtu_coeffs[10], real32_T rtd_states_on[4],
    B_PoolIirProcess_Model_Target_T *localB);
extern void Model_Target_InnerLink_Init(DW_InnerLink_Model_Target_T *localDW);
extern void Model_Target_InnerLink(real32_T rtu_rmsInput, real32_T rtu_peakInput,
    real32_T rtu_slowDecayDbPerSample, real32_T rtu_holdMarginDb, real32_T
    rtu_holdTimeSamples, real32_T rtu_fastDecayDbPerSample, real32_T
    rtu_minVolumeDb, real32_T rtu_maxVolumeDb, real_T rtu_AnnTrigger, real32_T
    rtu_mode1, DW_InnerLink_Model_Target_T *localDW);
extern void Model_Target_MATLABFunction_i(uint32_T rtu_vncEnableIn,
    B_MATLABFunction_Model_Target_a_T *localB);
extern void Model_Target_ForEachSubsystem(int32_T NumIters, const real32_T
    rtu_In1[32], const real32_T rtu_In2[32], real32_T rty_Out1[32]);
extern void Model_Target_rtcscalartovector(uint32_T rtu_Inp,
    B_rtcscalartovector_Model_Target_T *localB);
extern void Model_Target_calculate_vol_gain(uint32_T rtu_idx, const uint32_T
    rtu_volTableIdx[16], const real32_T rtu_volTabledB[16],
    B_calculate_vol_gain_Model_Target_T *localB);
extern void Model_Target_rtcscalartovector2(uint32_T rtu_Inp,
    B_rtcscalartovector2_Model_Target_T *localB);
extern void Model_Target_rtcscalartovector3(uint32_T rtu_Inp,
    B_rtcscalartovector3_Model_Target_T *localB);
extern void Model_Target_get_ramp_parameters(uint32_T rtu_rampTimeRTC, uint32_T
    rtu_rampRateRTC, real32_T rtu_rampTimeTune,
    B_get_ramp_parameters_Model_Target_T *localB);
extern void Model_Target_SetDsm(uint32_T rtp_Value);
extern void Model_Target_rgainyrampercontrol(real32_T rtu_target_gains_linear,
    real_T rtu_db_per_sec, real_T rtu_ramp_times_ms, real32_T
    rtu_sample_rate_in_hertz, real32_T rtu_frame_size);
extern void Model_Target_rgainyprocess(const uint8_T rtu_rampToChanMap[2], const
    real32_T rtu_audioIn[32], B_rgainyprocess_Model_Target_T *localB);
extern void Model_Target_SetDsm_g(uint32_T rtp_Value);
extern void Model_Target_rgainyrampercontrol_d(real32_T rtu_target_gains_linear,
    real_T rtu_db_per_sec, real_T rtu_ramp_times_ms, real32_T
    rtu_sample_rate_in_hertz, real32_T rtu_frame_size);
extern void Model_Target_rgainyprocess_e(const uint8_T rtu_rampToChanMap[2],
    const real32_T rtu_audioIn[32], B_rgainyprocess_Model_Target_f_T *localB);
extern void Model_Target_SetDsm_k(uint32_T rtp_Value);
extern void Model_Target_rgainyrampercontrol_c(real32_T rtu_target_gains_linear,
    real_T rtu_db_per_sec, real_T rtu_ramp_times_ms, real32_T
    rtu_sample_rate_in_hertz, real32_T rtu_frame_size);
extern void Model_Target_rgainyprocess_a(const uint8_T rtu_rampToChanMap[2],
    const real32_T rtu_audioIn[32], B_rgainyprocess_Model_Target_j_T *localB);
extern void Model_Target_SetDsm_d(uint32_T rtp_Value);
extern void Model_Target_rgainyrampercontrol_p(real32_T rtu_target_gains_linear,
    real_T rtu_db_per_sec, real_T rtu_ramp_times_ms, real32_T
    rtu_sample_rate_in_hertz, real32_T rtu_frame_size);
extern void Model_Target_rgainyprocess_i(const uint8_T rtu_rampToChanMap[2],
    const real32_T rtu_audioIn[32], B_rgainyprocess_Model_Target_i_T *localB);
extern void Model_Target_ApplyBpfGain(const real32_T rtu_audio[64], const
    real32_T rtu_gain[32], B_ApplyBpfGain_Model_Target_T *localB);
extern void Model_Target_Reset_l(real32_T rtd_states_f[6]);
extern void Model_Target_PoolIirProcess_n(const real32_T rtu_x[32], uint32_T
    rtu_numStages, const real32_T rtu_coeffs[13], real32_T rtd_states_f[6],
    B_PoolIirProcess_Model_Target_c_T *localB);
extern void Model_Target_InnerLink_i_Init(DW_InnerLink_Model_Target_g_T *localDW);
extern void Model_Target_InnerLink_p(real32_T rtu_rmsInput, real32_T
    rtu_peakInput, real32_T rtu_predictionDbspl, real32_T
    rtu_slowDecayDbPerSample, real32_T rtu_holdMarginDb, real32_T
    rtu_holdTimeSamples, real32_T rtu_fastDecayDbPerSample, real32_T
    rtu_minVolumeDb, real32_T rtu_maxVolumeDb, real32_T rtu_volumeDb, real_T
    rtu_AnnTrigger, real32_T rtu_mode1, B_InnerLink_Model_Target_b_T *localB,
    DW_InnerLink_Model_Target_g_T *localDW);
extern void Model_Target_lookupdbfromindex(const uint32_T rtu_volTableIdx[8],
    const real32_T rtu_volTabledB[8], uint32_T rtu_idx, int32_T rtu_db_offset,
    real32_T rtu_maxgain, B_lookupdbfromindex_Model_Target_T *localB);
extern void Model_Target_rampparameters(uint32_T rtu_rampRateRTC, uint32_T
    rtu_rampTimeRTC, real32_T rtu_rampTimeTune, B_rampparameters_Model_Target_T *
    localB);
extern void Model_Target_SetDsm_gr(uint32_T rtp_Value);
extern void Model_Target_rgainyrampercontrol_h(real32_T rtu_target_gains_linear,
    real32_T rtu_db_per_sec, real32_T rtu_ramp_times_ms, real32_T
    rtu_sample_rate_in_hertz, real32_T rtu_frame_size);
extern void Model_Target_rgainyprocess_k(const uint8_T rtu_rampToChanMap[3],
    const real32_T rtu_audioIn[64], B_rgainyprocess_Model_Target_g_T *localB);
extern void Model_Target_SetDsm_c(uint32_T rtp_Value);
extern void Model_Target_control(const real32_T rtu_targetGainLinear[4],
    real32_T rtu_rampdbPerSec, real32_T rtu_rampTimesMs, real32_T
    rtu_sampleRateInHertz, real32_T rtu_frameSize);
extern void Model_Target_Coeffs1stStage(const creal32_T rtu_Lin[129], const
    creal32_T rtu_Rin[129], B_Coeffs1stStage_Model_Target_T *localB);
extern void Model_Target_MATLABFunction_nq(const real32_T rtu_x[129], real32_T
    rtu_pole, real32_T rtu_gain, B_MATLABFunction_Model_Target_b_T *localB,
    DW_MATLABFunction_Model_Target_k_T *localDW);
extern void Model_Target_UpdateSteeringCoeffs(const real32_T rtu_Lstrks[129],
    const real32_T rtu_Rstrks[129], const real32_T rtu_Loks[129], const real32_T
    rtu_Roks[129], const real32_T rtu_Lxks[129], const real32_T rtu_Rxks[129],
    B_UpdateSteeringCoeffs_Model_Target_T *localB);
extern void Model_Target_FadingForward(real32_T rtu_backFadeGain, real32_T
    rtu_forwardDownmixThreshold, real32_T rtu_forwardDownmixMax, real32_T
    *rty_downmix);
extern void Model_Target_FadingBackward(real32_T rtu_frontFadeGain, real32_T
    rtu_backwardDownmixThreshold, real32_T rtu_backwardDownmixMax, real32_T
    *rty_downmix);
extern void Model_Target_process(const boolean_T *rtd_FRM_Freeze_CS, real32_T
    rtd_FRM_currentGain_CS[12], int32_T rtd_FRM_frameCount_CS[12], const
    real32_T rtd_FRM_rampCoeff_CS[12], const real32_T rtd_FRM_targetGain_CS[12]);
extern void Model_Target_process_o(const boolean_T *rtd_FRM_Freeze_Left,
    real32_T rtd_FRM_currentGain_Left[30], int32_T rtd_FRM_frameCount_Left[30],
    const real32_T rtd_FRM_rampCoeff_Left[30], const real32_T
    rtd_FRM_targetGain_Left[30]);
extern void Model_Target_process_i(const boolean_T *rtd_FRM_Freeze_Right,
    real32_T rtd_FRM_currentGain_Right[30], int32_T rtd_FRM_frameCount_Right[30],
    const real32_T rtd_FRM_rampCoeff_Right[30], const real32_T
    rtd_FRM_targetGain_Right[30]);
extern void Model_Target_control_m(const real32_T rtu_target_gains_linear[30],
    uint32_T rtu_rampRate, uint32_T rtu_rampTime, boolean_T *rtd_FRM_Freeze_Left,
    real32_T rtd_FRM_currentGain_Left[30], int32_T rtd_FRM_frameCount_Left[30],
    real32_T rtd_FRM_rampCoeff_Left[30], real32_T rtd_FRM_targetGain_Left[30]);
extern void Model_Target_control_e(const real32_T rtu_target_gains_linear[12],
    uint32_T rtu_rampRate, uint32_T rtu_rampTime, boolean_T *rtd_FRM_Freeze_CS,
    real32_T rtd_FRM_currentGain_CS[12], int32_T rtd_FRM_frameCount_CS[12],
    real32_T rtd_FRM_rampCoeff_CS[12], real32_T rtd_FRM_targetGain_CS[12]);
extern void Model_Target_control_a(const real32_T rtu_target_gains_linear[30],
    uint32_T rtu_rampRate, uint32_T rtu_rampTime, boolean_T
    *rtd_FRM_Freeze_Right, real32_T rtd_FRM_currentGain_Right[30], int32_T
    rtd_FRM_frameCount_Right[30], real32_T rtd_FRM_rampCoeff_Right[30], real32_T
    rtd_FRM_targetGain_Right[30]);
extern void Model_Target_Reset_m(real32_T rtd_states_b[10]);
extern void Model_Target_PoolIirProcess_i(const real32_T rtu_x[8], uint32_T
    rtu_numStages, const real32_T rtu_coeffs[21], real32_T rtd_states_b[10],
    B_PoolIirProcess_Model_Target_f_T *localB);
extern void Model_Target_process_p(const boolean_T *rtd_Premix_Freeze_CS,
    real32_T rtd_Premix_currentGain_CS[20], int32_T rtd_Premix_frameCount_CS[20],
    const real32_T rtd_Premix_rampCoeff_CS[20], const real32_T
    rtd_Premix_targetGain_CS[20]);
extern void Model_Target_process_in(const boolean_T *rtd_Premix_Freeze_Left,
    real32_T rtd_Premix_currentGain_Left[77], int32_T
    rtd_Premix_frameCount_Left[77], const real32_T rtd_Premix_rampCoeff_Left[77],
    const real32_T rtd_Premix_targetGain_Left[77]);
extern void Model_Target_process_f(const boolean_T *rtd_Premix_Freeze_Right,
    real32_T rtd_Premix_currentGain_Right[77], int32_T
    rtd_Premix_frameCount_Right[77], const real32_T rtd_Premix_rampCoeff_Right
    [77], const real32_T rtd_Premix_targetGain_Right[77]);
extern void Model_Target_control_p(const real32_T rtu_target_gains_linear[77],
    uint32_T rtu_rampRate, uint32_T rtu_rampTime, boolean_T
    *rtd_Premix_Freeze_Left, real32_T rtd_Premix_currentGain_Left[77], int32_T
    rtd_Premix_frameCount_Left[77], real32_T rtd_Premix_rampCoeff_Left[77],
    real32_T rtd_Premix_targetGain_Left[77]);
extern void Model_Target_control_l(const real32_T rtu_target_gains_linear[20],
    uint32_T rtu_rampRate, uint32_T rtu_rampTime, boolean_T
    *rtd_Premix_Freeze_CS, real32_T rtd_Premix_currentGain_CS[20], int32_T
    rtd_Premix_frameCount_CS[20], real32_T rtd_Premix_rampCoeff_CS[20], real32_T
    rtd_Premix_targetGain_CS[20]);
extern void Model_Target_control_c(const real32_T rtu_target_gains_linear[77],
    uint32_T rtu_rampRate, uint32_T rtu_rampTime, boolean_T
    *rtd_Premix_Freeze_Right, real32_T rtd_Premix_currentGain_Right[77], int32_T
    rtd_Premix_frameCount_Right[77], real32_T rtd_Premix_rampCoeff_Right[77],
    real32_T rtd_Premix_targetGain_Right[77]);
extern void Model_Target_TargetGainComputation(const real32_T rtu_minGain_Left
    [77], const real32_T rtu_detentGain_Left[77], const real32_T
    rtu_maxGain_Left[77], real32_T rtu_interpolation_method, uint32_T
    rtu_surr_level, real32_T rtd_Premix_SLCGain_Left[77]);
extern void Model_Target_TargetGainComputation_b(const real32_T rtu_minGain_CS
    [20], const real32_T rtu_detentGain_CS[20], const real32_T rtu_maxGain_CS[20],
    real32_T rtu_interpolation_method, uint32_T rtu_surr_level, real32_T
    rtd_Premix_SLCGain_CS[20]);
extern void Model_Target_TargetGainComputation_e(const real32_T
    rtu_minGain_Right[77], const real32_T rtu_detentGain_Right[77], const
    real32_T rtu_maxGain_Right[77], real32_T rtu_interpolation_method, uint32_T
    rtu_surr_level, real32_T rtd_Premix_SLCGain_Right[77]);
extern void Model_Target_TargetGainComputation_ex(const real32_T
    rtu_minGain_Left[30], const real32_T rtu_detentGain_Left[30], const real32_T
    rtu_maxGain_Left[30], real32_T rtu_interpolation_method, uint32_T
    rtu_surr_level, real32_T rtd_FRM_SLCGain_Left[30]);
extern void Model_Target_TargetGainComputation_l(const real32_T rtu_minGain_CS
    [12], const real32_T rtu_detentGain_CS[12], const real32_T rtu_maxGain_CS[12],
    real32_T rtu_interpolation_method, uint32_T rtu_surr_level, real32_T
    rtd_FRM_SLCGain_CS[12]);
extern void Model_Target_TargetGainComputation_j(const real32_T
    rtu_minGain_Right[30], const real32_T rtu_detentGain_Right[30], const
    real32_T rtu_maxGain_Right[30], real32_T rtu_interpolation_method, uint32_T
    rtu_surr_level, real32_T rtd_FRM_SLCGain_Right[30]);
extern void Model_Target_TargetGainComputation_k(const real32_T rtu_minGain[12],
    const real32_T rtu_detentGain[12], const real32_T rtu_maxGain[12], real32_T
    rtu_interpolation_method, uint32_T rtu_surr_level, real32_T
    rtd_VLSDrv_SLCGain[12]);
extern void Model_Target_TargetGainComputation_g(const real32_T rtu_minGain[12],
    const real32_T rtu_detentGain[12], const real32_T rtu_maxGain[12], real32_T
    rtu_interpolation_method, uint32_T rtu_surr_level, real32_T
    rtd_VLSPax_SLCGain[12]);
extern void Model_Target_InitializationandRMDL(const real32_T rtu_minGain_Left
    [77], const real32_T rtu_detentGain_Left[77], const real32_T
    rtu_maxGain_Left[77], real32_T rtu_interpolation_method, uint32_T
    rtu_surr_level, real32_T rtd_Premix_SLCGain_Left[77], real32_T
    rtd_Premix_currentGain_Left[77], int32_T rtd_Premix_frameCount_Left[77],
    real32_T rtd_Premix_targetGain_Left[77]);
extern void Model_Target_InitializationandRMDL_f(const real32_T rtu_minGain_CS
    [20], const real32_T rtu_detentGain_CS[20], const real32_T rtu_maxGain_CS[20],
    real32_T rtu_interpolation_method, uint32_T rtu_surr_level, real32_T
    rtd_Premix_SLCGain_CS[20], real32_T rtd_Premix_currentGain_CS[20], int32_T
    rtd_Premix_frameCount_CS[20], real32_T rtd_Premix_targetGain_CS[20]);
extern void Model_Target_InitializationandRMDL_n(const real32_T
    rtu_minGain_Right[77], const real32_T rtu_detentGain_Right[77], const
    real32_T rtu_maxGain_Right[77], real32_T rtu_interpolation_method, uint32_T
    rtu_surr_level, real32_T rtd_Premix_SLCGain_Right[77], real32_T
    rtd_Premix_currentGain_Right[77], int32_T rtd_Premix_frameCount_Right[77],
    real32_T rtd_Premix_targetGain_Right[77]);
extern void Model_Target_InitializationandRMDL_o(const real32_T
    rtu_minGain_Left[30], const real32_T rtu_detentGain_Left[30], const real32_T
    rtu_maxGain_Left[30], real32_T rtu_interpolation_method, uint32_T
    rtu_surr_level, real32_T rtd_FRM_SLCGain_Left[30], real32_T
    rtd_FRM_currentGain_Left[30], int32_T rtd_FRM_frameCount_Left[30], real32_T
    rtd_FRM_targetGain_Left[30]);
extern void Model_Target_InitializationandRMDL_k(const real32_T rtu_minGain_CS
    [12], const real32_T rtu_detentGain_CS[12], const real32_T rtu_maxGain_CS[12],
    real32_T rtu_interpolation_method, uint32_T rtu_surr_level, real32_T
    rtd_FRM_SLCGain_CS[12], real32_T rtd_FRM_currentGain_CS[12], int32_T
    rtd_FRM_frameCount_CS[12], real32_T rtd_FRM_targetGain_CS[12]);
extern void Model_Target_InitializationandRMDL_ou(const real32_T
    rtu_minGain_Right[30], const real32_T rtu_detentGain_Right[30], const
    real32_T rtu_maxGain_Right[30], real32_T rtu_interpolation_method, uint32_T
    rtu_surr_level, real32_T rtd_FRM_SLCGain_Right[30], real32_T
    rtd_FRM_currentGain_Right[30], int32_T rtd_FRM_frameCount_Right[30],
    real32_T rtd_FRM_targetGain_Right[30]);
extern void Model_Target_InitializationandRMDL_o4(const real32_T rtu_minGain[12],
    const real32_T rtu_detentGain[12], const real32_T rtu_maxGain[12], real32_T
    rtu_interpolation_method, uint32_T rtu_surr_level, real32_T
    rtd_VLSDrv_SLCGain[12], real32_T rtd_VLSDrv_currentGain[12], int32_T
    rtd_VLSDrv_frameCount[12], real32_T rtd_VLSDrv_targetGain[12]);
extern void Model_Target_InitializationandRMDL_b(const real32_T rtu_minGain[12],
    const real32_T rtu_detentGain[12], const real32_T rtu_maxGain[12], real32_T
    rtu_interpolation_method, uint32_T rtu_surr_level, real32_T
    rtd_VLSPax_SLCGain[12], real32_T rtd_VLSPax_currentGain[12], int32_T
    rtd_VLSPax_frameCount[12], real32_T rtd_VLSPax_targetGain[12]);
extern void Model_Target_Fade_Trigger_Generation(real32_T rtu_gainDifference,
    const real32_T rtu_1[12], real32_T rtu_2, real32_T rtu_3, real32_T rtu_4,
    real32_T rtu_5, real32_T rtu_6, real32_T rtu_7,
    B_Fade_Trigger_Generation_Model_Target_T *localB);
extern void Model_Target_process_c(const boolean_T *rtd_VLSDrv_Freeze, real32_T
    rtd_VLSDrv_currentGain[12], int32_T rtd_VLSDrv_frameCount[12], const
    real32_T rtd_VLSDrv_rampCoeff[12], const real32_T rtd_VLSDrv_targetGain[12]);
extern void Model_Target_control_as(const real32_T rtu_target_gains_linear[12],
    uint32_T rtu_rampRate, uint32_T rtu_rampTime, boolean_T *rtd_VLSDrv_Freeze,
    real32_T rtd_VLSDrv_currentGain[12], int32_T rtd_VLSDrv_frameCount[12],
    real32_T rtd_VLSDrv_rampCoeff[12], real32_T rtd_VLSDrv_targetGain[12]);
extern void Model_Target_process_e(const boolean_T *rtd_VLSPax_Freeze, real32_T
    rtd_VLSPax_currentGain[12], int32_T rtd_VLSPax_frameCount[12], const
    real32_T rtd_VLSPax_rampCoeff[12], const real32_T rtd_VLSPax_targetGain[12]);
extern void Model_Target_control_j(const real32_T rtu_target_gains_linear[12],
    uint32_T rtu_rampRate, uint32_T rtu_rampTime, boolean_T *rtd_VLSPax_Freeze,
    real32_T rtd_VLSPax_currentGain[12], int32_T rtd_VLSPax_frameCount[12],
    real32_T rtd_VLSPax_rampCoeff[12], real32_T rtd_VLSPax_targetGain[12]);
extern void Model_Target_indextolinear(real32_T rtu_scale, real32_T rtu_offset,
    uint32_T rtu_index, B_indextolinear_Model_Target_T *localB);
extern void Model_Target_rampparameters_g(real32_T rtu_data, uint32_T
    rtu_rampTimeRTC, uint32_T rtu_rampRateRTC, const real_T rtp_rampRateBounds[2],
    const real_T rtp_rampTimeBounds[2], B_rampparameters_Model_Target_k_T
    *localB);
extern void Model_Target_SetDsm_m(uint32_T rtp_Value);
extern void Model_Target_tonecontrol(real32_T rtu_boost_max_db, real32_T
    rtu_cut_max_db, real32_T rtu_normalized_gain_impact, real32_T rtu_db_per_sec,
    real32_T rtu_ramp_time_ms, real32_T rtu_sample_rate_in_hertz, real32_T
    rtu_frame_size);
extern void Model_Target_tone(const real32_T rtu_boost_coeffs[3], const real32_T
    rtu_cut_coeffs[3], const real32_T rtu_audioIn[64], B_tone_Model_Target_T
    *localB, DW_tone_Model_Target_T *localDW);
extern void Model_Target_SetDsm_c1(uint32_T rtp_Value);
extern void Model_Target_tonecontrol_l(real32_T rtu_boost_max_db, real32_T
    rtu_cut_max_db, real32_T rtu_normalized_gain_impact, real32_T rtu_db_per_sec,
    real32_T rtu_ramp_time_ms, real32_T rtu_sample_rate_in_hertz, real32_T
    rtu_frame_size);
extern void Model_Target_tone_k(const real32_T rtu_boost_coeffs[5], const
    real32_T rtu_cut_coeffs[5], const real32_T rtu_audioIn[64],
    B_tone_Model_Target_b_T *localB, DW_tone_Model_Target_d_T *localDW);
extern void Model_Target_SetDsm_i(uint32_T rtp_Value);
extern void Model_Target_tonecontrol_f(real32_T rtu_boost_max_db, real32_T
    rtu_cut_max_db, real32_T rtu_normalized_gain_impact, real32_T rtu_db_per_sec,
    real32_T rtu_ramp_time_ms, real32_T rtu_sample_rate_in_hertz, real32_T
    rtu_frame_size);
extern void Model_Target_tone_j(const real32_T rtu_boost_coeffs[3], const
    real32_T rtu_cut_coeffs[3], const real32_T rtu_audioIn[64],
    B_tone_Model_Target_g_T *localB, DW_tone_Model_Target_h_T *localDW);
extern void Model_Target_AlignmentDelay(void);
extern void HandleReqPreAmpMedusaSurroundLevelControlIndex(void);
extern void HandleReqPreAmpMedusaSurroundLevelControlReset(void);
extern void HandleReqPreAmpMedusaPostUpmixDisable(void);
extern void Model_Target_Medusa5H1Part3_Init(void);
extern void Model_Target_Medusa5H1Part3_Update(void);
extern void Model_Target_Medusa5H1Part3(void);
extern void InitTrigger_Model_141_472_249_99(void);
extern void InitTrigger_Model_141_472_249_224(void);
extern void InitTrigger_Model_141_472_254_172_156(void);
extern void InitTrigger_Model_141_473_11_172_156(void);
extern void InitTrigger_Model_141_595_232_99(void);
extern void InitTrigger_Model_141_595_232_224(void);
extern void InitTrigger_Model_141_595_237_172_156(void);
extern void HandleReqPreAmpMedusaHoligramDisable(void);
extern void Model_Target_Medusa5H1Part4_Init(void);
extern void Model_Target_Medusa5H1Part4(void);
extern void HandleReqPreAmpMedusaOverheadHoligramDisable(void);
extern void Model_Target_Medusa5H1Part5_Init(void);
extern void Model_Target_Medusa5H1Part5(void);
extern void Model_Target_Medusa5H1Part6_Init(void);
extern void Model_Target_Medusa5H1Part6(void);
extern void HandleReqPreAmpBassSet(void);
extern void HandleReqPreAmpMidrangeCfgSet(void);
extern void HandleReqPreAmpTrebleSet(void);
extern void Model_Target_PreAmpPart1_Init(void);
extern void Model_Target_PreAmpPart1_Update(void);
extern void Model_Target_PreAmpPart1(void);
extern void Model_Target_InputSelect(void);
extern void InitTrigger_Model_340_172_156(void);
extern void InitTrigger_Model_457_172_156(void);
extern void InitTrigger_Model_607_172_156(void);
extern void InitTrigger_Model_644_172_156(void);
extern void InitTrigger_Model_787_143_65(void);
extern void HandleReqNearAnn1Ann1NonEntGainCalcvncEnableRTC(void);
extern void HandleReqNearAnn1Ann1MuteSet(void);
extern void HandleReqNearAnn1Ann1VolumeSet(void);
extern void HandleReqNearAnn2Ann2NonEntGainCalcvncEnableRTC(void);
extern void HandleReqNearAnn2Ann2MuteSet(void);
extern void HandleReqNearAnn2Ann2VolumeSet(void);
extern void HandleReqNearAnn3Ann3NonEntGainCalcvncEnableRTC(void);
extern void HandleReqNearAnn3Ann3MuteSet(void);
extern void HandleReqNearAnn3Ann3VolumeSet(void);
extern void HandleReqNearAnn4Ann4NonEntGainCalcvncEnableRTC(void);
extern void HandleReqNearAnn4Ann4MuteSet(void);
extern void HandleReqNearAnn4Ann4VolumeSet(void);
extern void Model_Target_SrcDownAnn_Init(void);
extern void Model_Target_SrcDownAnnTID1(void);
extern void Model_Target_SrcDownAnnTID2(void);
extern void HandleReqPreAmpAudiopilotsetHVACSetting(void);
extern void HandleReqPreAmpAudiopilotgetAverageAudioPilotBoostLevel(void);
extern void HandleReqPreAmpVolumeCfgSet(void);
extern void HandleReqPreAmpBalance_CfgSet(void);
extern void InitTrigger_Model_141_246_85_172_156(void);
extern void InitTrigger_Model_141_246_91_172_156(void);
extern void InitTrigger_Model_141_248_38_65(void);
extern void Model_Target_Medusa5H1Part1_Init(void);
extern void Model_Target_Medusa5H1Part1(void);
extern void InitTrigger_Model_141_2207_65(void);
extern void InitTrigger_Model_141_2212_100(void);
extern void InitTrigger_Model_141_2873_65(void);
extern void InitTrigger_Model_141_2878_100(void);
extern void InitTrigger_Model_141_2931_156(void);
extern void InitTrigger_Model_141_3070_49_172_156(void);
extern void Model_Target_Medusa5H1Part2_Init(void);
extern void Model_Target_Medusa5H1Part2TID2(void);
extern void Model_Target_Medusa5H1Part2TID4(void);
extern void Model_Target_Medusa5H1Part2TID5(void);
extern void InitTrigger_Model_1112_65(void);
extern void Model_Target_SrcDownEnt_Init(void);
extern void Model_Target_SrcDownEntTID1(void);
extern void Model_Target_SrcDownEntTID2(void);
extern void Model_Target_SrcDownMic_Init(void);
extern void Model_Target_SrcDownMicTID1(void);
extern void Model_Target_SrcDownMicTID2(void);
extern void Model_Target_SrcUp_Init(void);
extern void Model_Target_SrcUpTID1(void);
extern void Model_Target_SrcUpTID2(void);
extern void Model_Target_SrcDownAnn_Term(void);
extern void Model_Target_SrcDownEnt_Term(void);
extern void Model_Target_SrcDownMic_Term(void);
extern void Model_Target_SrcUp_Term(void);

/* Exported data declaration */

/* Declaration for custom storage class: StructStaticInit */
extern Model_TargetControlDsms_type Model_TargetControlDsms;
extern Model_TargetInputSelectRouterCfgSetRtcOut_type
    Model_TargetInputSelectRouterCfgSetRtcOut;
extern Model_TargetNearAnn1Ann1MuteSetRtcOut_type
    Model_TargetNearAnn1Ann1MuteSetRtcOut;
extern Model_TargetNearAnn1Ann1NonEntGainCalcvncEnableRTCRtcOut_type
    Model_TargetNearAnn1Ann1NonEntGainCalcvncEnableRTCRtcOut;
extern Model_TargetNearAnn1Ann1VolumeSetRtcOut_type
    Model_TargetNearAnn1Ann1VolumeSetRtcOut;
extern Model_TargetNearAnn2Ann2MuteSetRtcOut_type
    Model_TargetNearAnn2Ann2MuteSetRtcOut;
extern Model_TargetNearAnn2Ann2NonEntGainCalcvncEnableRTCRtcOut_type
    Model_TargetNearAnn2Ann2NonEntGainCalcvncEnableRTCRtcOut;
extern Model_TargetNearAnn2Ann2VolumeSetRtcOut_type
    Model_TargetNearAnn2Ann2VolumeSetRtcOut;
extern Model_TargetNearAnn3Ann3MuteSetRtcOut_type
    Model_TargetNearAnn3Ann3MuteSetRtcOut;
extern Model_TargetNearAnn3Ann3NonEntGainCalcvncEnableRTCRtcOut_type
    Model_TargetNearAnn3Ann3NonEntGainCalcvncEnableRTCRtcOut;
extern Model_TargetNearAnn3Ann3VolumeSetRtcOut_type
    Model_TargetNearAnn3Ann3VolumeSetRtcOut;
extern Model_TargetNearAnn4Ann4MuteSetRtcOut_type
    Model_TargetNearAnn4Ann4MuteSetRtcOut;
extern Model_TargetNearAnn4Ann4NonEntGainCalcvncEnableRTCRtcOut_type
    Model_TargetNearAnn4Ann4NonEntGainCalcvncEnableRTCRtcOut;
extern Model_TargetNearAnn4Ann4VolumeSetRtcOut_type
    Model_TargetNearAnn4Ann4VolumeSetRtcOut;
extern Model_TargetNearAnnNoiseLevelRtcOut_type
    Model_TargetNearAnnNoiseLevelRtcOut;
extern Model_TargetNearChannel1_LocationRtcOut_type
    Model_TargetNearChannel1_LocationRtcOut;
extern Model_TargetNearChannel2_LocationRtcOut_type
    Model_TargetNearChannel2_LocationRtcOut;
extern Model_TargetNearChannel3_LocationRtcOut_type
    Model_TargetNearChannel3_LocationRtcOut;
extern Model_TargetNearChannel4_LocationRtcOut_type
    Model_TargetNearChannel4_LocationRtcOut;
extern Model_TargetNearDr_RouterSetRtcOut_type
    Model_TargetNearDr_RouterSetRtcOut;
extern Model_TargetNearEnableRtcOut_type Model_TargetNearEnableRtcOut;
extern Model_TargetNearFr_RouterSetRtcOut_type
    Model_TargetNearFr_RouterSetRtcOut;
extern Model_TargetPostProcessEnergyLimiterConfigEnableLimiterRtcOut_type
    Model_TargetPostProcessEnergyLimiterConfigEnableLimiterRtcOut;
extern Model_TargetPostProcessLimiterSoftclipDisableRtcOut_type
    Model_TargetPostProcessLimiterSoftclipDisableRtcOut;
extern Model_TargetPostProcessLimiterSoftclipParamSetRtcOut_type
    Model_TargetPostProcessLimiterSoftclipParamSetRtcOut;
extern Model_TargetPostProcessMuteCfgSetRtcOut_type
    Model_TargetPostProcessMuteCfgSetRtcOut;
extern Model_TargetPostProcessPostLimiterEqShutdownRtcOut_type
    Model_TargetPostProcessPostLimiterEqShutdownRtcOut;
extern Model_TargetPreAmpAudiopilotgetAverageAudioPilotBoostLevelRtcOut_type
    Model_TargetPreAmpAudiopilotgetAverageAudioPilotBoostLevelRtcOut;
extern Model_TargetPreAmpAudiopilotgetBassBoostRtcOut_type
    Model_TargetPreAmpAudiopilotgetBassBoostRtcOut;
extern Model_TargetPreAmpAudiopilotgetMidBoostRtcOut_type
    Model_TargetPreAmpAudiopilotgetMidBoostRtcOut;
extern Model_TargetPreAmpAudiopilotgetTrebleBoostRtcOut_type
    Model_TargetPreAmpAudiopilotgetTrebleBoostRtcOut;
extern Model_TargetPreAmpAudiopilotmuteReferenceRtcOut_type
    Model_TargetPreAmpAudiopilotmuteReferenceRtcOut;
extern Model_TargetPreAmpAudiopilotsetAudioPilotOnRtcOut_type
    Model_TargetPreAmpAudiopilotsetAudioPilotOnRtcOut;
extern Model_TargetPreAmpAudiopilotsetDynEQBoostOverRideRtcOut_type
    Model_TargetPreAmpAudiopilotsetDynEQBoostOverRideRtcOut;
extern Model_TargetPreAmpAudiopilotsetDyneqOnRtcOut_type
    Model_TargetPreAmpAudiopilotsetDyneqOnRtcOut;
extern Model_TargetPreAmpAudiopilotsetHFNoiseOverRideRtcOut_type
    Model_TargetPreAmpAudiopilotsetHFNoiseOverRideRtcOut;
extern Model_TargetPreAmpAudiopilotsetHVACSettingRtcOut_type
    Model_TargetPreAmpAudiopilotsetHVACSettingRtcOut;
extern Model_TargetPreAmpAudiopilotsetLFNoiseOverRideRtcOut_type
    Model_TargetPreAmpAudiopilotsetLFNoiseOverRideRtcOut;
extern Model_TargetPreAmpAudiopilotsetLoudSignalOverRideRtcOut_type
    Model_TargetPreAmpAudiopilotsetLoudSignalOverRideRtcOut;
extern Model_TargetPreAmpAudiopilotsetRatioOverRideRtcOut_type
    Model_TargetPreAmpAudiopilotsetRatioOverRideRtcOut;
extern Model_TargetPreAmpAudiopilotsetSpeedInRtcOut_type
    Model_TargetPreAmpAudiopilotsetSpeedInRtcOut;
extern Model_TargetPreAmpAudiopilotsetVNCBassBoostOverRideRtcOut_type
    Model_TargetPreAmpAudiopilotsetVNCBassBoostOverRideRtcOut;
extern Model_TargetPreAmpAudiopilotsetVNCMidBoostOverRideRtcOut_type
    Model_TargetPreAmpAudiopilotsetVNCMidBoostOverRideRtcOut;
extern Model_TargetPreAmpAudiopilotsetVNCSignalOverRideRtcOut_type
    Model_TargetPreAmpAudiopilotsetVNCSignalOverRideRtcOut;
extern Model_TargetPreAmpAudiopilotsetVNCTrebleBoostOverRideRtcOut_type
    Model_TargetPreAmpAudiopilotsetVNCTrebleBoostOverRideRtcOut;
extern Model_TargetPreAmpAudiopilotsetWBNoiseOverRideRtcOut_type
    Model_TargetPreAmpAudiopilotsetWBNoiseOverRideRtcOut;
extern Model_TargetPreAmpBalance_CfgSetRtcOut_type
    Model_TargetPreAmpBalance_CfgSetRtcOut;
extern Model_TargetPreAmpBassSetRtcOut_type Model_TargetPreAmpBassSetRtcOut;
extern Model_TargetPreAmpFadeControlSetRtcOut_type
    Model_TargetPreAmpFadeControlSetRtcOut;
extern Model_TargetPreAmpMedusaHoligramDisableRtcOut_type
    Model_TargetPreAmpMedusaHoligramDisableRtcOut;
extern Model_TargetPreAmpMedusaOverheadHoligramDisableRtcOut_type
    Model_TargetPreAmpMedusaOverheadHoligramDisableRtcOut;
extern Model_TargetPreAmpMedusaPostHoligramDisableRtcOut_type
    Model_TargetPreAmpMedusaPostHoligramDisableRtcOut;
extern Model_TargetPreAmpMedusaPostUpmixDisableRtcOut_type
    Model_TargetPreAmpMedusaPostUpmixDisableRtcOut;
extern Model_TargetPreAmpMedusaSurroundLevelControlIndexRtcOut_type
    Model_TargetPreAmpMedusaSurroundLevelControlIndexRtcOut;
extern Model_TargetPreAmpMedusaSurroundLevelControlResetRtcOut_type
    Model_TargetPreAmpMedusaSurroundLevelControlResetRtcOut;
extern Model_TargetPreAmpMidrangeCfgSetRtcOut_type
    Model_TargetPreAmpMidrangeCfgSetRtcOut;
extern Model_TargetPreAmpMuteControlSetRtcOut_type
    Model_TargetPreAmpMuteControlSetRtcOut;
extern Model_TargetPreAmpResetSleepingBeautyRtcOut_type
    Model_TargetPreAmpResetSleepingBeautyRtcOut;
extern Model_TargetPreAmpSleepingBeautyDRSetRtcOut_type
    Model_TargetPreAmpSleepingBeautyDRSetRtcOut;
extern Model_TargetPreAmpSleepingBeautyFRSetRtcOut_type
    Model_TargetPreAmpSleepingBeautyFRSetRtcOut;
extern Model_TargetPreAmpTrebleSetRtcOut_type Model_TargetPreAmpTrebleSetRtcOut;
extern Model_TargetPreAmpVolumeCfgSetRtcOut_type
    Model_TargetPreAmpVolumeCfgSetRtcOut;
extern Model_TargetTestRouterASDRouterAuxEnableRtcOut_type
    Model_TargetTestRouterASDRouterAuxEnableRtcOut;
extern Model_TargetTestRouterASDRouterAuxGainRtcOut_type
    Model_TargetTestRouterASDRouterAuxGainRtcOut;
extern Model_TargetTestRouterASDRouterAuxSelectRtcOut_type
    Model_TargetTestRouterASDRouterAuxSelectRtcOut;
extern Model_TargetTestRouterASDRouterMainGainRtcOut_type
    Model_TargetTestRouterASDRouterMainGainRtcOut;
extern Model_TargetTestRouterASDRouterMainSelectRtcOut_type
    Model_TargetTestRouterASDRouterMainSelectRtcOut;

#endif                                 /* RTW_HEADER_Model_Target_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
