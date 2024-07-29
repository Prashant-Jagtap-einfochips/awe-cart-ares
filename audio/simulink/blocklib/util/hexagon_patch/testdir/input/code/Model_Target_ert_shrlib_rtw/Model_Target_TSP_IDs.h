/*
 * File: Model_Target_TSP_IDs.h
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

#ifndef RTW_HEADER_Model_Target_TSP_IDs_h_
#define RTW_HEADER_Model_Target_TSP_IDs_h_
#include <stdint.h>

// ------------------------------------------------------------
// TSP COMMAND INFO CODES FOUND INSIDE TOKENS
// ------------------------------------------------------------
#define TSP_COMMAND_LIST_ADD           (8U)
#define TSP_COMMAND_LIST_SUB           (9U)
#define TSP_COMMAND_LIST_CLEAR         (12U)
#define TSP_COMMAND_CAPTURE            (13U)
#define TSP_COMMAND_RETRIEVE           (10U)
#define TSP_COMMAND_BLIND_READ         (11U)

// ------------------------------------------------------------
// TSP ACCESS TOKENS
// ------------------------------------------------------------
#define Model_Target_TSPID_CAPTURE     (0x10fd0000U)
#define Model_Target_TSPID_CLEAR       (0x10fc0000U)
#define TSP_ADD_NearAnn1Ann1NonEntGainCalcGetNonEntBoostLeveldB (0x10f80000U)
#define TSP_SUB_NearAnn1Ann1NonEntGainCalcGetNonEntBoostLeveldB (0x10f90000U)
#define TSP_RETRIEVE_NearAnn1Ann1NonEntGainCalcGetNonEntBoostLeveldB (0x10fa0000U)
#define TSP_BLIND_READ_NearAnn1Ann1NonEntGainCalcGetNonEntBoostLeveldB (0x10fb0000U)
#define TSP_ADD_NearAnn2Ann2NonEntGainCalcGetNonEntBoostLeveldB (0x10f80001U)
#define TSP_SUB_NearAnn2Ann2NonEntGainCalcGetNonEntBoostLeveldB (0x10f90001U)
#define TSP_RETRIEVE_NearAnn2Ann2NonEntGainCalcGetNonEntBoostLeveldB (0x10fa0001U)
#define TSP_BLIND_READ_NearAnn2Ann2NonEntGainCalcGetNonEntBoostLeveldB (0x10fb0001U)
#define TSP_ADD_NearAnn3Ann3NonEntGainCalcGetNonEntBoostLeveldB (0x10f80002U)
#define TSP_SUB_NearAnn3Ann3NonEntGainCalcGetNonEntBoostLeveldB (0x10f90002U)
#define TSP_RETRIEVE_NearAnn3Ann3NonEntGainCalcGetNonEntBoostLeveldB (0x10fa0002U)
#define TSP_BLIND_READ_NearAnn3Ann3NonEntGainCalcGetNonEntBoostLeveldB (0x10fb0002U)
#define TSP_ADD_NearAnn4Ann4NonEntGainCalcGetNonEntBoostLeveldB (0x10f80003U)
#define TSP_SUB_NearAnn4Ann4NonEntGainCalcGetNonEntBoostLeveldB (0x10f90003U)
#define TSP_RETRIEVE_NearAnn4Ann4NonEntGainCalcGetNonEntBoostLeveldB (0x10fa0003U)
#define TSP_BLIND_READ_NearAnn4Ann4NonEntGainCalcGetNonEntBoostLeveldB (0x10fb0003U)
#define TSP_ADD_PostProcessEnergyLimiterConfigComputedEnergy (0x10f80004U)
#define TSP_SUB_PostProcessEnergyLimiterConfigComputedEnergy (0x10f90004U)
#define TSP_RETRIEVE_PostProcessEnergyLimiterConfigComputedEnergy (0x10fa0004U)
#define TSP_BLIND_READ_PostProcessEnergyLimiterConfigComputedEnergy (0x10fb0004U)
#define TSP_ADD_PostProcessEnergyLimiterConfigCurGain (0x10f80005U)
#define TSP_SUB_PostProcessEnergyLimiterConfigCurGain (0x10f90005U)
#define TSP_RETRIEVE_PostProcessEnergyLimiterConfigCurGain (0x10fa0005U)
#define TSP_BLIND_READ_PostProcessEnergyLimiterConfigCurGain (0x10fb0005U)
#define TSP_ADD_PostProcessEnergyLimiterConfigEnergyThreshold (0x10f80008U)
#define TSP_SUB_PostProcessEnergyLimiterConfigEnergyThreshold (0x10f90008U)
#define TSP_RETRIEVE_PostProcessEnergyLimiterConfigEnergyThreshold (0x10fa0008U)
#define TSP_BLIND_READ_PostProcessEnergyLimiterConfigEnergyThreshold (0x10fb0008U)
#define TSP_ADD_PostProcessEnergyLimiterConfigCurState (0x10f80006U)
#define TSP_SUB_PostProcessEnergyLimiterConfigCurState (0x10f90006U)
#define TSP_RETRIEVE_PostProcessEnergyLimiterConfigCurState (0x10fa0006U)
#define TSP_BLIND_READ_PostProcessEnergyLimiterConfigCurState (0x10fb0006U)
#define TSP_ADD_PostProcessEnergyLimiterConfigEnableFlag (0x10f80007U)
#define TSP_SUB_PostProcessEnergyLimiterConfigEnableFlag (0x10f90007U)
#define TSP_RETRIEVE_PostProcessEnergyLimiterConfigEnableFlag (0x10fa0007U)
#define TSP_BLIND_READ_PostProcessEnergyLimiterConfigEnableFlag (0x10fb0007U)
#define TSP_ADD_PreAmpAudiopilotVehicleSpeed (0x10f8002eU)
#define TSP_SUB_PreAmpAudiopilotVehicleSpeed (0x10f9002eU)
#define TSP_RETRIEVE_PreAmpAudiopilotVehicleSpeed (0x10fa002eU)
#define TSP_BLIND_READ_PreAmpAudiopilotVehicleSpeed (0x10fb002eU)
#define TSP_ADD_PreAmpAudiopilotHvacDbspl (0x10f80016U)
#define TSP_SUB_PreAmpAudiopilotHvacDbspl (0x10f90016U)
#define TSP_RETRIEVE_PreAmpAudiopilotHvacDbspl (0x10fa0016U)
#define TSP_BLIND_READ_PreAmpAudiopilotHvacDbspl (0x10fb0016U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseDbspl (0x10f80018U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseDbspl (0x10f90018U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseDbspl (0x10fa0018U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseDbspl (0x10fb0018U)
#define TSP_ADD_PreAmpAudiopilotWbNoiseDbspl (0x10f8002fU)
#define TSP_SUB_PreAmpAudiopilotWbNoiseDbspl (0x10f9002fU)
#define TSP_RETRIEVE_PreAmpAudiopilotWbNoiseDbspl (0x10fa002fU)
#define TSP_BLIND_READ_PreAmpAudiopilotWbNoiseDbspl (0x10fb002fU)
#define TSP_ADD_PreAmpAudiopilotHfNoiseDbspl (0x10f8000fU)
#define TSP_SUB_PreAmpAudiopilotHfNoiseDbspl (0x10f9000fU)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoiseDbspl (0x10fa000fU)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoiseDbspl (0x10fb000fU)
#define TSP_ADD_PreAmpAudiopilotNoiseRatio (0x10f80029U)
#define TSP_SUB_PreAmpAudiopilotNoiseRatio (0x10f90029U)
#define TSP_RETRIEVE_PreAmpAudiopilotNoiseRatio (0x10fa0029U)
#define TSP_BLIND_READ_PreAmpAudiopilotNoiseRatio (0x10fb0029U)
#define TSP_ADD_PreAmpAudiopilotTotalBassBoostDb (0x10f8002dU)
#define TSP_SUB_PreAmpAudiopilotTotalBassBoostDb (0x10f9002dU)
#define TSP_RETRIEVE_PreAmpAudiopilotTotalBassBoostDb (0x10fa002dU)
#define TSP_BLIND_READ_PreAmpAudiopilotTotalBassBoostDb (0x10fb002dU)
#define TSP_ADD_PreAmpAudiopilotAudiopilotMidrangeBoostDb (0x10f8000bU)
#define TSP_SUB_PreAmpAudiopilotAudiopilotMidrangeBoostDb (0x10f9000bU)
#define TSP_RETRIEVE_PreAmpAudiopilotAudiopilotMidrangeBoostDb (0x10fa000bU)
#define TSP_BLIND_READ_PreAmpAudiopilotAudiopilotMidrangeBoostDb (0x10fb000bU)
#define TSP_ADD_PreAmpAudiopilotAudiopilotTrebleBoostDb (0x10f8000cU)
#define TSP_SUB_PreAmpAudiopilotAudiopilotTrebleBoostDb (0x10f9000cU)
#define TSP_RETRIEVE_PreAmpAudiopilotAudiopilotTrebleBoostDb (0x10fa000cU)
#define TSP_BLIND_READ_PreAmpAudiopilotAudiopilotTrebleBoostDb (0x10fb000cU)
#define TSP_ADD_PreAmpAudiopilotDyneqBassBoostDb (0x10f8000dU)
#define TSP_SUB_PreAmpAudiopilotDyneqBassBoostDb (0x10f9000dU)
#define TSP_RETRIEVE_PreAmpAudiopilotDyneqBassBoostDb (0x10fa000dU)
#define TSP_BLIND_READ_PreAmpAudiopilotDyneqBassBoostDb (0x10fb000dU)
#define TSP_ADD_PreAmpAudiopilotAudiopilotBassBoostDb (0x10f80009U)
#define TSP_SUB_PreAmpAudiopilotAudiopilotBassBoostDb (0x10f90009U)
#define TSP_RETRIEVE_PreAmpAudiopilotAudiopilotBassBoostDb (0x10fa0009U)
#define TSP_BLIND_READ_PreAmpAudiopilotAudiopilotBassBoostDb (0x10fb0009U)
#define TSP_ADD_PreAmpAudiopilotIgnoreMicFlag (0x10f80017U)
#define TSP_SUB_PreAmpAudiopilotIgnoreMicFlag (0x10f90017U)
#define TSP_RETRIEVE_PreAmpAudiopilotIgnoreMicFlag (0x10fa0017U)
#define TSP_BLIND_READ_PreAmpAudiopilotIgnoreMicFlag (0x10fb0017U)
#define TSP_ADD_PreAmpAudiopilotAudiopilotBoostGain (0x10f8000aU)
#define TSP_SUB_PreAmpAudiopilotAudiopilotBoostGain (0x10f9000aU)
#define TSP_RETRIEVE_PreAmpAudiopilotAudiopilotBoostGain (0x10fa000aU)
#define TSP_BLIND_READ_PreAmpAudiopilotAudiopilotBoostGain (0x10fb000aU)
#define TSP_ADD_PreAmpAudiopilotDyneqBoostGain (0x10f8000eU)
#define TSP_SUB_PreAmpAudiopilotDyneqBoostGain (0x10f9000eU)
#define TSP_RETRIEVE_PreAmpAudiopilotDyneqBoostGain (0x10fa000eU)
#define TSP_BLIND_READ_PreAmpAudiopilotDyneqBoostGain (0x10fb000eU)
#define TSP_ADD_PreAmpLevelWatchAudiopilotEnvDb (0x10f80030U)
#define TSP_SUB_PreAmpLevelWatchAudiopilotEnvDb (0x10f90030U)
#define TSP_RETRIEVE_PreAmpLevelWatchAudiopilotEnvDb (0x10fa0030U)
#define TSP_BLIND_READ_PreAmpLevelWatchAudiopilotEnvDb (0x10fb0030U)
#define TSP_ADD_PreAmpLevelWatchAudiopilotLevelDbspl (0x10f80031U)
#define TSP_SUB_PreAmpLevelWatchAudiopilotLevelDbspl (0x10f90031U)
#define TSP_RETRIEVE_PreAmpLevelWatchAudiopilotLevelDbspl (0x10fa0031U)
#define TSP_BLIND_READ_PreAmpLevelWatchAudiopilotLevelDbspl (0x10fb0031U)
#define TSP_ADD_PreAmpLevelWatchDyneqEnvDb (0x10f80032U)
#define TSP_SUB_PreAmpLevelWatchDyneqEnvDb (0x10f90032U)
#define TSP_RETRIEVE_PreAmpLevelWatchDyneqEnvDb (0x10fa0032U)
#define TSP_BLIND_READ_PreAmpLevelWatchDyneqEnvDb (0x10fb0032U)
#define TSP_ADD_PreAmpLevelWatchDyneqLevelDbspl (0x10f80033U)
#define TSP_SUB_PreAmpLevelWatchDyneqLevelDbspl (0x10f90033U)
#define TSP_RETRIEVE_PreAmpLevelWatchDyneqLevelDbspl (0x10fa0033U)
#define TSP_BLIND_READ_PreAmpLevelWatchDyneqLevelDbspl (0x10fb0033U)
#define TSP_ADD_PreAmpAudiopilotHfNoiseMicPsdLevel (0x10f80012U)
#define TSP_SUB_PreAmpAudiopilotHfNoiseMicPsdLevel (0x10f90012U)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoiseMicPsdLevel (0x10fa0012U)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoiseMicPsdLevel (0x10fb0012U)
#define TSP_ADD_PreAmpAudiopilotHfNoisePowerSpectralDensity (0x10f80014U)
#define TSP_SUB_PreAmpAudiopilotHfNoisePowerSpectralDensity (0x10f90014U)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoisePowerSpectralDensity (0x10fa0014U)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoisePowerSpectralDensity (0x10fb0014U)
#define TSP_ADD_PreAmpAudiopilotHfNoisePostSlew (0x10f80013U)
#define TSP_SUB_PreAmpAudiopilotHfNoisePostSlew (0x10f90013U)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoisePostSlew (0x10fa0013U)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoisePostSlew (0x10fb0013U)
#define TSP_ADD_PreAmpAudiopilotHfNoisePreSlew (0x10f80015U)
#define TSP_SUB_PreAmpAudiopilotHfNoisePreSlew (0x10f90015U)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoisePreSlew (0x10fa0015U)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoisePreSlew (0x10fb0015U)
#define TSP_ADD_PreAmpAudiopilotHfNoiseImpulseRejectionPmin (0x10f80011U)
#define TSP_SUB_PreAmpAudiopilotHfNoiseImpulseRejectionPmin (0x10f90011U)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoiseImpulseRejectionPmin (0x10fa0011U)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoiseImpulseRejectionPmin (0x10fb0011U)
#define TSP_ADD_PreAmpAudiopilotHfNoiseImpulseRejectionFlatness (0x10f80010U)
#define TSP_SUB_PreAmpAudiopilotHfNoiseImpulseRejectionFlatness (0x10f90010U)
#define TSP_RETRIEVE_PreAmpAudiopilotHfNoiseImpulseRejectionFlatness (0x10fa0010U)
#define TSP_BLIND_READ_PreAmpAudiopilotHfNoiseImpulseRejectionFlatness (0x10fb0010U)
#define TSP_ADD_PreAmpAudiopilotRatioPostSlew (0x10f8002aU)
#define TSP_SUB_PreAmpAudiopilotRatioPostSlew (0x10f9002aU)
#define TSP_RETRIEVE_PreAmpAudiopilotRatioPostSlew (0x10fa002aU)
#define TSP_BLIND_READ_PreAmpAudiopilotRatioPostSlew (0x10fb002aU)
#define TSP_ADD_PreAmpAudiopilotRatioPreSlew (0x10f8002bU)
#define TSP_SUB_PreAmpAudiopilotRatioPreSlew (0x10f9002bU)
#define TSP_RETRIEVE_PreAmpAudiopilotRatioPreSlew (0x10fa002bU)
#define TSP_BLIND_READ_PreAmpAudiopilotRatioPreSlew (0x10fb002bU)
#define TSP_ADD_PreAmpAudiopilotRatioUpdateFlag (0x10f8002cU)
#define TSP_SUB_PreAmpAudiopilotRatioUpdateFlag (0x10f9002cU)
#define TSP_RETRIEVE_PreAmpAudiopilotRatioUpdateFlag (0x10fa002cU)
#define TSP_BLIND_READ_PreAmpAudiopilotRatioUpdateFlag (0x10fb002cU)
#define TSP_ADD_PreAmpAudiopilotLfNoisePostSlew (0x10f80023U)
#define TSP_SUB_PreAmpAudiopilotLfNoisePostSlew (0x10f90023U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoisePostSlew (0x10fa0023U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoisePostSlew (0x10fb0023U)
#define TSP_ADD_PreAmpAudiopilotLfNoisePreSlew (0x10f80024U)
#define TSP_SUB_PreAmpAudiopilotLfNoisePreSlew (0x10f90024U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoisePreSlew (0x10fa0024U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoisePreSlew (0x10fb0024U)
#define TSP_ADD_PreAmpAudiopilotLfNoisePreSpeedBound (0x10f80025U)
#define TSP_SUB_PreAmpAudiopilotLfNoisePreSpeedBound (0x10f90025U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoisePreSpeedBound (0x10fa0025U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoisePreSpeedBound (0x10fb0025U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMax (0x10f80026U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMax (0x10f90026U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMax (0x10fa0026U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMax (0x10fb0026U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMin (0x10f80027U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMin (0x10f90027U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMin (0x10fa0027U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseSpeedBoundsCurrMin (0x10fb0027U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseSpeedBoundsFlag (0x10f80028U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseSpeedBoundsFlag (0x10f90028U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseSpeedBoundsFlag (0x10fa0028U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseSpeedBoundsFlag (0x10fb0028U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseMicLevel (0x10f8001fU)
#define TSP_SUB_PreAmpAudiopilotLfNoiseMicLevel (0x10f9001fU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseMicLevel (0x10fa001fU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseMicLevel (0x10fb001fU)
#define TSP_ADD_PreAmpAudiopilotLfNoiseMicOut (0x10f80020U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseMicOut (0x10f90020U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseMicOut (0x10fa0020U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseMicOut (0x10fb0020U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseMusicOut (0x10f80021U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseMusicOut (0x10f90021U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseMusicOut (0x10fa0021U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseMusicOut (0x10fb0021U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseNoiseOut (0x10f80022U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseNoiseOut (0x10f90022U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseNoiseOut (0x10fa0022U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseNoiseOut (0x10fb0022U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionFilterOut (0x10f8001aU)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionFilterOut (0x10f9001aU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionFilterOut (0x10fa001aU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionFilterOut (0x10fb001aU)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionFiltCoef (0x10f80019U)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionFiltCoef (0x10f90019U)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionFiltCoef (0x10fa0019U)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionFiltCoef (0x10fb0019U)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionImpulseVariance (0x10f8001bU)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionImpulseVariance (0x10f9001bU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionImpulseVariance (0x10fa001bU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionImpulseVariance (0x10fb001bU)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionMusicVariance (0x10f8001cU)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionMusicVariance (0x10f9001cU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionMusicVariance (0x10fa001cU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionMusicVariance (0x10fb001cU)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionNoiseVariance (0x10f8001dU)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionNoiseVariance (0x10f9001dU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionNoiseVariance (0x10fa001dU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionNoiseVariance (0x10fb001dU)
#define TSP_ADD_PreAmpAudiopilotLfNoiseImpulseRejectionTotalVariance (0x10f8001eU)
#define TSP_SUB_PreAmpAudiopilotLfNoiseImpulseRejectionTotalVariance (0x10f9001eU)
#define TSP_RETRIEVE_PreAmpAudiopilotLfNoiseImpulseRejectionTotalVariance (0x10fa001eU)
#define TSP_BLIND_READ_PreAmpAudiopilotLfNoiseImpulseRejectionTotalVariance (0x10fb001eU)
#define TSP_ADD_PreAmpMedusaDummyState (0x10f80034U)
#define TSP_SUB_PreAmpMedusaDummyState (0x10f90034U)
#define TSP_RETRIEVE_PreAmpMedusaDummyState (0x10fa0034U)
#define TSP_BLIND_READ_PreAmpMedusaDummyState (0x10fb0034U)
#define TSP_ADD_PreAmpMedusaMonoDetectorLevelCenter (0x10f80037U)
#define TSP_SUB_PreAmpMedusaMonoDetectorLevelCenter (0x10f90037U)
#define TSP_RETRIEVE_PreAmpMedusaMonoDetectorLevelCenter (0x10fa0037U)
#define TSP_BLIND_READ_PreAmpMedusaMonoDetectorLevelCenter (0x10fb0037U)
#define TSP_ADD_PreAmpMedusaMonoDetectorIsStereo (0x10f80036U)
#define TSP_SUB_PreAmpMedusaMonoDetectorIsStereo (0x10f90036U)
#define TSP_RETRIEVE_PreAmpMedusaMonoDetectorIsStereo (0x10fa0036U)
#define TSP_BLIND_READ_PreAmpMedusaMonoDetectorIsStereo (0x10fb0036U)
#define TSP_ADD_PreAmpMedusaMonoDetectorIsActive (0x10f80035U)
#define TSP_SUB_PreAmpMedusaMonoDetectorIsActive (0x10f90035U)
#define TSP_RETRIEVE_PreAmpMedusaMonoDetectorIsActive (0x10fa0035U)
#define TSP_BLIND_READ_PreAmpMedusaMonoDetectorIsActive (0x10fb0035U)

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

// ------------------------------------------------------------
// EMBEDDED XML FILE OF TSP TOKEN VALUES
// ------------------------------------------------------------
/* [XMLBEGIN]<?xml version="1.0" encoding="utf-8"?>
   <root version="1.0">
   <Core coreIndex="1" captureToken="0x10fd0000" clearToken="0x10fc0000"/>
   <TSP id="NearAnn1Ann1NonEntGainCalcGetNonEntBoostLeveldB" addToken="0x10f80000" subToken="0x10f90000" retrieveToken="0x10fa0000" blindReadToken="0x10fb0000" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="NearAnn2Ann2NonEntGainCalcGetNonEntBoostLeveldB" addToken="0x10f80001" subToken="0x10f90001" retrieveToken="0x10fa0001" blindReadToken="0x10fb0001" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="NearAnn3Ann3NonEntGainCalcGetNonEntBoostLeveldB" addToken="0x10f80002" subToken="0x10f90002" retrieveToken="0x10fa0002" blindReadToken="0x10fb0002" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="NearAnn4Ann4NonEntGainCalcGetNonEntBoostLeveldB" addToken="0x10f80003" subToken="0x10f90003" retrieveToken="0x10fa0003" blindReadToken="0x10fb0003" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PostProcessEnergyLimiterConfigComputedEnergy" addToken="0x10f80004" subToken="0x10f90004" retrieveToken="0x10fa0004" blindReadToken="0x10fb0004" dataType="single" width="24" dimensions="[1, 24]" coreIndex="0" coreTag=""/>
   <TSP id="PostProcessEnergyLimiterConfigCurGain" addToken="0x10f80005" subToken="0x10f90005" retrieveToken="0x10fa0005" blindReadToken="0x10fb0005" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PostProcessEnergyLimiterConfigEnergyThreshold" addToken="0x10f80008" subToken="0x10f90008" retrieveToken="0x10fa0008" blindReadToken="0x10fb0008" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PostProcessEnergyLimiterConfigCurState" addToken="0x10f80006" subToken="0x10f90006" retrieveToken="0x10fa0006" blindReadToken="0x10fb0006" dataType="int16" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PostProcessEnergyLimiterConfigEnableFlag" addToken="0x10f80007" subToken="0x10f90007" retrieveToken="0x10fa0007" blindReadToken="0x10fb0007" dataType="int16" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotVehicleSpeed" addToken="0x10f8002e" subToken="0x10f9002e" retrieveToken="0x10fa002e" blindReadToken="0x10fb002e" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotHvacDbspl" addToken="0x10f80016" subToken="0x10f90016" retrieveToken="0x10fa0016" blindReadToken="0x10fb0016" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseDbspl" addToken="0x10f80018" subToken="0x10f90018" retrieveToken="0x10fa0018" blindReadToken="0x10fb0018" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotWbNoiseDbspl" addToken="0x10f8002f" subToken="0x10f9002f" retrieveToken="0x10fa002f" blindReadToken="0x10fb002f" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotHfNoiseDbspl" addToken="0x10f8000f" subToken="0x10f9000f" retrieveToken="0x10fa000f" blindReadToken="0x10fb000f" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotNoiseRatio" addToken="0x10f80029" subToken="0x10f90029" retrieveToken="0x10fa0029" blindReadToken="0x10fb0029" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotTotalBassBoostDb" addToken="0x10f8002d" subToken="0x10f9002d" retrieveToken="0x10fa002d" blindReadToken="0x10fb002d" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotAudiopilotMidrangeBoostDb" addToken="0x10f8000b" subToken="0x10f9000b" retrieveToken="0x10fa000b" blindReadToken="0x10fb000b" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotAudiopilotTrebleBoostDb" addToken="0x10f8000c" subToken="0x10f9000c" retrieveToken="0x10fa000c" blindReadToken="0x10fb000c" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotDyneqBassBoostDb" addToken="0x10f8000d" subToken="0x10f9000d" retrieveToken="0x10fa000d" blindReadToken="0x10fb000d" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotAudiopilotBassBoostDb" addToken="0x10f80009" subToken="0x10f90009" retrieveToken="0x10fa0009" blindReadToken="0x10fb0009" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotIgnoreMicFlag" addToken="0x10f80017" subToken="0x10f90017" retrieveToken="0x10fa0017" blindReadToken="0x10fb0017" dataType="boolean" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotAudiopilotBoostGain" addToken="0x10f8000a" subToken="0x10f9000a" retrieveToken="0x10fa000a" blindReadToken="0x10fb000a" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotDyneqBoostGain" addToken="0x10f8000e" subToken="0x10f9000e" retrieveToken="0x10fa000e" blindReadToken="0x10fb000e" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpLevelWatchAudiopilotEnvDb" addToken="0x10f80030" subToken="0x10f90030" retrieveToken="0x10fa0030" blindReadToken="0x10fb0030" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpLevelWatchAudiopilotLevelDbspl" addToken="0x10f80031" subToken="0x10f90031" retrieveToken="0x10fa0031" blindReadToken="0x10fb0031" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpLevelWatchDyneqEnvDb" addToken="0x10f80032" subToken="0x10f90032" retrieveToken="0x10fa0032" blindReadToken="0x10fb0032" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpLevelWatchDyneqLevelDbspl" addToken="0x10f80033" subToken="0x10f90033" retrieveToken="0x10fa0033" blindReadToken="0x10fb0033" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotHfNoiseMicPsdLevel" addToken="0x10f80012" subToken="0x10f90012" retrieveToken="0x10fa0012" blindReadToken="0x10fb0012" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotHfNoisePowerSpectralDensity" addToken="0x10f80014" subToken="0x10f90014" retrieveToken="0x10fa0014" blindReadToken="0x10fb0014" dataType="single" width="65" dimensions="[65, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotHfNoisePostSlew" addToken="0x10f80013" subToken="0x10f90013" retrieveToken="0x10fa0013" blindReadToken="0x10fb0013" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotHfNoisePreSlew" addToken="0x10f80015" subToken="0x10f90015" retrieveToken="0x10fa0015" blindReadToken="0x10fb0015" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotHfNoiseImpulseRejectionPmin" addToken="0x10f80011" subToken="0x10f90011" retrieveToken="0x10fa0011" blindReadToken="0x10fb0011" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotHfNoiseImpulseRejectionFlatness" addToken="0x10f80010" subToken="0x10f90010" retrieveToken="0x10fa0010" blindReadToken="0x10fb0010" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotRatioPostSlew" addToken="0x10f8002a" subToken="0x10f9002a" retrieveToken="0x10fa002a" blindReadToken="0x10fb002a" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotRatioPreSlew" addToken="0x10f8002b" subToken="0x10f9002b" retrieveToken="0x10fa002b" blindReadToken="0x10fb002b" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotRatioUpdateFlag" addToken="0x10f8002c" subToken="0x10f9002c" retrieveToken="0x10fa002c" blindReadToken="0x10fb002c" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoisePostSlew" addToken="0x10f80023" subToken="0x10f90023" retrieveToken="0x10fa0023" blindReadToken="0x10fb0023" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoisePreSlew" addToken="0x10f80024" subToken="0x10f90024" retrieveToken="0x10fa0024" blindReadToken="0x10fb0024" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoisePreSpeedBound" addToken="0x10f80025" subToken="0x10f90025" retrieveToken="0x10fa0025" blindReadToken="0x10fb0025" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseSpeedBoundsCurrMax" addToken="0x10f80026" subToken="0x10f90026" retrieveToken="0x10fa0026" blindReadToken="0x10fb0026" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseSpeedBoundsCurrMin" addToken="0x10f80027" subToken="0x10f90027" retrieveToken="0x10fa0027" blindReadToken="0x10fb0027" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseSpeedBoundsFlag" addToken="0x10f80028" subToken="0x10f90028" retrieveToken="0x10fa0028" blindReadToken="0x10fb0028" dataType="double" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseMicLevel" addToken="0x10f8001f" subToken="0x10f9001f" retrieveToken="0x10fa001f" blindReadToken="0x10fb001f" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseMicOut" addToken="0x10f80020" subToken="0x10f90020" retrieveToken="0x10fa0020" blindReadToken="0x10fb0020" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseMusicOut" addToken="0x10f80021" subToken="0x10f90021" retrieveToken="0x10fa0021" blindReadToken="0x10fb0021" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseNoiseOut" addToken="0x10f80022" subToken="0x10f90022" retrieveToken="0x10fa0022" blindReadToken="0x10fb0022" dataType="single" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseImpulseRejectionFilterOut" addToken="0x10f8001a" subToken="0x10f9001a" retrieveToken="0x10fa001a" blindReadToken="0x10fb001a" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseImpulseRejectionFiltCoef" addToken="0x10f80019" subToken="0x10f90019" retrieveToken="0x10fa0019" blindReadToken="0x10fb0019" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseImpulseRejectionImpulseVariance" addToken="0x10f8001b" subToken="0x10f9001b" retrieveToken="0x10fa001b" blindReadToken="0x10fb001b" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseImpulseRejectionMusicVariance" addToken="0x10f8001c" subToken="0x10f9001c" retrieveToken="0x10fa001c" blindReadToken="0x10fb001c" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseImpulseRejectionNoiseVariance" addToken="0x10f8001d" subToken="0x10f9001d" retrieveToken="0x10fa001d" blindReadToken="0x10fb001d" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpAudiopilotLfNoiseImpulseRejectionTotalVariance" addToken="0x10f8001e" subToken="0x10f9001e" retrieveToken="0x10fa001e" blindReadToken="0x10fb001e" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpMedusaDummyState" addToken="0x10f80034" subToken="0x10f90034" retrieveToken="0x10fa0034" blindReadToken="0x10fb0034" dataType="int32" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpMedusaMonoDetectorLevelCenter" addToken="0x10f80037" subToken="0x10f90037" retrieveToken="0x10fa0037" blindReadToken="0x10fb0037" dataType="single" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpMedusaMonoDetectorIsStereo" addToken="0x10f80036" subToken="0x10f90036" retrieveToken="0x10fa0036" blindReadToken="0x10fb0036" dataType="uint32" width="1" dimensions="1" coreIndex="0" coreTag=""/>
   <TSP id="PreAmpMedusaMonoDetectorIsActive" addToken="0x10f80035" subToken="0x10f90035" retrieveToken="0x10fa0035" blindReadToken="0x10fb0035" dataType="int32" width="1" dimensions="[1, 1]" coreIndex="0" coreTag=""/>
   </root>
   [XMLEND] */
#endif                                 /* RTW_HEADER_Model_Target_TSP_IDs_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
