/****************************************************************************
*
*               Audio Framework
*               ---------------
*
****************************************************************************
*     ModTestAudio.h
****************************************************************************
*
*     Description:  Copies input wire to output wire
*
*     Copyright:    2007-2022 DSP Concepts, Inc. All rights reserved.
*                   3235 Kifer Road
*                   Santa Clara, CA 95054
*
***************************************************************************/

/**
 * @addtogroup Modules
 * @{
 */

/**
 * @file
 * @brief Copies input wire to output wire
 */

#ifndef _MOD_TESTAUDIO_H
#define _MOD_TESTAUDIO_H

#include "ModCommon.h"
#include "MathHelper.h"

#define NUM_CHANNELS_IN 2U
#define NUM_CHANNELS_OUT 2U
#define SAMPLERATE 48000U
#define BLOCKSIZE 32U
#define BASE_TASKID 0U
#define EXT_SCHEDULER 1U
#define CONTROL_MAX_PAYLOAD_SIZE 256U
#define MASK_TestAudio_validCtrlRsp 0x00000100
#define MASK_TestAudio_algoState 0x00000200
#define MASK_TestAudio_gain 0x00000400
#define MASK_TestAudio_STATE 0x00000800
#define MASK_TestAudio_logCTRL 0x00001000
#define MASK_TestAudio_CONTROL_REQUEST 0x00002000
#define MASK_TestAudio_CONTROL_RESPONSE 0x00004000
#define MASK_TestAudio_coeff 0x00008000
#define MASK_TestAudio_pHandle 0x00010000
#define OFFSET_TestAudio_validCtrlRsp 0x00000008
#define OFFSET_TestAudio_algoState 0x00000009
#define OFFSET_TestAudio_gain 0x0000000A
#define OFFSET_TestAudio_STATE 0x0000000B
#define OFFSET_TestAudio_logCTRL 0x0000000C
#define OFFSET_TestAudio_CONTROL_REQUEST 0x0000000D
#define OFFSET_TestAudio_CONTROL_RESPONSE 0x0000000E
#define OFFSET_TestAudio_coeff 0x0000000F
#define OFFSET_TestAudio_pHandle 0x00000010

#define CLASSID_TESTAUDIO (CLASS_ID_MODBASE + 60110)

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------
// Overall instance class
// ----------------------------------------------------------------------

typedef struct _awe_modTestAudioInstance
{
    ModuleInstanceDescriptor instance;
    INT32              validCtrlRsp;        // valid control response
    INT32              algoState;           // algorithm state active/bypass
    INT32              gain;                // Linear gain
    INT32              STATE;               // Toggles AudioPP Active/Bypass
    INT32              logCTRL;             // Debug Start/Stop
    UINT32*            CONTROL_REQUEST;     // Control Message Request
    UINT32*            CONTROL_RESPONSE;    // Control Message Response
    FLOAT32*           coeff;               // Coefficent Data
    void *             pHandle;             // Instance handle
} awe_modTestAudioInstance;

#if !defined(NOREDEF)
extern const ModClassModule awe_modTestAudioClass;
#endif // #if !defined(NOREDEF)

/* Dynamic instantiation is used by default.  When building for static
** code, define AWE_STATIC_CODE to eliminate the constructor function. */

#ifndef AWE_STATIC_CODE
ModInstanceDescriptor * awe_modTestAudioConstructor(INT32 * FW_RESTRICT retVal, UINT32 nIO, WireInstance ** FW_RESTRICT pWires, size_t argCount, const Sample * FW_RESTRICT args);
#endif // #ifndef AWE_STATIC_CODE


void awe_modTestAudioProcess(void *pInstance);

UINT32 awe_modTestAudioSet(void *pInstance, UINT32 mask);
 
UINT32 awe_modTestAudioGet(void *pInstance, UINT32 mask);



#ifdef __cplusplus
}
#endif


#endif // _MOD_TESTAUDIO_H

/**
 * @}
 *
 * End of file.
 */
