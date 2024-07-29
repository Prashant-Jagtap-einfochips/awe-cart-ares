/*******************************************************************************
 *  BOSE CORPORATION
 *  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 *  This program may not be reproduced, in whole or in part in any
 *  form or any means whatsoever without the written permission of:
 *     BOSE CORPORATION
 *     The Mountain
 *     Framingham, MA 01701-9168
 *******************************************************************************/
#include "OJSP.h"

int main(int argc, char*argv[])
{
   Platform_Handle_t hPlatform;

   platform_init();

   hPlatform = platform_get_handle();

#ifndef QNA_PIL_SHARED_LIBRARY
   
   platform_register_framework_init(hPlatform, OJSP_framework_init);
#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
   platform_register_audio_thread_handler(hPlatform, OJSP_audio_thread_handler); 
#elif defined(PLATFORM_BASERATE_AUDIOTHREAD)
   platform_register_audio_service_tick(hPlatform, OJSP_audio_service_tick); 
#endif
   platform_register_control_service_message(hPlatform, OJSP_control_service_message);
   platform_register_notification_poll(hPlatform, OJSP_notification_poll);
   platform_add_background_task(hPlatform, OJSP_background_task);
   
#endif  // QNA_PIL_SHARED_LIBRARY

   platform_main(hPlatform, argc, argv);

   platform_close(hPlatform);

   return 0;
}

