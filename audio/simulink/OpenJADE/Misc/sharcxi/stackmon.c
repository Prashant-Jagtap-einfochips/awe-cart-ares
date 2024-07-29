#define __LONG_LONG_PROCESSOR_TIME__

#include <sys/platform.h>
#include <builtins.h>
#include <stdio.h>

typedef struct heapstack_desc {
  void *base;
  size_t length;
} heapstack_desc_t;

extern heapstack_desc_t heapstack_table[2];

#pragma inline
#pragma always_inline
uint32_t stackmon_paint_stack(void)
{
   uint32_t * scan;
   uint32_t length;
   uint32_t curr_sp;

   int i;

   __asm volatile ("%0 = i7; \n" : "=r" (curr_sp) );

   scan = (uint32_t *)heapstack_table[1].base;
   length = curr_sp - (uint32_t)heapstack_table[1].base;

   for (i=0;i<(length>>2);i++)
   {
      *scan++ = 0xDEADBEEF;
   }

   return length;
}

#pragma inline
#pragma always_inline
uint32_t stackmon_stack_usage(void)
{
   uint32_t * base;
   uint32_t * scan;
   uint32_t length;
   int i;

   base = (uint32_t *)heapstack_table[1].base;
   length = (uint32_t)heapstack_table[1].length;

   scan = base;

   for (i=0;i<(length>>2);i++)
   {
      if (*scan != 0xDEADBEEF)
         break;
      scan++;
   }
   length = scan - base;
   return (length*4);
}

typedef struct stack_mon_stats
{
   uint64_t emuclk;
   uint32_t length;
   uint32_t dummy;
} stack_mon_stats_t;


uint32_t BAF_Callback_Stats(uint32_t *log, uint32_t len, uint32_t start_stop)
{
   uint32_t length;

   uint32_t cnt = sizeof(stack_mon_stats_t)/sizeof(uint32_t);

   stack_mon_stats_t *ptr = (stack_mon_stats_t *)log;

   if (len < cnt)
      return 0;

   if (start_stop == 0)
   {
      length = stackmon_paint_stack();
   }
   else
   {
      length = stackmon_stack_usage();
   }
   ptr->emuclk = __builtin_emuclk();
   ptr->length = length;

   return cnt;
}

void OJSP_Configure_Stats(void)
{

}
