/* ----------------------------------------------------------------------
** Audio Weaver Target Heap Used Header File
** Created on 29-Jul-2024 15:08:42
** ------------------------------------------------------------------- */

#if defined(SINGLE_HEAP)
#define MASTER_HEAP_SIZE (2309)
#define FASTB_HEAP_SIZE (0)
#define SLOW_HEAP_SIZE (0)
#define SHARED_HEAP_SIZE (33)
#else
#define MASTER_HEAP_SIZE (2231)
#define FASTB_HEAP_SIZE (17)
#define SLOW_HEAP_SIZE (61)
#define SHARED_HEAP_SIZE (33)
#endif

