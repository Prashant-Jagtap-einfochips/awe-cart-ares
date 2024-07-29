/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: BasicAudio_RTC_IDs.h
 *
 * Combined RTC message ID and payload definitions from all cores
 *
 */

#ifndef BASICAUDIO_RTC_IDS_H_
#define BASICAUDIO_RTC_IDS_H_
#include <stdint.h>
#include "ASDID.h"

// ------------------------------------------------------------
// CORE ENDIAN SETTINGS
// ------------------------------------------------------------
#define ENDIANNESS_CORE                LittleEndian


// ------------------------------------------------------------
// RTC MESSAGE IDS
// ------------------------------------------------------------
#define RTCID_BasicAudioBalanceSet     (0x10000000U)             // REQ, RSP, NTF '<S12>/RTC Request Response Trigger'
#define RTCID_BasicAudioBassSet        (0x10000001U)             // REQ, RSP, NTF '<S41>/RTC Request Response Trigger'
#define RTCID_BasicAudioMuteSet        (0x10000002U)             // REQ, RSP, NTF '<S69>/RTC Request Response Trigger'
#define RTCID_BasicAudioTrebleSet      (0x10000003U)             // REQ, RSP, NTF '<S93>/RTC Request Response Trigger'
#define RTCID_BasicAudioVolumeSet      (0x10000004U)             // REQ, RSP, NTF '<S120>/RTC Request Response Trigger'


// ------------------------------------------------------------
// RTC REQUEST PAYLOAD DEFINITIONS
// ------------------------------------------------------------

/// Request Payload Definition for BasicAudioBalanceSet
/// ----------------------------
/// @ingroup BasicAudioBalanceSet
/// This page describes the payload format for Request messages
/// used by the BasicAudioBalanceSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioBalanceSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioBalanceSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
BasicAudioBalanceSet_REQ_T;            /* '<S31>/UnpackREQ' */

/// Request Payload Definition for BasicAudioBassSet
/// ----------------------------
/// @ingroup BasicAudioBassSet
/// This page describes the payload format for Request messages
/// used by the BasicAudioBassSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioBassSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioBassSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
BasicAudioBassSet_REQ_T;               /* '<S57>/UnpackREQ' */

/// Request Payload Definition for BasicAudioMuteSet
/// ----------------------------
/// @ingroup BasicAudioMuteSet
/// This page describes the payload format for Request messages
/// used by the BasicAudioMuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioMuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioMuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t mute_switch;              // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
BasicAudioMuteSet_REQ_T;               /* '<S77>/UnpackREQ' */

/// Request Payload Definition for BasicAudioTrebleSet
/// ----------------------------
/// @ingroup BasicAudioTrebleSet
/// This page describes the payload format for Request messages
/// used by the BasicAudioTrebleSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioTrebleSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioTrebleSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
}
BasicAudioTrebleSet_REQ_T;             /* '<S109>/UnpackREQ' */

/// Request Payload Definition for BasicAudioVolumeSet
/// ----------------------------
/// @ingroup BasicAudioVolumeSet
/// This page describes the payload format for Request messages
/// used by the BasicAudioVolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioVolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioVolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    uint32_t gain_index;               // startChar:4 charSize:4
    uint32_t ramp_db_per_second;       // startChar:8 charSize:4
    uint32_t ramp_milliseconds;        // startChar:12 charSize:4
    int32_t gain_offset;               // startChar:16 charSize:4
}
BasicAudioVolumeSet_REQ_T;             /* '<S132>/UnpackREQ' */


// ------------------------------------------------------------
// RTC RESPONSE PAYLOAD DEFINITIONS
// ------------------------------------------------------------

/// Response Payload Definition for BasicAudioBalanceSet
/// ----------------------------
/// @ingroup BasicAudioBalanceSet
/// This page describes the payload format for Response messages
/// used by the BasicAudioBalanceSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioBalanceSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioBalanceSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
BasicAudioBalanceSet_RSP_T;            /* '<S32>/PackRSP' */

/// Response Payload Definition for BasicAudioBassSet
/// ----------------------------
/// @ingroup BasicAudioBassSet
/// This page describes the payload format for Response messages
/// used by the BasicAudioBassSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioBassSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioBassSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
BasicAudioBassSet_RSP_T;               /* '<S58>/PackRSP' */

/// Response Payload Definition for BasicAudioMuteSet
/// ----------------------------
/// @ingroup BasicAudioMuteSet
/// This page describes the payload format for Response messages
/// used by the BasicAudioMuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioMuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioMuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
BasicAudioMuteSet_RSP_T;               /* '<S78>/PackRSP' */

/// Response Payload Definition for BasicAudioTrebleSet
/// ----------------------------
/// @ingroup BasicAudioTrebleSet
/// This page describes the payload format for Response messages
/// used by the BasicAudioTrebleSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioTrebleSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioTrebleSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
BasicAudioTrebleSet_RSP_T;             /* '<S110>/PackRSP' */

/// Response Payload Definition for BasicAudioVolumeSet
/// ----------------------------
/// @ingroup BasicAudioVolumeSet
/// This page describes the payload format for Response messages
/// used by the BasicAudioVolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioVolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioVolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
BasicAudioVolumeSet_RSP_T;             /* '<S133>/PackRSP' */


// ------------------------------------------------------------
// RTC NOTIFICATION PAYLOAD DEFINITIONS
// ------------------------------------------------------------

/// Notification Payload Definition for BasicAudioBalanceSet
/// ----------------------------
/// @ingroup BasicAudioBalanceSet
/// This page describes the payload format for Notification messages
/// used by the BasicAudioBalanceSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioBalanceSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioBalanceSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
BasicAudioBalanceSet_NTF_T;            /* '<S27>/PackNTF' */

/// Notification Payload Definition for BasicAudioBassSet
/// ----------------------------
/// @ingroup BasicAudioBassSet
/// This page describes the payload format for Notification messages
/// used by the BasicAudioBassSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioBassSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioBassSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
BasicAudioBassSet_NTF_T;               /* '<S53>/PackNTF' */

/// Notification Payload Definition for BasicAudioMuteSet
/// ----------------------------
/// @ingroup BasicAudioMuteSet
/// This page describes the payload format for Notification messages
/// used by the BasicAudioMuteSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioMuteSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioMuteSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t mute_switch;              // startChar:8 charSize:4
}
BasicAudioMuteSet_NTF_T;               /* '<S73>/PackNTF' */

/// Notification Payload Definition for BasicAudioTrebleSet
/// ----------------------------
/// @ingroup BasicAudioTrebleSet
/// This page describes the payload format for Notification messages
/// used by the BasicAudioTrebleSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioTrebleSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioTrebleSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
BasicAudioTrebleSet_NTF_T;             /* '<S105>/PackNTF' */

/// Notification Payload Definition for BasicAudioVolumeSet
/// ----------------------------
/// @ingroup BasicAudioVolumeSet
/// This page describes the payload format for Notification messages
/// used by the BasicAudioVolumeSet RTC mechanism. For a more general
/// description of how this message works, see @ref BasicAudioVolumeSet.
typedef struct
{
    uint32_t msgId;                    // always RTCID_BasicAudioVolumeSet
    uint32_t caller_handle;            // startChar:0 charSize:4
    ASDSTATUS status_code;             // startChar:4 charSize:4
    uint32_t gain_index;               // startChar:8 charSize:4
}
BasicAudioVolumeSet_NTF_T;             /* '<S128>/PackNTF' */


// ------------------------------------------------------------
// PAYLOAD UNION MACROS
// ------------------------------------------------------------
#define ALL_REQ_PAYLOADS               BasicAudioBalanceSet_REQ_T BasicAudioBalanceSet; BasicAudioBassSet_REQ_T BasicAudioBassSet; BasicAudioMuteSet_REQ_T BasicAudioMuteSet; BasicAudioTrebleSet_REQ_T BasicAudioTrebleSet; BasicAudioVolumeSet_REQ_T BasicAudioVolumeSet;
#define ALL_RSP_PAYLOADS               BasicAudioBalanceSet_RSP_T BasicAudioBalanceSet; BasicAudioBassSet_RSP_T BasicAudioBassSet; BasicAudioMuteSet_RSP_T BasicAudioMuteSet; BasicAudioTrebleSet_RSP_T BasicAudioTrebleSet; BasicAudioVolumeSet_RSP_T BasicAudioVolumeSet;
#define ALL_NTF_PAYLOADS               BasicAudioBalanceSet_NTF_T BasicAudioBalanceSet; BasicAudioBassSet_NTF_T BasicAudioBassSet; BasicAudioMuteSet_NTF_T BasicAudioMuteSet; BasicAudioTrebleSet_NTF_T BasicAudioTrebleSet; BasicAudioVolumeSet_NTF_T BasicAudioVolumeSet;


// Request payloads
union AllRtcReqPayloads
{ ALL_REQ_PAYLOADS };

// Response payloads
union AllRtcRspPayloads
{ ALL_RSP_PAYLOADS };

// Notification payloads
union AllRtcNtfPayloads
{ ALL_NTF_PAYLOADS };

#endif

