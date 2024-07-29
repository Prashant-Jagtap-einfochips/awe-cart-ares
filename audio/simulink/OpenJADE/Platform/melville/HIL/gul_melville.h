#ifndef __GUL_MELVILLE_H__
#define __GUL_MELVILLE_H__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <services/spu/adi_spu.h>

#define PLATFORM_NUM_IN_CHAN  (8)
#define PLATFORM_NUM_OUT_CHAN (8)

#define FRAME_SIZE_PER_CHAN   (32)
#define FRAME_SIZE_IN         (FRAME_SIZE_PER_CHAN*(PLATFORM_NUM_IN_CHAN))  //256
#define FRAME_SIZE_OUT        (FRAME_SIZE_PER_CHAN*PLATFORM_NUM_OUT_CHAN) //256

/* sport buffers for ADC/DAC data */
extern int32_t sport_TxBuffer1[FRAME_SIZE_OUT];
extern int32_t sport_TxBuffer2[FRAME_SIZE_OUT];
extern int32_t sport_RxBuffer1[FRAME_SIZE_IN];
extern int32_t sport_RxBuffer2[FRAME_SIZE_IN];

/* SPU handle */
/* Valid only after oj_ezkit_init call */
extern ADI_SPU_HANDLE hSpu;

void oj_ezkit_init(void);
void oj_audio_start(void);

void oj_uart_txdma_start(unsigned char *src_addr, int cnt);
int oj_uart_txdma_complete(void);

void UART_putc(char c);
int UART_getc(void);
#endif /*__GUL_MELVILLE_H__*/


