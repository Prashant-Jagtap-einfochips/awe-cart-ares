/*
 * File: BasicAudio_TranslationEngine64_TOP_RcvTrans.c
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

#include "rtwtypes.h"
#include "BasicAudio_TranslationEngine64_TOP_RcvTrans.h"
#include "BasicAudio_TranslationEngine64.h" // for TOP translated inputs
#include "ASDID.h"                     // for SimulinkDatatypeId
#include <stdio.h>                     // for snprintf

// Struct for table (moved to *.h file, see MathWorks Case#02530176)
typedef struct TOP_RcvTrans_tag
{
    const char* top_name;             // String name of Target Overlay Parameter
    int top_sightings_count;   // Count how many times translated value received
    SimulinkDatatypeId top_in_dtype_id;
                             // Id number of numeric type (e.g. float, int, etc)
    SimulinkDatatypeId top_out_dtype_id;
                             // Id number of numeric type (e.g. float, int, etc)
    const char* top_in_dtype_name;     // Name of numeric type
    const char* top_out_dtype_name;    // Name of numeric type
    int top_is_complex;            // "1" if complex number (interleaved format)
    int top_element_count;             // count of numeric data values
    void* top_value_src_ptr;           // array of translated numeric values
    void* top_value_dst_ptr;       // Source Model only : the TOP storage itself
}
TOP_RcvTrans_type;

// ----- TOP_RcvTrans object table listing all instances
static TOP_RcvTrans_type TOP_RcvTrans_table[] =
{
    {                                  /* '<S9>/TOP' */
        "BasicAudioBalanceChanRampMap", 0,
        TSS_INT32, TSS_SINGLE, "int32", "single", 0, 2,
        BasicAudio_TranslationEngine64_B.ChanRampMap_i,
        0
    },

    {                                  /* '<S11>/TOP' */
        "BasicAudioBalanceBoostDisable", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_l0,
        0
    },

    {                                  /* '<S12>/TOP' */
        "BasicAudioBalanceOffset", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_iw,
        0
    },

    {                                  /* '<S13>/TOP' */
        "BasicAudioBalanceRampTime", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_d,
        0
    },

    {                                  /* '<S14>/TOP' */
        "BasicAudioBalanceSleepingBeautyMutesBass", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_ih,
        0
    },

    {                                  /* '<S15>/TOP' */
        "BasicAudioBalanceEnableSilentExtreme", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_g,
        0
    },

    {                                  /* '<S16>/TOP_TableIDb' */
        "BasicAudioBalanceTableDb", 0,
        TSS_SINGLE, TSS_SINGLE, "single", "single", 0, 30,
        BasicAudio_TranslationEngine64_B.tableDb,
        0
    },

    {                                  /* '<S16>/TOP_TableIdx' */
        "BasicAudioBalanceTableIdx", 0,
        TSS_UINT32, TSS_SINGLE, "uint32", "single", 0, 30,
        BasicAudio_TranslationEngine64_B.tableIdx,
        0
    },

    {                                  /* '<S43>/BoostCoeffs' */
        "BasicAudioBassBoostCoeffs", 0,
        TSS_SINGLE, TSS_SINGLE, "single", "single", 0, 3,
        BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate1.FilterCoeffs,
        0
    },

    {                                  /* '<S43>/CutCoeffs' */
        "BasicAudioBassCutCoeffs", 0,
        TSS_SINGLE, TSS_SINGLE, "single", "single", 0, 3,
        BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate2_e.FilterCoeffs,
        0
    },

    {                                  /* '<S43>/Boost_dB TOP' */
        "BasicAudioBassBoostMaximum_dB", 0,
        TSS_SINGLE, TSS_SINGLE, "single", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate3.maxOut,
        0
    },

    {                                  /* '<S43>/Cut_dB TOP' */
        "BasicAudioBassCutMaximum_dB", 0,
        TSS_SINGLE, TSS_SINGLE, "single", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate4_i.maxOut,
        0
    },

    {                                  /* '<S48>/TOP' */
        "BasicAudioBassOffset", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_p,
        0
    },

    {                                  /* '<S49>/TOP' */
        "BasicAudioBassramptime", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_lm,
        0
    },

    {                                  /* '<S50>/TOP' */
        "BasicAudioBassScale", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_h,
        0
    },

    {                                  /* '<S74>/TOP' */
        "BasicAudioMuteRampTime", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_b,
        0
    },

    {                                  /* '<S80>/BoostCoeffs' */
        "BasicAudioTrebleBoostCoeffs", 0,
        TSS_SINGLE, TSS_SINGLE, "single", "single", 0, 3,
        BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate1_d.FilterCoeffs,
        0
    },

    {                                  /* '<S80>/CutCoeffs' */
        "BasicAudioTrebleCutCoeffs", 0,
        TSS_SINGLE, TSS_SINGLE, "single", "single", 0, 3,
        BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate2_fq.FilterCoeffs,
        0
    },

    {                                  /* '<S80>/Boost_dB TOP' */
        "BasicAudioTrebleBoostMaximum_dB", 0,
        TSS_SINGLE, TSS_SINGLE, "single", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate3_fg.maxOut,
        0
    },

    {                                  /* '<S80>/Cut_dB TOP' */
        "BasicAudioTrebleCutMaximum_dB", 0,
        TSS_SINGLE, TSS_SINGLE, "single", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate4_lk.maxOut,
        0
    },

    {                                  /* '<S85>/TOP' */
        "BasicAudioTrebleOffset", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_e,
        0
    },

    {                                  /* '<S86>/TOP' */
        "BasicAudioTrebleramptime", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_l,
        0
    },

    {                                  /* '<S87>/TOP' */
        "BasicAudioTrebleScale", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_j,
        0
    },

    {                                  /* '<S110>/TOP_GainDB' */
        "BasicAudioVolumeTable_dB", 0,
        TSS_SINGLE, TSS_SINGLE, "single", "single", 0, 8,
        BasicAudio_TranslationEngine64_B.Table_dB,
        0
    },

    {                                  /* '<S110>/TOP_Index' */
        "BasicAudioVolumeTable_Idx", 0,
        TSS_UINT32, TSS_UINT32, "uint32", "uint32", 0, 8,
        BasicAudio_TranslationEngine64_B.Table_Idx,
        0
    },

    {                                  /* '<S113>/TOP' */
        "BasicAudioVolumemaxgain", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value_i,
        0
    },

    {                                  /* '<S111>/TOP' */
        "BasicAudioVolumeRampTime", 0,
        TSS_DOUBLE, TSS_SINGLE, "double", "single", 0, 1,
        &BasicAudio_TranslationEngine64_B.Value,
        0
    },
};

// Share table in a public interface for use by top-model
void* BasicAudio_TranslationEngine64_TOP_RcvTrans_table = TOP_RcvTrans_table;
int BasicAudio_TranslationEngine64_TOP_RcvTrans_table_count = sizeof
    (TOP_RcvTrans_table) / sizeof(TOP_RcvTrans_type);

// CAST_PRINT_STORE performs the cast, print, and store
// Assumed:srcArray,dstArray,numElem,buf,bufSize,srcFix.factor,failDetected
// Note that SRCPREFIX is preprended to source value evaluations
#define CAST_PRINT_STORE(STYPE,DTYPE,FMTSTR,SRCPREFIX) {int index; for (index = 0; index < numElem; index++){ STYPE srcVal = ((STYPE *)srcArray)[index]; DTYPE dstVal = (srcFix.factor)? (DTYPE)((double)SRCPREFIX((srcVal)*srcFix.factor)):(DTYPE)(SRCPREFIX srcVal); if (bufSize > 1){ int used = snprintf(buf, bufSize, FMTSTR, dstVal); if (used <= 0) {used=0;failDetected=1;} bufSize -= used; buf += used; } if(dstArray)((DTYPE*)dstArray)[index]=dstVal; }}

// SRC_DST_SWITCH is invoked by STYPE code and to then invoke CAST_PRINT_STORE by dstType id
// Assumed:dstType. Note that boolean type uses "!!" to convert to 0 or 1
#define SRC_DST_SWITCH(STYPE)          { switch (dstType) { case TSS_DOUBLE : {CAST_PRINT_STORE(STYPE,double ," %.17g",);break;} case TSS_SINGLE : {CAST_PRINT_STORE(STYPE,float ," %.9g" ,);break;} case TSS_INT8 : {CAST_PRINT_STORE(STYPE,int8_t ," %i" ,);break;} case TSS_UINT8 : {CAST_PRINT_STORE(STYPE,uint8_t ," %u" ,);break;} case TSS_INT16 : {CAST_PRINT_STORE(STYPE,int16_t ," %i" ,);break;} case TSS_UINT16 : {CAST_PRINT_STORE(STYPE,uint16_t ," %u" ,);break;} case TSS_INT32 : {CAST_PRINT_STORE(STYPE,int32_t ," %i" ,);break;} case TSS_UINT32 : {CAST_PRINT_STORE(STYPE,uint32_t ," %u" ,);break;} case TSS_BOOLEAN : {CAST_PRINT_STORE(STYPE,int8_t ," %i" ,!!);break;} default:{dstArray=0;CAST_PRINT_STORE(STYPE,double," %lf",);break;} } }

// This snippet of code shared by both TOP_MEX.c and TOP_MEX.tlc -- BUT
// due to TALARIA-104, the SS_INT8 etc types had to be changed to TSS_INT8
// to avoid conflicts. To keep generated code clean (even for translation engine)
// the TOP_MEX.c file includes mappings to the TSS_... names

// Decode fixed point data types
typedef struct FixedInfo_tag
{
    int unsupportedType;
    int isSigned;
    int intDTypeId;
    int totalBits;
    int fractionalBits;
    double factor;
}
FixedInfo;

static FixedInfo DecodeFixedTypeName(const char* typeName)
{
    FixedInfo info;
    char signedChar;
    info.unsupportedType = 0;
    info.factor = 0;
    int numFound = sscanf(typeName, "%cfix%d_En%d", &signedChar, &info.totalBits,
                          &info.fractionalBits);
    if (numFound == 3)
    {
        info.factor = 1.0/(1 << info.fractionalBits);
        info.isSigned = (signedChar == 's') ? 1 : 0;
        int bytesInInt = (info.totalBits + 7) / 8;
        switch (bytesInInt)
        {
          case 1:
            info.intDTypeId = (info.isSigned) ? TSS_INT8 : TSS_UINT8;
            break;

          case 2:
            info.intDTypeId = (info.isSigned) ? TSS_INT16 : TSS_UINT16;
            break;

          case 3:
            info.intDTypeId = (info.isSigned) ? TSS_INT32 : TSS_UINT32;
            break;

          case 4:
            info.intDTypeId = (info.isSigned) ? TSS_INT32 : TSS_UINT32;
            break;

          default:
            info.unsupportedType = 1;
            break;
        }
    }
    else
    {
        info.unsupportedType = 1;
    }

    return info;
}

static TOP_RcvTrans_type* GetTopRecord(TranslateEngineIterator topNumIn)
{
    // Validate input parameters
    if ((topNumIn.model < 0) || (topNumIn.model >= numTranslationRoots))
    {
        return 0;                      // model out of range
    }

    TranslationRoot* theRoot = translationRoots[topNumIn.model];
    if ((topNumIn.index < 0) || (topNumIn.index >= *theRoot->top_count))
    {
        return 0;                      // index out of range for specified model
    }

    TOP_RcvTrans_type* theTable = *(TOP_RcvTrans_type**)theRoot->top_table;
    return &theTable[topNumIn.index];
}

// Get the name of a TOP, or NULL if not found
const char* GetTopName(TranslateEngineIterator topNumberIn)
{
    const char* theName = 0;
    TOP_RcvTrans_type* recordIn = GetTopRecord(topNumberIn);
    if (recordIn)
    {
        theName = recordIn->top_name;
    }

    return theName;
}

// Dump the translated value to provided buffer & updating sightings
// Returns number of chars filled in buffer or negative if buffer too small
int DumpTranslatedValue(TranslateEngineIterator rcvTransNumberIn, char
                        * bufferInOut,
                        int bufferSizeIn)
{
    int failDetected = 0;
    TOP_RcvTrans_type* recordIn = GetTopRecord(rcvTransNumberIn);
    if (!recordIn)
    {
        return 0;                      // Top not found
    }

    // Remember how many time this TOP has been encountered
    recordIn->top_sightings_count++;

    // Setup variables for the nested switch macros:
    int srcType = recordIn->top_in_dtype_id;
    int dstType = recordIn->top_out_dtype_id;
    void * srcArray = recordIn->top_value_src_ptr;
    void * dstArray = recordIn->top_value_dst_ptr;
    int numElem = recordIn->top_element_count * (1 + recordIn->top_is_complex);
    char* buf = bufferInOut;
    int bufSize = bufferSizeIn;
    FixedInfo srcFix = DecodeFixedTypeName(recordIn->top_in_dtype_name);
    if (srcFix.factor)
    {
        // When decoding a fixed point type, set the srcType to the
        // integer format of the data. Then... the srcfix.factor will
        // be used to convert to floating point
        srcType = srcFix.intDTypeId;
    }

    // Start with the TOP name and datatype
    int nameChars = snprintf(buf, bufSize, "%s %s",
        recordIn->top_name, recordIn->top_out_dtype_name);
    if (nameChars <= 0)
    {
        // Older implementations of snprintf only give failure
        // (instead of indicating how many bytes would be needed)
        nameChars = 0;
        failDetected = 1;
    }

    bufSize -= nameChars;
    buf += nameChars;
    switch (srcType)
    {
      case TSS_DOUBLE:
        SRC_DST_SWITCH(double);
        break;

      case TSS_SINGLE:
        SRC_DST_SWITCH(float);
        break;

      case TSS_INT8:
        SRC_DST_SWITCH(int8_T);
        break;

      case TSS_UINT8:
        SRC_DST_SWITCH(uint8_T);
        break;

      case TSS_INT16:
        SRC_DST_SWITCH(int16_T);
        break;

      case TSS_UINT16:
        SRC_DST_SWITCH(uint16_T);
        break;

      case TSS_INT32:
        SRC_DST_SWITCH(int32_T);
        break;

      case TSS_UINT32:
        SRC_DST_SWITCH(uint32_T);
        break;

      case TSS_BOOLEAN:
        SRC_DST_SWITCH(int8_T);
        break;
    }

    return failDetected ? (-1) : (bufferSizeIn - bufSize);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
