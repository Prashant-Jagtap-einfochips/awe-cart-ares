/*******************************************************************************
 *  BOSE CORPORATION
 *  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 *  This program may not be reproduced, in whole or in part in any
 *  form or any means whatsoever without the written permission of:
 *     BOSE CORPORATION
 *     The Mountain
 *     Framingham, MA 01701-9168
 ********************************************************************************
 *  Title:        BAF Callback implementation for AWE module.
 *  Revision:      -
 *  Description:   -
 *
 *  References:    -
 *
 ********************************************************************************
 *  $Log: BAF_Callbacks.c $
 *
 *******************************************************************************/
#include "Framework.h"
#include "BAF.h"
#include "ModTestAudio.h"

void BAF_Callback_sys_process_ctx(uint32_t proc_Id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void *src, void *dst, void *arg)
{
    awe_modTestAudioInstance *S = (awe_modTestAudioInstance*)arg;
    WireInstance **pWires = ClassModule_GetWires(S);
    UINT32 numInPins = ClassModule_GetNInWires(S);
    FLOAT32 *ptrSrc = (FLOAT32 *)(pWires[0]->buffer);
    FLOAT32 *ptrDst = (FLOAT32 *)(pWires[numInPins]->buffer);
    FLOAT32 *ptrSrcBaf = (FLOAT32 *)src;
    FLOAT32 *ptrDstBaf = (FLOAT32 *)dst;
    UINT32 numChannelsIn = ClassWire_GetChannelCount(pWires[0]);
    UINT32 numChannelsOut = ClassWire_GetChannelCount(pWires[numInPins]);
    UINT32 blockSize = ClassWire_GetBlockSize(pWires[0]);
    UINT32 i = 0;
    UINT32 j = 0;

    if (proc_Id == 0)
    {
        /* Audio input channels */
        for (i = 0; i < numChannelsIn; i++)
        {
           FLOAT32 *pSrc = (ptrSrc + i);
           FLOAT32 *pDst = ptrDstBaf + (i * blockSize);
           for (j = 0; j < blockSize; j++)
           {
              *pDst = *pSrc;
              pSrc += numChannelsIn;
              pDst += 1;
           }
        }
    }
    else if (proc_Id == 1)
    {
        /* Audio output channels */
        for (i = 0; i < numChannelsOut; i++)
        {
           FLOAT32 *pSrc = ptrSrcBaf + (i * blockSize);
           FLOAT32 *pDst = (ptrDst + i);
           for (j = 0; j < blockSize; j++)
           {
              *pDst = *pSrc;
              pSrc += 1;
              pDst += numChannelsOut;
           }
        }
    }
}

int BAF_printf(const char *format, ...)
{
    return 0;
}

void BAF_Callback_post(uint32_t priority, void *arg)
{

}

void BAF_Callback_post_mask(uint32_t exec_mask, void* arg)
{

}

uint32_t BAF_Callback_clock(void)
{
    return 0;
}

uint32_t BAF_Callback_Stats(uint32_t *log, uint32_t len, uint32_t start_stop)
{
   return 0;
}

