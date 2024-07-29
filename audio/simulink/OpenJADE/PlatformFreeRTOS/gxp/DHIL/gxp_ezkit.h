#ifndef __GXP_EZKIT_H__
#define __GXP_EZKIT_H__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <services/spu/adi_spu.h>

#define PLATFORM_NUM_IN_CHAN  (4)
#define PLATFORM_NUM_OUT_CHAN (8)

#define FRAME_SIZE_PER_CHAN   (32)
#define FRAME_SIZE_IN         (FRAME_SIZE_PER_CHAN*PLATFORM_NUM_IN_CHAN)  //128
#define FRAME_SIZE_OUT        (FRAME_SIZE_PER_CHAN*PLATFORM_NUM_OUT_CHAN) //2*128

/* sport buffers for ADC/DAC data */
extern int32_t int_SP0ABuffer1[FRAME_SIZE_OUT];
extern int32_t int_SP0ABuffer2[FRAME_SIZE_OUT];
extern int32_t int_SP0ABuffer4[FRAME_SIZE_IN];
extern int32_t int_SP0ABuffer5[FRAME_SIZE_IN];

void oj_ezkit_init(void);
void oj_audio_init(void);
void oj_audio_start(void);

void oj_uart_txdma_start(unsigned char *src_addr, int cnt);
int oj_uart_txdma_complete(void);

void UART_putc(char c);
int UART_getc(void);
#endif /*__GXP_EZKIT_H__*/


