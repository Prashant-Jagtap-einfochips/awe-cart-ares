%% TLC code generation file - hand written source
%% INCLUDED BY: TOP_Direct.tlc

%% Code used only for DIRECT addressing mode

// -----------------------------------------------------------------
static void* ValidateArgsForTOP(const uint32_t token, const uint32_t offset, const
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
