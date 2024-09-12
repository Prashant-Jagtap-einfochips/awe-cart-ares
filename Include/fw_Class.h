/*******************************************************************************
*
*				Audio Framework
*				---------------
*
********************************************************************************
*	  fw_class.h
********************************************************************************
*
*	  Description:	AudioWeaver Framework class definitions
*
*	  Copyright:	(c) 2007-2021 DSP Concepts, Inc. All rights reserved.
*					3235 Kifer Road
*					Santa Clara, CA 95054
*
*******************************************************************************/
#ifndef _FW_CLASS_H
#define _FW_CLASS_H

#ifdef	__cplusplus
extern "C" {
#endif

/* --------- Class identifiers. */
#define CLASS_ID_BASE							0xBEEF0000U
#define CLASS_ID_END							0xBEEFFFFFU
#define CLASS_ID_MASK							0xFFFF0000U

#define CLASS_ID_INPUTPIN						(CLASS_ID_BASE + 1U)
#define CLASS_ID_OUTPUTPIN						(CLASS_ID_BASE + 2U)
#define CLASS_ID_PIN							(CLASS_ID_BASE + 3U)
#define CLASS_ID_LAYOUT							(CLASS_ID_BASE + 4U)

#define CLASS_ID_PINBASE						CLASS_ID_INPUTPIN
#define CLASS_ID_PINEND							CLASS_ID_PIN

/*	Class IDs for wires */
#define CLASS_ID_WIREBASE						(CLASS_ID_BASE + 0x80U)
#define CLASS_ID_WIREEND						(CLASS_ID_BASE + 0x7FFU)

#define CLASS_ID_WIRE							(CLASS_ID_WIREBASE)

/* Class IDs for modules */
#define CLASS_ID_MODBASE						(CLASS_ID_BASE + 0x800U)
#define CLASS_ID_MODEND							(CLASS_ID_END)

/* Instance name IDs */
#define NAME_BASE								0x7EEDB000U

#define NAME_INPUT_INTERLEAVED					(NAME_BASE + 1U)
#define NAME_OUTPUT_INTERLEAVED					(NAME_BASE + 2U)


/** Test if a given class is valid. */
#define IsClassValid(classID) \
	(((classID) >= CLASS_ID_BASE) && ((classID) <= CLASS_ID_END))

/** Test if a given class ID is that of a wire. */
#define IsClassWire(classID) \
	(((classID) >= CLASS_ID_WIREBASE) && ((classID) <= CLASS_ID_WIREEND))

/** Test if a given class ID is that of module. */
#define IsClassModule(classID) \
	(((classID) >= CLASS_ID_MODBASE) && ((classID) <= CLASS_ID_MODEND))

/** Test if a given class ID is that of a layout. */
#define IsClassLayout(classID) \
	((classID) == CLASS_ID_LAYOUT)

#if 0
	// The concept of pin class has gone away.
/** Test if a given class ID is that of PIN type. */
#define IsClassPin(classID) \
	(((classID) >= CLASS_ID_PINBASE) && ((classID) <= CLASS_ID_PINEND))
#endif


//----------

/* Handle packed pin (wire) counts. */
// this could be its own data type

/** Get input pin (wire) count from nIO. */
#define GetInputPinCount(x)			((x) & 0xffU)

/** Get output pin (wire) count from nIO. */
#define GetOutputPinCount(x)		(((x) >> 8) & 0xffU)

/** Get scratch pin (wire) count from nIO. */
#define GetScratchPinCount(x)		(((x) >> 16) & 0xffU)

#if 0		// Never used
/** Get feedback pin (wire) count from nIO. */
#define GetFeedbackPinCount(x)		(((x) >> 24) & 0xffU)
#endif

/** Get total pin (wire) count from nIO. */
#define GetWireCount(x)				( (((x) >> 24) & 0xffU) + (((x) >> 16) & 0xffU) + (((x) >> 8) & 0xffU) + ((x) & 0xffU))

#if 0		// Never used
#define MakeIOSize(nInputs, nOutputs, blockSize) \
	(((blockSize) << 16) | ((nOutputs) << 8) | (nInputs))
#endif

/** A class descriptor. Common to all classes. */
/*private*/ typedef struct _ClassDescriptor
{
	/** All constructors look the same. To make debugging easy, we pass a generic
	array of words to the constructor, which needs to figure out what to do with
	them.

	On return, we have a pointer to an initialized instance ready for use.
	*/
	struct _InstanceDescriptor *(*Constructor)(struct _AWEInstance *pAWE, INT32 *retVal, INT32 srate, UINT32 info1, UINT32 info2, UINT32 info3);

	/** Unique ID of the class - set at compile time. */
	UINT32 classID;
}
ClassDescriptor;


/** An instance descriptor. Common to all instances. */
/*private*/ typedef struct _InstanceDescriptor
{
	/** Chain instances together in a linked list. */
	struct _InstanceDescriptor *pNextInstance;		/* 0 */

	/** Pointer back to the static descriptor from which this instance was
	created. */
	const ClassDescriptor *pClassDescriptor;		/* 4 */

	/** The unique instance ID of this instance. */
	UINT32 nUniqueInstanceID;						/* 8 */
}
InstanceDescriptor;		/* 12 bytes */


#ifdef	__cplusplus
}
#endif

#endif	/* _FW_CLASS_H */

