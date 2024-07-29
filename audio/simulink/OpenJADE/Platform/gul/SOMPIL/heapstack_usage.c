#include "heapstack_usage.h"

#if defined(__ADSPSHARC__)
extern "asm" unsigned long ldf_heap_space;
extern "asm" unsigned long ldf_heap_length;
extern "asm" unsigned long ldf_stack_space;
extern "asm" unsigned long ldf_stack_length;

typedef struct heapstack_desc {
  void *base;
  size_t length;
} heapstack_desc_t;

const heapstack_desc_t heapstack_table[2] =
{
  { &ldf_heap_space, (unsigned long) &ldf_heap_length},
  { &ldf_stack_space, (unsigned long) &ldf_stack_length}
};

void overall_heapstack_usage(heapstack_info_t *hs)
{
   uint32_t * base;
   uint32_t * scan;
   uint32_t length;
   uint32_t heap;
   uint32_t stack;
   int i;

   base = (uint32_t *)heapstack_table[0].base;
   length = (uint32_t)heapstack_table[0].length;
   hs->heap.base = base;
   hs->heap.length = length;

   length /= 4;
   scan = (base + length) - 1;

   for (i=0;i<length;i++)
   {
      if (*scan != 0xDEADBEEF)
         break;
      scan--;
   }
   heap = (scan - base);

   hs->heap.usage = heap*4;

   base = (uint32_t *)heapstack_table[1].base;
   length = (uint32_t)heapstack_table[1].length; 
   hs->stack.base = base;
   hs->stack.length = length;

   scan = base;
   length /= 4;

   for (i=0;i<length;i++)
   {
      if (*scan != 0xDEADBEEF)
         break;
      scan++;
   }
   stack = (base+length) - scan;
   hs->stack.usage = stack*4;
}

#pragma never_inline
uint32_t module_paint_stack(void)
{
   uint32_t * scan;
   uint32_t length;
   uint32_t curr_sp;

   int i;

   __asm volatile ("%0 = i7; \n" : "=r" (curr_sp) );

   scan = (uint32_t *)heapstack_table[1].base;
   length = curr_sp - (uint32_t)heapstack_table[1].base; 

   length /= 4;

   for (i=0;i<length;i++)
   {
      *scan++ = 0xDEADBEEF;
   }

   return curr_sp;
}

#pragma never_inline
uint32_t module_stack_usage(uint32_t sp)
{
   uint32_t * base;
   uint32_t * scan;
   uint32_t length;
   int i;

   base = (uint32_t *)heapstack_table[1].base;
   length = sp - (uint32_t)heapstack_table[1].base; 

   scan = base;
   length /= 4;

   for (i=0;i<length;i++)
   {
      if (*scan != 0xDEADBEEF)
         break;
      scan++;
   }
   length = (base+length) - scan;
   return (length*4);
}
#endif /*defined(__ADSPSHARC__)*/

