#include <stdio.h>
#include <stdint.h>
#include "oj_platform.h"
#include "Message_Id.h"
#include "ram_file.h"
#include "output_stats.h"

#if !defined(FRAME_IDX_MAX)
#define FRAME_IDX_MAX (20)
#endif


#define CONTROL_LINE_SIZE  (8192)
uint8_t g_control_hex[CONTROL_LINE_SIZE];
uint32_t g_request_buff[CONTROL_LINE_SIZE/8];
uint32_t g_response_buff[CONTROL_LINE_SIZE/8];

/* BAF thread levels */
#define PLATFORM_TLVL_0       (0)
#define PLATFORM_TLVL_1       (1)
#define PLATFORM_TLVL_2       (2)
#define PLATFORM_TLVL_3       (3)
#define PLATFORM_TLVL_4       (4)
#define PLATFORM_TLVL_5       (5)
#define PLATFORM_TLVL_6       (6)
#define PLATFORM_TLVL_7       (7)
#define PLATFORM_TLVL_8       (8)
#define PLATFORM_TLVL_9       (9)
#define PLATFORM_TLVL_10      (10)
#define PLATFORM_TLVL_11      (11)

typedef struct Platform_Thread
{
   Platform_Handle_t hPlatform;
   uint32_t thread_level;
}  Platform_Thread_t;

typedef struct Platform_Config
{
   /* Callback functions */
   Platform_Func_framework_init             framework_init;

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
   Platform_Func_audio_thread_handler       audio_thread_handler;
   Platform_Thread_t                        thread[PLATFORM_NUM_AUDIOTHREADS];
#elif defined(PLATFORM_BASERATE_AUDIOTHREAD)
   Platform_Func_audio_service_tick         audio_service_tick;
#endif

   Platform_Func_control_service_message    control_service_message;
   Platform_Func_notification_poll          notification_poll;

   Platform_Func_background_task            background_tasks[PLATFORM_NUM_BACKGROUND_TASKS];

   /* Callback param */
   void   *callback_param;

   /* Control File */
   char *control_file_name;
   FILE *control_file;
   uint32_t control_cnt;
   uint8_t *control_hex_buff;
   uint32_t control_frame_idx;

   uint32_t *request_msg_buff;
   uint32_t *response_msg_buff;

   /* Control Response File */
   FILE *response_file;

   /* Frame Count */
   uint32_t audio_frame_cnt;
   uint32_t audio_frame_max;

   /* In/Out Files */
   char *audio_in_file_name;
   char *audio_out_file_name;
   char *audio_gold_file_name;
   FILE *audio_in_file;
   FILE *audio_out_file;
   FILE *audio_gold_file;

}  Platform_Config_t;

Platform_Config_t g_platfromConfig;

void platform_register_framework_init(Platform_Handle_t hPlatform, Platform_Func_framework_init framework_init)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->framework_init = framework_init;
}

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
void platform_register_audio_thread_handler(Platform_Handle_t hPlatform, Platform_Func_audio_thread_handler audio_thread_handler)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->audio_thread_handler = audio_thread_handler;
}
#elif defined(PLATFORM_BASERATE_AUDIOTHREAD)
void platform_register_audio_service_tick(Platform_Handle_t hPlatform, Platform_Func_audio_service_tick audio_service_tick)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->audio_service_tick = audio_service_tick;
}
#endif

void platform_register_control_service_message(Platform_Handle_t hPlatform, Platform_Func_control_service_message control_service_message)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->control_service_message = control_service_message;
}

void platform_register_notification_poll(Platform_Handle_t hPlatform, Platform_Func_notification_poll notification_poll)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->notification_poll = notification_poll;
}

int platform_add_background_task(Platform_Handle_t hPlatform, Platform_Func_background_task background_task)
{
   int i;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   for (i=0; i<PLATFORM_NUM_BACKGROUND_TASKS; i++)
   {
      if (pPlatform->background_tasks[i] == (Platform_Func_background_task)NULL)
      {
         pPlatform->background_tasks[i] = background_task;
         return 1;
      }
   }
   return 0;
}

int platform_remove_background_task(Platform_Handle_t hPlatform, Platform_Func_background_task background_task)
{
   int i;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   for (i=0; i<PLATFORM_NUM_BACKGROUND_TASKS; i++)
   {
      if (pPlatform->background_tasks[i] == background_task)
      {
         pPlatform->background_tasks[i] = (Platform_Func_background_task)NULL;
         return 1;
      }
   }
   return 0;
}

void platform_clear_background_tasks(Platform_Handle_t hPlatform)
{
   int i;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   for (i=0; i<PLATFORM_NUM_BACKGROUND_TASKS; i++)
   {
      pPlatform->background_tasks[i] = (Platform_Func_background_task)NULL;
   }
}

int platform_vprintf (const char * format, va_list arg )
{
   return vprintf ( format, arg );
}

int platform_printf(char* format, ...)
{
   va_list args;
   int val;
   va_start( args, format );
   val = vprintf(format, args );
   va_end(args);
   return val;
}

void xmit_control_service_request(FILE * fp, uint32_t cnt, uint32_t *buff)
{
   uint32_t idx;

   if (fp == NULL)
      return;

   fprintf(fp, "REQ:");
   for(idx = 0; idx < cnt; idx++)
   {
      fprintf(fp, " %8.8X", *buff++);
   }
   fprintf(fp, "\n");

   return;
}

void xmit_control_service_response(FILE *fp, char *service, uint32_t msg_id, uint32_t cnt, uint8_t *buff)
{
   uint32_t idx;

   if (fp == NULL)
      return;

   fprintf(fp, "%s: %8.8X %8.8x", service, msg_id, cnt);
   for(idx = 0; idx < cnt; idx++)
   {
      fprintf(fp, " %2.2X", *buff++);
   }
   fprintf(fp, "\n");

   return;
}

void xmit_control_service_error(FILE *fp, char *service, uint32_t msg_id, int32_t status)
{
   if (fp == NULL)
      return;

   fprintf(fp, "%s: %8.8X %8.8x\n", service, msg_id, status);

   return;
}


void platform_send_notification(Platform_control_msg *message)
{
   xmit_control_service_response(g_platfromConfig.response_file,
                                 "NTF",
                                 message->id,
                                 message->size,
                                 (uint8_t *)(message->data));
}

uint32_t platform_rcv_input_buffer(float *buff, uint32_t num_chan, uint32_t frame_size)
{
   FILE *fp = g_platfromConfig.audio_in_file;

   uint32_t cnt = fread(buff, sizeof(float), num_chan*frame_size, fp);

   if (cnt != num_chan*frame_size)
      platform_printf("Error: platform_rcv_input_buffer got %d exp % d\n", cnt, num_chan*frame_size);

   return num_chan;
}

uint32_t platform_xmt_output_buffer(float *buff, uint32_t num_chan, uint32_t frame_size)
{
   FILE *fpOut = g_platfromConfig.audio_out_file;
#if defined(CALCULATE_OUTPUT_STATS)
   FILE *fpRef = g_platfromConfig.audio_gold_file;
#endif
   uint32_t cnt = fwrite(buff, sizeof(float), num_chan*frame_size, fpOut);

   if (cnt != num_chan*frame_size)
      platform_printf("Error: platform_xmt_output_buffer got %d exp % d\n", cnt, num_chan*frame_size);

#if defined(CALCULATE_OUTPUT_STATS)
   update_output_stats(fpRef, buff, num_chan, frame_size);
#endif

   return num_chan;
}

#define INVALID_HEX_CHAR 0x100

static uint32_t hex2bin(uint8_t ch) {
   if (ch >= '0' && ch <= '9')
      return (ch - '0');
   if (ch >= 'A' && ch <= 'F')
      return (10 + ch - 'A');
   if (ch >= 'a' && ch <= 'f')
      return (10 + ch - 'a');
   return INVALID_HEX_CHAR;
}

uint32_t read_control_line(FILE *fp, uint8_t *hex_buff, uint32_t *bin_buff)
{
   uint32_t ridx, widx, cnt;
   uint8_t buff[8];
   uint32_t ch;

   fgets((char *)hex_buff, CONTROL_LINE_SIZE, fp);

   ridx = 0;
   widx = 0;
   cnt = 0;

   if (hex_buff[0] == '#')
      return 0;

   while(hex_buff[ridx] != '\n')
   {
      ch = hex2bin(hex_buff[ridx]);

      if (ch != INVALID_HEX_CHAR)
      {
         buff[cnt] = (ch & 0xF);
         cnt++;
         if (cnt == 8)
         {
            uint32_t d0, d1, d2, d3;

            d0 = (buff[0] << 4) | buff[1];
            d1 = (buff[2] << 4) | buff[3];
            d2 = (buff[4] << 4) | buff[5];
            d3 = (buff[6] << 4) | buff[7];

            bin_buff[widx] = (d3 << 24) | (d2 << 16) | (d1 << 8) | d0;

            cnt = 0;

            widx++;
         }
      }
      ridx++;
   }

   return widx;
}

int set_next_control_frame(Platform_Config_t *pPlatform)
{
   uint32_t cnt;

   pPlatform->control_cnt = 0;
   pPlatform->control_frame_idx = pPlatform->audio_frame_max;

   while (1)
   {
      if (feof(pPlatform->control_file))
      {
         return -1;
      }
      else
      {
         cnt = read_control_line(pPlatform->control_file,
                              pPlatform->control_hex_buff,
                              pPlatform->request_msg_buff);
         if (cnt >= 2)
         {
            pPlatform->control_cnt = cnt;
            pPlatform->control_frame_idx = pPlatform->request_msg_buff[0];
            return 0;
         }
      }
   }
}


void platform_poll_control_service_message(Platform_Config_t *pPlatform)
{
   Platform_control_msg req_msg;
   Platform_control_msg rsp_msg;
   int32_t status;

   while (pPlatform->control_frame_idx == pPlatform->audio_frame_cnt)
   {
      if (pPlatform->control_cnt >= 2)
      {
         req_msg.id = pPlatform->request_msg_buff[1];
         req_msg.size = (pPlatform->control_cnt - 2) * sizeof(uint32_t);
         req_msg.data = &(pPlatform->request_msg_buff[2]);

         xmit_control_service_request(pPlatform->response_file,
                                      pPlatform->control_cnt - 1,
                                      &(pPlatform->request_msg_buff[1]));

         if ((req_msg.id & 0x000000FF) == MESSAGE_SYS_NUM_FRAMES)
         {
            pPlatform->audio_frame_max = (req_msg.id >> 8);
         }
         else
         {
            rsp_msg.id = req_msg.id;
            rsp_msg.size = (CONTROL_LINE_SIZE/8) * sizeof(uint32_t);
            rsp_msg.data = &(pPlatform->response_msg_buff[0]);

            status = pPlatform->control_service_message(&req_msg, &rsp_msg, pPlatform->callback_param);

            if (status < 0)
            {
               xmit_control_service_error(pPlatform->response_file,
                                          "RSP",
                                          rsp_msg.id,
                                          status);
            }
            else
            {
               xmit_control_service_response(pPlatform->response_file,
                                             "RSP",
                                             rsp_msg.id,
                                             rsp_msg.size,
                                             (uint8_t *)(rsp_msg.data));
            }
         }
      }
      if (set_next_control_frame(pPlatform) == -1)
         break;
   }
}

void platform_print_usage(int argc, char *argv[])
{
   platform_printf("Usage: %s [-h] -c <control file> -i <input file> -g <reference file> -o <output file>\n", argv[0]);
   return;
}

int set_run_parameters(Platform_Config_t *pPlatform, int argc, char *argv[])
{
   int idx;
   pPlatform->control_file = NULL;
   pPlatform->response_file = stdout;
   pPlatform->audio_in_file = NULL;
   pPlatform->audio_out_file = NULL;
   pPlatform->audio_gold_file = NULL;

#if defined(ARGC_ARGV_SUPPORTED)
   pPlatform->control_file_name = NULL;
   pPlatform->audio_in_file_name = NULL;
   pPlatform->audio_out_file_name = NULL;
   pPlatform->audio_gold_file_name = NULL;
#else
   pPlatform->control_file_name = "request.ojx";
   pPlatform->audio_in_file_name = "audioIn.bin";
   pPlatform->audio_out_file_name = "audioOut.bin";
   pPlatform->audio_gold_file_name = "audioRef.bin";
#endif

#if defined(ARGC_ARGV_SUPPORTED)
   if (argc > 1)
   {
      idx = 1;
      while (idx < argc)
      {
         if ((argv[idx][0] == '-') && (argv[idx][1] == 'h'))
         {
            platform_print_usage(argc, argv);
            return -1;
         }
         else if ((argv[idx][0] == '-') && (idx+1 < argc))
         {
            if (argv[idx][1] == 'c')
            {
               pPlatform->control_file_name = argv[idx+1];
               idx++;
            }
            else if (argv[idx][1] == 'i')
            {
               pPlatform->audio_in_file_name = argv[idx+1];
               idx++;
            }
            else if (argv[idx][1] == 'o')
            {
               pPlatform->audio_out_file_name = argv[idx+1];
               idx++;
            }
            else if (argv[idx][1] == 'g')
            {
               pPlatform->audio_gold_file_name = argv[idx+1];
               idx++;
            }
         }
         idx++;
      }
   }
   else
   {
      platform_print_usage(argc, argv);
      return -1;
   }
#endif /*defined(ARGC_ARGV_SUPPORTED)*/
   if (pPlatform->control_file_name != NULL)
   {
      platform_printf("Info: control file name - %s\n", pPlatform->control_file_name);
      pPlatform->control_file = fopen(pPlatform->control_file_name, "r");
      if (pPlatform->control_file == NULL)
      {
         platform_printf("Error: opening control file %s failed\n", pPlatform->control_file_name);
         return -1;
      }
   }

   if (pPlatform->audio_in_file_name != NULL)
   {
      platform_printf("Info: audio input file name - %s\n", pPlatform->audio_in_file_name);
      pPlatform->audio_in_file = fopen(pPlatform->audio_in_file_name, "rb");
      if (pPlatform->audio_in_file == NULL)
      {
         platform_printf("Error: opening audio input file %s failed\n", pPlatform->audio_in_file_name);
         return -1;
      }
   }

   if (pPlatform->audio_out_file_name != NULL)
   {
      platform_printf("Info: audio output file name - %s\n", pPlatform->audio_out_file_name);
      pPlatform->audio_out_file = fopen(pPlatform->audio_out_file_name, "wb");
      if (pPlatform->audio_out_file == NULL)
      {
         platform_printf("Error: opening audio output file %s failed\n", pPlatform->audio_out_file_name);
         return -1;
      }
      ramio_files_shred(pPlatform->audio_out_file_name);
   }

   if (pPlatform->audio_gold_file_name != NULL)
   {
      platform_printf("Info: audio reference file name - %s\n", pPlatform->audio_gold_file_name);
      pPlatform->audio_gold_file = fopen(pPlatform->audio_gold_file_name, "rb");
      if (pPlatform->audio_gold_file == NULL)
      {
         platform_printf("Error: opening audio reference file %s failed\n", pPlatform->audio_gold_file_name);
         return -1;
      }
   }

   pPlatform->control_hex_buff = g_control_hex;
   pPlatform->request_msg_buff = g_request_buff;
   pPlatform->response_msg_buff = g_response_buff;

   return 0;
}


#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
void platform_audio_thread_post(Platform_Handle_t hPlatform, uint32_t priority)
{
   uint32_t cnt = 0;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;

   for (cnt = 0; cnt < PLATFORM_NUM_AUDIOTHREADS; cnt++)
   {
      if (pPlatform->thread[cnt].thread_level == priority)
      {
         pPlatform->audio_thread_handler(pPlatform->callback_param, priority);
      }
   }
}

static void platform_threads_init(Platform_Handle_t hPlatform)
{
   int32_t cnt;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;

   pPlatform->thread[0].thread_level = PLATFORM_TLVL_0;
   pPlatform->thread[1].thread_level = PLATFORM_TLVL_1;
   pPlatform->thread[2].thread_level = PLATFORM_TLVL_2;
   pPlatform->thread[3].thread_level = PLATFORM_TLVL_3;
   pPlatform->thread[4].thread_level = PLATFORM_TLVL_4;
   pPlatform->thread[5].thread_level = PLATFORM_TLVL_5;
   pPlatform->thread[6].thread_level = PLATFORM_TLVL_6;
   pPlatform->thread[7].thread_level = PLATFORM_TLVL_7;
   pPlatform->thread[8].thread_level = PLATFORM_TLVL_8;
   pPlatform->thread[9].thread_level = PLATFORM_TLVL_9;
   pPlatform->thread[10].thread_level = PLATFORM_TLVL_10;
   pPlatform->thread[11].thread_level = PLATFORM_TLVL_11;

   /* Install software interrupt handler and set their priority */
   for (cnt = 0; cnt < PLATFORM_NUM_AUDIOTHREADS; cnt++)
   {
      pPlatform->thread[cnt].hPlatform = hPlatform;
   }
}
#endif

void platform_init(void)
{
   platform_clear_background_tasks((Platform_Handle_t)&g_platfromConfig);

   ramio_files_init();
}

Platform_Handle_t platform_get_handle(void)
{
   return (Platform_Handle_t)&g_platfromConfig;
}

void platform_main(Platform_Handle_t hPlatform, int argc, char *argv[])
{
   int i;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;

   if (pPlatform == NULL)
   {
      platform_printf("Error: NULL Platform handle\n");
      return;
   }

   if (set_run_parameters(pPlatform, argc, argv) != 0)
      return;

   pPlatform->callback_param = pPlatform->framework_init();

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
   platform_threads_init(hPlatform);
#endif

   pPlatform->audio_frame_cnt = 0;
   pPlatform->audio_frame_max = FRAME_IDX_MAX;

   set_next_control_frame(pPlatform);

#if defined(CALCULATE_OUTPUT_STATS)
   init_output_stats();
#endif

   while (pPlatform->audio_frame_cnt < pPlatform->audio_frame_max)
   {
      /*platform_printf("Info: audio_frame_cnt %d\n", pPlatform->audio_frame_cnt);*/
      platform_poll_control_service_message(pPlatform);

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
      pPlatform->audio_thread_handler(pPlatform->callback_param, pPlatform->thread[PLATFORM_BASETHREADLEVEL].thread_level);
#else
      pPlatform->audio_service_tick(pPlatform->callback_param);
#endif


      pPlatform->notification_poll(pPlatform->callback_param);

      for (i=0; i<PLATFORM_NUM_BACKGROUND_TASKS; i++)
      {
         if (pPlatform->background_tasks[i] != (Platform_Func_background_task)NULL)
         {
            pPlatform->background_tasks[i](pPlatform->callback_param);
         }
      }
      pPlatform->audio_frame_cnt++;
   }

   platform_poll_control_service_message(pPlatform);
#if defined(CALCULATE_OUTPUT_STATS)
   print_output_stats(pPlatform->response_file);
#endif
}

void platform_close(Platform_Handle_t hPlatform)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   if (pPlatform != NULL)
   {
      if (pPlatform->control_file != NULL)
      {
         fclose(pPlatform->control_file);
      }
      if (pPlatform->audio_in_file != NULL)
      {
         fclose(pPlatform->audio_in_file);
      }
      if (pPlatform->audio_out_file != NULL)
      {
         fclose(pPlatform->audio_out_file);
      }
      if (pPlatform->audio_gold_file != NULL)
      {
         fclose(pPlatform->audio_gold_file);
      }
      ramio_files_close();
   }
}

