/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: Model_TSP_IDs.h
 *
 * Combined TSP message ID and payload definitions from all cores
 *
 */

#ifndef MODEL_TSP_IDS_H_
#define MODEL_TSP_IDS_H_
#include <stdint.h>

// ------------------------------------------------------------
// TSP ACCESS TOKENS
// ------------------------------------------------------------
#define Model_1_2_TSPID_CAPTURE        (0x200d0000U)
#define Model_1_2_TSPID_CLEAR          (0x200c0000U)
#define TSP_ADD_NearPhoneZones_Enable  (0x20180004U)
#define TSP_SUB_NearPhoneZones_Enable  (0x20190004U)
#define TSP_RETRIEVE_NearPhoneZones_Enable (0x201a0004U)
#define TSP_BLIND_READ_NearPhoneZones_Enable (0x201b0004U)
#define TSP_ADD_NearAnn1NonEntGainCalcGetNonEntBoostLeveldB (0x20180000U)
#define TSP_SUB_NearAnn1NonEntGainCalcGetNonEntBoostLeveldB (0x20190000U)
#define TSP_RETRIEVE_NearAnn1NonEntGainCalcGetNonEntBoostLeveldB (0x201a0000U)
#define TSP_BLIND_READ_NearAnn1NonEntGainCalcGetNonEntBoostLeveldB (0x201b0000U)
#define TSP_ADD_NearAnn2NonEntGainCalcGetNonEntBoostLeveldB (0x20180001U)
#define TSP_SUB_NearAnn2NonEntGainCalcGetNonEntBoostLeveldB (0x20190001U)
#define TSP_RETRIEVE_NearAnn2NonEntGainCalcGetNonEntBoostLeveldB (0x201a0001U)
#define TSP_BLIND_READ_NearAnn2NonEntGainCalcGetNonEntBoostLeveldB (0x201b0001U)
#define TSP_ADD_NearAnn3NonEntGainCalcGetNonEntBoostLeveldB (0x20180002U)
#define TSP_SUB_NearAnn3NonEntGainCalcGetNonEntBoostLeveldB (0x20190002U)
#define TSP_RETRIEVE_NearAnn3NonEntGainCalcGetNonEntBoostLeveldB (0x201a0002U)
#define TSP_BLIND_READ_NearAnn3NonEntGainCalcGetNonEntBoostLeveldB (0x201b0002U)
#define TSP_ADD_NearAnn4NonEntGainCalcGetNonEntBoostLeveldB (0x20180003U)
#define TSP_SUB_NearAnn4NonEntGainCalcGetNonEntBoostLeveldB (0x20190003U)
#define TSP_RETRIEVE_NearAnn4NonEntGainCalcGetNonEntBoostLeveldB (0x201a0003U)
#define TSP_BLIND_READ_NearAnn4NonEntGainCalcGetNonEntBoostLeveldB (0x201b0003U)
#define TSP_ADD_PreAmpAudiopilotVehicleSpeed (0x24280025U)
#define TSP_SUB_PreAmpAudiopilotVehicleSpeed (0x24290025U)
#define TSP_RETRIEVE_PreAmpAudiopilotVehicleSpeed (0x242a0025U)
#define TSP_BLIND_READ_PreAmpAudiopilotVehicleSpeed (0x242b0025U)
#define TSP_ADD_PreAmpAudiopilotHvacDbspl (0x2428000dU)
#define TSP_SUB_PreAmpAudiopilotHvacDbspl (0x2429000dU)
#define TSP_RETRIEVE_PreAmpAudiopilotHvacDbspl (0x242a000dU)
#define TSP_BLIND_READ_PreAmpAudiopilotHvacDbspl (0x242b000dU)
#define TSP_ADD_PreAmpAudiopilotLfNoiseDbspl (0x2428000fU)
#define TSP_SUB_PreAmpAudiopilotLfNoiseDbspl (0x2429000fU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseDbspl (0x242a000fU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseDbspl (0x242b000fU)
#define TSP_ADD_PreAmpAudiopilotWbNoiseDbspl (0x24280026U)
#define TSP_SUB_PreAmpAudiopilotWbNoiseDbspl (0x24290026U)
#define TSP_RETRIEVE_PreAmpAudiopilotWbNoiseDbspl (0x242a0026U)
#define TSP_BLIND_READ_PreAmpAudiopilotWbNoiseDbspl (0x242b0026U)
#define TSP_ADD_PreAmpAudiopilotHfNoiseDbspl (0x24280006U)
#define TSP_SUB_PreAmpAudiopilotHfNoiseDbspl (0x24290006U)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoiseDbspl (0x242a0006U)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoiseDbspl (0x242b0006U)
#define TSP_ADD_PreAmpAudiopilotNoiseRatio (0x24280020U)
#define TSP_SUB_PreAmpAudiopilotNoiseRatio (0x24290020U)
#define TSP_RETRIEVE_PreAmpAudiopilotNoiseRatio (0x242a0020U)
#define TSP_BLIND_READ_PreAmpAudiopilotNoiseRatio (0x242b0020U)
#define TSP_ADD_PreAmpAudiopilotTotalBassBoostDb (0x24280024U)
#define TSP_SUB_PreAmpAudiopilotTotalBassBoostDb (0x24290024U)
#define TSP_RETRIEVE_PreAmpAudiopilotTotalBassBoostDb (0x242a0024U)
#define TSP_BLIND_READ_PreAmpAudiopilotTotalBassBoostDb (0x242b0024U)
#define TSP_ADD_PreAmpAudiopilotAudiopilotMidrangeBoostDb (0x24280002U)
#define TSP_SUB_PreAmpAudiopilotAudiopilotMidrangeBoostDb (0x24290002U)
#define TSP_RETRIEVE_PreAmpAudiopilotAudiopilotMidrangeBoostDb (0x242a0002U)
#define TSP_BLIND_READ_PreAmpAudiopilotAudiopilotMidrangeBoostDb (0x242b0002U)
#define TSP_ADD_PreAmpAudiopilotAudiopilotTrebleBoostDb (0x24280003U)
#define TSP_SUB_PreAmpAudiopilotAudiopilotTrebleBoostDb (0x24290003U)
#define TSP_RETRIEVE_PreAmpAudiopilotAudiopilotTrebleBoostDb (0x242a0003U)
#define TSP_BLIND_READ_PreAmpAudiopilotAudiopilotTrebleBoostDb (0x242b0003U)
#define TSP_ADD_PreAmpAudiopilotDyneqBassBoostDb (0x24280004U)
#define TSP_SUB_PreAmpAudiopilotDyneqBassBoostDb (0x24290004U)
#define TSP_RETRIEVE_PreAmpAudiopilotDyneqBassBoostDb (0x242a0004U)
#define TSP_BLIND_READ_PreAmpAudiopilotDyneqBassBoostDb (0x242b0004U)
#define TSP_ADD_PreAmpAudiopilotAudiopilotBassBoostDb (0x24280000U)
#define TSP_SUB_PreAmpAudiopilotAudiopilotBassBoostDb (0x24290000U)
#define TSP_RETRIEVE_PreAmpAudiopilotAudiopilotBassBoostDb (0x242a0000U)
#define TSP_BLIND_READ_PreAmpAudiopilotAudiopilotBassBoostDb (0x242b0000U)
#define TSP_ADD_PreAmpAudiopilotIgnoreMicFlag (0x2428000eU)
#define TSP_SUB_PreAmpAudiopilotIgnoreMicFlag (0x2429000eU)
#define TSP_RETRIEVE_PreAmpAudiopilotIgnoreMicFlag (0x242a000eU)
#define TSP_BLIND_READ_PreAmpAudiopilotIgnoreMicFlag (0x242b000eU)
#define TSP_ADD_PreAmpAudiopilotAudiopilotBoostGain (0x24280001U)
#define TSP_SUB_PreAmpAudiopilotAudiopilotBoostGain (0x24290001U)
#define TSP_RETRIEVE_PreAmpAudiopilotAudiopilotBoostGain (0x242a0001U)
#define TSP_BLIND_READ_PreAmpAudiopilotAudiopilotBoostGain (0x242b0001U)
#define TSP_ADD_PreAmpAudiopilotDyneqBoostGain (0x24280005U)
#define TSP_SUB_PreAmpAudiopilotDyneqBoostGain (0x24290005U)
#define TSP_RETRIEVE_PreAmpAudiopilotDyneqBoostGain (0x242a0005U)
#define TSP_BLIND_READ_PreAmpAudiopilotDyneqBoostGain (0x242b0005U)
#define TSP_ADD_PreAmpLevelWatchAudiopilotEnvDb (0x24280027U)
#define TSP_SUB_PreAmpLevelWatchAudiopilotEnvDb (0x24290027U)
#define TSP_RETRIEVE_PreAmpLevelWatchAudiopilotEnvDb (0x242a0027U)
#define TSP_BLIND_READ_PreAmpLevelWatchAudiopilotEnvDb (0x242b0027U)
#define TSP_ADD_PreAmpLevelWatchAudiopilotLevelDbspl (0x24280028U)
#define TSP_SUB_PreAmpLevelWatchAudiopilotLevelDbspl (0x24290028U)
#define TSP_RETRIEVE_PreAmpLevelWatchAudiopilotLevelDbspl (0x242a0028U)
#define TSP_BLIND_READ_PreAmpLevelWatchAudiopilotLevelDbspl (0x242b0028U)
#define TSP_ADD_PreAmpLevelWatchDyneqEnvDb (0x24280029U)
#define TSP_SUB_PreAmpLevelWatchDyneqEnvDb (0x24290029U)
#define TSP_RETRIEVE_PreAmpLevelWatchDyneqEnvDb (0x242a0029U)
#define TSP_BLIND_READ_PreAmpLevelWatchDyneqEnvDb (0x242b0029U)
#define TSP_ADD_PreAmpLevelWatchDyneqLevelDbspl (0x2428002aU)
#define TSP_SUB_PreAmpLevelWatchDyneqLevelDbspl (0x2429002aU)
#define TSP_RETRIEVE_PreAmpLevelWatchDyneqLevelDbspl (0x242a002aU)
#define TSP_BLIND_READ_PreAmpLevelWatchDyneqLevelDbspl (0x242b002aU)
#define TSP_ADD_PreAmpAudiopilotHfNoiseMicPsdLevel (0x24280009U)
#define TSP_SUB_PreAmpAudiopilotHfNoiseMicPsdLevel (0x24290009U)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoiseMicPsdLevel (0x242a0009U)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoiseMicPsdLevel (0x242b0009U)
#define TSP_ADD_PreAmpAudiopilotHfNoisePowerSpectralDensity (0x2428000bU)
#define TSP_SUB_PreAmpAudiopilotHfNoisePowerSpectralDensity (0x2429000bU)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoisePowerSpectralDensity (0x242a000bU)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoisePowerSpectralDensity (0x242b000bU)
#define TSP_ADD_PreAmpAudiopilotHfNoisePostSlew (0x2428000aU)
#define TSP_SUB_PreAmpAudiopilotHfNoisePostSlew (0x2429000aU)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoisePostSlew (0x242a000aU)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoisePostSlew (0x242b000aU)
#define TSP_ADD_PreAmpAudiopilotHfNoisePreSlew (0x2428000cU)
#define TSP_SUB_PreAmpAudiopilotHfNoisePreSlew (0x2429000cU)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoisePreSlew (0x242a000cU)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoisePreSlew (0x242b000cU)
#define TSP_ADD_PreAmpAudiopilotHfNoiseImpulseRejectionPmin (0x24280008U)
#define TSP_SUB_PreAmpAudiopilotHfNoiseImpulseRejectionPmin (0x24290008U)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoiseImpulseRejectionPmin (0x242a0008U)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoiseImpulseRejectionPmin (0x242b0008U)
#define TSP_ADD_PreAmpAudiopilotHfNoiseImpulseRejectionFlatness (0x24280007U)
#define TSP_SUB_PreAmpAudiopilotHfNoiseImpulseRejectionFlatness (0x24290007U)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoiseImpulseRejectionFlatness (0x242a0007U)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoiseImpulseRejectionFlatness (0x242b0007U)
#define TSP_ADD_PreAmpAudiopilotRatioPostSlew (0x24280021U)
#define TSP_SUB_PreAmpAudiopilotRatioPostSlew (0x24290021U)
#define TSP_RETRIEVE_PreAmpAudiopilotRatioPostSlew (0x242a0021U)
#define TSP_BLIND_READ_PreAmpAudiopilotRatioPostSlew (0x242b0021U)
#define TSP_ADD_PreAmpAudiopilotRatioPreSlew (0x24280022U)
#define TSP_SUB_PreAmpAudiopilotRatioPreSlew (0x24290022U)
#define TSP_RETRIEVE_PreAmpAudiopilotRatioPreSlew (0x242a0022U)
#define TSP_BLIND_READ_PreAmpAudiopilotRatioPreSlew (0x242b0022U)
#define TSP_ADD_PreAmpAudiopilotRatioUpdateFlag (0x24280023U)
#define TSP_SUB_PreAmpAudiopilotRatioUpdateFlag (0x24290023U)
#define TSP_RETRIEVE_PreAmpAudiopilotRatioUpdateFlag (0x242a0023U)
#define TSP_BLIND_READ_PreAmpAudiopilotRatioUpdateFlag (0x242b0023U)
#define TSP_ADD_PreAmpAudiopilotLfNoisePostSlew (0x2428001aU)
#define TSP_SUB_PreAmpAudiopilotLfNoisePostSlew (0x2429001aU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoisePostSlew (0x242a001aU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoisePostSlew (0x242b001aU)
#define TSP_ADD_PreAmpAudiopilotLfNoisePreSlew (0x2428001bU)
#define TSP_SUB_PreAmpAudiopilotLfNoisePreSlew (0x2429001bU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoisePreSlew (0x242a001bU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoisePreSlew (0x242b001bU)
#define TSP_ADD_PreAmpAudiopilotLfNoisePreSpeedBound (0x2428001cU)
#define TSP_SUB_PreAmpAudiopilotLfNoisePreSpeedBound (0x2429001cU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoisePreSpeedBound (0x242a001cU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoisePreSpeedBound (0x242b001cU)
#define TSP_ADD_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMax (0x2428001dU)
#define TSP_SUB_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMax (0x2429001dU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMax (0x242a001dU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMax (0x242b001dU)
#define TSP_ADD_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMin (0x2428001eU)
#define TSP_SUB_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMin (0x2429001eU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMin (0x242a001eU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMin (0x242b001eU)
#define TSP_ADD_PreAmpAudiopilotLfNoiseSpeedBoundsFlag (0x2428001fU)
#define TSP_SUB_PreAmpAudiopilotLfNoiseSpeedBoundsFlag (0x2429001fU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseSpeedBoundsFlag (0x242a001fU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseSpeedBoundsFlag (0x242b001fU)
#define TSP_ADD_PreAmpAudiopilotLfNoiseMicLevel (0x24280016U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseMicLevel (0x24290016U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseMicLevel (0x242a0016U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseMicLevel (0x242b0016U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseMicOut (0x24280017U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseMicOut (0x24290017U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseMicOut (0x242a0017U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseMicOut (0x242b0017U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseMusicOut (0x24280018U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseMusicOut (0x24290018U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseMusicOut (0x242a0018U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseMusicOut (0x242b0018U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseNoiseOut (0x24280019U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseNoiseOut (0x24290019U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseNoiseOut (0x242a0019U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseNoiseOut (0x242b0019U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionFilterOut (0x24280011U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionFilterOut (0x24290011U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionFilterOut (0x242a0011U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionFilterOut (0x242b0011U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionFiltCoef (0x24280010U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionFiltCoef (0x24290010U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionFiltCoef (0x242a0010U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionFiltCoef (0x242b0010U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionImpulseVariance (0x24280012U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionImpulseVariance (0x24290012U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionImpulseVariance (0x242a0012U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionImpulseVariance (0x242b0012U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionMusicVariance (0x24280013U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionMusicVariance (0x24290013U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionMusicVariance (0x242a0013U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionMusicVariance (0x242b0013U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionNoiseVariance (0x24280014U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionNoiseVariance (0x24290014U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionNoiseVariance (0x242a0014U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionNoiseVariance (0x242b0014U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionTotalVariance (0x24280015U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionTotalVariance (0x24290015U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionTotalVariance (0x242a0015U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionTotalVariance (0x242b0015U)

#define Model_1_3_TSPID_CAPTURE        (0x200d0000U)
#define Model_1_3_TSPID_CLEAR          (0x200c0000U)
#define TSP_ADD_PreAmpMedusaSettingsDummyState (0x23280000U)
#define TSP_SUB_PreAmpMedusaSettingsDummyState (0x23290000U)
#define TSP_RETRIEVE_PreAmpMedusaSettingsDummyState (0x232a0000U)
#define TSP_BLIND_READ_PreAmpMedusaSettingsDummyState (0x232b0000U)
#define TSP_ADD_PreAmpMedusaSettingsMonoDetectorLevelCenter (0x25280002U)
#define TSP_SUB_PreAmpMedusaSettingsMonoDetectorLevelCenter (0x25290002U)
#define TSP_RETRIEVE_PreAmpMedusaSettingsMonoDetectorLevelCenter (0x252a0002U)
#define TSP_BLIND_READ_PreAmpMedusaSettingsMonoDetectorLevelCenter (0x252b0002U)
#define TSP_ADD_PreAmpMedusaSettingsMonoDetectorIsStereo (0x25280001U)
#define TSP_SUB_PreAmpMedusaSettingsMonoDetectorIsStereo (0x25290001U)
#define TSP_RETRIEVE_PreAmpMedusaSettingsMonoDetectorIsStereo (0x252a0001U)
#define TSP_BLIND_READ_PreAmpMedusaSettingsMonoDetectorIsStereo (0x252b0001U)
#define TSP_ADD_PreAmpMedusaSettingsMonoDetectorIsActive (0x25280000U)
#define TSP_SUB_PreAmpMedusaSettingsMonoDetectorIsActive (0x25290000U)
#define TSP_RETRIEVE_PreAmpMedusaSettingsMonoDetectorIsActive (0x252a0000U)
#define TSP_BLIND_READ_PreAmpMedusaSettingsMonoDetectorIsActive (0x252b0000U)

#define Model_1_7_TSPID_CAPTURE        (0x200d0000U)
#define Model_1_7_TSPID_CLEAR          (0x200c0000U)
#define TSP_ADD_PostProcessEnergyLimiterConfigComputedEnergy (0x20380000U)
#define TSP_SUB_PostProcessEnergyLimiterConfigComputedEnergy (0x20390000U)
#define TSP_RETRIEVE_PostProcessEnergyLimiterConfigComputedEnergy (0x203a0000U)
#define TSP_BLIND_READ_PostProcessEnergyLimiterConfigComputedEnergy (0x203b0000U)
#define TSP_ADD_PostProcessEnergyLimiterConfigCurGain (0x20380001U)
#define TSP_SUB_PostProcessEnergyLimiterConfigCurGain (0x20390001U)
#define TSP_RETRIEVE_PostProcessEnergyLimiterConfigCurGain (0x203a0001U)
#define TSP_BLIND_READ_PostProcessEnergyLimiterConfigCurGain (0x203b0001U)
#define TSP_ADD_PostProcessEnergyLimiterConfigEnergyThreshold (0x20380004U)
#define TSP_SUB_PostProcessEnergyLimiterConfigEnergyThreshold (0x20390004U)
#define TSP_RETRIEVE_PostProcessEnergyLimiterConfigEnergyThreshold (0x203a0004U)
#define TSP_BLIND_READ_PostProcessEnergyLimiterConfigEnergyThreshold (0x203b0004U)
#define TSP_ADD_PostProcessEnergyLimiterConfigCurState (0x20380002U)
#define TSP_SUB_PostProcessEnergyLimiterConfigCurState (0x20390002U)
#define TSP_RETRIEVE_PostProcessEnergyLimiterConfigCurState (0x203a0002U)
#define TSP_BLIND_READ_PostProcessEnergyLimiterConfigCurState (0x203b0002U)
#define TSP_ADD_PostProcessEnergyLimiterConfigEnableFlag (0x20380003U)
#define TSP_SUB_PostProcessEnergyLimiterConfigEnableFlag (0x20390003U)
#define TSP_RETRIEVE_PostProcessEnergyLimiterConfigEnableFlag (0x203a0003U)
#define TSP_BLIND_READ_PostProcessEnergyLimiterConfigEnableFlag (0x203b0003U)

#define Model_1_10_TSPID_CAPTURE       (0x200d0000U)
#define Model_1_10_TSPID_CLEAR         (0x200c0000U)
#define TSP_ADD_MicInRMS               (0x20680000U)
#define TSP_SUB_MicInRMS               (0x20690000U)
#define TSP_RETRIEVE_MicInRMS          (0x206a0000U)
#define TSP_BLIND_READ_MicInRMS        (0x206b0000U)


// ------------------------------------------------------------
// TSP REQUEST PAYLOADS
// ------------------------------------------------------------
typedef struct TspListAddReqTag
{
    uint32_t msgId;                    // add token for TSP
    uint32_t callerHandle;             // startChar:0 charSize:4
    uint32_t captureCount;             // startChar:4 charSize:4
    uint32_t skipInterval;             // startChar:8 charSize:4
}

TspListAddReqType;

typedef struct TspListSubReqTag
{
    uint32_t msgId;                    // sub token for TSP
    uint32_t callerHandle;             // startChar:0 charSize:4
}
TspListSubReqType;

typedef struct TspListClearReqTag
{
    uint32_t msgId;                    // TSPID_CLEAR - send to desired core
    uint32_t callerHandle;             // startChar:0 charSize:4
}
TspListClearReqType;

typedef struct TspCaptureReqTag
{
    uint32_t msgId;                    // TSPID_CAPTURE
    uint32_t callerHandle;             // startChar:0 charSize:4
}
TspCaptureReqType;

typedef struct TspRetrieveReqTag
{
    uint32_t msgId;                    // retrieve token for TSP
    uint32_t callerHandle;             // startChar:0 charSize:4
    uint32_t captureIndex;             // startChar:4 charSize:4
    uint32_t charOffset;               // startChar:8 charSize:4
    uint32_t charCount;                // startChar:12 charSize:4
}
TspRetrieveReqType;

typedef struct TspBlindReadReqTag
{
    uint32_t msgId;                    // blind read token for TSP
    uint32_t callerHandle;             // startChar:0 charSize:4
    uint32_t charOffset;               // startChar:8 charSize:4
    uint32_t charCount;                // startChar:12 charSize:4
}
TspBlindReadReqType;

typedef struct TspBlindWriteReqTag
{
    uint32_t msgId;                    // blind read token for TSP
    uint32_t callerHandle;             // startChar:0 charSize:4
    uint32_t charOffset;               // startChar:8 charSize:4
    uint32_t charCount;                // startChar:12 charSize:4
    int8_t data[];
}
TspBlindWriteReqType;


// ------------------------------------------------------------
// TSP RESPONSE PAYLOADS
// ------------------------------------------------------------
typedef struct TspListAddRspTag
{
    uint32_t msgId;                    // list add token for TSP
    uint32_t callerHandle;             // startChar:0 charSize:4
    int32_t statusCode;                // startChar:4 charSize:4
}
TspListAddRspType;

typedef struct TspListSubRspTag
{
    uint32_t msgId;                    // list sub token for TSP
    uint32_t callerHandle;             // startChar:0 charSize:4
    int32_t statusCode;                // startChar:4 charSize:4
}
TspListSubRspType;

typedef struct TspListClearRspTag
{
    uint32_t msgId;                    // TSPID_CLEAR
    uint32_t callerHandle;             // startChar:0 charSize:4
    int32_t statusCode;                // startChar:4 charSize:4
}
TspListClearRspType;

typedef struct TspCaptureRspTag
{
    uint32_t msgId;                    // TSPID_CAPTURE
    uint32_t callerHandle;             // startChar:0 charSize:4
    int32_t statusCode;                // startChar:4 charSize:4
}
TspCaptureRspType;

typedef struct TspRetrieveRspTag
{
    uint32_t msgId;                    // retrieve token for TSP
    uint32_t callerHandle;             // startChar:0 charSize:4
    int32_t statusCode;                // startChar:4 charSize:4
    uint32_t charCount;                // startChar:8 charSize:4
    int8_t data[];
}
TspRetrieveRspType;

typedef struct TspBlindReadRspTag
{
    uint32_t msgId;                    // blind read token for TSP
    uint32_t callerHandle;             // startChar:0 charSize:4
    int32_t statusCode;                // startChar:4 charSize:4
    uint32_t charCount;                // startChar:8 charSize:4
    int8_t data[];
}
TspBlindReadRspType;

typedef struct TspBlindWriteRspTag
{
    uint32_t msgId;                    // blind read token for TSP
    uint32_t callerHandle;             // startChar:0 charSize:4
    int32_t statusCode;                // startChar:4 charSize:4
}
TspBlindWriteRspType;


// ------------------------------------------------------------
// TSP NOTIFICATION PAYLOADS
// ------------------------------------------------------------
typedef struct TspCaptureNtfTag
{
    uint32_t msgId;                    // TSPID_CAPTURE
    uint32_t callerHandle;             // startChar:0 charSize:4
    int32_t statusCode;     // startChar:4 charSize:4 ASDSTATUS_RTC_NOTIFICATION
    uint32_t totalCaptureCount;        // startChar:8 charSize:4
}
TspCaptureNtfType;

#endif

