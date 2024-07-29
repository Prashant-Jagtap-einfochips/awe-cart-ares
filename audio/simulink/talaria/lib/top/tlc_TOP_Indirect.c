%% TLC code generation file - hand written source
%% INCLUDED BY: TOP_Indirect.tlc
        
%% Code used only for INDIRECT addressing mode

// -----------------------------------------------------------------
static void* ValidateArgsForTOP(const uint32_t token, const uint32_t offset, const
    uint32_t byteCount, int* retVal)
{
    void *addressOut = 0;
    *retVal = ASDSTATUS_OK;

    // Break apart the TOP token to get the pieces of interest
    uint32_t groupIdx = ASDID_GROUP(token);
    uint32_t partitionIdx = ASDID_PARTITION(token);
    uint32_t topIdx = TOPID_INDEX(token);
    
    // Use these pieces to access the partition root for this partition
    void** pointerArray = 0;
    uint32_t* sizeArray = 0;
    uint32_t count = 0;
    PartitionRoot_type* partitionRoot = GetPartitionRoot(groupIdx,partitionIdx);
    if (partitionRoot && partitionRoot->topTable && partitionRoot->topSize)
    {
        pointerArray = partitionRoot->topTable;
        sizeArray = partitionRoot->topSize;
        count = partitionRoot->topCount;
    }
    else
    {
        // This partition does not have TOPs...
        *retVal = ASDSTATUS_TOP_BAD_PARTITION_ID;
    }
    
    // Try to acquire the TOP pointer from the partition root
    if (pointerArray && sizeArray)
    {
        // validate TOP index within partition is good
        if (topIdx >= count)
        {
            *retVal = ASDSTATUS_TOP_BAD_INDEX;
        }
        else
        {
            // validate overlay data range is good
            uint32_t topSize = sizeArray[topIdx];
            uint32_t endOffset = offset + byteCount;
            if (endOffset > topSize)
            {
                *retVal = ASDSTATUS_TOP_OUT_OF_RANGE;
            }
            else
            {
                // input arguments have passed validation
                uint8_t* bytebuffer = pointerArray[topIdx];
                addressOut = &bytebuffer[offset];
            }
        }
    }

    return addressOut;
}

