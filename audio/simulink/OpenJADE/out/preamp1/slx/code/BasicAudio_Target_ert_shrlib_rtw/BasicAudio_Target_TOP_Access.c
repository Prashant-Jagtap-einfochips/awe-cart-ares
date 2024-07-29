/*
 * File: BasicAudio_Target_TOP_Access.c
 *
 * Code generated for Simulink model 'BasicAudio_Target'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:46:55 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "rtwtypes.h"
#include "ASDID.h"                     // for ASDID parsing macros
#include "BasicAudio_Target_TOP.h"
#include <string.h>                    // for memcpy
#include "BasicAudio_Target_TOP_Access.h"
#include "BasicAudio_Target_TOP_IDs.h" // for request structure

// Struct for overlay bank table
typedef struct TOP_BankTable_tag
{
    uint32_t bank_sizeof;
    void* bank_ptr;
}
TOP_BankTable_type;

// Struct for overlay group table
typedef struct TOP_GroupTable_tag
{
    uint32_t bank_count;
    TOP_BankTable_type* bank_table;
}
TOP_GroupTable_type;

static TOP_BankTable_type BasicAudio_BankTable[] =
{
    {
        TOP_BANK_BasicAudio_Target_BasicAudio0_SIZE, (void*)
        TOP_BANK_BasicAudio_Target_BasicAudio0
    },

    {
        TOP_BANK_BasicAudio_Target_BasicAudio1_SIZE, (void*)
        TOP_BANK_BasicAudio_Target_BasicAudio1
    },

    {
        TOP_BANK_BasicAudio_Target_BasicAudio2_SIZE, (void*)
        TOP_BANK_BasicAudio_Target_BasicAudio2
    },

    {
        TOP_BANK_BasicAudio_Target_BasicAudio3_SIZE, (void*)
        TOP_BANK_BasicAudio_Target_BasicAudio3
    },

    {
        TOP_BANK_BasicAudio_Target_BasicAudio4_SIZE, (void*)
        TOP_BANK_BasicAudio_Target_BasicAudio4
    },

    {
        TOP_BANK_BasicAudio_Target_BasicAudio5_SIZE, (void*)
        TOP_BANK_BasicAudio_Target_BasicAudio5
    },

    {
        TOP_BANK_BasicAudio_Target_BasicAudio6_SIZE, (void*)
        TOP_BANK_BasicAudio_Target_BasicAudio6
    },
};

// ----- The GROUP TABLE -----:
static TOP_GroupTable_type top_group_table[] =
{
    //TABLE-START
    {
        7, BasicAudio_BankTable
    },                                 // "BasicAudio" overlay group

    //TABLE-END
};

// -----------------------------------------------------------------
static void* ValidateArgsForTOP(const uint32_t token, const uint32_t offset,
    const
    uint32_t byteCount, int* retVal)
{
    uint32_t groupIdx = ASDID_GROUP(token);
    uint32_t bankIdx = TOPID_BANK(token);
    uint32_t theIdx = TOPID_OFFSET(token);
    uint32_t tableSize = sizeof(top_group_table) / sizeof(TOP_GroupTable_type);
    void *addressOut = 0;
    *retVal = ASDSTATUS_OK;

    // validate group index is between 1 to tableSize (inclusive)
    if (groupIdx == 0 || groupIdx > tableSize)
    {
        *retVal = ASDSTATUS_TOP_BAD_GROUP_ID;
    }
    else
    {
        // validate bank index is good
        TOP_GroupTable_type* gtableEntry = &top_group_table[groupIdx - 1];
        if (bankIdx >= gtableEntry->bank_count)
        {
            *retVal = ASDSTATUS_TOP_BAD_BANK_ID;
        }
        else
        {
            // validate overlay data range is good
            TOP_BankTable_type* ptableEntry = &gtableEntry->
                bank_table[bankIdx];
            uint32_t startOffset = theIdx + offset;
            uint32_t endOffset = startOffset + byteCount;
            if (startOffset > ptableEntry->bank_sizeof ||
                endOffset > ptableEntry->bank_sizeof)
            {
                *retVal = ASDSTATUS_TOP_OUT_OF_RANGE;
            }
            else
            {
                // input arguments have passed validation
                uint8_t* bytebuffer = ptableEntry->bank_ptr;
                addressOut = &bytebuffer[startOffset];
            }
        }
    }

    return addressOut;
}

// -----------------------------------------------------------------
static int SetTOP(const uint32_t token, const uint32_t offset, const uint32_t
                  byteCount,
                  const void* data)
{
    int errCode;
    void* theAddress = ValidateArgsForTOP(token, offset, byteCount, &errCode);
    if (theAddress && errCode == 0)
    {
        // Set the TOP data range
        memcpy(theAddress, data, byteCount);
    }

    return errCode;
}

// -----------------------------------------------------------------
static int GetTOP(const uint32_t token, const uint32_t offset, const uint32_t
                  byteCount,
                  void* data)
{
    int errCode;
    void* theAddress;
    theAddress = ValidateArgsForTOP(token, offset, byteCount, &errCode);
    if (theAddress && errCode == 0)
    {
        // Retrieve data from the TOP data range
        memcpy(data, theAddress, byteCount);
    }

    return errCode;
}

// -----------------------------------------------------------------
/** Handle request to read or write Target Overlay Parameter (TOP).
 *  @param pReqBuf Request buffer containing @ref TOP_WriteRequest_type
 *  @param reqBufSize number of bytes used in the request buffer, which can
 *         be described by the structure appropriate to the request:
 *         either @ref TOP_WriteResponse_type or @ref TOP_ReadResponse_type.
 *  @param pRspBuf Response buffer to receive response of either TOP_ReadResponse_type
 *         or TOP_WriteResponse_type, depending on the type of request
 *  @param rspBufSize Size of available bytes provided for the response to fill
 *  @return Returns the number of bytes utilized in the response buffer, or
 *          ASDSTATUS_RSP_BUFFER_SMALL if the response buffer is too small to
 *          contain a AsdIdStatus_type payload (which could in turn hold
 *          an error).
 **/
int BasicAudio_Target_HandleTopRequest(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize)
{
    // returns number of bytes
    int retVal = 0;
    int status = ASDSTATUS_OK;
    uint32_t reqRequiredBytes;
    uint32_t rspRequiredBytes;

    // parse out the message id and caller handle
    if (reqBufSize < (sizeof(uint32_T)*2) ||
        pReqBuf == 0)
    {
        return ASDSTATUS_REQ_BUFFER_SMALL;
    }

    uint32_t msgId = ((uint32_T*)pReqBuf)[0];
    uint32_t callerHandle = ((uint32_T*)pReqBuf)[1];
    if (TOPID_IS_WRITE(msgId))
    {
        // WRITE data from request buffer into memory. Respond with just the topId
        TOP_WriteRequest_type* topREQ = (TOP_WriteRequest_type*)pReqBuf;
        TOP_WriteResponse_type* topRSP = (TOP_WriteResponse_type*)pRspBuf;
        reqRequiredBytes = sizeof(TOP_WriteRequest_type) + topREQ->byteCount;
        rspRequiredBytes = sizeof(TOP_WriteResponse_type);
        uint8_t reqBufOK = (reqBufSize >= reqRequiredBytes) && pReqBuf;
        uint8_t rspBufOK = (rspBufSize >= rspRequiredBytes) && pRspBuf;
        if (reqBufOK && rspBufOK)
        {
            // Write the data into memory
            status = SetTOP(topREQ->topId, topREQ->offset, topREQ->byteCount,
                            topREQ->data);

            // Prepare response payload
            topRSP->topId = msgId;
            topRSP->callerHandle = callerHandle;
            topRSP->statusCode = status;
            retVal = (int)rspRequiredBytes;
        }
        else
        {
            // one of the buffers was too small
            status = reqBufOK ? ASDSTATUS_RSP_BUFFER_SMALL :
                ASDSTATUS_REQ_BUFFER_SMALL;
        }
    }
    else                               // (checking read vs. write)
    {
        // READ data from memory into the response buffer
        TOP_ReadRequest_type* topREQ = (TOP_ReadRequest_type*)pReqBuf;
        TOP_ReadResponse_type* topRSP = (TOP_ReadResponse_type*)pRspBuf;
        reqRequiredBytes = sizeof(TOP_ReadRequest_type);
        rspRequiredBytes = sizeof(TOP_ReadResponse_type) + topREQ->byteCount;
        uint8_t reqBufOK = (reqBufSize >= reqRequiredBytes) && pReqBuf;
        uint8_t rspBufOK = (rspBufSize >= rspRequiredBytes) && pRspBuf;
        if (reqBufOK && rspBufOK)
        {
            // Save the byte count
            uint32_t byteCount = topREQ->byteCount;

            // Copy the data from memory into the response buffer
            status = GetTOP(
                            topREQ->topId, topREQ->offset, topREQ->byteCount, // from REQ
                            topRSP->data);// to RSP

            // Prepare the rest of the response payload
            topRSP->topId = msgId;
            topRSP->callerHandle = callerHandle;
            topRSP->statusCode = status;
            topRSP->byteCount = byteCount;
            retVal = (int)rspRequiredBytes;
        }
        else
        {
            // one of the buffers was too small
            status = reqBufOK ? ASDSTATUS_RSP_BUFFER_SMALL :
                ASDSTATUS_REQ_BUFFER_SMALL;
        }
    }

    // If a problem has arisen, the response is only a status payload
    if (status < 0)
    {
        rspRequiredBytes = sizeof(AsdIdStatus_type);
        if ((rspBufSize >= rspRequiredBytes) && pRspBuf)
        {
            // Fill in the response with information from the request
            AsdIdStatus_type* topRSP = (AsdIdStatus_type*)pReqBuf;
            topRSP->msgId = msgId;
            topRSP->callerHandle = callerHandle;
            topRSP->statusCode = status;
            retVal = (int)rspRequiredBytes;
        }
        else
        {
            // otherwise, we cannot return a response buffer!
            retVal = ASDSTATUS_RSP_BUFFER_SMALL;
        }
    }

    return retVal;
}

static void FailBuildOnMismatchedSize( void )
{
    // Direct Addressing Mode Token Creation Check:
    // This function is never called and does nothing at runtime.
    // Instead, it is a compile-time check of the datatype sizes
    // in use by TOPs in this model. The datatype sizes assigned by
    // the toolchain compiler must match the sizes used to generate
    // TOP access tokens.
#undef COMPILEASSERT
#define COMPILEASSERT(condition)       ((void)sizeof(signed char[1 - 2*(int)(!(condition))]))

    COMPILEASSERT(4 == sizeof(real32_T));
    COMPILEASSERT(4 == sizeof(uint32_T));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
