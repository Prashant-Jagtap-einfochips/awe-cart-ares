#include <stdint.h>

typedef struct pkt_mon_stats
{
   uint64_t pcycles;
   uint64_t pktcnt;
} pkt_mon_stats_t;


static inline uint64_t get_pcycles(void)
{
   uint64_t reg;
   asm volatile ("%0=upcycle\n"
                 :"=r"(reg));
   return reg;
}

#define PKTCOUNT_MON 0x1000
#define SSR_CE_SET 0x800000
/*
    This function enables the packet count register for monitor mode.
*/
static inline void enable_pktcount(void)
{
  uint32_t mon_mode=PKTCOUNT_MON, set_ssrce=SSR_CE_SET;

  __asm__ __volatile__ (
    "r0 = usr\n"
   "r1 = %0\n"
   "r0 = or(r0,r1)\n"
   "usr = r0\n"
   "isync\n"
   "r0 = ssr\n"
   "r1 = %1\n"
   "r0 = or(r0, r1)\n"
   "ssr = r0\n"
   "isync\n"
   :
    : "r" (mon_mode), "r" (set_ssrce)
   : "r0", "r1"
  );
}

/*
    This function reads the packet count register for thread currently active.
*/
static inline uint64_t read_pktcount(void)
{
  uint32_t lo, hi;
  uint64_t pcyc;

  __asm__ __volatile__ (
    "%0 = pktcountlo\n"
    "%1 = pktcounthi\n"
   "isync\n"
    : "=r" (lo), "=r" (hi)
  );
  pcyc = (((uint64_t) hi)<<32) + lo;
  return pcyc;
}
/*
    This function reads the packet count register for thread currently active.
*/
//void write_pktcount(int counthi, int countlo)
static inline void write_pktcount(uint64_t count)
{
   __asm__ __volatile__ (
      "PKTCOUNT = %0\n"
      //"c18 = r0\n"
      "isync\n"
      :
      : "r" (count)
      : "r1", "r0"
   );
}

uint32_t BAF_Callback_Stats(uint32_t *log, uint32_t len, uint32_t start_stop)
{
   uint32_t cnt = sizeof(pkt_mon_stats_t)/sizeof(uint32_t);
   pkt_mon_stats_t *ptr = (pkt_mon_stats_t *)log;

   if (len < cnt)
      return 0;

   ptr->pcycles = get_pcycles();
   ptr->pktcnt = read_pktcount();

   return cnt;
}

void OJSP_Configure_Stats(void)
{
   enable_pktcount();
   write_pktcount(0ULL);
}

