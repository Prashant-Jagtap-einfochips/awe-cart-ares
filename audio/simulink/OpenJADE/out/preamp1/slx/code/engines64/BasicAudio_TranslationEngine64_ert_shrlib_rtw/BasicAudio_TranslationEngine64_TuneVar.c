/*
 * File: BasicAudio_TranslationEngine64_TuneVar.c
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

#include <ctype.h>
#include "rtwtypes.h"
#include "BasicAudio_TranslationEngine64_TuneVar.h"
#include "ASDID.h"                     // for SimulinkDatatypeId
#include "BasicAudio_TranslationEngine64.h" // for tune variable output signals
#include <stdio.h>                     // for sscanf, vsnprintf
#include <stdarg.h>                    // for variadic args (va_start)
#include <string.h>                    // for memset, memcpy, strlen, etc
#define MAX_TUNEVAR_DIM                7U

// TuneVar table record
typedef struct TuneVar_tag
{
    const char* name;                  // name of TuneVar
    int sightings_count;             // counts RecognizeTuneVariable invocations
    SimulinkDatatypeId dtype_id;      // SimulinkDatatypeId of numeric data type
    int string_parse;                  // 0=numeric input, 1=string input
    int element_bytes;                 // byte size of a single element
    int width;                         // maximum supported elements
    int max_size[MAX_TUNEVAR_DIM];     // maximum for each dimension
    int variable_dim;
                  // index of the dimension allowed to vary in size (for inputs)
    int num_dims;                      // number of dimensions
    int current_width;
             // width of current value (although stored in max dimensional size)
    void* default_value;
    int default_width;
    double* current_dims;              // dimensions of current value
    void* value_ptr;                   // array of current numeric value
    int translation_count;
                        // indicates how many translations feed off this TuneVar
    enum TranslationNumber *translations;
                        // array of translation numbers feeding off this TuneVar
}
TuneVar_type;

// Translation numbers local to this model:
enum TranslationNumber
{
    ID_Translation_BasicAudio_2333_295_79,/* '<S10>/S-Function' */
    ID_Translation_BasicAudio_2333_295_81_67_18,/* '<S21>/S-Function' */
    ID_Translation_BasicAudio_2333_295_83_67_18,/* '<S25>/S-Function' */
    ID_Translation_BasicAudio_2333_295_84_67_18,/* '<S29>/S-Function' */
    ID_Translation_BasicAudio_2333_295_143_67_18,/* '<S33>/S-Function' */
    ID_Translation_BasicAudio_2333_295_85_67_18,/* '<S37>/S-Function' */
    ID_Translation_BasicAudio_2333_295_90,/* '<S40>/S-Function' */
    ID_Translation_BasicAudio_2337_236_149,/* '<S45>/S-Function' */
    ID_Translation_BasicAudio_2337_236_153,/* '<S46>/S-Function' */
    ID_Translation_BasicAudio_2337_236_174,/* '<S44>/S-Function' */
    ID_Translation_BasicAudio_2337_236_176,/* '<S47>/S-Function' */
    ID_Translation_BasicAudio_2337_236_210_67_18,/* '<S61>/S-Function' */
    ID_Translation_BasicAudio_2337_236_229_67_18,/* '<S65>/S-Function' */
    ID_Translation_BasicAudio_2337_236_209_67_18,/* '<S69>/S-Function' */
    ID_Translation_BasicAudio_2341_254_276_67_18,/* '<S77>/S-Function' */
    ID_Translation_BasicAudio_2345_236_149,/* '<S82>/S-Function' */
    ID_Translation_BasicAudio_2345_236_153,/* '<S83>/S-Function' */
    ID_Translation_BasicAudio_2345_236_174,/* '<S81>/S-Function' */
    ID_Translation_BasicAudio_2345_236_176,/* '<S84>/S-Function' */
    ID_Translation_BasicAudio_2345_236_210_67_18,/* '<S98>/S-Function' */
    ID_Translation_BasicAudio_2345_236_229_67_18,/* '<S102>/S-Function' */
    ID_Translation_BasicAudio_2345_236_209_67_18,/* '<S106>/S-Function' */
    ID_Translation_BasicAudio_2349_263_280,/* '<S112>/S-Function' */
    ID_Translation_BasicAudio_2349_263_281_67_18,/* '<S118>/S-Function' */
    ID_Translation_BasicAudio_2349_263_256_67_18,/* '<S122>/S-Function' */
    NUM_TRANSLATION_NUMBERS
};

static real_T DefValue_BasicAudioBalanceChanRampMap[] =
{
    1.0, 2.0
};

static real_T DefValue_BasicAudioBalanceBoostDisable[] =
{
    0.0
};

static real_T DefValue_BasicAudioBalanceOffset[] =
{
    127.0
};

static real_T DefValue_BasicAudioBalanceRampTime[] =
{
    30.0
};

static real_T DefValue_BasicAudioBalanceSleepingBeautyMutesBass[] =
{
    1.0
};

static real_T DefValue_BasicAudioBalanceEnableSilentExtreme[] =
{
    0.0
};

static real_T DefValue_BasicAudioBalanceTable[] =
{
    0.0, 10.0, 31.0, 52.0, 74.0, 95.0, 116.0, 128.0, 138.0, 159.0, 180.0, 202.0,
    223.0, 244.0, 255.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, -30.0, -27.0, -14.0, -7.0, -5.0, -3.0, -1.0, 0.0, -1.0,
    -3.0, -5.0, -7.0, -14.0, -27.0, -30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};

static real_T DefValue_BasicAudioBassBoostFrequencyHz[] =
{
    200.0
};

static real_T DefValue_BasicAudioBassCutFrequencyHz[] =
{
    200.0
};

static real_T DefValue_BasicAudioBassBoostMaximum_dB[] =
{
    20.0
};

static real_T DefValue_BasicAudioBassCutMaximum_dB[] =
{
    20.0
};

static real_T DefValue_BasicAudioBassOffset[] =
{
    128.0
};

static real_T DefValue_BasicAudioBassramptime[] =
{
    30.0
};

static real_T DefValue_BasicAudioBassScale[] =
{
    127.0
};

static real_T DefValue_BasicAudioMuteRampTime[] =
{
    100.0
};

static real_T DefValue_BasicAudioTrebleBoostFrequencyHz[] =
{
    200.0
};

static real_T DefValue_BasicAudioTrebleCutFrequencyHz[] =
{
    200.0
};

static real_T DefValue_BasicAudioTrebleBoostMaximum_dB[] =
{
    20.0
};

static real_T DefValue_BasicAudioTrebleCutMaximum_dB[] =
{
    20.0
};

static real_T DefValue_BasicAudioTrebleOffset[] =
{
    128.0
};

static real_T DefValue_BasicAudioTrebleramptime[] =
{
    30.0
};

static real_T DefValue_BasicAudioTrebleScale[] =
{
    127.0
};

static real_T DefValue_BasicAudioVolumeTable[] =
{
    0.0, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -100.0, -30.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0
};

static real_T DefValue_BasicAudioVolumemaxgain[] =
{
    0.0
};

static real_T DefValue_BasicAudioVolumeRampTime[] =
{
    100.0
};

// ----- Dependent translations for each tune variable:
static enum TranslationNumber BasicAudioBalanceChanRampMap_Dependencies[] =/* '<S9>/TuneVarChanRampMap' */
{
    ID_Translation_BasicAudio_2333_295_79,
};

static enum TranslationNumber BasicAudioBalanceBoostDisable_Dependencies[] =/* '<S11>/Tune Variable' */
{
    ID_Translation_BasicAudio_2333_295_81_67_18,
};

static enum TranslationNumber BasicAudioBalanceOffset_Dependencies[] =/* '<S12>/Tune Variable' */
{
    ID_Translation_BasicAudio_2333_295_83_67_18,
};

static enum TranslationNumber BasicAudioBalanceRampTime_Dependencies[] =/* '<S13>/Tune Variable' */
{
    ID_Translation_BasicAudio_2333_295_84_67_18,
};

static enum TranslationNumber
    BasicAudioBalanceSleepingBeautyMutesBass_Dependencies[] =/* '<S14>/Tune Variable' */
{
    ID_Translation_BasicAudio_2333_295_143_67_18,
};

static enum TranslationNumber BasicAudioBalanceEnableSilentExtreme_Dependencies[]
    =                                  /* '<S15>/Tune Variable' */
{
    ID_Translation_BasicAudio_2333_295_85_67_18,
};

static enum TranslationNumber BasicAudioBalanceTable_Dependencies[] =/* '<S16>/TuneVarBalanceTable' */
{
    ID_Translation_BasicAudio_2333_295_90,
};

static enum TranslationNumber BasicAudioBassBoostFrequencyHz_Dependencies[] =/* '<S43>/BoostHz' */
{
    ID_Translation_BasicAudio_2337_236_149,
};

static enum TranslationNumber BasicAudioBassCutFrequencyHz_Dependencies[] =/* '<S43>/CutHz' */
{
    ID_Translation_BasicAudio_2337_236_153,
};

static enum TranslationNumber BasicAudioBassBoostMaximum_dB_Dependencies[] =/* '<S43>/Boost_dB' */
{
    ID_Translation_BasicAudio_2337_236_174,
};

static enum TranslationNumber BasicAudioBassCutMaximum_dB_Dependencies[] =/* '<S43>/Cut_dB' */
{
    ID_Translation_BasicAudio_2337_236_176,
};

static enum TranslationNumber BasicAudioBassOffset_Dependencies[] =/* '<S48>/Tune Variable' */
{
    ID_Translation_BasicAudio_2337_236_210_67_18,
};

static enum TranslationNumber BasicAudioBassramptime_Dependencies[] =/* '<S49>/Tune Variable' */
{
    ID_Translation_BasicAudio_2337_236_229_67_18,
};

static enum TranslationNumber BasicAudioBassScale_Dependencies[] =/* '<S50>/Tune Variable' */
{
    ID_Translation_BasicAudio_2337_236_209_67_18,
};

static enum TranslationNumber BasicAudioMuteRampTime_Dependencies[] =/* '<S74>/Tune Variable' */
{
    ID_Translation_BasicAudio_2341_254_276_67_18,
};

static enum TranslationNumber BasicAudioTrebleBoostFrequencyHz_Dependencies[] =/* '<S80>/BoostHz' */
{
    ID_Translation_BasicAudio_2345_236_149,
};

static enum TranslationNumber BasicAudioTrebleCutFrequencyHz_Dependencies[] =/* '<S80>/CutHz' */
{
    ID_Translation_BasicAudio_2345_236_153,
};

static enum TranslationNumber BasicAudioTrebleBoostMaximum_dB_Dependencies[] =/* '<S80>/Boost_dB' */
{
    ID_Translation_BasicAudio_2345_236_174,
};

static enum TranslationNumber BasicAudioTrebleCutMaximum_dB_Dependencies[] =/* '<S80>/Cut_dB' */
{
    ID_Translation_BasicAudio_2345_236_176,
};

static enum TranslationNumber BasicAudioTrebleOffset_Dependencies[] =/* '<S85>/Tune Variable' */
{
    ID_Translation_BasicAudio_2345_236_210_67_18,
};

static enum TranslationNumber BasicAudioTrebleramptime_Dependencies[] =/* '<S86>/Tune Variable' */
{
    ID_Translation_BasicAudio_2345_236_229_67_18,
};

static enum TranslationNumber BasicAudioTrebleScale_Dependencies[] =/* '<S87>/Tune Variable' */
{
    ID_Translation_BasicAudio_2345_236_209_67_18,
};

static enum TranslationNumber BasicAudioVolumeTable_Dependencies[] =/* '<S110>/TuneVar_Table' */
{
    ID_Translation_BasicAudio_2349_263_280,
};

static enum TranslationNumber BasicAudioVolumemaxgain_Dependencies[] =/* '<S113>/Tune Variable' */
{
    ID_Translation_BasicAudio_2349_263_281_67_18,
};

static enum TranslationNumber BasicAudioVolumeRampTime_Dependencies[] =/* '<S111>/Tune Variable' */
{
    ID_Translation_BasicAudio_2349_263_256_67_18,
};

// ----- Tune variable dispatch table:
static TuneVar_type tune_var_table[] =
{
    {                                  /* '<S9>/TuneVarChanRampMap' */
        "BasicAudioBalanceChanRampMap", 0, TSS_DOUBLE, 0, sizeof(real_T), 2,
        {
            1,
            2,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBalanceChanRampMap, 2,
        BasicAudio_TranslationEngine64_B.ChanRampMap.Size,
        BasicAudio_TranslationEngine64_B.ChanRampMap.Value, 1,
        BasicAudioBalanceChanRampMap_Dependencies
    },

    {                                  /* '<S11>/Tune Variable' */
        "BasicAudioBalanceBoostDisable", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBalanceBoostDisable, 1,
        BasicAudio_TranslationEngine64_B.BoostDisable.Size,
        &BasicAudio_TranslationEngine64_B.BoostDisable.Value, 1,
        BasicAudioBalanceBoostDisable_Dependencies
    },

    {                                  /* '<S12>/Tune Variable' */
        "BasicAudioBalanceOffset", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBalanceOffset, 1,
        BasicAudio_TranslationEngine64_B.Offset_k.Size,
        &BasicAudio_TranslationEngine64_B.Offset_k.Value, 1,
        BasicAudioBalanceOffset_Dependencies
    },

    {                                  /* '<S13>/Tune Variable' */
        "BasicAudioBalanceRampTime", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBalanceRampTime, 1,
        BasicAudio_TranslationEngine64_B.RampTime_d.Size,
        &BasicAudio_TranslationEngine64_B.RampTime_d.Value, 1,
        BasicAudioBalanceRampTime_Dependencies
    },

    {                                  /* '<S14>/Tune Variable' */
        "BasicAudioBalanceSleepingBeautyMutesBass", 0, TSS_DOUBLE, 0, sizeof
        (real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBalanceSleepingBeautyMutesBass, 1,
        BasicAudio_TranslationEngine64_B.SleepingBeautyMutesBass.Size,
        &BasicAudio_TranslationEngine64_B.SleepingBeautyMutesBass.Value, 1,
        BasicAudioBalanceSleepingBeautyMutesBass_Dependencies
    },

    {                                  /* '<S15>/Tune Variable' */
        "BasicAudioBalanceEnableSilentExtreme", 0, TSS_DOUBLE, 0, sizeof(real_T),
        1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBalanceEnableSilentExtreme, 1,
        BasicAudio_TranslationEngine64_B.EnableSilentExtreme.Size,
        &BasicAudio_TranslationEngine64_B.EnableSilentExtreme.Value, 1,
        BasicAudioBalanceEnableSilentExtreme_Dependencies
    },

    {                                  /* '<S16>/TuneVarBalanceTable' */
        "BasicAudioBalanceTable", 0, TSS_DOUBLE, 0, sizeof(real_T), 60,
        {
            30,
            2,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBalanceTable, 30,
        BasicAudio_TranslationEngine64_B.Table.Size,
        BasicAudio_TranslationEngine64_B.Table.Value, 1,
        BasicAudioBalanceTable_Dependencies
    },

    {                                  /* '<S43>/BoostHz' */
        "BasicAudioBassBoostFrequencyHz", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBassBoostFrequencyHz, 1,
        BasicAudio_TranslationEngine64_B.BoostFrequencyHz_o.Size,
        &BasicAudio_TranslationEngine64_B.BoostFrequencyHz_o.Value, 1,
        BasicAudioBassBoostFrequencyHz_Dependencies
    },

    {                                  /* '<S43>/CutHz' */
        "BasicAudioBassCutFrequencyHz", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBassCutFrequencyHz, 1,
        BasicAudio_TranslationEngine64_B.CutFrequencyHz_o.Size,
        &BasicAudio_TranslationEngine64_B.CutFrequencyHz_o.Value, 1,
        BasicAudioBassCutFrequencyHz_Dependencies
    },

    {                                  /* '<S43>/Boost_dB' */
        "BasicAudioBassBoostMaximum_dB", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBassBoostMaximum_dB, 1,
        BasicAudio_TranslationEngine64_B.BoostMaximum_dB_i.Size,
        &BasicAudio_TranslationEngine64_B.BoostMaximum_dB_i.Value, 1,
        BasicAudioBassBoostMaximum_dB_Dependencies
    },

    {                                  /* '<S43>/Cut_dB' */
        "BasicAudioBassCutMaximum_dB", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBassCutMaximum_dB, 1,
        BasicAudio_TranslationEngine64_B.CutMaximum_dB_a.Size,
        &BasicAudio_TranslationEngine64_B.CutMaximum_dB_a.Value, 1,
        BasicAudioBassCutMaximum_dB_Dependencies
    },

    {                                  /* '<S48>/Tune Variable' */
        "BasicAudioBassOffset", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBassOffset, 1,
        BasicAudio_TranslationEngine64_B.Offset_l.Size,
        &BasicAudio_TranslationEngine64_B.Offset_l.Value, 1,
        BasicAudioBassOffset_Dependencies
    },

    {                                  /* '<S49>/Tune Variable' */
        "BasicAudioBassramptime", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBassramptime, 1,
        BasicAudio_TranslationEngine64_B.ramptime_l.Size,
        &BasicAudio_TranslationEngine64_B.ramptime_l.Value, 1,
        BasicAudioBassramptime_Dependencies
    },

    {                                  /* '<S50>/Tune Variable' */
        "BasicAudioBassScale", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioBassScale, 1,
        BasicAudio_TranslationEngine64_B.Scale_o.Size,
        &BasicAudio_TranslationEngine64_B.Scale_o.Value, 1,
        BasicAudioBassScale_Dependencies
    },

    {                                  /* '<S74>/Tune Variable' */
        "BasicAudioMuteRampTime", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioMuteRampTime, 1,
        BasicAudio_TranslationEngine64_B.RampTime_c.Size,
        &BasicAudio_TranslationEngine64_B.RampTime_c.Value, 1,
        BasicAudioMuteRampTime_Dependencies
    },

    {                                  /* '<S80>/BoostHz' */
        "BasicAudioTrebleBoostFrequencyHz", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioTrebleBoostFrequencyHz, 1,
        BasicAudio_TranslationEngine64_B.BoostFrequencyHz.Size,
        &BasicAudio_TranslationEngine64_B.BoostFrequencyHz.Value, 1,
        BasicAudioTrebleBoostFrequencyHz_Dependencies
    },

    {                                  /* '<S80>/CutHz' */
        "BasicAudioTrebleCutFrequencyHz", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioTrebleCutFrequencyHz, 1,
        BasicAudio_TranslationEngine64_B.CutFrequencyHz.Size,
        &BasicAudio_TranslationEngine64_B.CutFrequencyHz.Value, 1,
        BasicAudioTrebleCutFrequencyHz_Dependencies
    },

    {                                  /* '<S80>/Boost_dB' */
        "BasicAudioTrebleBoostMaximum_dB", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioTrebleBoostMaximum_dB, 1,
        BasicAudio_TranslationEngine64_B.BoostMaximum_dB.Size,
        &BasicAudio_TranslationEngine64_B.BoostMaximum_dB.Value, 1,
        BasicAudioTrebleBoostMaximum_dB_Dependencies
    },

    {                                  /* '<S80>/Cut_dB' */
        "BasicAudioTrebleCutMaximum_dB", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioTrebleCutMaximum_dB, 1,
        BasicAudio_TranslationEngine64_B.CutMaximum_dB.Size,
        &BasicAudio_TranslationEngine64_B.CutMaximum_dB.Value, 1,
        BasicAudioTrebleCutMaximum_dB_Dependencies
    },

    {                                  /* '<S85>/Tune Variable' */
        "BasicAudioTrebleOffset", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioTrebleOffset, 1,
        BasicAudio_TranslationEngine64_B.Offset.Size,
        &BasicAudio_TranslationEngine64_B.Offset.Value, 1,
        BasicAudioTrebleOffset_Dependencies
    },

    {                                  /* '<S86>/Tune Variable' */
        "BasicAudioTrebleramptime", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioTrebleramptime, 1,
        BasicAudio_TranslationEngine64_B.ramptime.Size,
        &BasicAudio_TranslationEngine64_B.ramptime.Value, 1,
        BasicAudioTrebleramptime_Dependencies
    },

    {                                  /* '<S87>/Tune Variable' */
        "BasicAudioTrebleScale", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioTrebleScale, 1,
        BasicAudio_TranslationEngine64_B.Scale.Size,
        &BasicAudio_TranslationEngine64_B.Scale.Value, 1,
        BasicAudioTrebleScale_Dependencies
    },

    {                                  /* '<S110>/TuneVar_Table' */
        "BasicAudioVolumeTable", 0, TSS_DOUBLE, 0, sizeof(real_T), 16,
        {
            8,
            2,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioVolumeTable, 6,
        BasicAudio_TranslationEngine64_B.Table_i.Size,
        BasicAudio_TranslationEngine64_B.Table_i.Value, 1,
        BasicAudioVolumeTable_Dependencies
    },

    {                                  /* '<S113>/Tune Variable' */
        "BasicAudioVolumemaxgain", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioVolumemaxgain, 1,
        BasicAudio_TranslationEngine64_B.maxgain.Size,
        &BasicAudio_TranslationEngine64_B.maxgain.Value, 1,
        BasicAudioVolumemaxgain_Dependencies
    },

    {                                  /* '<S111>/Tune Variable' */
        "BasicAudioVolumeRampTime", 0, TSS_DOUBLE, 0, sizeof(real_T), 1,
        {
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        },
        0, 2, 0, DefValue_BasicAudioVolumeRampTime, 1,
        BasicAudio_TranslationEngine64_B.RampTime.Size,
        &BasicAudio_TranslationEngine64_B.RampTime.Value, 1,
        BasicAudioVolumeRampTime_Dependencies
    },
};

// Share table in a public interface for use by top-model
void* BasicAudio_TranslationEngine64_tune_var_table = tune_var_table;
int BasicAudio_TranslationEngine64_tune_var_table_count = sizeof(tune_var_table)
    / sizeof(TuneVar_type);

//

// -----------------------------------------------------
// Error reporting system: Local storage
static char tuneVarErrorBuffer[500];
static int tuneVarErrorCharsUsed = 0;

// forward declare local functions
static void ErrorVPrintf(const char* formatString, va_list args);
static void ErrorPrintf(const char* formatString, ...);

// Internal use print to error buffer
static void ErrorVPrintf(const char* formatString, va_list args)
{
    char* buffer = &tuneVarErrorBuffer[tuneVarErrorCharsUsed];
    int bufferSize = sizeof(tuneVarErrorBuffer) - tuneVarErrorCharsUsed;
    if (bufferSize)
    {
        int charsPrinted = vsnprintf(
            buffer, bufferSize, formatString, args);
        tuneVarErrorCharsUsed += charsPrinted;
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

// checks for string equality without regards to case
// returns 1 if lowercase strings are equal, 0 otherwise
static int insensitiveStrcmp(const char* A, const char* B)
{
    int i;
    for (i = 0; *A && *B; i++)
    {
        if (tolower(*A++) != tolower(*B++))
        {
            return 0;
        }
    }

    return (*A == *B);
}

static int GetColMajorIndex(TuneVar_type* aRecord, int rowMajorIndexIn)
{
    int colMajorIndex = -1;
    int maxElementIndex = aRecord->width;
    int d;
    if (rowMajorIndexIn >= maxElementIndex)
    {
        return -1;
    }

    // Determine position of this element (zero-based) in each dimension
    // using the current TuneVar size set above by "SetTuneVariableSize"
    // and the row-major index provided.
    if (aRecord->string_parse)
    {
        // STRING PARSING
        // each token is a full row, so TuneVar is just a list of strings
        // TODO: THIS NEEDS MULTI-DIM REWORKING. ONLY SUPPORTS LIST OF STRINGS.
        maxElementIndex = aRecord->width / aRecord->max_size[1];// get max rows
        if (rowMajorIndexIn >= maxElementIndex)
        {
            // do not allow the current row position to exceed maximum
            colMajorIndex = -1;
        }
        else
        {
            // For string_parse use, the column-major index is the row number!
            colMajorIndex = rowMajorIndexIn;
        }
    }
    else
    {
        // NORMAL NUMERIC PARSING
        // Find each dimension's position: divide index by total size of lower dims
        // and mod by the max for the dimension itself. For row-major indexes,
        // it's actually the second dimension (dim 1 aka columns) that comes first.
        int thisPos[MAX_TUNEVAR_DIM] =
        {
            0
        };

        int lowerDimsSize[MAX_TUNEVAR_DIM] =
        {
            0
        };

        // The first three dimensions take a little work to line up due to the permuation
        // of dim0 & dim1 (rows and columns)
        lowerDimsSize[1] = 1;
                    // in a text file, columns are the first dimension to change
        lowerDimsSize[0] = ((int)aRecord->current_dims[1]);// ... then rows
        lowerDimsSize[2] = lowerDimsSize[0] * ((int)aRecord->current_dims[0]);// ... and so on
        for (d = 3; d < aRecord->num_dims; d++)
        {
            lowerDimsSize[d] = lowerDimsSize[d-1] * ((int)aRecord->
                current_dims[d-1]);
        }

        // Now that the lower dimension sizes are set up, we can spin through to get positions:
        for (d = 0; d < aRecord->num_dims; d++)
        {
            thisPos[d] = (rowMajorIndexIn / lowerDimsSize[d]) % ((int)
                aRecord->current_dims[d]);
        }

        // to convert to col-major (for storage) recalculate the lower dims sizes with 2 changes
        // the row is now the dimension that matches 1-to-1 with overall index (i.e. first dim)
        // and the dimensions are based on maximum sizes instead of current sizes because the buffer
        // always works with maximal size allocated:
        lowerDimsSize[0] = 1;
        lowerDimsSize[1] = aRecord->max_size[0];
        for (d = 2; d < aRecord->num_dims; d++)
        {
            lowerDimsSize[d] = lowerDimsSize[d-1] * aRecord->max_size[d-1];
        }

        // ... then spin through to calculate overall col-based index
        colMajorIndex = 0;
        for (d = 0; d < aRecord->num_dims; d++)
        {
            colMajorIndex += thisPos[d] * lowerDimsSize[d];
        }
    }

    return colMajorIndex;
}

static TuneVar_type* GetTuneVarRecord(TranslateEngineIterator iteratorIn)
{
    if ((iteratorIn.model < 0) || (iteratorIn.model >= numTranslationRoots))
    {
        return 0;                      // model out of range
    }

    TranslationRoot* theRoot = translationRoots[iteratorIn.model];
    if ((iteratorIn.index < 0) || (iteratorIn.index >= *theRoot->tunevar_count))
    {
        return 0;                      // index out of range for specified model
    }

    TuneVar_type* theTable = *(TuneVar_type**)theRoot->tunevar_table;
    return &theTable[iteratorIn.index];
}

/*******************************************************/
/************  PUBLIC FUNCTIONS ************************/
/*******************************************************/

// Return a tune variable iterator, or INVALID_TRANSLATE_ITERATOR otherwise.
// No errors, error states, or other states / flags are modified.
TranslateEngineIterator GetTuneVarIteratorByName (const char* variableName)
{
    TranslateEngineIterator retVal = INVALID_TRANSLATE_ITERATOR;
    TranslateEngineIterator iterator;
    int foundIt = 0;
    TuneVar_type* aRecord;

    // Loop through table searching for a variable name match
    int m;
    for (m = 0; (m < numTranslationRoots) && (foundIt == 0); m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for (aRecord = GetTuneVarRecord(iterator); aRecord;
             aRecord = GetTuneVarRecord(iterator))
        {
            if (insensitiveStrcmp(variableName, aRecord->name))
            {
                // Found it!
                foundIt = 1;
                retVal = iterator;
                break;
            }

            iterator.index++;
        }
    }

    return retVal;
}

// Return a tune variable iterator if name is recognized, INVALID_TRANSLATE_ITERATOR otherwise.
// This routine also flips flags to recognize that a TuneVar has now been
// sighted and that associated translations should be run.
TranslateEngineIterator RecognizeTuneVariable(const char* variableName)
{
    TranslateEngineIterator iterator = GetTuneVarIteratorByName(variableName);
    TuneVar_type* theVar = GetTuneVarRecord(iterator);

    // If a variable was found, trigger all the flags announcing its arrival
    if (theVar)
    {
        theVar->sightings_count++;
        int tIdx;
        TranslateEngineIterator transItr;
        transItr.model = iterator.model;
        for (tIdx = 0; tIdx < theVar->translation_count; tIdx++)
        {
            transItr.index = theVar->translations[tIdx];
            TouchTranslation(transItr);
        }
    }
    else
    {
        // variable was *not* found. generate error string
        ErrorPrintf("Error: Unrecognized TuneVar [%s]\n", variableName);
    }

    return iterator;
}

// Reset all TuneVar sighting counts to zero
void ClearAllTuneSightings()
{
    TranslateEngineIterator iterator;
    TuneVar_type* aRecord = 0;
    int m;
    for (m = 0; m < numTranslationRoots; m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for (aRecord = GetTuneVarRecord(iterator); aRecord;
             aRecord = GetTuneVarRecord(iterator))
        {
            aRecord->sightings_count = 0;
            iterator.index++;
        }
    }
}

// Return count of how many times the specified TuneVar has been seen
int GetTuneVarSightings(TranslateEngineIterator tuneVarNumberIn)
{
    int sightings = 0;
    TuneVar_type* aRecord = GetTuneVarRecord(tuneVarNumberIn);
    if (aRecord)
    {
        sightings = aRecord->sightings_count;
    }

    return sightings;
}

// Return TuneVar name (or NULL if bad number)
const char* GetTuneVarName(TranslateEngineIterator tuneVarNumberIn)
{
    const char* name = 0;
    TuneVar_type* aRecord = GetTuneVarRecord(tuneVarNumberIn);
    if (aRecord)
    {
        name = aRecord->name;
    }

    return name;
}

const char* GetFirstTuneVarName(TranslateEngineIterator* tuneVarNumberIn)
{
    if (!tuneVarNumberIn)
        return 0;
    tuneVarNumberIn->model = 0;
    tuneVarNumberIn->index = 0;
    const char* aName = GetTuneVarName(*tuneVarNumberIn);
    if (aName == 0)
    {
        // First model had none. Try next:
        return GetNextTuneVarName(tuneVarNumberIn);
    }

    return aName;
}

const char* GetNextTuneVarName(TranslateEngineIterator* tuneVarNumberIn)
{
    if (!tuneVarNumberIn)
        return 0;

    // Check for next name in current model
    tuneVarNumberIn->index++;
    const char* aName = GetTuneVarName(*tuneVarNumberIn);

    // If no more - try the next model
    if (0 == aName)
    {
        // Name not found - so try the next model(s)
        tuneVarNumberIn->index = 0;
        while ((0 == aName) && (++(tuneVarNumberIn->model) < numTranslationRoots))
        {
            // name not found - so try the next model
            aName = GetTuneVarName(*tuneVarNumberIn);
        }
    }

    return aName;
}

// Set incoming TuneVar size - required for all TuneVars to account
// for variable size TuneVars and conversion from row-major to
// column-major storage indexing.
// Returns 0 for OK, 1 for error (does not set translation error state)
// ... but DOES set TuneVar error messages
int SetTuneVariableSize(TranslateEngineIterator tuneVarNumberIn, int countIn)
{
    const int errorCode = 1;
    int retVal = 0;

    // bail if provided number is not within the table
    TuneVar_type* aRecord = GetTuneVarRecord(tuneVarNumberIn);
    if (0 == aRecord)
        return errorCode;

    // otherwise, fetch the record, get maximum possible size, and clear storage
    int maxElementIndex = aRecord->width;
    memset(aRecord->value_ptr, 0, aRecord->element_bytes * maxElementIndex);

    // Determine input dimensions based on element count and the variable dimension
    // First reset all dimensions to max and find the quanta of elements:
    int elementQuanta = 1;
    int d;
    for (d = 0; d < aRecord->num_dims; d++)
    {
        aRecord->current_dims[d] = aRecord->max_size[d];
        if (d != aRecord->variable_dim)
        {
            elementQuanta *= aRecord->max_size[d];
        }
    }

    if (aRecord->string_parse)
    {
        // -------------- STRING PARSE SIZING ---------------
        // This is a string_parse TuneVar: each element (i.e. token) is a whole row
        maxElementIndex = aRecord->width / aRecord->max_size[1];// get max rows
        if (countIn > maxElementIndex)
        {
            // emit error - too much data here!
            ErrorPrintf(
                        "Error: TuneVar %s parses as a string and accepts only %d words. Ignoring %d extra words\n",
                        aRecord->name, maxElementIndex, countIn -
                        maxElementIndex);

            // NOTE: this error does *not* set the translation error state.
            retVal = errorCode;

            // enforce maximum size regardless of how much data provided
            // extra data will be ignored
            countIn = maxElementIndex;
        }

        // Determine the variable dimension, but not if it's columns
        // ... because the columns are always maxed out (copied above)
        if (aRecord->variable_dim != 1 && aRecord->variable_dim <
                aRecord->num_dims)
        {
            // take columns out of the picture:
            elementQuanta = elementQuanta / aRecord->max_size[1];

            // and calculate the variable dimension from what's left
            aRecord->current_dims[aRecord->variable_dim] = (int)((countIn) /
                elementQuanta);
        }

        // and record current width ("string parse" always fills full rows)
        aRecord->current_width = countIn * aRecord->max_size[1];
    }
    else
    {
        // -------------- NORMAL NUMERIC SIZING ---------------
        if (countIn > maxElementIndex)
        {
            // emit error - too much data here!
            ErrorPrintf(
                        "Error: TuneVar %s accepts only %d elements. Encountered %d extra elements\n",
                        aRecord->name, maxElementIndex, countIn -
                        maxElementIndex);

            // NOTE: this error does *not* set the translation error state.
            retVal = errorCode;

            // enforce maximum size regardless of how much data provided
            // extra data will be ignored
            countIn = maxElementIndex;
        }

        // Warn if the count does not fill out an even quanta
        // ... and then expand the count to an even quanta
        int extraNeeded = (elementQuanta - (countIn % elementQuanta)) %
            elementQuanta;
        if (extraNeeded)
        {
            // emit an error - not enough data here!
            ErrorPrintf(
                        "Error: TuneVar %s only accepts multiples of %d elements. Adding %d extra zeros to complete last grouping.\n",
                        aRecord->name, elementQuanta, extraNeeded);
            countIn = countIn + extraNeeded;

            // NOTE: this error does *not* set the translation error state. (so it's a warning I suppose)
            retVal = errorCode;
        }

        // Determine the variable dimension
        if (aRecord->variable_dim < aRecord->num_dims)
        {
            aRecord->current_dims[aRecord->variable_dim] = countIn /
                elementQuanta;
        }

        // and record current width
        aRecord->current_width = countIn;
    }

    return retVal;
}

// Set a single value of the tune variable. Provide total count of
// values in addition to the index of current value to set. The index
// traverses the TuneVar storage in row-major organization which is
// typical for C language matrix storage (i.e. same as the way a matrix
// is represented in a text file... listing out each row contiguously).
// Returns 0 for OK, 1 for error (does not set translation error state)
// ... but DOES set TuneVar error messages
int SetTuneVariableElement(TranslateEngineIterator tuneVarNumberIn, int
    rowMajorIndexIn, const char* stringElementValueIn)
{
    // By default, MATLAB/Simulink-generated code accesses data storage
    // in column major format where the matrix walks down through each
    // column first. This is different from standard C language and
    // from the rowMajorIndexIn assumptions for this function signature.
    const int errorCode = 1;
    int retVal = 0;

    // bail if provided number is not within the table
    TuneVar_type* aRecord = GetTuneVarRecord(tuneVarNumberIn);
    if (0 == aRecord)
        return errorCode;
    int colMajorIndex = GetColMajorIndex(aRecord, rowMajorIndexIn);
    if (colMajorIndex < 0)
    {
        // Error string should have been registered in SetTuneVariableSize above.
        // Just return the error code
        return errorCode;
    }

    // parse the value string, placing numeric value into the TuneVar
    SimulinkDatatypeId dTypeId = aRecord->dtype_id;
    void* valPtr = aRecord->value_ptr;
    switch (dTypeId)
    {
      case TSS_DOUBLE:
        sscanf(stringElementValueIn, "%lf",&((double*) valPtr)[colMajorIndex]);
        break;

      case TSS_SINGLE:
        sscanf(stringElementValueIn, "%f", &((float*) valPtr)[colMajorIndex]);
        break;

      case TSS_INT8:
        sscanf(stringElementValueIn, "%hhi", &((int8_T*) valPtr)[colMajorIndex]);
        break;

      case TSS_UINT8:
        if (aRecord->string_parse)
        {
            // Perform string parsing of element value... a single
            // ... string token contains all elements
            int strLength = (int)strlen(stringElementValueIn);
            int cols = aRecord->max_size[1];
            int c;
            char* destChar;
            if (strLength > cols)
            {
                strLength = cols;
                ErrorPrintf(
                            "Error: TuneVar %s accepts only %d characters. Encountered extra characters [%s]\n",
                            aRecord->name, cols, (char*)
                            &stringElementValueIn[cols]);

                // NOTE: this error does *not* set the translation error state.
                retVal = errorCode;    // return error code... string too long
            }

            // OK... just copy it in but use column-major layout
            // Note that for string_parse, colMajorIndex == rowMajorIndex == index-of-input-token
            destChar = (char*)valPtr;
            for (c=0; c < strLength; c++)
            {
                destChar[(c * aRecord->max_size[0]) + colMajorIndex] =
                    stringElementValueIn[c];
            }
        }
        else
        {
            // "Normal" numeric parsing of element value
            sscanf(stringElementValueIn, "%hhu", &((uint8_T*) valPtr)
                   [colMajorIndex]);
        }
        break;

      case TSS_INT16:
        sscanf(stringElementValueIn, "%hi", &((int16_T*) valPtr)[colMajorIndex]);
        break;

      case TSS_UINT16:
        sscanf(stringElementValueIn, "%hu", &((uint16_T*)valPtr)[colMajorIndex]);
        break;

      case TSS_INT32:
        sscanf(stringElementValueIn, "%i", &((int32_T*) valPtr)[colMajorIndex]);
        break;

      case TSS_UINT32:
        sscanf(stringElementValueIn, "%u", &((uint32_T*)valPtr)[colMajorIndex]);
        break;

      case TSS_BOOLEAN:
        sscanf(stringElementValueIn, "%hhi", &((int8_T*)valPtr)[colMajorIndex]);
        break;
    }

    return retVal;
}

// Retrieve reported error strings
const char* GetTuneVarErrorStrings()
{
    return &tuneVarErrorBuffer[0];
}

// Clear error state (external function)
void ClearTuneVarErrorStrings()
{
    tuneVarErrorBuffer[0] = 0;
    tuneVarErrorCharsUsed = 0;
}

// assumed: used,returnBuffer,remaining,aRecord->value_ptr,colMajorIndex
#define TYPED_PRINT(STYPE,FMTSTR)      {STYPE srcVal = ((STYPE*)aRecord->value_ptr)[colMajorIndex]; used=snprintf(&(returnBuffer[retUsed]),remaining,FMTSTR,srcVal);}

// returns space delimited list of elements
// sets TuneVar error strings upon error
int PrintTuneVarToBuffer(TranslateEngineIterator tuneVarNumberIn, char
    * returnBuffer, int returnBufferSize)
{
    int failDetected = 0;
    int retUsed = 0;
    int used = 0;                // characters used in the print of each element
    int remaining = returnBufferSize; // amount remaining to use in returnBuffer
    int i;

    // bail if provided number is not within the table
    TuneVar_type* aRecord = GetTuneVarRecord(tuneVarNumberIn);
    if (0 == aRecord)
        return 0;
    int elemsOut = aRecord->current_width;
    if ((aRecord->dtype_id == TSS_UINT8) && aRecord->string_parse)
    {
        elemsOut = aRecord->current_width / aRecord->max_size[1];
    }

    // loop through current width in ROW-MAJOR order
    for (i=0; i < elemsOut; i++)
    {
        // Start print of new element
        used = 0;
        int colMajorIndex = GetColMajorIndex(aRecord, i);
        if (colMajorIndex < 0)
            break;

        // output the element!
        switch (aRecord->dtype_id)
        {
          case TSS_DOUBLE :
            TYPED_PRINT(real64_T ," %.17g" );
            break;

          case TSS_SINGLE :
            TYPED_PRINT(real32_T ," %.9g" );
            break;

          case TSS_INT8 :
            TYPED_PRINT(int8_T ," %i" );
            break;

          case TSS_UINT8 :
            if (aRecord->string_parse)
            {
                // Copy out character-at-a-time using column-major layout
                // Note that for string_parse, colMajorIndex == rowMajorIndex == index-of-token
                int strLength = aRecord->max_size[1];
                char* destChar = (char*)&(returnBuffer[retUsed]);

                // keep track of this word, but leave room for a terminator
                int tempRemaining = remaining ? (remaining - 1) : 0;
                int c;

                // add a space between values
                used++;
                if (tempRemaining)
                {
                    *destChar++ = ' ';
                    tempRemaining--;
                }

                // pour in a single row
                for (c=0; c < strLength; c++)
                {
                    char* srcChar = &((char*)aRecord->value_ptr)[ c *
                        aRecord->max_size[0] + colMajorIndex ];
                    if (*srcChar)
                    {
                        used++;
                        if (tempRemaining)
                        {
                            *destChar++ = *srcChar;
                            tempRemaining--;
                        }
                    }
                }

                // terminate, but leave everything sitting on the terminator
                if (remaining)
                {
                    *destChar = 0;
                }
            }
            else
                TYPED_PRINT(uint8_T ," %u" );
            break;

          case TSS_INT16 :
            TYPED_PRINT(int16_T ," %i" );
            break;

          case TSS_UINT16 :
            TYPED_PRINT(uint16_T ," %u" );
            break;

          case TSS_INT32 :
            TYPED_PRINT(int32_T ," %i" );
            break;

          case TSS_UINT32 :
            TYPED_PRINT(uint32_T ," %u" );
            break;

          case TSS_BOOLEAN :
            TYPED_PRINT(int8_T ," %i" );
            break;

          default:
            ErrorPrintf("Bad TuneVar Type %s=%i",aRecord->name,aRecord->dtype_id);
            break;
        }

        // keep track of buffer usage, but don't let remaining size go negative
        retUsed += used;
        remaining -= used;
        remaining = (remaining < 0) ? 0 : remaining;
        if (used <= 0)
        {
            // Older implementations of snprintf only give failure
            // (instead of indicating how many bytes would be needed)
            failDetected = 1;
        }
    }

    if (failDetected)
    {
        // For old implementations of snprintf, this is what you get
        ErrorPrintf("Error: Translation engine return buffer is too small for TuneVar.\n");
    }
    else if (retUsed >= returnBufferSize)
    {
        // For C99 compliant snprintf, the error is more helpful:
        ErrorPrintf("Error: Return buffer requires %d bytes, but only %d provided\n",
                    retUsed, returnBufferSize);
    }

    return retUsed;
}

// Load default values for all TuneVars
void LoadAllTuneVarDefaults(void)
{
    TranslateEngineIterator iterator;
    TuneVar_type* aRecord = 0;

    // Loop through all TuneVars
    int m;
    for (m = 0; m < numTranslationRoots; m++)
    {
        iterator.model = m;
        iterator.index = 0;
        for (aRecord = GetTuneVarRecord(iterator); aRecord;
             aRecord = GetTuneVarRecord(iterator))
        {
            // Fully register that this TuneVar is seen/modified
            RecognizeTuneVariable(aRecord->name);

            // set the size of the value
            SetTuneVariableSize(iterator, aRecord->default_width);

            // copy in the actual default value itself
            int bufferSize = aRecord->element_bytes * aRecord->width;
            memcpy(aRecord->value_ptr, aRecord->default_value, bufferSize);

            // move on to next tunevar
            iterator.index++;
        }
    }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
