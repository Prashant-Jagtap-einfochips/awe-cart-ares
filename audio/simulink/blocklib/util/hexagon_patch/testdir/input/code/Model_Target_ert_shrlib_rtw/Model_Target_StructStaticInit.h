/*
 * File: Model_Target_StructStaticInit.h
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

#ifndef RTW_HEADER_Model_Target_StructStaticInit_h_
#define RTW_HEADER_Model_Target_StructStaticInit_h_
#include "rtwtypes.h"
#include "Model_Target_types.h"

/* StructStaticInit type definitions */
typedef struct Model_TargetControlDsms_tag
{
    rgain_bus NearAnn1Ann1VolumeRampers;/* '<S292>/ramp control' */
    rgain_bus NearAnn2Ann2VolumeRampers;/* '<S372>/ramp control' */
    rgain_bus NearAnn3Ann3VolumeRampers;/* '<S452>/ramp control' */
    rgain_bus NearAnn4Ann4VolumeRampers;/* '<S532>/ramp control' */
    rgain_bus PreAmpBalance_CfgRampers[4];/* '<S1142>/ramp control' */
    rgain_bus PreAmpBassRampers[2];    /* '<S1974>/ramp control' */
    rgain_bus PreAmpFadeRamperDRRampers[4];/* '<S1843>/ramp control' */
    rgain_bus PreAmpFadeRamperFRRampers[4];/* '<S1850>/ramp control' */
    rgain_bus PreAmpMidrangeCfgRampers[2];/* '<S1975>/ramp control' */
    rgain_bus PreAmpSleepingBeautyDRRampers[4];/* '<S1900>/ramp control' */
    rgain_bus PreAmpSleepingBeautyFRRampers[4];/* '<S1920>/ramp control' */
    rgain_bus PreAmpTrebleRampers[2];  /* '<S1977>/ramp control' */
    rgain_bus PreAmpVolumeCfgRampers;  /* '<S1115>/ramp control' */
    real32_T NearAnn1MuteCurrentGain;  /* '<S268>/currentGain' */
    real32_T NearAnn1MuteNframes;      /* '<S268>/nFrames' */
    real32_T NearAnn1MuteTargetLinearGain;/* '<S268>/targetLinearGain' */
    real32_T NearAnn2MuteCurrentGain;  /* '<S348>/currentGain' */
    real32_T NearAnn2MuteNframes;      /* '<S348>/nFrames' */
    real32_T NearAnn2MuteTargetLinearGain;/* '<S348>/targetLinearGain' */
    real32_T NearAnn3MuteCurrentGain;  /* '<S428>/currentGain' */
    real32_T NearAnn3MuteNframes;      /* '<S428>/nFrames' */
    real32_T NearAnn3MuteTargetLinearGain;/* '<S428>/targetLinearGain' */
    real32_T NearAnn4MuteCurrentGain;  /* '<S508>/currentGain' */
    real32_T NearAnn4MuteNframes;      /* '<S508>/nFrames' */
    real32_T NearAnn4MuteTargetLinearGain;/* '<S508>/targetLinearGain' */
    real32_T PostProcessMuteCurrentGain;/* '<S603>/currentGain' */
    real32_T PostProcessMuteNframes;   /* '<S603>/nFrames' */
    real32_T PostProcessMuteTargetLinearGain;/* '<S603>/targetLinearGain' */
    real32_T PreAmpMuteDRCurrentGain;  /* '<S1868>/currentGain' */
    real32_T PreAmpMuteDRNframes;      /* '<S1868>/nFrames' */
    real32_T PreAmpMuteDRTargetLinearGain;/* '<S1868>/targetLinearGain' */
    real32_T PreAmpMuteFRCurrentGain;  /* '<S1881>/currentGain' */
    real32_T PreAmpMuteFRNframes;      /* '<S1881>/nFrames' */
    real32_T PreAmpMuteFRTargetLinearGain;/* '<S1881>/targetLinearGain' */
    uint32_T NearAnn1Ann1VolumeFreeze; /* '<S292>/freeze control' */
    uint32_T NearAnn2Ann2VolumeFreeze; /* '<S372>/freeze control' */
    uint32_T NearAnn3Ann3VolumeFreeze; /* '<S452>/freeze control' */
    uint32_T NearAnn4Ann4VolumeFreeze; /* '<S532>/freeze control' */
    uint32_T PreAmpBalance_CfgFreeze;  /* '<S1142>/freeze control' */
    uint32_T PreAmpBassFreeze;         /* '<S1974>/freeze control' */
    uint32_T PreAmpBassPhase;          /* '<S1974>/phase control' */
    uint32_T PreAmpFadeRamperDRFreeze; /* '<S1843>/freeze control' */
    uint32_T PreAmpFadeRamperFRFreeze; /* '<S1850>/freeze control' */
    uint32_T PreAmpMidrangeCfgFreeze;  /* '<S1975>/freeze control' */
    uint32_T PreAmpMidrangeCfgPhase;   /* '<S1975>/phase control' */
    uint32_T PreAmpSleepingBeautyDRFreeze;/* '<S1900>/freeze control' */
    uint32_T PreAmpSleepingBeautyFRFreeze;/* '<S1920>/freeze control' */
    uint32_T PreAmpTrebleFreeze;       /* '<S1977>/freeze control' */
    uint32_T PreAmpTreblePhase;        /* '<S1977>/phase control' */
    uint32_T PreAmpVolumeCfgFreeze;    /* '<S1115>/freeze control' */
}
Model_TargetControlDsms_type;

typedef struct Model_TargetInputSelectRouterCfgSetRtcOut_tag
{
    int32_T REQ_routerMap[12];         /* '<S21>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S21>/UnpackREQ' */
}
Model_TargetInputSelectRouterCfgSetRtcOut_type;

typedef struct Model_TargetNearAnn1Ann1MuteSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S277>/UnpackREQ' */
    uint32_T REQ_mute_switch;          /* '<S277>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S277>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S277>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S277>/UnpackREQ' */
}
Model_TargetNearAnn1Ann1MuteSetRtcOut_type;

typedef struct Model_TargetNearAnn1Ann1NonEntGainCalcvncEnableRTCRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S257>/UnpackREQ' */
    uint32_T REQ_vncEnable;            /* '<S257>/UnpackREQ' */
}
Model_TargetNearAnn1Ann1NonEntGainCalcvncEnableRTCRtcOut_type;

typedef struct Model_TargetNearAnn1Ann1VolumeSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S305>/UnpackREQ' */
    uint32_T REQ_db_per_second;        /* '<S305>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S305>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S305>/UnpackREQ' */
    uint32_T REQ_volume_index;         /* '<S305>/UnpackREQ' */
}
Model_TargetNearAnn1Ann1VolumeSetRtcOut_type;

typedef struct Model_TargetNearAnn2Ann2MuteSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S357>/UnpackREQ' */
    uint32_T REQ_mute_switch;          /* '<S357>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S357>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S357>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S357>/UnpackREQ' */
}
Model_TargetNearAnn2Ann2MuteSetRtcOut_type;

typedef struct Model_TargetNearAnn2Ann2NonEntGainCalcvncEnableRTCRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S337>/UnpackREQ' */
    uint32_T REQ_vncEnable;            /* '<S337>/UnpackREQ' */
}
Model_TargetNearAnn2Ann2NonEntGainCalcvncEnableRTCRtcOut_type;

typedef struct Model_TargetNearAnn2Ann2VolumeSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S385>/UnpackREQ' */
    uint32_T REQ_db_per_second;        /* '<S385>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S385>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S385>/UnpackREQ' */
    uint32_T REQ_volume_index;         /* '<S385>/UnpackREQ' */
}
Model_TargetNearAnn2Ann2VolumeSetRtcOut_type;

typedef struct Model_TargetNearAnn3Ann3MuteSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S437>/UnpackREQ' */
    uint32_T REQ_mute_switch;          /* '<S437>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S437>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S437>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S437>/UnpackREQ' */
}
Model_TargetNearAnn3Ann3MuteSetRtcOut_type;

typedef struct Model_TargetNearAnn3Ann3NonEntGainCalcvncEnableRTCRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S417>/UnpackREQ' */
    uint32_T REQ_vncEnable;            /* '<S417>/UnpackREQ' */
}
Model_TargetNearAnn3Ann3NonEntGainCalcvncEnableRTCRtcOut_type;

typedef struct Model_TargetNearAnn3Ann3VolumeSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S465>/UnpackREQ' */
    uint32_T REQ_db_per_second;        /* '<S465>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S465>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S465>/UnpackREQ' */
    uint32_T REQ_volume_index;         /* '<S465>/UnpackREQ' */
}
Model_TargetNearAnn3Ann3VolumeSetRtcOut_type;

typedef struct Model_TargetNearAnn4Ann4MuteSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S517>/UnpackREQ' */
    uint32_T REQ_mute_switch;          /* '<S517>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S517>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S517>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S517>/UnpackREQ' */
}
Model_TargetNearAnn4Ann4MuteSetRtcOut_type;

typedef struct Model_TargetNearAnn4Ann4NonEntGainCalcvncEnableRTCRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S497>/UnpackREQ' */
    uint32_T REQ_vncEnable;            /* '<S497>/UnpackREQ' */
}
Model_TargetNearAnn4Ann4NonEntGainCalcvncEnableRTCRtcOut_type;

typedef struct Model_TargetNearAnn4Ann4VolumeSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S545>/UnpackREQ' */
    uint32_T REQ_db_per_second;        /* '<S545>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S545>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S545>/UnpackREQ' */
    uint32_T REQ_volume_index;         /* '<S545>/UnpackREQ' */
}
Model_TargetNearAnn4Ann4VolumeSetRtcOut_type;

typedef struct Model_TargetNearAnnNoiseLevelRtcOut_tag
{
    real32_T REQ_NoiseSig;             /* '<S233>/UnpackREQ' */
}
Model_TargetNearAnnNoiseLevelRtcOut_type;

typedef struct Model_TargetNearChannel1_LocationRtcOut_tag
{
    real32_T REQ_loc[2];               /* '<S144>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S144>/UnpackREQ' */
}
Model_TargetNearChannel1_LocationRtcOut_type;

typedef struct Model_TargetNearChannel2_LocationRtcOut_tag
{
    real32_T REQ_loc[2];               /* '<S138>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S138>/UnpackREQ' */
}
Model_TargetNearChannel2_LocationRtcOut_type;

typedef struct Model_TargetNearChannel3_LocationRtcOut_tag
{
    real32_T REQ_loc[2];               /* '<S156>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S156>/UnpackREQ' */
}
Model_TargetNearChannel3_LocationRtcOut_type;

typedef struct Model_TargetNearChannel4_LocationRtcOut_tag
{
    real32_T REQ_loc[2];               /* '<S150>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S150>/UnpackREQ' */
}
Model_TargetNearChannel4_LocationRtcOut_type;

typedef struct Model_TargetNearDr_RouterSetRtcOut_tag
{
    int32_T REQ_routerMap[18];         /* '<S71>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S71>/UnpackREQ' */
}
Model_TargetNearDr_RouterSetRtcOut_type;

typedef struct Model_TargetNearEnableRtcOut_tag
{
    uint32_T REQ_Enable;               /* '<S559>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S559>/UnpackREQ' */
}
Model_TargetNearEnableRtcOut_type;

typedef struct Model_TargetNearFr_RouterSetRtcOut_tag
{
    int32_T REQ_routerMap[18];         /* '<S115>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S115>/UnpackREQ' */
}
Model_TargetNearFr_RouterSetRtcOut_type;

typedef struct Model_TargetPostProcessEnergyLimiterConfigEnableLimiterRtcOut_tag
{
    uint32_T REQ_EnableGainLimiter;    /* '<S597>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S597>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S597>/UnpackREQ' */
}
Model_TargetPostProcessEnergyLimiterConfigEnableLimiterRtcOut_type;

typedef struct Model_TargetPostProcessLimiterSoftclipDisableRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S651>/UnpackREQ' */
    uint32_T REQ_disableMsg;           /* '<S651>/UnpackREQ' */
}
Model_TargetPostProcessLimiterSoftclipDisableRtcOut_type;

typedef struct Model_TargetPostProcessLimiterSoftclipParamSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S645>/UnpackREQ' */
    uint32_T REQ_paramSet;             /* '<S645>/UnpackREQ' */
}
Model_TargetPostProcessLimiterSoftclipParamSetRtcOut_type;

typedef struct Model_TargetPostProcessMuteCfgSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S612>/UnpackREQ' */
    uint32_T REQ_mute_switch;          /* '<S612>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S612>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S612>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S612>/UnpackREQ' */
}
Model_TargetPostProcessMuteCfgSetRtcOut_type;

typedef struct Model_TargetPostProcessPostLimiterEqShutdownRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S639>/UnpackREQ' */
}
Model_TargetPostProcessPostLimiterEqShutdownRtcOut_type;

typedef struct
    Model_TargetPreAmpAudiopilotgetAverageAudioPilotBoostLevelRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S820>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotgetAverageAudioPilotBoostLevelRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotgetBassBoostRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S720>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotgetBassBoostRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotgetMidBoostRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S726>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotgetMidBoostRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotgetTrebleBoostRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S732>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotgetTrebleBoostRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotmuteReferenceRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S690>/UnpackREQ' */
    uint32_T REQ_muteReference;        /* '<S690>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotmuteReferenceRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetAudioPilotOnRtcOut_tag
{
    uint32_T REQ_audiopilot_on;        /* '<S877>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S877>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetAudioPilotOnRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetDynEQBoostOverRideRtcOut_tag
{
    real32_T REQ_DynEQBoostOverRide;   /* '<S780>/UnpackREQ' */
    uint32_T REQ_DynEQBoostOverRideFlag;/* '<S780>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetDynEQBoostOverRideRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetDyneqOnRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S871>/UnpackREQ' */
    uint32_T REQ_dyneq_on;             /* '<S871>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetDyneqOnRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetHFNoiseOverRideRtcOut_tag
{
    real32_T REQ_HFNoiseOverRide;      /* '<S831>/UnpackREQ' */
    uint32_T REQ_HFNoiseOverRideFlag;  /* '<S831>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetHFNoiseOverRideRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetHVACSettingRtcOut_tag
{
    real32_T REQ_HVACSetting;          /* '<S684>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetHVACSettingRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetLFNoiseOverRideRtcOut_tag
{
    real32_T REQ_LFNoiseOverRide;      /* '<S837>/UnpackREQ' */
    uint32_T REQ_LFNoiseOverRideFlag;  /* '<S837>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetLFNoiseOverRideRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetLoudSignalOverRideRtcOut_tag
{
    real32_T REQ_LoudSignalOverRide;   /* '<S808>/UnpackREQ' */
    uint32_T REQ_LoudSignalOverRideFlag;/* '<S808>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetLoudSignalOverRideRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetRatioOverRideRtcOut_tag
{
    real32_T REQ_RatioOverRide;        /* '<S843>/UnpackREQ' */
    uint32_T REQ_RatioOverRideFlag;    /* '<S843>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetRatioOverRideRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetSpeedInRtcOut_tag
{
    real32_T REQ_speedIn;              /* '<S696>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetSpeedInRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetVNCBassBoostOverRideRtcOut_tag
{
    real32_T REQ_VNCBassBoostOverRide; /* '<S786>/UnpackREQ' */
    uint32_T REQ_VNCBassBoostOverRideFlag;/* '<S786>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetVNCBassBoostOverRideRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetVNCMidBoostOverRideRtcOut_tag
{
    real32_T REQ_VNCMidBoostOverRide;  /* '<S792>/UnpackREQ' */
    uint32_T REQ_VNCMidBoostOverRideFlag;/* '<S792>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetVNCMidBoostOverRideRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetVNCSignalOverRideRtcOut_tag
{
    real32_T REQ_VNCSignalOverRide;    /* '<S814>/UnpackREQ' */
    uint32_T REQ_VNCSignalOverRideFlag;/* '<S814>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetVNCSignalOverRideRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetVNCTrebleBoostOverRideRtcOut_tag
{
    real32_T REQ_VNCTrebleBoostOverRide;/* '<S798>/UnpackREQ' */
    uint32_T REQ_VNCTrebleBoostOverRideFlag;/* '<S798>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetVNCTrebleBoostOverRideRtcOut_type;

typedef struct Model_TargetPreAmpAudiopilotsetWBNoiseOverRideRtcOut_tag
{
    real32_T REQ_WBNoiseOverRide;      /* '<S849>/UnpackREQ' */
    uint32_T REQ_WBNoiseOverRideFlag;  /* '<S849>/UnpackREQ' */
}
Model_TargetPreAmpAudiopilotsetWBNoiseOverRideRtcOut_type;

typedef struct Model_TargetPreAmpBalance_CfgSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1162>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S1162>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S1162>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S1162>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S1162>/UnpackREQ' */
}
Model_TargetPreAmpBalance_CfgSetRtcOut_type;

typedef struct Model_TargetPreAmpBassSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1994>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S1994>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S1994>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S1994>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S1994>/UnpackREQ' */
}
Model_TargetPreAmpBassSetRtcOut_type;

typedef struct Model_TargetPreAmpFadeControlSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1841>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S1841>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S1841>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S1841>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S1841>/UnpackREQ' */
}
Model_TargetPreAmpFadeControlSetRtcOut_type;

typedef struct Model_TargetPreAmpMedusaHoligramDisableRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1768>/UnpackREQ' */
}
Model_TargetPreAmpMedusaHoligramDisableRtcOut_type;

typedef struct Model_TargetPreAmpMedusaOverheadHoligramDisableRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1780>/UnpackREQ' */
}
Model_TargetPreAmpMedusaOverheadHoligramDisableRtcOut_type;

typedef struct Model_TargetPreAmpMedusaPostHoligramDisableRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1799>/UnpackREQ' */
}
Model_TargetPreAmpMedusaPostHoligramDisableRtcOut_type;

typedef struct Model_TargetPreAmpMedusaPostUpmixDisableRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1698>/UnpackREQ' */
}
Model_TargetPreAmpMedusaPostUpmixDisableRtcOut_type;

typedef struct Model_TargetPreAmpMedusaSurroundLevelControlIndexRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1634>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S1634>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S1634>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S1634>/UnpackREQ' */
    uint32_T REQ_surround_index;       /* '<S1634>/UnpackREQ' */
}
Model_TargetPreAmpMedusaSurroundLevelControlIndexRtcOut_type;

typedef struct Model_TargetPreAmpMedusaSurroundLevelControlResetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1640>/UnpackREQ' */
}
Model_TargetPreAmpMedusaSurroundLevelControlResetRtcOut_type;

typedef struct Model_TargetPreAmpMidrangeCfgSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S2012>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S2012>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S2012>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S2012>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S2012>/UnpackREQ' */
}
Model_TargetPreAmpMidrangeCfgSetRtcOut_type;

typedef struct Model_TargetPreAmpMuteControlSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1866>/UnpackREQ' */
    uint32_T REQ_mute_switch;          /* '<S1866>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S1866>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S1866>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S1866>/UnpackREQ' */
}
Model_TargetPreAmpMuteControlSetRtcOut_type;

typedef struct Model_TargetPreAmpResetSleepingBeautyRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1898>/UnpackREQ' */
}
Model_TargetPreAmpResetSleepingBeautyRtcOut_type;

typedef struct Model_TargetPreAmpSleepingBeautyDRSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1918>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S1918>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S1918>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S1918>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S1918>/UnpackREQ' */
}
Model_TargetPreAmpSleepingBeautyDRSetRtcOut_type;

typedef struct Model_TargetPreAmpSleepingBeautyFRSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S1938>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S1938>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S1938>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S1938>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S1938>/UnpackREQ' */
}
Model_TargetPreAmpSleepingBeautyFRSetRtcOut_type;

typedef struct Model_TargetPreAmpTrebleSetRtcOut_tag
{
    uint32_T REQ_caller_handle;        /* '<S2048>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S2048>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S2048>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S2048>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S2048>/UnpackREQ' */
}
Model_TargetPreAmpTrebleSetRtcOut_type;

typedef struct Model_TargetPreAmpVolumeCfgSetRtcOut_tag
{
    int32_T REQ_gain_offset;           /* '<S1126>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S1126>/UnpackREQ' */
    uint32_T REQ_gain_index;           /* '<S1126>/UnpackREQ' */
    uint32_T REQ_ramp_db_per_second;   /* '<S1126>/UnpackREQ' */
    uint32_T REQ_ramp_milliseconds;    /* '<S1126>/UnpackREQ' */
    uint32_T REQ_sequence;             /* '<S1126>/UnpackREQ' */
}
Model_TargetPreAmpVolumeCfgSetRtcOut_type;

typedef struct Model_TargetTestRouterASDRouterAuxEnableRtcOut_tag
{
    int32_T REQ_EnableAuxFlag;         /* '<S2070>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S2070>/UnpackREQ' */
}
Model_TargetTestRouterASDRouterAuxEnableRtcOut_type;

typedef struct Model_TargetTestRouterASDRouterAuxGainRtcOut_tag
{
    real32_T REQ_AuxGain[24];          /* '<S2076>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S2076>/UnpackREQ' */
}
Model_TargetTestRouterASDRouterAuxGainRtcOut_type;

typedef struct Model_TargetTestRouterASDRouterAuxSelectRtcOut_tag
{
    int32_T REQ_AuxSelect[24];         /* '<S2082>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S2082>/UnpackREQ' */
}
Model_TargetTestRouterASDRouterAuxSelectRtcOut_type;

typedef struct Model_TargetTestRouterASDRouterMainGainRtcOut_tag
{
    real32_T REQ_mainGain[24];         /* '<S2094>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S2094>/UnpackREQ' */
}
Model_TargetTestRouterASDRouterMainGainRtcOut_type;

typedef struct Model_TargetTestRouterASDRouterMainSelectRtcOut_tag
{
    int32_T REQ_mainSelect[24];        /* '<S2100>/UnpackREQ' */
    uint32_T REQ_caller_handle;        /* '<S2100>/UnpackREQ' */
}
Model_TargetTestRouterASDRouterMainSelectRtcOut_type;

/* StructStaticInit instance declarations */
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

#endif                         /* RTW_HEADER_Model_Target_StructStaticInit_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
