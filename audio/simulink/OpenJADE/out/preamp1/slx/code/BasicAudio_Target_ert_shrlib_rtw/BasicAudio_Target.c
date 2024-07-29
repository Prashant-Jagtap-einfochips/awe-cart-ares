/*
 * File: BasicAudio_Target.c
 *
 * Code generated for Simulink model 'BasicAudio_Target'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:46:55 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "BasicAudio_Target.h"
#include "rtwtypes.h"
#include "BasicAudio_Target_private.h"
#include <math.h>
#include "BasicAudio_Target_types.h"
#include <string.h>

/* Block signals (default storage) */
B_BasicAudio_Target_T BasicAudio_Target_B;

/* Block states (default storage) */
DW_BasicAudio_Target_T BasicAudio_Target_DW;

/* Real-time model */
static RT_MODEL_BasicAudio_Target_T BasicAudio_Target_M_;
RT_MODEL_BasicAudio_Target_T *const BasicAudio_Target_M = &BasicAudio_Target_M_;

/* Forward declaration for local functions */
static void BasicAudio_Target_eml_find(const boolean_T x[2], int32_T i_data[],
    int32_T i_size[2]);

// BasicAudioVolumeSet RTC Request-Response Handler (RTC ID 0x10000004) '<S120>/RTC Request Response Trigger' ---------------------------
void RtcReqRsp_BasicAudioVolumeSet(void* pREQBuf, void* pRSPBuf)
{
    /* Output and update for function-call system: '<S120>/Request' */

    /* S-Function (RTC_BytePack): '<S132>/UnpackREQ' */
    BasicAudioVolumeSet_REQ_T* payloadREQ = (BasicAudioVolumeSet_REQ_T*)pREQBuf;
    BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_caller_handle =
        payloadREQ->caller_handle;
    BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_gain_index =
        payloadREQ->gain_index;
    BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_ramp_db_per_second =
        payloadREQ->ramp_db_per_second;
    BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_ramp_milliseconds =
        payloadREQ->ramp_milliseconds;
    BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_gain_offset =
        payloadREQ->gain_offset;
    (BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_sequence)++;// Increment sequence

    /* End of S-Function (RTC_BytePack) REQ unpacking */
    HandleReqBasicAudioVolumeSet();

    /* Output and update for function-call system: '<S120>/Response' */

    /* S-Function (RTC_BytePack): '<S133>/PackRSP' */
    BasicAudioVolumeSet_RSP_T* payloadRSP = (BasicAudioVolumeSet_RSP_T*)pRSPBuf;
    payloadRSP->caller_handle =
        BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_caller_handle;
    payloadRSP->status_code = 0;
    payloadRSP->gain_index =
        BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_gain_index;

    /* End of S-Function (RTC_BytePack) RSP packing */
}

// BasicAudioVolumeSet RTC NTF check complete '<S120>/NTF Trigger' ---------------------------------
void RtcNtfCheck_BasicAudioVolumeSet( void )
{
    /* Output and update for function-call system: '<S120>/NtfCheck' */

    /* MATLAB Function: '<S136>/rgainx check sequence' */
    /* MATLAB Function: '<S136>/rgainx check sequence' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'RTC/NtfCheck': '<S129>:1' */
    /* '<S129>:1:3' go(); */
    /*  Check if the ramping is done by comparing the target and current gains. */
    /* MATLAB Function 'Rgainy/NtfCheckSubsystem/rgainx check sequence': '<S147>:1' */
    /*  Assume the work is complete until proven otherwise */
    /* '<S147>:1:7' done = true; */
    BasicAudio_Target_B.done = true;

    /* '<S147>:1:8' for i=1:MaskNumRampers */
    /* '<S147>:1:9' if (Rampers(i).targetGain ~= Rampers(i).currentGain) */
    if (BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.targetGain !=
            BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.currentGain)
    {
        /* '<S147>:1:10' done = false; */
        BasicAudio_Target_B.done = false;

        /*  just proved otherwise */
    }

    /* End of MATLAB Function: '<S136>/rgainx check sequence' */

    /* Outputs for Function Call SubSystem: '<S118>/Triggered Subsystem' */
    /* SignalConversion generated from: '<S122>/In1' */
    BasicAudio_Target_B.In1 =
        BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_gain_index;

    /* End of Outputs for SubSystem: '<S118>/Triggered Subsystem' */
}

// BasicAudioVolumeSet RTC NTF message preparation and packing '<S120>/NTF Trigger' ---------------------------------
void RtcNtfPack_BasicAudioVolumeSet(void* pNTFBuf)
{
    // This message does not specify any special signal preparation
    // Package the notification signals

    /* Output and update for function-call system: '<S120>/Notification' */

    /* S-Function (RTC_BytePack): '<S128>/PackNTF' */
    BasicAudioVolumeSet_NTF_T* payloadNTF = (BasicAudioVolumeSet_NTF_T*)pNTFBuf;
    payloadNTF->caller_handle =
        BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_caller_handle;
    payloadNTF->status_code = 0;
    payloadNTF->gain_index = BasicAudio_Target_B.In1;

    /* End of S-Function (RTC_BytePack) NTF packing */
}

// BasicAudioBalanceSet RTC Request-Response Handler (RTC ID 0x10000000) '<S12>/RTC Request Response Trigger' ---------------------------
void RtcReqRsp_BasicAudioBalanceSet(void* pREQBuf, void* pRSPBuf)
{
    /* Output and update for function-call system: '<S12>/Request' */

    /* S-Function (RTC_BytePack): '<S31>/UnpackREQ' */
    BasicAudioBalanceSet_REQ_T* payloadREQ = (BasicAudioBalanceSet_REQ_T*)
        pREQBuf;
    BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_caller_handle =
        payloadREQ->caller_handle;
    BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_gain_index =
        payloadREQ->gain_index;
    BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_ramp_db_per_second =
        payloadREQ->ramp_db_per_second;
    BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_ramp_milliseconds =
        payloadREQ->ramp_milliseconds;
    (BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_sequence)++;// Increment sequence

    /* End of S-Function (RTC_BytePack) REQ unpacking */
    HandleReqBasicAudioBalanceSet();

    /* Output and update for function-call system: '<S12>/Response' */

    /* S-Function (RTC_BytePack): '<S32>/PackRSP' */
    BasicAudioBalanceSet_RSP_T* payloadRSP = (BasicAudioBalanceSet_RSP_T*)
        pRSPBuf;
    payloadRSP->caller_handle =
        BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_caller_handle;
    payloadRSP->status_code = 0;
    payloadRSP->gain_index =
        BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_gain_index;

    /* End of S-Function (RTC_BytePack) RSP packing */
}

// BasicAudioBalanceSet RTC NTF check complete '<S12>/NTF Trigger' ---------------------------------
void RtcNtfCheck_BasicAudioBalanceSet( void )
{
    /* Output and update for function-call system: '<S12>/NtfCheck' */
    {
        int32_T i;
        boolean_T exitg1;

        /* MATLAB Function: '<S11>/check sequence' */
        /* MATLAB Function: '<S11>/check sequence' */
        /*  Invoke the function trigger output */
        /* MATLAB Function 'RTC/NtfCheck': '<S28>:1' */
        /* '<S28>:1:3' go(); */
        /*  Function which checks if the notification sequence */
        /*  has completed successfully  */
        /* MATLAB Function 'BasicAudio/Balance/balance/InnerLink/check sequence': '<S25>:1' */
        /*  Assume the work is complete until proven otherwise */
        /* '<S25>:1:8' done = true; */
        BasicAudio_Target_B.done_p = true;

        /* '<S25>:1:10' for i=1:MaskNumRampers */
        i = 0;
        exitg1 = false;
        while ((!exitg1) && (i <= 3))
        {
            /* '<S25>:1:11' if (Rampers(i).targetGain ~= Rampers(i).currentGain) */
            if (BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[i].
                    targetGain !=
                    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[i].
                    currentGain)
            {
                /* '<S25>:1:12' done = false; */
                BasicAudio_Target_B.done_p = false;

                /*  just proved otherwise */
                exitg1 = true;
            }
            else
            {
                i++;
            }
        }

        /* Outputs for Function Call SubSystem: '<S11>/Triggered Subsystem' */
        /* SignalConversion generated from: '<S15>/In1' */
        BasicAudio_Target_B.In1_d =
            BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_gain_index;

        /* End of Outputs for SubSystem: '<S11>/Triggered Subsystem' */
    }
}

// BasicAudioBalanceSet RTC NTF message preparation and packing '<S12>/NTF Trigger' ---------------------------------
void RtcNtfPack_BasicAudioBalanceSet(void* pNTFBuf)
{
    // This message does not specify any special signal preparation
    // Package the notification signals

    /* Output and update for function-call system: '<S12>/Notification' */

    /* S-Function (RTC_BytePack): '<S27>/PackNTF' */
    BasicAudioBalanceSet_NTF_T* payloadNTF = (BasicAudioBalanceSet_NTF_T*)
        pNTFBuf;
    payloadNTF->caller_handle =
        BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_caller_handle;
    payloadNTF->status_code = 0;
    payloadNTF->gain_index = BasicAudio_Target_B.In1_d;

    /* End of S-Function (RTC_BytePack) NTF packing */
}

// BasicAudioMuteSet RTC Request-Response Handler (RTC ID 0x10000002) '<S69>/RTC Request Response Trigger' ---------------------------
void RtcReqRsp_BasicAudioMuteSet(void* pREQBuf, void* pRSPBuf)
{
    /* Output and update for function-call system: '<S69>/Request' */

    /* S-Function (RTC_BytePack): '<S77>/UnpackREQ' */
    BasicAudioMuteSet_REQ_T* payloadREQ = (BasicAudioMuteSet_REQ_T*)pREQBuf;
    BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_caller_handle =
        payloadREQ->caller_handle;
    BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_mute_switch =
        payloadREQ->mute_switch;
    BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_ramp_db_per_second =
        payloadREQ->ramp_db_per_second;
    BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_ramp_milliseconds =
        payloadREQ->ramp_milliseconds;
    (BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_sequence)++;// Increment sequence

    /* End of S-Function (RTC_BytePack) REQ unpacking */
    HandleReqBasicAudioMuteSet();

    /* Output and update for function-call system: '<S69>/Response' */

    /* S-Function (RTC_BytePack): '<S78>/PackRSP' */
    BasicAudioMuteSet_RSP_T* payloadRSP = (BasicAudioMuteSet_RSP_T*)pRSPBuf;
    payloadRSP->caller_handle =
        BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_caller_handle;
    payloadRSP->status_code = 0;
    payloadRSP->mute_switch =
        BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_mute_switch;

    /* End of S-Function (RTC_BytePack) RSP packing */
}

// BasicAudioMuteSet RTC NTF check complete '<S69>/NTF Trigger' ---------------------------------
void RtcNtfCheck_BasicAudioMuteSet( void )
{
    /* Output and update for function-call system: '<S69>/NtfCheck' */

    /* MATLAB Function: '<S68>/TriggeredFunction' */
    /* MATLAB Function: '<S68>/TriggeredFunction' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'RTC/NtfCheck': '<S74>:1' */
    /* '<S74>:1:3' go(); */
    /*  Function to check whether the ramp has completed.  */
    /*  If so, send out TRUE */
    /* MATLAB Function 'MuteRamper/TriggeredFunction': '<S80>:1' */
    /*  Assume the work is complete until proven otherwise */
    /* '<S80>:1:9' done = true; */
    BasicAudio_Target_B.done_e = true;

    /* '<S80>:1:10' if (targetLinearGain ~= currentGain) */
    if (BasicAudio_TargetControlDsms.BasicAudioMuteTargetLinearGain !=
            BasicAudio_TargetControlDsms.BasicAudioMuteCurrentGain)
    {
        /* '<S80>:1:11' done = false; */
        BasicAudio_Target_B.done_e = false;

        /*  just proved otherwise */
    }

    /* End of MATLAB Function: '<S68>/TriggeredFunction' */

    /* Outputs for Function Call SubSystem: '<S67>/Triggered Subsystem' */
    /* SignalConversion generated from: '<S70>/In1' */
    BasicAudio_Target_B.In1_c =
        BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_mute_switch;

    /* End of Outputs for SubSystem: '<S67>/Triggered Subsystem' */
}

// BasicAudioMuteSet RTC NTF message preparation and packing '<S69>/NTF Trigger' ---------------------------------
void RtcNtfPack_BasicAudioMuteSet(void* pNTFBuf)
{
    // This message does not specify any special signal preparation
    // Package the notification signals

    /* Output and update for function-call system: '<S69>/Notification' */

    /* S-Function (RTC_BytePack): '<S73>/PackNTF' */
    BasicAudioMuteSet_NTF_T* payloadNTF = (BasicAudioMuteSet_NTF_T*)pNTFBuf;
    payloadNTF->caller_handle =
        BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_caller_handle;
    payloadNTF->status_code = 0;
    payloadNTF->mute_switch = BasicAudio_Target_B.In1_c;

    /* End of S-Function (RTC_BytePack) NTF packing */
}

// BasicAudioBassSet RTC Request-Response Handler (RTC ID 0x10000001) '<S41>/RTC Request Response Trigger' ---------------------------
void RtcReqRsp_BasicAudioBassSet(void* pREQBuf, void* pRSPBuf)
{
    /* Output and update for function-call system: '<S41>/Request' */

    /* S-Function (RTC_BytePack): '<S57>/UnpackREQ' */
    BasicAudioBassSet_REQ_T* payloadREQ = (BasicAudioBassSet_REQ_T*)pREQBuf;
    BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_caller_handle =
        payloadREQ->caller_handle;
    BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_gain_index =
        payloadREQ->gain_index;
    BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_ramp_db_per_second =
        payloadREQ->ramp_db_per_second;
    BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_ramp_milliseconds =
        payloadREQ->ramp_milliseconds;
    (BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_sequence)++;// Increment sequence

    /* End of S-Function (RTC_BytePack) REQ unpacking */
    HandleReqBasicAudioBassSet();

    /* Output and update for function-call system: '<S41>/Response' */

    /* S-Function (RTC_BytePack): '<S58>/PackRSP' */
    BasicAudioBassSet_RSP_T* payloadRSP = (BasicAudioBassSet_RSP_T*)pRSPBuf;
    payloadRSP->caller_handle =
        BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_caller_handle;
    payloadRSP->status_code = 0;
    payloadRSP->gain_index =
        BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_gain_index;

    /* End of S-Function (RTC_BytePack) RSP packing */
}

// BasicAudioBassSet RTC NTF check complete '<S41>/NTF Trigger' ---------------------------------
void RtcNtfCheck_BasicAudioBassSet( void )
{
    /* Output and update for function-call system: '<S41>/NtfCheck' */

    /* MATLAB Function: '<S40>/check done' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'RTC/NtfCheck': '<S54>:1' */
    /* '<S54>:1:3' go(); */
    /* MATLAB Function 'BasicAudio/Bass/Tone/InnerLink/check done': '<S48>:1' */
    /*  Checking whether the frameCounts are exactly equal to 0 was leading to */
    /*  unpredictable behavior. Therefore, we now check whether or not the  */
    /*  frameCounts are less-than/equal-to 0. */
    /*  See SAMPLERATE-1196 & SAMPLERATE-1205 for more details.  */
    /* '<S48>:1:10' done = (Rampers(1).frameCount <= 0) && (Rampers(2).frameCount <= 0); */
    BasicAudio_Target_B.done_m =
        ((BasicAudio_TargetControlDsms.BasicAudioBassRampers[0].frameCount <= 0)
         && (BasicAudio_TargetControlDsms.BasicAudioBassRampers[1].frameCount <=
             0));

    /* Outputs for Function Call SubSystem: '<S40>/Triggered Subsystem' */
    /* SignalConversion generated from: '<S44>/In1' */
    BasicAudio_Target_B.In1_h =
        BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_gain_index;

    /* End of Outputs for SubSystem: '<S40>/Triggered Subsystem' */
}

// BasicAudioBassSet RTC NTF message preparation and packing '<S41>/NTF Trigger' ---------------------------------
void RtcNtfPack_BasicAudioBassSet(void* pNTFBuf)
{
    // This message does not specify any special signal preparation
    // Package the notification signals

    /* Output and update for function-call system: '<S41>/Notification' */

    /* S-Function (RTC_BytePack): '<S53>/PackNTF' */
    BasicAudioBassSet_NTF_T* payloadNTF = (BasicAudioBassSet_NTF_T*)pNTFBuf;
    payloadNTF->caller_handle =
        BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_caller_handle;
    payloadNTF->status_code = 0;
    payloadNTF->gain_index = BasicAudio_Target_B.In1_h;

    /* End of S-Function (RTC_BytePack) NTF packing */
}

// BasicAudioTrebleSet RTC Request-Response Handler (RTC ID 0x10000003) '<S93>/RTC Request Response Trigger' ---------------------------
void RtcReqRsp_BasicAudioTrebleSet(void* pREQBuf, void* pRSPBuf)
{
    /* Output and update for function-call system: '<S93>/Request' */

    /* S-Function (RTC_BytePack): '<S109>/UnpackREQ' */
    BasicAudioTrebleSet_REQ_T* payloadREQ = (BasicAudioTrebleSet_REQ_T*)pREQBuf;
    BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_caller_handle =
        payloadREQ->caller_handle;
    BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_gain_index =
        payloadREQ->gain_index;
    BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_ramp_db_per_second =
        payloadREQ->ramp_db_per_second;
    BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_ramp_milliseconds =
        payloadREQ->ramp_milliseconds;
    (BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_sequence)++;// Increment sequence

    /* End of S-Function (RTC_BytePack) REQ unpacking */
    HandleReqBasicAudioTrebleSet();

    /* Output and update for function-call system: '<S93>/Response' */

    /* S-Function (RTC_BytePack): '<S110>/PackRSP' */
    BasicAudioTrebleSet_RSP_T* payloadRSP = (BasicAudioTrebleSet_RSP_T*)pRSPBuf;
    payloadRSP->caller_handle =
        BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_caller_handle;
    payloadRSP->status_code = 0;
    payloadRSP->gain_index =
        BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_gain_index;

    /* End of S-Function (RTC_BytePack) RSP packing */
}

// BasicAudioTrebleSet RTC NTF check complete '<S93>/NTF Trigger' ---------------------------------
void RtcNtfCheck_BasicAudioTrebleSet( void )
{
    /* Output and update for function-call system: '<S93>/NtfCheck' */

    /* MATLAB Function: '<S92>/check done' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'RTC/NtfCheck': '<S106>:1' */
    /* '<S106>:1:3' go(); */
    /* MATLAB Function 'BasicAudio/Treble/Tone/InnerLink/check done': '<S100>:1' */
    /*  Checking whether the frameCounts are exactly equal to 0 was leading to */
    /*  unpredictable behavior. Therefore, we now check whether or not the  */
    /*  frameCounts are less-than/equal-to 0. */
    /*  See SAMPLERATE-1196 & SAMPLERATE-1205 for more details.  */
    /* '<S100>:1:10' done = (Rampers(1).frameCount <= 0) && (Rampers(2).frameCount <= 0); */
    BasicAudio_Target_B.done_l =
        ((BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[0].frameCount <=
          0) && (BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[1].
                 frameCount <= 0));

    /* Outputs for Function Call SubSystem: '<S92>/Triggered Subsystem' */
    /* SignalConversion generated from: '<S96>/In1' */
    BasicAudio_Target_B.In1_n =
        BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_gain_index;

    /* End of Outputs for SubSystem: '<S92>/Triggered Subsystem' */
}

// BasicAudioTrebleSet RTC NTF message preparation and packing '<S93>/NTF Trigger' ---------------------------------
void RtcNtfPack_BasicAudioTrebleSet(void* pNTFBuf)
{
    // This message does not specify any special signal preparation
    // Package the notification signals

    /* Output and update for function-call system: '<S93>/Notification' */

    /* S-Function (RTC_BytePack): '<S105>/PackNTF' */
    BasicAudioTrebleSet_NTF_T* payloadNTF = (BasicAudioTrebleSet_NTF_T*)pNTFBuf;
    payloadNTF->caller_handle =
        BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_caller_handle;
    payloadNTF->status_code = 0;
    payloadNTF->gain_index = BasicAudio_Target_B.In1_n;

    /* End of S-Function (RTC_BytePack) NTF packing */
}

/*
 * Output and update for function-call system:
 *    '<S13>/LockEnter'
 *    '<S14>/LockEnter'
 *    '<S42>/LockEnter'
 *    '<S43>/LockEnter'
 *    '<S94>/LockEnter'
 *    '<S95>/LockEnter'
 *    '<S138>/LockEnter'
 *    '<S139>/LockEnter'
 */
void BasicAudio_Target_LockEnter(void)
{
    /* M-S-Function: '<S34>/InsertLockCode' */
    TALARIA_LOCK_ENTER(-1);
}

/*
 * Output and update for function-call system:
 *    '<S13>/LockExit'
 *    '<S14>/LockExit'
 *    '<S42>/LockExit'
 *    '<S43>/LockExit'
 *    '<S94>/LockExit'
 *    '<S95>/LockExit'
 *    '<S138>/LockExit'
 *    '<S139>/LockExit'
 */
void BasicAudio_Target_LockExit(void)
{
    /* M-S-Function: '<S35>/InsertLockCode' */
    TALARIA_LOCK_EXIT(-1);
}

/* Output and update for function-call system: '<S11>/SetDsm' */
void BasicAudio_Target_SetDsm(void)
{
    /* S-Function (fcgen): '<S13>/Function-Call Generator' incorporates:
     *  SubSystem: '<S13>/LockEnter'
     */
    BasicAudio_Target_LockEnter();

    /* S-Function (fcgen): '<S13>/Function-Call Generator' incorporates:
     *  SubSystem: '<S13>/DsmWrite'
     */
    /* DataStoreWrite: '<S33>/Data Store Write' incorporates:
     *  Constant: '<S33>/Constant'
     */
    BasicAudio_TargetControlDsms.BasicAudioBalanceFreeze = 1U;

    /* S-Function (fcgen): '<S13>/Function-Call Generator' incorporates:
     *  SubSystem: '<S13>/LockExit'
     */
    BasicAudio_Target_LockExit();

    /* End of Outputs for S-Function (fcgen): '<S13>/Function-Call Generator' */
}

/* Output and update for function-call system: '<S11>/control' */
void BasicAudio_Target_control(const real32_T rtu_targetGainLinear[4], real32_T
    rtu_rampdbPerSec, real32_T rtu_rampTimesMs, real32_T rtu_sampleRateInHertz,
    real32_T rtu_frameSize)
{
    real32_T currentGainLinear_idx_0;
    real32_T currentGainLinear_idx_1;
    real32_T currentGainLinear_idx_2;
    real32_T currentGainLinear_idx_3;
    real32_T diff_idx_0;
    real32_T diff_idx_1;
    real32_T diff_idx_2;
    real32_T diff_idx_3;
    real32_T quantumMs;
    real32_T targetGainLinear;
    real32_T targetGainLinear_0;
    real32_T targetGainLinear_1;
    real32_T targetGainLinear_2;

    /*  % inputs */
    /*  parameters */
    /* MATLAB Function 'BasicAudio/Balance/balance/InnerLink/control': '<S26>:1' */
    /*  CONTROL LOGIC: */
    /*  Shift of 'balance' between the 'Right' and 'Left' channel groups is */
    /*  achieved by multiplying the corresponding 'cut' or 'boost' gain. As the */
    /*  index increases from 0, the balance shifts from left to right. Channels */
    /*  corresponding to the 'center' group are not modified.  */
    /*  rampdbPerSec takes precidence: */
    /*  If dbPerSec has a non-zero value then any ramp_times_ms values are */
    /*  ignored and the dbPerSec is used to determine ramper timing. */
    /*  Inputs: */
    /*    targetGainLinear: Indicates the gain (in linear scale) which needs to */
    /*        be applied to the audio. Gain values can be mapped to the ramper */
    /*        configuration. */
    /*    rampTimeMs: Indicates the time in milliseconds in which the gain of the */
    /*        rampers must change from their current to target values. */
    /*    rampdbPerSecond: Indicates the rate in dB/second at which the gain of the */
    /*        rampers must change from their current to target values. If this */
    /*        value is non-zero, it is used to compute the ramp time and */
    /*        'rampTimeMs' is discarded (if specified). */
    /*    rgain_SILENT_GAIN: Parameter defined in the initFcn callback. */
    /*  Control Data: */
    /*    Freeze(1) freezes the processing during this control operation */
    /*    Rampers(1) is the ramper for the 'Left' group. Boost vs. cut  */
    /*         is determined by the value of the balance index. */
    /*    Rampers(2) is the ramper for the 'Right' group. Boost vs. cut is determined by */
    /*         the value of the balance index. */
    /*    Rampers(3) is the ramper for the 'Center' group. Boost vs. cut is determined by */
    /*         the value of the balance index. */
    /*    Rampers(4) is the ramper for the 'Mono' group (supporting "Sleeping */
    /*        Beauty Mutes Bass"). Boost vs. cut is determined by the value of  */
    /*        the balance index. */
    /*  Step 1: FREEZE the gain ramper so that we can perform calculations based */
    /*  on the current gain value. (This step is done outside on canvas now) */
    /*  This block replaces much of rgainy, but uses its own silent gain value. */
    /*  The reason why is not documented, but it does impact all the test cases. */
    /*  Use this special value instead of RgainBusUtil.GetSilentGainLinear */
    /* '<S26>:1:46' rgain_SILENT_GAIN = single(10^(-126/20)); */
    /*  Step 2: Nudge the gain values a bit if they are sitting at zero. */
    /*  When this control set goes live, ensure that the current gain will move. */
    /*  If current gain is zero, replace with a value very close to zero. What we */
    /*  really want is a value which is on the edge of perception - not too small */
    /*  or large. If the value is too small, we'll waste some of our ramp time */
    /*  in perceived silence. If the value is too large, the audio will lurch */
    /*  into perceptability at the start of the ramp. */
    /*  UPDATE: This will now loop over 4 rampers */
    /* '<S26>:1:57' currentGainLinear = zeros(1,MaskNumRampers,'single'); */
    /* '<S26>:1:58' for i = 1:MaskNumRampers */
    /* '<S26>:1:59' currentGainLinear(i) = max(Rampers(i).currentGain, rgain_SILENT_GAIN); */
    currentGainLinear_idx_0 = fmaxf
        (BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[0].currentGain,
         5.0118723E-7F);

    /* '<S26>:1:60' targetGainLinear(i) = max(targetGainLinear(i), rgain_SILENT_GAIN); */
    targetGainLinear = fmaxf(rtu_targetGainLinear[0], 5.0118723E-7F);

    /* '<S26>:1:59' currentGainLinear(i) = max(Rampers(i).currentGain, rgain_SILENT_GAIN); */
    currentGainLinear_idx_1 = fmaxf
        (BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[1].currentGain,
         5.0118723E-7F);

    /* '<S26>:1:60' targetGainLinear(i) = max(targetGainLinear(i), rgain_SILENT_GAIN); */
    targetGainLinear_0 = fmaxf(rtu_targetGainLinear[1], 5.0118723E-7F);

    /* '<S26>:1:59' currentGainLinear(i) = max(Rampers(i).currentGain, rgain_SILENT_GAIN); */
    currentGainLinear_idx_2 = fmaxf
        (BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[2].currentGain,
         5.0118723E-7F);

    /* '<S26>:1:60' targetGainLinear(i) = max(targetGainLinear(i), rgain_SILENT_GAIN); */
    targetGainLinear_1 = fmaxf(rtu_targetGainLinear[2], 5.0118723E-7F);

    /* '<S26>:1:59' currentGainLinear(i) = max(Rampers(i).currentGain, rgain_SILENT_GAIN); */
    currentGainLinear_idx_3 = fmaxf
        (BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[3].currentGain,
         5.0118723E-7F);

    /* '<S26>:1:60' targetGainLinear(i) = max(targetGainLinear(i), rgain_SILENT_GAIN); */
    targetGainLinear_2 = fmaxf(rtu_targetGainLinear[3], 5.0118723E-7F);

    /*  Step 3a: If a slope in dB/second is provided, ignore the ramp time */
    /*  and instead calculate a new ramp time that matches the slope. */
    /* '<S26>:1:65' rampMs = zeros(1,MaskNumRampers,'single'); */
    /* '<S26>:1:66' if (rampdbPerSec) */
    if (rtu_rampdbPerSec != 0.0F)
    {
        /* '<S26>:1:67' currentGainDb = 20*log10(currentGainLinear); */
        /* '<S26>:1:68' targetGainDb = 20*log10(targetGainLinear); */
        /* '<S26>:1:69' diff = abs(currentGainDb - targetGainDb); */
        /* '<S26>:1:70' rampMs = diff / single(rampdbPerSec) * 1000; */
        diff_idx_0 = fabsf(20.0F * log10f(currentGainLinear_idx_0) - 20.0F *
                           log10f(targetGainLinear)) / rtu_rampdbPerSec *
            1000.0F;
        diff_idx_1 = fabsf(20.0F * log10f(currentGainLinear_idx_1) - 20.0F *
                           log10f(targetGainLinear_0)) / rtu_rampdbPerSec *
            1000.0F;
        diff_idx_2 = fabsf(20.0F * log10f(currentGainLinear_idx_2) - 20.0F *
                           log10f(targetGainLinear_1)) / rtu_rampdbPerSec *
            1000.0F;
        diff_idx_3 = fabsf(20.0F * log10f(currentGainLinear_idx_3) - 20.0F *
                           log10f(targetGainLinear_2)) / rtu_rampdbPerSec *
            1000.0F;
    }
    else
    {
        /* '<S26>:1:71' else */
        /* '<S26>:1:72' rampMs(:) = single(rampTimesMs); */
        diff_idx_0 = rtu_rampTimesMs;
        diff_idx_1 = rtu_rampTimesMs;
        diff_idx_2 = rtu_rampTimesMs;
        diff_idx_3 = rtu_rampTimesMs;
    }

    /*  Step 3b: QUANTIZE the desired ramp time to our FrameSize. Also retrieve  */
    /*  the whole number of frames required to best match requested ramp time. */
    /*  Ensure that at least one frame is used. The frame size will therefore */
    /*  determine the steepest ramp (i.e. within one frame you can go anywhere). */
    /* '<S26>:1:79' quantumMs = 1000 * frameSize / sampleRateInHertz; */
    quantumMs = 1000.0F * rtu_frameSize / rtu_sampleRateInHertz;

    /* '<S26>:1:80' numFrames = rampMs / quantumMs; */
    /* '<S26>:1:81' numFrames = max (1, ceil(numFrames)); */
    /* '<S26>:1:82' quantizedRampTimeMs = numFrames * quantumMs; */
    /*  UPDATE: Loop over 4 rampers now (L, R, C, and Mono) */
    /* '<S26>:1:85' for ramper=1:MaskNumRampers */
    diff_idx_0 = fmaxf(1.0F, ceilf(diff_idx_0 / quantumMs));

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
    /* '<S26>:1:98' factor = log( targetGainLinear(ramper) / currentGainLinear(ramper) ); */
    /* '<S26>:1:99' RampCoef = double(exp(factor / (sampleRateInHertz * quantizedRampTimeMs(ramper) / 1000))); */
    /*  Output control signals */
    /* '<S26>:1:102' Rampers(ramper).frameCount  = int32(numFrames(ramper)); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[0].frameCount =
        (int32_T)diff_idx_0;

    /* '<S26>:1:103' Rampers(ramper).targetGain  = targetGainLinear(ramper); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[0].targetGain =
        targetGainLinear;

    /* '<S26>:1:104' Rampers(ramper).rampCoeff = single(RampCoef); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[0].rampCoeff = expf
        (logf(targetGainLinear / currentGainLinear_idx_0) / (diff_idx_0 *
          quantumMs * rtu_sampleRateInHertz / 1000.0F));

    /* '<S26>:1:105' Rampers(ramper).currentGain = currentGainLinear(ramper); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[0].currentGain =
        currentGainLinear_idx_0;
    diff_idx_0 = fmaxf(1.0F, ceilf(diff_idx_1 / quantumMs));

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
    /* '<S26>:1:98' factor = log( targetGainLinear(ramper) / currentGainLinear(ramper) ); */
    /* '<S26>:1:99' RampCoef = double(exp(factor / (sampleRateInHertz * quantizedRampTimeMs(ramper) / 1000))); */
    /*  Output control signals */
    /* '<S26>:1:102' Rampers(ramper).frameCount  = int32(numFrames(ramper)); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[1].frameCount =
        (int32_T)diff_idx_0;

    /* '<S26>:1:103' Rampers(ramper).targetGain  = targetGainLinear(ramper); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[1].targetGain =
        targetGainLinear_0;

    /* '<S26>:1:104' Rampers(ramper).rampCoeff = single(RampCoef); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[1].rampCoeff = expf
        (logf(targetGainLinear_0 / currentGainLinear_idx_1) / (diff_idx_0 *
          quantumMs * rtu_sampleRateInHertz / 1000.0F));

    /* '<S26>:1:105' Rampers(ramper).currentGain = currentGainLinear(ramper); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[1].currentGain =
        currentGainLinear_idx_1;
    diff_idx_0 = fmaxf(1.0F, ceilf(diff_idx_2 / quantumMs));

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
    /* '<S26>:1:98' factor = log( targetGainLinear(ramper) / currentGainLinear(ramper) ); */
    /* '<S26>:1:99' RampCoef = double(exp(factor / (sampleRateInHertz * quantizedRampTimeMs(ramper) / 1000))); */
    /*  Output control signals */
    /* '<S26>:1:102' Rampers(ramper).frameCount  = int32(numFrames(ramper)); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[2].frameCount =
        (int32_T)diff_idx_0;

    /* '<S26>:1:103' Rampers(ramper).targetGain  = targetGainLinear(ramper); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[2].targetGain =
        targetGainLinear_1;

    /* '<S26>:1:104' Rampers(ramper).rampCoeff = single(RampCoef); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[2].rampCoeff = expf
        (logf(targetGainLinear_1 / currentGainLinear_idx_2) / (diff_idx_0 *
          quantumMs * rtu_sampleRateInHertz / 1000.0F));

    /* '<S26>:1:105' Rampers(ramper).currentGain = currentGainLinear(ramper); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[2].currentGain =
        currentGainLinear_idx_2;
    diff_idx_0 = fmaxf(1.0F, ceilf(diff_idx_3 / quantumMs));

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
    /* '<S26>:1:98' factor = log( targetGainLinear(ramper) / currentGainLinear(ramper) ); */
    /* '<S26>:1:99' RampCoef = double(exp(factor / (sampleRateInHertz * quantizedRampTimeMs(ramper) / 1000))); */
    /*  Output control signals */
    /* '<S26>:1:102' Rampers(ramper).frameCount  = int32(numFrames(ramper)); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[3].frameCount =
        (int32_T)diff_idx_0;

    /* '<S26>:1:103' Rampers(ramper).targetGain  = targetGainLinear(ramper); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[3].targetGain =
        targetGainLinear_2;

    /* '<S26>:1:104' Rampers(ramper).rampCoeff = single(RampCoef); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[3].rampCoeff = expf
        (logf(targetGainLinear_2 / currentGainLinear_idx_3) / (diff_idx_0 *
          quantumMs * rtu_sampleRateInHertz / 1000.0F));

    /* '<S26>:1:105' Rampers(ramper).currentGain = currentGainLinear(ramper); */
    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[3].currentGain =
        currentGainLinear_idx_3;

    /*  unfreeze the gain (This step is done outside on canvas now) */
}

/* Output and update for function-call system: '<S11>/SetDsm1' */
void BasicAudio_Target_SetDsm1(void)
{
    /* S-Function (fcgen): '<S14>/Function-Call Generator' incorporates:
     *  SubSystem: '<S14>/LockEnter'
     */
    BasicAudio_Target_LockEnter();

    /* S-Function (fcgen): '<S14>/Function-Call Generator' incorporates:
     *  SubSystem: '<S14>/DsmWrite'
     */
    /* DataStoreWrite: '<S36>/Data Store Write' incorporates:
     *  Constant: '<S36>/Constant'
     */
    BasicAudio_TargetControlDsms.BasicAudioBalanceFreeze = 0U;

    /* S-Function (fcgen): '<S14>/Function-Call Generator' incorporates:
     *  SubSystem: '<S14>/LockExit'
     */
    BasicAudio_Target_LockExit();

    /* End of Outputs for S-Function (fcgen): '<S14>/Function-Call Generator' */
}

/* Output and update for function-call system: '<S12>/REQ' */
void HandleReqBasicAudioBalanceSet(void)
{
    real32_T targetGains[4];
    real32_T boost_linear;
    real32_T cut_linear;
    real32_T left;
    uint8_T j;
    boolean_T exitg1;

    /* MATLAB Function: '<S11>/calculate_balance_gain' */
    /* MATLAB Function: '<S11>/calculate_balance_gain' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'RTC/REQ': '<S30>:1' */
    /* '<S30>:1:3' go(); */
    /*  Computes the balance gains for the 'left' and 'right' channel groups.  */
    /*  Based on the position of the balance index in 'balanceTableIdx', the  */
    /*  corresponding 'cut' gain is computed from the 'balanceTableDb'. If the  */
    /*  index lies between two entries in the table, the gain is computed by  */
    /*  linear interpolation between the corresponding gain table entries.  */
    /*  The 'boost' gain is computed based from the 'cut' gain depending on the */
    /*  value of the tune variable, 'boostDisable'. If set to '1', boost is 0 dB  */
    /*  and if '0', in order to preserve energy, the boost is computed based on */
    /*  the following equation: Boost^2 + Cut^2 = 2.  */
    /*  If 'silent extreme' is enabled and the index at either extreme is */
    /*  requested, channels belonging to the group which receives a gain 'cut' */
    /*  are muted. */
    /* MATLAB Function 'BasicAudio/Balance/balance/InnerLink/calculate_balance_gain': '<S23>:1' */
    /* '<S23>:1:18' j = uint8(1); */
    j = 1U;

    /* '<S23>:1:19' cut_linear = single(0.0); */
    cut_linear = 0.0F;

    /* '<S23>:1:21' targetGains = coder.nullcopy(ones(1,MaskNumRampers,'single')); */
    /* '<S23>:1:22' balanceIndex = single(balanceIndex); */
    /*  random comment */
    /*  Find the left and the right gains */
    /*  Based on the balance table, the dB value of the index is calculated */
    /* '<S23>:1:28' while (j <= numel(balanceTableIdx)) */
    exitg1 = false;
    while ((!exitg1) && (j <= 30))
    {
        /* '<S23>:1:29' if (balanceIndex <= balanceTableIdx(j)) */
        boost_linear = BasicAudio_Target_BasicAudio0.BalanceTableIdx[j - 1];
        if (BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_gain_index <=
                boost_linear)
        {
            /* '<S23>:1:30' if (j <= 1) */
            if (j <= 1)
            {
                /*  first indexed value is linearly interpolated to zero */
                /* '<S23>:1:31' if (balanceTableIdx(j)) */
                if (BasicAudio_Target_BasicAudio0.BalanceTableIdx[0] != 0.0F)
                {
                    /* '<S23>:1:32' percent = balanceIndex / balanceTableIdx(j); */
                    cut_linear = (real32_T)
                        BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_gain_index
                        / BasicAudio_Target_BasicAudio0.BalanceTableIdx[0];
                }
                else
                {
                    /* '<S23>:1:33' else */
                    /* '<S23>:1:34' percent = single(1); */
                    cut_linear = 1.0F;
                }

                /* '<S23>:1:36' cut_linear = percent * 10 ^ (balanceTableDb(j) / 20); */
                cut_linear *= powf(10.0F,
                                   BasicAudio_Target_BasicAudio0.BalanceTableDb
                                   [0] / 20.0F);
            }
            else
            {
                /* '<S23>:1:37' else */
                /*  other values are interpolated in db space */
                /* '<S23>:1:38' lastIdx = single(balanceTableIdx(j-1)); */
                /* '<S23>:1:39' percent = (balanceIndex - lastIdx) / (balanceTableIdx(j) - lastIdx); */
                /* '<S23>:1:40' cut_linear = 10 ^ (((percent * (balanceTableDb(j) - balanceTableDb(j-1))) + balanceTableDb(j-1)) / 20); */
                cut_linear = BasicAudio_Target_BasicAudio0.BalanceTableIdx[j - 2];
                left = BasicAudio_Target_BasicAudio0.BalanceTableDb[j - 2];
                cut_linear = powf(10.0F, (((real32_T)
                                    BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_gain_index
                                    - cut_linear) / (boost_linear - cut_linear) *
                                   (BasicAudio_Target_BasicAudio0.BalanceTableDb[
                                    j - 1] - left) + left) / 20.0F);
            }

            exitg1 = true;
        }
        else
        {
            /* '<S23>:1:43' else */
            /* '<S23>:1:44' j = j + 1; */
            j++;
        }
    }

    /*  Boost of > 0 dB is dependant on tuning. If the tune variable is SET (1), */
    /*  boost of 0 dB is applied (i.e. linear value of 1). If the tune variable */
    /*  is RESET (0), a boost of 3 dB is applied */
    /* '<S23>:1:51' if boostDisable == 0 */
    if (BasicAudio_Target_BasicAudio0.BalanceBoostDisable == 0.0F)
    {
        /* '<S23>:1:52' boost_linear = sqrt(2.0 - cut_linear^2); */
        boost_linear = sqrtf(2.0F - cut_linear * cut_linear);
    }
    else
    {
        /* '<S23>:1:53' else */
        /* '<S23>:1:54' boost_linear = single(1); */
        boost_linear = 1.0F;
    }

    /*  % if silent extreme is enabled and we're extreme, make the cut complete */
    /*  % (Sleeping Beauty) */
    /* '<S23>:1:59' if (EnableSilentExtreme && abs(balanceIndex - indexOffset) >= (indexOffset-1)) */
    if ((BasicAudio_Target_BasicAudio0.BalanceEnableSilentExtreme != 0.0F) &&
            (fabsf((real32_T)
                   BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_gain_index -
                   BasicAudio_Target_BasicAudio0.BalanceOffset) >=
             BasicAudio_Target_BasicAudio0.BalanceOffset - 1.0F))
    {
        /* '<S23>:1:60' cut_linear = single(0); */
        cut_linear = 0.0F;
    }

    /*  Assign left and right Gain Values */
    /* '<S23>:1:64' if (balanceIndex - indexOffset) > 0 */
    if ((real32_T)BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_gain_index -
            BasicAudio_Target_BasicAudio0.BalanceOffset > 0.0F)
    {
        /*  Left speakers attenuated */
        /* '<S23>:1:65' left = single(cut_linear); */
        left = cut_linear;

        /* '<S23>:1:66' right = single(boost_linear); */
        cut_linear = boost_linear;
    }
    else
    {
        /* '<S23>:1:67' else */
        /* '<S23>:1:68' left = single(boost_linear); */
        left = boost_linear;

        /* '<S23>:1:69' right = single(cut_linear); */
    }

    /*  Assign remaining gain values  */
    /* '<S23>:1:73' center = (left + right) * 0.5; */
    boost_linear = (left + cut_linear) * 0.5F;

    /* '<S23>:1:74' mono = center; */
    /*  Number to Content Key */
    /* '<S23>:1:77' targetGains(1) = left; */
    targetGains[0] = left;

    /* '<S23>:1:78' targetGains(2) = right; */
    targetGains[1] = cut_linear;

    /* '<S23>:1:79' targetGains(3) = center; */
    targetGains[2] = boost_linear;

    /* '<S23>:1:80' targetGains(4) = mono; */
    targetGains[3] = boost_linear;

    /*  (Sleeping Beauty) */
    /*  if silent extreme is enabled and we're extreme, cut center freqs */
    /* '<S23>:1:84' if (EnableSilentExtreme && abs(balanceIndex - indexOffset) >= (indexOffset-1)) */
    if ((BasicAudio_Target_BasicAudio0.BalanceEnableSilentExtreme != 0.0F) &&
            (fabsf((real32_T)
                   BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_gain_index -
                   BasicAudio_Target_BasicAudio0.BalanceOffset) >=
             BasicAudio_Target_BasicAudio0.BalanceOffset - 1.0F))
    {
        /* '<S23>:1:85' targetGains(3) = 0; */
        targetGains[2] = 0.0F;

        /* '<S23>:1:86' if (SleepingBeautyMutesBass) */
        if (BasicAudio_Target_BasicAudio0.BalanceSleepingBeautyMutesBass != 0.0F)
        {
            /* '<S23>:1:87' targetGains(4) = 0; */
            targetGains[3] = 0.0F;
        }
    }

    /* End of MATLAB Function: '<S11>/calculate_balance_gain' */

    /* MATLAB Function: '<S11>/calculate_ramp_parameters' */
    /* MATLAB Function: '<S11>/calculate_ramp_parameters' */
    cut_linear = BasicAudio_Target_BasicAudio0.BalanceRampTime;

    /*  % inputs */
    /*  parameters */
    /* MATLAB Function 'BasicAudio/Balance/balance/InnerLink/calculate_ramp_parameters': '<S24>:1' */
    /*  Ramp time and rate are provided to the block through RTC messages. However  */
    /*  in case neither of them are provided, the block will use the default ramp  */
    /*  time given in the tune variables. */
    /* '<S24>:1:9' ramptime = single(rampTimeTune); */
    /* '<S24>:1:10' rampRate = single(0); */
    left = 0.0F;

    /* '<S24>:1:12' if (rampRateRTC) */
    if (BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_ramp_db_per_second != 0U)
    {
        /* '<S24>:1:13' rampRate = single(rampRateRTC); */
        /* '<S24>:1:14' rampRate = max(rampRateBounds(1), rampRate); */
        /* '<S24>:1:15' rampRate = min(rampRateBounds(2), rampRate); */
        left = fminf(50000.0F, fmaxf(5.0F, (real32_T)
                      BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_ramp_db_per_second));
    }

    /* '<S24>:1:17' if (rampTimeRTC) */
    if (BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_ramp_milliseconds != 0U)
    {
        /* '<S24>:1:18' ramptime = single(rampTimeRTC); */
        cut_linear = (real32_T)
            BasicAudio_TargetBasicAudioBalanceSetRtcOut.REQ_ramp_milliseconds;
    }

    /* Outputs for Function Call SubSystem: '<S11>/SetDsm' */
    /* '<S24>:1:20' ramptime = max(rampTimeBounds(1), ramptime); */
    /* '<S24>:1:21' ramptime = min(rampTimeBounds(2), ramptime); */
    BasicAudio_Target_SetDsm();

    /* End of Outputs for SubSystem: '<S11>/SetDsm' */

    /* MATLAB Function: '<S11>/control' */
    /* MATLAB Function: '<S11>/calculate_ramp_parameters' */
    /* MATLAB Function: '<S11>/control' incorporates:
     *  MATLAB Function: '<S11>/calculate_ramp_parameters'
     *  S-Function (AudioStats_MEX): '<S11>/Audio Stats1'
     */
    BasicAudio_Target_control(targetGains, left, fminf(3000.0F, fmaxf(1.0F,
        cut_linear)), 48000.0, 32.0);

    /* Outputs for Function Call SubSystem: '<S11>/SetDsm1' */
    BasicAudio_Target_SetDsm1();

    /* End of Outputs for SubSystem: '<S11>/SetDsm1' */
}

/*
 * Output and update for function-call system:
 *    '<S40>/index to linear'
 *    '<S92>/index to linear'
 */
void BasicAudio_Target_indextolinear(real32_T rtu_scale, real32_T rtu_offset,
    uint32_T rtu_index, B_indextolinear_BasicAudio_Target_T *localB)
{
    /*  The midrange curve is configured as a piecewise linear plot, with */
    /*  configurable values of the maximum boost ('maxB') and maximum cut ('maxC') */
    /*  in dB.  */
    /*     dB ^ */
    /*        | */
    /*  maxB  ----------/ */
    /*        |        / */
    /*        |       / */
    /*   0    ------|---|----> index */
    /*        |   /128  255 */
    /*        |  / */
    /*  maxC  --/ */
    /*        | */
    /*  Hence, for any given index, the gain is calculated as a value in  */
    /*  between 0 and 1 for boost and 0 and -1 for cut. This will later be */
    /*  multiplied with the maxB or maxC to get the actual gain value.  */
    /*  (y - y1) / (x - x1) = (y1 - y2) / (x1 - x2) */
    /*  (y - 0) / (x - 128) = (0 - 1) / (128 - 255) */
    /*                    y = (x - 128)/127 */
    /*  'offset' represents the center point between boost and cut (i.e. 128) and */
    /*  'scale' represents how many intervals are available for boost/cut (i.e. 127) */
    /* MATLAB Function 'BasicAudio/Bass/Tone/InnerLink/index to linear': '<S49>:1' */
    /*  Apply the tuning calibration */
    /* '<S49>:1:27' floatIndex = single(index); */
    /* '<S49>:1:28' linearGain = single((floatIndex - offset) / scale); */
    localB->linearGain = ((real32_T)rtu_index - rtu_offset) / rtu_scale;

    /*  Ensure the result is between -1 and 1 */
    /* '<S49>:1:31' linearGain = min(linearGain, 1); */
    localB->linearGain = fminf(localB->linearGain, 1.0F);

    /* '<S49>:1:32' linearGain = max(linearGain, -1); */
    localB->linearGain = fmaxf(localB->linearGain, -1.0F);
}

/*
 * Output and update for function-call system:
 *    '<S40>/ramp parameters'
 *    '<S92>/ramp parameters'
 */
void BasicAudio_Target_rampparameters(real32_T rtu_data, uint32_T
    rtu_rampTimeRTC, uint32_T rtu_rampRateRTC, const real_T rtp_rampRateBounds[2],
    const real_T rtp_rampTimeBounds[2], B_rampparameters_BasicAudio_Target_T
    *localB)
{
    real32_T y;
    y = rtu_data;

    /*  % inputs */
    /*  parameters */
    /* MATLAB Function 'BasicAudio/Bass/Tone/InnerLink/ramp parameters': '<S50>:1' */
    /*  Ramp time mentioned in the RTC message gets priority over the tune */
    /*  variable. Boundary conditions are also included based on BDSP values. */
    /*  If neither ramp time nor rate are given in the RTC message (i.e. both */
    /*  = 0), then the ramp time in the Tune variable is taken as the default */
    /*  value for that message. */
    /* '<S50>:1:11' ramptime = single(data); */
    /* '<S50>:1:12' rampRate = single(0); */
    localB->rampRate = 0.0F;

    /* '<S50>:1:14' if (rampRateRTC) */
    if (rtu_rampRateRTC != 0U)
    {
        /* '<S50>:1:15' rampRate = single(rampRateRTC); */
        /* '<S50>:1:16' rampRate = max(rampRateBounds(1), rampRate); */
        /* '<S50>:1:17' rampRate = min(rampRateBounds(2), rampRate); */
        localB->rampRate = fminf((real32_T)rtp_rampRateBounds[1], fmaxf
            ((real32_T)rtp_rampRateBounds[0], (real32_T)rtu_rampRateRTC));
    }

    /* '<S50>:1:19' if (rampTimeRTC) */
    if (rtu_rampTimeRTC != 0U)
    {
        /* '<S50>:1:20' ramptime = single(rampTimeRTC); */
        y = (real32_T)rtu_rampTimeRTC;
    }

    /* '<S50>:1:22' ramptime = max(rampTimeBounds(1), ramptime); */
    /* '<S50>:1:23' ramptime = min(rampTimeBounds(2), ramptime); */
    localB->ramptime = fminf((real32_T)rtp_rampTimeBounds[1], fmaxf((real32_T)
        rtp_rampTimeBounds[0], y));
}

/* Output and update for function-call system: '<S40>/SetDsm' */
void BasicAudio_Target_SetDsm_a(void)
{
    /* S-Function (fcgen): '<S42>/Function-Call Generator' incorporates:
     *  SubSystem: '<S42>/LockEnter'
     */
    BasicAudio_Target_LockEnter();

    /* S-Function (fcgen): '<S42>/Function-Call Generator' incorporates:
     *  SubSystem: '<S42>/DsmWrite'
     */
    /* DataStoreWrite: '<S59>/Data Store Write' incorporates:
     *  Constant: '<S59>/Constant'
     */
    BasicAudio_TargetControlDsms.BasicAudioBassFreeze = 1U;

    /* S-Function (fcgen): '<S42>/Function-Call Generator' incorporates:
     *  SubSystem: '<S42>/LockExit'
     */
    BasicAudio_Target_LockExit();

    /* End of Outputs for S-Function (fcgen): '<S42>/Function-Call Generator' */
}

/* Output and update for function-call system: '<S40>/tone control' */
void BasicAudio_Target_tonecontrol(real32_T rtu_boost_max_db, real32_T
    rtu_cut_max_db, real32_T rtu_normalized_gain_impact, real32_T rtu_db_per_sec,
    real32_T rtu_ramp_time_ms, real32_T rtu_sample_rate_in_hertz, real32_T
    rtu_frame_size)
{
    suFnE4DIYISSjYbtQHPXOwD_BasicAudio_Target_T phaseConfig[2];
    real32_T currentGain_tmp;
    real32_T currentImpact_db;
    real32_T db_diff;
    real32_T quantum_ms;
    real32_T targetEffectiveGain;
    real32_T targetImpact_db;
    real32_T x;
    boolean_T currentIsBoost;
    boolean_T targetIsBoost;

    /*  %inputs */
    /*  % inputs % parameters */
    /* parameters */
    /* MATLAB Function 'BasicAudio/Bass/Tone/InnerLink/tone control': '<S52>:1' */
    /*  See Control summary at the end: */
    /*  Step 1: FREEZE the gain ramper so that we can perform calculations based */
    /*  on the current gain value. (This step now performed on canvas) */
    /*  Step 2a: IMPACT-TARGET Translate the normalized gain into a dB impact */
    /*  based on the maximum boost or cut values. */
    /*  ("-1" = maximum cut, "1" = maximum boost.) */
    /* '<S52>:1:16' targetIsBoost = (normalized_gain_impact >= 0); */
    targetIsBoost = (rtu_normalized_gain_impact >= 0.0F);

    /* '<S52>:1:17' if (targetIsBoost) */
    if (targetIsBoost)
    {
        /* '<S52>:1:18' targetImpact_db = normalized_gain_impact * boost_max_db; */
        targetImpact_db = rtu_normalized_gain_impact * rtu_boost_max_db;

        /* '<S52>:1:19' targetEffectiveGain = undb20(targetImpact_db); */
        /* UNDB20  Decibels to linear quantity */
        /*    Converts x from DB to absolute magnitude assuming that x is in volts. */
        /*    y = 10^(x/20) */
        /*  */
        /*  See also: DB20, UNDB10 */
        /*    Copyright 2018-2019 Bose Corporation */
        /*  Created : 9/7/1994 Bill Berardi */
        /* $Id: undb20.m 11244 2011-10-31 18:51:23Z rg20270 $ */
        /* 'undb20:12' y = exp(x * log(10)/20); */
        targetEffectiveGain = expf(targetImpact_db * 2.30258512F / 20.0F);

        /*  1 or greater */
    }
    else
    {
        /* '<S52>:1:20' else */
        /* '<S52>:1:21' targetImpact_db = normalized_gain_impact * cut_max_db; */
        targetImpact_db = rtu_normalized_gain_impact * rtu_cut_max_db;

        /* '<S52>:1:22' targetEffectiveGain = undb20(targetImpact_db); */
        /* UNDB20  Decibels to linear quantity */
        /*    Converts x from DB to absolute magnitude assuming that x is in volts. */
        /*    y = 10^(x/20) */
        /*  */
        /*  See also: DB20, UNDB10 */
        /*    Copyright 2018-2019 Bose Corporation */
        /*  Created : 9/7/1994 Bill Berardi */
        /* $Id: undb20.m 11244 2011-10-31 18:51:23Z rg20270 $ */
        /* 'undb20:12' y = exp(x * log(10)/20); */
        targetEffectiveGain = expf(targetImpact_db * 2.30258512F / 20.0F);

        /*  less than 1 */
    }

    /* '<S52>:1:25' actualTargetGainLinear = targetEffectiveGain; */
    /*  Step 2b: IMPACT-CURRENT Fetch the current gain from the previously */
    /*  active ramper. */
    /* '<S52>:1:29' currentPhase = Phase; */
    /* '<S52>:1:30' currentGain = Rampers(currentPhase).currentGain; */
    currentGain_tmp = BasicAudio_TargetControlDsms.BasicAudioBassRampers
        [(int32_T)BasicAudio_TargetControlDsms.BasicAudioBassPhase - 1].
        currentGain;

    /* '<S52>:1:31' currentIsBoost = (currentGain >= 1); */
    currentIsBoost = (currentGain_tmp >= 1.0F);

    /* '<S52>:1:32' currentImpact_db = db20(currentGain); */
    /* DB20  Linear quantity to decibels */
    /*    Converts x to dB assuming that x represents volts. */
    /*    y=20*log10(abs(x)); */
    /*  */
    /*  See Also: DB10, UNDB20 */
    /*    Copyright 2018-2019 Bose Corporation */
    /*  Bill Berardi */
    /*  $Id: db20.m 11813 2012-07-11 17:31:25Z rs7109 $ */
    /* 'db20:12' y = 20/log(10)*log(abs(x)); */
    currentImpact_db = logf(fabsf(currentGain_tmp)) * 8.68588924F;

    /*  Setup processing to continue on seamlessly while we work here */
    /* '<S52>:1:35' Rampers(1).currentGain = currentGain; */
    /* '<S52>:1:36' Phase = uint32(1); */
    BasicAudio_TargetControlDsms.BasicAudioBassPhase = 1U;

    /*  reset phase to 1 */
    /*  Now that we have both current and target impacts, find the difference */
    /* '<S52>:1:38' db_diff = abs(currentImpact_db - targetImpact_db); */
    db_diff = fabsf(currentImpact_db - targetImpact_db);

    /*  Step 3a: TIME- If a slope in dB/second is provided, ignore the ramp time */
    /* '<S52>:1:41' if (db_per_sec) */
    if (rtu_db_per_sec != 0.0F)
    {
        /* '<S52>:1:42' ramp_ms = db_diff / single(db_per_sec) * 1000; */
        x = db_diff / rtu_db_per_sec * 1000.0F;
    }
    else
    {
        /* '<S52>:1:43' else */
        /* '<S52>:1:44' ramp_ms = single(ramp_time_ms); */
        x = rtu_ramp_time_ms;
    }

    /*  Step 3b: TIME- QUANTIZE the desired ramp time to our FrameSize.  */
    /* '<S52>:1:48' quantum_ms = 1000 * frame_size / sample_rate_in_hertz; */
    quantum_ms = 1000.0F * rtu_frame_size / rtu_sample_rate_in_hertz;

    /* '<S52>:1:49' numFrames = ramp_ms / quantum_ms; */
    /* '<S52>:1:50' numFrames = max (1, ceil(numFrames)); */
    x = fmaxf(1.0F, ceilf(x / quantum_ms));

    /*  Step 4a: PHASES- Determine whether ramping from current gain to target */
    /*  gain requires a unity crossing in linear gain. */
    /* '<S52>:1:54' isTwoPhase = (currentIsBoost ~= targetIsBoost) && ... */
    /* '<S52>:1:55'              (currentGain ~= 1) && ... % cannot cross if you're already there */
    /* '<S52>:1:56'              (targetImpact_db ~= 1); */
    /*  % cannot cross if you're already there */
    /*  no crossing needed when impact already nothing */
    /*  Setup a structure to hold information about what the rampers should do */
    /* '<S52>:1:58' twoSingles = {single(0) single(0)}; */
    /* '<S52>:1:59' phaseConfig = struct('frames',twoSingles,'target_linear',twoSingles,'current_linear',twoSingles); */
    /* '<S52>:1:60' phaseConfig = tone_control_split(numFrames,currentImpact_db,db_diff,... */
    /* '<S52>:1:61'     currentGain,currentIsBoost,rgain_SILENT_GAIN,targetIsBoost,... */
    /* '<S52>:1:62'     targetEffectiveGain,isTwoPhase,phaseConfig); */
    /* '<S52>:1:104' if (isTwoPhase) */
    if ((currentIsBoost != targetIsBoost) && (currentGain_tmp != 1.0F) &&
            (targetImpact_db != 1.0F))
    {
        /*  Both boost and cut phases will be required to get where we are going. */
        /*  Make sure we have at least two frames to work with: */
        /* '<S52>:1:107' numFrames = max (2, round(numFrames)); */
        x = fmaxf(2.0F, x);

        /*  Divide up our ramp time into pieces for each phase to use */
        /* '<S52>:1:110' phase1Frames = ceil(numFrames * abs(currentImpact_db) / db_diff); */
        targetImpact_db = ceilf(x * fabsf(currentImpact_db) / db_diff);

        /* '<S52>:1:111' phase2Frames = numFrames - phase1Frames; */
        /*  Make sure phase 2 has at least 1 frame to work with */
        /* '<S52>:1:114' phase2Frames = max(1, phase2Frames); */
        phaseConfig[1].frames = fmaxf(1.0F, x - targetImpact_db);

        /*  Setup Phase 1 ramper that takes us to zero impact (i.e. unity gain) */
        /* '<S52>:1:117' phaseConfig(1).current_linear   = currentGain; */
        phaseConfig[0].current_linear = currentGain_tmp;

        /*  we know this is not 1 */
        /* '<S52>:1:118' phaseConfig(1).target_linear    = NudgeUnity(1,currentIsBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S52>:1:94' if (isPositive) */
        if (currentIsBoost)
        {
            /* '<S52>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            phaseConfig[0].target_linear = 1.00000048F;
        }
        else
        {
            /* '<S52>:1:96' else */
            /* '<S52>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            phaseConfig[0].target_linear = 0.999999523F;
        }

        /* '<S52>:1:119' phaseConfig(1).frames           = phase1Frames; */
        phaseConfig[0].frames = targetImpact_db;

        /*  Setup Phase 2 ramper that delivers us to target impact */
        /* '<S52>:1:122' phaseConfig(2).current_linear   = NudgeUnity(1,targetIsBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S52>:1:94' if (isPositive) */
        if (targetIsBoost)
        {
            /* '<S52>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            phaseConfig[1].current_linear = 1.00000048F;
        }
        else
        {
            /* '<S52>:1:96' else */
            /* '<S52>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            phaseConfig[1].current_linear = 0.999999523F;
        }

        /*  start quiet */
        /* '<S52>:1:123' phaseConfig(2).target_linear    = targetEffectiveGain; */
        phaseConfig[1].target_linear = targetEffectiveGain;

        /*  we know this is not 0 */
        /* '<S52>:1:124' phaseConfig(2).frames           = phase2Frames; */
    }
    else
    {
        /* '<S52>:1:125' else */
        /*  Only one phase is needed to accomplish our goal. We are are the */
        /*  target side of unity... but we must determine which side that is */
        /* '<S52>:1:128' if (currentGain == 1) */
        if (currentGain_tmp == 1.0F)
        {
            /*  One is in-between sides: use the target to determine the side */
            /* '<S52>:1:130' isBoost = targetIsBoost; */
            currentIsBoost = targetIsBoost;
        }
        else
        {
            /* '<S52>:1:131' else */
            /*  Current gain is already on one side of unity so use that */
            /* '<S52>:1:133' isBoost = currentIsBoost; */
        }

        /* '<S52>:1:135' phaseConfig(1).current_linear   = NudgeUnity(currentGain,isBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S52>:1:94' if (isPositive) */
        if (currentIsBoost)
        {
            /* '<S52>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            phaseConfig[0].current_linear = fmaxf(currentGain_tmp, 1.00000048F);

            /* '<S52>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            phaseConfig[0].target_linear = fmaxf(targetEffectiveGain,
                1.00000048F);
        }
        else
        {
            /* '<S52>:1:96' else */
            /* '<S52>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            phaseConfig[0].current_linear = fminf(currentGain_tmp, 0.999999523F);

            /* '<S52>:1:96' else */
            /* '<S52>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            phaseConfig[0].target_linear = fminf(targetEffectiveGain,
                0.999999523F);
        }

        /* '<S52>:1:136' phaseConfig(1).target_linear    = NudgeUnity(targetEffectiveGain,isBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S52>:1:94' if (isPositive) */
        /* '<S52>:1:137' phaseConfig(1).frames           = numFrames; */
        phaseConfig[0].frames = x;

        /*  Setup Phase 2 ramper to sit at the target gain */
        /* '<S52>:1:140' phaseConfig(2).current_linear   = NudgeUnity(targetEffectiveGain,isBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S52>:1:94' if (isPositive) */
        if (currentIsBoost)
        {
            /* '<S52>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            targetImpact_db = fmaxf(targetEffectiveGain, 1.00000048F);
            phaseConfig[1].current_linear = targetImpact_db;

            /* '<S52>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            phaseConfig[1].target_linear = targetImpact_db;
        }
        else
        {
            /* '<S52>:1:96' else */
            /* '<S52>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            targetImpact_db = fminf(targetEffectiveGain, 0.999999523F);
            phaseConfig[1].current_linear = targetImpact_db;

            /* '<S52>:1:96' else */
            /* '<S52>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            phaseConfig[1].target_linear = targetImpact_db;
        }

        /* '<S52>:1:141' phaseConfig(2).target_linear    = NudgeUnity(targetEffectiveGain,isBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S52>:1:94' if (isPositive) */
        /* '<S52>:1:142' phaseConfig(2).frames           = single(0); */
        phaseConfig[1].frames = 0.0F;
    }

    /*  Step 4b: PHASES- Setup rampers for both phases */
    /* '<S52>:1:65' for currentPhase=1:2 */
    /* '<S52>:1:66' if (phaseConfig(currentPhase).frames) */
    if (phaseConfig[0].frames != 0.0F)
    {
        /* '<S52>:1:67' ramp_ms = quantum_ms * phaseConfig(currentPhase).frames; */
        /* '<S52>:1:68' factor = log(phaseConfig(currentPhase).target_linear / phaseConfig(currentPhase).current_linear); */
        /* '<S52>:1:69' RampCoef = exp(factor / (sample_rate_in_hertz * ramp_ms / 1000)); */
        BasicAudio_TargetControlDsms.BasicAudioBassRampers[0].rampCoeff = expf
            (logf(phaseConfig[0].target_linear / phaseConfig[0].current_linear) /
             (quantum_ms * phaseConfig[0].frames * rtu_sample_rate_in_hertz /
              1000.0F));
    }
    else
    {
        /* '<S52>:1:70' else */
        /* '<S52>:1:71' RampCoef = single(1); */
        BasicAudio_TargetControlDsms.BasicAudioBassRampers[0].rampCoeff = 1.0F;
    }

    /* '<S52>:1:73' Rampers(currentPhase).rampCoeff   = single(RampCoef); */
    /* '<S52>:1:74' Rampers(currentPhase).targetGain  = actualTargetGainLinear; */
    BasicAudio_TargetControlDsms.BasicAudioBassRampers[0].targetGain =
        targetEffectiveGain;

    /*  phase 1 value ignored. */
    /* '<S52>:1:75' Rampers(currentPhase).currentGain = single(phaseConfig(currentPhase).current_linear); */
    BasicAudio_TargetControlDsms.BasicAudioBassRampers[0].currentGain =
        phaseConfig[0].current_linear;

    /* '<S52>:1:76' Rampers(currentPhase).frameCount  = int32(phaseConfig(currentPhase).frames); */
    BasicAudio_TargetControlDsms.BasicAudioBassRampers[0].frameCount = (int32_T)
        phaseConfig[0].frames;

    /* '<S52>:1:66' if (phaseConfig(currentPhase).frames) */
    if (phaseConfig[1].frames != 0.0F)
    {
        /* '<S52>:1:67' ramp_ms = quantum_ms * phaseConfig(currentPhase).frames; */
        /* '<S52>:1:68' factor = log(phaseConfig(currentPhase).target_linear / phaseConfig(currentPhase).current_linear); */
        /* '<S52>:1:69' RampCoef = exp(factor / (sample_rate_in_hertz * ramp_ms / 1000)); */
        BasicAudio_TargetControlDsms.BasicAudioBassRampers[1].rampCoeff = expf
            (logf(phaseConfig[1].target_linear / phaseConfig[1].current_linear) /
             (quantum_ms * phaseConfig[1].frames * rtu_sample_rate_in_hertz /
              1000.0F));
    }
    else
    {
        /* '<S52>:1:70' else */
        /* '<S52>:1:71' RampCoef = single(1); */
        BasicAudio_TargetControlDsms.BasicAudioBassRampers[1].rampCoeff = 1.0F;
    }

    /* '<S52>:1:73' Rampers(currentPhase).rampCoeff   = single(RampCoef); */
    /* '<S52>:1:74' Rampers(currentPhase).targetGain  = actualTargetGainLinear; */
    BasicAudio_TargetControlDsms.BasicAudioBassRampers[1].targetGain =
        targetEffectiveGain;

    /*  phase 1 value ignored. */
    /* '<S52>:1:75' Rampers(currentPhase).currentGain = single(phaseConfig(currentPhase).current_linear); */
    BasicAudio_TargetControlDsms.BasicAudioBassRampers[1].currentGain =
        phaseConfig[1].current_linear;

    /* '<S52>:1:76' Rampers(currentPhase).frameCount  = int32(phaseConfig(currentPhase).frames); */
    BasicAudio_TargetControlDsms.BasicAudioBassRampers[1].frameCount = (int32_T)
        phaseConfig[1].frames;

    /*  unfreeze the gain. (This step now performed on canvas) */
}

/* Output and update for function-call system: '<S40>/SetDsm1' */
void BasicAudio_Target_SetDsm1_m(void)
{
    /* S-Function (fcgen): '<S43>/Function-Call Generator' incorporates:
     *  SubSystem: '<S43>/LockEnter'
     */
    BasicAudio_Target_LockEnter();

    /* S-Function (fcgen): '<S43>/Function-Call Generator' incorporates:
     *  SubSystem: '<S43>/DsmWrite'
     */
    /* DataStoreWrite: '<S62>/Data Store Write' incorporates:
     *  Constant: '<S62>/Constant'
     */
    BasicAudio_TargetControlDsms.BasicAudioBassFreeze = 0U;

    /* S-Function (fcgen): '<S43>/Function-Call Generator' incorporates:
     *  SubSystem: '<S43>/LockExit'
     */
    BasicAudio_Target_LockExit();

    /* End of Outputs for S-Function (fcgen): '<S43>/Function-Call Generator' */
}

/* Output and update for function-call system: '<S41>/REQ' */
void HandleReqBasicAudioBassSet(void)
{
    /* MATLAB Function: '<S40>/index to linear' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'RTC/REQ': '<S56>:1' */
    /* '<S56>:1:3' go(); */
    BasicAudio_Target_indextolinear(BasicAudio_Target_BasicAudio0.BassScale,
        BasicAudio_Target_BasicAudio0.BassOffset,
        BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_gain_index,
        &BasicAudio_Target_B.sf_indextolinear);

    /* MATLAB Function: '<S40>/ramp parameters' */
    BasicAudio_Target_rampparameters(BasicAudio_Target_BasicAudio0.Bassramptime,
        BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_ramp_milliseconds,
        BasicAudio_TargetBasicAudioBassSetRtcOut.REQ_ramp_db_per_second,
        BasicAudio_Target_ConstP.pooled2, BasicAudio_Target_ConstP.pooled1,
        &BasicAudio_Target_B.sf_rampparameters);

    /* Outputs for Function Call SubSystem: '<S40>/SetDsm' */
    BasicAudio_Target_SetDsm_a();

    /* End of Outputs for SubSystem: '<S40>/SetDsm' */

    /* MATLAB Function: '<S40>/tone control' */
    /* MATLAB Function: '<S40>/tone control' incorporates:
     *  S-Function (AudioStats_MEX): '<S40>/Audio Stats'
     */
    BasicAudio_Target_tonecontrol
        (BasicAudio_Target_BasicAudio0.BassBoostMaximum_dB,
         BasicAudio_Target_BasicAudio0.BassCutMaximum_dB,
         BasicAudio_Target_B.sf_indextolinear.linearGain,
         BasicAudio_Target_B.sf_rampparameters.rampRate,
         BasicAudio_Target_B.sf_rampparameters.ramptime, 48000.0, 32.0);

    /* Outputs for Function Call SubSystem: '<S40>/SetDsm1' */
    BasicAudio_Target_SetDsm1_m();

    /* End of Outputs for SubSystem: '<S40>/SetDsm1' */
}

/* Output and update for atomic system: '<S40>/tone' */
void BasicAudio_Target_tone(const real32_T rtu_boost_coeffs[3], const real32_T
    rtu_cut_coeffs[3], const real32_T rtu_audioIn[64],
    B_tone_BasicAudio_Target_T *localB, DW_tone_BasicAudio_Target_T *localDW)
{
    int32_T q0;
    int32_T qY;
    real32_T aCoeffs;
    real32_T bCoeffs_idx_0;
    real32_T bCoeffs_idx_1;
    real32_T gain;
    real32_T k;
    uint32_T ramper;

    /*     % inputs % parameters */
    /* MATLAB Function 'BasicAudio/Bass/Tone/InnerLink/tone': '<S51>:1' */
    /*  Data Store objects (registered in "Edit Data" dialog */
    /*  Keep memory for the filter persistent from frame to frame */
    /*  figure out the frame size and channel count */
    /* '<S51>:1:15' [frame_size, channel_count] = size(audioIn); */
    /* '<S51>:1:17' if (isempty(toneFilterStates)) */
    /*  set the size and datatype of the output buffer */
    /* '<S51>:1:23' audioOut = coder.nullcopy(audioIn); */
    /* '<S51>:1:25' ramper = Phase; */
    ramper = BasicAudio_TargetControlDsms.BasicAudioBassPhase;

    /* '<S51>:1:26' ramperCount = numel(Rampers); */
    /* '<S51>:1:28' if (Freeze == false) */
    if (BasicAudio_TargetControlDsms.BasicAudioBassFreeze == 0U)
    {
        /* '<S51>:1:29' if (Rampers(ramper).frameCount == 0) */
        if (BasicAudio_TargetControlDsms.BasicAudioBassRampers[(int32_T)
                BasicAudio_TargetControlDsms.BasicAudioBassPhase - 1].frameCount
            == 0)
        {
            /*  The current ramper is finished. Is there another one? */
            /* '<S51>:1:31' if (ramper < ramperCount && Rampers(ramper+1).frameCount > 0) */
            if ((BasicAudio_TargetControlDsms.BasicAudioBassPhase < 2U) &&
                    (BasicAudio_TargetControlDsms.BasicAudioBassRampers[(int32_T)
                     BasicAudio_TargetControlDsms.BasicAudioBassPhase].
                     frameCount > 0))
            {
                /*  Yes - move on to the next phase */
                /* '<S51>:1:33' ramper = ramper + 1; */
                qY = (int32_T)(BasicAudio_TargetControlDsms.BasicAudioBassPhase
                               + /*MW:OvSatOk*/ 1U);
                ramper = BasicAudio_TargetControlDsms.BasicAudioBassPhase + 1U;

                /* '<S51>:1:34' Phase = ramper; */
                BasicAudio_TargetControlDsms.BasicAudioBassPhase =
                    BasicAudio_TargetControlDsms.BasicAudioBassPhase + 1U;

                /* '<S51>:1:35' k = Rampers(ramper).rampCoeff; */
                k = BasicAudio_TargetControlDsms.BasicAudioBassRampers[qY - 1].
                    rampCoeff;

                /* '<S51>:1:36' gain = Rampers(ramper).currentGain; */
                gain = BasicAudio_TargetControlDsms.BasicAudioBassRampers[qY - 1]
                    .currentGain;

                /* '<S51>:1:37' Rampers(ramper).frameCount = Rampers(ramper).frameCount - 1; */
                q0 = BasicAudio_TargetControlDsms.BasicAudioBassRampers[qY - 1].
                    frameCount;
                if (q0 < -2147483647)
                {
                    BasicAudio_TargetControlDsms.BasicAudioBassRampers[qY - 1].
                        frameCount = MIN_int32_T;
                }
                else
                {
                    BasicAudio_TargetControlDsms.BasicAudioBassRampers[qY - 1].
                        frameCount = q0 - 1;
                }
            }
            else
            {
                /* '<S51>:1:38' else */
                /*   Stop ramping and SNAP to the target */
                /* '<S51>:1:40' k = single(1); */
                k = 1.0F;

                /* '<S51>:1:41' gain = Rampers(ramper).targetGain; */
                gain = BasicAudio_TargetControlDsms.BasicAudioBassRampers
                    [(int32_T)BasicAudio_TargetControlDsms.BasicAudioBassPhase -
                    1].targetGain;

                /*  Update completion flag */
            }
        }
        else
        {
            /* '<S51>:1:44' else */
            /*  The ramp is *NOT* frozen or done: gain is moving */
            /* '<S51>:1:46' k = Rampers(ramper).rampCoeff; */
            k = BasicAudio_TargetControlDsms.BasicAudioBassRampers[(int32_T)
                BasicAudio_TargetControlDsms.BasicAudioBassPhase - 1].rampCoeff;

            /* '<S51>:1:47' gain = Rampers(ramper).currentGain; */
            gain = BasicAudio_TargetControlDsms.BasicAudioBassRampers[(int32_T)
                BasicAudio_TargetControlDsms.BasicAudioBassPhase - 1].
                currentGain;

            /* '<S51>:1:48' Rampers(ramper).frameCount = Rampers(ramper).frameCount - 1; */
            q0 = BasicAudio_TargetControlDsms.BasicAudioBassRampers[(int32_T)
                BasicAudio_TargetControlDsms.BasicAudioBassPhase - 1].frameCount;
            if (q0 < -2147483647)
            {
                BasicAudio_TargetControlDsms.BasicAudioBassRampers[(int32_T)
                    BasicAudio_TargetControlDsms.BasicAudioBassPhase - 1].
                    frameCount = MIN_int32_T;
            }
            else
            {
                BasicAudio_TargetControlDsms.BasicAudioBassRampers[(int32_T)
                    BasicAudio_TargetControlDsms.BasicAudioBassPhase - 1].
                    frameCount = q0 - 1;
            }
        }
    }
    else
    {
        /* '<S51>:1:50' else */
        /*  Frozen. Stay at current gain */
        /* '<S51>:1:52' k = single(1); */
        k = 1.0F;

        /* '<S51>:1:53' gain = Rampers(ramper).currentGain; */
        gain = BasicAudio_TargetControlDsms.BasicAudioBassRampers[(int32_T)
            BasicAudio_TargetControlDsms.BasicAudioBassPhase - 1].currentGain;
    }

    /*  Apply the tone filter */
    /* '<S51>:1:57' if (gain < 1) */
    if (gain < 1.0F)
    {
        /*  Cut */
        /* '<S51>:1:59' bCoeffs = cut_coeffs(1:2); */
        bCoeffs_idx_0 = rtu_cut_coeffs[0];
        bCoeffs_idx_1 = rtu_cut_coeffs[1];

        /* '<S51>:1:60' aCoeffs = cut_coeffs(3); */
        aCoeffs = rtu_cut_coeffs[2];
    }
    else
    {
        /* '<S51>:1:61' else */
        /*  Boost */
        /* '<S51>:1:63' bCoeffs = boost_coeffs(1:2); */
        bCoeffs_idx_0 = rtu_boost_coeffs[0];
        bCoeffs_idx_1 = rtu_boost_coeffs[1];

        /* '<S51>:1:64' aCoeffs = boost_coeffs(3); */
        aCoeffs = rtu_boost_coeffs[2];
    }

    /*  [filtered,toneFilterStates] = filter(bCoeffs, aCoeffs, audioIn, toneFilterStates); */
    /*  Loop through samples, adjusting gain and applying to channels */
    /* '<S51>:1:70' for n=1:frame_size */
    for (q0 = 0; q0 < 32; q0++)
    {
        real32_T filterOut;
        real32_T filterOut_tmp;

        /*  Update the gain and then subtract one before applying it to the */
        /*  filtered signal. Disregarding phase shifting  */
        /* '<S51>:1:73' gain = gain * k; */
        gain *= k;

        /* '<S51>:1:74' filterGain = gain - 1; */
        /*  Loop through channels adding in gain-adjusted filtered samples */
        /* '<S51>:1:76' for chanIdx = 1:channel_count */
        /* '<S51>:1:78' filterOut = (bCoeffs(1) * audioIn(n,chanIdx)) + toneFilterStates(chanIdx); */
        filterOut = bCoeffs_idx_0 * rtu_audioIn[q0] + localDW->toneFilterStates
            [0];

        /* '<S51>:1:79' toneFilterStates(chanIdx) = (bCoeffs(2) * audioIn(n,chanIdx)) + (aCoeffs(1) * filterOut); */
        localDW->toneFilterStates[0] = bCoeffs_idx_1 * rtu_audioIn[q0] + aCoeffs
            * filterOut;

        /* '<S51>:1:81' audioOut(n,chanIdx) = audioIn(n,chanIdx) + (filterOut * filterGain); */
        localB->audioOut[q0] = (gain - 1.0F) * filterOut + rtu_audioIn[q0];

        /* '<S51>:1:78' filterOut = (bCoeffs(1) * audioIn(n,chanIdx)) + toneFilterStates(chanIdx); */
        filterOut_tmp = rtu_audioIn[q0 + 32];
        filterOut = filterOut_tmp * bCoeffs_idx_0 + localDW->toneFilterStates[1];

        /* '<S51>:1:79' toneFilterStates(chanIdx) = (bCoeffs(2) * audioIn(n,chanIdx)) + (aCoeffs(1) * filterOut); */
        localDW->toneFilterStates[1] = filterOut_tmp * bCoeffs_idx_1 + aCoeffs *
            filterOut;

        /* '<S51>:1:81' audioOut(n,chanIdx) = audioIn(n,chanIdx) + (filterOut * filterGain); */
        localB->audioOut[q0 + 32] = (gain - 1.0F) * filterOut + filterOut_tmp;
    }

    /* '<S51>:1:85' Rampers(ramper).currentGain = gain; */
    BasicAudio_TargetControlDsms.BasicAudioBassRampers[(int32_T)ramper - 1].
        currentGain = gain;
}

/* Output and update for function-call system: '<S69>/REQ' */
void HandleReqBasicAudioMuteSet(void)
{
    real32_T rampRate;
    real32_T y;

    /* MATLAB Function: '<S67>/calculate_ramp_rate' */
    /* MATLAB Function: '<S67>/calculate_ramp_rate' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'RTC/REQ': '<S76>:1' */
    /* '<S76>:1:3' go(); */
    y = BasicAudio_Target_BasicAudio0.MuteRampTime;

    /*  % Inputs */
    /*  parameters */
    /* MATLAB Function 'BasicAudio/Mute/mute/InnerLink/MuteControl/calculate_ramp_rate': '<S72>:1' */
    /*  Ramp time and rate are provided to the block through RTC messages. However  */
    /*  in case neither of them are provided, the block will use the default ramp  */
    /*  time given in the tune variable. */
    /* '<S72>:1:8' ramptime = single(rampTimeTune); */
    /* '<S72>:1:9' rampRate = single(0); */
    rampRate = 0.0F;

    /* '<S72>:1:11' if (rampRateRTC) */
    if (BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_ramp_db_per_second != 0U)
    {
        /* '<S72>:1:12' rampRate = single(rampRateRTC); */
        /* '<S72>:1:13' rampRate = max(rampRateBounds(1), rampRate); */
        /* '<S72>:1:14' rampRate = min(rampRateBounds(2), rampRate); */
        rampRate = fminf(50000.0F, fmaxf(5.0F, (real32_T)
                          BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_ramp_db_per_second));
    }

    /* '<S72>:1:16' if (rampTimeRTC) */
    if (BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_ramp_milliseconds != 0U)
    {
        /* '<S72>:1:17' ramptime = single(rampTimeRTC); */
        y = (real32_T)
            BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_ramp_milliseconds;
    }

    /* MATLAB Function: '<S68>/get_ramp_coeffs' */
    /* MATLAB Function: '<S68>/get_ramp_coeffs' incorporates:
     *  DataStoreRead: '<S68>/Data Store Read'
     *  MATLAB Function: '<S67>/calculate_ramp_rate'
     *  S-Function (AudioStats_MEX): '<S68>/Audio Stats'
     */
    /* '<S72>:1:19' ramptime = max(rampTimeBounds(1), ramptime); */
    /* '<S72>:1:20' ramptime = min(rampTimeBounds(2), ramptime); */
    y = fminf(3000.0F, fmaxf(1.0F, y));

    /* MATLAB Function 'MuteRamper/get_ramp_coeffs': '<S81>:1' */
    /* '<S81>:1:5' targetLinearGain = single(1.0); */
    BasicAudio_TargetControlDsms.BasicAudioMuteTargetLinearGain = 1.0F;

    /* '<S81>:1:7' if muteSwitch */
    if (BasicAudio_TargetBasicAudioMuteSetRtcOut.REQ_mute_switch != 0U)
    {
        /* '<S81>:1:8' targetLinearGain = single(max(0, silentGainLinear)); */
        BasicAudio_TargetControlDsms.BasicAudioMuteTargetLinearGain =
            4.7315126E-7F;
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
    /* '<S81>:1:24' if (rampingRate_dBPerSec) */
    if (rampRate != 0.0F)
    {
        /* '<S81>:1:25' currentGain_db = 20*log10(max(currentGainValue, silentGainLinear)); */
        /* '<S81>:1:26' targetGain_db = 20*log10( targetLinearGain); */
        /* '<S81>:1:27' diff = abs(currentGain_db - targetGain_db); */
        /* '<S81>:1:28' rampingTime_ms = diff / single(rampingRate_dBPerSec) * 1000; */
        y = fabsf(20.0F * log10f(fmaxf
                   (BasicAudio_TargetControlDsms.BasicAudioMuteCurrentGain,
                    4.7315126E-7F)) - 20.0F * log10f
                  (BasicAudio_TargetControlDsms.BasicAudioMuteTargetLinearGain))
            / rampRate * 1000.0F;
    }

    /* '<S81>:1:31' quantum_ms = 1000 * frameSize / Fs_Hz; */
    rampRate = 32000.0F / 48000.0;

    /* '<S81>:1:32' numFrames = rampingTime_ms / quantum_ms; */
    /*  Do a ceil rather than a round to insure that we create enough */
    /*  frames. i.e. a fraction of a frame is promoted to an addition frame */
    /* '<S81>:1:35' nFrames = max (1, ceil(numFrames)); */
    BasicAudio_TargetControlDsms.BasicAudioMuteNframes = fmaxf(1.0F, ceilf(y /
        rampRate));

    /* '<S81>:1:36' quantized_rampTime_ms = nFrames * quantum_ms; */
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
    /* '<S81>:1:50' ramping_coeff= exp(log( targetLinearGain /max(currentGainValue, silentGainLinear) ) / (Fs_Hz * quantized_rampTime_ms / 1000)); */
    BasicAudio_Target_B.ramping_coeff = logf
        (BasicAudio_TargetControlDsms.BasicAudioMuteTargetLinearGain / fmaxf
         (BasicAudio_TargetControlDsms.BasicAudioMuteCurrentGain, 4.7315126E-7F))
        / (BasicAudio_TargetControlDsms.BasicAudioMuteNframes * rampRate *
           48000.0 / 1000.0F);
    BasicAudio_Target_B.ramping_coeff = expf(BasicAudio_Target_B.ramping_coeff);

    /* End of MATLAB Function: '<S68>/get_ramp_coeffs' */
}

/* Output and update for function-call system: '<S92>/SetDsm' */
void BasicAudio_Target_SetDsm_l(void)
{
    /* S-Function (fcgen): '<S94>/Function-Call Generator' incorporates:
     *  SubSystem: '<S94>/LockEnter'
     */
    BasicAudio_Target_LockEnter();

    /* S-Function (fcgen): '<S94>/Function-Call Generator' incorporates:
     *  SubSystem: '<S94>/DsmWrite'
     */
    /* DataStoreWrite: '<S111>/Data Store Write' incorporates:
     *  Constant: '<S111>/Constant'
     */
    BasicAudio_TargetControlDsms.BasicAudioTrebleFreeze = 1U;

    /* S-Function (fcgen): '<S94>/Function-Call Generator' incorporates:
     *  SubSystem: '<S94>/LockExit'
     */
    BasicAudio_Target_LockExit();

    /* End of Outputs for S-Function (fcgen): '<S94>/Function-Call Generator' */
}

/* Output and update for function-call system: '<S92>/tone control' */
void BasicAudio_Target_tonecontrol_o(real32_T rtu_boost_max_db, real32_T
    rtu_cut_max_db, real32_T rtu_normalized_gain_impact, real32_T rtu_db_per_sec,
    real32_T rtu_ramp_time_ms, real32_T rtu_sample_rate_in_hertz, real32_T
    rtu_frame_size)
{
    suFnE4DIYISSjYbtQHPXOwD_BasicAudio_Target_T phaseConfig[2];
    real32_T currentGain_tmp;
    real32_T currentImpact_db;
    real32_T db_diff;
    real32_T quantum_ms;
    real32_T targetEffectiveGain;
    real32_T targetImpact_db;
    real32_T x;
    boolean_T currentIsBoost;
    boolean_T targetIsBoost;

    /*  %inputs */
    /*  % inputs % parameters */
    /* parameters */
    /* MATLAB Function 'BasicAudio/Treble/Tone/InnerLink/tone control': '<S104>:1' */
    /*  See Control summary at the end: */
    /*  Step 1: FREEZE the gain ramper so that we can perform calculations based */
    /*  on the current gain value. (This step now performed on canvas) */
    /*  Step 2a: IMPACT-TARGET Translate the normalized gain into a dB impact */
    /*  based on the maximum boost or cut values. */
    /*  ("-1" = maximum cut, "1" = maximum boost.) */
    /* '<S104>:1:16' targetIsBoost = (normalized_gain_impact >= 0); */
    targetIsBoost = (rtu_normalized_gain_impact >= 0.0F);

    /* '<S104>:1:17' if (targetIsBoost) */
    if (targetIsBoost)
    {
        /* '<S104>:1:18' targetImpact_db = normalized_gain_impact * boost_max_db; */
        targetImpact_db = rtu_normalized_gain_impact * rtu_boost_max_db;

        /* '<S104>:1:19' targetEffectiveGain = undb20(targetImpact_db); */
        /* UNDB20  Decibels to linear quantity */
        /*    Converts x from DB to absolute magnitude assuming that x is in volts. */
        /*    y = 10^(x/20) */
        /*  */
        /*  See also: DB20, UNDB10 */
        /*    Copyright 2018-2019 Bose Corporation */
        /*  Created : 9/7/1994 Bill Berardi */
        /* $Id: undb20.m 11244 2011-10-31 18:51:23Z rg20270 $ */
        /* 'undb20:12' y = exp(x * log(10)/20); */
        targetEffectiveGain = expf(targetImpact_db * 2.30258512F / 20.0F);

        /*  1 or greater */
    }
    else
    {
        /* '<S104>:1:20' else */
        /* '<S104>:1:21' targetImpact_db = normalized_gain_impact * cut_max_db; */
        targetImpact_db = rtu_normalized_gain_impact * rtu_cut_max_db;

        /* '<S104>:1:22' targetEffectiveGain = undb20(targetImpact_db); */
        /* UNDB20  Decibels to linear quantity */
        /*    Converts x from DB to absolute magnitude assuming that x is in volts. */
        /*    y = 10^(x/20) */
        /*  */
        /*  See also: DB20, UNDB10 */
        /*    Copyright 2018-2019 Bose Corporation */
        /*  Created : 9/7/1994 Bill Berardi */
        /* $Id: undb20.m 11244 2011-10-31 18:51:23Z rg20270 $ */
        /* 'undb20:12' y = exp(x * log(10)/20); */
        targetEffectiveGain = expf(targetImpact_db * 2.30258512F / 20.0F);

        /*  less than 1 */
    }

    /* '<S104>:1:25' actualTargetGainLinear = targetEffectiveGain; */
    /*  Step 2b: IMPACT-CURRENT Fetch the current gain from the previously */
    /*  active ramper. */
    /* '<S104>:1:29' currentPhase = Phase; */
    /* '<S104>:1:30' currentGain = Rampers(currentPhase).currentGain; */
    currentGain_tmp = BasicAudio_TargetControlDsms.BasicAudioTrebleRampers
        [(int32_T)BasicAudio_TargetControlDsms.BasicAudioTreblePhase - 1].
        currentGain;

    /* '<S104>:1:31' currentIsBoost = (currentGain >= 1); */
    currentIsBoost = (currentGain_tmp >= 1.0F);

    /* '<S104>:1:32' currentImpact_db = db20(currentGain); */
    /* DB20  Linear quantity to decibels */
    /*    Converts x to dB assuming that x represents volts. */
    /*    y=20*log10(abs(x)); */
    /*  */
    /*  See Also: DB10, UNDB20 */
    /*    Copyright 2018-2019 Bose Corporation */
    /*  Bill Berardi */
    /*  $Id: db20.m 11813 2012-07-11 17:31:25Z rs7109 $ */
    /* 'db20:12' y = 20/log(10)*log(abs(x)); */
    currentImpact_db = logf(fabsf(currentGain_tmp)) * 8.68588924F;

    /*  Setup processing to continue on seamlessly while we work here */
    /* '<S104>:1:35' Rampers(1).currentGain = currentGain; */
    /* '<S104>:1:36' Phase = uint32(1); */
    BasicAudio_TargetControlDsms.BasicAudioTreblePhase = 1U;

    /*  reset phase to 1 */
    /*  Now that we have both current and target impacts, find the difference */
    /* '<S104>:1:38' db_diff = abs(currentImpact_db - targetImpact_db); */
    db_diff = fabsf(currentImpact_db - targetImpact_db);

    /*  Step 3a: TIME- If a slope in dB/second is provided, ignore the ramp time */
    /* '<S104>:1:41' if (db_per_sec) */
    if (rtu_db_per_sec != 0.0F)
    {
        /* '<S104>:1:42' ramp_ms = db_diff / single(db_per_sec) * 1000; */
        x = db_diff / rtu_db_per_sec * 1000.0F;
    }
    else
    {
        /* '<S104>:1:43' else */
        /* '<S104>:1:44' ramp_ms = single(ramp_time_ms); */
        x = rtu_ramp_time_ms;
    }

    /*  Step 3b: TIME- QUANTIZE the desired ramp time to our FrameSize.  */
    /* '<S104>:1:48' quantum_ms = 1000 * frame_size / sample_rate_in_hertz; */
    quantum_ms = 1000.0F * rtu_frame_size / rtu_sample_rate_in_hertz;

    /* '<S104>:1:49' numFrames = ramp_ms / quantum_ms; */
    /* '<S104>:1:50' numFrames = max (1, ceil(numFrames)); */
    x = fmaxf(1.0F, ceilf(x / quantum_ms));

    /*  Step 4a: PHASES- Determine whether ramping from current gain to target */
    /*  gain requires a unity crossing in linear gain. */
    /* '<S104>:1:54' isTwoPhase = (currentIsBoost ~= targetIsBoost) && ... */
    /* '<S104>:1:55'              (currentGain ~= 1) && ... % cannot cross if you're already there */
    /* '<S104>:1:56'              (targetImpact_db ~= 1); */
    /*  % cannot cross if you're already there */
    /*  no crossing needed when impact already nothing */
    /*  Setup a structure to hold information about what the rampers should do */
    /* '<S104>:1:58' twoSingles = {single(0) single(0)}; */
    /* '<S104>:1:59' phaseConfig = struct('frames',twoSingles,'target_linear',twoSingles,'current_linear',twoSingles); */
    /* '<S104>:1:60' phaseConfig = tone_control_split(numFrames,currentImpact_db,db_diff,... */
    /* '<S104>:1:61'     currentGain,currentIsBoost,rgain_SILENT_GAIN,targetIsBoost,... */
    /* '<S104>:1:62'     targetEffectiveGain,isTwoPhase,phaseConfig); */
    /* '<S104>:1:104' if (isTwoPhase) */
    if ((currentIsBoost != targetIsBoost) && (currentGain_tmp != 1.0F) &&
            (targetImpact_db != 1.0F))
    {
        /*  Both boost and cut phases will be required to get where we are going. */
        /*  Make sure we have at least two frames to work with: */
        /* '<S104>:1:107' numFrames = max (2, round(numFrames)); */
        x = fmaxf(2.0F, x);

        /*  Divide up our ramp time into pieces for each phase to use */
        /* '<S104>:1:110' phase1Frames = ceil(numFrames * abs(currentImpact_db) / db_diff); */
        targetImpact_db = ceilf(x * fabsf(currentImpact_db) / db_diff);

        /* '<S104>:1:111' phase2Frames = numFrames - phase1Frames; */
        /*  Make sure phase 2 has at least 1 frame to work with */
        /* '<S104>:1:114' phase2Frames = max(1, phase2Frames); */
        phaseConfig[1].frames = fmaxf(1.0F, x - targetImpact_db);

        /*  Setup Phase 1 ramper that takes us to zero impact (i.e. unity gain) */
        /* '<S104>:1:117' phaseConfig(1).current_linear   = currentGain; */
        phaseConfig[0].current_linear = currentGain_tmp;

        /*  we know this is not 1 */
        /* '<S104>:1:118' phaseConfig(1).target_linear    = NudgeUnity(1,currentIsBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S104>:1:94' if (isPositive) */
        if (currentIsBoost)
        {
            /* '<S104>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            phaseConfig[0].target_linear = 1.00000048F;
        }
        else
        {
            /* '<S104>:1:96' else */
            /* '<S104>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            phaseConfig[0].target_linear = 0.999999523F;
        }

        /* '<S104>:1:119' phaseConfig(1).frames           = phase1Frames; */
        phaseConfig[0].frames = targetImpact_db;

        /*  Setup Phase 2 ramper that delivers us to target impact */
        /* '<S104>:1:122' phaseConfig(2).current_linear   = NudgeUnity(1,targetIsBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S104>:1:94' if (isPositive) */
        if (targetIsBoost)
        {
            /* '<S104>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            phaseConfig[1].current_linear = 1.00000048F;
        }
        else
        {
            /* '<S104>:1:96' else */
            /* '<S104>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            phaseConfig[1].current_linear = 0.999999523F;
        }

        /*  start quiet */
        /* '<S104>:1:123' phaseConfig(2).target_linear    = targetEffectiveGain; */
        phaseConfig[1].target_linear = targetEffectiveGain;

        /*  we know this is not 0 */
        /* '<S104>:1:124' phaseConfig(2).frames           = phase2Frames; */
    }
    else
    {
        /* '<S104>:1:125' else */
        /*  Only one phase is needed to accomplish our goal. We are are the */
        /*  target side of unity... but we must determine which side that is */
        /* '<S104>:1:128' if (currentGain == 1) */
        if (currentGain_tmp == 1.0F)
        {
            /*  One is in-between sides: use the target to determine the side */
            /* '<S104>:1:130' isBoost = targetIsBoost; */
            currentIsBoost = targetIsBoost;
        }
        else
        {
            /* '<S104>:1:131' else */
            /*  Current gain is already on one side of unity so use that */
            /* '<S104>:1:133' isBoost = currentIsBoost; */
        }

        /* '<S104>:1:135' phaseConfig(1).current_linear   = NudgeUnity(currentGain,isBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S104>:1:94' if (isPositive) */
        if (currentIsBoost)
        {
            /* '<S104>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            phaseConfig[0].current_linear = fmaxf(currentGain_tmp, 1.00000048F);

            /* '<S104>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            phaseConfig[0].target_linear = fmaxf(targetEffectiveGain,
                1.00000048F);
        }
        else
        {
            /* '<S104>:1:96' else */
            /* '<S104>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            phaseConfig[0].current_linear = fminf(currentGain_tmp, 0.999999523F);

            /* '<S104>:1:96' else */
            /* '<S104>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            phaseConfig[0].target_linear = fminf(targetEffectiveGain,
                0.999999523F);
        }

        /* '<S104>:1:136' phaseConfig(1).target_linear    = NudgeUnity(targetEffectiveGain,isBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S104>:1:94' if (isPositive) */
        /* '<S104>:1:137' phaseConfig(1).frames           = numFrames; */
        phaseConfig[0].frames = x;

        /*  Setup Phase 2 ramper to sit at the target gain */
        /* '<S104>:1:140' phaseConfig(2).current_linear   = NudgeUnity(targetEffectiveGain,isBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S104>:1:94' if (isPositive) */
        if (currentIsBoost)
        {
            /* '<S104>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            targetImpact_db = fmaxf(targetEffectiveGain, 1.00000048F);
            phaseConfig[1].current_linear = targetImpact_db;

            /* '<S104>:1:95' adjustedLinearGain = max(linearGain, 1 + silentGain); */
            phaseConfig[1].target_linear = targetImpact_db;
        }
        else
        {
            /* '<S104>:1:96' else */
            /* '<S104>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            targetImpact_db = fminf(targetEffectiveGain, 0.999999523F);
            phaseConfig[1].current_linear = targetImpact_db;

            /* '<S104>:1:96' else */
            /* '<S104>:1:97' adjustedLinearGain = min(linearGain, 1 - silentGain); */
            phaseConfig[1].target_linear = targetImpact_db;
        }

        /* '<S104>:1:141' phaseConfig(2).target_linear    = NudgeUnity(targetEffectiveGain,isBoost,rgain_SILENT_GAIN); */
        /*  Nudge the gain values a bit if they are sitting right at one (aka unity). */
        /*  When a control set goes live, ensure that the current gain will move. */
        /*  If current gain is one, replace with a value very close to one. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. Remember that normally we */
        /*  worry about gains of 0 -- this checks instead against 1 because filter */
        /*  gain is effective gain minuus 1. */
        /* '<S104>:1:94' if (isPositive) */
        /* '<S104>:1:142' phaseConfig(2).frames           = single(0); */
        phaseConfig[1].frames = 0.0F;
    }

    /*  Step 4b: PHASES- Setup rampers for both phases */
    /* '<S104>:1:65' for currentPhase=1:2 */
    /* '<S104>:1:66' if (phaseConfig(currentPhase).frames) */
    if (phaseConfig[0].frames != 0.0F)
    {
        /* '<S104>:1:67' ramp_ms = quantum_ms * phaseConfig(currentPhase).frames; */
        /* '<S104>:1:68' factor = log(phaseConfig(currentPhase).target_linear / phaseConfig(currentPhase).current_linear); */
        /* '<S104>:1:69' RampCoef = exp(factor / (sample_rate_in_hertz * ramp_ms / 1000)); */
        BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[0].rampCoeff = expf
            (logf(phaseConfig[0].target_linear / phaseConfig[0].current_linear) /
             (quantum_ms * phaseConfig[0].frames * rtu_sample_rate_in_hertz /
              1000.0F));
    }
    else
    {
        /* '<S104>:1:70' else */
        /* '<S104>:1:71' RampCoef = single(1); */
        BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[0].rampCoeff = 1.0F;
    }

    /* '<S104>:1:73' Rampers(currentPhase).rampCoeff   = single(RampCoef); */
    /* '<S104>:1:74' Rampers(currentPhase).targetGain  = actualTargetGainLinear; */
    BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[0].targetGain =
        targetEffectiveGain;

    /*  phase 1 value ignored. */
    /* '<S104>:1:75' Rampers(currentPhase).currentGain = single(phaseConfig(currentPhase).current_linear); */
    BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[0].currentGain =
        phaseConfig[0].current_linear;

    /* '<S104>:1:76' Rampers(currentPhase).frameCount  = int32(phaseConfig(currentPhase).frames); */
    BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[0].frameCount =
        (int32_T)phaseConfig[0].frames;

    /* '<S104>:1:66' if (phaseConfig(currentPhase).frames) */
    if (phaseConfig[1].frames != 0.0F)
    {
        /* '<S104>:1:67' ramp_ms = quantum_ms * phaseConfig(currentPhase).frames; */
        /* '<S104>:1:68' factor = log(phaseConfig(currentPhase).target_linear / phaseConfig(currentPhase).current_linear); */
        /* '<S104>:1:69' RampCoef = exp(factor / (sample_rate_in_hertz * ramp_ms / 1000)); */
        BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[1].rampCoeff = expf
            (logf(phaseConfig[1].target_linear / phaseConfig[1].current_linear) /
             (quantum_ms * phaseConfig[1].frames * rtu_sample_rate_in_hertz /
              1000.0F));
    }
    else
    {
        /* '<S104>:1:70' else */
        /* '<S104>:1:71' RampCoef = single(1); */
        BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[1].rampCoeff = 1.0F;
    }

    /* '<S104>:1:73' Rampers(currentPhase).rampCoeff   = single(RampCoef); */
    /* '<S104>:1:74' Rampers(currentPhase).targetGain  = actualTargetGainLinear; */
    BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[1].targetGain =
        targetEffectiveGain;

    /*  phase 1 value ignored. */
    /* '<S104>:1:75' Rampers(currentPhase).currentGain = single(phaseConfig(currentPhase).current_linear); */
    BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[1].currentGain =
        phaseConfig[1].current_linear;

    /* '<S104>:1:76' Rampers(currentPhase).frameCount  = int32(phaseConfig(currentPhase).frames); */
    BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[1].frameCount =
        (int32_T)phaseConfig[1].frames;

    /*  unfreeze the gain. (This step now performed on canvas) */
}

/* Output and update for function-call system: '<S92>/SetDsm1' */
void BasicAudio_Target_SetDsm1_f(void)
{
    /* S-Function (fcgen): '<S95>/Function-Call Generator' incorporates:
     *  SubSystem: '<S95>/LockEnter'
     */
    BasicAudio_Target_LockEnter();

    /* S-Function (fcgen): '<S95>/Function-Call Generator' incorporates:
     *  SubSystem: '<S95>/DsmWrite'
     */
    /* DataStoreWrite: '<S114>/Data Store Write' incorporates:
     *  Constant: '<S114>/Constant'
     */
    BasicAudio_TargetControlDsms.BasicAudioTrebleFreeze = 0U;

    /* S-Function (fcgen): '<S95>/Function-Call Generator' incorporates:
     *  SubSystem: '<S95>/LockExit'
     */
    BasicAudio_Target_LockExit();

    /* End of Outputs for S-Function (fcgen): '<S95>/Function-Call Generator' */
}

/* Output and update for function-call system: '<S93>/REQ' */
void HandleReqBasicAudioTrebleSet(void)
{
    /* MATLAB Function: '<S92>/index to linear' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'RTC/REQ': '<S108>:1' */
    /* '<S108>:1:3' go(); */
    BasicAudio_Target_indextolinear(BasicAudio_Target_BasicAudio0.TrebleScale,
        BasicAudio_Target_BasicAudio0.TrebleOffset,
        BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_gain_index,
        &BasicAudio_Target_B.sf_indextolinear_ee);

    /* MATLAB Function: '<S92>/ramp parameters' */
    BasicAudio_Target_rampparameters
        (BasicAudio_Target_BasicAudio0.Trebleramptime,
         BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_ramp_milliseconds,
         BasicAudio_TargetBasicAudioTrebleSetRtcOut.REQ_ramp_db_per_second,
         BasicAudio_Target_ConstP.pooled2, BasicAudio_Target_ConstP.pooled1,
         &BasicAudio_Target_B.sf_rampparameters_ho);

    /* Outputs for Function Call SubSystem: '<S92>/SetDsm' */
    BasicAudio_Target_SetDsm_l();

    /* End of Outputs for SubSystem: '<S92>/SetDsm' */

    /* MATLAB Function: '<S92>/tone control' */
    /* MATLAB Function: '<S92>/tone control' incorporates:
     *  S-Function (AudioStats_MEX): '<S92>/Audio Stats'
     */
    BasicAudio_Target_tonecontrol_o
        (BasicAudio_Target_BasicAudio0.TrebleBoostMaximum_dB,
         BasicAudio_Target_BasicAudio0.TrebleCutMaximum_dB,
         BasicAudio_Target_B.sf_indextolinear_ee.linearGain,
         BasicAudio_Target_B.sf_rampparameters_ho.rampRate,
         BasicAudio_Target_B.sf_rampparameters_ho.ramptime, 48000.0, 32.0);

    /* Outputs for Function Call SubSystem: '<S92>/SetDsm1' */
    BasicAudio_Target_SetDsm1_f();

    /* End of Outputs for SubSystem: '<S92>/SetDsm1' */
}

/* Output and update for atomic system: '<S92>/tone' */
void BasicAudio_Target_tone_e(const real32_T rtu_boost_coeffs[3], const real32_T
    rtu_cut_coeffs[3], const real32_T rtu_audioIn[64],
    B_tone_BasicAudio_Target_h_T *localB, DW_tone_BasicAudio_Target_p_T *localDW)
{
    int32_T q0;
    int32_T qY;
    real32_T aCoeffs;
    real32_T bCoeffs_idx_0;
    real32_T bCoeffs_idx_1;
    real32_T gain;
    real32_T k;
    uint32_T ramper;

    /*     % inputs % parameters */
    /* MATLAB Function 'BasicAudio/Treble/Tone/InnerLink/tone': '<S103>:1' */
    /*  Data Store objects (registered in "Edit Data" dialog */
    /*  Keep memory for the filter persistent from frame to frame */
    /*  figure out the frame size and channel count */
    /* '<S103>:1:15' [frame_size, channel_count] = size(audioIn); */
    /* '<S103>:1:17' if (isempty(toneFilterStates)) */
    /*  set the size and datatype of the output buffer */
    /* '<S103>:1:23' audioOut = coder.nullcopy(audioIn); */
    /* '<S103>:1:25' ramper = Phase; */
    ramper = BasicAudio_TargetControlDsms.BasicAudioTreblePhase;

    /* '<S103>:1:26' ramperCount = numel(Rampers); */
    /* '<S103>:1:28' if (Freeze == false) */
    if (BasicAudio_TargetControlDsms.BasicAudioTrebleFreeze == 0U)
    {
        /* '<S103>:1:29' if (Rampers(ramper).frameCount == 0) */
        if (BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[(int32_T)
                BasicAudio_TargetControlDsms.BasicAudioTreblePhase - 1].
                frameCount == 0)
        {
            /*  The current ramper is finished. Is there another one? */
            /* '<S103>:1:31' if (ramper < ramperCount && Rampers(ramper+1).frameCount > 0) */
            if ((BasicAudio_TargetControlDsms.BasicAudioTreblePhase < 2U) &&
                    (BasicAudio_TargetControlDsms.BasicAudioTrebleRampers
                     [(int32_T)
                     BasicAudio_TargetControlDsms.BasicAudioTreblePhase].
                     frameCount > 0))
            {
                /*  Yes - move on to the next phase */
                /* '<S103>:1:33' ramper = ramper + 1; */
                qY = (int32_T)
                    (BasicAudio_TargetControlDsms.BasicAudioTreblePhase +
                     /*MW:OvSatOk*/ 1U);
                ramper = BasicAudio_TargetControlDsms.BasicAudioTreblePhase + 1U;

                /* '<S103>:1:34' Phase = ramper; */
                BasicAudio_TargetControlDsms.BasicAudioTreblePhase =
                    BasicAudio_TargetControlDsms.BasicAudioTreblePhase + 1U;

                /* '<S103>:1:35' k = Rampers(ramper).rampCoeff; */
                k = BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[qY - 1]
                    .rampCoeff;

                /* '<S103>:1:36' gain = Rampers(ramper).currentGain; */
                gain = BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[qY -
                    1].currentGain;

                /* '<S103>:1:37' Rampers(ramper).frameCount = Rampers(ramper).frameCount - 1; */
                q0 = BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[qY - 1]
                    .frameCount;
                if (q0 < -2147483647)
                {
                    BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[qY - 1]
                        .frameCount = MIN_int32_T;
                }
                else
                {
                    BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[qY - 1]
                        .frameCount = q0 - 1;
                }
            }
            else
            {
                /* '<S103>:1:38' else */
                /*   Stop ramping and SNAP to the target */
                /* '<S103>:1:40' k = single(1); */
                k = 1.0F;

                /* '<S103>:1:41' gain = Rampers(ramper).targetGain; */
                gain = BasicAudio_TargetControlDsms.BasicAudioTrebleRampers
                    [(int32_T)BasicAudio_TargetControlDsms.BasicAudioTreblePhase
                    - 1].targetGain;

                /*  Update completion flag */
            }
        }
        else
        {
            /* '<S103>:1:44' else */
            /*  The ramp is *NOT* frozen or done: gain is moving */
            /* '<S103>:1:46' k = Rampers(ramper).rampCoeff; */
            k = BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[(int32_T)
                BasicAudio_TargetControlDsms.BasicAudioTreblePhase - 1].
                rampCoeff;

            /* '<S103>:1:47' gain = Rampers(ramper).currentGain; */
            gain = BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[(int32_T)
                BasicAudio_TargetControlDsms.BasicAudioTreblePhase - 1].
                currentGain;

            /* '<S103>:1:48' Rampers(ramper).frameCount = Rampers(ramper).frameCount - 1; */
            q0 = BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[(int32_T)
                BasicAudio_TargetControlDsms.BasicAudioTreblePhase - 1].
                frameCount;
            if (q0 < -2147483647)
            {
                BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[(int32_T)
                    BasicAudio_TargetControlDsms.BasicAudioTreblePhase - 1].
                    frameCount = MIN_int32_T;
            }
            else
            {
                BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[(int32_T)
                    BasicAudio_TargetControlDsms.BasicAudioTreblePhase - 1].
                    frameCount = q0 - 1;
            }
        }
    }
    else
    {
        /* '<S103>:1:50' else */
        /*  Frozen. Stay at current gain */
        /* '<S103>:1:52' k = single(1); */
        k = 1.0F;

        /* '<S103>:1:53' gain = Rampers(ramper).currentGain; */
        gain = BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[(int32_T)
            BasicAudio_TargetControlDsms.BasicAudioTreblePhase - 1].currentGain;
    }

    /*  Apply the tone filter */
    /* '<S103>:1:57' if (gain < 1) */
    if (gain < 1.0F)
    {
        /*  Cut */
        /* '<S103>:1:59' bCoeffs = cut_coeffs(1:2); */
        bCoeffs_idx_0 = rtu_cut_coeffs[0];
        bCoeffs_idx_1 = rtu_cut_coeffs[1];

        /* '<S103>:1:60' aCoeffs = cut_coeffs(3); */
        aCoeffs = rtu_cut_coeffs[2];
    }
    else
    {
        /* '<S103>:1:61' else */
        /*  Boost */
        /* '<S103>:1:63' bCoeffs = boost_coeffs(1:2); */
        bCoeffs_idx_0 = rtu_boost_coeffs[0];
        bCoeffs_idx_1 = rtu_boost_coeffs[1];

        /* '<S103>:1:64' aCoeffs = boost_coeffs(3); */
        aCoeffs = rtu_boost_coeffs[2];
    }

    /*  [filtered,toneFilterStates] = filter(bCoeffs, aCoeffs, audioIn, toneFilterStates); */
    /*  Loop through samples, adjusting gain and applying to channels */
    /* '<S103>:1:70' for n=1:frame_size */
    for (q0 = 0; q0 < 32; q0++)
    {
        real32_T filterOut;
        real32_T filterOut_tmp;

        /*  Update the gain and then subtract one before applying it to the */
        /*  filtered signal. Disregarding phase shifting  */
        /* '<S103>:1:73' gain = gain * k; */
        gain *= k;

        /* '<S103>:1:74' filterGain = gain - 1; */
        /*  Loop through channels adding in gain-adjusted filtered samples */
        /* '<S103>:1:76' for chanIdx = 1:channel_count */
        /* '<S103>:1:78' filterOut = (bCoeffs(1) * audioIn(n,chanIdx)) + toneFilterStates(chanIdx); */
        filterOut = bCoeffs_idx_0 * rtu_audioIn[q0] + localDW->toneFilterStates
            [0];

        /* '<S103>:1:79' toneFilterStates(chanIdx) = (bCoeffs(2) * audioIn(n,chanIdx)) + (aCoeffs(1) * filterOut); */
        localDW->toneFilterStates[0] = bCoeffs_idx_1 * rtu_audioIn[q0] + aCoeffs
            * filterOut;

        /* '<S103>:1:81' audioOut(n,chanIdx) = audioIn(n,chanIdx) + (filterOut * filterGain); */
        localB->audioOut[q0] = (gain - 1.0F) * filterOut + rtu_audioIn[q0];

        /* '<S103>:1:78' filterOut = (bCoeffs(1) * audioIn(n,chanIdx)) + toneFilterStates(chanIdx); */
        filterOut_tmp = rtu_audioIn[q0 + 32];
        filterOut = filterOut_tmp * bCoeffs_idx_0 + localDW->toneFilterStates[1];

        /* '<S103>:1:79' toneFilterStates(chanIdx) = (bCoeffs(2) * audioIn(n,chanIdx)) + (aCoeffs(1) * filterOut); */
        localDW->toneFilterStates[1] = filterOut_tmp * bCoeffs_idx_1 + aCoeffs *
            filterOut;

        /* '<S103>:1:81' audioOut(n,chanIdx) = audioIn(n,chanIdx) + (filterOut * filterGain); */
        localB->audioOut[q0 + 32] = (gain - 1.0F) * filterOut + filterOut_tmp;
    }

    /* '<S103>:1:85' Rampers(ramper).currentGain = gain; */
    BasicAudio_TargetControlDsms.BasicAudioTrebleRampers[(int32_T)ramper - 1].
        currentGain = gain;
}

/* Output and update for function-call system: '<S119>/lookup db from index' */
void BasicAudio_Target_lookupdbfromindex(const uint32_T rtu_volTableIdx[8],
    const real32_T rtu_volTabledB[8], uint32_T rtu_idx, int32_T rtu_db_offset,
    real32_T rtu_maxgain, B_lookupdbfromindex_BasicAudio_Target_T *localB)
{
    int32_T db_offset;
    int32_T tmp;
    real32_T percent;
    uint32_T idx;
    uint8_T j;
    boolean_T exitg1;
    idx = rtu_idx;
    db_offset = rtu_db_offset;

    /*  */
    /*  Lookup volume table  */
    /*  Output Gain vlue from table */
    /*  */
    /* MATLAB Function 'BasicAudio/Volume/volume/InnerLink/Index2DbTable/lookup db from index': '<S127>:1' */
    /* '<S127>:1:8' j = uint8(1); */
    j = 1U;

    /* '<S127>:1:9' gain_value = single(0.0); */
    localB->gain_value = 0.0F;

    /* '<S127>:1:10' interpolated_gain_dB=single(0.0); */
    /* '<S127>:1:11' interpolated_gain_dB_with_offset=single(0.0); */
    /*  workaround to type problem */
    /* '<S127>:1:14' idx = cast(idx, class(volTableIdx(end))); */
    /*   bounds checking for vol - clip to max idx */
    /* '<S127>:1:17' if (idx > volTableIdx(end)) */
    if (rtu_idx > rtu_volTableIdx[7])
    {
        /* '<S127>:1:18' idx = volTableIdx(end); */
        idx = rtu_volTableIdx[7];
    }

    /*   bounds checking for gain offset(No Boost) - clip to max offset level */
    /* '<S127>:1:21' if (db_offset > int32(maxgain)) */
    percent = roundf(rtu_maxgain);
    if (percent < 2.14748365E+9F)
    {
        if (percent >= -2.14748365E+9F)
        {
            tmp = (int32_T)percent;
        }
        else
        {
            tmp = MIN_int32_T;
        }
    }
    else
    {
        tmp = MAX_int32_T;
    }

    if (rtu_db_offset > tmp)
    {
        /* '<S127>:1:22' db_offset = int32(maxgain); */
        if (percent < 2.14748365E+9F)
        {
            if (percent >= -2.14748365E+9F)
            {
                db_offset = (int32_T)percent;
            }
            else
            {
                db_offset = MIN_int32_T;
            }
        }
        else
        {
            db_offset = MAX_int32_T;
        }
    }

    /* '<S127>:1:24' if (db_offset < int32(-10.0)) */
    if (db_offset < -10)
    {
        /* '<S127>:1:25' db_offset = int32(-10.0); */
        db_offset = -10;
    }

    /*  search through volume table to find index closest to vol */
    /* '<S127>:1:29' while (j <= numel(volTableIdx)) */
    exitg1 = false;
    while ((!exitg1) && (j <= 8))
    {
        uint32_T q0;

        /* '<S127>:1:30' if (idx <= volTableIdx(j)) */
        q0 = rtu_volTableIdx[j - 1];
        if (idx <= q0)
        {
            /* '<S127>:1:31' if (j <= 1) */
            if (j <= 1)
            {
                /*  first indexed value is linearly interpolated to zero */
                /* '<S127>:1:32' if (volTableIdx(j)) */
                if (rtu_volTableIdx[0] != 0U)
                {
                    /* '<S127>:1:33' percent = single(idx) / single(volTableIdx(j)); */
                    percent = (real32_T)idx / (real32_T)rtu_volTableIdx[0];
                }
                else
                {
                    /* '<S127>:1:34' else */
                    /* '<S127>:1:35' percent = single(1); */
                    percent = 1.0F;
                }

                /* '<S127>:1:37' gain_value = percent * 10 ^ ( (volTabledB(j)+ single(db_offset)) / 20); */
                localB->gain_value = powf(10.0F, (rtu_volTabledB[0] + (real32_T)
                    db_offset) / 20.0F) * percent;
            }
            else
            {
                uint32_T qY;
                uint32_T qY_tmp;

                /* '<S127>:1:38' else */
                /*  other values are interpolated in db space */
                /* '<S127>:1:39' lastIdx = volTableIdx(j-1); */
                /* '<S127>:1:40' percent = single(idx - lastIdx) / single(volTableIdx(j) - lastIdx); */
                /* '<S127>:1:41' interpolated_gain_dB = (percent * (volTabledB(j) - volTabledB(j-1) ) ) + volTabledB(j-1); */
                /* '<S127>:1:42' interpolated_gain_dB_with_offset= interpolated_gain_dB + single(db_offset); */
                /* '<S127>:1:43' gain_value = 10 ^ ( (interpolated_gain_dB_with_offset) / 20); */
                qY_tmp = rtu_volTableIdx[j - 2];
                qY = idx - /*MW:OvSatOk*/ qY_tmp;
                if (qY > idx)
                {
                    qY = 0U;
                }

                idx = q0 - /*MW:OvSatOk*/ qY_tmp;
                if (idx > q0)
                {
                    idx = 0U;
                }

                percent = rtu_volTabledB[j - 2];
                localB->gain_value = powf(10.0F, (((rtu_volTabledB[j - 1] -
                    percent) * ((real32_T)qY / (real32_T)idx) + percent) +
                    (real32_T)db_offset) / 20.0F);
            }

            exitg1 = true;
        }
        else
        {
            /* '<S127>:1:46' else */
            /* '<S127>:1:47' j = j + 1; */
            j++;
        }
    }
}

/* Output and update for function-call system: '<S118>/ramp parameters' */
void BasicAudio_Target_rampparameters_o(uint32_T rtu_rampRateRTC, uint32_T
    rtu_rampTimeRTC, real32_T rtu_rampTimeTune,
    B_rampparameters_BasicAudio_Target_m_T *localB)
{
    real32_T y;
    y = rtu_rampTimeTune;

    /*  % inputs */
    /*  parameters */
    /* MATLAB Function 'BasicAudio/Volume/volume/InnerLink/ramp parameters': '<S124>:1' */
    /*  Ramp time and rate are provided to the block through RTC messages. However  */
    /*  in case neither of them are provided, the block will use the default ramp  */
    /*  time given in the tune variables. */
    /* '<S124>:1:9' ramptime = single(rampTimeTune); */
    /* '<S124>:1:10' rampRate = single(0); */
    localB->rampRate = 0.0F;

    /* '<S124>:1:12' if (rampRateRTC) */
    if (rtu_rampRateRTC != 0U)
    {
        /* '<S124>:1:13' rampRate = single(rampRateRTC); */
        /* '<S124>:1:14' rampRate = max(rampRateBounds(1), rampRate); */
        /* '<S124>:1:15' rampRate = min(rampRateBounds(2), rampRate); */
        localB->rampRate = fminf(50000.0F, fmaxf(5.0F, (real32_T)rtu_rampRateRTC));
    }

    /* '<S124>:1:17' if (rampTimeRTC) */
    if (rtu_rampTimeRTC != 0U)
    {
        /* '<S124>:1:18' ramptime = single(rampTimeRTC); */
        y = (real32_T)rtu_rampTimeRTC;
    }

    /* '<S124>:1:20' ramptime = max(rampTimeBounds(1), ramptime); */
    /* '<S124>:1:21' ramptime = min(rampTimeBounds(2), ramptime); */
    localB->ramptime = fminf(3000.0F, fmaxf(1.0F, y));
}

/* Output and update for function-call system: '<S135>/SetDsm' */
void BasicAudio_Target_SetDsm_f(void)
{
    /* S-Function (fcgen): '<S138>/Function-Call Generator' incorporates:
     *  SubSystem: '<S138>/LockEnter'
     */
    BasicAudio_Target_LockEnter();

    /* S-Function (fcgen): '<S138>/Function-Call Generator' incorporates:
     *  SubSystem: '<S138>/DsmWrite'
     */
    /* DataStoreWrite: '<S141>/Data Store Write' incorporates:
     *  Constant: '<S141>/Constant'
     */
    BasicAudio_TargetControlDsms.BasicAudioVolumeFreeze = 1U;

    /* S-Function (fcgen): '<S138>/Function-Call Generator' incorporates:
     *  SubSystem: '<S138>/LockExit'
     */
    BasicAudio_Target_LockExit();

    /* End of Outputs for S-Function (fcgen): '<S138>/Function-Call Generator' */
}

/* Output and update for function-call system: '<S135>/rgainy ramper control' */
void BasicAudio_Target_rgainyrampercontrol(real32_T rtu_target_gains_linear,
    real32_T rtu_db_per_sec, real32_T rtu_ramp_times_ms, real32_T
    rtu_sample_rate_in_hertz, real32_T rtu_frame_size)
{
    int32_T ramper;
    real32_T quantum_ms;
    real32_T targetGain;

    /*  Dimensions of target_gains_linear must match the number of rampers.  */
    /*  */
    /*  db_per_sec takes precidence: */
    /*  If db_per_sec has a non-zero value then any ramp_times_ms values are */
    /*  ignored and the db_per_sec is used to determine ramper timing. */
    /*  */
    /*  The dimension of ramp_times_ms is flexible and does not need to match the */
    /*  ramper count. Its values are assigned to rampers in rotating order. If */
    /*  there are 5 rampers and only two times provided, then rampers 1,3, and 5 */
    /*  get the first ramp time and rampers 2 and 4 receive the second ramp time. */
    /*  Any extra ramp times (beyond the number of rampers) are silently ignored. */
    /* MATLAB Function 'Rgainy/ControlSubsystem/rgainy ramper control': '<S140>:1' */
    /* '<S140>:1:20' rgain_SILENT_GAIN = RgainBusUtil.GetSilentGainLinear(); */
    /*  Get the sizes of the provided target and time specifications */
    /* '<S140>:1:23' numGains = numel(target_gains_linear); */
    /* '<S140>:1:24' numTimes = numel(ramp_times_ms); */
    /* '<S140>:1:25' numdbpersecs=numel(db_per_sec); */
    /*  Step 1: FREEZE the gain ramper so that we can perform calculations based */
    /*  on the current gain value. (This step is done outside on canvas now) */
    /* '<S140>:1:30' for ramper=1:MaskNumRampers */
    targetGain = fmaxf(rtu_target_gains_linear, 5.0118723E-7F);
    quantum_ms = 1000.0F * rtu_frame_size / rtu_sample_rate_in_hertz;
    BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.targetGain =
        rtu_target_gains_linear;
    for (ramper = 0; ramper < 1; ramper++)
    {
        real32_T currentGain;
        real32_T numFrames;

        /*  assign gains and ramp times */
        /* '<S140>:1:33' targetGain = single(target_gains_linear(mod(ramper-1,numGains)+1)); */
        /* '<S140>:1:34' ramp_ms    = single(ramp_times_ms      (mod(ramper-1,numTimes)+1)); */
        numFrames = rtu_ramp_times_ms;

        /* '<S140>:1:35' ramp_dbPerSec= single(db_per_sec      (mod(ramper-1,numdbpersecs)+1)); */
        /*  Step 2: Nudge the gain values a bit if they are sitting at zero. */
        /*  When this control set goes live, ensure that the current gain will move. */
        /*  If current gain is zero, replace with a value very close to zero. What we */
        /*  really want is a value which is on the edge of perception - not too small */
        /*  or large. If the value is too small, we'll waste some of our ramp time */
        /*  in perceived silence. If the value is too large, the audio will lurch */
        /*  into perceptability at the start of the ramp. */
        /* '<S140>:1:44' currentGain = max(Rampers(ramper).currentGain, rgain_SILENT_GAIN); */
        currentGain = fmaxf
            (BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.currentGain,
             5.0118723E-7F);

        /* '<S140>:1:45' targetGain  = max(targetGain, rgain_SILENT_GAIN); */
        /*  Step 3a: If a slope in dB/second is provided, ignore the ramp time */
        /*  and instead calculate a new ramp time that matches the slope. */
        /* '<S140>:1:49' if (ramp_dbPerSec) */
        if (rtu_db_per_sec != 0.0F)
        {
            /* '<S140>:1:50' currentGain_db = 20*log10(currentGain); */
            /* '<S140>:1:51' targetGain_db = 20*log10(targetGain); */
            /* '<S140>:1:52' diff = abs(currentGain_db - targetGain_db); */
            /* '<S140>:1:53' ramp_ms = diff / single(ramp_dbPerSec) * 1000; */
            numFrames = fabsf(20.0F * log10f(currentGain) - 20.0F * log10f
                              (targetGain)) / rtu_db_per_sec * 1000.0F;
        }

        /*  Step 3b: QUANTIZE the desired ramp time to our FrameSize. Also retrieve  */
        /*  the whole number of frames required to best match requested ramp time. */
        /*  Ensure that at least one frame is used. The frame size will therefore */
        /*  determine the steepest ramp (i.e. within one frame you can go anywhere). */
        /* '<S140>:1:60' quantum_ms = 1000 * frame_size / sample_rate_in_hertz; */
        /* '<S140>:1:61' numFrames = ramp_ms / quantum_ms; */
        /*  Do a ceil rather than a round to insure that we create enough */
        /*  frames. i.e. a fraction of a frame is promoted to an addition frame */
        /*  numFrames = max (1, round(numFrames)); */
        /* '<S140>:1:65' numFrames = max (1, ceil(numFrames)); */
        numFrames = fmaxf(1.0F, ceilf(numFrames / quantum_ms));

        /* '<S140>:1:66' quantized_rampTime_ms = numFrames * quantum_ms; */
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
        /* '<S140>:1:80' factor = log( targetGain / currentGain ); */
        /* '<S140>:1:81' RampCoef = exp(factor / (sample_rate_in_hertz * quantized_rampTime_ms / 1000)); */
        BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.rampCoeff = expf
            (logf(targetGain / currentGain) / (numFrames * quantum_ms *
              rtu_sample_rate_in_hertz / 1000.0F));

        /*  Output control signals */
        /* '<S140>:1:84' Rampers(ramper).frameCount  = int32(numFrames); */
        BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.frameCount =
            (int32_T)numFrames;

        /* '<S140>:1:85' Rampers(ramper).rampCoeff   = single(RampCoef); */
        /* '<S140>:1:86' Rampers(ramper).targetGain  = target_gains_linear(ramper); */
        /* '<S140>:1:87' Rampers(ramper).currentGain = currentGain; */
        BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.currentGain =
            currentGain;
    }

    /*  Step 4: UNFREEZE the gain ramper (performed outside on canvas now) */
}

/* Output and update for function-call system: '<S135>/SetDsm1' */
void BasicAudio_Target_SetDsm1_g(void)
{
    /* S-Function (fcgen): '<S139>/Function-Call Generator' incorporates:
     *  SubSystem: '<S139>/LockEnter'
     */
    BasicAudio_Target_LockEnter();

    /* S-Function (fcgen): '<S139>/Function-Call Generator' incorporates:
     *  SubSystem: '<S139>/DsmWrite'
     */
    /* DataStoreWrite: '<S144>/Data Store Write' incorporates:
     *  Constant: '<S144>/Constant'
     */
    BasicAudio_TargetControlDsms.BasicAudioVolumeFreeze = 0U;

    /* S-Function (fcgen): '<S139>/Function-Call Generator' incorporates:
     *  SubSystem: '<S139>/LockExit'
     */
    BasicAudio_Target_LockExit();

    /* End of Outputs for S-Function (fcgen): '<S139>/Function-Call Generator' */
}

/* Output and update for function-call system: '<S120>/REQ' */
void HandleReqBasicAudioVolumeSet(void)
{
    /* MATLAB Function: '<S119>/lookup db from index' */
    /*  Invoke the function trigger output */
    /* MATLAB Function 'RTC/REQ': '<S131>:1' */
    /* '<S131>:1:3' go(); */
    BasicAudio_Target_lookupdbfromindex
        (&(BasicAudio_Target_BasicAudio0.VolumeTable_Idx[0]),
         &(BasicAudio_Target_BasicAudio0.VolumeTable_dB[0]),
         BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_gain_index,
         BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_gain_offset,
         BasicAudio_Target_BasicAudio0.Volumemaxgain,
         &BasicAudio_Target_B.sf_lookupdbfromindex);

    /* MATLAB Function: '<S118>/ramp parameters' */
    BasicAudio_Target_rampparameters_o
        (BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_ramp_db_per_second,
         BasicAudio_TargetBasicAudioVolumeSetRtcOut.REQ_ramp_milliseconds,
         BasicAudio_Target_BasicAudio0.VolumeRampTime,
         &BasicAudio_Target_B.sf_rampparameters_o);

    /* Outputs for Function Call SubSystem: '<S135>/SetDsm' */
    BasicAudio_Target_SetDsm_f();

    /* End of Outputs for SubSystem: '<S135>/SetDsm' */

    /* MATLAB Function: '<S135>/rgainy ramper control' */
    /* MATLAB Function: '<S135>/rgainy ramper control' incorporates:
     *  S-Function (AudioStats_MEX): '<S135>/Audio Stats'
     */
    BasicAudio_Target_rgainyrampercontrol
        (BasicAudio_Target_B.sf_lookupdbfromindex.gain_value,
         BasicAudio_Target_B.sf_rampparameters_o.rampRate,
         BasicAudio_Target_B.sf_rampparameters_o.ramptime, 48000.0, 32.0);

    /* Outputs for Function Call SubSystem: '<S135>/SetDsm1' */
    BasicAudio_Target_SetDsm1_g();

    /* End of Outputs for SubSystem: '<S135>/SetDsm1' */
}

/* Output and update for atomic system: '<S134>/rgainy process' */
void BasicAudio_Target_rgainyprocess(const uint8_T rtu_rampToChanMap[3], const
    real32_T rtu_audioIn[64], B_rgainyprocess_BasicAudio_Target_T *localB)
{
    int32_T b_tmp;
    int32_T chanCount;
    int32_T n;
    real32_T gain;
    real32_T k;
    uint8_T d;
    uint8_T noRampChannel;

    /* MATLAB Function 'Rgainy/AudioRateSubsystem/rgainy process': '<S137>:1' */
    /*  set the size of the output buffer */
    /* '<S137>:1:8' audioOut = coder.nullcopy(audioIn); */
    /* '<S137>:1:10' [frame_size,channel_count] = size(audioIn); */
    /* '<S137>:1:11' gainOut = single(ones(1,channel_count)); */
    localB->gainOut[0] = 1.0F;
    localB->gainOut[1] = 1.0F;

    /*  Channel indexes start in the map just after the ramper channel counts. */
    /*  Create an iterator to walk the channel indexes in rampToChanMap */
    /*  (set its value to "point" just *before* the first channel index, since */
    /*  we'll add a channel count to it during use below) */
    /* '<S137>:1:17' chanIterator = cast(MaskNumRampers,'like',rampToChanMap); */
    /*  Loop through rampers and apply their gain coefficients */
    /* '<S137>:1:20' for ramper = 1:MaskNumRampers */
    /* '<S137>:1:21' k = single(1); */
    k = 1.0F;

    /* '<S137>:1:22' gain = Rampers(ramper).currentGain; */
    gain = BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.currentGain;

    /* '<S137>:1:23' if (Freeze(1) == false) */
    if (BasicAudio_TargetControlDsms.BasicAudioVolumeFreeze == 0U)
    {
        /* '<S137>:1:24' if (Rampers(ramper).frameCount == 0) */
        if (BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.frameCount == 0)
        {
            /*  The ramp is finished. Stop ramping and SNAP to the target */
            /* '<S137>:1:26' gain = Rampers(ramper).targetGain; */
            gain =
                BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.targetGain;
        }
        else
        {
            /* '<S137>:1:27' else */
            /*  The ramp is *NOT* frozen or done: gain is moving */
            /* '<S137>:1:29' k = Rampers(ramper).rampCoeff; */
            k = BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.rampCoeff;

            /* '<S137>:1:30' Rampers(ramper).frameCount = Rampers(ramper).frameCount - 1; */
            BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.frameCount =
                BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.frameCount
                - 1;
        }
    }

    /*  Loop through samples, adjusting gain and applying to channels */
    /* '<S137>:1:34' for n=1:frame_size */
    b_tmp = rtu_rampToChanMap[0];
    for (n = 0; n < 32; n++)
    {
        /* '<S137>:1:35' gain = gain * k; */
        gain *= k;

        /*  Loop through channels associated with this ramper */
        /* '<S137>:1:37' for chanCount = 1:rampToChanMap(ramper) */
        for (chanCount = 0; chanCount < b_tmp; chanCount++)
        {
            int32_T audioOut_tmp;

            /* '<S137>:1:38' chanIdx = rampToChanMap(chanIterator + chanCount); */
            /* '<S137>:1:39' audioOut(n,chanIdx) = audioIn(n,chanIdx) * gain; */
            audioOut_tmp = ((rtu_rampToChanMap[(uint8_T)((uint32_T)chanCount +
                              2U) - 1] - 1) << 5) + n;
            localB->audioOut[audioOut_tmp] = rtu_audioIn[audioOut_tmp] * gain;
        }
    }

    /* '<S137>:1:42' for chanCount = 1:rampToChanMap(ramper) */
    for (n = 0; n < b_tmp; n++)
    {
        /* '<S137>:1:43' chanIdx = rampToChanMap(chanIterator + chanCount); */
        /* '<S137>:1:44' gainOut(chanIdx) = gain; */
        localB->gainOut[rtu_rampToChanMap[(uint8_T)((uint32_T)n + 2U) - 1] - 1] =
            gain;
    }

    /* '<S137>:1:46' Rampers(ramper).currentGain = gain; */
    BasicAudio_TargetControlDsms.BasicAudioVolumeRampers.currentGain = gain;

    /* '<S137>:1:47' chanIterator = chanIterator + rampToChanMap(ramper); */
    /*  handle any channels that are not assigned to a ramper (i.e. no gain) */
    /* '<S137>:1:50' for noRampChannel=(chanIterator+1):(channel_count+MaskNumRampers) */
    d = (uint8_T)(rtu_rampToChanMap[0] + 2U);
    for (noRampChannel = d; noRampChannel < 4; noRampChannel++)
    {
        uint8_T chanIdx;

        /* '<S137>:1:51' chanIdx = rampToChanMap(noRampChannel); */
        chanIdx = rtu_rampToChanMap[noRampChannel - 1];

        /* '<S137>:1:52' for n=1:frame_size */
        /* '<S137>:1:53' audioOut(n,chanIdx) = audioIn(n,chanIdx); */
        memcpy(&localB->audioOut[(chanIdx << 5) + -32], &rtu_audioIn[(chanIdx <<
                5) + -32], sizeof(real32_T) << 5U);
    }
}

/* Function for MATLAB Function: '<S11>/balanceProcess' */
static void BasicAudio_Target_eml_find(const boolean_T x[2], int32_T i_data[],
    int32_T i_size[2])
{
    int32_T idx;
    int32_T ii;
    boolean_T exitg1;
    idx = 0;
    i_size[0] = 1;
    ii = 0;
    exitg1 = false;
    while ((!exitg1) && (ii < 2))
    {
        if (x[ii])
        {
            idx++;
            i_data[idx - 1] = ii + 1;
            if (idx >= 2)
            {
                exitg1 = true;
            }
            else
            {
                ii++;
            }
        }
        else
        {
            ii++;
        }
    }

    if (idx < 1)
    {
        i_size[1] = 0;
    }
    else
    {
        i_size[1] = idx;
    }
}

/* Model step function */
void BasicAudio_Target_step(void)
{
    /* local scratch DWork variables */
    int32_T ForEach_itr;
    int32_T d_data[2];
    int32_T d_size[2];
    int32_T b;
    int32_T ch;
    int32_T i;
    int32_T q0;
    real32_T rampCoeffMultipliers[32];
    real32_T rtb_Merge[32];
    real32_T gain;
    real32_T rtb_Max;
    boolean_T tmp[2];

    /* Outputs for Atomic SubSystem: '<Root>/Input' */
    /* SignalConversion: '<S148>/Numerical Unity' incorporates:
     *  Inport: '<Root>/Inport'
     *  Switch: '<S148>/Environment Switch'
     */
    memcpy(&BasicAudio_Target_B.NumericalUnity[0], &UnitTest_Target_AudioIn[0],
           sizeof(real32_T) << 6U);

    /* End of Outputs for S-Function (RTC_REQ_RSP): '<S120>/RTC Request Response Trigger' */
    /* End of Outputs for S-Function (RTC_NTF): '<S120>/NTF Trigger' */
    /* MATLAB Function: '<S134>/rgainy process' */
    BasicAudio_Target_rgainyprocess(BasicAudio_Target_ConstB.AudioStatsFunc,
        BasicAudio_Target_B.NumericalUnity,
        &BasicAudio_Target_B.sf_rgainyprocess);

    /* M-S-Function: '<S118>/FeaturePath' */
    /*  Rgainy outputs gains of all channels. In the entertainment path, we only */
    /*  have one ramper meaning that the same gain gets applied to all channels. */
    /*  The following code extracts the gain of just the first channel as output. */
    /* MATLAB Function 'BasicAudio/Volume/volume/InnerLink/select one channel gain': '<S125>:1' */
    /* '<S125>:1:5' gainSingleChannel = squeeze(gainIn(:,1,:)); */
    /* End of Outputs for S-Function (RTC_REQ_RSP): '<S12>/RTC Request Response Trigger' */
    /* End of Outputs for S-Function (RTC_NTF): '<S12>/NTF Trigger' */
    /* MATLAB Function: '<S11>/balanceProcess' incorporates:
     *  S-Function (TOP_MEX): '<S11>/TOP'
     */
    /*  % inputs */
    /*  parameters */
    /* MATLAB Function 'BasicAudio/Balance/balance/InnerLink/balanceProcess': '<S22>:1' */
    /*  Applies the Left and Right gains to the respective channels via multiple */
    /*    rampers */
    /*  Inputs: */
    /*   audioIn: Input audio */
    /*   channelToRamperMap: Input channels mapped to their corresponding ramper. */
    /*   MaskNumRampers: Number of rampers (set in the Mask file) = 4 */
    /*  Data Store objects (registered in "Edit Data" dialog) */
    /*  Figure out the frame size, channel count and number of rampers */
    /* '<S22>:1:16' [frameSize, ~] = size(audioIn); */
    /*  set the size and datatype of the output buffer */
    /* '<S22>:1:19' audioOut = coder.nullcopy(audioIn); */
    /*    Every channel needs a ramper, but every ramper does not need to be on a */
    /*    channel necessarily. */
    /* '<S22>:1:23' for ramper = 1:MaskNumRampers */
    for (i = 0; i < 4; i++)
    {
        /*  Get the gains */
        /* '<S22>:1:25' gain = Rampers(ramper).currentGain; */
        gain = BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[i].
            currentGain;

        /* '<S22>:1:26' rampCoeffMultipliers = ones(1, frameSize, 'single'); */
        for (q0 = 0; q0 < 32; q0++)
        {
            rampCoeffMultipliers[q0] = 1.0F;
        }

        /* '<S22>:1:27' if (Freeze(1) == false) */
        if (BasicAudio_TargetControlDsms.BasicAudioBalanceFreeze == 0U)
        {
            /* '<S22>:1:28' if (Rampers(ramper).frameCount == 0) */
            if (BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[i].
                    frameCount == 0)
            {
                /*  The ramp is finished. Stop ramping and SNAP to the target */
                /* '<S22>:1:30' gain = Rampers(ramper).targetGain; */
                gain = BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[i].
                    targetGain;
            }
            else
            {
                /* '<S22>:1:31' else */
                /*  The ramp is *NOT* frozen or done: gain is moving */
                /* '<S22>:1:33' k = Rampers(ramper).rampCoeff; */
                /* '<S22>:1:34' rampCoeffMultipliers = (k * rampCoeffMultipliers) .^ (1:frameSize); */
                for (q0 = 0; q0 < 32; q0++)
                {
                    rampCoeffMultipliers[q0] = powf
                        (BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[i]
                         .rampCoeff, (real32_T)((real_T)q0 + 1.0));
                }

                /* '<S22>:1:35' Rampers(ramper).frameCount = Rampers(ramper).frameCount - 1; */
                q0 = BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[i].
                    frameCount;
                if (q0 < -2147483647)
                {
                    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[i].
                        frameCount = MIN_int32_T;
                }
                else
                {
                    BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[i].
                        frameCount = q0 - 1;
                }
            }
        }

        /*  Apply the gains to the corresponding channels via the Mask parameter */
        /*  and tuneTop paramter "channelToRamperMap".   */
        /* '<S22>:1:41' currentChannelGroup = find(channelToRamperMap == ramper); */
        tmp[0] = ((real_T)(i + 1) ==
                  BasicAudio_Target_BasicAudio0.BalanceChanRampMap[0]);
        tmp[1] = ((real_T)(i + 1) ==
                  BasicAudio_Target_BasicAudio0.BalanceChanRampMap[1]);
        BasicAudio_Target_eml_find(tmp, d_data, d_size);

        /* '<S22>:1:42' for ch = 1:numel(currentChannelGroup) */
        b = d_size[1];
        for (ch = 0; ch < b; ch++)
        {
            int32_T d_tmp;

            /* '<S22>:1:43' audioOut(:,currentChannelGroup(ch)) = (gain * rampCoeffMultipliers)' .* audioIn(:,currentChannelGroup(ch)); */
            d_tmp = d_data[ch];
            for (q0 = 0; q0 < 32; q0++)
            {
                BasicAudio_Target_B.audioOut[q0 + ((d_tmp - 1) << 5)] =
                    BasicAudio_Target_B.sf_rgainyprocess.audioOut[((d_tmp - 1) <<
                    5) + q0] * (gain * rampCoeffMultipliers[q0]);
            }
        }

        /* '<S22>:1:45' Rampers(ramper).currentGain = gain * rampCoeffMultipliers(end); */
        BasicAudio_TargetControlDsms.BasicAudioBalanceRampers[i].currentGain =
            gain * rampCoeffMultipliers[31];
    }

    /*  If ramper value of -1 is encountered, this indicates an unused channel. */
    /*  Assign values of zero to all samples of these channels. */
    /* '<S22>:1:50' unusedChannelGroup = find(channelToRamperMap == -1); */
    tmp[0] = (BasicAudio_Target_BasicAudio0.BalanceChanRampMap[0] == -1.0F);
    tmp[1] = (BasicAudio_Target_BasicAudio0.BalanceChanRampMap[1] == -1.0F);
    BasicAudio_Target_eml_find(tmp, d_data, d_size);

    /* '<S22>:1:51' for unusedCh = 1:numel(unusedChannelGroup) */
    i = d_size[1];
    for (b = 0; b < i; b++)
    {
        /* '<S22>:1:52' audioOut(:,unusedChannelGroup(unusedCh)) = 0; */
        q0 = d_data[b];
        memset(&BasicAudio_Target_B.audioOut[(q0 << 5) + -32], 0, sizeof
               (real32_T) << 5U);
    }

    /* End of MATLAB Function: '<S11>/balanceProcess' */
    /* Outputs for Atomic SubSystem: '<S1>/Mute' */
    /* DataStoreRead: '<S68>/Data Store Read1' */
    gain = BasicAudio_TargetControlDsms.BasicAudioMuteTargetLinearGain;

    /* End of Outputs for S-Function (RTC_REQ_RSP): '<S69>/RTC Request Response Trigger' */

    /* Outputs for Atomic SubSystem: '<S68>/Ramping_Subsystem' */
    /* MinMax: '<S82>/Max' incorporates:
     *  Constant: '<S82>/minGain'
     *  DataStoreRead: '<S68>/Data Store Read'
     */
    rtb_Max = fmaxf(BasicAudio_TargetControlDsms.BasicAudioMuteCurrentGain,
                    4.7315126E-7F);

    /* If: '<S82>/If' incorporates:
     *  Abs: '<S82>/Abs3'
     *  Constant: '<S68>/snapThresh'
     *  DataStoreRead: '<S82>/frames'
     *  Sum: '<S82>/Add'
     */
    if ((fabsf(gain - rtb_Max) > 1.0E-9F) &&
            (BasicAudio_TargetControlDsms.BasicAudioMuteNframes > 0.0F))
    {
        /* Outputs for IfAction SubSystem: '<S82>/ifActionSubsystem' incorporates:
         *  ActionPort: '<S85>/Action Port'
         */
        /* DataStoreWrite: '<S85>/Data Store Write' incorporates:
         *  Constant: '<S85>/Constant1'
         *  Sum: '<S85>/Add'
         */
        BasicAudio_TargetControlDsms.BasicAudioMuteNframes =
            BasicAudio_TargetControlDsms.BasicAudioMuteNframes - 1.0F;

        /* If: '<S87>/If' incorporates:
         *  Delay: '<S87>/Delay'
         *  DiscreteFilter: '<S85>/ramping_filter'
         *  Sum: '<S87>/Add2'
         */
        if (gain - BasicAudio_Target_DW.Delay_DSTATE != 0.0F)
        {
            BasicAudio_Target_DW.ramping_filter_icLoad = 1U;
        }

        /* End of If: '<S87>/If' */

        /* DiscreteFilter: '<S85>/ramping_filter' incorporates:
         *  Merge: '<S82>/Merge'
         *  Product: '<S85>/Matrix Multiply'
         *  Product: '<S86>/Multiply'
         */
        if (BasicAudio_Target_DW.ramping_filter_icLoad != 0)
        {
            BasicAudio_Target_DW.ramping_filter_states =
                BasicAudio_Target_B.ramping_coeff * rtb_Max;
        }

        i = 0;
        for (ch = 0; ch < 32; ch++)
        {
            rtb_Merge[i] = BasicAudio_Target_DW.ramping_filter_states;
            BasicAudio_Target_DW.ramping_filter_states = 0.0F -
                -BasicAudio_Target_B.ramping_coeff * rtb_Merge[i];
            i++;
        }

        /* Update for Delay: '<S87>/Delay' */
        BasicAudio_Target_DW.Delay_DSTATE = gain;

        /* Update for DiscreteFilter: '<S85>/ramping_filter' */
        BasicAudio_Target_DW.ramping_filter_icLoad = 0U;

        /* End of Outputs for SubSystem: '<S82>/ifActionSubsystem' */
    }
    else
    {
        /* Outputs for IfAction SubSystem: '<S82>/elseActionSubsystem' incorporates:
         *  ActionPort: '<S84>/Action Port'
         */
        /* Product: '<S84>/Multiply' incorporates:
         *  Merge: '<S82>/Merge'
         */
        for (i = 0; i < 32; i++)
        {
            rtb_Merge[i] = gain;
        }

        /* End of Product: '<S84>/Multiply' */
        /* End of Outputs for SubSystem: '<S82>/elseActionSubsystem' */
    }

    /* End of If: '<S82>/If' */

    /* S-Function (sdspperm2): '<S79>/Variable Selector' incorporates:
     *  DataStoreWrite: '<S68>/Data Store Write'
     *  Merge: '<S82>/Merge'
     */
    BasicAudio_TargetControlDsms.BasicAudioMuteCurrentGain = rtb_Merge[31];

    /* Outputs for Iterator SubSystem: '<S83>/For Each Subsystem' incorporates:
     *  ForEach: '<S90>/For Each'
     */
    for (ForEach_itr = 0; ForEach_itr < 32; ForEach_itr++)
    {
        /* ForEachSliceSelector generated from: '<S90>/In1' incorporates:
         *  Merge: '<S82>/Merge'
         */
        gain = rtb_Merge[ForEach_itr];

        /* ForEachSliceAssignment generated from: '<S90>/Out1' incorporates:
         *  ForEachSliceSelector generated from: '<S90>/In2'
         *  Product: '<S90>/Multiply'
         */
        BasicAudio_Target_B.ImpAsg_InsertedFor_Out1_at_inport_0[ForEach_itr] =
            gain * BasicAudio_Target_B.audioOut[ForEach_itr];
        BasicAudio_Target_B.ImpAsg_InsertedFor_Out1_at_inport_0[ForEach_itr + 32]
            = BasicAudio_Target_B.audioOut[ForEach_itr + 32] * gain;
    }

    /* End of Outputs for SubSystem: '<S83>/For Each Subsystem' */
    /* End of Outputs for SubSystem: '<S68>/Ramping_Subsystem' */
    /* End of Outputs for S-Function (RTC_NTF): '<S69>/NTF Trigger' */
    /* End of Outputs for S-Function (RTC_REQ_RSP): '<S41>/RTC Request Response Trigger' */
    /* End of Outputs for S-Function (RTC_NTF): '<S41>/NTF Trigger' */

    /* MATLAB Function: '<S40>/tone' */
    BasicAudio_Target_tone(&(BasicAudio_Target_BasicAudio0.BassBoostCoeffs[0]),
                           &(BasicAudio_Target_BasicAudio0.BassCutCoeffs[0]),
                           BasicAudio_Target_B.ImpAsg_InsertedFor_Out1_at_inport_0,
                           &BasicAudio_Target_B.sf_tone,
                           &BasicAudio_Target_DW.sf_tone);

    /* End of Outputs for S-Function (RTC_REQ_RSP): '<S93>/RTC Request Response Trigger' */
    /* End of Outputs for S-Function (RTC_NTF): '<S93>/NTF Trigger' */

    /* MATLAB Function: '<S92>/tone' */
    BasicAudio_Target_tone_e(&(BasicAudio_Target_BasicAudio0.TrebleBoostCoeffs[0]),
        &(BasicAudio_Target_BasicAudio0.TrebleCutCoeffs[0]),
        BasicAudio_Target_B.sf_tone.audioOut, &BasicAudio_Target_B.sf_tone_e,
        &BasicAudio_Target_DW.sf_tone_e);

    /* Outputs for Atomic SubSystem: '<Root>/Output' */
    /* SignalConversion generated from: '<S4>/In1' */
    memcpy(&UnitTest_Target_AudioOut[0],
           &BasicAudio_Target_B.sf_tone_e.audioOut[0], sizeof(real32_T) << 6U);
}

/* Model initialize function */
void BasicAudio_Target_initialize(void)
{
    /* SystemInitialize for Atomic SubSystem: '<S1>/Mute' */

    /* SystemInitialize for Atomic SubSystem: '<S68>/Ramping_Subsystem' */
    /* SystemInitialize for IfAction SubSystem: '<S82>/ifActionSubsystem' */
    /* InitializeConditions for DiscreteFilter: '<S85>/ramping_filter' */
    BasicAudio_Target_DW.ramping_filter_icLoad = 1U;

    /* End of SystemInitialize for SubSystem: '<S82>/ifActionSubsystem' */
    /* End of SystemInitialize for SubSystem: '<S68>/Ramping_Subsystem' */

    /* End of SystemInitialize for SubSystem: '<S1>/Mute' */
}

/* Model terminate function */
void BasicAudio_Target_terminate(void)
{
    /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
