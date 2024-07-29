/*
 * File: BasicAudio_Target_SCH_API.h
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

#ifndef RTW_HEADER_BasicAudio_Target_SCH_API_h_
#define RTW_HEADER_BasicAudio_Target_SCH_API_h_
#include "ASDID.h"

// ------------------------------------------------------------
// SCH MESSAGE IDS (Static Version Token)
// ------------------------------------------------------------
#define SCHID_GetTalariaVersion1       (0x10f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash1        (0x10f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash1        (0x10f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash1        (0x10f70003U)             // REQ, RSP

// ------------------------------------------------------------
// DATATYPES FOR COMMAND HANDLER DISPATCH TABLES
// ------------------------------------------------------------
// Function pointer to request/response handler
typedef void (*RTC_REQ_RSP_Handler)(void* pReqBuf, void* pRspBuf);

// Struct for dispatch tables
typedef struct RTC_REQ_RSP_Table_tag
{
    RTC_REQ_RSP_Handler request_response_func;
    uint32_t request_payload_bytes;
    uint32_t response_payload_bytes;
}

BasicAudio_Target_RTC_REQ_RSP_type;

// ------------------------------------------------------------
// SCH REQUEST PAYLOAD DEFINITIONS
// ------------------------------------------------------------

/// Request Payload Definition for GetTalariaVersion1
/// ----------------------------
/// @ingroup GetTalariaVersion1
/// This page describes the payload format for Request messages
/// used by the GetTalariaVersion1 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion1.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion1
    uint32_t caller_handle;
}

GetTalariaVersion1_REQ_T;

//--------------------------------------
// payload structure shared by all core 1 COMPAT_HASH requests
typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    uint32_t name_hash;
}

GetCompatHash1_REQ_T;

/// Request Payload Definition for GetTopCompatHash1
/// ----------------------------
/// @ingroup GetTopCompatHash1
/// This page describes the payload format for Request messages
/// used by the GetTopCompatHash1 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash1.
typedef GetCompatHash1_REQ_T GetTopCompatHash1_REQ_T;

/// Request Payload Definition for GetTspCompatHash1
/// ----------------------------
/// @ingroup GetTspCompatHash1
/// This page describes the payload format for Request messages
/// used by the GetTspCompatHash1 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash1.
typedef GetCompatHash1_REQ_T GetTspCompatHash1_REQ_T;

/// Request Payload Definition for GetRtcCompatHash1
/// ----------------------------
/// @ingroup GetRtcCompatHash1
/// This page describes the payload format for Request messages
/// used by the GetRtcCompatHash1 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash1.
typedef GetCompatHash1_REQ_T GetRtcCompatHash1_REQ_T;

// ------------------------------------------------------------
// SCH RESPONSE PAYLOAD DEFINITIONS
// ------------------------------------------------------------

/// Response Payload Definition for GetTalariaVersion1
/// ----------------------------
/// @ingroup GetTalariaVersion1
/// This page describes the payload format for Response messages
/// used by the GetTalariaVersion1 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion1.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion1
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint8_t version_string[8];         // e.g. "V2100AE"
}

GetTalariaVersion1_RSP_T;

typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint32_t name_hash;
    uint32_t compat_hash;
}

GetCompatHash1_RSP_T;

/// Response Payload Definition for GetTopCompatHash1
/// ----------------------------
/// @ingroup GetTopCompatHash1
/// This page describes the payload format for Response messages
/// used by the GetTopCompatHash1 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash1.
typedef GetCompatHash1_RSP_T GetTopCompatHash1_RSP_T;

/// Response Payload Definition for GetTspCompatHash1
/// ----------------------------
/// @ingroup GetTspCompatHash1
/// This page describes the payload format for Response messages
/// used by the GetTspCompatHash1 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash1.
typedef GetCompatHash1_RSP_T GetTspCompatHash1_RSP_T;

/// Response Payload Definition for GetRtcCompatHash1
/// ----------------------------
/// @ingroup GetRtcCompatHash1
/// This page describes the payload format for Response messages
/// used by the GetRtcCompatHash1 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash1.
typedef GetCompatHash1_RSP_T GetRtcCompatHash1_RSP_T;

// ------------------------------------------------------------
// SCH NOTIFICATION PAYLOAD DEFINITIONS
// ------------------------------------------------------------

// GetTalariaVersion1 does not define a Notification payload.

// GetTopCompatHash1 does not define a Notification payload.

// GetTspCompatHash1 does not define a Notification payload.

// GetRtcCompatHash1 does not define a Notification payload.

// ------------------------------------------------------------
// SCH ASYNCHRONOUS C-LANGUAGE FUNCTION API
// ------------------------------------------------------------
// Handle inbound request. Returns number of bytes in response
// or negative value upon error (See ASDID.h for returned ASDSTATUS codes.)
int BasicAudio_Target_HandleSchRequest(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize);
int BasicAudio_Target_UseDispatchTable(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize,
    BasicAudio_Target_RTC_REQ_RSP_type* table, uint32_t tableElements);

#endif                             /* RTW_HEADER_BasicAudio_Target_SCH_API_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
