/*
 * File: PostProcess.c
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

#include "PostProcess.h"
#include "rt_sys_Model_Target_100.h"
#include "qhmath.h"
#include <math.h>
#include "rtwtypes.h"
#include "pooliir_hexagon.h"
#include "qhblas.h"
#include "qhcomplex.h"
#include "qhdsp.h"
#include "qhdsp_common.h"
#include "qhdsp_iir.h"
#include "rt_sys_Model_Target_107.h"
#include "rt_sys_Model_Target_105.h"
#include "rt_sys_Model_Target_104.h"
#include <string.h>
#include "math.h"
#include "Model_Target_private.h"
#include "PoolDelayStorage.h"
#include "tsp_capture_settings.h"

/* Includes for objects with custom storage classes */
#include "Model_Target.h"

/* Declare variables for internal data of system '<S6>/PostProcess' */
B_PostProcess_Model_Target_T Model_Target_PostProcess_B;
DW_PostProcess_Model_Target_T Model_Target_PostProcess_DW;

/* Output and update for function-call system: '<S604>/REQ' */
void HandleReqPostProcessMuteCfgSet(void)
{
    real32_T quantum_ms;
    real32_T x;

    /* MATLAB Function: '<S602>/calculate_ramp_rate' */
    /* MATLAB Function: '<S602>/calculate_ramp_rate' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'RTC/REQ': '<S611>:1' */
    /* '<S611>:1:3' go(); */
    Model_Target_calculate_ramp_rate
        (Model_TargetPostProcessMuteCfgSetRtcOut.REQ_ramp_db_per_second,
         Model_TargetPostProcessMuteCfgSetRtcOut.REQ_ramp_milliseconds,
         Model_Target_PostProcess0.MuteCfgRampTime,
         &Model_Target_PostProcess_B.sf_calculate_ramp_rate_g);

    /* MATLAB Function: '<S603>/get_ramp_coeffs' */
    /* MATLAB Function: '<S603>/get_ramp_coeffs' incorporates:
     *  S-Function (AudioStats_MEX): '<S603>/Audio Stats'
     */
    x = Model_Target_PostProcess_B.sf_calculate_ramp_rate_g.ramptime;

    /* MATLAB Function 'MuteRamper/get_ramp_coeffs': '<S616>:1' */
    /* '<S616>:1:5' targetLinearGain = single(1.0); */
    Model_TargetControlDsms.PostProcessMuteTargetLinearGain = 1.0F;

    /* '<S616>:1:7' if muteSwitch */
    if (Model_TargetPostProcessMuteCfgSetRtcOut.REQ_mute_switch != 0U)
    {
        /* '<S616>:1:8' targetLinearGain = single(max(0, silentGainLinear)); */
        Model_TargetControlDsms.PostProcessMuteTargetLinearGain = 4.7315126E-7F;
    }

    /*  % Nudge the gain values a bit if they are sitting at zero. */
    /*  % If current gain is zero, replace with a value very close to zero. What we */
    /*  % really want is a value which is on the edge of perception - not too small */
    /*  % or large. If the value is too small, we'll waste some of our ramp time */
    /*  % in perceived silence. If the value is too large, the audio will lurch */
    /*  % into perceptability at the start of the ramp. */
    /*  % So, 'max(currentGainValue, silentGainLinear)' & 'max(TargetGain, silentGainLinear)' are */
    /*  % used in equations */
    /*  In order to have the desired ramp time, the slope must vary dependent */
    /*  on the gain difference between target and current value. */
    /*  Equivalent ramp rate is therefore calculated as follows: */
    /* '<S616>:1:24' if (rampingRate_dBPerSec) */
    if (Model_Target_PostProcess_B.sf_calculate_ramp_rate_g.rampRate != 0.0F)
    {
        /* '<S616>:1:25' currentGain_db = 20*log10(max(currentGainValue, silentGainLinear)); */
        /* '<S616>:1:26' targetGain_db = 20*log10( targetLinearGain); */
        /* '<S616>:1:27' diff = abs(currentGain_db - targetGain_db); */
        /* '<S616>:1:28' rampingTime_ms = diff / single(rampingRate_dBPerSec) * 1000; */
        x = qhmath_div_f(qhmath_abs_f(20.0F * qhmath_log10_f(fmaxf
                           (Model_Target_PostProcess_B.DataStoreRead,
                            4.7315126E-7F)) - 20.0F * qhmath_log10_f
                          (Model_TargetControlDsms.PostProcessMuteTargetLinearGain)),
                         Model_Target_PostProcess_B.sf_calculate_ramp_rate_g.rampRate)
            * 1000.0F;
    }

    /* '<S616>:1:31' quantum_ms = 1000 * frameSize / Fs_Hz; */
    quantum_ms = qhmath_div_f(32000.0F, 48000.0);

    /* '<S616>:1:32' numFrames = rampingTime_ms / quantum_ms; */
    /*  Do a ceil rather than a round to insure that we create enough */
    /*  frames. i.e. a fraction of a frame is promoted to an addition frame */
    /* '<S616>:1:35' nFrames = max (1, ceil(numFrames)); */
    Model_TargetControlDsms.PostProcessMuteNframes = fmaxf(1.0F, qhmath_ceil_f
        (qhmath_div_f(x, quantum_ms)));

    /* '<S616>:1:36' quantized_rampTime_ms = nFrames * quantum_ms; */
    /*  In order to have the desired ramp time, the slope must vary dependent */
    /*  on the gain differencebetween target and current value. */
    /*  Ramper's coefficient is therefore calculated as follows: */
    /*  */
    /*    TargetGain = CurrentGain *( RampCoef ^ (Fs*RampTime_ms/1000) ) */
    /*  */
    /*  therefore... */
    /*  */
    /*                      log(TargetGain/CurrentGain) */
    /*    log(RampCoef) = ------------------------------ */
    /*                         Fs*(RampTime_ms/1000) */
    /*  */
    /* '<S616>:1:50' ramping_coeff= exp(log( targetLinearGain /max(currentGainValue, silentGainLinear) ) / (Fs_Hz * quantized_rampTime_ms / 1000)); */
    Model_Target_PostProcess_B.ramping_coeff = qhmath_div_f(qhmath_log_f
        (qhmath_div_f(Model_TargetControlDsms.PostProcessMuteTargetLinearGain,
                      fmaxf(Model_Target_PostProcess_B.DataStoreRead,
                            4.7315126E-7F))), qhmath_div_f
        (Model_TargetControlDsms.PostProcessMuteNframes * quantum_ms * 48000.0,
         1000.0F));
    Model_Target_PostProcess_B.ramping_coeff = qhmath_exp_f
        (Model_Target_PostProcess_B.ramping_coeff);

    /* End of MATLAB Function: '<S603>/get_ramp_coeffs' */
}

/* Output and update for function-call system: '<S631>/init' */
void InitTrigger_Model_76_172_156(void)
{
    /* MATLAB Function: '<S630>/PoolIirInit' */
    /* MATLAB Function: '<S630>/PoolIirInit' incorporates:
     *  S-Function (TOP_MEX): '<S626>/TOP'
     *  S-Function (TOP_MEX): '<S626>/TOP1'
     */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'ITC/init': '<S634>:1' */
    /* '<S634>:1:3' init(); */
    /*  Process function for Pooliir */
    /*  */
    /*  Parameters: */
    /*    x:                  input channels [frame size x number of inputs] */
    /*    NumStages:          array of filter lengths [NumChannels x 1] */
    /*    CoeffsTarget:       coefficient sets [total filter lengths x 1] */
    /*  */
    /*  Global parameters: */
    /*    states:             working memory set up during roommode reset/init CRL */
    /*                        function call [StatesSize x 1] */
    /* -------------------------------------------------------------------------- */
    /* MATLAB Function 'PoolIirWithReset/PoolIir/TargetSpecific/PoolIirInit': '<S632>:1' */
    /*  Extract configuration information from inputs */
    /* '<S632>:1:15' [FrameSize, NumChannels] = size(x); */
    /*  Initialize variables */
    /*  Get the work memory size */
    /* '<S632>:1:21' StatesSize = numel(states); */
    /*  Output */
    /* '<S632>:1:24' VariantStr = char(VariantUint8); */
    /* '<S632>:1:26' if(coder.target('MATLAB') || coder.target('Sfun')) */
    /* '<S632>:1:28' else */
    /* '<S632>:1:29' switch(VariantStr) */
    /* '<S632>:1:30' case{"Hexagon"} */
    /* '<S632>:1:31' NumCoeffPerSosStage = 4; */
    /* '<S632>:1:32' y = pooliir_hexagon_init(single(x), uint32(FrameSize), uint32(NumChannels), uint32(NumStages), single(CoeffsTarget), ... */
    /* '<S632>:1:33'                                      single(states), uint32(StatesSize),uint32(MaxNumOfThreads), uint32(MaxNumSosStages*NumCoeffPerSosStage)); */
    pooliir_init(&PostProcPostLimiterEq56836states[0],
                 &Model_Target_PostProcess_B.y[0], 32U, 24U,
                 &(Model_Target_PostProcess0.PostLimiterEqPooliirNumStages[0]),
                 &(Model_Target_PostProcess0.PostLimiterEqpooliirCoeffs[0]),
                 &PostProcPostLimiterEq56836states[0], 6161U, 0U, 512U);

    /* '<S632>:1:34' states(1) = y; */
}

/* System initialize for atomic system: '<S6>/PostProcess' */
void Model_Target_PostProcess_Init(void)
{
    int32_T i;

    /* user code (Initialize function Body) */

    /* InitTriggerControl '<S631>/ITC1' */
    InitTrigger_Model_76_172_156();

    /* Start for DataStoreMemory: '<S571>/CurrentGain' */
    Model_Target_PostProcess_DW.CurrentGainValue = 1.0F;

    /* InitializeConditions for Delay: '<S572>/Delay1' */
    Model_Target_PostProcess_DW.icLoad = true;

    /* InitializeConditions for Delay: '<S572>/Delay2' */
    Model_Target_PostProcess_DW.Delay2_DSTATE_n = 1.0F;

    /* SystemInitialize for Atomic SubSystem: '<S603>/Ramping_Subsystem' */
    /* SystemInitialize for IfAction SubSystem: '<S617>/ifActionSubsystem' */
    /* InitializeConditions for DiscreteFilter: '<S620>/ramping_filter' */
    Model_Target_PostProcess_DW.ramping_filter_icLoad = 1U;

    /* End of SystemInitialize for SubSystem: '<S617>/ifActionSubsystem' */
    /* End of SystemInitialize for SubSystem: '<S603>/Ramping_Subsystem' */

    /* SystemInitialize for MATLAB Function: '<S599>/MATLAB Function' */
    /* '<S581>:1:10' EBL_EnableCounter=int16(0); */
    /* '<S600>:1:22' state = single(1 * ones(1,channel_count)); */
    for (i = 0; i < 24; i++)
    {
        Model_Target_PostProcess_DW.state[i] = 1.0F;
    }

    /* End of SystemInitialize for MATLAB Function: '<S599>/MATLAB Function' */
}

/* Output and update for atomic system: '<S6>/PostProcess' */
void Model_Target_PostProcess(void)
{
    /* local scratch DWork variables */
    int32_T ForEach_itr;
    real_T rtb_Add1_a[24];
    real_T rtb_DataTypeConversion[24];
    real_T rtb_DataTypeConversion_e;
    real_T rtb_SumofElements;
    int32_T i;
    int32_T rtb_DataStoreRead1_f_tmp;
    int32_T s;
    real32_T rtb_Merge_in[32];
    real32_T downCoeff;
    real32_T gdiff;
    real32_T gp;
    real32_T lpfCoeff;
    real32_T rtb_DataStoreRead1_f_tmp_tmp;
    real32_T rtb_DataStoreRead1_ov;
    real32_T rtb_Delay_ey;
    real32_T rtb_Magsq_d;
    real32_T rtb_Max_g;
    real32_T upCoeff;
    real32_T *attack_param;
    real32_T *decay_param;
    real32_T *k1_param;
    real32_T *maxAttack_param;
    boolean_T rtb_LogicalOperator1_o;
    boolean_T rtb_Merge_ps;

    /* DataStoreRead: '<S603>/Data Store Read' */
    Model_Target_PostProcess_B.DataStoreRead =
        Model_TargetControlDsms.PostProcessMuteCurrentGain;

    /* DataStoreRead: '<S603>/Data Store Read1' */
    rtb_DataStoreRead1_ov =
        Model_TargetControlDsms.PostProcessMuteTargetLinearGain;

    /* End of Outputs for S-Function (RTC_REQ_RSP): '<S604>/RTC Request Response Trigger' */

    /* Outputs for Atomic SubSystem: '<S603>/Ramping_Subsystem' */
    /* MinMax: '<S617>/Max' incorporates:
     *  Constant: '<S617>/minGain'
     */
    rtb_Max_g = fmaxf(Model_Target_PostProcess_B.DataStoreRead, 4.7315126E-7F);

    /* If: '<S617>/If' incorporates:
     *  Abs: '<S617>/Abs3'
     *  Constant: '<S603>/snapThresh'
     *  DataStoreRead: '<S617>/frames'
     *  Sum: '<S617>/Add'
     */
    if ((qhmath_abs_f(rtb_DataStoreRead1_ov - rtb_Max_g) > 1.0E-9F) &&
            (Model_TargetControlDsms.PostProcessMuteNframes > 0.0F))
    {
        /* Outputs for IfAction SubSystem: '<S617>/ifActionSubsystem' incorporates:
         *  ActionPort: '<S620>/Action Port'
         */
        /* DataStoreWrite: '<S620>/Data Store Write' incorporates:
         *  Constant: '<S620>/Constant1'
         *  Sum: '<S620>/Add'
         */
        Model_TargetControlDsms.PostProcessMuteNframes =
            Model_TargetControlDsms.PostProcessMuteNframes - 1.0F;

        /* If: '<S622>/If' incorporates:
         *  Delay: '<S622>/Delay'
         *  Sum: '<S622>/Add2'
         */
        if (rtb_DataStoreRead1_ov - Model_Target_PostProcess_DW.Delay_DSTATE_l ==
            0.0F)
        {
            /* Outputs for IfAction SubSystem: '<S622>/If Action Subsystem' incorporates:
             *  ActionPort: '<S623>/Action Port'
             */
            Model_Target_IfActionSubsystem(&rtb_Merge_ps);

            /* End of Outputs for SubSystem: '<S622>/If Action Subsystem' */
        }
        else
        {
            /* Outputs for IfAction SubSystem: '<S622>/If Action Subsystem1' incorporates:
             *  ActionPort: '<S624>/Action Port'
             */
            Model_Target_IfActionSubsystem1(&rtb_Merge_ps);

            /* End of Outputs for SubSystem: '<S622>/If Action Subsystem1' */
        }

        /* End of If: '<S622>/If' */

        /* DiscreteFilter: '<S620>/ramping_filter' incorporates:
         *  Merge: '<S617>/Merge'
         *  Product: '<S620>/Matrix Multiply'
         *  Product: '<S621>/Multiply'
         */
        if (rtb_Merge_ps)
        {
            Model_Target_PostProcess_DW.ramping_filter_icLoad = 1U;
        }

        if (Model_Target_PostProcess_DW.ramping_filter_icLoad != 0)
        {
            Model_Target_PostProcess_DW.ramping_filter_states =
                Model_Target_PostProcess_B.ramping_coeff * rtb_Max_g;
        }

        s = 0;
        for (i = 0; i < 32; i++)
        {
            rtb_Merge_in[s] = Model_Target_PostProcess_DW.ramping_filter_states;
            Model_Target_PostProcess_DW.ramping_filter_states = 0.0F -
                -Model_Target_PostProcess_B.ramping_coeff * rtb_Merge_in[s];
            s++;
        }

        /* End of DiscreteFilter: '<S620>/ramping_filter' */

        /* Update for Delay: '<S622>/Delay' */
        Model_Target_PostProcess_DW.Delay_DSTATE_l = rtb_DataStoreRead1_ov;

        /* Update for DiscreteFilter: '<S620>/ramping_filter' */
        Model_Target_PostProcess_DW.ramping_filter_icLoad = 0U;

        /* End of Outputs for SubSystem: '<S617>/ifActionSubsystem' */
    }
    else
    {
        /* Outputs for IfAction SubSystem: '<S617>/elseActionSubsystem' incorporates:
         *  ActionPort: '<S619>/Action Port'
         */
        Model_Target_elseActionSubsystem(rtb_DataStoreRead1_ov, rtb_Merge_in);

        /* End of Outputs for SubSystem: '<S617>/elseActionSubsystem' */
    }

    /* End of If: '<S617>/If' */

    /* S-Function (sdspperm2): '<S614>/Variable Selector' incorporates:
     *  DataStoreWrite: '<S603>/Data Store Write'
     *  Merge: '<S617>/Merge'
     */
    Model_TargetControlDsms.PostProcessMuteCurrentGain = rtb_Merge_in[31];

    /* Outputs for Iterator SubSystem: '<S618>/For Each Subsystem' incorporates:
     *  ForEach: '<S625>/For Each'
     */
    for (ForEach_itr = 0; ForEach_itr < 32; ForEach_itr++)
    {
        /* ForEachSliceSelector generated from: '<S625>/In1' incorporates:
         *  Merge: '<S617>/Merge'
         */
        rtb_DataStoreRead1_ov = rtb_Merge_in[ForEach_itr];

        /* ForEachSliceAssignment generated from: '<S625>/Out1' incorporates:
         *  ForEachSliceSelector generated from: '<S625>/In2'
         *  Product: '<S625>/Multiply'
         */
        for (s = 0; s < 24; s++)
        {
            /* ForEachSliceSelector generated from: '<S625>/In2' */
            i = (s << 5) + ForEach_itr;
            Model_Target_PostProcess_B.ImpAsg_InsertedFor_Out1_at_inport_0[i] =
                Model_Target_B.out[i] * rtb_DataStoreRead1_ov;
        }

        /* End of ForEachSliceAssignment generated from: '<S625>/Out1' */
    }

    /* End of Outputs for SubSystem: '<S618>/For Each Subsystem' */
    /* End of Outputs for SubSystem: '<S603>/Ramping_Subsystem' */
    /* End of Outputs for S-Function (RTC_NTF): '<S604>/NTF Trigger' */
    /* End of Outputs for S-Function (RTC_REQ_RSP): '<S568>/RTC Request Response Trigger' */
    /* End of Outputs for S-Function (RTC_REQ_RSP): '<S569>/RTC Request Response Trigger' */
    /* MATLAB Function: '<S599>/MATLAB Function' incorporates:
     *  ForEachSliceAssignment generated from: '<S625>/Out1'
     *  S-Function (TOP_MEX): '<S599>/attack'
     *  S-Function (TOP_MEX): '<S599>/attack1'
     *  S-Function (TOP_MEX): '<S599>/decay'
     *  S-Function (TOP_MEX): '<S599>/decay1'
     *  S-Function (TOP_MEX): '<S599>/k1'
     *  S-Function (TOP_MEX): '<S599>/k2'
     *  S-Function (TOP_MEX): '<S599>/maxAttack'
     *  S-Function (TOP_MEX): '<S599>/maxAttack1'
     */
    /*  This is the main function for the Limiter. */
    /*  It takes an audio input, as well as 8 tunable variables (2 sets of 4), */
    /*  which are attack, maxAttack, k1, and decay. It will limit, or apply */
    /*  a gain reduction, to any samples that are essentially above a  */
    /*  pre-set threshold.  */
    /* MATLAB Function 'PostProcess/PostProcess/Limiter/Limiter/MATLAB Function': '<S600>:1' */
    /*  Grab data from input */
    /* '<S600>:1:10' y = coder.nullcopy(u); */
    /* '<S600>:1:11' [frame_size, channel_count] = size(u); */
    /*  Initialize memory */
    /*  attack = single(2.5768580901e-002 * ones(1,channel_count)); */
    /*  decay = single(1.0002610981e+000 * ones(1,channel_count)); */
    /* '<S600>:1:16' maxGain = single(1); */
    /*  maxAttack = single(3.1622776602e-001 * ones(1,channel_count)); */
    /*  k1 = single(2.8631756556e-002 * ones(1,channel_count)); */
    /* '<S600>:1:21' if isempty(state) */
    /* '<S600>:1:25' if (param_set == 1) */
    if (Model_TargetPostProcessLimiterSoftclipParamSetRtcOut.REQ_paramSet == 1U)
    {
        /* '<S600>:1:26' attack_param = attack; */
        /* '<S600>:1:27' k1_param = k1; */
        /* '<S600>:1:28' decay_param = decay; */
        /* '<S600>:1:29' maxAttack_param = maxAttack; */
        attack_param = (&(Model_Target_PostProcess0.LimiterCfgattack[0]));
        k1_param = (&(Model_Target_PostProcess0.LimiterCfgk1[0]));
        decay_param = (&(Model_Target_PostProcess0.LimiterCfgdecay[0]));
        maxAttack_param = (&(Model_Target_PostProcess0.LimiterCfgmaxAttack[0]));
    }
    else
    {
        /* '<S600>:1:30' else */
        /* '<S600>:1:31' attack_param = attack_low; */
        /* '<S600>:1:32' k1_param = k1_low; */
        /* '<S600>:1:33' decay_param = decay_low; */
        /* '<S600>:1:34' maxAttack_param = maxAttack_low; */
        attack_param = (&(Model_Target_PostProcess0.LimiterCfgattack_low[0]));
        k1_param = (&(Model_Target_PostProcess0.LimiterCfgk1_low[0]));
        decay_param = (&(Model_Target_PostProcess0.LimiterCfgdecay_low[0]));
        maxAttack_param = (&(Model_Target_PostProcess0.LimiterCfgmaxAttack_low[0]));
    }

    /* '<S600>:1:38' Limiter_Active = uint32(0); */
    /*  Check for a disable RTC message, pass audio through unaffected if so */
    /* '<S600>:1:40' if (disableMsg) */
    if (Model_TargetPostProcessLimiterSoftclipDisableRtcOut.REQ_disableMsg != 0U)
    {
        /* '<S600>:1:41' y(:) = u(:); */
        memcpy(&Model_Target_PostProcess_B.y_f[0],
               &Model_Target_PostProcess_B.ImpAsg_InsertedFor_Out1_at_inport_0[0],
               768U * sizeof(real32_T));
    }
    else
    {
        /* '<S600>:1:42' else */
        /*  Process the audio */
        /* '<S600>:1:44' for i = 1:channel_count */
        for (i = 0; i < 24; i++)
        {
            /* '<S600>:1:45' k0 = 1 + attack_param(i); */
            /* '<S600>:1:46' g1 = state(i); */
            rtb_DataStoreRead1_ov = Model_Target_PostProcess_DW.state[i];

            /* '<S600>:1:47' temp = single(0); */
            /* '<S600>:1:48' temp1 = temp; */
            /* '<S600>:1:49' for s = 1:frame_size */
            for (s = 0; s < 32; s++)
            {
                rtb_Max_g = attack_param[i];

                /*  Compute the gain for this sample. Implementation copied from BDSP */
                /* '<S600>:1:51' p1 = k1_param(i) * g1; */
                /* '<S600>:1:52' p2 = u(s, i); */
                /* '<S600>:1:53' p2 = abs(p2); */
                /* '<S600>:1:54' temp = temp + p2; */
                /* '<S600>:1:55' g1 = g1 * decay_param(i); */
                /* '<S600>:1:56' p2 = p2 * p1; */
                /* '<S600>:1:57' p2 = max(p2, attack_param(i)); */
                /* '<S600>:1:58' p2 = k0 - p2; */
                /* '<S600>:1:59' p2 = max(p2, maxAttack_param(i)); */
                /* '<S600>:1:60' g1 = g1 * p2; */
                /* '<S600>:1:61' g1 = min(g1, maxGain); */
                rtb_DataStoreRead1_f_tmp = (i << 5) + s;
                rtb_Delay_ey =
                    Model_Target_PostProcess_B.ImpAsg_InsertedFor_Out1_at_inport_0
                    [rtb_DataStoreRead1_f_tmp];
                rtb_DataStoreRead1_ov = fminf(fmaxf((rtb_Max_g + 1.0F) - fmaxf
                    (k1_param[i] * rtb_DataStoreRead1_ov * qhmath_abs_f
                     (rtb_Delay_ey), rtb_Max_g), maxAttack_param[i]) *
                    (rtb_DataStoreRead1_ov * decay_param[i]), 1.0F);

                /*  Apply the gain */
                /* '<S600>:1:63' y(s, i) = u(s, i) * g1; */
                Model_Target_PostProcess_B.y_f[rtb_DataStoreRead1_f_tmp] =
                    rtb_Delay_ey * rtb_DataStoreRead1_ov;

                /* '<S600>:1:64' p3 = abs(y(s,i)); */
                /* '<S600>:1:65' temp1 = temp1 + p3; */
            }

            /* '<S600>:1:68' state(i) = g1; */
            Model_Target_PostProcess_DW.state[i] = rtb_DataStoreRead1_ov;

            /* '<S600>:1:69' if temp1 < temp */
        }
    }

    /* End of MATLAB Function: '<S599>/MATLAB Function' */

    /* Math: '<S573>/Magsq'
     *
     * About '<S573>/Magsq':
     *  Operator: magnitude^2
     */
    for (i = 0; i < 768; i++)
    {
        rtb_DataStoreRead1_ov = Model_Target_PostProcess_B.y_f[i];
        Model_Target_PostProcess_B.Magsq[i] = rtb_DataStoreRead1_ov *
            rtb_DataStoreRead1_ov;
    }

    /* End of Math: '<S573>/Magsq' */
    for (i = 0; i < 24; i++)
    {
        /* Sum: '<S573>/Add' incorporates:
         *  Math: '<S573>/Magsq'
         *
         * About '<S573>/Magsq':
         *  Operator: magnitude^2
         */
        s = i << 5;
        rtb_DataStoreRead1_ov = -0.0F;
        for (rtb_DataStoreRead1_f_tmp = 0; rtb_DataStoreRead1_f_tmp < 32;
                rtb_DataStoreRead1_f_tmp++)
        {
            rtb_DataStoreRead1_ov += Model_Target_PostProcess_B.Magsq[s +
                rtb_DataStoreRead1_f_tmp];
        }

        /* Sum: '<S579>/Add1' incorporates:
         *  Delay: '<S579>/Delay2'
         *  Product: '<S579>/Product3'
         *  Product: '<S579>/Product4'
         *  Sum: '<S573>/Add'
         */
        rtb_SumofElements = rtb_DataStoreRead1_ov *
            Model_Target_PostProcess_ConstB.Subtract1 +
            Model_Target_PostProcess_ConstB.MathFunction *
            Model_Target_PostProcess_DW.Delay2_DSTATE[i];

        /* Gain: '<S572>/Multiply' incorporates:
         *  Gain: '<S572>/Multiply1'
         *  Math: '<S572>/Log10'
         *
         * About '<S572>/Log10':
         *  Operator: log10
         */
        rtb_DataTypeConversion_e = log10(0.0625 * rtb_SumofElements) * 10.0;

        /* DataTypeConversion: '<S572>/Data Type Conversion1' */
        Model_Target_PostProcess_B.DataTypeConversion1[i] = (real32_T)
            rtb_DataTypeConversion_e;

        /* Sum: '<S579>/Add1' */
        rtb_Add1_a[i] = rtb_SumofElements;

        /* Gain: '<S572>/Multiply' incorporates:
         *  DataTypeConversion: '<S592>/Data Type Conversion'
         */
        rtb_DataTypeConversion[i] = rtb_DataTypeConversion_e;
    }

    /* Delay: '<S572>/Delay1' incorporates:
     *  DataStoreRead: '<S571>/Data Store Read'
     */
    if (Model_Target_PostProcess_DW.icLoad)
    {
        Model_Target_PostProcess_DW.Delay1_DSTATE =
            Model_Target_PostProcess_DW.CurrentGainValue;
    }

    /* Delay: '<S572>/Delay1' */
    Model_Target_PostProcess_B.CurrentGain =
        Model_Target_PostProcess_DW.Delay1_DSTATE;

    /* Sum: '<S592>/Sum of Elements' */
    rtb_SumofElements = -0.0;
    for (rtb_DataStoreRead1_f_tmp = 0; rtb_DataStoreRead1_f_tmp < 24;
            rtb_DataStoreRead1_f_tmp++)
    {
        /* DataTypeConversion: '<S592>/Data Type Conversion' incorporates:
         *  RelationalOperator: '<S592>/Relational Operator'
         */
        rtb_DataTypeConversion_e =
            (rtb_DataTypeConversion[rtb_DataStoreRead1_f_tmp] >
             Model_Target_PostProcess_ConstB.DEBUG_EBL_THRESHOLD_DBSPL);

        /* Sum: '<S592>/Sum of Elements' */
        rtb_SumofElements += rtb_DataTypeConversion_e;

        /* DataTypeConversion: '<S592>/Data Type Conversion' incorporates:
         *  RelationalOperator: '<S592>/Relational Operator'
         */
        rtb_DataTypeConversion[rtb_DataStoreRead1_f_tmp] =
            rtb_DataTypeConversion_e;
    }

    /* Delay: '<S572>/Delay2' */
    rtb_Max_g = Model_Target_PostProcess_DW.Delay2_DSTATE_n;

    /* Logic: '<S572>/Logical Operator' incorporates:
     *  Delay: '<S572>/Delay2'
     *  RelationalOperator: '<S572>/Relational Operator2'
     *  RelationalOperator: '<S572>/Relational Operator4'
     */
    rtb_Merge_ps = ((rtb_SumofElements >
                     Model_Target_PostProcess_ConstB.RoundingFunction_i) &&
                    (Model_Target_PostProcess_DW.Delay2_DSTATE_n >=
                     Model_Target_PostProcess_ConstB.RoundingFunction1));

    /* Logic: '<S572>/Logical Operator1' incorporates:
     *  Delay: '<S572>/Delay2'
     *  RelationalOperator: '<S572>/Relational Operator1'
     *  RelationalOperator: '<S572>/Relational Operator3'
     */
    rtb_LogicalOperator1_o = ((rtb_SumofElements <=
        Model_Target_PostProcess_ConstB.RoundingFunction_i) &&
        (Model_Target_PostProcess_DW.Delay2_DSTATE_n >=
         Model_Target_PostProcess_ConstB.RoundingFunction));

    /* MATLAB Function: '<S572>/EBL State' incorporates:
     *  Constant: '<S591>/Constant1'
     */
    /*  */
    /*  Return gain notifications based on current gain and thresholds */
    /*  */
    /* MATLAB Function 'PostProcess/PostProcess/EnergyLimiter/EnergyLimiter1/EnergyRunningAvg /EBL State': '<S580>:1' */
    /* '<S580>:1:6' ELNotification = single(0); */
    rtb_DataStoreRead1_ov = 0.0F;

    /* '<S580>:1:7' if((currgain<maxgain)) */
    if (Model_Target_PostProcess_B.CurrentGain < 1.0F)
    {
        /* '<S580>:1:8' if(aboveth) */
        if (rtb_Merge_ps)
        {
            /*     %% Gain reduction when energy limiter active */
            /* '<S580>:1:10' ELNotification = single(1.0); */
            rtb_DataStoreRead1_ov = 1.0F;
        }

        /* if((currgain==mingain)) */
        /* '<S580>:1:13' if(belowth) */
        if (rtb_LogicalOperator1_o)
        {
            /*         %% Increased gain notification when energy limiter active */
            /* '<S580>:1:15' ELNotification= single(2.0); */
            rtb_DataStoreRead1_ov = 2.0F;
        }
    }
    else if (Model_Target_PostProcess_B.CurrentGain == 1.0F)
    {
        /* '<S580>:1:17' elseif(currgain== maxgain) */
        /*     %% No Gain reduction when energy limiter active */
        /* '<S580>:1:19' ELNotification= single(3.0); */
        rtb_DataStoreRead1_ov = 3.0F;
    }
    else
    {
        /* '<S580>:1:20' else */
        /* '<S580>:1:21' ELNotification= single(4.0); */
        rtb_DataStoreRead1_ov = 4.0F;
    }

    /* End of MATLAB Function: '<S572>/EBL State' */

    /* Delay: '<S572>/Delay' */
    /* '<S580>:1:24' y = single(ELNotification); */
    rtb_Delay_ey = Model_Target_PostProcess_DW.Delay_DSTATE;

    /* End of Outputs for S-Function (RTC_REQ_RSP): '<S574>/RTC Request Response Trigger' */

    /* DataTypeConversion: '<S572>/Data Type Conversion4' */
    rtb_DataStoreRead1_f_tmp_tmp = (real32_T)
        Model_TargetPostProcessEnergyLimiterConfigEnableLimiterRtcOut.REQ_EnableGainLimiter;

    /* MATLAB Function: '<S572>/EBL Transition' incorporates:
     *  DataTypeConversion: '<S572>/Data Type Conversion4'
     *  Delay: '<S572>/Delay3'
     */
    /*  */
    /*  Calculate EBL states */
    /*  */
    /*  Calculate next state and transition from previous state, current state, previous enable */
    /*  flag, and current enable flag */
    /*  */
    /* MATLAB Function 'PostProcess/PostProcess/EnergyLimiter/EnergyLimiter1/EnergyRunningAvg /EBL Transition': '<S581>:1' */
    /* '<S581>:1:10' if isempty(EBL_EnableCounter) */
    /*  List of all EBL states and transitions */
    /* '<S581>:1:13' EBL_EnableFlag_False         = single(0.0); */
    /* '<S581>:1:14' EBL_EnableFlag_True          = single(1.0); */
    /* '<S581>:1:15' EBL_StateNoUpdate            = single(0.0); */
    /* '<S581>:1:16' EBL_StateAttackInProgress    = single(1.0); */
    /* '<S581>:1:17' EBL_StateRecoveryInProgress  = single(2.0); */
    /* '<S581>:1:18' EBL_StateDisengaged          = single(3.0); */
    /* '<S581>:1:19' EBL_TransitionNoChange       = single(0.0); */
    /* '<S581>:1:20' EBL_TransitionAttackStart    = single(1.0); */
    /* '<S581>:1:21' EBL_TransitionRecoverDone    = single(2.0); */
    /*  First, check if the enable flag is updated */
    /* '<S581>:1:24' if(preState==EBL_StateAttackInProgress     && ... */
    /* '<S581>:1:25'         curEnableFlag==EBL_EnableFlag_True && ... */
    /* '<S581>:1:26'         preEnableFlag==EBL_EnableFlag_False) */
    if ((rtb_Delay_ey == 1.0F) &&
            (Model_TargetPostProcessEnergyLimiterConfigEnableLimiterRtcOut.REQ_EnableGainLimiter
             == 1.0F) && (Model_Target_PostProcess_DW.Delay3_DSTATE == 0.0F))
    {
        /* '<S581>:1:27' EBL_EnableCounter = EBL_EnableCounter+1; */
        s = Model_Target_PostProcess_DW.EBL_EnableCounter_o + 1;
        if (Model_Target_PostProcess_DW.EBL_EnableCounter_o + 1 > 32767)
        {
            s = 32767;
        }

        Model_Target_PostProcess_DW.EBL_EnableCounter_o = (int16_T)s;

        /*  EBL enabled while attack in progress */
        /* '<S581>:1:29' transition=EBL_TransitionAttackStart; */
        Model_Target_PostProcess_B.transition = 1.0F;

        /* '<S581>:1:30' if(curState == EBL_StateNoUpdate) */
        if (rtb_DataStoreRead1_ov == 0.0F)
        {
            /* '<S581>:1:31' nextState=preState; */
            rtb_DataStoreRead1_ov = 1.0F;
        }
        else
        {
            /* '<S581>:1:32' else */
            /* '<S581>:1:33' nextState=curState; */
        }

        /*  Check if the EBL state is updated */
        /* '<S581>:1:39' if(curState == EBL_StateNoUpdate) */
    }
    else if (rtb_DataStoreRead1_ov == 0.0F)
    {
        /* '<S581>:1:40' transition=EBL_TransitionNoChange; */
        Model_Target_PostProcess_B.transition = 0.0F;

        /* '<S581>:1:41' nextState=preState; */
        rtb_DataStoreRead1_ov = rtb_Delay_ey;

        /* '<S581>:1:43' else */
        /* '<S581>:1:44' nextState=curState; */
        /*  Determine the transition based on EBL state */
        /* '<S581>:1:48' if(preState==EBL_StateDisengaged && ... */
        /* '<S581>:1:49'    curState==EBL_StateAttackInProgress) */
    }
    else if ((rtb_Delay_ey == 3.0F) && (rtb_DataStoreRead1_ov == 1.0F))
    {
        /*  attack begins */
        /* '<S581>:1:51' transition=EBL_TransitionAttackStart; */
        Model_Target_PostProcess_B.transition = 1.0F;
    }
    else if ((rtb_Delay_ey == 2.0F) && (rtb_DataStoreRead1_ov == 3.0F))
    {
        /* '<S581>:1:52' elseif(preState==EBL_StateRecoveryInProgress && ... */
        /* '<S581>:1:53'        curState==EBL_StateDisengaged) */
        /*  recovery done */
        /* '<S581>:1:55' transition=EBL_TransitionRecoverDone; */
        Model_Target_PostProcess_B.transition = 2.0F;
    }
    else
    {
        /* '<S581>:1:56' else */
        /* '<S581>:1:57' transition=EBL_TransitionNoChange; */
        Model_Target_PostProcess_B.transition = 0.0F;
    }

    /* End of MATLAB Function: '<S572>/EBL Transition' */

    /* DataTypeConversion: '<S572>/Data Type Conversion5' */
    rtb_Delay_ey = fmodf(qhmath_floor_f(rtb_DataStoreRead1_ov), 65536.0F);

    /* DataTypeConversion: '<S572>/Data Type Conversion5' */
    Model_Target_PostProcess_B.DataTypeConversion5 = (int16_T)(rtb_Delay_ey <
        0.0F ? (int32_T)(int16_T)-(int16_T)(uint16_T)-rtb_Delay_ey : (int32_T)
        (int16_T)(uint16_T)rtb_Delay_ey);

    /* DataTypeConversion: '<S572>/Data Type Conversion6' */
    Model_Target_PostProcess_B.DataTypeConversion6 = (int16_T)
        Model_TargetPostProcessEnergyLimiterConfigEnableLimiterRtcOut.REQ_EnableGainLimiter;

    /* Switch: '<S572>/Switch' incorporates:
     *  Constant: '<S572>/Constant'
     *  MinMax: '<S572>/MinMax'
     *  Product: '<S572>/Product2'
     *  Switch: '<S572>/Switch1'
     *  Switch: '<S572>/Switch2'
     *  Switch: '<S572>/Switch3'
     */
    if (rtb_Merge_ps)
    {
        rtb_Delay_ey = (real32_T)fmax(Model_Target_PostProcess_B.CurrentGain *
            Model_Target_PostProcess_ConstB.MathFunction2,
            Model_Target_PostProcess_ConstB.MathFunction4);
        rtb_Max_g = 0.0F;
    }
    else if (rtb_LogicalOperator1_o)
    {
        /* Switch: '<S572>/Switch1' incorporates:
         *  Constant: '<S591>/Constant1'
         *  MinMax: '<S572>/MinMax1'
         *  Product: '<S572>/Product1'
         */
        rtb_Delay_ey = fminf((real32_T)
                             (Model_Target_PostProcess_ConstB.MathFunction1 *
                              Model_Target_PostProcess_B.CurrentGain), 1.0F);

        /* Switch: '<S572>/Switch3' incorporates:
         *  Constant: '<S572>/Constant1'
         */
        rtb_Max_g = 0.0F;
    }
    else
    {
        /* Switch: '<S572>/Switch1' */
        rtb_Delay_ey = Model_Target_PostProcess_B.CurrentGain;

        /* Switch: '<S572>/Switch3' incorporates:
         *  Constant: '<S572>/Constant2'
         *  Sum: '<S572>/Add'
         */
        rtb_Max_g++;
    }

    /* End of Switch: '<S572>/Switch' */
    /* End of Outputs for S-Function (RTC_NTF): '<S574>/NTF Trigger' */

    /* MATLAB Function: '<S571>/RamperProcess' incorporates:
     *  Constant: '<S571>/Constant2'
     *  Constant: '<S571>/Constant6'
     *  Constant: '<S571>/Constant7'
     *  DataStoreRead: '<S571>/Data Store Read'
     *  S-Function (AudioStats_MEX): '<S571>/Audio Stats'
     */
    /* MATLAB Function 'PostProcess/PostProcess/EnergyLimiter/EnergyLimiter1/RamperProcess': '<S575>:1' */
    /* '<S575>:1:2' M = 1; */
    /*  for matlab 1 based indexing */
    /* '<S575>:1:3' nf = single(size(x,1)); */
    /*  get the frame size. DO NOT USE length() since it may */
    /* '<S575>:1:4' y = single(zeros(size(x))); */
    /*  more channels than samples per frame. */
    /* Defines for status */
    /* '<S575>:1:7' RAMP_IDLE        = 0; */
    /* '<S575>:1:7' RAMPING_UP       = 1; */
    /* '<S575>:1:7' RAMPING_DOWN     = -1; */
    /* '<S575>:1:9' numChs = single(size(x,2)); */
    /* '<S575>:1:9' numRampers = 1; */
    /*  Configure the rgain block to have an attack (or decay) rate that */
    /*  is twice as fast as the ratio of GainChangeStepIndB to */
    /*  GainIncreaseDurationInSeconds (or */
    /*  GainDecreaseDurationInSeconds). That way, the rgain block will */
    /*  have settled to the target value before a new gain change occurs. */
    /* '<S575>:1:15' rampUpRates_dBPerSec= (2*GainChangedB/GainInc); */
    /* '<S575>:1:15' rampDownRates_dBPerSec=(-2*GainChangedB/GainDec); */
    /* '<S575>:1:16' frameSize=nf; */
    /* '<S575>:1:16' Fs=SampleRate; */
    /* '<S575>:1:16' lpfTau=frameSize/Fs; */
    /* '<S575>:1:16' snapThresh=single(1e-5); */
    /* '<S575>:1:16' mindB=single(-100.0); */
    /* '<S575>:1:19' upCoeff       = undb20(rampUpRates_dBPerSec/Fs); */
    /* UNDB20  Decibels to linear quantity */
    /*    Converts x from DB to absolute magnitude assuming that x is in volts. */
    /*    y = 10^(x/20) */
    /*  */
    /*  See also: DB20, UNDB10 */
    /*    Copyright 2018-2019 Bose Corporation */
    /*  Created : 9/7/1994 Bill Berardi */
    /* $Id: PostProcess.c 177773 2024-02-27 10:36:46Z ad55945 $ */
    /* 'undb20:12' y = exp(x * log(10)/20); */
    upCoeff = qhmath_exp_f(qhmath_div_f(qhmath_div_f(2.0F, 48000.0) *
                            2.30258512F, 20.0F));

    /* '<S575>:1:19' upCoeffN  = upCoeff .^ frameSize; */
    /* '<S575>:1:20' downCoeff     = undb20(-abs(rampDownRates_dBPerSec)/Fs); */
    /* UNDB20  Decibels to linear quantity */
    /*    Converts x from DB to absolute magnitude assuming that x is in volts. */
    /*    y = 10^(x/20) */
    /*  */
    /*  See also: DB20, UNDB10 */
    /*    Copyright 2018-2019 Bose Corporation */
    /*  Created : 9/7/1994 Bill Berardi */
    /* $Id: PostProcess.c 177773 2024-02-27 10:36:46Z ad55945 $ */
    /* 'undb20:12' y = exp(x * log(10)/20); */
    downCoeff = qhmath_exp_f(qhmath_div_f(qhmath_div_f(-2.0F, 48000.0) *
        2.30258512F, 20.0F));

    /* '<S575>:1:20' downCoeffN  = downCoeff .^ frameSize; */
    /* '<S575>:1:21' lpfCoeff      = single(exp(-1./(lpfTau.*Fs))); */
    lpfCoeff = qhmath_exp_f(qhmath_div_f(-1.0F, qhmath_div_f(32.0F, 48000.0) *
        48000.0));

    /* '<S575>:1:21' minGain = undb20(mindB); */
    /* '<S575>:1:22' targetCoeffSet =single(zeros(numRampers,1)); */
    /* '<S575>:1:22' activeCoeffSet =single(zeros(numRampers,1)); */
    /* '<S575>:1:23' status = RAMP_IDLE; */
    /* '<S575>:1:23' freeze=single(0.0); */
    /* '<S575>:1:24' targetRamper =single(zeros(numChs,1)); */
    /* '<S575>:1:24' activeRamper=single(zeros(numChs,1)); */
    /* '<S575>:1:26' if(isempty(prevGain)) */
    if (!Model_Target_PostProcess_DW.prevGain_not_empty)
    {
        /* '<S575>:1:27' prevGain=single((PreviousGain*ones(numRampers,1))); */
        Model_Target_PostProcess_DW.prevGain =
            Model_Target_PostProcess_DW.CurrentGainValue;
        Model_Target_PostProcess_DW.prevGain_not_empty = true;
    }

    /* '<S575>:1:29' targetGain=targetgainlinear; */
    /* '<S575>:1:30' if(targetgainlinear<1) */
    /* '<S575>:1:33' rGain=single(zeros(numRampers,1)); */
    /*  simulate use of scratch buffers for ramped envelopes */
    /* '<S575>:1:37' scratchBufs = single(zeros(nf,numRampers)); */
    /*  loop over rampers first */
    /* '<S575>:1:39' for r = 1:numRampers */
    /*  update active set with target */
    /* '<S575>:1:41' activeCoeffSet(r) = targetCoeffSet(r); */
    /* '<S575>:1:42' setIdx = activeCoeffSet(r) + M; */
    /*  get all coeffs from active set */
    /* '<S575>:1:44' lpfCoeff = lpfCoeff(setIdx); */
    /* '<S575>:1:46' gp = prevGain(r); */
    gp = Model_Target_PostProcess_DW.prevGain;

    /* '<S575>:1:47' if freeze */
    /* '<S575>:1:49' else */
    /* '<S575>:1:50' gt = targetGain(r); */
    /* '<S575>:1:52' gdiff = gt-gp; */
    gdiff = Model_Target_PostProcess_B.CurrentGain -
        Model_Target_PostProcess_DW.prevGain;

    /*  Snap to target gain if it's close enough */
    /* '<S575>:1:55' if( abs(gdiff) <= snapThresh ) */
    if (qhmath_abs_f(gdiff) <= 1.0E-5F)
    {
        /* '<S575>:1:56' prevGain(r) = gt; */
        Model_Target_PostProcess_DW.prevGain =
            Model_Target_PostProcess_B.CurrentGain;

        /* '<S575>:1:57' status(r) = RAMP_IDLE; */
        /* '<S575>:1:58' scratchBufs(:,r) = gt; */
        for (i = 0; i < 32; i++)
        {
            rtb_Merge_in[i] = Model_Target_PostProcess_B.CurrentGain;
        }
    }
    else
    {
        /*  Ramp up or down? */
        /* '<S575>:1:63' if( gdiff>0 ) */
        if (gdiff > 0.0F)
        {
            /* '<S575>:1:64' status(r) = RAMPING_UP; */
            /*  Check for ramp done */
            /* '<S575>:1:67' temp = max(gp,minGain); */
            downCoeff = fmaxf(Model_Target_PostProcess_DW.prevGain,
                              9.99999338E-6F);

            /*  previous gain cannot be zero for exponential ramp */
            /* '<S575>:1:68' if( temp*upCoeffN(setIdx) < gt ) */
            if (downCoeff * qhmath_pow_f(upCoeff, 32.0F) <
                    Model_Target_PostProcess_B.CurrentGain)
            {
                /*  Ramp not done, so do zero-input exponential ramp */
                /* '<S575>:1:70' a = upCoeff(setIdx); */
                lpfCoeff = upCoeff;

                /* '<S575>:1:70' b0 = single(0.0); */
                upCoeff = 0.0F;

                /* '<S575>:1:70' gp = temp; */
                gp = downCoeff;
            }
            else
            {
                /* '<S575>:1:71' else */
                /*  LPF with target gain as input */
                /* '<S575>:1:73' a = lpfCoeff; */
                /* '<S575>:1:73' b0 = 1-a; */
                upCoeff = 1.0F - lpfCoeff;
            }

            /* '<S575>:1:75' else */
            /* '<S575>:1:76' status(r) = RAMPING_DOWN; */
            /*  Check for ramp done */
            /* '<S575>:1:78' if( gp*downCoeffN(setIdx) > gt ) */
        }
        else if (Model_Target_PostProcess_DW.prevGain * qhmath_pow_f(downCoeff,
                  32.0F) > Model_Target_PostProcess_B.CurrentGain)
        {
            /*  Ramp not done, so do zero-input exponential ramp */
            /* '<S575>:1:80' a = downCoeff(setIdx); */
            lpfCoeff = downCoeff;

            /* '<S575>:1:80' b0 = single(0.0); */
            upCoeff = 0.0F;
        }
        else
        {
            /* '<S575>:1:81' else */
            /*  LPF with target gain as input */
            /* '<S575>:1:83' a = lpfCoeff; */
            /* '<S575>:1:83' b0 = 1-a; */
            upCoeff = 1.0F - lpfCoeff;
        }

        /*  Calculate gain envelope for current ramper */
        /*      zi = a*gp;    % initial condition for filter */
        /*      scratchBufs(:,r) = filter(b0,[1 -a],gt*ones(nf,1),zi); */
        /*  use single() to force MATLAB to compute gain envelope in 32-bit */
        /*  precision to match the performance of current 32-bit DSP processors */
        /* '<S575>:1:91' for k = 1:nf */
        for (i = 0; i < 32; i++)
        {
            /* scratchBufs(k,r) = double(single(b0)*single(gt) + (single(a)*single(gp)));  */
            /* '<S575>:1:93' scratchBufs(k,r) = b0*gt + a*gp; */
            gp = upCoeff * Model_Target_PostProcess_B.CurrentGain + lpfCoeff *
                gp;

            /* '<S575>:1:93' gp = scratchBufs(k,r); */
            rtb_Merge_in[i] = gp;
        }

        /*  save state */
        /* '<S575>:1:96' prevGain(r) = scratchBufs(end,r); */
        Model_Target_PostProcess_DW.prevGain = rtb_Merge_in[31];
    }

    /*  then apply ramped gains to channels */
    /* '<S575>:1:100' for ch = 1:numChs */
    for (i = 0; i < 24; i++)
    {
        /* '<S575>:1:101' activeRamper(ch) = targetRamper(ch); */
        /*  update active ramper with target */
        /* '<S575>:1:102' if  activeRamper(ch) ~= -1 */
        /* '<S575>:1:103' rGain = scratchBufs(:,activeRamper(ch) + M); */
        /*  get ramped envelope assigned to ch */
        /* '<S575>:1:104' y(:,ch) = rGain.*x(:,ch); */
        for (s = 0; s < 32; s++)
        {
            rtb_DataStoreRead1_f_tmp = (i << 5) + s;
            Model_Target_PostProcess_B.Magsq[rtb_DataStoreRead1_f_tmp] =
                Model_Target_PostProcess_B.y_f[rtb_DataStoreRead1_f_tmp] *
                rtb_Merge_in[s];
        }
    }

    /* End of MATLAB Function: '<S571>/RamperProcess' */

    /* Switch: '<S571>/Switch' */
    rtb_Merge_ps =
        (Model_TargetPostProcessEnergyLimiterConfigEnableLimiterRtcOut.REQ_EnableGainLimiter
         > 0U);
    for (s = 0; s < 768; s++)
    {
        rtb_Magsq_d = Model_Target_PostProcess_B.Magsq[s];
        if (!rtb_Merge_ps)
        {
            rtb_Magsq_d = Model_Target_PostProcess_B.y_f[s];
        }

        Model_Target_PostProcess_B.Magsq[s] = rtb_Magsq_d;
    }

    /* End of Switch: '<S571>/Switch' */
    /* MATLAB Function: '<S653>/MATLAB Function' incorporates:
     *  Switch: '<S571>/Switch'
     */
    /*  This is the main function for the Soft-Clipper */
    /*  It takes in audio, typically from the Limiter, and applies a gain */
    /*  reduction when appropriate.  */
    /*  It takes in two sets of tunable parameters. */
    /*  Each set represents values corresponding to a car that either stays on, */
    /*  or shuts off, when idling.  */
    /* MATLAB Function 'PostProcess/PostProcess/Soft Clipper/SoftClipper/MATLAB Function': '<S654>:1' */
    /* '<S654>:1:9' y = coder.nullcopy(u); */
    /* '<S654>:1:10' [frame_size, channel_count] = size(u); */
    /*  Choose whether we are using the set of coefficients for a car that shuts */
    /*  off while idling, or not. */
    /* '<S654>:1:14' if (param_set == 1) */
    if (Model_TargetPostProcessLimiterSoftclipParamSetRtcOut.REQ_paramSet == 1U)
    {
        /* '<S654>:1:15' xmin_param = xmin; */
        lpfCoeff = Model_Target_PostProcess0.Sclipxmin;

        /* '<S654>:1:16' xmax_param = xmax; */
        gp = Model_Target_PostProcess0.Sclipxmax;

        /* '<S654>:1:17' p2_param = p2; */
        upCoeff = Model_Target_PostProcess0.Sclipp2;
    }
    else
    {
        /* '<S654>:1:18' else */
        /* '<S654>:1:19' xmin_param = xmin_low; */
        lpfCoeff = Model_Target_PostProcess0.Sclipxmin_low;

        /* '<S654>:1:20' xmax_param = xmax_low; */
        gp = Model_Target_PostProcess0.Sclipxmax_low;

        /* '<S654>:1:21' p2_param = p2_low; */
        upCoeff = Model_Target_PostProcess0.Sclipp2_low;
    }

    /* '<S654>:1:25' Sclip_Active = uint32(0); */
    /*  If RTC Disable message is 1, do not perform soft-clipping */
    /* '<S654>:1:27' if (disableMsg) */
    if (Model_TargetPostProcessLimiterSoftclipDisableRtcOut.REQ_disableMsg != 0U)
    {
        /* '<S654>:1:28' y(:) = u(:); */
        memcpy(&Model_Target_PostProcess_B.y[0],
               &Model_Target_PostProcess_B.Magsq[0], 768U * sizeof(real32_T));
    }
    else
    {
        /* '<S654>:1:29' else */
        /*      Loop over all channels */
        /* '<S654>:1:31' for i = 1:channel_count */
        for (i = 0; i < 24; i++)
        {
            /*          Loop over each sample of each channel */
            /* '<S654>:1:33' for j = 1:frame_size */
            for (s = 0; s < 32; s++)
            {
                rtb_DataStoreRead1_f_tmp = (i << 5) + s;
                rtb_Magsq_d =
                    Model_Target_PostProcess_B.Magsq[rtb_DataStoreRead1_f_tmp];

                /* '<S654>:1:34' x1 = abs( u(j,i) ); */
                /* '<S654>:1:35' x1 = min(x1, xmax_param); */
                downCoeff = fminf(qhmath_abs_f(rtb_Magsq_d), gp);

                /* '<S654>:1:36' x2 = x1 - xmin_param; */
                /* '<S654>:1:37' x2 = max(x2, single(0)); */
                gdiff = fmaxf(downCoeff - lpfCoeff, 0.0F);

                /* '<S654>:1:38' x2 = x2 * x2; */
                /* '<S654>:1:39' x2 = x2 * p2_param; */
                /* '<S654>:1:40' x1 = x1 - x2; */
                /* '<S654>:1:41' y(j,i) = copysignf(x1, u(j,i)); */
                Model_Target_PostProcess_B.y[rtb_DataStoreRead1_f_tmp] =
                    copysignf(downCoeff - gdiff * gdiff * upCoeff, rtb_Magsq_d);
            }

            /* '<S654>:1:43' if (max(abs(u(:,i)))> xmin_param) */
        }
    }

    /* End of MATLAB Function: '<S653>/MATLAB Function' */
    /* End of Outputs for S-Function (InitTriggerControl): '<S631>/ITC1' */

    /* MATLAB Function: '<S630>/PoolIirProcess' */
    /*  Process function for Pooliir */
    /*  */
    /*  Parameters: */
    /*    x:              input channels [frame size x number of inputs] */
    /*    NumStages:      array of filter lengths per channel [NumChannels x 1] */
    /*    CoeffsTarget:   coefficient set used for the specific target [total sim filter lengths x 1] */
    /*   */
    /*  Global parameters: */
    /*    states: working memory set up during roommode reset/init CRL */
    /*                    function call [StatesSize x 1] */
    /* -------------------------------------------------------------------------- */
    /* MATLAB Function 'PoolIirWithReset/PoolIir/TargetSpecific/PoolIirProcess': '<S633>:1' */
    /*  Extract configuration information from inputs */
    /* '<S633>:1:15' [~, NumChannels] = size(x); */
    /*  Initialize variables */
    /* '<S633>:1:20' VariantStr = char(VariantUint8); */
    /* '<S633>:1:22' if(coder.target('MATLAB') || coder.target('Sfun')) */
    /* '<S633>:1:29' else */
    /* '<S633>:1:30' switch(VariantStr) */
    /* '<S633>:1:31' case{"Hexagon"} */
    /* '<S633>:1:32' y = pooliir_hexagon_process(single(x), single(states)); */
    pooliir_process((&(UnitTest_Target_AudioOut[0])),
                    &Model_Target_PostProcess_B.y[0],
                    &PostProcPostLimiterEq56836states[0]);

    /* End of Outputs for S-Function (RTC_REQ_RSP): '<S627>/RTC Request Response Trigger' */
    /* End of Outputs for S-Function (RTC_NTF): '<S627>/NTF Trigger' */
    /* End of Outputs for S-Function (RTC_NTF): '<S568>/NTF Trigger' */
    /* End of Outputs for S-Function (RTC_NTF): '<S569>/NTF Trigger' */

    /* Update for Delay: '<S579>/Delay2' incorporates:
     *  Sum: '<S579>/Add1'
     */
    memcpy(&Model_Target_PostProcess_DW.Delay2_DSTATE[0], &rtb_Add1_a[0], 24U *
           sizeof(real_T));

    /* Update for M-S-Function: '<S586>/TSP' */
    // TSP advanced triggered capture for PostProcessEnergyLimiterConfigComputedEnergy '<S586>/TSP':
#if TSP_ENABLE_ADVANCED

    TSP_StepCapture(TspDispatchTable0000, 4,
                    &Model_Target_PostProcess_B.DataTypeConversion1[0]);

#endif                                 //TSP_ENABLE_ADVANCED

    /* Update for Delay: '<S572>/Delay1' */
    Model_Target_PostProcess_DW.icLoad = false;
    Model_Target_PostProcess_DW.Delay1_DSTATE = rtb_Delay_ey;

    /* Update for M-S-Function: '<S587>/TSP' */
    // TSP advanced triggered capture for PostProcessEnergyLimiterConfigCurGain '<S587>/TSP':
#if TSP_ENABLE_ADVANCED

    TSP_StepCapture(TspDispatchTable0000, 5,
                    &Model_Target_PostProcess_B.CurrentGain);

#endif                                 //TSP_ENABLE_ADVANCED

    /* Update for M-S-Function: '<S588>/TSP' */
    // TSP advanced triggered capture for PostProcessEnergyLimiterConfigEnergyThreshold '<S588>/TSP':
#if TSP_ENABLE_ADVANCED

    TSP_StepCapture(TspDispatchTable0000, 8,
                    &Model_Target_PostProcess_ConstB.DataTypeConversion3);

#endif                                 //TSP_ENABLE_ADVANCED

    /* Update for Delay: '<S572>/Delay2' */
    Model_Target_PostProcess_DW.Delay2_DSTATE_n = rtb_Max_g;

    /* Update for Delay: '<S572>/Delay' */
    Model_Target_PostProcess_DW.Delay_DSTATE = rtb_DataStoreRead1_ov;

    /* Update for Delay: '<S572>/Delay3' incorporates:
     *  DataTypeConversion: '<S572>/Data Type Conversion4'
     */
    Model_Target_PostProcess_DW.Delay3_DSTATE = rtb_DataStoreRead1_f_tmp_tmp;

    /* Update for M-S-Function: '<S589>/TSP' */
    // TSP advanced triggered capture for PostProcessEnergyLimiterConfigCurState '<S589>/TSP':
#if TSP_ENABLE_ADVANCED

    TSP_StepCapture(TspDispatchTable0000, 6,
                    &Model_Target_PostProcess_B.DataTypeConversion5);

#endif                                 //TSP_ENABLE_ADVANCED

    /* Update for M-S-Function: '<S590>/TSP' */
    // TSP advanced triggered capture for PostProcessEnergyLimiterConfigEnableFlag '<S590>/TSP':
#if TSP_ENABLE_ADVANCED

    TSP_StepCapture(TspDispatchTable0000, 7,
                    &Model_Target_PostProcess_B.DataTypeConversion6);

#endif                                 //TSP_ENABLE_ADVANCED

}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
