#if !defined(HEAPSTACK_USAGE_H_INCLUDED__)
#define HEAPSTACK_USAGE_H_INCLUDED__

#include <stddef.h>
#include <stdint.h>

#if defined(__ADSPSHARC__)
typedef struct memblock_info {
  void *base;
  size_t length;
  size_t usage;
} memblock_info_t;

typedef struct heapstack_info {
  memblock_info_t heap;
  memblock_info_t stack;
} heapstack_info_t;

void overall_heapstack_usage(heapstack_info_t *hs);
uint32_t module_paint_stack(void);
uint32_t module_stack_usage(uint32_t sp);
#endif /*defined(__ADSPSHARC__)*/


#endif /*HEAPSTACK_USAGE_H_INCLUDED__*/ 

