/*******************************************************************************
*
*				Audio Framework
*				---------------
*
********************************************************************************
*	  fw_Wire.h
********************************************************************************
*
*	  Description:	AudioWeaver Framework wire header file
*
*	  Copyright:	(c) 2007-2022 DSP Concepts, Inc. All rights reserved.
*					3235 Kifer Road
*					Santa Clara, CA 95054
*
*******************************************************************************/
#ifndef _FW_WIRE_H
#define _FW_WIRE_H

#include "Errors.h"
#include "fw_Common.h"
#include "fw_Class.h"
#include "fw_Pin.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct _WireInstance
{
	/** The basic instance data. */
	InstanceDescriptor instanceDescriptor;

	/** The wire buffer. */
	Sample *buffer;

	/** Wire sample rate. */
	float sampleRate;

    /** Wire content info 1 */
	UINT32 wireInfo1;

    /** Wire content info 2 */
	UINT32 wireInfo2;

    /** Wire content info 3 */
	UINT32 wireInfo3;

	/** What pin is bound to this wire. default NULL. */
	IOPinDescriptor *m_pBoundPin;

#if !defined(NO_TEST_PADDING) || defined(ENABLE_MEMORY_BOUNDS_CHECK)
#if defined(BUILD64) || defined(AWE_STORAGE_ALIGN4)
	UINT32 padding2;
#endif
	UINT32 padding;
#endif
}
WireInstance;


/**
 * @brief Wire instance constructor.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [out] retVal				to receive the error code
 * @param [in] srate				sample rate
 * @param [in] info1				packed wire info1
 * @param [in] info2				packed wire info2
 * @param [in] info3				packed wire info3
 * @return							constructed object or NULL on error
 */
/*private*/ InstanceDescriptor *ClassWire_Constructor(struct _AWEInstance *pAWE, INT32 *retVal, INT32 srate, UINT32 info1, UINT32 info2, UINT32 info3);

/**
 * @brief Generic wire instance constructor used by all wire constructors.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [out] retVal				to receive the error code
 * @param [in] sampleRate			sample rate of wire
 * @param [in] info1				packed wire info1
 * @param [in] info2				packed wire info2
 * @param [in] info3				packed wire info3
 * @param [in] pClass				point to wire class
 * @return							constructed object or NULL on error
 */
/*private*/ InstanceDescriptor *GenericWire_Constructor(struct _AWEInstance *pAWE, INT32 *retVal, float sampleRate, UINT32 info1, UINT32 info2, UINT32 info3, const ClassDescriptor *pClass);

/**
 * @brief Get the type of a wire.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] wireID				ID of wire to get
 * @param [out] pClassID			class of wire
 * @param [out] sampleRate			sample rate of wire
 * @param [out] info1				first info word
 * @param [out] info2				second info word
 * @param [out] info3				third info word
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwGetWireType(const struct _AWEInstance *pAWE, UINT32 wireID, UINT32 *pClassID, float *sampleRate, UINT32 *info1, UINT32 *info2, UINT32 *info3);


/**
 * @brief Wire buffer pool constructor.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] size				    size of wire buffer pool
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwCreateWireBufferPool(struct _AWEInstance* pAWE, UINT32 size);

/**
 * @brief Wire instance constructor at particular location of the wire buffer pool.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [out] retVal				to receive the error code
 * @param [in] srate				sample rate
 * @param [in] info1				packed wire info1
 * @param [in] info2				packed wire info2
 * @param [in] info3				packed wire info3
 * @param [in] location				wire buffer location inside of the wire buffer pool
 * @return							constructed object or NULL on error
 */
/*private*/ InstanceDescriptor* awe_fwCreateWireInBufferPool(struct _AWEInstance* pAWE, INT32* retVal, INT32 srate, UINT32 info1, UINT32 info2, UINT32 info3, UINT32 location);

/** wireInfo1 */
#define NUM_CHANNELS_MASK       (0x3FFU)    // 10 bits, range: 0-1023
#define NUM_CHANNELS_OFFSET     (0U)

#define MAX_BLOCKSIZE_MASK      (0x1FFFFU)  // 17 bits, range: 0-131071
#define MAX_BLOCKSIZE_OFFSET    (10U)

#define IS_COMPLEX_MASK         (0x1U)      // 1 bit, range: 0-1
#define IS_COMPLEX_OFFSET       (27U)

#define SAMPLE_SIZE_MASK        (0xFU)      // 4 bits, range: 0-15
#define SAMPLE_SIZE_OFFSET      (28U)

/** wireInfo2 */
#define BLOCK_SIZE_MASK         (0x1FFFFU)  // 17 bits, range: 0-131071
#define BLOCK_SIZE_OFFSET       (0U)

#define DATA_TYPE_MASK          (0x3FU)     // 6 bits, range: 0-63
#define DATA_TYPE_OFFSET        (17U)

/** wireInfo3 */
#define NUM_ROWS_MASK           (0x3FFU)    // 10 bits, range: 0-1023
#define NUM_ROWS_OFFSET         (0U)

#define NUM_COLUMNS_MASK        (0x3FFU)    // 10 bits, range: 0-1023
#define NUM_COLUMNS_OFFSET      (10U)

#define IS_IPC_MASK             (0x1U)      // 1 bit, range: 0-1
#define IS_IPC_OFFSET           (20U)

#define IS_PRIVATE_MASK         (0x1U)      // 1 bit, range: 0-1
#define IS_PRIVATE_OFFSET       (21U)

#define IS_CLOCK_MASTER_MASK    (0x1U)      // 1 bit, range: 0-1
#define IS_CLOCK_MASTER_OFFSET  (22U)

/** Helper function to set a range of bits in a wire info */
static VEC_INLINE UINT32 WireInfo_SetBits(UINT32 wireInfo, UINT32 val, const UINT32 mask, const UINT32 offset)
{
    return (wireInfo & ~(mask << offset)) | ((val & mask) << offset);
}

/** Create info1 word. */
#define INFO1_PROPS(channels, maxBlockSize, complex, nSizeBytes) \
	((channels & NUM_CHANNELS_MASK) | ((maxBlockSize & MAX_BLOCKSIZE_MASK) << MAX_BLOCKSIZE_OFFSET) | \
    ((complex & IS_COMPLEX_MASK) << IS_COMPLEX_OFFSET) | ((nSizeBytes & SAMPLE_SIZE_MASK) << SAMPLE_SIZE_OFFSET))

/** Get the channels from info1. */
#define WireInfo1_GetChannels(x)		((x) & NUM_CHANNELS_MASK)

/** Get the channel count of a wire. */
#define ClassWire_GetChannelCount(W)	(WireInfo1_GetChannels((W)->wireInfo1))

/** Set the number of channels of a wire. */
static VEC_INLINE INT32 ClassWire_SetChannelCount(WireInstance * wire, const UINT32 numChannels)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo1 = WireInfo_SetBits(wire->wireInfo1, numChannels, NUM_CHANNELS_MASK, NUM_CHANNELS_OFFSET);
    return E_SUCCESS;
}

/** Get the max block size from info1. */
#define WireInfo1_GetMaxBlockSize(x)	(((x) >> MAX_BLOCKSIZE_OFFSET) & MAX_BLOCKSIZE_MASK)

/** Get max block size*/
#define ClassWire_GetMaxBlockSize(W)	(WireInfo1_GetMaxBlockSize((W)->wireInfo1))

/** Set the maximum block size of a wire. */
static VEC_INLINE INT32 ClassWire_SetMaxBlockSize(WireInstance * wire, const UINT32 maxBlockSize)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo1 = WireInfo_SetBits(wire->wireInfo1, maxBlockSize, MAX_BLOCKSIZE_MASK, MAX_BLOCKSIZE_OFFSET);
    return E_SUCCESS;
}

/** Get is complex from info1. */
#define WireInfo1_IsComplex(x)			(((x) >> IS_COMPLEX_OFFSET) & IS_COMPLEX_MASK)

/** Get if the wire is complex. */
#define ClassWire_GetComplex(W)			(WireInfo1_IsComplex((W)->wireInfo1))

/** Set the complexity status of a wire. */
static VEC_INLINE INT32 ClassWire_SetComplex(WireInstance * wire, const UINT32 isComplex)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo1 = WireInfo_SetBits(wire->wireInfo1, isComplex, IS_COMPLEX_MASK, IS_COMPLEX_OFFSET);
    return E_SUCCESS;
}

/** Get sample width from info1. */
#define WireInfo1_SampleWidthBytes(x)	(((x) >> SAMPLE_SIZE_OFFSET) & SAMPLE_SIZE_MASK)

/** Get the wire sample width in bytes. */
#define ClassWire_GetNSampleSize(W)		(WireInfo1_SampleWidthBytes((W)->wireInfo1))

/** Set the sample size of a wire. */
static VEC_INLINE INT32 ClassWire_SetNSampleSize(WireInstance * wire, const UINT32 sampleSizeBytes)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo1 = WireInfo_SetBits(wire->wireInfo1, sampleSizeBytes, SAMPLE_SIZE_MASK, SAMPLE_SIZE_OFFSET);    
    return E_SUCCESS;
}

/** Create info2 word. */
#define INFO2_PROPS(blockSize, dataType) \
	((blockSize & BLOCK_SIZE_MASK) | ((dataType & DATA_TYPE_MASK) << DATA_TYPE_OFFSET))

/** Get the block size from info2. */
#define WireInfo2_GetBlockSize(x)		((x) & BLOCK_SIZE_MASK)

/** Get the block size of a wire. */
#define ClassWire_GetBlockSize(W)		(WireInfo2_GetBlockSize((W)->wireInfo2))

/** Set the block size of a wire. */
static VEC_INLINE INT32 ClassWire_SetBlockSize(WireInstance * wire, const UINT32 blockSize)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo2 = WireInfo_SetBits(wire->wireInfo2, blockSize, BLOCK_SIZE_MASK, BLOCK_SIZE_OFFSET);     
    return E_SUCCESS;
}

/** Get the data type from info2. */
#define WireInfo2_GetDataType(x)		(((x) >> DATA_TYPE_OFFSET) & DATA_TYPE_MASK)

/** Get the data type of a wire. */
#define ClassWire_GetDataType(W)		(WireInfo2_GetDataType((W)->wireInfo2))

/** Set the data type of a wire. */
static VEC_INLINE INT32 ClassWire_SetDataType(WireInstance * wire, const UINT32 dataType)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo2 = WireInfo_SetBits(wire->wireInfo2, dataType, DATA_TYPE_MASK, DATA_TYPE_OFFSET);        
    return E_SUCCESS;
}


/** Create info3 word. */
#define INFO3_PROPS(rows, cols) \
	((((cols) & NUM_COLUMNS_MASK) << NUM_COLUMNS_OFFSET) | ((rows) & NUM_ROWS_MASK))

/** Get the rows from info3. */
#define WireInfo3_GetRows(x)            ((x) & NUM_ROWS_MASK)

/** Get the wire number of rows. */
#define ClassWire_GetNumMatRows(W)      (WireInfo3_GetRows((W)->wireInfo3))

/** Set the number of rows of a wire. */
static VEC_INLINE INT32 ClassWire_SetNumMatRows(WireInstance * wire, const UINT32 numMatRows)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo3 = WireInfo_SetBits(wire->wireInfo3, numMatRows, NUM_ROWS_MASK, NUM_ROWS_OFFSET);
    return E_SUCCESS;
}

/** Get the columns from info3. */
#define WireInfo3_GetCols(x)            (((x) >> NUM_COLUMNS_OFFSET) & NUM_COLUMNS_MASK)

/** Get the wire number of columns. */
#define ClassWire_GetNumMatCols(W)      (WireInfo3_GetCols((W)->wireInfo3))

/** Set the number of columns of a wire. */
static VEC_INLINE INT32 ClassWire_SetNumMatCols(WireInstance * wire, const UINT32 numMatCols)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo3 = WireInfo_SetBits(wire->wireInfo3, numMatCols, NUM_COLUMNS_MASK, NUM_COLUMNS_OFFSET);
    return E_SUCCESS;
}

/** The IPC bit in info3. */
#define IS_IPC_BIT              (IS_IPC_MASK << IS_IPC_OFFSET)

/** Get the IPC status from info3. */
#define WireInfo3_IsIPC(x)      (((x) >> IS_IPC_OFFSET) & IS_IPC_MASK)

/** Get the IPC status from the wire. */
#define ClassWire_GetIPC(W)     (WireInfo3_IsIPC((W)->wireInfo3))

/** Set the IPC status of a wire. */
static VEC_INLINE INT32 ClassWire_SetIPC(WireInstance * wire, const UINT32 isIPC)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo3 = WireInfo_SetBits(wire->wireInfo3, isIPC, IS_IPC_MASK, IS_IPC_OFFSET);
    return E_SUCCESS;
}

/** The private pin bit in info3. */
#define IS_PRIVATE_BIT                  (IS_PRIVATE_MASK << IS_PRIVATE_OFFSET)

/** Test if pin is private. */
#define INFO3_IS_PRIVATE_BIT(info3)     ((IS_PRIVATE_BIT & (info3)) != 0)

/** Get the private status from the wire. */
#define ClassWire_GetPrivate(W)         (INFO3_IS_PRIVATE_BIT((W)->wireInfo3))

/** Set the private status of a wire. */
static VEC_INLINE INT32 ClassWire_SetPrivate(WireInstance * wire, const UINT32 isPrivate)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo3 = WireInfo_SetBits(wire->wireInfo3, isPrivate, IS_PRIVATE_MASK, IS_PRIVATE_OFFSET);
    return E_SUCCESS;
}

/** The clock master bit. */
#define CLOCK_MASTER_BIT                (IS_CLOCK_MASTER_MASK << IS_CLOCK_MASTER_OFFSET)

/** Test if pin is the clock master. */
#define INFO3_IS_CLOCK_MASTER(info3)    ((CLOCK_MASTER_BIT & (info3)) != 0)

/** Get the clock master status from the wire. */
#define ClassWire_GetClockMaster(W)     (INFO3_IS_CLOCK_MASTER((W)->wireInfo3))

/** Set the clock master status of a wire. */
static VEC_INLINE INT32 ClassWire_SetClockMaster(WireInstance * wire, const UINT32 isClockMaster)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }
    wire->wireInfo3 = WireInfo_SetBits(wire->wireInfo3, isClockMaster, IS_CLOCK_MASTER_MASK, IS_CLOCK_MASTER_OFFSET);
    return E_SUCCESS;
}


/** Set the sample rate of a wire. */
static VEC_INLINE INT32 ClassWire_SetSampleRate(WireInstance * wire, const FLOAT32 sampleRate)
{
    if (!wire)
    {
        return E_NOT_OBJECT;
    }    
    wire->sampleRate = sampleRate;
    return E_SUCCESS;
}

/** Get the sample rate of a wire. */
#define ClassWire_GetSampleRate(W)		((W)->sampleRate)	// float

/** Get the number of samples in a wire. */
#define ClassWire_GetNumSamples(W)		(ClassWire_GetBlockSize(W) * ClassWire_GetChannelCount(W))

#ifdef	__cplusplus
}
#endif

#endif	/* _FW_WIRE_H */
