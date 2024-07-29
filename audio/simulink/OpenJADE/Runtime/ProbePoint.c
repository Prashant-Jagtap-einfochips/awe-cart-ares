/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
*******************************************************************************/
/****************************************************
 * @file Propoint.c
 *
 * @brief Implementation of the Propoint.c - support of component level profiling
 *
 * @author: cb1037694
 *
 * OJProbePoint Bitfield Format
 *   <reserved:8-bit><thread_level:8-bit><process_id:8-bit><start_stop:8-bit>
 *   reserved 0x00

 *   start_stop  either (0-start) or (1-stop)
 * sample propoints
 *    probepoint_fg_begin   = 0x00018000;
 *    probepoint_fg_end     = 0x00018001;
 *    probepoint_hf0_begin  = 0x00018100;
 *    probepoint_hf0_end    = 0x00018101;
 *    probepoint_hf1_begin  = 0x00018200;
 *    probepoint_hf1_end    = 0x00018201;
 *    probepoint_avas_begin = 0x00018300;
 *    probepoint_avas_end   = 0x00018301;
 *
 *    probepoint_eor_begin  = 0x00008000;
 *    probepoint_eor_end    = 0x00008001;
 *    probepoint_eoe_begin  = 0x00008100;
 *    probepoint_eoe_end    = 0x00008101;
 *    probepoint_icc_begin  = 0x00008200;
 *    probepoint_icc_end    = 0x00008201;
 *
 ****************************************************/

#include "Logger.h"
#include "BAF_Callback.h"


#if defined(BAF_LOGGER)

void probe_point(uint32_t id)
{
   uint32_t start_stop = (id >> 0) & 0xFF;
   uint32_t process_id = (id >> 8) & 0xFF;
   uint32_t thread_level = (id >> 16) & 0xFF;

   if (start_stop == 0)
   {
#if defined(BAF_STATSLOGGER)
		Logger_stats(LOG_TYPE_CAPTURE, thread_level, process_id, 0);
#else
		Logger_print(LOG_TYPE_EXEC, thread_level, "Bgn:%d t:%10u\n", process_id, BAF_Callback_clock());
#endif /*defined(BAF_STATSLOGGER)*/
   }
   else
   {
#if defined(BAF_STATSLOGGER)
		Logger_stats(LOG_TYPE_CAPTURE, thread_level, process_id, 1);
#else
		Logger_print(LOG_TYPE_EXEC, thread_level, "End:%d t:%10u\n", process_id, BAF_Callback_clock());
#endif /*defined(BAF_STATSLOGGER)*/
   }
}

#endif
