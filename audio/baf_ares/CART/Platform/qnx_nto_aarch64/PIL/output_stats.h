#ifndef __OUTPUT_STATS_H__
#define __OUTPUT_STATS_H__

#include <stdio.h>
#include <stdint.h>
#include "ram_file.h"

void init_output_stats(void);
void print_output_stats(FILE *fp);
void update_output_stats(RAMIO_FILE *fp, float *buff, uint32_t num_chan, uint32_t frame_size);


#endif //__OUTPUT_STATS_H__

