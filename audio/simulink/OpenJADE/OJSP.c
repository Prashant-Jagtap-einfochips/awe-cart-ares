#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

#if defined(__hexagon__)
#include <hexagon_sim_timer.h>
#endif

#if defined(__QNXNTO__)
#include <sys/neutrino.h>
#endif

#if defined(__RT685_HIFI4__)
#include <hifi4_ccount.h>
#endif

#if defined(__HIFI4_SIMULATOR__)
#include <hifi4sim_ccount.h>
#endif

#if defined(__QCC7226_HIFI4__)
#include <qcc7226_ccount.h>
#endif

#if defined(__AB1585_HIFI5__)
#include <hifi5_ccount.h>
#endif

#include "BAF_Message_Id.h"
#include "BAF.h"
#include "OJSP.h"
#include "heapstack_usage.h"

#if defined(SPC1)
   #define SPC "SPC1> "
#elif defined(SPC2)
   #define SPC "SPC2> "
#else
   #define SPC ""
#endif

#define _DERR(x) x
#define _DERR0(x) //x
#define _D(x) x
#define PRINTF platform_printf

#if defined(__ADSPSHARC__)
#define SEG_SDRAM_DATA _Pragma ("section(\"seg_sdram\", DOUBLEANY)" )
#else
#define SEG_SDRAM_DATA
#endif

#define HEAP_SIZE 0x4000
uint32_t heap_buffer[HEAP_SIZE / sizeof(uint32_t)];

#if defined(BAF_LOGGER) && defined(BAF_STATSLOGGER)
void OJSP_Configure_Stats(void);
#endif

#if defined(BAF_LOGGER)
SEG_SDRAM_DATA uint32_t heap_log_buffer[BAF_LOGGER_LOG_SIZE / sizeof(uint32_t)];
#endif

uint32_t req_Talaria[80] ;
uint32_t rsp_Talaria[80] ;

typedef struct OJSP_Config
{
   BAF *baf;
#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
   Platform_Handle_t hPlatform;
   uint32_t audio_frame_cnt[PLATFORM_NUM_AUDIOTHREADS];
#else
   uint32_t audio_frame_cnt;
#endif
}  OJSP_Config_t;

OJSP_Config_t g_ojspConfig;

void OJSP_notification(BAF *baf, BAF_Message *message) {
   Platform_control_msg notification_msg;

   notification_msg.id = message->id;
   notification_msg.size = message->size;
   notification_msg.data = message->data;

   platform_send_notification(&notification_msg);
}

int32_t OJSP_control_service_message(Platform_control_msg *p_request_message,
                                  Platform_control_msg *p_response_message,
                                  void *arg) {
   int32_t status;
   OJSP_Config_t *ojsp = (OJSP_Config_t *)arg;
   BAF *baf = ojsp->baf;

   BAF_Message baf_message_req;
   BAF_Message baf_message_rsp;

   baf_message_req.id = p_request_message->id;
   baf_message_req.size = p_request_message->size;
   baf_message_req.data = p_request_message->data;

   baf_message_rsp.size = p_response_message->size;
   baf_message_rsp.data = p_response_message->data;

   status = (int32_t)BAF_command(baf, &baf_message_req, &baf_message_rsp);

   p_response_message->id = baf_message_rsp.id;
   p_response_message->size = baf_message_rsp.size;
   p_response_message->data = baf_message_rsp.data;

   return status;
}

void OJSP_background_task(void *arg)
{
}

int32_t OJSP_notification_poll(void *arg) {
   /* Poll for notifications */
   int32_t status;
   OJSP_Config_t *ojsp = (OJSP_Config_t *)arg;
   BAF *baf = ojsp->baf;

   BAF_Message baf_message_req;
   BAF_Message baf_message_rsp;

   baf_message_req.id = BAF_MESSAGE_NOTIFICATION_POLL;
   baf_message_req.data = NULL;
   baf_message_req.size = 0;
   baf_message_rsp.data = rsp_Talaria;
   baf_message_rsp.size = sizeof(rsp_Talaria);

   status = (int32_t)BAF_command(baf, &baf_message_req, &baf_message_rsp); // poll
   return status;
}

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
void OJSP_audio_thread_handler(void *arg, uint32_t thread_level)
{
   OJSP_Config_t *ojsp = (OJSP_Config_t *)arg;
   ojsp->audio_frame_cnt[thread_level]++;
   BAF_exec(ojsp->baf, thread_level, arg);
}

void BAF_Callback_post(uint32_t priority, void* arg) {
   OJSP_Config_t *ojsp = (OJSP_Config_t *)arg;
   platform_audio_thread_post(ojsp->hPlatform, priority);
}
#elif defined(PLATFORM_BASERATE_AUDIOTHREAD)
void OJSP_audio_service_tick(void *arg) {
   OJSP_Config_t *ojsp = (OJSP_Config_t *)arg;
   ojsp->audio_frame_cnt++;
   BAF_exec(ojsp->baf, 0, NULL);
}

void BAF_Callback_post(uint32_t priority, void* arg) {
   BAF_exec(g_ojspConfig.baf, priority, NULL);
}
#endif /*defined(PLATFORM_BASERATE_AUDIOTHREAD)*/


void BAF_Callback_sys_process(uint32_t proc_Id, uint32_t frame_size,
      uint32_t num_chan_in, uint32_t num_chan_out, void *src, void *dst) {
   uint32_t num_chan;

   switch (proc_Id) {
   case 0:
      num_chan = platform_rcv_input_buffer((float *) dst, num_chan_out, frame_size);
      if (num_chan != num_chan_out)
      {
         _DERR0(PRINTF(SPC "Error: platform_rcv_input_buffer num_chan exp %lu got %lu\n",num_chan_out, num_chan));
      }
      break;
   case 1:
      num_chan = platform_xmt_output_buffer((float *) src, num_chan_in, frame_size);
      if (num_chan != num_chan_in)
      {
         _DERR0(PRINTF(SPC "Error: platform_xmt_output_buffer num_chan exp %lu got %lu\n",num_chan_in, num_chan));
      }
   break;
   case 2:
      num_chan = platform_rcv_input_buffer((float *) dst, num_chan_out, frame_size);
      if (num_chan != num_chan_out)
      {
         _DERR0(PRINTF(SPC "Error: platform_rcv_input_buffer num_chan exp %lu got %lu\n",num_chan_out, num_chan));
      }
      break;
   case 3:
      num_chan = platform_xmt_output_buffer((float *) src, num_chan_in, frame_size);
      if (num_chan != num_chan_in)
      {
         _DERR0(PRINTF(SPC "Error: platform_xmt_output_buffer num_chan exp %lu got %lu\n",num_chan_in, num_chan));
      }
      break;
   default:
      _DERR(PRINTF(SPC "Error: BAF_Callback_sys_process unexpected SYS call %lu\n",proc_Id));
   }
}


void BAF_Callback_sys_process_ctx(uint32_t proc_Id, uint32_t frame_size,
      uint32_t num_chan_in, uint32_t num_chan_out, void *src, void *dst, void* arg) {
   uint32_t num_chan;

   switch (proc_Id) {
   case 0:
      num_chan = platform_rcv_input_buffer((float *) dst, num_chan_out, frame_size);
      if (num_chan != num_chan_out)
      {
         _DERR0(PRINTF(SPC "Error: platform_rcv_input_buffer num_chan exp %lu got %lu\n",num_chan_out, num_chan));
      }
      break;
   case 1:
      num_chan = platform_xmt_output_buffer((float *) src, num_chan_in, frame_size);
      if (num_chan != num_chan_in)
      {
         _DERR0(PRINTF(SPC "Error: platform_xmt_output_buffer num_chan exp %lu got %lu\n",num_chan_in, num_chan));
      }
   break;
   case 2:
      num_chan = platform_rcv_input_buffer((float *) dst, num_chan_out, frame_size);
      if (num_chan != num_chan_out)
      {
         _DERR0(PRINTF(SPC "Error: platform_rcv_input_buffer num_chan exp %lu got %lu\n",num_chan_out, num_chan));
      }
      break;
   case 3:
      num_chan = platform_xmt_output_buffer((float *) src, num_chan_in, frame_size);
      if (num_chan != num_chan_in)
      {
         _DERR0(PRINTF(SPC "Error: platform_xmt_output_buffer num_chan exp %lu got %lu\n",num_chan_in, num_chan));
      }
      break;
   default:
      _DERR(PRINTF(SPC "Error: BAF_Callback_sys_process unexpected SYS call %lu\n",proc_Id));
   }
}


#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
uint32_t BAF_Callback_clock_cnt = 0;
#endif

uint32_t BAF_Callback_clock(void) {
#if defined(__ADSPSHARC__)
   return __builtin_emuclk();
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
   BAF_Callback_clock_cnt++;
   return BAF_Callback_clock_cnt;
#elif defined(__hexagon__)
   return (uint32_t)hexagon_sim_read_pcycles();
#elif defined(__QNXNTO__)
   return (uint32_t)ClockCycles();
#elif defined(__RT685_HIFI4__)
   return (uint32_t)hifi4_get_ccount();
#elif defined(__HIFI4_SIMULATOR__)
   return (uint32_t)hifi4sim_get_ccount();
#elif defined(__QCC7226_HIFI4__)
    return (uint32_t)qcc7226_get_ccount();
#elif defined(__AB1585_HIFI5__)
   return (uint32_t)hifi5_get_ccount();
#else
   return 0;
#endif
}

void BAF_Callback_post_mask(uint32_t exec_mask, void* arg)
{

}

int BAF_printf(char* fmt, ...)
{
   va_list args;
   int val;

   va_start( args, fmt );
   val = platform_vprintf(fmt, args );
   va_end(args);
   return val;
}

void *OJSP_framework_init(void) {
   BAF *baf;
   BAF_Resource *resource;
   int32_t error;

   baf = BAF_create(NULL);

   resource = BAF_resource(baf);

   if (resource->memory_size < HEAP_SIZE) {
      resource->memory_pointer = heap_buffer;
#if defined(BAF_LOGGER)
      resource->memory_log_pointer = heap_log_buffer;
      resource->memory_log_size  = BAF_LOGGER_LOG_SIZE;
      resource->print_log_size   = BAF_LOGGER_PRINTLOG_SIZE;
      resource->capture_log_size = BAF_LOGGER_CAPTURELOG_SIZE;
      resource->exec_log_size    = BAF_LOGGER_EXECLOG_SIZE;
#endif
      resource->base_thread = PLATFORM_BASETHREADLEVEL;
      resource->ext_scheduler = FRAMEWORK_EXT_SCHEDULER;
   } else {
      _DERR(PRINTF(SPC "Error: OJSP_init not enough memory req %d supplied %d\n", resource->memory_size, HEAP_SIZE));
      return (void *)NULL;
   }

   error = BAF_init(baf);
   if (error) {
      _DERR(PRINTF(SPC "Error: OJSP_init BAF_init failure %08lX\n", error));
      return (void *)NULL;
   }

   _D(PRINTF(SPC "BAF Version %s\n", BAF_version(baf)));

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
   {
      int i;
      for (i=0; i<PLATFORM_NUM_AUDIOTHREADS; i++)
         g_ojspConfig.audio_frame_cnt[i] = 0;
      g_ojspConfig.hPlatform = platform_get_handle();
   }
#elif defined(PLATFORM_BASERATE_AUDIOTHREAD)
   g_ojspConfig.audio_frame_cnt = 0;
#endif
   g_ojspConfig.baf = baf;

   BAF_register_notify(baf, OJSP_notification);

#if defined(BAF_LOGGER) && defined(BAF_STATSLOGGER)
   OJSP_Configure_Stats();
#endif

   return (void *)&g_ojspConfig;
}

size_t OJSP_heap_log_buffer_size(void)
{
#if defined(BAF_LOGGER)
  return (size_t)(BAF_LOGGER_LOG_SIZE);
#else
  return 0;
#endif
}

void *OJSP_heap_log_buffer_addr(void)
{
#if defined(BAF_LOGGER)
  return (void *)&heap_log_buffer[0];
#else
  return (void*) NULL;
#endif
}


