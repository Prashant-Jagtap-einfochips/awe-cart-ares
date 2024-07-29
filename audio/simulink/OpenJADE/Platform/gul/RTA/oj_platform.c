#include <sys/platform.h>
#include <sys/adi_core.h>
#include <drivers/sport/adi_sport.h>
#include <services/int/adi_int.h>
#include <services/int/adi_sec.h>

#include "oj_platform.h"
#include "gul_ezkit.h"

#if defined(ENABLE_GHAM_MODULE)
#include "gul_driverInit.h"
#endif

#if defined(UART_REMOTE_ECHO)
#define D_UART_putc(x) UART_putc(x)
#else
#define D_UART_putc(x) //UART_putc(x)
#endif

#define SPORT4A_SEC_PRIORITY   (4)
#define SPORT4B_SEC_PRIORITY   (4)
#define IIR0_SEC_PRIORITY      (5)
#define FIR0_SEC_PRIORITY      (6)
#define EMDMA0_SEC_PRIORITY    (7)
#define EMDMA1_SEC_PRIORITY    (8)

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
   xmit_control_service_response(stdout,
                                 "NTF",
                                 message->id,
                                 message->size,
                                 (uint8_t *)(message->data));
}

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

uint32_t platform_xmt_output_buffer(float *buff, uint32_t num_chan, uint32_t frame_size)
{
   uint32_t i, j;
   int32_t *output_buff = g_platfromConfig.output_buff;

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

   return num_chan;
}

#define SEG_EXT_DATA _Pragma ("section(\"seg_sdram\", DOUBLEANY)" )
SEG_EXT_DATA uint8_t req_msg_buf[8192];
SEG_EXT_DATA uint8_t rsp_msg_buf[8192];

Platform_control_msg req_msg = { 0, 0, &req_msg_buf[4] }; // skip the BAF_id
Platform_control_msg rsp_msg = { 0, 40, rsp_msg_buf };

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

void platform_poll_control_service_message(Platform_Config_t *pPlatform)
{
    static uint8_t prv_rx_char = 0;
    static uint8_t rx_char = 0;
    static uint32_t req_msg_count = 0;
    uint32_t req_byte_count;
    int32_t status;
    int ch = UART_getc();

    while (ch != -1) {
        prv_rx_char = rx_char;
        rx_char = ch;
        D_UART_putc(rx_char);
        req_msg_count++;

        if (rx_char == '\n') {
            req_byte_count = (req_msg_count - 1) >> 1;
            if (req_byte_count >= 4) {
               req_msg.id = *(uint32_t*) &req_msg_buf[0];
               req_msg.size = req_byte_count - 4;
               rsp_msg.size = sizeof(rsp_msg_buf);
               status = pPlatform->control_service_message(&req_msg, &rsp_msg, pPlatform->callback_param);
               if (status < 0)
               {
                  xmit_control_service_error(stdout,
                                             "RSP",
                                             rsp_msg.id,
                                             status);
               }
               else
               {
                  xmit_control_service_response(stdout,
                                                "RSP",
                                                rsp_msg.id,
                                                rsp_msg.size,
                                                (uint8_t *)(rsp_msg.data));
               }
               /* Give back the prompt */
               UART_putc('>');
               UART_putc('\n');
            }
            else{
               UART_putc('?');
               UART_putc('\n');
            }
            req_msg_count = 0;
        }
        else if (hex2bin(rx_char)==INVALID_HEX_CHAR) {
            req_msg_count--;
        }
        else {
            if (((req_msg_count & 1) == 0)&& ( req_msg_count > 0 )) {
                uint8_t rx_byte = hex2bin(rx_char) | (hex2bin(prv_rx_char) << 4);
                req_msg_buf[(req_msg_count >> 1) - 1] = rx_byte;
            }
        }
        ch = UART_getc();
    }
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
   pPlatform->input_buff = input_buff;

   pPlatform->output_buff = output_buff;

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
   adi_sec_Raise(pPlatform->thread[PLATFORM_BASETHREADLEVEL].sec_id);
#elif defined(PLATFORM_BASERATE_AUDIOTHREAD)
   pPlatform->audio_service_tick(pPlatform->callback_param);
#endif /*defined(PLATFORM_MULTIRATE_AUDIOTHREADS)*/
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
   adi_sec_SetPriority(INTR_SPORT4_A_DMA,  SPORT4A_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_A_STAT, SPORT4A_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_B_DMA,  SPORT4B_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_B_STAT, SPORT4B_SEC_PRIORITY);
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
   pPlatform->callback_param = pPlatform->framework_init();

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
   platform_threads_init(hPlatform);
#endif

   oj_platform_sec_priorities();

   oj_audio_start();

   while (1)
   {
      pPlatform->notification_poll(pPlatform->callback_param);

      platform_poll_control_service_message(pPlatform);

      for (i=0; i<PLATFORM_NUM_BACKGROUND_TASKS; i++)
      {
         if (pPlatform->background_tasks[i] != (Platform_Func_background_task)NULL)
         {
            pPlatform->background_tasks[i](pPlatform->callback_param);
         }
      }
   }
}

void platform_close(Platform_Handle_t hPlatform)
{
}


