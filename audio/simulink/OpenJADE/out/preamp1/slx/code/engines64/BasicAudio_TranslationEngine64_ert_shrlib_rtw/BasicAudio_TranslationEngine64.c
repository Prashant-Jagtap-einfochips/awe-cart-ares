/*
 * File: BasicAudio_TranslationEngine64.c
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

#include "BasicAudio_TranslationEngine64.h"
#include "BasicAudio_TranslationEngine64_types.h"
#include "rtwtypes.h"
#include "BasicAudio_TranslationEngine64_private.h"
#include <math.h>
#include <string.h>
#include "rt_nonfinite.h"

/* Block signals (default storage) */
B_BasicAudio_TranslationEngine64_T BasicAudio_TranslationEngine64_B;

/* Real-time model */
static RT_MODEL_BasicAudio_TranslationEngine64_T
    BasicAudio_TranslationEngine64_M_;
RT_MODEL_BasicAudio_TranslationEngine64_T *const
    BasicAudio_TranslationEngine64_M = &BasicAudio_TranslationEngine64_M_;

/* Forward declaration for local functions */
static void BasicAudio_TranslationEngine64_unique_vector(const real_T a_data[],
    const int32_T *a_size, real_T b_data[], int32_T *b_size, int32_T ndx_data[],
    int32_T *ndx_size);
real_T rt_roundd_snf(real_T u)
{
    real_T y;
    if (fabs(u) < 4.503599627370496E+15)
    {
        if (u >= 0.5)
        {
            y = floor(u + 0.5);
        }
        else if (u > -0.5)
        {
            y = u * 0.0;
        }
        else
        {
            y = ceil(u - 0.5);
        }
    }
    else
    {
        y = u;
    }

    return y;
}

/* Output and update for function-call system: '<S10>/tfunc' */
void FuncTranslation_BasicAudio_2333_295_79(void)
{
    /* MATLAB Function: '<S9>/channel_ramper_translation' */
    /* MATLAB Function: '<S9>/channel_ramper_translation' incorporates:
     *  S-Function (TuneVarMEX): '<S9>/TuneVarChanRampMap'
     *
     * Block description for '<S9>/TuneVarChanRampMap':
     *  Specifies the ramper to which each channel is mapped. Replace the channel number with:
     *  '1' - 'Right'; '2' - 'Left'; '3' - 'Center'. Currently, the 'Center' group is assigned a linear gain of 1.
     */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S18>:1' */
    /* '<S18>:1:3' f(); */
    /*  % inputs */
    /*  parameters */
    /* MATLAB Function 'BasicAudio/Balance/balance/InnerLink/channel_ramper_translation': '<S17>:1' */
    /*  Verifies if each channel is assigned to one of the 4 possible rampers.  */
    /*  Acceptable rampers: */
    /*  1: left */
    /*  2: right */
    /*  3: center */
    /*  4: unused */
    /* '<S17>:1:11' argName = 'Balance'; */
    /*  Ensure each channel is present in the map */
    /* '<S17>:1:14' inputMapWidth = numel(TuneChanRampMap.Value); */
    /* '<S17>:1:15' if (inputMapWidth ~= channelCount) */
    /*  Verify that the channels are assigned to valid group numbers. */
    /* '<S17>:1:24' if (numel(unique(TuneChanRampMap.Value)) > (MaskNumRampers + 1)) */
    /* '<S17>:1:31' if (sum(TuneChanRampMap.Value == 0) > 0) */
    /* '<S17>:1:37' ChanRampMap = int32(TuneChanRampMap.Value); */
    BasicAudio_TranslationEngine64_B.ChanRampMap_i[0] = (int32_T)rt_roundd_snf
        (BasicAudio_TranslationEngine64_B.ChanRampMap.Value[0]);
    BasicAudio_TranslationEngine64_B.ChanRampMap_i[1] = (int32_T)rt_roundd_snf
        (BasicAudio_TranslationEngine64_B.ChanRampMap.Value[1]);
}

/* Output and update for function-call system: '<S21>/tfunc' */
void FuncTranslation_BasicAudio_2333_295_81_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S19>/Subsystem' */
    /* SignalConversion generated from: '<S20>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S22>:1' */
    /* '<S22>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_l0 =
        BasicAudio_TranslationEngine64_B.BoostDisable.Value;

    /* End of Outputs for SubSystem: '<S19>/Subsystem' */
}

/* Output and update for function-call system: '<S25>/tfunc' */
void FuncTranslation_BasicAudio_2333_295_83_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S23>/Subsystem' */
    /* SignalConversion generated from: '<S24>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S26>:1' */
    /* '<S26>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_iw =
        BasicAudio_TranslationEngine64_B.Offset_k.Value;

    /* End of Outputs for SubSystem: '<S23>/Subsystem' */
}

/* Output and update for function-call system: '<S29>/tfunc' */
void FuncTranslation_BasicAudio_2333_295_84_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S27>/Subsystem' */
    /* SignalConversion generated from: '<S28>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S30>:1' */
    /* '<S30>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_d =
        BasicAudio_TranslationEngine64_B.RampTime_d.Value;

    /* End of Outputs for SubSystem: '<S27>/Subsystem' */
}

/* Output and update for function-call system: '<S33>/tfunc' */
void FuncTranslation_BasicAudio_2333_295_143_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S31>/Subsystem' */
    /* SignalConversion generated from: '<S32>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S34>:1' */
    /* '<S34>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_ih =
        BasicAudio_TranslationEngine64_B.SleepingBeautyMutesBass.Value;

    /* End of Outputs for SubSystem: '<S31>/Subsystem' */
}

/* Output and update for function-call system: '<S37>/tfunc' */
void FuncTranslation_BasicAudio_2333_295_85_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S35>/Subsystem' */
    /* SignalConversion generated from: '<S36>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S38>:1' */
    /* '<S38>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_g =
        BasicAudio_TranslationEngine64_B.EnableSilentExtreme.Value;

    /* End of Outputs for SubSystem: '<S35>/Subsystem' */
}

/* Function for MATLAB Function: '<S16>/Balance_Table_Translation' */
static void BasicAudio_TranslationEngine64_unique_vector(const real_T a_data[],
    const int32_T *a_size, real_T b_data[], int32_T *b_size, int32_T ndx_data[],
    int32_T *ndx_size)
{
    real_T x;
    int32_T idx_data[30];
    int32_T iwork_data[30];
    int32_T b_j;
    int32_T i;
    int32_T i2;
    int32_T kEnd;
    int32_T n;
    int32_T pEnd;
    int32_T qEnd;
    boolean_T exitg1;
    if ((int8_T)*a_size - 1 >= 0)
    {
        memset(&idx_data[0], 0, (uint32_T)(int8_T)*a_size * sizeof(int32_T));
    }

    if (*a_size != 0)
    {
        i = *a_size - 1;
        for (i2 = 1; i2 <= i; i2 += 2)
        {
            if ((a_data[i2 - 1] <= a_data[i2]) || rtIsNaN(a_data[i2]))
            {
                idx_data[i2 - 1] = i2;
                idx_data[i2] = i2 + 1;
            }
            else
            {
                idx_data[i2 - 1] = i2 + 1;
                idx_data[i2] = i2;
            }
        }

        if (((uint32_T)*a_size & 1U) != 0U)
        {
            idx_data[*a_size - 1] = *a_size;
        }

        i = 2;
        while (i < (*a_size + 1) - 1)
        {
            i2 = i << 1;
            b_j = 1;
            pEnd = i + 1;
            while (pEnd < *a_size + 1)
            {
                int32_T e_k;
                int32_T q;
                n = b_j;
                q = pEnd - 1;
                qEnd = b_j + i2;
                if (qEnd > *a_size + 1)
                {
                    qEnd = *a_size + 1;
                }

                e_k = 0;
                kEnd = qEnd - b_j;
                while (e_k + 1 <= kEnd)
                {
                    int32_T b_tmp_tmp;
                    x = a_data[idx_data[q] - 1];
                    b_tmp_tmp = idx_data[n - 1];
                    if ((a_data[b_tmp_tmp - 1] <= x) || rtIsNaN(x))
                    {
                        iwork_data[e_k] = b_tmp_tmp;
                        n++;
                        if (n == pEnd)
                        {
                            while (q + 1 < qEnd)
                            {
                                e_k++;
                                iwork_data[e_k] = idx_data[q];
                                q++;
                            }
                        }
                    }
                    else
                    {
                        iwork_data[e_k] = idx_data[q];
                        q++;
                        if (q + 1 == qEnd)
                        {
                            while (n < pEnd)
                            {
                                e_k++;
                                iwork_data[e_k] = idx_data[n - 1];
                                n++;
                            }
                        }
                    }

                    e_k++;
                }

                for (pEnd = 0; pEnd < kEnd; pEnd++)
                {
                    idx_data[(b_j + pEnd) - 1] = iwork_data[pEnd];
                }

                b_j = qEnd;
                pEnd = qEnd + i;
            }

            i = i2;
        }
    }

    n = *a_size;
    for (i = 0; i < n; i++)
    {
        b_data[i] = a_data[idx_data[i] - 1];
    }

    b_j = 0;
    while ((b_j + 1 <= *a_size) && rtIsInf(b_data[b_j]) && (b_data[b_j] < 0.0))
    {
        b_j++;
    }

    n = b_j;
    b_j = *a_size;
    while ((b_j >= 1) && rtIsNaN(b_data[b_j - 1]))
    {
        b_j--;
    }

    i = *a_size - b_j;
    exitg1 = false;
    while ((!exitg1) && (b_j >= 1))
    {
        x = b_data[b_j - 1];
        if (rtIsInf(x) && (x > 0.0))
        {
            b_j--;
        }
        else
        {
            exitg1 = true;
        }
    }

    qEnd = (*a_size - b_j) - i;
    i2 = -1;
    if (n > 0)
    {
        i2 = 0;
    }

    while (n + 1 <= b_j)
    {
        x = b_data[n];
        kEnd = n;
        do
        {
            n++;
        }
        while (!((n + 1 > b_j) || (b_data[n] != x)));

        i2++;
        b_data[i2] = x;
        idx_data[i2] = idx_data[kEnd];
    }

    if (qEnd > 0)
    {
        i2++;
        b_data[i2] = b_data[b_j];
        idx_data[i2] = idx_data[b_j];
    }

    n = b_j + qEnd;
    for (b_j = 0; b_j < i; b_j++)
    {
        qEnd = n + b_j;
        pEnd = (i2 + b_j) + 1;
        b_data[pEnd] = b_data[qEnd];
        idx_data[pEnd] = idx_data[qEnd];
    }

    i2 = (i - 1 < 0 ? i2 : i2 + i) + 1;
    if (i2 < 1)
    {
        *b_size = 0;
    }
    else
    {
        *b_size = i2;
    }

    *ndx_size = i2;
    if ((uint8_T)i2 - 1 >= 0)
    {
        memcpy(&ndx_data[0], &idx_data[0], (uint8_T)i2 * sizeof(int32_T));
    }
}

/* Output and update for function-call system: '<S40>/tfunc' */
void FuncTranslation_BasicAudio_2333_295_90(void)
{
    real_T cleanIdx_data[30];
    real_T tmp_data[30];
    int32_T indx_data[30];
    int32_T i;
    int32_T loop_ub;
    real32_T tmp_0;
    uint32_T tmp;
    char_T k_data[78];
    char_T i_data[74];
    char_T j_data[13];
    int8_T g_tmp_data[30];
    int8_T remainingPairs_data[30];
    static const char_T l[74] =
        "%s must have at least 2 index pairs. Encounted value with only %f pairs\\n";
    static const char_T m[13] = "BalanceTable";
    static const char_T n[78] =
        "%s out of range table index %f encountered. Index must be between 0 and %d.\\n";
    int32_T indx_size;
    int32_T remainingPairs_size_idx_1;

    /* MATLAB Function: '<S16>/Balance_Table_Translation' */
    /* MATLAB Function: '<S16>/Balance_Table_Translation' incorporates:
     *  S-Function (TuneVarMEX): '<S16>/TuneVarBalanceTable'
     *
     * Block description for '<S16>/TuneVarBalanceTable':
     *  Define the balance curve using a table, with the first column being
     *  the list of indices and the second column being the corresponding
     *  balance gains in dB. Gain values for indices in between the elements
     *  in the table are linearly interpolated.
     */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S41>:1' */
    /* '<S41>:1:3' f(); */
    /*  Set up the size and datatype of our TOP outputs: */
    /* MATLAB Function 'BasicAudio/Balance/balance/InnerLink/TuneVarTable/Balance_Table_Translation': '<S39>:1' */
    /* '<S39>:1:5' maxTableSize = numel(balanceTable.Value) / 2; */
    /* '<S39>:1:6' idxType = 'uint8'; */
    /* '<S39>:1:7' idxStorageType = 'uint32'; */
    /* '<S39>:1:8' dbType = 'single'; */
    /* '<S39>:1:9' tableIdx = zeros(1,maxTableSize,idxStorageType); */
    /* '<S39>:1:10' tableDb = zeros(1,maxTableSize,dbType); */
    memset(&BasicAudio_TranslationEngine64_B.tableIdx[0], 0, 30U * sizeof
           (uint32_T));
    memset(&BasicAudio_TranslationEngine64_B.tableDb[0], 0, 30U * sizeof
           (real32_T));

    /*  separate the pairs into their index and value components */
    /* '<S39>:1:13' givenPairs = balanceTable.Size(1); */
    /* '<S39>:1:14' indexes = balanceTable.Value(1:givenPairs,1); */
    /* '<S39>:1:15' values = balanceTable.Value(1:givenPairs,2); */
    /*  then sort and ignore duplicates */
    /* '<S39>:1:18' [cleanIdx,map] = unique(indexes); */
    if (BasicAudio_TranslationEngine64_B.Table.Size[0] < 1.0)
    {
        loop_ub = -1;
    }
    else
    {
        loop_ub = (int32_T)BasicAudio_TranslationEngine64_B.Table.Size[0] - 1;
    }

    i = loop_ub + 1;
    if (loop_ub >= 0)
    {
        memcpy(&tmp_data[0], &BasicAudio_TranslationEngine64_B.Table.Value[0],
               (uint32_T)(loop_ub + 1) * sizeof(real_T));
    }

    BasicAudio_TranslationEngine64_unique_vector(tmp_data, &i, cleanIdx_data,
        &loop_ub, indx_data, &indx_size);

    /*  Validate input sizes */
    /* '<S39>:1:21' numPairs = numel(map); */
    /* '<S39>:1:22' argName='BalanceTable'; */
    /*  Validate: enough data? */
    /* '<S39>:1:25' if (numPairs < 2) */
    if (indx_size < 2)
    {
        /* '<S39>:1:26' TranslateError('%s must have at least 2 index pairs. Encounted value with only %f pairs\n', argName, numPairs); */
        /* TRANSLATEERROR (For use in Functions triggered by TranslationTrigger blocks.) */
        /* Print error message during tune data translation. */
        /*   Do not include a final newline as this behavior is already built in. */
        /*   Note that escape sequences such as '\n' will not work here. To emit */
        /*   special characters, pass them in as arguments to %c or %s format */
        /*   specifiers. */
        /*    Copyright 2018-2019 Bose Corporation */
        /* 'TranslateError:10' coder.extrinsic('gcb') */
        /* 'TranslateError:11' coder.extrinsic('warning') */
        /* 'TranslateError:12' coder.allowpcode('plain'); */
        /*  enable code generation from p-file */
        /* 'TranslateError:14' if (coder.target('MATLAB')) */
        /* 'TranslateError:19' if (coder.target('Sfun')) */
        /* 'TranslateError:22' else */
        /*  Generated C-language code */
        /* 'TranslateError:24' count = numel(varargin); */
        /* 'TranslateError:25' fixedArgs = varargin; */
        /* 'TranslateError:26' for i=1:count */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:28' fixedArgs{i} = [varargin{i} char(0)]; */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:28' fixedArgs{i} = [varargin{i} char(0)]; */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:31' coder.ceval('TranslateError',fixedArgs{:}); */
        memcpy(&i_data[0], &l[0], 74U * sizeof(char_T));
        for (i = 0; i < 13; i++)
        {
            j_data[i] = m[i];
        }

        TranslateError(&i_data[0], &j_data[0], (real_T)indx_size);
    }

    /*  Validate: Highest index still below 255? */
    /* '<S39>:1:30' if (cleanIdx(1) < 0 || cleanIdx(end) > intmax(idxType)) */
    if ((cleanIdx_data[0] < 0.0) || (cleanIdx_data[loop_ub - 1] > 255.0))
    {
        /* '<S39>:1:31' TranslateError('%s out of range table index %f encountered. Index must be between 0 and %d.\n',argName,cleanIdx(end),intmax(idxType)); */
        /* TRANSLATEERROR (For use in Functions triggered by TranslationTrigger blocks.) */
        /* Print error message during tune data translation. */
        /*   Do not include a final newline as this behavior is already built in. */
        /*   Note that escape sequences such as '\n' will not work here. To emit */
        /*   special characters, pass them in as arguments to %c or %s format */
        /*   specifiers. */
        /*    Copyright 2018-2019 Bose Corporation */
        /* 'TranslateError:10' coder.extrinsic('gcb') */
        /* 'TranslateError:11' coder.extrinsic('warning') */
        /* 'TranslateError:12' coder.allowpcode('plain'); */
        /*  enable code generation from p-file */
        /* 'TranslateError:14' if (coder.target('MATLAB')) */
        /* 'TranslateError:19' if (coder.target('Sfun')) */
        /* 'TranslateError:22' else */
        /*  Generated C-language code */
        /* 'TranslateError:24' count = numel(varargin); */
        /* 'TranslateError:25' fixedArgs = varargin; */
        /* 'TranslateError:26' for i=1:count */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:28' fixedArgs{i} = [varargin{i} char(0)]; */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:28' fixedArgs{i} = [varargin{i} char(0)]; */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:31' coder.ceval('TranslateError',fixedArgs{:}); */
        memcpy(&k_data[0], &n[0], 78U * sizeof(char_T));
        for (i = 0; i < 13; i++)
        {
            j_data[i] = m[i];
        }

        TranslateError(&k_data[0], &j_data[0], cleanIdx_data[loop_ub - 1],
                       MAX_uint8_T);
    }

    /*  fill in the table buffer with provided pairs */
    /* '<S39>:1:35' tableIdx(1:numPairs) = cleanIdx(1:numPairs); */
    if (indx_size < 1)
    {
        loop_ub = -1;
    }
    else
    {
        loop_ub = indx_size - 1;
    }

    for (i = 0; i <= loop_ub; i++)
    {
        BasicAudio_TranslationEngine64_B.tableIdx[i] = (uint32_T)rt_roundd_snf
            (cleanIdx_data[i]);
    }

    /* '<S39>:1:36' tableDb(1:numPairs) = values(map(1:numPairs)); */
    if (indx_size < 1)
    {
        loop_ub = 0;
    }
    else
    {
        loop_ub = indx_size;
    }

    for (i = 0; i < loop_ub; i++)
    {
        BasicAudio_TranslationEngine64_B.tableDb[i] = (real32_T)
            BasicAudio_TranslationEngine64_B.Table.Value[indx_data[i] + 29];
    }

    /*  fill the rest of the table buffer with the last entry */
    /* '<S39>:1:39' remainingPairs = numel(map)+1:maxTableSize; */
    if (indx_size + 1 > 30)
    {
        remainingPairs_size_idx_1 = 0;
    }
    else
    {
        remainingPairs_size_idx_1 = 30 - indx_size;
        loop_ub = 29 - indx_size;
        for (i = 0; i <= loop_ub; i++)
        {
            remainingPairs_data[i] = (int8_T)((indx_size + 1) + i);
        }
    }

    /* '<S39>:1:40' tableIdx(remainingPairs) = tableIdx(numPairs); */
    if (remainingPairs_size_idx_1 - 1 >= 0)
    {
        memcpy(&g_tmp_data[0], &remainingPairs_data[0], (uint32_T)
               remainingPairs_size_idx_1 * sizeof(int8_T));
    }

    tmp = BasicAudio_TranslationEngine64_B.tableIdx[indx_size - 1];
    for (i = 0; i < remainingPairs_size_idx_1; i++)
    {
        BasicAudio_TranslationEngine64_B.tableIdx[g_tmp_data[i] - 1] = tmp;
    }

    /* '<S39>:1:41' tableDb(remainingPairs) =  tableDb(numPairs); */
    tmp_0 = BasicAudio_TranslationEngine64_B.tableDb[indx_size - 1];
    for (i = 0; i < remainingPairs_size_idx_1; i++)
    {
        BasicAudio_TranslationEngine64_B.tableDb[g_tmp_data[i] - 1] = tmp_0;
    }

    /* End of MATLAB Function: '<S16>/Balance_Table_Translation' */
}

/*
 * Output and update for function-call system:
 *    '<S43>/tone filter tune translate3'
 *    '<S43>/tone filter tune translate4'
 *    '<S80>/tone filter tune translate3'
 *    '<S80>/tone filter tune translate4'
 */
void BasicAudio_TranslationEngine64_tonefiltertunetranslate3(const
    bus_double_1_1 *rtu_maxIn,
    B_tonefiltertunetranslate3_BasicAudio_TranslationEngine64_T *localB)
{
    /*  % inputs % parameters */
    /* MATLAB Function 'BasicAudio/Bass/Tone/InnerLink/tone filter tune translate3': '<S53>:1' */
    /*  Just make sure the maximum is a positive dB value */
    /* '<S53>:1:6' maxOut = single(abs(maxIn.Value)); */
    localB->maxOut = (real32_T)fabs(rtu_maxIn->Value);
}

/* Output and update for function-call system: '<S44>/tfunc' */
void FuncTranslation_BasicAudio_2337_236_174(void)
{
    /* MATLAB Function: '<S43>/tone filter tune translate3' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S55>:1' */
    /* '<S55>:1:3' f(); */
    BasicAudio_TranslationEngine64_tonefiltertunetranslate3
        (&BasicAudio_TranslationEngine64_B.BoostMaximum_dB_i,
         &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate3);
}

/*
 * Output and update for function-call system:
 *    '<S43>/tone filter tune translate1'
 *    '<S43>/tone filter tune translate2'
 */
void BasicAudio_TranslationEngine64_tonefiltertunetranslate1(const
    bus_double_1_1 *rtu_FilterFrequencyHz, real32_T rtu_Fs,
    B_tonefiltertunetranslate1_BasicAudio_TranslationEngine64_T *localB)
{
    real32_T B0;
    real32_T omega;

    /*  % inputs */
    /*  parameters */
    /* MATLAB Function 'BasicAudio/Bass/Tone/InnerLink/tone filter tune translate1': '<S51>:1' */
    /* '<S51>:1:6' omega = 2 * pi * FilterFrequencyHz.Value / Fs; */
    omega = (real32_T)(6.2831853071795862 * rtu_FilterFrequencyHz->Value) /
        rtu_Fs;

    /* '<S51>:1:8' A = (1 - sin(omega)) / cos(omega); */
    omega = (1.0F - sinf(omega)) / cosf(omega);

    /* '<S51>:1:10' if (MaskFilterType == ToneMask.TypeLowpass) */
    /*  lowpass coeffs (e.g. bass) */
    /* '<S51>:1:12' B0 = (1 - A) / 2; */
    B0 = (1.0F - omega) / 2.0F;

    /* '<S51>:1:13' B1 = B0; */
    /* '<S51>:1:20' FilterCoeffs = single([B0 B1 A]); */
    localB->FilterCoeffs[0] = B0;
    localB->FilterCoeffs[1] = B0;
    localB->FilterCoeffs[2] = omega;
}

/* Output and update for function-call system: '<S45>/tfunc' */
void FuncTranslation_BasicAudio_2337_236_149(void)
{
    /* MATLAB Function: '<S43>/tone filter tune translate1' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S56>:1' */
    /* '<S56>:1:3' f(); */
    BasicAudio_TranslationEngine64_tonefiltertunetranslate1
        (&BasicAudio_TranslationEngine64_B.BoostFrequencyHz_o,
         BasicAudio_TranslationEngine64_ConstB.CapturedTuneConst_j,
         &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate1);
}

/* Output and update for function-call system: '<S46>/tfunc' */
void FuncTranslation_BasicAudio_2337_236_153(void)
{
    /* MATLAB Function: '<S43>/tone filter tune translate2' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S57>:1' */
    /* '<S57>:1:3' f(); */
    BasicAudio_TranslationEngine64_tonefiltertunetranslate1
        (&BasicAudio_TranslationEngine64_B.CutFrequencyHz_o,
         BasicAudio_TranslationEngine64_ConstB.CapturedTuneConst_j,
         &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate2_e);
}

/* Output and update for function-call system: '<S47>/tfunc' */
void FuncTranslation_BasicAudio_2337_236_176(void)
{
    /* MATLAB Function: '<S43>/tone filter tune translate4' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S58>:1' */
    /* '<S58>:1:3' f(); */
    BasicAudio_TranslationEngine64_tonefiltertunetranslate3
        (&BasicAudio_TranslationEngine64_B.CutMaximum_dB_a,
         &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate4_i);
}

/* Output and update for function-call system: '<S61>/tfunc' */
void FuncTranslation_BasicAudio_2337_236_210_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S59>/Subsystem' */
    /* SignalConversion generated from: '<S60>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S62>:1' */
    /* '<S62>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_p =
        BasicAudio_TranslationEngine64_B.Offset_l.Value;

    /* End of Outputs for SubSystem: '<S59>/Subsystem' */
}

/* Output and update for function-call system: '<S65>/tfunc' */
void FuncTranslation_BasicAudio_2337_236_229_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S63>/Subsystem' */
    /* SignalConversion generated from: '<S64>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S66>:1' */
    /* '<S66>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_lm =
        BasicAudio_TranslationEngine64_B.ramptime_l.Value;

    /* End of Outputs for SubSystem: '<S63>/Subsystem' */
}

/* Output and update for function-call system: '<S69>/tfunc' */
void FuncTranslation_BasicAudio_2337_236_209_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S67>/Subsystem' */
    /* SignalConversion generated from: '<S68>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S70>:1' */
    /* '<S70>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_h =
        BasicAudio_TranslationEngine64_B.Scale_o.Value;

    /* End of Outputs for SubSystem: '<S67>/Subsystem' */
}

/* Output and update for function-call system: '<S77>/tfunc' */
void FuncTranslation_BasicAudio_2341_254_276_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S75>/Subsystem' */
    /* SignalConversion generated from: '<S76>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S78>:1' */
    /* '<S78>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_b =
        BasicAudio_TranslationEngine64_B.RampTime_c.Value;

    /* End of Outputs for SubSystem: '<S75>/Subsystem' */
}

/* Output and update for function-call system: '<S81>/tfunc' */
void FuncTranslation_BasicAudio_2345_236_174(void)
{
    /* MATLAB Function: '<S80>/tone filter tune translate3' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S92>:1' */
    /* '<S92>:1:3' f(); */
    BasicAudio_TranslationEngine64_tonefiltertunetranslate3
        (&BasicAudio_TranslationEngine64_B.BoostMaximum_dB,
         &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate3_fg);
}

/*
 * Output and update for function-call system:
 *    '<S80>/tone filter tune translate1'
 *    '<S80>/tone filter tune translate2'
 */
void BasicAudio_TranslationEngine64_tonefiltertunetranslate1_d(const
    bus_double_1_1 *rtu_FilterFrequencyHz, real32_T rtu_Fs,
    B_tonefiltertunetranslate1_BasicAudio_TranslationEngine64_i_T *localB)
{
    real32_T B0;
    real32_T omega;

    /*  % inputs */
    /*  parameters */
    /* MATLAB Function 'BasicAudio/Treble/Tone/InnerLink/tone filter tune translate1': '<S88>:1' */
    /* '<S88>:1:6' omega = 2 * pi * FilterFrequencyHz.Value / Fs; */
    omega = (real32_T)(6.2831853071795862 * rtu_FilterFrequencyHz->Value) /
        rtu_Fs;

    /* '<S88>:1:8' A = (1 - sin(omega)) / cos(omega); */
    omega = (1.0F - sinf(omega)) / cosf(omega);

    /* '<S88>:1:10' if (MaskFilterType == ToneMask.TypeLowpass) */
    /* '<S88>:1:14' else */
    /*  highpass coeffs (e.g. treble) */
    /* '<S88>:1:16' B0 = (1 + A) / 2; */
    B0 = (omega + 1.0F) / 2.0F;

    /* '<S88>:1:17' B1 = 0 - B0; */
    /* '<S88>:1:20' FilterCoeffs = single([B0 B1 A]); */
    localB->FilterCoeffs[0] = B0;
    localB->FilterCoeffs[1] = 0.0F - B0;
    localB->FilterCoeffs[2] = omega;
}

/* Output and update for function-call system: '<S82>/tfunc' */
void FuncTranslation_BasicAudio_2345_236_149(void)
{
    /* MATLAB Function: '<S80>/tone filter tune translate1' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S93>:1' */
    /* '<S93>:1:3' f(); */
    BasicAudio_TranslationEngine64_tonefiltertunetranslate1_d
        (&BasicAudio_TranslationEngine64_B.BoostFrequencyHz,
         BasicAudio_TranslationEngine64_ConstB.CapturedTuneConst,
         &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate1_d);
}

/* Output and update for function-call system: '<S83>/tfunc' */
void FuncTranslation_BasicAudio_2345_236_153(void)
{
    /* MATLAB Function: '<S80>/tone filter tune translate2' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S94>:1' */
    /* '<S94>:1:3' f(); */
    BasicAudio_TranslationEngine64_tonefiltertunetranslate1_d
        (&BasicAudio_TranslationEngine64_B.CutFrequencyHz,
         BasicAudio_TranslationEngine64_ConstB.CapturedTuneConst,
         &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate2_fq);
}

/* Output and update for function-call system: '<S84>/tfunc' */
void FuncTranslation_BasicAudio_2345_236_176(void)
{
    /* MATLAB Function: '<S80>/tone filter tune translate4' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S95>:1' */
    /* '<S95>:1:3' f(); */
    BasicAudio_TranslationEngine64_tonefiltertunetranslate3
        (&BasicAudio_TranslationEngine64_B.CutMaximum_dB,
         &BasicAudio_TranslationEngine64_B.sf_tonefiltertunetranslate4_lk);
}

/* Output and update for function-call system: '<S98>/tfunc' */
void FuncTranslation_BasicAudio_2345_236_210_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S96>/Subsystem' */
    /* SignalConversion generated from: '<S97>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S99>:1' */
    /* '<S99>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_e =
        BasicAudio_TranslationEngine64_B.Offset.Value;

    /* End of Outputs for SubSystem: '<S96>/Subsystem' */
}

/* Output and update for function-call system: '<S102>/tfunc' */
void FuncTranslation_BasicAudio_2345_236_229_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S100>/Subsystem' */
    /* SignalConversion generated from: '<S101>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S103>:1' */
    /* '<S103>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_l =
        BasicAudio_TranslationEngine64_B.ramptime.Value;

    /* End of Outputs for SubSystem: '<S100>/Subsystem' */
}

/* Output and update for function-call system: '<S106>/tfunc' */
void FuncTranslation_BasicAudio_2345_236_209_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S104>/Subsystem' */
    /* SignalConversion generated from: '<S105>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S107>:1' */
    /* '<S107>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_j =
        BasicAudio_TranslationEngine64_B.Scale.Value;

    /* End of Outputs for SubSystem: '<S104>/Subsystem' */
}

/* Output and update for function-call system: '<S112>/tfunc' */
void FuncTranslation_BasicAudio_2349_263_280(void)
{
    real_T b_b_data[8];
    real_T x;
    int32_T idx_data[8];
    int32_T iwork_data[8];
    int32_T c_i;
    int32_T f_j;
    int32_T i;
    int32_T i2;
    int32_T kEnd;
    int32_T pEnd;
    int32_T qEnd;
    char_T h_data[77];
    char_T f_data[74];
    char_T g_data[6];
    static const char_T r[74] =
        "%s must have at least 2 index pairs. Encounted value with only %f pairs\\n";
    static const char_T t[77] =
        "%s out of range table index %f encountered. Index must be between 0 and 1.\\n";
    static const char_T s[6] = "Table";
    boolean_T exitg1;

    /* MATLAB Function: '<S110>/table translation' */
    /* MATLAB Function: '<S110>/table translation' incorporates:
     *  S-Function (TuneVarMEX): '<S110>/TuneVar_Table'
     *
     * Block description for '<S110>/TuneVar_Table':
     *  <HTML>Gain table maps index to gain.
     *  The index values are <FONT color="red">normalized</Font> floats between 0 and 1 while the gains are in dB (i.e. 0 implies no attenuation).
     *  </html>
     */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S115>:1' */
    /* '<S115>:1:3' f(); */
    /*  */
    /*  Demux table matrix into one dimesional value and index vectors */
    /*  */
    /*  Set up the size and datatype of our TOP outputs: */
    /* MATLAB Function 'BasicAudio/Volume/volume/InnerLink/Index2DbTable/table translation': '<S114>:1' */
    /* '<S114>:1:8' maxTableSize = numel(Table.Value) / 2; */
    /* '<S114>:1:9' idxType = 'uint8'; */
    /* '<S114>:1:10' idxStorageType = 'uint32'; */
    /* '<S114>:1:11' dbType = 'single'; */
    /* '<S114>:1:12' Table_Idx = zeros(1,maxTableSize,idxStorageType); */
    /* '<S114>:1:13' Table_dB = zeros(1,maxTableSize,dbType); */
    for (i = 0; i < 8; i++)
    {
        BasicAudio_TranslationEngine64_B.Table_Idx[i] = 0U;
        BasicAudio_TranslationEngine64_B.Table_dB[i] = 0.0F;
    }

    /*  separate the pairs into their index and value components */
    /* '<S114>:1:16' givenPairs = Table.Size(1); */
    /* '<S114>:1:17' indexes = Table.Value(1:givenPairs,1); */
    if (BasicAudio_TranslationEngine64_B.Table_i.Size[0] < 1.0)
    {
        i = 0;
    }
    else
    {
        i = (int32_T)BasicAudio_TranslationEngine64_B.Table_i.Size[0];
    }

    /* '<S114>:1:18' values = Table.Value(1:givenPairs,2); */
    /*  then sort and ignore duplicates */
    /* '<S114>:1:21' [cleanIdx,map] = unique(indexes); */
    if (i - 1 >= 0)
    {
        memset(&idx_data[0], 0, (uint32_T)i * sizeof(int32_T));
    }

    if (i != 0)
    {
        for (c_i = 1; c_i <= i - 1; c_i += 2)
        {
            if ((BasicAudio_TranslationEngine64_B.Table_i.Value[c_i - 1] <=
                    BasicAudio_TranslationEngine64_B.Table_i.Value[c_i]) ||
                    rtIsNaN(BasicAudio_TranslationEngine64_B.Table_i.Value[c_i]))
            {
                idx_data[c_i - 1] = c_i;
                idx_data[c_i] = c_i + 1;
            }
            else
            {
                idx_data[c_i - 1] = c_i + 1;
                idx_data[c_i] = c_i;
            }
        }

        if (((uint32_T)i & 1U) != 0U)
        {
            idx_data[i - 1] = i;
        }

        c_i = 2;
        while (c_i < i)
        {
            i2 = c_i << 1;
            f_j = 1;
            pEnd = c_i + 1;
            while (pEnd < i + 1)
            {
                int32_T f_k;
                int32_T p;
                int32_T q;
                p = f_j;
                q = pEnd - 1;
                qEnd = f_j + i2;
                if (qEnd > i + 1)
                {
                    qEnd = i + 1;
                }

                f_k = 0;
                kEnd = qEnd - f_j;
                while (f_k + 1 <= kEnd)
                {
                    int32_T b_b_tmp_tmp;
                    x = BasicAudio_TranslationEngine64_B.Table_i.Value[idx_data[q]
                        - 1];
                    b_b_tmp_tmp = idx_data[p - 1];
                    if ((BasicAudio_TranslationEngine64_B.Table_i.Value[b_b_tmp_tmp
                         - 1] <= x) || rtIsNaN(x))
                    {
                        iwork_data[f_k] = b_b_tmp_tmp;
                        p++;
                        if (p == pEnd)
                        {
                            while (q + 1 < qEnd)
                            {
                                f_k++;
                                iwork_data[f_k] = idx_data[q];
                                q++;
                            }
                        }
                    }
                    else
                    {
                        iwork_data[f_k] = idx_data[q];
                        q++;
                        if (q + 1 == qEnd)
                        {
                            while (p < pEnd)
                            {
                                f_k++;
                                iwork_data[f_k] = idx_data[p - 1];
                                p++;
                            }
                        }
                    }

                    f_k++;
                }

                for (pEnd = 0; pEnd < kEnd; pEnd++)
                {
                    idx_data[(f_j + pEnd) - 1] = iwork_data[pEnd];
                }

                f_j = qEnd;
                pEnd = qEnd + c_i;
            }

            c_i = i2;
        }
    }

    c_i = (uint8_T)i;
    for (i2 = 0; i2 < c_i; i2++)
    {
        b_b_data[i2] =
            BasicAudio_TranslationEngine64_B.Table_i.Value[idx_data[i2] - 1];
    }

    f_j = 0;
    while ((f_j + 1 <= i) && rtIsInf(b_b_data[f_j]) && (b_b_data[f_j] < 0.0))
    {
        f_j++;
    }

    c_i = f_j;
    f_j = i;
    while ((f_j >= 1) && rtIsNaN(b_b_data[f_j - 1]))
    {
        f_j--;
    }

    i2 = i - f_j;
    exitg1 = false;
    while ((!exitg1) && (f_j >= 1))
    {
        x = b_b_data[f_j - 1];
        if (rtIsInf(x) && (x > 0.0))
        {
            f_j--;
        }
        else
        {
            exitg1 = true;
        }
    }

    qEnd = (i - f_j) - i2;
    i = -1;
    if (c_i > 0)
    {
        i = 0;
    }

    while (c_i + 1 <= f_j)
    {
        x = b_b_data[c_i];
        kEnd = c_i;
        do
        {
            c_i++;
        }
        while (!((c_i + 1 > f_j) || (b_b_data[c_i] != x)));

        i++;
        b_b_data[i] = x;
        idx_data[i] = idx_data[kEnd];
    }

    if (qEnd > 0)
    {
        i++;
        b_b_data[i] = b_b_data[f_j];
        idx_data[i] = idx_data[f_j];
    }

    c_i = (f_j + qEnd) - 1;
    for (f_j = 0; f_j < i2; f_j++)
    {
        qEnd = (f_j + c_i) + 1;
        pEnd = (i + f_j) + 1;
        b_b_data[pEnd] = b_b_data[qEnd];
        idx_data[pEnd] = idx_data[qEnd];
    }

    i = (i2 - 1 < 0 ? i : i + i2) + 1;
    if (i < 1)
    {
        c_i = -1;
    }
    else
    {
        c_i = i - 1;
    }

    pEnd = i;
    i = (uint8_T)i;
    if (i - 1 >= 0)
    {
        memcpy(&iwork_data[0], &idx_data[0], (uint32_T)i * sizeof(int32_T));
    }

    /*  Validate input sizes */
    /* '<S114>:1:24' numPairs = numel(map); */
    /* '<S114>:1:25' argName='Table'; */
    /*  Validate: enough data? */
    /* '<S114>:1:28' if (numPairs < 2) */
    if (pEnd < 2)
    {
        /* '<S114>:1:29' TranslateError('%s must have at least 2 index pairs. Encounted value with only %f pairs\n', argName, numPairs); */
        /* TRANSLATEERROR (For use in Functions triggered by TranslationTrigger blocks.) */
        /* Print error message during tune data translation. */
        /*   Do not include a final newline as this behavior is already built in. */
        /*   Note that escape sequences such as '\n' will not work here. To emit */
        /*   special characters, pass them in as arguments to %c or %s format */
        /*   specifiers. */
        /*    Copyright 2018-2019 Bose Corporation */
        /* 'TranslateError:10' coder.extrinsic('gcb') */
        /* 'TranslateError:11' coder.extrinsic('warning') */
        /* 'TranslateError:12' coder.allowpcode('plain'); */
        /*  enable code generation from p-file */
        /* 'TranslateError:14' if (coder.target('MATLAB')) */
        /* 'TranslateError:19' if (coder.target('Sfun')) */
        /* 'TranslateError:22' else */
        /*  Generated C-language code */
        /* 'TranslateError:24' count = numel(varargin); */
        /* 'TranslateError:25' fixedArgs = varargin; */
        /* 'TranslateError:26' for i=1:count */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:28' fixedArgs{i} = [varargin{i} char(0)]; */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:28' fixedArgs{i} = [varargin{i} char(0)]; */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:31' coder.ceval('TranslateError',fixedArgs{:}); */
        memcpy(&f_data[0], &r[0], 74U * sizeof(char_T));
        for (qEnd = 0; qEnd < 6; qEnd++)
        {
            g_data[qEnd] = s[qEnd];
        }

        TranslateError(&f_data[0], &g_data[0], (real_T)pEnd);
    }

    /*  Validate: Highest index still below 1.0? */
    /* '<S114>:1:33' if (cleanIdx(end) > 1) */
    if (b_b_data[c_i] > 1.0)
    {
        /* '<S114>:1:34' TranslateError('%s out of range table index %f encountered. Index must be between 0 and 1.\n',argName,cleanIdx(end)); */
        /* TRANSLATEERROR (For use in Functions triggered by TranslationTrigger blocks.) */
        /* Print error message during tune data translation. */
        /*   Do not include a final newline as this behavior is already built in. */
        /*   Note that escape sequences such as '\n' will not work here. To emit */
        /*   special characters, pass them in as arguments to %c or %s format */
        /*   specifiers. */
        /*    Copyright 2018-2019 Bose Corporation */
        /* 'TranslateError:10' coder.extrinsic('gcb') */
        /* 'TranslateError:11' coder.extrinsic('warning') */
        /* 'TranslateError:12' coder.allowpcode('plain'); */
        /*  enable code generation from p-file */
        /* 'TranslateError:14' if (coder.target('MATLAB')) */
        /* 'TranslateError:19' if (coder.target('Sfun')) */
        /* 'TranslateError:22' else */
        /*  Generated C-language code */
        /* 'TranslateError:24' count = numel(varargin); */
        /* 'TranslateError:25' fixedArgs = varargin; */
        /* 'TranslateError:26' for i=1:count */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:28' fixedArgs{i} = [varargin{i} char(0)]; */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:28' fixedArgs{i} = [varargin{i} char(0)]; */
        /* 'TranslateError:27' if (ischar(varargin{i})) */
        /* 'TranslateError:31' coder.ceval('TranslateError',fixedArgs{:}); */
        memcpy(&h_data[0], &t[0], 77U * sizeof(char_T));
        for (qEnd = 0; qEnd < 6; qEnd++)
        {
            g_data[qEnd] = s[qEnd];
        }

        TranslateError(&h_data[0], &g_data[0], b_b_data[c_i]);
    }

    /*  fill in the table buffer with provided pairs */
    /* '<S114>:1:38' for i=1:numPairs */
    for (i2 = 0; i2 < pEnd; i2++)
    {
        /* '<S114>:1:39' Table_Idx(i) = intmax(idxType) * cleanIdx(i); */
        BasicAudio_TranslationEngine64_B.Table_Idx[i2] = (uint8_T)rt_roundd_snf
            (255.0 * b_b_data[i2]);

        /* '<S114>:1:40' Table_dB(i) = values(map(i)); */
        BasicAudio_TranslationEngine64_B.Table_dB[i2] = (real32_T)
            BasicAudio_TranslationEngine64_B.Table_i.Value[iwork_data[i2] + 7];
    }

    /*  fill the rest of the table buffer with the last entry */
    /* '<S114>:1:43' for j=numel(map)+1:maxTableSize */
    f_j = 7 - pEnd;
    for (c_i = 0; c_i <= f_j; c_i++)
    {
        i2 = pEnd + c_i;

        /* '<S114>:1:44' Table_Idx(j) = Table_Idx(numPairs); */
        BasicAudio_TranslationEngine64_B.Table_Idx[i2] =
            BasicAudio_TranslationEngine64_B.Table_Idx[pEnd - 1];

        /* '<S114>:1:45' Table_dB(j) =  Table_dB(numPairs); */
        BasicAudio_TranslationEngine64_B.Table_dB[i2] =
            BasicAudio_TranslationEngine64_B.Table_dB[pEnd - 1];
    }

    /* End of MATLAB Function: '<S110>/table translation' */
}

/* Output and update for function-call system: '<S118>/tfunc' */
void FuncTranslation_BasicAudio_2349_263_281_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S116>/Subsystem' */
    /* SignalConversion generated from: '<S117>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S119>:1' */
    /* '<S119>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value_i =
        BasicAudio_TranslationEngine64_B.maxgain.Value;

    /* End of Outputs for SubSystem: '<S116>/Subsystem' */
}

/* Output and update for function-call system: '<S122>/tfunc' */
void FuncTranslation_BasicAudio_2349_263_256_67_18(void)
{
    /* Outputs for Function Call SubSystem: '<S120>/Subsystem' */
    /* SignalConversion generated from: '<S121>/Bus Selector' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'TranslateTrigger/tfunc': '<S123>:1' */
    /* '<S123>:1:3' f(); */
    BasicAudio_TranslationEngine64_B.Value =
        BasicAudio_TranslationEngine64_B.RampTime.Value;

    /* End of Outputs for SubSystem: '<S120>/Subsystem' */
}

/* Model step function */
void BasicAudio_TranslationEngine64_step(void)
{
    /* End of Outputs for S-Function (TranslateTrigger): '<S10>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S21>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S25>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S29>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S33>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S37>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S40>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S45>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S46>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S44>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S47>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S61>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S65>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S69>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S77>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S82>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S83>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S81>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S84>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S98>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S102>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S106>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S112>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S118>/S-Function' */
    /* End of Outputs for S-Function (TranslateTrigger): '<S122>/S-Function' */
}

/* Model initialize function */
void BasicAudio_TranslationEngine64_initialize(void)
{
    /* Registration code */

    /* initialize non-finites */
    rt_InitInfAndNaN(sizeof(real_T));

    /* initialize error status */
    rtmSetErrorStatus(BasicAudio_TranslationEngine64_M, (NULL));

    /* block I/O */
    (void) memset(((void *) &BasicAudio_TranslationEngine64_B), 0,
                  sizeof(B_BasicAudio_TranslationEngine64_T));

    /* user code (Start function Body) */

    // Load default values for all TuneVars
    LoadAllTuneVarDefaults();

    /* SystemInitialize for Atomic SubSystem: '<S1>/Balance' */

    /* user code (Initialize function Body) */

    // Register translation engine among the model roots
    BasicAudio_TranslationEngine64_InitializeTranslationRoot();

    /* End of SystemInitialize for SubSystem: '<S1>/Balance' */
}

/* Model terminate function */
void BasicAudio_TranslationEngine64_terminate(void)
{
    /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
