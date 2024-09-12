/*******************************************************************************
*
*				Audio Framework
*				---------------
*
********************************************************************************
*	  fw_Pin.h
********************************************************************************
*
*	  Description:	AudioWeaver Framework pin header file
*
*	  Copyright:	(c) 2007-2021 DSP Concepts, Inc. All rights reserved.
*					3235 Kifer Road
*					Santa Clara, CA 95054
*
*******************************************************************************/
#ifndef _FW_PIN_H
#define _FW_PIN_H

#include "fw_Common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/** Bit field used to check if the calling order is awe_audioGetPumpMask -> awe_audioExportSample or awe_audioExportSample -> awe_audioGetPumpMask. */
#define PIN_PUMP_MASK_FIRST		0x01U

/** Bit field used to clear internal parameters on the first call to awe_audioExportSample. */
#define PIN_EXPORT_ONCE			0x02U

/** Macros to Set/Clear/Check bit field for PIN_PUMP_MASK_FIRST. */
#define SET_PIN_PUMP_MASK_FIRST(pPin)	((pPin)->m_pin_flags |= PIN_PUMP_MASK_FIRST)
#define CLR_PIN_PUMP_MASK_FIRST(pPin)	((pPin)->m_pin_flags &= ~PIN_PUMP_MASK_FIRST)
#define IS_PIN_PUMP_MASK_FIRST(pPin)	((pPin)->m_pin_flags & PIN_PUMP_MASK_FIRST)

/** Macros to Set/Clear/Check bit field for PIN_EXPORT_ONCE. */
#define SET_EXPORT_ONCE(pPin)	((pPin)->m_pin_flags |= PIN_EXPORT_ONCE)
#define CLR_EXPORT_ONCE(pPin)	((pPin)->m_pin_flags &= ~PIN_EXPORT_ONCE)
#define IS_EXPORT_ONCE(pPin)	((pPin)->m_pin_flags & PIN_EXPORT_ONCE)

/** Description of bound wires. */
/*private*/ typedef struct _CoreWireDescr
{
	/** The core this wire is on. */
	UINT32 coreID;					/* 4 */

	/** The wire on the core - pointer in that core's address space. */
	struct _WireInstance *pWire;	/* 8 */
} CoreWireDescr;

/** An I/O pin instance. These objects are defined at compile
time to match the physical I/O. */

#define IOPINDESCRIPTOR_FRAMEWORK                                                               \
	/** The basic instance data. */                                                             \
	InstanceDescriptor instanceDescriptor;	/* 12 */                                            \
                                                                                                \
	/** Pointer to the buffer allocated by the wire - copied from the wire at bind time. */     \
	Sample *pOrigBuffer;					/* 16 */                                            \
                                                                                                \
	/** Pointer to the double buffer allocated at bind time. */                                 \
	Sample *pSecondBuffer;					/* 20 */                                            \
                                                                                                \
	/** Pin sample rate. */                                                                     \
	float sampleRate;						/* 24 */                                            \
                                                                                                \
	/* Number of channels		10 bits - 0x3ff     */                                          \
	/* Max block size			17 bits - 0x7fffC00 */                                          \
	/* isComplex				1 bit - 0x8000000   */                                          \
	/* Sample size in bytes.	4 bits - 0xf0000000 */                                          \
	UINT32 wireInfo1;						/* 28 */                                            \
                                                                                                \
	/* Block size				17 bits - 0x1ffff   */                                          \
	/* Data Type				6 bits - 0x7e0000   */                                          \
	UINT32 wireInfo2;						/* 32 */                                            \
                                                                                                \
	/* Rows						10 bits - 0x3ff     */                                          \
	/* Columns					10 bits - 0xffc00   */                                          \
	/* IsIPC					1 bit - 0x100000    */                                          \
	/* IsPrivate				1 bit - 0x200000    */                                          \
	/* Clock master				1 bit - 0x400000    */                                          \
	UINT32 wireInfo3;						/* 36 */                                            \
                                                                                                \
	/** Flags to control buffer flipping. */                                                    \
	UINT32 ctrlFlags;						/* 40 */                                            \
                                                                                                \
	/** Number of bound wires. */                                                               \
	UINT32 nBoundWires;						/* 44 */                                            \
                                                                                                \
	/** Array of bound wires. */                                                                \
	CoreWireDescr boundWires[1];			/* 44 + 8 = 52 */									\
                                                                                                \
	/** Optional named pin. */                                                                  \
	UINT32 m_pinName[2];					/* 60 */                                            \
                                                                                                \
	/** Flags to control buffer flipping. */                                                    \
	UINT32 tmpCtrlFlags;				    /* 64 */                                            \
                                                                                                \
	/** Variable to maintain boolean flas. */													\
	UINT32 m_pin_flags;						/* 68 */                                            \

#include "Pin.h"

/** Test if a pin's size is compatible with an I/O pin's size. */
#define IsCompatiblePinSize(wireBlock, pinBlock) \
	(wireBlock % pinBlock == 0)

/**
 * @brief Returns a pointer to where the platform should write input data.
 * @param [in] pinIdx				which input pin
 * @param [in] chan					which channel
 * @param [out] stride				data stride
 * @return							write pointr or NULL on error
 */
/*private*/ INT32* awe_fwGetInputChannelPtr(const struct _AWEInstance *pAWE, UINT32 pinIdx, UINT32 chan, INT32 *stride);

/**
 * @brief Returns a pointer to where the platform should read output data from.
 * @param [in] pinIdx				which input pin
 * @param [in] chan					which channel
 * @param [out] stride				data stride
 * @return							read pointr or NULL on error
 */
/*private*/ INT32* awe_fwGetOutputChannelPtr(const struct _AWEInstance *pAWE, UINT32 pinIdx, UINT32 chan, INT32 *stride);

/**
 * @brief Handles double buffering of the Layout's I/O pins.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] pinIdx				which input pin
 * #param [in] samplesPerTick		number of samples this tick
 * @return							bitvector of ready layouts
 */
/*private*/ UINT32 awe_fwAudioDMAComplete(const struct _AWEInstance *pAWE, UINT32 pinIdx, INT32 samplesPerTick);

/**
 * @brief Get the first I/O object in the object chain.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [out] pObject				pointer to receive first object pointer
 * @param [out] pClassID			pointer to receive first object class
 * @param [out] sampleRate			pin sample rate
 * @param [out] info1				pin info1
 * @param [out] info2				pin info2
 * @param [out] info3				pin info3
 * @param [out] name0				first name word
 * @param [out] name1				second name word
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwGetFirstIO(const struct _AWEInstance* pAWE, InstanceDescriptor** pObject, UINT32* pClassID, float* sampleRate,
	UINT32* info1, UINT32* info2, UINT32* info3, UINT32* name0, UINT32* name1);

/**
 * @brief Get the next I/O object in the object chain.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] pinID				ID of current object
 * @param [out] pObject				pointer to receive next object pointer
 * @param [out] pClassID			pointer to receive next object class
 * @param [out] sampleRate			pin sample rate
 * @param [out] info1				pin info1
 * @param [out] info2				pin info2
 * @param [out] info3				pin info3
 * @param [out] name0				first name word
 * @param [out] name1				second name word
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwGetNextIO(const struct _AWEInstance* pAWE, UINT32 pinID, InstanceDescriptor** pObject, UINT32* pClassID, float* sampleRate,
	UINT32* info1, UINT32* info2, UINT32* info3, UINT32* name0, UINT32* name1);

/**
 * @brief Get the type of a pin.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] wireID				ID of pin to get
 * @param [out] pClassID			class of pin
 * @param [out] pBoundID			ID of bound wire
 * @param [out] sampleRate			sample rate of wire
 * @param [out] info1				first info word
 * @param [out] info2				second info word
 * @param [out] info3				third info word
 * @param [out] name0				first name word
 * @param [out] name1				second name word
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwGetPinType(const struct _AWEInstance* pAWE, UINT32 pinID, UINT32* pClassID, UINT32* pBoundID, float* sampleRate,
	UINT32* info1, UINT32* info2, UINT32* info3, UINT32* name0, UINT32* name1);

/**
 * @brief Bind a wire to an I/O object.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] wireID				ID of the wire to bind
 * @param [in] pinID				pin ID of the pin to bind
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwBindIOToWire(struct _AWEInstance* pAWE, UINT32 wireID, UINT32 pinID);

/**
 * @brief This must be called before doing anything else with AWE.
 * @param [in] pvAWE				AWE instance pointer (this)
 *
 * Sets all the I/O pins to a known state.
 */
INT32 awe_fwInitIoPins(struct _AWEInstance* pAWE);

#ifdef	__cplusplus
}
#endif

#endif	/* _FW_PIN_H */

