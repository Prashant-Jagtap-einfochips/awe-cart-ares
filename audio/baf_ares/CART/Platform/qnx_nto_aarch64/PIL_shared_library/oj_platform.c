#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "Module_Wrapper.h"
#include "oj_platform.h"
#include "Message_Id.h"
#include "CART_ID.h"
#include "CART_Message.h"
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

int test_so_main(Platform_Config_t* pPlatform);

module_funcs_t * module_dlsyms = NULL;

// Find max(request, response) payload for application in bytes. Must be a
// multiple of 4 to size rspTalaria correctly.
#define MAX_MSG_BUF_SIZE 512

#define MSG_HEADER_SIZE CART_MESSAGE_HEADER_BYTES

// Application's response buffer.

uint32_t rspTalaria[(MAX_MSG_BUF_SIZE - MSG_HEADER_SIZE) >> 2];

uint32_t resp_id;

uint32_t resp_size;

// Used for notification by platform_notification_poll().
uint8_t reqBuf[MAX_MSG_BUF_SIZE];

// Used for other commands by platform_control_service_message().
uint8_t cmdPayloadBuffer[MAX_MSG_BUF_SIZE];

int32_t platform_control_service_message(Platform_control_msg *p_request_message,
                                  Platform_control_msg *p_response_message,
                                  void *arg) 
{
    int32_t status = -1;

    if (NULL != module_dlsyms)
    {
      // For this test application, the request buffer MUST be large
      // enough to fit response.
      const uint32_t offset = CART_MESSAGE_HEADER_BYTES;

      const uint32_t msgid = p_request_message->id;

      const uint32_t payloadSize = p_request_message->size;

      CART_Phy_Message *phyMsg = (CART_Phy_Message *)cmdPayloadBuffer;

      // Here we should only tunnel the message. Not overrite message type etc. The caller of this 
      // method should have set. 
      CART_Message_copy_header(phyMsg, (CART_Message *)p_request_message);

      if (payloadSize + offset > MAX_MSG_BUF_SIZE)
      {
         printf("Error: request payload size %u too big \n", payloadSize);
         return -1;
      }

      memcpy((void*)(phyMsg->data), p_request_message->data, payloadSize);

      status = module_dlsyms->pipe_data(arg, MAX_MSG_BUF_SIZE, cmdPayloadBuffer);

      CART_Phy_Message *phyRsp = (CART_Phy_Message *)(&cmdPayloadBuffer[payloadSize + offset]);

      p_response_message->id = CART_Message_get_id(phyRsp);
      p_response_message->size = CART_Message_get_size(phyRsp);

      if (p_response_message->size > (MAX_MSG_BUF_SIZE - offset))
      {
          printf("Error: invalid response payload size %u. \n", p_response_message->size);
          status = -1;
      }
      else
      {
          memcpy(p_response_message->data , (void*)(phyRsp->data),  p_response_message->size);
      }

#ifdef VERBOSE_LOGGING
    if (p_response_message->size != p_request_message->id)
    {
        printf("platform_control_service_message: req and resp msg id are not same! \n");
    }
#endif  // VERBOSE_LOGGING

    }
    else
    {
        printf("platform_control_service_message: library NULL. \n");
        status = -1;
    }

    return status;
}

int32_t platform_notification_poll(void *arg)
{
    /* Poll for notifications */
    int32_t status;

    if (NULL != module_dlsyms)
    {
        // No payload for (CART_)MESSAGE_NOTIFICATION_POLL type msg.
        uint32_t payloadSize = 0;
        CART_Phy_Message phyMsg[2] = {0};
        
        CART_Message_set_id(&phyMsg[0], MESSAGE_NOTIFICATION_POLL);
        CART_Message_set_src(&phyMsg[0], CART_ID_PLATFORM);
        CART_Message_set_dst(&phyMsg[0], CART_ID_BAF_ALL);

        // CART_MESSAGE_TYPE_NORSP is a response is not required.
        CART_Message_set_type(&phyMsg[0], CART_MESSAGE_TYPE_REQ);
        CART_Message_set_handle(&phyMsg[0], 0);
        CART_Message_set_size(&phyMsg[0], payloadSize);

        status = module_dlsyms->pipe_data(arg, CART_MESSAGE_HEADER_BYTES, (void*)&phyMsg[0]);

        const uint32_t offset = CART_MESSAGE_HEADER_BYTES;

        // Response comes back in request buffer.
        CART_Phy_Message *phyRsp = (CART_Phy_Message *)(&phyMsg[1]);

        resp_id = CART_Message_get_id(phyRsp);
        resp_size = CART_Message_get_size(phyRsp);

        if (resp_size >  offset)
        {
            printf("Error: invalid response payload size %u. \n", resp_size);
            status = -1;
        }
        else
        {
            memcpy((void*)rspTalaria, (void*)(phyRsp->data),  resp_size);
        }
    }
    else
    {
        printf("platform_notification_poll: library NULL. \n");
        status = -1;
    }

    return status;
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

   uint32_t cnt = RAMIO_fwrite(buff, sizeof(float), num_chan*frame_size, fpOut);

   if (cnt != num_chan*frame_size)
      platform_printf("Error: platform_xmt_output_buffer got %d exp % d\n", cnt, num_chan*frame_size);

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

            status = platform_control_service_message(&req_msg, &rsp_msg, pPlatform->callback_param);

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
   pPlatform->control_file = NULL;
   pPlatform->response_file = stdout;
   pPlatform->audio_in_file = NULL;
   pPlatform->audio_out_file = NULL;
   pPlatform->audio_gold_file = NULL;

   pPlatform->control_file_name = "request.ojx";
   pPlatform->audio_in_file_name = "audioIn.bin";
   pPlatform->audio_out_file_name = "audioOut.bin";
   pPlatform->audio_gold_file_name = "audioRef.bin";

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

   // Clear all functions pointers.
   g_platfromConfig.framework_init = NULL;
}

Platform_Handle_t platform_get_handle(void)
{
   return (Platform_Handle_t)&g_platfromConfig;
}

void platform_main(Platform_Handle_t hPlatform, int argc, char *argv[])
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;

   printf("oj_platform: platform_main >>> ");

   if (pPlatform == NULL)
   {
      platform_printf("Error: NULL Platform handle\n");
      return;
   }

   if (set_run_parameters(pPlatform, argc, argv) != 0)
      return;

   pPlatform->audio_frame_cnt = 0;
   pPlatform->audio_frame_max = FRAME_IDX_MAX;

   set_next_control_frame(pPlatform);


   if (0 != test_so_main(pPlatform))
   {
      printf("Shared library error, exiting. \n");
      return;
   }

   platform_poll_control_service_message(pPlatform);


   printf("\n oj_platform: platform_main <<< \n");
}

size_t BAF_logger_size(void);
void *BAF_logger_addr(void);

size_t OJSP_heap_log_buffer_size(void);
void *OJSP_heap_log_buffer_addr(void);

extern unsigned char data_out_binary[];
extern unsigned int data_out_length;

void platform_fdump(char *file_name, void *buff, size_t len)
{
   FILE *fp = fopen(file_name, "wb");
   if (fp != NULL)
   {
      int cnt;
      int sz = 256;
      int tot = len;

      while (tot > 0)
      {
         cnt = (tot > sz) ? sz : tot;
         fwrite(buff, 1, cnt, fp);
         tot = tot - cnt;
         buff = buff + cnt;
      }
      fclose(fp);
   }
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

      {
         platform_fdump("baf_logger.bin", BAF_logger_addr(), BAF_logger_size());
         platform_fdump("heap_log_buffer.bin", OJSP_heap_log_buffer_addr(), OJSP_heap_log_buffer_size());
         platform_fdump("audioOut.bin", data_out_binary, data_out_length);
      }
   }
}

int test_so_main(Platform_Config_t *pPlatform)
{
   void* dlhandle = NULL;

   dlhandle = dlopen( "lib_bose.so", RTLD_NOW );

   if (NULL == dlhandle)
   {
       platform_printf("\n dlopen failed. %s! \n", dlerror());
      return -1;
   }

   module_dlsyms = dlsym( dlhandle, "module_funcs" );

   if (NULL == module_dlsyms)
   {
      platform_printf("\n module_dlsyms NULL. %s! \n", dlerror());
      return -1;
   }
#ifdef VERBOSE_LOGGING
   else
   {
      platform_printf("\n dlopen - dlsym: Got module_dlsyms. \n");
   }
#endif

   // Handle to underlying library instance 
   void* hLib; 

   module_dlsyms->create( &hLib );

   // For shared library case, callback is set to handle returned by library. 
   if (NULL == pPlatform->callback_param)
   {
      pPlatform->callback_param = hLib;
   }

   module_dlsyms->initialize( hLib, NULL);

   platform_printf("\n Shared Ojbect initialize() done. Begin audio processing. \n"); 

   // These value should/could be queried from DLL if getters need to be tested:
   // num_chan, frame_size   
   unsigned int num_in_chan = 0;

   unsigned int num_out_chan = 0;

   unsigned int frame_size = 0;

   // Check heap allocation and usage. MALLOC_TRY is crashing. Use stack buffer.
#ifdef MALLOC_TRY
   float *file_audio_in = NULL;

   float *file_audio_out = NULL;
#else
   // 
   static float file_audio_in[256];  // > num_chan * frame_size.

   static float file_audio_out[256];

   memset(file_audio_out, 0, sizeof(float)*256);
#endif

   if (ERR_OK == module_dlsyms->get_channel_counts(hLib, &num_in_chan, &num_out_chan))
   {
      platform_printf("\n Get_data channel cnt: input %u output %u.\n", num_in_chan, num_out_chan);


      if (ERR_OK == module_dlsyms->get_samples_per_frame(hLib, &frame_size))
      {
         platform_printf("\n  get_data: frame size: %u\n", frame_size); 

#ifdef MALLOC_TRY
         file_audio_in = malloc(sizeof(float) * num_chan * frame_size);

         file_audio_out = malloc(sizeof(float) * num_chan * frame_size);

         if ((NULL == file_audio_in) || (NULL == file_audio_out))
         {
            platform_printf("\n ERROR: file_audio_in. \n"); 
            return -1;
         }
#endif
      }
      else
      {
         platform_printf("\n ERROR: get_data. \n"); 
         return -1;
      }
   }
   else
   {
      platform_printf("\n ERROR: get_data. \n"); 
      return -1;
   }

   RAMIO_FILE *fp = g_platfromConfig.audio_in_file;
   RAMIO_FILE *fpOut = NULL;

   if (NULL == fp)
   {
      platform_printf("\n ERROR: RAMIO_FILE NULL. \n"); 
      return -1;
   }
   else
   {
      fpOut = g_platfromConfig.audio_out_file;

      if (NULL == fpOut)
      {
         platform_printf("\n ERROR: OUT RAMIO_FILE NULL. \n"); 
         return -1;
      }
   }

   while (pPlatform->audio_frame_cnt < pPlatform->audio_frame_max)
   {
      platform_poll_control_service_message(pPlatform);

      uint32_t cnt = RAMIO_fread(file_audio_in, sizeof(float), num_in_chan*frame_size, fp);

      if (cnt != num_in_chan*frame_size)
      {
         platform_printf("Error: RAMIO_fread got %d exp % d\n", cnt, num_in_chan*frame_size);
         break;
      }

      // DLL will call BAF_exec in Process
      module_dlsyms->process(hLib, &file_audio_in[0],  &file_audio_out[0]);

      pPlatform->audio_frame_cnt++; 

      platform_notification_poll(pPlatform->callback_param);

      // Account for different input and output number of channels.
      cnt = RAMIO_fwrite(file_audio_out, sizeof(float), num_out_chan*frame_size, fpOut);

      if (cnt != num_out_chan*frame_size)
      {
         platform_printf("Error: platform_xmt_output_buffer got %d exp % d\n", cnt, num_out_chan*frame_size);
         break;
      }
   }
   
    module_dlsyms->deinitialize( hLib);

    module_dlsyms->destroy( &hLib );            

    // DLL handling
    dlclose( dlhandle );
   
#ifdef MALLOC_TRY
    if (file_audio_in)
      free(file_audio_in);

    if (file_audio_out)
      free(file_audio_in);
#endif

    printf("Processing complete. \n");

    return 0;
}

