/*******************************************************************************
*
*				Audio Framework
*				---------------
*
********************************************************************************
*	  Framework.h
********************************************************************************
*
*	  Description:	AudioWeaver Framework main header file
*
*	  Copyright:	(c) 2007-2021 DSP Concepts, Inc. All rights reserved.
*					3235 Kifer Road
*					Santa Clara, CA 95054
*
*******************************************************************************/
#ifndef _FRAMEWORK_H
#define _FRAMEWORK_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*private*/ struct _AWEInstance;

#include "TargetProcessor.h"
#include "fw_Class.h"
#include "fw_Pin.h"
#include "fw_Wire.h"
#include "fw_Module.h"
#include "fw_Layout.h"
#include "fw_FlashFSInstance.h"
#include "fw_AWEInstance.h"
#include "fw_Heap.h"
#include "fw_ModuleClassAccess.h"

//------------------------------------------------------------------------------
/* Global objects */
//------------------------------------------------------------------------------

/** Used in special modules that need to access the framework. */
extern AWE_THREAD AWEInstance *g_pAWE_instance;

/** The table of classes known to the framework. */
extern const ClassDescriptor *g_class_descriptor_table[2];

#ifdef BUILD64
/** Table of offsets for modules. */
extern const UINT32 g_module_class_offsets[];

/** Table of offsets for module instances. */
extern const UINT32 g_module_instance_offsets[];

/** Table of offsets for wires. */
extern const UINT32 g_wire_offsets[];

/** Table of offsets for layout instances. */
extern const UINT32 g_layout_offsets[];

/** Dummy for computing 64 bit offsets without having the actual module class available. */
/*private*/ typedef struct _ModClassModule2
{
	ModClassModule module;
	UINT32 m_offset[1];
} ModClassModule2;
#endif

/** The table of module descriptions. Truly global. */
extern const ModClassModule *g_module_descriptor_table[];

/** The number of elements in g_module_descriptor_table. Truly global. */
extern UINT32 g_module_descriptor_table_size;

/** Class descriptor for layouts. */
extern const ClassDescriptor LayoutClass_Descriptor;

/** Class descriptor for wires. */
extern const ClassDescriptor WireClass_Descriptor;

#ifdef	__cplusplus
}
#endif

#endif	/* _FRAMEWORK_H */
