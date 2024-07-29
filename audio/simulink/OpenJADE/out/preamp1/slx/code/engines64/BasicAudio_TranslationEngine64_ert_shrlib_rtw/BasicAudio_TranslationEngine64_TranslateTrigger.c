/*
 * File: BasicAudio_TranslationEngine64_TranslateTrigger.c
 *
 * Code generated for Simulink model 'BasicAudio_TranslationEngine64'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:48:46 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include <stdint.h>
#include "TranslateError.h"
#include "BasicAudio_TranslationEngine64_TranslateTrigger.h"
#include "BasicAudio_TranslationEngine64_TuneVar.h"
#include "BasicAudio_TranslationEngine64_TOP_RcvTrans.h"
#include "BasicAudio_TranslationEngine64_ModelCount.h"
#include <stdio.h>                     // for vsnprintf
#include <string.h>                    // for strncpy
#include <stdarg.h>                    // for variadic args (va_start)

// Translation table record
typedef void (*TranslationFunc)(void);
typedef struct TranslationRecord_tag
{
    int updated_tune_vars;             // "dirty flag" counting updated TuneVars
    TranslationFunc translation_func;  // translation function
    int tunevar_count;      // indicates how many TuneVars feed this translation
    enum TuneVarNumber *tunevars;
                              // array of TuneVar IDs that feed this translation
    int top_count;// indicates how many TOPs receive value from this translation
    enum TOP_RcvTransNumber *tops;
    // array of TOP_RcvTrans numbers that receive values from this translation
}
TranslationRecord_type;

enum TuneVarNumber
{
    ID_TuneVar_BasicAudioBalanceChanRampMap,/* '<S9>/TuneVarChanRampMap' */
    ID_TuneVar_BasicAudioBalanceBoostDisable,/* '<S11>/Tune Variable' */
    ID_TuneVar_BasicAudioBalanceOffset,/* '<S12>/Tune Variable' */
    ID_TuneVar_BasicAudioBalanceRampTime,/* '<S13>/Tune Variable' */
    ID_TuneVar_BasicAudioBalanceSleepingBeautyMutesBass,/* '<S14>/Tune Variable' */
    ID_TuneVar_BasicAudioBalanceEnableSilentExtreme,/* '<S15>/Tune Variable' */
    ID_TuneVar_BasicAudioBalanceTable, /* '<S16>/TuneVarBalanceTable' */
    ID_TuneVar_BasicAudioBassBoostFrequencyHz,/* '<S43>/BoostHz' */
    ID_TuneVar_BasicAudioBassCutFrequencyHz,/* '<S43>/CutHz' */
    ID_TuneVar_BasicAudioBassBoostMaximum_dB,/* '<S43>/Boost_dB' */
    ID_TuneVar_BasicAudioBassCutMaximum_dB,/* '<S43>/Cut_dB' */
    ID_TuneVar_BasicAudioBassOffset,   /* '<S48>/Tune Variable' */
    ID_TuneVar_BasicAudioBassramptime, /* '<S49>/Tune Variable' */
    ID_TuneVar_BasicAudioBassScale,    /* '<S50>/Tune Variable' */
    ID_TuneVar_BasicAudioMuteRampTime, /* '<S74>/Tune Variable' */
    ID_TuneVar_BasicAudioTrebleBoostFrequencyHz,/* '<S80>/BoostHz' */
    ID_TuneVar_BasicAudioTrebleCutFrequencyHz,/* '<S80>/CutHz' */
    ID_TuneVar_BasicAudioTrebleBoostMaximum_dB,/* '<S80>/Boost_dB' */
    ID_TuneVar_BasicAudioTrebleCutMaximum_dB,/* '<S80>/Cut_dB' */
    ID_TuneVar_BasicAudioTrebleOffset, /* '<S85>/Tune Variable' */
    ID_TuneVar_BasicAudioTrebleramptime,/* '<S86>/Tune Variable' */
    ID_TuneVar_BasicAudioTrebleScale,  /* '<S87>/Tune Variable' */
    ID_TuneVar_BasicAudioVolumeTable,  /* '<S110>/TuneVar_Table' */
    ID_TuneVar_BasicAudioVolumemaxgain,/* '<S113>/Tune Variable' */
    ID_TuneVar_BasicAudioVolumeRampTime,/* '<S111>/Tune Variable' */
    NUM_TUNEVAR_NUMBERS
};

enum TOP_RcvTransNumber
{
    ID_TOP_RcvTrans_BasicAudioBalanceChanRampMap,/* '<S9>/TOP' */
    ID_TOP_RcvTrans_BasicAudioBalanceBoostDisable,/* '<S11>/TOP' */
    ID_TOP_RcvTrans_BasicAudioBalanceOffset,/* '<S12>/TOP' */
    ID_TOP_RcvTrans_BasicAudioBalanceRampTime,/* '<S13>/TOP' */
    ID_TOP_RcvTrans_BasicAudioBalanceSleepingBeautyMutesBass,/* '<S14>/TOP' */
    ID_TOP_RcvTrans_BasicAudioBalanceEnableSilentExtreme,/* '<S15>/TOP' */
    ID_TOP_RcvTrans_BasicAudioBalanceTableDb,/* '<S16>/TOP_TableIDb' */
    ID_TOP_RcvTrans_BasicAudioBalanceTableIdx,/* '<S16>/TOP_TableIdx' */
    ID_TOP_RcvTrans_BasicAudioBassBoostCoeffs,/* '<S43>/BoostCoeffs' */
    ID_TOP_RcvTrans_BasicAudioBassCutCoeffs,/* '<S43>/CutCoeffs' */
    ID_TOP_RcvTrans_BasicAudioBassBoostMaximum_dB,/* '<S43>/Boost_dB TOP' */
    ID_TOP_RcvTrans_BasicAudioBassCutMaximum_dB,/* '<S43>/Cut_dB TOP' */
    ID_TOP_RcvTrans_BasicAudioBassOffset,/* '<S48>/TOP' */
    ID_TOP_RcvTrans_BasicAudioBassramptime,/* '<S49>/TOP' */
    ID_TOP_RcvTrans_BasicAudioBassScale,/* '<S50>/TOP' */
    ID_TOP_RcvTrans_BasicAudioMuteRampTime,/* '<S74>/TOP' */
    ID_TOP_RcvTrans_BasicAudioTrebleBoostCoeffs,/* '<S80>/BoostCoeffs' */
    ID_TOP_RcvTrans_BasicAudioTrebleCutCoeffs,/* '<S80>/CutCoeffs' */
    ID_TOP_RcvTrans_BasicAudioTrebleBoostMaximum_dB,/* '<S80>/Boost_dB TOP' */
    ID_TOP_RcvTrans_BasicAudioTrebleCutMaximum_dB,/* '<S80>/Cut_dB TOP' */
    ID_TOP_RcvTrans_BasicAudioTrebleOffset,/* '<S85>/TOP' */
    ID_TOP_RcvTrans_BasicAudioTrebleramptime,/* '<S86>/TOP' */
    ID_TOP_RcvTrans_BasicAudioTrebleScale,/* '<S87>/TOP' */
    ID_TOP_RcvTrans_BasicAudioVolumeTable_dB,/* '<S110>/TOP_GainDB' */
    ID_TOP_RcvTrans_BasicAudioVolumeTable_Idx,/* '<S110>/TOP_Index' */
    ID_TOP_RcvTrans_BasicAudioVolumemaxgain,/* '<S113>/TOP' */
    ID_TOP_RcvTrans_BasicAudioVolumeRampTime,/* '<S111>/TOP' */
    NUM_TOP_RCVTRANSNUMBERS
};

// ----- Translation functions (generated in the model):
extern void
           /* [BasicAudioBalanceChanRampMap]-->[BasicAudioBalanceChanRampMap] */
    FuncTranslation_BasicAudio_2333_295_79(void);
                     /* Translation_BasicAudio_2333_295_79:'<S10>/S-Function' */
extern void
         /* [BasicAudioBalanceBoostDisable]-->[BasicAudioBalanceBoostDisable] */
    FuncTranslation_BasicAudio_2333_295_81_67_18(void);
               /* Translation_BasicAudio_2333_295_81_67_18:'<S21>/S-Function' */
extern void          /* [BasicAudioBalanceOffset]-->[BasicAudioBalanceOffset] */
    FuncTranslation_BasicAudio_2333_295_83_67_18(void);
               /* Translation_BasicAudio_2333_295_83_67_18:'<S25>/S-Function' */
extern void      /* [BasicAudioBalanceRampTime]-->[BasicAudioBalanceRampTime] */
    FuncTranslation_BasicAudio_2333_295_84_67_18(void);
               /* Translation_BasicAudio_2333_295_84_67_18:'<S29>/S-Function' */
extern void
/* [BasicAudioBalanceSleepingBeautyMutesBass]-->[BasicAudioBalanceSleepingBeautyMutesBass] */
    FuncTranslation_BasicAudio_2333_295_143_67_18(void);
              /* Translation_BasicAudio_2333_295_143_67_18:'<S33>/S-Function' */
extern void
/* [BasicAudioBalanceEnableSilentExtreme]-->[BasicAudioBalanceEnableSilentExtreme] */
    FuncTranslation_BasicAudio_2333_295_85_67_18(void);
               /* Translation_BasicAudio_2333_295_85_67_18:'<S37>/S-Function' */
extern void
/* [BasicAudioBalanceTable]-->[BasicAudioBalanceTableDb,BasicAudioBalanceTableIdx] */
    FuncTranslation_BasicAudio_2333_295_90(void);
                     /* Translation_BasicAudio_2333_295_90:'<S40>/S-Function' */
extern void /* [BasicAudioBassBoostFrequencyHz]-->[BasicAudioBassBoostCoeffs] */
    FuncTranslation_BasicAudio_2337_236_149(void);
                    /* Translation_BasicAudio_2337_236_149:'<S45>/S-Function' */
extern void     /* [BasicAudioBassCutFrequencyHz]-->[BasicAudioBassCutCoeffs] */
    FuncTranslation_BasicAudio_2337_236_153(void);
                    /* Translation_BasicAudio_2337_236_153:'<S46>/S-Function' */
extern void
         /* [BasicAudioBassBoostMaximum_dB]-->[BasicAudioBassBoostMaximum_dB] */
    FuncTranslation_BasicAudio_2337_236_174(void);
                    /* Translation_BasicAudio_2337_236_174:'<S44>/S-Function' */
extern void  /* [BasicAudioBassCutMaximum_dB]-->[BasicAudioBassCutMaximum_dB] */
    FuncTranslation_BasicAudio_2337_236_176(void);
                    /* Translation_BasicAudio_2337_236_176:'<S47>/S-Function' */
extern void                /* [BasicAudioBassOffset]-->[BasicAudioBassOffset] */
    FuncTranslation_BasicAudio_2337_236_210_67_18(void);
              /* Translation_BasicAudio_2337_236_210_67_18:'<S61>/S-Function' */
extern void            /* [BasicAudioBassramptime]-->[BasicAudioBassramptime] */
    FuncTranslation_BasicAudio_2337_236_229_67_18(void);
              /* Translation_BasicAudio_2337_236_229_67_18:'<S65>/S-Function' */
extern void                  /* [BasicAudioBassScale]-->[BasicAudioBassScale] */
    FuncTranslation_BasicAudio_2337_236_209_67_18(void);
              /* Translation_BasicAudio_2337_236_209_67_18:'<S69>/S-Function' */
extern void            /* [BasicAudioMuteRampTime]-->[BasicAudioMuteRampTime] */
    FuncTranslation_BasicAudio_2341_254_276_67_18(void);
              /* Translation_BasicAudio_2341_254_276_67_18:'<S77>/S-Function' */
extern void
        /* [BasicAudioTrebleBoostFrequencyHz]-->[BasicAudioTrebleBoostCoeffs] */
    FuncTranslation_BasicAudio_2345_236_149(void);
                    /* Translation_BasicAudio_2345_236_149:'<S82>/S-Function' */
extern void /* [BasicAudioTrebleCutFrequencyHz]-->[BasicAudioTrebleCutCoeffs] */
    FuncTranslation_BasicAudio_2345_236_153(void);
                    /* Translation_BasicAudio_2345_236_153:'<S83>/S-Function' */
extern void
     /* [BasicAudioTrebleBoostMaximum_dB]-->[BasicAudioTrebleBoostMaximum_dB] */
    FuncTranslation_BasicAudio_2345_236_174(void);
                    /* Translation_BasicAudio_2345_236_174:'<S81>/S-Function' */
extern void
         /* [BasicAudioTrebleCutMaximum_dB]-->[BasicAudioTrebleCutMaximum_dB] */
    FuncTranslation_BasicAudio_2345_236_176(void);
                    /* Translation_BasicAudio_2345_236_176:'<S84>/S-Function' */
extern void            /* [BasicAudioTrebleOffset]-->[BasicAudioTrebleOffset] */
    FuncTranslation_BasicAudio_2345_236_210_67_18(void);
              /* Translation_BasicAudio_2345_236_210_67_18:'<S98>/S-Function' */
extern void        /* [BasicAudioTrebleramptime]-->[BasicAudioTrebleramptime] */
    FuncTranslation_BasicAudio_2345_236_229_67_18(void);
             /* Translation_BasicAudio_2345_236_229_67_18:'<S102>/S-Function' */
extern void              /* [BasicAudioTrebleScale]-->[BasicAudioTrebleScale] */
    FuncTranslation_BasicAudio_2345_236_209_67_18(void);
             /* Translation_BasicAudio_2345_236_209_67_18:'<S106>/S-Function' */
extern void
/* [BasicAudioVolumeTable]-->[BasicAudioVolumeTable_Idx,BasicAudioVolumeTable_dB] */
    FuncTranslation_BasicAudio_2349_263_280(void);
                   /* Translation_BasicAudio_2349_263_280:'<S112>/S-Function' */
extern void          /* [BasicAudioVolumemaxgain]-->[BasicAudioVolumemaxgain] */
    FuncTranslation_BasicAudio_2349_263_281_67_18(void);
             /* Translation_BasicAudio_2349_263_281_67_18:'<S118>/S-Function' */
extern void        /* [BasicAudioVolumeRampTime]-->[BasicAudioVolumeRampTime] */
    FuncTranslation_BasicAudio_2349_263_256_67_18(void);
             /* Translation_BasicAudio_2349_263_256_67_18:'<S122>/S-Function' */

// -----------------------------------------------------
// ----- Source TuneVars for each translation function:
static enum TuneVarNumber Translation_BasicAudio_2333_295_79_Sources[] =
{                                      /* '<S10>/S-Function' */
    ID_TuneVar_BasicAudioBalanceChanRampMap,
};

static enum TuneVarNumber Translation_BasicAudio_2333_295_81_67_18_Sources[] =
{                                      /* '<S21>/S-Function' */
    ID_TuneVar_BasicAudioBalanceBoostDisable,
};

static enum TuneVarNumber Translation_BasicAudio_2333_295_83_67_18_Sources[] =
{                                      /* '<S25>/S-Function' */
    ID_TuneVar_BasicAudioBalanceOffset,
};

static enum TuneVarNumber Translation_BasicAudio_2333_295_84_67_18_Sources[] =
{                                      /* '<S29>/S-Function' */
    ID_TuneVar_BasicAudioBalanceRampTime,
};

static enum TuneVarNumber Translation_BasicAudio_2333_295_143_67_18_Sources[] =
{                                      /* '<S33>/S-Function' */
    ID_TuneVar_BasicAudioBalanceSleepingBeautyMutesBass,
};

static enum TuneVarNumber Translation_BasicAudio_2333_295_85_67_18_Sources[] =
{                                      /* '<S37>/S-Function' */
    ID_TuneVar_BasicAudioBalanceEnableSilentExtreme,
};

static enum TuneVarNumber Translation_BasicAudio_2333_295_90_Sources[] =
{                                      /* '<S40>/S-Function' */
    ID_TuneVar_BasicAudioBalanceTable,
};

static enum TuneVarNumber Translation_BasicAudio_2337_236_149_Sources[] =
{                                      /* '<S45>/S-Function' */
    ID_TuneVar_BasicAudioBassBoostFrequencyHz,
};

static enum TuneVarNumber Translation_BasicAudio_2337_236_153_Sources[] =
{                                      /* '<S46>/S-Function' */
    ID_TuneVar_BasicAudioBassCutFrequencyHz,
};

static enum TuneVarNumber Translation_BasicAudio_2337_236_174_Sources[] =
{                                      /* '<S44>/S-Function' */
    ID_TuneVar_BasicAudioBassBoostMaximum_dB,
};

static enum TuneVarNumber Translation_BasicAudio_2337_236_176_Sources[] =
{                                      /* '<S47>/S-Function' */
    ID_TuneVar_BasicAudioBassCutMaximum_dB,
};

static enum TuneVarNumber Translation_BasicAudio_2337_236_210_67_18_Sources[] =
{                                      /* '<S61>/S-Function' */
    ID_TuneVar_BasicAudioBassOffset,
};

static enum TuneVarNumber Translation_BasicAudio_2337_236_229_67_18_Sources[] =
{                                      /* '<S65>/S-Function' */
    ID_TuneVar_BasicAudioBassramptime,
};

static enum TuneVarNumber Translation_BasicAudio_2337_236_209_67_18_Sources[] =
{                                      /* '<S69>/S-Function' */
    ID_TuneVar_BasicAudioBassScale,
};

static enum TuneVarNumber Translation_BasicAudio_2341_254_276_67_18_Sources[] =
{                                      /* '<S77>/S-Function' */
    ID_TuneVar_BasicAudioMuteRampTime,
};

static enum TuneVarNumber Translation_BasicAudio_2345_236_149_Sources[] =
{                                      /* '<S82>/S-Function' */
    ID_TuneVar_BasicAudioTrebleBoostFrequencyHz,
};

static enum TuneVarNumber Translation_BasicAudio_2345_236_153_Sources[] =
{                                      /* '<S83>/S-Function' */
    ID_TuneVar_BasicAudioTrebleCutFrequencyHz,
};

static enum TuneVarNumber Translation_BasicAudio_2345_236_174_Sources[] =
{                                      /* '<S81>/S-Function' */
    ID_TuneVar_BasicAudioTrebleBoostMaximum_dB,
};

static enum TuneVarNumber Translation_BasicAudio_2345_236_176_Sources[] =
{                                      /* '<S84>/S-Function' */
    ID_TuneVar_BasicAudioTrebleCutMaximum_dB,
};

static enum TuneVarNumber Translation_BasicAudio_2345_236_210_67_18_Sources[] =
{                                      /* '<S98>/S-Function' */
    ID_TuneVar_BasicAudioTrebleOffset,
};

static enum TuneVarNumber Translation_BasicAudio_2345_236_229_67_18_Sources[] =
{                                      /* '<S102>/S-Function' */
    ID_TuneVar_BasicAudioTrebleramptime,
};

static enum TuneVarNumber Translation_BasicAudio_2345_236_209_67_18_Sources[] =
{                                      /* '<S106>/S-Function' */
    ID_TuneVar_BasicAudioTrebleScale,
};

static enum TuneVarNumber Translation_BasicAudio_2349_263_280_Sources[] =
{                                      /* '<S112>/S-Function' */
    ID_TuneVar_BasicAudioVolumeTable,
};

static enum TuneVarNumber Translation_BasicAudio_2349_263_281_67_18_Sources[] =
{                                      /* '<S118>/S-Function' */
    ID_TuneVar_BasicAudioVolumemaxgain,
};

static enum TuneVarNumber Translation_BasicAudio_2349_263_256_67_18_Sources[] =
{                                      /* '<S122>/S-Function' */
    ID_TuneVar_BasicAudioVolumeRampTime,
};

// -----------------------------------------------------
// ----- Dependent TOPs for each translation function:
static enum TOP_RcvTransNumber Translation_BasicAudio_2333_295_79_Destinations[]
    =
{                                      /* '<S10>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBalanceChanRampMap,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2333_295_81_67_18_Destinations[] =
{                                      /* '<S21>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBalanceBoostDisable,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2333_295_83_67_18_Destinations[] =
{                                      /* '<S25>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBalanceOffset,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2333_295_84_67_18_Destinations[] =
{                                      /* '<S29>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBalanceRampTime,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2333_295_143_67_18_Destinations[] =
{                                      /* '<S33>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBalanceSleepingBeautyMutesBass,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2333_295_85_67_18_Destinations[] =
{                                      /* '<S37>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBalanceEnableSilentExtreme,
};

static enum TOP_RcvTransNumber Translation_BasicAudio_2333_295_90_Destinations[]
    =
{                                      /* '<S40>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBalanceTableDb,
    ID_TOP_RcvTrans_BasicAudioBalanceTableIdx,
};

static enum TOP_RcvTransNumber Translation_BasicAudio_2337_236_149_Destinations[]
    =
{                                      /* '<S45>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBassBoostCoeffs,
};

static enum TOP_RcvTransNumber Translation_BasicAudio_2337_236_153_Destinations[]
    =
{                                      /* '<S46>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBassCutCoeffs,
};

static enum TOP_RcvTransNumber Translation_BasicAudio_2337_236_174_Destinations[]
    =
{                                      /* '<S44>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBassBoostMaximum_dB,
};

static enum TOP_RcvTransNumber Translation_BasicAudio_2337_236_176_Destinations[]
    =
{                                      /* '<S47>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBassCutMaximum_dB,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2337_236_210_67_18_Destinations[] =
{                                      /* '<S61>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBassOffset,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2337_236_229_67_18_Destinations[] =
{                                      /* '<S65>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBassramptime,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2337_236_209_67_18_Destinations[] =
{                                      /* '<S69>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioBassScale,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2341_254_276_67_18_Destinations[] =
{                                      /* '<S77>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioMuteRampTime,
};

static enum TOP_RcvTransNumber Translation_BasicAudio_2345_236_149_Destinations[]
    =
{                                      /* '<S82>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioTrebleBoostCoeffs,
};

static enum TOP_RcvTransNumber Translation_BasicAudio_2345_236_153_Destinations[]
    =
{                                      /* '<S83>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioTrebleCutCoeffs,
};

static enum TOP_RcvTransNumber Translation_BasicAudio_2345_236_174_Destinations[]
    =
{                                      /* '<S81>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioTrebleBoostMaximum_dB,
};

static enum TOP_RcvTransNumber Translation_BasicAudio_2345_236_176_Destinations[]
    =
{                                      /* '<S84>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioTrebleCutMaximum_dB,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2345_236_210_67_18_Destinations[] =
{                                      /* '<S98>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioTrebleOffset,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2345_236_229_67_18_Destinations[] =
{                                      /* '<S102>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioTrebleramptime,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2345_236_209_67_18_Destinations[] =
{                                      /* '<S106>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioTrebleScale,
};

static enum TOP_RcvTransNumber Translation_BasicAudio_2349_263_280_Destinations[]
    =
{                                      /* '<S112>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioVolumeTable_Idx,
    ID_TOP_RcvTrans_BasicAudioVolumeTable_dB,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2349_263_281_67_18_Destinations[] =
{                                      /* '<S118>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioVolumemaxgain,
};

static enum TOP_RcvTransNumber
    Translation_BasicAudio_2349_263_256_67_18_Destinations[] =
{                                      /* '<S122>/S-Function' */
    ID_TOP_RcvTrans_BasicAudioVolumeRampTime,
};

// -----------------------------------------------------
// ----- Translation object table listing all translations
static TranslationRecord_type translation_table[] =
{
    // Translation record for BasicAudioBalanceChanRampMap '<S10>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2333_295_79, 1,
        Translation_BasicAudio_2333_295_79_Sources, 1,
        Translation_BasicAudio_2333_295_79_Destinations
    },

    // Translation record for BasicAudioBalanceBoostDisable '<S21>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2333_295_81_67_18, 1,
        Translation_BasicAudio_2333_295_81_67_18_Sources, 1,
        Translation_BasicAudio_2333_295_81_67_18_Destinations
    },

    // Translation record for BasicAudioBalanceOffset '<S25>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2333_295_83_67_18, 1,
        Translation_BasicAudio_2333_295_83_67_18_Sources, 1,
        Translation_BasicAudio_2333_295_83_67_18_Destinations
    },

    // Translation record for BasicAudioBalanceRampTime '<S29>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2333_295_84_67_18, 1,
        Translation_BasicAudio_2333_295_84_67_18_Sources, 1,
        Translation_BasicAudio_2333_295_84_67_18_Destinations
    },

    // Translation record for BasicAudioBalanceSleepingBeautyMutesBass '<S33>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2333_295_143_67_18, 1,
        Translation_BasicAudio_2333_295_143_67_18_Sources, 1,
        Translation_BasicAudio_2333_295_143_67_18_Destinations
    },

    // Translation record for BasicAudioBalanceEnableSilentExtreme '<S37>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2333_295_85_67_18, 1,
        Translation_BasicAudio_2333_295_85_67_18_Sources, 1,
        Translation_BasicAudio_2333_295_85_67_18_Destinations
    },

    // Translation record for BasicAudioBalanceTableDb,BasicAudioBalanceTableIdx '<S40>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2333_295_90, 1,
        Translation_BasicAudio_2333_295_90_Sources, 2,
        Translation_BasicAudio_2333_295_90_Destinations
    },

    // Translation record for BasicAudioBassBoostCoeffs '<S45>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2337_236_149, 1,
        Translation_BasicAudio_2337_236_149_Sources, 1,
        Translation_BasicAudio_2337_236_149_Destinations
    },

    // Translation record for BasicAudioBassCutCoeffs '<S46>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2337_236_153, 1,
        Translation_BasicAudio_2337_236_153_Sources, 1,
        Translation_BasicAudio_2337_236_153_Destinations
    },

    // Translation record for BasicAudioBassBoostMaximum_dB '<S44>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2337_236_174, 1,
        Translation_BasicAudio_2337_236_174_Sources, 1,
        Translation_BasicAudio_2337_236_174_Destinations
    },

    // Translation record for BasicAudioBassCutMaximum_dB '<S47>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2337_236_176, 1,
        Translation_BasicAudio_2337_236_176_Sources, 1,
        Translation_BasicAudio_2337_236_176_Destinations
    },

    // Translation record for BasicAudioBassOffset '<S61>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2337_236_210_67_18, 1,
        Translation_BasicAudio_2337_236_210_67_18_Sources, 1,
        Translation_BasicAudio_2337_236_210_67_18_Destinations
    },

    // Translation record for BasicAudioBassramptime '<S65>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2337_236_229_67_18, 1,
        Translation_BasicAudio_2337_236_229_67_18_Sources, 1,
        Translation_BasicAudio_2337_236_229_67_18_Destinations
    },

    // Translation record for BasicAudioBassScale '<S69>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2337_236_209_67_18, 1,
        Translation_BasicAudio_2337_236_209_67_18_Sources, 1,
        Translation_BasicAudio_2337_236_209_67_18_Destinations
    },

    // Translation record for BasicAudioMuteRampTime '<S77>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2341_254_276_67_18, 1,
        Translation_BasicAudio_2341_254_276_67_18_Sources, 1,
        Translation_BasicAudio_2341_254_276_67_18_Destinations
    },

    // Translation record for BasicAudioTrebleBoostCoeffs '<S82>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2345_236_149, 1,
        Translation_BasicAudio_2345_236_149_Sources, 1,
        Translation_BasicAudio_2345_236_149_Destinations
    },

    // Translation record for BasicAudioTrebleCutCoeffs '<S83>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2345_236_153, 1,
        Translation_BasicAudio_2345_236_153_Sources, 1,
        Translation_BasicAudio_2345_236_153_Destinations
    },

    // Translation record for BasicAudioTrebleBoostMaximum_dB '<S81>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2345_236_174, 1,
        Translation_BasicAudio_2345_236_174_Sources, 1,
        Translation_BasicAudio_2345_236_174_Destinations
    },

    // Translation record for BasicAudioTrebleCutMaximum_dB '<S84>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2345_236_176, 1,
        Translation_BasicAudio_2345_236_176_Sources, 1,
        Translation_BasicAudio_2345_236_176_Destinations
    },

    // Translation record for BasicAudioTrebleOffset '<S98>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2345_236_210_67_18, 1,
        Translation_BasicAudio_2345_236_210_67_18_Sources, 1,
        Translation_BasicAudio_2345_236_210_67_18_Destinations
    },

    // Translation record for BasicAudioTrebleramptime '<S102>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2345_236_229_67_18, 1,
        Translation_BasicAudio_2345_236_229_67_18_Sources, 1,
        Translation_BasicAudio_2345_236_229_67_18_Destinations
    },

    // Translation record for BasicAudioTrebleScale '<S106>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2345_236_209_67_18, 1,
        Translation_BasicAudio_2345_236_209_67_18_Sources, 1,
        Translation_BasicAudio_2345_236_209_67_18_Destinations
    },

    // Translation record for BasicAudioVolumeTable_Idx,BasicAudioVolumeTable_dB '<S112>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2349_263_280, 1,
        Translation_BasicAudio_2349_263_280_Sources, 2,
        Translation_BasicAudio_2349_263_280_Destinations
    },

    // Translation record for BasicAudioVolumemaxgain '<S118>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2349_263_281_67_18, 1,
        Translation_BasicAudio_2349_263_281_67_18_Sources, 1,
        Translation_BasicAudio_2349_263_281_67_18_Destinations
    },

    // Translation record for BasicAudioVolumeRampTime '<S122>/S-Function'
    {
        0, &
        FuncTranslation_BasicAudio_2349_263_256_67_18, 1,
        Translation_BasicAudio_2349_263_256_67_18_Sources, 1,
        Translation_BasicAudio_2349_263_256_67_18_Destinations
    },
};

// Share table in a public interface for use by top-model
void* BasicAudio_TranslationEngine64_translation_table = translation_table;
int BasicAudio_TranslationEngine64_translation_table_count = sizeof
    (translation_table) / sizeof(TranslationRecord_type);

// -----------------------------------------------------
// Local storage
static unsigned int theTranslationErrorState = 0;
static char translateErrorBuffer[1024];
static int translateErrorCharsUsed = 0;

// -----------------------------------------------------
// Translation engine entry points:

// forward declare local functions
static void SetTranslationErrorState(unsigned int errorState);
static void ErrorVPrintf(const char* formatString, va_list args);
static void ErrorPrintf(const char* formatString, ...);

// -----------------------------------------------------
// Top-level model root array to access engine tables:
int numTranslationRoots = 0;
TranslationRoot* translationRoots[BasicAudio_TranslationEngine64_ModelCount] =
{
    0
};

void RegisterTranslationRoot(TranslationRoot* aRoot)
{
    int i;
    const int arraySize = BasicAudio_TranslationEngine64_ModelCount;
    for (i=0; i < arraySize; i++)
    {
        if (aRoot == translationRoots[i])
        {
            // already here!
            return;
        }

        if (0 == translationRoots[i])
        {
            // found a slot at the end
            translationRoots[i] = aRoot;
            numTranslationRoots = i+1;
            return;
        }
    }

    // ERROR! Array size was too small! This should not happen.
    while (1) ;
}

TranslateEngineIterator INVALID_TRANSLATE_ITERATOR =
{
    -1, -1
};

int IsValidTranslateIterator(TranslateEngineIterator anIterator)
{
    return (anIterator.model >= 0) && (anIterator.index >= 0);
}

static TranslationRecord_type* GetTranslationRecord(TranslateEngineIterator
    translationNumIn)
{
    // Validate input parameters
    if ((translationNumIn.model < 0) || (translationNumIn.model >=
            numTranslationRoots))
    {
        return 0;                      // model out of range
    }

    TranslationRoot* theRoot = translationRoots[translationNumIn.model];
    if ((translationNumIn.index < 0) || (translationNumIn.index >=
            *theRoot->translation_count))
    {
        return 0;                      // index out of range for specified model
    }

    TranslationRecord_type* theTable = *(TranslationRecord_type**)
        theRoot->translation_table;
    return &theTable[translationNumIn.index];
}

// Return the translation record that outputs specified TOP (or INVALID_TRANSLATE_ITERATOR if not found)
static TranslateEngineIterator FindTranslationForTop(const char* topNameIn)
{
    TranslateEngineIterator retVal = INVALID_TRANSLATE_ITERATOR;
    TranslateEngineIterator theOne;
    int m;
    int foundIt = 0;
    TranslationRecord_type* aRecord;

    // Loop through models
    for (m = 0; (0 == foundIt) && (m < numTranslationRoots); m++)
    {
        theOne.model = m;
        theOne.index = 0;

        // Loop through translations of this model
        for (aRecord = GetTranslationRecord(theOne); (0 == foundIt) && aRecord;
             aRecord = GetTranslationRecord(theOne))
        {
            // Loop through TOPs of this translation
            int topIndex;
            for (topIndex = 0; (0 == foundIt) && (topIndex < aRecord->top_count);
                 topIndex++)
            {
                TranslateEngineIterator aTop;
                aTop.model = m;
                aTop.index = aRecord->tops[topIndex];
                const char* aTopName = GetTopName(aTop);
                if (aTopName && (0 == strcmp(aTopName, topNameIn)))
                {
                    // Found it!
                    foundIt = 1;
                    retVal = theOne;
                    break;
                }
            }

            // move on to the next translation
            theOne.index++;
        }
    }

    return retVal;
}

// Retrieve space-delimited list of TuneVars needed for specified translation
// Returns char count placed in buffer
static int ListTuneVarsForTranslation(TranslateEngineIterator translationNumIn,
    char* bufferInOut, int bufferSizeIn)
{
    char* buf = bufferInOut;
    int bufSize = bufferSizeIn;
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (aRecord)
    {
        // TOPs can only be fed by one translation - which was provided
        // Loop through and dump TuneVar names
        int i;
        for (i = 0; i < aRecord->tunevar_count; i++)
        {
            TranslateEngineIterator aTuneVar;
            aTuneVar.model = translationNumIn.model;
            aTuneVar.index = aRecord->tunevars[i];
            int used = 0;
            if (i > 0)
            {
                used = snprintf(buf, bufSize, " ");
                buf += used;
                bufSize -= used;
            }

            used = snprintf(buf, bufSize, "%s", GetTuneVarName(aTuneVar));
            buf += used;
            bufSize -= used;
        }
    }

    return (bufferSizeIn - bufSize);
}

// Returns space-delimited list of TuneVars (in bufferInOut) that feed specified top
// Returns char count placed in buffer
int ListTuneVarsForTop(const char* topNameIn, char* bufferInOut, int
                       bufferSizeIn)
{
    TranslateEngineIterator theTranslation = FindTranslationForTop(topNameIn);
    return ListTuneVarsForTranslation(theTranslation,bufferInOut,bufferSizeIn);
}

// Touch translation object to indicate new data is available and
// must be translated.
void TouchTranslation(TranslateEngineIterator translationNumIn)
{
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (aRecord)
    {
        aRecord->updated_tune_vars++;
    }
}

// Get a translation's dirty flag in the form of updated TuneVar count
int GetTranslationUpdatedTuneVarCount(TranslateEngineIterator translationNumIn)
{
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (aRecord)
    {
        return aRecord->updated_tune_vars;
    }

    return 0;                          // bad iterator
}

// Clear all flags that would indicate a translation must be run
void ClearAllTranslationRequiredFlags( void )
{
    TranslationRecord_type* aRecord;
    TranslateEngineIterator translationNum;
    int m;
    for (m = 0; m < numTranslationRoots; m++)
    {
        translationNum.model = m;
        translationNum.index = 0;
        for (aRecord = GetTranslationRecord(translationNum); aRecord;
             aRecord = GetTranslationRecord(translationNum))
        {
            aRecord->updated_tune_vars = 0;
            translationNum.index++;
        }
    }
}

// Return the first translation record that has been touched to run (or INVALID_TRANSLATE_ITERATOR if none)
TranslateEngineIterator FindFirstNeededTranslation( void )
{
    TranslateEngineIterator retVal = INVALID_TRANSLATE_ITERATOR;
    TranslateEngineIterator theFirst;
    int m;
    TranslationRecord_type* aRecord;
    for (m = 0; m < numTranslationRoots; m++)
    {
        theFirst.model = m;
        theFirst.index = 0;
        for (aRecord = GetTranslationRecord(theFirst); aRecord;
             aRecord = GetTranslationRecord(theFirst))
        {
            if (aRecord->updated_tune_vars != 0)
            {
                // Found it!
                retVal = theFirst;
                break;
            }

            theFirst.index++;
        }
    }

    return retVal;
}

// Return the next translation record that has been touched to run (or INVALID_TRANSLATE_ITERATOR if no more)
TranslateEngineIterator FindNextNeededTranslation(TranslateEngineIterator
    afterThisOne)
{
    TranslateEngineIterator retVal = INVALID_TRANSLATE_ITERATOR;
    TranslateEngineIterator nextOne;
    nextOne.index = afterThisOne.index + 1;
    int m;
    int foundIt = 0;
    TranslationRecord_type* aRecord;
    for (m = afterThisOne.model; (0 == foundIt) && (m < numTranslationRoots); m
            ++)
    {
        nextOne.model = m;
        for (aRecord = GetTranslationRecord(nextOne); aRecord;
             aRecord = GetTranslationRecord(nextOne))
        {
            if (aRecord->updated_tune_vars != 0)
            {
                // Found it!
                foundIt = 1;
                retVal = nextOne;
                break;
            }

            // move on to the next translation
            nextOne.index++;
        }

        // Reset the index in preparation for the next model search
        nextOne.index = 0;
    }

    return retVal;
}

// Verify that all TuneVars read by the specified translation have been seen
// Returns 0 if OK; nonzero if error and sets translation error state
int CheckTranslationSourceSightingsOK(TranslateEngineIterator translationNumIn)
{
    int retVal = 0;                    // assume signtings are OK
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (0 == aRecord)
        return 0;                      // if translation not found just bail ok

    // Loop through and check that all source TuneVars have been seen
    int i;
    for (i = 0; i < aRecord->tunevar_count; i++)
    {
        TranslateEngineIterator aTuneVar;
        aTuneVar.model = translationNumIn.model;
        aTuneVar.index = aRecord->tunevars[i];
        int sightings = GetTuneVarSightings(aTuneVar);
        if (sightings == 0)
        {
            // Error - have not seen this one yet!
            retVal = 1;
            ErrorPrintf("Missing TuneVar %s required for translation#%d:%d\n",
                        GetTuneVarName(aTuneVar), translationNumIn.model,
                        translationNumIn.index);

            // Missing TuneVar. Set the error state
            SetTranslationErrorState(1);
        }
    }

    return retVal;
}

// Checks source sightings & calls translation function
// Reset dirty flags only if requested and no error in translation
// returns error state (0 means OK);
int CallTranslation(TranslateEngineIterator translationNumIn, int
                    cleanFlagIfNoError)
{
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (0 == aRecord)
        return 0;                      // if translation not found just bail ok

    // remember the error state and then clear it so we can check *this*
    // translation's encounters with error
    unsigned int errorStateBefore = GetTranslationErrorState();
    SetTranslationErrorState(0);

    // Get the translation record and run the translation
    int err = CheckTranslationSourceSightingsOK(translationNumIn);
    if (0 == err)
    {
        aRecord->translation_func();
    }

    unsigned int errorStateAfter = GetTranslationErrorState();
    if (0 != errorStateAfter)
    {
        ErrorPrintf("... while processing TuneVars { ");
        int i;
        for (i = 0; i < aRecord->tunevar_count; i++)
        {
            TranslateEngineIterator tuneVar;
            tuneVar.model = translationNumIn.model;
            tuneVar.index = aRecord->tunevars[i];
            ErrorPrintf("%s ", GetTuneVarName(tuneVar));
        }

        ErrorPrintf("}\n");
    }

    // If requested to clean the flag and no errors happened
    if (cleanFlagIfNoError && 0 == errorStateAfter)
    {
        // Clear the dirty flag after a successful translation
        aRecord->updated_tune_vars = 0;
    }

    // If no new errors happened, bring back previous errors
    if (0 == errorStateAfter)
    {
        SetTranslationErrorState(errorStateBefore);
    }

    return GetTranslationErrorState();
}

// Returns characters dumped, or 0 if nothing to dump (i.e. top index too high), negative on buffer too small
int DumpTranslatedTOP(TranslateEngineIterator translationNumIn, int topIndexIn,
                      char* bufferInOut, int bufferSizeIn)
{
    int charsDumped = 0;
    TranslationRecord_type* aRecord = GetTranslationRecord(translationNumIn);
    if (aRecord)
    {
        if (topIndexIn >= 0 && topIndexIn < aRecord->top_count)
        {
            TranslateEngineIterator aTop;
            aTop.model = translationNumIn.model;
            aTop.index = aRecord->tops[topIndexIn];
            charsDumped = DumpTranslatedValue(aTop,bufferInOut,bufferSizeIn);
            if (charsDumped < 0)
            {
                ErrorPrintf("Error: Translation engine return buffer too small for TOP\n");
                SetTranslationErrorState(1);
            }
        }
    }

    return charsDumped;
}

static void SetTranslationErrorState(unsigned int errorState)
{
    theTranslationErrorState = errorState;
}

// Retrieve translation error state
unsigned int GetTranslationErrorState( void )
{
    return theTranslationErrorState;
}

// Internal use print to error buffer
static void ErrorVPrintf(const char* formatString, va_list args)
{
    char* buffer = &translateErrorBuffer[translateErrorCharsUsed];
    int bufferSize = sizeof(translateErrorBuffer) - translateErrorCharsUsed;
    if (bufferSize)
    {
        int charsPrinted = vsnprintf(
            buffer, bufferSize, formatString, args);
        translateErrorCharsUsed += charsPrinted;
    }
}

// Internal use print to error buffer
static void ErrorPrintf(const char* formatString, ...)
{
    va_list args;
    va_start(args, formatString);
    ErrorVPrintf(formatString, args);
    va_end(args);
}

// Report a translation error (works like printf)
// Called from the model code
void TranslateError(const char* formatString, ...)
{
    va_list args;
    va_start(args, formatString);
    ErrorVPrintf(formatString, args);
    va_end(args);

    // Add newline and set error state
    ErrorPrintf("\n");
    SetTranslationErrorState(1);
}

// Report a translation warning (works like printf)
// Called from the model code
void TranslateWarning(const char* formatString, ...)
{
    va_list args;
    va_start(args, formatString);
    ErrorVPrintf(formatString, args);
    va_end(args);

    // Add newline and set error state
    ErrorPrintf("\n");
}

// Retrieve reported error strings
const char* GetTranslationErrorStrings( void )
{
    return &translateErrorBuffer[0];
}

// Clear error state
void ClearTranslationErrorState( void )
{
    translateErrorBuffer[0] = 0;
    translateErrorCharsUsed = 0;
    SetTranslationErrorState(0);
}

TranslationRoot BasicAudio_TranslationEngine64_TranslationRoot =
{
    &BasicAudio_TranslationEngine64_tune_var_table,
    &BasicAudio_TranslationEngine64_translation_table,
    &BasicAudio_TranslationEngine64_TOP_RcvTrans_table,
    &BasicAudio_TranslationEngine64_tune_var_table_count,
    &BasicAudio_TranslationEngine64_translation_table_count,
    &BasicAudio_TranslationEngine64_TOP_RcvTrans_table_count
};

void BasicAudio_TranslationEngine64_InitializeTranslationRoot()
{
    RegisterTranslationRoot(&BasicAudio_TranslationEngine64_TranslationRoot);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
