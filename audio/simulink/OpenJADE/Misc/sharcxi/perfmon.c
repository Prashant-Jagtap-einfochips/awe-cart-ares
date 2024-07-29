#define __LONG_LONG_PROCESSOR_TIME__

#include <sys/platform.h>
#include <builtins.h>
#include <stdio.h>

typedef enum perf_mon_modes {
	perf_mon_dmc_mode = 2, /* DMCACHE */
	perf_mon_pmc_mode = 3, /* PMCACHE */
	perf_mon_ic_mode = 4   /* ICACHE */
} e_perf_mon_modes;

typedef struct perf_mon_stats
{
   uint64_t emuclk;
   uint16_t cntr3;
   uint16_t cntr4;
   uint16_t cntr5;
   uint16_t cntr6;
} perf_mon_stats_t;

#pragma inline
#pragma always_inline
static void wait_pipeline(void)
{
	asm volatile ("NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;NOP;\n");
}

void perf_mon_print(e_perf_mon_modes mode)
{
  switch (mode) {
    case perf_mon_dmc_mode:
      printf("%8d occurrences of DM-Cache hits\n"
             "%8d occurrences of Crosscheck hits\n"
             "%8d occurrences of DM-Cache miss without writeback\n"
             "%8d occurrences of DM-Cache miss with writeback\n",
             *pREG_PFM_CNTR3, *pREG_PFM_CNTR4,
             *pREG_PFM_CNTR5, *pREG_PFM_CNTR6);
      break;
    case perf_mon_pmc_mode:
      printf("%8d occurrences of PM-Cache hits\n"
             "%8d occurrences of Crosscheck hits\n"
             "%8d occurrences of PM-Cache miss without writeback\n"
             "%8d occurrences of PM-Cache miss with writeback\n",
             *pREG_PFM_CNTR3, *pREG_PFM_CNTR4,
             *pREG_PFM_CNTR5, *pREG_PFM_CNTR6);
      break;
    case perf_mon_ic_mode:
      printf("%8d occurrences of I-Cache hits\n"
             "%8d occurrences of I-Cache misses\n",
             *pREG_PFM_CNTR3, *pREG_PFM_CNTR4);
      break;
  };
}

#pragma inline
static void perf_mon_configure(e_perf_mon_modes mode)
{
  *pREG_PFM_CFG &= ~(BITM_PFM_CFG_DMC|BITM_PFM_CFG_PMC|BITM_PFM_CFG_IC);
  switch (mode) {
    case perf_mon_dmc_mode:
      *pREG_PFM_CFG |= BITM_PFM_CFG_DMC;
      wait_pipeline();
      break;
    case perf_mon_pmc_mode:
      *pREG_PFM_CFG |= BITM_PFM_CFG_PMC;
      wait_pipeline();
      break;
    case perf_mon_ic_mode:
      *pREG_PFM_CFG |= BITM_PFM_CFG_IC;
      wait_pipeline();
      break;
  };
}

#pragma inline
static void perf_mon_clr(void)
{
   *pREG_PFM_CNTR3CLR = 1;
   *pREG_PFM_CNTR4CLR = 1;
   *pREG_PFM_CNTR5CLR = 1;
   *pREG_PFM_CNTR6CLR = 1;
   wait_pipeline();
}

#pragma inline
static void perf_mon_start(void)
{
   *pREG_PFM_CNTR3START = 1;
   *pREG_PFM_CNTR4START = 1;
   *pREG_PFM_CNTR5START = 1;
   *pREG_PFM_CNTR6START = 1;
   wait_pipeline();
}

#pragma inline
static void perf_mon_pause(void)
{
   *pREG_PFM_CNTR3PAUSE = 1;
   *pREG_PFM_CNTR4PAUSE = 1;
   *pREG_PFM_CNTR5PAUSE = 1;
   *pREG_PFM_CNTR6PAUSE = 1;
   wait_pipeline();
}

uint32_t BAF_Callback_Stats(uint32_t *log, uint32_t len, uint32_t start_stop)
{
   uint32_t cnt = sizeof(perf_mon_stats_t)/sizeof(uint32_t);

   perf_mon_stats_t *ptr = (perf_mon_stats_t *)log;

   if (len < cnt)
      return 0;

   ptr->emuclk = __builtin_emuclk();
   ptr->cntr3 = *pREG_PFM_CNTR3;
   ptr->cntr4 = *pREG_PFM_CNTR4;
   ptr->cntr5 = *pREG_PFM_CNTR5;
   ptr->cntr6 = *pREG_PFM_CNTR6;
   perf_mon_clr();
   perf_mon_start();

   return cnt;
}

void OJSP_Configure_Stats(void)
{
#if defined(PERFMON_MODE_DMCACHE)
   e_perf_mon_modes mode = perf_mon_dmc_mode;
#elif defined(PERFMON_MODE_PMCACHE)
   e_perf_mon_modes mode = perf_mon_pmc_mode;
#elif defined(PERFMON_MODE_ICACHE)
   e_perf_mon_modes mode = perf_mon_ic_mode;
#endif

   perf_mon_configure(mode);
}

