/*
 * File: Model_Target_TOP.h
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

#ifndef RTW_HEADER_Model_Target_TOP_h_
#define RTW_HEADER_Model_Target_TOP_h_
#include "rtwtypes.h"

typedef struct Model_Target_InputSelect0_tag
{
    int32_T RouterCfgOutMap[12];       /* '<S12>/TOP' */
}
Model_Target_InputSelect0_type;

#define TOP_BANK_Model_Target_InputSelect0 &Model_Target_InputSelect0
#define TOP_BANK_Model_Target_InputSelect0_SIZE sizeof(Model_Target_InputSelect0_type)

typedef struct Model_Target_Near0_tag
{
    int32_T Dr_EqDelays[7];            /* '<S41>/TOP' */
    int32_T Dr_RouterOutMap[18];       /* '<S63>/TOP' */
    int32_T Fr_EqDelays[7];            /* '<S85>/TOP' */
    int32_T Fr_RouterOutMap[18];       /* '<S107>/TOP' */
    real32_T Ann1Ann1MuteRampTime;     /* '<S271>/TOP' */
    real32_T Ann1Ann1NonEntGainCalcBoostOverRide;/* '<S246>/TOP' */
    real32_T Ann1Ann1NonEntGainCalcBoostOverRideFlag;/* '<S247>/TOP' */
    real32_T Ann1Ann1NonEntGainCalcDen[2];/* '<S249>/den' */
    real32_T Ann1Ann1NonEntGainCalcMaxBoost;/* '<S243>/TOP' */
    real32_T Ann1Ann1NonEntGainCalcMaxSpl;/* '<S262>/TOP' */
    real32_T Ann1Ann1NonEntGainCalcMinVncSignal;/* '<S261>/TOP' */
    real32_T Ann1Ann1NonEntGainCalcNoiseOverRide;/* '<S244>/TOP' */
    real32_T Ann1Ann1NonEntGainCalcNoiseOverRideFlag;/* '<S245>/TOP' */
    real32_T Ann1Ann1NonEntGainCalcNum[2];/* '<S249>/num' */
    real32_T Ann1Ann1NonEntGainCalcVncEnableTuneVar;/* '<S248>/TOP' */
    real32_T Ann1Ann1NonEntGainCalcVncSlope;/* '<S242>/TOP' */
    real32_T Ann1Ann1NonEntGainCalcVncThresh;/* '<S241>/TOP' */
    real32_T Ann1Ann1NonEntVncAppliedNum[2];/* '<S266>/num' */
    real32_T Ann1Ann1NonEntVncAppliedden[2];/* '<S266>/den' */
    real32_T Ann1Ann1VolumeRampRate;   /* '<S294>/TOP' */
    real32_T Ann1Ann1VolumeRampTime;   /* '<S295>/TOP' */
    real32_T Ann1Ann1VolumeTable_dB[16];/* '<S238>/TOP_GainDB' */
    real32_T Ann1LevelDetectFastDecayDbPerSample;/* '<S198>/TOP' */
    real32_T Ann1LevelDetectHoldTimeSamples;/* '<S200>/TOP' */
    real32_T Ann1LevelDetectMaxVolumeDb;/* '<S201>/TOP' */
    real32_T Ann1LevelDetectMinVolumeDb;/* '<S202>/TOP' */
    real32_T Ann1LevelDetectSlowDecayDbPerSample;/* '<S203>/TOP' */
    real32_T Ann1LevelDetecthold_margin_db;/* '<S199>/TOP' */
    real32_T Ann2Ann2MuteRampTime;     /* '<S351>/TOP' */
    real32_T Ann2Ann2NonEntGainCalcBoostOverRide;/* '<S326>/TOP' */
    real32_T Ann2Ann2NonEntGainCalcBoostOverRideFlag;/* '<S327>/TOP' */
    real32_T Ann2Ann2NonEntGainCalcDen[2];/* '<S329>/den' */
    real32_T Ann2Ann2NonEntGainCalcMaxBoost;/* '<S323>/TOP' */
    real32_T Ann2Ann2NonEntGainCalcMaxSpl;/* '<S342>/TOP' */
    real32_T Ann2Ann2NonEntGainCalcMinVncSignal;/* '<S341>/TOP' */
    real32_T Ann2Ann2NonEntGainCalcNoiseOverRide;/* '<S324>/TOP' */
    real32_T Ann2Ann2NonEntGainCalcNoiseOverRideFlag;/* '<S325>/TOP' */
    real32_T Ann2Ann2NonEntGainCalcNum[2];/* '<S329>/num' */
    real32_T Ann2Ann2NonEntGainCalcVncEnableTuneVar;/* '<S328>/TOP' */
    real32_T Ann2Ann2NonEntGainCalcVncSlope;/* '<S322>/TOP' */
    real32_T Ann2Ann2NonEntGainCalcVncThresh;/* '<S321>/TOP' */
    real32_T Ann2Ann2NonEntVncAppliedNum[2];/* '<S346>/num' */
    real32_T Ann2Ann2NonEntVncAppliedden[2];/* '<S346>/den' */
    real32_T Ann2Ann2VolumeRampRate;   /* '<S374>/TOP' */
    real32_T Ann2Ann2VolumeRampTime;   /* '<S375>/TOP' */
    real32_T Ann2Ann2VolumeTable_dB[16];/* '<S318>/TOP_GainDB' */
    real32_T Ann2LevelDetectFastDecayDbPerSample;/* '<S206>/TOP' */
    real32_T Ann2LevelDetectHoldMarginDb;/* '<S207>/TOP' */
    real32_T Ann2LevelDetectHoldTimeSamples;/* '<S208>/TOP' */
    real32_T Ann2LevelDetectMaxVolumeDb;/* '<S209>/TOP' */
    real32_T Ann2LevelDetectMinVolumeDb;/* '<S210>/TOP' */
    real32_T Ann2LevelDetectSlowDecayDbPerSample;/* '<S211>/TOP' */
    real32_T Ann3Ann3MuteRampTime;     /* '<S431>/TOP' */
    real32_T Ann3Ann3NonEntGainCalcBoostOverRide;/* '<S406>/TOP' */
    real32_T Ann3Ann3NonEntGainCalcBoostOverRideFlag;/* '<S407>/TOP' */
    real32_T Ann3Ann3NonEntGainCalcDen[2];/* '<S409>/den' */
    real32_T Ann3Ann3NonEntGainCalcMaxBoost;/* '<S403>/TOP' */
    real32_T Ann3Ann3NonEntGainCalcMaxSpl;/* '<S422>/TOP' */
    real32_T Ann3Ann3NonEntGainCalcMinVncSignal;/* '<S421>/TOP' */
    real32_T Ann3Ann3NonEntGainCalcNoiseOverRide;/* '<S404>/TOP' */
    real32_T Ann3Ann3NonEntGainCalcNoiseOverRideFlag;/* '<S405>/TOP' */
    real32_T Ann3Ann3NonEntGainCalcNum[2];/* '<S409>/num' */
    real32_T Ann3Ann3NonEntGainCalcVncEnableTuneVar;/* '<S408>/TOP' */
    real32_T Ann3Ann3NonEntGainCalcVncSlope;/* '<S402>/TOP' */
    real32_T Ann3Ann3NonEntGainCalcVncThresh;/* '<S401>/TOP' */
    real32_T Ann3Ann3NonEntVncAppliedNum[2];/* '<S426>/num' */
    real32_T Ann3Ann3NonEntVncAppliedden[2];/* '<S426>/den' */
    real32_T Ann3Ann3VolumeRampRate;   /* '<S454>/TOP' */
    real32_T Ann3Ann3VolumeRampTime;   /* '<S455>/TOP' */
    real32_T Ann3Ann3VolumeTable_dB[16];/* '<S398>/TOP_GainDB' */
    real32_T Ann3LevelDetectFastDecayDbPerSample;/* '<S214>/TOP' */
    real32_T Ann3LevelDetectHoldMarginDb;/* '<S215>/TOP' */
    real32_T Ann3LevelDetectHoldTimeSamples;/* '<S216>/TOP' */
    real32_T Ann3LevelDetectMaxVolumeDb;/* '<S217>/TOP' */
    real32_T Ann3LevelDetectMinVolumeDb;/* '<S218>/TOP' */
    real32_T Ann3LevelDetectSlowDecayDbPerSample;/* '<S219>/TOP' */
    real32_T Ann4Ann4MuteRampTime;     /* '<S511>/TOP' */
    real32_T Ann4Ann4NonEntGainCalcBoostOverRide;/* '<S486>/TOP' */
    real32_T Ann4Ann4NonEntGainCalcBoostOverRideFlag;/* '<S487>/TOP' */
    real32_T Ann4Ann4NonEntGainCalcDen[2];/* '<S489>/den' */
    real32_T Ann4Ann4NonEntGainCalcMaxBoost;/* '<S483>/TOP' */
    real32_T Ann4Ann4NonEntGainCalcMaxSpl;/* '<S502>/TOP' */
    real32_T Ann4Ann4NonEntGainCalcMinVncSignal;/* '<S501>/TOP' */
    real32_T Ann4Ann4NonEntGainCalcNoiseOverRide;/* '<S484>/TOP' */
    real32_T Ann4Ann4NonEntGainCalcNoiseOverRideFlag;/* '<S485>/TOP' */
    real32_T Ann4Ann4NonEntGainCalcNum[2];/* '<S489>/num' */
    real32_T Ann4Ann4NonEntGainCalcVncEnableTuneVar;/* '<S488>/TOP' */
    real32_T Ann4Ann4NonEntGainCalcVncSlope;/* '<S482>/TOP' */
    real32_T Ann4Ann4NonEntGainCalcVncThresh;/* '<S481>/TOP' */
    real32_T Ann4Ann4NonEntVncAppliedNum[2];/* '<S506>/num' */
    real32_T Ann4Ann4NonEntVncAppliedden[2];/* '<S506>/den' */
    real32_T Ann4Ann4VolumeRampRate;   /* '<S534>/TOP' */
    real32_T Ann4Ann4VolumeRampTime;   /* '<S535>/TOP' */
    real32_T Ann4Ann4VolumeTable_dB[16];/* '<S478>/TOP_GainDB' */
    real32_T Ann4LevelDetectFastDecayDbPerSample;/* '<S222>/TOP' */
    real32_T Ann4LevelDetectHoldMarginDb;/* '<S223>/TOP' */
    real32_T Ann4LevelDetectHoldTimeSamples;/* '<S224>/TOP' */
    real32_T Ann4LevelDetectMaxVolumeDb;/* '<S225>/TOP' */
    real32_T Ann4LevelDetectMinVolumeDb;/* '<S226>/TOP' */
    real32_T Ann4LevelDetectSlowDecayDbPerSample;/* '<S227>/TOP' */
    real32_T Coefficients[164];        /* '<S159>/TOP' */
    real32_T Dr_eqdelays[4];           /* '<S53>/TOP' */
    real32_T Dr_mix[875];              /* '<S76>/TOP' */
    real32_T Fr_PoolIirTunablepooliirCoeffs[404];/* '<S99>/TOP1' */
    real32_T Fr_PooliirpooliirCoeffs[405];/* '<S87>/TOP1' */
    real32_T Fr_eqdelays[4];           /* '<S97>/TOP' */
    real32_T Fr_mix[225];              /* '<S120>/TOP' */
    real32_T LevelDetectPreEmphasisShelfFilterpooliirCoeffs[10];/* '<S191>/TOP1' */
    real32_T LevelDetectSysMaxDbspl;   /* '<S190>/TOP' */
    real32_T RampDbPerSample;          /* '<S129>/TOP' */
    uint32_T Ann1Ann1VolumeTable_Idx[16];/* '<S238>/TOP_Index' */
    uint32_T Ann2Ann2VolumeTable_Idx[16];/* '<S318>/TOP_Index' */
    uint32_T Ann3Ann3VolumeTable_Idx[16];/* '<S398>/TOP_Index' */
    uint32_T Ann4Ann4VolumeTable_Idx[16];/* '<S478>/TOP_Index' */
    uint32_T Dr_PoolIirTunablePooliirNumStages[4];/* '<S55>/TOP' */
    uint32_T Dr_PooliirPooliirNumStages[7];/* '<S43>/TOP' */
    uint32_T Fr_PoolIirTunablePooliirNumStages[4];/* '<S99>/TOP' */
    uint32_T Fr_PooliirPooliirNumStages[5];/* '<S87>/TOP' */
    uint32_T LevelDetectPreEmphasisShelfFilterPooliirNumStages[2];/* '<S191>/TOP' */
    uint32_T NumStages[4];             /* '<S159>/TOP1' */
}
Model_Target_Near0_type;

#define TOP_BANK_Model_Target_Near0    &Model_Target_Near0
#define TOP_BANK_Model_Target_Near0_SIZE sizeof(Model_Target_Near0_type)

typedef struct Model_Target_PostProcess0_tag
{
    real32_T LimiterCfgattack[24];     /* '<S599>/attack' */
    real32_T LimiterCfgattack_low[24]; /* '<S599>/attack1' */
    real32_T LimiterCfgdecay[24];      /* '<S599>/decay' */
    real32_T LimiterCfgdecay_low[24];  /* '<S599>/decay1' */
    real32_T LimiterCfgk1[24];         /* '<S599>/k1' */
    real32_T LimiterCfgk1_low[24];     /* '<S599>/k2' */
    real32_T LimiterCfgmaxAttack[24];  /* '<S599>/maxAttack' */
    real32_T LimiterCfgmaxAttack_low[24];/* '<S599>/maxAttack1' */
    real32_T MuteCfgRampTime;          /* '<S606>/TOP' */
    real32_T PostLimiterEqpooliirCoeffs[536];/* '<S626>/TOP1' */
    real32_T Sclipp2;                  /* '<S653>/p2 TOP' */
    real32_T Sclipp2_low;              /* '<S653>/p2 TOP1' */
    real32_T Sclipxmax;                /* '<S653>/xmax TOP' */
    real32_T Sclipxmax_low;            /* '<S653>/xmax TOP1' */
    real32_T Sclipxmin;                /* '<S653>/xmin TOP' */
    real32_T Sclipxmin_low;            /* '<S653>/xmin TOP1' */
    uint32_T PostLimiterEqPooliirNumStages[24];/* '<S626>/TOP' */
}
Model_Target_PostProcess0_type;

#define TOP_BANK_Model_Target_PostProcess0 &Model_Target_PostProcess0
#define TOP_BANK_Model_Target_PostProcess0_SIZE sizeof(Model_Target_PostProcess0_type)

typedef struct Model_Target_PreAmp0_tag
{
    int32_T AudiopilotHfNoiseRefLatencySamples;/* '<S957>/TOP' */
    int32_T AudiopilotLfNoiseRefLatencySamples;/* '<S1018>/TOP' */
    int32_T AudiopilotMidrangeLpfAlignDelaySamples;/* '<S712>/TOP' */
    int32_T LevelDetectMusicDelaySamples;/* '<S1102>/TOP' */
    int32_T MedusaDRSpeakerDelay[30];  /* '<S1784>/TOP5' */
    int32_T MedusaDeciRateBassDelay;   /* '<S1445>/Decimated Bass Delay' */
    int32_T MedusaFRSpeakerDelay[22];  /* '<S1784>/TOP3' */
    int32_T MedusaFullRateBassDelay;   /* '<S1445>/Full Rate Bass Delay' */
    int32_T MedusaMidrangeDelay;       /* '<S1445>/Alignment Delay' */
    int32_T MedusaMonoDetectorEnable;  /* '<S1395>/TOP' */
    int32_T MedusaTrebleDelay;         /* '<S1208>/Treble Delay' */
    int32_T VLSXtalkCancellationDelay[16];/* '<S1947>/TOP' */
    real32_T A;                        /* '<S2019>/TOP' */
    real32_T AnnPred;                  /* '<S2026>/TOP' */
    real32_T AudiopilotAntiZipperRampLpfDenominator[2];/* '<S702>/TOP1' */
    real32_T AudiopilotAntiZipperRampLpfNumerator;/* '<S702>/TOP' */
    real32_T AudiopilotBassBpfDenominator[3];/* '<S706>/TOP1' */
    real32_T AudiopilotBassBpfNumerator[3];/* '<S706>/TOP' */
    real32_T AudiopilotBassDownwardSlopeDbPerDb;/* '<S760>/TOP1' */
    real32_T AudiopilotBassDownwardThreshDbspl;/* '<S755>/TOP' */
    real32_T AudiopilotBassMaxAudiopilotBoostDb;/* '<S756>/TOP' */
    real32_T AudiopilotBassMaxDyneqBoostDb;/* '<S757>/TOP' */
    real32_T AudiopilotBassMaxTotalBoostDb;/* '<S758>/TOP' */
    real32_T AudiopilotBassSlope0Dbperdb;/* '<S742>/TOP' */
    real32_T AudiopilotBassSlope1Dbperdb;/* '<S742>/TOP3' */
    real32_T AudiopilotBassThresh0Db;  /* '<S743>/TOP' */
    real32_T AudiopilotBassThresh1Db;  /* '<S744>/TOP' */
    real32_T AudiopilotDyneqBassSlopeDbPerDb;/* '<S762>/TOP' */
    real32_T AudiopilotDyneqBassThreshDbspl;/* '<S761>/TOP' */
    real32_T AudiopilotEnableRampCoef; /* '<S861>/TOP' */
    real32_T AudiopilotHfNoiseHighBoundDbspl;/* '<S746>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionFlatnessHighIndex;/* '<S929>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionFlatnessLowIndex;/* '<S930>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionFlatnessThresh;/* '<S931>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionLevelUpdateCoef;/* '<S935>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionMinSearchCoef;/* '<S932>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionMinSearchMinGain;/* '<S933>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionMinstatAlphaFactor;/* '<S921>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionMinstatAlphaMin;/* '<S922>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionMinstatBeta;/* '<S925>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionMinstatMin;/* '<S926>/TOP' */
    real32_T AudiopilotHfNoiseImpulseRejectionSubWinLenSamples;/* '<S934>/TOP' */
    real32_T AudiopilotHfNoiseLowBoundDbspl;/* '<S747>/TOP' */
    real32_T AudiopilotHfNoiseOffsetDb;/* '<S912>/TOP' */
    real32_T AudiopilotHfNoisePreprocessAntiAliaspooliirCoeffs[45];/* '<S943>/TOP1' */
    real32_T AudiopilotHfNoisePreprocessBandpassLowpasspooliirCoeffs[77];/* '<S950>/TOP1' */
    real32_T AudiopilotHfNoiseRefPowerMin;/* '<S893>/RefPowerMin' */
    real32_T AudiopilotHfNoiseSlewAttackDbPerSample;/* '<S937>/TOP' */
    real32_T AudiopilotHfNoiseSlewDecayDbPerSample;/* '<S938>/TOP' */
    real32_T AudiopilotHfNoiseSlewThresholdDb;/* '<S939>/TOP' */
    real32_T AudiopilotHvacTable[32];  /* '<S679>/TOP' */
    real32_T AudiopilotLevelDetectFastDecayDbPerSample;/* '<S1076>/TOP' */
    real32_T AudiopilotLevelDetectHoldMarginDb;/* '<S1077>/TOP' */
    real32_T AudiopilotLevelDetectHoldTimeSamples;/* '<S1078>/TOP' */
    real32_T AudiopilotLevelDetectMaxVolumeDb;/* '<S1080>/TOP' */
    real32_T AudiopilotLevelDetectMinVolumeDb;/* '<S1081>/TOP' */
    real32_T AudiopilotLevelDetectSlowDecayDbPerSample;/* '<S1082>/TOP' */
    real32_T AudiopilotLfNoiseEnergyLpfCoef;/* '<S960>/TOP' */
    real32_T AudiopilotLfNoiseHighBoundDbspl;/* '<S748>/TOP' */
    real32_T AudiopilotLfNoiseImpulseRejectionGamma0;/* '<S989>/TOP' */
    real32_T AudiopilotLfNoiseImpulseRejectionImpulseVarianceFilterCoef;/* '<S992>/TOP' */
    real32_T AudiopilotLfNoiseImpulseRejectionImpulseVarianceScale;/* '<S993>/TOP' */
    real32_T AudiopilotLfNoiseImpulseRejectionMusicVarianceScale;/* '<S995>/TOP' */
    real32_T AudiopilotLfNoiseImpulseRejectionNoiseVarianceScale;/* '<S998>/TOP' */
    real32_T AudiopilotLfNoiseImpulseRejectionOutputMin;/* '<S999>/TOP' */
    real32_T AudiopilotLfNoiseLowBoundDbspl;/* '<S749>/TOP' */
    real32_T AudiopilotLfNoiseMicpooliirCoeffs[13];/* '<S1004>/TOP1' */
    real32_T AudiopilotLfNoiseOffsetDb;/* '<S969>/TOP' */
    real32_T AudiopilotLfNoiseRefpooliirCoeffs[13];/* '<S1011>/TOP1' */
    real32_T AudiopilotLfNoiseSlewAttackDbPerSample;/* '<S1023>/TOP' */
    real32_T AudiopilotLfNoiseSlewDecayDbPerSample;/* '<S1024>/TOP' */
    real32_T AudiopilotLfNoiseSlewSlowDbPerSample;/* '<S1026>/TOP' */
    real32_T AudiopilotLfNoiseSlewThresholdDb;/* '<S1027>/TOP' */
    real32_T AudiopilotLfNoiseSpeedBoundsAxis[128];/* '<S1032>/TOP' */
    real32_T AudiopilotLfNoiseSpeedBoundsMaxDbspl[128];/* '<S1033>/TOP' */
    real32_T AudiopilotLfNoiseSpeedBoundsMinBoundsLowestValDbspl;/* '<S1034>/TOP1' */
    real32_T AudiopilotLfNoiseSpeedBoundsMinDbspl[128];/* '<S1034>/TOP' */
    real32_T AudiopilotLfNoiseSpeedBoundsOn;/* '<S1035>/TOP' */
    real32_T AudiopilotLfNoiseSysMaxDbspl;/* '<S976>/TOP' */
    real32_T AudiopilotMidrangeDownwardSlopeDbPerDb;/* '<S760>/TOP' */
    real32_T AudiopilotMidrangeDownwardThreshDbspl;/* '<S764>/TOP' */
    real32_T AudiopilotMidrangeLpfDenominator[5];/* '<S707>/TOP1' */
    real32_T AudiopilotMidrangeLpfNumerator[5];/* '<S707>/TOP' */
    real32_T AudiopilotMidrangeMaxTotalBoostDb;/* '<S765>/TOP' */
    real32_T AudiopilotMidrangeSlope0Dbperdb;/* '<S742>/TOP1' */
    real32_T AudiopilotMidrangeSlope1Dbperdb;/* '<S742>/TOP4' */
    real32_T AudiopilotMidrangeThresh0Db;/* '<S750>/TOP' */
    real32_T AudiopilotMidrangeThresh1Db;/* '<S751>/TOP' */
    real32_T AudiopilotMinSignalLevelDbspl;/* '<S802>/TOP' */
    real32_T AudiopilotRatioEnergyLpfCoef;/* '<S1037>/TOP' */
    real32_T AudiopilotRatioHighBoundDb;/* '<S1038>/TOP' */
    real32_T AudiopilotRatioImpulseRejectionGamma0;/* '<S1057>/TOP' */
    real32_T AudiopilotRatioImpulseRejectionImpulseVarianceLpfCoef;/* '<S1058>/TOP' */
    real32_T AudiopilotRatioImpulseRejectionImpulseVarianceScale;/* '<S1059>/TOP' */
    real32_T AudiopilotRatioImpulseRejectionNoiseVarianceScale;/* '<S1060>/TOP' */
    real32_T AudiopilotRatioLowBoundDb;/* '<S1039>/TOP' */
    real32_T AudiopilotRatioSlewAttackDbPerSample;/* '<S1053>/TOP' */
    real32_T AudiopilotRatioSlewDecayDbPerSample;/* '<S1054>/TOP' */
    real32_T AudiopilotRatioSlewThresholdDb;/* '<S1055>/TOP' */
    real32_T AudiopilotRatioUpdateFlagHoldTimeSamples;/* '<S1046>/TOP' */
    real32_T AudiopilotRatioUpdateHfNoiseThreshDbspl;/* '<S1047>/TOP' */
    real32_T AudiopilotRatioUpdateLfNoiseThreshDbspl;/* '<S1048>/TOP' */
    real32_T AudiopilotRatioUpdateLfNoiseUpdatePeriodSamples;/* '<S1049>/TOP' */
    real32_T AudiopilotRatioUpdateLfNoiseUpdateThreshDb;/* '<S1050>/TOP' */
    real32_T AudiopilotRatioUpdateSpeedUpdateThreshMph;/* '<S1051>/TOP' */
    real32_T AudiopilotTrebleDownwardSlopeDbPerDb;/* '<S760>/TOP3' */
    real32_T AudiopilotTrebleDownwardThreshDbspl;/* '<S766>/TOP' */
    real32_T AudiopilotTrebleMaxTotalBoostDb;/* '<S767>/TOP' */
    real32_T AudiopilotTrebleSlope0Dbperdb;/* '<S742>/TOP2' */
    real32_T AudiopilotTrebleSlope1Dbperdb;/* '<S742>/TOP5' */
    real32_T AudiopilotTrebleThresh0Db;/* '<S753>/TOP' */
    real32_T AudiopilotTrebleThresh1Db;/* '<S754>/TOP' */
    real32_T Balance_CfgBoostDisable;  /* '<S1147>/TOP' */
    real32_T Balance_CfgChanRampMap[2];/* '<S1142>/TOP' */
    real32_T Balance_CfgEnableSilentExtreme;/* '<S1151>/TOP' */
    real32_T Balance_CfgOffset;        /* '<S1148>/TOP' */
    real32_T Balance_CfgRampTime;      /* '<S1149>/TOP' */
    real32_T Balance_CfgSleepingBeautyMutesBass;/* '<S1150>/TOP' */
    real32_T Balance_CfgTableDb[30];   /* '<S1152>/TOP_TableIDb' */
    real32_T Balance_CfgTableIdx[30];  /* '<S1152>/TOP_TableIdx' */
    real32_T BassBoostCoeffs[3];       /* '<S1974>/BoostCoeffs' */
    real32_T BassBoostMaximum_dB;      /* '<S1974>/Boost_dB TOP' */
    real32_T BassCutCoeffs[3];         /* '<S1974>/CutCoeffs' */
    real32_T BassCutMaximum_dB;        /* '<S1974>/Cut_dB TOP' */
    real32_T BassOffset;               /* '<S1982>/TOP' */
    real32_T BassScale;                /* '<S1984>/TOP' */
    real32_T Bassramptime;             /* '<S1983>/TOP' */
    real32_T DyneqLevelDetectFastDecayDbPerSample;/* '<S1089>/TOP' */
    real32_T DyneqLevelDetectHoldMarginDb;/* '<S1090>/TOP' */
    real32_T DyneqLevelDetectHoldTimeSamples;/* '<S1091>/TOP' */
    real32_T DyneqLevelDetectMaxVolumeDb;/* '<S1093>/TOP' */
    real32_T DyneqLevelDetectMinVolumeDb;/* '<S1094>/TOP' */
    real32_T DyneqLevelDetectSlowDecayDbPerSample;/* '<S1095>/TOP' */
    real32_T FadeControlBoostDisable;  /* '<S1830>/TOP' */
    real32_T FadeControlEnableSilentExtreme;/* '<S1831>/TOP' */
    real32_T FadeControlRampTime;      /* '<S1834>/TOP' */
    real32_T FadeControlTableDb[30];   /* '<S1832>/TOPDb' */
    real32_T FadeControlTableIdx[30];  /* '<S1832>/TOPIndex' */
    real32_T FadeControloffset;        /* '<S1833>/TOP' */
    real32_T FadeRamperDRChannelToRamperMap[30];/* '<S1843>/TOP_ChanRampMap' */
    real32_T FadeRamperDRTOPFilterCoefficients[3];/* '<S1843>/TOP' */
    real32_T FadeRamperFRChannelToRamperMap[22];/* '<S1850>/TOP_ChanRampMap' */
    real32_T FadeRamperFRTOPFilterCoefficients[3];/* '<S1850>/TOP' */
    real32_T G;                        /* '<S2023>/TOP' */
    real32_T LevelDetectPreemphasispooliirCoeffs[10];/* '<S1107>/TOP1' */
    real32_T LevelDetectSysMaxDbspl;   /* '<S1105>/TOP' */
    real32_T Limiterattack[32];        /* '<S2027>/attack' */
    real32_T Limiterdecay[32];         /* '<S2027>/decay' */
    real32_T LimitermaxAttack[32];     /* '<S2027>/maxAttack' */
    real32_T MedusaBassManagerHpfpooliirCoeffs[26];/* '<S1175>/TOP1' */
    real32_T MedusaBassManagerLpfpooliirCoeffs[26];/* '<S1183>/TOP1' */
    real32_T MedusaDRHoligramOutputMapping[18];/* '<S1732>/TOP' */
    real32_T MedusaDRHoligramPoolIirTunableConfigpooliirCoeffs[5346];/* '<S1723>/TOP1' */
    real32_T MedusaDeciRateFdpCoeffsESDLowerbounddB;/* '<S1227>/TOP3' */
    real32_T MedusaDeciRateFdpCoeffsESDStepSize;/* '<S1227>/TOP4' */
    real32_T MedusaDeciRateFdpCoeffsESDUpperbounddB;/* '<S1227>/TOP2' */
    real32_T MedusaDeciRateFdpCoeffsLokRokMaxScaleFactor;/* '<S1227>/TOP1' */
    real32_T MedusaDeciRateFdpCoeffsXpMax;/* '<S1227>/TOP' */
    real32_T MedusaDeciRateFdpCoeffsXpMin;/* '<S1227>/TOP5' */
    real32_T MedusaDeciRateFdpSpumDirectPathSamplesDec;/* '<S1217>/TOP3' */
    real32_T MedusaDeciRateFdpSpumFastPsdSmoothFactor;/* '<S1217>/TOP' */
    real32_T MedusaDeciRateFdpSpumLsSmoothFactor;/* '<S1217>/TOP4' */
    real32_T MedusaDeciRateFdpSpumOverwrite;/* '<S1215>/TOP' */
    real32_T MedusaDeciRateFdpSpumSlowPsdSmoothFactor;/* '<S1217>/TOP1' */
    real32_T MedusaDummyTune;          /* '<S1193>/TOP' */
    real32_T MedusaFRHoligramIirConfigpooliirCoeffs[726];/* '<S1734>/TOP1' */
    real32_T MedusaFullRateFdpCoeffsESDLowerbounddB;/* '<S1322>/TOP3' */
    real32_T MedusaFullRateFdpCoeffsESDStepSize;/* '<S1322>/TOP4' */
    real32_T MedusaFullRateFdpCoeffsESDUpperbounddB;/* '<S1322>/TOP2' */
    real32_T MedusaFullRateFdpCoeffsLokRokMaxScaleFactor;/* '<S1322>/TOP1' */
    real32_T MedusaFullRateFdpCoeffsXpMax;/* '<S1322>/TOP' */
    real32_T MedusaFullRateFdpCoeffsXpMin;/* '<S1322>/TOP5' */
    real32_T MedusaFullRateFdpSpumDirectPathSamplesDec;/* '<S1313>/TOP3' */
    real32_T MedusaFullRateFdpSpumFastPsdSmoothFactor;/* '<S1313>/TOP' */
    real32_T MedusaFullRateFdpSpumLsSmoothFactor;/* '<S1313>/TOP4' */
    real32_T MedusaFullRateFdpSpumOverwrite;/* '<S1311>/TOP' */
    real32_T MedusaFullRateFdpSpumSlowPsdSmoothFactor;/* '<S1313>/TOP1' */
    real32_T MedusaFullRateMixerBackwardDownmixMax;/* '<S1474>/TOP' */
    real32_T MedusaFullRateMixerBackwardDownmixThreshold;/* '<S1475>/TOP' */
    real32_T MedusaFullRateMixerDetentLevel_CS[12];/* '<S1519>/TOP' */
    real32_T MedusaFullRateMixerDetentLevel_CS_CAE[4];/* '<S1519>/TOP1' */
    real32_T MedusaFullRateMixerDetentLevel_Left[30];/* '<S1520>/TOP' */
    real32_T MedusaFullRateMixerDetentLevel_Left_CAE[10];/* '<S1520>/TOP1' */
    real32_T MedusaFullRateMixerDetentLevel_Right[30];/* '<S1521>/TOP' */
    real32_T MedusaFullRateMixerDetentLevel_Right_CAE[10];/* '<S1521>/TOP1' */
    real32_T MedusaFullRateMixerDownmixWeights_CS[12];/* '<S1476>/TOP' */
    real32_T MedusaFullRateMixerDownmixWeights_CS_CAE[4];/* '<S1476>/TOP1' */
    real32_T MedusaFullRateMixerDownmixWeights_Left[30];/* '<S1477>/TOP' */
    real32_T MedusaFullRateMixerDownmixWeights_Left_CAE[10];/* '<S1477>/TOP1' */
    real32_T MedusaFullRateMixerDownmixWeights_Right[30];/* '<S1478>/TOP' */
    real32_T MedusaFullRateMixerDownmixWeights_Right_CAE[10];/* '<S1478>/TOP1' */
    real32_T MedusaFullRateMixerForwardDownmixMax;/* '<S1480>/TOP' */
    real32_T MedusaFullRateMixerForwardDownmixThreshold;/* '<S1479>/TOP' */
    real32_T MedusaFullRateMixerInterpolation_Method;/* '<S1522>/TOP' */
    real32_T MedusaFullRateMixerMaxLevel_CS[12];/* '<S1523>/TOP' */
    real32_T MedusaFullRateMixerMaxLevel_CS_CAE[4];/* '<S1523>/TOP1' */
    real32_T MedusaFullRateMixerMaxLevel_Left[30];/* '<S1524>/TOP' */
    real32_T MedusaFullRateMixerMaxLevel_Left_CAE[10];/* '<S1524>/TOP1' */
    real32_T MedusaFullRateMixerMaxLevel_Right[30];/* '<S1525>/TOP' */
    real32_T MedusaFullRateMixerMaxLevel_Right_CAE[10];/* '<S1525>/TOP1' */
    real32_T MedusaFullRateMixerMinLevel_CS[12];/* '<S1526>/TOP' */
    real32_T MedusaFullRateMixerMinLevel_CS_CAE[4];/* '<S1526>/TOP1' */
    real32_T MedusaFullRateMixerMinLevel_Left[30];/* '<S1527>/TOP' */
    real32_T MedusaFullRateMixerMinLevel_Left_CAE[10];/* '<S1527>/TOP1' */
    real32_T MedusaFullRateMixerMinLevel_Right[30];/* '<S1528>/TOP' */
    real32_T MedusaFullRateMixerMinLevel_Right_CAE[10];/* '<S1528>/TOP1' */
    real32_T MedusaHeadrestCompEQIirConfigpooliirCoeffs[972];/* '<S1756>/TOP1' */
    real32_T MedusaMonoDetectorHPFCoeffs[10];/* '<S1399>/TOP11' */
    real32_T MedusaMonoDetectorLRMatchTolerance;/* '<S1399>/TOP7' */
    real32_T MedusaMonoDetectorLSSmoothGain;/* '<S1399>/TOP3' */
    real32_T MedusaMonoDetectorLSSmoothPole;/* '<S1399>/TOP2' */
    real32_T MedusaMonoDetectorLSSmoothResetGain;/* '<S1399>/TOP5' */
    real32_T MedusaMonoDetectorLSSmoothResetPole;/* '<S1399>/TOP4' */
    real32_T MedusaMonoDetectorLevelGain;/* '<S1399>/TOP1' */
    real32_T MedusaMonoDetectorLevelPole;/* '<S1399>/TOP' */
    real32_T MedusaMonoDetectorThresholdMono;/* '<S1399>/TOP8' */
    real32_T MedusaMonoDetectorThresholdStereo;/* '<S1399>/TOP9' */
    real32_T MedusaMonoDetectorThresholdZeroLevel;/* '<S1399>/TOP6' */
    real32_T MedusaPremixBackwardDownmixMax;/* '<S1565>/TOP' */
    real32_T MedusaPremixBackwardDownmixThreshold;/* '<S1566>/TOP' */
    real32_T MedusaPremixDetentLevel_CS[20];/* '<S1612>/TOP' */
    real32_T MedusaPremixDetentLevel_CS_CAE[4];/* '<S1612>/TOP1' */
    real32_T MedusaPremixDetentLevel_Left[77];/* '<S1613>/TOP' */
    real32_T MedusaPremixDetentLevel_Left_CAE[33];/* '<S1613>/TOP1' */
    real32_T MedusaPremixDetentLevel_Right[77];/* '<S1614>/TOP' */
    real32_T MedusaPremixDetentLevel_Right_CAE[33];/* '<S1614>/TOP1' */
    real32_T MedusaPremixDownmixWeights_CS[20];/* '<S1567>/TOP' */
    real32_T MedusaPremixDownmixWeights_CS_CAE[4];/* '<S1567>/TOP1' */
    real32_T MedusaPremixDownmixWeights_Left[77];/* '<S1568>/TOP' */
    real32_T MedusaPremixDownmixWeights_Left_CAE[33];/* '<S1568>/TOP1' */
    real32_T MedusaPremixDownmixWeights_Right[77];/* '<S1569>/TOP' */
    real32_T MedusaPremixDownmixWeights_Right_CAE[33];/* '<S1569>/TOP1' */
    real32_T MedusaPremixForwardDownmixMax;/* '<S1571>/TOP' */
    real32_T MedusaPremixForwardDownmixThreshold;/* '<S1570>/TOP' */
    real32_T MedusaPremixInterpolation_Method;/* '<S1615>/TOP' */
    real32_T MedusaPremixMaxLevel_CS[20];/* '<S1616>/TOP' */
    real32_T MedusaPremixMaxLevel_CS_CAE[4];/* '<S1616>/TOP1' */
    real32_T MedusaPremixMaxLevel_Left[77];/* '<S1617>/TOP' */
    real32_T MedusaPremixMaxLevel_Left_CAE[33];/* '<S1617>/TOP1' */
    real32_T MedusaPremixMaxLevel_Right[77];/* '<S1618>/TOP' */
    real32_T MedusaPremixMaxLevel_Right_CAE[33];/* '<S1618>/TOP1' */
    real32_T MedusaPremixMinLevel_CS[20];/* '<S1619>/TOP' */
    real32_T MedusaPremixMinLevel_CS_CAE[4];/* '<S1619>/TOP1' */
    real32_T MedusaPremixMinLevel_Left[77];/* '<S1620>/TOP' */
    real32_T MedusaPremixMinLevel_Left_CAE[33];/* '<S1620>/TOP1' */
    real32_T MedusaPremixMinLevel_Right[77];/* '<S1621>/TOP' */
    real32_T MedusaPremixMinLevel_Right_CAE[33];/* '<S1621>/TOP1' */
    real32_T MedusaPremixMonoSurroundCosPhi;/* '<S1541>/TOP1' */
    real32_T MedusaPremixMonoSurroundPhi;/* '<S1541>/TOP' */
    real32_T MedusaPremixMonoSurroundSinPhi;/* '<S1541>/TOP2' */
    real32_T MedusaRoutingMap[24];     /* '<S1794>/TOP' */
    real32_T MedusaSPUMDecayEstDecaySmoothFactor;/* '<S1406>/TOP1' */
    real32_T MedusaSPUMDecayEstDirectPathFramesFs;/* '<S1406>/TOP6' */
    real32_T MedusaSPUMDecayEstFastAttack;/* '<S1406>/TOP2' */
    real32_T MedusaSPUMDecayEstHPFpooliirCoeffs[50];/* '<S1420>/TOP1' */
    real32_T MedusaSPUMDecayEstNoiseFloor;/* '<S1413>/TOP' */
    real32_T MedusaSPUMDecayEstOnsetThreshold;/* '<S1414>/TOP' */
    real32_T MedusaSPUMDecayEstPSDSmoothFactor;/* '<S1406>/TOP' */
    real32_T MedusaSPUMDecayEstSlewThreshold;/* '<S1415>/TOP' */
    real32_T MedusaSPUMDecayEstSlowAttack;/* '<S1406>/TOP3' */
    real32_T MedusaSPUMDecayEstStepDecrement;/* '<S1406>/TOP5' */
    real32_T MedusaSPUMDecayEstStepIncrement;/* '<S1406>/TOP4' */
    real32_T MedusaVLSDrvGainsBackwardDownmixMax;/* '<S1650>/TOP' */
    real32_T MedusaVLSDrvGainsBackwardDownmixThreshold;/* '<S1651>/TOP' */
    real32_T MedusaVLSDrvGainsDetentLevel[12];/* '<S1665>/TOP' */
    real32_T MedusaVLSDrvGainsDownmixWeights[12];/* '<S1652>/TOP' */
    real32_T MedusaVLSDrvGainsForwardDownmixMax;/* '<S1654>/TOP' */
    real32_T MedusaVLSDrvGainsForwardDownmixThreshold;/* '<S1653>/TOP' */
    real32_T MedusaVLSDrvGainsInterpolation_Method;/* '<S1666>/TOP' */
    real32_T MedusaVLSDrvGainsMaxLevel[12];/* '<S1667>/TOP' */
    real32_T MedusaVLSDrvGainsMinLevel[12];/* '<S1668>/TOP' */
    real32_T MedusaVLSPaxGainsBackwardDownmixMax;/* '<S1675>/TOP' */
    real32_T MedusaVLSPaxGainsBackwardDownmixThreshold;/* '<S1676>/TOP' */
    real32_T MedusaVLSPaxGainsDetentLevel[12];/* '<S1690>/TOP' */
    real32_T MedusaVLSPaxGainsDownmixWeights[12];/* '<S1677>/TOP' */
    real32_T MedusaVLSPaxGainsForwardDownmixMax;/* '<S1679>/TOP' */
    real32_T MedusaVLSPaxGainsForwardDownmixThreshold;/* '<S1678>/TOP' */
    real32_T MedusaVLSPaxGainsInterpolation_Method;/* '<S1691>/TOP' */
    real32_T MedusaVLSPaxGainsMaxLevel[12];/* '<S1692>/TOP' */
    real32_T MedusaVLSPaxGainsMinLevel[12];/* '<S1693>/TOP' */
    real32_T MedusaWiggleFilter[1776]; /* '<S1447>/TOP' */
    real32_T MedusatailCompEQpooliirCoeffs[164];/* '<S1450>/TOP1' */
    real32_T MidrangeCfgBoostCoeffs[5];/* '<S1975>/BoostCoeffs' */
    real32_T MidrangeCfgBoostMaximum_dB;/* '<S1975>/Boost_dB TOP' */
    real32_T MidrangeCfgCutCoeffs[5];  /* '<S1975>/CutCoeffs' */
    real32_T MidrangeCfgCutMaximum_dB; /* '<S1975>/Cut_dB TOP' */
    real32_T MidrangeCfgOffset;        /* '<S2000>/TOP' */
    real32_T MidrangeCfgRampTime;      /* '<S2001>/TOP' */
    real32_T MidrangeCfgScale;         /* '<S2002>/TOP' */
    real32_T MuteControlRampTime;      /* '<S1860>/TOP' */
    real32_T NearDelay;                /* '<S1807>/TOP' */
    real32_T NearWiggleCoeffs[2048];   /* '<S1806>/TOP' */
    real32_T Offset;                   /* '<S2028>/TOP' */
    real32_T SleepingBeautyDRChannelToRamperMap[30];/* '<S1900>/TOP' */
    real32_T SleepingBeautyDRMutesBass;/* '<S1907>/TOP' */
    real32_T SleepingBeautyDROffset;   /* '<S1905>/TOP' */
    real32_T SleepingBeautyDRRampTime; /* '<S1906>/TOP' */
    real32_T SleepingBeautyDRTableDb[30];/* '<S1908>/TOP' */
    real32_T SleepingBeautyDRTableIdx[30];/* '<S1908>/TOP1' */
    real32_T SleepingBeautyFRChannelToRamperMap[22];/* '<S1920>/TOP' */
    real32_T SleepingBeautyFRMutesBass;/* '<S1927>/TOP' */
    real32_T SleepingBeautyFROffset;   /* '<S1925>/TOP' */
    real32_T SleepingBeautyFRRampTime; /* '<S1926>/TOP' */
    real32_T SleepingBeautyFRTableDb[30];/* '<S1928>/TOP' */
    real32_T SleepingBeautyFRTableIdx[30];/* '<S1928>/TOP1' */
    real32_T TrebleBoostCoeffs[3];     /* '<S1977>/BoostCoeffs' */
    real32_T TrebleBoostMaximum_dB;    /* '<S1977>/Boost_dB TOP' */
    real32_T TrebleCutCoeffs[3];       /* '<S1977>/CutCoeffs' */
    real32_T TrebleCutMaximum_dB;      /* '<S1977>/Cut_dB TOP' */
    real32_T TrebleOffset;             /* '<S2036>/TOP' */
    real32_T TrebleScale;              /* '<S2038>/TOP' */
    real32_T Trebleramptime;           /* '<S2037>/TOP' */
    real32_T UpperThreshold;           /* '<S2029>/TOP' */
    real32_T VLSXtalkCancellationIIRpooliirCoeffs[976];/* '<S1950>/TOP1' */
    real32_T VolumeCfgRampTime;        /* '<S1117>/TOP' */
    real32_T VolumeCfgTable_dB[8];     /* '<S1113>/TOP_GainDB' */
    real32_T VolumeCfgmaxgain;         /* '<S1120>/TOP' */
    uint32_T AudiopilotHfNoisePreprocessAntiAliasPooliirNumStages[5];/* '<S943>/TOP' */
    uint32_T AudiopilotHfNoisePreprocessBandpassLowpassPooliirNumStages[5];/* '<S950>/TOP' */
    uint32_T AudiopilotLfNoiseMicPooliirNumStages;/* '<S1004>/TOP' */
    uint32_T AudiopilotLfNoiseRefPooliirNumStages;/* '<S1011>/TOP' */
    uint32_T AudiopilotTwoDowngrade;   /* '<S672>/TOP' */
    uint32_T LevelDetectPreemphasisPooliirNumStages[2];/* '<S1107>/TOP' */
    uint32_T MedusaBassManagerHpfPooliirNumStages[2];/* '<S1175>/TOP' */
    uint32_T MedusaBassManagerLpfPooliirNumStages[2];/* '<S1183>/TOP' */
    uint32_T MedusaDRHoligramFilterLengths[66];/* '<S1711>/TOP1' */
    uint32_T MedusaDRHoligramInputMapping[66];/* '<S1731>/TOP' */
    uint32_T MedusaDRHoligramPaddedFilterLengths[66];/* '<S1711>/TOP2' */
    uint32_T MedusaDRHoligramPoolIirTunableConfigPooliirNumStages[66];/* '<S1723>/TOP' */
    uint32_T MedusaFRHoligramIirConfigPooliirNumStages[22];/* '<S1734>/TOP' */
    uint32_T MedusaHeadrestCompEQFilterLengths[12];/* '<S1706>/TOP1' */
    uint32_T MedusaHeadrestCompEQIirConfigPooliirNumStages[12];/* '<S1756>/TOP' */
    uint32_T MedusaHeadrestCompEQPaddedFilterLengths[12];/* '<S1706>/TOP2' */
    uint32_T MedusaMonoDetectorHPFNumStages[2];/* '<S1399>/TOP10' */
    uint32_T MedusaSPUMDecayEstHPFPooliirNumStages[2];/* '<S1420>/TOP' */
    uint32_T MedusatailCompEQPooliirNumStages[4];/* '<S1450>/TOP' */
    uint32_T VLSGenerationDrvPaddedFilterLengths[20];/* '<S1941>/TOP2' */
    uint32_T VLSGenerationPaxPaddedFilterLengths[20];/* '<S1942>/TOP2' */
    uint32_T VLSXtalkCancellationIIRPooliirNumStages[16];/* '<S1950>/TOP' */
    uint32_T VolumeCfgTable_Idx[8];    /* '<S1113>/TOP_Index' */
}
Model_Target_PreAmp0_type;

#define TOP_BANK_Model_Target_PreAmp0  &Model_Target_PreAmp0
#define TOP_BANK_Model_Target_PreAmp0_SIZE sizeof(Model_Target_PreAmp0_type)

typedef struct Model_Target_PreAmp1_tag
{
    real32_T MedusaDRHoligramFIRCoeffs[35112];/* '<S1711>/TOP' */
    real32_T MedusaHeadrestCompEQFIRCoeffs[6384];/* '<S1706>/TOP' */
    real32_T VLSGenerationDrvFIRCoeffs[5760];/* '<S1941>/TOP' */
    real32_T VLSGenerationPaxFIRCoeffs[5760];/* '<S1942>/TOP' */
}
Model_Target_PreAmp1_type;

#define TOP_BANK_Model_Target_PreAmp1  &Model_Target_PreAmp1
#define TOP_BANK_Model_Target_PreAmp1_SIZE sizeof(Model_Target_PreAmp1_type)

typedef struct Model_Target_TestRouter0_tag
{
    int32_T ASDRouterEnableAux;        /* '<S2064>/TOP' */
    int32_T ASDRouterMainSelect[24];   /* '<S2085>/TOP' */
    int32_T AuxSelect[24];             /* '<S2054>/TOP' */
}
Model_Target_TestRouter0_type;

#define TOP_BANK_Model_Target_TestRouter0 &Model_Target_TestRouter0
#define TOP_BANK_Model_Target_TestRouter0_SIZE sizeof(Model_Target_TestRouter0_type)

// Unused banks
// ------------
#define TOP_BANK_Model_Target_InputSelect1 NULL
#define TOP_BANK_Model_Target_InputSelect1_SIZE 0
#define TOP_BANK_Model_Target_InputSelect2 NULL
#define TOP_BANK_Model_Target_InputSelect2_SIZE 0
#define TOP_BANK_Model_Target_InputSelect3 NULL
#define TOP_BANK_Model_Target_InputSelect3_SIZE 0
#define TOP_BANK_Model_Target_InputSelect4 NULL
#define TOP_BANK_Model_Target_InputSelect4_SIZE 0
#define TOP_BANK_Model_Target_InputSelect5 NULL
#define TOP_BANK_Model_Target_InputSelect5_SIZE 0
#define TOP_BANK_Model_Target_InputSelect6 NULL
#define TOP_BANK_Model_Target_InputSelect6_SIZE 0
#define TOP_BANK_Model_Target_Near1    NULL
#define TOP_BANK_Model_Target_Near1_SIZE 0
#define TOP_BANK_Model_Target_Near2    NULL
#define TOP_BANK_Model_Target_Near2_SIZE 0
#define TOP_BANK_Model_Target_Near3    NULL
#define TOP_BANK_Model_Target_Near3_SIZE 0
#define TOP_BANK_Model_Target_Near4    NULL
#define TOP_BANK_Model_Target_Near4_SIZE 0
#define TOP_BANK_Model_Target_Near5    NULL
#define TOP_BANK_Model_Target_Near5_SIZE 0
#define TOP_BANK_Model_Target_Near6    NULL
#define TOP_BANK_Model_Target_Near6_SIZE 0
#define TOP_BANK_Model_Target_PostProcess1 NULL
#define TOP_BANK_Model_Target_PostProcess1_SIZE 0
#define TOP_BANK_Model_Target_PostProcess2 NULL
#define TOP_BANK_Model_Target_PostProcess2_SIZE 0
#define TOP_BANK_Model_Target_PostProcess3 NULL
#define TOP_BANK_Model_Target_PostProcess3_SIZE 0
#define TOP_BANK_Model_Target_PostProcess4 NULL
#define TOP_BANK_Model_Target_PostProcess4_SIZE 0
#define TOP_BANK_Model_Target_PostProcess5 NULL
#define TOP_BANK_Model_Target_PostProcess5_SIZE 0
#define TOP_BANK_Model_Target_PostProcess6 NULL
#define TOP_BANK_Model_Target_PostProcess6_SIZE 0
#define TOP_BANK_Model_Target_PreAmp2  NULL
#define TOP_BANK_Model_Target_PreAmp2_SIZE 0
#define TOP_BANK_Model_Target_PreAmp3  NULL
#define TOP_BANK_Model_Target_PreAmp3_SIZE 0
#define TOP_BANK_Model_Target_PreAmp4  NULL
#define TOP_BANK_Model_Target_PreAmp4_SIZE 0
#define TOP_BANK_Model_Target_PreAmp5  NULL
#define TOP_BANK_Model_Target_PreAmp5_SIZE 0
#define TOP_BANK_Model_Target_PreAmp6  NULL
#define TOP_BANK_Model_Target_PreAmp6_SIZE 0
#define TOP_BANK_Model_Target_TestRouter1 NULL
#define TOP_BANK_Model_Target_TestRouter1_SIZE 0
#define TOP_BANK_Model_Target_TestRouter2 NULL
#define TOP_BANK_Model_Target_TestRouter2_SIZE 0
#define TOP_BANK_Model_Target_TestRouter3 NULL
#define TOP_BANK_Model_Target_TestRouter3_SIZE 0
#define TOP_BANK_Model_Target_TestRouter4 NULL
#define TOP_BANK_Model_Target_TestRouter4_SIZE 0
#define TOP_BANK_Model_Target_TestRouter5 NULL
#define TOP_BANK_Model_Target_TestRouter5_SIZE 0
#define TOP_BANK_Model_Target_TestRouter6 NULL
#define TOP_BANK_Model_Target_TestRouter6_SIZE 0

extern Model_Target_InputSelect0_type Model_Target_InputSelect0;
extern Model_Target_Near0_type Model_Target_Near0;
extern Model_Target_PostProcess0_type Model_Target_PostProcess0;
extern Model_Target_PreAmp0_type Model_Target_PreAmp0;
extern Model_Target_PreAmp1_type Model_Target_PreAmp1;
extern Model_Target_TestRouter0_type Model_Target_TestRouter0;

#endif                                 /* RTW_HEADER_Model_Target_TOP_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
