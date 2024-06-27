/*******************************************************************************
*
********************************************************************************
*     ControlAudio_ModuleListInst0.h  Created on 21-Jun-2024 15:54:44
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

#define TOTALNUMBEROFCLASSES 7

extern const UINT32 awe_modDCSourceIntClass;
extern const UINT32 awe_modDeinterleaveClass;
extern const UINT32 awe_modInterleaveClass;
extern const UINT32 awe_modParamSetClass;
extern const UINT32 awe_modSinkClass;
extern const UINT32 awe_modTestAudioClass;
extern const UINT32 awe_modTypeConversionClass;


#define LISTOFCLASSOBJECTS \
&awe_modDCSourceIntClass, \
&awe_modDeinterleaveClass, \
&awe_modInterleaveClass, \
&awe_modParamSetClass, \
&awe_modSinkClass, \
&awe_modTestAudioClass, \
&awe_modTypeConversionClass
