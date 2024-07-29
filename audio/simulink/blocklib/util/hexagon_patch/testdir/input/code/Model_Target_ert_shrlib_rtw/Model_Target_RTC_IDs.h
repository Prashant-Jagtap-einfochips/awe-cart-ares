/*
 * File: Model_Target_RTC_IDs.h
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

#ifndef RTW_HEADER_Model_Target_RTC_IDs_h_
#define RTW_HEADER_Model_Target_RTC_IDs_h_
#include <stdint.h>
#include "ASDID.h"

// ------------------------------------------------------------
// CORE ENDIAN SETTINGS
// ------------------------------------------------------------
#define ENDIANNESS_CORE                LittleEndian

// ------------------------------------------------------------
// RTC MESSAGE IDS
// ------------------------------------------------------------
#define RTCID_InputSelectRouterCfgSet  (0x10010000U)             // REQ, RSP '<S15>/RTC Request Response Trigger'
#define RTCID_NearFr_RouterSet         (0x10010014U)             // REQ, RSP '<S109>/RTC Request Response Trigger'
#define RTCID_NearAnn1Ann1VolumeSet    (0x10000003U)             // REQ, RSP, NTF '<S291>/RTC Request Response Trigger'
#define RTCID_NearAnn2Ann2VolumeSet    (0x10000006U)             // REQ, RSP, NTF '<S371>/RTC Request Response Trigger'
#define RTCID_NearAnn3Ann3VolumeSet    (0x10000009U)             // REQ, RSP, NTF '<S451>/RTC Request Response Trigger'
#define RTCID_NearAnn4Ann4VolumeSet    (0x1000000cU)             // REQ, RSP, NTF '<S531>/RTC Request Response Trigger'
#define RTCID_NearAnn1Ann1NonEntGainCalcvncEnableRTC (0x10010002U) // REQ, RSP '<S240>/RTC Request Response Trigger'
#define RTCID_NearAnn1Ann1MuteSet      (0x10000001U)             // REQ, RSP, NTF '<S269>/RTC Request Response Trigger'
#define RTCID_NearAnn2Ann2NonEntGainCalcvncEnableRTC (0x10010005U) // REQ, RSP '<S320>/RTC Request Response Trigger'
#define RTCID_NearAnn2Ann2MuteSet      (0x10000004U)             // REQ, RSP, NTF '<S349>/RTC Request Response Trigger'
#define RTCID_NearAnn3Ann3NonEntGainCalcvncEnableRTC (0x10010008U) // REQ, RSP '<S400>/RTC Request Response Trigger'
#define RTCID_NearAnn3Ann3MuteSet      (0x10000007U)             // REQ, RSP, NTF '<S429>/RTC Request Response Trigger'
#define RTCID_NearAnn4Ann4NonEntGainCalcvncEnableRTC (0x1001000bU) // REQ, RSP '<S480>/RTC Request Response Trigger'
#define RTCID_NearAnn4Ann4MuteSet      (0x1000000aU)             // REQ, RSP, NTF '<S509>/RTC Request Response Trigger'
#define RTCID_NearAnnNoiseLevel        (0x1002000dU)             // REQ *only* '<S176>/RTC Request Response Trigger'
#define RTCID_NearChannel1_Location    (0x1001000eU)             // REQ, RSP '<S131>/RTC Request Response Trigger'
#define RTCID_NearChannel2_Location    (0x1001000fU)             // REQ, RSP '<S130>/RTC Request Response Trigger'
#define RTCID_NearChannel3_Location    (0x10010010U)             // REQ, RSP '<S133>/RTC Request Response Trigger'
#define RTCID_NearChannel4_Location    (0x10010011U)             // REQ, RSP '<S132>/RTC Request Response Trigger'
#define RTCID_NearDr_RouterSet         (0x10010012U)             // REQ, RSP '<S65>/RTC Request Response Trigger'
#define RTCID_NearEnable               (0x10010013U)             // REQ, RSP '<S27>/RTC Request Response Trigger'
#define RTCID_PostProcessMuteCfgSet    (0x10000018U)             // REQ, RSP, NTF '<S604>/RTC Request Response Trigger'
#define RTCID_PostProcessLimiterSoftclipParamSet (0x10010017U)   // REQ, RSP '<S568>/RTC Request Response Trigger'
#define RTCID_PostProcessLimiterSoftclipDisable (0x10010016U)    // REQ, RSP '<S569>/RTC Request Response Trigger'
#define RTCID_PostProcessEnergyLimiterConfigEnableLimiter (0x10000015U) // REQ, RSP, NTF '<S574>/RTC Request Response Trigger'
#define RTCID_PostProcessPostLimiterEqShutdown (0x10010019U)     // REQ, RSP '<S627>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotmuteReference (0x1001001eU)        // REQ, RSP '<S674>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetSpeedIn (0x10020027U)           // REQ *only* '<S675>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetHVACSetting (0x10020023U)       // REQ *only* '<S678>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetDynEQBoostOverRide (0x10020020U) // REQ *only* '<S772>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetVNCTrebleBoostOverRide (0x1002002bU) // REQ *only* '<S775>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetLoudSignalOverRide (0x10020025U) // REQ *only* '<S801>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetVNCSignalOverRide (0x1002002aU) // REQ *only* '<S803>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetLFNoiseOverRide (0x10020024U)   // REQ *only* '<S823>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetWBNoiseOverRide (0x1002002cU)   // REQ *only* '<S826>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetHFNoiseOverRide (0x10020022U)   // REQ *only* '<S822>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetRatioOverRide (0x10020026U)     // REQ *only* '<S825>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetDyneqOn (0x10010021U)           // REQ, RSP '<S862>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetAudioPilotOn (0x1001001fU)      // REQ, RSP '<S863>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetVNCBassBoostOverRide (0x10020028U) // REQ *only* '<S773>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotgetBassBoost (0x1001001bU)         // REQ, RSP '<S699>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetVNCMidBoostOverRide (0x10020029U) // REQ *only* '<S774>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotgetMidBoost (0x1001001cU)          // REQ, RSP '<S700>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotgetTrebleBoost (0x1001001dU)       // REQ, RSP '<S701>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotgetAverageAudioPilotBoostLevel (0x1001001aU) // REQ, RSP '<S737>/RTC Request Response Trigger'
#define RTCID_PreAmpVolumeCfgSet       (0x1000003cU)             // REQ, RSP, NTF '<S1114>/RTC Request Response Trigger'
#define RTCID_PreAmpBalance_CfgSet     (0x1000002dU)             // REQ, RSP, NTF '<S1143>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaSurroundLevelControlIndex (0x10000034U) // REQ, RSP, NTF '<S1626>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaSurroundLevelControlReset (0x10010035U) // REQ, RSP '<S1627>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaPostUpmixDisable (0x10010033U)         // REQ, RSP '<S1432>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaHoligramDisable (0x10010030U)          // REQ, RSP '<S1703>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaOverheadHoligramDisable (0x10010031U)  // REQ, RSP '<S1772>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaPostHoligramDisable (0x10010032U)      // REQ, RSP '<S1792>/RTC Request Response Trigger'
#define RTCID_PreAmpMuteControlSet     (0x10000037U)             // REQ, RSP, NTF '<S1858>/RTC Request Response Trigger'
#define RTCID_PreAmpFadeControlSet     (0x1000002fU)             // REQ, RSP, NTF '<S1828>/RTC Request Response Trigger'
#define RTCID_PreAmpSleepingBeautyDRSet (0x10000039U)            // REQ, RSP, NTF '<S1901>/RTC Request Response Trigger'
#define RTCID_PreAmpSleepingBeautyFRSet (0x1000003aU)            // REQ, RSP, NTF '<S1921>/RTC Request Response Trigger'
#define RTCID_PreAmpResetSleepingBeauty (0x10010038U)            // REQ, RSP '<S1822>/RTC Request Response Trigger'
#define RTCID_PreAmpBassSet            (0x1000002eU)             // REQ, RSP, NTF '<S1978>/RTC Request Response Trigger'
#define RTCID_PreAmpMidrangeCfgSet     (0x10000036U)             // REQ, RSP, NTF '<S1996>/RTC Request Response Trigger'
#define RTCID_PreAmpTrebleSet          (0x1000003bU)             // REQ, RSP, NTF '<S2032>/RTC Request Response Trigger'
#define RTCID_TestRouterASDRouterMainSelect (0x10010041U)        // REQ, RSP '<S2062>/RTC Request Response Trigger'
#define RTCID_TestRouterASDRouterMainGain (0x10010040U)          // REQ, RSP '<S2061>/RTC Request Response Trigger'
#define RTCID_TestRouterASDRouterAuxEnable (0x1001003dU)         // REQ, RSP '<S2057>/RTC Request Response Trigger'
#define RTCID_TestRouterASDRouterAuxSelect (0x1001003fU)         // REQ, RSP '<S2059>/RTC Request Response Trigger'
#define RTCID_TestRouterASDRouterAuxGain (0x1001003eU)           // REQ, RSP '<S2058>/RTC Request Response Trigger'

// ------------------------------------------------------------
// PAYLOAD UNION MACROS
// ------------------------------------------------------------
#define ALL_REQ_PAYLOADS               InputSelectRouterCfgSet_REQ_T InputSelectRouterCfgSet; NearDr_RouterSet_REQ_T NearDr_RouterSet; NearFr_RouterSet_REQ_T NearFr_RouterSet; NearChannel2_Location_REQ_T NearChannel2_Location; NearChannel1_Location_REQ_T NearChannel1_Location; NearChannel4_Location_REQ_T NearChannel4_Location; NearChannel3_Location_REQ_T NearChannel3_Location; NearAnnNoiseLevel_REQ_T NearAnnNoiseLevel; NearAnn1Ann1NonEntGainCalcvncEnableRTC_REQ_T NearAnn1Ann1NonEntGainCalcvncEnableRTC; NearAnn1Ann1MuteSet_REQ_T NearAnn1Ann1MuteSet; NearAnn1Ann1VolumeSet_REQ_T NearAnn1Ann1VolumeSet; NearAnn2Ann2NonEntGainCalcvncEnableRTC_REQ_T NearAnn2Ann2NonEntGainCalcvncEnableRTC; NearAnn2Ann2MuteSet_REQ_T NearAnn2Ann2MuteSet; NearAnn2Ann2VolumeSet_REQ_T NearAnn2Ann2VolumeSet; NearAnn3Ann3NonEntGainCalcvncEnableRTC_REQ_T NearAnn3Ann3NonEntGainCalcvncEnableRTC; NearAnn3Ann3MuteSet_REQ_T NearAnn3Ann3MuteSet; NearAnn3Ann3VolumeSet_REQ_T NearAnn3Ann3VolumeSet; NearAnn4Ann4NonEntGainCalcvncEnableRTC_REQ_T NearAnn4Ann4NonEntGainCalcvncEnableRTC; NearAnn4Ann4MuteSet_REQ_T NearAnn4Ann4MuteSet; NearAnn4Ann4VolumeSet_REQ_T NearAnn4Ann4VolumeSet; NearEnable_REQ_T NearEnable; PostProcessEnergyLimiterConfigEnableLimiter_REQ_T PostProcessEnergyLimiterConfigEnableLimiter; PostProcessMuteCfgSet_REQ_T PostProcessMuteCfgSet; PostProcessPostLimiterEqShutdown_REQ_T PostProcessPostLimiterEqShutdown; PostProcessLimiterSoftclipParamSet_REQ_T PostProcessLimiterSoftclipParamSet; PostProcessLimiterSoftclipDisable_REQ_T PostProcessLimiterSoftclipDisable; PreAmpAudiopilotsetHVACSetting_REQ_T PreAmpAudiopilotsetHVACSetting; PreAmpAudiopilotmuteReference_REQ_T PreAmpAudiopilotmuteReference; PreAmpAudiopilotsetSpeedIn_REQ_T PreAmpAudiopilotsetSpeedIn; PreAmpAudiopilotgetBassBoost_REQ_T PreAmpAudiopilotgetBassBoost; PreAmpAudiopilotgetMidBoost_REQ_T PreAmpAudiopilotgetMidBoost; PreAmpAudiopilotgetTrebleBoost_REQ_T PreAmpAudiopilotgetTrebleBoost; PreAmpAudiopilotsetDynEQBoostOverRide_REQ_T PreAmpAudiopilotsetDynEQBoostOverRide; PreAmpAudiopilotsetVNCBassBoostOverRide_REQ_T PreAmpAudiopilotsetVNCBassBoostOverRide; PreAmpAudiopilotsetVNCMidBoostOverRide_REQ_T PreAmpAudiopilotsetVNCMidBoostOverRide; PreAmpAudiopilotsetVNCTrebleBoostOverRide_REQ_T PreAmpAudiopilotsetVNCTrebleBoostOverRide; PreAmpAudiopilotsetLoudSignalOverRide_REQ_T PreAmpAudiopilotsetLoudSignalOverRide; PreAmpAudiopilotsetVNCSignalOverRide_REQ_T PreAmpAudiopilotsetVNCSignalOverRide; PreAmpAudiopilotgetAverageAudioPilotBoostLevel_REQ_T PreAmpAudiopilotgetAverageAudioPilotBoostLevel; PreAmpAudiopilotsetHFNoiseOverRide_REQ_T PreAmpAudiopilotsetHFNoiseOverRide; PreAmpAudiopilotsetLFNoiseOverRide_REQ_T PreAmpAudiopilotsetLFNoiseOverRide; PreAmpAudiopilotsetRatioOverRide_REQ_T PreAmpAudiopilotsetRatioOverRide; PreAmpAudiopilotsetWBNoiseOverRide_REQ_T PreAmpAudiopilotsetWBNoiseOverRide; PreAmpAudiopilotsetDyneqOn_REQ_T PreAmpAudiopilotsetDyneqOn; PreAmpAudiopilotsetAudioPilotOn_REQ_T PreAmpAudiopilotsetAudioPilotOn; PreAmpVolumeCfgSet_REQ_T PreAmpVolumeCfgSet; PreAmpBalance_CfgSet_REQ_T PreAmpBalance_CfgSet; PreAmpMedusaSurroundLevelControlIndex_REQ_T PreAmpMedusaSurroundLevelControlIndex; PreAmpMedusaSurroundLevelControlReset_REQ_T PreAmpMedusaSurroundLevelControlReset; PreAmpMedusaPostUpmixDisable_REQ_T PreAmpMedusaPostUpmixDisable; PreAmpMedusaHoligramDisable_REQ_T PreAmpMedusaHoligramDisable; PreAmpMedusaOverheadHoligramDisable_REQ_T PreAmpMedusaOverheadHoligramDisable; PreAmpMedusaPostHoligramDisable_REQ_T PreAmpMedusaPostHoligramDisable; PreAmpFadeControlSet_REQ_T PreAmpFadeControlSet; PreAmpMuteControlSet_REQ_T PreAmpMuteControlSet; PreAmpResetSleepingBeauty_REQ_T PreAmpResetSleepingBeauty; PreAmpSleepingBeautyDRSet_REQ_T PreAmpSleepingBeautyDRSet; PreAmpSleepingBeautyFRSet_REQ_T PreAmpSleepingBeautyFRSet; PreAmpBassSet_REQ_T PreAmpBassSet; PreAmpMidrangeCfgSet_REQ_T PreAmpMidrangeCfgSet; PreAmpTrebleSet_REQ_T PreAmpTrebleSet; TestRouterASDRouterAuxEnable_REQ_T TestRouterASDRouterAuxEnable; TestRouterASDRouterAuxGain_REQ_T TestRouterASDRouterAuxGain; TestRouterASDRouterAuxSelect_REQ_T TestRouterASDRouterAuxSelect; TestRouterASDRouterMainGain_REQ_T TestRouterASDRouterMainGain; TestRouterASDRouterMainSelect_REQ_T TestRouterASDRouterMainSelect;
#define ALL_RSP_PAYLOADS               InputSelectRouterCfgSet_RSP_T InputSelectRouterCfgSet; NearDr_RouterSet_RSP_T NearDr_RouterSet; NearFr_RouterSet_RSP_T NearFr_RouterSet; NearChannel2_Location_RSP_T NearChannel2_Location; NearChannel1_Location_RSP_T NearChannel1_Location; NearChannel4_Location_RSP_T NearChannel4_Location; NearChannel3_Location_RSP_T NearChannel3_Location; NearAnn1Ann1NonEntGainCalcvncEnableRTC_RSP_T NearAnn1Ann1NonEntGainCalcvncEnableRTC; NearAnn1Ann1MuteSet_RSP_T NearAnn1Ann1MuteSet; NearAnn1Ann1VolumeSet_RSP_T NearAnn1Ann1VolumeSet; NearAnn2Ann2NonEntGainCalcvncEnableRTC_RSP_T NearAnn2Ann2NonEntGainCalcvncEnableRTC; NearAnn2Ann2MuteSet_RSP_T NearAnn2Ann2MuteSet; NearAnn2Ann2VolumeSet_RSP_T NearAnn2Ann2VolumeSet; NearAnn3Ann3NonEntGainCalcvncEnableRTC_RSP_T NearAnn3Ann3NonEntGainCalcvncEnableRTC; NearAnn3Ann3MuteSet_RSP_T NearAnn3Ann3MuteSet; NearAnn3Ann3VolumeSet_RSP_T NearAnn3Ann3VolumeSet; NearAnn4Ann4NonEntGainCalcvncEnableRTC_RSP_T NearAnn4Ann4NonEntGainCalcvncEnableRTC; NearAnn4Ann4MuteSet_RSP_T NearAnn4Ann4MuteSet; NearAnn4Ann4VolumeSet_RSP_T NearAnn4Ann4VolumeSet; NearEnable_RSP_T NearEnable; PostProcessEnergyLimiterConfigEnableLimiter_RSP_T PostProcessEnergyLimiterConfigEnableLimiter; PostProcessMuteCfgSet_RSP_T PostProcessMuteCfgSet; PostProcessPostLimiterEqShutdown_RSP_T PostProcessPostLimiterEqShutdown; PostProcessLimiterSoftclipParamSet_RSP_T PostProcessLimiterSoftclipParamSet; PostProcessLimiterSoftclipDisable_RSP_T PostProcessLimiterSoftclipDisable; PreAmpAudiopilotmuteReference_RSP_T PreAmpAudiopilotmuteReference; PreAmpAudiopilotgetBassBoost_RSP_T PreAmpAudiopilotgetBassBoost; PreAmpAudiopilotgetMidBoost_RSP_T PreAmpAudiopilotgetMidBoost; PreAmpAudiopilotgetTrebleBoost_RSP_T PreAmpAudiopilotgetTrebleBoost; PreAmpAudiopilotgetAverageAudioPilotBoostLevel_RSP_T PreAmpAudiopilotgetAverageAudioPilotBoostLevel; PreAmpAudiopilotsetDyneqOn_RSP_T PreAmpAudiopilotsetDyneqOn; PreAmpAudiopilotsetAudioPilotOn_RSP_T PreAmpAudiopilotsetAudioPilotOn; PreAmpVolumeCfgSet_RSP_T PreAmpVolumeCfgSet; PreAmpBalance_CfgSet_RSP_T PreAmpBalance_CfgSet; PreAmpMedusaSurroundLevelControlIndex_RSP_T PreAmpMedusaSurroundLevelControlIndex; PreAmpMedusaSurroundLevelControlReset_RSP_T PreAmpMedusaSurroundLevelControlReset; PreAmpMedusaPostUpmixDisable_RSP_T PreAmpMedusaPostUpmixDisable; PreAmpMedusaHoligramDisable_RSP_T PreAmpMedusaHoligramDisable; PreAmpMedusaOverheadHoligramDisable_RSP_T PreAmpMedusaOverheadHoligramDisable; PreAmpMedusaPostHoligramDisable_RSP_T PreAmpMedusaPostHoligramDisable; PreAmpFadeControlSet_RSP_T PreAmpFadeControlSet; PreAmpMuteControlSet_RSP_T PreAmpMuteControlSet; PreAmpResetSleepingBeauty_RSP_T PreAmpResetSleepingBeauty; PreAmpSleepingBeautyDRSet_RSP_T PreAmpSleepingBeautyDRSet; PreAmpSleepingBeautyFRSet_RSP_T PreAmpSleepingBeautyFRSet; PreAmpBassSet_RSP_T PreAmpBassSet; PreAmpMidrangeCfgSet_RSP_T PreAmpMidrangeCfgSet; PreAmpTrebleSet_RSP_T PreAmpTrebleSet; TestRouterASDRouterAuxEnable_RSP_T TestRouterASDRouterAuxEnable; TestRouterASDRouterAuxGain_RSP_T TestRouterASDRouterAuxGain; TestRouterASDRouterAuxSelect_RSP_T TestRouterASDRouterAuxSelect; TestRouterASDRouterMainGain_RSP_T TestRouterASDRouterMainGain; TestRouterASDRouterMainSelect_RSP_T TestRouterASDRouterMainSelect;
#define ALL_NTF_PAYLOADS               NearAnn1Ann1MuteSet_NTF_T NearAnn1Ann1MuteSet; NearAnn1Ann1VolumeSet_NTF_T NearAnn1Ann1VolumeSet; NearAnn2Ann2MuteSet_NTF_T NearAnn2Ann2MuteSet; NearAnn2Ann2VolumeSet_NTF_T NearAnn2Ann2VolumeSet; NearAnn3Ann3MuteSet_NTF_T NearAnn3Ann3MuteSet; NearAnn3Ann3VolumeSet_NTF_T NearAnn3Ann3VolumeSet; NearAnn4Ann4MuteSet_NTF_T NearAnn4Ann4MuteSet; NearAnn4Ann4VolumeSet_NTF_T NearAnn4Ann4VolumeSet; PostProcessEnergyLimiterConfigEnableLimiter_NTF_T PostProcessEnergyLimiterConfigEnableLimiter; PostProcessMuteCfgSet_NTF_T PostProcessMuteCfgSet; PreAmpVolumeCfgSet_NTF_T PreAmpVolumeCfgSet; PreAmpBalance_CfgSet_NTF_T PreAmpBalance_CfgSet; PreAmpMedusaSurroundLevelControlIndex_NTF_T PreAmpMedusaSurroundLevelControlIndex; PreAmpFadeControlSet_NTF_T PreAmpFadeControlSet; PreAmpMuteControlSet_NTF_T PreAmpMuteControlSet; PreAmpSleepingBeautyDRSet_NTF_T PreAmpSleepingBeautyDRSet; PreAmpSleepingBeautyFRSet_NTF_T PreAmpSleepingBeautyFRSet; PreAmpBassSet_NTF_T PreAmpBassSet; PreAmpMidrangeCfgSet_NTF_T PreAmpMidrangeCfgSet; PreAmpTrebleSet_NTF_T PreAmpTrebleSet;

// ------------------------------------------------------------
// RTC REQUEST PAYLOAD DEFINITIONS
// ------------------------------------------------------------

/// Request Payload Definition for InputSelectRouterCfgSet
/// ----------------------------
/// @ingroup InputSelectRouterCfgSet
/// This page describes the payload format for Request messages
/// used by the InputSelectRouterCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref InputSelectRouterCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_InputSelectRouterCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    int32_t routerMap[12];             // startChar:4 charSize:48
}
InputSelectRouterCfgSet_REQ_T;         /* '<S21>/UnpackREQ' */

#define InputSelectRouterCfgSet_REQ_routerMap_SIZE 12            /* '<S21>/UnpackREQ' */

/// Request Payload Definition for NearDr_RouterSet
/// ----------------------------
/// @ingroup NearDr_RouterSet
/// This page describes the payload format for Request messages
/// used by the NearDr_RouterSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearDr_RouterSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearDr_RouterSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    int32_t routerMap[18];             // startChar:4 charSize:72
}
NearDr_RouterSet_REQ_T;                /* '<S71>/UnpackREQ' */

#define NearDr_RouterSet_REQ_routerMap_SIZE 18                   /* '<S71>/UnpackREQ' */

/// Request Payload Definition for NearFr_RouterSet
/// ----------------------------
/// @ingroup NearFr_RouterSet
/// This page describes the payload format for Request messages
/// used by the NearFr_RouterSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearFr_RouterSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearFr_RouterSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    int32_t routerMap[18];             // startChar:4 charSize:72
}
NearFr_RouterSet_REQ_T;                /* '<S115>/UnpackREQ' */

#define NearFr_RouterSet_REQ_routerMap_SIZE 18                   /* '<S115>/UnpackREQ' */

/// Request Payload Definition for NearChannel2_Location
/// ----------------------------
/// @ingroup NearChannel2_Location
/// This page describes the payload format for Request messages
/// used by the NearChannel2_Location RTC mechanism. For a more general
/// description of how this message works, see @ref NearChannel2_Location.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearChannel2_Location
    uint32_t caller_handle;            // startChar:0 charSize:4
    float loc[2];                      // startChar:4 charSize:8
}
NearChannel2_Location_REQ_T;           /* '<S138>/UnpackREQ' */

#define NearChannel2_Location_REQ_loc_SIZE 2                     /* '<S138>/UnpackREQ' */

/// Request Payload Definition for NearChannel1_Location
/// ----------------------------
/// @ingroup NearChannel1_Location
/// This page describes the payload format for Request messages
/// used by the NearChannel1_Location RTC mechanism. For a more general
/// description of how this message works, see @ref NearChannel1_Location.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearChannel1_Location
    uint32_t caller_handle;            // startChar:0 charSize:4
    float loc[2];                      // startChar:4 charSize:8
}
NearChannel1_Location_REQ_T;           /* '<S144>/UnpackREQ' */

#define NearChannel1_Location_REQ_loc_SIZE 2                     /* '<S144>/UnpackREQ' */

/// Request Payload Definition for NearChannel4_Location
/// ----------------------------
/// @ingroup NearChannel4_Location
/// This page describes the payload format for Request messages
/// used by the NearChannel4_Location RTC mechanism. For a more general
/// description of how this message works, see @ref NearChannel4_Location.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearChannel4_Location
    uint32_t caller_handle;            // startChar:0 charSize:4
    float loc[2];                      // startChar:4 charSize:8
}
NearChannel4_Location_REQ_T;           /* '<S150>/UnpackREQ' */

#define NearChannel4_Location_REQ_loc_SIZE 2                     /* '<S150>/UnpackREQ' */

/// Request Payload Definition for NearChannel3_Location
/// ----------------------------
/// @ingroup NearChannel3_Location
/// This page describes the payload format for Request messages
/// used by the NearChannel3_Location RTC mechanism. For a more general
/// description of how this message works, see @ref NearChannel3_Location.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearChannel3_Location
    uint32_t caller_handle;            // startChar:0 charSize:4
    float loc[2];                      // startChar:4 charSize:8
}
NearChannel3_Location_REQ_T;           /* '<S156>/UnpackREQ' */

#define NearChannel3_Location_REQ_loc_SIZE 2                     /* '<S156>/UnpackREQ' */

/// Request Payload Definition for NearAnnNoiseLevel
/// ----------------------------
/// @ingroup NearAnnNoiseLevel
/// This page describes the payload format for Request messages
/// used by the NearAnnNoiseLevel RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnnNoiseLevel.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnnNoiseLevel
    float NoiseSig;                    // startChar:0 charSize:4
}
NearAnnNoiseLevel_REQ_T;               /* '<S233>/UnpackREQ' */

/// Request Payload Definition for NearAnn1Ann1NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn1Ann1NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Request messages
/// used by the NearAnn1Ann1NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1Ann1NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;       // always RTCID_NearAnn1Ann1NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t vncEnable;                // startChar:4 charSize:4
}
NearAnn1Ann1NonEntGainCalcvncEnableRTC_REQ_T;/* '<S257>/UnpackREQ' */

/// Request Payload Definition for NearAnn1Ann1MuteSet
/// ----------------------------
/// @ingroup NearAnn1Ann1MuteSet
/// This page describes the payload format for Request messages
/// used by the NearAnn1Ann1MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1Ann1MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn1Ann1MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t mute_switch;              // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn1Ann1MuteSet_REQ_T;             /* '<S277>/UnpackREQ' */

/// Request Payload Definition for NearAnn1Ann1VolumeSet
/// ----------------------------
/// @ingroup NearAnn1Ann1VolumeSet
/// This page describes the payload format for Request messages
/// used by the NearAnn1Ann1VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1Ann1VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn1Ann1VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t volume_index;             // startChar:4 charSize:4
    uint32_t db_per_second;            // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn1Ann1VolumeSet_REQ_T;           /* '<S305>/UnpackREQ' */

/// Request Payload Definition for NearAnn2Ann2NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn2Ann2NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Request messages
/// used by the NearAnn2Ann2NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2Ann2NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;       // always RTCID_NearAnn2Ann2NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t vncEnable;                // startChar:4 charSize:4
}
NearAnn2Ann2NonEntGainCalcvncEnableRTC_REQ_T;/* '<S337>/UnpackREQ' */

/// Request Payload Definition for NearAnn2Ann2MuteSet
/// ----------------------------
/// @ingroup NearAnn2Ann2MuteSet
/// This page describes the payload format for Request messages
/// used by the NearAnn2Ann2MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2Ann2MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn2Ann2MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t mute_switch;              // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn2Ann2MuteSet_REQ_T;             /* '<S357>/UnpackREQ' */

/// Request Payload Definition for NearAnn2Ann2VolumeSet
/// ----------------------------
/// @ingroup NearAnn2Ann2VolumeSet
/// This page describes the payload format for Request messages
/// used by the NearAnn2Ann2VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2Ann2VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn2Ann2VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t volume_index;             // startChar:4 charSize:4
    uint32_t db_per_second;            // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn2Ann2VolumeSet_REQ_T;           /* '<S385>/UnpackREQ' */

/// Request Payload Definition for NearAnn3Ann3NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn3Ann3NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Request messages
/// used by the NearAnn3Ann3NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3Ann3NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;       // always RTCID_NearAnn3Ann3NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t vncEnable;                // startChar:4 charSize:4
}
NearAnn3Ann3NonEntGainCalcvncEnableRTC_REQ_T;/* '<S417>/UnpackREQ' */

/// Request Payload Definition for NearAnn3Ann3MuteSet
/// ----------------------------
/// @ingroup NearAnn3Ann3MuteSet
/// This page describes the payload format for Request messages
/// used by the NearAnn3Ann3MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3Ann3MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn3Ann3MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t mute_switch;              // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn3Ann3MuteSet_REQ_T;             /* '<S437>/UnpackREQ' */

/// Request Payload Definition for NearAnn3Ann3VolumeSet
/// ----------------------------
/// @ingroup NearAnn3Ann3VolumeSet
/// This page describes the payload format for Request messages
/// used by the NearAnn3Ann3VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3Ann3VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn3Ann3VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t volume_index;             // startChar:4 charSize:4
    uint32_t db_per_second;            // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn3Ann3VolumeSet_REQ_T;           /* '<S465>/UnpackREQ' */

/// Request Payload Definition for NearAnn4Ann4NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn4Ann4NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Request messages
/// used by the NearAnn4Ann4NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4Ann4NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;       // always RTCID_NearAnn4Ann4NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t vncEnable;                // startChar:4 charSize:4
}
NearAnn4Ann4NonEntGainCalcvncEnableRTC_REQ_T;/* '<S497>/UnpackREQ' */

/// Request Payload Definition for NearAnn4Ann4MuteSet
/// ----------------------------
/// @ingroup NearAnn4Ann4MuteSet
/// This page describes the payload format for Request messages
/// used by the NearAnn4Ann4MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4Ann4MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn4Ann4MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t mute_switch;              // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn4Ann4MuteSet_REQ_T;             /* '<S517>/UnpackREQ' */

/// Request Payload Definition for NearAnn4Ann4VolumeSet
/// ----------------------------
/// @ingroup NearAnn4Ann4VolumeSet
/// This page describes the payload format for Request messages
/// used by the NearAnn4Ann4VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4Ann4VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn4Ann4VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t volume_index;             // startChar:4 charSize:4
    uint32_t db_per_second;            // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn4Ann4VolumeSet_REQ_T;           /* '<S545>/UnpackREQ' */

/// Request Payload Definition for NearEnable
/// ----------------------------
/// @ingroup NearEnable
/// This page describes the payload format for Request messages
/// used by the NearEnable RTC mechanism. For a more general
/// description of how this message works, see @ref NearEnable.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearEnable
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t Enable;                   // startChar:4 charSize:4
}
NearEnable_REQ_T;                      /* '<S559>/UnpackREQ' */

/// Request Payload Definition for PostProcessEnergyLimiterConfigEnableLimiter
/// ----------------------------
/// @ingroup PostProcessEnergyLimiterConfigEnableLimiter
/// This page describes the payload format for Request messages
/// used by the PostProcessEnergyLimiterConfigEnableLimiter RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessEnergyLimiterConfigEnableLimiter.
typedef struct
{
    uint32_t msgId;  // always RTCID_PostProcessEnergyLimiterConfigEnableLimiter
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t EnableGainLimiter;        // startChar:4 charSize:4
}
PostProcessEnergyLimiterConfigEnableLimiter_REQ_T;/* '<S597>/UnpackREQ' */

/// Request Payload Definition for PostProcessMuteCfgSet
/// ----------------------------
/// @ingroup PostProcessMuteCfgSet
/// This page describes the payload format for Request messages
/// used by the PostProcessMuteCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessMuteCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PostProcessMuteCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t mute_switch;              // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PostProcessMuteCfgSet_REQ_T;           /* '<S612>/UnpackREQ' */

/// Request Payload Definition for PostProcessPostLimiterEqShutdown
/// ----------------------------
/// @ingroup PostProcessPostLimiterEqShutdown
/// This page describes the payload format for Request messages
/// used by the PostProcessPostLimiterEqShutdown RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessPostLimiterEqShutdown.
typedef struct
{
    uint32_t msgId;             // always RTCID_PostProcessPostLimiterEqShutdown
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PostProcessPostLimiterEqShutdown_REQ_T;/* '<S639>/UnpackREQ' */

/// Request Payload Definition for PostProcessLimiterSoftclipParamSet
/// ----------------------------
/// @ingroup PostProcessLimiterSoftclipParamSet
/// This page describes the payload format for Request messages
/// used by the PostProcessLimiterSoftclipParamSet RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessLimiterSoftclipParamSet.
typedef struct
{
    uint32_t msgId;           // always RTCID_PostProcessLimiterSoftclipParamSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t paramSet;                 // startChar:4 charSize:4
}
PostProcessLimiterSoftclipParamSet_REQ_T;/* '<S645>/UnpackREQ' */

/// Request Payload Definition for PostProcessLimiterSoftclipDisable
/// ----------------------------
/// @ingroup PostProcessLimiterSoftclipDisable
/// This page describes the payload format for Request messages
/// used by the PostProcessLimiterSoftclipDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessLimiterSoftclipDisable.
typedef struct
{
    uint32_t msgId;            // always RTCID_PostProcessLimiterSoftclipDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t disableMsg;               // startChar:4 charSize:4
}
PostProcessLimiterSoftclipDisable_REQ_T;/* '<S651>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetHVACSetting
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetHVACSetting
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetHVACSetting RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetHVACSetting.
typedef struct
{
    uint32_t msgId;               // always RTCID_PreAmpAudiopilotsetHVACSetting
    float HVACSetting;                 // startChar:0 charSize:4
}
PreAmpAudiopilotsetHVACSetting_REQ_T;  /* '<S684>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotmuteReference
/// ----------------------------
/// @ingroup PreAmpAudiopilotmuteReference
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotmuteReference RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotmuteReference.
typedef struct
{
    uint32_t msgId;                // always RTCID_PreAmpAudiopilotmuteReference
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t muteReference;            // startChar:4 charSize:4
}
PreAmpAudiopilotmuteReference_REQ_T;   /* '<S690>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetSpeedIn
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetSpeedIn
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetSpeedIn RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetSpeedIn.
typedef struct
{
    uint32_t msgId;                   // always RTCID_PreAmpAudiopilotsetSpeedIn
    float speedIn;                     // startChar:0 charSize:4
}
PreAmpAudiopilotsetSpeedIn_REQ_T;      /* '<S696>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotgetBassBoost
/// ----------------------------
/// @ingroup PreAmpAudiopilotgetBassBoost
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotgetBassBoost RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotgetBassBoost.
typedef struct
{
    uint32_t msgId;                 // always RTCID_PreAmpAudiopilotgetBassBoost
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpAudiopilotgetBassBoost_REQ_T;    /* '<S720>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotgetMidBoost
/// ----------------------------
/// @ingroup PreAmpAudiopilotgetMidBoost
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotgetMidBoost RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotgetMidBoost.
typedef struct
{
    uint32_t msgId;                  // always RTCID_PreAmpAudiopilotgetMidBoost
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpAudiopilotgetMidBoost_REQ_T;     /* '<S726>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotgetTrebleBoost
/// ----------------------------
/// @ingroup PreAmpAudiopilotgetTrebleBoost
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotgetTrebleBoost RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotgetTrebleBoost.
typedef struct
{
    uint32_t msgId;               // always RTCID_PreAmpAudiopilotgetTrebleBoost
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpAudiopilotgetTrebleBoost_REQ_T;  /* '<S732>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetDynEQBoostOverRide
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetDynEQBoostOverRide
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetDynEQBoostOverRide RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetDynEQBoostOverRide.
typedef struct
{
    uint32_t msgId;        // always RTCID_PreAmpAudiopilotsetDynEQBoostOverRide
    uint32_t DynEQBoostOverRideFlag;   // startChar:0 charSize:4
    float DynEQBoostOverRide;          // startChar:4 charSize:4
}
PreAmpAudiopilotsetDynEQBoostOverRide_REQ_T;/* '<S780>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetVNCBassBoostOverRide
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetVNCBassBoostOverRide
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetVNCBassBoostOverRide RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetVNCBassBoostOverRide.
typedef struct
{
    uint32_t msgId;      // always RTCID_PreAmpAudiopilotsetVNCBassBoostOverRide
    uint32_t VNCBassBoostOverRideFlag; // startChar:0 charSize:4
    float VNCBassBoostOverRide;        // startChar:4 charSize:4
}
PreAmpAudiopilotsetVNCBassBoostOverRide_REQ_T;/* '<S786>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetVNCMidBoostOverRide
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetVNCMidBoostOverRide
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetVNCMidBoostOverRide RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetVNCMidBoostOverRide.
typedef struct
{
    uint32_t msgId;       // always RTCID_PreAmpAudiopilotsetVNCMidBoostOverRide
    uint32_t VNCMidBoostOverRideFlag;  // startChar:0 charSize:4
    float VNCMidBoostOverRide;         // startChar:4 charSize:4
}
PreAmpAudiopilotsetVNCMidBoostOverRide_REQ_T;/* '<S792>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetVNCTrebleBoostOverRide
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetVNCTrebleBoostOverRide
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetVNCTrebleBoostOverRide RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetVNCTrebleBoostOverRide.
typedef struct
{
    uint32_t msgId;    // always RTCID_PreAmpAudiopilotsetVNCTrebleBoostOverRide
    uint32_t VNCTrebleBoostOverRideFlag;// startChar:0 charSize:4
    float VNCTrebleBoostOverRide;      // startChar:4 charSize:4
}
PreAmpAudiopilotsetVNCTrebleBoostOverRide_REQ_T;/* '<S798>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetLoudSignalOverRide
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetLoudSignalOverRide
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetLoudSignalOverRide RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetLoudSignalOverRide.
typedef struct
{
    uint32_t msgId;        // always RTCID_PreAmpAudiopilotsetLoudSignalOverRide
    uint32_t LoudSignalOverRideFlag;   // startChar:0 charSize:4
    float LoudSignalOverRide;          // startChar:4 charSize:4
}
PreAmpAudiopilotsetLoudSignalOverRide_REQ_T;/* '<S808>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetVNCSignalOverRide
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetVNCSignalOverRide
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetVNCSignalOverRide RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetVNCSignalOverRide.
typedef struct
{
    uint32_t msgId;         // always RTCID_PreAmpAudiopilotsetVNCSignalOverRide
    uint32_t VNCSignalOverRideFlag;    // startChar:0 charSize:4
    float VNCSignalOverRide;           // startChar:4 charSize:4
}
PreAmpAudiopilotsetVNCSignalOverRide_REQ_T;/* '<S814>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotgetAverageAudioPilotBoostLevel
/// ----------------------------
/// @ingroup PreAmpAudiopilotgetAverageAudioPilotBoostLevel
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotgetAverageAudioPilotBoostLevel RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotgetAverageAudioPilotBoostLevel.
typedef struct
{
    uint32_t msgId;
                  // always RTCID_PreAmpAudiopilotgetAverageAudioPilotBoostLevel
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpAudiopilotgetAverageAudioPilotBoostLevel_REQ_T;/* '<S820>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetHFNoiseOverRide
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetHFNoiseOverRide
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetHFNoiseOverRide RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetHFNoiseOverRide.
typedef struct
{
    uint32_t msgId;           // always RTCID_PreAmpAudiopilotsetHFNoiseOverRide
    uint32_t HFNoiseOverRideFlag;      // startChar:0 charSize:4
    float HFNoiseOverRide;             // startChar:4 charSize:4
}
PreAmpAudiopilotsetHFNoiseOverRide_REQ_T;/* '<S831>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetLFNoiseOverRide
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetLFNoiseOverRide
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetLFNoiseOverRide RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetLFNoiseOverRide.
typedef struct
{
    uint32_t msgId;           // always RTCID_PreAmpAudiopilotsetLFNoiseOverRide
    uint32_t LFNoiseOverRideFlag;      // startChar:0 charSize:4
    float LFNoiseOverRide;             // startChar:4 charSize:4
}
PreAmpAudiopilotsetLFNoiseOverRide_REQ_T;/* '<S837>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetRatioOverRide
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetRatioOverRide
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetRatioOverRide RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetRatioOverRide.
typedef struct
{
    uint32_t msgId;             // always RTCID_PreAmpAudiopilotsetRatioOverRide
    uint32_t RatioOverRideFlag;        // startChar:0 charSize:4
    float RatioOverRide;               // startChar:4 charSize:4
}
PreAmpAudiopilotsetRatioOverRide_REQ_T;/* '<S843>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetWBNoiseOverRide
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetWBNoiseOverRide
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetWBNoiseOverRide RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetWBNoiseOverRide.
typedef struct
{
    uint32_t msgId;           // always RTCID_PreAmpAudiopilotsetWBNoiseOverRide
    uint32_t WBNoiseOverRideFlag;      // startChar:0 charSize:4
    float WBNoiseOverRide;             // startChar:4 charSize:4
}
PreAmpAudiopilotsetWBNoiseOverRide_REQ_T;/* '<S849>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetDyneqOn
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetDyneqOn
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetDyneqOn RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetDyneqOn.
typedef struct
{
    uint32_t msgId;                   // always RTCID_PreAmpAudiopilotsetDyneqOn
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t dyneq_on;                 // startChar:4 charSize:4
}
PreAmpAudiopilotsetDyneqOn_REQ_T;      /* '<S871>/UnpackREQ' */

/// Request Payload Definition for PreAmpAudiopilotsetAudioPilotOn
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetAudioPilotOn
/// This page describes the payload format for Request messages
/// used by the PreAmpAudiopilotsetAudioPilotOn RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetAudioPilotOn.
typedef struct
{
    uint32_t msgId;              // always RTCID_PreAmpAudiopilotsetAudioPilotOn
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t audiopilot_on;            // startChar:4 charSize:4
}
PreAmpAudiopilotsetAudioPilotOn_REQ_T; /* '<S877>/UnpackREQ' */

/// Request Payload Definition for PreAmpVolumeCfgSet
/// ----------------------------
/// @ingroup PreAmpVolumeCfgSet
/// This page describes the payload format for Request messages
/// used by the PreAmpVolumeCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpVolumeCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpVolumeCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
    int32_t gain_offset;               // startChar:16 charSize:4
}
PreAmpVolumeCfgSet_REQ_T;              /* '<S1126>/UnpackREQ' */

/// Request Payload Definition for PreAmpBalance_CfgSet
/// ----------------------------
/// @ingroup PreAmpBalance_CfgSet
/// This page describes the payload format for Request messages
/// used by the PreAmpBalance_CfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpBalance_CfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpBalance_CfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpBalance_CfgSet_REQ_T;            /* '<S1162>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaSurroundLevelControlIndex
/// ----------------------------
/// @ingroup PreAmpMedusaSurroundLevelControlIndex
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaSurroundLevelControlIndex RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSurroundLevelControlIndex.
typedef struct
{
    uint32_t msgId;        // always RTCID_PreAmpMedusaSurroundLevelControlIndex
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t surround_index;           // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpMedusaSurroundLevelControlIndex_REQ_T;/* '<S1634>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaSurroundLevelControlReset
/// ----------------------------
/// @ingroup PreAmpMedusaSurroundLevelControlReset
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaSurroundLevelControlReset RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSurroundLevelControlReset.
typedef struct
{
    uint32_t msgId;        // always RTCID_PreAmpMedusaSurroundLevelControlReset
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpMedusaSurroundLevelControlReset_REQ_T;/* '<S1640>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaPostUpmixDisable
/// ----------------------------
/// @ingroup PreAmpMedusaPostUpmixDisable
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaPostUpmixDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaPostUpmixDisable.
typedef struct
{
    uint32_t msgId;                 // always RTCID_PreAmpMedusaPostUpmixDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpMedusaPostUpmixDisable_REQ_T;    /* '<S1698>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaHoligramDisable
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaHoligramDisable.
typedef struct
{
    uint32_t msgId;                  // always RTCID_PreAmpMedusaHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpMedusaHoligramDisable_REQ_T;     /* '<S1768>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaOverheadHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaOverheadHoligramDisable
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaOverheadHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaOverheadHoligramDisable.
typedef struct
{
    uint32_t msgId;          // always RTCID_PreAmpMedusaOverheadHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpMedusaOverheadHoligramDisable_REQ_T;/* '<S1780>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaPostHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaPostHoligramDisable
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaPostHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaPostHoligramDisable.
typedef struct
{
    uint32_t msgId;              // always RTCID_PreAmpMedusaPostHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpMedusaPostHoligramDisable_REQ_T; /* '<S1799>/UnpackREQ' */

/// Request Payload Definition for PreAmpFadeControlSet
/// ----------------------------
/// @ingroup PreAmpFadeControlSet
/// This page describes the payload format for Request messages
/// used by the PreAmpFadeControlSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpFadeControlSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpFadeControlSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpFadeControlSet_REQ_T;            /* '<S1841>/UnpackREQ' */

/// Request Payload Definition for PreAmpMuteControlSet
/// ----------------------------
/// @ingroup PreAmpMuteControlSet
/// This page describes the payload format for Request messages
/// used by the PreAmpMuteControlSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMuteControlSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpMuteControlSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t mute_switch;              // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpMuteControlSet_REQ_T;            /* '<S1866>/UnpackREQ' */

/// Request Payload Definition for PreAmpResetSleepingBeauty
/// ----------------------------
/// @ingroup PreAmpResetSleepingBeauty
/// This page describes the payload format for Request messages
/// used by the PreAmpResetSleepingBeauty RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpResetSleepingBeauty.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpResetSleepingBeauty
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpResetSleepingBeauty_REQ_T;       /* '<S1898>/UnpackREQ' */

/// Request Payload Definition for PreAmpSleepingBeautyDRSet
/// ----------------------------
/// @ingroup PreAmpSleepingBeautyDRSet
/// This page describes the payload format for Request messages
/// used by the PreAmpSleepingBeautyDRSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpSleepingBeautyDRSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpSleepingBeautyDRSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpSleepingBeautyDRSet_REQ_T;       /* '<S1918>/UnpackREQ' */

/// Request Payload Definition for PreAmpSleepingBeautyFRSet
/// ----------------------------
/// @ingroup PreAmpSleepingBeautyFRSet
/// This page describes the payload format for Request messages
/// used by the PreAmpSleepingBeautyFRSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpSleepingBeautyFRSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpSleepingBeautyFRSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpSleepingBeautyFRSet_REQ_T;       /* '<S1938>/UnpackREQ' */

/// Request Payload Definition for PreAmpBassSet
/// ----------------------------
/// @ingroup PreAmpBassSet
/// This page describes the payload format for Request messages
/// used by the PreAmpBassSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpBassSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpBassSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpBassSet_REQ_T;                   /* '<S1994>/UnpackREQ' */

/// Request Payload Definition for PreAmpMidrangeCfgSet
/// ----------------------------
/// @ingroup PreAmpMidrangeCfgSet
/// This page describes the payload format for Request messages
/// used by the PreAmpMidrangeCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMidrangeCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpMidrangeCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpMidrangeCfgSet_REQ_T;            /* '<S2012>/UnpackREQ' */

/// Request Payload Definition for PreAmpTrebleSet
/// ----------------------------
/// @ingroup PreAmpTrebleSet
/// This page describes the payload format for Request messages
/// used by the PreAmpTrebleSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpTrebleSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpTrebleSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpTrebleSet_REQ_T;                 /* '<S2048>/UnpackREQ' */

/// Request Payload Definition for TestRouterASDRouterAuxEnable
/// ----------------------------
/// @ingroup TestRouterASDRouterAuxEnable
/// This page describes the payload format for Request messages
/// used by the TestRouterASDRouterAuxEnable RTC mechanism. For a more general
/// description of how this message works, see @ref TestRouterASDRouterAuxEnable.
typedef struct
{
    uint32_t msgId;                 // always RTCID_TestRouterASDRouterAuxEnable
    uint32_t caller_handle;            // startChar:0 charSize:4
    int32_t EnableAuxFlag;             // startChar:4 charSize:4
}
TestRouterASDRouterAuxEnable_REQ_T;    /* '<S2070>/UnpackREQ' */

/// Request Payload Definition for TestRouterASDRouterAuxGain
/// ----------------------------
/// @ingroup TestRouterASDRouterAuxGain
/// This page describes the payload format for Request messages
/// used by the TestRouterASDRouterAuxGain RTC mechanism. For a more general
/// description of how this message works, see @ref TestRouterASDRouterAuxGain.
typedef struct
{
    uint32_t msgId;                   // always RTCID_TestRouterASDRouterAuxGain
    uint32_t caller_handle;            // startChar:0 charSize:4
    float AuxGain[24];                 // startChar:4 charSize:96
}
TestRouterASDRouterAuxGain_REQ_T;      /* '<S2076>/UnpackREQ' */

#define TestRouterASDRouterAuxGain_REQ_AuxGain_SIZE 24           /* '<S2076>/UnpackREQ' */

/// Request Payload Definition for TestRouterASDRouterAuxSelect
/// ----------------------------
/// @ingroup TestRouterASDRouterAuxSelect
/// This page describes the payload format for Request messages
/// used by the TestRouterASDRouterAuxSelect RTC mechanism. For a more general
/// description of how this message works, see @ref TestRouterASDRouterAuxSelect.
typedef struct
{
    uint32_t msgId;                 // always RTCID_TestRouterASDRouterAuxSelect
    uint32_t caller_handle;            // startChar:0 charSize:4
    int32_t AuxSelect[24];             // startChar:4 charSize:96
}
TestRouterASDRouterAuxSelect_REQ_T;    /* '<S2082>/UnpackREQ' */

#define TestRouterASDRouterAuxSelect_REQ_AuxSelect_SIZE 24       /* '<S2082>/UnpackREQ' */

/// Request Payload Definition for TestRouterASDRouterMainGain
/// ----------------------------
/// @ingroup TestRouterASDRouterMainGain
/// This page describes the payload format for Request messages
/// used by the TestRouterASDRouterMainGain RTC mechanism. For a more general
/// description of how this message works, see @ref TestRouterASDRouterMainGain.
typedef struct
{
    uint32_t msgId;                  // always RTCID_TestRouterASDRouterMainGain
    uint32_t caller_handle;            // startChar:0 charSize:4
    float mainGain[24];                // startChar:4 charSize:96
}
TestRouterASDRouterMainGain_REQ_T;     /* '<S2094>/UnpackREQ' */

#define TestRouterASDRouterMainGain_REQ_mainGain_SIZE 24         /* '<S2094>/UnpackREQ' */

/// Request Payload Definition for TestRouterASDRouterMainSelect
/// ----------------------------
/// @ingroup TestRouterASDRouterMainSelect
/// This page describes the payload format for Request messages
/// used by the TestRouterASDRouterMainSelect RTC mechanism. For a more general
/// description of how this message works, see @ref TestRouterASDRouterMainSelect.
typedef struct
{
    uint32_t msgId;                // always RTCID_TestRouterASDRouterMainSelect
    uint32_t caller_handle;            // startChar:0 charSize:4
    int32_t mainSelect[24];            // startChar:4 charSize:96
}
TestRouterASDRouterMainSelect_REQ_T;   /* '<S2100>/UnpackREQ' */

#define TestRouterASDRouterMainSelect_REQ_mainSelect_SIZE 24     /* '<S2100>/UnpackREQ' */

// ------------------------------------------------------------
// RTC RESPONSE PAYLOAD DEFINITIONS
// ------------------------------------------------------------

/// Response Payload Definition for InputSelectRouterCfgSet
/// ----------------------------
/// @ingroup InputSelectRouterCfgSet
/// This page describes the payload format for Response messages
/// used by the InputSelectRouterCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref InputSelectRouterCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_InputSelectRouterCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    int32_t routerMap[12];             // startChar:8 charSize:48
}
InputSelectRouterCfgSet_RSP_T;         /* '<S22>/PackRSP' */

#define InputSelectRouterCfgSet_RSP_routerMap_SIZE 12            /* '<S22>/PackRSP' */

/// Response Payload Definition for NearDr_RouterSet
/// ----------------------------
/// @ingroup NearDr_RouterSet
/// This page describes the payload format for Response messages
/// used by the NearDr_RouterSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearDr_RouterSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearDr_RouterSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    int32_t routerMap[18];             // startChar:8 charSize:72
}
NearDr_RouterSet_RSP_T;                /* '<S72>/PackRSP' */

#define NearDr_RouterSet_RSP_routerMap_SIZE 18                   /* '<S72>/PackRSP' */

/// Response Payload Definition for NearFr_RouterSet
/// ----------------------------
/// @ingroup NearFr_RouterSet
/// This page describes the payload format for Response messages
/// used by the NearFr_RouterSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearFr_RouterSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearFr_RouterSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    int32_t routerMap[18];             // startChar:8 charSize:72
}
NearFr_RouterSet_RSP_T;                /* '<S116>/PackRSP' */

#define NearFr_RouterSet_RSP_routerMap_SIZE 18                   /* '<S116>/PackRSP' */

/// Response Payload Definition for NearChannel2_Location
/// ----------------------------
/// @ingroup NearChannel2_Location
/// This page describes the payload format for Response messages
/// used by the NearChannel2_Location RTC mechanism. For a more general
/// description of how this message works, see @ref NearChannel2_Location.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearChannel2_Location
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearChannel2_Location_RSP_T;           /* '<S139>/PackRSP' */

/// Response Payload Definition for NearChannel1_Location
/// ----------------------------
/// @ingroup NearChannel1_Location
/// This page describes the payload format for Response messages
/// used by the NearChannel1_Location RTC mechanism. For a more general
/// description of how this message works, see @ref NearChannel1_Location.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearChannel1_Location
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearChannel1_Location_RSP_T;           /* '<S145>/PackRSP' */

/// Response Payload Definition for NearChannel4_Location
/// ----------------------------
/// @ingroup NearChannel4_Location
/// This page describes the payload format for Response messages
/// used by the NearChannel4_Location RTC mechanism. For a more general
/// description of how this message works, see @ref NearChannel4_Location.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearChannel4_Location
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearChannel4_Location_RSP_T;           /* '<S151>/PackRSP' */

/// Response Payload Definition for NearChannel3_Location
/// ----------------------------
/// @ingroup NearChannel3_Location
/// This page describes the payload format for Response messages
/// used by the NearChannel3_Location RTC mechanism. For a more general
/// description of how this message works, see @ref NearChannel3_Location.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearChannel3_Location
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearChannel3_Location_RSP_T;           /* '<S157>/PackRSP' */

// NearAnnNoiseLevel does not define a Response payload.

/// Response Payload Definition for NearAnn1Ann1NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn1Ann1NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Response messages
/// used by the NearAnn1Ann1NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1Ann1NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;       // always RTCID_NearAnn1Ann1NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t enable;                   // startChar:8 charSize:4
}
NearAnn1Ann1NonEntGainCalcvncEnableRTC_RSP_T;/* '<S258>/PackRSP' */

/// Response Payload Definition for NearAnn1Ann1MuteSet
/// ----------------------------
/// @ingroup NearAnn1Ann1MuteSet
/// This page describes the payload format for Response messages
/// used by the NearAnn1Ann1MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1Ann1MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn1Ann1MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
NearAnn1Ann1MuteSet_RSP_T;             /* '<S278>/PackRSP' */

/// Response Payload Definition for NearAnn1Ann1VolumeSet
/// ----------------------------
/// @ingroup NearAnn1Ann1VolumeSet
/// This page describes the payload format for Response messages
/// used by the NearAnn1Ann1VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1Ann1VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn1Ann1VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn1Ann1VolumeSet_RSP_T;           /* '<S306>/PackRSP' */

/// Response Payload Definition for NearAnn2Ann2NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn2Ann2NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Response messages
/// used by the NearAnn2Ann2NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2Ann2NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;       // always RTCID_NearAnn2Ann2NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t enable;                   // startChar:8 charSize:4
}
NearAnn2Ann2NonEntGainCalcvncEnableRTC_RSP_T;/* '<S338>/PackRSP' */

/// Response Payload Definition for NearAnn2Ann2MuteSet
/// ----------------------------
/// @ingroup NearAnn2Ann2MuteSet
/// This page describes the payload format for Response messages
/// used by the NearAnn2Ann2MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2Ann2MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn2Ann2MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
NearAnn2Ann2MuteSet_RSP_T;             /* '<S358>/PackRSP' */

/// Response Payload Definition for NearAnn2Ann2VolumeSet
/// ----------------------------
/// @ingroup NearAnn2Ann2VolumeSet
/// This page describes the payload format for Response messages
/// used by the NearAnn2Ann2VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2Ann2VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn2Ann2VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn2Ann2VolumeSet_RSP_T;           /* '<S386>/PackRSP' */

/// Response Payload Definition for NearAnn3Ann3NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn3Ann3NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Response messages
/// used by the NearAnn3Ann3NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3Ann3NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;       // always RTCID_NearAnn3Ann3NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t enable;                   // startChar:8 charSize:4
}
NearAnn3Ann3NonEntGainCalcvncEnableRTC_RSP_T;/* '<S418>/PackRSP' */

/// Response Payload Definition for NearAnn3Ann3MuteSet
/// ----------------------------
/// @ingroup NearAnn3Ann3MuteSet
/// This page describes the payload format for Response messages
/// used by the NearAnn3Ann3MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3Ann3MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn3Ann3MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
NearAnn3Ann3MuteSet_RSP_T;             /* '<S438>/PackRSP' */

/// Response Payload Definition for NearAnn3Ann3VolumeSet
/// ----------------------------
/// @ingroup NearAnn3Ann3VolumeSet
/// This page describes the payload format for Response messages
/// used by the NearAnn3Ann3VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3Ann3VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn3Ann3VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn3Ann3VolumeSet_RSP_T;           /* '<S466>/PackRSP' */

/// Response Payload Definition for NearAnn4Ann4NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn4Ann4NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Response messages
/// used by the NearAnn4Ann4NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4Ann4NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;       // always RTCID_NearAnn4Ann4NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t enable;                   // startChar:8 charSize:4
}
NearAnn4Ann4NonEntGainCalcvncEnableRTC_RSP_T;/* '<S498>/PackRSP' */

/// Response Payload Definition for NearAnn4Ann4MuteSet
/// ----------------------------
/// @ingroup NearAnn4Ann4MuteSet
/// This page describes the payload format for Response messages
/// used by the NearAnn4Ann4MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4Ann4MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn4Ann4MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
NearAnn4Ann4MuteSet_RSP_T;             /* '<S518>/PackRSP' */

/// Response Payload Definition for NearAnn4Ann4VolumeSet
/// ----------------------------
/// @ingroup NearAnn4Ann4VolumeSet
/// This page describes the payload format for Response messages
/// used by the NearAnn4Ann4VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4Ann4VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn4Ann4VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn4Ann4VolumeSet_RSP_T;           /* '<S546>/PackRSP' */

/// Response Payload Definition for NearEnable
/// ----------------------------
/// @ingroup NearEnable
/// This page describes the payload format for Response messages
/// used by the NearEnable RTC mechanism. For a more general
/// description of how this message works, see @ref NearEnable.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearEnable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearEnable_RSP_T;                      /* '<S560>/PackRSP' */

/// Response Payload Definition for PostProcessEnergyLimiterConfigEnableLimiter
/// ----------------------------
/// @ingroup PostProcessEnergyLimiterConfigEnableLimiter
/// This page describes the payload format for Response messages
/// used by the PostProcessEnergyLimiterConfigEnableLimiter RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessEnergyLimiterConfigEnableLimiter.
typedef struct
{
    uint32_t msgId;  // always RTCID_PostProcessEnergyLimiterConfigEnableLimiter
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PostProcessEnergyLimiterConfigEnableLimiter_RSP_T;/* '<S598>/PackRSP' */

/// Response Payload Definition for PostProcessMuteCfgSet
/// ----------------------------
/// @ingroup PostProcessMuteCfgSet
/// This page describes the payload format for Response messages
/// used by the PostProcessMuteCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessMuteCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PostProcessMuteCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
PostProcessMuteCfgSet_RSP_T;           /* '<S613>/PackRSP' */

/// Response Payload Definition for PostProcessPostLimiterEqShutdown
/// ----------------------------
/// @ingroup PostProcessPostLimiterEqShutdown
/// This page describes the payload format for Response messages
/// used by the PostProcessPostLimiterEqShutdown RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessPostLimiterEqShutdown.
typedef struct
{
    uint32_t msgId;             // always RTCID_PostProcessPostLimiterEqShutdown
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PostProcessPostLimiterEqShutdown_RSP_T;/* '<S640>/PackRSP' */

/// Response Payload Definition for PostProcessLimiterSoftclipParamSet
/// ----------------------------
/// @ingroup PostProcessLimiterSoftclipParamSet
/// This page describes the payload format for Response messages
/// used by the PostProcessLimiterSoftclipParamSet RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessLimiterSoftclipParamSet.
typedef struct
{
    uint32_t msgId;           // always RTCID_PostProcessLimiterSoftclipParamSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t paramSet;                 // startChar:8 charSize:4
}
PostProcessLimiterSoftclipParamSet_RSP_T;/* '<S646>/PackRSP' */

/// Response Payload Definition for PostProcessLimiterSoftclipDisable
/// ----------------------------
/// @ingroup PostProcessLimiterSoftclipDisable
/// This page describes the payload format for Response messages
/// used by the PostProcessLimiterSoftclipDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessLimiterSoftclipDisable.
typedef struct
{
    uint32_t msgId;            // always RTCID_PostProcessLimiterSoftclipDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t disableMsg;               // startChar:8 charSize:4
}
PostProcessLimiterSoftclipDisable_RSP_T;/* '<S652>/PackRSP' */

// PreAmpAudiopilotsetHVACSetting does not define a Response payload.

/// Response Payload Definition for PreAmpAudiopilotmuteReference
/// ----------------------------
/// @ingroup PreAmpAudiopilotmuteReference
/// This page describes the payload format for Response messages
/// used by the PreAmpAudiopilotmuteReference RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotmuteReference.
typedef struct
{
    uint32_t msgId;                // always RTCID_PreAmpAudiopilotmuteReference
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t muteReference;            // startChar:8 charSize:4
}
PreAmpAudiopilotmuteReference_RSP_T;   /* '<S691>/PackRSP' */

// PreAmpAudiopilotsetSpeedIn does not define a Response payload.

/// Response Payload Definition for PreAmpAudiopilotgetBassBoost
/// ----------------------------
/// @ingroup PreAmpAudiopilotgetBassBoost
/// This page describes the payload format for Response messages
/// used by the PreAmpAudiopilotgetBassBoost RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotgetBassBoost.
typedef struct
{
    uint32_t msgId;                 // always RTCID_PreAmpAudiopilotgetBassBoost
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    float bassBoost;                   // startChar:8 charSize:4
}
PreAmpAudiopilotgetBassBoost_RSP_T;    /* '<S721>/PackRSP' */

/// Response Payload Definition for PreAmpAudiopilotgetMidBoost
/// ----------------------------
/// @ingroup PreAmpAudiopilotgetMidBoost
/// This page describes the payload format for Response messages
/// used by the PreAmpAudiopilotgetMidBoost RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotgetMidBoost.
typedef struct
{
    uint32_t msgId;                  // always RTCID_PreAmpAudiopilotgetMidBoost
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    float midBoost;                    // startChar:8 charSize:4
}
PreAmpAudiopilotgetMidBoost_RSP_T;     /* '<S727>/PackRSP' */

/// Response Payload Definition for PreAmpAudiopilotgetTrebleBoost
/// ----------------------------
/// @ingroup PreAmpAudiopilotgetTrebleBoost
/// This page describes the payload format for Response messages
/// used by the PreAmpAudiopilotgetTrebleBoost RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotgetTrebleBoost.
typedef struct
{
    uint32_t msgId;               // always RTCID_PreAmpAudiopilotgetTrebleBoost
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    float trebleBoost;                 // startChar:8 charSize:4
}
PreAmpAudiopilotgetTrebleBoost_RSP_T;  /* '<S733>/PackRSP' */

// PreAmpAudiopilotsetDynEQBoostOverRide does not define a Response payload.

// PreAmpAudiopilotsetVNCBassBoostOverRide does not define a Response payload.

// PreAmpAudiopilotsetVNCMidBoostOverRide does not define a Response payload.

// PreAmpAudiopilotsetVNCTrebleBoostOverRide does not define a Response payload.

// PreAmpAudiopilotsetLoudSignalOverRide does not define a Response payload.

// PreAmpAudiopilotsetVNCSignalOverRide does not define a Response payload.

/// Response Payload Definition for PreAmpAudiopilotgetAverageAudioPilotBoostLevel
/// ----------------------------
/// @ingroup PreAmpAudiopilotgetAverageAudioPilotBoostLevel
/// This page describes the payload format for Response messages
/// used by the PreAmpAudiopilotgetAverageAudioPilotBoostLevel RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotgetAverageAudioPilotBoostLevel.
typedef struct
{
    uint32_t msgId;
                  // always RTCID_PreAmpAudiopilotgetAverageAudioPilotBoostLevel
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    float level;                       // startChar:8 charSize:4
}
PreAmpAudiopilotgetAverageAudioPilotBoostLevel_RSP_T;/* '<S821>/PackRSP' */

// PreAmpAudiopilotsetHFNoiseOverRide does not define a Response payload.

// PreAmpAudiopilotsetLFNoiseOverRide does not define a Response payload.

// PreAmpAudiopilotsetRatioOverRide does not define a Response payload.

// PreAmpAudiopilotsetWBNoiseOverRide does not define a Response payload.

/// Response Payload Definition for PreAmpAudiopilotsetDyneqOn
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetDyneqOn
/// This page describes the payload format for Response messages
/// used by the PreAmpAudiopilotsetDyneqOn RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetDyneqOn.
typedef struct
{
    uint32_t msgId;                   // always RTCID_PreAmpAudiopilotsetDyneqOn
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t dyneq_on;                 // startChar:8 charSize:4
}
PreAmpAudiopilotsetDyneqOn_RSP_T;      /* '<S872>/PackRSP' */

/// Response Payload Definition for PreAmpAudiopilotsetAudioPilotOn
/// ----------------------------
/// @ingroup PreAmpAudiopilotsetAudioPilotOn
/// This page describes the payload format for Response messages
/// used by the PreAmpAudiopilotsetAudioPilotOn RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpAudiopilotsetAudioPilotOn.
typedef struct
{
    uint32_t msgId;              // always RTCID_PreAmpAudiopilotsetAudioPilotOn
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t audiopilot_on;            // startChar:8 charSize:4
}
PreAmpAudiopilotsetAudioPilotOn_RSP_T; /* '<S878>/PackRSP' */

/// Response Payload Definition for PreAmpVolumeCfgSet
/// ----------------------------
/// @ingroup PreAmpVolumeCfgSet
/// This page describes the payload format for Response messages
/// used by the PreAmpVolumeCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpVolumeCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpVolumeCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpVolumeCfgSet_RSP_T;              /* '<S1127>/PackRSP' */

/// Response Payload Definition for PreAmpBalance_CfgSet
/// ----------------------------
/// @ingroup PreAmpBalance_CfgSet
/// This page describes the payload format for Response messages
/// used by the PreAmpBalance_CfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpBalance_CfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpBalance_CfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpBalance_CfgSet_RSP_T;            /* '<S1163>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaSurroundLevelControlIndex
/// ----------------------------
/// @ingroup PreAmpMedusaSurroundLevelControlIndex
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaSurroundLevelControlIndex RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSurroundLevelControlIndex.
typedef struct
{
    uint32_t msgId;        // always RTCID_PreAmpMedusaSurroundLevelControlIndex
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t surround_index;           // startChar:8 charSize:4
}
PreAmpMedusaSurroundLevelControlIndex_RSP_T;/* '<S1635>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaSurroundLevelControlReset
/// ----------------------------
/// @ingroup PreAmpMedusaSurroundLevelControlReset
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaSurroundLevelControlReset RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSurroundLevelControlReset.
typedef struct
{
    uint32_t msgId;        // always RTCID_PreAmpMedusaSurroundLevelControlReset
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaSurroundLevelControlReset_RSP_T;/* '<S1641>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaPostUpmixDisable
/// ----------------------------
/// @ingroup PreAmpMedusaPostUpmixDisable
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaPostUpmixDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaPostUpmixDisable.
typedef struct
{
    uint32_t msgId;                 // always RTCID_PreAmpMedusaPostUpmixDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaPostUpmixDisable_RSP_T;    /* '<S1699>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaHoligramDisable
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaHoligramDisable.
typedef struct
{
    uint32_t msgId;                  // always RTCID_PreAmpMedusaHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaHoligramDisable_RSP_T;     /* '<S1769>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaOverheadHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaOverheadHoligramDisable
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaOverheadHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaOverheadHoligramDisable.
typedef struct
{
    uint32_t msgId;          // always RTCID_PreAmpMedusaOverheadHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaOverheadHoligramDisable_RSP_T;/* '<S1781>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaPostHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaPostHoligramDisable
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaPostHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaPostHoligramDisable.
typedef struct
{
    uint32_t msgId;              // always RTCID_PreAmpMedusaPostHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaPostHoligramDisable_RSP_T; /* '<S1800>/PackRSP' */

/// Response Payload Definition for PreAmpFadeControlSet
/// ----------------------------
/// @ingroup PreAmpFadeControlSet
/// This page describes the payload format for Response messages
/// used by the PreAmpFadeControlSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpFadeControlSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpFadeControlSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpFadeControlSet_RSP_T;            /* '<S1842>/PackRSP' */

/// Response Payload Definition for PreAmpMuteControlSet
/// ----------------------------
/// @ingroup PreAmpMuteControlSet
/// This page describes the payload format for Response messages
/// used by the PreAmpMuteControlSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMuteControlSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpMuteControlSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
PreAmpMuteControlSet_RSP_T;            /* '<S1867>/PackRSP' */

/// Response Payload Definition for PreAmpResetSleepingBeauty
/// ----------------------------
/// @ingroup PreAmpResetSleepingBeauty
/// This page describes the payload format for Response messages
/// used by the PreAmpResetSleepingBeauty RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpResetSleepingBeauty.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpResetSleepingBeauty
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpResetSleepingBeauty_RSP_T;       /* '<S1899>/PackRSP' */

/// Response Payload Definition for PreAmpSleepingBeautyDRSet
/// ----------------------------
/// @ingroup PreAmpSleepingBeautyDRSet
/// This page describes the payload format for Response messages
/// used by the PreAmpSleepingBeautyDRSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpSleepingBeautyDRSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpSleepingBeautyDRSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpSleepingBeautyDRSet_RSP_T;       /* '<S1919>/PackRSP' */

/// Response Payload Definition for PreAmpSleepingBeautyFRSet
/// ----------------------------
/// @ingroup PreAmpSleepingBeautyFRSet
/// This page describes the payload format for Response messages
/// used by the PreAmpSleepingBeautyFRSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpSleepingBeautyFRSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpSleepingBeautyFRSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpSleepingBeautyFRSet_RSP_T;       /* '<S1939>/PackRSP' */

/// Response Payload Definition for PreAmpBassSet
/// ----------------------------
/// @ingroup PreAmpBassSet
/// This page describes the payload format for Response messages
/// used by the PreAmpBassSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpBassSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpBassSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpBassSet_RSP_T;                   /* '<S1995>/PackRSP' */

/// Response Payload Definition for PreAmpMidrangeCfgSet
/// ----------------------------
/// @ingroup PreAmpMidrangeCfgSet
/// This page describes the payload format for Response messages
/// used by the PreAmpMidrangeCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMidrangeCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpMidrangeCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpMidrangeCfgSet_RSP_T;            /* '<S2013>/PackRSP' */

/// Response Payload Definition for PreAmpTrebleSet
/// ----------------------------
/// @ingroup PreAmpTrebleSet
/// This page describes the payload format for Response messages
/// used by the PreAmpTrebleSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpTrebleSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpTrebleSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpTrebleSet_RSP_T;                 /* '<S2049>/PackRSP' */

/// Response Payload Definition for TestRouterASDRouterAuxEnable
/// ----------------------------
/// @ingroup TestRouterASDRouterAuxEnable
/// This page describes the payload format for Response messages
/// used by the TestRouterASDRouterAuxEnable RTC mechanism. For a more general
/// description of how this message works, see @ref TestRouterASDRouterAuxEnable.
typedef struct
{
    uint32_t msgId;                 // always RTCID_TestRouterASDRouterAuxEnable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
TestRouterASDRouterAuxEnable_RSP_T;    /* '<S2071>/PackRSP' */

/// Response Payload Definition for TestRouterASDRouterAuxGain
/// ----------------------------
/// @ingroup TestRouterASDRouterAuxGain
/// This page describes the payload format for Response messages
/// used by the TestRouterASDRouterAuxGain RTC mechanism. For a more general
/// description of how this message works, see @ref TestRouterASDRouterAuxGain.
typedef struct
{
    uint32_t msgId;                   // always RTCID_TestRouterASDRouterAuxGain
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
TestRouterASDRouterAuxGain_RSP_T;      /* '<S2077>/PackRSP' */

/// Response Payload Definition for TestRouterASDRouterAuxSelect
/// ----------------------------
/// @ingroup TestRouterASDRouterAuxSelect
/// This page describes the payload format for Response messages
/// used by the TestRouterASDRouterAuxSelect RTC mechanism. For a more general
/// description of how this message works, see @ref TestRouterASDRouterAuxSelect.
typedef struct
{
    uint32_t msgId;                 // always RTCID_TestRouterASDRouterAuxSelect
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
TestRouterASDRouterAuxSelect_RSP_T;    /* '<S2083>/PackRSP' */

/// Response Payload Definition for TestRouterASDRouterMainGain
/// ----------------------------
/// @ingroup TestRouterASDRouterMainGain
/// This page describes the payload format for Response messages
/// used by the TestRouterASDRouterMainGain RTC mechanism. For a more general
/// description of how this message works, see @ref TestRouterASDRouterMainGain.
typedef struct
{
    uint32_t msgId;                  // always RTCID_TestRouterASDRouterMainGain
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
TestRouterASDRouterMainGain_RSP_T;     /* '<S2095>/PackRSP' */

/// Response Payload Definition for TestRouterASDRouterMainSelect
/// ----------------------------
/// @ingroup TestRouterASDRouterMainSelect
/// This page describes the payload format for Response messages
/// used by the TestRouterASDRouterMainSelect RTC mechanism. For a more general
/// description of how this message works, see @ref TestRouterASDRouterMainSelect.
typedef struct
{
    uint32_t msgId;                // always RTCID_TestRouterASDRouterMainSelect
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
TestRouterASDRouterMainSelect_RSP_T;   /* '<S2101>/PackRSP' */

// ------------------------------------------------------------
// RTC NOTIFICATION PAYLOAD DEFINITIONS
// ------------------------------------------------------------

// InputSelectRouterCfgSet does not define a Notification payload.

// NearDr_RouterSet does not define a Notification payload.

// NearFr_RouterSet does not define a Notification payload.

// NearChannel2_Location does not define a Notification payload.

// NearChannel1_Location does not define a Notification payload.

// NearChannel4_Location does not define a Notification payload.

// NearChannel3_Location does not define a Notification payload.

// NearAnnNoiseLevel does not define a Notification payload.

// NearAnn1Ann1NonEntGainCalcvncEnableRTC does not define a Notification payload.

/// Notification Payload Definition for NearAnn1Ann1MuteSet
/// ----------------------------
/// @ingroup NearAnn1Ann1MuteSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn1Ann1MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1Ann1MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn1Ann1MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
NearAnn1Ann1MuteSet_NTF_T;             /* '<S273>/PackNTF' */

/// Notification Payload Definition for NearAnn1Ann1VolumeSet
/// ----------------------------
/// @ingroup NearAnn1Ann1VolumeSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn1Ann1VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1Ann1VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn1Ann1VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn1Ann1VolumeSet_NTF_T;           /* '<S301>/PackNTF' */

// NearAnn2Ann2NonEntGainCalcvncEnableRTC does not define a Notification payload.

/// Notification Payload Definition for NearAnn2Ann2MuteSet
/// ----------------------------
/// @ingroup NearAnn2Ann2MuteSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn2Ann2MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2Ann2MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn2Ann2MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
NearAnn2Ann2MuteSet_NTF_T;             /* '<S353>/PackNTF' */

/// Notification Payload Definition for NearAnn2Ann2VolumeSet
/// ----------------------------
/// @ingroup NearAnn2Ann2VolumeSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn2Ann2VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2Ann2VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn2Ann2VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn2Ann2VolumeSet_NTF_T;           /* '<S381>/PackNTF' */

// NearAnn3Ann3NonEntGainCalcvncEnableRTC does not define a Notification payload.

/// Notification Payload Definition for NearAnn3Ann3MuteSet
/// ----------------------------
/// @ingroup NearAnn3Ann3MuteSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn3Ann3MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3Ann3MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn3Ann3MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
NearAnn3Ann3MuteSet_NTF_T;             /* '<S433>/PackNTF' */

/// Notification Payload Definition for NearAnn3Ann3VolumeSet
/// ----------------------------
/// @ingroup NearAnn3Ann3VolumeSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn3Ann3VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3Ann3VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn3Ann3VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn3Ann3VolumeSet_NTF_T;           /* '<S461>/PackNTF' */

// NearAnn4Ann4NonEntGainCalcvncEnableRTC does not define a Notification payload.

/// Notification Payload Definition for NearAnn4Ann4MuteSet
/// ----------------------------
/// @ingroup NearAnn4Ann4MuteSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn4Ann4MuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4Ann4MuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn4Ann4MuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
NearAnn4Ann4MuteSet_NTF_T;             /* '<S513>/PackNTF' */

/// Notification Payload Definition for NearAnn4Ann4VolumeSet
/// ----------------------------
/// @ingroup NearAnn4Ann4VolumeSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn4Ann4VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4Ann4VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn4Ann4VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn4Ann4VolumeSet_NTF_T;           /* '<S541>/PackNTF' */

// NearEnable does not define a Notification payload.

/// Notification Payload Definition for PostProcessEnergyLimiterConfigEnableLimiter
/// ----------------------------
/// @ingroup PostProcessEnergyLimiterConfigEnableLimiter
/// This page describes the payload format for Notification messages
/// used by the PostProcessEnergyLimiterConfigEnableLimiter RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessEnergyLimiterConfigEnableLimiter.
typedef struct
{
    uint32_t msgId;  // always RTCID_PostProcessEnergyLimiterConfigEnableLimiter
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t ELNotification;           // startChar:8 charSize:4
}
PostProcessEnergyLimiterConfigEnableLimiter_NTF_T;/* '<S593>/PackNTF' */

/// Notification Payload Definition for PostProcessMuteCfgSet
/// ----------------------------
/// @ingroup PostProcessMuteCfgSet
/// This page describes the payload format for Notification messages
/// used by the PostProcessMuteCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PostProcessMuteCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PostProcessMuteCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
PostProcessMuteCfgSet_NTF_T;           /* '<S608>/PackNTF' */

// PostProcessPostLimiterEqShutdown does not define a Notification payload.

// PostProcessLimiterSoftclipParamSet does not define a Notification payload.

// PostProcessLimiterSoftclipDisable does not define a Notification payload.

// PreAmpAudiopilotsetHVACSetting does not define a Notification payload.

// PreAmpAudiopilotmuteReference does not define a Notification payload.

// PreAmpAudiopilotsetSpeedIn does not define a Notification payload.

// PreAmpAudiopilotgetBassBoost does not define a Notification payload.

// PreAmpAudiopilotgetMidBoost does not define a Notification payload.

// PreAmpAudiopilotgetTrebleBoost does not define a Notification payload.

// PreAmpAudiopilotsetDynEQBoostOverRide does not define a Notification payload.

// PreAmpAudiopilotsetVNCBassBoostOverRide does not define a Notification payload.

// PreAmpAudiopilotsetVNCMidBoostOverRide does not define a Notification payload.

// PreAmpAudiopilotsetVNCTrebleBoostOverRide does not define a Notification payload.

// PreAmpAudiopilotsetLoudSignalOverRide does not define a Notification payload.

// PreAmpAudiopilotsetVNCSignalOverRide does not define a Notification payload.

// PreAmpAudiopilotgetAverageAudioPilotBoostLevel does not define a Notification payload.

// PreAmpAudiopilotsetHFNoiseOverRide does not define a Notification payload.

// PreAmpAudiopilotsetLFNoiseOverRide does not define a Notification payload.

// PreAmpAudiopilotsetRatioOverRide does not define a Notification payload.

// PreAmpAudiopilotsetWBNoiseOverRide does not define a Notification payload.

// PreAmpAudiopilotsetDyneqOn does not define a Notification payload.

// PreAmpAudiopilotsetAudioPilotOn does not define a Notification payload.

/// Notification Payload Definition for PreAmpVolumeCfgSet
/// ----------------------------
/// @ingroup PreAmpVolumeCfgSet
/// This page describes the payload format for Notification messages
/// used by the PreAmpVolumeCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpVolumeCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpVolumeCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpVolumeCfgSet_NTF_T;              /* '<S1122>/PackNTF' */

/// Notification Payload Definition for PreAmpBalance_CfgSet
/// ----------------------------
/// @ingroup PreAmpBalance_CfgSet
/// This page describes the payload format for Notification messages
/// used by the PreAmpBalance_CfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpBalance_CfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpBalance_CfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpBalance_CfgSet_NTF_T;            /* '<S1158>/PackNTF' */

/// Notification Payload Definition for PreAmpMedusaSurroundLevelControlIndex
/// ----------------------------
/// @ingroup PreAmpMedusaSurroundLevelControlIndex
/// This page describes the payload format for Notification messages
/// used by the PreAmpMedusaSurroundLevelControlIndex RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSurroundLevelControlIndex.
typedef struct
{
    uint32_t msgId;        // always RTCID_PreAmpMedusaSurroundLevelControlIndex
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t surround_index;           // startChar:8 charSize:4
}
PreAmpMedusaSurroundLevelControlIndex_NTF_T;/* '<S1630>/PackNTF' */

// PreAmpMedusaSurroundLevelControlReset does not define a Notification payload.

// PreAmpMedusaPostUpmixDisable does not define a Notification payload.

// PreAmpMedusaHoligramDisable does not define a Notification payload.

// PreAmpMedusaOverheadHoligramDisable does not define a Notification payload.

// PreAmpMedusaPostHoligramDisable does not define a Notification payload.

/// Notification Payload Definition for PreAmpFadeControlSet
/// ----------------------------
/// @ingroup PreAmpFadeControlSet
/// This page describes the payload format for Notification messages
/// used by the PreAmpFadeControlSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpFadeControlSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpFadeControlSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpFadeControlSet_NTF_T;            /* '<S1837>/PackNTF' */

/// Notification Payload Definition for PreAmpMuteControlSet
/// ----------------------------
/// @ingroup PreAmpMuteControlSet
/// This page describes the payload format for Notification messages
/// used by the PreAmpMuteControlSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMuteControlSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpMuteControlSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
PreAmpMuteControlSet_NTF_T;            /* '<S1862>/PackNTF' */

// PreAmpResetSleepingBeauty does not define a Notification payload.

/// Notification Payload Definition for PreAmpSleepingBeautyDRSet
/// ----------------------------
/// @ingroup PreAmpSleepingBeautyDRSet
/// This page describes the payload format for Notification messages
/// used by the PreAmpSleepingBeautyDRSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpSleepingBeautyDRSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpSleepingBeautyDRSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpSleepingBeautyDRSet_NTF_T;       /* '<S1914>/PackNTF' */

/// Notification Payload Definition for PreAmpSleepingBeautyFRSet
/// ----------------------------
/// @ingroup PreAmpSleepingBeautyFRSet
/// This page describes the payload format for Notification messages
/// used by the PreAmpSleepingBeautyFRSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpSleepingBeautyFRSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpSleepingBeautyFRSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpSleepingBeautyFRSet_NTF_T;       /* '<S1934>/PackNTF' */

/// Notification Payload Definition for PreAmpBassSet
/// ----------------------------
/// @ingroup PreAmpBassSet
/// This page describes the payload format for Notification messages
/// used by the PreAmpBassSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpBassSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpBassSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpBassSet_NTF_T;                   /* '<S1990>/PackNTF' */

/// Notification Payload Definition for PreAmpMidrangeCfgSet
/// ----------------------------
/// @ingroup PreAmpMidrangeCfgSet
/// This page describes the payload format for Notification messages
/// used by the PreAmpMidrangeCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMidrangeCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpMidrangeCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpMidrangeCfgSet_NTF_T;            /* '<S2008>/PackNTF' */

/// Notification Payload Definition for PreAmpTrebleSet
/// ----------------------------
/// @ingroup PreAmpTrebleSet
/// This page describes the payload format for Notification messages
/// used by the PreAmpTrebleSet RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpTrebleSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_PreAmpTrebleSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
PreAmpTrebleSet_NTF_T;                 /* '<S2044>/PackNTF' */

// TestRouterASDRouterAuxEnable does not define a Notification payload.

// TestRouterASDRouterAuxGain does not define a Notification payload.

// TestRouterASDRouterAuxSelect does not define a Notification payload.

// TestRouterASDRouterMainGain does not define a Notification payload.

// TestRouterASDRouterMainSelect does not define a Notification payload.
#endif                                 /* RTW_HEADER_Model_Target_RTC_IDs_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
