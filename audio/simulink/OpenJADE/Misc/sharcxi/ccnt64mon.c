#define __LONG_LONG_PROCESSOR_TIME__

#include <sys/platform.h>
#include <builtins.h>

typedef struct ccnt64_mon_stats
{
   uint64_t emuclk;
} ccnt64_mon_stats_t;

uint32_t BAF_Callback_Stats(uint32_t *log, uint32_t len, uint32_t start_stop)
{
   uint32_t cnt = sizeof(ccnt64_mon_stats_t)/sizeof(uint32_t);

   ccnt64_mon_stats_t *ptr = (ccnt64_mon_stats_t *)log;

   if (len < cnt)
      return 0;

   ptr->emuclk = __builtin_emuclk();

   return cnt;
}

void OJSP_Configure_Stats(void)
{

}


