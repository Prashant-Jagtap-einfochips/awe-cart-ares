/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: BasicAudio_TOP_IDs.h
 *
 * Combined TOP message ID and payload definitions from all cores
 *
 */

#ifndef BASICAUDIO_TOP_IDS_H_
#define BASICAUDIO_TOP_IDS_H_
#include <stdint.h>
#include "ASDID.h"

// ------------------------------------------------------------
// TOP ACCESS TOKENS
// ------------------------------------------------------------
#define TOP_WRITE_BasicAudioBalanceBoostDisable (0x11000000U)
#define TOP_READ_BasicAudioBalanceBoostDisable (0x91000000U)
#define TOP_WRITE_BasicAudioBalanceChanRampMap (0x11000004U)
#define TOP_READ_BasicAudioBalanceChanRampMap (0x91000004U)
#define TOP_WRITE_BasicAudioBalanceEnableSilentExtreme (0x1100000cU)
#define TOP_READ_BasicAudioBalanceEnableSilentExtreme (0x9100000cU)
#define TOP_WRITE_BasicAudioBalanceOffset (0x11000010U)
#define TOP_READ_BasicAudioBalanceOffset (0x91000010U)
#define TOP_WRITE_BasicAudioBalanceRampTime (0x11000014U)
#define TOP_READ_BasicAudioBalanceRampTime (0x91000014U)
#define TOP_WRITE_BasicAudioBalanceSleepingBeautyMutesBass (0x11000018U)
#define TOP_READ_BasicAudioBalanceSleepingBeautyMutesBass (0x91000018U)
#define TOP_WRITE_BasicAudioBalanceTableDb (0x1100001cU)
#define TOP_READ_BasicAudioBalanceTableDb (0x9100001cU)
#define TOP_WRITE_BasicAudioBalanceTableIdx (0x11000094U)
#define TOP_READ_BasicAudioBalanceTableIdx (0x91000094U)
#define TOP_WRITE_BasicAudioBassBoostCoeffs (0x1100010cU)
#define TOP_READ_BasicAudioBassBoostCoeffs (0x9100010cU)
#define TOP_WRITE_BasicAudioBassBoostMaximum_dB (0x11000118U)
#define TOP_READ_BasicAudioBassBoostMaximum_dB (0x91000118U)
#define TOP_WRITE_BasicAudioBassCutCoeffs (0x1100011cU)
#define TOP_READ_BasicAudioBassCutCoeffs (0x9100011cU)
#define TOP_WRITE_BasicAudioBassCutMaximum_dB (0x11000128U)
#define TOP_READ_BasicAudioBassCutMaximum_dB (0x91000128U)
#define TOP_WRITE_BasicAudioBassOffset (0x1100012cU)
#define TOP_READ_BasicAudioBassOffset  (0x9100012cU)
#define TOP_WRITE_BasicAudioBassScale  (0x11000130U)
#define TOP_READ_BasicAudioBassScale   (0x91000130U)
#define TOP_WRITE_BasicAudioBassramptime (0x11000134U)
#define TOP_READ_BasicAudioBassramptime (0x91000134U)
#define TOP_WRITE_BasicAudioMuteRampTime (0x11000138U)
#define TOP_READ_BasicAudioMuteRampTime (0x91000138U)
#define TOP_WRITE_BasicAudioTrebleBoostCoeffs (0x1100013cU)
#define TOP_READ_BasicAudioTrebleBoostCoeffs (0x9100013cU)
#define TOP_WRITE_BasicAudioTrebleBoostMaximum_dB (0x11000148U)
#define TOP_READ_BasicAudioTrebleBoostMaximum_dB (0x91000148U)
#define TOP_WRITE_BasicAudioTrebleCutCoeffs (0x1100014cU)
#define TOP_READ_BasicAudioTrebleCutCoeffs (0x9100014cU)
#define TOP_WRITE_BasicAudioTrebleCutMaximum_dB (0x11000158U)
#define TOP_READ_BasicAudioTrebleCutMaximum_dB (0x91000158U)
#define TOP_WRITE_BasicAudioTrebleOffset (0x1100015cU)
#define TOP_READ_BasicAudioTrebleOffset (0x9100015cU)
#define TOP_WRITE_BasicAudioTrebleScale (0x11000160U)
#define TOP_READ_BasicAudioTrebleScale (0x91000160U)
#define TOP_WRITE_BasicAudioTrebleramptime (0x11000164U)
#define TOP_READ_BasicAudioTrebleramptime (0x91000164U)
#define TOP_WRITE_BasicAudioVolumeRampTime (0x11000168U)
#define TOP_READ_BasicAudioVolumeRampTime (0x91000168U)
#define TOP_WRITE_BasicAudioVolumeTable_dB (0x1100016cU)
#define TOP_READ_BasicAudioVolumeTable_dB (0x9100016cU)
#define TOP_WRITE_BasicAudioVolumemaxgain (0x1100018cU)
#define TOP_READ_BasicAudioVolumemaxgain (0x9100018cU)
#define TOP_WRITE_BasicAudioVolumeTable_Idx (0x11000190U)
#define TOP_READ_BasicAudioVolumeTable_Idx (0x91000190U)


// ------------------------------------------------------------
// TOP ACCESS REQUEST PAYLOADS
// ------------------------------------------------------------
typedef struct
{
    uint32_t topId;                    // Any TOP_WRITE_ token
    uint32_t callerHandle;
    uint32_t offset;
    uint32_t byteCount;
    uint8_t data[];
}
TOP_WriteRequest_type;

typedef TOP_WriteRequest_type TOP_Request_type;// backward compatibility
typedef struct
{
    uint32_t topId;                    // Any TOP_READ_ token
    uint32_t callerHandle;
    uint32_t offset;
    uint32_t byteCount;
}
TOP_ReadRequest_type;


// ------------------------------------------------------------
// TOP ACCESS RESPONSE PAYLOADS
// ------------------------------------------------------------
// See also AsdIdStatus_type in ASDID.h
typedef struct
{
    uint32_t topId;
    uint32_t callerHandle;
    ASDSTATUS statusCode;
}
TOP_WriteResponse_type;

typedef struct
{
    uint32_t topId;
    uint32_t callerHandle;
    ASDSTATUS statusCode;
    uint32_t byteCount;
    uint8_t data[];
}
TOP_ReadResponse_type;



#endif

