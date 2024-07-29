#ifndef __GUL_EZKIT_H__
#define __GUL_EZKIT_H__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PLATFORM_NUM_IN_CHAN  (4)
#define PLATFORM_NUM_OUT_CHAN (4)

#define FRAME_SIZE_PER_CHAN   (32)
#define FRAME_SIZE_IN         (FRAME_SIZE_PER_CHAN*PLATFORM_NUM_IN_CHAN)  //128
#define FRAME_SIZE_OUT        (FRAME_SIZE_PER_CHAN*PLATFORM_NUM_OUT_CHAN) //128

void oj_ezkit_init(void);
void oj_audio_start(void);
uint32_t resumbitAdcDacBuffers(void *pADC, void *pDAC);

void UART_putc(char c);
int UART_getc(void);
#endif /*__GUL_EZKIT_H__*/


