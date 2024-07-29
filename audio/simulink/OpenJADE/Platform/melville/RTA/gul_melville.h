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
#define FRAM_SIZE_CMD         (64)

/* sport buffers for ADC/DAC data */
extern int32_t sport_TxBuffer1[FRAME_SIZE_OUT + FRAM_SIZE_CMD];
extern int32_t sport_TxBuffer2[FRAME_SIZE_OUT + FRAM_SIZE_CMD];
extern int32_t sport_RxBuffer1[FRAME_SIZE_IN + FRAM_SIZE_CMD];
extern int32_t sport_RxBuffer2[FRAME_SIZE_IN + FRAM_SIZE_CMD];

/* SPU handle */
/* Valid only after oj_ezkit_init call */
extern ADI_SPU_HANDLE hSpu;

void gul_melville_init(void);
void gul_melville_audio_start(void);

void UART_putc(char c);
int UART_getc(void);
#endif /*__GUL_MELVILLE_H__*/


