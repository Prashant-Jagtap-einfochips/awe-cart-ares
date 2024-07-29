/*
 * File: BasicAudio_Target_RTC_REQ_RSP.c
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

#include "rtwtypes.h"
#include "BasicAudio_Target_RTC_IDs.h"
#include "BasicAudio_Target_RTC_REQ_RSP.h"
#include "ASDID.h"                     // for AsdIdStatus_type and other defines
#include "BasicAudio_Target_SCH_API.h"

// ----- Function handler prototypes:
extern void RtcReqRsp_BasicAudioBalanceSet(void* pReqBuf, void* pRspBuf);
extern void RtcReqRsp_BasicAudioBassSet(void* pReqBuf, void* pRspBuf);
extern void RtcReqRsp_BasicAudioMuteSet(void* pReqBuf, void* pRspBuf);
extern void RtcReqRsp_BasicAudioTrebleSet(void* pReqBuf, void* pRspBuf);
extern void RtcReqRsp_BasicAudioVolumeSet(void* pReqBuf, void* pRspBuf);

// ----- RtcDispatchTable0000 DISPATCH TABLE -----:
static BasicAudio_Target_RTC_REQ_RSP_type RtcDispatchTable0000[] =
{
    // "BasicAudioBalanceSet" request-response handler '<S12>/RTC Request Response Trigger':
    {                                  /*Index RtcDispatchTable0000.0*/
        &RtcReqRsp_BasicAudioBalanceSet,
        sizeof(BasicAudioBalanceSet_REQ_T),/*20 char*/
        sizeof(BasicAudioBalanceSet_RSP_T)/*16 char*/
    },

    // "BasicAudioBassSet" request-response handler '<S41>/RTC Request Response Trigger':
    {                                  /*Index RtcDispatchTable0000.1*/
        &RtcReqRsp_BasicAudioBassSet,
        sizeof(BasicAudioBassSet_REQ_T),/*20 char*/
        sizeof(BasicAudioBassSet_RSP_T)/*16 char*/
    },

    // "BasicAudioMuteSet" request-response handler '<S69>/RTC Request Response Trigger':
    {                                  /*Index RtcDispatchTable0000.2*/
        &RtcReqRsp_BasicAudioMuteSet,
        sizeof(BasicAudioMuteSet_REQ_T),/*20 char*/
        sizeof(BasicAudioMuteSet_RSP_T)/*16 char*/
    },

    // "BasicAudioTrebleSet" request-response handler '<S93>/RTC Request Response Trigger':
    {                                  /*Index RtcDispatchTable0000.3*/
        &RtcReqRsp_BasicAudioTrebleSet,
        sizeof(BasicAudioTrebleSet_REQ_T),/*20 char*/
        sizeof(BasicAudioTrebleSet_RSP_T)/*16 char*/
    },

    // "BasicAudioVolumeSet" request-response handler '<S120>/RTC Request Response Trigger':
    {                                  /*Index RtcDispatchTable0000.4*/
        &RtcReqRsp_BasicAudioVolumeSet,
        sizeof(BasicAudioVolumeSet_REQ_T),/*24 char*/
        sizeof(BasicAudioVolumeSet_RSP_T)/*16 char*/
    },
};

static uint32_t RtcDispatchTable0000Count = sizeof(RtcDispatchTable0000)/sizeof
    (RtcDispatchTable0000[0]);

/** Handle Run Time Control request (RTC) and generate response payload (if any).
 * NOTE: This function integrates with HandleAsdIdRequest. It is not intended
 * to be called directly if @ref HandleAsdIdRequest is more convenient.
 *  @param pReqBuf Request buffer containing RTC request*  @param reqBufSize number of bytes used in the request buffer
 *  @param pRspBuf Response buffer to receive response of AsdIdStatus_type*  @param rspBufSize Size of available bytes provided for the response to fill
 *  @return Returns the toal number of bytes utilized in the response buffer, or
 *          ASDSTATUS_RSP_BUFFER_SMALL if the response buffer is too small to
 *          contain a AsdIdStatus_type payload. If the returned value is positive,
 *          the caller must check the AsdIdStatus_type's statusCode. If the
 *          status code is less than zero, then any further reponse "data" as
 *          defined by the RTC service will not be present (i.e. there probably
 *          was not space for it anyway).
 *
 * @ingroup rtc
 */
int BasicAudio_Target_HandleRtcRequest(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize)
{
    // Gain access to the appropriate command table
    uint32_t msgId = ((uint32_t*)pReqBuf)[0];
    BasicAudio_Target_RTC_REQ_RSP_type* theTable = 0;
    uint32_t tableSize = 0;
    theTable = RtcDispatchTable0000;
    tableSize = sizeof(RtcDispatchTable0000) / sizeof
        (BasicAudio_Target_RTC_REQ_RSP_type);
    return BasicAudio_Target_UseDispatchTable(pReqBuf,reqBufSize,pRspBuf,
        rspBufSize,theTable,tableSize);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
