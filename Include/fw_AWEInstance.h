/*******************************************************************************
*
*				Audio Framework
*				---------------
*
********************************************************************************
*	  AWEInstance.h
********************************************************************************
*
*	  Description:	AudioWeaver AWEInstance main header file
*
*	  Copyright:	(c) 2007-2021 DSP Concepts, Inc. All rights reserved.
*					3235 Kifer Road
*					Santa Clara, CA 95054
*
*******************************************************************************/
#ifndef _FRAMEWORK_AWEINSTANCE_H
#define _FRAMEWORK_AWEINSTANCE_H

#ifdef	__cplusplus
extern "C" {
#endif

/** Defines for processor type. */
#define PROCESSOR_TYPE_NATIVE       1U
#define PROCESSOR_TYPE_SHARC        2U  // SHARC 214xx
#define PROCESSOR_TYPE_BLACKFIN     3U
#define PROCESSOR_TYPE_CORTEXM4     4U
#define PROCESSOR_TYPE_OMAP         5U
#define PROCESSOR_TYPE_DSK6713      6U
#define PROCESSOR_TYPE_HIFI2DSP     7U  // Hifi 24 bit
#define PROCESSOR_TYPE_HIFIMINIDSP  7U  // Hifi 24 bit
#define PROCESSOR_TYPE_CORTEXM3     8U
#define PROCESSOR_TYPE_IMX25        9U
#define PROCESSOR_TYPE_CORTEXM7     10U
#define PROCESSOR_TYPE_C674x        11U
#define PROCESSOR_TYPE_CortexA5     12U
#define PROCESSOR_TYPE_CortexA7     13U
#define PROCESSOR_TYPE_CortexA8     14U
#define PROCESSOR_TYPE_CortexA9     15U
#define PROCESSOR_TYPE_CortexA12    16U
#define PROCESSOR_TYPE_CortexA15    17U
#define PROCESSOR_TYPE_CortexA53    18U
#define PROCESSOR_TYPE_CortexA57    19U
#define PROCESSOR_TYPE_ARM9         20U
#define PROCESSOR_TYPE_ARM11        21U
#define PROCESSOR_TYPE_HEXAGON      22U // QUALCOMM HEXAGON
#define PROCESSOR_TYPE_HIFI3DSP     23U // Generic HiFi 3
#define PROCESSOR_TYPE_S1000        24U // Intel S1000 for Sue Creek
#define PROCESSOR_TYPE_HEMILITE     25U // Knowles processor in IA-610
#define PROCESSOR_TYPE_CORTEXA72    26U
#define PROCESSOR_TYPE_CORTEXA35    27U
#define PROCESSOR_TYPE_C66xx        28U
#define PROCESSOR_TYPE_SHARC215XX   29U // SHARC 215xx family
#define PROCESSOR_TYPE_HIFI4DSP     30U // Generic HiFi 4
#define PROCESSOR_TYPE_DELTAMAX     31U // Knowles deltaMax processor
#define PROCESSOR_TYPE_HEMIDELTA    32U // Knowles hemiDelta processor
#define PROCESSOR_TYPE_CEVA         33U // CEVA processor
#define PROCESSOR_TYPE_FORDHIFI4    34U // Ford iMX8 HiFi4 processor
#define PROCESSOR_TYPE_CEVAX2       35U // CEVA X2 processor
#define PROCESSOR_TYPE_UNASSIGNED   36U // Placeholder type
#define PROCESSOR_TYPE_CORTEXM33    37U
#define PROCESSOR_TYPE_SC589        38U // ADI SC589
#define PROCESSOR_TYPE_HIFI5DSP     39U // Generic HiFi 5
#define PROCESSOR_TYPE_CORTEXM55    40U // CORTEXM55

/** This structure defines the target info. */
/*private*/ typedef struct _TargetInfo
{
	/** Target sample rate. */
	float m_sampleRate;					/* 0 */

	/** Target profile clock speed in Hz. */
	float m_profileClockSpeed;			/* 4 */

	/** Base block size of target (usually 32). */
	UINT32 m_base_block_size;			/* 8 */

	/** Packed field. */
	UINT32 m_packedData;				/* 12 */

	/** Target version - high byte is base framework version and must match. */
	UINT32 m_version;					/* 16 */

	/** Packed field for buffer size, s, input pin count, output pin count. */
	UINT32 m_proxy_buffer_size;			/* 20 */

	/** Target name up to 16 characters. */
	UINT32 m_packedName[2];				/* 24 */

	/** Clock speed of this core. */
	float m_coreClockSpeed;				/* 32 */

	/** ID of this core. */
	UINT32 m_coreID;					/* 36 */

	/** Feature bits. */
	UINT32 m_features;					/* 40 */
}
TargetInfo;

/** This structure defines extended target info. */
/* private */ typedef struct _ExtendedInfo
{
	/** The user version field. */
	UINT32 userVersion;

	/** Bit packed informational word 1 */
		/** 
			bit 0: AWECoreOS product identifier (set in AWECoreOS.c only)				
                        bit 1: Is the target implementing the shared heap multi-instance model (1 - Yes, otherwise static)


                        bit 2-10: Hot fix version			
                        bit 11-31: Unused
		*/
	UINT32 infoWord1;
	
	/** Currently undefined fields that will report zeros. */
	UINT32 notSpecified[11];

} ExtendedInfo;	// Total length 13 words

/** Create the target version - for V5 must have first argument of 5. */
#define MAKE_TARGET_VERSION(v1, v2, v3, v4) \
	( (((v1) & 0xffU) << 24) | (((v2) & 0xffU) << 16) | (((v3) & 0xffU) << 8) | ((v4) & 0xffU) )

/** Create a word containing packed characters. */
#define MAKE_PACKED_STRING(c1, c2, c3, c4) \
	( (((c4) & 0xffU) << 24) | (((c3) & 0xffU) << 16) | (((c2) & 0xffU) << 8) | ((c1) & 0xffU) )

/** Set the m_proxy_buffer_size target info member. */
#define PACKED_BUFFER_SIZE(bufferSize, cores, nInputPins, nOutputPins, smpCore) \
	(((bufferSize - 1U) & 0x1fffU) | (((cores - 1U) & 0x3fU) << 13) | (((nInputPins - 1U) & 0x3fU) << 20) | (((nOutputPins - 1U) & 0x3fU) << 26) | (((smpCore) & 1U) << 19))

/** Get the packet buffer size from m_proxy_buffer_size. */
#define GET_TARGET_PACKET_BUFFER_LEN(x) (((x).m_proxy_buffer_size & 0x1fffU) + 1U)

/** The the target API. */
#define GET_TARGET_API(x)				(((x).m_version >> 24) & 0xff)

/** Get the core count from m_proxy_buffer_size. */
#define GET_TARGET_CORES(x)				((((x).m_proxy_buffer_size >> 13) & 0x3fU) + 1U)

#define SMP_CORE_BIT	0x80000U

/** Test if the core is static. */
#define IS_SMP_CORE(x)					(((x).m_proxy_buffer_size & SMP_CORE_BIT) != 0U)

/** Get the number of input pins from m_proxy_buffer_size. */
#define GET_TARGET_NINPUT_PINS(x)		((((x).m_proxy_buffer_size >> 20) & 0x3fU) + 1U)

/** Get the number of output pins from m_proxy_buffer_size. */
#define GET_TARGET_NOUTPUT_PINS(x)		((((x).m_proxy_buffer_size >> 26) & 0x3fU) + 1U)

/** Get the base block size of the target. */
#define GET_TARGET_BASE_BLOCK_SIZE(x)	((x).m_base_block_size & 0xfffU)

/** Get the number of threads on the target. */
#define GET_TARGET_THREADS(x)			(((x).m_base_block_size >> 12) & 0xfU)

/** Set only on special targets */
#define FLOAT_IO_PINS		0x100000U

/** Test if the target uses float I/O pins. */
#define IS_IO_PIN_FLOAT(x)				(((x).m_base_block_size & FLOAT_IO_PINS) != 0U)

/** BSP must use this macro to set the bit in TargetInfo. */
#define SET_IS_PIN_FLOAT(x)				((x).m_base_block_size |= FLOAT_IO_PINS)

/** Get the clock domain of the core - 0 is main domain. */
#define GET_CLOCK_DOMAIN(x)				(((x).m_base_block_size >> 16) & 0xfU)

/** Target was compiled with align 4. */
#define USING_ALIGN4		0x200000U

/** Target has buffer pool support. */
#define HAS_BUFFER_POOL		0x400000U

/** Note using align4. */
#define SET_USING_ALIGN4(x)				((x).m_base_block_size |= USING_ALIGN4)

/** Note buffer pool supported. */
#define SET_HAS_BUFFER_POOL(x)			((x).m_base_block_size |= HAS_BUFFER_POOL)

#define MAKE_BLOCK_SIZE_PACKED(block_size, nThreads) \
	((block_size) & 0xfffU) | (((nThreads) & 0xfU) << 12)

/** Create target info m_packedData. */
#define MAKE_TARGET_INFO_PACKED(sizeof_int, num_inputs, num_outputs, is_floating_point, is_flash_supported, processor_type) \
	( ((sizeof_int) & 0xfU) | (((is_flash_supported) & 1U) << 4) | (((is_floating_point) & 1U) << 5) \
	| (((num_inputs) & 0xffU) << 8) | (((num_outputs) & 0xffU) << 16) | (((processor_type) & 0x7fU) << 24) )

/** Access sizeof(int) from target info m_packedData. */
#define TARGET_INFO_SIZOF_INT(targetinfo) \
	((targetinfo).m_packedData & 0xfU)

/** Access flash supported from target info m_packedData. */
#define TARGET_INFO_IS_FLASH_SUPPORTED(targetinfo) \
	(((targetinfo).m_packedData & 0x10U) != 0)

/** Access is floating point from target info m_packedData. */
#define TARGET_INFO_IS_FLOATING_POINT(targetinfo) \
	(((targetinfo).m_packedData & 0x20U) != 0)

/** Bit if target uses padding. */
#define TARGET_USES_PADDING		0x40U

/** Report if heap uses padding. */
#define TARGET_INFO_USES_PADDING(targetinfo) \
	(((targetinfo).m_packedData & TARGET_USES_PADDING) != 0)

/** Access num inputs from target info m_packedData. */
#define TARGET_INFO_NUM_INPUTS(targetinfo) \
	(((targetinfo).m_packedData >> 8) & 0xffU)

/** Access num outputs from target info m_packedData. */
#define TARGET_INFO_NUM_OUTPUTS(targetinfo) \
	(((targetinfo).m_packedData >> 16) & 0xffU)

/** Access processor type from target info m_packedData. */
#define TARGET_INFO_PROCESSOR_TYPE(targetinfo) \
	(((targetinfo).m_packedData >> 24) & 0x7fU)

/** Internal temporary array of wire pointers size */
#define AWE_WIRES_POINTERS_SCRATCH_SIZE	256

// We will define the AWEInstance structure by first defining its
// private framework members and then using that definition to 
// extend the public api declaration of the structure.

#define AWEINSTANCE_FRAMEWORK                                                                                \
	/** Head of chain of class objects. */                                                                   \
	InstanceDescriptor *m_pObjectsHead;                                                                      \
                                                                                                             \
	/** Tail of chain of class objects. */                                                                   \
	InstanceDescriptor *m_pObjectsTail;                                                                      \
                                                                                                             \
	/** Head of chain of I/O pin instances.                                                                  \
	Must be initialized to point at the last item in the above. */                                           \
	IOPinDescriptor *m_pIOPinHead;                                                                           \
                                                                                                             \
	/** Tick object pointer. */                                                                              \
	InstanceDescriptor *m_pTickObject;                                                                       \
                                                                                                             \
	/** Head of list of layouts. */                                                                          \
	LayoutInstance *m_pLayoutHead;                                                                           \
                                                                                                             \
	/** Tail of list of layouts. */                                                                          \
	LayoutInstance *m_pLayoutTail;                                                                           \
                                                                                                             \
	/** The pointer to the lookup table - initially NULL. */                                                 \
	void **m_lookup_table;                                                                                   \
                                                                                                             \
	/** Index of next free second heap word. */                                                              \
	UINT32 m_fastb_heap_index;                                                                               \
                                                                                                             \
	/** Index of next free master heap word. */                                                              \
	UINT32 m_master_heap_index;                                                                              \
                                                                                                             \
	/** Index of next free slow heap word. */                                                                \
	UINT32 m_slow_heap_index;                                                                                \
                                                                                                             \
	/** Initialized to 0 on program start or restart, incremented each time an                               \
	object of anything is created. */                                                                        \
	UINT32 m_unique_object_ID_counter;                                                                       \
                                                                                                             \
	/** The maximum ID supported by the lookup table. */                                                     \
	UINT32 m_maxID;                                                                                          \
                                                                                                             \
	/** Some general purpose flag bits. */                                                                   \
	UINT32 m_general_flags;                                                                                  \
                                                                                                             \
	/** Head of linked list of bound wires. */                                                               \
	WireInstance *m_pBindHead;                                                                               \
                                                                                                             \
	/** Previous sequence number for AWE packets. */                                                         \
	UINT32 msg_seglast;                                                                                      \
                                                                                                             \
	/** Extended target info. */                                                                             \
	ExtendedInfo extendedInfo;                                                                               \
                                                                                                             \
	/** Size of inputPin table array, default 1. */                                                          \
    UINT32 inputPinCount;                                                                                    \
                                                                                                             \
	/** Size of outputPin table array, default 1. */                                                         \
    UINT32 outputPinCount;                                                                                   \
                                                                                                             \
	/** Type of this instance (ARM9, CortexA, long set of possible values). */                               \
    INT32 processorType;                                                                                     \
                                                                                                             \
	/** TRUE if target is float. */                                                                          \
    BOOL hasFloatSupport;                                                                                    \
                                                                                                             \
	/** TRUE if the instance is SMP. */                                                                      \
    BOOL isSMP;                                                                                              \
                                                                                                             \
	/** Smoothing shift range 1-8. */                                                                        \
	UINT32 m_smooth_shift;                                                                                   \
                                                                                                             \
	/** The pointer to the wire buffer pool. */                                                              \
	void* m_wire_buffer_pool;                                                                                \
                                                                                                             \
	/** Its size. */                                                                                         \
	UINT32 m_buffer_pool_size;                                                                               \
                                                                                                             \
	/** The state of the multi-instance packet manager */                                                    \
	UINT32 multiPacketState;                                                                                 \
                                                                                                             \
    /** Private function pointers. */                                                                        \
                                                                                                             \
    /** Platform init flash file system */                                                                   \
    BOOL (*m_pAwe_InitFlashFileSystem)(AWEFlashFSInstance * pAWEFlashFSInstance);                            \
                                                                                                             \
    /** Platform execute file files */                                                                       \
    INT32 (*m_pAwe_ExecuteFlashFiles)(struct _AWEInstance * pAWE, AWEFlashFSInstance * pAWEFlashFSInstance); \
                                                                                                             \
    /** Platform flash file system command handler */                                                        \
    INT32 (*m_pAwe_FlashFileSystemCommandHandler)(struct _AWEInstance * pAWE);				                 \
																							                 \
	/** Used in cycle count correction due to preemption */									                 \
	/** Array of pointers to number of AWE Instances in the system */						                 \
	struct _AWEInstance **pAWEInstances;													                 \
	INT32 numAWEInstancesOnCore;															                 \
																							                 \
	/** Void pointer to hold mutex or critical instance for internal profiling specific lock/unlock */		 \
	void *pCriticalSec;																						 \
																							                 \
	/** Max wires in a module */															                 \
	WireInstance *Wires[AWE_WIRES_POINTERS_SCRATCH_SIZE];													 \
																											 \
	INT32 resetState;

#include "AWEInstance.h"

/** Set the started flag. */
#define SET_STARTED(pAWE)   ((pAWE)->m_general_flags |= AWE_AUDIO_STARTED)

/** Clear the started flag. */
#define CLR_STARTED(pAWE)   ((pAWE)->m_general_flags &= ~AWE_AUDIO_STARTED)

/** Set the started multi-instance flag. */
#define SET_MULTI_INSTANCE_STARTED(pAWE)   ((pAWE)->pSharedHeap[SHARED_HEAP_IPC_AUDIO_STARTED] |= AWE_AUDIO_STARTED)

/** Clear the started multi-instance flag. */
#define CLR_MULTI_INSTANCE_STARTED(pAWE)   ((pAWE)->pSharedHeap[SHARED_HEAP_IPC_AUDIO_STARTED] &= ~AWE_AUDIO_STARTED)

/** Is multi-instance audio started. */
#define IS_MULTI_INSTANCE_STARTED(pAWE)    ((pAWE)->pSharedHeap != NULL ? (pAWE->pSharedHeap[SHARED_HEAP_IPC_AUDIO_STARTED] > 0 ? 1:0) : 0)

/** Is audio started. */
#define IS_STARTED(pAWE)    (IS_MULTI_INSTANCE_STARTED(pAWE) ? 1 : (((pAWE)->m_general_flags & AWE_AUDIO_STARTED) > 0 ? 1:0))

/** Is deferred bit set. */
#define IS_DEFERRED_SET(pAWE)   ((pAWE)->m_general_flags & AWE_DEFERRED_SET)

/** Clear the deferred bit. */
#define CLR_DEFERRED_SET(pAWE)  (pAWE)->m_general_flags &= ~AWE_DEFERRED_SET

/**
 * @brief Initialize the instance.
 * @param pvAWE						instance to initialize
 * @param initInfo					how to initialize
 */
INT32 awe_fwInit(void *pvAWE);

/**
 * @brief Initialize the hot fix version, if any.
 * @param pvAWE						instance to initialize
 */
void awe_fwInitHotFixVer(AWEInstance* pAWE);
/**
 * @brief Copy the target info to the caller.
 * @param [out] pTarget				buffer to receive target info
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwGetTargetInfo(const AWEInstance *pAWE, TargetInfo *pTarget);

/**
 * @brief Initialize the instance's target info.
 * @brief [out] pTargetInfo     -  target info struct to pack data into
 * @param [in] coreID           -  Core ID: must be 0 (increments sequentially for multi core systems)
 * @param [in] coreClockSpeed   -  Processor core clock speed in Hz
 * @param [in] sampleSpeed      -  Profiling clock - can be same as core clock or smaller than core clock. Most embedded targets will have the same value as core clock
 * @param [in] name             -  String for name of the target up to 16 characters maximum (Just for the display)
 * @param [in] procType         -  Type of the processor (just for display).
 * @param [in] isFloat          -  Native type of the processor - floating or fixed (1 - for floating point, 0 - for fixed point)
 * @param [in] isFlash          -  AWE flash file system supported or not (1 - for supported and 0 - for not supported)
 * @param [in] nInputs          -  Number of input pins
 * @param [in] nOutputs         -  Number of output pins
 * @param [in] isSMP            -  Is the processor of type static core or SMP (1 - SMP, otherwise static)
 * @param [in] threads          -  Number of threads supported. Default every AWE target will have 2 threads to support two different block processing
 * @param [in] sampleRate       -  Fundamental sample rate of the system (default to 48KHz)
 * @param [in] nInputChannels   -  Number of input channels
 * @param [in] nOutputChannels  -  Number of output channels
 * @param [in] nVerMajor        -  Major version letter used in version string - 8.Major.Minor.Proc (8 is the AWE version)
 * @param [in] nVerMinor        -  Minor version number used in version string - 8.Major.Minor.Proc (8 is the AWE version)
 * @param [in] nVerProc         -  Target repo version number used in version string - 8.Major.Minor.Proc (8 is the AWE version). For example, awecore-cortexm is an example repo for Cortex-M target.
 * @param [in] blockSize        -  Audio Weaver fundamental block - 32 in provided BSP (32 is the default used in many targets)
 * @param [in] commandBufferLen -  Communication buffer length in 32-bit words
 * @param [in] nCores           -  Number of cores
 */
/*private*/ void awe_fwPackTargetInfo(TargetInfo *pTargetInfo, UINT32 coreID, float coreClockSpeed, float sampleSpeed,
	const char *name, INT32 procType, INT32 isFloat, INT32 isFlash,
	INT32 nInputs, INT32 nOutputs, INT32 isSMP, INT32 threads, float sampleRate,
	INT32 nInputChannels, INT32 nOutputChannels, INT32 nVerMajor, INT32 nVerMinor, INT32 nVerProc, UINT32 blockSize, UINT32 commandBufferLen);

/**
 * @brief Initialize the packed name (not SHARC).
 * @param [in] pPackedName          pointer to packed name array
 * @param [in] name                 name as a string
 */
/*private*/ void awe_fwSetPackedName(UINT32 *pPackedName, const char *name);

/**
 * @brief Returns the number of modules in the table.
 * @param [in] pAWE					AWE instance pointer (this)
 * @return							number of classes in module class table
 */
/*private*/ UINT32 awe_fwGetAweInstanceModuleCount(const AWEInstance *pAWE);

/**
 * @brief Get the information for the specified module.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] index				index of item to get
 * @param [out] pDescr				pointer to receive class pointer
 * @param [out] classID				pointer to receive class ID
 * @param [out] packedParameters	pointer to receive packed parameters
 * @return							class id or error code
 */
/*private*/ INT32 awe_fwGetAweInstanceModuleInfo(const AWEInstance *pAWE, UINT32 index, UINT32 *classID, UINT32 *packedParameters);

/**
 * @brief Lookup a module class by ID.
 * @param [in] classID				class to look for
 * @return							class or NULL on error
 */
/*private*/ const ModClassModule *awe_fwGetAweInstanceModuleClass(const AWEInstance *pAWE, UINT32 classID);

/*******************************************************************************
* These below functions exist in fw_AWEInstance.h now, but eventually will be 
* moved to their respective component header files when refactoring is complete
* This is determined by the following weblink:
* https://docs.google.com/spreadsheets/d/1EaHz1yKtj1M5v_LPK3xrCs_avJcygr7I3Pbg24vnaHE/edit#gid=0
*******************************************************************************/

/******************************* MODULE FUNCTIONS *****************************/
/**
 * @brief Pack version number of module
 * @param [out] retVal		0 or error code
 */
/*private*/ void awe_fwSetPackedVersion(INT32 * retVal);


/**
 * @brief Pump the specified module.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] pModule				module to pump
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwPumpModule(AWEInstance *pAWE, ModuleInstanceDescriptor *pModule);


/**
 * @brief Initialize packet communications.
 * @param[out] packet_buf			return pointer to packet buffer
 * @param [out] reply_buf			return pointer to reply buffer
 * @param[out] buf_length			return packet buffer length in words
 */
void awe_fwGetPacketInfo(const void *pvAWE, UINT32 **packet_buf, UINT32 **reply_buf, UINT32 *buf_length);

/**
 * @brief Handle a packet for any core
 * @return							error code
 */
INT32 awe_packetProcessUnified(AWEInstance * pAWE);


#ifdef __cplusplus
}
#endif

#endif	/* _FRAMEWORK_AWEINSTANCE_H */

