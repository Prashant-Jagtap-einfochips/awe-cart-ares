/*
 * Model_CoreMap.h : Determine core assignment from a token or atomic id
 *
 */

// Retrieve core number from message identifier token
// (e.g. from RTC_IDs, TOP_IDs, or TSP_IDs *.h files)
char GetCoreFromToken(unsigned int tokenIn);

// Retrieve core number from atomic id 
// (e.g. from ASDID_ATOMICID macro or PartitionList *.h file)
char GetCoreFromAtomicId(unsigned int atomicIdIn);
