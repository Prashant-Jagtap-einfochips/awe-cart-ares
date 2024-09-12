/*******************************************************************************
*
*               Audio Framework
*               ---------------
*
********************************************************************************
*     fw_heap.h
********************************************************************************
*
*     Description:  AudioWeaver Framework heap header file
*
*     Copyright:    (c) 2007-2021 DSP Concepts, Inc. All rights reserved.
*                   3235 Kifer Road
*                   Santa Clara, CA 95054
*
*******************************************************************************/
#ifndef _FW_HEAP_H
#define _FW_HEAP_H

/*------------------ Heap declarations ------------------*/

/* For the PC, all heaps are the same. No point doing anything else really. */
/** Items accessed from the inner loop, such as Wire Buffers. */
#define AWE_HEAP_FAST       (UINT32)1U

/** Infrequently used items which could be placed in external memory. */
#define AWE_HEAP_SLOW       (UINT32)2U

/** Another fast heap which is distinct from AWE_HEAP_FAST. This is used to separate
state and coefficients, as in FIR filters. */
#define AWE_HEAP_FASTB      (UINT32)3U

/** Optional heap placed in shared memory. 
 *  Used in multi-instance setups to abstract shared audio buffering and IPC flagging */
#define AWE_HEAP_SHARED     (UINT32)4U

/** Fast Heap -> FastB Heap -> Slow Heap */
#define AWE_HEAP_FAST2SLOW ((AWE_HEAP_FAST) | ((AWE_HEAP_FASTB) << (4)) | ((AWE_HEAP_SLOW) << (8)))

/** Fast Heap -> FastB Heap */
#define AWE_HEAP_FAST2FASTB ((AWE_HEAP_FAST) | ((AWE_HEAP_FASTB) << (4)))

/** Fast B Heap -> Fast Heap */
#define AWE_HEAP_FASTB2FAST ((AWE_HEAP_FASTB) | ((AWE_HEAP_FAST) << (4)))

/** This performs the allocation in the fast B heap, and if this fails, in the slow heap */
#define AWE_HEAP_FASTB2SLOW ((AWE_HEAP_FASTB) | ((AWE_HEAP_SLOW) << (4)))

/* For the heap size function, we want to define offsets for each heap */
#define FAST_HEAP_A_CUR_SIZE_OFFSET 0
#define FAST_HEAP_B_CUR_SIZE_OFFSET 1
#define SLOW_HEAP_CUR_SIZE_OFFSET   2
#define FAST_HEAP_A_SIZE_OFFSET     3
#define FAST_HEAP_B_SIZE_OFFSET     4
#define SLOW_HEAP_SIZE_OFFSET       5
#define PHEAPS_SIZE_TOTAL_SIZE      6

/* For the shared heap size function, we want to define offsets for each array entry */
#define SHARED_HEAP_CUR_SIZE_OFFSET   0
#define SHARED_HEAP_SIZE_OFFSET       1
#define SHARED_PHEAPS_SIZE_TOTAL_SIZE 2

/* Shared Heap Header Structure */
#define SHARED_HEAP_INDEX						(0)
#define SHARED_HEAP_IPC_AUDIO_STARTED			(SHARED_HEAP_INDEX + 1)
#define SHARED_HEAP_IPC_PACKET_RETURN			(SHARED_HEAP_IPC_AUDIO_STARTED + 1)
#define SHARED_HEAP_IPC_PACKET_FLAG				(SHARED_HEAP_IPC_PACKET_RETURN + 1)
#define SHARED_HEAP_INST0_PIN_SIZE				(SHARED_HEAP_IPC_PACKET_FLAG + 1)
#define SHARED_HEAP_PASS_PACKET_TO_INST			(SHARED_HEAP_INST0_PIN_SIZE + 1)
#define SHARED_HEAP_IPC_BUFFER_CUR_INDEX		(SHARED_HEAP_PASS_PACKET_TO_INST + 1)
#define SHARED_HEAP_END_OF_HEADER				(SHARED_HEAP_IPC_BUFFER_CUR_INDEX + 1)
// Allocate extra numProcessingInstances words for AWE-7495
#define SHARED_HEAP_END_MISC_HEADER(pAWE)		(SHARED_HEAP_END_OF_HEADER + (pAWE->numProcessingInstances * 2) + 1)
#define SHARED_HEAP_IPC_RESET_START_LOC			(SHARED_HEAP_END_OF_HEADER + pAWE->numProcessingInstances)

/* State of multi-instance packet manager */
#define PACKET_STATE_IDLE    (0) //packet manager is not pending on a response from another instance
#define PACKET_STATE_PENDING (1) //packet manager is pending on a response from another instance
#define PACKET_STATE_DESTROY (2) //packet manager is performing a multi-instance destroy
#define PACKET_STATE_START   (3) //packet manager is performing a multi-instance start
#define PACKET_STATE_STOP    (4) //packet manager is performing a multi-instance stop

/* IPC Packet Flag Marked as invalid instance ID to 
 * indicate that AWECore is waiting for a packet */
#define PACKET_MASTER_NOTIFY (MAX_PROCESSING_INSTANCES)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Return the number of heaps. 
 * @return							number of heaps
 */
UINT32 awe_fwGetHeapCount(AWEInstance *pAWE);

/**
 * @brief Return the sizes of the heaps.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [out] heaps				array of 6 words to receive values
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwGetHeapSize(const AWEInstance *pAWE, UINT32 *pHeaps);

/**
 * @brief Return the sizes of the shared heap.
 * @param [in] pAWE                 AWE instance pointer (this)
 * @param [out] heaps               array of 2 words to receive values
 * @return                          0 or error code
 */
/*private*/ INT32 awe_fwGetSharedHeapSize(const AWEInstance *pAWE, UINT32 *pHeaps);

/**
 * @brief Allocate a block of storage from the heap.
 * @param [in] size                 size in bytes of storage to allocate
 * @param [in] heapIndex            which heap(s) to allocate on
 * @param [out] retVal              error code on failure
 * @return                          pointer to allocated storage
 *
 * If zero returned, the reason is returned in retVal. The storage is zeroed.
 */
/*private*/ void *awe_fwMalloc(UINT32 size, UINT32 heapIndex, INT32 *retVal);

/**
 * @brief Allocate a block of storage from the heap. This function is similar to 
 * awe_fwMalloc() with additional argument of AudioWeaver instance pointer
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] size					size in bytes of storage to allocate
 * @param [in] heapIndex			which heap(s) to allocate on
 * @param [out] retVal				error code on failure
 * @return							pointer to allocated storage
 *
 * If zero returned, the reason is returned in retVal. The storage is zeroed.
 */
/*private*/ void *awe_fwMallocInst(AWEInstance *pAWE, UINT32 size, UINT32 heapIndex, INT32 *retVal);

/**
 * @brief Allocate an aligned block of storage from the heap.
 * @param [in] size                 size in bytes of storage to allocate
 * @param [in] align                alignemnt required in bytes
 * @param [in] heapIndex            which heap(s) to allocate on
 * @param [out] retVal              error code on failure
 * @return                          pointer to allocated storage
 *
 * If zero returned, the reason is returned in retVal. The storage is zeroed.
 * Alignments are coerced to a multiple of sizeof(int).
 */
/*private*/ void *awe_fwMallocAligned(UINT32 size, UINT32 align, UINT32 heapIndex, INT32 *retVal);

/**
 * @brief Allocate a block of storage from the heap with both align and AWE Instance Options
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] size					size in bytes of storage to allocate
 * @param [in] heapIndex			which heap(s) to allocate on
 * @param [out] retVal				error code on failure
 * @return							pointer to allocated storage
 *
 * If zero returned, the reason is returned in retVal. The storage is zeroed.
 */
/*private*/ void *awe_fwMallocCommon(AWEInstance *pAWE, UINT32 size, UINT32 align, UINT32 heapIndex, INT32 *retVal);

/**
 * @brief Reset the framework to its initial state.
 * @param [in] pAWE					AWE instance pointer (this)
 */
/*private*/ void awe_fwDestroy(AWEInstance *pAWE);

/**
 * #brief Allocate a block of storage from shared memory.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] size					size in bytes of storage to allocate
 * @param [out] retVal				error code on failure
 * @return							pointer to allocated storage
 *
 * If zero returned, the reason is returned in retVal.
 * Implemented only for those targets that need special handling
 * which should only br non-SMP multicore systems.
 */

#if !defined(NO_TEST_PADDING) || defined(ENABLE_MEMORY_BOUNDS_CHECK)
/**
 * @brief Pad Heaps and increment index with a single word
 * @param [in] pAWE					AWE instance pointer (this)
 */
/*private*/ void awe_heapPad(AWEInstance* pAWE);
#endif

/**
 * @brief Initializes the shared heap used in multi-instance targets
 * @param [in] pAWE                 AWE instance pointer (this)
 * @return                          E_SUCCESS or relevant error message
 */
/*private*/ INT32 awe_fwInitSharedHeap(AWEInstance *pAWE);

#ifdef __cplusplus
}
#endif

#endif // _FW_HEAP_H
