/*******************************************************************************
*
*				Audio Framework
*				---------------
*
********************************************************************************
*	  fw_Module.h
********************************************************************************
*
*	  Description:	AudioWeaver Framework Module header file
*
*	  Copyright:	(c) 2007-2021 DSP Concepts, Inc. All rights reserved.
*					3235 Kifer Road
*					Santa Clara, CA 95054
*
*******************************************************************************/
#ifndef _FW_MODULE_H
#define _FW_MODULE_H

#include "fw_Common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*private*/ typedef struct _ModClassDescriptor
{
	/** All constructors look the same. To make debugging easy, we pass a generic
	array of words to the constructor, which needs to figure out what to do with
	them.

	On return, we have a pointer to an initialized instance ready for use.
	*/
	struct _ModInstanceDescriptor *(*Constructor)(
			INT32 * FW_RESTRICT retVal,
			UINT32 nIO,
			WireInstance **pWires,
			size_t argCount,
			const Sample* FW_RESTRICT args);		/* 0 */

	/** Unique ID of the class - set at compile time. */
	UINT32 classID;									/* 4 */
}
ModClassDescriptor;		/* 8 bytes */

/** Class descriptor for audio modules. */
/*private*/ typedef struct _ModClassModule
{
	/** The basic class data. */
	ModClassDescriptor modClassDescriptor;				/* 0 */

	/** Pump function for the module. */
	void (*pProcessFunc)(void *pInstance);				/* 8 */

	/** Bypass function for the module. */
	void (*pBypassFunc)(void *pInstance);				/* 12 */

	/** Set function. */
	UINT32 (*pSet)(void *pInstance, UINT32 mask);		/* 16 */

	/** Get function. */
	UINT32 (*pGet)(void *pInstance, UINT32 mask);		/* 20 */

	/** This value is unused */
	UINT32 nModuleVersion;								/* 24 */

	/** The number of construction module parameters. */
	UINT32 nPackedParameters;							/* 28 */

	/** Vector of 64 bits specifying the type of constructor
	arguments. Bit 0 starts at the first member past the
	instance object. Bit set means argument is float.
	Bits past the last argument are don't care, should be 0. */
	UINT32 bitVectorFloat[2];							/* 32 */
}
ModClassModule;		/* 40 bytes */

/** Set the packed argument count nPackedParameters. */
#define ClassModule_PackArgCounts(nPublicArgs, nPrivateArgs) \
	(((nPrivateArgs) << 16) | ((nPublicArgs) & 0xffffU))

/** Get the number of public arguments from nPackedParameters. */
#define ClassMod_GetNPublicArgs(packed) \
	((packed) & 0xffffU)

/** Get the number of private arguments from nPackedParameters. */
#define ClassMod_GetNPrivateArgs(packed) \
	(((packed) >> 16) & 0xffffU)


/* This macro declares a module class. For 64 bits we have an extra member
 * which is a vector of member offsets to translate indexes to struct offsets. */

#ifdef BUILD64
/* 64 bit version has an array of member offsets following. */
#define CREATE_MODULE_CLASS(class_name, nMembers)	\
	typedef struct EVAL(_, class_name) \
	{ \
		ModClassModule modBaseClassDescriptor; \
		UINT32 m_offset[(nMembers <= 0)? 1: nMembers]; \
	} class_name;

#else
/* 32 bit version is just an alias for the module class. */
#define CREATE_MODULE_CLASS(class_name, nMembers)	\
	typedef struct EVAL(_, class_name) \
	{ \
		ModClassModule modBaseClassDescriptor; \
	} class_name;

#endif


/** A module instance descriptor. Common to all module instances. */
/*private*/ typedef struct _ModInstanceDescriptor
{
	/** Chain instances together in a linked list. */
	struct _ModInstanceDescriptor *pNextInstance;		/* 0 */

	/** Pointer back to the static descriptor from which this instance was
	created. */
	const ModClassModule *pModClassDescriptor;			/* 4 */

	/** The unique instance ID of this instance. */
	UINT32 nUniqueInstanceID;							/* 8 */
}
ModInstanceDescriptor;		/* 12 bytes */


/* Low order 2 bits of flags high byte are the module active state. */

/** Mask for active bits. */
#define MODULE_ACTIVE_MASK			0x03000000U

/** Module is active. */
#define MODULE_ACTIVE				0x00000000U

/** Module is bypassed. */
#define MODULE_BYPASS				0x01000000U

/** Module is muted. */
#define MODULE_MUTE					0x02000000U

/** Module is inactive. */
#define MODULE_INACTIVE				0x03000000U


/** This bit allows for deferred execution of the module's set function in the main thread. */
#define MODULE_GENERAL_DEFERRED_SET			0x08000000U

/** Test if the deferred bit is set. */
#define awe_modIsDeferredSet(S)				((((S)->instance.packedFlags) & (MODULE_GENERAL_DEFERRED_SET)) != (0))

/** Set the deferred bit to v */
//#define awe_modSetDeferredSet(S, v)			(S)->instance.packedFlags &= ~MODULE_GENERAL_DEFERRED_SET; (S)->instance.packedFlags |= ((v & 1U) << 27); g_pAWE_instance->m_general_flags |= 0x80
void awe_modSetDeferredSet(void *S, UINT32 v);


/** High 4 bits of flags high byte are available for any use. */
#define MODULE_GENERAL_MASK			0xf0000000U

/** Get the general flag bits. */
#define GetGeneralFlagBits(S)		((((S)->instance.packedFlags) >> (28)) & (0x1fU))

/** Set the general flag bits to v. */
#define SetGeneralFlagBits(S, v)	(S)->instance.packedFlags &= ~MODULE_GENERAL_MASK; (S)->instance.packedFlags |= ((v & 0xfU) << 28)

/** Mask to be evaluated in the module set method to trigger all set variables */
#define SETALLMASK (0xFFFFFF00)

/** Trigger all module set variables to be computed */	
#define AWE_CLASSMODULE_SETALL(pMod) ClassModule_Set((ModuleInstanceDescriptor*)pMod, SETALLMASK)


/** A module instance. The instance is variable length on the heap, the size
being determined by the constructor arguments. */
/*private*/ typedef struct _ModuleInstanceDescriptor
{
	/** The basic instance data. */
	ModInstanceDescriptor instanceDescriptor;			/* 0 */

	/** Pointer to owning layout instance (type is a forward reference). */
	struct _LayoutInstance *pOwner;						/* 12 */

	/** Array of input and output wires. There are nInwires + nOutWires + nScratchWires elements. */
	WireInstance **pWires;								/* 16 */

	/** Pump function for the module. May be empty, pProcessFunc, or, pBypassFunc. */
	void (*pProcessFunc)(void *pInstance);				/* 20 */

	/** Control flags. */
	UINT32 packedFlags;									/* 24 */

	/** Module specific profiling time.	 This is number of cycles times 256. */
	UINT32 profileTime;									/* 28 */
}
ModuleInstanceDescriptor;		/* 32 bytes */

/*-------------------- Generic ---------------------*/
/* Bypass function that fills all output channels with 1.0 */
void IOAllOnesModule_Bypass(void *pInstance);

/** Bypass function for any module that has corresponding inputs and outputs. */
void IOMatchModule_Bypass(void *pInstance);

/** Bypass function for modules that copies channels straight across */
void IOAcrossModule_Bypass(void *pInstance);

/* Bypass function for modules with disparate I/O.  It matches up inputs and outputs as best as
** it can. */

void IOMatchUpModule_Bypass(void *pInstance);

/** Mute function for any module. */
void GenericModule_Mute(void *pInstance);

/** Inactive function for any module. */
void GenericModule_Inactive(void *pInstance);

/** Call a module's set function. */
INT32 ClassModule_Set(ModuleInstanceDescriptor *pInstance, UINT32 mask);

/** Call a module's get function. */
#define ClassModule_Get(pInstance, mask) \
if ((pInstance) != NULL) { \
	UINT32 (*pGet)(void *a, UINT32 b) = (pInstance)->instanceDescriptor.pModClassDescriptor->pGet; \
	if (pGet != NULL) { \
		(void)pGet(pInstance, mask); \
	} \
}

/** Pack the sizes into the flags word. */
#define ClassModule_PackFlags(nInWires, nOutWires, nScratchWires) \
	((((nScratchWires) & 0xffU) << 16) | (((nOutWires) & 0xffU) << 8) | ((nInWires) & 0xffU))

/** Get the input wire count 0..255 */
#define ClassMod_GetNInWires(packed) \
	((packed) & 0xffU)

/** Get the output wire count 0..255 */
#define ClassMod_GetNOutWires(packed) \
	(((packed) >> 8) & 0xffU)

/** Get the scratch wire count. */
#define ClassMod_GetNScratchWires(packed) \
	(((packed) >> 16) & 0xffU)

/** Get the total number of wires */
#define ClassMod_GetWireCount(packed) \
	(((packed) & 0xffU) + (((packed) >> 8) & 0xffU) + (((packed) >> 16) & 0xffU))

/** Get the module state. */
#define ClassMod_GetModuleState(packed) \
	(((packed) >> 24) & 0x03U)


/** These macros operate on high-level module objects. The goal is to hide the
	details of the implementation from the module writer. */

/** Get the wires that are attached to the module */
#define ClassModule_GetWires(S)		((S)->instance.pWires)

/** Get the input wire count 0..255 */
#define ClassModule_GetNInWires(S) \
	(ClassMod_GetNInWires((S)->instance.packedFlags))

/** Get the output wire count 0..255 */
#define ClassModule_GetNOutWires(S) \
	(ClassMod_GetNOutWires((S)->instance.packedFlags))

/** Get the scratch wire count. */
#define ClassModule_GetNScratchWires(S) \
	(ClassMod_GetNScratchWires((S)->instance.packedFlags))

/** Get the module state. */
#define ClassModule_GetModuleState(S) \
	(ClassMod_GetModuleState((S)->instance.packedFlags))

/** Get the module objectID. */
#define ClassModule_GetObjectID(S) \
	((S)->instance.instanceDescriptor.nUniqueInstanceID)
	

/**
 * @brief Directly calls the module process function without profiling
 * @param [in] pModule				the module to process
 */
/*private*/ static VEC_INLINE void ClassModule_ExecuteNP(ModuleInstanceDescriptor *pModule)
{
	pModule->pProcessFunc(pModule);
}

/**
 * @brief Base class module instance constructor.
 * @param [in] pClass				class of object to construct
 * @param [out] retVal				return error code
 * @param [in] nIO					packed wire info
 * @param [in] pWires				array of wires
 * @param [in] argCount				number of arguments
 * @param [in] args					the arguments
 * @return							constructed object or NULL on error
 *
 * This function must be called first to construct the base module, then derived
 * classes may operate on the constructed object further.
 */
/*private*/ ModInstanceDescriptor *BaseClassModule_Constructor(
		const ModClassModule * FW_RESTRICT pClass,
		INT32 * FW_RESTRICT retVal,
		UINT32 nIO,
		WireInstance * const * FW_RESTRICT pWires,
		size_t argCount,
		const Sample * FW_RESTRICT args);

/**
 * @brief Generic module instance constructor.
 * @param [in] classID				class ID of object to construct
 * @param [out] retVal				return error code
 * @param [in] nIO					packed wire info
 * @param [in] pWires				array of wires
 * @param [in] argCount				number of arguments
 * @param [in] args					the arguments
 * @return							constructed object or NULL on error
 */
/*private*/ ModInstanceDescriptor *ClassModule_Constructor(
		UINT32 classID,
		INT32 * FW_RESTRICT retVal,
		UINT32 nIO,
		WireInstance **pWires,
		size_t argCount,
		const Sample * FW_RESTRICT args);

/**
 * @brief Base class module instance constructor.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] pClass				class of object to construct
 * @param [out] retVal				return error code
 * @param [in] nIO					packed wire info
 * @param [in] pWireIDs				array of wire IDs
 * @param [in] argCount				number of arguments
 * @param [in] args					the arguments
 * @return							constructed object or NULL on error
 *
 * This function must be called first to construct the base module, then derived
 * classes may operate on the constructed object further.
 */
/*private*/ ModInstanceDescriptor *awe_fwModuleClassBaseConstructor(struct _AWEInstance *pAWE,
		const ModClassModule * FW_RESTRICT pClass,
		INT32 * FW_RESTRICT retVal,
		UINT32 nIO,
		WireInstance * const * FW_RESTRICT pWires,
		size_t argCount,
		const Sample * FW_RESTRICT args);

/**
 * @brief Generic module instance constructor.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] classID				class ID of object to construct
 * @param [out] retVal				return error code
 * @param [in] nIO					packed wire info
 * @param [in] pWireIDs				array of wire IDs
 * @param [in] argCount				number of arguments
 * @param [in] args					the arguments
 * @return							constructed object or NULL on error
 */
/*private*/ ModInstanceDescriptor *awe_fwModuleClassConstructorIDs(
		struct _AWEInstance *pAWE,
		UINT32 classID,
		INT32 * FW_RESTRICT retVal,
		UINT32 nIO,
		const UINT32 *pWireIDs,
		size_t argCount,
		const Sample * FW_RESTRICT args);

/**
 * @brief Generic module instance constructor.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] classID				class ID of object to construct
 * @param [out] retVal				return error code
 * @param [in] nIO					packed wire info
 * @param [in] pWires				array of wires
 * @param [in] argCount				number of arguments
 * @param [in] args					the arguments
 * @return							constructed object or NULL on error
 */
/*private*/ ModInstanceDescriptor *awe_fwModuleClassConstructorObjs(
		struct _AWEInstance *pAWE,
		UINT32 classID,
		INT32 * FW_RESTRICT retVal,
		UINT32 nIO,
		WireInstance **pWires,
		size_t argCount,
		const Sample * FW_RESTRICT args);
		
#ifdef	__cplusplus
}
#endif

#endif	/* _FW_MODULE_H */

