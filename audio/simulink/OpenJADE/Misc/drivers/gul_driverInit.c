#include "gul_driverInit.h"
#include "gMDMA.h"
#include "gHAM.h"

int32_t gul_driverInit(void *args)
{
   gulDriverInit_t *d = (gulDriverInit_t *)args;

   adi_sec_SetPriority(INTR_IIR0_DMA,     d->iir0_sec_priority);
   adi_sec_SetPriority(INTR_FIR0_DMA,     d->fir0_sec_priority);
   adi_sec_SetPriority(INTR_EMDMA0_DONE,  d->emdma0_sec_priority);
   adi_sec_SetPriority(INTR_EMDMA1_DONE,  d->emdma1_sec_priority);

   {
      /* Make EMDMA to generate secure transactions */
      ADI_SPU_RESULT spuResult = adi_spu_EnableMasterSecure(d->hSpu, ADI_SPU0_EMDMA0_ID, true);
      if (spuResult != ADI_SPU_SUCCESS)
      {
         /*printf("Error: Failed to enable Master secure for EMDMA\n");*/
         return -1;
      }
   }
   {
      /* Initialize the GUL EMDMA streams for usage by Simulink delay
       * (stream 1) and accelerator (stream 0) blocks */
      gMdmaResult_t gMdmaResult = gMdmaOpen( GMDMA_STREAM_0 );
      gMdmaResult |= gMdmaOpen( GMDMA_STREAM_1 );
      if (GMDMA_SUCCESS != gMdmaResult)
      {
         /* printf("Error: Failure to open EMDMA streams\n"); */
         return -1;
      }

      /* Initialize the GUL HW Accelerator Manager framework. Note, must be
       * called prior to calling accelerator configuration APIs. */
      gHamResult_t gHamResult = gHamInitializeAccelerators();
      if (GHAM_SUCCESS != gHamResult)
      {
         /* printf("Error: Failure to initalize Accelerators\n"); */
         return -1;
      }
   }
   return 0;
}
