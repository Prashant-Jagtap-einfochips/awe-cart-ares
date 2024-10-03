/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: Model_RTC_IDs.h
 *
 * Combined RTC message ID and payload definitions from all cores
 *
 */

#ifndef MODEL_RTC_IDS_H_
#define MODEL_RTC_IDS_H_
#include <stdint.h>
#include "ASDID.h"

// ------------------------------------------------------------
// CORE ENDIAN SETTINGS
// ------------------------------------------------------------
#define ENDIANNESS_CORE_1_1            LittleEndian

#define ENDIANNESS_CORE_1_2            LittleEndian

#define ENDIANNESS_CORE_1_3            LittleEndian

#define ENDIANNESS_CORE_1_4            LittleEndian

#define ENDIANNESS_CORE_1_5            LittleEndian

#define ENDIANNESS_CORE_1_6            LittleEndian

#define ENDIANNESS_CORE_1_7            LittleEndian

#define ENDIANNESS_CORE_1_8            LittleEndian

#define ENDIANNESS_CORE_1_9            LittleEndian

#define ENDIANNESS_CORE_1_10           LittleEndian


// ------------------------------------------------------------
// RTC MESSAGE IDS
// ------------------------------------------------------------
#define RTCID_ArmPreAmpBassSet         (0x10400000U)             // REQ, RSP, NTF '<S28>/RTC Request Response Trigger'
#define RTCID_ArmPreAmpMidrangeCfgSet  (0x10400001U)             // REQ, RSP, NTF '<S52>/RTC Request Response Trigger'
#define RTCID_ArmPreAmpTrebleSet       (0x10400002U)             // REQ, RSP, NTF '<S76>/RTC Request Response Trigger'
#define RTCID_ArmPreAmpVolumeRefCfgSet (0x10400003U)             // REQ, RSP, NTF '<S101>/RTC Request Response Trigger'

#define RTCID_NearFr_Channel1_Virtualization (0x10110017U)       // REQ, RSP '<S134>/RTC Request Response Trigger'
#define RTCID_NearFr_Channel2_Virtualization (0x10110018U)       // REQ, RSP '<S133>/RTC Request Response Trigger'
#define RTCID_NearFr_Channel3_Virtualization (0x10110019U)       // REQ, RSP '<S135>/RTC Request Response Trigger'
#define RTCID_NearFr_Channel4_Virtualization (0x1011001aU)       // REQ, RSP '<S136>/RTC Request Response Trigger'
#define RTCID_NearFr_RouterSet         (0x1011001bU)             // REQ, RSP '<S120>/RTC Request Response Trigger'
#define RTCID_NearAnn1VolumeSet        (0x10100002U)             // REQ, RSP, NTF '<S335>/RTC Request Response Trigger'
#define RTCID_NearAnn2VolumeSet        (0x10100005U)             // REQ, RSP, NTF '<S422>/RTC Request Response Trigger'
#define RTCID_NearAnn3VolumeSet        (0x10100008U)             // REQ, RSP, NTF '<S509>/RTC Request Response Trigger'
#define RTCID_NearAnn4VolumeSet        (0x1010000bU)             // REQ, RSP, NTF '<S596>/RTC Request Response Trigger'
#define RTCID_NearPhoneZones_Enable    (0x1011001cU)             // REQ, RSP '<S22>/RTC Request Response Trigger'
#define RTCID_NearAnn1NonEntGainCalcvncEnableRTC (0x10110001U)   // REQ, RSP '<S308>/RTC Request Response Trigger'
#define RTCID_NearAnn1Ann1MuteSet      (0x10100000U)             // REQ, RSP, NTF '<S285>/RTC Request Response Trigger'
#define RTCID_NearAnn2NonEntGainCalcvncEnableRTC (0x10110004U)   // REQ, RSP '<S395>/RTC Request Response Trigger'
#define RTCID_NearAnn2Ann2MuteSet      (0x10100003U)             // REQ, RSP, NTF '<S372>/RTC Request Response Trigger'
#define RTCID_NearAnn3NonEntGainCalcvncEnableRTC (0x10110007U)   // REQ, RSP '<S482>/RTC Request Response Trigger'
#define RTCID_NearAnn3Ann3MuteSet      (0x10100006U)             // REQ, RSP, NTF '<S459>/RTC Request Response Trigger'
#define RTCID_NearAnn4NonEntGainCalcvncEnableRTC (0x1011000aU)   // REQ, RSP '<S569>/RTC Request Response Trigger'
#define RTCID_NearAnn4Ann4MuteSet      (0x10100009U)             // REQ, RSP, NTF '<S546>/RTC Request Response Trigger'
#define RTCID_NearAnnNoiseLevel        (0x1012000cU)             // REQ *only* '<S219>/RTC Request Response Trigger'
#define RTCID_NearChannel1_Location    (0x1011000dU)             // REQ, RSP '<S171>/RTC Request Response Trigger'
#define RTCID_NearChannel2_Location    (0x1011000eU)             // REQ, RSP '<S170>/RTC Request Response Trigger'
#define RTCID_NearChannel3_Location    (0x1011000fU)             // REQ, RSP '<S173>/RTC Request Response Trigger'
#define RTCID_NearChannel4_Location    (0x10110010U)             // REQ, RSP '<S172>/RTC Request Response Trigger'
#define RTCID_NearDr_Channel1_Virtualization (0x10110011U)       // REQ, RSP '<S65>/RTC Request Response Trigger'
#define RTCID_NearDr_Channel2_Virtualization (0x10110012U)       // REQ, RSP '<S64>/RTC Request Response Trigger'
#define RTCID_NearDr_Channel3_Virtualization (0x10110013U)       // REQ, RSP '<S66>/RTC Request Response Trigger'
#define RTCID_NearDr_Channel4_Virtualization (0x10110014U)       // REQ, RSP '<S67>/RTC Request Response Trigger'
#define RTCID_NearDr_RouterSet         (0x10110015U)             // REQ, RSP '<S51>/RTC Request Response Trigger'
#define RTCID_NearEnable               (0x10110016U)             // REQ, RSP '<S15>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotmuteReference (0x14210004U)        // REQ, RSP '<S652>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetSpeedIn (0x1422000dU)           // REQ *only* '<S653>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetHVACSetting (0x14220009U)       // REQ *only* '<S656>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetDynEQBoostOverRide (0x14220006U) // REQ *only* '<S750>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetVNCTrebleBoostOverRide (0x14220011U) // REQ *only* '<S753>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetLoudSignalOverRide (0x1422000bU) // REQ *only* '<S779>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetVNCSignalOverRide (0x14220010U) // REQ *only* '<S781>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetLFNoiseOverRide (0x1422000aU)   // REQ *only* '<S801>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetWBNoiseOverRide (0x14220012U)   // REQ *only* '<S804>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetHFNoiseOverRide (0x14220008U)   // REQ *only* '<S800>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetRatioOverRide (0x1422000cU)     // REQ *only* '<S803>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetDyneqOn (0x14210007U)           // REQ, RSP '<S840>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetAudioPilotOn (0x14210005U)      // REQ, RSP '<S841>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetVNCBassBoostOverRide (0x1422000eU) // REQ *only* '<S751>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotgetBassBoost (0x14210001U)         // REQ, RSP '<S677>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotsetVNCMidBoostOverRide (0x1422000fU) // REQ *only* '<S752>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotgetMidBoost (0x14210002U)          // REQ, RSP '<S678>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotgetTrebleBoost (0x14210003U)       // REQ, RSP '<S679>/RTC Request Response Trigger'
#define RTCID_PreAmpAudiopilotgetAverageAudioPilotBoostLevel (0x14210000U) // REQ, RSP '<S715>/RTC Request Response Trigger'
#define RTCID_PreAmpVolumeCfgSet       (0x14200013U)             // REQ, RSP, NTF '<S1092>/RTC Request Response Trigger'

#define RTCID_PreAmpBalance_CfgSet     (0x1b200000U)             // REQ, RSP, NTF '<S30>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaSettingsFadeControl (0x16200000U)      // REQ, RSP, NTF '<S513>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaSettingsSurroundLevelControlIndex (0x16200002U) // REQ, RSP, NTF '<S522>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaSettingsPostUpmixDisable (0x16210001U) // REQ, RSP '<S294>/RTC Request Response Trigger'

#define RTCID_PreAmpMedusaSettingsHoligramDisable (0x17210000U)  // REQ, RSP '<S39>/RTC Request Response Trigger'

#define RTCID_PreAmpMedusaSettingsOverheadHoligramDisable (0x18210000U) // REQ, RSP '<S41>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable (0x1e210000U) // REQ, RSP '<S72>/RTC Request Response Trigger'

#define RTCID_PreAmpMuteControlSet     (0x19200001U)             // REQ, RSP, NTF '<S97>/RTC Request Response Trigger'
#define RTCID_PreAmpFadeControlSet     (0x19200000U)             // REQ, RSP, NTF '<S55>/RTC Request Response Trigger'
#define RTCID_PreAmpSleepingBeautyDRSet (0x19200003U)            // REQ, RSP, NTF '<S140>/RTC Request Response Trigger'
#define RTCID_PreAmpSleepingBeautyFRSet (0x19200004U)            // REQ, RSP, NTF '<S166>/RTC Request Response Trigger'
#define RTCID_PreAmpResetSleepingBeauty (0x19210002U)            // REQ, RSP '<S49>/RTC Request Response Trigger'

#define RTCID_PostProcessLimiterSoftclipParamSet (0x10310002U)   // REQ, RSP '<S18>/RTC Request Response Trigger'
#define RTCID_PostProcessLimiterSoftclipDisable (0x10310001U)    // REQ, RSP '<S19>/RTC Request Response Trigger'
#define RTCID_PostProcessEnergyLimiterConfigEnableLimiter (0x10300000U) // REQ, RSP, NTF '<S24>/RTC Request Response Trigger'
#define RTCID_PostProcessMuteCfgSet    (0x10300003U)             // REQ, RSP, NTF '<S54>/RTC Request Response Trigger'
#define RTCID_PostProcessPostLimiterEqShutdown (0x10310004U)     // REQ, RSP '<S77>/RTC Request Response Trigger'
#define RTCID_PreAmpMedusaPostHoligramPostHoligramDisable (0x11210000U) // REQ, RSP '<S130>/RTC Request Response Trigger'

#define RTCID_InputSelectRouterCfgSet  (0x10010000U)             // REQ, RSP '<S18>/RTC Request Response Trigger'

#define RTCID_TestRouterASDRouterMainSelect (0x10510004U)        // REQ, RSP '<S47>/RTC Request Response Trigger'
#define RTCID_TestRouterASDRouterMainGain (0x10510003U)          // REQ, RSP '<S46>/RTC Request Response Trigger'
#define RTCID_TestRouterASDRouterAuxEnable (0x10510000U)         // REQ, RSP '<S42>/RTC Request Response Trigger'
#define RTCID_TestRouterASDRouterAuxSelect (0x10510002U)         // REQ, RSP '<S44>/RTC Request Response Trigger'
#define RTCID_TestRouterASDRouterAuxGain (0x10510001U)           // REQ, RSP '<S43>/RTC Request Response Trigger'

#define RTCID_MicInRMSOutputRMS        (0x10610001U)             // REQ, RSP '<S25>/RTC Request Response Trigger'
#define RTCID_MicInRMSEnable           (0x10610000U)             // REQ, RSP '<S18>/RTC Request Response Trigger'


// ------------------------------------------------------------
// RTC REQUEST PAYLOAD DEFINITIONS
// ------------------------------------------------------------

/// Request Payload Definition for ArmPreAmpBassSet
/// ----------------------------
/// @ingroup ArmPreAmpBassSet
/// This page describes the payload format for Request messages
/// used by the ArmPreAmpBassSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpBassSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpBassSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
ArmPreAmpBassSet_REQ_T;                /* '<S44>/UnpackREQ' */

/// Request Payload Definition for ArmPreAmpMidrangeCfgSet
/// ----------------------------
/// @ingroup ArmPreAmpMidrangeCfgSet
/// This page describes the payload format for Request messages
/// used by the ArmPreAmpMidrangeCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpMidrangeCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpMidrangeCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
ArmPreAmpMidrangeCfgSet_REQ_T;         /* '<S68>/UnpackREQ' */

/// Request Payload Definition for ArmPreAmpTrebleSet
/// ----------------------------
/// @ingroup ArmPreAmpTrebleSet
/// This page describes the payload format for Request messages
/// used by the ArmPreAmpTrebleSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpTrebleSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpTrebleSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
ArmPreAmpTrebleSet_REQ_T;              /* '<S92>/UnpackREQ' */

/// Request Payload Definition for ArmPreAmpVolumeRefCfgSet
/// ----------------------------
/// @ingroup ArmPreAmpVolumeRefCfgSet
/// This page describes the payload format for Request messages
/// used by the ArmPreAmpVolumeRefCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpVolumeRefCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpVolumeRefCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
    int32_t gain_offset;               // startChar:16 charSize:4
}
ArmPreAmpVolumeRefCfgSet_REQ_T;        /* '<S113>/UnpackREQ' */

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
    int32_t routerMap[17];             // startChar:4 charSize:68
}
NearDr_RouterSet_REQ_T;                /* '<S57>/UnpackREQ' */

#define NearDr_RouterSet_REQ_routerMap_SIZE 17                   /* '<S57>/UnpackREQ' */

/// Request Payload Definition for NearDr_Channel2_Virtualization
/// ----------------------------
/// @ingroup NearDr_Channel2_Virtualization
/// This page describes the payload format for Request messages
/// used by the NearDr_Channel2_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearDr_Channel2_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearDr_Channel2_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    float Enable;                      // startChar:4 charSize:4
}
NearDr_Channel2_Virtualization_REQ_T;  /* '<S72>/UnpackREQ' */

/// Request Payload Definition for NearDr_Channel1_Virtualization
/// ----------------------------
/// @ingroup NearDr_Channel1_Virtualization
/// This page describes the payload format for Request messages
/// used by the NearDr_Channel1_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearDr_Channel1_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearDr_Channel1_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    float Enable;                      // startChar:4 charSize:4
}
NearDr_Channel1_Virtualization_REQ_T;  /* '<S78>/UnpackREQ' */

/// Request Payload Definition for NearDr_Channel3_Virtualization
/// ----------------------------
/// @ingroup NearDr_Channel3_Virtualization
/// This page describes the payload format for Request messages
/// used by the NearDr_Channel3_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearDr_Channel3_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearDr_Channel3_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    float Enable;                      // startChar:4 charSize:4
}
NearDr_Channel3_Virtualization_REQ_T;  /* '<S84>/UnpackREQ' */

/// Request Payload Definition for NearDr_Channel4_Virtualization
/// ----------------------------
/// @ingroup NearDr_Channel4_Virtualization
/// This page describes the payload format for Request messages
/// used by the NearDr_Channel4_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearDr_Channel4_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearDr_Channel4_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    float Enable;                      // startChar:4 charSize:4
}
NearDr_Channel4_Virtualization_REQ_T;  /* '<S90>/UnpackREQ' */

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
    int32_t routerMap[17];             // startChar:4 charSize:68
}
NearFr_RouterSet_REQ_T;                /* '<S126>/UnpackREQ' */

#define NearFr_RouterSet_REQ_routerMap_SIZE 17                   /* '<S126>/UnpackREQ' */

/// Request Payload Definition for NearFr_Channel2_Virtualization
/// ----------------------------
/// @ingroup NearFr_Channel2_Virtualization
/// This page describes the payload format for Request messages
/// used by the NearFr_Channel2_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearFr_Channel2_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearFr_Channel2_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    float Enable;                      // startChar:4 charSize:4
}
NearFr_Channel2_Virtualization_REQ_T;  /* '<S141>/UnpackREQ' */

/// Request Payload Definition for NearFr_Channel1_Virtualization
/// ----------------------------
/// @ingroup NearFr_Channel1_Virtualization
/// This page describes the payload format for Request messages
/// used by the NearFr_Channel1_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearFr_Channel1_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearFr_Channel1_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    float Enable;                      // startChar:4 charSize:4
}
NearFr_Channel1_Virtualization_REQ_T;  /* '<S147>/UnpackREQ' */

/// Request Payload Definition for NearFr_Channel3_Virtualization
/// ----------------------------
/// @ingroup NearFr_Channel3_Virtualization
/// This page describes the payload format for Request messages
/// used by the NearFr_Channel3_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearFr_Channel3_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearFr_Channel3_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    float Enable;                      // startChar:4 charSize:4
}
NearFr_Channel3_Virtualization_REQ_T;  /* '<S153>/UnpackREQ' */

/// Request Payload Definition for NearFr_Channel4_Virtualization
/// ----------------------------
/// @ingroup NearFr_Channel4_Virtualization
/// This page describes the payload format for Request messages
/// used by the NearFr_Channel4_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearFr_Channel4_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearFr_Channel4_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    float Enable;                      // startChar:4 charSize:4
}
NearFr_Channel4_Virtualization_REQ_T;  /* '<S159>/UnpackREQ' */

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
NearChannel2_Location_REQ_T;           /* '<S178>/UnpackREQ' */

#define NearChannel2_Location_REQ_loc_SIZE 2                     /* '<S178>/UnpackREQ' */

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
NearChannel1_Location_REQ_T;           /* '<S184>/UnpackREQ' */

#define NearChannel1_Location_REQ_loc_SIZE 2                     /* '<S184>/UnpackREQ' */

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
NearChannel4_Location_REQ_T;           /* '<S190>/UnpackREQ' */

#define NearChannel4_Location_REQ_loc_SIZE 2                     /* '<S190>/UnpackREQ' */

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
NearChannel3_Location_REQ_T;           /* '<S196>/UnpackREQ' */

#define NearChannel3_Location_REQ_loc_SIZE 2                     /* '<S196>/UnpackREQ' */

/// Request Payload Definition for NearPhoneZones_Enable
/// ----------------------------
/// @ingroup NearPhoneZones_Enable
/// This page describes the payload format for Request messages
/// used by the NearPhoneZones_Enable RTC mechanism. For a more general
/// description of how this message works, see @ref NearPhoneZones_Enable.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearPhoneZones_Enable
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t Enable;                   // startChar:4 charSize:4
}
NearPhoneZones_Enable_REQ_T;           /* '<S216>/UnpackREQ' */

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
NearAnnNoiseLevel_REQ_T;               /* '<S276>/UnpackREQ' */

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
NearAnn1Ann1MuteSet_REQ_T;             /* '<S293>/UnpackREQ' */

/// Request Payload Definition for NearAnn1NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn1NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Request messages
/// used by the NearAnn1NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;           // always RTCID_NearAnn1NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t vncEnable;                // startChar:4 charSize:4
}
NearAnn1NonEntGainCalcvncEnableRTC_REQ_T;/* '<S325>/UnpackREQ' */

/// Request Payload Definition for NearAnn1VolumeSet
/// ----------------------------
/// @ingroup NearAnn1VolumeSet
/// This page describes the payload format for Request messages
/// used by the NearAnn1VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn1VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t volume_index;             // startChar:4 charSize:4
    uint32_t db_per_second;            // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn1VolumeSet_REQ_T;               /* '<S349>/UnpackREQ' */

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
NearAnn2Ann2MuteSet_REQ_T;             /* '<S380>/UnpackREQ' */

/// Request Payload Definition for NearAnn2NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn2NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Request messages
/// used by the NearAnn2NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;           // always RTCID_NearAnn2NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t vncEnable;                // startChar:4 charSize:4
}
NearAnn2NonEntGainCalcvncEnableRTC_REQ_T;/* '<S412>/UnpackREQ' */

/// Request Payload Definition for NearAnn2VolumeSet
/// ----------------------------
/// @ingroup NearAnn2VolumeSet
/// This page describes the payload format for Request messages
/// used by the NearAnn2VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn2VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t volume_index;             // startChar:4 charSize:4
    uint32_t db_per_second;            // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn2VolumeSet_REQ_T;               /* '<S436>/UnpackREQ' */

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
NearAnn3Ann3MuteSet_REQ_T;             /* '<S467>/UnpackREQ' */

/// Request Payload Definition for NearAnn3NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn3NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Request messages
/// used by the NearAnn3NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;           // always RTCID_NearAnn3NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t vncEnable;                // startChar:4 charSize:4
}
NearAnn3NonEntGainCalcvncEnableRTC_REQ_T;/* '<S499>/UnpackREQ' */

/// Request Payload Definition for NearAnn3VolumeSet
/// ----------------------------
/// @ingroup NearAnn3VolumeSet
/// This page describes the payload format for Request messages
/// used by the NearAnn3VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn3VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t volume_index;             // startChar:4 charSize:4
    uint32_t db_per_second;            // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn3VolumeSet_REQ_T;               /* '<S523>/UnpackREQ' */

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
NearAnn4Ann4MuteSet_REQ_T;             /* '<S554>/UnpackREQ' */

/// Request Payload Definition for NearAnn4NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn4NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Request messages
/// used by the NearAnn4NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;           // always RTCID_NearAnn4NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t vncEnable;                // startChar:4 charSize:4
}
NearAnn4NonEntGainCalcvncEnableRTC_REQ_T;/* '<S586>/UnpackREQ' */

/// Request Payload Definition for NearAnn4VolumeSet
/// ----------------------------
/// @ingroup NearAnn4VolumeSet
/// This page describes the payload format for Request messages
/// used by the NearAnn4VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn4VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t volume_index;             // startChar:4 charSize:4
    uint32_t db_per_second;            // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
NearAnn4VolumeSet_REQ_T;               /* '<S610>/UnpackREQ' */

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
NearEnable_REQ_T;                      /* '<S630>/UnpackREQ' */

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
PreAmpAudiopilotsetHVACSetting_REQ_T;  /* '<S662>/UnpackREQ' */

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
PreAmpAudiopilotmuteReference_REQ_T;   /* '<S668>/UnpackREQ' */

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
PreAmpAudiopilotsetSpeedIn_REQ_T;      /* '<S674>/UnpackREQ' */

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
PreAmpAudiopilotgetBassBoost_REQ_T;    /* '<S698>/UnpackREQ' */

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
PreAmpAudiopilotgetMidBoost_REQ_T;     /* '<S704>/UnpackREQ' */

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
PreAmpAudiopilotgetTrebleBoost_REQ_T;  /* '<S710>/UnpackREQ' */

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
PreAmpAudiopilotsetDynEQBoostOverRide_REQ_T;/* '<S758>/UnpackREQ' */

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
PreAmpAudiopilotsetVNCBassBoostOverRide_REQ_T;/* '<S764>/UnpackREQ' */

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
PreAmpAudiopilotsetVNCMidBoostOverRide_REQ_T;/* '<S770>/UnpackREQ' */

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
PreAmpAudiopilotsetVNCTrebleBoostOverRide_REQ_T;/* '<S776>/UnpackREQ' */

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
PreAmpAudiopilotsetLoudSignalOverRide_REQ_T;/* '<S786>/UnpackREQ' */

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
PreAmpAudiopilotsetVNCSignalOverRide_REQ_T;/* '<S792>/UnpackREQ' */

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
PreAmpAudiopilotgetAverageAudioPilotBoostLevel_REQ_T;/* '<S798>/UnpackREQ' */

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
PreAmpAudiopilotsetHFNoiseOverRide_REQ_T;/* '<S809>/UnpackREQ' */

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
PreAmpAudiopilotsetLFNoiseOverRide_REQ_T;/* '<S815>/UnpackREQ' */

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
PreAmpAudiopilotsetRatioOverRide_REQ_T;/* '<S821>/UnpackREQ' */

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
PreAmpAudiopilotsetWBNoiseOverRide_REQ_T;/* '<S827>/UnpackREQ' */

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
PreAmpAudiopilotsetDyneqOn_REQ_T;      /* '<S849>/UnpackREQ' */

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
PreAmpAudiopilotsetAudioPilotOn_REQ_T; /* '<S855>/UnpackREQ' */

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
PreAmpVolumeCfgSet_REQ_T;              /* '<S1104>/UnpackREQ' */

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
PreAmpBalance_CfgSet_REQ_T;            /* '<S49>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaSettingsFadeControl
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsFadeControl
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaSettingsFadeControl RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsFadeControl.
typedef struct
{
    uint32_t msgId;              // always RTCID_PreAmpMedusaSettingsFadeControl
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t fade_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpMedusaSettingsFadeControl_REQ_T; /* '<S518>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaSettingsSurroundLevelControlIndex
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsSurroundLevelControlIndex
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaSettingsSurroundLevelControlIndex RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsSurroundLevelControlIndex.
typedef struct
{
    uint32_t msgId;// always RTCID_PreAmpMedusaSettingsSurroundLevelControlIndex
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t surround_index;           // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
PreAmpMedusaSettingsSurroundLevelControlIndex_REQ_T;/* '<S527>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaSettingsPostUpmixDisable
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsPostUpmixDisable
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaSettingsPostUpmixDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsPostUpmixDisable.
typedef struct
{
    uint32_t msgId;         // always RTCID_PreAmpMedusaSettingsPostUpmixDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpMedusaSettingsPostUpmixDisable_REQ_T;/* '<S716>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaSettingsHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsHoligramDisable
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaSettingsHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsHoligramDisable.
typedef struct
{
    uint32_t msgId;          // always RTCID_PreAmpMedusaSettingsHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpMedusaSettingsHoligramDisable_REQ_T;/* '<S77>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaSettingsOverheadHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsOverheadHoligramDisable
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaSettingsOverheadHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsOverheadHoligramDisable.
typedef struct
{
    uint32_t msgId;  // always RTCID_PreAmpMedusaSettingsOverheadHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpMedusaSettingsOverheadHoligramDisable_REQ_T;/* '<S65>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable.
typedef struct
{
    uint32_t msgId;
           // always RTCID_PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable_REQ_T;/* '<S94>/UnpackREQ' */

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
PreAmpFadeControlSet_REQ_T;            /* '<S68>/UnpackREQ' */

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
PreAmpMuteControlSet_REQ_T;            /* '<S105>/UnpackREQ' */

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
PreAmpResetSleepingBeauty_REQ_T;       /* '<S137>/UnpackREQ' */

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
PreAmpSleepingBeautyDRSet_REQ_T;       /* '<S157>/UnpackREQ' */

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
PreAmpSleepingBeautyFRSet_REQ_T;       /* '<S183>/UnpackREQ' */

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
PostProcessEnergyLimiterConfigEnableLimiter_REQ_T;/* '<S47>/UnpackREQ' */

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
PostProcessMuteCfgSet_REQ_T;           /* '<S62>/UnpackREQ' */

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
PostProcessPostLimiterEqShutdown_REQ_T;/* '<S87>/UnpackREQ' */

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
PostProcessLimiterSoftclipParamSet_REQ_T;/* '<S93>/UnpackREQ' */

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
PostProcessLimiterSoftclipDisable_REQ_T;/* '<S99>/UnpackREQ' */

/// Request Payload Definition for PreAmpMedusaPostHoligramPostHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaPostHoligramPostHoligramDisable
/// This page describes the payload format for Request messages
/// used by the PreAmpMedusaPostHoligramPostHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaPostHoligramPostHoligramDisable.
typedef struct
{
    uint32_t msgId;  // always RTCID_PreAmpMedusaPostHoligramPostHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
}
PreAmpMedusaPostHoligramPostHoligramDisable_REQ_T;/* '<S149>/UnpackREQ' */

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
    int32_t routerMap[14];             // startChar:4 charSize:56
}
InputSelectRouterCfgSet_REQ_T;         /* '<S24>/UnpackREQ' */

#define InputSelectRouterCfgSet_REQ_routerMap_SIZE 14            /* '<S24>/UnpackREQ' */

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
TestRouterASDRouterAuxEnable_REQ_T;    /* '<S55>/UnpackREQ' */

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
    float AuxGain[21];                 // startChar:4 charSize:84
}
TestRouterASDRouterAuxGain_REQ_T;      /* '<S61>/UnpackREQ' */

#define TestRouterASDRouterAuxGain_REQ_AuxGain_SIZE 21           /* '<S61>/UnpackREQ' */

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
    int32_t AuxSelect[21];             // startChar:4 charSize:84
}
TestRouterASDRouterAuxSelect_REQ_T;    /* '<S67>/UnpackREQ' */

#define TestRouterASDRouterAuxSelect_REQ_AuxSelect_SIZE 21       /* '<S67>/UnpackREQ' */

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
    float mainGain[21];                // startChar:4 charSize:84
}
TestRouterASDRouterMainGain_REQ_T;     /* '<S79>/UnpackREQ' */

#define TestRouterASDRouterMainGain_REQ_mainGain_SIZE 21         /* '<S79>/UnpackREQ' */

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
    int32_t mainSelect[21];            // startChar:4 charSize:84
}
TestRouterASDRouterMainSelect_REQ_T;   /* '<S85>/UnpackREQ' */

#define TestRouterASDRouterMainSelect_REQ_mainSelect_SIZE 21     /* '<S85>/UnpackREQ' */

/// Request Payload Definition for MicInRMSOutputRMS
/// ----------------------------
/// @ingroup MicInRMSOutputRMS
/// This page describes the payload format for Request messages
/// used by the MicInRMSOutputRMS RTC mechanism. For a more general
/// description of how this message works, see @ref MicInRMSOutputRMS.
typedef struct
{
    uint32_t msgId;                    // always RTCID_MicInRMSOutputRMS
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t outputRMS;                // startChar:4 charSize:4
}
MicInRMSOutputRMS_REQ_T;               /* '<S30>/UnpackREQ' */

/// Request Payload Definition for MicInRMSEnable
/// ----------------------------
/// @ingroup MicInRMSEnable
/// This page describes the payload format for Request messages
/// used by the MicInRMSEnable RTC mechanism. For a more general
/// description of how this message works, see @ref MicInRMSEnable.
typedef struct
{
    uint32_t msgId;                    // always RTCID_MicInRMSEnable
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t enable;                   // startChar:4 charSize:4
}
MicInRMSEnable_REQ_T;                  /* '<S36>/UnpackREQ' */


// ------------------------------------------------------------
// RTC RESPONSE PAYLOAD DEFINITIONS
// ------------------------------------------------------------

/// Response Payload Definition for ArmPreAmpBassSet
/// ----------------------------
/// @ingroup ArmPreAmpBassSet
/// This page describes the payload format for Response messages
/// used by the ArmPreAmpBassSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpBassSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpBassSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
ArmPreAmpBassSet_RSP_T;                /* '<S45>/PackRSP' */

/// Response Payload Definition for ArmPreAmpMidrangeCfgSet
/// ----------------------------
/// @ingroup ArmPreAmpMidrangeCfgSet
/// This page describes the payload format for Response messages
/// used by the ArmPreAmpMidrangeCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpMidrangeCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpMidrangeCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
ArmPreAmpMidrangeCfgSet_RSP_T;         /* '<S69>/PackRSP' */

/// Response Payload Definition for ArmPreAmpTrebleSet
/// ----------------------------
/// @ingroup ArmPreAmpTrebleSet
/// This page describes the payload format for Response messages
/// used by the ArmPreAmpTrebleSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpTrebleSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpTrebleSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
ArmPreAmpTrebleSet_RSP_T;              /* '<S93>/PackRSP' */

/// Response Payload Definition for ArmPreAmpVolumeRefCfgSet
/// ----------------------------
/// @ingroup ArmPreAmpVolumeRefCfgSet
/// This page describes the payload format for Response messages
/// used by the ArmPreAmpVolumeRefCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpVolumeRefCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpVolumeRefCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
ArmPreAmpVolumeRefCfgSet_RSP_T;        /* '<S114>/PackRSP' */

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
    int32_t routerMap[17];             // startChar:8 charSize:68
}
NearDr_RouterSet_RSP_T;                /* '<S58>/PackRSP' */

#define NearDr_RouterSet_RSP_routerMap_SIZE 17                   /* '<S58>/PackRSP' */

/// Response Payload Definition for NearDr_Channel2_Virtualization
/// ----------------------------
/// @ingroup NearDr_Channel2_Virtualization
/// This page describes the payload format for Response messages
/// used by the NearDr_Channel2_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearDr_Channel2_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearDr_Channel2_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearDr_Channel2_Virtualization_RSP_T;  /* '<S73>/PackRSP' */

/// Response Payload Definition for NearDr_Channel1_Virtualization
/// ----------------------------
/// @ingroup NearDr_Channel1_Virtualization
/// This page describes the payload format for Response messages
/// used by the NearDr_Channel1_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearDr_Channel1_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearDr_Channel1_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearDr_Channel1_Virtualization_RSP_T;  /* '<S79>/PackRSP' */

/// Response Payload Definition for NearDr_Channel3_Virtualization
/// ----------------------------
/// @ingroup NearDr_Channel3_Virtualization
/// This page describes the payload format for Response messages
/// used by the NearDr_Channel3_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearDr_Channel3_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearDr_Channel3_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearDr_Channel3_Virtualization_RSP_T;  /* '<S85>/PackRSP' */

/// Response Payload Definition for NearDr_Channel4_Virtualization
/// ----------------------------
/// @ingroup NearDr_Channel4_Virtualization
/// This page describes the payload format for Response messages
/// used by the NearDr_Channel4_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearDr_Channel4_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearDr_Channel4_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearDr_Channel4_Virtualization_RSP_T;  /* '<S91>/PackRSP' */

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
    int32_t routerMap[17];             // startChar:8 charSize:68
}
NearFr_RouterSet_RSP_T;                /* '<S127>/PackRSP' */

#define NearFr_RouterSet_RSP_routerMap_SIZE 17                   /* '<S127>/PackRSP' */

/// Response Payload Definition for NearFr_Channel2_Virtualization
/// ----------------------------
/// @ingroup NearFr_Channel2_Virtualization
/// This page describes the payload format for Response messages
/// used by the NearFr_Channel2_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearFr_Channel2_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearFr_Channel2_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearFr_Channel2_Virtualization_RSP_T;  /* '<S142>/PackRSP' */

/// Response Payload Definition for NearFr_Channel1_Virtualization
/// ----------------------------
/// @ingroup NearFr_Channel1_Virtualization
/// This page describes the payload format for Response messages
/// used by the NearFr_Channel1_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearFr_Channel1_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearFr_Channel1_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearFr_Channel1_Virtualization_RSP_T;  /* '<S148>/PackRSP' */

/// Response Payload Definition for NearFr_Channel3_Virtualization
/// ----------------------------
/// @ingroup NearFr_Channel3_Virtualization
/// This page describes the payload format for Response messages
/// used by the NearFr_Channel3_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearFr_Channel3_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearFr_Channel3_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearFr_Channel3_Virtualization_RSP_T;  /* '<S154>/PackRSP' */

/// Response Payload Definition for NearFr_Channel4_Virtualization
/// ----------------------------
/// @ingroup NearFr_Channel4_Virtualization
/// This page describes the payload format for Response messages
/// used by the NearFr_Channel4_Virtualization RTC mechanism. For a more general
/// description of how this message works, see @ref NearFr_Channel4_Virtualization.
typedef struct
{
    uint32_t msgId;               // always RTCID_NearFr_Channel4_Virtualization
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearFr_Channel4_Virtualization_RSP_T;  /* '<S160>/PackRSP' */

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
NearChannel2_Location_RSP_T;           /* '<S179>/PackRSP' */

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
NearChannel1_Location_RSP_T;           /* '<S185>/PackRSP' */

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
NearChannel4_Location_RSP_T;           /* '<S191>/PackRSP' */

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
NearChannel3_Location_RSP_T;           /* '<S197>/PackRSP' */

/// Response Payload Definition for NearPhoneZones_Enable
/// ----------------------------
/// @ingroup NearPhoneZones_Enable
/// This page describes the payload format for Response messages
/// used by the NearPhoneZones_Enable RTC mechanism. For a more general
/// description of how this message works, see @ref NearPhoneZones_Enable.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearPhoneZones_Enable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
NearPhoneZones_Enable_RSP_T;           /* '<S217>/PackRSP' */

// NearAnnNoiseLevel does not define a Response payload.

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
NearAnn1Ann1MuteSet_RSP_T;             /* '<S294>/PackRSP' */

/// Response Payload Definition for NearAnn1NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn1NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Response messages
/// used by the NearAnn1NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;           // always RTCID_NearAnn1NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t enable;                   // startChar:8 charSize:4
}
NearAnn1NonEntGainCalcvncEnableRTC_RSP_T;/* '<S326>/PackRSP' */

/// Response Payload Definition for NearAnn1VolumeSet
/// ----------------------------
/// @ingroup NearAnn1VolumeSet
/// This page describes the payload format for Response messages
/// used by the NearAnn1VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn1VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn1VolumeSet_RSP_T;               /* '<S350>/PackRSP' */

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
NearAnn2Ann2MuteSet_RSP_T;             /* '<S381>/PackRSP' */

/// Response Payload Definition for NearAnn2NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn2NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Response messages
/// used by the NearAnn2NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;           // always RTCID_NearAnn2NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t enable;                   // startChar:8 charSize:4
}
NearAnn2NonEntGainCalcvncEnableRTC_RSP_T;/* '<S413>/PackRSP' */

/// Response Payload Definition for NearAnn2VolumeSet
/// ----------------------------
/// @ingroup NearAnn2VolumeSet
/// This page describes the payload format for Response messages
/// used by the NearAnn2VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn2VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn2VolumeSet_RSP_T;               /* '<S437>/PackRSP' */

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
NearAnn3Ann3MuteSet_RSP_T;             /* '<S468>/PackRSP' */

/// Response Payload Definition for NearAnn3NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn3NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Response messages
/// used by the NearAnn3NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;           // always RTCID_NearAnn3NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t enable;                   // startChar:8 charSize:4
}
NearAnn3NonEntGainCalcvncEnableRTC_RSP_T;/* '<S500>/PackRSP' */

/// Response Payload Definition for NearAnn3VolumeSet
/// ----------------------------
/// @ingroup NearAnn3VolumeSet
/// This page describes the payload format for Response messages
/// used by the NearAnn3VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn3VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn3VolumeSet_RSP_T;               /* '<S524>/PackRSP' */

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
NearAnn4Ann4MuteSet_RSP_T;             /* '<S555>/PackRSP' */

/// Response Payload Definition for NearAnn4NonEntGainCalcvncEnableRTC
/// ----------------------------
/// @ingroup NearAnn4NonEntGainCalcvncEnableRTC
/// This page describes the payload format for Response messages
/// used by the NearAnn4NonEntGainCalcvncEnableRTC RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4NonEntGainCalcvncEnableRTC.
typedef struct
{
    uint32_t msgId;           // always RTCID_NearAnn4NonEntGainCalcvncEnableRTC
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t enable;                   // startChar:8 charSize:4
}
NearAnn4NonEntGainCalcvncEnableRTC_RSP_T;/* '<S587>/PackRSP' */

/// Response Payload Definition for NearAnn4VolumeSet
/// ----------------------------
/// @ingroup NearAnn4VolumeSet
/// This page describes the payload format for Response messages
/// used by the NearAnn4VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn4VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn4VolumeSet_RSP_T;               /* '<S611>/PackRSP' */

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
NearEnable_RSP_T;                      /* '<S631>/PackRSP' */

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
PreAmpAudiopilotmuteReference_RSP_T;   /* '<S669>/PackRSP' */

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
PreAmpAudiopilotgetBassBoost_RSP_T;    /* '<S699>/PackRSP' */

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
PreAmpAudiopilotgetMidBoost_RSP_T;     /* '<S705>/PackRSP' */

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
PreAmpAudiopilotgetTrebleBoost_RSP_T;  /* '<S711>/PackRSP' */

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
PreAmpAudiopilotgetAverageAudioPilotBoostLevel_RSP_T;/* '<S799>/PackRSP' */

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
PreAmpAudiopilotsetDyneqOn_RSP_T;      /* '<S850>/PackRSP' */

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
PreAmpAudiopilotsetAudioPilotOn_RSP_T; /* '<S856>/PackRSP' */

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
PreAmpVolumeCfgSet_RSP_T;              /* '<S1105>/PackRSP' */

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
PreAmpBalance_CfgSet_RSP_T;            /* '<S50>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaSettingsFadeControl
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsFadeControl
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaSettingsFadeControl RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsFadeControl.
typedef struct
{
    uint32_t msgId;              // always RTCID_PreAmpMedusaSettingsFadeControl
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t fade_index;               // startChar:8 charSize:4
}
PreAmpMedusaSettingsFadeControl_RSP_T; /* '<S519>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaSettingsSurroundLevelControlIndex
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsSurroundLevelControlIndex
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaSettingsSurroundLevelControlIndex RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsSurroundLevelControlIndex.
typedef struct
{
    uint32_t msgId;// always RTCID_PreAmpMedusaSettingsSurroundLevelControlIndex
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t surround_index;           // startChar:8 charSize:4
}
PreAmpMedusaSettingsSurroundLevelControlIndex_RSP_T;/* '<S528>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaSettingsPostUpmixDisable
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsPostUpmixDisable
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaSettingsPostUpmixDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsPostUpmixDisable.
typedef struct
{
    uint32_t msgId;         // always RTCID_PreAmpMedusaSettingsPostUpmixDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaSettingsPostUpmixDisable_RSP_T;/* '<S717>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaSettingsHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsHoligramDisable
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaSettingsHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsHoligramDisable.
typedef struct
{
    uint32_t msgId;          // always RTCID_PreAmpMedusaSettingsHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaSettingsHoligramDisable_RSP_T;/* '<S78>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaSettingsOverheadHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsOverheadHoligramDisable
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaSettingsOverheadHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsOverheadHoligramDisable.
typedef struct
{
    uint32_t msgId;  // always RTCID_PreAmpMedusaSettingsOverheadHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaSettingsOverheadHoligramDisable_RSP_T;/* '<S66>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable.
typedef struct
{
    uint32_t msgId;
           // always RTCID_PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable_RSP_T;/* '<S95>/PackRSP' */

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
PreAmpFadeControlSet_RSP_T;            /* '<S69>/PackRSP' */

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
PreAmpMuteControlSet_RSP_T;            /* '<S106>/PackRSP' */

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
PreAmpResetSleepingBeauty_RSP_T;       /* '<S138>/PackRSP' */

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
PreAmpSleepingBeautyDRSet_RSP_T;       /* '<S158>/PackRSP' */

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
PreAmpSleepingBeautyFRSet_RSP_T;       /* '<S184>/PackRSP' */

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
PostProcessEnergyLimiterConfigEnableLimiter_RSP_T;/* '<S48>/PackRSP' */

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
PostProcessMuteCfgSet_RSP_T;           /* '<S63>/PackRSP' */

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
PostProcessPostLimiterEqShutdown_RSP_T;/* '<S88>/PackRSP' */

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
PostProcessLimiterSoftclipParamSet_RSP_T;/* '<S94>/PackRSP' */

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
PostProcessLimiterSoftclipDisable_RSP_T;/* '<S100>/PackRSP' */

/// Response Payload Definition for PreAmpMedusaPostHoligramPostHoligramDisable
/// ----------------------------
/// @ingroup PreAmpMedusaPostHoligramPostHoligramDisable
/// This page describes the payload format for Response messages
/// used by the PreAmpMedusaPostHoligramPostHoligramDisable RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaPostHoligramPostHoligramDisable.
typedef struct
{
    uint32_t msgId;  // always RTCID_PreAmpMedusaPostHoligramPostHoligramDisable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaPostHoligramPostHoligramDisable_RSP_T;/* '<S150>/PackRSP' */

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
    int32_t routerMap[14];             // startChar:8 charSize:56
}
InputSelectRouterCfgSet_RSP_T;         /* '<S25>/PackRSP' */

#define InputSelectRouterCfgSet_RSP_routerMap_SIZE 14            /* '<S25>/PackRSP' */

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
TestRouterASDRouterAuxEnable_RSP_T;    /* '<S56>/PackRSP' */

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
TestRouterASDRouterAuxGain_RSP_T;      /* '<S62>/PackRSP' */

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
TestRouterASDRouterAuxSelect_RSP_T;    /* '<S68>/PackRSP' */

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
TestRouterASDRouterMainGain_RSP_T;     /* '<S80>/PackRSP' */

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
TestRouterASDRouterMainSelect_RSP_T;   /* '<S86>/PackRSP' */

/// Response Payload Definition for MicInRMSOutputRMS
/// ----------------------------
/// @ingroup MicInRMSOutputRMS
/// This page describes the payload format for Response messages
/// used by the MicInRMSOutputRMS RTC mechanism. For a more general
/// description of how this message works, see @ref MicInRMSOutputRMS.
typedef struct
{
    uint32_t msgId;                    // always RTCID_MicInRMSOutputRMS
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t ResponsePayload;          // startChar:8 charSize:4
}
MicInRMSOutputRMS_RSP_T;               /* '<S31>/PackRSP' */

/// Response Payload Definition for MicInRMSEnable
/// ----------------------------
/// @ingroup MicInRMSEnable
/// This page describes the payload format for Response messages
/// used by the MicInRMSEnable RTC mechanism. For a more general
/// description of how this message works, see @ref MicInRMSEnable.
typedef struct
{
    uint32_t msgId;                    // always RTCID_MicInRMSEnable
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t EnableResponsePayload;    // startChar:8 charSize:4
}
MicInRMSEnable_RSP_T;                  /* '<S37>/PackRSP' */


// ------------------------------------------------------------
// RTC NOTIFICATION PAYLOAD DEFINITIONS
// ------------------------------------------------------------

/// Notification Payload Definition for ArmPreAmpBassSet
/// ----------------------------
/// @ingroup ArmPreAmpBassSet
/// This page describes the payload format for Notification messages
/// used by the ArmPreAmpBassSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpBassSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpBassSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
ArmPreAmpBassSet_NTF_T;                /* '<S40>/PackNTF' */

/// Notification Payload Definition for ArmPreAmpMidrangeCfgSet
/// ----------------------------
/// @ingroup ArmPreAmpMidrangeCfgSet
/// This page describes the payload format for Notification messages
/// used by the ArmPreAmpMidrangeCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpMidrangeCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpMidrangeCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
ArmPreAmpMidrangeCfgSet_NTF_T;         /* '<S64>/PackNTF' */

/// Notification Payload Definition for ArmPreAmpTrebleSet
/// ----------------------------
/// @ingroup ArmPreAmpTrebleSet
/// This page describes the payload format for Notification messages
/// used by the ArmPreAmpTrebleSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpTrebleSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpTrebleSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
ArmPreAmpTrebleSet_NTF_T;              /* '<S88>/PackNTF' */

/// Notification Payload Definition for ArmPreAmpVolumeRefCfgSet
/// ----------------------------
/// @ingroup ArmPreAmpVolumeRefCfgSet
/// This page describes the payload format for Notification messages
/// used by the ArmPreAmpVolumeRefCfgSet RTC mechanism. For a more general
/// description of how this message works, see @ref ArmPreAmpVolumeRefCfgSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_ArmPreAmpVolumeRefCfgSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
ArmPreAmpVolumeRefCfgSet_NTF_T;        /* '<S109>/PackNTF' */

// NearDr_RouterSet does not define a Notification payload.

// NearDr_Channel2_Virtualization does not define a Notification payload.

// NearDr_Channel1_Virtualization does not define a Notification payload.

// NearDr_Channel3_Virtualization does not define a Notification payload.

// NearDr_Channel4_Virtualization does not define a Notification payload.

// NearFr_RouterSet does not define a Notification payload.

// NearFr_Channel2_Virtualization does not define a Notification payload.

// NearFr_Channel1_Virtualization does not define a Notification payload.

// NearFr_Channel3_Virtualization does not define a Notification payload.

// NearFr_Channel4_Virtualization does not define a Notification payload.

// NearChannel2_Location does not define a Notification payload.

// NearChannel1_Location does not define a Notification payload.

// NearChannel4_Location does not define a Notification payload.

// NearChannel3_Location does not define a Notification payload.

// NearPhoneZones_Enable does not define a Notification payload.

// NearAnnNoiseLevel does not define a Notification payload.

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
NearAnn1Ann1MuteSet_NTF_T;             /* '<S289>/PackNTF' */

// NearAnn1NonEntGainCalcvncEnableRTC does not define a Notification payload.

/// Notification Payload Definition for NearAnn1VolumeSet
/// ----------------------------
/// @ingroup NearAnn1VolumeSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn1VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn1VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn1VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn1VolumeSet_NTF_T;               /* '<S345>/PackNTF' */

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
NearAnn2Ann2MuteSet_NTF_T;             /* '<S376>/PackNTF' */

// NearAnn2NonEntGainCalcvncEnableRTC does not define a Notification payload.

/// Notification Payload Definition for NearAnn2VolumeSet
/// ----------------------------
/// @ingroup NearAnn2VolumeSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn2VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn2VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn2VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn2VolumeSet_NTF_T;               /* '<S432>/PackNTF' */

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
NearAnn3Ann3MuteSet_NTF_T;             /* '<S463>/PackNTF' */

// NearAnn3NonEntGainCalcvncEnableRTC does not define a Notification payload.

/// Notification Payload Definition for NearAnn3VolumeSet
/// ----------------------------
/// @ingroup NearAnn3VolumeSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn3VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn3VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn3VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn3VolumeSet_NTF_T;               /* '<S519>/PackNTF' */

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
NearAnn4Ann4MuteSet_NTF_T;             /* '<S550>/PackNTF' */

// NearAnn4NonEntGainCalcvncEnableRTC does not define a Notification payload.

/// Notification Payload Definition for NearAnn4VolumeSet
/// ----------------------------
/// @ingroup NearAnn4VolumeSet
/// This page describes the payload format for Notification messages
/// used by the NearAnn4VolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref NearAnn4VolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_NearAnn4VolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
NearAnn4VolumeSet_NTF_T;               /* '<S606>/PackNTF' */

// NearEnable does not define a Notification payload.

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
PreAmpVolumeCfgSet_NTF_T;              /* '<S1100>/PackNTF' */

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
PreAmpBalance_CfgSet_NTF_T;            /* '<S45>/PackNTF' */

/// Notification Payload Definition for PreAmpMedusaSettingsFadeControl
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsFadeControl
/// This page describes the payload format for Notification messages
/// used by the PreAmpMedusaSettingsFadeControl RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsFadeControl.
typedef struct
{
    uint32_t msgId;              // always RTCID_PreAmpMedusaSettingsFadeControl
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
}
PreAmpMedusaSettingsFadeControl_NTF_T; /* '<S514>/PackNTF' */

/// Notification Payload Definition for PreAmpMedusaSettingsSurroundLevelControlIndex
/// ----------------------------
/// @ingroup PreAmpMedusaSettingsSurroundLevelControlIndex
/// This page describes the payload format for Notification messages
/// used by the PreAmpMedusaSettingsSurroundLevelControlIndex RTC mechanism. For a more general
/// description of how this message works, see @ref PreAmpMedusaSettingsSurroundLevelControlIndex.
typedef struct
{
    uint32_t msgId;// always RTCID_PreAmpMedusaSettingsSurroundLevelControlIndex
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t surround_index;           // startChar:8 charSize:4
}
PreAmpMedusaSettingsSurroundLevelControlIndex_NTF_T;/* '<S523>/PackNTF' */

// PreAmpMedusaSettingsPostUpmixDisable does not define a Notification payload.
// PreAmpMedusaSettingsHoligramDisable does not define a Notification payload.
// PreAmpMedusaSettingsOverheadHoligramDisable does not define a Notification payload.

// PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable does not define a Notification payload.
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
PreAmpFadeControlSet_NTF_T;            /* '<S64>/PackNTF' */

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
PreAmpMuteControlSet_NTF_T;            /* '<S101>/PackNTF' */

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
PreAmpSleepingBeautyDRSet_NTF_T;       /* '<S153>/PackNTF' */

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
PreAmpSleepingBeautyFRSet_NTF_T;       /* '<S179>/PackNTF' */

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
PostProcessEnergyLimiterConfigEnableLimiter_NTF_T;/* '<S43>/PackNTF' */

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
PostProcessMuteCfgSet_NTF_T;           /* '<S58>/PackNTF' */

// PostProcessPostLimiterEqShutdown does not define a Notification payload.

// PostProcessLimiterSoftclipParamSet does not define a Notification payload.

// PostProcessLimiterSoftclipDisable does not define a Notification payload.

// PreAmpMedusaPostHoligramPostHoligramDisable does not define a Notification payload.
// InputSelectRouterCfgSet does not define a Notification payload.
// TestRouterASDRouterAuxEnable does not define a Notification payload.

// TestRouterASDRouterAuxGain does not define a Notification payload.

// TestRouterASDRouterAuxSelect does not define a Notification payload.

// TestRouterASDRouterMainGain does not define a Notification payload.

// TestRouterASDRouterMainSelect does not define a Notification payload.
// MicInRMSOutputRMS does not define a Notification payload.

// MicInRMSEnable does not define a Notification payload.

// ------------------------------------------------------------
// PAYLOAD UNION MACROS
// ------------------------------------------------------------
#define ALL_REQ_PAYLOADS_1_1           ArmPreAmpBassSet_REQ_T ArmPreAmpBassSet; ArmPreAmpMidrangeCfgSet_REQ_T ArmPreAmpMidrangeCfgSet; ArmPreAmpTrebleSet_REQ_T ArmPreAmpTrebleSet; ArmPreAmpVolumeRefCfgSet_REQ_T ArmPreAmpVolumeRefCfgSet;
#define ALL_RSP_PAYLOADS_1_1           ArmPreAmpBassSet_RSP_T ArmPreAmpBassSet; ArmPreAmpMidrangeCfgSet_RSP_T ArmPreAmpMidrangeCfgSet; ArmPreAmpTrebleSet_RSP_T ArmPreAmpTrebleSet; ArmPreAmpVolumeRefCfgSet_RSP_T ArmPreAmpVolumeRefCfgSet;
#define ALL_NTF_PAYLOADS_1_1           ArmPreAmpBassSet_NTF_T ArmPreAmpBassSet; ArmPreAmpMidrangeCfgSet_NTF_T ArmPreAmpMidrangeCfgSet; ArmPreAmpTrebleSet_NTF_T ArmPreAmpTrebleSet; ArmPreAmpVolumeRefCfgSet_NTF_T ArmPreAmpVolumeRefCfgSet;

#define ALL_REQ_PAYLOADS_1_2           NearDr_RouterSet_REQ_T NearDr_RouterSet; NearDr_Channel2_Virtualization_REQ_T NearDr_Channel2_Virtualization; NearDr_Channel1_Virtualization_REQ_T NearDr_Channel1_Virtualization; NearDr_Channel3_Virtualization_REQ_T NearDr_Channel3_Virtualization; NearDr_Channel4_Virtualization_REQ_T NearDr_Channel4_Virtualization; NearFr_RouterSet_REQ_T NearFr_RouterSet; NearFr_Channel2_Virtualization_REQ_T NearFr_Channel2_Virtualization; NearFr_Channel1_Virtualization_REQ_T NearFr_Channel1_Virtualization; NearFr_Channel3_Virtualization_REQ_T NearFr_Channel3_Virtualization; NearFr_Channel4_Virtualization_REQ_T NearFr_Channel4_Virtualization; NearChannel2_Location_REQ_T NearChannel2_Location; NearChannel1_Location_REQ_T NearChannel1_Location; NearChannel4_Location_REQ_T NearChannel4_Location; NearChannel3_Location_REQ_T NearChannel3_Location; NearPhoneZones_Enable_REQ_T NearPhoneZones_Enable; NearAnnNoiseLevel_REQ_T NearAnnNoiseLevel; NearAnn1Ann1MuteSet_REQ_T NearAnn1Ann1MuteSet; NearAnn1NonEntGainCalcvncEnableRTC_REQ_T NearAnn1NonEntGainCalcvncEnableRTC; NearAnn1VolumeSet_REQ_T NearAnn1VolumeSet; NearAnn2Ann2MuteSet_REQ_T NearAnn2Ann2MuteSet; NearAnn2NonEntGainCalcvncEnableRTC_REQ_T NearAnn2NonEntGainCalcvncEnableRTC; NearAnn2VolumeSet_REQ_T NearAnn2VolumeSet; NearAnn3Ann3MuteSet_REQ_T NearAnn3Ann3MuteSet; NearAnn3NonEntGainCalcvncEnableRTC_REQ_T NearAnn3NonEntGainCalcvncEnableRTC; NearAnn3VolumeSet_REQ_T NearAnn3VolumeSet; NearAnn4Ann4MuteSet_REQ_T NearAnn4Ann4MuteSet; NearAnn4NonEntGainCalcvncEnableRTC_REQ_T NearAnn4NonEntGainCalcvncEnableRTC; NearAnn4VolumeSet_REQ_T NearAnn4VolumeSet; NearEnable_REQ_T NearEnable; PreAmpAudiopilotsetHVACSetting_REQ_T PreAmpAudiopilotsetHVACSetting; PreAmpAudiopilotmuteReference_REQ_T PreAmpAudiopilotmuteReference; PreAmpAudiopilotsetSpeedIn_REQ_T PreAmpAudiopilotsetSpeedIn; PreAmpAudiopilotgetBassBoost_REQ_T PreAmpAudiopilotgetBassBoost; PreAmpAudiopilotgetMidBoost_REQ_T PreAmpAudiopilotgetMidBoost; PreAmpAudiopilotgetTrebleBoost_REQ_T PreAmpAudiopilotgetTrebleBoost; PreAmpAudiopilotsetDynEQBoostOverRide_REQ_T PreAmpAudiopilotsetDynEQBoostOverRide; PreAmpAudiopilotsetVNCBassBoostOverRide_REQ_T PreAmpAudiopilotsetVNCBassBoostOverRide; PreAmpAudiopilotsetVNCMidBoostOverRide_REQ_T PreAmpAudiopilotsetVNCMidBoostOverRide; PreAmpAudiopilotsetVNCTrebleBoostOverRide_REQ_T PreAmpAudiopilotsetVNCTrebleBoostOverRide; PreAmpAudiopilotsetLoudSignalOverRide_REQ_T PreAmpAudiopilotsetLoudSignalOverRide; PreAmpAudiopilotsetVNCSignalOverRide_REQ_T PreAmpAudiopilotsetVNCSignalOverRide; PreAmpAudiopilotgetAverageAudioPilotBoostLevel_REQ_T PreAmpAudiopilotgetAverageAudioPilotBoostLevel; PreAmpAudiopilotsetHFNoiseOverRide_REQ_T PreAmpAudiopilotsetHFNoiseOverRide; PreAmpAudiopilotsetLFNoiseOverRide_REQ_T PreAmpAudiopilotsetLFNoiseOverRide; PreAmpAudiopilotsetRatioOverRide_REQ_T PreAmpAudiopilotsetRatioOverRide; PreAmpAudiopilotsetWBNoiseOverRide_REQ_T PreAmpAudiopilotsetWBNoiseOverRide; PreAmpAudiopilotsetDyneqOn_REQ_T PreAmpAudiopilotsetDyneqOn; PreAmpAudiopilotsetAudioPilotOn_REQ_T PreAmpAudiopilotsetAudioPilotOn; PreAmpVolumeCfgSet_REQ_T PreAmpVolumeCfgSet;
#define ALL_RSP_PAYLOADS_1_2           NearDr_RouterSet_RSP_T NearDr_RouterSet; NearDr_Channel2_Virtualization_RSP_T NearDr_Channel2_Virtualization; NearDr_Channel1_Virtualization_RSP_T NearDr_Channel1_Virtualization; NearDr_Channel3_Virtualization_RSP_T NearDr_Channel3_Virtualization; NearDr_Channel4_Virtualization_RSP_T NearDr_Channel4_Virtualization; NearFr_RouterSet_RSP_T NearFr_RouterSet; NearFr_Channel2_Virtualization_RSP_T NearFr_Channel2_Virtualization; NearFr_Channel1_Virtualization_RSP_T NearFr_Channel1_Virtualization; NearFr_Channel3_Virtualization_RSP_T NearFr_Channel3_Virtualization; NearFr_Channel4_Virtualization_RSP_T NearFr_Channel4_Virtualization; NearChannel2_Location_RSP_T NearChannel2_Location; NearChannel1_Location_RSP_T NearChannel1_Location; NearChannel4_Location_RSP_T NearChannel4_Location; NearChannel3_Location_RSP_T NearChannel3_Location; NearPhoneZones_Enable_RSP_T NearPhoneZones_Enable; NearAnn1Ann1MuteSet_RSP_T NearAnn1Ann1MuteSet; NearAnn1NonEntGainCalcvncEnableRTC_RSP_T NearAnn1NonEntGainCalcvncEnableRTC; NearAnn1VolumeSet_RSP_T NearAnn1VolumeSet; NearAnn2Ann2MuteSet_RSP_T NearAnn2Ann2MuteSet; NearAnn2NonEntGainCalcvncEnableRTC_RSP_T NearAnn2NonEntGainCalcvncEnableRTC; NearAnn2VolumeSet_RSP_T NearAnn2VolumeSet; NearAnn3Ann3MuteSet_RSP_T NearAnn3Ann3MuteSet; NearAnn3NonEntGainCalcvncEnableRTC_RSP_T NearAnn3NonEntGainCalcvncEnableRTC; NearAnn3VolumeSet_RSP_T NearAnn3VolumeSet; NearAnn4Ann4MuteSet_RSP_T NearAnn4Ann4MuteSet; NearAnn4NonEntGainCalcvncEnableRTC_RSP_T NearAnn4NonEntGainCalcvncEnableRTC; NearAnn4VolumeSet_RSP_T NearAnn4VolumeSet; NearEnable_RSP_T NearEnable; PreAmpAudiopilotmuteReference_RSP_T PreAmpAudiopilotmuteReference; PreAmpAudiopilotgetBassBoost_RSP_T PreAmpAudiopilotgetBassBoost; PreAmpAudiopilotgetMidBoost_RSP_T PreAmpAudiopilotgetMidBoost; PreAmpAudiopilotgetTrebleBoost_RSP_T PreAmpAudiopilotgetTrebleBoost; PreAmpAudiopilotgetAverageAudioPilotBoostLevel_RSP_T PreAmpAudiopilotgetAverageAudioPilotBoostLevel; PreAmpAudiopilotsetDyneqOn_RSP_T PreAmpAudiopilotsetDyneqOn; PreAmpAudiopilotsetAudioPilotOn_RSP_T PreAmpAudiopilotsetAudioPilotOn; PreAmpVolumeCfgSet_RSP_T PreAmpVolumeCfgSet;
#define ALL_NTF_PAYLOADS_1_2           NearAnn1Ann1MuteSet_NTF_T NearAnn1Ann1MuteSet; NearAnn1VolumeSet_NTF_T NearAnn1VolumeSet; NearAnn2Ann2MuteSet_NTF_T NearAnn2Ann2MuteSet; NearAnn2VolumeSet_NTF_T NearAnn2VolumeSet; NearAnn3Ann3MuteSet_NTF_T NearAnn3Ann3MuteSet; NearAnn3VolumeSet_NTF_T NearAnn3VolumeSet; NearAnn4Ann4MuteSet_NTF_T NearAnn4Ann4MuteSet; NearAnn4VolumeSet_NTF_T NearAnn4VolumeSet; PreAmpVolumeCfgSet_NTF_T PreAmpVolumeCfgSet;

#define ALL_REQ_PAYLOADS_1_3           PreAmpBalance_CfgSet_REQ_T PreAmpBalance_CfgSet; PreAmpMedusaSettingsFadeControl_REQ_T PreAmpMedusaSettingsFadeControl; PreAmpMedusaSettingsSurroundLevelControlIndex_REQ_T PreAmpMedusaSettingsSurroundLevelControlIndex; PreAmpMedusaSettingsPostUpmixDisable_REQ_T PreAmpMedusaSettingsPostUpmixDisable;
#define ALL_RSP_PAYLOADS_1_3           PreAmpBalance_CfgSet_RSP_T PreAmpBalance_CfgSet; PreAmpMedusaSettingsFadeControl_RSP_T PreAmpMedusaSettingsFadeControl; PreAmpMedusaSettingsSurroundLevelControlIndex_RSP_T PreAmpMedusaSettingsSurroundLevelControlIndex; PreAmpMedusaSettingsPostUpmixDisable_RSP_T PreAmpMedusaSettingsPostUpmixDisable;
#define ALL_NTF_PAYLOADS_1_3           PreAmpBalance_CfgSet_NTF_T PreAmpBalance_CfgSet; PreAmpMedusaSettingsFadeControl_NTF_T PreAmpMedusaSettingsFadeControl; PreAmpMedusaSettingsSurroundLevelControlIndex_NTF_T PreAmpMedusaSettingsSurroundLevelControlIndex;

#define ALL_REQ_PAYLOADS_1_4           PreAmpMedusaSettingsHoligramDisable_REQ_T PreAmpMedusaSettingsHoligramDisable;
#define ALL_RSP_PAYLOADS_1_4           PreAmpMedusaSettingsHoligramDisable_RSP_T PreAmpMedusaSettingsHoligramDisable;
#define ALL_NTF_PAYLOADS_1_4

#define ALL_REQ_PAYLOADS_1_5           PreAmpMedusaSettingsOverheadHoligramDisable_REQ_T PreAmpMedusaSettingsOverheadHoligramDisable; PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable_REQ_T PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable;
#define ALL_RSP_PAYLOADS_1_5           PreAmpMedusaSettingsOverheadHoligramDisable_RSP_T PreAmpMedusaSettingsOverheadHoligramDisable; PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable_RSP_T PreAmpMedusaSettingsHeadrestEqHeadrestHoligramDisable;
#define ALL_NTF_PAYLOADS_1_5

#define ALL_REQ_PAYLOADS_1_6           PreAmpFadeControlSet_REQ_T PreAmpFadeControlSet; PreAmpMuteControlSet_REQ_T PreAmpMuteControlSet; PreAmpResetSleepingBeauty_REQ_T PreAmpResetSleepingBeauty; PreAmpSleepingBeautyDRSet_REQ_T PreAmpSleepingBeautyDRSet; PreAmpSleepingBeautyFRSet_REQ_T PreAmpSleepingBeautyFRSet;
#define ALL_RSP_PAYLOADS_1_6           PreAmpFadeControlSet_RSP_T PreAmpFadeControlSet; PreAmpMuteControlSet_RSP_T PreAmpMuteControlSet; PreAmpResetSleepingBeauty_RSP_T PreAmpResetSleepingBeauty; PreAmpSleepingBeautyDRSet_RSP_T PreAmpSleepingBeautyDRSet; PreAmpSleepingBeautyFRSet_RSP_T PreAmpSleepingBeautyFRSet;
#define ALL_NTF_PAYLOADS_1_6           PreAmpFadeControlSet_NTF_T PreAmpFadeControlSet; PreAmpMuteControlSet_NTF_T PreAmpMuteControlSet; PreAmpSleepingBeautyDRSet_NTF_T PreAmpSleepingBeautyDRSet; PreAmpSleepingBeautyFRSet_NTF_T PreAmpSleepingBeautyFRSet;

#define ALL_REQ_PAYLOADS_1_7           PostProcessEnergyLimiterConfigEnableLimiter_REQ_T PostProcessEnergyLimiterConfigEnableLimiter; PostProcessMuteCfgSet_REQ_T PostProcessMuteCfgSet; PostProcessPostLimiterEqShutdown_REQ_T PostProcessPostLimiterEqShutdown; PostProcessLimiterSoftclipParamSet_REQ_T PostProcessLimiterSoftclipParamSet; PostProcessLimiterSoftclipDisable_REQ_T PostProcessLimiterSoftclipDisable; PreAmpMedusaPostHoligramPostHoligramDisable_REQ_T PreAmpMedusaPostHoligramPostHoligramDisable;
#define ALL_RSP_PAYLOADS_1_7           PostProcessEnergyLimiterConfigEnableLimiter_RSP_T PostProcessEnergyLimiterConfigEnableLimiter; PostProcessMuteCfgSet_RSP_T PostProcessMuteCfgSet; PostProcessPostLimiterEqShutdown_RSP_T PostProcessPostLimiterEqShutdown; PostProcessLimiterSoftclipParamSet_RSP_T PostProcessLimiterSoftclipParamSet; PostProcessLimiterSoftclipDisable_RSP_T PostProcessLimiterSoftclipDisable; PreAmpMedusaPostHoligramPostHoligramDisable_RSP_T PreAmpMedusaPostHoligramPostHoligramDisable;
#define ALL_NTF_PAYLOADS_1_7           PostProcessEnergyLimiterConfigEnableLimiter_NTF_T PostProcessEnergyLimiterConfigEnableLimiter; PostProcessMuteCfgSet_NTF_T PostProcessMuteCfgSet;

#define ALL_REQ_PAYLOADS_1_8           InputSelectRouterCfgSet_REQ_T InputSelectRouterCfgSet;
#define ALL_RSP_PAYLOADS_1_8           InputSelectRouterCfgSet_RSP_T InputSelectRouterCfgSet;
#define ALL_NTF_PAYLOADS_1_8

#define ALL_REQ_PAYLOADS_1_9           TestRouterASDRouterAuxEnable_REQ_T TestRouterASDRouterAuxEnable; TestRouterASDRouterAuxGain_REQ_T TestRouterASDRouterAuxGain; TestRouterASDRouterAuxSelect_REQ_T TestRouterASDRouterAuxSelect; TestRouterASDRouterMainGain_REQ_T TestRouterASDRouterMainGain; TestRouterASDRouterMainSelect_REQ_T TestRouterASDRouterMainSelect;
#define ALL_RSP_PAYLOADS_1_9           TestRouterASDRouterAuxEnable_RSP_T TestRouterASDRouterAuxEnable; TestRouterASDRouterAuxGain_RSP_T TestRouterASDRouterAuxGain; TestRouterASDRouterAuxSelect_RSP_T TestRouterASDRouterAuxSelect; TestRouterASDRouterMainGain_RSP_T TestRouterASDRouterMainGain; TestRouterASDRouterMainSelect_RSP_T TestRouterASDRouterMainSelect;
#define ALL_NTF_PAYLOADS_1_9

#define ALL_REQ_PAYLOADS_1_10          MicInRMSOutputRMS_REQ_T MicInRMSOutputRMS; MicInRMSEnable_REQ_T MicInRMSEnable;
#define ALL_RSP_PAYLOADS_1_10          MicInRMSOutputRMS_RSP_T MicInRMSOutputRMS; MicInRMSEnable_RSP_T MicInRMSEnable;
#define ALL_NTF_PAYLOADS_1_10


// Request payloads
union AllRtcReqPayloads
{ ALL_REQ_PAYLOADS_1_1 ALL_REQ_PAYLOADS_1_2 ALL_REQ_PAYLOADS_1_3 ALL_REQ_PAYLOADS_1_4 ALL_REQ_PAYLOADS_1_5 ALL_REQ_PAYLOADS_1_6 ALL_REQ_PAYLOADS_1_7 ALL_REQ_PAYLOADS_1_8 ALL_REQ_PAYLOADS_1_9 ALL_REQ_PAYLOADS_1_10 };

// Response payloads
union AllRtcRspPayloads
{ ALL_RSP_PAYLOADS_1_1 ALL_RSP_PAYLOADS_1_2 ALL_RSP_PAYLOADS_1_3 ALL_RSP_PAYLOADS_1_4 ALL_RSP_PAYLOADS_1_5 ALL_RSP_PAYLOADS_1_6 ALL_RSP_PAYLOADS_1_7 ALL_RSP_PAYLOADS_1_8 ALL_RSP_PAYLOADS_1_9 ALL_RSP_PAYLOADS_1_10 };

// Notification payloads
union AllRtcNtfPayloads
{ ALL_NTF_PAYLOADS_1_1 ALL_NTF_PAYLOADS_1_2 ALL_NTF_PAYLOADS_1_3 ALL_NTF_PAYLOADS_1_4 ALL_NTF_PAYLOADS_1_5 ALL_NTF_PAYLOADS_1_6 ALL_NTF_PAYLOADS_1_7 ALL_NTF_PAYLOADS_1_8 ALL_NTF_PAYLOADS_1_9 ALL_NTF_PAYLOADS_1_10 };

#endif

