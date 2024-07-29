#include <stdio.h>
#include <stdint.h>
#include <sys/platform.h>
#include <sys/adi_core.h>
#include <drivers/sport/adi_sport.h>
#include <services/int/adi_int.h>
#include <services/int/adi_sec.h>
#include <services/spu/adi_spu.h>
#include <stdlib.h>
#include <string.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "oj_platform.h"
#include "gxp_ezkit.h"

/* Simulink model Talaria framework auto-generated files */
#include "TalariaAPI.h"
#include "TalariaCrossCore.h"
#if defined(INTERCORE_XFER_MDMA)
#include "MDMA_Intercore.h"
#endif /*defined(INTERCORE_XFER_MDMA)*/

#if defined(SPC1)
   #define SPC "SPC1> "
#elif defined(SPC2)
   #define SPC "SPC2> "
#else
   #define SPC ""
#endif

#if defined(ENABLE_GHAM_MODULE)
#include "gxpHAM.h"
#include "gEMDMA.h"
#endif

#if defined(UART_REMOTE_ECHO)
#define D_UART_putc(x) UART_putc(x)
#else
#define D_UART_putc(x) //UART_putc(x)
#endif

#define SEG_L1_DATA    _Pragma ("section(\"seg_l1_dmda\", DOUBLEANY)" )

#define SPORT4A_SEC_PRIORITY   (4)
#define SPORT4B_SEC_PRIORITY   (4)
#define TRU0_SLV8_SEC_PRIORITY (4)
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
#if defined(SPC2)
   TaskHandle_t                             frame_sync_notification_task_handle;
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

#define STDOUT_BUF_LEN   (256)
#define SEG_L2_UNCACHED _Pragma ("section(\"seg_l2_uncached\", DOUBLEANY)" )
SEG_L2_UNCACHED char stdout_buf[STDOUT_BUF_LEN];

int platform_vprintf (const char * format, va_list args )
{
   int count = vsnprintf( stdout_buf, STDOUT_BUF_LEN, format, args );
#if defined(SPC1)
   UARTIO_write( stdout_buf, count);
#endif
   return count;
}

int platform_printf(char* format, ...)
{
   va_list args;
   int val;
   va_start( args, format );
   val = platform_vprintf(format, args );
   va_end(args);
   return val;
}

void xmit_control_service_response(FILE *fp, char *service, uint32_t msg_id, uint32_t cnt, uint8_t *buff)
{
   uint32_t idx;

   ( void ) fp;

   platform_printf("%s: %8.8X %8.8x", service, msg_id, cnt);
   for(idx = 0; idx < cnt; idx++)
   {
      platform_printf(" %2.2X", *buff++);
   }
   platform_printf("\n");

   return;
}

void xmit_control_service_error(FILE *fp, char *service, uint32_t msg_id, int32_t status)
{
   ( void ) fp;

   platform_printf("%s: %8.8X %8.8x\n", service, msg_id, status);

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
   uint32_t i, j, m;
   int32_t *input_buff = g_platfromConfig.input_buff;

   if (frame_size != FRAME_SIZE_PER_CHAN)
      return 0;

   if (num_chan > PLATFORM_NUM_IN_CHAN)
   {
      num_chan = PLATFORM_NUM_IN_CHAN;
   }

   /* Deinterleave the audio input channels from ADC, convert to float, and copy
   * to model channel input buffers for processing. */
   float buf[PLATFORM_NUM_IN_CHAN];
   for (i = 0, m = 0; i < FRAME_SIZE_PER_CHAN * PLATFORM_NUM_IN_CHAN; i += PLATFORM_NUM_IN_CHAN, m++)
   {
      /* ADC audio inputs are mapped such that TDM slot 0 = J1 (RIGHT), slot 1 =
      * J1 (LEFT), slot 2 = J2 (RIGHT), and slot 3 = J2 (LEFT).  DAC TDM slot
      * outputs map the same way as the inputs. */
      buf[0] =  __builtin_conv_RtoF ( input_buff[i+1] );
      buf[1] =  __builtin_conv_RtoF ( input_buff[i] );
      buf[2] =  __builtin_conv_RtoF ( input_buff[i+3] );
      buf[3] =  __builtin_conv_RtoF ( input_buff[i+2] );

      for (j = 0; j < num_chan; j++)
      {
         buff[m + (j * FRAME_SIZE_PER_CHAN)] = buf[j];
      }
   }

   return num_chan;
}

/* ------------------------------------------------------------------------------------
 * Mapping of audio Stereo input and output jacks to SPORT TDM slots coming from ADC
 * and going to the DAC:
 *    Right Channels (RED) map to TDM even slot indexes (0, 2, ...)
 *    Left Channels (WHITE) map to TDM odd slot indexes (1, 3, ...)
 * ----------------------------------------------------------------------------------*/

/* Transfer 32 sample frame audio channels contained in output buffers to DAC TDM
 * channel slots after converting them from 32 bit floating point to 32 bit fixed
 * point and mapping them to the correct DAC TDM slot.
 *
 *   channels[] = Pointers to output vector for each model audio channel to output.
 *                Note model audio channel pointers in array are ordered from Stereo
 *                output J17 (LEFT), J17 (RIGHT), J18 (LEFT), J18 (RIGHT), ... , J20
 *                (LEFT), and J20 (RIGHT), with the maximum number of channel
 *                pointers being 8. NULL pointers can be used for any channel to
 *                skip an Stereo output if desired.
 *   numChans  =  Number of channel pointers in channel array
 */
uint32_t platform_xmt_output_buffer(float *buff, uint32_t num_chan, uint32_t frame_size)
{
   uint32_t i, j, m;
   int32_t *output_buff = g_platfromConfig.output_buff;

   if (frame_size != FRAME_SIZE_PER_CHAN)
      return 0;

   if (num_chan > PLATFORM_NUM_OUT_CHAN)
   {
      num_chan = PLATFORM_NUM_OUT_CHAN;
   }

   float value[PLATFORM_NUM_OUT_CHAN] = { 0.0 };
   for (i = 0, m = 0; i < FRAME_SIZE_PER_CHAN * PLATFORM_NUM_OUT_CHAN; i += PLATFORM_NUM_OUT_CHAN, m++)
   {
      for (j = 0; j < num_chan; j++)
      {
         value[j] = buff[m + (j * FRAME_SIZE_PER_CHAN)];
      }

      /* Map each model output channel to the corresponding DAC output, converting
       * output to fixed point 32 bit value and then scaling. */
      output_buff[i] = __builtin_conv_FtoR ( value[1] );
      output_buff[i + 1] = __builtin_conv_FtoR ( value[0] );
      output_buff[i + 2] = __builtin_conv_FtoR ( value[3] );
      output_buff[i + 3] = __builtin_conv_FtoR ( value[2] );
      output_buff[i + 4] = __builtin_conv_FtoR ( value[5] );
      output_buff[i + 5] = __builtin_conv_FtoR ( value[4] );
      output_buff[i + 6] = __builtin_conv_FtoR ( value[7] );
      output_buff[i + 7] = __builtin_conv_FtoR ( value[6] );
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

#if defined(SPC1)
static inline void TriggerFrameSyncInterrupt(void)
{
   /* Trigger SW driven interrupt 0 to Sharc 2 for frame sync */
   *pREG_TRU0_MTR = TRGM_SYS_SOFT0_MST;
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

   TriggerFrameSyncInterrupt();

   /* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
      it will get set to pdTRUE inside the interrupt safe API function if a
      context switch is required. */
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;

   pPlatform->input_buff = input_buff;

   pPlatform->output_buff = output_buff;

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
#endif

#if defined(SPC2)
/**
 *  Interrupt Handler for frame sync from SHARC 1
 *
 *  param iid = ID of the interrupt
 *  param handlerArg = pointer to callback function
 */
static void FrameSyncInterruptHandler(uint32_t iid, void* handlerArg)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)handlerArg;

   /* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
      it will get set to pdTRUE inside the interrupt safe API function if a
      context switch is required. */
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;

   /* Send a notification directly to the task */
   vTaskNotifyGiveFromISR(
       pPlatform->frame_sync_notification_task_handle,
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

/**
 *  Sets up handling of frame sync events from core 1
 */
static void SetupFrameSyncHandling( Platform_Config_t *pPlatform )
{
    /* Setup core 2 TRU event interrupt handler for core 1 frame sync event */
    if ( adi_sec_EnableEdgeSense( INTR_TRU0_SLV8, true ) != ADI_SEC_SUCCESS )
    {
       platform_printf(SPC "Error: SetupFrameSyncHandling adi_sec_EnableEdgeSense failed\n");
       return;
    }

    if( adi_int_InstallHandler( INTR_TRU0_SLV8, FrameSyncInterruptHandler, (void *)pPlatform, true ) != ADI_INT_SUCCESS )
    {
       platform_printf(SPC "Error: SetupFrameSyncHandling adi_int_InstallHandler failed\n");
       return;
    }

    adi_sec_SetPriority(INTR_TRU0_SLV8,     TRU0_SLV8_SEC_PRIORITY);

    /* Configure TRU to route TRU master software driven event to TRU slave */
    *pREG_TRU0_GCTL = 0x00000001;            /* Enable the TRU */
    *pREG_TRU0_SSR165 = TRGM_SYS_SOFT0_MST;  /* Slave TRU0 Intr Request 8 of Core 2
                                              * triggered by master SW driven
                                              * interrupt 0 (SOFT0_MST) by core 1 */

    /* Memory Barrier */
    asm("SYNC;");
}
#endif /*SPC2*/

static void platform_audio_thread_handler( void *pvParameters )
{
   Platform_Thread_t *pThread = (Platform_Thread_t *)pvParameters;
   Platform_Config_t *pPlatform = (Platform_Config_t *)(pThread->hPlatform);

   if (pThread->thread_level == PLATFORM_BASETHREADLEVEL)
   {
      while (1)
      {
         /* Wait to be triggered */
         ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

         pThread->stepTaskActive = 1;
         pThread->stepTaskRuns++;

         /* Step model ping pong buffers for this frame */
         MODEL_FUNC( StepPingPongs )();

         /* Perform audio processing for this rate */
         pPlatform->audio_thread_handler(pPlatform->callback_param, pThread->thread_level);

#if defined(INTERCORE_XFER_MDMA)
         /* Start the MDMA cross core audio data transfer */
         MDMA_launchTx();

         /* Prepare the MDMA RX for next frame's transmission */
         MDMA_blockAndLaunchRx();
#endif /*defined(INTERCORE_XFER_MDMA)*/

         pThread->stepTaskActive = 0;
      }
   }
   else
   {
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


#if defined(SPC2)
extern volatile uint32_t sharcCore2Initialized;
#endif
#if defined(SPC1)
#pragma section("dm_shared_l2")
volatile uint32_t sharcCore2Initialized = false;
#endif

static void platform_driver_thread_handler( void *pvParameters )
{
   int i;
   Platform_Config_t *pPlatform = (Platform_Config_t *)pvParameters;

#if defined(SPC1)
   oj_audio_init();
#endif

#if defined(SPC1)
   TriggerFrameSyncInterrupt();
   /* Wait for SHARC core 2 initialization to complete before
     * enabling the SPORT handling of audio input frames
     */
   while( sharcCore2Initialized != true ) { }
#endif

#if defined(SPC2)
   pPlatform->frame_sync_notification_task_handle = pPlatform->driver_task_handle;
   ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
   pPlatform->frame_sync_notification_task_handle = pPlatform->thread[PLATFORM_BASETHREADLEVEL].task_handle;
   sharcCore2Initialized = true;
#endif

#if defined(SPC1)
   oj_audio_start();
#endif

   MODEL_FUNC( InitializePingPongControl )();

   /* Initialize Talaria cross core active ping pong model access pointers
   * and MDMA audio routing functionality
   */
#if defined(INTERCORE_XFER_MDMA)
   if ( MDMA_Init( ) != MDMA_SUCCESS )
   {
      platform_printf(SPC "Error: Failed to initialize MDMA intercore xfer\n");
      exit(EXIT_FAILURE);
   }
#endif /*defined(INTERCORE_XFER_MDMA)*/

   while (1)
   {
      pPlatform->notification_poll(pPlatform->callback_param);

#if defined(SPC1)
      platform_poll_control_service_message(pPlatform);
#endif

      for (i=0; i<PLATFORM_NUM_BACKGROUND_TASKS; i++)
      {
         if (pPlatform->background_tasks[i] != (Platform_Func_background_task)NULL)
         {
            pPlatform->background_tasks[i](pPlatform->callback_param);
         }
      }
   }
}

#if !defined(PLATFORM_DRIVER_STACK_SIZE)
#define PLATFORM_DRIVER_STACK_SIZE       (800)
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

void oj_platform_sec_priorities(void)
{
   adi_sec_SetPriority(INTR_SPORT4_A_DMA,  SPORT4A_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_A_STAT, SPORT4A_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_B_DMA,  SPORT4B_SEC_PRIORITY);
   adi_sec_SetPriority(INTR_SPORT4_B_STAT, SPORT4B_SEC_PRIORITY);
}


Platform_Handle_t platform_get_handle(void)
{
   return (Platform_Handle_t)&g_platfromConfig;
}

void platform_init(void)
{
   oj_ezkit_init();

#if defined(SPC2)
    SetupFrameSyncHandling((Platform_Config_t *)&g_platfromConfig);
#endif

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

       /* Make EMDMA to generate secure transactions: in gxp_ezkit.c/SPU_Init for HIL/RTA */

       /* Initialize EMDMA stream assigned to the SHARC+ core currently running
       *    SHARC+ Core 1 = EMDMA Stream 0
       *    SHARC+ Core 2 = EMDMA Stream 1
       */
       gEmdmaResult_t gEmdmaResult = gEmdmaOpen( gEmdmaGetCoreStreamAssignment() );
       if ( gEmdmaResult != GEMDMA_SUCCESS )
       {
          platform_printf(SPC "Error: Failed to open EMDMA\n");
          return;
       }

#if defined(SPC1)
       /* Disable all FIR and IIR accelerator clocks. They will be enabled only
       * if the accelerator device is actually used. */
       gxpHamResult_t gFirResult = gxpHamFirDisableAllClocks();
       if ( gFirResult != GXPHAM_SUCCESS )
       {
          platform_printf(SPC "Error: Failed to disable FIR accelerator clocks\n");
          return;
       }

       gxpHamResult_t gIirResult = gxpHamIirDisableAllClocks();
       if( gIirResult != GXPHAM_SUCCESS )
       {
          platform_printf("Error: Failed to disable IIR accelerator clocks\n");
          return;
       }
#endif /*defined(SPC1)*/
    }
#endif /*defined(ENABLE_GHAM_MODULE)*/

   oj_platform_sec_priorities();

   platform_clear_background_tasks((Platform_Handle_t)&g_platfromConfig);
}

void platform_main(Platform_Handle_t hPlatform, int argc, char *argv[])
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;

   if (pPlatform == NULL)
   {
      platform_printf(SPC "Error: NULL Platform handle\n");
      return;
   }
   pPlatform->callback_param = pPlatform->framework_init();

   platform_audio_threads_init(hPlatform);

   platform_driver_thread_init(hPlatform);

   /* Start the scheduler to start the tasks executing. */
   vTaskStartScheduler();
}

void platform_close(Platform_Handle_t hPlatform)
{
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

