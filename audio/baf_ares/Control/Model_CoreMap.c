/*
 * Model_CoreMap.c : Determine core assignment from token.
 * The Talaria partitioning tool generates this file.
 */


#include "ASDID.h" // for token parsing macros
#include "Model_CoreMap.h"

static const char coreMap[256];

char GetCoreFromToken(unsigned int tokenIn)
{
    char coreNum = 0;
    if (ASDID_IS_SCH(tokenIn))
    {
        coreNum = SCHID_CORE_NUM(tokenIn);
    }
    else
    {
        int groupNumber = ASDID_GROUP(tokenIn);
        int partitionNumber = ASDID_PARTITION(tokenIn);
        int lookupIndex = (groupNumber * 16U) + partitionNumber;
        coreNum = coreMap[lookupIndex];
    }
    return coreNum;
}

char GetCoreFromAtomicId(unsigned int atomicIdIn)
{
    return (atomicIdIn < sizeof(coreMap)) ? coreMap[atomicIdIn] : 255U;
}

static const char coreMap[256] = 
{
    8U, // Group 0, Partition 0 is on 1_8
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    2U, // Group 1, Partition 0 is on 1_2
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    7U, // Group 2, Partition 1 is on 1_7
    2U, // Group 2, Partition 2 is on 1_2
    3U, // Group 2, Partition 3 is on 1_3
    2U, // Group 2, Partition 4 is on 1_2
    3U, // Group 2, Partition 5 is on 1_3
    3U, // Group 2, Partition 6 is on 1_3
    4U, // Group 2, Partition 7 is on 1_4
    5U, // Group 2, Partition 8 is on 1_5
    6U, // Group 2, Partition 9 is on 1_6
    6U, // Group 2, Partition 10 is on 1_6
    3U, // Group 2, Partition 11 is on 1_3
    255U, // not used
    3U, // Group 2, Partition 13 is on 1_3
    5U, // Group 2, Partition 14 is on 1_5
    6U, // Group 2, Partition 15 is on 1_6
    7U, // Group 3, Partition 0 is on 1_7
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    1U, // Group 4, Partition 0 is on 1_1
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    9U, // Group 5, Partition 0 is on 1_9
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    10U, // Group 6, Partition 0 is on 1_10
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
    255U, // not used
};

