#include <stdio.h>
#include <stdint.h>
#include <sys/platform.h>
#include <sys/adi_core.h>
#include <services/int/adi_sec.h>
#include <services/spu/adi_spu.h>
#include <services/pwr/adi_pwr.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "oj_platform.h"
#include "Message_Id.h"
#include "ram_file.h"
#include "output_stats.h"

#if defined(ENABLE_GHAM_MODULE)
#include "gxpHAM.h"
#include "gEMDMA.h"
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

#define SPORT4A_SEC_PRIORITY   (4)
#define SPORT4B_SEC_PRIORITY   (4)
#define IIR0_SEC_PRIORITY      (5)
#define IIR1_SEC_PRIORITY      (5)
#define IIR2_SEC_PRIORITY      (5)
#define IIR3_SEC_PRIORITY      (5)
#define IIR4_SEC_PRIORITY      (5)
#define IIR5_SEC_PRIORITY      (5)
#define IIR6_SEC_PRIORITY      (5)
#define IIR7_SEC_PRIORITY      (5)
#define FIR0_SEC_PRIORITY      (6)
#define FIR1_SEC_PRIORITY      (6)
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

   /* Control File */
   char *control_file_name;
   RAMIO_FILE *control_file;
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
   RAMIO_FILE *audio_in_file;
   RAMIO_FILE *audio_out_file;
   RAMIO_FILE *audio_gold_file;

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
   RAMIO_FILE *fp = g_platfromConfig.audio_in_file;

   uint32_t cnt = RAMIO_fread(buff, sizeof(float), num_chan*frame_size, fp);

   if (cnt != num_chan*frame_size)
      platform_printf("Error: platform_rcv_input_buffer got %d exp % d\n", cnt, num_chan*frame_size);

   return num_chan;
}

uint32_t platform_xmt_output_buffer(float *buff, uint32_t num_chan, uint32_t frame_size)
{
   RAMIO_FILE *fpOut = g_platfromConfig.audio_out_file;
#if defined(CALCULATE_OUTPUT_STATS)
   RAMIO_FILE *fpRef = g_platfromConfig.audio_gold_file;
#endif
   uint32_t cnt = RAMIO_fwrite(buff, sizeof(float), num_chan*frame_size, fpOut);

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
      pPlatform->control_file = RAMIO_fopen(pPlatform->control_file_name, "r");
      if (pPlatform->control_file == NULL)
      {
         platform_printf("Error: opening control file %s failed\n", pPlatform->control_file_name);
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

#if defined(PIL_INTERFRAMEDELAY)
   platform_printf("Info: using additional inter frame delay %d cycles\n", PIL_INTERFRAMEDELAY);
#endif

   pPlatform->control_hex_buff = g_control_hex;
   pPlatform->request_msg_buff = g_request_buff;
   pPlatform->response_msg_buff = g_response_buff;

   return 0;
}

static inline void platform_delay(void)
{
#if defined(PIL_INTERFRAMEDELAY)
   volatile int delay = PIL_INTERFRAMEDELAY;
   while (delay--)
   {
      asm("nop;");
   }
#endif
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

   while (pPlatform->audio_frame_cnt < pPlatform->audio_frame_max)
   {
      /*platform_printf("Info: audio_frame_cnt %d\n", pPlatform->audio_frame_cnt);*/
      platform_poll_control_service_message(pPlatform);

      xTaskNotifyGive( pPlatform->thread[PLATFORM_BASETHREADLEVEL].task_handle );

      platform_delay();

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
   platform_close((Platform_Handle_t)pPlatform);
   exit(EXIT_SUCCESS);
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


#if defined(PIL_EZKIT_ONLY)
#define MHZTOHZ            (1000000u)
#define EZKIT_CLKIN        (25u  * MHZTOHZ)

/* Memory required for the SPU operation */
uint8_t SpuMemory[ADI_SPU_MEMORY_SIZE];

void oj_platform_sec_priorities(void)
{
   adi_sec_SetPriority(INTR_SPORT4_A_DMA,  SPORT4A_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_A_STAT, SPORT4A_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_B_DMA,  SPORT4B_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_B_STAT, SPORT4B_SEC_PRIORITY);
}
#endif /*defined(PIL_EZKIT_ONLY)*/

void gxp_core_enable(void)
{
   adi_core_enable(ADI_CORE_SHARC1);
}

void platform_init(void)
{
#if defined(PIL_EZKIT_ONLY)
    int32_t result = 0;

    /* SPU handle */
    ADI_SPU_HANDLE      hSpu;

    result = adi_pwr_Init(0u, EZKIT_CLKIN);
    if(result != 0) {
       platform_printf("Error: Power initialization failed\n");
       return;
    }

    result = adi_sec_Init();
    if (result != 0) {
       platform_printf("Error: adi_sec_Init failed\n");
       return;
    }

    /* Initialize SPU Service */
    result = adi_spu_Init(0u, SpuMemory, NULL, NULL, &hSpu);
    if (result != 0) {
       platform_printf("Error: Failed to initialize SPU service\n");
       return;
    }

#if defined(ENABLE_GHAM_MODULE)
    /* Initialize the GXP HW Accelerator Manager framework. Note, must be
     * called prior to calling accelerator configuration APIs. */
    {
       adi_sec_SetPriority(INTR_IIR0_DMA,     IIR0_SEC_PRIORITY);
       adi_sec_SetPriority(INTR_IIR1_DMA,     IIR1_SEC_PRIORITY);
       adi_sec_SetPriority(INTR_IIR2_DMA,     IIR2_SEC_PRIORITY);
       adi_sec_SetPriority(INTR_IIR3_DMA,     IIR3_SEC_PRIORITY);
       adi_sec_SetPriority(INTR_IIR4_DMA,     IIR4_SEC_PRIORITY);
       adi_sec_SetPriority(INTR_IIR5_DMA,     IIR5_SEC_PRIORITY);
       adi_sec_SetPriority(INTR_IIR6_DMA,     IIR6_SEC_PRIORITY);
       adi_sec_SetPriority(INTR_IIR7_DMA,     IIR7_SEC_PRIORITY);

       adi_sec_SetPriority(INTR_FIR0_DMA,     FIR0_SEC_PRIORITY);
       adi_sec_SetPriority(INTR_FIR1_DMA,     FIR1_SEC_PRIORITY);

       adi_sec_SetPriority(INTR_EMDMA0_DONE,  EMDMA0_SEC_PRIORITY);
       adi_sec_SetPriority(INTR_EMDMA1_DONE,  EMDMA1_SEC_PRIORITY);

       /* Make EMDMA to generate secure transactions */
       result = adi_spu_EnableMasterSecure(hSpu, ADI_SPU0_EMDMA0_ID, true);
       if (result != 0)
       {
          platform_printf("Error: Failed to enable Master secure for EMDMA\n");
          return;
       }

       /* Initialize EMDMA stream assigned to the SHARC+ core currently running
       *    SHARC+ Core 1 = EMDMA Stream 0
       *    SHARC+ Core 2 = EMDMA Stream 1
       */
       gEmdmaResult_t gEmdmaResult = gEmdmaOpen( gEmdmaGetCoreStreamAssignment() );
       if ( gEmdmaResult != GEMDMA_SUCCESS )
       {
          platform_printf("Error: Failed to open EMDMA\n");
          return;
       }

       /* Disable all FIR and IIR accelerator clocks. They will be enabled only
       * if the accelerator device is actually used. */
       gxpHamResult_t gFirResult = gxpHamFirDisableAllClocks();
       if ( gFirResult != GXPHAM_SUCCESS )
       {
          platform_printf("Error: Failed to disable FIR accelerator clocks\n");
          return;
       }

       gxpHamResult_t gIirResult = gxpHamIirDisableAllClocks();
       if( gIirResult != GXPHAM_SUCCESS )
       {
          platform_printf("Error: Failed to disable IIR accelerator clocks\n");
          return;
       }
    }
#endif
#endif /*defined(PIL_EZKIT_ONLY)*/

   platform_clear_background_tasks((Platform_Handle_t)&g_platfromConfig);

   ramio_files_init();
}

Platform_Handle_t platform_get_handle(void)
{
   return (Platform_Handle_t)&g_platfromConfig;
}

void platform_main(Platform_Handle_t hPlatform, int argc, char *argv[])
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;

   if (pPlatform == NULL)
   {
      platform_printf("Error: NULL Platform handle\n");
      return;
   }

   if (set_run_parameters(pPlatform, argc, argv) != 0)
      return;

   pPlatform->callback_param = pPlatform->framework_init();

#if defined(PIL_EZKIT_ONLY)
   oj_platform_sec_priorities();
#endif

   pPlatform->audio_frame_cnt = 0;
   pPlatform->audio_frame_max = FRAME_IDX_MAX;

   set_next_control_frame(pPlatform);

#if defined(CALCULATE_OUTPUT_STATS)
   init_output_stats();
#endif

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

