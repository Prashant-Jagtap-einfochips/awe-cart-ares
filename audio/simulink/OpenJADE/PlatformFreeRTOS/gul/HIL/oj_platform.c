#include <string.h>
#include <sys/platform.h>
#include <sys/adi_core.h>
#include <drivers/sport/adi_sport.h>
#include <services/int/adi_int.h>
#include <services/int/adi_sec.h>
#include <services/spu/adi_spu.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "oj_platform.h"
#include "Message_Id.h"
#include "ram_file.h"
#include "output_stats.h"
#include "gul_ezkit.h"

#if defined(ENABLE_GHAM_MODULE)
#include "gul_driverInit.h"
#endif

#if !defined(FRAME_IDX_MAX)
#define FRAME_IDX_MAX (20)
#endif
#define SEG_SDRAM_DATA _Pragma ("section(\"seg_sdram\", DOUBLEANY)" )
#define SEG_L1_DATA    _Pragma ("section(\"seg_l1_dmda\", DOUBLEANY)" )

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
#define IIR0_SEC_PRIORITY      (5)
#define FIR0_SEC_PRIORITY      (6)
#define EMDMA0_SEC_PRIORITY    (7)
#define EMDMA1_SEC_PRIORITY    (8)

/* stack size for each thread level */
/* TODO use a JConfig API */
#if !defined(FRAMEWORK_TASK00_STACK_SIZE)
#define FRAMEWORK_TASK00_STACK_SIZE       (1200)
#endif
#if !defined(FRAMEWORK_TASK01_STACK_SIZE)
#define FRAMEWORK_TASK01_STACK_SIZE       (1400)
#endif
#if !defined(FRAMEWORK_TASK02_STACK_SIZE)
#define FRAMEWORK_TASK02_STACK_SIZE       (1200)
#endif
#if !defined(FRAMEWORK_TASK03_STACK_SIZE)
#define FRAMEWORK_TASK03_STACK_SIZE       (1200)
#endif
#if !defined(FRAMEWORK_TASK04_STACK_SIZE)
#define FRAMEWORK_TASK04_STACK_SIZE       (1200)
#endif
#if !defined(FRAMEWORK_TASK05_STACK_SIZE)
#define FRAMEWORK_TASK05_STACK_SIZE       (1200)
#endif
#if !defined(FRAMEWORK_TASK06_STACK_SIZE)
#define FRAMEWORK_TASK06_STACK_SIZE       (1200)
#endif
#if !defined(FRAMEWORK_TASK07_STACK_SIZE)
#define FRAMEWORK_TASK07_STACK_SIZE       (1200)
#endif
#if !defined(FRAMEWORK_TASK08_STACK_SIZE)
#define FRAMEWORK_TASK08_STACK_SIZE       (1200)
#endif
#if !defined(FRAMEWORK_TASK09_STACK_SIZE)
#define FRAMEWORK_TASK09_STACK_SIZE       (400)
#endif
#if !defined(FRAMEWORK_TASK10_STACK_SIZE)
#define FRAMEWORK_TASK10_STACK_SIZE       (400)
#endif
#if !defined(FRAMEWORK_TASK11_STACK_SIZE)
#define FRAMEWORK_TASK11_STACK_SIZE       (400)
#endif

/* StaticTask_t is a publicly accessible structure that has the same size and
alignment requirements as the real TCB structure.  It is provided as a mechanism
for applications to know the size of the TCB (which is dependent on the
architecture and configuration file settings) without breaking the strict data
hiding policy by exposing the real TCB. */
static StaticTask_t frameworkTaskTCB[PLATFORM_NUM_AUDIOTHREADS];

SEG_L1_DATA static StackType_t frameworkTask00Stack[FRAMEWORK_TASK00_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask01Stack[FRAMEWORK_TASK01_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask02Stack[FRAMEWORK_TASK02_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask03Stack[FRAMEWORK_TASK03_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask04Stack[FRAMEWORK_TASK04_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask05Stack[FRAMEWORK_TASK05_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask06Stack[FRAMEWORK_TASK06_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask07Stack[FRAMEWORK_TASK07_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask08Stack[FRAMEWORK_TASK08_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask09Stack[FRAMEWORK_TASK09_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask10Stack[FRAMEWORK_TASK10_STACK_SIZE];
SEG_L1_DATA static StackType_t frameworkTask11Stack[FRAMEWORK_TASK11_STACK_SIZE];

static StackType_t *frameworkTaskStack[] = {
   &frameworkTask00Stack[0],
   &frameworkTask01Stack[0],
   &frameworkTask02Stack[0],
   &frameworkTask03Stack[0],
   &frameworkTask04Stack[0],
   &frameworkTask05Stack[0],
   &frameworkTask06Stack[0],
   &frameworkTask07Stack[0],
   &frameworkTask08Stack[0],
   &frameworkTask09Stack[0],
   &frameworkTask10Stack[0],
   &frameworkTask11Stack[0]
};

static uint16_t frameworkTaskStackSize[] = {
   FRAMEWORK_TASK00_STACK_SIZE,
   FRAMEWORK_TASK01_STACK_SIZE,
   FRAMEWORK_TASK02_STACK_SIZE,
   FRAMEWORK_TASK03_STACK_SIZE,
   FRAMEWORK_TASK04_STACK_SIZE,
   FRAMEWORK_TASK05_STACK_SIZE,
   FRAMEWORK_TASK06_STACK_SIZE,
   FRAMEWORK_TASK07_STACK_SIZE,
   FRAMEWORK_TASK08_STACK_SIZE,
   FRAMEWORK_TASK09_STACK_SIZE,
   FRAMEWORK_TASK10_STACK_SIZE,
   FRAMEWORK_TASK11_STACK_SIZE
};

const char * frameworkTaskName[] = {
    "Step00",
    "Step01",
    "Step02",
    "Step03",
    "Step04",
    "Step05",
    "Step06",
    "Step07",
    "Step08",
    "Step09",
    "Step10",
    "Step11"
};

typedef struct Platform_Thread
{
   Platform_Handle_t hPlatform;
   uint32_t thread_level;
   TaskHandle_t task_handle;
   uint32_t     stepTaskRuns;
   uint32_t     stepTaskOverruns;
   uint8_t      stepTaskActive;
}  Platform_Thread_t;

typedef struct Platform_Config
{
   /* Callback functions */
   Platform_Func_framework_init             framework_init;

   Platform_Func_audio_thread_handler       audio_thread_handler;
   Platform_Thread_t                        thread[PLATFORM_NUM_AUDIOTHREADS];
   TaskHandle_t                             driver_task_handle;

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
   RAMIO_FILE *control_file;
   uint32_t control_cnt;
   uint8_t *control_hex_buff;
   uint32_t control_frame_idx;

   uint32_t *request_msg_buff;
   uint32_t *response_msg_buff;

   /* Control Response File */
   char *response_file_name;
   RAMIO_FILE *response_file;
   RAMIO_FILE *response_file_sav;
   response_state_t response_state;

   /* Frame Count */
   uint32_t audio_frame_cnt;
   uint32_t audio_frame_max;

   /* In/Out Files */
   char *audio_in_file_name;
   char *audio_out_file_name;
   char *audio_gold_file_name;
   RAMIO_FILE *audio_in_file;
   RAMIO_FILE *audio_out_file;
   RAMIO_FILE *audio_gold_file;

   platform_state_t platform_state;

}  Platform_Config_t;

Platform_Config_t g_platfromConfig;

void platform_register_framework_init(Platform_Handle_t hPlatform, Platform_Func_framework_init framework_init)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->framework_init = framework_init;
}

void platform_register_audio_thread_handler(Platform_Handle_t hPlatform, Platform_Func_audio_thread_handler audio_thread_handler)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->audio_thread_handler = audio_thread_handler;
}

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
   RAMIO_FILE * fp = pPlatform->response_file;
   if (fp == NULL)
      return 0;

   return RAMIO_vfprintf ( fp, format, arg );
}

int platform_printf(char* format, ...)
{
   va_list args;
   int val;
   Platform_Handle_t hPlatform = platform_get_handle();
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   RAMIO_FILE * fp = pPlatform->response_file;
   if (fp == NULL)
      return 0;

   va_start( args, format );
   val = RAMIO_vfprintf(fp, format, args );
   va_end(args);
   return val;
}

void xmit_control_service_request(RAMIO_FILE *fp, uint32_t cnt, uint32_t *buff)
{
   uint32_t idx;

   if (fp == NULL)
      return;

   RAMIO_fprintf(fp, "REQ:");
   for(idx = 0; idx < cnt; idx++)
   {
      RAMIO_fprintf(fp, " %8.8X", *buff++);
   }
   RAMIO_fprintf(fp, "\n");

   return;
}

void xmit_control_service_response(RAMIO_FILE *fp, char *service, uint32_t msg_id, uint32_t cnt, uint8_t *buff)
{
   uint32_t idx;

   if (fp == NULL)
      return;

   RAMIO_fprintf(fp, "%s: %8.8X %8.8x", service, msg_id, cnt);
   for(idx = 0; idx < cnt; idx++)
   {
      RAMIO_fprintf(fp, " %2.2X", *buff++);
   }
   RAMIO_fprintf(fp, "\n");

   return;
}

void xmit_control_service_error(RAMIO_FILE *fp, char *service, uint32_t msg_id, int32_t status)
{
   if (fp == NULL)
      return;

   RAMIO_fprintf(fp, "%s: %8.8X %8.8x\n", service, msg_id, status);

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
   RAMIO_FILE *fp = g_platfromConfig.audio_in_file;

   uint32_t cnt = RAMIO_fread(buff, sizeof(float), num_chan*frame_size, fp);

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
      RAMIO_FILE *fpOut = g_platfromConfig.audio_out_file;

      uint32_t cnt = RAMIO_fwrite(buff, sizeof(float), num_chan*frame_size, fpOut);

      if (cnt != num_chan*frame_size)
         platform_printf("Error: platform_xmt_output_buffer got %d exp % d\n", cnt, num_chan*frame_size);
   }
#endif
#if defined(COMPARE_OUTPUT_BUFFER_WITH_REF)
   {
      RAMIO_FILE *fpRef = g_platfromConfig.audio_gold_file;

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

uint32_t read_control_line(RAMIO_FILE *fp, uint8_t *hex_buff, uint32_t *bin_buff)
{
   uint32_t ridx, widx, cnt;
   uint8_t buff[8];
   uint32_t ch;

   RAMIO_fgets((char *)hex_buff, CONTROL_LINE_SIZE, fp);

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
      if (RAMIO_feof(pPlatform->control_file))
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
      pPlatform->control_file = RAMIO_fopen(pPlatform->control_file_name, "r");
      if (pPlatform->control_file == NULL)
      {
         platform_printf("Error: opening control file %s failed\n", pPlatform->control_file_name);
         return -1;
      }
   }

   if (pPlatform->response_file_name != NULL)
   {
      platform_printf("Info: response file name - %s\n", pPlatform->response_file_name);
      pPlatform->response_file = RAMIO_fopen(pPlatform->response_file_name, "r+");
      if (pPlatform->response_file == NULL)
      {
         platform_printf("Error: opening response file %s failed\n", pPlatform->response_file_name);
         return -1;
      }
   }

   if (pPlatform->audio_in_file_name != NULL)
   {
      platform_printf("Info: audio input file name - %s\n", pPlatform->audio_in_file_name);
      pPlatform->audio_in_file = RAMIO_fopen(pPlatform->audio_in_file_name, "rb");
      if (pPlatform->audio_in_file == NULL)
      {
         platform_printf("Error: opening audio input file %s failed\n", pPlatform->audio_in_file_name);
         return -1;
      }
   }

   if (pPlatform->audio_out_file_name != NULL)
   {
      platform_printf("Info: audio output file name - %s\n", pPlatform->audio_out_file_name);
      pPlatform->audio_out_file = RAMIO_fopen(pPlatform->audio_out_file_name, "wb");
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
      pPlatform->audio_gold_file = RAMIO_fopen(pPlatform->audio_gold_file_name, "rb");
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

/* *pREG_SEC0_END = *pREG_SEC0_CSID0;
*/

void oj_platform_sec_priorities(void)
{
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

/* Model step function SPORTCallback */
void deploy_target_step( void     *pAppHandle,
                         uint32_t  nEvent,
                         void     *pArg)
{
   int32_t *input_buff;
   int32_t *output_buff;

   if (nEvent != ADI_SPORT_EVENT_TX_BUFFER_PROCESSED)
   {
      return;
   }

   Platform_Config_t *pPlatform = (Platform_Config_t *)pAppHandle;

   output_buff = (int32_t *)pArg;
   if (output_buff == int_SP0ABuffer1)
   {
      input_buff = int_SP0ABuffer4;
   }
   else if (output_buff == int_SP0ABuffer2)
   {
      input_buff = int_SP0ABuffer5;
   }
   else
   {
      platform_printf("Error: Unexpected output buffer 0x%8.8x expected 0x%8.8x or 0x%8.8x\n", (uint32_t)pArg, (uint32_t)int_SP0ABuffer1, (uint32_t)int_SP0ABuffer2);
      return;
   }
   if ((pPlatform->platform_state == PLATFORM_STATE_RUN) &&
       (pPlatform->audio_frame_cnt < pPlatform->audio_frame_max))
   {
      /* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
         it will get set to pdTRUE inside the interrupt safe API function if a
         context switch is required. */
      BaseType_t xHigherPriorityTaskWoken = pdFALSE;

      pPlatform->audio_frame_cnt++;

      pPlatform->input_buff = input_buff;

      pPlatform->output_buff = output_buff;

      platform_poll_control_service_message(pPlatform);

      /* Send a notification directly to the task */
      vTaskNotifyGiveFromISR(
          pPlatform->thread[PLATFORM_BASETHREADLEVEL].task_handle,
          &xHigherPriorityTaskWoken );

      /* Pass the xHigherPriorityTaskWoken value into portYIELD_FROM_ISR(). If
           xHigherPriorityTaskWoken was set to pdTRUE inside vTaskNotifyGiveFromISR()
           then calling portYIELD_FROM_ISR() will request a context switch. If
           xHigherPriorityTaskWoken is still pdFALSE then calling
           portYIELD_FROM_ISR() will have no effect. The implementation of
           portYIELD_FROM_ISR() used by the Windows port includes a return statement,
           which is why this function does not explicitly return a value. */
      portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
   }
   else
   {
      memset(output_buff, 0, FRAME_SIZE_OUT*sizeof(int32_t));
   }
}

static void platform_audio_thread_handler( void *pvParameters )
{
   Platform_Thread_t *pThread = (Platform_Thread_t *)pvParameters;
   Platform_Config_t *pPlatform = (Platform_Config_t *)(pThread->hPlatform);

   while (1)
   {
      /* Wait to be triggered */
      ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

      pThread->stepTaskActive = 1;
      pThread->stepTaskRuns++;

      /* Perform audio processing for this rate */
      pPlatform->audio_thread_handler(pPlatform->callback_param, pThread->thread_level);

      pThread->stepTaskActive = 0;
   }
}

void platform_audio_thread_post(Platform_Handle_t hPlatform, uint32_t priority)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;

   pPlatform->thread[priority].stepTaskOverruns += pPlatform->thread[priority].stepTaskActive;

   xTaskNotifyGive( pPlatform->thread[priority].task_handle );
}

static void platform_audio_threads_init(Platform_Handle_t hPlatform)
{
   int32_t cnt;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   uint32_t BAFThreadLevel = 0; /* zero is the highest priority */
   uint32_t OSTaskPriority = ( tskIDLE_PRIORITY + 3 ) + PLATFORM_NUM_AUDIOTHREADS; /* zero is the lowest priority */

   for (cnt = 0; cnt < PLATFORM_NUM_AUDIOTHREADS; cnt++)
   {
      Platform_Thread_t *pThread = &(pPlatform->thread[cnt]);

      pThread->hPlatform = hPlatform;
      pThread->thread_level = BAFThreadLevel;

      // Create the OS task
      pThread->task_handle = xTaskCreateStatic(
         platform_audio_thread_handler,  /* Pointer to the function that implements the task. */
         frameworkTaskName[cnt],         /* Text name for the task.  This is to facilitate debugging only. */
         frameworkTaskStackSize[cnt],    /* Stack depth - most small microcontrollers will use much less stack than this. */
         pThread,                        /* The task parameter. */
         OSTaskPriority,                 /* This task will run at priority */
         frameworkTaskStack[cnt],        /* The buffer to use as the task's stack. */
         &frameworkTaskTCB[cnt] );       /* The variable that will hold the task's TCB. */
      BAFThreadLevel++;
      OSTaskPriority--;

      // Clear the task run counters
      pThread->stepTaskRuns = 0;
      pThread->stepTaskOverruns = 0;
      pThread->stepTaskActive = 0;
   }
}

static void platform_driver_thread_handler( void *pvParameters )
{
   int i;
   Platform_Config_t *pPlatform = (Platform_Config_t *)pvParameters;

   oj_audio_start();

   while (1)
   {
      switch (pPlatform->platform_state)
      {
         case PLATFORM_STATE_INIT:
            init_output_stats();

            RAMIO_fseek(pPlatform->control_file, 0, SEEK_SET);
            RAMIO_fseek(pPlatform->audio_in_file, 0, SEEK_SET);
            RAMIO_fseek(pPlatform->audio_out_file, 0, SEEK_SET);
            RAMIO_fseek(pPlatform->audio_gold_file, 0, SEEK_SET);

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

#if defined(COMPARE_OUTPUT_BUFFER_WITH_REF)
               print_output_stats(pPlatform->response_file);
#endif

               pPlatform->response_file = NULL;
               pPlatform->response_state = RESPONSE_STATE_DUMP;
               dump_response_file(pPlatform->response_file_name);
#if !defined(HIL_FREERUNNING)
               platform_close((Platform_Handle_t)pPlatform);
               exit(EXIT_SUCCESS);
#endif
            }
            else if (pPlatform->response_state == RESPONSE_STATE_DUMP)
            {
               if (oj_uart_txdma_complete() == 1)
               {
                  pPlatform->response_file = pPlatform->response_file_sav;
                  RAMIO_fseek(pPlatform->response_file, 0, SEEK_SET);
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

#if !defined(PLATFORM_DRIVER_STACK_SIZE)
#define PLATFORM_DRIVER_STACK_SIZE       (400)
#endif
SEG_L1_DATA static StackType_t platformDriverStack[PLATFORM_DRIVER_STACK_SIZE];
static StaticTask_t platformDriverTCB;

static void platform_driver_thread_init(Platform_Handle_t hPlatform)
{
   int32_t cnt;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   uint32_t DriverTaskPriority = ( tskIDLE_PRIORITY + 3 );

   pPlatform->driver_task_handle = xTaskCreateStatic(
      platform_driver_thread_handler,  /* Pointer to the function that implements the task. */
      "Drv",                          /* Text name for the task.  This is to facilitate debugging only. */
      PLATFORM_DRIVER_STACK_SIZE,     /* Stack depth - most small microcontrollers will use much less stack than this. */
      pPlatform,                      /* The task parameter. */
      DriverTaskPriority,             /* This task will run at priority */
      platformDriverStack,            /* The buffer to use as the task's stack. */
      &platformDriverTCB );            /* The variable that will hold the task's TCB. */
}

void platform_init(void)
{
   platform_clear_background_tasks((Platform_Handle_t)&g_platfromConfig);

   oj_ezkit_init();

#if defined(ENABLE_GHAM_MODULE)
    /* Initialize the GUL HW Accelerator Manager framework. Note, must be
     * called prior to calling accelerator configuration APIs. */
    {
       gulDriverInit_t cfg;
       cfg.hSpu = hSpu;
       cfg.iir0_sec_priority = IIR0_SEC_PRIORITY;
       cfg.fir0_sec_priority = FIR0_SEC_PRIORITY;
       cfg.emdma0_sec_priority = EMDMA0_SEC_PRIORITY;
       cfg.emdma1_sec_priority = EMDMA1_SEC_PRIORITY;
       gul_driverInit((void *)&cfg);
    }
#endif
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

   oj_platform_sec_priorities();

   platform_audio_threads_init(hPlatform);

   platform_driver_thread_init(hPlatform);

   /* Start the scheduler to start the tasks executing. */
   vTaskStartScheduler();
}

void platform_close(Platform_Handle_t hPlatform)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   if (pPlatform != NULL)
   {
      if (pPlatform->control_file != NULL)
      {
         RAMIO_fclose(pPlatform->control_file);
      }
      if (pPlatform->response_file != NULL)
      {
         RAMIO_fclose(pPlatform->response_file);
      }
      if (pPlatform->audio_in_file != NULL)
      {
         RAMIO_fclose(pPlatform->audio_in_file);
      }
      if (pPlatform->audio_out_file != NULL)
      {
         RAMIO_fclose(pPlatform->audio_out_file);
      }
      if (pPlatform->audio_gold_file != NULL)
      {
         RAMIO_fclose(pPlatform->audio_gold_file);
      }
      ramio_files_close();
   }
}

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
/* RTOS memory */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

void vApplicationGetIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer, StackType_t ** ppxIdleTaskStackBuffer, uint32_t * pulIdleTaskStackSize)
{
   *ppxIdleTaskTCBBuffer   = &xIdleTaskTCB;
   *ppxIdleTaskStackBuffer = uxIdleTaskStack;
   *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

#if ( configUSE_TIMERS == 1 )
/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
   static StaticTask_t xTimerTaskTCB;
   static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

   /* Pass out a pointer to the StaticTask_t structure in which the Timer
   task's state will be stored. */
   *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

   /* Pass out the array that will be used as the Timer task's stack. */
   *ppxTimerTaskStackBuffer = uxTimerTaskStack;

   /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
   Note that, as the array is necessarily of type StackType_t,
   configMINIMAL_STACK_SIZE is specified in words, not bytes. */
   *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif /* configUSE_TIMERS == 1 */
#endif /* configSUPPORT_STATIC_ALLOCATION == 1 */


/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
   ( void ) pcTaskName;
   ( void ) pxTask;

   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
   function is called if a stack overflow is detected. */
   taskDISABLE_INTERRUPTS();
   for ( ;; );
}

/*-----------------------------------------------------------*/

void vAssertCalled( const char * pcFile, unsigned long ulLine )
{
   volatile unsigned long ul = 0;

   platform_printf("Error: vAssertCalled at line %d of %s\n", ulLine, pcFile);

   while ( ul == 0 )
   {
      NOP();
      NOP();
   }
}

