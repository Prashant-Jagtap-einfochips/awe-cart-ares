/*******************************************************************************
*
*				Audio Framework
*				---------------
*
********************************************************************************
*	  Framework.h
********************************************************************************
*
*	  Description:	AudioWeaver Framework layout header file
*
*	  Copyright:	(c) 2007-2023 DSP Concepts, Inc. All rights reserved.
*					3235 Kifer Road
*					Santa Clara, CA 95054
*
*******************************************************************************/
#ifndef _FW_LAYOUT_H
#define _FW_LAYOUT_H

#include "fw_Common.h"

#ifdef	__cplusplus
extern "C" {
#endif

/** Bit to record if the tick (idle) function was called. */
#define AWE_TICKED          0x01U

/** Bit to suppress profiling. */
#define AWE_NO_PROFILE      0x10U

/** Bit to suppress outer profiling. */
#define AWE_NO_PROFILE2     0x20U

/** Audio started flag. */
#define AWE_AUDIO_STARTED   0x40U

/** A deferred bit was set. */
#define AWE_DEFERRED_SET    0x80U

/** Layout Loaded flag. */
#define AWE_LAYOUT_VALID    0x100U

//** Pump overflow flag. sticky bit, set when overflow is detected. Cleared on read by awe_fwGetProfileValues()    */
#define AWE_PUMP_OVERFLOW        0x0002U

//** Pump Active Detect Enable flag. Enable / disable Overflow detection at AWECore, if Disabled, BSP / AWECore OS is responsible to detect overflow */
#define AWE_PUMP_ACTIVE_DETECT_ENABLE          0x0004U
//Bit position in the word
#define AWE_PUMP_ACTIVE_DETECT_ENABLE_B        0x2U

/**  AWE_PUMP_ACTIVE_DETECT_ENABLE_MASK mask */
#define AWE_PUMP_ACTIVE_DETECT_ENABLE_CLR_MASK     0x0FFFFFFFBU

/** Flag to indicate auto reset status (in progress or not) */
#define AWE_SYS_AUTO_RESET_STATUS	0x200U

/** Flag to enable/disable auto reset feature up on overflow occurred */
#define AWE_SYS_AUTO_RESET_ENABLE	0x400U

//** Pump active flag, set on entering a pump and cleared on exit */
//** One bit reserved for each layout, Supporting 16 clock divided layouts */
#define AWE_PUMP_ACTIVE_LAYOUT_1   0x1000U
#define AWE_PUMP_ACTIVE_LAYOUT_2   0x2000U
#define AWE_PUMP_ACTIVE_LAYOUT_3   0x4000U
#define AWE_PUMP_ACTIVE_LAYOUT_4   0x8000U
#define AWE_PUMP_ACTIVE_LAYOUT_5   0x10000U
#define AWE_PUMP_ACTIVE_LAYOUT_6   0x20000U
#define AWE_PUMP_ACTIVE_LAYOUT_7   0x40000U
#define AWE_PUMP_ACTIVE_LAYOUT_8   0x80000U
#define AWE_PUMP_ACTIVE_LAYOUT_9   0x100000U
#define AWE_PUMP_ACTIVE_LAYOUT_10  0x200000U
#define AWE_PUMP_ACTIVE_LAYOUT_11  0x400000U
#define AWE_PUMP_ACTIVE_LAYOUT_12  0x800000U
#define AWE_PUMP_ACTIVE_LAYOUT_13  0x1000000U
#define AWE_PUMP_ACTIVE_LAYOUT_14  0x2000000U
#define AWE_PUMP_ACTIVE_LAYOUT_15  0x4000000U
#define AWE_PUMP_ACTIVE_LAYOUT_16  0x8000000U

/** These are the macros for individual bit fields in m_layout_flags variable in LayoutInstance */
#define LAYOUT_PUMPING			0x00000001U
#define LAYOUT_OVERFLOW			0x00000002U
#define LAYOUT_ACTIVE			0x00000004U
#define LAYOUT_FIRST_FRAME		0x00000008U
#define LAYOUT_RESET_ACTIVE		0x00000010U

// Bit position in the word
#define AWE_PUMP_ACTIVE_LAYOUT_B  12

/** Set of pump active bits. */
#define AWE_PUMP_ACTIVE_MASK     0x0FFFF000U

/** Set of pump active bits. */
#define AWE_PUMP_OVF_MAX_AVG_CYCLES 0xFFFFFFFFU


/** Reset logic macros */
#define LAYOUT_OVERFLOW_DETECTED	0x1U
#define LAYOUT_INITIATE_RESET		0x2U
#define LAYOUT_RESET_COMPLETED		0x3U

#define LAYOUT_RESET_CHECK				0x0
#define LAYOUT_RESET_DETECTED			0x1
#define	LAYOUT_RESET_WAIT_FOR_ALL_DONE	0x2
#define LAYOUT_RESET_DONE				0x3
#define LAYOUT_RESET_RELEASE			0x4

#define LAYOUT_ACTIVE_ON_INST_MASK		0x80000000

/** Set the layout loaded flag. */
#define SET_LAYOUT_VALID(pAWE)  ((pAWE)->m_general_flags |= AWE_LAYOUT_VALID)

/** Clear the started flag. */
#define CLR_LAYOUT_VALID(pAWE)  ((pAWE)->m_general_flags &= ~AWE_LAYOUT_VALID)

/** IS pump Overflow detected  */
#define IS_PUMP_OVERFLOW(pLayout)	((pLayout)->m_layout_flags & LAYOUT_OVERFLOW)

/** Set pump overflow  */
#define SET_PUMP_OVERFLOW(pLayout)	((pLayout)->m_layout_flags |= LAYOUT_OVERFLOW)

/** Clear pump overflow  */
#define CLR_PUMP_OVERFLOW(pLayout)	((pLayout)->m_layout_flags &= ~LAYOUT_OVERFLOW)

/** Set the pump active for the layout */
#define SET_PUMP_ACTIVE(pLayout)	((pLayout)->m_layout_flags |= LAYOUT_PUMPING)

/** clear the pump active for the layout */
#define CLR_PUMP_ACTIVE(pLayout)		((pLayout)->m_layout_flags &= ~LAYOUT_PUMPING)

/** Is PUMP active for the layout */
#define IS_LAYOUT_PUMP_ACTIVE(pLayout)	((pLayout)->m_layout_flags & LAYOUT_PUMPING)

/** Set the layout active bit (used in preemption overhead correction) */
#define SET_LAYOUT_ACTIVE(pLayout)	((pLayout)->m_layout_flags |= LAYOUT_ACTIVE)

/** Clear the layout active bit (used in preemption overhead correction) */
#define CLR_LAYOUT_ACTIVE(pLayout)		((pLayout)->m_layout_flags &= ~LAYOUT_ACTIVE)

/** Is Layout active? (used in preemption overhead correction) */
#define IS_LAYOUT_ACTIVE(pLayout)	((pLayout)->m_layout_flags & LAYOUT_ACTIVE)

/** Set the layout first frame flag (used to begin average count with instantaneous on start up for faster converge) */
#define SET_LAYOUT_PUMP_FIRST_FRAME(pLayout)	(pLayout->m_layout_flags |= LAYOUT_FIRST_FRAME)

/** Clear the layout first frame flag (used to begin average count with instantaneous on start up for faster converge) */
#define CLR_LAYOUT_PUMP_FIRST_FRAME(pLayout)	(pLayout->m_layout_flags &= ~LAYOUT_FIRST_FRAME)

/** Is the layout first frame flag true? (used to begin average count with instantaneous on start up for faster converge) */
#define IS_LAYOUT_PUMP_FIRST_FRAME(pLayout)	(pLayout->m_layout_flags & LAYOUT_FIRST_FRAME)

/** Set the layout reset active flag (used to reset IPC modules to maintain synchronization up on underrun/overrun on different instances) */
#define SET_LAYOUT_RESET_ACTIVE(pLayout)	(pLayout->m_layout_flags |= LAYOUT_RESET_ACTIVE)

/** Clear the layout reset active flag (used to reset IPC modules to maintain synchronization up on underrun/overrun on different instances) */
#define CLR_LAYOUT_RESET_ACTIVE(pLayout)	(pLayout->m_layout_flags &= ~LAYOUT_RESET_ACTIVE)

/** Is layout reset active flag true? (used to reset IPC modules to maintain synchronization up on underrun/overrun on different instances) */
#define IS_LAYOUT_RESET_ACTIVE(pLayout)		(pLayout->m_layout_flags & LAYOUT_RESET_ACTIVE)

/** Enable AWE_PUMP_ACTIVE_DETECT_ENABLE */
#define SET_AWE_PUMP_ACTIVE_DETECT_ENABLE(pAWE) ((pAWE)->m_general_flags |= AWE_PUMP_ACTIVE_DETECT_ENABLE)

/** Clear AWE_PUMP_ACTIVE_DETECT_ENABLE  */
#define CLR_AWE_PUMP_ACTIVE_DETECT_ENABLE(pAWE) ((pAWE)->m_general_flags &= ~AWE_PUMP_ACTIVE_DETECT_ENABLE)

/** Get the AWE_PUMP_ACTIVE_DETECT_ENABLE bit */
#define GET_AWE_PUMP_ACTIVE_DETECT_ENABLE_BIT(pAWE) (((pAWE)->m_general_flags & AWE_PUMP_ACTIVE_DETECT_ENABLE) >> AWE_PUMP_ACTIVE_DETECT_ENABLE_B )

/** Is audio started. */
#define IS_LAYOUT_VALID(pAWE)   (((pAWE)->m_general_flags & AWE_LAYOUT_VALID) > 0 ? 1:0)

/** Set system reset in progress flag */
#define SET_AWE_SYS_RESET_IN_PROGRESS(pAWE) ((pAWE)->m_general_flags |= AWE_SYS_AUTO_RESET_STATUS)

/** Clear system reset in progress flag */
#define CLR_AWE_SYS_RESET_IN_PROGRESS(pAWE) ((pAWE)->m_general_flags &= ~AWE_SYS_AUTO_RESET_STATUS)

/** Check if system reset in progress flag true? */
#define IS_AWE_SYS_RESET_IN_PROGRESS(pAWE) ((pAWE)->m_general_flags & AWE_SYS_AUTO_RESET_STATUS)

/** Set auto reset feature enabled */
#define SET_AWE_SYS_AUTO_RESET_ENABLE(pAWE) ((pAWE)->m_general_flags |= AWE_SYS_AUTO_RESET_ENABLE)

/** Clear auto reset feature enabled */
#define CLR_AWE_SYS_AUTO_RESET_ENABLE(pAWE) ((pAWE)->m_general_flags &= ~AWE_SYS_AUTO_RESET_ENABLE)

/** Is auto reset feature enabled? */
#define IS_AWE_SYS_AUTO_RESET_ENABLE(pAWE) ((pAWE)->m_general_flags & AWE_SYS_AUTO_RESET_ENABLE)

/** A layout instance. The instance is variable length on the heap, the size
being determined by the constructor arguments. */
/*private*/ typedef struct _LayoutInstance
{
	/** The basic instance data. */
	InstanceDescriptor instanceDescriptor;

	struct _LayoutInstance *pNextLayout;

	/** When 1, called on every iteration, otherwise called less frequently. */
	UINT32 nDivider;

	/** When zero, will execute, and be set to nDivider-1, otherwise will be decremeneted. */
	UINT32 nCurrentDivide;

	/** Average cycles per process. Times 256. */
	UINT32 averageCycles;

	/** Peak cycles per process. Times 256. */
	UINT32 peakCycles;

	/** Number of processes. */
	UINT32 processCount;

	/** Instantaneous cycles for the entire layout that executed.  Times 256. */
	UINT32 instCycles;

	/** Averaged time in cycles between calls to the processing function.  Times 256. */
	UINT32 timePerProcess;

	/** Percentage CPU time */
	float percentCPU;	// strictly float, not FLOAT32

	/** Number of modules in the layout. */
	UINT32 nModules;

	/** The starting time for the last call to Layout_Process. */
	UINT32 m_last_start_time;

	/* Private. */
	void *m_private;

	/* Private. */
	UINT32 m_private2;

	UINT32 m_layout_flags;

	/** Variable which manages overhead of preemption at module level */
	UINT32 overhead_mod;

	/** Variable which manages overhead of preemption at top level */
	UINT32 overhead_top;

	/** cycle correction can only occur if core affinity is set **/
	INT32 coreAffinity;

	/** Array of module pointers. Allocated based on the number of modules in the layout */
	ModuleInstanceDescriptor **pModuleInstances;
}
LayoutInstance;

/**
 * @brief Layout instance constructor.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [out] retVal				error code
 * @param [in] nModules				number of modules to allocate space for
 * @param [in] nDivider				layout divider to use
 * @return							constructed object or NULL on error
 */
/*private*/ InstanceDescriptor *awe_fwLayoutConstructor(struct _AWEInstance *pAWE, INT32 *retVal, INT32 nModules, INT32 nDivider);

/**
 * @brief Add one or more modules to a layout.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] argCount				argument count
 * @param [in] args					the arguments
 * @return							E_SUCCESS or error code
 *
 * The arguments are:
 *
 *	args[0] - ID of the layout
 *	args[1] - offset in module table
 *	args[2+] - array of module IDs
 *
 */
/*private*/ INT32 awe_fwAddModuleToLayout(const struct _AWEInstance *pAWE, UINT32 argCount, const Sample *args);

/**
 * @brief Generic layout pump function.
 * @param [in] pAWE						AWE instance pointer (this)
 * @param [in] pLayout					layout instance to pump
 * @param [in] enableCycleCorrection	Flag to enable/disable cycle overhead correction
 * @param [out] pInstCycles				pointer to receive instantaneous cycles
 * @param [out] pTimePerProcess			pointer to receive time per process
 * @return								E_SUCCESS or error code
 */
/*private*/ void awe_fwLayoutProcess(struct _AWEInstance *pAWE, LayoutInstance *pLayout, const INT32 enableCycleCorrection, UINT32 *pInstCycles, UINT32 *pTimePerProcess);

/**
 * @brief Test if AWE is ready to run. This will return the "layout mask" which is a bit vector of threads to run.
   For a simple, single threaded system, the bitvector will be 0x1.
   If an OVER_FLOW (previous pump is not complete at this point in time) is detected for any of the layouts, then the " Layout mask"
   bit for the said layout will be reset, Indication the said layout is cant be scheduled to pump
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [out] dividers			array of thread dividers
 * @param [out] dividersSize		thread dividers array size
 * @return							bitvector of ready layouts
 */
/*private*/ UINT32 awe_fwEvalLayoutMask(struct _AWEInstance *pAWE, UINT32 *dividers, UINT32 dividersSize);

/**
 * Master framework tick function. This loops through all the modules in the layout and check if they require
 * deferred processing. If a module requires deferred processing, then its Set function is called with a mask
 * of 0xFFFFFF00 and this function returns 1.
 * @param [in] pAWE					AWE instance pointer (this)
 * @return							E_SUCCESS or error code
 */
INT32 awe_fwTick(void* pAWE);

/**
 * @brief Get the next object available number.
 * @param [in] pAWE					AWE instance pointer (this)
 * @return							next object ID
 */
/*private*/ UINT32 awe_fwGetNextAvailableObjectID(struct _AWEInstance *pAWE);

/**
 * @brief Get the first object in the object chain.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [out] pObject				pointer to receive first object pointer
 * @param [out] pClassID			pointer to receive first object class
 * @return							E_SUCCESS or error code
 */
/*private*/ INT32 awe_fwGetFirstLayoutObject(const struct _AWEInstance *pAWE, InstanceDescriptor **pObject, UINT32 *pClassID);

/**
 * @brief Get an object based on its unique ID.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] ID					ID of object to find
 * @param [out] pObject				pointer to receive found object pointer
 * @param [out] pClassID			pointer to receive found object class
 * @return							E_SUCCESS or error code
 */
/*private*/ INT32 awe_fwGetObjectByID(const struct _AWEInstance *pAWE, UINT32 ID, InstanceDescriptor **pObject, UINT32 *pClassID);

/**
 * @brief Create a table for O(1) object lookup.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] maxID				max ID to size table for
 * @return							E_SUCCESS or error code
 */
/*private*/ INT32 awe_fwcreateLookupTable(struct _AWEInstance *pAWE, UINT32 maxID);

/**
 * @brief Set an object pointer in the lookup table.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] ID					ID to set
 * @param [in] pObject				object pointer to set
 * @return							E_SUCCESS or error code
 */
/*private*/ void awe_fwSetIDPointer(const struct _AWEInstance *pAWE, UINT32 id, InstanceDescriptor *pObject);

/**
 * @brief Lookup an object by ID. This is effective only when lookup table is created.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] id					ID to lookup
 * @return							object pointer or NULL if not found
 */
/*private*/ void *awe_fwLookupID(const struct _AWEInstance *pAWE, UINT32 id);

/**
 * @brief Get the first object in the object chain.
 * @param [out] pObject				pointer to receive first object pointer
 * @param [out] pClassID			pointer to receive first object class
 * @return							E_SUCCESS or error code
 */
/*private*/ INT32 awe_fwGetFirstObject(InstanceDescriptor **pObject, UINT32 *pClassID);

/**
 * @brief Get the next object in the object chain.
 * @param [in] currentObject		current object pointer
 * @param [out] pObject				pointer to receive next object pointer
 * @param [out] pClassID			pointer to receive next object class
 * @return							E_SUCCESS or error code
 */
/*private*/ INT32 awe_fwGetNextObject(const InstanceDescriptor *currentObject, InstanceDescriptor **pObject, UINT32 *pClassID);

/**
 * @brief Get an object based on its unique ID.
 * @param [in] ID					ID of object to find
 * @param [out] pObject				pointer to receive found object pointer
 * @param [out] pClassID			pointer to receive found object class
 * @return							E_SUCCESS or error code
 */
/*private*/ INT32 awe_fwGetObjectByID2(UINT32 ID, InstanceDescriptor **pObject, UINT32 *pClassID);

/**
 * @brief Return the type of the class this object was created from.
 * @param [in] pObject				class to look in
 * @return							class ID
 */
/*private*/ UINT32 awe_fwGetClassType(const InstanceDescriptor *pObject);

/**
 * @brief Change the ID of a module
 * @param [in] moduleID				ID of module to change
 * @param [in] newModuleID			ID to assign
 * @return							E_SUCCESS or error code
 */
/*private*/ INT32 awe_fwSetObjectID(const struct _AWEInstance *pAWE, UINT32 moduleID, UINT32 newModuleID);

/**
 * @brief Return the profiling information for a specified layout.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] layoutNumber			the layout index to query. If it is -1, query the profiling of all layouts combined
 * @param [out] pAverageCycles		pointer to receive average cycles * 128
 * @param [out] pTimePerProcess		pointer to receive total cycles * 128
 * @return							0 or error code
*/
/*private*/ INT32 awe_fwGetProfileValues(struct _AWEInstance *pAWE, INT32 layoutNumber, UINT32 *pAverageCycles, UINT32 *pTimePerProcess);

/**
 * @brief Clear the profiling information for a specified layout.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] layoutNumber			the layout index to clear. -1 to clear all layouts profiling information
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwClearProfileValues(const struct _AWEInstance *pAWE, INT32 layoutNumber);

/**
 * @brief Internal function which initiates reset logic if overflow is detected in any layout in any instance
 * @param [in] pAWE					AWE instance pointer (this)
 * @return                          Returns True if detected overflow and until reset state is done. Otherwise False.
 */
/*private*/ INT32 awe_fwChecktoTriggerReset(struct _AWEInstance* pAWE);

/**
 * @brief Internal function to se overflow detected flag in the shared memory of the corresponding instance
 * @param [in] pAWE					AWE instance pointer (this)
 */
/*private*/ void awe_fwSetOverflowOccurredMulti(struct _AWEInstance* pAWE);

#ifdef	__cplusplus
}
#endif

#endif	/* _FW_LAYOUT_H */
