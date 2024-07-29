#include <string.h>
#include <sys/platform.h>
#include <sys/adi_core.h>
#include <drivers/sport/adi_sport.h>
#include <services/int/adi_int.h>
#include <services/int/adi_sec.h>

#include "oj_platform.h"
#include "Message_Id.h"
#include "ram_file.h"
#include "output_stats.h"
#include "sc584_ezkit.h"

#if !defined(FRAME_IDX_MAX)
#define FRAME_IDX_MAX (20)
#endif
#define SEG_SDRAM_DATA _Pragma ("section(\"seg_sdram\", DOUBLEANY)" )

#define CONTROL_LINE_SIZE  (8192)
SEG_SDRAM_DATA uint8_t g_control_hex[CONTROL_LINE_SIZE];
SEG_SDRAM_DATA uint32_t g_request_buff[CONTROL_LINE_SIZE/8];
SEG_SDRAM_DATA uint32_t g_response_buff[CONTROL_LINE_SIZE/8];

typedef enum response_state_enum {
   RESPONSE_STATE_NULL=0,
   RESPONSE_STATE_WRITE=1,
   RESPONSE_STATE_DUMP=2
} response_state_t;

typedef enum platform_state_enum {
   PLATFORM_STATE_NULL=0,
   PLATFORM_STATE_INIT=1,
   PLATFORM_STATE_RUN=2,
   PLATFORM_STATE_TEARDOWN=3
} platform_state_t;

#define SPORT4A_SEC_PRIORITY   (4)
#define SPORT4B_SEC_PRIORITY   (4)
#define UART0_TXDMA_SEC_PRIORITY   (5)

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

/* Priority level of various BAF threads */
#define PLATFORM_PLVL_0       (9)
#define PLATFORM_PLVL_1       (10)
#define PLATFORM_PLVL_2       (11)
#define PLATFORM_PLVL_3       (12)
#define PLATFORM_PLVL_4       (13)
#define PLATFORM_PLVL_5       (14)
#define PLATFORM_PLVL_6       (15)
#define PLATFORM_PLVL_7       (16)
#define PLATFORM_PLVL_8       (17)
#define PLATFORM_PLVL_9       (18)
#define PLATFORM_PLVL_10      (10)
#define PLATFORM_PLVL_11      (11)

typedef struct Platform_Thread
{
   Platform_Handle_t hPlatform;
   uint32_t sec_id;
   uint32_t sec_priority;
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

   /* input and output buffers for current frame */
   int32_t *input_buff;
   int32_t *output_buff;

   /* Control File */
   char *control_file_name;
   FILE *control_file;
   uint32_t control_cnt;
   uint8_t *control_hex_buff;
   uint32_t control_frame_idx;

   uint32_t *request_msg_buff;
   uint32_t *response_msg_buff;

   /* Control Response File */
   char *response_file_name;
   FILE *response_file;
   FILE *response_file_sav;
   response_state_t response_state;

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

   platform_state_t platform_state;

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
   Platform_Handle_t hPlatform = platform_get_handle();
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   FILE * fp = pPlatform->response_file;
   if (fp == NULL)
      return 0;

   return vfprintf ( fp, format, arg );
}

int platform_printf(char* format, ...)
{
   va_list args;
   int val;
   Platform_Handle_t hPlatform = platform_get_handle();
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   FILE * fp = pPlatform->response_file;
   if (fp == NULL)
      return 0;

   va_start( args, format );
   val = vfprintf(fp, format, args );
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

#define READ_INPUT_BUFFER_FROM_FILE
#if defined(READ_INPUT_BUFFER_FROM_FILE)
uint32_t platform_rcv_input_buffer(float *buff, uint32_t num_chan, uint32_t frame_size)
{
   FILE *fp = g_platfromConfig.audio_in_file;

   uint32_t cnt = fread(buff, sizeof(float), num_chan*frame_size, fp);

   if (cnt != num_chan*frame_size)
      platform_printf("Error: platform_rcv_input_buffer got %d exp % d\n", cnt, num_chan*frame_size);

   return num_chan;
}
#else
uint32_t platform_rcv_input_buffer(float *buff, uint32_t num_chan, uint32_t frame_size)
{
   uint32_t i, j;
   int32_t *input_buff = g_platfromConfig.input_buff;

   if (frame_size != FRAME_SIZE_PER_CHAN)
      return 0;

   if (num_chan > PLATFORM_NUM_IN_CHAN)
   {
      num_chan = PLATFORM_NUM_IN_CHAN;
   }

   for (i = 0; i < num_chan; i++)
   {
      for (j = 0; j < frame_size; j++)
      {
         buff[j + i * frame_size] =
         __builtin_conv_RtoF ( input_buff[j * PLATFORM_NUM_IN_CHAN + i] );
      }
   }

   return num_chan;
}
#endif

#if defined(HIL_FREERUNNING)
#define COMPARE_OUTPUT_BUFFER_WITH_REF
#define XMIT_OUTPUT_BUFFER_TO_DAC
#else
#define WRITE_OUTPUT_BUFFER_TO_FILE
#endif

uint32_t platform_xmt_output_buffer(float *buff, uint32_t num_chan, uint32_t frame_size)
{
   uint32_t i, j;
   int32_t *output_buff = g_platfromConfig.output_buff;

#if defined(WRITE_OUTPUT_BUFFER_TO_FILE)
   {
      FILE *fpOut = g_platfromConfig.audio_out_file;

      uint32_t cnt = fwrite(buff, sizeof(float), num_chan*frame_size, fpOut);

      if (cnt != num_chan*frame_size)
         platform_printf("Error: platform_xmt_output_buffer got %d exp % d\n", cnt, num_chan*frame_size);
   }
#endif
#if defined(COMPARE_OUTPUT_BUFFER_WITH_REF)
   {
      FILE *fpRef = g_platfromConfig.audio_gold_file;

      update_output_stats(fpRef, buff, num_chan, frame_size);
   }
#endif

#if defined(XMIT_OUTPUT_BUFFER_TO_DAC)
   if (frame_size != FRAME_SIZE_PER_CHAN)
      return 0;

   if (num_chan > PLATFORM_NUM_OUT_CHAN)
   {
      num_chan = PLATFORM_NUM_OUT_CHAN;
   }

   for (i = 0; i < num_chan; i++)
   {
      for (j = 0; j < frame_size; j++)
      {
         output_buff[j * PLATFORM_NUM_OUT_CHAN + i] =
         __builtin_conv_FtoR( buff[j + i * frame_size] );
      }
   }
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
   pPlatform->response_file = NULL;
   pPlatform->audio_in_file = NULL;
   pPlatform->audio_out_file = NULL;
   pPlatform->audio_gold_file = NULL;

#if defined(ARGC_ARGV_SUPPORTED)
   pPlatform->control_file_name = NULL;
   pPlatform->response_file_name = NULL;
   pPlatform->audio_in_file_name = NULL;
   pPlatform->audio_out_file_name = NULL;
   pPlatform->audio_gold_file_name = NULL;
#else
   pPlatform->control_file_name = "request.ojx";
   pPlatform->response_file_name = "response.ojx";
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
            else if (argv[idx][1] == 'r')
            {
               pPlatform->response_file_name = argv[idx+1];
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

   if (pPlatform->response_file_name != NULL)
   {
      platform_printf("Info: response file name - %s\n", pPlatform->response_file_name);
      pPlatform->response_file = fopen(pPlatform->response_file_name, "r+");
      if (pPlatform->response_file == NULL)
      {
         platform_printf("Error: opening response file %s failed\n", pPlatform->response_file_name);
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

   pPlatform->response_state = RESPONSE_STATE_WRITE;
   pPlatform->response_file_sav = pPlatform->response_file;

   pPlatform->platform_state = PLATFORM_STATE_INIT;

   return 0;
}

/* Model step function SPORTCallback */
void deploy_target_step( void     *pAppHandle,
                         uint32_t  nEvent,
                         void     *pArg)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)pAppHandle;

   if (nEvent == ADI_SPORT_EVENT_RX_BUFFER_PROCESSED)
   {
      pPlatform->input_buff = (int32_t *)pArg;
   }
   else if (nEvent == ADI_SPORT_EVENT_TX_BUFFER_PROCESSED)
   {
      pPlatform->output_buff = (int32_t *)pArg;
   }
   else
   {
      return;
   }

   if ((pPlatform->input_buff != NULL) && (pPlatform->output_buff != NULL))
   {
      if ((pPlatform->platform_state == PLATFORM_STATE_RUN) &&
          (pPlatform->audio_frame_cnt < pPlatform->audio_frame_max))
      {

         pPlatform->audio_frame_cnt++;

         platform_poll_control_service_message(pPlatform);

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
         adi_sec_Raise(pPlatform->thread[PLATFORM_BASETHREADLEVEL].sec_id);
#elif defined(PLATFORM_BASERATE_AUDIOTHREAD)
         pPlatform->audio_service_tick(pPlatform->callback_param);
#endif /*defined(PLATFORM_MULTIRATE_AUDIOTHREADS)*/
      }
      else
      {
         memset(pPlatform->output_buff, 0, FRAME_SIZE_OUT*sizeof(int32_t));
      }

      resumbitAdcDacBuffers(pPlatform->input_buff, pPlatform->output_buff);

      pPlatform->input_buff = (int32_t *)NULL;
      pPlatform->output_buff = (int32_t *)NULL;
   }
}

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
static void platform_audio_thread_handler(const uint32_t iid, void *arg)
{
   Platform_Thread_t *thread = (Platform_Thread_t *)arg;
   Platform_Config_t *pPlatform = (Platform_Config_t *)(thread->hPlatform);
   pPlatform->audio_thread_handler(pPlatform->callback_param, thread->thread_level);
}

void platform_audio_thread_post(Platform_Handle_t hPlatform, uint32_t priority)
{
   uint32_t cnt = 0;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;

   for (cnt = 0; cnt < PLATFORM_NUM_AUDIOTHREADS; cnt++)
   {
      if (pPlatform->thread[cnt].thread_level == priority)
      {
         adi_sec_Raise(pPlatform->thread[cnt].sec_id);
      }
   }
}

static void platform_threads_init(Platform_Handle_t hPlatform)
{
   int32_t cnt;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;

   /* Following sec_id and priority level settings are just examples */
   pPlatform->thread[0].sec_id = INTR_SYS_SOFT0_INT;
   pPlatform->thread[1].sec_id = INTR_SYS_SOFT1_INT;
   pPlatform->thread[2].sec_id = INTR_SYS_SOFT2_INT;
   pPlatform->thread[3].sec_id = INTR_SYS_SOFT3_INT;
   pPlatform->thread[4].sec_id = INTR_SYS_SOFT4_INT;
   pPlatform->thread[5].sec_id = INTR_SYS_SOFT5_INT;
   pPlatform->thread[6].sec_id = INTR_SYS_SOFT6_INT;
   pPlatform->thread[7].sec_id = INTR_SYS_SOFT7_INT;
   pPlatform->thread[8].sec_id = INTR_TIMER0_TMR0;
   pPlatform->thread[9].sec_id = INTR_TIMER0_TMR1;
   pPlatform->thread[10].sec_id = INTR_TIMER0_TMR2;
   pPlatform->thread[11].sec_id = INTR_TIMER0_TMR3;

   pPlatform->thread[0].sec_priority = PLATFORM_PLVL_0;
   pPlatform->thread[1].sec_priority = PLATFORM_PLVL_1;
   pPlatform->thread[2].sec_priority = PLATFORM_PLVL_2;
   pPlatform->thread[3].sec_priority = PLATFORM_PLVL_3;
   pPlatform->thread[4].sec_priority = PLATFORM_PLVL_4;
   pPlatform->thread[5].sec_priority = PLATFORM_PLVL_5;
   pPlatform->thread[6].sec_priority = PLATFORM_PLVL_6;
   pPlatform->thread[7].sec_priority = PLATFORM_PLVL_7;
   pPlatform->thread[8].sec_priority = PLATFORM_PLVL_8;
   pPlatform->thread[9].sec_priority = PLATFORM_PLVL_9;
   pPlatform->thread[10].sec_priority = PLATFORM_PLVL_10;
   pPlatform->thread[11].sec_priority = PLATFORM_PLVL_11;

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
      adi_int_InstallHandler( pPlatform->thread[cnt].sec_id,
                              (ADI_INT_HANDLER_PTR)platform_audio_thread_handler,
                              (void *)&pPlatform->thread[cnt], true);
      adi_sec_SetPriority(pPlatform->thread[cnt].sec_id,
                          pPlatform->thread[cnt].sec_priority);
   }
}
#endif

void oj_platform_sec_priorities(void)
{
   adi_sec_SetPriority(INTR_UART0_TXDMA,   UART0_TXDMA_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_A_DMA,  SPORT4A_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_A_STAT, SPORT4A_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_B_DMA,  SPORT4B_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_B_STAT, SPORT4B_SEC_PRIORITY);
}

void dump_response_file(char *response_file_name)
{
   unsigned char * src_addr = ramio_file_start_addr(response_file_name);
   int cnt = ramio_file_curr_pos(response_file_name);
#if defined(HIL_FREERUNNING)
   oj_uart_txdma_start(src_addr, cnt);
#else
   {
      int n = 0;
      while (n<cnt)
      {
         int m = cnt - n;
         if (m > 80) m = 80;
         fwrite(src_addr, 1, m,  stdout);
         src_addr += m;
         n = n + m;
      }
      fflush(stdout);
   }
#endif
}

void platform_init(void)
{
   platform_clear_background_tasks((Platform_Handle_t)&g_platfromConfig);

   oj_ezkit_init();



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

   pPlatform->input_buff = (int32_t *)NULL;
   pPlatform->output_buff = (int32_t *)NULL;

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
   platform_threads_init(hPlatform);
#endif

   oj_platform_sec_priorities();

   oj_audio_start();

   while (1)
   {
      switch (pPlatform->platform_state)
      {
         case PLATFORM_STATE_INIT:
            init_output_stats();

            fseek(pPlatform->control_file, 0, SEEK_SET);
            fseek(pPlatform->audio_in_file, 0, SEEK_SET);
            fseek(pPlatform->audio_out_file, 0, SEEK_SET);
            fseek(pPlatform->audio_gold_file, 0, SEEK_SET);

            pPlatform->audio_frame_cnt = 0;
            pPlatform->audio_frame_max = FRAME_IDX_MAX;

            set_next_control_frame(pPlatform);
            platform_poll_control_service_message(pPlatform);
            pPlatform->platform_state = PLATFORM_STATE_RUN;
            break;
         case PLATFORM_STATE_RUN:
            pPlatform->notification_poll(pPlatform->callback_param);

            for (i=0; i<PLATFORM_NUM_BACKGROUND_TASKS; i++)
            {
               if (pPlatform->background_tasks[i] != (Platform_Func_background_task)NULL)
               {
                  pPlatform->background_tasks[i](pPlatform->callback_param);
               }
            }
            if (pPlatform->audio_frame_cnt >= pPlatform->audio_frame_max)
            {
               pPlatform->platform_state = PLATFORM_STATE_TEARDOWN;
            }
            break;
         case PLATFORM_STATE_TEARDOWN:

            if (pPlatform->response_state == RESPONSE_STATE_WRITE)
            {
               platform_poll_control_service_message(pPlatform);

               print_output_stats(pPlatform->response_file);

               fflush(pPlatform->response_file);

               pPlatform->response_file = NULL;
               pPlatform->response_state = RESPONSE_STATE_DUMP;
               dump_response_file(pPlatform->response_file_name);
#if !defined(HIL_FREERUNNING)
               return;
#endif
            }
            else if (pPlatform->response_state == RESPONSE_STATE_DUMP)
            {
               if (oj_uart_txdma_complete() == 1)
               {
                  pPlatform->response_file = pPlatform->response_file_sav;
                  fseek(pPlatform->response_file, 0, SEEK_SET);
                  pPlatform->response_state = RESPONSE_STATE_WRITE;
               }
            }
            pPlatform->platform_state = PLATFORM_STATE_INIT;

            break;
         default:
            break;
      }
   }
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
      if (pPlatform->response_file != NULL)
      {
         fclose(pPlatform->response_file);
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

