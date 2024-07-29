#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "output_stats.h"
#include "oj_platform.h"

#define MAX_OUTPUT_CHANNEL   (48)

typedef struct
{
   float max;
   float min;
   float sum;
   int   count;
}OutputStats_t;

#pragma DATA_SECTION(g_output_stats, ".oj_ext_data")
OutputStats_t g_output_stats[MAX_OUTPUT_CHANNEL];

void init_output_stats(void)
{
   int ch;
   for (ch=0;ch<MAX_OUTPUT_CHANNEL;ch++)
   {
      g_output_stats[ch].max = 0.0f;
      g_output_stats[ch].min = 0.0f;
      g_output_stats[ch].sum = 0.0f;
      g_output_stats[ch].count = 0;
   }
   return;
}

void print_output_stats(FILE *fp)
{
   int ch;

   if (fp == NULL)
      return;

   for (ch=0;ch<MAX_OUTPUT_CHANNEL;ch++)
   {
      if (g_output_stats[ch].count > 0)
      {
         fprintf(fp, "channel[%d].stats max %f min %f avg %f\n",
                ch,
                g_output_stats[ch].max,
                g_output_stats[ch].min,
                g_output_stats[ch].sum/g_output_stats[ch].count);
      } 
   }
   return;
}

void update_output_stats(RAMIO_FILE *fp, float *buff, uint32_t num_chan, uint32_t frame_size)
{
#ifdef __TI_COMPILER_VERSION__
 #if __TI_COMPILER_VERSION__ < 8000000
   float gold[32];
   if ( frame_size > 32)
   {
      platform_printf("Error: Frame size (%d) too big. adjust gold[size]\n", frame_size);
      return;
   }
 #else
   float gold[frame_size];
 #endif
#else
   float gold[frame_size];
#endif

   int ch, idx;

   for (ch = 0; ch < num_chan; ch++)
   {
      uint32_t cnt = RAMIO_fread(gold, sizeof(float), frame_size, fp);

      if (cnt != frame_size)
      {
         platform_printf("Error: ch[%d] update_output_stats got %d exp % d\n", ch, cnt, frame_size);
         break;
      }

      for (idx = 0; idx < frame_size; idx++)
      {
         float ref = gold[idx];
         float sig = buff[ch * frame_size + idx];
         float adiff = fabsf(ref - sig);

         g_output_stats[ch].max = adiff > g_output_stats[ch].max ? adiff : g_output_stats[ch].max;
         g_output_stats[ch].min = adiff < g_output_stats[ch].min ? adiff : g_output_stats[ch].min; 
         g_output_stats[ch].sum = adiff + g_output_stats[ch].sum;
         g_output_stats[ch].count++;
      }
   }
   return;
}
