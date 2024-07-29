/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: BasicAudio_SCH_IDs.h
 *
 * Combined SCH message ID and payload definitions from all cores
 *
 */

#ifndef BASICAUDIO_SCH_IDS_H_
#define BASICAUDIO_SCH_IDS_H_
#include <stdint.h>
#include "ASDID.h"

// ------------------------------------------------------------
// SCH MESSAGE IDS (Static Version Token)
// ------------------------------------------------------------
#define SCHID_GetTalariaVersion1       (0x10f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash1        (0x10f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash1        (0x10f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash1        (0x10f70003U)             // REQ, RSP


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

#endif

