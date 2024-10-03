/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: Model_SCH_IDs.h
 *
 * Combined SCH message ID and payload definitions from all cores
 *
 */

#ifndef MODEL_SCH_IDS_H_
#define MODEL_SCH_IDS_H_
#include <stdint.h>
#include "ASDID.h"

// ------------------------------------------------------------
// SCH MESSAGE IDS (Static Version Token)
// ------------------------------------------------------------
#define SCHID_GetTalariaVersion1       (0x10f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash1        (0x10f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash1        (0x10f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash1        (0x10f70003U)             // REQ, RSP

#define SCHID_GetTalariaVersion2       (0x20f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash2        (0x20f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash2        (0x20f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash2        (0x20f70003U)             // REQ, RSP

#define SCHID_GetTalariaVersion3       (0x30f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash3        (0x30f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash3        (0x30f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash3        (0x30f70003U)             // REQ, RSP

#define SCHID_GetTalariaVersion4       (0x40f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash4        (0x40f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash4        (0x40f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash4        (0x40f70003U)             // REQ, RSP

#define SCHID_GetTalariaVersion5       (0x50f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash5        (0x50f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash5        (0x50f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash5        (0x50f70003U)             // REQ, RSP

#define SCHID_GetTalariaVersion6       (0x60f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash6        (0x60f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash6        (0x60f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash6        (0x60f70003U)             // REQ, RSP

#define SCHID_GetTalariaVersion7       (0x70f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash7        (0x70f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash7        (0x70f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash7        (0x70f70003U)             // REQ, RSP

#define SCHID_GetTalariaVersion0       (0x00f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash0        (0x00f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash0        (0x00f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash0        (0x00f70003U)             // REQ, RSP

#define SCHID_GetTalariaVersion1       (0x10f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash1        (0x10f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash1        (0x10f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash1        (0x10f70003U)             // REQ, RSP

#define SCHID_GetTalariaVersion2       (0x20f70000U)             // REQ, RSP
#define SCHID_GetTopCompatHash2        (0x20f70001U)             // REQ, RSP
#define SCHID_GetTspCompatHash2        (0x20f70002U)             // REQ, RSP
#define SCHID_GetRtcCompatHash2        (0x20f70003U)             // REQ, RSP


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

/// Request Payload Definition for GetTalariaVersion2
/// ----------------------------
/// @ingroup GetTalariaVersion2
/// This page describes the payload format for Request messages
/// used by the GetTalariaVersion2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion2.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion2
    uint32_t caller_handle;
}

GetTalariaVersion2_REQ_T;

//--------------------------------------
// payload structure shared by all core 2 COMPAT_HASH requests
typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    uint32_t name_hash;
}

GetCompatHash2_REQ_T;

/// Request Payload Definition for GetTopCompatHash2
/// ----------------------------
/// @ingroup GetTopCompatHash2
/// This page describes the payload format for Request messages
/// used by the GetTopCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash2.
typedef GetCompatHash2_REQ_T GetTopCompatHash2_REQ_T;

/// Request Payload Definition for GetTspCompatHash2
/// ----------------------------
/// @ingroup GetTspCompatHash2
/// This page describes the payload format for Request messages
/// used by the GetTspCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash2.
typedef GetCompatHash2_REQ_T GetTspCompatHash2_REQ_T;

/// Request Payload Definition for GetRtcCompatHash2
/// ----------------------------
/// @ingroup GetRtcCompatHash2
/// This page describes the payload format for Request messages
/// used by the GetRtcCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash2.
typedef GetCompatHash2_REQ_T GetRtcCompatHash2_REQ_T;

/// Request Payload Definition for GetTalariaVersion3
/// ----------------------------
/// @ingroup GetTalariaVersion3
/// This page describes the payload format for Request messages
/// used by the GetTalariaVersion3 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion3.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion3
    uint32_t caller_handle;
}

GetTalariaVersion3_REQ_T;

//--------------------------------------
// payload structure shared by all core 3 COMPAT_HASH requests
typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    uint32_t name_hash;
}

GetCompatHash3_REQ_T;

/// Request Payload Definition for GetTopCompatHash3
/// ----------------------------
/// @ingroup GetTopCompatHash3
/// This page describes the payload format for Request messages
/// used by the GetTopCompatHash3 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash3.
typedef GetCompatHash3_REQ_T GetTopCompatHash3_REQ_T;

/// Request Payload Definition for GetTspCompatHash3
/// ----------------------------
/// @ingroup GetTspCompatHash3
/// This page describes the payload format for Request messages
/// used by the GetTspCompatHash3 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash3.
typedef GetCompatHash3_REQ_T GetTspCompatHash3_REQ_T;

/// Request Payload Definition for GetRtcCompatHash3
/// ----------------------------
/// @ingroup GetRtcCompatHash3
/// This page describes the payload format for Request messages
/// used by the GetRtcCompatHash3 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash3.
typedef GetCompatHash3_REQ_T GetRtcCompatHash3_REQ_T;

/// Request Payload Definition for GetTalariaVersion4
/// ----------------------------
/// @ingroup GetTalariaVersion4
/// This page describes the payload format for Request messages
/// used by the GetTalariaVersion4 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion4.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion4
    uint32_t caller_handle;
}

GetTalariaVersion4_REQ_T;

//--------------------------------------
// payload structure shared by all core 4 COMPAT_HASH requests
typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    uint32_t name_hash;
}

GetCompatHash4_REQ_T;

/// Request Payload Definition for GetTopCompatHash4
/// ----------------------------
/// @ingroup GetTopCompatHash4
/// This page describes the payload format for Request messages
/// used by the GetTopCompatHash4 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash4.
typedef GetCompatHash4_REQ_T GetTopCompatHash4_REQ_T;

/// Request Payload Definition for GetTspCompatHash4
/// ----------------------------
/// @ingroup GetTspCompatHash4
/// This page describes the payload format for Request messages
/// used by the GetTspCompatHash4 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash4.
typedef GetCompatHash4_REQ_T GetTspCompatHash4_REQ_T;

/// Request Payload Definition for GetRtcCompatHash4
/// ----------------------------
/// @ingroup GetRtcCompatHash4
/// This page describes the payload format for Request messages
/// used by the GetRtcCompatHash4 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash4.
typedef GetCompatHash4_REQ_T GetRtcCompatHash4_REQ_T;

/// Request Payload Definition for GetTalariaVersion5
/// ----------------------------
/// @ingroup GetTalariaVersion5
/// This page describes the payload format for Request messages
/// used by the GetTalariaVersion5 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion5.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion5
    uint32_t caller_handle;
}

GetTalariaVersion5_REQ_T;

//--------------------------------------
// payload structure shared by all core 5 COMPAT_HASH requests
typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    uint32_t name_hash;
}

GetCompatHash5_REQ_T;

/// Request Payload Definition for GetTopCompatHash5
/// ----------------------------
/// @ingroup GetTopCompatHash5
/// This page describes the payload format for Request messages
/// used by the GetTopCompatHash5 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash5.
typedef GetCompatHash5_REQ_T GetTopCompatHash5_REQ_T;

/// Request Payload Definition for GetTspCompatHash5
/// ----------------------------
/// @ingroup GetTspCompatHash5
/// This page describes the payload format for Request messages
/// used by the GetTspCompatHash5 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash5.
typedef GetCompatHash5_REQ_T GetTspCompatHash5_REQ_T;

/// Request Payload Definition for GetRtcCompatHash5
/// ----------------------------
/// @ingroup GetRtcCompatHash5
/// This page describes the payload format for Request messages
/// used by the GetRtcCompatHash5 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash5.
typedef GetCompatHash5_REQ_T GetRtcCompatHash5_REQ_T;

/// Request Payload Definition for GetTalariaVersion6
/// ----------------------------
/// @ingroup GetTalariaVersion6
/// This page describes the payload format for Request messages
/// used by the GetTalariaVersion6 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion6.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion6
    uint32_t caller_handle;
}

GetTalariaVersion6_REQ_T;

//--------------------------------------
// payload structure shared by all core 6 COMPAT_HASH requests
typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    uint32_t name_hash;
}

GetCompatHash6_REQ_T;

/// Request Payload Definition for GetTopCompatHash6
/// ----------------------------
/// @ingroup GetTopCompatHash6
/// This page describes the payload format for Request messages
/// used by the GetTopCompatHash6 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash6.
typedef GetCompatHash6_REQ_T GetTopCompatHash6_REQ_T;

/// Request Payload Definition for GetTspCompatHash6
/// ----------------------------
/// @ingroup GetTspCompatHash6
/// This page describes the payload format for Request messages
/// used by the GetTspCompatHash6 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash6.
typedef GetCompatHash6_REQ_T GetTspCompatHash6_REQ_T;

/// Request Payload Definition for GetRtcCompatHash6
/// ----------------------------
/// @ingroup GetRtcCompatHash6
/// This page describes the payload format for Request messages
/// used by the GetRtcCompatHash6 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash6.
typedef GetCompatHash6_REQ_T GetRtcCompatHash6_REQ_T;

/// Request Payload Definition for GetTalariaVersion7
/// ----------------------------
/// @ingroup GetTalariaVersion7
/// This page describes the payload format for Request messages
/// used by the GetTalariaVersion7 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion7.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion7
    uint32_t caller_handle;
}

GetTalariaVersion7_REQ_T;

//--------------------------------------
// payload structure shared by all core 7 COMPAT_HASH requests
typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    uint32_t name_hash;
}

GetCompatHash7_REQ_T;

/// Request Payload Definition for GetTopCompatHash7
/// ----------------------------
/// @ingroup GetTopCompatHash7
/// This page describes the payload format for Request messages
/// used by the GetTopCompatHash7 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash7.
typedef GetCompatHash7_REQ_T GetTopCompatHash7_REQ_T;

/// Request Payload Definition for GetTspCompatHash7
/// ----------------------------
/// @ingroup GetTspCompatHash7
/// This page describes the payload format for Request messages
/// used by the GetTspCompatHash7 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash7.
typedef GetCompatHash7_REQ_T GetTspCompatHash7_REQ_T;

/// Request Payload Definition for GetRtcCompatHash7
/// ----------------------------
/// @ingroup GetRtcCompatHash7
/// This page describes the payload format for Request messages
/// used by the GetRtcCompatHash7 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash7.
typedef GetCompatHash7_REQ_T GetRtcCompatHash7_REQ_T;

/// Request Payload Definition for GetTalariaVersion0
/// ----------------------------
/// @ingroup GetTalariaVersion0
/// This page describes the payload format for Request messages
/// used by the GetTalariaVersion0 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion0.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion0
    uint32_t caller_handle;
}

GetTalariaVersion0_REQ_T;

//--------------------------------------
// payload structure shared by all core 0 COMPAT_HASH requests
typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    uint32_t name_hash;
}

GetCompatHash0_REQ_T;

/// Request Payload Definition for GetTopCompatHash0
/// ----------------------------
/// @ingroup GetTopCompatHash0
/// This page describes the payload format for Request messages
/// used by the GetTopCompatHash0 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash0.
typedef GetCompatHash0_REQ_T GetTopCompatHash0_REQ_T;

/// Request Payload Definition for GetTspCompatHash0
/// ----------------------------
/// @ingroup GetTspCompatHash0
/// This page describes the payload format for Request messages
/// used by the GetTspCompatHash0 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash0.
typedef GetCompatHash0_REQ_T GetTspCompatHash0_REQ_T;

/// Request Payload Definition for GetRtcCompatHash0
/// ----------------------------
/// @ingroup GetRtcCompatHash0
/// This page describes the payload format for Request messages
/// used by the GetRtcCompatHash0 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash0.
typedef GetCompatHash0_REQ_T GetRtcCompatHash0_REQ_T;

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

/// Request Payload Definition for GetTalariaVersion2
/// ----------------------------
/// @ingroup GetTalariaVersion2
/// This page describes the payload format for Request messages
/// used by the GetTalariaVersion2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion2.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion2
    uint32_t caller_handle;
}

GetTalariaVersion2_REQ_T;

//--------------------------------------
// payload structure shared by all core 2 COMPAT_HASH requests
typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    uint32_t name_hash;
}

GetCompatHash2_REQ_T;

/// Request Payload Definition for GetTopCompatHash2
/// ----------------------------
/// @ingroup GetTopCompatHash2
/// This page describes the payload format for Request messages
/// used by the GetTopCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash2.
typedef GetCompatHash2_REQ_T GetTopCompatHash2_REQ_T;

/// Request Payload Definition for GetTspCompatHash2
/// ----------------------------
/// @ingroup GetTspCompatHash2
/// This page describes the payload format for Request messages
/// used by the GetTspCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash2.
typedef GetCompatHash2_REQ_T GetTspCompatHash2_REQ_T;

/// Request Payload Definition for GetRtcCompatHash2
/// ----------------------------
/// @ingroup GetRtcCompatHash2
/// This page describes the payload format for Request messages
/// used by the GetRtcCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash2.
typedef GetCompatHash2_REQ_T GetRtcCompatHash2_REQ_T;


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

/// Response Payload Definition for GetTalariaVersion2
/// ----------------------------
/// @ingroup GetTalariaVersion2
/// This page describes the payload format for Response messages
/// used by the GetTalariaVersion2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion2.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion2
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint8_t version_string[8];         // e.g. "V2100AE"
}

GetTalariaVersion2_RSP_T;

typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint32_t name_hash;
    uint32_t compat_hash;
}

GetCompatHash2_RSP_T;

/// Response Payload Definition for GetTopCompatHash2
/// ----------------------------
/// @ingroup GetTopCompatHash2
/// This page describes the payload format for Response messages
/// used by the GetTopCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash2.
typedef GetCompatHash2_RSP_T GetTopCompatHash2_RSP_T;

/// Response Payload Definition for GetTspCompatHash2
/// ----------------------------
/// @ingroup GetTspCompatHash2
/// This page describes the payload format for Response messages
/// used by the GetTspCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash2.
typedef GetCompatHash2_RSP_T GetTspCompatHash2_RSP_T;

/// Response Payload Definition for GetRtcCompatHash2
/// ----------------------------
/// @ingroup GetRtcCompatHash2
/// This page describes the payload format for Response messages
/// used by the GetRtcCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash2.
typedef GetCompatHash2_RSP_T GetRtcCompatHash2_RSP_T;

/// Response Payload Definition for GetTalariaVersion3
/// ----------------------------
/// @ingroup GetTalariaVersion3
/// This page describes the payload format for Response messages
/// used by the GetTalariaVersion3 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion3.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion3
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint8_t version_string[8];         // e.g. "V2100AE"
}

GetTalariaVersion3_RSP_T;

typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint32_t name_hash;
    uint32_t compat_hash;
}

GetCompatHash3_RSP_T;

/// Response Payload Definition for GetTopCompatHash3
/// ----------------------------
/// @ingroup GetTopCompatHash3
/// This page describes the payload format for Response messages
/// used by the GetTopCompatHash3 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash3.
typedef GetCompatHash3_RSP_T GetTopCompatHash3_RSP_T;

/// Response Payload Definition for GetTspCompatHash3
/// ----------------------------
/// @ingroup GetTspCompatHash3
/// This page describes the payload format for Response messages
/// used by the GetTspCompatHash3 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash3.
typedef GetCompatHash3_RSP_T GetTspCompatHash3_RSP_T;

/// Response Payload Definition for GetRtcCompatHash3
/// ----------------------------
/// @ingroup GetRtcCompatHash3
/// This page describes the payload format for Response messages
/// used by the GetRtcCompatHash3 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash3.
typedef GetCompatHash3_RSP_T GetRtcCompatHash3_RSP_T;

/// Response Payload Definition for GetTalariaVersion4
/// ----------------------------
/// @ingroup GetTalariaVersion4
/// This page describes the payload format for Response messages
/// used by the GetTalariaVersion4 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion4.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion4
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint8_t version_string[8];         // e.g. "V2100AE"
}

GetTalariaVersion4_RSP_T;

typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint32_t name_hash;
    uint32_t compat_hash;
}

GetCompatHash4_RSP_T;

/// Response Payload Definition for GetTopCompatHash4
/// ----------------------------
/// @ingroup GetTopCompatHash4
/// This page describes the payload format for Response messages
/// used by the GetTopCompatHash4 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash4.
typedef GetCompatHash4_RSP_T GetTopCompatHash4_RSP_T;

/// Response Payload Definition for GetTspCompatHash4
/// ----------------------------
/// @ingroup GetTspCompatHash4
/// This page describes the payload format for Response messages
/// used by the GetTspCompatHash4 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash4.
typedef GetCompatHash4_RSP_T GetTspCompatHash4_RSP_T;

/// Response Payload Definition for GetRtcCompatHash4
/// ----------------------------
/// @ingroup GetRtcCompatHash4
/// This page describes the payload format for Response messages
/// used by the GetRtcCompatHash4 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash4.
typedef GetCompatHash4_RSP_T GetRtcCompatHash4_RSP_T;

/// Response Payload Definition for GetTalariaVersion5
/// ----------------------------
/// @ingroup GetTalariaVersion5
/// This page describes the payload format for Response messages
/// used by the GetTalariaVersion5 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion5.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion5
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint8_t version_string[8];         // e.g. "V2100AE"
}

GetTalariaVersion5_RSP_T;

typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint32_t name_hash;
    uint32_t compat_hash;
}

GetCompatHash5_RSP_T;

/// Response Payload Definition for GetTopCompatHash5
/// ----------------------------
/// @ingroup GetTopCompatHash5
/// This page describes the payload format for Response messages
/// used by the GetTopCompatHash5 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash5.
typedef GetCompatHash5_RSP_T GetTopCompatHash5_RSP_T;

/// Response Payload Definition for GetTspCompatHash5
/// ----------------------------
/// @ingroup GetTspCompatHash5
/// This page describes the payload format for Response messages
/// used by the GetTspCompatHash5 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash5.
typedef GetCompatHash5_RSP_T GetTspCompatHash5_RSP_T;

/// Response Payload Definition for GetRtcCompatHash5
/// ----------------------------
/// @ingroup GetRtcCompatHash5
/// This page describes the payload format for Response messages
/// used by the GetRtcCompatHash5 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash5.
typedef GetCompatHash5_RSP_T GetRtcCompatHash5_RSP_T;

/// Response Payload Definition for GetTalariaVersion6
/// ----------------------------
/// @ingroup GetTalariaVersion6
/// This page describes the payload format for Response messages
/// used by the GetTalariaVersion6 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion6.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion6
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint8_t version_string[8];         // e.g. "V2100AE"
}

GetTalariaVersion6_RSP_T;

typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint32_t name_hash;
    uint32_t compat_hash;
}

GetCompatHash6_RSP_T;

/// Response Payload Definition for GetTopCompatHash6
/// ----------------------------
/// @ingroup GetTopCompatHash6
/// This page describes the payload format for Response messages
/// used by the GetTopCompatHash6 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash6.
typedef GetCompatHash6_RSP_T GetTopCompatHash6_RSP_T;

/// Response Payload Definition for GetTspCompatHash6
/// ----------------------------
/// @ingroup GetTspCompatHash6
/// This page describes the payload format for Response messages
/// used by the GetTspCompatHash6 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash6.
typedef GetCompatHash6_RSP_T GetTspCompatHash6_RSP_T;

/// Response Payload Definition for GetRtcCompatHash6
/// ----------------------------
/// @ingroup GetRtcCompatHash6
/// This page describes the payload format for Response messages
/// used by the GetRtcCompatHash6 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash6.
typedef GetCompatHash6_RSP_T GetRtcCompatHash6_RSP_T;

/// Response Payload Definition for GetTalariaVersion7
/// ----------------------------
/// @ingroup GetTalariaVersion7
/// This page describes the payload format for Response messages
/// used by the GetTalariaVersion7 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion7.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion7
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint8_t version_string[8];         // e.g. "V2100AE"
}

GetTalariaVersion7_RSP_T;

typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint32_t name_hash;
    uint32_t compat_hash;
}

GetCompatHash7_RSP_T;

/// Response Payload Definition for GetTopCompatHash7
/// ----------------------------
/// @ingroup GetTopCompatHash7
/// This page describes the payload format for Response messages
/// used by the GetTopCompatHash7 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash7.
typedef GetCompatHash7_RSP_T GetTopCompatHash7_RSP_T;

/// Response Payload Definition for GetTspCompatHash7
/// ----------------------------
/// @ingroup GetTspCompatHash7
/// This page describes the payload format for Response messages
/// used by the GetTspCompatHash7 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash7.
typedef GetCompatHash7_RSP_T GetTspCompatHash7_RSP_T;

/// Response Payload Definition for GetRtcCompatHash7
/// ----------------------------
/// @ingroup GetRtcCompatHash7
/// This page describes the payload format for Response messages
/// used by the GetRtcCompatHash7 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash7.
typedef GetCompatHash7_RSP_T GetRtcCompatHash7_RSP_T;

/// Response Payload Definition for GetTalariaVersion0
/// ----------------------------
/// @ingroup GetTalariaVersion0
/// This page describes the payload format for Response messages
/// used by the GetTalariaVersion0 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion0.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion0
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint8_t version_string[8];         // e.g. "V2100AE"
}

GetTalariaVersion0_RSP_T;

typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint32_t name_hash;
    uint32_t compat_hash;
}

GetCompatHash0_RSP_T;

/// Response Payload Definition for GetTopCompatHash0
/// ----------------------------
/// @ingroup GetTopCompatHash0
/// This page describes the payload format for Response messages
/// used by the GetTopCompatHash0 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash0.
typedef GetCompatHash0_RSP_T GetTopCompatHash0_RSP_T;

/// Response Payload Definition for GetTspCompatHash0
/// ----------------------------
/// @ingroup GetTspCompatHash0
/// This page describes the payload format for Response messages
/// used by the GetTspCompatHash0 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash0.
typedef GetCompatHash0_RSP_T GetTspCompatHash0_RSP_T;

/// Response Payload Definition for GetRtcCompatHash0
/// ----------------------------
/// @ingroup GetRtcCompatHash0
/// This page describes the payload format for Response messages
/// used by the GetRtcCompatHash0 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash0.
typedef GetCompatHash0_RSP_T GetRtcCompatHash0_RSP_T;

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

/// Response Payload Definition for GetTalariaVersion2
/// ----------------------------
/// @ingroup GetTalariaVersion2
/// This page describes the payload format for Response messages
/// used by the GetTalariaVersion2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTalariaVersion2.
typedef struct
{
    uint32_t msgId;                    // always SCHID_GetTalariaVersion2
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint8_t version_string[8];         // e.g. "V2100AE"
}

GetTalariaVersion2_RSP_T;

typedef struct
{
    uint32_t msgId;                    //SCHID_Get...CompatHash
    uint32_t caller_handle;
    ASDSTATUS status_code;
    uint32_t name_hash;
    uint32_t compat_hash;
}

GetCompatHash2_RSP_T;

/// Response Payload Definition for GetTopCompatHash2
/// ----------------------------
/// @ingroup GetTopCompatHash2
/// This page describes the payload format for Response messages
/// used by the GetTopCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTopCompatHash2.
typedef GetCompatHash2_RSP_T GetTopCompatHash2_RSP_T;

/// Response Payload Definition for GetTspCompatHash2
/// ----------------------------
/// @ingroup GetTspCompatHash2
/// This page describes the payload format for Response messages
/// used by the GetTspCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetTspCompatHash2.
typedef GetCompatHash2_RSP_T GetTspCompatHash2_RSP_T;

/// Response Payload Definition for GetRtcCompatHash2
/// ----------------------------
/// @ingroup GetRtcCompatHash2
/// This page describes the payload format for Response messages
/// used by the GetRtcCompatHash2 RTC mechanism. For a more general
/// description of how this message works, see @ref GetRtcCompatHash2.
typedef GetCompatHash2_RSP_T GetRtcCompatHash2_RSP_T;


// ------------------------------------------------------------
// SCH NOTIFICATION PAYLOAD DEFINITIONS
// ------------------------------------------------------------

// GetTalariaVersion1 does not define a Notification payload.

// GetTopCompatHash1 does not define a Notification payload.

// GetTspCompatHash1 does not define a Notification payload.

// GetRtcCompatHash1 does not define a Notification payload.

// GetTalariaVersion2 does not define a Notification payload.

// GetTopCompatHash2 does not define a Notification payload.

// GetTspCompatHash2 does not define a Notification payload.

// GetRtcCompatHash2 does not define a Notification payload.

// GetTalariaVersion3 does not define a Notification payload.

// GetTopCompatHash3 does not define a Notification payload.

// GetTspCompatHash3 does not define a Notification payload.

// GetRtcCompatHash3 does not define a Notification payload.

// GetTalariaVersion4 does not define a Notification payload.

// GetTopCompatHash4 does not define a Notification payload.

// GetTspCompatHash4 does not define a Notification payload.

// GetRtcCompatHash4 does not define a Notification payload.

// GetTalariaVersion5 does not define a Notification payload.

// GetTopCompatHash5 does not define a Notification payload.

// GetTspCompatHash5 does not define a Notification payload.

// GetRtcCompatHash5 does not define a Notification payload.

// GetTalariaVersion6 does not define a Notification payload.

// GetTopCompatHash6 does not define a Notification payload.

// GetTspCompatHash6 does not define a Notification payload.

// GetRtcCompatHash6 does not define a Notification payload.

// GetTalariaVersion7 does not define a Notification payload.

// GetTopCompatHash7 does not define a Notification payload.

// GetTspCompatHash7 does not define a Notification payload.

// GetRtcCompatHash7 does not define a Notification payload.

// GetTalariaVersion0 does not define a Notification payload.

// GetTopCompatHash0 does not define a Notification payload.

// GetTspCompatHash0 does not define a Notification payload.

// GetRtcCompatHash0 does not define a Notification payload.

// GetTalariaVersion1 does not define a Notification payload.

// GetTopCompatHash1 does not define a Notification payload.

// GetTspCompatHash1 does not define a Notification payload.

// GetRtcCompatHash1 does not define a Notification payload.

// GetTalariaVersion2 does not define a Notification payload.

// GetTopCompatHash2 does not define a Notification payload.

// GetTspCompatHash2 does not define a Notification payload.

// GetRtcCompatHash2 does not define a Notification payload.

#endif

