/*******************************************************************************
*
********************************************************************************
*     ControlAudio_ModuleListInst0.h  Created on 29-Jul-2024 15:08:42
********************************************************************************
*
*     Description:  Module list for the target
*
*     Copyright: (c) 2023 DSP Concepts, Inc. All rights reserved.
*                         3235 Kifer Road
*                         Santa Clara, CA 95054-1527
*
*******************************************************************************/
#include "AWECore.h"

#define TOTALNUMBEROFCLASSES 8

extern const UINT32 awe_modControlClass;
extern const UINT32 awe_modDCSourceIntClass;
extern const UINT32 awe_modDeinterleaveClass;
extern const UINT32 awe_modInterleaveClass;
extern const UINT32 awe_modParamSetClass;
extern const UINT32 awe_modSinkClass;
extern const UINT32 awe_modTestAudioClass;
extern const UINT32 awe_modTypeConversionClass;


#define LISTOFCLASSOBJECTS \
&awe_modControlClass, \
&awe_modDCSourceIntClass, \
&awe_modDeinterleaveClass, \
&awe_modInterleaveClass, \
&awe_modParamSetClass, \
&awe_modSinkClass, \
&awe_modTestAudioClass, \
&awe_modTypeConversionClass
