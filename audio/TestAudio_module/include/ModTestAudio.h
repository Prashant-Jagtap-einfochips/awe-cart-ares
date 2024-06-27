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

#define MASK_TestAudio_gain 0x00000100
#define MASK_TestAudio_coeff 0x00000200
#define OFFSET_TestAudio_gain 0x00000008
#define OFFSET_TestAudio_coeff 0x00000009

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
    INT32              gain;                // Linear gain
    FLOAT32*           coeff;               // Coefficent Data
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
