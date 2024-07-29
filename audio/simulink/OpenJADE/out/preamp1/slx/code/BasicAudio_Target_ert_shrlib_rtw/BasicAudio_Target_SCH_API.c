/*
 * File: BasicAudio_Target_SCH_API.c
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

#include "BasicAudio_Target_SCH_API.h"
#include "TOP_COMPAT.h"
#include "TSP_COMPAT.h"
#include "RTC_COMPAT.h"
#include <string.h>                    // for memcpy

const char* talariaVersion1 = "V2200BK";

// ------------------------------------------------------------
// SCH message handlers
// ------------------------------------------------------------
extern void SchReqRsp_GetTalariaVersion1(void*, void*);
extern void SchReqRsp_GetTopCompatHash1(void*, void*);
extern void SchReqRsp_GetTspCompatHash1(void*, void*);
extern void SchReqRsp_GetRtcCompatHash1(void*, void*);
static void SharedGetHash(void* pREQBuf, void* pRSPBuf, uint32_t* nameArray,
    uint32_t* hashArray, uint32_t numElements);

// ----- Struct for dispatch tables
BasicAudio_Target_RTC_REQ_RSP_type SchDispathTable1[] =
{
    {
        &SchReqRsp_GetTalariaVersion1,
        sizeof(GetTalariaVersion1_REQ_T),
        sizeof(GetTalariaVersion1_RSP_T)
    },

    {
        &SchReqRsp_GetTopCompatHash1,
        sizeof(GetTopCompatHash1_REQ_T),
        sizeof(GetTopCompatHash1_RSP_T)
    },

    {
        &SchReqRsp_GetTspCompatHash1,
        sizeof(GetTspCompatHash1_REQ_T),
        sizeof(GetTspCompatHash1_RSP_T)
    },

    {
        &SchReqRsp_GetRtcCompatHash1,
        sizeof(GetRtcCompatHash1_REQ_T),
        sizeof(GetRtcCompatHash1_RSP_T)
    }
};

// ----- Handler functions
void SchReqRsp_GetTalariaVersion1(void* pREQBuf, void* pRSPBuf)
{
    GetTalariaVersion1_REQ_T* payloadREQ = (GetTalariaVersion1_REQ_T*)pREQBuf;
    uint32_t callerHandle = payloadREQ->caller_handle;
    ASDSTATUS statusCode = 0;
    int i;
    GetTalariaVersion1_RSP_T* payloadRSP = (GetTalariaVersion1_RSP_T*)pRSPBuf;
    payloadRSP->caller_handle = callerHandle;
    payloadRSP->status_code = statusCode;

    // copy in the talaria version string, padding with zeros
    const char* src = talariaVersion1;
    for (i=0; i < sizeof(payloadRSP->version_string); i++)
    {
        payloadRSP->version_string[i] = (*src) ? *src++ : 0;
    }
}

#define COMPATARRAYSIZE(x)             (sizeof(x)/sizeof((x)[0]))

void SchReqRsp_GetTopCompatHash1(void* pREQBuf, void* pRSPBuf)
{

#ifndef TOP_COMPAT

    // Compatibility values are not available
    SharedGetHash(pREQBuf,pRSPBuf,0,0,0);

#else

    static uint32_t nameArray[] = TOP_COMPAT_NAME_HASH;
    static uint32_t hashArray[] = TOP_COMPAT;
    FAIL_BUILD_IF_FALSE(COMPATARRAYSIZE(nameArray) == COMPATARRAYSIZE(hashArray));
    uint32_t numElements = sizeof(nameArray)/sizeof(nameArray[0]);
    SharedGetHash(pREQBuf,pRSPBuf,&(nameArray[0]),&(hashArray[0]),numElements);

#endif

}

void SchReqRsp_GetTspCompatHash1(void* pREQBuf, void* pRSPBuf)
{

#ifndef TOP_COMPAT

    // Compatibility values are not available
    SharedGetHash(pREQBuf,pRSPBuf,0,0,0);

#else

    static uint32_t nameArray[] = TSP_COMPAT_NAME_HASH;
    static uint32_t hashArray[] = TSP_COMPAT;
    FAIL_BUILD_IF_FALSE(COMPATARRAYSIZE(nameArray) == COMPATARRAYSIZE(hashArray));
    uint32_t numElements = sizeof(nameArray)/sizeof(nameArray[0]);
    SharedGetHash(pREQBuf,pRSPBuf,&(nameArray[0]),&(hashArray[0]),numElements);

#endif

}

void SchReqRsp_GetRtcCompatHash1(void* pREQBuf, void* pRSPBuf)
{

#ifndef TOP_COMPAT

    // Compatibility values are not available
    SharedGetHash(pREQBuf,pRSPBuf,0,0,0);

#else

    static uint32_t nameArray[] = RTC_COMPAT_NAME_HASH;
    static uint32_t hashArray[] = RTC_COMPAT;
    FAIL_BUILD_IF_FALSE(COMPATARRAYSIZE(nameArray) == COMPATARRAYSIZE(hashArray));
    uint32_t numElements = sizeof(nameArray)/sizeof(nameArray[0]);
    SharedGetHash(pREQBuf,pRSPBuf,&(nameArray[0]),&(hashArray[0]),numElements);

#endif

}

static void SharedGetHash(void* pREQBuf, void* pRSPBuf, uint32_t* nameArray,
    uint32_t* hashArray, uint32_t numElements)
{
    GetCompatHash1_REQ_T* payloadREQ = (GetCompatHash1_REQ_T*)pREQBuf;
    uint32_t callerHandle = payloadREQ->caller_handle;
    uint32_t reqNameHash = payloadREQ->name_hash;
    uint32_t compatHash = 0;

    // Assume the worst
    ASDSTATUS statusCode = ASDSTATUS_NO_COMPAT_CODES;
    if (nameArray && hashArray)
    {
        // The codes are available, so upgrade the worse case:
        uint32_t i;
        statusCode = ASDSTATUS_VALUE_NOT_AVAILABLE;
        for (i=0; i < numElements; i++)
        {
            if (reqNameHash == nameArray[i])
            {
                compatHash = hashArray[i];
                statusCode = ASDSTATUS_OK;
                break;
            }
        }
    }

    GetCompatHash1_RSP_T* payloadRSP = (GetCompatHash1_RSP_T*)pRSPBuf;
    payloadRSP->caller_handle = callerHandle;
    payloadRSP->status_code = statusCode;
    payloadRSP->name_hash = reqNameHash;
    payloadRSP->compat_hash = compatHash;
}

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
int BasicAudio_Target_UseDispatchTable(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize,
    BasicAudio_Target_RTC_REQ_RSP_type* theTable, uint32_t tableSize)
{
    // returns number of bytes
    int retVal = 0;
    int status = ASDSTATUS_OK;

    // parse out the message id
    if (reqBufSize < (sizeof(uint32_t)) ||
        pReqBuf == 0)
    {
        return ASDSTATUS_REQ_BUFFER_SMALL;
    }

    uint32_t msgId = ((uint32_t*)pReqBuf)[0];

    // Assume bogus caller handle until we're sure we've got one
    uint32_t callerHandle = 0xffffffff;

    // setup cast of the response buffer (if any)
    AsdIdStatus_type* rspStatusBuf = (AsdIdStatus_type*)pRspBuf;

    // Lookup the RTC service (by index) in the array
    uint32_t rtcIdx = RTCID_INDEX(msgId);
    if (rtcIdx < tableSize)
    {
        // Index was within array - here is our service entry:
        BasicAudio_Target_RTC_REQ_RSP_type* pTableEntry = &theTable[rtcIdx];
        uint32_t reqBytes = pTableEntry->request_payload_bytes;
        uint32_t rspBytes = pTableEntry->response_payload_bytes;

        // Validate the sizes for provided request and response buffers
        // (allow either to be bigger than expected - just ignore extra bytes)
        uint8_t reqBufOK = (reqBufSize >= reqBytes) &&
            (pReqBuf || (reqBytes==0));
        uint8_t rspBufOK = (rspBufSize >= rspBytes) &&
            (pRspBuf || (rspBytes==0));

        // We need both buffers to be OK if we want to call the service handler:
        if (reqBufOK && rspBufOK)
        {
            // Request and response buffers are OK. Proceed with the call.
            pTableEntry->request_response_func(pReqBuf, pRspBuf);
            retVal = pTableEntry->response_payload_bytes;
            if (retVal)                // copy in msgid if RSP payload enabled
            {
                // The caller handle and status code are set by the REQ RSP func called above.
                // However, the msgId must be manually copied in here as the REQ RSP func
                // is not aware of its own msgId.
                rspStatusBuf->msgId = msgId;
            }
        }
        else
        {
            // one of the buffers was too small
            status = reqBufOK ? ASDSTATUS_RSP_BUFFER_SMALL :
                ASDSTATUS_REQ_BUFFER_SMALL;

            // if there is supposed to be a response and the request is big enough to
            // contain a caller handle - grab it here
            if (rspBytes && reqBufSize >= sizeof(uint32_t)*2)
            {
                callerHandle = ((uint32_t*)pReqBuf)[1];
            }
        }
    }
    else                               // rtc index is *not* within the table
    {
        status = ASDSTATUS_RTC_UNKNOWN_SERVICE;
    }

    // If the request-response was successful, everything is already all set.
    // Otherwise - handle errors by attempting to return an error response:
    if (status < 0)
    {
        if (rspBufSize >= sizeof(AsdIdStatus_type) && pRspBuf)
        {
            // Response buffer looks ready to receive error, so return it
            // Copy caller handle and message id still available from the REQ:
            rspStatusBuf->msgId = msgId;
            rspStatusBuf->callerHandle = callerHandle;

            // fill in the error status code
            rspStatusBuf->statusCode = status;
            retVal = sizeof(AsdIdStatus_type);
        }
        else
        {
            // Cannot even return an error payload so return error code.
            // The RSP buffer is too small, perhaps even to send back a
            // more specific error code.
            retVal = ASDSTATUS_RSP_BUFFER_SMALL;
        }
    }

    return retVal;
}

int BasicAudio_Target_HandleSchRequest(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize)
{
    // Gain access to the appropriate command table
    BasicAudio_Target_RTC_REQ_RSP_type* theTable = SchDispathTable1;
    uint32_t tableSize = sizeof(SchDispathTable1)/sizeof(SchDispathTable1[0]);
    return BasicAudio_Target_UseDispatchTable(pReqBuf,reqBufSize,pRspBuf,
        rspBufSize,theTable,tableSize);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
