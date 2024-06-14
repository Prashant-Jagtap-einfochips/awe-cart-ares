/****************************************************************************
*
*               Audio Framework
*               ---------------
*
****************************************************************************
*     ModControl.h
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

#ifndef _MOD_CONTROL_H
#define _MOD_CONTROL_H

#include "ModCommon.h"
#include "MathHelper.h"
#include "ModTestAudio.h"

#define MASK_Control_coeff 0x00000100
#define OFFSET_Control_coeff 0x00000008

#define CLASSID_CONTROL (CLASS_ID_MODBASE + 60120)

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------
// Overall instance class
// ----------------------------------------------------------------------

typedef struct _awe_modControlInstance
{
    ModuleInstanceDescriptor instance;
    
    FLOAT32*           coeff;               // Coefficent Data
    awe_modTestAudioInstance* pTestAudio;
} awe_modControlInstance;

#if !defined(NOREDEF)
extern const ModClassModule awe_modControlClass;
#endif // #if !defined(NOREDEF)

/* Dynamic instantiation is used by default.  When building for static
** code, define AWE_STATIC_CODE to eliminate the constructor function. */

#ifndef AWE_STATIC_CODE
ModInstanceDescriptor * awe_modControlConstructor(INT32 * FW_RESTRICT retVal, UINT32 nIO, WireInstance ** FW_RESTRICT pWires, size_t argCount, const Sample * FW_RESTRICT args);
#endif // #ifndef AWE_STATIC_CODE


void awe_modControlProcess(void *pInstance);

UINT32 awe_modControlSet(void *pInstance, UINT32 mask);
 
UINT32 awe_modControlGet(void *pInstance, UINT32 mask);



#ifdef __cplusplus
}
#endif


#endif // _MOD_CONTROL_H

/**
 * @}
 *
 * End of file.
 */
